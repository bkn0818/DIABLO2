#include "stdafx.h"
#include "ImageManager.h"


imageManager::imageManager()
{
}


imageManager::~imageManager()
{
}


void imageManager::release(void)
{
	deleteAll();
}



/*
===================================================================
이미지 추가
===================================================================
*/

//리소스에서 추가
/*
Image* ImageManager::AddImage(string strKey, const DWORD resID, int width, int height, bool trans, COLORREF transColor, BOOL useAlphaBlend)
{
//먼저 동일한 키 값을 가진 이미지가 있는지 검색
Image* img = FindImage(strKey);

//만약에 이미지가 있으면~ 그 이미지를 반환
if (img) return img;

//여기까지 왔으면 없는거니까
img = new Image;

//만약 제대로 이미지가 초기화 되지 않았으면 널값으로 초기화
if (FAILED(img->Init(resID, width, height, trans, transColor, useAlphaBlend)))
{
SAFE_DELETE(img);

return NULL;
}

//굉장히 잘 된 상황이므로 맵에 넣어줍니다 ㅋ _ㅋ
_mImageList.insert(make_pair(strKey, img));

return img;
}
*/

// 파일에서 추가
image* imageManager::addImage(wstring strKey, LPCWSTR fileName, int width, int height)
{
	//먼저 동일한 키 값을 가진 이미지가 있는지 검색
	image* img = findImage(strKey);

	// 이미지가 있으면 그 이미지를 반환
	if (img) return img;

	// 이미지가 없으면 메모리 동적 할당
	img = new image;

	//이미지를 초기화하고 실패했을 경우 메모리 해제
	if (FAILED(img->init(fileName, width, height)))
	{
		SAFE_DELETE(img);

		return NULL;
	}

	//맵에 추가
	_mImageList.insert(make_pair(strKey, img));

	return img;
}


//프레임 이미지 추가
image* imageManager::addFrameImage(wstring strKey, LPCWSTR fileName, int width, int height, int frameNumX, int frameNumY)
{
	//먼저 동일한 키 값을 가진 이미지가 있는지 검색
	image* img = findImage(strKey);

	// 이미지가 있으면 그 이미지를 반환
	if (img) return img;

	// 이미지가 없으면 메모리 동적 할당
	img = new image;

	//이미지를 초기화하고 실패했을 경우 메모리 해제
	if (FAILED(img->init(fileName, width, height, frameNumX, frameNumY)))
	{
		SAFE_DELETE(img);

		return NULL;
	}

	//맵에 추가
	_mImageList.insert(make_pair(strKey, img));

	return img;

}


//이미지 찾는 함수
image* imageManager::findImage(wstring strKey)
{
	mapImageIter key = _mImageList.find(strKey);

	if (key != _mImageList.end())
	{
		return key->second;
	}

	return NULL;
}


BOOL imageManager::deleteImage(wstring strKey)
{
	//해당 키 값이 존재하는지 검사한다
	mapImageIter key = _mImageList.find(strKey);

	if (key != _mImageList.end())
	{
		key->second->release();
		SAFE_DELETE(key->second);
		_mImageList.erase(key);

		return true;
	}

	return false;

}

BOOL imageManager::deleteAll(void)
{
	mapImageIter iter = _mImageList.begin();

	for (; iter != _mImageList.end();)
	{
		if (iter->second != NULL)
		{
			iter->second->release();
			SAFE_DELETE(iter->second);
			iter = _mImageList.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	_mImageList.clear();

	return TRUE;
}

/*
===================================================================
렌더 함수 - Left Top 기준으로 렌더
===================================================================
*/


//기본 출력
void imageManager::render(wstring strKey, bool onCamera, float opacity)
{
	image* img = findImage(strKey);
	if (img) img->render(onCamera, opacity);
}


//원래 크기 - 키값, 그려줄 x좌표, y좌표, 투명도
void imageManager::render(wstring strKey, float destX, float destY, bool onCamera, float opacity)
{
	image* img = findImage(strKey);
	if (img) img->render(destX, destY, onCamera, opacity);
}

//변형 크기 - 키값, 그려줄 x좌표, y좌표, 출력할 너비, 출력할 높이, 투명도
void imageManager::render(wstring strKey, float destX, float destY, int showWidth, int showHeight, bool onCamera, float opacity)
{
	image* img = findImage(strKey);
	if (img) img->render(destX, destY, showWidth, showHeight, onCamera, opacity);
}


//원래 크기로 잘라서 가져오기 - 키값, 그려줄 x좌표, y좌표 -> 가져올 곳의 left, top, 가로 크기, 세로 크기, 투명도
void imageManager::render(wstring strKey, float destX, float destY, float sourX, float sourY,
	int sourWidth, int sourHeight, bool onCamera, float opacity)
{
	image* img = findImage(strKey);
	if (img) img->render(destX, destY, sourX, sourY,
		sourWidth, sourHeight, onCamera, opacity);
}

//변형 크기로 잘라서 가져오기 - 키값, 그려줄 x좌표, y좌표, 출력할 너비, 출력할 높이 -> 가져올 곳의 left, top, 가로 크기, 세로 크기, 투명도
void imageManager::render(wstring strKey, float destX, float destY, int showWidth, int showHeight, float sourX, float sourY,
	int sourWidth, int sourHeight, bool onCamera, float opacity)
{
	image* img = findImage(strKey);
	if (img) img->render(destX, destY, showWidth, showHeight, sourX, sourY,
		sourWidth, sourHeight, onCamera, opacity);
}



//원래 크기 프레임렌더 - 키값, 그려줄 x, y 좌표, 가로 방향 프레임 인덱스, 세로 방향 프레임 인덱스, 투명도
void imageManager::frameRender(wstring strKey, float destX, float destY, int currentFrameX, int currentFrameY, bool onCamera, float opacity)
{
	image* img = findImage(strKey);
	if (img) img->frameRender(destX, destY, currentFrameX, currentFrameY, onCamera, opacity);
}

//변형 크기 프레임렌더 - 키값, 그려줄 x, y 좌표, 출력할 너비, 출력할 높이, 가로 방향 프레임 인덱스, 세로 방향 프레임 인덱스, 투명도
void imageManager::frameRender(wstring strKey, float destX, float destY, int showWidth, int showHeight, int currentFrameX, int currentFrameY, bool onCamera, float opacity)
{
	image* img = findImage(strKey);
	if (img) img->frameRender(destX, destY, showWidth, showHeight, currentFrameX, currentFrameY, onCamera, opacity);
}



//루프 렌더 - 수정 필요
/*
void ImageManager::LoopRender(string strKey, const LPRECT drawArea, int offSetX, int offSetY, float opacity)
{
Image* img = FindImage(strKey);
if (img) img->LoopRender(drawArea, offSetX, offSetY, opacity);
}
*/