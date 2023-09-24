#pragma once
#include <windows.h>

enum Style {
	Regular = 0,
	Bold = 1,
	Italic = 2,
	ItalicBold = 3
};

void SetFont(HWND hwnd, size_t height, Style style) {
	const wchar_t* fontName = nullptr;
	switch(style){
		case Style::Regular: {
			AddFontResource(L"Font\\Montserrat-Regular.ttf");
			fontName = L"Montserrat Regular";
			break;
		}
		case Style::Bold: {
			AddFontResource(L"Font\\Montserrat-Bold.ttf");
			fontName = L"Montserrat Bold";
			break;
		}
		case Style::Italic: {
			AddFontResource(L"Font\\Montserrat-Italic.ttf");
			fontName = L"Montserrat Italic";
			break;
		}
		case Style::ItalicBold: {
			AddFontResource(L"Font\\Montserrat-ItalicBold.ttf");
			fontName = L"Montserrat ItalicBold";
			break;
		}

	}
	
	
	HFONT hMontserrat = CreateFont(
		/* font parameters */
		height,                  // Height of the font
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
		fontName     // Font name (use the actual font name)
	);
	SendMessage(hwnd, WM_SETFONT, (WPARAM)hMontserrat, TRUE);
}