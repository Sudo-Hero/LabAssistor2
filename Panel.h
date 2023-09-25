#pragma once
#ifndef _PANEL_
#define _PANEL_
#include <windows.h>



class Panel {
private:
	HINSTANCE hInstance;
	HWND hWnd;
	static LRESULT CALLBACK PanelProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public:
	explicit Panel(HINSTANCE hinstance, int CmdShow);
	Panel(const Panel&) = delete;
	void Run();
};

#endif // !PANEl