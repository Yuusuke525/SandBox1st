#pragma once

#include "Charactor.h"

#define PLAYER_PRIORITY 50000
#define BULLET_PRIORITY 55000

class CPlayer : public CCharactor
{
public:
	CPlayer();

private:
	CTexture tex_player;
	CInput *input;
	CSound missile;

protected:
	virtual void Init();
	virtual void Exec();

private:
	int shoottime;	// ç≈å„Ç…î≠éÀÇ≥ÇÍÇƒÇ©ÇÁÇÃéûä‘
	void Shoot();

private:
	bool destroyed;
public:
	void Destroy();

private:
	bool noenemy;
public:
	void SetNoEnemy(bool flag);
	virtual void GetHitRect(RECTF &result);

private:
	bool powerup;
public:
	void PowerUp(){ powerup = true; }
};

class CPlayerBullet : public CCharactor
{
public:
	CPlayerBullet(float sx, float sy, float moveangle);
private:
	float angle;
protected:
	virtual void Init();
	virtual void Exec();
};

class CPowerupItem : public CCharactor
{
private:
	int frame, animframe;
	CPlayer *player;
public:
	CPowerupItem(float sx, float sy);
protected:
	virtual void Init();
	virtual void Exec();
};