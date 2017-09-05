#include "stdafx.h"
#include "Image.h"

//pragma == �����Ϸ� ���ñ�
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
�̹��� �ʱ�ȭ
===================================================================
*/
// ���Ϸκ��� �о�鿩�� �� ���
HRESULT image::init(LPCWSTR pFileName, int width, int height)
{

	if (pFileName == NULL) return E_FAIL;

	if (_imageInfo != NULL) release();

	// �̹��� ���� ����
	HRESULT hr = E_FAIL;
	_imageInfo = new IMAGE_INFO;
	_imageInfo->width = width;
	_imageInfo->height = height;
	_imageInfo->currentFrameX = 0;
	_imageInfo->currentFrameY = 0;
	_imageInfo->frameWidth = width;
	_imageInfo->frameHeight = height;


	// ���� ��� ����
	int len;
	len = wcslen(pFileName);
	_fileName = new WCHAR[len + 1];
	wcscpy(_fileName, pFileName);


	// WIC�� ����ϱ� ���� Factory ��ü ����
	hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&_imageInfo->pWICImagingFactory));
	assert(hr == S_OK && "Instance is not created");

	// ���ڴ� ����
	hr = _imageInfo->pWICImagingFactory->CreateDecoderFromFilename(_fileName, NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand,
		&(_imageInfo->pWICDecoder));
	assert(hr == S_OK && "Decoder is not created");

	// ù ��° �������� ����� �� �ִ� ��ü ����
	hr = _imageInfo->pWICDecoder->GetFrame(0, &_imageInfo->pWICFrameDecoder);
	assert(hr == S_OK && "Object is not created");

	// ���� ������ ����
	hr = _imageInfo->pWICImagingFactory->CreateFormatConverter(&_imageInfo->pWICFormatConverter);
	assert(hr == S_OK && "Converter is not created");

	// ��Ʈ������ ��ȯ
	hr = _imageInfo->pWICFormatConverter->Initialize(_imageInfo->pWICFrameDecoder, GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeCustom);

	// ��ȯ�� �̹��� ������ ����Ͽ� D2D�� ��Ʈ�� ����
	hr = DIRECT2D->_renderTarget->CreateBitmapFromWicBitmap(_imageInfo->pWICFormatConverter, NULL, &_imageInfo->pBitmap);


	// �̹����� �������� �ʾ��� ���
	if (_imageInfo->pBitmap == 0)
	{
		release();
		return E_FAIL;
	}

	return S_OK;
}


/*
===================================================================
������ �̹��� �ʱ�ȭ
===================================================================
*/

// ���� ũ��� �����ϰ� ����� ��
HRESULT image::init(LPCWSTR pFileName, int width, int height, int frameNumX, int frameNumY)
{
	if (pFileName == NULL) return E_FAIL;

	if (_imageInfo != NULL) release();

	// �̹��� ���� ����
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

	// ���� ��� ����
	int len;
	len = wcslen(pFileName);
	_fileName = new WCHAR[len + 1];
	wcscpy(_fileName, pFileName);


	// WIC�� ����ϱ� ���� Factory ��ü ����
	hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&_imageInfo->pWICImagingFactory));
	assert(hr == S_OK && "Instance is not created");

	// ���ڴ� ����
	hr = _imageInfo->pWICImagingFactory->CreateDecoderFromFilename(_fileName, NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand,
		&(_imageInfo->pWICDecoder));
	assert(hr == S_OK && "Decoter is not created");

	// ù ��° �������� ����� �� �ִ� ��ü ����
	hr = _imageInfo->pWICDecoder->GetFrame(0, &_imageInfo->pWICFrameDecoder);
	assert(hr == S_OK && "Decoder is not created");

	// ���� ������ ����
	hr = _imageInfo->pWICImagingFactory->CreateFormatConverter(&_imageInfo->pWICFormatConverter);
	assert(hr == S_OK && "Converter is not created");

	// ��Ʈ������ ��ȯ
	hr = _imageInfo->pWICFormatConverter->Initialize(_imageInfo->pWICFrameDecoder, GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeCustom);

	// ��ȯ�� �̹��� ������ ����Ͽ� D2D�� ��Ʈ�� ����
	hr = DIRECT2D->_renderTarget->CreateBitmapFromWicBitmap(_imageInfo->pWICFormatConverter, NULL, &_imageInfo->pBitmap);

	
	// �̹����� �������� �ʾ��� ���
	if (_imageInfo->pBitmap == 0)
	{
		release();
		return E_FAIL;
	}

	return S_OK;
}



