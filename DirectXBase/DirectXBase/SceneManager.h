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


// 引数 nextScene にシーンを変更する
void SceneMgr_ChangeScene(eScene nextScene);
