#pragma once
#include "EnemyBase.h"

enum BossStatus
{
	boss_pattern1,	// 第一形態
	boss_pattern2,	// 第二形態
	boss_pattern3,	// 第三（最終）形態
	boss_explosion	// 爆発状態
};

class CBoss : public CEnemyBase
{
public:
	CBoss(void);
protected:
	virtual void Init();
	virtual void Exec();

private:
	CSprite battery;	// 砲台のグラフィック
	float bangle[2];	// 砲台が向いている角度×２
	float bypos[5];		// 砲台の当たり判定の基準位置(y軸のみ)

	void ShootFromNozzle(float speed);

	BossStatus status;

	int frame;
    int moveflag1, moveflag2;
    float mx, my;

	void Action1();			// 第一形態時のアクションを実行
	void Action2();			// 第二形態時のアクションを実行
	void Action3();			// 第三形態時のアクションを実行
	bool ActionDestroy();	// 爆発時のアクションを実行
							// (戻り値trueでアクションが終了したことを伝える)

	void MiniExplosion();

private:
	RECTF bodyarea[6];	// 本体の当たり領域
public:
	virtual bool HitTest(CCharactor* target);
	virtual void Damaged();
};
