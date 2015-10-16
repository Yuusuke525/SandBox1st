#pragma once
#include "Charactor.h"

#define EXPLOSION_PRIORITY 90000

class CExplosion : public CCharactor
{
public:
	CExplosion(float sx, float sy);
	CExplosion(float sx, float sy, float size, float angle, float speed);
private:
	void Reset(float sx, float sy, float size, float angle, float speed);
	int frame, animframe;
	float mx, my, exsize;
protected:
	virtual void Init();
	virtual void Exec();
};
