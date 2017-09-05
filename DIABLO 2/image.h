#pragma once
#include "animation.h"

//============================================
//			## D2D Image Class ##
//============================================

class image
{
public:
	// 이미지 정보 구조체
	typedef struct tagImageInfo
	{
		DWORD					resID;
		IWICImagingFactory*		pWICImagingFactory;		//Windows Imaging Component를 위한 Factory 인터페이스
		IWICFormatConverter*	pWICFormatConverter;	//Format Converter
		IWICBitmapDecoder*		pWICDecoder;			//Bitmap Decoder
		IWICBitmapFrameDecode*	pWICFrameDecoder;		//프레임 단위로 디코딩할 Decoder 인터페이스
		ID2D1Bitmap*			pBitmap;				//D2D용 비트맵
		int						width;					//원본 이미지 폭
		int						height;					//원본 이미지 높이
		int						currentFrameX;			//현재 프레임X(번째)
		int						currentFrameY;			//현재 프레임Y(번째)
		int						maxFrameX;				//최대프레임 
		int						maxFrameY;
		int						frameWidth;				//프레임 가로 크기
		int						frameHeight;			//프레임 세로 크기
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
	LPIMAGE_INFO	_imageInfo;		//이미지 구조체
	LPWSTR			_fileName;		//파일 이름 담을 캐릭터포인터


public:
	image();
	~image();


	/*
	===================================================================
	이미지 초기화
	===================================================================
	*/

	//리소스로부터 읽어들여서 쓸 경우
	//HRESULT Init(const DWORD resID, int width, int height);

	// 일반 이미지를 파일에서 가져올 때, 파일명, 원본 너비, 원본 높이
	HRESULT init(LPCWSTR pFileName, int width, int height);

	// 프레임 이미지를 파일에서 가져올 때, 파일명, 원본 너비, 원본 높이, 가로 프레임 수, 세로 프레임 수
	HRESULT init(LPCWSTR pFileName, int width, int height, int frameNumX, int frameNumY);

	// 이미지 릴리즈
	void release(void);


	/*
	===================================================================
	D2D 사용 렌더 함수
	- 카메라 이동 보정 & 범위에 들어가는 내용만 렌더
	- Left Top 기준
	===================================================================
	*/

	//기본 출력
	void render(bool onCamera, float opacity);

	//원래 크기 출력 - 그려줄 x좌표, y좌표, 투명도
	void render(float destX, float destY, bool onCamera, float opacity);

	//변형 크기 출력 - 그려줄 x좌표, y좌표, 출력할 너비, 출력할 높이, 투명도
	void render(float destX, float destY, int showWidth, int showHeight, bool onCamera, float opacity);

	//원래 크기로 잘라서 가져오기 - 그려줄 x좌표, y좌표 -> 가져올 곳의 left, top, 가로 크기, 세로 크기, 투명도
	void render(float destX, float destY, float sourX, float sourY,
		int sourWidth, int sourHeight, bool onCamera, float opacity);

	//변형 크기로 잘라서 가져오기 - 그려줄 x좌표, y좌표, 출력할 너비, 출력할 높이 -> 가져올 곳의 left, top, 가로 크기, 세로 크기, 투명도
	void render(float destX, float destY, int showWidth, int showHeight, float sourX, float sourY,
		int sourWidth, int sourHeight, bool onCamera, float opacity);


	//원래 크기 프레임렌더 - 그려줄 x, y 좌표, 가로 방향 프레임 인덱스, 세로 방향 프레임 인덱스, 투명도
	void frameRender(float destX, float destY, int currentFrameX, int currentFrameY, bool onCamera, float opacity);

	//변형 크기 프레임렌더 - 그려줄 x, y 좌표, 출력할 너비, 출력할 높이, 가로 방향 프레임 인덱스, 세로 방향 프레임 인덱스, 투명도
	void frameRender(float destX, float destY, int showWidth, int showHeight, int currentFrameX, int currentFrameY, bool onCamera, float opacity);

	// 원본 애니 렌더
	void aniRender(float destX, float destY, animation* ani, bool onCamera, float opacity);

	void aniRender(float destX, float destY, int showWidth, int showHeight, animation* ani, bool onCamera, float opacity);

	//루프 렌더 - 수정 필요
	//void LoopRender(const LPRECT drawArea, int offSetX = 0, int offSetY = 0, float opacity = 1.0f);


	/*
	===================================================================
	이미지 정보 Get, Set
	===================================================================
	*/

	//가로 해상도
	inline int getWidth(void) { return _imageInfo->width; }

	//세로 해상도
	inline int getHeight(void) { return _imageInfo->height; }

	//프레임 이미지 정보
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