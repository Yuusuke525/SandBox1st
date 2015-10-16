#pragma once

#include <tchar.h>
#include <Windows.h>

#include <DxErr.h>
#include <d3d9.h>
#include <d3dx9.h>

#include <iostream>
//#include <list>


#pragma comment(lib, "winmm.lib") 
#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )
# pragma comment(lib, "DxErr.lib")
# pragma comment(lib, "version.lib")

#if _DEBUG  
#include <crtdbg.h>  
#define new  ::new( _NORMAL_BLOCK, __FILE__, __LINE__ )  
#endif  


using namespace std;

#define RELEASE(x){if (x) x->Release();}

//class CMyLib;		//���炩���߃f�[�^��錾

////���X�g�ŊǗ���������\���̂Ő錾
//#define NAME_SIZE 32
//struct ListData{
//	int priority;	//���s����
//	char name[NAME_SIZE];//�Ǘ�����
//	CMyLib* gameobj;//�I�u�W�F�N�g�̃|�C���^
//	bool autodelete;
//};

class CMyLib{
private:
	static HWND hWnd;
	static HINSTANCE hInstance;

	BOOL Initialize(HWND hEnd, HINSTANCE hInstance);		// ���������ɌĂяo��  
	void Uninitialize();	// �I�����ɌĂяo��
	//void DoAllTasks();	//�^�X�N���܂Ƃ߂ď���


	friend int APIENTRY _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int);

	//static list<ListData> objectlist;
	//static bool it_moved;
	//static list<ListData>::iterator it_task;


public:
	static const HWND GetHWnd(){ return hWnd; }
	static const HINSTANCE GetInstance(){ return hInstance; }

	virtual ~CMyLib(){}

	//CMyLib* FindObject(char *name);

protected:
	static LPDIRECT3D9 pD3D;
	static LPDIRECT3DDEVICE9 pD3Ddevice;
	static LPD3DXSPRITE pSprite;


	virtual void Init(){}
	virtual void Exec(){}


//
//public:
//	static void AppendObject(
//		ListData &literal	//�\���̂Œ��ړn���Ƃ��̎Q��  
//		);
//	static void AppendObject(
//		CMyLib *object,    // �Q�[���I�u�W�F�N�g�̃|�C���^  
//		int priority,           // ���s���ʁi�l�����Ȃ��قǗD�悳���j  
//		bool autodelete         // �I�u�W�F�N�g�̎����폜  
//		);
//	static void AppendObject(
//		CMyLib *object,    // �Q�[���I�u�W�F�N�g�̃|�C���^  
//		int priority,           // ���s����  
//		LPCSTR name,            // �I�u�W�F�N�g�̖���  
//		bool autodelete         // �I�u�W�F�N�g�̎����폜  
//		);
//
//	static void RemoveObject(CMyLib *target);
};



