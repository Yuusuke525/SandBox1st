#pragma once

#include "GameObject.h"

#include <dsound.h>
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")

class DECLSPEC CSound : public CGameObject
{
private:
	friend int APIENTRY _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int);

	static LPDIRECTSOUND8 pDSound;
	static void CreateDirectSound(HWND hWnd);
	static void ReleaseDirectSound();

	LPDIRECTSOUNDBUFFER pDSBuffer;

	// çƒê∂èIóπÇä÷ím
	LPDIRECTSOUNDNOTIFY pDSNotify;
	HANDLE hEvent;
	static DWORD WINAPI NotifyFunction(LPVOID param);

	void Reset();
public:
	CSound();
	CSound(LPCTSTR filename);
	~CSound();

	CSound(const CSound&);
	CSound& operator=(const CSound&);


	BOOL Load(LPCTSTR filename);
	void Play();
	void Stop();
	void SetVolume(LONG volume);
	void SetPan(LONG lPan);

	void EnableDeleteByEnd();
};
