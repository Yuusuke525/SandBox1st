#pragma once

#include "GameDef.h"

#define BACKGROUND_PRIORITY 5000

class CBackGround : public CGameObject
{
private:
	CTexture tex;

	int linepos;	// Åã•”‚Ìƒ‰ƒCƒ“ŠÇ—”Ô†
	float pos_y;
	CSprite base_left, base_right;
	CSprite pipe_left, pipe_right;
public:
	CBackGround(void);
protected:
	virtual void Init();
	virtual void Exec();
};
