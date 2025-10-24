//=============================================================================
// 
// ブロック処理 [block.cpp]
// Author : ENDO HIDETO
// 
//=============================================================================
//#include "effect.h"
//#include "game.h"
#include "block.h"
#include "input.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureBlock[BLOCKTYPE_MAX] = {};			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBlock = NULL;					//頂点バッファへのポインタ
Block g_aBlock[MAX_BLOCK];										//オブジェクト情報
Edit g_Edit;													//エディット情報
BlockInfo g_aBlockInfo[MAX_BLOCK] =
{
	{D3DXVECTOR3(0.0f, SCREEN_HEIGHT - 30, 0.0f), SCREEN_WIDTH, 30.0f,D3DXVECTOR3(0, 0, 0), 0,BLOCKTYPE_NORMAL,0},
	{D3DXVECTOR3(300, 605, 0.0f)	,80	,10,	D3DXVECTOR3(0, 0, 0), 0,BLOCKTYPE_NORMAL,0},
	{D3DXVECTOR3(425, 485, 0.0f)	,100,10,	D3DXVECTOR3(0, 0, 0), 0,BLOCKTYPE_NORMAL,0},
	{D3DXVECTOR3(640, 410, 0.0f)	,100,290,	D3DXVECTOR3(0, 0, 0), 0,BLOCKTYPE_NORMAL,0},
	{D3DXVECTOR3(840, 550, 0.0f)	,75	,10,	D3DXVECTOR3(0, 0, 0), 0,BLOCKTYPE_NORMAL,0},
	{D3DXVECTOR3(725, 535, 0.0f)	,65	,10,	D3DXVECTOR3(0, 0, 0), 0,BLOCKTYPE_NORMAL,0},
	{D3DXVECTOR3(825, 45, 0.0f)		,30	,270,	D3DXVECTOR3(0, 0, 0), 0,BLOCKTYPE_NORMAL,0},
	{D3DXVECTOR3(935, 380, 0.0f)	,100,10,	D3DXVECTOR3(0, 0, 0), 0,BLOCKTYPE_NORMAL,0},
	{D3DXVECTOR3(505, 655, 0.0f)	,135,35,	D3DXVECTOR3(0, 0, 0), 0,BLOCKTYPE_SPIKE	,0},
};
int g_nSetBlock;											//設置済みブロック
int g_nLoadBlock;											//読み込んだブロック

//プロトタイプ宣言
void BlockEdit(VERTEX_2D* pVtx);	//エディット関連
void PrintBlock(void);				//ブロック設置情報をテキストファイルに出力
void OpenFile(void);				//ファイルから読み込む

//=======================================
// ブロック初期化処理
//=======================================
void InitBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタ
	VERTEX_2D* pVtx;								//頂点情報へのポインタ

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\Object\\block000.jpg",&g_pTextureBlock[BLOCKTYPE_NORMAL]);
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\Object\\spike.png", &g_pTextureBlock[BLOCKTYPE_SPIKE]);
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\Object\\block004.jpg",&g_pTextureBlock[BLOCKTYPE_ICE]);

	//頂点バッファの読み込み
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * BLOCK_VER_NUM * (MAX_BLOCK + 1),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBlock,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK + 1; nCntBlock++, pVtx += 4)
	{
		//各ブロック構造体初期化
		g_aBlock[nCntBlock].bUse = false;

		//頂点座標を設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

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
	g_pVtxBuffBlock->Unlock();

	//エディットモードオフ
	g_Edit.bEdit = false;
	g_Edit.nSetBlock = 0;
	g_Edit.nTagBlock = 0;
	g_nSetBlock = 0;
	g_nLoadBlock = 9;

	//ファイル読み込み
	OpenFile();

	//ブロック設置
	for (int nCntBlock = 0; nCntBlock < g_nLoadBlock; nCntBlock++)
	{
		SetBlock(g_aBlockInfo[nCntBlock].pos, g_aBlockInfo[nCntBlock].fWidth,
			g_aBlockInfo[nCntBlock].fHeight, g_aBlockInfo[nCntBlock].move, g_aBlockInfo[nCntBlock].fRange,g_aBlockInfo[nCntBlock].type,g_aBlockInfo[nCntBlock].fDirection);
	}

}
//=======================================
// ブロック終了処理
//=======================================
void UninitBlock(void)
{
	for (int nCntTex = 0; nCntTex < BLOCKTYPE_MAX; nCntTex++)
	{
		if (g_pTextureBlock[nCntTex] != NULL)
		{
			g_pTextureBlock[nCntTex]->Release();
			g_pTextureBlock[nCntTex] = NULL;
		}
	}

	if (g_pVtxBuffBlock != NULL)
	{
		g_pVtxBuffBlock->Release();
		g_pVtxBuffBlock = NULL;
	}
}

