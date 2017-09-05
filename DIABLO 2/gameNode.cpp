#include "stdafx.h"
#include "gameNode.h"


gameNode::gameNode()
{
}


gameNode::~gameNode()
{
}

HRESULT gameNode::init(void)
{
	
	_managerInit = false;

	return S_OK;
}

HRESULT gameNode::init(bool managerInit)
{
	_leftButtonDown = false;
	_managerInit = managerInit;

	if (_managerInit)
	{
		DIRECT2D->init();
		KEYMANAGER->init();
		TIMEMANAGER->init();
		EFFECTMANAGER->init();
		SOUNDMANAGER->init();
		SCENEMANAGER->init();
		TXTDATA->init();
		DATABASE->init();
		CAMERAMANAGER->init();
		ASTARMANAGER->init();
	}

	return S_OK;
}

void gameNode::release(void)
{
	if (_managerInit)
	{
		SOUNDMANAGER->release();
		TIMEMANAGER->release();
		DIRECT2D->release();

		
		DIRECT2D->releaseSingleton();
		KEYMANAGER->releaseSingleton();
		IMAGEMANAGER->releaseSingleton();
		TIMEMANAGER->releaseSingleton();
		EFFECTMANAGER->releaseSingleton();
		SOUNDMANAGER->releaseSingleton();
		SCENEMANAGER->releaseSingleton();
		TXTDATA->releaseSingleton();
		DATABASE->releaseSingleton();
		CAMERAMANAGER->releaseSingleton();
		ASTARMANAGER->releaseSingleton();
	}
	
}

void gameNode::update(void)
{
	SOUNDMANAGER->update();
	CAMERAMANAGER->update();
}

void gameNode::render(void)
{

}


LRESULT gameNode::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	
	switch (iMessage)
	{

		//마우스 좌표를 읽어오는 함수
		case WM_MOUSEMOVE:
			_ptMouse.x = static_cast<float>LOWORD(lParam);
			_ptMouse.y = static_cast<float>HIWORD(lParam);

		break;

		case WM_KEYDOWN:
			switch (wParam)
			{
				case VK_ESCAPE:
					PostMessage(hWnd, WM_DESTROY, 0, 0);
				break;
			
			}
		break;

		case WM_LBUTTONDOWN:
			_leftButtonDown = true;
			break;
		case WM_LBUTTONUP:
			_leftButtonDown = false;
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
		break;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}