#include "stdafx.h"
#include "effect.h"
#include "animation.h"


effect::effect()
	: _effectImage(NULL),
	_effectAnimation(NULL),
	_isRunning(false),
	_x(0), _y(0)
{
}


effect::~effect()
{

}

HRESULT effect::init(image* effectImage, int frameW, int frameH, int fps, float elapsedTime)
{
	//이펙트 이미지가 없다면 실행하지 말어라
	if (!effectImage) return E_FAIL;

	_isRunning = false;
	_effectImage = effectImage;
	_elapsedTime = elapsedTime;

	if (!_effectAnimation) _effectAnimation = new animation;

	_effectAnimation->init(_effectImage->getWidth(), _effectImage->getHeight(), frameW, frameH);
	_effectAnimation->setDefPlayFrame(false, false);
	_effectAnimation->setFPS(fps);
	_effectAnimation->stop();

	return S_OK;
}

void effect::release(void)
{
	_effectImage = NULL;
	SAFE_DELETE(_effectAnimation);
}

void effect::update(void) 
{
	//실행중이지 않다면 돌려보내고
	if (!_isRunning) return;

	//애니메이션 프레임 업데이트
	_effectAnimation->frameUpdate(_elapsedTime);

	//플레이 상태가 아니면			꺼준다
	if (!_effectAnimation->isPlay()) killEffect();

}

void effect::render(bool onCamera, float opacity) 
{
	if (!_isRunning) return;

	_effectImage->aniRender(_x, _y, _effectAnimation, onCamera, opacity);
}


void effect::startEffect(int x, int y)
{
	//이미지나 애니메이션 둘중 하나라도 없으면 실행시키지마라
	if (!_effectImage || !_effectAnimation) return;

	_x = x - (_effectAnimation->getFrameWidth() / 2);
	_y = y - (_effectAnimation->getFrameHeight() / 2);

	_isRunning = true;
	_effectAnimation->start();

}


void effect::killEffect(void)
{
	_isRunning = false;
}
