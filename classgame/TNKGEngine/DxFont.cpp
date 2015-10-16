#include "DxFont.h"

CDxFont::CDxFont()
{
	font = NULL;
	Create(16);
}

CDxFont::CDxFont(int fontsize)
{
 	font = NULL;
	Create(fontsize);
}

CDxFont::~CDxFont()
{
	RELEASE(font);
}

BOOL CDxFont::Create(int fontsize)
{
	RELEASE(font);

	int result = D3DXCreateFont(
		pD3Ddevice,						// 取得元となるデバイス
		fontsize,						// 幅
		NULL,							// 高さ（規定の高さ）
		FW_DONTCARE,					// 太さ（規定の太さ）
		NULL,							// ミップマップレベル（なし）
		FALSE,							// 斜体（なし）
		ANSI_CHARSET,					// 文字セット（ANSI）
		OUT_DEFAULT_PRECIS,				// 出力精度（標準）
		DEFAULT_QUALITY,				// 出力品質（標準）
		DEFAULT_PITCH | FF_DONTCARE,	// フォントピッチ&フォントファミリ（システムに任せる）
		_T("MS UI Gothic"),				// フォント名
		&font							// 格納先
	);

	return (result == 0) ? FALSE : TRUE;
}

const LPD3DXFONT CDxFont::GetD3DXFont()
{
	return font;
}

void CDxFont::Draw
	(LPCTSTR text, int count, LPRECT pRect,
	DWORD Format, D3DXCOLOR Color)
{
	if(font){
		pSprite->Begin(NULL);	// ２Ｄの描画開始を宣言
		D3DXMATRIX mtrx;
		D3DXMatrixTranslation(&mtrx, 0.0f, 0.0f, 0.0f);
		pSprite->SetTransform(&mtrx);

		font->DrawText(
			pSprite,	// 描画に用いるスプライトオブジェクト
			text,		// 表示する文字列
			count,		// 表示する文字数。-1で文字列すべて
			pRect,		// 文字列を描画する範囲
			Format,		// 文字列描画設定
			Color		// 文字列描画色
		);
		
		pSprite->End();			// 描画終了の宣言
	}
}

void CDxFont::Draw(LPCTSTR text, int x, int y)
{
	// 最低限の指定だけですむものぐさ版
	RECT rect = {x, y, x, y};
	Draw(text, -1, &rect,
		DT_NOCLIP | DT_SINGLELINE,
		0xFFFFFFFF);
}
