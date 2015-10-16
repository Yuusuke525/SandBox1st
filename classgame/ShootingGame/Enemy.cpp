#include "Enemy.h"
#include "Boss.h"
#include "Explosion.h"


CEnemyPattern::CEnemyPattern()
{
	frame = 0;
}

void CEnemyPattern::Exec()
{
	if(frame == 0){
		CBGMusic *bgm = (CBGMusic*)FindObject("bgm1");
		bgm->Play(true);
	}else if(frame > 150 && frame < 400){
		if(frame % 20 == 0){
			AppendObject(new CEnemy1(40.0f), ENEMY_PRIORITY, true);
		}
	}else if(frame >= 600 && frame < 850){
		if(frame % 20 == 0){
			AppendObject(new CEnemy1(600.0f), ENEMY_PRIORITY, true);
		}
	}else if(frame >= 1050 && frame < 1300){
		if(frame % 40 == 0){
			AppendObject(new CEnemy1(40.0f), ENEMY_PRIORITY, true);
		}else if(frame % 40 == 20){
			AppendObject(new CEnemy1(600.0f), ENEMY_PRIORITY, true);
		}
	}else if(frame == 1500){
		AppendObject(new CEnemy3(100.0f), ENEMY_PRIORITY, true);
	}else if(frame > 2000 && frame <= 2400){
		if(frame % 30 == 0){
			AppendObject(new CEnemy2
				((float)(50 + (rand() % 540))), ENEMY_PRIORITY, true);
		}

		if(frame == 2400){
			CBGMusic *bgm = (CBGMusic*)FindObject("bgm1");
			bgm->Fade(5000, -5000);
		}
	}else if(frame == 2600){
		CBGMusic *bgm = (CBGMusic*)FindObject("bgm2");
		bgm->Play(true);
		AppendObject(new CBoss(), ENEMY_PRIORITY, true);
	}

	frame++;
}


///////////////////////////////////////////////////////////
// CEnemy1
///////////////////////////////////////////////////////////

CEnemy1::CEnemy1(float sx)
{
	this->x = sx;
	hardness = 1;
}

void CEnemy1::Init()
{
	CEnemyBase::Init();

	sprite.SetTexture((CTexture*)FindItemBox("zakotex"));
	sprite.SetSpriteSize(32, 32);

	frame = 0;
	animframe = 0;

	y = -16.0f;
	accel_x = 0.0f;
	left = (x < 320.0f);
}

void CEnemy1::Exec()
{
	y += 3.0f;

	x += accel_x;
	if(left == true){
		accel_x += 0.05f;
	}else{
		accel_x -= 0.05f;
	}

	if(y > 640.0f + 16.0f || x < -16.0f || x > 640.0f + 16.0f)
	{
		// ��ʊO�ɏo�������
		RemoveObject(this);
		return;
	}

	frame++;
	if(frame >= 10){
		animframe++;
		if(animframe > 5) animframe = 0;

		frame = 0;
		sprite.SetFrame(animframe);
	}

	sprite.Draw(x, y);

	if(this->HitTest(player)){
		player->Destroy();
		this->Damaged();
	}
}


///////////////////////////////////////////////////////////
// CEnemy2
///////////////////////////////////////////////////////////

CEnemy2::CEnemy2(float sx)
{
	x = sx;
	y = -16.0f;
	hardness = 3;
	shoot = false;
}

void CEnemy2::Init()
{
	CEnemyBase::Init();

	sprite.SetTexture((CTexture*)FindItemBox("zakotex"));
	sprite.SetSpriteSize(32, 32);
	sprite.SetFrame(6);
}

void CEnemy2::Exec()
{
	if(shoot == false && y > 100){
		float angle = atan2f(player->GetPosY() - y, player->GetPosX() - x);
		AppendObject(
			new CEnemyBullet(x, y, angle, 2.0f),
			ENEMYBULLET_PRIORITY, true);
		shoot = true;
	}

	y += 2.0f;

	if(y > 480.0f + 16.0f || x < -16.0f || x > 640.0f + 16.0f)
	{
		RemoveObject(this);
		return;
	}

	sprite.Draw(x, y);

	if(this->HitTest(player)){
		player->Destroy();
		this->Damaged();
	}
}


///////////////////////////////////////////////////////////
// CEnemy3
///////////////////////////////////////////////////////////

CEnemy3::CEnemy3(float sx)
{
	x = sx;
	left = (x < 320.0f);
	shoot = false;
	hardness = 20;
	frame = 0;
}

void CEnemy3::Init()
{
	CEnemyBase::Init();

	sprite.SetTexture((CTexture*)FindItemBox("cboss"));
	sprite.SetSpriteSize(128, 64);
}

void CEnemy3::Exec()
{
	// �ړ�
	if(y <= 150){
		y += 1.0f;
	}else{
		// �r�����琅���ړ��ɐ؂�ւ�
		if(left == true){
			x += 1.0f;
		}else{
			x -= 1.0f;
		}

		if(x > 704.0f && x < -64.0f){
			RemoveObject(this);
			return;
		}
	}

	// �e�̔���
	if(frame % 120 == 0 && frame > 0){
		//120�t���[�����ƂɘA�˃t���O��؂�ւ���
		shoot = !shoot;

		// �A�ˏI������ł����
		// �^���Ɍ�������5way�e�𔭎˂���
		if(shoot == false){
			float angle[5] = {90.0f, 70.0f, 50.0f, 110.0f, 130.0f};
			for(int i = 0; i < 5; i++){
			AppendObject(
				new CEnemyBullet(x, y + 18, d2r(angle[i]), 2.5f),
				ENEMYBULLET_PRIORITY, true);
			}
		}
	}
	
	// �A�˃t���O���L���ɂȂ��Ă���̂ł����
	// 20�t���[�����Ƃɐ^���ɒe�𔭎�
	if(shoot == true){
		if(frame % 20 == 0){
			AppendObject(
				new CEnemyBullet(x - 40, y + 37, d2r(90.0f), 2.0f),
				ENEMYBULLET_PRIORITY, true);
			AppendObject(
				new CEnemyBullet(x + 40, y + 37, d2r(90.0f), 2.0f),
				ENEMYBULLET_PRIORITY, true);
		}
	}

	frame++;

	sprite.Draw(x, y);

	if(this->HitTest(player)){
		player->Destroy();
		this->Damaged();
	}
}

void CEnemy3::Damaged()
{
	hardness--;
	if(hardness == 0){
		// 10�̔����������_���Ŋg�U������
		for(int i = 0; i < 10; i++){
			float angle = d2r((float)(36 * i));
			float size = 0.5f + 2.0f * (float)rand() / (float)(RAND_MAX + 1);
			float speed = 1.0f + 5.0f * (float)rand() / (float)(RAND_MAX + 1);
			AppendObject(new CExplosion(x, y, size, angle, speed),
				EXPLOSION_PRIORITY, true);
		}

		CSound *se = new CSound(*(CSound*)FindItemBox("explode2"));
		se->EnableDeleteByEnd();
		se->Play();

		AppendObject(new CPowerupItem(x, y), PLAYER_PRIORITY, true);
		RemoveObject(this);
	}
}
