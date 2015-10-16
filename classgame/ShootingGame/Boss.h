#pragma once
#include "EnemyBase.h"

enum BossStatus
{
	boss_pattern1,	// ���`��
	boss_pattern2,	// ���`��
	boss_pattern3,	// ��O�i�ŏI�j�`��
	boss_explosion	// �������
};

class CBoss : public CEnemyBase
{
public:
	CBoss(void);
protected:
	virtual void Init();
	virtual void Exec();

private:
	CSprite battery;	// �C��̃O���t�B�b�N
	float bangle[2];	// �C�䂪�����Ă���p�x�~�Q
	float bypos[5];		// �C��̓����蔻��̊�ʒu(y���̂�)

	void ShootFromNozzle(float speed);

	BossStatus status;

	int frame;
    int moveflag1, moveflag2;
    float mx, my;

	void Action1();			// ���`�Ԏ��̃A�N�V���������s
	void Action2();			// ���`�Ԏ��̃A�N�V���������s
	void Action3();			// ��O�`�Ԏ��̃A�N�V���������s
	bool ActionDestroy();	// �������̃A�N�V���������s
							// (�߂�ltrue�ŃA�N�V�������I���������Ƃ�`����)

	void MiniExplosion();

private:
	RECTF bodyarea[6];	// �{�̂̓�����̈�
public:
	virtual bool HitTest(CCharactor* target);
	virtual void Damaged();
};
