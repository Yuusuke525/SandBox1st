#include "Explosion.h"

CExplosion::CExplosion(float sx, float sy)
{
	Reset(sx, sy, 1.5f, 0.0f, 0.0f);
}

CExplosion::CExplosion(float sx, float sy, float size, float angle, float speed)
{
	Reset(sx, sy, size, angle, speed);
}

void CExplosion::Reset(float sx, float sy, float size, float angle, float speed)
{
	frame = animframe = 0;
	x = sx;
	y = sy;

	exsize = size;

	mx = cosf(angle) * speed;
	my = sinf(angle) * speed;
}

void CExplosion::Init()
{
	sprite.SetTexture((CTexture*)FindItemBox("explode"));
	sprite.SetSpriteSize(64, 64);
}

void CExplosion::Exec()
{
	frame++;
	if(frame >= 2){
		animframe++;
		if(animframe > 15){
			RemoveObject(this);
			return;
		}
		
		frame = 0;
		sprite.SetFrame(animframe);
	}

	x += mx;
	y += my;

	sprite.Draw(x, y, exsize, exsize);
}
