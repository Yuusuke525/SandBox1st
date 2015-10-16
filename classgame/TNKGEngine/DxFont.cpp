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
		pD3Ddevice,						// �擾���ƂȂ�f�o�C�X
		fontsize,						// ��
		NULL,							// �����i�K��̍����j
		FW_DONTCARE,					// �����i�K��̑����j
		NULL,							// �~�b�v�}�b�v���x���i�Ȃ��j
		FALSE,							// �Άi�Ȃ��j
		ANSI_CHARSET,					// �����Z�b�g�iANSI�j
		OUT_DEFAULT_PRECIS,				// �o�͐��x�i�W���j
		DEFAULT_QUALITY,				// �o�͕i���i�W���j
		DEFAULT_PITCH | FF_DONTCARE,	// �t�H���g�s�b�`&�t�H���g�t�@�~���i�V�X�e���ɔC����j
		_T("MS UI Gothic"),				// �t�H���g��
		&font							// �i�[��
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
		pSprite->Begin(NULL);	// �Q�c�̕`��J�n��錾
		D3DXMATRIX mtrx;
		D3DXMatrixTranslation(&mtrx, 0.0f, 0.0f, 0.0f);
		pSprite->SetTransform(&mtrx);

		font->DrawText(
			pSprite,	// �`��ɗp����X�v���C�g�I�u�W�F�N�g
			text,		// �\�����镶����
			count,		// �\�����镶�����B-1�ŕ����񂷂ׂ�
			pRect,		// �������`�悷��͈�
			Format,		// ������`��ݒ�
			Color		// ������`��F
		);
		
		pSprite->End();			// �`��I���̐錾
	}
}

void CDxFont::Draw(LPCTSTR text, int x, int y)
{
	// �Œ���̎w�肾���ł��ނ��̂�����
	RECT rect = {x, y, x, y};
	Draw(text, -1, &rect,
		DT_NOCLIP | DT_SINGLELINE,
		0xFFFFFFFF);
}
