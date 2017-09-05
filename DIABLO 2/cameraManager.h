#pragma once
#include "singletonBase.h"

class cameraManager : public singletonBase<cameraManager>
{
private:
	float _x;
	float _y;
	float _zoomSize;
	float _moveZoomX;
	float _moveZoomY;

public:
	cameraManager() {};
	~cameraManager() {};


	HRESULT init();
	void update();


	inline float getX() { return _x; }
	inline float getY() { return _y; }
	inline void setX(float x) { _x = x; }
	inline void setY(float y) { _y = y; }
	inline void setPosition(float x, float y)
	{
		_x = x;
		_y = y;
	}


	inline float getZoomX() { return _moveZoomX; }
	inline float getZoomY() { return _moveZoomY; }
	inline void setZoomX(float x) { _moveZoomX = x; }
	inline void setZoomY(float y) { _moveZoomY = y; }

	inline float getZoomSize() { return _zoomSize; }
	inline void setZoomSize(float size) { _zoomSize = size; }


};

