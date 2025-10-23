//=============================================================================
// 
// ゲーム画面 [game.h]
// Author : ENDO HIDETO
// 
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"

typedef enum
{
	GAME_NONE = 0,		//ゲームをしていない
	GAME_START,			//起動
	GAME_RUNNING,		//進行中
	GAME_RESPAWN,		//リスポーン
	GAME_DEAD,			//死
	GAME_CLEAR,			//クリア
	GAME_MAX
}GAMESTATE;

//プロトタイプ宣言
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);

GAMESTATE GetGameState(void);			//ゲームの状態を取得
void SetGameState(GAMESTATE state);		//ゲームの状態を設定
#endif // !_GAME_H_
