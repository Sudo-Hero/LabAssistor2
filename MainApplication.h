#pragma once
#ifndef _MAIN_
#define _MAIN_
#include <windows.h>





class MainApplication {
private:
	HINSTANCE hInstance;
	HWND hWnd;
	WNDCLASS wc = { 0 };
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
public:
	static bool loggedIn;
	explicit MainApplication(HINSTANCE hinstance, int CmdShow);
	MainApplication(const MainApplication&) = delete;
	void Run();

	HWND getHWND()const;

	~MainApplication() {
		UnregisterClass(wc.lpszClassName, hInstance);
	}
};


#endif // !1