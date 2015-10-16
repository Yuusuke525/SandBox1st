#include "direct3d/direct3d.h"
#include "direct3d/sprite.h"
#include "Direct3D\texture.h"
#include "tchar.h"

struct WeponData
{
	int WeponNumber;
	char WeponName[30];
	int AttackPoint;
	int TextureNum;
};



class C_EDIT 
{
private:
	IDirect3DDevice9* pDevice3D;

	float W_WIDTH;
	float W_HEIGHT;


	Texture EditWepontexL[5];
	Texture EditWepontexR[5];
	Texture EditWraptex[4];

	Sprite WrapSprite[6];
	Sprite WeponSprite[2];

	WeponData RWeponData[5];
	WeponData LWeponData[5];

	void Init();
	void Exit();


	

public:
	C_EDIT();
	C_EDIT(IDirect3DDevice9* pD3Device, int Width, int HIGHT);
	~C_EDIT();

	int WeponSpeck();

	int intoWeponDataL(int num,
		int WeponNumber,
		char WeponName[30],
		int AttackPoint,
		int TextureNum
		);

	int intoWeponDataR(int num,
		int WeponNumber,
		char WeponName[30],
		int AttackPoint,
		int TextureNum
		);

	int Wrap();
	int WeponDraw(int L, int R);

protected:
	
	int SpeckDraw(int Lnum,int Rnum);
	
};
