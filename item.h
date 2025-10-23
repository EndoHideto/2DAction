//=============================================================================
// 
// アイテム処理 [item.h]
// Author : ENDO HIDETO
// 
//=============================================================================
#ifndef _ITEM_H_
#define _ITEM_H_	//2重インクルード防止

#include "main.h"

#define ITEM_VER_NUM (4)		//頂点数
#define GOAL_SIZE (80)			//アイテムの大きさ

//ブロック構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;		//現在の位置
	float fSize;			//大きさ
	bool bUse;				//使っているか
}Item;

//プロトタイプ宣言
void InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);

#endif // !_ITEM_H_
