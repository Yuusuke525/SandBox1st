
#include <Windows.h>
#include <vector>
#include <tchar.h>
#include "MyLibUse.h"


LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}


//WinMain�֐�
int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//�E�B���h�E�쐬

	const TCHAR* WC_BASIC = _T("BASIC_WINDOW");

	//�V���v���E�B���h�E�N���X�ݒ�
	WNDCLASSEX wcex = { sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, WndProc, 0, 0, hInstance,
		(HICON)LoadImage(NULL, MAKEINTRESOURCE(IDI_APPLICATION), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED),
		(HCURSOR)LoadImage(NULL, MAKEINTRESOURCE(IDC_ARROW), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED),
		(HBRUSH)GetStockObject(WHITE_BRUSH), NULL, WC_BASIC, NULL };

	//�V���v���E�B���h�E�N���X
	if (!RegisterClassEx(&wcex))
	{
		return false;
	}

	//�E�B���h�E�������̓f�B�X�v���C�Ɉˑ�����B
	const int WINDOW_WIDTH = 800;
	const int WINDOW_HEIGHT = 600;

	//�E�B���h�E�̍쐬
	HWND hWnd = CreateWindowEx(0, WC_BASIC,
		_T("Apprication"), WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_VISIBLE, CW_USEDEFAULT,
		CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);

	//--------------------------------
	//DirectSound�f�o�C�X�쐬
	CMyLib mylib;
	mylib.Initialize(hWnd, hInstance);
	

	//wav�t�@�C���ǂݍ���
	Wave wave[3];
	//wav�t�@�C���͉����K���ɗp�ӂ��Ă�������

	wave[0].Load(_T("katana.wav"));
	wave[1].Load(_T("bomb.wav"));
	wave[2].Load(_T("battle.wav"));
	//�t�@�C���ǂݍ��݂͂����܂�

	//�Z�J���_���o�b�t�@�̍쐬�@�ǂݍ��񂾂Ɖ��f�[�^���R�s�[
	SoundBuffer sb[3];
	for (int i = 0; i < 3; i++)
	{
		sb[i].Create(directSound.pDirectSound8, wave[i].WaveFormat, wave[i].WaveData, wave[i].DataSize);

	}

	//--------------------------------
	//direct3D

	//---------------------------------
	//directInput


	//--------------------------------------------------------------------------------------------
	//�e�N�X�`���̃��[�h

	//�摜�͉����K���ɗp�ӂ��Ă�������

		//�X�v���C�g�̍쐬


	unsigned long frame = 0;

	//-------------------------------------------------------------------------------------------

	//�����̍Đ�
	/*sb[0].Play(false);
	sb[1].Play(true);
	sb[2].Play(true);*/

	int spritey = 0;

	SetRenderState(direct3d.pDevice3D, RENDER_ALPHATEST);

	float rotation = 0.0f;

	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		// �A�v���P�[�V�����ɑ����Ă��郁�b�Z�[�W�����b�Z�[�W�L���[����擾����
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			DispatchMessage(&msg);	// �A�v���P�[�V�����̊e�E�B���h�E�v���V�[�W���Ƀ��b�Z�[�W��]������
		}
		// ���b�Z�[�W���������Ă��Ȃ��Ƃ�
		else
		{
			if (SUCCEEDED(direct3d.pDevice3D->BeginScene()))
			{

				//�L�[��ԍX�V
				directInput.Update();



				//---
				//�����ɃQ�[���������������ƂɂȂ�͂�
				//--


				//�ȉ���ʂ̕`�揈��

				DWORD ClearColor = 0xff808080;//�w�i�N���A�F
				//�w�i�N���A
				direct3d.pDevice3D->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, ClearColor, 1.0f, 0);


				
			}

			//�`�攽�f
			direct3d.pDevice3D->Present(NULL, NULL, NULL, NULL);

			frame++;
		}
	}

	mylib.Uninitialize();
	directInput.Release();

	return 0;
}