//=======================================
// ブロック更新処理
//=======================================
void UpdateBlock(void)
{
	VERTEX_2D* pVtx;		//頂点情報へのポインタ
	Block* pBlock;			//ブロックポインタ
	int nId;				//書き出しメッセージ返り値

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

#if _DEBUG
	//エディット有効/無効
	if (GetKeyboardTrigger(DIK_0) == true)
		g_Edit.bEdit = g_Edit.bEdit ^ 1;

	//書き出し
	if (GetKeyboardTrigger(DIK_9) == true)
	{
		PrintBlock();
	}

	//読み込み
	if (GetKeyboardTrigger(DIK_8) == true)
	{
		//設置済みブロックを未使用にする
		for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
		{
			if (g_aBlock[nCntBlock].bUse == true)
			{
				g_nSetBlock--;
				g_aBlock[nCntBlock].bUse = false;
			}
		}

		//ファイルから設置情報を読み込む
		OpenFile();

		//読み込んだ数だけ設置する
		for (int nCntBlock = 0; nCntBlock < g_nLoadBlock; nCntBlock++)
		{
			SetBlock(g_aBlockInfo[nCntBlock].pos, g_aBlockInfo[nCntBlock].fWidth,
				g_aBlockInfo[nCntBlock].fHeight, g_aBlockInfo[nCntBlock].move, g_aBlockInfo[nCntBlock].fRange, g_aBlockInfo[nCntBlock].type, g_aBlockInfo[nCntBlock].fDirection);
		}
	}
#endif

	if (g_Edit.bEdit)
	{
		BlockEdit(pVtx);

		//頂点バッファのロック解除
		g_pVtxBuffBlock->Unlock();

		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);
	}
	else
	{

		for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++, pVtx += 4)
		{
			if (g_aBlock[nCntBlock].bUse)
			{
				pBlock = &g_aBlock[nCntBlock];
				pBlock->posOld = pBlock->pos;		//過去の位置を保存

				//ブロック移動
				pBlock->pos += pBlock->move;

				if (pBlock->pos.x < pBlock->posOrigin.x || pBlock->posOrigin.x + pBlock->fRange < pBlock->pos.x)
				{
					pBlock->move.x *= -1;
				}

				if (pBlock->pos.y < pBlock->posOrigin.y || pBlock->posOrigin.y + pBlock->fRange < pBlock->pos.y)
				{
					pBlock->move.y *= -1;
				}

				//頂点座標を設定
				pVtx[0].pos = D3DXVECTOR3(pBlock->pos.x, pBlock->pos.y, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(pBlock->pos.x + sin(D3DX_PI * 0.5f + pBlock->fDirection) * pBlock->fWidth, pBlock->pos.y + cos(D3DX_PI * 0.5f + pBlock->fDirection) * pBlock->fHeight, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pBlock->pos.x + sin(D3DX_PI * 0.0f + pBlock->fDirection) * pBlock->fWidth, pBlock->pos.y + cos(D3DX_PI * 0.0f + pBlock->fDirection) * pBlock->fHeight, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pBlock->pos.x + sin(pBlock->fAngle + pBlock->fDirection) * pBlock->fLength, pBlock->pos.y + cos(pBlock->fAngle + pBlock->fDirection) * pBlock->fLength, 0.0f);
#if 0
				//widthとheightでの方法
				pVtx[0].pos = D3DXVECTOR3(pBlock->pos.x, pBlock->pos.y, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(pBlock->pos.x + pBlock->fWidth, pBlock->pos.y, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pBlock->pos.x, pBlock->pos.y + pBlock->fHeight, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pBlock->pos.x + pBlock->fWidth, pBlock->pos.y + pBlock->fHeight, 0.0f);
#endif

				//頂点カラー設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				if (pBlock->type == BLOCKTYPE_SPIKE)
				{
					pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2((int)(pBlock->fWidth / BLOCKTEX_INTERVAL), 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2((int)(pBlock->fWidth / BLOCKTEX_INTERVAL),1.0f);
				}
				else
				{
					pVtx[0].tex = D3DXVECTOR2(pBlock->pos.x / BLOCKTEX_INTERVAL, pBlock->pos.y / BLOCKTEX_INTERVAL);
					pVtx[1].tex = D3DXVECTOR2((pBlock->pos.x + pBlock->fWidth) / BLOCKTEX_INTERVAL, pBlock->pos.y / BLOCKTEX_INTERVAL);
					pVtx[2].tex = D3DXVECTOR2(pBlock->pos.x / BLOCKTEX_INTERVAL, (pBlock->pos.y + pBlock->fHeight) / BLOCKTEX_INTERVAL);
					pVtx[3].tex = D3DXVECTOR2((pBlock->pos.x + pBlock->fWidth) / BLOCKTEX_INTERVAL, (pBlock->pos.y + pBlock->fHeight) / BLOCKTEX_INTERVAL);
				}
			}
		}
	}

	//頂点バッファのロック解除
	g_pVtxBuffBlock->Unlock();
}

//=======================================
// ブロックエディット操作
void BlockEdit(VERTEX_2D* pVtx)
{
	Block* pBlock = &g_aBlock[g_Edit.nTagBlock];			//ブロックポインタ
	float fTmp;
	float fTmpWidth,fTmpHeight;

	//ブロック設置
	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		SetBlock(D3DXVECTOR3(100, 100, 0), 100, 100, D3DXVECTOR3(0, 0, 0), 0, BLOCKTYPE_NORMAL,0.0f);
	}
	//ブロック削除
	if (GetKeyboardTrigger(DIK_BACK) == true)
	{
		if (g_Edit.nSetBlock > 1)
		{
			g_aBlock[g_Edit.nTagBlock].bUse = false;
			g_nSetBlock--;
			g_Edit.nSetBlock = g_nSetBlock;
			while (1)
			{
				g_Edit.nTagBlock++;
				if (g_Edit.nTagBlock >= MAX_BLOCK)
					g_Edit.nTagBlock = 0;
				if (g_aBlock[g_Edit.nTagBlock].bUse == true)
					break;
			}
		}
	}

	//次のブロックへ
	if (GetKeyboardTrigger(DIK_7) == true)
	{
		while (1)
		{
			g_Edit.nTagBlock++;
			if (g_Edit.nTagBlock >= MAX_BLOCK)
				g_Edit.nTagBlock = 0;
			if (g_aBlock[g_Edit.nTagBlock].bUse == true)
			{//使用中のブロックが見つかったら
				pBlock = &g_aBlock[g_Edit.nTagBlock];
				fTmpWidth = pBlock->fWidth;
				fTmpHeight = pBlock->fHeight;
				break;
			}
		}
	}
	//前のブロックへ
	if (GetKeyboardTrigger(DIK_6) == true)
	{
		while (1)
		{
			g_Edit.nTagBlock--;
			if (g_Edit.nTagBlock < 0)
				g_Edit.nTagBlock = MAX_BLOCK-1;
			if (g_aBlock[g_Edit.nTagBlock].bUse == true)
			{//使用中のブロックが見つかったら
				pBlock = &g_aBlock[g_Edit.nTagBlock];
				fTmpWidth = pBlock->fWidth;
				fTmpHeight = pBlock->fHeight;
				break;
			}

		}
	}

	//移動
	if (GetKeyboardRepeat(DIK_UP) == true)
	{
		g_Edit.pBlock->pos.y -= 5;
		g_Edit.pBlock->posOrigin.y -= 5;
	}
	if (GetKeyboardRepeat(DIK_DOWN) == true)
	{
		g_Edit.pBlock->pos.y += 5;
		g_Edit.pBlock->posOrigin.y += 5;
	}
	if (GetKeyboardRepeat(DIK_LEFT) == true)
	{
		g_Edit.pBlock->pos.x -= 5;
		g_Edit.pBlock->posOrigin.x -= 5;
	}
	if (GetKeyboardRepeat(DIK_RIGHT) == true)
	{
		g_Edit.pBlock->pos.x += 5;
		g_Edit.pBlock->posOrigin.x += 5;
	}

	//大きさ
	if (GetKeyboardRepeat(DIK_UP) == true && GetKeyboardPress(DIK_LSHIFT) == true)
	{
		if (g_Edit.pBlock->fHeight < SCREEN_HEIGHT)
			g_Edit.pBlock->fHeight += 5;
		//対角線を修正
		pBlock->fAngle = atan2f(pBlock->fWidth, pBlock->fHeight);
		pBlock->fLength = sqrt(pow(pBlock->fWidth, 2) + pow(pBlock->fHeight, 2));
	}
	if (GetKeyboardRepeat(DIK_DOWN) == true && GetKeyboardPress(DIK_LSHIFT) == true)
	{
		if (0 < g_Edit.pBlock->fHeight)
			g_Edit.pBlock->fHeight -= 5;
		//対角線を修正
		pBlock->fAngle = atan2f(pBlock->fWidth, pBlock->fHeight);
		pBlock->fLength = sqrt(pow(pBlock->fWidth, 2) + pow(pBlock->fHeight, 2));
	}
	if (GetKeyboardRepeat(DIK_LEFT) == true && GetKeyboardPress(DIK_LSHIFT) == true)
	{
		if (0 < g_Edit.pBlock->fWidth)
			g_Edit.pBlock->fWidth -= 5;
		//対角線を修正
		pBlock->fAngle = atan2f(pBlock->fWidth, pBlock->fHeight);
		pBlock->fLength = sqrt(pow(pBlock->fWidth, 2) + pow(pBlock->fHeight, 2));
	}
	if (GetKeyboardRepeat(DIK_RIGHT) == true && GetKeyboardPress(DIK_LSHIFT) == true)
	{
		if (g_Edit.pBlock->fWidth < SCREEN_WIDTH)
			g_Edit.pBlock->fWidth += 5;
		//対角線を修正
		pBlock->fAngle = atan2f(pBlock->fWidth, pBlock->fHeight);
		pBlock->fLength = sqrt(pow(pBlock->fWidth, 2) + pow(pBlock->fHeight, 2));
	}

	//回転
	if (GetKeyboardRepeat(DIK_O) == true)
	{//反時計回り　９０度刻み
		pBlock->fDirection += D3DX_PI * 0.5f;
		fTmp = pBlock->fWidth;
		pBlock->fWidth = pBlock->fHeight;
		pBlock->fHeight = fTmp;
	}
	if (GetKeyboardRepeat(DIK_K) == true)
	{//*時計回り　９０度刻み
		pBlock->fDirection -= D3DX_PI * 0.5f;
		fTmp = pBlock->fWidth;
		pBlock->fWidth = pBlock->fHeight;
		pBlock->fHeight = fTmp;
	}

	if (GetKeyboardRepeat(DIK_M) == true)
	{//元に戻せない
		pBlock->fDirection = 0;
	}

	//速度・移動範囲
	if (GetKeyboardRepeat(DIK_I) == true)
	{
		if (GetKeyboardPress(DIK_LSHIFT) == true)
		{
			g_Edit.pBlock->move.y += 0.1;
		}
		else
		{
			g_Edit.pBlock->move.x += 0.1;
		}
	}
	if (GetKeyboardRepeat(DIK_J) == true)
	{
		if (GetKeyboardPress(DIK_LSHIFT) == true)
		{
			g_Edit.pBlock->move.y -= 0.1;
		}
		else
		{
			g_Edit.pBlock->move.x -= 0.1;
		}
	}
	if (GetKeyboardRepeat(DIK_U) == true)
	{
		g_Edit.pBlock->fRange += 5;
	}
	if (GetKeyboardRepeat(DIK_H) == true)
	{
		g_Edit.pBlock->fRange -= 5;
	}

	//種類
	if (GetKeyboardTrigger(DIK_5) == true)
	{
		if ((int)g_Edit.pBlock->type + 1 < BLOCKTYPE_MAX)
		{
			g_Edit.pBlock->type = (BlockType)((int)g_Edit.pBlock->type + 1);
		}
		else
		{
			g_Edit.pBlock->type = (BlockType)0;
		}
	}
	if (GetKeyboardTrigger(DIK_4) == true)
	{
		if(0 < (int)g_Edit.pBlock->type)
		g_Edit.pBlock->type = (BlockType)((int)g_Edit.pBlock->type - 1);
		else
			g_Edit.pBlock->type = (BlockType)((int)BLOCKTYPE_MAX - 1);
	}

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++, pVtx += 4)
	{
		if (g_aBlock[nCntBlock].bUse)
		{
			pBlock = &g_aBlock[nCntBlock];

			//頂点座標を設定
			pVtx[0].pos = D3DXVECTOR3(pBlock->pos.x, pBlock->pos.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pBlock->pos.x + sin(D3DX_PI * 0.5f + pBlock->fDirection) * pBlock->fWidth, pBlock->pos.y + cos(D3DX_PI * 0.5f + pBlock->fDirection) * pBlock->fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pBlock->pos.x + sin(D3DX_PI * 0.0f + pBlock->fDirection) * pBlock->fWidth, pBlock->pos.y + cos(D3DX_PI * 0.0f + pBlock->fDirection) * pBlock->fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pBlock->pos.x + sin(pBlock->fAngle + pBlock->fDirection) * pBlock->fLength, pBlock->pos.y + cos(pBlock->fAngle + pBlock->fDirection) * pBlock->fLength, 0.0f);

			if (nCntBlock == g_Edit.nTagBlock)
			{//選択中のブロックを黄色くする
				//頂点カラー設定
				pVtx[0].col = D3DXCOLOR(0.5f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.5f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.5f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(0.5f, 1.0f, 1.0f, 1.0f);
			}
			else
			{//それ以外は通常色
				//頂点カラー設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
		}
	}
}

//=======================================
// ブロック描画処理
//=======================================
void DrawBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタ

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		g_pVtxBuffBlock,
		0,
		sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBlock[g_aBlock[nCntBlock].type]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntBlock, 2);
		}
	}
}

