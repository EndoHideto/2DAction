//=============================================================================
// 
// リザルト [result.h]
// Author : ENDO HIDETO
// 
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_	//2重インクルード防止

#include "main.h"
#include "timer.h"
#include "input.h"

#define RESULT_THING (TIMER_PLACE + 3)		//リザルト画面に表示する四角の数
#define RESULT_TEXTURE (4)					//リザルト画面で使うテクスチャの数
#define COUNTER_RESURT (50)					//リザルト画面を表示し続けるフレーム数
#define RESULT_WIDTH (400)					//枠の幅
#define RESULT_HEIGHT (500)					//枠の高さ
#define RESULTTIMER_LINE (120.0f)			//リザルト枠内のタイマーの位置
#define RESULTTIMER_WIDTH (45)					//タイマー一桁の幅
#define RESULTTIMER_HEIGHT (90)					//タイマー一桁の高さ
#define RESULTOPERATE_LINE (400.0f)			//続ける・やめる　の位置
#define OPERATE_WIDTH (160)					//続ける・やめる　の幅
#define OPERATE_HEIGHT (40)					//続ける・やめる　の高さ
#define RESULT_MOVE (10.0f)					//降りてくる速さ
#define RESULT_CHOICE (8.0f)				//選択中の強調表示

typedef enum
{
	RESULTCONTINUE_RETRY = 0,				//もう一度
	RESULTCONTINUE_QUIT,					//終了
	RESULTCONTINUE_MAX
}ResultContinue;

typedef struct
{
	D3DXVECTOR3 posBox;										//枠表示位置
	D3DXVECTOR3 posTime;									//タイム表示位置
	D3DXVECTOR3 posOperate[RESULTCONTINUE_MAX];				//リザルト画面での操作表示位置
	int nCounter;											//表示時間
	int nTime;												//クリアタイム
	int nChose;												//選択中の番号
	bool bMove;												//動いているか
}Result;

//プロトタイプ宣言
void InitResult(void);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);
void SetResult(void);
#endif // !_RESULT_H_
