//=============================================================================
// 
// フェードの処理 [fade.cpp]
// Author : ENDO HIDETO
// 
//=============================================================================
#include "fade.h"
//#include "pause.h"

//背景構造体の定義
typedef struct
{
	FADE mode;
	float fAlpha;
	MODE nextmode;
}Fade;

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureFade = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFade = NULL;		//頂点バッファへのポインタ
Fade g_aFade;										//フェード情報

//=======================================
// フェードの初期化処理
//=======================================
void InitFade(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタ
	VERTEX_2D* pVtx;								//頂点情報へのポインタ

	//テクスチャ読み込み

	//頂点バッファの読み込み
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*FADE_VER_NUM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffFade,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffFade->Lock(0, 0,(void**)&pVtx, 0);

	//フェード未使用・透明に設定
	g_aFade.mode = FADE_NONE;
	g_aFade.fAlpha = 0.0f;

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
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_aFade.fAlpha);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_aFade.fAlpha);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_aFade.fAlpha);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_aFade.fAlpha);

	//テクスチャの座標設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
  	
	//頂点バッファのロック解除
	g_pVtxBuffFade->Unlock();
}

//=======================================
// フェードのの終了処理
//=======================================
void UninitFade(void)
{
	if (g_pTextureFade != NULL)
	{
		g_pTextureFade->Release();
		g_pTextureFade = NULL;
	}

	if (g_pVtxBuffFade != NULL)
	{
		g_pVtxBuffFade->Release();
		g_pVtxBuffFade = NULL;
	}
}

//=======================================
// フェードの更新処理
//=======================================
void UpdateFade(void)
{
	MODE mode = GetMode();				//現在のモード取得
	VERTEX_2D* pVtx;					//頂点情報へのポインタ

	if (g_aFade.mode!= FADE_NONE)
	{
		if (g_aFade.mode == FADE_IN)
		{//フェードイン　透明にする
			g_aFade.fAlpha -= 0.05f;

			if (g_aFade.fAlpha <= 0.0f)
			{//透明度が０以下になったら
				g_aFade.fAlpha = 0.0f;
				g_aFade.mode = FADE_NONE;
			}
		}
		else if (g_aFade.mode == FADE_OUT)
		{//フェードアウト　暗くする
			g_aFade.fAlpha += 0.05f;

			if (g_aFade.fAlpha >= 1.0f)
			{//透明度が１以上になったら
				g_aFade.fAlpha = 1.0f;
				g_aFade.mode = FADE_IN;

				//次モードセット
				SetMode(g_aFade.nextmode);
			}
		}

		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

		//頂点カラー設定
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_aFade.fAlpha);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_aFade.fAlpha);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_aFade.fAlpha);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_aFade.fAlpha);

		//頂点バッファのロック解除

		g_pVtxBuffFade->Unlock();
	}
}

//=======================================
// フェードの描画処理
//=======================================
void DrawFade(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタ

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		g_pVtxBuffFade,
		0,
		sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, NULL);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);

}

//=======================================
// フェード設定処理
//=======================================
void SetFade(MODE nextmode)
{
	//フェードアウト開始
	g_aFade.mode = FADE_OUT;

	//次のモードを保存
	g_aFade.nextmode = nextmode;

	//フェードイン
	if (g_aFade.fAlpha >= 1.0f)
	{//透明度が１以上になったら
		g_aFade.mode = FADE_IN;
	}
}

//=======================================
// フェード状態取得
//=======================================
FADE GetFade(void)
{
	return g_aFade.mode;
}
