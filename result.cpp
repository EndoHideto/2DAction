//=============================================================================
// 
// リザルト画面の処理 [result.cpp]
// Author : ENDO HIDETO
// 
//=============================================================================
#include "result.h"
#include "fade.h"
#include "game.h"
//#include "sound.h"

//背景構造体の定義

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureResult[RESULT_TEXTURE] = { NULL };	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;				//頂点バッファへのポインタ
Result g_Result;												//リザルト情報

//プロトタイプ宣言
void ResultAppear(VERTEX_2D* pVtx);								//リザルトが出てくる処理
void ResultCtrl(VERTEX_2D* pVtx);								//リザルト画面での操作

//=======================================
// 初期化処理
//=======================================
void InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタ
	VERTEX_2D* pVtx;								//頂点情報へのポインタ

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\BG\\ResultBG.png",&g_pTextureResult[0]);
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\Word\\number001.png",&g_pTextureResult[1]);
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\Word\\pause001.png",&g_pTextureResult[2]);
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\Word\\pause002.png",&g_pTextureResult[3]);

	//頂点バッファの読み込み
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)* 4 * RESULT_THING,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResult,
		NULL);

	//カウンタークリア
	g_Result.nCounter = 0;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResult->Lock(0, 0,(void**)&pVtx, 0);

	//基準位置保存
	g_Result.posBox = D3DXVECTOR3((SCREEN_WIDTH - RESULT_WIDTH) * 0.5f,-RESULT_HEIGHT, 0.0f);
	g_Result.posTime = D3DXVECTOR3((SCREEN_WIDTH - TIMER_PLACE * RESULTTIMER_WIDTH) * 0.5f, g_Result.posBox.y + RESULTTIMER_LINE, 0.0f);
	g_Result.posOperate[RESULTCONTINUE_RETRY] = D3DXVECTOR3(g_Result.posBox.x + (RESULT_WIDTH - OPERATE_WIDTH * 2) * 0.25f, g_Result.posBox.y + RESULTOPERATE_LINE, 0.0f);
	g_Result.posOperate[RESULTCONTINUE_QUIT] = D3DXVECTOR3(((SCREEN_WIDTH - OPERATE_WIDTH + RESULT_WIDTH*0.5f) * 0.5f), g_Result.posBox.y + RESULTOPERATE_LINE, 0.0f);
	g_Result.nChose = 0;
	g_Result.bMove = false;

	for (int nCntResult = 0; nCntResult < RESULT_THING; nCntResult++, pVtx += 4)
	{
		//頂点座標を設定
		pVtx[0].pos = g_Result.posBox;
		pVtx[1].pos = g_Result.posBox;
		pVtx[2].pos = g_Result.posBox;
		pVtx[3].pos = g_Result.posBox;

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラー設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャの座標設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	}
	//頂点バッファのロック解除
	g_pVtxBuffResult->Unlock();

	//音楽再生
	//PlaySound(SOUND_LABEL_BGM004);
}

//=======================================
// 終了処理
//=======================================
void UninitResult(void)
{
	for (int nCntTex = 0; nCntTex < RESULT_TEXTURE; nCntTex++)
	{
		if (g_pTextureResult[nCntTex] != NULL)
		{
			g_pTextureResult[nCntTex]->Release();
			g_pTextureResult[nCntTex] = NULL;
		}
	}

	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}

	//音楽停止
	//StopSound();
}

//=======================================
// 更新処理
//=======================================
void UpdateResult(void)
{
	VERTEX_2D* pVtx;		//頂点情報へのポインタ

	g_Result.nCounter++;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);


	if (g_Result.nCounter >= COUNTER_RESURT)
	{//一定時間経過か決定キーが押されたら

		//下に動かす
		ResultAppear(pVtx);

		if (g_Result.bMove == false)
		{
			//選ぶ
			ResultCtrl(pVtx);
		}
	}

	//カウントを終了まで進める
	if ((GetKeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_A) == true) && g_Result.nCounter < COUNTER_RESURT)
	{
		g_Result.nCounter += COUNTER_RESURT;
	}
	
	//頂点バッファのロック解除
	g_pVtxBuffResult->Unlock();
}

