//=============================================================================
// 
// 復活時の処理 [retry.h]
// Author : ENDO HIDETO
// 
//=============================================================================
#ifndef _RETRY_H_
#define _RETRY_H_	//2重インクルード防止

#include "main.h"

#define RETRY_WIDTH (80.0f)
#define RETRY_HEIGHT (40.0f)

//プロトタイプ宣言
void InitRetry(void);
void UninitRetry(void);
void UpdateRetry(void);
void DrawRetry(void);

#endif // !_RETRY_H_
