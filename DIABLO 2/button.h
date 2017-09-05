#pragma once
#include "gameNode.h"

typedef void(*CALLBACK_FUNCTION)(void);

enum BUTTONDIRECTION
{
	BUTTONDIRECTION_NULL,
	BUTTONDIRECTION_UP,
	BUTTONDIRECTION_DOWN
};

class button : public gameNode
{
private:
	BUTTONDIRECTION _direction;

	const WCHAR* _imageName;
	image* _image;
	RECT _rc;
	float _x, _y;

	POINT _btnDownFramePoint;
	POINT _btnUpFramePoint;

	//CALLBACK_FUNCTION _callbackFunction;
	BOOL _push;

public:
	HRESULT init(const WCHAR* imageName, int x, int y,
		POINT btnDownFramePoint, POINT btnUpFramePoint);
	void release(void);
	void update(void);
	void render(void);

	inline BOOL getPush() { return _push; }


	button();
	~button();
};

