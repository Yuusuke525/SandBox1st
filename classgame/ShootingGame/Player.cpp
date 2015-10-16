#include "Player.h"
#include "EnemyBase.h"
#include "Explosion.h"
#include "Mode.h"

CPlayer::CPlayer()
{
	tex_player.Load(_T("data\\player.png"));

	sprite.SetTexture(&tex_player);
	sprite.SetSpriteSize(64, 64);

	destroyed = false;
	noenemy = false;
	powerup = false;
}

void CPlayer::Init()
{
	input = (CInput*)FindObject("input");
	if(input == NULL){
		input = new CInput();
		AppendObject(input, 0, "input", true);
	}

	missile = *(CSound*)FindItemBox("missile");

	x = 320.0f;
	y = 400.0f;

	shoottime = 0;
}

void CPlayer::Exec()
{
	if(destroyed == true) return;

	const float move_s = 4.0f;
	const float move_a = move_s / sqrtf(2.0f);

	BYTE ppos = input->GetPovPosition();

	BOOL left = input->IsKeyDown(DIK_LEFT);
	left |= (ppos & PP_LEFT) ? TRUE : FALSE ;
	BOOL right = input->IsKeyDown(DIK_RIGHT);
	right |= (ppos & PP_RIGHT) ? TRUE : FALSE ;
	BOOL up = input->IsKeyDown(DIK_UP);
	up |= (ppos & PP_UP) ? TRUE : FALSE ;
	BOOL down = input->IsKeyDown(DIK_DOWN);
	down |= (ppos & PP_DOWN) ? TRUE : FALSE ;

	if(up & left){
		x -= move_a;
		y -= move_a;
	}else if(up & right){
		x += move_a;
		y -= move_a;
	}else if(down & left){
		x -= move_a;
		y += move_a;
	}else if(down & right){
		x += move_a;
		y += move_a;
	}else if(up){
		y -= move_s;
	}else if(down){
		y += move_s;
	}else if(left){
		x -= move_s;
	}else if(right){
		x += move_s;
	}

	if(x < 32.0f) x = 32.0f;
	else if(x > 640.0f - 32.0f) x = 640.0f - 32.0f;

	if(y < 32.0f) y = 32.0f;
	else if(y > 480.0f - 32.0f) y = 480.0f - 32.0f;

	if(left){
		sprite.SetFrame(2);
	}else if(right){
		sprite.SetFrame(1);
	}else{
		sprite.SetFrame(0);
	}


	// 最後の発射から10フレーム経っていれば
	// 発射ボタンの入力を受け付ける
	if(shoottime > 10 && (input->IsKeyDown(DIK_SPACE) || input->IsButtonDown(0)))
	{
		Shoot();
		shoottime = 0;
	}
	else if(shoottime > 5 && (input->IsKeyPressed(DIK_SPACE) || input->IsButtonPressed(0)))
	{
		Shoot();
		shoottime = 0;
	}else{
		shoottime++;
	}


	sprite.Draw(x, y);
}

void CPlayer::Shoot()
{
	AppendObject(new CPlayerBullet(x, y - 8.0f, d2r(0.0f)), BULLET_PRIORITY, true);

	if(powerup == true){
		// パワーアップ状態なら左右２方向にも弾を発射
		AppendObject(new CPlayerBullet(x, y - 8.0f, d2r(20.0f)), BULLET_PRIORITY, true);
		AppendObject(new CPlayerBullet(x, y - 8.0f, d2r(-20.0f)), BULLET_PRIORITY, true);
	}

	missile.Play();
}

void CPlayer::Destroy()
{
	destroyed = true;

	((CSound*)FindItemBox("explode2"))->Play();
	AppendObject(new CExplosion(x, y), EXPLOSION_PRIORITY, true);

	// ゲームオーバー画面を表示
	AppendObject(new CContinue(false), CONTINUE_PRIORITY, true);
}

void CPlayer::GetHitRect(RECTF &result)
{
	if(destroyed == true || noenemy == true){
		// いかなるキャラクターとも重なることがない領域を設定
		result.SetRect(-10000.0f, -10000.0f, -10000.0f, -10000.0f);
	}else{
		CCharactor::GetHitRect(result);
		result.Deflate(24.0f, 24.0f);
	}
}

void CPlayer::SetNoEnemy(bool flag)
{
	noenemy = flag;
}


/////////////////////////////////////////////////////////////////
// CPlayerBullet
/////////////////////////////////////////////////////////////////

CPlayerBullet::CPlayerBullet(float sx, float sy, float moveangle)
{
	x = sx;
	y = sy;
	angle = moveangle;
}

void CPlayerBullet::Init()
{
	sprite.SetTexture((CTexture*)FindItemBox("pbullet"));
}

void CPlayerBullet::Exec()
{
	const float move = 8.0f;
	float mx = move * sinf(angle);
	float my = move * cosf(angle);

	x += mx;
	y -= my;

	if(y < -16.0f){
		RemoveObject(this);
		return;
	}

	CPrioEnum pe;
	CGameObject *g;
	CEnemyBase *e;
	CreateEnumeration(ENEMY_PRIORITY, ENEMY_PRIORITY + 10000, &pe);

	while(g = pe.GetNext()){
		e = dynamic_cast<CEnemyBase*>(g);
		if(e == NULL) continue;

		if(e->HitTest(this)){
			e->Damaged();	// 敵の被弾処理
			RemoveObject(this);
			return;
		}
	}

	sprite.Draw(x, y, angle);
}


/////////////////////////////////////////////////////////////////
// PowerupItem
/////////////////////////////////////////////////////////////////

CPowerupItem::CPowerupItem(float sx, float sy)
{
	x = sx;
	y = sy;
	frame = 0;
	animframe = 0;
}

void CPowerupItem::Init()
{
	sprite.SetTexture((CTexture*)FindItemBox("item"));
	sprite.SetSpriteSize(32, 17);

	player = (CPlayer*)FindItemBox("player");
}

void CPowerupItem::Exec()
{
	// アニメーション処理
	frame++;
	if(frame == 10){
		frame = 0;
		sprite.SetFrame(animframe++);
		if(animframe >= 6) animframe = 0;
	}

	y += 1.2f;
	if(y > 500.0f){
		RemoveObject(this);
		return;
	}

	// プレイヤーと衝突したら、プレイヤーにパワーアップ処理を行う
	if(player->HitTest(this)){
		player->PowerUp();
		RemoveObject(this);
		return;
	}

	sprite.Draw(x, y);
}
