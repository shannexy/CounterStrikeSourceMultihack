#include "isurface.hpp"

#include "../core/interfaces.hpp"

unsigned long Fonts::mainFont;
unsigned long Fonts::tabFont;
unsigned long Fonts::espFont;

#undef CreateFont
#undef DrawText

void Fonts::initializeFonts() noexcept
{
	mainFont = interfaces::surface->CreateFont();
	tabFont = interfaces::surface->CreateFont();
	espFont = interfaces::surface->CreateFont();

	interfaces::surface->SetFontGlyphSet(mainFont, "Verdana", 12, 400, FONT_FLAG_DROPSHADOW);
	interfaces::surface->SetFontGlyphSet(espFont, "Verdana", 16, 400, FONT_FLAG_DROPSHADOW);
	interfaces::surface->SetFontGlyphSet(tabFont, "Exonence", 42, 200, FONT_FLAG_DROPSHADOW | FONT_FLAG_ANTIALIAS);
}

void Draw::String(int x, int y, unsigned long font, CColor color, const char* text, bool centered)
{
	size_t o_Size = strlen(text) + 1;
	const size_t n_Size = 1024;
	size_t convertedChars = 0;
	wchar_t wcstring[n_Size];
	mbstowcs_s(&convertedChars, wcstring, o_Size, text, _TRUNCATE);

	int width, height;
	interfaces::surface->GetTextSize(font, wcstring, width, height);

	if (centered) {
		x -= width / 2;
	}

	interfaces::surface->DrawSetTextFont(font);
	interfaces::surface->DrawSetColor(color.r, color.g, color.b, color.a);
	interfaces::surface->DrawSetTextPosition(x, y);
	interfaces::surface->DrawText(wcstring, wcslen(wcstring));
}
