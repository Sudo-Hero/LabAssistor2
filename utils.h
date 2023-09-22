#pragma once
#ifndef _UTILS_
#define _UTILS_


#include <windows.h>
#include <gdiplus.h>
#include <windowsx.h>
#include <iostream>
#pragma comment (lib,"Gdiplus.lib")

#define _UTILS
#define BTN_H 40
#define BTN_W 80

using namespace Gdiplus;

struct WNDSTRUCT {
	int _x, _y;
	int _height, _width;
	std::wstring _text;
	DWORD _style;
	HMENU _id;
	HWND _hParentWnd;
	HINSTANCE hInst;
	HWND _hWnd;
};

//*************************************************************************************
class Label {

private:
	int _x;
	int _y;
	REAL _size = 16;
	FontStyle _fontStyle;
	std::wstring _text;
	FontFamily pFontFamily;

	void LoadFonts();
	void setRegion();
protected:
	SolidBrush* _brush = nullptr;
public:
	RECT _Lregion;
	Label() = delete;
	Label(const Label&) = delete;
	explicit Label(int x, int y, const std::wstring& text, REAL size, FontStyle fontStyle)
		:_x{ x },
		_y{ y },
		_text{ text },
		_size{ size },
		_fontStyle{ fontStyle }
	 {
		LoadFonts();
		_brush = new SolidBrush(Color(255, 90, 90, 90));
		setRegion();
		
	}
	
	std::wstring getText() const;
	RECT getRect();
	void Draw(HDC& hdc);
};

//FontFamily* Label::pFontFamily = nullptr;

class ErrorLabel: public Label {

public:
	ErrorLabel() = delete;
	ErrorLabel(const ErrorLabel&) = delete;
	explicit ErrorLabel(int x, int y, const std::wstring& text, REAL size, FontStyle fontStyle)
		:Label(x, y, text, size, fontStyle)
	{
		
		_brush = new SolidBrush(Color(255, 255, 0, 0));
	}
	~ErrorLabel() {
		delete _brush;
	}
};

class BlueLabel : public Label {

public:
	BlueLabel() = delete;
	BlueLabel(const BlueLabel&) = delete;
	explicit BlueLabel(float x, float y, const std::wstring& text, REAL size, FontStyle fontStyle)
		:Label(x, y, text, size, fontStyle)
	{

		_brush = new SolidBrush(Color(100, 149, 237));
	}
	~BlueLabel() {
		delete _brush;
	}
};
class DarkBlueLabel : public Label {

public:
	DarkBlueLabel() = delete;
	DarkBlueLabel(const DarkBlueLabel&) = delete;
	explicit DarkBlueLabel(int x, int y, const std::wstring& text, REAL size, FontStyle fontStyle)
		:Label(x, y, text, size, fontStyle)
	{

		_brush = new SolidBrush(Color(0, 0, 248));
	}
	~DarkBlueLabel() {
		delete _brush;
	}
};

class WhiteLabel : public Label {

public:
	WhiteLabel() = delete;
	WhiteLabel(const WhiteLabel&) = delete;
	explicit WhiteLabel(int x, int y, const std::wstring& text, REAL size, FontStyle fontStyle)
		:Label(x, y, text, size, fontStyle)
	{

		_brush = new SolidBrush(Color(250, 249, 246));
	}
	~WhiteLabel(){
		delete _brush;
	}
};

class DarkLabel : public Label {

public:
	DarkLabel() = delete;
	DarkLabel(const DarkLabel&) = delete;
	explicit DarkLabel(int x, int y, const std::wstring& text, REAL size, FontStyle fontStyle)
		:Label(x, y, text, size, fontStyle)
	{

		_brush = new SolidBrush(Color(192, 192, 192));
	}
	~DarkLabel() {
		delete _brush;
	}
};

class DarkExLabel : public Label {

public:
	DarkExLabel() = delete;
	DarkExLabel(const DarkExLabel&) = delete;
	explicit DarkExLabel(int x, int y, const std::wstring& text, REAL size, FontStyle fontStyle)
		:Label(x, y, text, size, fontStyle)
	{

		_brush = new SolidBrush(Color(90, 90, 90));
	}
	~DarkExLabel() {
		delete _brush;
	}
};
//*****************************************************************************************************
class InputBox{
private:
	WNDSTRUCT InpBoxStruct;
	
public:
	
	//CreateWindow(L"EDIT", L"Username", WS_CHILD | WS_VISIBLE | ES_PASSWORD,
		//50, 50, 200, 30, hWnd, (HMENU)1, hinst, 0);
	InputBox() = delete;
	InputBox(const InputBox&) = delete;
	
