#pragma once
#include "singletonBase.h"
#include <map>

#include "inc/fmod.hpp"

#pragma comment (lib, "lib/fmodex_vc.lib")

using namespace FMOD;

#define SOUNDBUFFER 50
#define EXTRACHANNELBUFFER 5

#define TOTALSOUNDBUFFER (SOUNDBUFFER + EXTRACHANNELBUFFER)

class soundManager : public singletonBase<soundManager>
{
private:
	typedef map<wstring, Sound**> arrSounds;
	typedef map<wstring, Sound**>::iterator arrSoundsIter;
	typedef map<wstring, Channel**> arrChannels;
	typedef map<wstring, Channel**>::iterator arrChannelsIter;

private:
	System* _system;
	Sound** _sound;
	Channel** _channel;

	arrSounds _mTotalSounds;

public:
	HRESULT init();
	void release();
	void update();

	void addSound(wstring keyName, wstring soundName, bool bgm, bool loop);
	void play(wstring keyName, float volume = 1.0f); //0.0 ~ 1.0 == 0 ~ 255
	void stop(wstring keyName);
	void pause(wstring keyName);
	void resume(wstring keyName);

	bool isPlaySound(wstring keyName);
	bool isPauseSound(wstring keyName);




	soundManager();
	~soundManager();
};

