#include "Charactor.h"

RECTF::RECTF()
{
	left = top = right = bottom = 0.0f;
}

RECTF::RECTF(float x1, float y1, float x2, float y2, bool center)
{
	if(center == true){
		// x1, y1�͋�`�̒������W�A
		// x2, y2�͉����A�c���Ƃ݂Ȃ�
		left = x1 - x2;
		right = x1 + x2;
		top = y1 - y2;
		bottom = y1 + y2;
	}else{
		// x1,y1�͍���Ax2,y2�͉E���̍��W�ƌ��Ȃ�
		SetRect(x1, y1, x2, y2);
	}
}

void RECTF::SetRect(float left, float top, float right, float bottom)
{
	// ���W���i�[����
	this->left = left;
	this->right = right;
	this->top = top;
	this->bottom = bottom;
}

void RECTF::Shift(float mx, float my)
{
	// ��`�̍��W���w��l�������炷
	left += mx;
	right += mx;
	top += my;
	bottom += my;
}

void RECTF::ReverseX()
{
	// y���𒆐S�ɍ��W�𔽓]����
	left = -left;
	right = -right;

	// ���E�̑召�֌W���t�]����̂ŏC��
	float f = left;
	left = right;
	right = f;
}

void RECTF::ReverseY()
{
	// x���𒆐S�ɍ��W�𔽓]����
	top = -top;
	bottom = -bottom;

	float f = top;
	top = bottom;
	bottom = f;
}

void RECTF::Inflate(float x, float y)
{
	// ��`���w��l���g�傷��
	left -= x;
	right += x;
	top -= y;
	bottom += y;
}

void RECTF::Deflate(float x, float y)
{
	// ��`���w��l���k������
	left += x;
	right -= x;
	if(left > right) left = right = (left - right) / 2.0f;

	top += y;
	bottom -= y;
	if(top > bottom) top = bottom = (top - bottom) / 2.0f;
}

bool RECTF::IsCross(RECTF *target)
{
	// target�̋�`�Ƃ��̃N���X�̋�`���d�Ȃ邩����
	if(this->left <= target->right && target->left <= this->right
	&& this->top <= target->bottom && target->top <= this->bottom)
	{
		return true;
	}

	return false;
}

void RECTF::operator =(const RECTF &origin)
{
	// �N���X�̑���ɂ��f�[�^�̃R�s�[
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