#include "stdafx.h"
#include "direct2DManager.h"


direct2DManager::direct2DManager(){}


direct2DManager::~direct2DManager(){}


HRESULT direct2DManager::init()
{
	_direct2DFactory = NULL;
	_renderTarget = NULL;
	_defaultBrush = NULL;
	_defaultTextFormat = NULL;
	_customTextFormat = NULL;
	_writeFactory = NULL;
	
	//pBitmapRenderTarget = nullptr;
	//pBackBuffer = nullptr;


	HRESULT hResult = E_FAIL;
	
	//---------------------------------------------------------------------------
	//	D2D Factory 생성
	//---------------------------------------------------------------------------
	hResult = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &_direct2DFactory);

	assert(hResult == S_OK && "CreateFactory is not created");

	
	//---------------------------------------------------------------------------
	//	Hwnd Render Target 생성
	//---------------------------------------------------------------------------
	hResult = _direct2DFactory->CreateHwndRenderTarget(
		RenderTargetProperties(),
		HwndRenderTargetProperties(_hWnd, SizeU(WINSIZEX, WINSIZEY)),
		&_renderTarget);

	assert(hResult == S_OK && "CreateRenderTarget is not created");

	//---------------------------------------------------------------------------
	//	Bitmap Render Target 생성
	//---------------------------------------------------------------------------

	

	//---------------------------------------------------------------------------
	//	IDWriteFactory 생성
	//---------------------------------------------------------------------------
	hResult = DWriteCreateFactory
	(
		DWRITE_FACTORY_TYPE_SHARED,										// 프로세서 공유모드타입
		__uuidof(_writeFactory),										// COM 관련 인자
		reinterpret_cast<IUnknown **>(&_writeFactory)
	);					// 반환값

	assert(hResult == S_OK && "CreateFactory is not created");


	// 브러쉬 생성
	_defaultBrush = createBrush(RGB(0, 0, 0));

	//	기본 텍스트 포맷 생성
	_writeFactory->CreateTextFormat
	(
		L"맑은고딕",										//폰트 이름
		NULL,											//폰트 컬렉션 ( 안쓰면 NULL)
		DWRITE_FONT_WEIGHT_REGULAR,						//폰트 굵기
		DWRITE_FONT_STYLE_NORMAL,						//폰트 스타일
		DWRITE_FONT_STRETCH_NORMAL,						//스트레치, 근데 어떤 값이든 별반 차이를 못느낀다.
		17.0f,											//폰트 사이즈
		L"",											//미국은 en-us, 한국은 ko-KR 일본은 ja-JP, 영국은 en-uk
		&_defaultTextFormat								//결과가 지정된 포인터
	);

	// COM 라이브러리 초기화 -> 호출하지 않으면 CoCreateInstance가 제대로 수행되지 않음
	CoInitialize(NULL);
	
	return S_OK;
}
void direct2DManager::release()
{
	SAFE_RELEASE_CAPITAL(_defaultBrush);
	SAFE_RELEASE_CAPITAL(_defaultTextFormat);
	SAFE_RELEASE_CAPITAL(_writeFactory);
	SAFE_RELEASE_CAPITAL(_renderTarget);
	SAFE_RELEASE_CAPITAL(_direct2DFactory);
	SAFE_RELEASE_CAPITAL(_customTextFormat);
}


void direct2DManager::beginDraw()
{
	_renderTarget->BeginDraw();
	_renderTarget->Clear(ColorF(ColorF::Black));
}

void direct2DManager::endDraw()
{
	_renderTarget->EndDraw();
}

void direct2DManager::reCreateResource()
{
	HRESULT hResult = _renderTarget->EndDraw();

	if (hResult == D2DERR_RECREATE_TARGET)
	{
		this->release();
		InvalidateRect(_hWnd, NULL, false);
		this->init();
	}
}