//=======================================
// ブロック当たり判定
//=======================================
void CollisionBlock(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove,float fTagHeight,float fTagWidth, Block** pBlock,int*pLand)
{
	bool bHitHead = false;
	bool bHitSpike = false;
	D3DXVECTOR3 posKeep = *pPos;
	D3DXVECTOR3 moveKeep = *pMove;

	if (pBlock != NULL)
	{
		*pBlock = NULL;
	}

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{
			if (g_aBlock[nCntBlock].pos.y <= (*pPos).y
				&& g_aBlock[nCntBlock].pos.x < (*pPos).x + fTagWidth
				&& (*pPos).x - fTagWidth < g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth)
			{//上からのめりこみ
				if (g_aBlock[nCntBlock].pos.x < (*pPosOld).x + fTagWidth
					&& (*pPosOld).x - fTagWidth < g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth)
				{//過去の位置が修正されてる＝壁に沿って移動中である
					if ((*pPosOld).y <= g_aBlock[nCntBlock].pos.y)
					{//前回の位置と比較
						(*pPos).y = g_aBlock[nCntBlock].pos.y;
						(*pMove).y = 0;
						*pBlock = &g_aBlock[nCntBlock];
						*pLand = BLOCKHIT_LAND;			//着地
					}
				}
			}

			if ((*pPos).y - fTagHeight <= g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight
				&& g_aBlock[nCntBlock].pos.x < (*pPos).x + fTagWidth
				&& (*pPos).x - fTagWidth < g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth)
			{//下からのめりこみ
				if (g_aBlock[nCntBlock].pos.x < (*pPosOld).x + fTagWidth
					&& (*pPosOld).x - fTagWidth < g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth)
				{//過去の位置が修正されてる＝壁に沿って移動中である
					if (g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight <= (*pPosOld).y - fTagHeight)
					{//前回の位置と比較
						(*pPos).y = g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight + fTagHeight;
						(*pMove).y *= 0.5f;
					}
				}
			}

			if (g_aBlock[nCntBlock].pos.x <= (*pPos).x + fTagWidth
				&& g_aBlock[nCntBlock].pos.y < (*pPos).y
				&& (*pPos).y - fTagHeight < g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight)
			{//左からのめりこみ
				if ((*pPosOld).x + fTagWidth <= g_aBlock[nCntBlock].pos.x)
				{//前回の位置と比較
					(*pPos).x = g_aBlock[nCntBlock].pos.x - fTagWidth;
					*pBlock = &g_aBlock[nCntBlock];	//壁の速度を渡す
					if (*pLand == BLOCKHIT_NONE)
					{
						*pLand = BLOCKHIT_LEFT;
					}
				}
			}

			if ((*pPos).x - fTagWidth <= g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth
				&& g_aBlock[nCntBlock].pos.y < (*pPos).y
				&& (*pPos).y - fTagHeight < g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight)
			{//右からのめりこみ
				if (g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth <= (*pPosOld).x - fTagWidth)
				{//前回の位置と比較
					(*pPos).x = g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth + fTagWidth;
					*pBlock = &g_aBlock[nCntBlock];	//壁の速度を渡す
					if (*pLand == BLOCKHIT_NONE)
					{
						*pLand = BLOCKHIT_RIGHT;
					}
				}
			}
		}
	}
}

