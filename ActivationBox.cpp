#include "ActivationBox.h"
#include "utils.h"
#include "Font.h"
#include "Activation.h"
#include <string>

#define BLUISH_WHITE RGB(244, 245, 255)

ActivationBoxAct buttons;


LRESULT CALLBACK ActivationBox::ActivationBoxProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {

    case WM_COMMAND:
        if (HIWORD(wParam) == EN_SETFOCUS) {
            wchar_t ch[2];
            if (!GetWindowText(buttons.getProdctKeyHandle(), ch, 1)) {
                SetWindowText(buttons.getProdctKeyHandle(), L"");
            }
        }
        break;
        
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED) {
            buttons.removeRect();

        }
        if (wParam == SIZE_RESTORED) {
            buttons.setRect();

        }
        break;
    case WM_MOUSEMOVE:
    {
        // Track mouse hover
        POINT cursorPos;
        GetCursorPos(&cursorPos);
        ScreenToClient(hwndDlg, &cursorPos);
        return buttons.checkForHover(cursorPos.x, cursorPos.y);
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
        int xPos = GET_X_LPARAM(lParam);
        int yPos = GET_Y_LPARAM(lParam);
        buttons.checkForClick(xPos, yPos);
        /*if (PtInRect(&ok._region, {xPos, yPos})) {
            // Mouse click is inside the button
            ok.isButtonPressed = true;
            MessageBoxA(NULL, "fsdf ", "dfdfs", MB_OK);
            // Change the button's appearance to indicate the press state
            // Handle the button click action here
        }*/
    }
    break;
    case WM_CTLCOLORSTATIC:
        SetTextColor((HDC)wParam, RGB(255, 255, 255));
        SetBkColor((HDC)wParam, BLUISH_WHITE);
        return INT_PTR(CreateSolidBrush(BLUISH_WHITE));
    case WM_CTLCOLORBTN: {
        HDC hdcButton = (HDC)wParam;
        HBRUSH g_hBrushTextColor = CreateSolidBrush(RGB(255, 0, 0)); // Red color brush
        SetTextColor(hdcButton, RGB(0, 0, 255)); // Set the text color to blue
        SetBkMode(hdcButton, TRANSPARENT); // Make the background transparent
        return (LRESULT)g_hBrushTextColor;
    }

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwndDlg, &ps);
        buttons.DrawButtons(hdc);


        EndPaint(hwndDlg, &ps);
        break;
    }

    default:
        return DefWindowProc(hwndDlg, uMsg, wParam, lParam);
    }

}

void ActivationBox::RegisterActClass() {
    wc.lpfnWndProc = ActivationBoxProc;
    wc.lpszClassName = L"Activation Window";
    wc.hbrBackground = CreateSolidBrush(BLUISH_WHITE);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClass(&wc);
}


void ActivationBox::CreateAcivationWindow(HWND& hParentwnd) {
    hMain = hParentwnd;
    hwndDlg = CreateWindow(wc.lpszClassName, L"", WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 900, 500, hMain, NULL, NULL, NULL);
    buttons.DrawInputs(hwndDlg);

}

void ActivationBoxAct::DrawButtons(HDC & hdc) {
    BlueExLabel header(20, 20, L"Activate License", 30, FontStyleBold);
    BlueLabel details1(20, 20 + 30 + 50, L"Your product key should be displayed in the LabAssistor account you can login into your account ", 16, FontStyleBold);
    BlueLabel details2(20, 20 + 30 + 50 + 20, L"and paste the key here.", 16, FontStyleBold);
    BlueLabel details3(20, 20 + 30 + 50 + 20 + 50, L"The produt key looks similar to this:", 16, FontStyleBold);
    BlueLabel details4(20, 20 + 30 + 50 + 20 + 50 + 20, L"PRODUCT KEY: XXXX-XXXX-XXXX-XXXX", 16, FontStyleBold);
    BlueLabel label(20, 20 + 30 + 50 + 20 + 50 + 20 + 70, L"Product Key", 16, FontStyleBold);

    header.Draw(hdc);
    details1.Draw(hdc);
    details2.Draw(hdc);
    details3.Draw(hdc);
    details4.Draw(hdc);
    label.Draw(hdc);

    BlueButton SubmitKey(0, L"Submit Key", 20, 20 + 30 + 50 + 20 + 50 + 20 + 70 + 30 + 70, FontStyleBold);
    SubmitKey.DrawBtn(hdc);
    BlueButton Finish(1, L" Finish", 20 + 300, 20 + 30 + 50 + 20 + 50 + 20 + 70 + 30 + 70, FontStyleBold);
    Finish.DrawBtn(hdc);

    rect[0] = RECT(SubmitKey.getRect());
    rect[1] = RECT(Finish.getRect());
}

void ActivationBoxAct::DrawInputs(HWND & hParentDlg) {
    InputBox hProductKeyInp((HMENU)0, L"Enter Product Key", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_UPPERCASE, 20, 20 + 30 + 50 + 20 + 50 + 20 + 70 + 30, 30, 500, hParentDlg);
    hProductKeyInp.Draw();
    hProductKey = hProductKeyInp.getHWND();
    SetFont(hProductKeyInp.getHWND(), 22, Style::Regular);
    SendMessage(hProductKeyInp.getHWND(), EM_SETLIMITTEXT, (WPARAM)19, NULL);
    
}



void ActivationBoxAct::checkForClick(int xPos, int yPos) {
    //Click on Submit
    if (PtInRect(&rect[0], { xPos, yPos })) {
        char productKey[20];
        GetWindowTextA(buttons.getProdctKeyHandle(), productKey, 20);
        int len = std::strlen(productKey);
        if (len != 19) {
            MessageBoxA(NULL, "Please enter valid key", "Submit", MB_OK | MB_ICONINFORMATION);
            MessageBoxA(NULL, productKey, "Submit", MB_OK | MB_ICONINFORMATION);
            MessageBoxA(NULL, std::to_string(std::strlen(productKey)).c_str(), "asd", MB_OK | MB_ICONINFORMATION);
        }
        else {
            MessageBoxA(NULL, "Checking key please wait", "Submit", MB_OK | MB_ICONINFORMATION);
            if (!submitKey(productKey)) {
                MessageBoxA(NULL, "Please enter valid key", "Submit", MB_OK | MB_ICONINFORMATION);
            }
        }
        
    }
    //Click on Finish
    if (PtInRect(&rect[1], { xPos, yPos })) {
        PostQuitMessage(0);
    }
}


bool ActivationBoxAct::checkForHover(int xPos, int yPos) {
    
    for (int i = 0; i <= 3; i++) {
        if (PtInRect(&rect[i], { xPos, yPos })) {
            // Change the cursor to the hand cursor.
            SetCursor(LoadCursor(nullptr, IDC_HAND));
            return TRUE; // Stop further processing of the message.
        }
    }

    SetCursor(LoadCursor(nullptr, IDC_ARROW));
    return TRUE;
}

void ActivationBoxAct::setRect() {
    rect = new RECT[2];
}

void ActivationBoxAct::removeRect() {
    delete[] rect;
}

HWND ActivationBoxAct::getProdctKeyHandle() const {
    return hProductKey;
}
