//=============================================================================
// 
// ゴール処理 [goal.cpp]
// Author : ENDO HIDETO
// 
//=============================================================================
//#include "effect.h"
//#include "game.h"
#include "goal.h"
#include "input.h"
#include "player.h"
#include "timer.h"


//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureGoal[TEXTURE_GOAL] = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGoal = NULL;			//頂点バッファへのポインタ
Goal g_goal;											//オブジェクト情報
bool g_bFinish;											//クリアフラグ

//プロトタイプ宣言

//=======================================
// ゴール初期化処理
//=======================================
void InitGoal(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタ
	VERTEX_2D* pVtx;								//頂点情報へのポインタ

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\object\\close.png",&g_pTextureGoal[1]);
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\object\\goal.png",&g_pTextureGoal[0]);

	//頂点バッファの読み込み
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)* GOAL_VER_NUM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGoal,
		NULL);

	g_goal.pos = D3DXVECTOR3(1200,300,0);
	g_goal.fLength = sqrt(GOAL_SIZE*GOAL_SIZE);
	g_goal.fAngle = atan2f(GOAL_SIZE, GOAL_SIZE);
	g_goal.fRot = 0;
	g_goal.nOpen = 0;
	g_bFinish = false;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGoal->Lock(0, 0,(void**)&pVtx, 0);

	//頂点座標を設定
	pVtx[0].pos.x = g_goal.pos.x - sinf(-g_goal.fRot + g_goal.fAngle) * g_goal.fLength;
	pVtx[0].pos.y = g_goal.pos.y - cosf(-g_goal.fRot + g_goal.fAngle) * g_goal.fLength;
	pVtx[1].pos.x = g_goal.pos.x + sinf(g_goal.fRot + g_goal.fAngle) * g_goal.fLength;
	pVtx[1].pos.y = g_goal.pos.y - cosf(g_goal.fRot + g_goal.fAngle) * g_goal.fLength;
	pVtx[2].pos.x = g_goal.pos.x - sinf(g_goal.fRot + g_goal.fAngle) * g_goal.fLength;
	pVtx[2].pos.y = g_goal.pos.y + cosf(g_goal.fRot + g_goal.fAngle) * g_goal.fLength;
	pVtx[3].pos.x = g_goal.pos.x + sinf(-g_goal.fRot + g_goal.fAngle) * g_goal.fLength;
	pVtx[3].pos.y = g_goal.pos.y + cosf(-g_goal.fRot + g_goal.fAngle) * g_goal.fLength;

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
	g_pVtxBuffGoal->Unlock();
}

//=======================================
// ゴール終了処理
//=======================================
void UninitGoal(void)
{
	for (int nCntTex = 0; nCntTex < TEXTURE_GOAL; nCntTex++)
	{
		if (g_pTextureGoal[nCntTex] != NULL)
		{
			g_pTextureGoal[nCntTex]->Release();
			g_pTextureGoal[nCntTex] = NULL;
		}
	}

	if (g_pVtxBuffGoal != NULL)
	{
		g_pVtxBuffGoal->Release();
		g_pVtxBuffGoal = NULL;
	}
}

//=======================================
// ゴール更新処理
//=======================================
void UpdateGoal(void)
{
	VERTEX_2D* pVtx;		//頂点情報へのポインタ
	Player* pPlayer = GetPlayer();

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGoal->Lock(0, 0, (void**)&pVtx, 0);

	if (g_goal.pos.y - cosf(g_goal.fAngle) * g_goal.fLength < pPlayer->pos.y
		&& pPlayer->pos.y - pPlayer->fHeight < g_goal.pos.y + cosf(g_goal.fAngle) * g_goal.fLength
		&& g_goal.pos.x - sinf(g_goal.fAngle) * g_goal.fLength < pPlayer->pos.x + pPlayer->fWidth
		&& pPlayer->pos.x - pPlayer->fWidth < g_goal.pos.x + sinf(g_goal.fAngle) * g_goal.fLength)
	{//当たったら
		StopTimer();
		g_bFinish = true;

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
	g_goal.fRot += 0.02f;
	pVtx[0].pos.x = g_goal.pos.x - sinf(-g_goal.fRot + g_goal.fAngle) * g_goal.fLength;
	pVtx[0].pos.y = g_goal.pos.y - cosf(-g_goal.fRot + g_goal.fAngle) * g_goal.fLength;
	pVtx[1].pos.x = g_goal.pos.x + sinf(g_goal.fRot + g_goal.fAngle) * g_goal.fLength;
	pVtx[1].pos.y = g_goal.pos.y - cosf(g_goal.fRot + g_goal.fAngle) * g_goal.fLength;
	pVtx[2].pos.x = g_goal.pos.x - sinf(g_goal.fRot + g_goal.fAngle) * g_goal.fLength;
	pVtx[2].pos.y = g_goal.pos.y + cosf(g_goal.fRot + g_goal.fAngle) * g_goal.fLength;
	pVtx[3].pos.x = g_goal.pos.x + sinf(-g_goal.fRot + g_goal.fAngle) * g_goal.fLength;
	pVtx[3].pos.y = g_goal.pos.y + cosf(-g_goal.fRot + g_goal.fAngle) * g_goal.fLength;

	//頂点バッファのロック解除
	g_pVtxBuffGoal->Unlock();
}

//=======================================
// ゴール描画処理
//=======================================
void DrawGoal(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタ

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		g_pVtxBuffGoal,
		0,
		sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureGoal[g_goal.nOpen]);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

}

//=======================================
// クリアフラグ取得
//=======================================
bool GameFinish(void)
{
	return g_bFinish;
}
