#pragma once
#include "singletonBase.h"
#include <string>
#include <map>

class gameNode;

using namespace std;

class sceneManager : public singletonBase <sceneManager>
{
public:
	typedef map<wstring, gameNode*> mapSceneList;
	typedef map<wstring, gameNode*>::iterator mapSceneIter;
private:
	static gameNode* _currentScene;
	static gameNode* _loadingScene;
	static gameNode* _readyScene;

	mapSceneList _mSceneList;
	mapSceneIter _miSceneList;
	mapSceneList _mLoadingSceneList;

	DWORD _loadingThreadID;

public:
	sceneManager();
	~sceneManager();

	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	//¾À Ãß°¡
	gameNode* addScene(wstring sceneName, gameNode* scene);

	//·Îµù ¾À Ãß°¡
	gameNode* addLoadingScene(wstring loadingSceneName, gameNode* scene);

	HRESULT changeScene(wstring sceneName);
	HRESULT changeScene(wstring sceneName, wstring loadingSceneName);

	friend DWORD CALLBACK loadingThread(LPVOID prc);

	inline gameNode* getCurScene() { return _currentScene; }

	inline map<wstring, gameNode*> getMap() { return _mSceneList; }
	inline map<wstring, gameNode*>::iterator getIMap() { return _miSceneList; }

	//ÇØ´ç¾À «n
	bool sceneCheck(wstring sceneName);
};

