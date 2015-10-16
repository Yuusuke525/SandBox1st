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
		DXTRACE_MSG(_T("DirectXDevice�̏������Ɏ��s���܂���"));
		return FALSE;
	}

	D3DDISPLAYMODE d3ddm;
	if (FAILED(pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm))){
		DXTRACE_MSG(_T("DirectX3DDevice�̏������Ɏ��s���܂���"));
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
		DXTRACE_MSG(_T("3DDeviceObject�̏������Ɏ��s�܂���"));
		return FALSE;
	}

	pD3Ddevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pD3Ddevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pD3Ddevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	if (FAILED(D3DXCreateSprite(pD3Ddevice, &pSprite))){
		DXTRACE_MSG(_T("SpriteObject�̍쐬�Ɏ��s���܂���"));
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
//	// �������֐������s  
//	listdata.gameobj->Init();
//
//	// ���X�g�̐擪�ɒǉ�  
//	if (objectlist.size() == 0){
//		objectlist.push_front(listdata);
//		return;
//	}
//
//	// �I�u�W�F�N�g�̏��ʂɏ]���}��  
//	list<ListData>::iterator i;
//	for (i = objectlist.begin(); i != objectlist.end(); i++)
//	{
//		if ((*i).priority > listdata.priority){
//			objectlist.insert(i, listdata);
//			return;
//		}
//	}
//
//	// �Ԃɓ����Ƃ��낪�Ȃ���΁A���X�g�̍Ō�ɒǉ�  
//	objectlist.push_back(listdata);
//}
//
//void CMyLib::AppendObject(CMyLib *object,
//	int priority, bool autodelete)
//{
//	// ���O�𖢋L�������ɂ��ē]��  
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
//			// ���Ɉړ�  
//			it_task++;
//		}
//		else{
//			// �t���O�����Z�b�g  
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