#include "stdafx.h"
#include "Image.h"

//pragma == 컴파일러 지시기
#pragma comment(lib, "msimg32.lib")

// Constructor - Member Initializer
image::image()
	: _imageInfo(NULL),
	_fileName(NULL)
{
}


image::~image()
{
}


/*
===================================================================
이미지 초기화
===================================================================
*/
// 파일로부터 읽어들여서 쓸 경우
HRESULT image::init(LPCWSTR pFileName, int width, int height)
{

	if (pFileName == NULL) return E_FAIL;

	if (_imageInfo != NULL) release();

	// 이미지 정보 생성
	HRESULT hr = E_FAIL;
	_imageInfo = new IMAGE_INFO;
	_imageInfo->width = width;
	_imageInfo->height = height;
	_imageInfo->currentFrameX = 0;
	_imageInfo->currentFrameY = 0;
	_imageInfo->frameWidth = width;
	_imageInfo->frameHeight = height;


	// 파일 경로 복사
	int len;
	len = wcslen(pFileName);
	_fileName = new WCHAR[len + 1];
	wcscpy(_fileName, pFileName);


	// WIC를 사용하기 위한 Factory 객체 생성
	hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&_imageInfo->pWICImagingFactory));
	assert(hr == S_OK && "Instance is not created");

	// 디코더 생성
	hr = _imageInfo->pWICImagingFactory->CreateDecoderFromFilename(_fileName, NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand,
		&(_imageInfo->pWICDecoder));
	assert(hr == S_OK && "Decoder is not created");

	// 첫 번째 프레임을 사용할 수 있는 객체 구성
	hr = _imageInfo->pWICDecoder->GetFrame(0, &_imageInfo->pWICFrameDecoder);
	assert(hr == S_OK && "Object is not created");

	// 포맷 컨버터 생성
	hr = _imageInfo->pWICImagingFactory->CreateFormatConverter(&_imageInfo->pWICFormatConverter);
	assert(hr == S_OK && "Converter is not created");

	// 비트맵으로 변환
	hr = _imageInfo->pWICFormatConverter->Initialize(_imageInfo->pWICFrameDecoder, GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeCustom);

	// 변환된 이미지 형식을 사용하여 D2D용 비트맵 생성
	hr = DIRECT2D->_renderTarget->CreateBitmapFromWicBitmap(_imageInfo->pWICFormatConverter, NULL, &_imageInfo->pBitmap);


	// 이미지가 생성되지 않았을 경우
	if (_imageInfo->pBitmap == 0)
	{
		release();
		return E_FAIL;
	}

	return S_OK;
}


/*
===================================================================
프레임 이미지 초기화
===================================================================
*/

// 실제 크기와 동일하게 출력할 때
HRESULT image::init(LPCWSTR pFileName, int width, int height, int frameNumX, int frameNumY)
{
	if (pFileName == NULL) return E_FAIL;

	if (_imageInfo != NULL) release();

	// 이미지 정보 생성
	HRESULT hr = E_FAIL;
	_imageInfo = new IMAGE_INFO;
	_imageInfo->width = width;
	_imageInfo->height = height;
	_imageInfo->currentFrameX = 0;
	_imageInfo->currentFrameY = 0;
	_imageInfo->maxFrameX = frameNumX - 1;
	_imageInfo->maxFrameY = frameNumY - 1;
	_imageInfo->frameWidth = width / frameNumX;
	_imageInfo->frameHeight = height / frameNumY;

	// 파일 경로 복사
	int len;
	len = wcslen(pFileName);
	_fileName = new WCHAR[len + 1];
	wcscpy(_fileName, pFileName);


	// WIC를 사용하기 위한 Factory 객체 생성
	hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&_imageInfo->pWICImagingFactory));
	assert(hr == S_OK && "Instance is not created");

	// 디코더 생성
	hr = _imageInfo->pWICImagingFactory->CreateDecoderFromFilename(_fileName, NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand,
		&(_imageInfo->pWICDecoder));
	assert(hr == S_OK && "Decoter is not created");

	// 첫 번째 프레임을 사용할 수 있는 객체 구성
	hr = _imageInfo->pWICDecoder->GetFrame(0, &_imageInfo->pWICFrameDecoder);
	assert(hr == S_OK && "Decoder is not created");

	// 포맷 컨버터 생성
	hr = _imageInfo->pWICImagingFactory->CreateFormatConverter(&_imageInfo->pWICFormatConverter);
	assert(hr == S_OK && "Converter is not created");

	// 비트맵으로 변환
	hr = _imageInfo->pWICFormatConverter->Initialize(_imageInfo->pWICFrameDecoder, GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeCustom);

	// 변환된 이미지 형식을 사용하여 D2D용 비트맵 생성
	hr = DIRECT2D->_renderTarget->CreateBitmapFromWicBitmap(_imageInfo->pWICFormatConverter, NULL, &_imageInfo->pBitmap);

	
	// 이미지가 생성되지 않았을 경우
	if (_imageInfo->pBitmap == 0)
	{
		release();
		return E_FAIL;
	}

	return S_OK;
}



