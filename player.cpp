//=============================================================================
// 
// プレイヤーの処理 [player.cpp]
// Author : ENDO HIDETO
// 
//=============================================================================
#include "input.h"
#include "player.h"
#include "retry.h"

//マクロ定義

//プレイヤーアニメーション構造体

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTexturePlayer[PLAYERSTATE_MAX] = {};		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;				//頂点バッファへのポインタ
Player g_Player;
float g_fJunpSpeed;
float g_fGravity;

//プロトタイプ宣言
void PlayerMove(void);
void PlayerJump(void);
void Collision(void);

//=======================================
// プレイヤーの初期化処理
//=======================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタ
	VERTEX_2D* pVtx;								//頂点情報へのポインタ

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\Charcter\\cat_wait.png",&g_pTexturePlayer[PLAYERSTATE_NORMAL]);	//待機
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\Charcter\\cat_walk.png",&g_pTexturePlayer[PLAYERSTATE_WALK]);		//歩き
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\Charcter\\cat_walk.png",&g_pTexturePlayer[PLAYERSTATE_JUMP]);		//ジャンプ
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\Charcter\\cat_walk.png",&g_pTexturePlayer[PLAYERSTATE_AIRJUMP]);	//二段ジャンプ
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\Charcter\\cat_walk.png",&g_pTexturePlayer[PLAYERSTATE_FALL]);		//落下
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\Charcter\\NULL",&g_pTexturePlayer[PLAYERSTATE_DEATH]);	//復活待ち

	//頂点バッファの読み込み
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL);

	//初期位置と移動量を設定
	g_Player.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_Player.pos = PLAYER_START_POS;		//初期位置
	g_Player.posOld = PLAYER_START_POS;
	g_Player.move = MOVE_ZERO;
	g_Player.fDirectionMove = 0.5f;			//左右（0 ＜＞ 0.5）
	g_Player.fWidth = PLAYER_WIDTH;
	g_Player.fHeight = PLAYER_HEIGHT;
	g_Player.pBlock = NULL;
	g_Player.bUse = true;
	g_Player.nDeath = 0;
	g_Player.nCntStart = 0;
	g_fJunpSpeed = SPEED_JUMP;				//ジャンプのパワー
	g_fGravity = GRAVITY;					//重力

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0,(void**)&pVtx, 0);

	//プレイヤー本体の頂点座標を設定
	pVtx[0].pos = D3DXVECTOR3(g_Player.pos.x - g_Player.fWidth, g_Player.pos.y - g_Player.fHeight,0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Player.pos.x + g_Player.fWidth, g_Player.pos.y - g_Player.fHeight,0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Player.pos.x - g_Player.fWidth, g_Player.pos.y,0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Player.pos.x + g_Player.fWidth, g_Player.pos.y,0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラー設定
	pVtx[0].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

	//テクスチャの座標設定
	pVtx[0].tex = D3DXVECTOR2(0.0f,  0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.25f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f,  0.5f);
	pVtx[3].tex = D3DXVECTOR2(0.25f, 0.5f);

	//頂点バッファのロック解除
	g_pVtxBuffPlayer->Unlock();

}

//=======================================
// プレイヤーの終了処理
//=======================================
void UninitPlayer(void)
{
	for (int nCntTex = 0; nCntTex < PLAYERSTATE_MAX; nCntTex++)
	{
		if (g_pTexturePlayer[nCntTex] != NULL)
		{
			g_pTexturePlayer[nCntTex]->Release();
			g_pTexturePlayer[nCntTex] = NULL;
		}
	}

	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}
}

