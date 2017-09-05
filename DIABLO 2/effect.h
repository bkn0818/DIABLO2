#pragma once
#include "gameNode.h"

class animation;

class effect : public gameNode
{
protected:
	int				_x;					//이펙트 좌표위치 Left
	int				_y;					//이펙트 좌표위치 top
		
	image*			_effectImage;		//이펙트 이미지
	animation*		_effectAnimation;	//이펙트 애니메이션
	BOOL			_isRunning;			//이펙트 활성화 여부
	float			_elapsedTime;		//이펙트 경과시간

public:

	virtual HRESULT init(image* effectImage, int frameW, int frameH, int fps, float elapsedTime);
	virtual void release(void);
	virtual void update(void);
	virtual void render(bool onCamera, float opacity);

	void startEffect(int x, int y);

	virtual void killEffect(void);

	BOOL getIsRunning(void) { return _isRunning; }

	effect();
	~effect();
};

