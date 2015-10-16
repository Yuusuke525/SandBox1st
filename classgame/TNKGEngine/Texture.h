#pragma once

#include "GameObject.h"

class DECLSPEC CTexture : public CGameObject
{
private:
	LPDIRECT3DTEXTURE9 texture;
	int texwidth, texheight;

public:
	CTexture();
	CTexture(LPCTSTR filename);
	~CTexture();

	BOOL Load(LPCTSTR filename);

	void GetTextureSize(int *width, int *height);

	const LPDIRECT3DTEXTURE9 GetTexture(){ return texture; }
};
