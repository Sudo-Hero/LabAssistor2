#pragma once
#ifndef _SIGNIN_
#define _SIGNIN_

#include "utils.h"
//#include "Logger.h"
#include "mysql_conn.h"



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
	sql::Connection* conn;
	FileLogger *logger = nullptr;
	bool isStarted = startMysqlConn(conn);
	//Mysql Components 
	
public:
	//SignIn(const SignIn&) = delete;
	SignIn() {
		logger = new FileLogger("logger.txt");
		if (!isStarted) {
			logger->log("MYSQL Failed");
		}
	}
	//Draw components
	void Draw(HDC& hdc);
	void DrawInputs(HWND& hwnd);
	void checkForClick(int xPos, int yPos);
	bool checkForHover(int xPos, int yPos);

	void removeRect();
	void setRect();
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