#include "SignIn.h"
#include "common.h"


/*******************************************
1. Have to make images clickable
********************************************/

void SignIn::DrawBk(HDC& hdc) {
	Graphics graphics(hdc);
	Image bkGroundImg(L"Image\\SignIn.png");
	Image logoImg(L"Image\\logo.png");
	Image microsoftImg(L"Image\\microsoft.png");
	Image githubImg(L"Image\\github.png");
	Image securityImg(L"Image\\security.png");
	
	RECT window { };
	bool isWindow = GetWindowRect(hMain, &window);
	int width = window.right - window.left;
	int height = window.bottom - window.top;
	graphics.DrawImage(&bkGroundImg,0,0, width, height);
	graphics.DrawImage(&logoImg, 15, 90, 642, 150);
	graphics.DrawImage(&microsoftImg, 55, 453);
	graphics.DrawImage(&githubImg, 230, 439, 210, 81);
	graphics.DrawImage(&securityImg, 55,550, 20, 22);

	rect[0] = { 55, 453,(LONG)microsoftImg.GetWidth() + 55, (LONG)microsoftImg.GetHeight() + 453 };
	rect[1] = { 230, 453,(LONG)210 + 230, (LONG)81 + 439 };
	
}

void SignIn::DrawMessages(HDC&hdc) {
	Graphics graphics(hdc);
	WhiteLabel message1(55, 275, L"The first inovative lab manager with more options", 25, FontStyleBold);
	message1.Draw(hdc);
	
	DarkLabel message2(55, 333, L"You can automate many of the repetitive tasks that are common in labs.", 15, FontStyleBold);
	message2.Draw(hdc);
	DarkLabel message3(55, 353, L"It helps tocollect and analyze data from lab equipment.", 15, FontStyleBold);
	message3.Draw(hdc);
	DarkLabel message4(55, 373, L"Lab Assistant can help labs save time with barious operations.", 15, FontStyleBold);
	message4.Draw(hdc);

	WhiteLabel message5(80, 550, L"Keeps Your Data Safe", 12, FontStyleBold);
	message5.Draw(hdc);
}

void SignIn::DrawForum(HDC& hdc) {
	WhiteCard card1(750, 115, 350, 500);
	card1.Draw(hdc);
	
	DarkExLabel signIn(781, 135, L"Sign In", 25, FontStyleRegular);
	signIn.Draw(hdc);

	DarkExLabel _or(942 + 40, 135 + 9, L"or", 16, FontStyleRegular);
	_or.Draw(hdc);

	DarkBlueLabel signUp(965 + 40, 135 + 9, L"SignUp", 16, FontStyleRegular);
	signUp.Draw(hdc);

	Graphics graphics(hdc);
	Image emailImg(L"Image\\email.png");
	LightGreyLine line1(814 - 30, 255, 814 + 300 - 30, 255, 10);// Related with email textbox
	line1.Draw(hdc);
	graphics.DrawImage(&emailImg, 785, 230, 20, 15);

	Image lockImg(L"Image\\lock.png");
	LightGreyLine line2(814 - 30, 305 + 20, 814 + 300 - 30, 305 + 20, 10);// Related with password textbox
	line2.Draw(hdc);
	graphics.DrawImage(&lockImg, 783, 228 + 50 +20, 25, 20);

	BlueButton login(1, L"                      Login", 814 - 40, 305 + 70 + 20);
	login.DrawBtn(hdc);

	rect[2] = RECT(login.getRect());
	rect[3] = RECT(signUp.getRect());
	
	
}

