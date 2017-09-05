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
	�̹��� �߰�
	===================================================================
	*/

	//���ҽ����� �̹��� �߰�
	//Image* AddImage(string strKey, const DWORD resID, int width, int height, bool trans, COLORREF transColor, BOOL useAlphaBlend = false);

	//�Ϲ� �̹��� �߰�
	image* addImage(wstring strKey, LPCWSTR fileName, int width, int height);

	//������ �̹��� �߰�
	image* addFrameImage(wstring strKey, LPCWSTR fileName, int width, int height, int frameNumX, int frameNumY);




	//�̹��� ã�� �Լ�
	image* findImage(wstring strKey);

	BOOL deleteImage(wstring strKey);

	BOOL deleteAll(void);



	/*
	===================================================================
	���� �Լ� - Left Top �������� ����
	===================================================================
	*/


	//�⺻ ���
	void render(wstring strKey, bool onCamera, float opacity);

	//���� ũ�� - Ű��, �׷��� x��ǥ, y��ǥ, ����
	void render(wstring strKey, float destX, float destY, bool onCamera, float opacity);

	//���� ũ�� - Ű��, �׷��� x��ǥ, y��ǥ, ����� �ʺ�, ����� ���̤� ����
	void render(wstring strKey, float destX, float destY, int showWidth, int showHeight, bool onCamera, float opacity);

	//���� ũ��� �߶� �������� - Ű��, �׷��� x��ǥ, y��ǥ -> ������ ���� left, top, ���� ũ��, ���� ũ��, ����
	void render(wstring strKey, float destX, float destY, float sourX, float sourY,
		int sourWidth, int sourHeight, bool onCamera, float opacity);

	//���� ũ��� �߶� �������� - Ű��, �׷��� x��ǥ, y��ǥ, ����� �ʺ�, ����� ���� -> ������ ���� left, top, ���� ũ��, ���� ũ��, ����
	void render(wstring strKey, float destX, float destY, int showWidth, int showHeight, float sourX, float sourY,
		int sourWidth, int sourHeight, bool onCamera, float opacity);





	//���� ũ�� �����ӷ��� - Ű��, �׷��� x, y ��ǥ, ���� ���� ������ �ε���, ���� ���� ������ �ε���, ����
	void frameRender(wstring strKey, float destX, float destY, int currentFrameX, int currentFrameY, bool onCamera,float opacity);

	//���� ũ�� �����ӷ��� - Ű��, �׷��� x, y ��ǥ, ����� �ʺ�, ����� ����, ���� ���� ������ �ε���, ���� ���� ������ �ε���, ����
	void frameRender(wstring strKey, float destX, float destY, int showWidth, int showHeight, int currentFrameX, int currentFrameY, bool onCamera, float opacity);


	//���� ���� - Image::LoopRender ���� �ʿ�
	//void LoopRender(string strKey, const LPRECT drawArea, int offSetX = 0, int offSetY = 0, float opacity = 1.0f);



	imageManager();
	~imageManager();
};

