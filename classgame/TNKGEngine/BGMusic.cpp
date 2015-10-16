#include "BGMusic.h"

void CBGMusic::InitializeBGM()
{
	pBuilder = NULL;
	pMediaCtrl = NULL;
	pMediaSeeking = NULL;
	pBasicAudio = NULL;

	bLoop = FALSE;
	bFading = FALSE;
}

void CBGMusic::Release()
{
	RELEASE(pBasicAudio);
	RELEASE(pMediaSeeking);
	RELEASE(pMediaCtrl);
	RELEASE(pBuilder);
}

CBGMusic::CBGMusic()
{
	InitializeBGM();
}

CBGMusic::CBGMusic(LPCTSTR filename)
{
	InitializeBGM();
	Load(filename);
}

CBGMusic::~CBGMusic()
{
	Release();
}

void CBGMusic::Load(LPCTSTR filename)
{
	Release();

	HRESULT hr = CoCreateInstance(
		CLSID_FilterGraph,		// COMの識別子
		NULL,					// 特別に取得する情報はないのでNULL
		CLSCTX_INPROC_SERVER,	// このプログラム内でのみ使うことを指定
		IID_IGraphBuilder,		// 取得するインターフェース
		(LPVOID*)&pBuilder);	// 格納先

	if(FAILED(hr)){
		DXTRACE_MSG(_T("IGraphBuilderの作成に失敗しました"));
		return;
	}

#ifdef _UNICODE
	pBuilder->RenderFile(filename, NULL);
#else
	wchar_t wfname[MAX_PATH];
	MultiByteToWideChar(CP_ACP, 0, filename, -1, wfname, MAX_PATH);
	pBuilder->RenderFile(wfname, NULL);
#endif

	pBuilder->QueryInterface(IID_IMediaControl, (LPVOID*)&pMediaCtrl);
	pBuilder->QueryInterface(IID_IMediaSeeking, (LPVOID*)&pMediaSeeking);
	pBuilder->QueryInterface(IID_IBasicAudio, (LPVOID*)&pBasicAudio);
}

void CBGMusic::Play(BOOL loop)
{
	if(pMediaCtrl){
		bLoop = loop;
		if(pMediaSeeking){
			LONGLONG s = 0;
			pMediaSeeking->SetPositions(
				&s, AM_SEEKING_AbsolutePositioning,
				NULL, AM_SEEKING_NoPositioning);
		}
		pMediaCtrl->Run();
	}
}

void CBGMusic::Stop()
{
	if(pMediaCtrl) pMediaCtrl->Stop();
}

void CBGMusic::SetVolume(LONG volume)
{
	if(pBasicAudio) pBasicAudio->put_Volume(volume);
}

void CBGMusic::Exec()
{
	if(bLoop == TRUE && pMediaSeeking){
		LONGLONG s, e;
		pMediaSeeking->GetPositions(&s, &e);
		if(s >= e){
			s = 0;
			pMediaSeeking->SetPositions(
				&s, AM_SEEKING_AbsolutePositioning,
				NULL, AM_SEEKING_NoPositioning);
		}
	}

	if(bFading == TRUE){
		float t = (float)timeGetTime() - time_s;
		pBasicAudio->put_Volume((long)(delta * t + vol_s));

		TCHAR str[20];
		_stprintf_s(str, 20, _T("%f"), delta * t + vol_s);
		//DXTRACE_MSG(str);
		
		if(t >= time_e) bFading = FALSE;
	}
}

void CBGMusic::Fade(DWORD endtime, LONG volume)
{
	if(!pBasicAudio){
		DXTRACE_MSG(_T("ボリュームの設定ができません。"));
		return;
	}

	bFading = TRUE;

	LONG v;
	pBasicAudio->get_Volume(&v);

	vol_s = (float)v;
	float vol_e = (float)volume;

	time_s = (float)timeGetTime();
	time_e = (float)endtime;

	delta = (vol_e - vol_s) / time_e;
}
