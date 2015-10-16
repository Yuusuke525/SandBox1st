#include "SceneManager.h"
eScene Scene = Init_EditScene;


void SceneMgr_Update(IDirect3DDevice9* pD3Device, int WINDOW_WIDTH, int WINDOW_HEIGHT)
{
	switch (Scene)
	{
	case Init_EditScene:
		
		break;
	case Scene_EditScene:

		break;
	case Exit_EditScene:
		break;

	}
}


void SceneMgr_ChangeScene(eScene nextScene)
{
	Scene = nextScene;
}