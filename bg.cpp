//=============================================================================
// 
// 背景の処理 [bg.cpp]
// Author : ENDO HIDETO
// 
//=============================================================================
#include "bg.h"
#include "player.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureBG[BGLAYER_MAX] = {};		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBG = NULL;			//頂点バッファへのポインタ

//=======================================
// 背景の初期化処理
//=======================================
void InitBG(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//デバイスへのポインタ
	VERTEX_2D* pVtx;									//頂点情報へのポインタ

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\BG\\town000.png",&g_pTextureBG[BG_0]);
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\BG\\town001.png",&g_pTextureBG[BG_1]);
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\BG\\town002.png",&g_pTextureBG[BG_2]);


	//頂点バッファの読み込み
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * BG_VER_NUM * BGLAYER_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBG,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBG->Lock(0, 0,(void**)&pVtx, 0);

	for (int nCntBG = 0; nCntBG < BGLAYER_MAX; nCntBG++,pVtx+=4)
	{
		//頂点座標を設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

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
		pVtx[1].tex = D3DXVECTOR2(1.2f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.2f, 1.0f);
	}

	//頂点バッファのロック解除
	g_pVtxBuffBG->Unlock();
}

//=======================================
// 背景の終了処理
//=======================================
void UninitBG(void)
{
	for (int nCntTex = 0; nCntTex < BGLAYER_MAX; nCntTex++)
	{
		if (g_pTextureBG[nCntTex] != NULL)
		{
			g_pTextureBG[nCntTex]->Release();
			g_pTextureBG[nCntTex] = NULL;
		}
	}

	if (g_pVtxBuffBG != NULL)
	{
		g_pVtxBuffBG->Release();
		g_pVtxBuffBG = NULL;
	}
}

//=======================================
// 背景更新処理
//=======================================
void UpdateBG(void)
{
	VERTEX_2D* pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBG = 0; nCntBG < BGLAYER_MAX ; nCntBG++, pVtx += 4)
	{
		//テクスチャの座標設定
		pVtx[0].tex = D3DXVECTOR2(0.0f + GetPlayer()->pos.x * 0.00005 * (BGLAYER_MAX - nCntBG), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.2f + GetPlayer()->pos.x * 0.00005 * (BGLAYER_MAX - nCntBG), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + GetPlayer()->pos.x * 0.00005 * (BGLAYER_MAX - nCntBG), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.2f + GetPlayer()->pos.x * 0.00005 * (BGLAYER_MAX - nCntBG), 1.0f);
	}

	//頂点バッファのロック解除
	g_pVtxBuffBG->Unlock();

}

//=======================================
// 背景描画処理
//=======================================
void DrawBG(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタ

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		g_pVtxBuffBG,
		0,
		sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTex = 0; nCntTex < BGLAYER_MAX; nCntTex++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureBG [nCntTex] );

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntTex, 2);
	}
}
