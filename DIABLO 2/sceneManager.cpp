#include "stdafx.h"
#include "sceneManager.h"
#include "gameNode.h"

//private도 Public 처럼 접근가능케 해주는게 friend 
DWORD CALLBACK loadingThread(LPVOID prc)
{
	//교체준비된 씬을 초기화 해두고
	sceneManager::_readyScene->init();

	//현재 씬에 바꿔낀다
	sceneManager::_currentScene = sceneManager::_readyScene;

	sceneManager::_loadingScene->release();
	sceneManager::_loadingScene = NULL;
	sceneManager::_readyScene = NULL;

	return 0;
}

gameNode* sceneManager::_currentScene = NULL;
gameNode* sceneManager::_loadingScene = NULL;
gameNode* sceneManager::_readyScene = NULL;

sceneManager::sceneManager()
{
}


sceneManager::~sceneManager()
{

}

HRESULT sceneManager::init(void)
{
	_currentScene = NULL;
	_loadingScene = NULL;
	_readyScene = NULL;


	return S_OK;
}

void sceneManager::release(void)
{
	mapSceneIter miSceneList = _mSceneList.begin();

	for (; miSceneList != _mSceneList.end();)
	{
		if (miSceneList->second != NULL)
		{
			if (miSceneList->second == _currentScene) miSceneList->second->release();

			SAFE_DELETE(miSceneList->second);

			miSceneList = _mSceneList.erase(miSceneList);
		}
		else ++miSceneList;
	}

	_mSceneList.clear();
}

void sceneManager::update(void)	
{
	if (_currentScene) _currentScene->update();
}

void sceneManager::render(void)	
{
	if (_currentScene) _currentScene->render();
}


//씬 추가
gameNode* sceneManager::addScene(wstring sceneName, gameNode* scene)
{
	//해당 씬이 존재치 않으면 널 반환
	if (!scene) return NULL;

	_mSceneList.insert(make_pair(sceneName, scene));

	return scene;
}


//로딩 씬 추가
gameNode* sceneManager::addLoadingScene(wstring loadingSceneName, gameNode* scene)
{
	//해당 씬이 존재치 않으면 널 반환
	if (!scene) return NULL;

	_mSceneList.insert(make_pair(loadingSceneName, scene));

	return scene;
}


HRESULT sceneManager::changeScene(wstring sceneName)							
{
	mapSceneIter find = _mSceneList.find(sceneName);

	//씬이 존재하지 않으면 실패를 알려줘라
	if (find == _mSceneList.end()) return E_FAIL;

	//찾은 씬이 현재씬이면 바꾸지 않는다
	if (find->second == _currentScene) return S_OK;

	//씬 전환이 성공하면~
	if (SUCCEEDED(find->second->init()))
	{
		//순서가 마음에 안들면 먼저 씬을 바꾸고, 그 전에 씬에 데이터를 날려주는
		//형태로 바꿔도 상관없다.

		if (_currentScene) _currentScene->release();

		_currentScene = find->second;

		return S_OK;
	}

	return E_FAIL;
}

HRESULT sceneManager::changeScene(wstring sceneName, wstring loadingSceneName)
{
	mapSceneIter find = _mSceneList.find(sceneName);

	//씬이 존재하지 않으면 실패를 알려줘라
	if (find == _mSceneList.end()) return E_FAIL;

	//찾은 씬이 현재씬이면 바꾸지 않는다
	if (find->second == _currentScene) return S_OK;

	mapSceneIter findLoading = _mLoadingSceneList.find(loadingSceneName);

	if (find == _mLoadingSceneList.end()) return changeScene(loadingSceneName);

	//씬 전환이 성공하면~
	if (SUCCEEDED(find->second->init()))
	{
		if (_currentScene) _currentScene->release();

		_loadingScene = findLoading->second;

		_readyScene = find->second;

		CloseHandle(CreateThread(NULL, 0, loadingThread, NULL, 0, &_loadingThreadID));

	}

	return E_FAIL;
}


bool sceneManager::sceneCheck(wstring sceneName)
{
	mapSceneIter find = _mSceneList.find(sceneName);

	if (find == _mSceneList.end()) return false;
	if (find->second == _currentScene) return true;
	else return false;
}