void direct2DManager::ResizeWindowSize(UINT uWidth, UINT uHeight)
{
	if (_renderTarget != NULL)
	{
		HRESULT hResult = _renderTarget->Resize(SizeU(uWidth, uHeight));

		if (hResult != S_OK)
		{
			this->release();
		}
	}
}

ID2D1SolidColorBrush* direct2DManager::createBrush(COLORREF rgb, float opacity)
{
	HRESULT hResult;
	ID2D1SolidColorBrush* brush;
	brush = NULL;

	hResult = _renderTarget->CreateSolidColorBrush(ColorF(rgb, opacity), &brush);
	
	return brush;
}


void direct2DManager::drawTextD2D(ID2D1SolidColorBrush* brush, LPCWSTR string, float startX, float startY, float endX, float endY)
{
	float x1 = startX;
	float y1 = startY;
	float x2 = endX;
	float y2 = endY;

	_renderTarget->DrawTextW(string, lstrlenW(string), _defaultTextFormat, RectF(x1, y1, x2, y2), brush);
	
	
}

void direct2DManager::drawTextD2D(ID2D1SolidColorBrush* brush, LPCWSTR fontName, float fontSize, LPCWSTR string, float startX, float startY, float endX, float endY)
{
	float x1 = startX;
	float y1 = startY;
	float x2 = endX;
	float y2 = endY;

	//	TextFormat 생성
	_writeFactory->CreateTextFormat(fontName, NULL, DWRITE_FONT_WEIGHT_HEAVY,
		DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, fontSize, L"", &_customTextFormat);


	_renderTarget->DrawTextW(string, lstrlenW(string), _customTextFormat, RectF(x1, y1, x2, y2), brush);

	SAFE_RELEASE_CAPITAL(_customTextFormat);
}


void direct2DManager::drawLine(ID2D1SolidColorBrush* brush, float startX, float startY, float endX, float endY, bool onCamera, float strokeWidth)
{
	float x1;
	float y1;
	float x2;
	float y2;

	if (onCamera)
	{
		x1 = startX - CAMERAMANAGER->getX();
		y1 = startY - CAMERAMANAGER->getY();
		x2 = endX - CAMERAMANAGER->getX();
		y2 = endY - CAMERAMANAGER->getY();
	}
	else
	{
		x1 = startX;
		y1 = startY;
		x2 = endX;
		y2 = endY;
	}

	if (x1 < 0 && x2 < 0) return;
	if (y1 < 0 && y2 < 0) return;
	if (x1 > WINSIZEX && x2 > WINSIZEX) return;
	if (y1 > WINSIZEY && y2 > WINSIZEY) return;

	_renderTarget->DrawLine(Point2F(x1, y1), Point2F(x2, y2), brush, strokeWidth);

	//사용 예
	//						디폴트 색( 검정)												   선 굵기
	//DIRECT2D->drawLine(DIRECT2D->_defaultBrush, _rc.left, _rc.top, _rc.right, _rc.bottom, 10);
}


void direct2DManager::drawRectangle(ID2D1SolidColorBrush* brush, float startX, float startY, float endX, float endY, bool onCamera, float strokeWidth)				 
{
	float x1;
	float y1;
	float x2;
	float y2;

	if (onCamera)
	{
		x1 = startX - CAMERAMANAGER->getX();
		y1 = startY - CAMERAMANAGER->getY();
		x2 = endX - CAMERAMANAGER->getX();
		y2 = endY - CAMERAMANAGER->getY();
	}
	else
	{
		x1 = startX;
		y1 = startY;
		x2 = endX;
		y2 = endY;
	}

	if (x1 < 0 && x2 < 0) return;
	if (y1 < 0 && y2 < 0) return;
	if (x1 > WINSIZEX && x2 > WINSIZEX) return;
	if (y1 > WINSIZEY && y2 > WINSIZEY) return;


	_renderTarget->DrawRectangle(RectF(x1, y1, x2, y2), brush, strokeWidth);

	//사용 예
	//					     	디폴트 색( 검정)												   선 굵기
	//DIRECT2D->drawRectangle(DIRECT2D->_defaultBrush, _rc.left, _rc.top, _rc.right, _rc.bottom, 10);
}


