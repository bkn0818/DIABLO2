#include "stdafx.h"
#include "loadingScene.h"


loadingScene::loadingScene()
{
}


loadingScene::~loadingScene()
{
}

HRESULT loadingScene::init()
{
	_ld = new loading;
	_ld->init();
	this->loadingImage();
	this->loadingSound();

	return S_OK;
}
void loadingScene::release()
{
	SAFE_DELETE(_ld);
}
void loadingScene::update()
{
	_ld->update();
	if (_ld->loadingDone()) SCENEMANAGER->changeScene(L"MenuScene");
}
void loadingScene::render()
{
	_ld->render();
}
void loadingScene::loadingImage()
{
	_ld->loadFrameImage(L"character_idle", L"townIdle", 1216, 1279, 16, 16);
	_ld->loadFrameImage(L"character_townwalk", L"townWalk", 608, 1311, 8, 16);
	_ld->loadFrameImage(L"tiles", L"townTiles", 800, 2959, 5, 37);
	_ld->loadFrameImage(L"objects", L"townObjects", 5760, 2502, 20, 10);
}
void loadingScene::loadingSound()
{
	
}