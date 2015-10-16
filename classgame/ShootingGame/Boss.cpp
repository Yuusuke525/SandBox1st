#include "Boss.h"
#include "Explosion.h"
#include "Mode.h"

#define BUTTERY_X 196.0f
#define BUTTERY_Y (-2.0f)

#define BOSS_HARDNESS 500

CBoss::CBoss(void)
{
	status = boss_pattern1;

	x = 320.0f;
	y = -65.0f;

	hardness = BOSS_HARDNESS;

	// �����蔻��̗̈���i�[
	bodyarea[0].SetRect(0,26,83,128);
	bodyarea[1].SetRect(82,46,106,66);
	bodyarea[2].SetRect(105,22,125,46);
	bodyarea[3].SetRect(126,5,148,40);
	bodyarea[4].SetRect(149,25,174,49);
	bodyarea[5].SetRect(174,40,199,69);

	bangle[0] = bangle[1] = 0.0f;
	bypos[0] = -72.0f;
	bypos[1] = -32.0f;
	bypos[2] = 0.0f;
	bypos[3] = 32.0f;
	bypos[4] = 72.0f;
}

void CBoss::Init()
{
	CEnemyBase::Init();

	sprite.SetTexture((CTexture*)FindItemBox("bosstex"));
	sprite.SetSpriteSize(200, 136);
	sprite.SetCenterPosition(CP_LEFT);

	battery.SetTexture((CTexture*)FindItemBox("bosstex"));
	battery.SetSpriteRect(200, 0, 240, 184);

	frame = 0;
}

void CBoss::Exec()
{
    switch(status){
    case boss_pattern1: Action1(); break;
    case boss_pattern2: Action2(); break;
    case boss_pattern3: Action3(); break;
    default:
        if(ActionDestroy() == true){
			// �Q�[���N���A�[��ʂ�\��
			AppendObject(new CContinue(true), CONTINUE_PRIORITY, true);

            // �����t���O���Ԃ��ꂽ�玩�g������
            RemoveObject(this);
            return;
        }
    }

	sprite.Draw(x, y);				// �E�����`��
	sprite.Draw(x, y, -1.0f, 1.0f);	// ���E���]���č������`��

	battery.Draw(x - BUTTERY_X, y - BUTTERY_Y, bangle[0]);
	battery.Draw(x + BUTTERY_X, y - BUTTERY_Y, bangle[1]);

	if(this->HitTest(player) && hardness > 0){
		player->Destroy();
	}
}

bool CBoss::HitTest(CCharactor* target)
{
	if(hardness <= 0) return false;

	RECTF trect, brect;
	target->GetHitRect(trect);

	for(int i = 0; i < 2; i++){
		// �{�̕����Ƃ̓����蔻��
		for(int j = 0; j < 6; j++){
			brect = bodyarea[j];

			// �������̔���p�ɍ��E�t�]������
			if(i == 0) brect.ReverseX();

			// �̈���{�X�̈ʒu�ɍ��킹��
			brect.Shift(x, y - 64.0f);
			if(brect.IsCross(&trect)) return true;
		}

		// �C�䕔���Ƃ̓����蔻��
		for(int j = 0; j < 5; j++){
			// ��]��̍��W���v�Z
			float xx = -sinf(bangle[i]) * bypos[j];
			float yy = cosf(bangle[i]) * bypos[j];

			// ��]��̍��W�ɐ����ړ��ʂ�����
			if(i == 0){
				xx += x - BUTTERY_X;
			}else{
				xx += x + BUTTERY_X;
			}
			yy += y + BUTTERY_Y;
			
			// �̈�ɓK�p
			brect.SetRect(-20.0f, -20.0f, 20.0f, 20.0f);
			brect.Shift(xx, yy);

			if(brect.IsCross(&trect)) return true;
		}
	}

	return false;
}

// �C��ɑ΂��锭�ˌ��̑��Έʒu
#define MUZZLE_X 20.0f
#define MUZZLE_Y 92.0f

