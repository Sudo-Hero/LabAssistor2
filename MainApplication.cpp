#include "MainApplication.h"
#ifndef _UTILS
#include "utils.h"
#endif
#ifndef _SIGNIN
#include "SignIn.h"
#endif
#include <iostream>
#include <string>

SignIn signIn;

bool MainApplication::loggedIn = false;

MainApplication::MainApplication(HINSTANCE hinstance, int CmdShow) :hInstance{ hinstance } {
	WNDCLASS wc = { 0 };
	MainApplication::loggedIn = false;
	wc.hInstance = hInstance;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpfnWndProc = MainApplication::WindowProc;
	wc.lpszClassName = L"Main Window";
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);

	RegisterClass(&wc);
	hWnd = CreateWindow(wc.lpszClassName, L"Lab Assistor", WS_CAPTION | WS_VISIBLE | WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 1140, 682, NULL, NULL, hInstance, NULL);
	ShowWindow(hWnd, CmdShow);

	//InputBox zusername((HMENU)1, L"Username", WS_CHILD | WS_VISIBLE | WS_BORDER, 50, 50, 100, 100, hWnd);
	//zusername.Draw();
	signIn.DrawInputs(hWnd);
	signIn.ActivationCheck();
}

void MainApplication::Run() {
	MSG msg = {};
	while(GetMessage(&msg,NULL, NULL,NULL)){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

LRESULT CALLBACK MainApplication::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	HDC hdc;
	
	
	PAINTSTRUCT ps;
	switch (msg) {
	case WM_COMMAND:
		if (HIWORD(wparam) == EN_SETFOCUS && LOWORD(wparam) == 1) {
			wchar_t ch[2];
			if (!GetWindowText(signIn.getEmailHWND(), ch, 1)) {
				SetWindowText(signIn.getEmailHWND(), L"");
			}
		}
		if (HIWORD(wparam) == EN_SETFOCUS && LOWORD(wparam) == 2) {
			wchar_t ch[2];
			if (!GetWindowText(signIn.getPassHWND(), ch, 1)) {
				SetWindowText(signIn.getPassHWND(), L"");
			}
		}
		break;

	case WM_CREATE:
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		//if(signIn.isLo)
		if (!signIn.getStatus()) {
			MainApplication::loggedIn = true;
			signIn.Draw(hdc);
		}
		
		
		EndPaint(hwnd, &ps);
		break;
	case WM_SIZE:
		if (wparam == SIZE_MINIMIZED) {
			signIn.removeRect();
			
		}
		if (wparam == SIZE_RESTORED) {
			signIn.setRect();

		}
		break;
	case WM_MOUSEMOVE:
	{
		// Track mouse hover
		POINT cursorPos;
		GetCursorPos(&cursorPos);
		ScreenToClient(hwnd, &cursorPos);
		return signIn.checkForHover(cursorPos.x, cursorPos.y);
		/*if (PtInRect(&ok._region, {xPos, yPos})) {
			// Mouse is over the button
			// Change the button's appearance or handle hover state as needed

		}
		else {
			// Mouse is not over the button
			// Reset the button's appearance or hover state as needed
		}*/
	}
	return 0;

	case WM_LBUTTONDOWN:
	{
		// Check if the mouse click is inside the button's rectangular bounds
		int xPos = GET_X_LPARAM(lparam);
		int yPos = GET_Y_LPARAM(lparam);
		signIn.checkForClick(xPos, yPos);
		/*if (PtInRect(&ok._region, {xPos, yPos})) {
			// Mouse click is inside the button
			ok.isButtonPressed = true;
			MessageBoxA(NULL, "fsdf ", "dfdfs", MB_OK);
			// Change the button's appearance to indicate the press state
			// Handle the button click action here
		}*/
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
	return 0;
}

HWND MainApplication::getHWND() const{
	return hWnd;
}