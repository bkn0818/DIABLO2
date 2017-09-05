#pragma once
#include "singletonBase.h"


class direct2DManager : public singletonBase<direct2DManager>
{
public:

	ID2D1Factory*				_direct2DFactory;		//팩토리를 생성해야 그리든지 뭐든지 할 수 있다.
	ID2D1HwndRenderTarget*		_renderTarget;			//윈도우에 그리기 위해 HwndRender에다가 타겟을 잡는다
	IDWriteFactory*				_writeFactory;			//텍스트를 쓰기 위한 팩토리 생성
	IDWriteTextFormat*			_defaultTextFormat;		//기본 텍스트 포맷
	IDWriteTextFormat*			_customTextFormat;		//커스텀 텍스트 포맷
	ID2D1SolidColorBrush*		_defaultBrush;			//기본 검정색 브러쉬

	

	HRESULT init();
	void release();
	void beginDraw();									//최종 렌더에 처음에 넣어준다.
	void endDraw();										//최종 렌더에 나중에 넣어준다.
	ID2D1SolidColorBrush* createBrush(COLORREF rgb, float opacity = 1.0f);


	void drawTextD2D
	(
		ID2D1SolidColorBrush* brush,								// 글자색
		LPCWSTR string,												// 문자열
		float startX, float startY, float endX, float endY			// 텍스트 렉트박스
	);

	
	void drawTextD2D
	(
		ID2D1SolidColorBrush* brush,								// 글자색
		LPCWSTR fontName,											// 폰트이름
		float fontSize, LPCWSTR string,								// 폰트 크기, 문자열
		float startX, float startY, float endX, float endY			// 텍스트 렉트박스
	);


	void drawLine(ID2D1SolidColorBrush* brush, float startX, float startY, float endX, float endY, bool onCamera, float strokeWidth = 1.0f);
	void drawRectangle(ID2D1SolidColorBrush* brush, float startX, float startY, float endX, float endY, bool onCamera, float strokeWidth = 1.0f);
	void drawEllipse(ID2D1SolidColorBrush* brush, float startX, float startY, float endX, float endY, bool onCamera, float strokeWidth = 1.0f);
	void fillRectangle(ID2D1SolidColorBrush* brush, float startX, float startY, float endX, float endY, bool onCamera);
	void fillEllipse(ID2D1SolidColorBrush* brush, float startX, float startY, float endX, float endY, bool onCamera);

	void reCreateResource();
	void ResizeWindowSize(UINT uWidth, UINT uHeight);

	direct2DManager();
	~direct2DManager();
};

