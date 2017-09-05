#include "stdafx.h"
#include "diablo2.h"


diablo2::diablo2()
{
}


diablo2::~diablo2()
{
}

//초기화 해주는 함수
HRESULT diablo2::init(void)
{
	gameNode::init(true);

	SCENEMANAGER->addScene(L"LoadingScene", new loadingScene);
	SCENEMANAGER->addScene(L"MapToolScene", new mapToolScene);
	SCENEMANAGER->changeScene(L"MapToolScene");


	return S_OK;
}

//메모리 해제 함수
void diablo2::release(void)
{
	gameNode::release();


}

//연산해주는 곳
void diablo2::update(void)
{
	gameNode::update();
	SCENEMANAGER->update();

	if (KEYMANAGER->isOnceKeyDown('H')) SCENEMANAGER->changeScene(L"MenuScene");
}

//여기가 그려주는 곳
void diablo2::render(void)
{
	if (DIRECT2D->_renderTarget->CheckWindowState() != D2D1_WINDOW_STATE_OCCLUDED)
	{
		DIRECT2D->beginDraw();
		//==============================================================================


		SCENEMANAGER->render();


		//================================================================================
		//건들지마라 이거도
		TIMEMANAGER->render();
		DIRECT2D->endDraw();
		DIRECT2D->reCreateResource();
	}
}