//=======================================
// プレイヤー更新処理
//=======================================
void UpdatePlayer(void)
{
	VERTEX_2D* pVtx;								//頂点情報へのポインタ

	if(g_Player.state == PLAYERSTATE_DEATH)
	{//死んでいたら
		g_Player.nDeath++;
		g_Player.nCntStart = 0;
	}
	else if (g_Player.bUse == false)
	{//操作不可能なら
		g_Player.pos = PLAYER_START_POS;
		g_Player.move = MOVE_ZERO;
		g_Player.bUse = true;
	}
	else if (g_Player.bUse == true && g_Player.state != PLAYERSTATE_DEATH)
	{//生きていて操作可能なら
		//前回の位置位置を保存
		g_Player.posOld = g_Player.pos;

		g_Player.nCntStart++;
		g_Player.nCntAnim++;
		if (g_Player.nCntAnim % 8 == 0)
		{
			g_Player.nPtnAnim++;
		}
		//プレイヤーの移動・操作
		PlayerMove();
	}
		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

		//プレイヤー本体の頂点座標を設定
		pVtx[0].pos = D3DXVECTOR3(g_Player.pos.x - g_Player.fWidth, g_Player.pos.y - g_Player.fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Player.pos.x + g_Player.fWidth, g_Player.pos.y - g_Player.fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Player.pos.x - g_Player.fWidth, g_Player.pos.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Player.pos.x + g_Player.fWidth, g_Player.pos.y, 0.0f);

		//頂点カラー設定
		pVtx[0].col = g_Player.col;
		pVtx[1].col = g_Player.col;
		pVtx[2].col = g_Player.col;
		pVtx[3].col = g_Player.col;

		//テクスチャの座標設定
		pVtx[0].tex = D3DXVECTOR2(0.25f * g_Player.nPtnAnim, g_Player.fDirectionMove);
		pVtx[1].tex = D3DXVECTOR2(0.25f * (g_Player.nPtnAnim + 1), g_Player.fDirectionMove);
		pVtx[2].tex = D3DXVECTOR2(0.25f * g_Player.nPtnAnim, g_Player.fDirectionMove + 0.5f);
		pVtx[3].tex = D3DXVECTOR2(0.25f * (g_Player.nPtnAnim + 1), g_Player.fDirectionMove + 0.5f);

		//頂点バッファのロック解除
		g_pVtxBuffPlayer->Unlock();
	
#if _DEBUG
	if(GetKeyboardRepeat(DIK_E) == true)
	{
		g_fJunpSpeed += 0.1;
	}
	if(GetKeyboardRepeat(DIK_Q) == true)
	{
		g_fJunpSpeed -= 0.1;
	}
	if(GetKeyboardRepeat(DIK_C) == true)
	{
		g_fGravity += 0.02;
	}
	if(GetKeyboardRepeat(DIK_Z) == true)
	{
		g_fGravity -= 0.02;
	}
	if (GetKeyboardRepeat(DIK_R) == true)
	{
		g_Player.col.r = (((int)(g_Player.col.r * 10) + 1) % 11) * 0.1f;
	}
	if (GetKeyboardRepeat(DIK_G) == true)
	{
		g_Player.col.g = (((int)(g_Player.col.g * 10) + 1) % 11) * 0.1f;

	}
	if (GetKeyboardRepeat(DIK_B) == true)
	{
		g_Player.col.b = (((int)(g_Player.col.b * 10) + 1) % 11) * 0.1f;

	}

#endif
}

