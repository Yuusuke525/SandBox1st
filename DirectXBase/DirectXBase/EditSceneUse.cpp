#include "EditScene.h"
#include "SceneManager.h"
void Init(IDirect3DDevice9* pD3Device, int WINDOW_WIDTH, int WINDOW_HEIGHT){
	C_EDIT *edit = new C_EDIT(pD3Device, WINDOW_WIDTH, WINDOW_HEIGHT);
	SceneMgr_ChangeScene(Scene_EditScene);
}
