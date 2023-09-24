#pragma once
#ifndef _ACTIVATIONBOX_
#define _ACTIVATIONBOX_


#include <windows.h>

class ActivationBox {
private:
    HWND hwndDlg, hMain;
    WNDCLASS wc{};

    static LRESULT CALLBACK ActivationBoxProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
public:
    ActivationBox() {

    }

    ~ActivationBox() {
        UnregisterClass(wc.lpszClassName, NULL);
        DestroyWindow(hwndDlg);
        EnableWindow(hMain, true);
    }


    void RegisterActClass();
    void CreateAcivationWindow(HWND& hParentwnd);
    
};

class ActivationBoxAct {
private:
    HWND hMainDlg, hProductKey;
    RECT* rect;
public:
    
    ActivationBoxAct() {}
    void checkForClick(int xPos, int yPos);
    bool checkForHover(int xPos, int yPos);
    void removeRect();
    void setRect();
    void DrawInputs(HWND &hParentDlg);
    void DrawButtons(HDC &hdc);

    HWND getProdctKeyHandle() const;
};

#endif // !_ACTIVATIONBOX_