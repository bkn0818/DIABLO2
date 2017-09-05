#include "stdafx.h"
#include "diablo2.h"


diablo2::diablo2()
{
}


diablo2::~diablo2()
{
}

//�ʱ�ȭ ���ִ� �Լ�
HRESULT diablo2::init(void)
{
	gameNode::init(true);

	SCENEMANAGER->addScene(L"LoadingScene", new loadingScene);
	SCENEMANAGER->addScene(L"MapToolScene", new mapToolScene);
	SCENEMANAGER->changeScene(L"MapToolScene");


	return S_OK;
}

//�޸� ���� �Լ�
void diablo2::release(void)
{
	gameNode::release();


}

//�������ִ� ��
void diablo2::update(void)
{
	gameNode::update();
	SCENEMANAGER->update();

	if (KEYMANAGER->isOnceKeyDown('H')) SCENEMANAGER->changeScene(L"MenuScene");
}

//���Ⱑ �׷��ִ� ��
void diablo2::render(void)
{
	if (DIRECT2D->_renderTarget->CheckWindowState() != D2D1_WINDOW_STATE_OCCLUDED)
	{
		DIRECT2D->beginDraw();
		//==============================================================================


		SCENEMANAGER->render();


		//================================================================================
		//�ǵ������� �̰ŵ�
		TIMEMANAGER->render();
		DIRECT2D->endDraw();
		DIRECT2D->reCreateResource();
	}
}
