#include "Mode.h"

#include "player.h"
#include "Enemy.h"
#include "background.h"

/////////////////////////////////////////////////////////////////////

CTitle::CTitle()
{
	rate = 0.0f;
}

void CTitle::Init()
{
	titletex.Load(_T("data\\title.png"));

	titlesp_left.SetTexture(&titletex);
	titlesp_left.SetSpriteRect(0, 0, 242, 125);
	titlesp_left.SetCenterPosition(CP_RIGHT | CP_BOTTOM);

	titlesp_right.SetTexture(&titletex);
	titlesp_right.SetSpriteRect(0, 125, 242, 250);
	titlesp_right.SetCenterPosition(CP_LEFT | CP_BOTTOM);

	message.Create(16);

	input = (CInput*)FindObject("input");
	if(input == NULL){
		input = new CInput();
		AppendObject(input, 0, "input", true);
	}
}

void CTitle::Exec()
{
	// タイトルをズームイン
	rate += 0.02f;
	if(rate > 1.0f) rate = 1.0f;

	titlesp_left.Draw(320.0f, 200.0f, rate, rate);
	titlesp_right.Draw(320.0f, 200.0f, rate, rate);


	// メッセージを表示
	RECT rc;
	SetRect(&rc, 0, 350, 640, 400);
	message.Draw(_T("Press 1 OR Enter"), -1, &rc,
		DT_TOP | DT_CENTER | DT_SINGLELINE, 0xFFFFFFFF);


	// スタートボタンが押されたら、メインモードに移行する
	if(input->IsButtonPressed(0) || input->IsKeyPressed(DIK_RETURN)){
		RemoveObject(1, INT_MAX - 1);
		AppendObject(new CLoadStage(), 1, true);
		return;
	}
}

/////////////////////////////////////////////////////////////////////

// 引数gameclearがtrueならゲームクリアモード
// falseならゲームオーバーモード
CContinue::CContinue(bool gameclear)
{
	clear = gameclear;
}

void CContinue::Init()
{
	input = (CInput*)FindObject("input");
	msg1.Create(48);
	msg2.Create(16);

	// 敵の出現をやめる
	RemoveObject(FindObject("epattern"));
}

void CContinue::Exec()
{
	// メッセージの表示
	const TCHAR m1[] = _T("GAME CLEAR");
	const TCHAR m2[] = _T("GAME OVER");

	RECT rc;

	SetRect(&rc, 0, 0, 640, 480);
	msg1.Draw(clear ? m1 : m2, -1, &rc,
		DT_VCENTER | DT_CENTER | DT_SINGLELINE, 0xFFFFFFFF);

	SetRect(&rc, 0, 350, 640, 400);
	msg2.Draw(_T("Press 1 OR Enter"), -1, &rc,
		DT_TOP | DT_CENTER | DT_SINGLELINE, 0xFFFFFFFF);

	if(input->IsButtonPressed(0) || input->IsKeyPressed(DIK_RETURN)){
		if(clear == true){
			// タイトルに戻る
			AppendObject(new CTitle(), 1, true);
		}else{
			// ステージデータを読み直す
			AppendObject(new CLoadStage(), 1, true);
		}
		RemoveObject(10, INT_MAX - 1);
		return;
	}
}

/////////////////////////////////////////////////////////////////////

void CLoadStage::Init()
{
	// 別スレッドでロードを開始
	_beginthread(this->LoadThread, 0, NULL);
	RemoveObject(this);
}

void CLoadStage::LoadThread(void *data)
{
	// ここで時間のかかる処理を実行
	ListData ld;

	ResetItemBox();

	// 背景
	ld.gameobj = new CBackGround();
	ld.autodelete = true;
	strcpy_s(ld.name, 32, "bg");
	ld.priority = BACKGROUND_PRIORITY;
	temp.push_back(ld);

	// プレイヤー
	ld.gameobj = new CPlayer();
	ld.autodelete = true;
	strcpy_s(ld.name, 32, "player");
	ld.priority = PLAYER_PRIORITY;
	temp.push_back(ld);
	AppendItemBox("player", ld.gameobj);

	// 敵出現ルーチン
	ld.gameobj = new CEnemyPattern();
	ld.autodelete = true;
	strcpy_s(ld.name, 32, "epattern");
	ld.priority = 100;
	temp.push_back(ld);


	CTexture *tex;

	tex = new CTexture(_T("data\\explode.png"));
	AppendItemBox("explode", tex);
	AppendObject(tex, 1000, true);

	tex = new CTexture(_T("data\\pbullet.png"));
	AppendItemBox("pbullet", tex);
	AppendObject(tex, 1000, true);

	tex = new CTexture(_T("data\\ebullet.png"));
	AppendItemBox("ebullet", tex);
	AppendObject(tex, 1000, true);

	// 雑魚敵のテクスチャ
	tex = new CTexture(_T("data\\zako.png"));
	AppendItemBox("zakotex", tex);
	AppendObject(tex, 1000, true);

	tex = new CTexture(_T("data\\cboss.png"));
	AppendItemBox("cboss", tex);
	AppendObject(tex, 1000, true);

	// アイテムのテクスチャ
	tex = new CTexture(_T("data\\item.png"));
	AppendItemBox("item", tex);
	AppendObject(tex, 1000, true);

	// ボスのテクスチャ
	tex = new CTexture(_T("data\\boss.png"));
	AppendItemBox("bosstex", tex);
	AppendObject(tex, 1000, true);



	// ミサイル発射効果音
	ld.gameobj = new CSound(_T("data\\missile.wav"));
	ld.autodelete = true;
	strcpy_s(ld.name, 32, "missile");
	ld.priority = 1000;
	temp.push_back(ld);
	AppendItemBox("missile", ld.gameobj);

	// 爆発効果音1
	ld.gameobj = new CSound(_T("data\\explode1.wav"));
	ld.autodelete = true;
	strcpy_s(ld.name, 32, "explode1");
	ld.priority = 1000;
	temp.push_back(ld);
	AppendItemBox("explode1", ld.gameobj);

	// 爆発効果音2
	ld.gameobj = new CSound(_T("data\\explode2.wav"));
	ld.autodelete = true;
	strcpy_s(ld.name, 32, "explode2");
	ld.priority = 1000;
	temp.push_back(ld);
	AppendItemBox("explode2", ld.gameobj);

	// 爆発効果音3
	ld.gameobj = new CSound(_T("data\\explode3.wav"));
	ld.autodelete = true;
	strcpy_s(ld.name, 32, "explode3");
	ld.priority = 1000;
	temp.push_back(ld);
	AppendItemBox("explode3", ld.gameobj);

	// メインＢＧＭ
	ld.gameobj = new CBGMusic(_T("data\\bgm.mp3"));
	ld.autodelete = true;
	strcpy_s(ld.name, 32, "bgm1");
	ld.priority = 1000;
	temp.push_back(ld);

	// ボスＢＧＭ
	ld.gameobj = new CBGMusic(_T("data\\boss.mp3"));
	ld.autodelete = true;
	strcpy_s(ld.name, 32, "bgm2");
	ld.priority = 1000;
	temp.push_back(ld);

	complete = true;
}
