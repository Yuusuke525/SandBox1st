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
	// 既存のデータを解放
	RELEASE(pDSBuffer);

	Reset();
	if(pDSound) pDSound->DuplicateSoundBuffer(s.pDSBuffer, &pDSBuffer);

	return *this;
}

BOOL CSound::Load(LPCTSTR filename)
{
	// waveファイルを開く
	FILE *fp;
	if(_tfopen_s(&fp, filename, _T("rb"))){
		DXTRACE_MSG(_T("ファイルが開けません"));
		return FALSE;
	}

	// 本当にwaveファイルかどうか調べる
	char buf[10];
	fread(buf, 4, 1, fp);
	if(memcmp(buf, "RIFF", 4) != 0){
		DXTRACE_MSG(_T("RIFFフォーマットではありません"));
		return FALSE;
	}

	// RIFFデータサイズは省略
	fseek(fp, 4, SEEK_CUR);

 	fread(buf, 8, 1, fp);
	if(memcmp(buf, "WAVEfmt ", 8) != 0){
		DXTRACE_MSG(_T("WAVEフォーマットではないか、フォーマット定義がありません。"));
		return FALSE;
	}

	// fmtデータサイズエリアを読み飛ばす
	fseek(fp, 4, SEEK_CUR);

    // フォーマット情報を取得
	WAVEFORMATEX wavf;
	fread(&wavf, sizeof(WAVEFORMATEX) - 2, 1, fp);

	// 音楽データの開始を意味する「data」の文字列があるか調べる
	ZeroMemory(buf, 10);
	while(strcmp("data", buf)){
		buf[0] = fgetc(fp);
		if(buf[0] == EOF){
			DXTRACE_MSG(_T("波形データ定義が見つかりません。"));
			fclose(fp);
			return FALSE;
		}
		if(buf[0] == 'd') fread(&buf[1], 1, 3, fp);
	}

	// 音楽データサイズ取得
	int wsize;
	fread(&wsize, sizeof(int), 1, fp);

	// CreateSoundBufferに送信するための音楽情報を作成
	DSBUFFERDESC desc;
	ZeroMemory(&desc, sizeof(DSBUFFERDESC));
	desc.dwSize = sizeof(DSBUFFERDESC);
	desc.dwFlags = DSBCAPS_LOCDEFER | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPOSITIONNOTIFY;
	desc.dwBufferBytes = wsize;
	desc.lpwfxFormat = &wavf;

	RELEASE(pDSBuffer);
	pDSound->CreateSoundBuffer(&desc, &pDSBuffer, NULL);

    //アクセス可能なバッファのサイズ
    DWORD  buff_size;
    //WAVバッファアクセスポイントを格納する為のポインタ
    LPVOID pvAudioPtr;

	// バッファロック
	pDSBuffer->Lock(0, 0,			// バッファ全体をロックするため、数値の指定は不要
		&pvAudioPtr, &buff_size,	// 書き込むバッファを取得するためのポインタ
		NULL, NULL,					// 2つに分けて書き込むこともできる
		DSBLOCK_ENTIREBUFFER		// バッファすべてをロック
		);

	//サウンドデータをバッファへ書き込む
	fread(pvAudioPtr, buff_size, 1, fp);

	//ロック解除
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
			// 再生終了を監視するスレッドを立ち上げる
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

	// IDirectSoundNotifyの作成
	HRESULT hr = pDSBuffer->QueryInterface
		(IID_IDirectSoundNotify, (LPVOID*)&pDSNotify);
	
	// 通知イベントに使用するハンドルの作成
	hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	// イベント発生に関する情報を作成
	DSBPOSITIONNOTIFY pn;
	pn.dwOffset = DSBPN_OFFSETSTOP;	// 終了まで再生
	pn.hEventNotify = hEvent;		// ハンドラを関連づける

	// イベントデータを登録
	pDSNotify->SetNotificationPositions(1, &pn);
}

DWORD CSound::NotifyFunction(LPVOID param)
{
	PlayThreadData *data = (PlayThreadData*)param;

	// 再生終了通知が来るまで待機
	WaitForMultipleObjects(1, &data->hEvent, FALSE, INFINITE);

	// 終了したら対象のオブジェクトを削除
	delete data->soundobj;

	delete data;

	return 0L;
}