#pragma once

//===========================================
// ## 17.04.05 ## CommonMacroFunction ##
//===========================================

inline POINT PointMake(int x, int y)
{
	POINT pt = { x, y };

	return pt;
}

//Line
inline void LineMake(HDC hdc, int x1, int y1, int x2, int y2)
{
	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);
}

//Rect
//시작좌표를 기점으로 만든다 시작좌표X 시작좌표Y 가로크기 세로크기
inline RECT RectMake(int x, int y, int width, int height)
{

	RECT rc = { x, y, x + width, y + height };

	return rc;
}

//중점좌표를 기점으로 만든다 중점좌표X, 중점좌표Y, 가로크기, 세로크기 
inline RECT RectMakeCenter(int x, int y, int width, int height)
{
	RECT rc = { x - (width / 2) , y - (height / 2) , x + (width / 2), y + (height / 2) };

	return rc;
}

inline RECT RectMakeBottomCenter(int x, int y, int width, int height)
{
	RECT rc = { x - width / 2, y - height, x + width / 2, y };

	return rc;
}

//Rectangle
inline void RectangleMake(HDC hdc, int x, int y, int width, int height)
{
	Rectangle(hdc, x, y, x + width, y + height);
}

inline void RectangleMakeCenter(HDC hdc, int x, int y, int width, int height)
{
	Rectangle(hdc, x - (width / 2), y - (height / 2), x + (width / 2), y + (height / 2));
}

//Ellipse

inline void EllipseMake(HDC hdc, int x, int y, int width, int height)
{
	Ellipse(hdc, x, y, x + width, y + height);
}

inline void EllipseMakeCenter(HDC hdc, int x, int y, int width, int height)
{
	Ellipse(hdc, x - (width / 2), y - (height / 2), x + (width / 2), y + (height / 2));
}