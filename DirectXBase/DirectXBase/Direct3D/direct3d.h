#pragma once 

#define D3D_DEBUG_INFO	//Direct3D�f�o�b�O�t���O


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
	RENDER_DEFAULT,		//�f�t�H���g
	RENDER_ALPHATEST,	//���e�X�g
	RENDER_HALFADD,		//�����Z����
	RENDER_ADD			//���Z����

};


void SetRenderState(IDirect3DDevice9* pD3Device, RENDERSTATE RenderrState);

//-------------------------------
class Direct3D
{
public:

	IDirect3D9*	pD3D9 = NULL;				//Direct3D�f�o�C�X�����p�I�u�W�F�N�g
	IDirect3DDevice9* pDevice3D = NULL;		//Direct3D�̃f�o�C�X�@�X�v���C�g�̕\����e�N�X�`���̃��[�h���ɕK�v

	//�R���X�g���N�^�@�f�X�g���N�^
	Direct3D();
	~Direct3D();

	//�֐���`
	bool Create(HWND hWmd, int Width, int Height);
};