void CBoss::ShootFromNozzle(float speed)
{
	// �C�e����e�̔���
	for(int i = 0; i < 2; i++){
		// �C����̒��S�ʒu���v�Z
		float xx = -sinf(bangle[i]) * MUZZLE_Y;
		float yy = cosf(bangle[i]) * MUZZLE_Y;
		if(i == 0){
			xx += x - BUTTERY_X;
		}else{
			xx += x + BUTTERY_X;
		}
		yy += y + BUTTERY_Y;

		// �e�𔭎�
		AppendObject(
			new CEnemyBullet(xx, yy, bangle[i] + d2r(90.0f), speed),
			ENEMYBULLET_PRIORITY, true);
	}
}

void CBoss::MiniExplosion()
{
    // �召����Ȃ�30�̔������g�U������
    float xx, yy, a, s;
    for(int i = 0; i < 30; i++){
        xx = x + (float)(rand() % 300 - 150);
        yy = y + (float)(rand() % 100 - 50);
        a = atan2f(x - xx, yy - y);
        s = 2.0f + 4.0f * (float)rand() / (float)RAND_MAX;
        AppendObject(new CExplosion(xx, yy,
            2.0f, a, s), EXPLOSION_PRIORITY, true);
    }

	((CSound*)FindItemBox("explode2"))->Play();
}

void CBoss::Action1()
{
    // �^���Ɉړ��ŐÎ~
    if(y < 80.0f) y += 2.0f;

    // �C����v���[���[�Ɍ�����
    bangle[0] = -atan2f(
        player->GetPosX() - (x - BUTTERY_X),
        player->GetPosY() - (y + BUTTERY_Y));
    bangle[1] = -atan2f(
        player->GetPosX() - (x + BUTTERY_X),
        player->GetPosY() - (y + BUTTERY_Y));

    frame++;
    if(frame == 80){
        ShootFromNozzle(1.0f);
        frame = 0;
    }
}

void CBoss::Action2()
{
    // ���E�Ɉړ�
    if(moveflag1 == 0){
        x -= 0.5f;
        if(x < 100.0f) moveflag1 = 1;
    }else{
        x += 0.5f;
        if(x > 540.0f) moveflag1 = 0;
    }

    frame++;
    if(frame % 40 == 0){
        ShootFromNozzle(2.0f);
    }

    if(frame == 240){
        // ������������g�U�e
        float range[] = {30.0f, 60.0f, 90.0f, 120.0f, 150.0f};
        for(int i = 0; i < 5; i++){
            AppendObject(
                new CEnemyBullet(x, y + 50.0f, d2r(range[i]), 3.0f),
                ENEMYBULLET_PRIORITY, true);
        }

        frame = 0;
    }
}

