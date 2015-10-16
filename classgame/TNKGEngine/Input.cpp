#include "Input.h"

CInput::CInput()
{
	pInput = NULL;
	pKeyDevice = NULL;
	pPadDevice = NULL;
}

CInput::~CInput()
{
	RELEASE(pPadDevice);
	RELEASE(pKeyDevice);
	RELEASE(pInput);
}

void CInput::Init()
{
	if(pInput == NULL){
		// インターフェイスの取得
		HRESULT hr;
		hr = DirectInput8Create(
			GetHInstance(),			// ソフトのインスタンスハンドル
			DIRECTINPUT_VERSION,	// DirectInputのバージョン
			IID_IDirectInput8,		// 取得するインターフェイスのタイプ
			(LPVOID*)&pInput,		// インターフェイスの格納先
			NULL					// COM集成の制御オブジェクト（使わないのでNULL）
			);

		if(FAILED(hr)){
			DXTRACE_MSG(_T("DirectInputの初期化に失敗しました"));
			return;
		}

		hr = pInput->CreateDevice(
			GUID_SysKeyboard,	// 受け付ける入力デバイス
			&pKeyDevice,		// IDirectInputDevice8格納先
			NULL				// COM集成の制御オブジェクト（使わないのでNULL）
			);

		if(FAILED(hr)){
			DXTRACE_MSG(_T("DirectInputDeviceの初期化に失敗しました"));
			return;
		}

		// データフォーマットの設定
		pKeyDevice->SetDataFormat(&c_dfDIKeyboard);

		// 協調モードの設定
		pKeyDevice->SetCooperativeLevel(GetHWnd(),
			DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

		// キーバッファの初期化
		ZeroMemory(keydata, 256);


		// ジョイパッドの作成
		enumdata ed;
		ed.pInput = pInput;
		ed.ppPadDevice = &pPadDevice;
		pInput->EnumDevices(
			DI8DEVCLASS_GAMECTRL, EnumJoypad,
			&ed, DIEDFL_ATTACHEDONLY);

		if(pPadDevice){
			// アナログキーのデータを設定
			pPadDevice->EnumObjects(EnumObject, pPadDevice, DIDFT_AXIS);

			pPadDevice->SetCooperativeLevel(GetHWnd(),
				DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
			hr = pPadDevice->SetDataFormat(&c_dfDIJoystick2);
			if(FAILED(hr)) RELEASE(pPadDevice);
		}
	}
}

BOOL CALLBACK CInput::EnumJoypad(const DIDEVICEINSTANCE* pInstance, LPVOID pContext)
{
	enumdata *ed = (enumdata*)pContext;

	HRESULT hr;
	hr = ed->pInput->CreateDevice
		(pInstance->guidInstance, ed->ppPadDevice, NULL);
	if(FAILED(hr)) return DIENUM_CONTINUE;

	// デバイスが作成できたのでループを抜ける
	return DIENUM_STOP;
}

BOOL CALLBACK CInput::EnumObject(LPCDIDEVICEOBJECTINSTANCE pInstance, LPVOID pvRef)
{
	DIPROPRANGE range;
	range.diph.dwSize = sizeof(DIPROPRANGE);
	range.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	range.diph.dwObj = pInstance->dwType;
	range.diph.dwHow = DIPH_BYID;
	range.lMin = -10;
	range.lMax = +10;
	
	LPDIRECTINPUTDEVICE8 pInputDev = (LPDIRECTINPUTDEVICE8)pvRef;
	pInputDev->SetProperty(DIPROP_RANGE, &range.diph);

	return DIENUM_CONTINUE;
}

void CInput::Exec()
{
	if(pKeyDevice){
		// 入力の受け付け開始
		pKeyDevice->Acquire();

		memcpy(lastkeydata, keydata, 256);
		pKeyDevice->GetDeviceState(256, keydata);
	}

	if(pPadDevice){
		// ジョイパッドデータの取得
		pPadDevice->Poll();

		// 入力の受付開始
		pPadDevice->Acquire();

		memcpy(&lastpaddata, &paddata, sizeof(DIJOYSTATE2));
		pPadDevice->GetDeviceState(sizeof(DIJOYSTATE2), &paddata);
	}
}

BOOL CInput::IsKeyDown(int key)
{
	if(pKeyDevice == NULL) return FALSE;
	return (keydata[key] & 0x80) ? TRUE : FALSE;
}

BOOL CInput::IsKeyPressed(int key)
{
	if(pKeyDevice == NULL) return FALSE;

	// 現在押されていてかつ直前に押されていないときTRUEを返す
	if((keydata[key] & 0x80) && !(lastkeydata[key] & 0x80)) return TRUE;
	return FALSE;
}

BOOL CInput::IsKeyReleased(int key)
{
	if(pKeyDevice == NULL) return FALSE;

	// 現在押されておらずかつ直前には押されていたときTRUEを返す
	if(!(keydata[key] & 0x80) && (lastkeydata[key] & 0x80)) return TRUE;
	return FALSE;
}

BYTE CInput::GetPovPosition()
{
	if(pPadDevice == NULL) return 0x0;

	// デジタル入力から判断
	switch(paddata.rgdwPOV[0]){
		case 0:
			return PP_UP;
		case 4500:
			return PP_UP | PP_RIGHT;
		case 9000:
			return PP_RIGHT;
		case 13500:
			return PP_RIGHT | PP_DOWN;
		case 18000:
			return PP_DOWN;
		case 22500:
			return PP_DOWN | PP_LEFT;
		case 27000:
			return PP_LEFT;
		case 31500:
			return PP_LEFT | PP_UP;
	}

	// アナログ入力から判断
	BYTE result = 0x0;
	if(paddata.lX > 5) result |= PP_RIGHT;
	else if(paddata.lX < -5) result |= PP_LEFT;
	if(paddata.lY > 5) result |= PP_DOWN;
	else if(paddata.lY < -5) result |= PP_UP;

	return result;
}

BOOL CInput::IsButtonDown(int pos)
{
	if(pPadDevice == NULL) return FALSE;
	return paddata.rgbButtons[pos];
}

BOOL CInput::IsButtonPressed(int pos)
{
	if(pPadDevice == NULL) return FALSE;

	if(paddata.rgbButtons[pos] && !lastpaddata.rgbButtons[pos]) return TRUE;
	return FALSE;
}

BOOL CInput::IsButtonReleased(int pos)
{
	if(pPadDevice == NULL) return FALSE;

	if(!paddata.rgbButtons[pos] && lastpaddata.rgbButtons[pos]) return TRUE;
	return FALSE;
}
