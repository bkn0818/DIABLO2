#pragma once
#include "gameNode.h"
#include "loadingScene.h"
#include "mapToolScene.h"


class diablo2 : public gameNode
{
private:

	
	

public:
	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	

	diablo2();
	~diablo2();
};

