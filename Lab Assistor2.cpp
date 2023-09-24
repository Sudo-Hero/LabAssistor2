#include <objidl.h>
#include <gdiplus.h>
#include <iostream>
#include "Logger.h"

#pragma comment (lib,"Gdiplus.lib")

using namespace Gdiplus;
#include "MainApplication.h"

int WINAPI wWinMain(HINSTANCE hinstance, HINSTANCE hPrevinstance, PWSTR CmdLine, int CmdShow) {
    // Create a unique name for the mutex (e.g., based on your application name)
    FileLogger logger("log.txt");
    const wchar_t* mutexName = L"LabAssistor";
    // Attempt to create a mutex
    HANDLE hMutex = CreateMutex(NULL, TRUE, mutexName);

    // Check if the mutex already exists (another instance is running)
    if (GetLastError() == ERROR_ALREADY_EXISTS || hMutex == NULL) {
        std::cout << "" << std::endl;
        logger.log("Another instance is already running. Exiting.");
        MessageBoxA(NULL, "Application already running", "Error", MB_OK | MB_ICONERROR);
        // Release the mutex handle
        if (hMutex != NULL) {
            CloseHandle(hMutex);
        }
        return 1;
    }

	
	logger.log("//******************************************************************************//");
    logger.log("This is the first instance of the application.");
	ULONG token;
	GdiplusStartupInput GdiInput;
	GdiplusStartup(&token, &GdiInput, NULL);
	MainApplication app(hinstance, CmdShow);
	app.Run();
    CloseHandle(hMutex);
	GdiplusShutdown(token);
	return 0;
}