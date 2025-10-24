//=============================================================================
// 
// ブロックの処理 [block.h]
// Author : ENDO HIDETO
// 
//=============================================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_	//2重インクルード防止

#include "main.h"

#define BLOCK_VER_NUM		(4)							//頂点数
#define MAX_BLOCK			(255)						//ブロック最大数
#define FILE_SAVEDATA		"data\\STAGE\\block.bin"	//ブロック情報を保存するファイル
#define BLOCKTEX_INTERVAL	(30.0f)						//ブロックテクスチャを繰り返す幅

#define BLOCKHIT_NONE		(0b0000)					//ブロックに当たってない
#define BLOCKHIT_LAND		(0b1000)					//地面に立ったとき
#define BLOCKHIT_HEAD		(0b0100)					//頭が当たった時
#define BLOCKHIT_RIGHT		(0b0010)					//右からあたる
#define BLOCKHIT_LEFT		(0b0001)					//左からあたる

//ブロックの種類列挙型
typedef enum
{
	BLOCKTYPE_NORMAL = 0,
	BLOCKTYPE_SPIKE,
	BLOCKTYPE_ICE,
	BLOCKTYPE_MAX
}BlockType;



//ブロック構造体の定義
typedef struct
{
	D3DXVECTOR3 posOrigin;	//設置位置
	D3DXVECTOR3 pos;		//現在の位置
	D3DXVECTOR3 posOld;		//前の位置
	float fWidth;			//幅
	float fHeight;			//高さ
	float fAngle;			//対角線の角度
	float fLength;			//対角線の長さ
	D3DXVECTOR3 move;		//移動速度
	float fRange;			//移動する範囲
	BlockType type;			//ブロックの種類
	float fDirection;		//向き（90度刻み）
	bool bUse;				//使っているか
}Block;

//ブロック設置情報構造体
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	float fWidth;			//幅
	float fHeight;			//高さ
	D3DXVECTOR3 move;		//移動速度
	float fRange;			//移動する範囲
	BlockType type;			//ブロックの種類
	float fDirection;		//テクスチャ方向
}BlockInfo;

//エディット情報の構造体
typedef struct
{
	int nSetBlock;			//設置済みブロック数
	int nTagBlock;			//現在選択中のブロックの番号
	Block* pBlock;			//そのポインタ
	bool bEdit;				//エディットモードON・OFF
}Edit;

//プロトタイプ宣言
void InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);
void CollisionBlock(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fTagHeight, float fTagWidth, Block** pBlock,int *pLand);
void SetBlock(D3DXVECTOR3 pos,float fWidth, float fHeight,D3DXVECTOR3 move, float fRange,BlockType type,float fDirection);
Block* GetBlock(void);


Edit GetEdit(void);

#endif // !_BLOCK_H_
