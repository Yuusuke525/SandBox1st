#include "Sprite.h"

CSprite::CSprite()
{
	Reset();
}

CSprite::CSprite(CTexture *source)
{
	Reset();
	SetTexture(source);
}

void CSprite::Reset()
{
	texture = NULL;
	cpos = CP_MIDDLECENTER;
	orig_x = orig_y = 0.0f;
	texwidth = 0;
	swidth = sheight = 0;
	SetRect(&drawrect, 0, 0, 0, 0);
}

void CSprite::SetTexture(CTexture *source)
{
	if(source == NULL) return;
	texture = source;

	int h;
	texture->GetTextureSize(&texwidth, &h);
	SetSpriteSize(texwidth, h);
}

void CSprite::SetSpriteSize(int width, int height)
{
	// スプライトのサイズを変更
	swidth = width;
	sheight = height;

	// 原点位置の更新
	SetCenterPosition(cpos);

	// フレームのリセット
	SetFrame(0);
}

void CSprite::GetSpriteSize(int *width, int *height)
{
	*width = swidth;
	*height = sheight;
}

void CSprite::SetSpriteRect(int left, int top, int right, int bottom)
{
	swidth = right - left;
	sheight = bottom - top;

	drawrect.left = left;
	drawrect.right = right;
	drawrect.top = top;
	drawrect.bottom = bottom;

	SetCenterPosition(cpos);
}

void CSprite::SetCenterPosition(BYTE pos)
{
	// 原点の位置を計算
	cpos = pos;

	if(pos & CP_LEFT){
		orig_x = 0.0f;
	}else if(pos & CP_RIGHT){
		orig_x = (float)swidth;
	}else{
		orig_x = (float)swidth / 2.0f;
	}

	if(pos & CP_TOP){
		orig_y = 0.0f;
	}else if(pos & CP_BOTTOM){
		orig_y = (float)sheight;
	}else{
		orig_y = (float)sheight / 2.0f;
	}
}

BYTE CSprite::GetCenterPosition()
{
	return cpos;
}

void CSprite::SetFrame(int frame)
{
	// 指定したフレーム値より転送範囲を計算
	if(swidth == 0) return;
	int d = texwidth / swidth;
	if(d == 0) return;

	int xpos = frame % d;
	int ypos = frame / d;

	drawrect.left = xpos * swidth;
	drawrect.right = drawrect.left + swidth;
	drawrect.top = ypos * sheight;
	drawrect.bottom = drawrect.top + sheight;
}

void CSprite::Draw(float x, float y, int alpha)
{
	Draw(x, y, 1.0f, 1.0f, 0.0f, alpha);
}

void CSprite::Draw(float x, float y, float ex, float ey, int alpha)
{
	Draw(x, y, ex, ey, 0.0f, alpha);
}

void CSprite::Draw(float x, float y, float r, int alpha)
{
	Draw(x, y, 1.0f, 1.0f, r, alpha);
}

void CSprite::Draw(float x, float y, float ex, float ey, float r, int alpha)
{
	if(texture == NULL){
		DXTRACE_MSG(_T("テクスチャが読み込まれていません"));
		return;
	}

	D3DXMATRIX mtrx1, mtrx2;

	// 原点を重ね合わせる平行移動
	D3DXMatrixTranslation(&mtrx1, -orig_x, -orig_y, 0.0f);

	// 拡大行列と合成
	if(ex != 1.0f || ey != 1.0f){
		D3DXMatrixScaling(&mtrx2, ex, ey, 1.0f);
		D3DXMatrixMultiply(&mtrx1, &mtrx1, &mtrx2);
	}

	// 回転行列との合成
	if(r != 0.0f){
		D3DXMatrixRotationZ(&mtrx2, r);
		D3DXMatrixMultiply(&mtrx1, &mtrx1, &mtrx2);
	}

	// 指定の場所へ移動する行列との合成
	D3DXMatrixTranslation(&mtrx2, x, y, 0.0f);
	D3DXMatrixMultiply(&mtrx1, &mtrx1, &mtrx2);


	pSprite->Begin(NULL);

	pSprite->SetTransform(&mtrx1);
	pSprite->Draw(texture->GetTexture(), &drawrect, NULL, NULL, 0x00FFFFFF | ((BYTE)alpha << 24));

	pSprite->End();
}

