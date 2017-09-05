#pragma once
#include "singletonBase.h"
#include <vector>
#include <map>

class effect;

class effectManager : public singletonBase<effectManager>
{
private:
	typedef vector<effect*> arrEffects;
	typedef vector<effect*>::iterator iterEffects;

	typedef map<wstring, arrEffects> arrEffect;
	typedef map<wstring, arrEffects>::iterator iterEffect;

	typedef vector<map<wstring, arrEffects>> arrTotalEffect;
	typedef vector<map<wstring, arrEffects>>::iterator iterTotalEffect;

private:
	arrTotalEffect _vTotalEffects;

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(bool onCamera, float opacity);

	void addEffect(wstring effectName, const WCHAR* imageName, const WCHAR* fileName, int imageWidth, int imageHeight, int effectWidth, int effectHeight, int fps, float elapsedTime, int buffer);

	void play(wstring effectName, int x, int y);


	effectManager();
	~effectManager();
};

