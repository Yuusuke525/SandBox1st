#pragma once

#include "ClassGameDef.h"

#include <tchar.h>
#include <windows.h>

#include <list>
#include <string>
using namespace std;

#include <hash_map>
using namespace stdext;

#include <dxerr.h>
#include <d3d9.h>
#include <d3dx9.h>

#if _DEBUG
#include <crtdbg.h>
#define new  new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif

#define RELEASE(x) if(x){ x->Release(); x = NULL; }
inline float d2r(float d)
{
	return (d / 180.0f * D3DX_PI);
}

class CGameObject;

#define NAME_SIZE 32
struct ListData
{
	int priority;			// 実行順位
	char name[NAME_SIZE];	// 管理名称
	CGameObject* gameobj;	// オブジェクトのポインタ
	bool autodelete;		// リスト削除と同時にオブジェクトも削除
};

class DECLSPEC CEnumeration
{
protected:
	list<ListData>::iterator ipos, iend;
public:
	virtual CGameObject* GetNext() = 0;
};

class DECLSPEC CKeyEnum : public CEnumeration
{
private:
	char keyword[NAME_SIZE];
public:
	friend CGameObject;
	virtual CGameObject* GetNext();
};

class DECLSPEC CPrioEnum : public CEnumeration
{
private:
	int start, end;
public:
	friend CGameObject;
	CGameObject* GetNext();
};

class DECLSPEC CGameObject
{
private:
	static HWND hWnd;
	static HINSTANCE hInstance;

	friend int APIENTRY _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int);

	BOOL Initialize(HWND hWnd, HINSTANCE hInstance);
	void Uninitialize();
	void DoAllTasks();

	// リストの宣言
	static list<ListData> objectlist;

	// アイテムボックスの宣言
	static hash_map<string, CGameObject*> itembox;

	// タスク処理時における仮イテレータ
	static bool it_moved;
	static list<ListData>::iterator it_task;

public:
	virtual ~CGameObject(){}

	static const HWND GetHWnd(){ return hWnd; }
	static const HINSTANCE GetHInstance(){ return hInstance; }

	static void AppendObject(ListData &listdata);
	static void AppendObject(CGameObject *object, int priority, bool autodelete);
	static void AppendObject(CGameObject *object,
		int priority, LPCSTR name, bool autodelete);

	static void RemoveObject(CGameObject *target);
	static void RemoveObject(int p_begin, int p_end);

	static CGameObject* FindObject(char *name);

	static void CreateEnumeration(const char *keyword, CKeyEnum *cenum);
	static void CreateEnumeration(int p_begin, int p_end, CPrioEnum *cenum);

	static void AppendItemBox(LPCSTR name, CGameObject *object);
	static void RemoveItemBox(LPCSTR name);
	static void ResetItemBox();
	static CGameObject* FindItemBox(LPCSTR name);

protected:
	static LPDIRECT3D9 pD3D;
	static LPDIRECT3DDEVICE9 pD3Ddevice;
	static LPD3DXSPRITE pSprite;

	virtual void Init(){}
	virtual void Exec(){}
};
