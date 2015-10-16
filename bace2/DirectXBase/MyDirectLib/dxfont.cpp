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
		pD3Ddevice,						//�擾���ƂȂ�f�o�C�X
		fontsize,						//��
		NULL,							//�����i����̍����j
		FW_DONTCARE,					//�����i����̑���)
		NULL,							//�~�j�}�b�v���x���i�Ȃ��j
		FALSE,							//�Α�
		DEFAULT_CHARSET,					//�����Z�b�g(ANSI)
		OUT_DEFAULT_PRECIS,				//�o�͐��x(�W��)
		DEFAULT_QUALITY,				//�o�͕i��(�W��)
		DEFAULT_PITCH | FF_DONTCARE,	//�t�H���g�s�b�`���t�H���g�t�@�~���i�V�X�e���ɔC����j
		_T("���C���I"),				//�t�H���g��
		&font							//�i�[��
		);

	return (result == 0) ? FALSE : TRUE;
}

const LPD3DXFONT CDxFont::GetD3DXFont(){
	return font;
}

void CDxFont::Draw(LPCTSTR text, int count, LPRECT pRect,
	DWORD Format, D3DXCOLOR Color){
	if (font){
		pSprite->Begin(NULL);	//2D�̕`��J�n��錾

		font->DrawText(
			pSprite,	//�`��ɗp����X�v���C�g�I�u�W�F�N�g
			text,		//�\�����镶����
			count,		//�\�����镶�����B-1�ł��ׂ�
			pRect,		//�������`�悷��͈�
			Format,		//������`��ݒ�
			Color		//������`��F
			);
		pSprite->End();			//�I���錾
	}
}

void CDxFont::Draw(LPCTSTR text, int x, int y){
	//�Œ���̎w�肾���ōςނ��̂�����
	//[x,y]�̈ʒu������ɂ��ĕ����񂪕`�悳���
	RECT rect = { x, y, x, y };
	Draw(text, -1, &rect, DT_NOCLIP | DT_SINGLELINE,
		0xFFFFFFFF);
}

