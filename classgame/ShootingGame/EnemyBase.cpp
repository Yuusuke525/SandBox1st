#include "EnemyBase.h"

#include "Explosion.h"

///////////////////////////////////////////////////////////
// CEnemyBase
///////////////////////////////////////////////////////////

void CEnemyBase::Init()
{
	player = (CPlayer*)FindItemBox("player");
}

void CEnemyBase::Damaged()
{
	// 残り耐久度を１減らす
	hardness--;
	if(hardness == 0){
		CSound *se = new CSound(*(CSound*)FindItemBox("explode1"));
		se->EnableDeleteByEnd();
		se->Play();

		// 爆発処理を追加
		AppendObject(new CExplosion(x, y), EXPLOSION_PRIORITY, true);
		// 自身を削除
		RemoveObject(this);
	}
}


///////////////////////////////////////////////////////////
// CEnemyBullet
///////////////////////////////////////////////////////////

CEnemyBullet::CEnemyBullet(float sx, float sy, float angle, float speed)
{
	x = sx;
	y = sy;
	move_x = cosf(angle) * speed;
	move_y = sinf(angle) * speed;
}

void CEnemyBullet::Init()
{
	sprite.SetTexture((CTexture*)FindItemBox("ebullet"));
	player = (CPlayer*)FindItemBox("player");
}

void CEnemyBullet::Exec()
{
	// プレイヤーとの衝突判定
	if(player->HitTest(this) == true){
		player->Destroy();
		RemoveObject(this);
		return;
	}

	x += move_x;
	y += move_y;
	if(x < -50.0f || x > 700.0f || y < -50.0f || y > 520.0f){
		// 画面外に出たら削除
		RemoveObject(this);
		return;
	}

	sprite.Draw(x, y);
}
