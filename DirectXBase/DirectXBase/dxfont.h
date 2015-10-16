#pragma once
#include "direct3D/direct3D.h"
#include <windows.h>
#include <tchar.h>
class CDxFont {
private:
	LPD3DXSPRITE	pSprite;
	LPD3DXFONT font;
	IDirect3DDevice9* pD3Ddevice;
public:
	CDxFont(IDirect3DDevice9* pD3Device);

	CDxFont(int fontsize);
	~CDxFont();

	BOOL Create(int fontsize);
	void Draw(LPCTSTR text, int x, int y);
	void Draw(LPCTSTR text, int count,
		LPRECT pRect, DWORD Format, D3DXCOLOR Color);

	const LPD3DXFONT GetD3DXFont();
};

