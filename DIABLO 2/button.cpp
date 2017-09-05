#include "stdafx.h"
#include "button.h"


button::button()
{
}


button::~button()
{
}

HRESULT button::init(const WCHAR* imageName, int x, int y,
	POINT btnDownFramePoint, POINT btnUpFramePoint)
{
	//_callbackFunction = static_cast<CALLBACK_FUNCTION>(cbFunction);
	_push = false;
	_direction = BUTTONDIRECTION_NULL;

	_x = x;
	_y = y;

	_btnUpFramePoint = btnUpFramePoint;
	_btnDownFramePoint = btnDownFramePoint;

	_imageName = imageName;
	_image = IMAGEMANAGER->findImage(imageName);

	_rc = RectMakeCenter(x, y, _image->getFrameWidth(), _image->getFrameHeight());


	return S_OK;
}

void button::release(void)
{

}

void button::update(void)
{
	if (PtInRect(&_rc, _ptMouse))
	{
		if (_leftButtonDown)
		{
			_direction = BUTTONDIRECTION_DOWN;
			
		}
		else if (!_leftButtonDown && _direction == BUTTONDIRECTION_DOWN)
		{
			_direction = BUTTONDIRECTION_UP;
			_push = true;
			
		}
	}
	else
	{
		_direction = BUTTONDIRECTION_NULL;
		_push = false;
	}
}

void button::render(void)
{
	switch (_direction)
	{
		case BUTTONDIRECTION_NULL: case BUTTONDIRECTION_UP:
			_image->frameRender(_rc.left, _rc.top,
				_btnUpFramePoint.x, _btnUpFramePoint.y, false, 1.0f);
		break;
		case BUTTONDIRECTION_DOWN:
			_image->frameRender(_rc.left, _rc.top,
				_btnDownFramePoint.x, _btnDownFramePoint.y, false, 1.0f);
		break;
	}
}