#pragma once

#include "Player.h"

#define ENEMY_PRIORITY 60000

class CEnemyBase : public CCharactor
{
protected:
	CPlayer *player;

	int hardness;
	virtual void Init();
public:
	virtual void Damaged();
};

#define ENEMYBULLET_PRIORITY 65000

class CEnemyBullet : public CCharactor
{
private:
	CPlayer *player;		// ターゲットとなるプレイヤークラス
	float move_x, move_y;	// １フレームごとの移動距離
public:
	// sx,sy:初期位置 angle:移動方向 speed:移動方向
	CEnemyBullet(float sx, float xy, float angle, float speed);
	virtual void Init();
	virtual void Exec();
};