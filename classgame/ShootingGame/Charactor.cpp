#include "Charactor.h"

RECTF::RECTF()
{
	left = top = right = bottom = 0.0f;
}

RECTF::RECTF(float x1, float y1, float x2, float y2, bool center)
{
	if(center == true){
		// x1, y1は矩形の中央座標、
		// x2, y2は横幅、縦幅とみなす
		left = x1 - x2;
		right = x1 + x2;
		top = y1 - y2;
		bottom = y1 + y2;
	}else{
		// x1,y1は左上、x2,y2は右下の座標と見なす
		SetRect(x1, y1, x2, y2);
	}
}

void RECTF::SetRect(float left, float top, float right, float bottom)
{
	// 座標を格納する
	this->left = left;
	this->right = right;
	this->top = top;
	this->bottom = bottom;
}

void RECTF::Shift(float mx, float my)
{
	// 矩形の座標を指定値だけずらす
	left += mx;
	right += mx;
	top += my;
	bottom += my;
}

void RECTF::ReverseX()
{
	// y軸を中心に座標を反転する
	left = -left;
	right = -right;

	// 左右の大小関係が逆転するので修正
	float f = left;
	left = right;
	right = f;
}

void RECTF::ReverseY()
{
	// x軸を中心に座標を反転する
	top = -top;
	bottom = -bottom;

	float f = top;
	top = bottom;
	bottom = f;
}

void RECTF::Inflate(float x, float y)
{
	// 矩形を指定値分拡大する
	left -= x;
	right += x;
	top -= y;
	bottom += y;
}

void RECTF::Deflate(float x, float y)
{
	// 矩形を指定値分縮小する
	left += x;
	right -= x;
	if(left > right) left = right = (left - right) / 2.0f;

	top += y;
	bottom -= y;
	if(top > bottom) top = bottom = (top - bottom) / 2.0f;
}

bool RECTF::IsCross(RECTF *target)
{
	// targetの矩形とこのクラスの矩形が重なるか判定
	if(this->left <= target->right && target->left <= this->right
	&& this->top <= target->bottom && target->top <= this->bottom)
	{
		return true;
	}

	return false;
}

void RECTF::operator =(const RECTF &origin)
{
	// クラスの代入によるデータのコピー
	SetRect(origin.left, origin.top, origin.right, origin.bottom);
}

///////////////////////////////////////////////////////////////
// CCharactor
///////////////////////////////////////////////////////////////

CCharactor::CCharactor()
{
	x = y = 0.0f;
}

bool CCharactor::HitTest(CCharactor *target)
{
	RECTF r1, r2;
	this->GetHitRect(r1);
	target->GetHitRect(r2);

	return r1.IsCross(&r2);
}

void CCharactor::GetSpriteRect(RECTF &result)
{
	int w, h;
	sprite.GetSpriteSize(&w, &h);

	BYTE pos = sprite.GetCenterPosition();
	if(pos & CP_LEFT){
		result.left = x;
		result.right = result.left + (float)w;
	}else if(pos & CP_RIGHT){
		result.left = x - (float)w;
		result.right = x;
	}else{
		result.left = x - (float)w / 2.0f;
		result.right = result.left + (float)w;
	}

	if(pos & CP_TOP){
		result.top = y;
		result.bottom = result.top + (float)h;
	}else if(pos & CP_BOTTOM){
		result.top = y - (float)h;
		result.right = y;
	}else{
		result.top = y - (float)h / 2.0f;
		result.bottom = result.top + (float)h;
	}
}

void CCharactor::GetHitRect(RECTF &result)
{
	GetSpriteRect(result);
}