	explicit InputBox(HMENU id, std::wstring text, DWORD style, int x, int y, int height, int width, HWND hParentWnd):
		InpBoxStruct{ x, y, height, width, text, style, id, hParentWnd}
	{}
	HWND getHWND() const;
	void Draw();
};

/******************************************************************************************************************************************
For buttons the id indicated the type of the button (which color in its backgroud).
For E.g,
ID = 1 - Blue Background With White Text (Used for Highlight)
ID = 2 - Pale Blue With White Text  (Used for UnHighlighted)
ID = 3 - Gold Background With Black Text (Used for Extra Highlight)
***********************************************************/
class Button {
private:
	std::wstring _text;

	void setWidth();
	void setRegion();
protected:
	int _x, _y;
	int _height = BTN_H, _width;
	DWORD _id;
	SolidBrush *_btnBrush = nullptr;
	
	float getXCenter() const;
	float getYCenter() const;
	void DrawBk(HDC hdc);
public:
	bool isButtonPressed = false;
	RECT _region;
	Button() = delete;
	Button(const Button&) = delete;
	explicit Button(DWORD id,std::wstring text, int x, int y)
		:_id{ id },
		_text{ text },
		_x{ x}, 
		_y{y}
	{
		setWidth();
		setRegion();
	}
	
};

class BlueButton : public Button,public WhiteLabel {
protected:

public:
	BlueButton() = delete;
	BlueButton(const Button&) = delete;
	explicit BlueButton(DWORD id, std::wstring text, int x, int y)
		: Button(id, text, x, y), WhiteLabel( x + 10, y + 10,text, 16, FontStyleRegular) {
		delete _btnBrush;
		_btnBrush = new SolidBrush{ Color(0, 75, 255) };
		
	}
	void DrawBtn(HDC hdc);
	void DrawBtn(HDC hdc, int destx, int desty);
	RECT getRect();
	~BlueButton() {
		delete _btnBrush;
	}
};

class PaleButton : public Button, public BlueLabel {
protected:

public:
	PaleButton() = delete;
	PaleButton(const Button&) = delete;
	explicit PaleButton(DWORD id, std::wstring text, int x, int y)
		: Button(id, text, x, y), BlueLabel(x + 10, y + 10, text, 16, FontStyleRegular) {
		delete _btnBrush;
		_btnBrush = new SolidBrush{ Color(215, 229, 240) };

	}
	void DrawBtn(HDC hdc);
	
	RECT getRect();
	~PaleButton() {
		delete _btnBrush;
	}
};
//*******************************************************************************************************************


/*******************************************************************************************************************
											Create Card
*******************************************************************************************************************/

class Card {

private:
	int _x;
	int _y;
	int _height, _width;

protected:
	SolidBrush* _brush = nullptr;
	void DrawCard(HDC& hdc);
	Card() = delete;
	Card(const Card&) = delete;
	explicit Card(int x, int y, int width, int height)
		:_x{ x },
		_y{ y },
		_width{ width },
		_height{ height }
	{
		//_brush = new SolidBrush(Color(0, 0, 0));
	}
};


class WhiteCard : public Card{
public:

	WhiteCard() = delete;
	WhiteCard(const WhiteCard&) = delete;
	explicit WhiteCard(int x, int y, int width, int height)
		:Card(x, y ,width, height)
	{
		_brush = new SolidBrush(Color(255, 255, 255));

	}
	void Draw(HDC& hdc);
	~WhiteCard() {
		delete _brush;
	}

};

//*******************************************************************************************************************

class Line {

private:
	int _srcx, _srcy;
	int _destx, _desty;
	REAL _size;
	
protected:
	Pen* _pen = nullptr;
	explicit Line(int srcx, int srcy, int destx, int desty, REAL size)
		:_srcx{ srcx },
		_srcy{ srcy },
		_destx{ destx },
		_desty{ desty },
		_size{ size }
	{}
	void DrawLine(HDC& hdc);
public:
	Line() = delete;
	Line(const Line&) = delete;
};

class LightGreyLine : private Line {
public:
	LightGreyLine(int srcx, int srcy, int destx, int desty, REAL size)
		:Line{srcx, srcy, destx, desty, size}
	{
		_pen = new Pen(Color(211, 211, 211));
	}
	void Draw(HDC& hdc);
	~LightGreyLine() {
		delete _pen;
	}
};

#endif // !_UTILS_