// 릴리즈
void image::release(void)
{
	SAFE_DELETE(_imageInfo);
	SAFE_DELETE(_fileName);
}


/*
===================================================================
D2D 사용 렌더 함수
- 카메라 이동 보정 & 범위에 들어가는 내용만 렌더
- Left Top 기준
===================================================================
*/


//기본 출력
void image::render(bool onCamera, float opacity)
{
	float posX;
	float posY;

	if (onCamera)
	{
		posX = 0 - CAMERAMANAGER->getX();
		posY = 0 - CAMERAMANAGER->getY();
	}
	else
	{
		posX = 0;
		posY = 0;
	}

	if (_imageInfo->pBitmap != NULL)
	{
		if (posX + _imageInfo->width < 0) return;
		if (posY + _imageInfo->height< 0) return;
		if (posX > WINSIZEX) return;
		if (posY > WINSIZEY) return;

		D2D1_RECT_F dxArea = RectF(posX, posY, posX + _imageInfo->width, posY + _imageInfo->height);
		D2D1_RECT_F dxArea2 = RectF(0, 0, _imageInfo->width, _imageInfo->height);
		DIRECT2D->_renderTarget->DrawBitmap(_imageInfo->pBitmap, dxArea, opacity,
			D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, dxArea2);
	}
}


//원래 크기 출력
void image::render(float destX, float destY, bool onCamera, float opacity)
{
	float posX = destX;
	float posY = destY;

	if (onCamera)
	{
		posX = destX - CAMERAMANAGER->getX();
		posY = destY - CAMERAMANAGER->getY();
	}
	else
	{
		posX = destX;
		posY = destY;
	}

	if (_imageInfo->pBitmap != NULL)
	{
		if (posX + _imageInfo->width < 0) return;
		if (posY + _imageInfo->height< 0) return;
		if (posX > WINSIZEX) return;
		if (posY > WINSIZEY) return;


		D2D1_RECT_F dxArea = RectF(posX, posY, posX + _imageInfo->width, posY + _imageInfo->height);
		D2D1_RECT_F dxArea2 = RectF(0, 0, _imageInfo->width, _imageInfo->height);
		DIRECT2D->_renderTarget->DrawBitmap(_imageInfo->pBitmap, dxArea, opacity,
			D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, dxArea2);

	}
}

//변형 크기 출력
void image::render(float destX, float destY, int showWidth, int showHeight, bool onCamera, float opacity)
{
	float posX;
	float posY;

	if (onCamera)
	{
		posX = destX - CAMERAMANAGER->getX();
		posY = destY - CAMERAMANAGER->getY();
	}
	else
	{
		posX = destX;
		posY = destY;
	}


	if (_imageInfo->pBitmap != NULL)
	{
		if (posX + showWidth < 0) return;
		if (posY + showHeight < 0) return;
		if (posX > WINSIZEX) return;
		if (posY > WINSIZEY) return;

		D2D1_RECT_F dxArea = RectF(posX, posY, posX + showWidth, posY + showHeight);
		D2D1_RECT_F dxArea2 = RectF(0, 0, _imageInfo->width, _imageInfo->height);
		DIRECT2D->_renderTarget->DrawBitmap(_imageInfo->pBitmap, dxArea, opacity,
			D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, dxArea2);
	}
}

