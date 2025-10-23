//=============================================================================
// 
// 復活時の処理 [retry.cpp]
// Author : ENDO HIDETO
// 
//=============================================================================
#include "input.h"
#include "retry.h"
#include "player.h"
#include "timer.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureRetry = NULL;				//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRetry = NULL;			//頂点バッファへのポインタ
D3DXVECTOR3 g_posRetry;									//表示位置

//=======================================
// 背景の初期化処理
//=======================================
void InitRetry(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//デバイスへのポインタ
	VERTEX_2D* pVtx;									//頂点情報へのポインタ

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\BG\\pause001.png",&g_pTextureRetry);


	//頂点バッファの読み込み
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRetry,
		NULL);

	//位置を格納
	g_posRetry = D3DXVECTOR3(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 0.0f);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRetry->Lock(0, 0,(void**)&pVtx, 0);

	//頂点座標を設定
	pVtx[0].pos = D3DXVECTOR3(g_posRetry.x - RETRY_WIDTH, g_posRetry.y - RETRY_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posRetry.x + RETRY_WIDTH, g_posRetry.y - RETRY_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posRetry.x - RETRY_WIDTH, g_posRetry.y + RETRY_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posRetry.x + RETRY_WIDTH, g_posRetry.y + RETRY_HEIGHT, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラー設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

	//テクスチャの座標設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファのロック解除
	g_pVtxBuffRetry->Unlock();
}

//=======================================
// 背景の終了処理
//=======================================
void UninitRetry(void)
{
	if (g_pTextureRetry != NULL)
	{
		g_pTextureRetry->Release();
		g_pTextureRetry = NULL;
	}

	if (g_pVtxBuffRetry != NULL)
	{
		g_pVtxBuffRetry->Release();
		g_pVtxBuffRetry = NULL;
	}
}

//=======================================
// 背景更新処理
//=======================================
void UpdateRetry(void)
{
	VERTEX_2D* pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRetry->Lock(0, 0, (void**)&pVtx, 0);

	if (GetPlayer()->state == PLAYERSTATE_DEATH)
	{//死んだらリトライと表示
		//頂点カラー設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		if (GetKeyboardTrigger(DIK_SPACE) == true || GetKeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_A) == true)
		{//スペース・エンター・Aボタンで復活
			GetPlayer()->state = PLAYERSTATE_NORMAL;	//状態を復活
			RestartTimer();								//タイマーを再開
		}
	}
	else
	{
		//頂点カラー設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	}

	//頂点バッファのロック解除
	g_pVtxBuffRetry->Unlock();

}

//=======================================
// 背景描画処理
//=======================================
void DrawRetry(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタ

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		g_pVtxBuffRetry,
		0,
		sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureRetry);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