void SignIn::DrawInputs(HWND& hwnd) {
	hMain = hwnd;
	std::wstring emailText = L"Enter Email";
	InputBox email((HMENU)1, emailText, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 781 + 30, 225, 28, 273, hMain);//Y - Sign In Label + 45
	email.Draw();
	hEmail = email.getHWND();
	if (AddFontResource(L"Font\\Montserrat-Regular.ttf") == 0) {
		// Handle the error if font loading fails
		// You can use GetLastError() to get the specific error code
	}
	hMontserrat = CreateFont(
		/* font parameters */
		22,                  // Height of the font
		0,                    // Width of the font (0 for default)
		0,                    // Escapement angle
		0,                    // Orientation angle
		FW_NORMAL,            // Font weight (e.g., FW_NORMAL for normal weight)
		FALSE,                // Italic
		FALSE,                // Underline
		FALSE,                // Strikeout
		DEFAULT_CHARSET,      // Character set
		OUT_DEFAULT_PRECIS,   // Output precision
		CLIP_DEFAULT_PRECIS,  // Clipping precision
		PROOF_QUALITY,      // Quality
		DEFAULT_PITCH,        // Pitch and family
		L"Montserrat Regular"     // Font name (use the actual font name)
	);
	
	SendMessage(email.getHWND(), WM_SETFONT, (WPARAM)hMontserrat, TRUE);
	std::wstring PassText = L"Enter Email";
	InputBox password((HMENU)2, emailText, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_PASSWORD, 781 + 30, 225 + 70, 28, 273, hMain);//Y - Sign In Label + 45
	password.Draw();
	hPassword = password.getHWND();
	SendMessage(password.getHWND(), WM_SETFONT, (WPARAM)hMontserrat, TRUE);

}


void SignIn::Draw(HDC& hdc) {
	
	DrawBk(hdc);
	DrawMessages(hdc);
	DrawForum(hdc);
}


void SignIn::checkForClick(int xPos, int yPos) {
	//Click on microfost logo
	if (PtInRect(&rect[0], {xPos, yPos})) {
		const wchar_t* url = L"https://apps.microsoft.com/store/apps";

		HINSTANCE result = ShellExecute(nullptr, L"open", url, nullptr, nullptr, SW_SHOWNORMAL);

		if ((intptr_t)result > 32) {
			std::cout << "Webpage opened successfully." << std::endl;
		}
		else {
			std::cout << "Failed to open webpage." << std::endl;
		}


	}
	//Click on github logo
	if (PtInRect(&rect[1], { xPos, yPos })) {
		const wchar_t* url = L"https://github.com/Sudo-Hero/";

		HINSTANCE result = ShellExecute(nullptr, L"open", url, nullptr, nullptr, SW_SHOWNORMAL);

		if ((intptr_t)result > 32) {
			std::cout << "Webpage opened successfully." << std::endl;
		}
		else {
			std::cout << "Failed to open webpage." << std::endl;
		}
	}
	//Click on Login
	if (PtInRect(&rect[2], { xPos, yPos })) {
		char emailtext[MAX_EMAIL_LEN];
		GetWindowTextA(hEmail, emailtext, MAX_EMAIL_LEN);
		std::string email(emailtext);
		removeTrailingSpaces(email);
		if (!checkEmailFormat(email)) {
			MessageBoxA(NULL, "Incorrect email format", "Incorrect Details", MB_OK | MB_ICONERROR);
			//SInvalidateRect()
			return;
		}

		char passtext[MAX_EMAIL_LEN];
		GetWindowTextA(hPassword, passtext, MAX_EMAIL_LEN);
		std::string password(passtext);
		if (!checkDetails(email, password)) {
			MessageBoxA(NULL, "Please enter vaild email and password", "Incorrect details", MB_OK | MB_ICONERROR);
			return;
		}
		MessageBoxA(NULL, "Signup Prompt", "Signing up", MB_OK | MB_ICONINFORMATION);
	}
	//Click on SignUp
	if (PtInRect(&rect[3], { xPos, yPos })) {
		const wchar_t* url = L"http://localhost/CWms/users/register.php";

		HINSTANCE result = ShellExecute(nullptr, L"open", url, nullptr, nullptr, SW_SHOWNORMAL);

		if ((intptr_t)result > 32) {
			std::cout << "Webpage opened successfully." << std::endl;
		}
		else {
			std::cout << "Failed to open webpage." << std::endl;
		}
	}
}


bool SignIn::checkForHover(int xPos, int yPos) {
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

void SignIn::setRect() {
	rect = new RECT[5];
}

void SignIn::removeRect() {
	delete[] rect;
}

HWND SignIn::getEmailHWND() const{
	return hEmail;
}

HWND SignIn::getPassHWND() const{
	return hPassword;
}