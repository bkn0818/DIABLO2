#pragma once
#include "gameNode.h"

class animation;

class effect : public gameNode
{
protected:
	int				_x;					//����Ʈ ��ǥ��ġ Left
	int				_y;					//����Ʈ ��ǥ��ġ top
		
	image*			_effectImage;		//����Ʈ �̹���
	animation*		_effectAnimation;	//����Ʈ �ִϸ��̼�
	BOOL			_isRunning;			//����Ʈ Ȱ��ȭ ����
	float			_elapsedTime;		//����Ʈ ����ð�

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

