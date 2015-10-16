#include "Test.h"

#include "Player.h"
#include "Enemy.h"
#include "Boss.h"
#include "Background.h"

void CTest::Init()
{
	// テクスチャの作成・格納
	CTexture *tex;

	tex = new CTexture(_T("data\\zako.png"));
	AppendItemBox("zakotex", tex);
	AppendObject(tex, 1000, true);

	tex = new CTexture(_T("data\\cboss.png"));
	AppendItemBox("cboss", tex);
	AppendObject(tex, 1000, true);

	tex = new CTexture(_T("data\\item.png"));
	AppendItemBox("item", tex);
	AppendObject(tex, 1000, true);

	tex = new CTexture(_T("data\\boss.png"));
	AppendItemBox("bosstex", tex);
	AppendObject(tex, 1000, true);

	AppendObject(new CBackGround(), BACKGROUND_PRIORITY, true);

	tex = new CTexture(_T("data\\explode.png"));
	AppendItemBox("explode", tex);
	AppendObject(tex, 1000, true);

	tex = new CTexture(_T("data\\pbullet.png"));
	AppendItemBox("pbullet", tex);
	AppendObject(tex, 1000, true);

	tex = new CTexture(_T("data\\ebullet.png"));
	AppendItemBox("ebullet", tex);
	AppendObject(tex, 1000, true);

	// プレイヤーデータの作成
	CPlayer *player = new CPlayer();
	AppendItemBox("player", player);
	AppendObject(player, PLAYER_PRIORITY, true);

	frame = 0;
}

void CTest::Exec()
{
	frame++;
	if(frame > 300){
		AppendObject(new CBoss(), ENEMY_PRIORITY, true);
		RemoveObject(this);
	}

	frame++;
	if(frame > 200 && frame % 1000 == 0){
		CEnemy3 *enemy = new CEnemy3((float)(rand() % 640));
		AppendObject(enemy, ENEMY_PRIORITY, true);
	}
}