// ������
void image::release(void)
{
	SAFE_DELETE(_imageInfo);
	SAFE_DELETE(_fileName);
}


/*
===================================================================
D2D ��� ���� �Լ�
- ī�޶� �̵� ���� & ������ ���� ���븸 ����
- Left Top ����
===================================================================
*/


//�⺻ ���
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


//���� ũ�� ���
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

//���� ũ�� ���
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

//���� ũ��� �߶� ���
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

//���� ũ��� �߶� ���
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
������ �̹��� ���� �Լ�
- ī�޶� �̵� ���� & ������ ���� ���븸 ����
- Left Top ����
===================================================================
*/

//���� ũ�� �����ӷ���
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

//���� ũ�� �����ӷ���
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
���� ���� �Լ�
- ī�޶� �̵� ���� & ������ ���� ���븸 ����
- Left Top ����
###	���� �ʿ� ###
===================================================================
*/

/*
void Image::LoopRender(const LPRECT drawArea, int offSetX, int offSetY, float opacity)
{
//offSet���� ������ ��� �ٽ� �纸��
if (offSetX < 0) offSetX = imageInfo->width + (offSetX % imageInfo->width);
if (offSetY < 0) offSetY = imageInfo->height + (offSetY % imageInfo->height);

//�׷����� �ҽ��� ����(�̹���) ����
RECT rcSour;
int sourWidth;
int sourHeight;

//�׷����� ����
RECT rcDest;

int drawAreaX = drawArea->left;
int drawAreaY = drawArea->top;
int drawAreaW = drawArea->right - drawArea->left;
int drawAreaH = drawArea->bottom - drawArea->top;

//���� ����
for (int y = 0; y < drawAreaH; y += sourHeight)
{
//�̹��� �ҽ��� ���� ���
rcSour.top = (y + offSetY) % imageInfo->height;
rcSour.bottom = imageInfo->height;
sourHeight = rcSour.bottom - rcSour.top;

//�ҽ��� ������ ��ο� �����(�׷��ִ� ����) ������ ������
if (y + sourHeight > drawAreaH)
{
//�Ѿ �׸��� ����ŭ ������ ������
rcSour.bottom -= (y + sourHeight) - drawAreaH;
sourHeight = rcSour.bottom - rcSour.top;
}

rcDest.top = y + drawAreaY;
rcDest.bottom = rcDest.top + sourHeight;

//���� ������ �ȿ��ٰ�!
for (int x = 0; x < drawAreaW; x += sourWidth)
{
//�ҽ��� ���� ���� ���
rcSour.left = (x + offSetX) % imageInfo->width;
rcSour.right = imageInfo->width;
sourWidth = rcSour.right - rcSour.left;

if (x + sourWidth > drawAreaW)
{
rcSour.right -= (x + sourWidth) - drawAreaW;
sourWidth = rcSour.right - rcSour.left;
}

//�׷����� ����
rcDest.left = x + drawAreaX;
rcDest.right = rcDest.left + sourWidth;

//�׷��ش�
Render(rcDest.left, rcDest.top,
rcSour.left, rcSour.top,
rcSour.right - rcSour.left,
rcSour.bottom - rcSour.top, opacity);
}
}
}

*/