#include "Task.h"

// �^�X�N�����̊J�n�ƏI�����s���v���O�����R�[�h

void CTaskHead::Init()
{
	lasttime = 0;
}

//��60FPS
#define WAIT_TIME 16

void CTaskHead::Exec()
{
	//�Ō�ɍs������������̎��Ԃ𒲂ׂ�
	DWORD ntime = timeGetTime();
	DWORD rtime = ntime - lasttime;
	if(rtime <= WAIT_TIME){
		//�E�F�C�g�������s��
		Sleep(WAIT_TIME - rtime);
	}
	lasttime = ntime;

	//�摜�̃N���A
	pD3Ddevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,0), 1.0f, 0 );
    
	//�J�n�錾
    pD3Ddevice->BeginScene();
}

void CTaskTail::Exec()
{
	// �\��
	pD3Ddevice->EndScene();
	pD3Ddevice->Present( NULL, NULL, NULL, NULL );
}



bool CLoadData::complete;
list<ListData> CLoadData::temp;

CLoadData::CLoadData()
{
    // static�f�[�^�����Z�b�g
    complete = false;
    temp.clear();

    // �ϐ��̏�����
    brightness = 255;
    font = new CDxFont(20);
}

CLoadData::~CLoadData()
{
    if(font) delete font;

    // ���f���ꂽ�̂ł���΁A�ǉ��\�肾�����I�u�W�F�N�g��j������
    if(complete == false){
        list<ListData>::iterator i = temp.begin();
        while(i != temp.end()){
            if((*i).autodelete == true) delete (*i).gameobj;
            i = temp.erase(i);
        }
    }
}

void CLoadData::Exec()
{
    if(complete == false){
        // �o�b�N�O���E���h�������܂��s���Ă���
        brightness -= 2;
        if(brightness < 0) brightness = 255;

        RECT rc = {0,0,630,470};
        font->Draw(L"Now Loading...", -1, &rc,
            DT_BOTTOM | DT_RIGHT | DT_SINGLELINE,
            0x00FFFFFF | ((BYTE)brightness << 24));
    }else{
        // �o�b�N�O���E���h�ŗp�ӂ��ꂽ�N���X���^�X�N�ɒǉ�����
        list<ListData>::iterator i;
        for(i = temp.begin(); i != temp.end(); i++){
            AppendObject(*i);
        }

        // �������g�͗p�ς݂Ȃ̂Ŕj��
        RemoveObject(this);
    }
}


