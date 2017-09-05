#include "stdafx.h"
#include "cameraManager.h"
HRESULT cameraManager::init()
{
	_x = _y = 0;
	_zoomSize = 1.0f;
	_moveZoomX = _moveZoomY = 0;
	return S_OK;
}


void cameraManager::update()
{
	DIRECT2D->_renderTarget->SetTransform
	(
		Matrix3x2F::Translation(0, 0) * Matrix3x2F::Scale(SizeF(_zoomSize, _zoomSize), Point2F(WINSIZEX / 2 + _moveZoomX, WINSIZEY / 2 + _moveZoomY))
	);
}