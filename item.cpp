//=============================================================================
// 
// アイテム処理 [item.cpp]
// Author : ENDO HIDETO
// 
//=============================================================================
#include "item.h"
#include "input.h"
#include "player.h"


//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureItem = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffItem = NULL;		//頂点バッファへのポインタ
Item g_item;										//オブジェクト情報

//プロトタイプ宣言

//=======================================
// ゴール初期化処理
//=======================================
void InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタ
	VERTEX_2D* pVtx;								//頂点情報へのポインタ

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\object\\close.png",&g_pTextureItem);

	//頂点バッファの読み込み
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)* ITEM_VER_NUM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffItem,
		NULL);

	g_item.pos = D3DXVECTOR3(1200,300,0);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffItem->Lock(0, 0,(void**)&pVtx, 0);

	//頂点座標を設定
	pVtx[0].pos = D3DXVECTOR3(g_item.pos.x, g_item.pos.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_item.pos.x + g_item.fSize, g_item.pos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_item.pos.x, g_item.pos.y + g_item.fSize, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_item.pos.x + g_item.fSize, g_item.pos.y + g_item.fSize, 0.0f);

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

	//頂点バッファのロック解除
	g_pVtxBuffItem->Unlock();
}

//=======================================
// ゴール終了処理
//=======================================
void UninitItem(void)
{
	if (g_pTextureItem != NULL)
	{
		g_pTextureItem->Release();
		g_pTextureItem = NULL;
	}

	if (g_pVtxBuffItem != NULL)
	{
		g_pVtxBuffItem->Release();
		g_pVtxBuffItem = NULL;
	}
}

//=======================================
// ゴール更新処理
//=======================================
void UpdateItem(void)
{
	VERTEX_2D* pVtx;		//頂点情報へのポインタ
	Player* pPlayer = GetPlayer();

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	if (g_item.pos.y < pPlayer->pos.y
		&& pPlayer->pos.y - pPlayer->fHeight < g_item.pos.y + g_item.fSize
		&& g_item.pos.x < pPlayer->pos.x + pPlayer->fWidth
		&& pPlayer->pos.x - pPlayer->fWidth < g_item.pos.x + g_item.fSize)
	{//当たったら
		pPlayer->bUse = false;

		//頂点カラー設定
		pVtx[0].col = D3DXCOLOR(1.0f, 0.8f, 0.8f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.8f, 0.8f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.8f, 0.8f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.8f, 0.8f, 1.0f);
	}
	else
	{
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	}

	//頂点バッファのロック解除
	g_pVtxBuffItem->Unlock();
}

//=======================================
// ゴール描画処理
//=======================================
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタ

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		g_pVtxBuffItem,
		0,
		sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureItem);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

}
