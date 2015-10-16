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

//class CMyLib;		//あらかじめデータを宣言

////リストで管理する情報を構造体で宣言
//#define NAME_SIZE 32
//struct ListData{
//	int priority;	//実行順位
//	char name[NAME_SIZE];//管理名称
//	CMyLib* gameobj;//オブジェクトのポインタ
//	bool autodelete;
//};

class CMyLib{
private:
	static HWND hWnd;
	static HINSTANCE hInstance;

	BOOL Initialize(HWND hEnd, HINSTANCE hInstance);		// 初期化時に呼び出す  
	void Uninitialize();	// 終了時に呼び出す
	//void DoAllTasks();	//タスクをまとめて処理


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
//		ListData &literal	//構造体で直接渡すときの参照  
//		);
//	static void AppendObject(
//		CMyLib *object,    // ゲームオブジェクトのポインタ  
//		int priority,           // 実行順位（値が少ないほど優先される）  
//		bool autodelete         // オブジェクトの自動削除  
//		);
//	static void AppendObject(
//		CMyLib *object,    // ゲームオブジェクトのポインタ  
//		int priority,           // 実行順位  
//		LPCSTR name,            // オブジェクトの名称  
//		bool autodelete         // オブジェクトの自動削除  
//		);
//
//	static void RemoveObject(CMyLib *target);
};



