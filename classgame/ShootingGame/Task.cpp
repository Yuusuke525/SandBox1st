#include "Task.h"

// タスク処理の開始と終了を行うプログラムコード

void CTaskHead::Init()
{
	lasttime = 0;
}

//約60FPS
#define WAIT_TIME 16

void CTaskHead::Exec()
{
	//最後に行った処理からの時間を調べる
	DWORD ntime = timeGetTime();
	DWORD rtime = ntime - lasttime;
	if(rtime <= WAIT_TIME){
		//ウェイト処理を行う
		Sleep(WAIT_TIME - rtime);
	}
	lasttime = ntime;

	//画像のクリア
	pD3Ddevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,0), 1.0f, 0 );
    
	//開始宣言
    pD3Ddevice->BeginScene();
}

void CTaskTail::Exec()
{
	// 表示
	pD3Ddevice->EndScene();
	pD3Ddevice->Present( NULL, NULL, NULL, NULL );
}



bool CLoadData::complete;
list<ListData> CLoadData::temp;

CLoadData::CLoadData()
{
    // staticデータをリセット
    complete = false;
    temp.clear();

    // 変数の初期化
    brightness = 255;
    font = new CDxFont(20);
}

CLoadData::~CLoadData()
{
    if(font) delete font;

    // 中断されたのであれば、追加予定だったオブジェクトを破棄する
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
        // バックグラウンド処理がまだ行われている
        brightness -= 2;
        if(brightness < 0) brightness = 255;

        RECT rc = {0,0,630,470};
        font->Draw(L"Now Loading...", -1, &rc,
            DT_BOTTOM | DT_RIGHT | DT_SINGLELINE,
            0x00FFFFFF | ((BYTE)brightness << 24));
    }else{
        // バックグラウンドで用意されたクラスをタスクに追加する
        list<ListData>::iterator i;
        for(i = temp.begin(); i != temp.end(); i++){
            AppendObject(*i);
        }

        // 自分自身は用済みなので破棄
        RemoveObject(this);
    }
}


