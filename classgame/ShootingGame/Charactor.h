#pragma once

#include "GameDef.h"

class RECTF
{
public:
	// —Ìˆæl‹÷‚Ì”’l‚ğŠi”[
	float left, top, right, bottom;

	RECTF();
	RECTF(float x1, float y1, float x2, float y2, bool center = false);

	void SetRect(float left, float top, float right, float bottom);

	void Shift(float mx, float my);
	void ReverseX();
	void ReverseY();
	void Inflate(float x, float y);
	void Deflate(float x, float y);
	bool IsCross(RECTF *target);

	void operator =(const RECTF& origin);  
};

class CCharactor : public CGameObject
{
public:
	CCharactor();

protected:
	CSprite sprite;
	float x, y;

public:
	float GetPosX(){ return x; }
	float GetPosY(){ return y; }

	void GetSpriteRect(RECTF &result);
	virtual void GetHitRect(RECTF &result);
	virtual bool HitTest(CCharactor *target);
};

