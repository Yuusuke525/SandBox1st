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
		// �C���^�[�t�F�C�X�̎擾
		HRESULT hr;
		hr = DirectInput8Create(
			GetHInstance(),			// �\�t�g�̃C���X�^���X�n���h��
			DIRECTINPUT_VERSION,	// DirectInput�̃o�[�W����
			IID_IDirectInput8,		// �擾����C���^�[�t�F�C�X�̃^�C�v
			(LPVOID*)&pInput,		// �C���^�[�t�F�C�X�̊i�[��
			NULL					// COM�W���̐���I�u�W�F�N�g�i�g��Ȃ��̂�NULL�j
			);

		if(FAILED(hr)){
			DXTRACE_MSG(_T("DirectInput�̏������Ɏ��s���܂���"));
			return;
		}

		hr = pInput->CreateDevice(
			GUID_SysKeyboard,	// �󂯕t������̓f�o�C�X
			&pKeyDevice,		// IDirectInputDevice8�i�[��
			NULL				// COM�W���̐���I�u�W�F�N�g�i�g��Ȃ��̂�NULL�j
			);

		if(FAILED(hr)){
			DXTRACE_MSG(_T("DirectInputDevice�̏������Ɏ��s���܂���"));
			return;
		}

		// �f�[�^�t�H�[�}�b�g�̐ݒ�
		pKeyDevice->SetDataFormat(&c_dfDIKeyboard);

		// �������[�h�̐ݒ�
		pKeyDevice->SetCooperativeLevel(GetHWnd(),
			DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

		// �L�[�o�b�t�@�̏�����
		ZeroMemory(keydata, 256);


		// �W���C�p�b�h�̍쐬
		enumdata ed;
		ed.pInput = pInput;
		ed.ppPadDevice = &pPadDevice;
		pInput->EnumDevices(
			DI8DEVCLASS_GAMECTRL, EnumJoypad,
			&ed, DIEDFL_ATTACHEDONLY);

		if(pPadDevice){
			// �A�i���O�L�[�̃f�[�^��ݒ�
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

	// �f�o�C�X���쐬�ł����̂Ń��[�v�𔲂���
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
		// ���͂̎󂯕t���J�n
		pKeyDevice->Acquire();

		memcpy(lastkeydata, keydata, 256);
		pKeyDevice->GetDeviceState(256, keydata);
	}

	if(pPadDevice){
		// �W���C�p�b�h�f�[�^�̎擾
		pPadDevice->Poll();

		// ���͂̎�t�J�n
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

	// ���݉�����Ă��Ă����O�ɉ�����Ă��Ȃ��Ƃ�TRUE��Ԃ�
	if((keydata[key] & 0x80) && !(lastkeydata[key] & 0x80)) return TRUE;
	return FALSE;
}

BOOL CInput::IsKeyReleased(int key)
{
	if(pKeyDevice == NULL) return FALSE;

	// ���݉�����Ă��炸�����O�ɂ͉�����Ă����Ƃ�TRUE��Ԃ�
	if(!(keydata[key] & 0x80) && (lastkeydata[key] & 0x80)) return TRUE;
	return FALSE;
}

BYTE CInput::GetPovPosition()
{
	if(pPadDevice == NULL) return 0x0;

	// �f�W�^�����͂��画�f
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

	// �A�i���O���͂��画�f
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
