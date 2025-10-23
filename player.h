//=============================================================================
// 
// プレイヤーの処理 [player.h]
// Author : ENDO HIDETO
// 
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_				//2重インクルード防止

#include "main.h"
#include "block.h"

//マクロ定義
#define PLAYER_WIDTH		(30)								//プレイヤーの幅
#define PLAYER_HEIGHT		(60)								//プレイヤーの高さ
#define PLAYER_COL			D3DXCOLOR(1.0f,1.0f,1.0f,1.0f)		//初期の色
#define PLAYER_START_POS	D3DXVECTOR3(50.0f,600.0f,0.0f)		//出現位置
#define MOVE_ZERO			D3DXVECTOR3(0.0f,0.0f,0.0f)			//move値をゼロにしたいとき使う
#define ACCEL				(4)									//移動速度
#define SPEED_JUMP			(9)									//ジャンプ力ぅ
#define SPEED_DASH			(20)								//ダッシュの速度(未実装)
#define GRAVITY				(0.35)								//重力
#define REVIVALTIME			(20)								//復活時操作を受け付けない時間

//プレイヤー状態列挙
typedef enum
{
	PLAYERSTATE_NORMAL = 0,		//待機
	PLAYERSTATE_WALK,			//歩き
	PLAYERSTATE_JUMP,			//ジャンプ
	PLAYERSTATE_AIRJUMP,		//二段ジャンプ
	PLAYERSTATE_FALL,			//落下
	PLAYERSTATE_DEATH,			//死亡
	PLAYERSTATE_MAX
}PLAYERSTATE;

//プレイヤー構造体の定義
typedef struct
{
	PLAYERSTATE state;			//ジャンプ状態
	D3DXCOLOR col;				//色
	D3DXVECTOR3 pos;			//現在位置
	D3DXVECTOR3 posOld;			//前回位置
	D3DXVECTOR3 move;			//速度
	float fWidth;				//幅
	float fHeight;				//高さ
	int nCntAnim;				//アニメーションカウンタ
	int nPtnAnim;				//アニメーションパターン
	int nCntStart;				//開始数フレームは移動不可
	float fDirectionMove;		//左右の向き（テクスチャ上段下段切り替え）
	Block* pBlock;				//対象のブロックへのポインタ
	int nDeath;					//死亡回数
	int nRevivalCnter;			//復活までのカウンター
	bool bUse;					//生きている間true
	bool bDash;					//突進中true
}Player;

//プロトタイプ宣言
void InitPlayer(void);			//初期化
void UninitPlayer(void);		//終了
void UpdatePlayer(void);		//更新
void DrawPlayer(void);			//描画

void GetPlayer(float *pGravity,float*pJunpSpeed);			//情報の取得
Player* GetPlayer(void);									//情報の取得
#endif // !_PLAYER_H_
