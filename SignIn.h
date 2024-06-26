#pragma once
#ifndef _SIGNIN_
#define _SIGNIN_

#include "utils.h"
//#include "Logger.h"
#include "mysql_conn.h"
#include "SignIn_Check.h"
#include "ActivationBox.h"

#define MAX_EMAIL_LEN 100

/***************************************************
UI  :- Draw signin components
Logic :- Handle Logics
/***************************************************/
class SignIn {
private:
	void DrawBk(HDC& hdc);
	void DrawMessages(HDC& hdc);
	void DrawForum(HDC& hdc);
	HFONT hMontserrat;
	HWND hMain,hEmail, hPassword;
	RECT *rect;
	sql::Connection* conn = nullptr;
	FileLogger *logger = nullptr;
	bool isStarted;
	bool isActivated;
	bool isFirstTime = false;
	bool isLoggedin = false;
	//Mysql Components 
	ActivationBox activationbox;
	void askForToken();
	void Destroy();
public:
	//SignIn(const SignIn&) = delete;
	SignIn() {
		conn = startMysqlConn();
		logger = new FileLogger("log.txt");
		if (!conn) {
			logger->log("MYSQL Failed");
			MessageBoxA(NULL, "MYSQL Failed", "Something went wrong", MB_ICONERROR | MB_OK);
			PostQuitMessage(-1);
			exit(-1);
		}
		 
	}
	//Draw components
	void Draw(HDC& hdc);
	void DrawInputs(HWND& hwnd);
	void checkForClick(int xPos, int yPos);
	bool checkForHover(int xPos, int yPos);

	void removeRect();
	void setRect();

	HWND getEmailHWND() const;
	HWND getPassHWND() const;
	bool getStatus() const;
	//Logic Part
	void ActivationCheck();
	bool checkEmailFormat(std::string email);
	bool checkDetails(std::string email, std::string password);

	~SignIn() {
		DestroyWindow(hEmail);
		DestroyWindow(hPassword);
		RemoveFontResource(L"Font\\Montserrat-Regular.ttf");
		DeleteObject(hMontserrat);  // Release the font handle
		delete logger;
		delete conn;
		//delete[] rect;
	}
};
#endif // !_SIGNIN_