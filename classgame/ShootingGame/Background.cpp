#include "BackGround.h"

// 1ライン当たりの縦幅
#define LINEHEIGHT 150.0f

CBackGround::CBackGround(void)
{
	// 正気状態で、最上部のライン管理番号が何番になるかを計算
	linepos = (int)((480.0f + LINEHEIGHT) / LINEHEIGHT) + 1;

	// 一番上のラインを描画する位置
	pos_y = -LINEHEIGHT;
}

void CBackGround::Init()
{
	// テクスチャの読み込み・関連づけ
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
	// ラインごとのスプライト配置
	int bc = linepos;
	for(float y = pos_y; y < 480.0f; y += LINEHEIGHT, bc--){
		if(bc % 4 == 0){
			// ライン管理番号が４で割り切れる（つまり４回に１回）
			// のであれば、特別なスプライトを配置する
			base_left.Draw(0.0f, y);
			base_right.Draw(640.0f, y);
		}else{
			for(float yy = 0; yy < 150.0f; yy += 30.0f){
				pipe_left.Draw(0.0f, y + yy);
				pipe_right.Draw(640.0f, y + yy);
			}
		}
	}

	// スクロール処理
	pos_y += 2.0f;
	if(pos_y >= 0.0f){
		// 一番上のラインが描画領域に入ったのであれば
		// 新しいラインを追加するため、ライン管理番号を一増やす
		linepos++;
		pos_y = -LINEHEIGHT;
	}
}