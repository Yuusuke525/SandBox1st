#include "MyLib.h"

HWND		CMyLib::hWnd = NULL;
HINSTANCE	CMyLib::hInstance = NULL;

LPDIRECT3D9			CMyLib::pD3D = NULL;
LPDIRECT3DDEVICE9	CMyLib::pD3Ddevice = NULL;
LPD3DXSPRITE		CMyLib::pSprite = NULL;

BOOL CMyLib::Initialize(HWND hWnd, HINSTANCE hInstance){
	this->hWnd = hWnd;
	this->hInstance = hInstance;

	pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (pD3D == NULL){
		DXTRACE_MSG(_T("DirectXDeviceの初期化に失敗しました"));
		return FALSE;
	}

	D3DDISPLAYMODE d3ddm;
	if (FAILED(pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm))){
		DXTRACE_MSG(_T("DirectX3DDeviceの初期化に失敗しました"));
		return FALSE;
	}

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if (FAILED(pD3D->CreateDevice(
		D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &pD3Ddevice))){
		DXTRACE_MSG(_T("3DDeviceObjectの初期化に失敗ました"));
		return FALSE;
	}

	pD3Ddevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pD3Ddevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pD3Ddevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	if (FAILED(D3DXCreateSprite(pD3Ddevice, &pSprite))){
		DXTRACE_MSG(_T("SpriteObjectの作成に失敗しました"));
		return FALSE;
	}
	return TRUE;
}

void CMyLib::Uninitialize(){
	RELEASE(pSprite);
	RELEASE(pD3Ddevice);
	RELEASE(pD3D);
}

//
//list<ListData>CMyLib::objectlist;
//bool CMyLib::it_moved;
//list<ListData>::iterator CMyLib::it_task;
//
//void CMyLib::AppendObject(ListData &listdata)
//{
//	if (listdata.gameobj == NULL) return;
//
//	// 初期化関数を実行  
//	listdata.gameobj->Init();
//
//	// リストの先頭に追加  
//	if (objectlist.size() == 0){
//		objectlist.push_front(listdata);
//		return;
//	}
//
//	// オブジェクトの順位に従い挿入  
//	list<ListData>::iterator i;
//	for (i = objectlist.begin(); i != objectlist.end(); i++)
//	{
//		if ((*i).priority > listdata.priority){
//			objectlist.insert(i, listdata);
//			return;
//		}
//	}
//
//	// 間に入れるところがなければ、リストの最後に追加  
//	objectlist.push_back(listdata);
//}
//
//void CMyLib::AppendObject(CMyLib *object,
//	int priority, bool autodelete)
//{
//	// 名前を未記入扱いにして転送  
//	AppendObject(object, priority, NULL, autodelete);
//}
//
//void CMyLib::AppendObject(CMyLib *object,
//	int priority, LPCSTR name, bool autodelete)
//{
//	ListData ld;
//	ld.priority = priority;
//	if (name != NULL){
//		strcpy_s(ld.name, NAME_SIZE, name);
//	}
//	else{
//		ZeroMemory(ld.name, NAME_SIZE * sizeof(char));
//	}
//	ld.gameobj = object;
//	ld.autodelete = autodelete;
//
//	AppendObject(ld);
//}
//
//void CMyLib::RemoveObject(CMyLib *target)
//{
//	list<ListData>::iterator i = objectlist.begin();
//	while (i != objectlist.end())
//	{
//		if ((*i).gameobj == target){
//			objectlist.erase(i);
//			return;
//		}
//		else{
//			i++;
//		}
//	}
//}
//
//void CMyLib::DoAllTasks()
//{
//	it_task = objectlist.begin();
//
//	it_moved = false;
//	while (it_task != objectlist.end()){
//		it_task->gameobj->Exec();
//		if (it_moved == false){
//			// 次に移動  
//			it_task++;
//		}
//		else{
//			// フラグをリセット  
//			it_moved = false;
//		}
//	}
//}
//
//CMyLib* CMyLib::FindObject(char *name)
//{
//	list<ListData>::iterator i;
//	for (i = objectlist.begin(); i != objectlist.end(); i++){
//		if (strcmp((*i).name, name) == 0) return (*i).gameobj;
//	}
//
//	return NULL;
//}