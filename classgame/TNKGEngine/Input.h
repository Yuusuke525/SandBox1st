#pragma once

#include "GameObject.h"

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

// パッドの方向キー用
#define PP_UP 0x1
#define PP_RIGHT 0x2
#define PP_DOWN 0x4
#define PP_LEFT 0x8

// デバイス列挙関数に渡すデータを格納する構造体
struct enumdata
{
	LPDIRECTINPUT8 pInput;
	LPDIRECTINPUTDEVICE8 *ppPadDevice;
};

class DECLSPEC CInput : public CGameObject
{
private:
	LPDIRECTINPUT8 pInput;

	LPDIRECTINPUTDEVICE8 pKeyDevice;
	BYTE keydata[256], lastkeydata[256];

	LPDIRECTINPUTDEVICE8 pPadDevice;
	DIJOYSTATE2 paddata, lastpaddata;

	static BOOL CALLBACK EnumJoypad(const DIDEVICEINSTANCE* pInstance, LPVOID pContext);
	static BOOL CALLBACK EnumObject(LPCDIDEVICEOBJECTINSTANCE pInstance, LPVOID pvRef);
public:
	CInput();
	~CInput();

	BOOL IsKeyDown(int key);
	BOOL IsKeyPressed(int key);
	BOOL IsKeyReleased(int key);

	BYTE GetPovPosition();
	BOOL IsButtonDown(int pos);
	BOOL IsButtonPressed(int pos);
	BOOL IsButtonReleased(int pos);

protected:
	void Init();
	void Exec();
};
