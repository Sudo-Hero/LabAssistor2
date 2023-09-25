#include "Panel.h"
#include "utils.h"
#include <iostream>
#include <string>


Panel::Panel(HINSTANCE hinstance, int CmdShow) :hInstance{ hinstance } {
	WNDCLASS wc = { 0 };

	wc.hInstance = hInstance;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpfnWndProc = Panel::PanelProc;
	wc.lpszClassName = L"Panel Window";
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);

	RegisterClass(&wc);
	hWnd = CreateWindow(wc.lpszClassName, L"Lab Assistor", WS_CAPTION | WS_VISIBLE | WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 1140, 682, NULL, NULL, hInstance, NULL);
	ShowWindow(hWnd, CmdShow);

	//InputBox zusername((HMENU)1, L"Username", WS_CHILD | WS_VISIBLE | WS_BORDER, 50, 50, 100, 100, hWnd);
	//zusername.Draw();
}

void Panel::Run() {
	MSG msg = {};
	while (GetMessage(&msg, NULL, NULL, NULL)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

LRESULT CALLBACK Panel::PanelProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	HDC hdc;


	PAINTSTRUCT ps;
	switch (msg) {
	case WM_COMMAND:
		
		break;

	case WM_CREATE:
		break;

	case WM_PAINT:
		break;

	case WM_SIZE:
		break;

	case WM_MOUSEMOVE:{
	}
	return 0;

	case WM_LBUTTONDOWN:{
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