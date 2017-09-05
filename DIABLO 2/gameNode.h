#pragma once
#include "image.h"


class gameNode
{
private:
	
	bool _managerInit;

public:
	virtual HRESULT init(void);
	virtual HRESULT init(bool managerInit);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	////인터페이스
	//virtual void move(void) = 0;	//순수가상함수
	//virtual void jump(void) = 0;
	//virtual void attack(void) = 0;



	

	LRESULT MainProc(HWND, UINT, WPARAM, LPARAM);


	gameNode();
	virtual ~gameNode();
};

