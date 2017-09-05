#include "stdafx.h"
#include "soundManager.h"


soundManager::soundManager()
	: _system(NULL), _sound(NULL), _channel(NULL)
{
}


soundManager::~soundManager()
{
}

HRESULT soundManager::init()
{
	//FMOD ���� ������ ���ڴٰ� ����
	System_Create(&_system);

	_system->init(TOTALSOUNDBUFFER, FMOD_INIT_NORMAL, 0);

	_sound = new Sound*[TOTALSOUNDBUFFER];
	_channel = new Channel*[TOTALSOUNDBUFFER];

	memset(_sound, 0, sizeof(Sound*) * (TOTALSOUNDBUFFER));
	memset(_channel, 0, sizeof(Channel*) * (TOTALSOUNDBUFFER));


	return S_OK;
}

void soundManager::release()
{
	if ((_channel != NULL || _sound != NULL))
	{
		for (int i = 0; i < TOTALSOUNDBUFFER; i++)
		{
			if (_channel != NULL)
			{
				_channel[i]->stop();
			}

			if (_sound != NULL)
			{
				_sound[i]->release();
			}
		}
	}

	//�޸� �����
	SAFE_DELETE_ARRAY(_channel);
	SAFE_DELETE_ARRAY(_sound);

	//�ý��� �ݱ� 
	if (_system != NULL)
	{
		_system->release();
		_system->close();
	}
}

void soundManager::update()
{
	_system->update();
}

void soundManager::addSound(wstring keyName, wstring soundName, bool bgm, bool loop)
{
	//������ �����ų����?
	if (loop)
	{
		if (bgm)
		{
			_system->createStream((const char*)soundName.c_str(), FMOD_LOOP_NORMAL | FMOD_UNICODE, 0, &_sound[_mTotalSounds.size()]);
		}
		else
		{
			_system->createSound((const char*)soundName.c_str(), FMOD_LOOP_NORMAL | FMOD_UNICODE, 0, &_sound[_mTotalSounds.size()]);
		}
	}
	else //������ �ƴ϶��
	{
		if (bgm)
		{
			_system->createStream((const char*)soundName.c_str(), FMOD_DEFAULT | FMOD_UNICODE, 0, &_sound[_mTotalSounds.size()]);
		}
		else
		{
			_system->createSound((const char*)soundName.c_str(), FMOD_DEFAULT | FMOD_UNICODE, 0, &_sound[_mTotalSounds.size()]);
		}
	}

	//pair<��1, ��2>(����1, ����2)
	_mTotalSounds.insert(make_pair(keyName, &_sound[_mTotalSounds.size()]));
}

void soundManager::play(wstring keyName, float volume) // 0 ~ 255 == 0.0 ~ 1.0f
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		//������ �ִٸ�
		if (keyName == iter->first)
		{
			_system->playSound(FMOD_CHANNEL_FREE, _sound[count], false, &_channel[count]);

			_channel[count]->setVolume(volume);
			break;
		}
	}
}

void soundManager::stop(wstring keyName)
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		//������ �ִٸ�
		if (keyName == iter->first)
		{
			_channel[count]->stop();
			break;
		}
	}
}

void soundManager::pause(wstring keyName)
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		//������ �ִٸ�
		if (keyName == iter->first)
		{
			_channel[count]->setPaused(true);
			break;
		}
	}
}

void soundManager::resume(wstring keyName)
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		//������ �ִٸ�
		if (keyName == iter->first)
		{
			_channel[count]->setPaused(false);
			break;
		}
	}
}

bool soundManager::isPlaySound(wstring keyName)
{
	bool isPlay;
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->isPlaying(&isPlay);
			break;
		}
	}

	return isPlay;
}

bool soundManager::isPauseSound(wstring keyName)
{
	bool isPause;
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->getPaused(&isPause);
			break;
		}
	}

	return isPause;
}