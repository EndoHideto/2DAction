//=============================================================================
// 
// ゴール処理 [goal.h]
// Author : ENDO HIDETO
// 
//=============================================================================
#ifndef _GOAL_H_
#define _GOAL_H_	//2重インクルード防止

#include "main.h"

#define GOAL_VER_NUM (4)		//頂点数
#define GOAL_SIZE (110)			//ゴールの大きさ
#define TEXTURE_GOAL (2)		//テクスチャ数

//ブロック構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;		//現在の位置
	float fLength;			//大きさ
	float fAngle;			//形
	float fRot;				//回転
	int nOpen;				//0 使用不可　/1 使用可
	bool bUse;				//使っているか
}Goal;

//プロトタイプ宣言
void InitGoal(void);
void UninitGoal(void);
void UpdateGoal(void);
void DrawGoal(void);
bool GameFinish(void);

#endif // !_GOAL_H_
