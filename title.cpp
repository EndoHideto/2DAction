//=============================================================================
// 
// タイトル [title.cpp]
// Author : ENDO HIDETO
// 
//=============================================================================
#include "input.h"
#include "title.h"
#include "fade.h"
//#include "sound.h"

//構造体の定義

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureTitle[TITLE_TEXTURE] = {};		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;				//頂点バッファへのポインタ
int g_nCouonterTitle;										//タイトル画面のカウンター
TitileObject g_titleObj[TITLE_THING];						//タイトル画面に表示する物

D3DXVECTOR3 g_posTitle;										//タイトルの位置
D3DXVECTOR3 g_posLight[2];									//照明の位置
D3DXCOLOR g_pressAlpha;										//Press Enterの色
float g_fLenght;											//対角線の長さ
float g_fAngle;												//対角線の角度
float g_fRot;												//対角線の向き

//プロトタイプ宣言
void TitleSet(void);										//初期化

//=======================================
// 背景の初期化処理
//=======================================
void InitTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタ
	VERTEX_2D* pVtx;								//頂点情報へのポインタ

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\BG\\town.png",&g_pTextureTitle[0]);				//背景
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\Word\\Title_Jump Cat.png",&g_pTextureTitle[1]);	//タイトル
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\Word\\end.png",&g_pTextureTitle[2]);				//始める
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\Word\\end.png",&g_pTextureTitle[3]);				//終る
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\Object\\light.png",&g_pTextureTitle[4]);			//飾り


	//頂点バッファの読み込み
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * TITLE_THING,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitle,
		NULL);

	//タイトルの位置
	g_posTitle = D3DXVECTOR3 TITLE_POS;
	//ライトの対角線
	g_fLenght =(float)sqrt(200*200 + 500*500);
	g_fAngle  = atan2f(200,500);
	g_fRot = 0;
	g_posLight[0] = D3DXVECTOR3(65.0f, 720.0f, 0.0f);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0,(void**)&pVtx, 0);

	for (int nCntTitle = 0; nCntTitle < TITLE_THING; nCntTitle++, pVtx += 4)
	{
		//背景頂点座標を設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//テクスチャの座標設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//頂点カラー設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
	}
	//頂点バッファのロック解除
	g_pVtxBuffTitle->Unlock();

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);
	//背景頂点座標を設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//タイトル
	pVtx[4].pos = D3DXVECTOR3(g_posTitle.x - TITLE_WIDTH,g_posTitle.y - TITLE_HEIGHT, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(g_posTitle.x + TITLE_WIDTH,g_posTitle.y - TITLE_HEIGHT, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(g_posTitle.x - TITLE_WIDTH,g_posTitle.y,0.0f);
	pVtx[7].pos = D3DXVECTOR3(g_posTitle.x + TITLE_WIDTH,g_posTitle.y,0.0f);

	//照明
	pVtx[ 8].pos = D3DXVECTOR3(g_posLight[0].x - sin(g_fAngle) * g_fLenght, g_posLight[0].y - cos(g_fAngle) * g_fLenght, 0.0f);
	pVtx[ 9].pos = D3DXVECTOR3(g_posLight[0].x + sin(g_fAngle) * g_fLenght, g_posLight[0].y - cos(g_fAngle) * g_fLenght, 0.0f);
	pVtx[10].pos = D3DXVECTOR3(g_posLight[0].x - sin(g_fAngle) * g_fLenght, g_posLight[0].y, 0.0f);
	pVtx[11].pos = D3DXVECTOR3(g_posLight[0].x + sin(g_fAngle) * g_fLenght, g_posLight[0].y, 0.0f);

	//頂点バッファのロック解除
	g_pVtxBuffTitle->Unlock();

	//音楽再生
	//PlaySound(SOUND_LABEL_BGM003);

	//カウンター初期化
	g_nCouonterTitle = 0;
}

//=======================================
// 背景の終了処理
//=======================================
void UninitTitle(void)
{
	for (int nCntTex = 0; nCntTex < TITLE_TEXTURE; nCntTex++)
	{
		if (g_pTextureTitle[nCntTex] != NULL)
		{
			g_pTextureTitle[nCntTex]->Release();
			g_pTextureTitle[nCntTex] = NULL;
		}
	}

	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}

	//音楽停止
	//StopSound();
}

//=======================================
// 背景更新処理
//=======================================
void UpdateTitle(void)
{
	VERTEX_2D* pVtx;		//頂点情報へのポインタ
	
	
	if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_A) == true)
	{//決定キーが押されたら
		//モード遷移
		SetFade(MODE_GAME);
	}

	if (GetKeyboardPress(DIK_3) == true)
	{
		g_fRot -=	0.01f;
	}
	if (GetKeyboardPress(DIK_4) == true)
	{
		g_fRot +=	0.01f;
	}

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	//照明
	pVtx[ 8].pos = D3DXVECTOR3(g_posLight[0].x - sin(g_fAngle - g_fRot) * g_fLenght, g_posLight[0].y - cos(g_fAngle - g_fRot) * g_fLenght, 0.0f);
	pVtx[ 9].pos = D3DXVECTOR3(g_posLight[0].x + sin(g_fAngle + g_fRot) * g_fLenght, g_posLight[0].y - cos(g_fAngle + g_fRot) * g_fLenght, 0.0f);
	pVtx[10].pos = D3DXVECTOR3(g_posLight[0].x - sin(D3DX_PI * 0.5 + g_fRot) * 200, g_posLight[0].y + cos(D3DX_PI * 0.5 + g_fRot) * 200, 0.0f);
	pVtx[11].pos = D3DXVECTOR3(g_posLight[0].x + sin(D3DX_PI * 0.5 - g_fRot) * 200, g_posLight[0].y + cos(D3DX_PI * 0.5 - g_fRot) * 200, 0.0f);

	//頂点バッファのロック解除
	g_pVtxBuffTitle->Unlock();
}

//=======================================
// 背景描画処理
//=======================================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタ

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		g_pVtxBuffTitle,
		0,
		sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTitle[0]);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTitle[1]);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, 2);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTitle[2]);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 8, 2);

}

//=======================================
// 初期化（見やすさのため関数分け
void TitleSet(void)
{
	g_titleObj[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//背景
	g_titleObj[1].pos = D3DXVECTOR3 TITLE_POS;				//ゲームタイトル
	g_titleObj[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//始める
	g_titleObj[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//おわる
	g_titleObj[4].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//飾りライト１
	g_titleObj[5].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//飾りライト２

}