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
//������ǥ�� �������� ����� ������ǥX ������ǥY ����ũ�� ����ũ��
inline RECT RectMake(int x, int y, int width, int height)
{

	RECT rc = { x, y, x + width, y + height };

	return rc;
}

//������ǥ�� �������� ����� ������ǥX, ������ǥY, ����ũ��, ����ũ�� 
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