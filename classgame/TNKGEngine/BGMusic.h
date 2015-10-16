#pragma once
#include "gameobject.h"

#include <strmif.h>
#include <control.h>
#include <uuids.h>
#pragma comment (lib,"strmiids.lib")

class DECLSPEC CBGMusic : public CGameObject
{
private:
	IGraphBuilder *pBuilder;
	IMediaControl *pMediaCtrl;
	IMediaSeeking *pMediaSeeking;
	IBasicAudio *pBasicAudio;

	BOOL bLoop;
	
	BOOL bFading;
	float time_s, time_e, vol_s, delta;

	void InitializeBGM();
	void Release();

public:
	CBGMusic();
	CBGMusic(LPCTSTR filename);
	~CBGMusic();

	void Load(LPCTSTR filename);
	void Play(BOOL loop);
	void Stop();

	void SetVolume(LONG volume);
	void Fade(DWORD endtime, LONG volume);

protected:
	void Exec();
};
