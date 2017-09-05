#pragma once
#include "gameNode.h"
#include "progressBar.h"

enum LOAD_KIND
{
	LOAD_KIND_IMAGE_0,

	LOAD_KIND_FRAMEIMAGE_0,

	LOAD_KIND_SOUND,
	LOAD_KIND_END
};

struct tagImageResource
{
	wstring keyName;
	const WCHAR* fileName;
	float x, y;
	int width, height;
	int frameX, frameY;
};

struct tagSoundResource
{
	wstring keyName;
	const WCHAR* fileName;
	bool bgm;
	bool loop;
};

class loadItem
{
private:
	LOAD_KIND _kind;
	tagImageResource _imageResource;
	tagSoundResource _soundResource;
	

public:
	
	HRESULT initForImage(wstring keyName, const WCHAR* fileName, int width, int height);
	

	HRESULT initForFrameImage(wstring keyName, const WCHAR* fileName, int width, int height,
		int frameX, int frameY);
	HRESULT initForSound(wstring keyName, const WCHAR* soundName, bool bgm = FALSE, bool loop = FALSE);


	LOAD_KIND getLoadingKind(void) { return _kind; }

	tagImageResource getImageResource(void) { return _imageResource; }
	tagSoundResource getSoundResource(void) { return _soundResource; }

	loadItem();
	~loadItem();
};

class loading : public gameNode
{
private:
	typedef vector<loadItem*> arrLoadItem;
	typedef vector<loadItem*>::iterator arrLoadItemIter;

private:
	arrLoadItem		_vLoadItem;

	image*			_background;
	progressBar*	_loadingBar;

	int				_currentGauge;

	WCHAR _loadingFileName[64];

public:
	HRESULT init();
	void release();
	void update();
	void render();

	
	void loadImage(wstring keyName, const WCHAR* fileName, int width, int height);
	
	void loadFrameImage(wstring keyName, const WCHAR* fileName, int width, int height, int frameX, int frameY);
	void loadSound(wstring key, const WCHAR* soundName, BOOL bgm = FALSE, BOOL loop = FALSE);

	BOOL loadingDone();

	vector<loadItem*> getLoadItem() { return _vLoadItem; }

	loading();
	~loading();
};

