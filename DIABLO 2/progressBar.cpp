#include "stdafx.h"
#include "progressBar.h"


progressBar::progressBar()
{
}


progressBar::~progressBar()
{

}

HRESULT progressBar::init(int x, int y, int width, int height)
{
	_x = x;
	_y = y;

	_rcProgress = RectMake(x, y, width, height);

	//_progressBarTop = IMAGEMANAGER->addImage("topBar", "hpBarTop.bmp", x, y, width, height, true, RGB(255, 0, 255));
	//_progressBarBottom = IMAGEMANAGER->addImage("backBar", "hpBarBottom.bmp", x, y, width, height, true, RGB(255, 0, 255));

	_width = _progressBarTop->getWidth();

	return S_OK;
}

//HRESULT progressBar::init(string topImage, string bottomImage, float x, float y, int width, int height)
//{
//	_x = x;
//	_y = y;
//
//	_rcProgress = RectMake(x, y, width, height);
//
//	CHAR strTopImageName[128];
//	CHAR strBottomImageName[128];
//
//	ZeroMemory(strTopImageName, sizeof(strTopImageName));
//	ZeroMemory(strBottomImageName, sizeof(strBottomImageName));
//
//	sprintf_s(strTopImageName, "Image/Loading/%s.png", topImage.c_str());
//	sprintf_s(strBottomImageName, "Image/Loading/%s.png", bottomImage.c_str());
//
//	//MultiByteToWideChar(CP_ACP, 0, strTopImageName, -1,  )
//
//	_progressBarBottom = IMAGEMANAGER->addImage(bottomImage.c_str(), strBottomImageName, width, height);
//	_progressBarTop = IMAGEMANAGER->addImage(topImage.c_str(), strTopImageName, width, height);
//
//	_width = _progressBarTop->getWidth();
//
//	return S_OK;
//}
HRESULT progressBar::init(WCHAR* topImage, WCHAR* bottomImage, float x, float y, int width, int height)
{
	_x = x;
	_y = y;

	_rcProgress = RectMake(x, y, width, height);

	WCHAR strTopImageName[128];
	WCHAR strBottomImageName[128];

	ZeroMemory(strTopImageName, sizeof(strTopImageName));
	ZeroMemory(strBottomImageName, sizeof(strBottomImageName));

	swprintf_s(strTopImageName, L"Image/%s.png", topImage);
	swprintf_s(strBottomImageName, L"Image/%s.png", bottomImage);

	_progressBarBottom = IMAGEMANAGER->addImage(bottomImage, strBottomImageName, width, height);
	_progressBarTop = IMAGEMANAGER->addImage(topImage, strTopImageName, width, height);

	_width = _progressBarTop->getWidth();

	return S_OK;
}

void progressBar::release()
{

}

void progressBar::update() 
{
	_rcProgress = RectMake(_x, _y,
		_progressBarBottom->getWidth(), _progressBarBottom->getHeight());
}

void progressBar::render(bool onCamera, float opacity) 
{
	//IMAGEMANAGER->render("backBar", getMemDC(),
	//	_rcProgress.left + _progressBarBottom->getWidth() / 2,
	//	_y + _progressBarBottom->getHeight() / 2,
	//	0, 0,
	//	_progressBarBottom->getWidth(), _progressBarBottom->getHeight());
	//
	//IMAGEMANAGER->render("topBar", getMemDC(),
	//	_rcProgress.left + _progressBarTop->getWidth() / 2,
	//	_y + _progressBarTop->getHeight() / 2,
	//	0, 0,
	//	_width, _progressBarTop->getHeight());

	_progressBarBottom->render(_rcProgress.left, _rcProgress.top, 0, 0,
		_progressBarBottom->getWidth(),
		_progressBarBottom->getHeight(), onCamera, opacity);

	_progressBarTop->render(_rcProgress.left, _rcProgress.top, 0, 0,
		_width,
		_progressBarTop->getHeight(), onCamera, opacity);



}


void progressBar::setGauge(float currentGauge, float maxGauge)
{
	//비율 구하는 공식이니 머리에 넣어뒁
	_width = (currentGauge / maxGauge) * _progressBarBottom->getWidth();
}