//원래 크기로 잘라서 출력
void image::render(float destX, float destY, float sourX, float sourY, int sourWidth, int sourHeight, bool onCamera, float opacity)
{
	float posX;
	float posY;

	if (onCamera)
	{
		posX = destX - CAMERAMANAGER->getX();
		posY = destY - CAMERAMANAGER->getY();
	}
	else
	{
		posX = destX;
		posY = destY;
	}

	if (_imageInfo->pBitmap != NULL)
	{
		if (posX + sourWidth < 0) return;
		if (posY + sourHeight < 0) return;
		if (posX > WINSIZEX) return;
		if (posY > WINSIZEY) return;

		
		D2D1_RECT_F dxArea = RectF(posX, posY, posX + sourWidth, posY + sourHeight);
		D2D1_RECT_F dxArea2 = RectF(sourX, sourY, sourX + sourWidth, sourY + sourHeight);
		DIRECT2D->_renderTarget->DrawBitmap(_imageInfo->pBitmap, dxArea, opacity,
			D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, dxArea2);
	}

}

//변형 크기로 잘라서 출력
void image::render(float destX, float destY, int showWidth, int showHeight, float sourX, float sourY, int sourWidth, int sourHeight, bool onCamera, float opacity)
{
	float posX;
	float posY;

	if (onCamera)
	{
		posX = destX - CAMERAMANAGER->getX();
		posY = destY - CAMERAMANAGER->getY();
	}
	else
	{
		posX = destX;
		posY = destY;
	}

	if (_imageInfo->pBitmap != NULL)
	{
		if (posX + showWidth < 0) return;
		if (posY + showHeight < 0) return;
		if (posX > WINSIZEX) return;
		if (posY > WINSIZEY) return;

		D2D1_RECT_F dxArea = RectF(posX, posY, posX + showWidth, posY + showHeight);
		D2D1_RECT_F dxArea2 = RectF(sourX, sourY, sourX + sourWidth, sourY + sourHeight);
		DIRECT2D->_renderTarget->DrawBitmap(_imageInfo->pBitmap, dxArea, opacity,
			D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, dxArea2);
	}

}




/*
===================================================================
프레임 이미지 렌더 함수
- 카메라 이동 보정 & 범위에 들어가는 내용만 렌더
- Left Top 기준
===================================================================
*/

//원래 크기 프레임렌더
void image::frameRender(float destX, float destY, int currentFrameX, int currentFrameY, bool onCamera, float opacity)
{
	float posX;
	float posY;

	if (onCamera)
	{
		posX = destX - CAMERAMANAGER->getX();
		posY = destY - CAMERAMANAGER->getY();
	}
	else
	{
		posX = destX;
		posY = destY;
	}

	if (_imageInfo->pBitmap != NULL)
	{
		if (posX + _imageInfo->frameWidth < 0) return;
		if (posY + _imageInfo->frameHeight < 0) return;
		if (posX > WINSIZEX) return;
		if (posY > WINSIZEY) return;

		D2D1_RECT_F dxArea = RectF(posX, posY, posX + _imageInfo->frameWidth, posY + _imageInfo->frameHeight);
		D2D1_RECT_F dxArea2 = RectF(currentFrameX * _imageInfo->frameWidth, currentFrameY * _imageInfo->frameHeight,
			(currentFrameX + 1) * _imageInfo->frameWidth, (currentFrameY + 1) * _imageInfo->frameHeight);
		DIRECT2D->_renderTarget->DrawBitmap(_imageInfo->pBitmap, dxArea, opacity,
			D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, dxArea2);
	}
}