//=======================================
// リザルトの出現
void ResultAppear(VERTEX_2D* pVtx)
{
	//頂点座標を設定
	pVtx[0].pos = g_Result.posBox;//枠
	pVtx[1].pos = D3DXVECTOR3(g_Result.posBox.x + RESULT_WIDTH, g_Result.posBox.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Result.posBox.x, g_Result.posBox.y + RESULT_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Result.posBox.x + RESULT_WIDTH, g_Result.posBox.y + RESULT_HEIGHT, 0.0f);
	pVtx += 4;

	for (int nCntNum = 0; nCntNum < TIMER_PLACE; nCntNum++, pVtx += 4)
	{//タイム
		pVtx[0].pos = D3DXVECTOR3(g_Result.posTime.x + (RESULTTIMER_WIDTH * nCntNum), g_Result.posTime.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Result.posTime.x + (RESULTTIMER_WIDTH * (nCntNum + 1)), g_Result.posTime.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Result.posTime.x + (RESULTTIMER_WIDTH * nCntNum), g_Result.posTime.y + RESULTTIMER_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Result.posTime.x + (RESULTTIMER_WIDTH * (nCntNum + 1)), g_Result.posTime.y + RESULTTIMER_HEIGHT, 0.0f);
	}

	//続ける
	pVtx[0].pos = D3DXVECTOR3(g_Result.posOperate[RESULTCONTINUE_RETRY].x, g_Result.posOperate[RESULTCONTINUE_RETRY].y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Result.posOperate[RESULTCONTINUE_RETRY].x + OPERATE_WIDTH, g_Result.posOperate[RESULTCONTINUE_RETRY].y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Result.posOperate[RESULTCONTINUE_RETRY].x, g_Result.posOperate[RESULTCONTINUE_RETRY].y + OPERATE_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Result.posOperate[RESULTCONTINUE_RETRY].x + OPERATE_WIDTH, g_Result.posOperate[RESULTCONTINUE_RETRY].y + OPERATE_HEIGHT, 0.0f);
	pVtx += 4;
	//やめる
	pVtx[0].pos = D3DXVECTOR3(g_Result.posOperate[RESULTCONTINUE_QUIT].x, g_Result.posOperate[RESULTCONTINUE_QUIT].y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Result.posOperate[RESULTCONTINUE_QUIT].x + OPERATE_WIDTH, g_Result.posOperate[RESULTCONTINUE_QUIT].y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Result.posOperate[RESULTCONTINUE_QUIT].x, g_Result.posOperate[RESULTCONTINUE_QUIT].y + OPERATE_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Result.posOperate[RESULTCONTINUE_QUIT].x + OPERATE_WIDTH, g_Result.posOperate[RESULTCONTINUE_QUIT].y + OPERATE_HEIGHT, 0.0f);

	//下に動かす
	if (g_Result.posBox.y < (SCREEN_HEIGHT - RESULT_HEIGHT) * 0.5f)
	{
		g_Result.posBox.y += RESULT_MOVE;
		g_Result.posTime.y += RESULT_MOVE;
		g_Result.posOperate[RESULTCONTINUE_RETRY].y += RESULT_MOVE;
		g_Result.posOperate[RESULTCONTINUE_QUIT].y += RESULT_MOVE;
	}
	else
	{//一定距離で止める
		g_Result.posBox.y = (SCREEN_HEIGHT - RESULT_HEIGHT) * 0.5f;
		g_Result.posTime.y = g_Result.posBox.y + RESULTTIMER_LINE;
		g_Result.posOperate[RESULTCONTINUE_RETRY].y = g_Result.posBox.y + RESULTOPERATE_LINE;
		g_Result.posOperate[RESULTCONTINUE_QUIT].y = g_Result.posBox.y + RESULTOPERATE_LINE;
		g_Result.bMove = false;

	}

}

