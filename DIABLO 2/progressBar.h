#pragma once
#include "gameNode.h"

class progressBar : public gameNode
{
private:
	RECT _rcProgress;
	int _x, _y;
	float _width;

	image* _progressBarTop;
	image* _progressBarBottom;


public:
	HRESULT init(int x, int y, int width, int height);
	HRESULT init(WCHAR* topImage, WCHAR* bottomImage, float x, float y, int width, int height);
	void release();
	void update();
	void render(bool onCamera, float opacity);

	void setGauge(float currentGauge, float maxGauge);

	//게이지 바 위치 조절 함쑤
	void setX(int x) { _x = x; }
	void setY(int y) { _y = y; }

	progressBar();
	~progressBar();
};

