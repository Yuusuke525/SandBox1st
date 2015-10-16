#include "dxfont.h"

CDxFont::CDxFont(){
	font = NULL;
	Create(16);
}

CDxFont::CDxFont(int fontsize){
	font = NULL;
	Create(fontsize);
}

CDxFont::~CDxFont(){
	RELEASE(font);
}


BOOL CDxFont::Create(int fontsize){
	RELEASE(font);

	int result = D3DXCreateFont(
		pD3Ddevice,						//取得元となるデバイス
		fontsize,						//幅
		NULL,							//高さ（既定の高さ）
		FW_DONTCARE,					//太さ（既定の太さ)
		NULL,							//ミニマップレベル（なし）
		FALSE,							//斜体
		DEFAULT_CHARSET,					//文字セット(ANSI)
		OUT_DEFAULT_PRECIS,				//出力精度(標準)
		DEFAULT_QUALITY,				//出力品質(標準)
		DEFAULT_PITCH | FF_DONTCARE,	//フォントピッチ＆フォントファミリ（システムに任せる）
		_T("メイリオ"),				//フォント名
		&font							//格納先
		);

	return (result == 0) ? FALSE : TRUE;
}

const LPD3DXFONT CDxFont::GetD3DXFont(){
	return font;
}

void CDxFont::Draw(LPCTSTR text, int count, LPRECT pRect,
	DWORD Format, D3DXCOLOR Color){
	if (font){
		pSprite->Begin(NULL);	//2Dの描画開始を宣言

		font->DrawText(
			pSprite,	//描画に用いるスプライトオブジェクト
			text,		//表示する文字列
			count,		//表示する文字数。-1ですべて
			pRect,		//文字列を描画する範囲
			Format,		//文字列描画設定
			Color		//文字列描画色
			);
		pSprite->End();			//終了宣言
	}
}

void CDxFont::Draw(LPCTSTR text, int x, int y){
	//最低限の指定だけで済むものぐさ版
	//[x,y]の位置を左上にして文字列が描画される
	RECT rect = { x, y, x, y };
	Draw(text, -1, &rect, DT_NOCLIP | DT_SINGLELINE,
		0xFFFFFFFF);
}

