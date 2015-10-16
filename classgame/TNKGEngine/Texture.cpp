#include "Texture.h"

CTexture::CTexture()
{
	texture = NULL;
}

CTexture::CTexture(LPCTSTR filename)
{
	texture = NULL;
	Load(filename);
}

CTexture::~CTexture()
{
	RELEASE(texture);
}

void CTexture::GetTextureSize(int *width, int *height)
{
	*width = texwidth;
	*height = texheight;
}

BOOL CTexture::Load(LPCTSTR filename)
{
	RELEASE(texture);
	BOOL b = SUCCEEDED(D3DXCreateTextureFromFile
		(pD3Ddevice, filename, &texture));

	if(b == TRUE){
		D3DSURFACE_DESC desc;

		// 最上位のテクスチャ情報を取得
		texture->GetLevelDesc(0, &desc);

		texwidth = (int)desc.Width;
		texheight = (int)desc.Height;
	}

	return b;
}
