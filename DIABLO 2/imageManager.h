#pragma once
#include "singletonBase.h"
#include "image.h"
#include <map>

class imageManager : public singletonBase<imageManager>
{
public:
	typedef map<wstring, image*> mapImageList;
	typedef map<wstring, image*>::iterator mapImageIter;

private:
	mapImageList _mImageList;

public:

	void release(void);


	/*
	===================================================================
	이미지 추가
	===================================================================
	*/

	//리소스에서 이미지 추가
	//Image* AddImage(string strKey, const DWORD resID, int width, int height, bool trans, COLORREF transColor, BOOL useAlphaBlend = false);

	//일반 이미지 추가
	image* addImage(wstring strKey, LPCWSTR fileName, int width, int height);

	//프레임 이미지 추가
	image* addFrameImage(wstring strKey, LPCWSTR fileName, int width, int height, int frameNumX, int frameNumY);




	//이미지 찾는 함수
	image* findImage(wstring strKey);

	BOOL deleteImage(wstring strKey);

	BOOL deleteAll(void);



	/*
	===================================================================
	렌더 함수 - Left Top 기준으로 렌더
	===================================================================
	*/


	//기본 출력
	void render(wstring strKey, bool onCamera, float opacity);

	//원래 크기 - 키값, 그려줄 x좌표, y좌표, 투명도
	void render(wstring strKey, float destX, float destY, bool onCamera, float opacity);

	//변형 크기 - 키값, 그려줄 x좌표, y좌표, 출력할 너비, 출력할 높이ㅡ 투명도
	void render(wstring strKey, float destX, float destY, int showWidth, int showHeight, bool onCamera, float opacity);

	//원래 크기로 잘라서 가져오기 - 키값, 그려줄 x좌표, y좌표 -> 가져올 곳의 left, top, 가로 크기, 세로 크기, 투명도
	void render(wstring strKey, float destX, float destY, float sourX, float sourY,
		int sourWidth, int sourHeight, bool onCamera, float opacity);

	//변형 크기로 잘라서 가져오기 - 키값, 그려줄 x좌표, y좌표, 출력할 너비, 출력할 높이 -> 가져올 곳의 left, top, 가로 크기, 세로 크기, 투명도
	void render(wstring strKey, float destX, float destY, int showWidth, int showHeight, float sourX, float sourY,
		int sourWidth, int sourHeight, bool onCamera, float opacity);





	//원래 크기 프레임렌더 - 키값, 그려줄 x, y 좌표, 가로 방향 프레임 인덱스, 세로 방향 프레임 인덱스, 투명도
	void frameRender(wstring strKey, float destX, float destY, int currentFrameX, int currentFrameY, bool onCamera,float opacity);

	//변형 크기 프레임렌더 - 키값, 그려줄 x, y 좌표, 출력할 너비, 출력할 높이, 가로 방향 프레임 인덱스, 세로 방향 프레임 인덱스, 투명도
	void frameRender(wstring strKey, float destX, float destY, int showWidth, int showHeight, int currentFrameX, int currentFrameY, bool onCamera, float opacity);


	//루프 렌더 - Image::LoopRender 수정 필요
	//void LoopRender(string strKey, const LPRECT drawArea, int offSetX = 0, int offSetY = 0, float opacity = 1.0f);



	imageManager();
	~imageManager();
};

