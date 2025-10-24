//=============================================================================
// 
// タイマーの処理 [timer.h]
// Author : ENDO HIDETO
// 
//=============================================================================
#include "timer.h"
#include "player.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTexturTimer[TIMER_TEXTURE] = { NULL };		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTimer = NULL;						//頂点バッファへのポインタ
Timer g_timer;

//=======================================
// タイマー初期化処理
//=======================================
void InitTimer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタ
	VERTEX_2D* pVtx;								//頂点情報へのポインタ

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\object\\timer.png", &g_pTexturTimer[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Word\\number001.png", &g_pTexturTimer[1]);

	//頂点バッファの読み込み
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * (TIMER_PLACE + 1),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTimer,
		NULL);

	//タイマー構造体初期化
	g_timer.posCounter = D3DXVECTOR3((SCREEN_WIDTH - TIMER_PLACE * TIMER_WIDTH) * 0.5f, 40, 0);	//位置
	g_timer.fWidth = TIMER_WIDTH;				//幅
	g_timer.fHeight = TIMER_HEIGHT;				//高さ
	g_timer.posStart = TIMERSTART_OBJPOS;		//スタートオブジェクトの位置
	g_timer.fSize = 50;							//文字サイズ
	g_timer.bTimerStart = false;				//スタートフラグ
	g_timer.bTimerBreak = false;				//タイマー一時停止フラグ
	g_timer.nBreak = 0;							//タイマー一時停止一階分の差分
	g_timer.nSubTime = 0;						//タイマー一時停止合計

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	//計測開始オブジェクトの設定
	//頂点座標を設定
	pVtx[0].pos = g_timer.posStart;
	pVtx[1].pos = D3DXVECTOR3(g_timer.posStart.x + g_timer.fSize, g_timer.posStart.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_timer.posStart.x, g_timer.posStart.y + g_timer.fSize, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_timer.posStart.x + g_timer.fSize, g_timer.posStart.y + g_timer.fSize, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラー設定
	pVtx[0].col = D3DXCOLOR(0.8f, 0.8f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(0.8f, 0.8f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(0.8f, 0.8f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(0.8f, 0.8f, 1.0f, 1.0f);

	//テクスチャの座標設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx += 4;

	//タイマーの設定
	for (int nCntPlace = 0; nCntPlace < TIMER_PLACE; nCntPlace++, pVtx += 4)
	{
		//頂点座標を設定
		pVtx[0].pos = D3DXVECTOR3(g_timer.posCounter.x + (g_timer.fWidth * nCntPlace), g_timer.posCounter.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_timer.posCounter.x + (g_timer.fWidth * (nCntPlace + 1)), g_timer.posCounter.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_timer.posCounter.x + (g_timer.fWidth * nCntPlace), g_timer.posCounter.y + g_timer.fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_timer.posCounter.x + (g_timer.fWidth * (nCntPlace + 1)), g_timer.posCounter.y + g_timer.fHeight, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラー設定
		pVtx[0].col = D3DXCOLOR(0.8f, 0.8f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.8f, 0.8f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.8f, 0.8f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.8f, 0.8f, 1.0f, 1.0f);

		if (nCntPlace == 2)
		{
			//テクスチャの座標設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.5f);
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.5f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
		}
		else if (nCntPlace == 5)
		{
			//テクスチャの座標設定
			pVtx[0].tex = D3DXVECTOR2(0.1f, 0.5f);
			pVtx[1].tex = D3DXVECTOR2(0.2f, 0.5f);
			pVtx[2].tex = D3DXVECTOR2(0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.2f, 1.0f);
		}
		else
		{			//テクスチャの座標設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
		}
	}
	//頂点バッファのロック解除
	g_pVtxBuffTimer->Unlock();
}

//=======================================
// タイマー終了処理
//=======================================
void UninitTimer(void)
{
	for (int nCntTex = 0; nCntTex < TIMER_TEXTURE; nCntTex++)
	{
		if (g_pTexturTimer[nCntTex] != NULL)
		{
			g_pTexturTimer[nCntTex]->Release();
			g_pTexturTimer[nCntTex] = NULL;
		}
	}

	if (g_pVtxBuffTimer != NULL)
	{
		g_pVtxBuffTimer->Release();
		g_pVtxBuffTimer = NULL;
	}
}

//=======================================
// タイマー更新処理
//=======================================
void UpdateTimer(void)
{
	Player* pPlayer = GetPlayer();	//プレイヤー情報
	VERTEX_2D* pVtx;				//頂点情報へのポインタ
	time_t nowtime;					//現在時刻
	int nDigit[TIMER_PLACE];		//一桁ずつ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	if (g_timer.bTimerStart == false)
	{
		if (GetPlayer()->bUse == true)
		{
			if (g_timer.posStart.y < pPlayer->pos.y
				&& pPlayer->pos.y - pPlayer->fHeight < g_timer.posStart.y + g_timer.fSize
				&& g_timer.posStart.x < pPlayer->pos.x + pPlayer->fWidth
				&& pPlayer->pos.x - pPlayer->fWidth < g_timer.posStart.x + g_timer.fSize)
			{//当たったらタイマースタート
				g_timer.bTimerStart = true;
				g_timer.starttime = timeGetTime();

				//時計を透明にする
				pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
				pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
				pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
				pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			}
		}
	}
	else
	{
		nowtime = timeGetTime();
		if (g_timer.bTimerBreak == false)
		{//常に時刻を保存　止めた瞬間更新が止まる
			g_timer.breaktime = nowtime;
		}

		//一時停止時間を計測し、差し引く
		g_timer.nBreak = (nowtime - g_timer.breaktime);
		g_timer.nTime = (nowtime - g_timer.starttime - g_timer.nBreak - g_timer.nSubTime) * 0.1f;
		if (g_timer.nTime < 600000)
		{
			nDigit[0] = g_timer.nTime / 60000;			//X0:00.00
			nDigit[1] = g_timer.nTime / 6000 % 10;		//0X:00.00
														//00_00.00
			nDigit[3] = g_timer.nTime % 6000 / 1000;	//00:X0.00
			nDigit[4] = g_timer.nTime % 60000 / 100;	//00:0X.00
														//00:00_00
			nDigit[6] = g_timer.nTime % 600000 / 10;	//00:00.X0
			nDigit[7] = g_timer.nTime % 6000000;		//00:00.0X
		}
		else
		{//表示限界超え
			for (int nCntNum = 0; nCntNum < TIMER_PLACE; nCntNum++)
				nDigit[nCntNum] = 9;
		}

		for (int nCntNum = 0; nCntNum < TIMER_PLACE; nCntNum++)
		{
			pVtx += 4;
			if (nCntNum != 2 && nCntNum != 5)
			{
				//テクスチャの座標設定
				pVtx[0].tex = D3DXVECTOR2(0.1f * nDigit[nCntNum], 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.1f * (nDigit[nCntNum] + 1), 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.1f * nDigit[nCntNum], 0.5f);
				pVtx[3].tex = D3DXVECTOR2(0.1f * (nDigit[nCntNum] + 1), 0.5f);
			}
		}
	}

	//頂点バッファのロック解除
	g_pVtxBuffTimer->Unlock();
}

//=======================================
// タイマー描画処理
//=======================================
void DrawTimer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタ

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		g_pVtxBuffTimer,
		0,
		sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_timer.bTimerStart)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexturTimer[1]);

		//ポリゴンの描画
		for (int nCntResult = 0; nCntResult < TIMER_PLACE; nCntResult++)
		{
			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * (nCntResult + 1), 2);
		}
	}
	else
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexturTimer[0]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}

}

//=======================================
//タイマーストップ
//=======================================
void StopTimer(void)
{
	g_timer.bTimerStart = false;
}

//=======================================
//タイマー一時停止・リスタート
//=======================================
void BreakTimer(bool bTimer) 
{
	g_timer.bTimerBreak = bTimer;
	if (g_timer.bTimerBreak)
	{//一時停止したら
		g_timer.nSubTime += g_timer.nBreak;	//前回の一時停止時間を合計に加算
		g_timer.nBreak = 0;					//初期化
	}
}

//=======================================
//タイマー情報の取得
//=======================================
Timer GetTimer(void)
{
	return g_timer;
}