#include "stdafx.h"
#include "loading.h"

loadItem::loadItem(){}
loadItem::~loadItem(){}



HRESULT loadItem::initForImage(wstring keyName, const WCHAR* fileName, int width, int height)
{
	_kind = LOAD_KIND_IMAGE_0;

	memset(&_imageResource, 0, sizeof(tagImageResource));

	_imageResource.keyName = keyName;
	_imageResource.fileName = fileName;
	_imageResource.width = width;
	_imageResource.height = height;


	return S_OK;
}


HRESULT loadItem::initForFrameImage(wstring keyName, const WCHAR* fileName, int width, int height,
	int frameX, int frameY)
{
	_kind = LOAD_KIND_FRAMEIMAGE_0;

	memset(&_imageResource, 0, sizeof(tagImageResource));

	_imageResource.keyName = keyName;
	_imageResource.fileName = fileName;
	_imageResource.width = width;
	_imageResource.height = height;
	_imageResource.frameX = frameX;
	_imageResource.frameY = frameY;

	return S_OK;
}


HRESULT loadItem::initForSound(wstring keyName, const WCHAR* soundName, bool bgm, bool loop)
{
	_kind = LOAD_KIND_SOUND;
	memset(&_soundResource, 0, sizeof(tagSoundResource));
	_soundResource.keyName = keyName;
	_soundResource.fileName = soundName;
	_soundResource.bgm = bgm;
	_soundResource.loop = loop;

	return S_OK;
}

loading::loading()
{
}


loading::~loading()
{

}

HRESULT loading::init()
{
	//_loadingBar = new progressBar;
	//_loadingBar->setGauge(0, 0);
	//
	//_currentGauge = 0;
	//
	//ZeroMemory(&_loadingFileName, sizeof(_loadingFileName));

	
	return S_OK;
}

void loading::release()
{
	SAFE_DELETE(_loadingBar);
}

void loading::update() 
{
	//_loadingBar->update();
}

void loading::render() 
{
	//_background->render(false, 1.0f);
	//_loadingBar->render(false, 1.0f);

	DIRECT2D->drawTextD2D(DIRECT2D->_defaultBrush, L"맑은 고딕", 20, _loadingFileName, 10, WINSIZEY - 100, 800, WINSIZEY);
}




void loading::loadImage(wstring keyName, const WCHAR* fileName, int width, int height)
{
	loadItem* item = new loadItem;
	item->initForImage(keyName, fileName, width, height);

	_vLoadItem.push_back(item);
}


void loading::loadFrameImage(wstring keyName, const WCHAR* fileName, int width, int height, int frameX, int frameY)
{
	loadItem* item = new loadItem;
	item->initForFrameImage(keyName, fileName, width, height, frameX, frameY);

	_vLoadItem.push_back(item);
}
void loading::loadSound(wstring key, const WCHAR* soundName, BOOL bgm, BOOL loop)
{
	loadItem* item = new loadItem;
	item->initForSound(key, soundName, bgm, loop);
	_vLoadItem.push_back(item);
}


//로딩하는 곳
BOOL loading::loadingDone()
{

	//로딩 게이지가 올라간다는 것은 메모리에 이미지, 사운드 등이 올라가고있고,
	//벡터사이즈보다 같거나 크다는 것은. 로딩이 끝났다는 걸 의미
	if (_currentGauge >= _vLoadItem.size())
	{
		return TRUE;
	}

	loadItem* item = _vLoadItem[_currentGauge];

	switch (item->getLoadingKind())
	{
		case LOAD_KIND_IMAGE_0:
		{
			tagImageResource img = item->getImageResource();
			IMAGEMANAGER->addImage(img.keyName, img.fileName, img.width, img.height);
			swprintf_s(_loadingFileName, L"%s", img.fileName);
		}
		break;

		

		case LOAD_KIND_FRAMEIMAGE_0:
		{
			tagImageResource img = item->getImageResource();
			IMAGEMANAGER->addFrameImage(img.keyName, img.fileName, img.width, img.height, img.frameX, img.frameY);
			swprintf_s(_loadingFileName, L"%s", img.fileName);
		}
		break;


		case LOAD_KIND_SOUND:
		{
			tagSoundResource sound = item->getSoundResource();
			SOUNDMANAGER->addSound(sound.keyName, sound.fileName, sound.bgm, sound.loop);
			swprintf_s(_loadingFileName, L"%s", sound.fileName);
		}
		break;
	}

	_loadingBar->setGauge(_currentGauge, _vLoadItem.size());

	_currentGauge++;


	return FALSE;
}
