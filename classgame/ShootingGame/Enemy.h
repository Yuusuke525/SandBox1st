#pragma once

#include "EnemyBase.h"

class CEnemyPattern : public CGameObject
{
public:
    CEnemyPattern();
private:
    int frame;
protected:
    virtual void Exec();
};

class CEnemy1 : public CEnemyBase
{
public:
	CEnemy1(float sx);
private:
	int frame, animframe;
	bool left;
	float accel_x;
protected:
	virtual void Init();
	virtual void Exec();
};

class CEnemy2 : public CEnemyBase
{
private:
	bool shoot;	// 発射済みかどうかのフラグ
public:
	CEnemy2(float sx);
protected:
	virtual void Init();
	virtual void Exec();
};

class CEnemy3 : public CEnemyBase
{
private:
	bool left;		// 自身の初期位置は画面の左かどうか
	bool shoot;		// 弾発射フラグ
	int frame;		// 登場してから経過したフレーム数
public:
	CEnemy3(float sx);
protected:
	virtual void Init();
	virtual void Exec();
public:
	virtual void Damaged();	// 中ボスなので雑魚とは違う爆発にする
};