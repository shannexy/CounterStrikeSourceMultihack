#pragma once

#include "../utils/memory.hpp"
#include "ccolor.hpp"
enum EFontDrawType : std::int32_t
{
	FONT_DRAW_DEFAULT = 0,
	FONT_DRAW_NONADDITIVE,
	FONT_DRAW_ADDITIVE
};

enum EFontFlag
{
	FONT_FLAG_NONE,
	FONT_FLAG_ITALIC = 0x001,
	FONT_FLAG_UNDERLINE = 0x002,
	FONT_FLAG_STRIKEOUT = 0x004,
	FONT_FLAG_SYMBOL = 0x008,
	FONT_FLAG_ANTIALIAS = 0x010,
	FONT_FLAG_GAUSSIANBLUR = 0x020,
	FONT_FLAG_ROTARY = 0x040,
	FONT_FLAG_DROPSHADOW = 0x080,
	FONT_FLAG_ADDITIVE = 0x100,
	FONT_FLAG_OUTLINE = 0x200,
	FONT_FLAG_CUSTOM = 0x400,
	FONT_FLAG_BITMAP = 0x800,
};

namespace Fonts {
	extern unsigned long mainFont;
	extern unsigned long tabFont;
	extern unsigned long espFont;

	void initializeFonts() noexcept;
}

namespace Draw {
	void String(int x, int y, unsigned long font, CColor color, const char* text, bool centered = false);
}

class ISurface
{
public:
	constexpr void DrawSetColor(int r, int g, int b, int a = 255) noexcept {
		memory::Call<void>(this, 11, r, g, b, a);
	}

	constexpr void DrawSetColor(CColor color) noexcept {
		memory::Call<void>(this, 10, color);
	}

	constexpr void DrawFilledRect(int x, int y, int xx, int yy) noexcept
	{
		memory::Call<void>(this, 12, x, y, xx, yy);
	}

	constexpr void DrawOutlinedRect(int x, int y, int xx, int yy) noexcept
	{
		memory::Call<void>(this, 14, x, y, xx, yy);
	}

	constexpr void DrawLine(int x, int y, int xx, int yy) noexcept {
		memory::Call<void>(this, 15, x, y, xx, yy);
	}

	constexpr void DrawSetTextFont(unsigned long eFont) noexcept {
		memory::Call<void>(this, 17, eFont);
	}

	constexpr void DrawSetTextColor(int r, int g, int b, int a) noexcept {
		memory::Call<void>(this, 1, r, g, b, a);
	}

	constexpr void DrawSetTextPosition(int x, int y) noexcept {
		memory::Call<void>(this, 20, x, y);
	}

	constexpr void DrawOutlinedCircle(int x, int y, int xx, int yy) noexcept {
		memory::Call<void>(this, 99, x, y, xx, yy);
	}

	constexpr void DrawText(const wchar_t* chText, int iSize) noexcept {
		memory::Call<void>(this, 22, chText, iSize, 0);
	}

	constexpr void UnlockCursor() noexcept {
		memory::Call<void>(this, 61);
	}

	constexpr void SetCursorAlwaysVisible(bool visible) noexcept {
		memory::Call<void>(this, 52, visible);
	}

	constexpr void LockCursor() noexcept {
		memory::Call<void>(this, 62);
	}

	constexpr unsigned long CreateFont() noexcept
	{
		return memory::Call<unsigned long>(this, 66);
	}

	constexpr void SetFontGlyphSet(unsigned long uFont, const char* chFontName, int iTall, int iHeight, int iFlags) noexcept {
		memory::Call<void>(this, 67, uFont, chFontName, iTall, iHeight, 0, 0, iFlags, 0, 0);
	}

	constexpr bool AddCustomFontFile(const char* fontName, const char* fontFileName) noexcept
	{
		return memory::Call<bool>(this, 68, fontName, fontFileName);
	}

	constexpr void GetTextSize(unsigned long font, const wchar_t* text, int& wide, int& tall) noexcept {
		memory::Call<void>(this, 75, font, text, std::ref(wide), std::ref(tall));
	}
};

enum paintModeT
{
	PAINT_UIPANELS = (1 << 0),
	PAINT_INGAMEPANELS = (1 << 1),
	PAINT_CURSOR = (1 << 2)
};