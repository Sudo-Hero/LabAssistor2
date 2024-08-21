// ClientClass.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <thread>
#include <iostream>
#include <string>
#include "Client.h"
#include <commctrl.h>
#include <thread>
#include <codecvt>

#pragma comment (lib, "comctl32.lib")
#pragma comment(linker, "\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define HOSTNAME 0
#define WM_MSG_RECIVED (WM_USER + 1 )
HWND hwnd;
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
Client client;


std::string getData() {
    std::string data = "This is my data";
    return data;
}

int main()
{
    HINSTANCE hInstance = GetModuleHandle(NULL);
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_PROGRESS_CLASS; // Enable the progress bar control
    InitCommonControlsEx(&icex);

    const wchar_t CLASS_NAME[] = L"Client Application";

    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window.

    hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"Client Application",    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 350,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, SW_SHOW);
    client.Init();
    client.SetSystemInfo();
    NONCLIENTMETRICS ncm;
    ncm.cbSize = sizeof(NONCLIENTMETRICS);
    SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0);

    // Increase the font size
    ncm.lfMessageFont.lfHeight += 45; // Change this factor as needed
    // Create a wstring_convert object
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

    // Convert the narrow string to a wide string
    std::wstring hostname =L"Hostname:  "+  converter.from_bytes(client._hostname);
    // Create a static control
    HWND hostnamelbl = CreateWindow(
        L"STATIC",           // predefined class name
        hostname.c_str(),    // text
        WS_VISIBLE | WS_CHILD,  // styles
        10,                      // starting x position
        10,                      // starting y position
        900,                     // width
        50,                      // height
        hwnd,                   // parent window
        NULL,                   // No menu
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
        NULL                    // pointer not needed
    );
    SendMessage(hostnamelbl, WM_SETFONT, (WPARAM)CreateFontIndirect(&ncm.lfMessageFont), TRUE);
    std::wstring ip = L"IP: " + converter.from_bytes(client._ipaddress);
    HWND iplbl = CreateWindow(
        L"STATIC",           // predefined class name
        ip.c_str(),    // text
        WS_VISIBLE | WS_CHILD,  // styles
        10,                      // starting x position
        10+50,                      // starting y position
        900,                     // width
        50,                      // height
        hwnd,                   // parent window
        NULL,                   // No menu
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
        NULL                    // pointer not needed
    );
    SendMessage(iplbl, WM_SETFONT, (WPARAM)CreateFontIndirect(&ncm.lfMessageFont), TRUE);
    HWND ping = CreateWindow(
        L"Button",           // predefined class name
        L"PING",    // text
        WS_VISIBLE | WS_CHILD,  // styles
        10 + 140,                      // starting x position
        10 + 50 + 140,                      // starting y position
        90,                     // width
        40,                      // height
        hwnd,                   // parent window
        NULL,                   // No menu
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
        NULL                    // pointer not needed
    );
    SendMessage(ping, WM_SETFONT, (WPARAM)CreateFontIndirect(&ncm.lfMessageFont), TRUE);
    client.SetListener();
    std::thread (&Client::AcceptConnection, &client).detach();
    std::thread(&Client::RecvCommand, &client).detach();
    // Run the message loop.

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;

}



LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_MSG_RECIVED:
        MessageBoxA(NULL, "Message Received", NULL, MB_OK);
        break;
    case WM_CREATE:
    {

        break;
    }
    case WM_CTLCOLORSTATIC:
    {
        // Set the background brush to null, making it transparent
        HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
        SetBkMode((HDC)wParam, TRANSPARENT);
        return (INT_PTR)hBrush;
    }
    case WM_COMMAND:
    {

        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // All painting occurs here, between BeginPaint and EndPaint.

        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

        EndPaint(hwnd, &ps);
    }
    return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}