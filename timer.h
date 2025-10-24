//=============================================================================
// 
// タイマーの処理 [timer.h]
// Author : ENDO HIDETO
// 
//=============================================================================
#ifndef TIMER_H_
#define TIMER_H_				//2重インクルード防止

#include "main.h"
#include <time.h>

//マクロ定義
#define TIMER_TEXTURE (2)		//timer.cppで使用するテクスチャ数
#define TIMER_PLACE (8)			//表示する桁数
#define TIMER_WIDTH (30)		//タイマー一桁の幅
#define TIMER_HEIGHT (60)		//				高さ
#define TIMERSTART_OBJPOS D3DXVECTOR3(85.0f,SCREEN_HEIGHT - 200.0f,0.0f)	//タイマー開始のオブジェクトの位置

//タイマー構造体
typedef struct
{
	D3DXVECTOR3 posCounter;		//タイムの基準位置
	float fWidth;				//カウンターの幅
	float fHeight;				//カウンターの高さ
	D3DXVECTOR3 posStart;		//計測開始オブジェクトの位置
	float fSize;				//計測開始オブジェクトのサイズ
	time_t tStarttime;			//開始時刻
	int nTime;					//経過タイム(秒)
	bool bTimerStart;			//タイマーが動いているかどうか
	bool bTimerBreak;			//タイマーが一時停止しているかどうか
	time_t breaktime;			//ブレイクが発生した時刻
	time_t tBreak;					//一時停止中の差分
	time_t tSubTime;				//差分の合計
}Timer;

//プロトタイプ宣言
void InitTimer(void);			//初期化
void UninitTimer(void);			//終了
void UpdateTimer(void);			//更新
void DrawTimer(void);			//描画

void StopTimer(void);			//タイマーストップ
void BreakTimer(bool bTimer);	//タイマー一時停止・リスタート
Timer GetTimer(void);			//タイマー情報の取得
#endif // !TIMER_H_
