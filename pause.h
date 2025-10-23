//=============================================================================
// 
// ポーズ処理 [pause.h]
// Author : ENDO HIDETO
// 
//=============================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "main.h"

//マクロ定義
#define PAUSE_TEXTURE (4)		//ポーズ処理で使用するテクスチャ種類
#define PAUSE_WIDTH (250.0f)	//選択されていないポーズ選択肢の横幅
#define PAUSE_HEIGHT (50.0f)	//選択されていないポーズ選択肢の高さ
#define PAUSE_CHOICE (10.0f)	//選択されたときの拡大
#define PAUSE_LINE (60.0f)		//ポーズ選択肢の間隔

//ポーズメニュー列挙型
typedef enum
{
	PAUSE_CONTINUE = 0,			//コンティニュー
	PAUSE_RETRY,				//リトライ
	PAUSE_QUIT,					//タイトルへ
	PAUSE_MAX			
}PAUSEMENU;


//プロトタイプ宣言
void InitPause(void);		//初期化
void UninitPause(void);		//終了
void UpdatePause(void);		//更新
void DrawPause(void);		//描画
bool GetPause(void);		//ポーズ中かどうか取得

#endif // !_PAUSE_H_