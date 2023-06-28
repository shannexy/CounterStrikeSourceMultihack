#pragma once


#include "ccolor.hpp"
#include "cmatrix.hpp"
#include "cvector.hpp"

#define PRINTF_FORMAT_STRING

class IVDebugOverlay
{
public:
	virtual void AddEntityTextOverlay(int ent_index, int line_offset, float duration, int r, int g, int b, int a, PRINTF_FORMAT_STRING const char* format, ...) = 0;
	virtual void AddBoxOverlay(const CVector& origin, const CVector& mins, const CVector& max, CVector const& orientation, int r, int g, int b, int a, float duration) = 0;
	virtual void AddTriangleOverlay(const CVector& p1, const CVector& p2, const CVector& p3, int r, int g, int b, int a, bool noDepthTest, float duration) = 0;
	virtual void AddLineOverlay(const CVector& origin, const CVector& dest, int r, int g, int b, bool noDepthTest, float duration) = 0;
	virtual void AddTextOverlay(const CVector& origin, float duration, PRINTF_FORMAT_STRING const char* format, ...) = 0;
	virtual void AddTextOverlay(const CVector& origin, int line_offset, float duration, PRINTF_FORMAT_STRING const char* format, ...) = 0;
	virtual void AddScreenTextOverlay(float flXPos, float flYPos, float flDuration, int r, int g, int b, int a, const char* text) = 0;
	virtual void AddSweptBoxOverlay(const CVector& start, const CVector& end, const CVector& mins, const CVector& max, const CVector& angles, int r, int g, int b, int a, float flDuration) = 0;
	virtual void AddGridOverlay(const CVector& origin) = 0;
	virtual int ScreenPosition(const CVector& point, CVector& screen) = 0;
	virtual int ScreenPosition(float flXPos, float flYPos, CVector& screen) = 0;
};