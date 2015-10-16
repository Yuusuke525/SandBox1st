#include "GameObject.h"

CGameObject* CKeyEnum::GetNext()
{
	char *p1, *p2;
	while(ipos != iend){
		p1 = keyword;
		p2 = (*ipos).name;

		while(*p1 != '\0'){
			if(*p1 != *p2) break;
			p1++;
			p2++;
		}
		
		if(*p1 == '\0'){
			CGameObject *g = ipos->gameobj;
			ipos++;
			return g;
		}

		ipos++;
	}

	return NULL;
}

CGameObject* CPrioEnum::GetNext()
{
	while(ipos != iend){
		if(ipos->priority > this->end){
			ipos = iend;
			return NULL;
		}else if(ipos->priority >= this->start){
			CGameObject *g = ipos->gameobj;
			ipos++;
			return g;
		}

		ipos++;
	}

	return NULL;
}


HWND		CGameObject::hWnd = NULL;
HINSTANCE	CGameObject::hInstance = NULL;

list<ListData> CGameObject::objectlist;
hash_map<string, CGameObject*> CGameObject::itembox;

bool CGameObject::it_moved;
list<ListData>::iterator CGameObject::it_task;

LPDIRECT3D9			CGameObject::pD3D = NULL;
LPDIRECT3DDEVICE9	CGameObject::pD3Ddevice = NULL;
LPD3DXSPRITE		CGameObject::pSprite = NULL;

BOOL CGameObject::Initialize(HWND hWnd, HINSTANCE hInstance)
{
	this->hWnd = hWnd;
	this->hInstance = hInstance;

	pD3D = Direct3DCreate9(D3D_SDK_VERSION);
    if(pD3D == NULL){
		DXTRACE_MSG(_T("DirectXDeviceの初期化に失敗しました"));
		return FALSE;
    }

    D3DDISPLAYMODE d3ddm;
    if(FAILED(pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
    {
		DXTRACE_MSG(_T("DirectX3DDeviceの初期化に失敗しました"));
		return FALSE;
    }

    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    d3dpp.PresentationInterval  = D3DPRESENT_INTERVAL_IMMEDIATE;

	if(FAILED(pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hWnd,
		D3DCREATE_MIXED_VERTEXPROCESSING,
		&d3dpp,&pD3Ddevice)))
	{
		DXTRACE_MSG(_T("3DDeviceObjectの初期化に失敗しました"));
		return false;
	}

    pD3Ddevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
    pD3Ddevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
    pD3Ddevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );

    if(FAILED(D3DXCreateSprite(pD3Ddevice, &pSprite)))
    {
       DXTRACE_MSG(_T("SpriteObjectの作成に失敗しました"));
       return FALSE;
    }

	return TRUE;
}

void CGameObject::Uninitialize()
{
	RELEASE(pSprite);
	RELEASE(pD3Ddevice);
	RELEASE(pD3D);

	// リストにnewで追加したデータをすべて消去
	list<ListData>::iterator i;
	for(i = objectlist.begin(); i != objectlist.end(); i++)
	{
		if((*i).autodelete == true) delete (*i).gameobj;
	}
}

void CGameObject::AppendObject(ListData &listdata)
{
	if(listdata.gameobj == NULL) return;

	// 初期化関数を実行
	listdata.gameobj->Init();

	// リストの先頭に追加
	if(objectlist.size() == 0){
		objectlist.push_front(listdata);
		return;
	}

	// オブジェクトの順位に従い挿入
	list<ListData>::iterator i;
	for(i = objectlist.begin(); i != objectlist.end(); i++)
	{
		if((*i).priority > listdata.priority){
			objectlist.insert(i, listdata);
			return;
		}
	}

	// 間に入れるところがなければ、リストの最後に追加
	objectlist.push_back(listdata);
}

void CGameObject::AppendObject(CGameObject *object,
	int priority, bool autodelete)
{
	// 名前を未記入扱いにして転送
	AppendObject(object, priority, NULL, autodelete);
}

void CGameObject::AppendObject(CGameObject *object,
	int priority, LPCSTR name, bool autodelete)
{
	ListData ld;
	ld.priority = priority;
	if(name != NULL){
		strcpy_s(ld.name, NAME_SIZE, name);
	}else{
		ZeroMemory(ld.name, NAME_SIZE * sizeof(char));
	}
	ld.gameobj = object;
	ld.autodelete = autodelete;

	AppendObject(ld);
}

void CGameObject::RemoveObject(CGameObject *target)
{
	list<ListData>::iterator i = objectlist.begin();
	while(i != objectlist.end())
	{
		if((*i).gameobj == target){
			// 消去するイテレータが現在タスク処理中のものであれば
			// フラグを立てて、タスクのイテレータを次へ移動
			if(it_task == i){
				it_moved = true;
				it_task++;
			}

			// ゲームオブジェクトを解放
			if(i->autodelete) delete i->gameobj;

			objectlist.erase(i);
			return;
		}else{
			i++;
		}
	}
}

void CGameObject::RemoveObject(int p_begin, int p_end)
{
	list<ListData>::iterator i = objectlist.begin();
	while(i != objectlist.end()){
		if((*i).priority > p_end){
			break;
		}else if((*i).priority >= p_begin){
			if(it_task == i){
				it_moved = true;
				it_task++;
			}

			if(i->autodelete) delete i->gameobj;
			i = objectlist.erase(i);
		}else{
			i++;
		}
	}
}

CGameObject* CGameObject::FindObject(char *name)
{
	list<ListData>::iterator i;
	for(i = objectlist.begin(); i != objectlist.end(); i++){
		if(strcmp((*i).name, name) == 0) return (*i).gameobj;
	}

	return NULL;
}

void CGameObject::DoAllTasks()
{
	it_task = objectlist.begin();

	it_moved = false;
	while(it_task != objectlist.end()){
		it_task->gameobj->Exec();
		if(it_moved == false){
			// 次に移動
			it_task++;
		}else{
			// フラグをリセット
			it_moved = false;
		}
	}
}

void CGameObject::CreateEnumeration(const char *keyword, CKeyEnum *cenum)
{
	cenum->ipos = objectlist.begin();
	cenum->iend = objectlist.end();

	strcpy_s(cenum->keyword, NAME_SIZE, keyword);
}

void CGameObject::CreateEnumeration(int p_begin, int p_end, CPrioEnum *cenum)
{
	cenum->ipos = objectlist.begin();
	cenum->iend = objectlist.end();

	cenum->start = p_begin;
	cenum->end = p_end;
}

void CGameObject::AppendItemBox(LPCSTR name, CGameObject *object)
{
	itembox.insert(make_pair(name, object));
}

void CGameObject::RemoveItemBox(LPCSTR name)
{
	itembox.erase(name);
}

void CGameObject::ResetItemBox()
{
	itembox.clear();
}

CGameObject* CGameObject::FindItemBox(LPCSTR name)
{
	hash_map<string, CGameObject*>::iterator i;
	i = itembox.find(name);

	if(i == itembox.end()) return NULL;

	return i->second;
}
