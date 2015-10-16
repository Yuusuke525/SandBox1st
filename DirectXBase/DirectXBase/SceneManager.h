#pragma once
#include "EditScene.h"
typedef enum{
	Init_EditScene,
	Scene_EditScene,
	Exit_EditScene,
	Scene_title,
	Scene_map,
	Scene_hantei
}eScene;

void SceneMgr_Update(IDirect3DDevice9* pD3Device, int WINDOW_WIDTH, int WINDOW_HEIGHT);


// ˆø” nextScene ‚ÉƒV[ƒ“‚ğ•ÏX‚·‚é
void SceneMgr_ChangeScene(eScene nextScene);