void direct2DManager::drawEllipse(ID2D1SolidColorBrush* brush, float startX, float startY, float endX, float endY, bool onCamera, float strokeWidth)				 
{
	float x1;
	float y1;
	float x2;
	float y2;
	float width = endX - startX;
	float height = endY - startY;

	if (onCamera)
	{
		x1 = startX - CAMERAMANAGER->getX();
		y1 = startY - CAMERAMANAGER->getY();
		x2 = endX - CAMERAMANAGER->getX();
		y2 = endY - CAMERAMANAGER->getY();
	}
	else
	{
		x1 = startX;
		y1 = startY;
		x2 = endX;
		y2 = endY;
	}

	if (x1 < 0 && x2 < 0) return;
	if (y1 < 0 && y2 < 0) return;
	if (x1 > WINSIZEX && x2 > WINSIZEX) return;
	if (y1 > WINSIZEY && y2 > WINSIZEY) return;

	D2D1_ELLIPSE ellipse;
	ellipse.point.x = x1 + width * 0.5;
	ellipse.point.y = y1 + height * 0.5;
	ellipse.radiusX = width * 0.5;
	ellipse.radiusY = height * 0.5;

	_renderTarget->DrawEllipse(&ellipse, brush, strokeWidth);

	//사용 예
	//					     	디폴트 색( 검정)												   선 굵기
	//DIRECT2D->drawEllipse(DIRECT2D->_defaultBrush, _rc.left, _rc.top, _rc.right, _rc.bottom, 10);
}


void direct2DManager::fillRectangle(ID2D1SolidColorBrush* brush, float startX, float startY, float endX, float endY, bool onCamera)										 
{
	float x1;
	float y1;
	float x2;
	float y2;

	if (onCamera)
	{
		x1 = startX - CAMERAMANAGER->getX();
		y1 = startY - CAMERAMANAGER->getY();
		x2 = endX - CAMERAMANAGER->getX();
		y2 = endY - CAMERAMANAGER->getY();
	}
	else
	{
		x1 = startX;
		y1 = startY;
		x2 = endX;
		y2 = endY;
	}

	if (x1 < 0 && x2 < 0) return;
	if (y1 < 0 && y2 < 0) return;
	if (x1 > WINSIZEX && x2 > WINSIZEX) return;
	if (y1 > WINSIZEY && y2 > WINSIZEY) return;

	_renderTarget->FillRectangle(RectF(x1, y1, x2, y2), brush);

}


void direct2DManager::fillEllipse(ID2D1SolidColorBrush* brush, float startX, float startY, float endX, float endY, bool onCamera)											 
{

	float x1;
	float y1;
	float x2;
	float y2;

	if (onCamera)
	{
		x1 = startX - CAMERAMANAGER->getX();
		y1 = startY - CAMERAMANAGER->getY();
		x2 = endX - CAMERAMANAGER->getX();
		y2 = endY - CAMERAMANAGER->getY();
	}
	else
	{
		x1 = startX;
		y1 = startY;
		x2 = endX;
		y2 = endY;
	}
	float width = endX - startX;
	float height = endY - startY;


	if (x1 < 0 && x2 < 0) return;
	if (y1 < 0 && y2 < 0) return;
	if (x1 > WINSIZEX && x2 > WINSIZEX) return;
	if (y1 > WINSIZEY && y2 > WINSIZEY) return;

	D2D1_ELLIPSE ellipse;
	ellipse.point.x = x1 + width * 0.5;
	ellipse.point.y = y1 + height * 0.5;
	ellipse.radiusX = width * 0.5;
	ellipse.radiusY = height * 0.5;

	_renderTarget->FillEllipse(&ellipse, brush);
}