//=======================================
// リザルトの操作
void ResultCtrl(VERTEX_2D* pVtx)
{
	pVtx += 4 * (TIMER_PLACE + 1);

	//メニュー位置設定
	for (int nCntResult = 0; nCntResult < RESULTCONTINUE_MAX; nCntResult++, pVtx += 4)
	{

		if (g_Result.nChose % RESULTCONTINUE_MAX == nCntResult)
		{//選択中のメニューの強調
			//頂点座標
			pVtx[0].pos = D3DXVECTOR3(g_Result.posOperate[nCntResult].x - RESULT_CHOICE, g_Result.posOperate[nCntResult].y - RESULT_CHOICE, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_Result.posOperate[nCntResult].x + OPERATE_WIDTH + RESULT_CHOICE, g_Result.posOperate[nCntResult].y - RESULT_CHOICE, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_Result.posOperate[nCntResult].x - RESULT_CHOICE, g_Result.posOperate[nCntResult].y + OPERATE_HEIGHT + RESULT_CHOICE, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_Result.posOperate[nCntResult].x + OPERATE_WIDTH + RESULT_CHOICE, g_Result.posOperate[nCntResult].y + OPERATE_HEIGHT + RESULT_CHOICE, 0.0f);

			//頂点カラー設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		}
		else
		{
			//頂点座標
			pVtx[0].pos = g_Result.posOperate[nCntResult];
			pVtx[1].pos = D3DXVECTOR3(g_Result.posOperate[nCntResult].x + OPERATE_WIDTH, g_Result.posOperate[nCntResult].y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_Result.posOperate[nCntResult].x, g_Result.posOperate[nCntResult].y + OPERATE_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_Result.posOperate[nCntResult].x + OPERATE_WIDTH, g_Result.posOperate[nCntResult].y + OPERATE_HEIGHT, 0.0f);

			//頂点カラー設定
			pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[2].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		}
	}

	//メニュー操作
	if (GetKeyboardTrigger(DIK_A) == true || GetJoypadTrigger(JOYKEY_RIGHT) == true)
	{
		g_Result.nChose += (int)RESULTCONTINUE_MAX - 1;
	}
	if (GetKeyboardTrigger(DIK_D) == true || GetJoypadTrigger(JOYKEY_LEFT) == true)
	{
		g_Result.nChose++;
	}

	//メニュー操作を実行
	if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_SPACE) == true|| GetJoypadTrigger(JOYKEY_A) == true)
	{
		switch (g_Result.nChose % RESULTCONTINUE_MAX)
		{

		case RESULTCONTINUE_RETRY:
			g_Result.nChose = 0;
			SetFade(MODE_GAME);
			break;

		case RESULTCONTINUE_QUIT:
			g_Result.nChose = 0;
			SetFade(MODE_TITLE);
			break;

		default:
			break;
		}
	}

}

//=======================================
// 描画処理
//=======================================
void DrawResult(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタ

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		g_pVtxBuffResult,
		0,
		sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//背景テクスチャの設定
	pDevice->SetTexture(0, g_pTextureResult[0]);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//数字テクスチャの設定
	pDevice->SetTexture(0, g_pTextureResult[1]);

	for (int nCntResult = 0; nCntResult < TIMER_PLACE; nCntResult++)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * (nCntResult + 1), 2);
	}

	//CONTINUEテクスチャの設定
	pDevice->SetTexture(0, g_pTextureResult[2]);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * (TIMER_PLACE + 1), 2);

	//QUITテクスチャの設定
	pDevice->SetTexture(0, g_pTextureResult[3]);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * (TIMER_PLACE + 2), 2);

}

//=======================================
// リザルト情報の設定
//=======================================
void SetResult(void)
{
	VERTEX_2D* pVtx;		//頂点情報へのポインタ
	int nDigit[TIMER_PLACE];

	//クリアタイムの取得	
	g_Result.nTime = GetTimer().nTime;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);
	pVtx += 4;
	//一桁ずつ格納
	if (g_Result.nTime < 600000)
	{
		nDigit[0] = g_Result.nTime / 60000;			//X0:00.00
		nDigit[1] = g_Result.nTime / 6000 % 10;		//0X:00.00
													//00_00.00
		nDigit[3] = g_Result.nTime % 6000 / 1000;		//00:X0.00
		nDigit[4] = g_Result.nTime % 60000 / 100;		//00:0X.00
													//00:00_00
		nDigit[6] = g_Result.nTime % 600000 / 10;		//00:00.X0
		nDigit[7] = g_Result.nTime % 6000000;			//00:00.0X
	}
	else
	{//表示限界超え
		for (int nCntNum = 0; nCntNum < TIMER_PLACE; nCntNum++)
			nDigit[nCntNum] = 9;
	}

	//タイム表示
	for (int nCntNum = 0; nCntNum < TIMER_PLACE; nCntNum++, pVtx += 4)
	{
		if (nCntNum == 2)
		{//分：秒
			//テクスチャの座標設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.5f);
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.5f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
		}
		else if (nCntNum == 5)
		{//整数.少数
			//テクスチャの座標設定
			pVtx[0].tex = D3DXVECTOR2(0.1f, 0.5f);
			pVtx[1].tex = D3DXVECTOR2(0.2f, 0.5f);
			pVtx[2].tex = D3DXVECTOR2(0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.2f, 1.0f);
		}
		else
		{//テクスチャの座標設定
			//テクスチャの座標設定
			pVtx[0].tex = D3DXVECTOR2(0.1f * nDigit[nCntNum], 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f * (nDigit[nCntNum] + 1), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.1f * nDigit[nCntNum], 0.5f);
			pVtx[3].tex = D3DXVECTOR2(0.1f * (nDigit[nCntNum] + 1), 0.5f);
		}
	}

	//うごいてよし
	g_Result.bMove = true;

	//頂点バッファのロック解除
	g_pVtxBuffResult->Unlock();

}