#include <objidl.h>
#include <gdiplus.h>
#include <iostream>
#include "Logger.h"

#pragma comment (lib,"Gdiplus.lib")

using namespace Gdiplus;
#include "MainApplication.h"

int WINAPI wWinMain(HINSTANCE hinstance, HINSTANCE hPrevinstance, PWSTR CmdLine, int CmdShow) {
	ULONG token;
	GdiplusStartupInput GdiInput;
	GdiplusStartup(&token, &GdiInput, NULL);
	MainApplication app(hinstance, CmdShow);
	app.Run();
	GdiplusShutdown(token);
	return 0;
}