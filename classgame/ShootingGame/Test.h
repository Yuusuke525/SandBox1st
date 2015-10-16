#pragma once

#include "GameDef.h"

class CTest : public CGameObject
{
private:
	int frame;
protected:
	virtual void Init();
	virtual void Exec();
};
