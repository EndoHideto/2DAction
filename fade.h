//=============================================================================
// 
// ふぇ～どの処理 [fade.h]
// Author : ENDO HIDETO
// 
//=============================================================================
#ifndef _FADE_H_
#define _FADE_H_				//2重インクルード防止

#include "main.h"

#define FADE_VER_NUM (4)		//頂点数
#define FADE_LAYER (3)			//フェードレイヤー数

//フェード列挙型定義
typedef enum
{
	FADE_NONE = 0,				//何もしてない状態
	FADE_IN,					//フェードイン
	FADE_OUT,					//フェードアウト
	FADE_MAX
}FADE;

//プロトタイプ宣言
void InitFade(void);
void UninitFade(void);
void UpdateFade(void);
void DrawFade(void);
void SetFade(MODE nextmode);
FADE GetFade(void);

#endif // !_FADE_H_
