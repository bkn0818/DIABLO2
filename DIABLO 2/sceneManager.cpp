#include "stdafx.h"
#include "sceneManager.h"
#include "gameNode.h"

//private�� Public ó�� ���ٰ����� ���ִ°� friend 
DWORD CALLBACK loadingThread(LPVOID prc)
{
	//��ü�غ�� ���� �ʱ�ȭ �صΰ�
	sceneManager::_readyScene->init();

	//���� ���� �ٲ㳤��
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


//�� �߰�
gameNode* sceneManager::addScene(wstring sceneName, gameNode* scene)
{
	//�ش� ���� ����ġ ������ �� ��ȯ
	if (!scene) return NULL;

	_mSceneList.insert(make_pair(sceneName, scene));

	return scene;
}


//�ε� �� �߰�
gameNode* sceneManager::addLoadingScene(wstring loadingSceneName, gameNode* scene)
{
	//�ش� ���� ����ġ ������ �� ��ȯ
	if (!scene) return NULL;

	_mSceneList.insert(make_pair(loadingSceneName, scene));

	return scene;
}


HRESULT sceneManager::changeScene(wstring sceneName)							
{
	mapSceneIter find = _mSceneList.find(sceneName);

	//���� �������� ������ ���и� �˷����
	if (find == _mSceneList.end()) return E_FAIL;

	//ã�� ���� ������̸� �ٲ��� �ʴ´�
	if (find->second == _currentScene) return S_OK;

	//�� ��ȯ�� �����ϸ�~
	if (SUCCEEDED(find->second->init()))
	{
		//������ ������ �ȵ�� ���� ���� �ٲٰ�, �� ���� ���� �����͸� �����ִ�
		//���·� �ٲ㵵 �������.

		if (_currentScene) _currentScene->release();

		_currentScene = find->second;

		return S_OK;
	}

	return E_FAIL;
}

HRESULT sceneManager::changeScene(wstring sceneName, wstring loadingSceneName)
{
	mapSceneIter find = _mSceneList.find(sceneName);

	//���� �������� ������ ���и� �˷����
	if (find == _mSceneList.end()) return E_FAIL;

	//ã�� ���� ������̸� �ٲ��� �ʴ´�
	if (find->second == _currentScene) return S_OK;

	mapSceneIter findLoading = _mLoadingSceneList.find(loadingSceneName);

	if (find == _mLoadingSceneList.end()) return changeScene(loadingSceneName);

	//�� ��ȯ�� �����ϸ�~
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