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

	BYTE cpos;	// ��_�̈ʒu
	float orig_x, orig_y;	// ���_�̈ʒu
	int texwidth;	// �e�N�X�`���̕��i�c������T�C�Y���O��j

	int swidth, sheight;	// �X�v���C�g�̃T�C�Y
	RECT drawrect;			// �]���͈�

	void Reset();

public:
	CSprite();
	CSprite(CTexture *source);

	void SetTexture(CTexture *source);

	void SetCenterPosition(BYTE pos);
	BYTE GetCenterPosition();

	void GetSpriteSize(int *width, int *height);
	void SetSpriteSize(int width, int height);

	// �t���[���P�ʂł͂Ȃ�����͈̔͂̃O���t�B�b�N��]�������Ƃ��ɂ�
	// SetSpriteRect()�Œ��ڔ͈͂��w��ł���悤�ɂ���
	void SetSpriteRect(int left, int top, int right, int bottom);

	void SetFrame(int frame);

	void Draw(float x, float y, int alpha = 255);
	void Draw(float x, float y, float r, int alpha = 255);
	void Draw(float x, float y, float ex, float ey, int alpha = 255);
	void Draw(float x, float y, float ex, float ey, float r, int alpha = 255);
};
