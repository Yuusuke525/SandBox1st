#pragma once

#include "Task.h"


// タイトルモード
class CTitle : public CGameObject
{
private:
	CInput *input;

	CDxFont message;
	CTexture titletex;
	CSprite titlesp_left, titlesp_right;

	float rate;
protected:
	virtual void Init();
	virtual void Exec();
public:
	CTitle();
};


// ゲームクリア・ゲームオーバーモード
#define CONTINUE_PRIORITY 100000

class CContinue : public CGameObject
{
public:
	CContinue(bool gameclear);
private:
	CInput *input;
	bool clear;
	CDxFont msg1, msg2;
protected:
	virtual void Init();
	virtual void Exec();
};



class CLoadStage : public CLoadData
{
protected:
	virtual void Init();
	static void LoadThread(void *data);
};