//=======================================
// ブロック設置処理
//=======================================
void SetBlock(D3DXVECTOR3 pos, float fWidth, float fHeight, D3DXVECTOR3 move,float fRange, BlockType type,float fDirection)
{
	float fRot;
	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == false)
		{
			Block* pBlock = &g_aBlock[nCntBlock];

			//値を保存
			pBlock->pos = pos;
			pBlock->fWidth = fWidth;
			pBlock->fHeight = fHeight;
			pBlock->move = move;
			pBlock->fRange = fRange;
			pBlock->type = type;
			pBlock->fDirection = fDirection;

			//対角線
			pBlock->fAngle = atan2f(fWidth,fHeight);
			pBlock->fLength = sqrt(pow(fWidth, 2) + pow(fHeight, 2));

			fRot = atan2f(pBlock->move.x, pBlock->move.y);	//移動方向を保存
			pBlock->posOrigin = pos;

			g_nSetBlock++;
			g_Edit.nSetBlock = g_nSetBlock;
			pBlock->bUse = true;
			break;
		}
	}
}

//=======================================
// オブジェクト情報を取得
//=======================================
Block* GetBlock(void)
{
	return &g_aBlock[0];
}

//=======================================
//エディット情報を取得
//=======================================
Edit GetEdit(void)
{
	g_Edit.pBlock = &g_aBlock[g_Edit.nTagBlock];
	return g_Edit;
}

