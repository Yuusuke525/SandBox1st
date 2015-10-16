#pragma once

#define DIRECTSOUND_VERSION 0x800

#pragma comment(lib,"dsound.lib")
#pragma comment(lib,"d3dxof.lib")
#pragma comment(lib,"dxguid.lib")

#include <comdef.h>
#include <dsound.h>

#include <tchar.h>


//DirectSoundクラス

class DirectSound
{
public:
	IDirectSound8 * pDirectSound8;//directsoundデバイス

	DirectSound();
	~DirectSound();

	bool Create(HWND hWnd);

};
