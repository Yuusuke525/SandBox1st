#include "EditScene.h"


C_EDIT::C_EDIT(int Width,int Hight){
	W_WIDTH = Width;
	W_HEIGHT = Hight;

	Init();
}

C_EDIT::~C_EDIT(){
	
}

void C_EDIT::Init(){

	//EditWraptex[0].Load(pDevice3D, _T(""));
	EditWraptex[1].Load(pDevice3D, _T("img/Left_or_Right.png"));
	EditWraptex[2].Load(pDevice3D, _T("img/speck.png"));
	EditWraptex[3].Load(pDevice3D, _T("img/WeponU.png"));


	EditWepontexL[0].Load(pDevice3D, _T("img/Wepon.png"));
	EditWepontexL[1].Load(pDevice3D, _T("img/Wepon.png"));
	EditWepontexL[2].Load(pDevice3D, _T("img/Wepon.png"));
	EditWepontexL[3].Load(pDevice3D, _T("img/Wepon.png"));
	EditWepontexL[4].Load(pDevice3D, _T("img/Wepon.png"));


	EditWepontexR[0].Load(pDevice3D, _T("img/Wepon.png"));
	EditWepontexR[1].Load(pDevice3D, _T("img/Wepon.png"));
	EditWepontexR[2].Load(pDevice3D, _T("img/Wepon.png"));
	EditWepontexR[3].Load(pDevice3D, _T("img/Wepon.png"));
	EditWepontexR[4].Load(pDevice3D, _T("img/Wepon.png"));


	WrapSprite[0].SetPos(160, 100);//Left
	WrapSprite[0].SetSize(240, 120);
	WrapSprite[1].SetPos(1120, 100);//Right
	WrapSprite[1].SetSize(240, 120);
	WrapSprite[2].SetPos(160, 440);//Lspeck
	WrapSprite[2].SetSize(240, 480);
	WrapSprite[3].SetPos(1120, 440);//RSpeck
	WrapSprite[3].SetSize(240, 480);
	WrapSprite[4].SetPos(460, 360);//武器画面
	WrapSprite[4].SetSize(280, 640);
	WrapSprite[5].SetPos(820, 360);
	WrapSprite[5].SetSize(280, 640);


	WeponSprite[0].SetPos(460, 360);
	WeponSprite[0].SetSize(200, 200);
	WeponSprite[1].SetPos(820, 360);
	WeponSprite[1].SetSize(200, 200);


	intoWeponDataL(0, 0, "グレイキャノン(二点砲)", 20, 0);

	intoWeponDataR(0, 0, "ヴァルヴァキャノン(一点砲)", 25, 0);

}

int C_EDIT::intoWeponDataL(int num,
	int WeponNumber,
	char WeponName[30],
	int AttackPoint,
	int TextureNum){
	
		LWeponData[num].WeponNumber = WeponNumber;
		strcpy(LWeponData[num].WeponName, WeponName);
		LWeponData[num].AttackPoint = AttackPoint;
		LWeponData[num].TextureNum = TextureNum;
		return 0;
}

int C_EDIT::intoWeponDataR(int num,
	int WeponNumber,
	char WeponName[30],
	int AttackPoint,
	int TextureNum){

	RWeponData[num].WeponNumber = WeponNumber;
	strcpy(RWeponData[num].WeponName, WeponName);
	RWeponData[num].AttackPoint = AttackPoint;
	RWeponData[num].TextureNum = TextureNum;
	return 0;
}


int C_EDIT::Wrap(){
	
		WrapSprite[0].Draw(pDevice3D, EditWraptex[1].pTexture);
		WrapSprite[1].Draw(pDevice3D, EditWraptex[1].pTexture);
		WrapSprite[2].Draw(pDevice3D, EditWraptex[2].pTexture);
		WrapSprite[3].Draw(pDevice3D, EditWraptex[2].pTexture);
		WrapSprite[4].Draw(pDevice3D, EditWraptex[3].pTexture);
		WrapSprite[5].Draw(pDevice3D, EditWraptex[3].pTexture);
	return 0;
}

int C_EDIT::WeponDraw(int L,int R){
	WeponSprite[0].Draw(pDevice3D, EditWepontexL[LWeponData[L].TextureNum].pTexture);
	WeponSprite[1].Draw(pDevice3D, EditWepontexR[LWeponData[R].TextureNum].pTexture);
	return 0;
}

int C_EDIT::WeponSpeck(){
	return 0;
}

int C_EDIT::SpeckDraw(int Lnum,int Rnum){

	return 0;
}