//=======================================
//ブロック設置情報をファイルに出力
void PrintBlock(void)
{
	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{
			g_aBlockInfo[nCntBlock].pos = g_aBlock[nCntBlock].pos;				//位置
			g_aBlockInfo[nCntBlock].fWidth = g_aBlock[nCntBlock].fWidth;		//幅
			g_aBlockInfo[nCntBlock].fHeight = g_aBlock[nCntBlock].fHeight;		//高さ
			g_aBlockInfo[nCntBlock].move = g_aBlock[nCntBlock].move;			//移動速度
			g_aBlockInfo[nCntBlock].fRange = g_aBlock[nCntBlock].fRange;		//移動する範囲
			g_aBlockInfo[nCntBlock].type = g_aBlock[nCntBlock].type;			//ブロックの種類
		}
	}

	FILE* pFile;
	pFile = fopen(FILE_SAVEDATA, "wb");
	if (pFile != NULL)
	{	//ファイルが開けたら
		fwrite(&g_nSetBlock, sizeof(int), 1, pFile);
		fwrite(&g_aBlockInfo[0],sizeof(BlockInfo),MAX_BLOCK,pFile);

		//ファイルを閉じる
		fclose(pFile);
	}
#if _DEBUG
	else
	{//開けなかったら
		MessageBox(GetActiveWindow(), "開けませんでした", "fileerror", MB_OK);
	}
#endif
}

//=======================================
//ブロック設置情報をファイル読み込み
void OpenFile(void)
{
	FILE* pFile;
	HWND hWnd;
	pFile = fopen(FILE_SAVEDATA, "rb");
	if (pFile != NULL)
	{	//ファイルが開けたら
		fread(&g_nLoadBlock, sizeof(int), 1, pFile);
		fread(&g_aBlockInfo[0], sizeof(BlockInfo), MAX_BLOCK, pFile);

	}
#if _DEBUG
	else
	{//開けなかったら
		MessageBox(GetActiveWindow(), "開けませんでした", "fileerror", MB_OK);
	}
#endif
}