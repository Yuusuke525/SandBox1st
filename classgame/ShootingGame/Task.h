#pragma once

#include "GameDef.h"

class CTaskHead : public CGameObject
{
private:
	DWORD lasttime;
protected:
	void Init();
	void Exec();
};

class CTaskTail : public CGameObject
{
protected:
	void Exec();
};

class CLoadData : public CGameObject
{
private:
    int brightness;
    CDxFont *font;

public:
    CLoadData();
    virtual ~CLoadData();

protected:
    static bool complete;
    static list<ListData> temp;

    virtual void Exec();
};

