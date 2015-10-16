#include "Sound.h"

LPDIRECTSOUND8 CSound::pDSound = NULL;

void CSound::CreateDirectSound(HWND hWnd)
{
	DirectSoundCreate8(&DSDEVID_DefaultPlayback, &pDSound, NULL);
	pDSound->SetCooperativeLevel(hWnd, DSSCL_PRIORITY);
}

void CSound::ReleaseDirectSound()
{
	RELEASE(pDSound);
}

void CSound::Reset()
{
	pDSBuffer = NULL;
	pDSNotify = NULL;
	hEvent = NULL;
}

CSound::CSound()
{
	Reset();
}

CSound::CSound(LPCTSTR filename)
{
	Reset();
	Load(filename);
}

CSound::~CSound()
{
	if(hEvent) CloseHandle(hEvent);
	RELEASE(pDSNotify);
	RELEASE(pDSBuffer);
}

CSound::CSound(const CSound &s)
{
	Reset();
	if(pDSound) pDSound->DuplicateSoundBuffer(s.pDSBuffer, &pDSBuffer);
}

CSound& CSound::operator =(const CSound &s)
{
	// �����̃f�[�^�����
	RELEASE(pDSBuffer);

	Reset();
	if(pDSound) pDSound->DuplicateSoundBuffer(s.pDSBuffer, &pDSBuffer);

	return *this;
}

BOOL CSound::Load(LPCTSTR filename)
{
	// wave�t�@�C�����J��
	FILE *fp;
	if(_tfopen_s(&fp, filename, _T("rb"))){
		DXTRACE_MSG(_T("�t�@�C�����J���܂���"));
		return FALSE;
	}

	// �{����wave�t�@�C�����ǂ������ׂ�
	char buf[10];
	fread(buf, 4, 1, fp);
	if(memcmp(buf, "RIFF", 4) != 0){
		DXTRACE_MSG(_T("RIFF�t�H�[�}�b�g�ł͂���܂���"));
		return FALSE;
	}

	// RIFF�f�[�^�T�C�Y�͏ȗ�
	fseek(fp, 4, SEEK_CUR);

 	fread(buf, 8, 1, fp);
	if(memcmp(buf, "WAVEfmt ", 8) != 0){
		DXTRACE_MSG(_T("WAVE�t�H�[�}�b�g�ł͂Ȃ����A�t�H�[�}�b�g��`������܂���B"));
		return FALSE;
	}

	// fmt�f�[�^�T�C�Y�G���A��ǂݔ�΂�
	fseek(fp, 4, SEEK_CUR);

    // �t�H�[�}�b�g�����擾
	WAVEFORMATEX wavf;
	fread(&wavf, sizeof(WAVEFORMATEX) - 2, 1, fp);

	// ���y�f�[�^�̊J�n���Ӗ�����udata�v�̕����񂪂��邩���ׂ�
	ZeroMemory(buf, 10);
	while(strcmp("data", buf)){
		buf[0] = fgetc(fp);
		if(buf[0] == EOF){
			DXTRACE_MSG(_T("�g�`�f�[�^��`��������܂���B"));
			fclose(fp);
			return FALSE;
		}
		if(buf[0] == 'd') fread(&buf[1], 1, 3, fp);
	}

	// ���y�f�[�^�T�C�Y�擾
	int wsize;
	fread(&wsize, sizeof(int), 1, fp);

	// CreateSoundBuffer�ɑ��M���邽�߂̉��y�����쐬
	DSBUFFERDESC desc;
	ZeroMemory(&desc, sizeof(DSBUFFERDESC));
	desc.dwSize = sizeof(DSBUFFERDESC);
	desc.dwFlags = DSBCAPS_LOCDEFER | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPOSITIONNOTIFY;
	desc.dwBufferBytes = wsize;
	desc.lpwfxFormat = &wavf;

	RELEASE(pDSBuffer);
	pDSound->CreateSoundBuffer(&desc, &pDSBuffer, NULL);

    //�A�N�Z�X�\�ȃo�b�t�@�̃T�C�Y
    DWORD  buff_size;
    //WAV�o�b�t�@�A�N�Z�X�|�C���g���i�[����ׂ̃|�C���^
    LPVOID pvAudioPtr;

	// �o�b�t�@���b�N
	pDSBuffer->Lock(0, 0,			// �o�b�t�@�S�̂����b�N���邽�߁A���l�̎w��͕s�v
		&pvAudioPtr, &buff_size,	// �������ރo�b�t�@���擾���邽�߂̃|�C���^
		NULL, NULL,					// 2�ɕ����ď������ނ��Ƃ��ł���
		DSBLOCK_ENTIREBUFFER		// �o�b�t�@���ׂĂ����b�N
		);

	//�T�E���h�f�[�^���o�b�t�@�֏�������
	fread(pvAudioPtr, buff_size, 1, fp);

	//���b�N����
	pDSBuffer->Unlock(pvAudioPtr, buff_size, NULL, NULL);

	fclose(fp);

	return TRUE;
}

struct PlayThreadData
{
	HANDLE hEvent;
	CSound *soundobj;
};

void CSound::Play()
{
	if(pDSBuffer){
		pDSBuffer->SetCurrentPosition(0);
		pDSBuffer->Play(0, 0, 0);

		if(pDSNotify){
			// �Đ��I�����Ď�����X���b�h�𗧂��グ��
			PlayThreadData *data = new PlayThreadData;
			data->hEvent = hEvent;
			data->soundobj = this;
			DWORD threadID;
			CreateThread(NULL, 0, CSound::NotifyFunction, data, 0, &threadID);
		}
	}
}

void CSound::Stop()
{
	if(pDSBuffer) pDSBuffer->Stop();
}

void CSound::SetVolume(LONG volume)
{
	if(pDSBuffer) pDSBuffer->SetVolume(volume);
}

void CSound::SetPan(LONG lPan)
{
	if(pDSBuffer) pDSBuffer->SetPan(lPan);
}

void CSound::EnableDeleteByEnd()
{
	if(pDSBuffer == NULL) return;

	// IDirectSoundNotify�̍쐬
	HRESULT hr = pDSBuffer->QueryInterface
		(IID_IDirectSoundNotify, (LPVOID*)&pDSNotify);
	
	// �ʒm�C�x���g�Ɏg�p����n���h���̍쐬
	hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	// �C�x���g�����Ɋւ�������쐬
	DSBPOSITIONNOTIFY pn;
	pn.dwOffset = DSBPN_OFFSETSTOP;	// �I���܂ōĐ�
	pn.hEventNotify = hEvent;		// �n���h�����֘A�Â���

	// �C�x���g�f�[�^��o�^
	pDSNotify->SetNotificationPositions(1, &pn);
}

DWORD CSound::NotifyFunction(LPVOID param)
{
	PlayThreadData *data = (PlayThreadData*)param;

	// �Đ��I���ʒm������܂őҋ@
	WaitForMultipleObjects(1, &data->hEvent, FALSE, INFINITE);

	// �I��������Ώۂ̃I�u�W�F�N�g���폜
	delete data->soundobj;

	delete data;

	return 0L;
}