void CBoss::Action3()
{
    const float movedata[][2] = 
    {
        {120.0f, 80.0f},
        {220.0f, 180.0f},
        {320.0f, 80.0f},
        {420.0f, 180.0f},
        {520.0f, 80.0f},
    };

    x += mx;
    y += my;

    // ���W�͕��������_�̂��߁A�덷���l�����Ă���
    if(x > movedata[moveflag1][0] - 0.5f
    && x < movedata[moveflag1][0] + 0.5f
    && y > movedata[moveflag1][1] - 0.5f
    && y < movedata[moveflag1][1] + 0.5f)
    {
        // �ڕW�ʒu�ɓ���

        // �덷���Ȃ���
        x = movedata[moveflag1][0];
        y = movedata[moveflag1][1];

        // ���̖ڕW�ʒu�ւ̈ړ��ʂ��v�Z
        if(moveflag2 == 0){
            mx = movedata[moveflag1 + 1][0] - movedata[moveflag1][0];
            my = movedata[moveflag1 + 1][1] - movedata[moveflag1][1];
        
            moveflag1++;
            if(moveflag1 == 4) moveflag2 = 1;
        }else{
            mx = movedata[moveflag1 - 1][0] - movedata[moveflag1][0];
            my = movedata[moveflag1 - 1][1] - movedata[moveflag1][1];
        
            moveflag1--;
            if(moveflag1 == 0) moveflag2 = 0;
        }

        // ���x�𒲐�
        mx /= 80.0f;
        my /= 80.0f;
    }

    frame++;
    if(frame < 400){
        // ���X�ɖC����O���ֈړ�
        bangle[0] = d2r(75.0f * (float)frame / 400.0f);
        bangle[1] = d2r(-75.0f * (float)frame / 400.0f);
    }else if(frame < 600){
        int frame2 = frame - 400;

        // ��C�ɖC�������ɖ߂�
        bangle[0] = d2r(75.0f - 140.0f * (float)frame2 / 200.0f);
        bangle[1] = d2r(-75.0f + 140.0f * (float)frame2 / 200.0f);

        // �C���߂��Ȃ���e���΂�܂�
        if(frame2 % 15 == 0){
            ShootFromNozzle(1.0f);
        }
    }else if(frame < 1000){
        int frame2 = frame - 600;

        // �C������̈ʒu�֖߂�
        bangle[0] = d2r(-65.0f + 65.0f * (float)frame2 / 400.0f);
        bangle[1] = d2r(65.0f - 65.0f * (float)frame2 / 400.0f);
    }else{
        frame = 0;
    }
}

bool CBoss::ActionDestroy()
{
	frame++;
	if(frame < 500){
		// 500�t���[���ڂ܂ł̓{�X���ӂŃ����_���ɔ����𔭐�������
		if(frame % 5 == 0){
			float xx = x + (float)(rand() % 500 - 250);
			float yy = y + (float)(rand() % 300 - 150);
			float sz = 1.0f + 2.0f * (float)rand() / (float)RAND_MAX;
			AppendObject(new CExplosion(xx, yy, sz, 0.0f, 0.0f),
				EXPLOSION_PRIORITY, true);
		}

		y += 0.2f;

		if(frame % 15 == 0){
			((CSound*)FindItemBox("explode1"))->Play();
		}

		return false;
	}else{
		// 500�t���[���ڂő唚�����N����
		float xx, yy, a, s, z;
		for(int i = 0; i < 50; i++){
			xx = x + (float)(rand() % 100 - 50);
			yy = y + (float)(rand() % 100 - 50);
			a = atan2f(x - xx, yy - y);
			s = 5.0f + 10.0f * (float)rand() / (float)RAND_MAX;
			z = 3.0f + 8.0f * (float)rand() / (float)RAND_MAX;
			AppendObject(new CExplosion(xx, yy,
				2.0f, a, s), EXPLOSION_PRIORITY, true);
		}

        CSound *se = new CSound(*(CSound*)FindItemBox("explode3"));
        se->EnableDeleteByEnd();
        se->Play();

		return true;
	}
}

void CBoss::Damaged()
{
	hardness--;
	if(status == boss_pattern1 && hardness <= BOSS_HARDNESS * 2 / 3){
		MiniExplosion();

		// �ړ��t���O�̃Z�b�g
		frame = 0;
		moveflag1 = 0;

		// �C��ʒu�̃��Z�b�g
		bangle[0] = bangle[1] = 0.0f;

		status = boss_pattern2;
	}else if(status == boss_pattern2 && hardness <= BOSS_HARDNESS / 3){
		MiniExplosion();

		// �ړ��t���O�̃Z�b�g
		frame = 0;
		moveflag1 = 2;
		moveflag2 = 0;
		mx = -(x - 320.0f) / 50.0f;
		my = -(y - 80.0f) / 50.0f;

		// �C��ʒu�̃��Z�b�g
		bangle[0] = bangle[1] = 0.0f;

		status = boss_pattern3;
	}else if(status == boss_pattern3 && hardness <= 0){
		// ���|���h�����߁A�v���C���[�𖳓G��Ԃɂ���
		player->SetNoEnemy(true);

		frame = 0;
		status = boss_explosion;
	}
}
