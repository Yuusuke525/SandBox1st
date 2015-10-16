こんばんは
ヒューマンアカデミーゲームカレッジ天野です。
仕事お疲れ様です。

今回連絡されていただいたのは
「シーン管理」について教えてほしかったからです

EditSceneに入っているclass C_EDITを使いたいのですが、

SceneManager.cpp
///////////////////////////////////////////////////////////////////

void SceneMgr_Update(IDirect3DDevice9* pD3Device, int WINDOW_WIDTH, int WINDOW_HEIGHT)
{
	switch (Scene)
	{
	case Init_EditScene:
		C_EDIT *edit = new C_EDIT(pD3Device, WINDOW_WIDTH, WINDOW_HEIGHT);
	SceneMgr_ChangeScene(Scene_EditScene);
		break;
	case Scene_EditScene:
		edit->draw();
		break;
	case Exit_EditScene:
		delete edit;
		break;

	}
}
/////////////////////////////////////////////////////////////////////
実質的な処理としてはこのような流れにしたいのですがこのように記述してもcase内で
宣言すると別のcaseで使えないようなので悩んでいます。

グローバル関数を使わないようにしているのですが難しいです。

どのようにすれば安全に素早く管理できるのでしょうか？