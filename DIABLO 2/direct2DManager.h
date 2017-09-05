#pragma once
#include "singletonBase.h"


class direct2DManager : public singletonBase<direct2DManager>
{
public:

	ID2D1Factory*				_direct2DFactory;		//���丮�� �����ؾ� �׸����� ������ �� �� �ִ�.
	ID2D1HwndRenderTarget*		_renderTarget;			//�����쿡 �׸��� ���� HwndRender���ٰ� Ÿ���� ��´�
	IDWriteFactory*				_writeFactory;			//�ؽ�Ʈ�� ���� ���� ���丮 ����
	IDWriteTextFormat*			_defaultTextFormat;		//�⺻ �ؽ�Ʈ ����
	IDWriteTextFormat*			_customTextFormat;		//Ŀ���� �ؽ�Ʈ ����
	ID2D1SolidColorBrush*		_defaultBrush;			//�⺻ ������ �귯��

	

	HRESULT init();
	void release();
	void beginDraw();									//���� ������ ó���� �־��ش�.
	void endDraw();										//���� ������ ���߿� �־��ش�.
	ID2D1SolidColorBrush* createBrush(COLORREF rgb, float opacity = 1.0f);


	void drawTextD2D
	(
		ID2D1SolidColorBrush* brush,								// ���ڻ�
		LPCWSTR string,												// ���ڿ�
		float startX, float startY, float endX, float endY			// �ؽ�Ʈ ��Ʈ�ڽ�
	);

	
	void drawTextD2D
	(
		ID2D1SolidColorBrush* brush,								// ���ڻ�
		LPCWSTR fontName,											// ��Ʈ�̸�
		float fontSize, LPCWSTR string,								// ��Ʈ ũ��, ���ڿ�
		float startX, float startY, float endX, float endY			// �ؽ�Ʈ ��Ʈ�ڽ�
	);


	void drawLine(ID2D1SolidColorBrush* brush, float startX, float startY, float endX, float endY, bool onCamera, float strokeWidth = 1.0f);
	void drawRectangle(ID2D1SolidColorBrush* brush, float startX, float startY, float endX, float endY, bool onCamera, float strokeWidth = 1.0f);
	void drawEllipse(ID2D1SolidColorBrush* brush, float startX, float startY, float endX, float endY, bool onCamera, float strokeWidth = 1.0f);
	void fillRectangle(ID2D1SolidColorBrush* brush, float startX, float startY, float endX, float endY, bool onCamera);
	void fillEllipse(ID2D1SolidColorBrush* brush, float startX, float startY, float endX, float endY, bool onCamera);

	void reCreateResource();
	void ResizeWindowSize(UINT uWidth, UINT uHeight);

	direct2DManager();
	~direct2DManager();
};

