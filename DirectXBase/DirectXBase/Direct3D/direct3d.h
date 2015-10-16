#pragma once 

#define D3D_DEBUG_INFO	//Direct3Dデバッグフラグ


#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3dxof.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

#include <comdef.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <mmsystem.h>

#define RELEASE(x){if (x) x->Release();}

enum RENDERSTATE
{
	RENDER_DEFAULT,		//デフォルト
	RENDER_ALPHATEST,	//αテスト
	RENDER_HALFADD,		//半加算合成
	RENDER_ADD			//加算合成

};


void SetRenderState(IDirect3DDevice9* pD3Device, RENDERSTATE RenderrState);

//-------------------------------
class Direct3D
{
public:

	IDirect3D9*	pD3D9 = NULL;				//Direct3Dデバイス生成用オブジェクト
	IDirect3DDevice9* pDevice3D = NULL;		//Direct3Dのデバイス　スプライトの表示やテクスチャのロード時に必要

	//コンストラクタ　デストラクタ
	Direct3D();
	~Direct3D();

	//関数定義
	bool Create(HWND hWmd, int Width, int Height);
};
