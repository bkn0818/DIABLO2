#include "stdafx.h"
#include "timeManager.h"


timeManager::timeManager()
	: _timer(NULL)
{
}


timeManager::~timeManager()
{

}

HRESULT timeManager::init()
{
	_timer = new timer;
	_timer->init();

	return S_OK;
}

void timeManager::release()
{
	if (_timer != NULL)
	{
		SAFE_DELETE(_timer);
		_timer = NULL;
	}
}

void timeManager::update(float lock)
{
	if (_timer != NULL)
	{
		_timer->tick(lock);
	}
}

void timeManager::render()
{
	WCHAR str[256];
	//string frameRate;
	//SetBkMode(hdc, TRANSPARENT);
	//TRANSPARENT : 폰트 뒷배경 투명
	//OPAQUE : 폰트 뒷배경 불투명


#ifdef _DEBUG
	{
		//FPS
		swprintf_s(str, L"framePerSec(FPS) : %d", _timer->getFrameRate());
		DIRECT2D->drawTextD2D(DIRECT2D->_defaultBrush, str, 10, 10, 200, 20);

		//WorldTime
		swprintf_s(str, L"worldTime : %f", _timer->getWorldTime());
		DIRECT2D->drawTextD2D(DIRECT2D->_defaultBrush, str, 10, 30, 200, 50);

		//ElapsedTime
		swprintf_s(str, L"elaspedTime : %f", _timer->getElapsedTime());
		DIRECT2D->drawTextD2D(DIRECT2D->_defaultBrush, str, 10, 50, 200, 70);
	}
#else
	{
		//FPS
		swprintf_s(str, L"framePerSec(FPS) : %d", _timer->getFrameRate());
		DIRECT2D->drawTextD2D(DIRECT2D->_defaultBrush, str, 10, 10, 200, 20);

		//WorldTime
		swprintf_s(str, L"worldTime : %f", _timer->getWorldTime());
		DIRECT2D->drawTextD2D(DIRECT2D->_defaultBrush, str, 10, 30, 200, 50);

		//ElapsedTime
		swprintf_s(str, L"elaspedTime : %f", _timer->getElapsedTime());
		DIRECT2D->drawTextD2D(DIRECT2D->_defaultBrush, str, 10, 50, 200, 70);
	}
#endif

}