//=======================================
// プレイヤーの移動処理
void PlayerMove()
{	
	if (REVIVALTIME < g_Player.nCntStart)
	{
		//移動操作
		if ((GetKeyboardPress(DIK_A) == true) || (GetKeyboardPress(DIK_D) == true))
		{//キーボード操作
			if (GetKeyboardPress(DIK_A) == true)
			{//左
				g_Player.move.x = sinf(-0.5f * D3DX_PI) * ACCEL;
				g_Player.fDirectionMove = 0.0f;
			}

			if (GetKeyboardPress(DIK_D) == true)
			{//右
				g_Player.move.x = sinf(0.5f * D3DX_PI) * ACCEL;
				g_Player.fDirectionMove = 0.5f;
			}

		}
		else if (GetJoypadStick(LEFT).x != 0 || GetJoypadStick(LEFT).y != 0)
		{//コントローラー操作

			g_Player.move.x = GetJoypadStick(LEFT).x * ACCEL;
			if (g_Player.move.x < 0)
			{
				g_Player.fDirectionMove = 0.0f;
			}
			if (0 < g_Player.move.x)
			{
				g_Player.fDirectionMove = 0.5f;
			}
		}
		else if (-0.2f <= g_Player.move.x && g_Player.move.x <= 0.2f)
		{//速度が一定以下で停止
			g_Player.move.x = 0;
		}

		//ジャンプ
		PlayerJump();
	}

	//重力の影響
	g_Player.move.y += g_fGravity;

	//移動
	g_Player.pos += g_Player.move;

	//当たり判定
	Collision();

	//ブロックの影響を受けた移動
	if (g_Player.pBlock != NULL)
	{
		g_Player.move += g_Player.pBlock->move;
		if (g_Player.pBlock->type == BLOCKTYPE_SPIKE)
		{
			g_Player.state = PLAYERSTATE_DEATH;
			g_Player.bUse = false;
		}
	}

	//慣性
	g_Player.move.x += (0.0 - g_Player.move.x) * 0.1f;
}

//=======================================
// プレイヤーのジャンプ処理
void PlayerJump(void)
{
	if (GetKeyboardTrigger(DIK_SPACE) == true || GetJoypadTrigger(JOYKEY_A) == true)
	{//ジャンプ処理
		if (g_Player.state != PLAYERSTATE_JUMP && g_Player.state != PLAYERSTATE_AIRJUMP && g_Player.state != PLAYERSTATE_FALL)
		{
			g_Player.state = PLAYERSTATE_JUMP;
			g_Player.move.y = -g_fJunpSpeed;
		}
		else if (g_Player.state != PLAYERSTATE_AIRJUMP && g_Player.state != PLAYERSTATE_FALL)
		{
			g_Player.state = PLAYERSTATE_AIRJUMP;
			g_Player.move.y = -g_fJunpSpeed * 0.8f;
		}
	}
}

//=======================================
// 外側の当たり判定
void Collision(void)
{
	Player* pPlayer = &g_Player;

	if (pPlayer->pos.x < 0)
	{
		pPlayer->pos.x = 0;
		if(g_Player.move.x < 0.0f)
		g_Player.move.x = 0.0f;
	}

	if (SCREEN_WIDTH < pPlayer->pos.x)
	{
		pPlayer->pos.x = SCREEN_WIDTH;
		if(g_Player.move.x > 0.0f)
		g_Player.move.x = 0.0f;
	}

	if (pPlayer->pos.y >= SCREEN_HEIGHT)
	{
		if (g_Player.move.y > 0.0f)
		{
			g_Player.move.y = 0.0f;
		}
		g_Player.state = PLAYERSTATE_NORMAL;
		g_Player.pos.y = SCREEN_HEIGHT;
	}

	if (CollisionBlock(&pPlayer->pos, &pPlayer->posOld, &pPlayer->move,pPlayer->fHeight,pPlayer->fWidth,&pPlayer->pBlock) == true)
	{//着地したら
		g_Player.bDash = false;
		if(g_Player.move.x != 0)
		g_Player.state = PLAYERSTATE_WALK;
		else
		g_Player.state = PLAYERSTATE_NORMAL;
	}
	else if(g_Player.state != PLAYERSTATE_JUMP && g_Player.state != PLAYERSTATE_AIRJUMP)
	{
		g_Player.state = PLAYERSTATE_JUMP;
	}
}

//=======================================
// プレイヤー描画処理
//=======================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタ

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		g_pVtxBuffPlayer,
		0,
		sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//プレイヤーテクスチャの設定
	pDevice->SetTexture(0, g_pTexturePlayer[g_Player.state]);

	//プレイヤーポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=======================================
// プレイヤーCPPからの情報の取得
//=======================================
void GetPlayer(float* pGravity, float* pJunpSpeed)
{
	*pGravity = g_fGravity;
	*pJunpSpeed = g_fJunpSpeed;
}

//=======================================
// プレイヤー情報の取得
//=======================================
Player* GetPlayer(void)
{
	return &g_Player;
}