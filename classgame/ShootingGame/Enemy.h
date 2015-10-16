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
	bool shoot;	// ���ˍς݂��ǂ����̃t���O
public:
	CEnemy2(float sx);
protected:
	virtual void Init();
	virtual void Exec();
};

class CEnemy3 : public CEnemyBase
{
private:
	bool left;		// ���g�̏����ʒu�͉�ʂ̍����ǂ���
	bool shoot;		// �e���˃t���O
	int frame;		// �o�ꂵ�Ă���o�߂����t���[����
public:
	CEnemy3(float sx);
protected:
	virtual void Init();
	virtual void Exec();
public:
	virtual void Damaged();	// ���{�X�Ȃ̂ŎG���Ƃ͈Ⴄ�����ɂ���
};