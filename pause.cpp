//=============================================================================
// 
// ポーズ処理 [pause.cpp]
// Author : ENDO HIDETO
// 
//=============================================================================
#include "fade.h"
#include "input.h"
#include "pause.h"

//背景構造体の定義

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTexturePause[PAUSE_TEXTURE] = { NULL };		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;						//頂点バッファへのポインタ
D3DXVECTOR3 g_PauseMenupos;											//メニューを表示する位置の基準点
int g_nChose;														//選択されているメニュー
bool g_bPause = false;												//ポーズ状態の切り替え

//=======================================
// 初期化処理
//=======================================
void InitPause(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタ
	VERTEX_2D* pVtx;								//頂点情報へのポインタ

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\BG\\pause101.png",
		&g_pTexturePause[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Word\\pause000.png",
		&g_pTexturePause[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Word\\pause001.png",
		&g_pTexturePause[2]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Word\\pause002.png",
		&g_pTexturePause[3]);

	//頂点バッファの読み込み
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * (PAUSE_TEXTURE + 1),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause,
		NULL);

	//ポーズメニュー選択初期化
	g_nChose = 0;

	//ポーズメニュー基準位置
	g_PauseMenupos = D3DXVECTOR3((SCREEN_WIDTH - PAUSE_WIDTH) * 0.5f, (SCREEN_HEIGHT - (PAUSE_LINE * PAUSE_MAX)) * 0.5f, 0.0f);

	//ポーズ解除
	g_bPause = false;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	//UI背景 頂点座標を設定
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
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f);

	//テクスチャの座標設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.625f, 1.0f);
	pVtx += 4;

	//--------------
	//ポーズ中の選択肢
	for (int nCntPause = 0; nCntPause < PAUSE_MAX; nCntPause++,pVtx += 4)
	{
		//各テキスト頂点座標
		pVtx[0].pos = g_PauseMenupos;
		pVtx[1].pos = D3DXVECTOR3(g_PauseMenupos.x + PAUSE_WIDTH, g_PauseMenupos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_PauseMenupos.x, g_PauseMenupos.y + PAUSE_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_PauseMenupos.x + PAUSE_WIDTH, g_PauseMenupos.y + PAUSE_HEIGHT, 0.0f);

		//次のメニュー基準位置
		g_PauseMenupos.y += PAUSE_LINE;

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラー設定
		pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);

		//テクスチャの座標設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	
	//頂点バッファのロック解除
	g_pVtxBuffPause->Unlock();
}

//=======================================
// 終了処理
//=======================================
void UninitPause(void)
{
	for (int nCntPause = 0; nCntPause < PAUSE_TEXTURE; nCntPause++)
	{
		if (g_pTexturePause[nCntPause] != NULL)
		{
			g_pTexturePause[nCntPause]->Release();
			g_pTexturePause[nCntPause] = NULL;
		}
	}

	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}
}

//=======================================
// 更新処理
//=======================================
void UpdatePause(void)
{
	if (GetFade() == FADE_NONE)
	{
		if (GetKeyboardTrigger(DIK_P) == true || GetJoypadTrigger(JOYKEY_START) == true)
		{
			g_bPause = g_bPause ^ 1;
			g_nChose = 0;
		}
	}

	if (g_bPause == true)
	{
		VERTEX_2D* pVtx;	//頂点情報へのポインタ


		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

		//ポーズメニュー基準位置
		g_PauseMenupos = D3DXVECTOR3((SCREEN_WIDTH - PAUSE_WIDTH) * 0.5f, (SCREEN_HEIGHT - (PAUSE_LINE * PAUSE_MAX)) * 0.5f, 0.0f);

		//メニュー位置設定
		for (int nCntPause = 0; nCntPause < PAUSE_MAX; nCntPause++)
		{
			pVtx += 4;

			if (g_nChose % PAUSE_MAX == nCntPause)
			{//選択中のメニューの強調
				//頂点座標
				pVtx[0].pos = D3DXVECTOR3(g_PauseMenupos.x - PAUSE_CHOICE				, g_PauseMenupos.y - PAUSE_CHOICE, 0.0f);;
				pVtx[1].pos = D3DXVECTOR3(g_PauseMenupos.x + PAUSE_WIDTH + PAUSE_CHOICE	, g_PauseMenupos.y - PAUSE_CHOICE, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_PauseMenupos.x - PAUSE_CHOICE				, g_PauseMenupos.y + PAUSE_HEIGHT + PAUSE_CHOICE, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_PauseMenupos.x + PAUSE_WIDTH + PAUSE_CHOICE	, g_PauseMenupos.y + PAUSE_HEIGHT + PAUSE_CHOICE, 0.0f);

				//頂点カラー設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			}
			else
			{
				//頂点座標
				pVtx[0].pos = g_PauseMenupos;
				pVtx[1].pos = D3DXVECTOR3(g_PauseMenupos.x + 250, g_PauseMenupos.y, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_PauseMenupos.x, g_PauseMenupos.y + 50, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_PauseMenupos.x + 250, g_PauseMenupos.y + 50, 0.0f);

				//頂点カラー設定
				pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
				pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			}

			//次のメニュー基準位置
			g_PauseMenupos.y += 60.0f;
		}

		//メニュー操作
		if (GetKeyboardTrigger(DIK_W) == true || GetJoypadTrigger(JOYKEY_UP) == true)
		{
			g_nChose += (int)PAUSE_MAX - 1;
		}
		if (GetKeyboardTrigger(DIK_S) == true || GetJoypadTrigger(JOYKEY_DOWN) == true)
		{
			g_nChose++;
		}

		//メニュー操作を実行
		if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_A) == true)
		{
			switch (g_nChose % PAUSE_MAX)
			{
			case PAUSE_CONTINUE:
				g_nChose = 0;			//ポーズメニュー選択初期化
				g_bPause = false;
				break;

			case PAUSE_RETRY:
				g_bPause = false;
				g_nChose = 0;
				SetFade(MODE_GAME);
				break;

			case PAUSE_QUIT:
				g_bPause = false;
				g_nChose = 0;
				SetFade(MODE_TITLE);
				break;

			default:
				break;
			}
		}
		//頂点バッファのロック解除
		g_pVtxBuffPause->Unlock();
	}
}

//=======================================
// 描画処理
//=======================================
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタ

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		g_pVtxBuffPause,
		0,
		sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntPause = 0; nCntPause < PAUSE_MAX + 1; nCntPause++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexturePause[nCntPause]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,nCntPause * 4, 2);
	}
}

//=======================================
// ポーズ中かどうか取得
//=======================================
bool GetPause(void)
{
	return g_bPause;
}