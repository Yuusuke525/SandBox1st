#pragma once

#include "Texture.h"

#define CP_MIDDLECENTER 0
#define CP_LEFT 1
#define CP_CENTER 2
#define CP_RIGHT 4
#define CP_TOP 8
#define CP_MIDDLE 16
#define CP_BOTTOM 32

class DECLSPEC CSprite : public CGameObject
{
private:
	CTexture *texture;

	BYTE cpos;	// 基点の位置
	float orig_x, orig_y;	// 原点の位置
	int texwidth;	// テクスチャの幅（縦横同一サイズが前提）

	int swidth, sheight;	// スプライトのサイズ
	RECT drawrect;			// 転送範囲

	void Reset();

public:
	CSprite();
	CSprite(CTexture *source);

	void SetTexture(CTexture *source);

	void SetCenterPosition(BYTE pos);
	BYTE GetCenterPosition();

	void GetSpriteSize(int *width, int *height);
	void SetSpriteSize(int width, int height);

	// フレーム単位ではなく特定の範囲のグラフィックを転送したときには
	// SetSpriteRect()で直接範囲を指定できるようにする
	void SetSpriteRect(int left, int top, int right, int bottom);

	void SetFrame(int frame);

	void Draw(float x, float y, int alpha = 255);
	void Draw(float x, float y, float r, int alpha = 255);
	void Draw(float x, float y, float ex, float ey, int alpha = 255);
	void Draw(float x, float y, float ex, float ey, float r, int alpha = 255);
};
