#pragma once
#include "utils.h"

void  Label::LoadFonts() {
	PrivateFontCollection xpFont;
	INT                   found = 0;
	INT                   count = 0;
	FontFamily			  fontFamily;
	if (_fontStyle == FontStyleRegular) {
		xpFont.AddFontFile(L"Font\\Montserrat-Regular.ttf");
	}
	else if (_fontStyle == FontStyleBold) {
		xpFont.AddFontFile(L"Font\\Montserrat-Bold.ttf");
	}
	else if (_fontStyle == FontStyleItalic) {
		xpFont.AddFontFile(L"Font\\Montserrat-Italic.ttf");
	}
	
	xpFont.GetFamilies(1, &pFontFamily, &found); 
}

void Label::setRegion() {
	_Lregion = {_x, _y, _x + (int)_text.length() * ((int)_size - 5), _y + (int)_size};
}

RECT Label::getRect() {
	return _Lregion;
}

void Label::Draw(HDC& hdc) {
	Graphics graphics(hdc);
	graphics.SetSmoothingMode(SmoothingModeHighQuality);
	graphics.SetTextRenderingHint(TextRenderingHintAntiAlias);
	PointF				point((float)_x,(float) _y);
	Font				pFont(&pFontFamily, _size, _fontStyle, UnitPixel);

	graphics.DrawString(_text.c_str(), -1, &pFont, point, _brush);
}

std::wstring Label::getText() const {
	return _text;
}

void InputBox::Draw() {
	InpBoxStruct._hWnd = CreateWindow(L"EDIT", InpBoxStruct._text.c_str(), InpBoxStruct._style, InpBoxStruct._x, InpBoxStruct._y, InpBoxStruct._width, InpBoxStruct._height, InpBoxStruct._hParentWnd, InpBoxStruct._id, InpBoxStruct.hInst, NULL);
	int dd = GetLastError();
	ShowWindow(InpBoxStruct._hWnd, SW_SHOW);
}

HWND InputBox::getHWND() const {
	return InpBoxStruct._hWnd;
}
//***************************************Button********************************************
void Button::DrawBk(HDC hdc) {
	Graphics graphics(hdc);
	graphics.SetSmoothingMode(SmoothingModeHighQuality);
	Pen pen(_btnBrush, 2.0f);
	// Create a round rectangle path
	GraphicsPath path;
	int radius = 15;
	path.AddArc(_x, _y, 2 * radius, 2 * radius, 180, 90);                            // Top-left corner
	path.AddArc(_x + _width - 2 * radius, _y, 2 * radius, 2 * radius, 270, 90);       // Top-right corner
	path.AddArc(_x + _width - 2 * radius, _y + _height - 2 * radius, 2 * radius, 2 * radius, 0, 90);  // Bottom-right corner
	path.AddArc(_x, _y + _height - 2 * radius, 2 * radius, 2 * radius, 90, 90);        // Bottom-left corner
	path.CloseFigure();

	// Draw the round-edge rectangle
	graphics.FillPath(_btnBrush, &path);
	graphics.DrawPath(&pen, &path);
	
}

void Button::setWidth() {
	int size = 16;
	
	_width = 10 + (_text.length() * 10) + 12;
}

void Button::setRegion() {
	_region = { _x , _y, _x + _width, _y + _height };
}

float Button::getXCenter() const {
	return (_x + (_width / 2) - 13);
}

float Button::getYCenter() const {
	return (_y + (_height / 2) - 10);
}
//-------------------------------------BlueButton------------------------------//
void BlueButton::DrawBtn(HDC hdc) {
	DrawBk(hdc);
	Draw(hdc);
}

RECT BlueButton::getRect() {
	return _region;
}
//-------------------------------------PaleButton------------------------------//
void PaleButton::DrawBtn(HDC hdc) {
	DrawBk(hdc);
	Draw(hdc);
}

RECT PaleButton::getRect() {
	return _region;
}
//***********************************************************************************************

//*********************************Create Card***************************************************
void Card::DrawCard(HDC& hdc) {
	Graphics graphics(hdc);
	graphics.SetSmoothingMode(SmoothingModeHighQuality);
	Pen pen(_brush, 2.0f);
	// Create a round rectangle path
	GraphicsPath path;
	int radius = 25;
	path.AddArc(_x, _y, 2 * radius, 2 * radius, 180, 90);                            // Top-left corner
	path.AddArc(_x + _width - 2 * radius, _y, 2 * radius, 2 * radius, 270, 90);       // Top-right corner
	path.AddArc(_x + _width - 2 * radius, _y + _height - 2 * radius, 2 * radius, 2 * radius, 0, 90);  // Bottom-right corner
	path.AddArc(_x, _y + _height - 2 * radius, 2 * radius, 2 * radius, 90, 90);        // Bottom-left corner
	path.CloseFigure();

	// Draw the round-edge rectangle
	graphics.FillPath(_brush, &path);
	graphics.DrawPath(&pen, &path);

}

void WhiteCard::Draw(HDC& hdc) {
	DrawCard(hdc);
}


void Line::DrawLine(HDC& hdc) {
	Graphics graphics(hdc);

	Status ff = graphics.DrawLine(_pen, _srcx, _srcy, _destx, _desty);
}

void LightGreyLine::Draw(HDC& hdc) {
	DrawLine(hdc);
}