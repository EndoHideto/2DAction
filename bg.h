//=============================================================================
// 
// 背景の処理 [bg.h]
// Author : ENDO HIDETO
// 
//=============================================================================
#ifndef _BG_H_
#define _BG_H_	//2重インクルード防止

#include "main.h"

#define BG_VER_NUM (4)		//頂点数

typedef enum
{
	BG_0 = 0,
	BG_1,
	BG_2,
	BGLAYER_MAX
}Bglayer;

//プロトタイプ宣言
void InitBG(void);
void UninitBG(void);
void UpdateBG(void);
void DrawBG(void);

#endif // !_BG_H_
