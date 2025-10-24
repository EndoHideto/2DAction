//=============================================================================
//
// メインヘッダー [main.h]
// Author : ENDO HIDETO
//
//=============================================================================
#ifndef _MAIN_H_							//このマクロが定義されてなかったら
#define _MAIN_H_							//2重インクルード防止用マクロ

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "d3dx9.h"
#define DIRECTINPUT_VERSION (0x0800)		//ビルド時の警告対処用
#include "dinput.h"							//入力処理に必要
#include "xaudio2.h"						//サウンド処理に必要
#include "xinput.h"							//ジョイパッド処理に必要

//ライブラリのリンク
#pragma comment (lib,"d3d9.lib")
#pragma comment (lib,"d3dx9.lib")
#pragma comment (lib,"dxguid.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "dinput8.lib")		//入力処理用
#pragma comment (lib, "xinput.lib")			//ジョイキー処理に必要

//マクロ定義
#define SCREEN_WIDTH (1280)					//ウィンドウの幅
#define SCREEN_HEIGHT (720)					//ウィンドウの高さ
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)		//頂点フォーマット

//画面遷移（モード）の種類
typedef enum
{
	MODE_TITLE = 0,		//タイトル
	MODE_SELECT,		//ステージセレクト
	MODE_GAME,			//ゲーム
	MODE_MAX
}MODE;

//頂点情報[2D]の構造体定義
typedef struct
{
	D3DXVECTOR3 pos;		//頂点座標
	float rhw;				//座標変換用係数
	D3DCOLOR col;			//カラー
	D3DXVECTOR2 tex;		//テクスチャ座標
}VERTEX_2D;

//プロトタイプ宣言
LPDIRECT3DDEVICE9 GetDevice(void);	//デバイスの取得
void SetMode(MODE mode);			//モードの設定
MODE GetMode(void);					//モードの取得

#endif // !_MAIN_H_
