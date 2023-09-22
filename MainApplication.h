#pragma once
#ifndef _MAIN_
#define _MAIN_
#include <windows.h>





class MainApplication {
private:
	HINSTANCE hInstance;
	HWND hWnd;
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public:
	explicit MainApplication(HINSTANCE hinstance, int CmdShow);
	MainApplication(const MainApplication&) = delete;
	void Run();
};

#endif // !1