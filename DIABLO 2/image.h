#pragma once
#include "animation.h"

//============================================
//			## D2D Image Class ##
//============================================

class image
{
public:
	// �̹��� ���� ����ü
	typedef struct tagImageInfo
	{
		DWORD					resID;
		IWICImagingFactory*		pWICImagingFactory;		//Windows Imaging Component�� ���� Factory �������̽�
		IWICFormatConverter*	pWICFormatConverter;	//Format Converter
		IWICBitmapDecoder*		pWICDecoder;			//Bitmap Decoder
		IWICBitmapFrameDecode*	pWICFrameDecoder;		//������ ������ ���ڵ��� Decoder �������̽�
		ID2D1Bitmap*			pBitmap;				//D2D�� ��Ʈ��
		int						width;					//���� �̹��� ��
		int						height;					//���� �̹��� ����
		int						currentFrameX;			//���� ������X(��°)
		int						currentFrameY;			//���� ������Y(��°)
		int						maxFrameX;				//�ִ������� 
		int						maxFrameY;
		int						frameWidth;				//������ ���� ũ��
		int						frameHeight;			//������ ���� ũ��
		tagImageInfo()
		{
			resID = 0;
			pWICImagingFactory = nullptr;
			pWICFormatConverter = nullptr;
			pWICDecoder = nullptr;
			pWICFrameDecoder = nullptr;
			pBitmap = nullptr;
			width = 0;
			height = 0;
			currentFrameX = 0;
			currentFrameY = 0;
			maxFrameX = 0;
			maxFrameY = 0;
			frameWidth = 0;
			frameHeight = 0;
		}
	}IMAGE_INFO, *LPIMAGE_INFO;

private:
	LPIMAGE_INFO	_imageInfo;		//�̹��� ����ü
	LPWSTR			_fileName;		//���� �̸� ���� ĳ����������


public:
	image();
	~image();


	/*
	===================================================================
	�̹��� �ʱ�ȭ
	===================================================================
	*/

	//���ҽ��κ��� �о�鿩�� �� ���
	//HRESULT Init(const DWORD resID, int width, int height);

	// �Ϲ� �̹����� ���Ͽ��� ������ ��, ���ϸ�, ���� �ʺ�, ���� ����
	HRESULT init(LPCWSTR pFileName, int width, int height);

	// ������ �̹����� ���Ͽ��� ������ ��, ���ϸ�, ���� �ʺ�, ���� ����, ���� ������ ��, ���� ������ ��
	HRESULT init(LPCWSTR pFileName, int width, int height, int frameNumX, int frameNumY);

	// �̹��� ������
	void release(void);


	/*
	===================================================================
	D2D ��� ���� �Լ�
	- ī�޶� �̵� ���� & ������ ���� ���븸 ����
	- Left Top ����
	===================================================================
	*/

	//�⺻ ���
	void render(bool onCamera, float opacity);

	//���� ũ�� ��� - �׷��� x��ǥ, y��ǥ, ����
	void render(float destX, float destY, bool onCamera, float opacity);

	//���� ũ�� ��� - �׷��� x��ǥ, y��ǥ, ����� �ʺ�, ����� ����, ����
	void render(float destX, float destY, int showWidth, int showHeight, bool onCamera, float opacity);

	//���� ũ��� �߶� �������� - �׷��� x��ǥ, y��ǥ -> ������ ���� left, top, ���� ũ��, ���� ũ��, ����
	void render(float destX, float destY, float sourX, float sourY,
		int sourWidth, int sourHeight, bool onCamera, float opacity);

	//���� ũ��� �߶� �������� - �׷��� x��ǥ, y��ǥ, ����� �ʺ�, ����� ���� -> ������ ���� left, top, ���� ũ��, ���� ũ��, ����
	void render(float destX, float destY, int showWidth, int showHeight, float sourX, float sourY,
		int sourWidth, int sourHeight, bool onCamera, float opacity);


	//���� ũ�� �����ӷ��� - �׷��� x, y ��ǥ, ���� ���� ������ �ε���, ���� ���� ������ �ε���, ����
	void frameRender(float destX, float destY, int currentFrameX, int currentFrameY, bool onCamera, float opacity);

	//���� ũ�� �����ӷ��� - �׷��� x, y ��ǥ, ����� �ʺ�, ����� ����, ���� ���� ������ �ε���, ���� ���� ������ �ε���, ����
	void frameRender(float destX, float destY, int showWidth, int showHeight, int currentFrameX, int currentFrameY, bool onCamera, float opacity);

	// ���� �ִ� ����
	void aniRender(float destX, float destY, animation* ani, bool onCamera, float opacity);

	void aniRender(float destX, float destY, int showWidth, int showHeight, animation* ani, bool onCamera, float opacity);

	//���� ���� - ���� �ʿ�
	//void LoopRender(const LPRECT drawArea, int offSetX = 0, int offSetY = 0, float opacity = 1.0f);


	/*
	===================================================================
	�̹��� ���� Get, Set
	===================================================================
	*/

	//���� �ػ�
	inline int getWidth(void) { return _imageInfo->width; }

	//���� �ػ�
	inline int getHeight(void) { return _imageInfo->height; }

	//������ �̹��� ����
	inline void setFrameX(int frameX)
	{
		_imageInfo->currentFrameX = frameX;
		if (frameX > _imageInfo->maxFrameX) _imageInfo->currentFrameX = _imageInfo->maxFrameX;
	}

	inline void setFrameY(int frameY)
	{
		_imageInfo->currentFrameY = frameY;
		if (frameY > _imageInfo->maxFrameY) _imageInfo->currentFrameY = _imageInfo->maxFrameY;
	}

	inline int getMaxFrameX(void) { return _imageInfo->maxFrameX; }
	inline int getMaxFrameY(void) { return _imageInfo->maxFrameY; }

	inline int getFrameX(void) { return _imageInfo->currentFrameX; }
	inline int getFrameY(void) { return _imageInfo->currentFrameY; }

	inline int getFrameWidth(void) { return _imageInfo->frameWidth; }
	inline int getFrameHeight(void) { return _imageInfo->frameHeight; }

};