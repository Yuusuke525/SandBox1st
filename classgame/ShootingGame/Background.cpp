#include "BackGround.h"

// 1���C��������̏c��
#define LINEHEIGHT 150.0f

CBackGround::CBackGround(void)
{
	// ���C��ԂŁA�ŏ㕔�̃��C���Ǘ��ԍ������ԂɂȂ邩���v�Z
	linepos = (int)((480.0f + LINEHEIGHT) / LINEHEIGHT) + 1;

	// ��ԏ�̃��C����`�悷��ʒu
	pos_y = -LINEHEIGHT;
}

void CBackGround::Init()
{
	// �e�N�X�`���̓ǂݍ��݁E�֘A�Â�
	tex.Load(_T("data\\bg.png"));

	base_left.SetTexture(&tex);
	base_left.SetSpriteRect(0, 0, 320, 150);
	base_left.SetCenterPosition(CP_TOP | CP_LEFT);

	base_right.SetTexture(&tex);
	base_right.SetSpriteRect(0, 150, 320, 300);
	base_right.SetCenterPosition(CP_TOP | CP_RIGHT);

	pipe_left.SetTexture(&tex);
	pipe_left.SetSpriteRect(320, 0, 448, 30);
	pipe_left.SetCenterPosition(CP_TOP | CP_LEFT);

	pipe_right.SetTexture(&tex);
	pipe_right.SetSpriteRect(320, 30, 448, 60);
	pipe_right.SetCenterPosition(CP_TOP | CP_RIGHT);
}

void CBackGround::Exec()
{
	// ���C�����Ƃ̃X�v���C�g�z�u
	int bc = linepos;
	for(float y = pos_y; y < 480.0f; y += LINEHEIGHT, bc--){
		if(bc % 4 == 0){
			// ���C���Ǘ��ԍ����S�Ŋ���؂��i�܂�S��ɂP��j
			// �̂ł���΁A���ʂȃX�v���C�g��z�u����
			base_left.Draw(0.0f, y);
			base_right.Draw(640.0f, y);
		}else{
			for(float yy = 0; yy < 150.0f; yy += 30.0f){
				pipe_left.Draw(0.0f, y + yy);
				pipe_right.Draw(640.0f, y + yy);
			}
		}
	}

	// �X�N���[������
	pos_y += 2.0f;
	if(pos_y >= 0.0f){
		// ��ԏ�̃��C�����`��̈�ɓ������̂ł����
		// �V�������C����ǉ����邽�߁A���C���Ǘ��ԍ����ꑝ�₷
		linepos++;
		pos_y = -LINEHEIGHT;
	}
}