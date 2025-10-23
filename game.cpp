//=============================================================================
// 
// ゲーム画面 [game.cpp]
// Author : ENDO HIDETO
// 
//=============================================================================
#include "bg.h"
#include "block.h"
//#include "effect.h"
//#include "explosion.h"
#include "fade.h"
#include "game.h"
#include "goal.h"
#include "input.h"
//#include "particle.h"
#include "pause.h"
#include "player.h"
#include "result.h"
#include "retry.h"
//#include "score.h"
//#include "sound.h"
//#include "stage.h"
#include "timer.h"

//グローバル変数宣言
bool g_bDrawPause = true;
GAMESTATE g_gameState = GAME_NONE;
bool g_bTrans = true;				//画面遷移
int g_nSelectStage;

//プロトタイプ宣言
void GameRunning(void);					//ゲーム進行中の処理

//=======================================
// ゲーム画面初期化処理
//=======================================
void InitGame(void)
{
	//ポーズ初期化
	InitPause();

	//背景の初期化
	InitBG();

	//プレイヤー初期化
	InitPlayer();

	//ブロック初期化
	InitBlock();

	//ゴール初期化
	InitGoal();

	//リトライ処理の初期化
	InitRetry();

	//タイマー初期化
	InitTimer();

	//リザルト初期化
	InitResult();

#if 0
	//ステージ初期化
	InitStage();

	//エフェクトの初期化
	InitEffect();

	//パーティクルの初期化
	InitParticle();
	//サウンドの再生
	PlaySound(SOUND_LABEL_BGM000);
#endif
#if _DEBUG
	g_bTrans = false;
#endif
	g_gameState = GAME_START;
	g_bDrawPause = true;		//F1でポーズメニューを消す（デバッグ用
}

//=======================================
// ゲーム画面終了処理
//=======================================
void UninitGame(void)
{
	//サウンド停止
	//StopSound();

	//ポーズ終了
	UninitPause();

	//プレイヤー終了
	UninitPlayer();		

	//ブロック終了
	UninitBlock();

	//ゴール終了
	UninitGoal();

	//リトライ処理終了
	UninitRetry();

	//タイマー終了
	UninitTimer();
#if 0
	//ステージ終了
	UninitStage();

	//エフェクト終了
	UninitEffect();

	//パーティクル終了
	UninitParticle();

#endif
	//背景終了
	UninitBG();

	//リザルト終了
	UninitResult();
}

//=======================================
// ゲーム画面更新処理
//=======================================
void UpdateGame(void)
{
	switch (g_gameState)
	{
	case GAME_START:	//起動
		g_gameState = GAME_RUNNING;
		break;

	case GAME_RUNNING:	//進行中
		GameRunning();
		break;

	case GAME_RESPAWN:	//リスポーン
		g_gameState = GAME_RUNNING;
		break;

	case GAME_CLEAR:	//クリア
		UpdateResult();
		break;

	default:
		break;
	}
}

//=======================================
// ゲーム進行中
void GameRunning(void)
{
		
	//クリア判定
	if (GameFinish() == true)
	{//goal.cppで終了判定
		g_gameState = GAME_CLEAR;
		SetResult();
	}

	UpdatePause();			//ポーズ画面更新

	if (GetPause() == false)
	{//ゲーム実行中
		UpdatePlayer();			//プレイヤー更新
		UpdateRetry();			//リトライ更新
		UpdateBlock();			//ブロック更新
		UpdateGoal();			//ゴール更新
		UpdateTimer();			//タイマー更新
		UpdateBG();				//背景更新

		//UpdateStage();			//ステージ更新
		//UpdateParticle();		//パーティクル更新
		//UpdateEffect();			//エフェクト更新
	}

	//ポーズ切り替え
	if (GetKeyboardTrigger(DIK_F1) == true)
		g_bDrawPause = g_bDrawPause ^ 1;

#if _DEBUG
	//デバッグ用コマンド
	if (GetKeyboardTrigger(DIK_F2) == true)
	{//クリア
		g_gameState = GAME_CLEAR;
		SetResult();
	}
#endif
}

//=======================================
// ゲーム画面描画処理
//=======================================
void DrawGame(void)
{
	DrawBG();			//背景描画
	DrawPlayer();		//プレイヤー描画
	DrawBlock();		//ブロック描画
	DrawGoal();			//ゴール描画
	DrawTimer();		//タイマー描画
	DrawRetry();		//リトライ描画
	DrawResult();		//リザルト描画
#if 0
	DrawStage();		//ステージ描画
	DrawParticle();		//パーティクル描画
	DrawEffect();		//エフェクト描画
#endif

	if (GetPause() == true && g_bDrawPause)
	{//ポーズ画面の描画
		DrawPause();
	}
}

//=======================================
//  ゲーム状態取得
//=======================================
GAMESTATE GetGameState(void)
{
	return g_gameState;
}

//=======================================
// ゲーム状態設定
//=======================================
void SetGameState(GAMESTATE state)
{
	g_gameState = state;
}
