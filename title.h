//=============================================================================
// 
// タイトル [title.h]
// Author : ENDO HIDETO
// 
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_	//2重インクルード防止

#include "main.h"

#define TITLE_THING		(6)						//設置するもの
#define TITLE_TEXTURE	(5)						//テクスチャ数
#define TITLE_POS		(410.0f,700.0f,0.0f)	//タイトルの位置
#define TITLE_WIDTH		(360.0f)				//			幅の半分
#define TITLE_HEIGHT	(400.0f)				//			高さ
#define TITLE_POS_START	()
#define TITLE_POS_END	()
#define TITLE_POS_LIGHT (65.0f, 720.0f, 0.0f)
#define LIGHT_WIDTH		(200.0f)				//ライト（飾り）の幅の半分
#define LIGHT_HEIGHT	(500.0f)				//ライト（飾り）の高さ

typedef struct
{
	D3DXVECTOR3 pos;
	float fWidth;
	float fHeight;
	float fAngle;
	float fLength;
}TitileObject;

//プロトタイプ宣言
void InitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);

#endif // !_TITLE_H_