//변형 크기 프레임렌더
void image::frameRender(float destX, float destY, int showWidth, int showHeight, int currentFrameX, int currentFrameY, bool onCamera, float opacity)
{
	float posX;
	float posY;

	if (onCamera)
	{
		posX = destX - CAMERAMANAGER->getX();
		posY = destY - CAMERAMANAGER->getY();
	}
	else
	{
		posX = destX;
		posY = destY;
	}

	if (_imageInfo->pBitmap != NULL)
	{		
		if (posX + showWidth < 0) return;
		if (posY + showHeight < 0) return;
		if (posX > WINSIZEX) return;
		if (posY > WINSIZEY) return;

		D2D1_RECT_F dxArea = RectF(posX, posY, posX + showWidth, posY + showHeight);
		D2D1_RECT_F dxArea2 = RectF(currentFrameX * _imageInfo->frameWidth, currentFrameY * _imageInfo->frameHeight,
			(currentFrameX + 1) * _imageInfo->frameWidth, (currentFrameY + 1) * _imageInfo->frameHeight);
		DIRECT2D->_renderTarget->DrawBitmap(_imageInfo->pBitmap, dxArea, opacity,
			D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, dxArea2);
	}
}


void image::aniRender(float destX, float destY, animation* ani, bool onCamera, float opacity)
{
	this->render(destX, destY, ani->getFramePos().x, ani->getFramePos().y, ani->getFrameWidth(), ani->getFrameHeight(), onCamera, opacity);
}

void image::aniRender(float destX, float destY, int showWidth, int showHeight, animation* ani, bool onCamera, float opacity)
{
	this->render(destX, destY, showWidth, showHeight, ani->getFramePos().x, ani->getFramePos().y, ani->getFrameWidth(), ani->getFrameHeight(), onCamera, opacity);
}

/*
===================================================================
루프 렌더 함수
- 카메라 이동 보정 & 범위에 들어가는 내용만 렌더
- Left Top 기준
###	수정 필요 ###
===================================================================
*/

/*
void Image::LoopRender(const LPRECT drawArea, int offSetX, int offSetY, float opacity)
{
//offSet값이 음수일 경우 다시 재보정
if (offSetX < 0) offSetX = imageInfo->width + (offSetX % imageInfo->width);
if (offSetY < 0) offSetY = imageInfo->height + (offSetY % imageInfo->height);

//그려지는 소스의 영역(이미지) 셋팅
RECT rcSour;
int sourWidth;
int sourHeight;

//그려지는 영역
RECT rcDest;

int drawAreaX = drawArea->left;
int drawAreaY = drawArea->top;
int drawAreaW = drawArea->right - drawArea->left;
int drawAreaH = drawArea->bottom - drawArea->top;

//세로 먼저
for (int y = 0; y < drawAreaH; y += sourHeight)
{
//이미지 소스의 높이 계산
rcSour.top = (y + offSetY) % imageInfo->height;
rcSour.bottom = imageInfo->height;
sourHeight = rcSour.bottom - rcSour.top;

//소스의 영역이 드로우 에어리어(그려주는 영역) 밖으로 나가면
if (y + sourHeight > drawAreaH)
{
//넘어간 그림의 값만큼 밑으로 보낸다
rcSour.bottom -= (y + sourHeight) - drawAreaH;
sourHeight = rcSour.bottom - rcSour.top;
}

rcDest.top = y + drawAreaY;
rcDest.bottom = rcDest.top + sourHeight;

//가로 루프는 안에다가!
for (int x = 0; x < drawAreaW; x += sourWidth)
{
//소스의 영역 가로 계산
rcSour.left = (x + offSetX) % imageInfo->width;
rcSour.right = imageInfo->width;
sourWidth = rcSour.right - rcSour.left;

if (x + sourWidth > drawAreaW)
{
rcSour.right -= (x + sourWidth) - drawAreaW;
sourWidth = rcSour.right - rcSour.left;
}

//그려지는 영역
rcDest.left = x + drawAreaX;
rcDest.right = rcDest.left + sourWidth;

//그려준다
Render(rcDest.left, rcDest.top,
rcSour.left, rcSour.top,
rcSour.right - rcSour.left,
rcSour.bottom - rcSour.top, opacity);
}
}
}

*/