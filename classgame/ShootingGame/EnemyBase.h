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
	CPlayer *player;		// �^�[�Q�b�g�ƂȂ�v���C���[�N���X
	float move_x, move_y;	// �P�t���[�����Ƃ̈ړ�����
public:
	// sx,sy:�����ʒu angle:�ړ����� speed:�ړ�����
	CEnemyBullet(float sx, float xy, float angle, float speed);
	virtual void Init();
	virtual void Exec();
};