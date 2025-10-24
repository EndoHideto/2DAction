//=============================================================================
// 
// �v���C���[�̏��� [player.cpp]
// Author : ENDO HIDETO
// 
//=============================================================================
#include "input.h"
#include "player.h"
#include "retry.h"
#include "timer.h"

//�}�N����`

//�v���C���[�A�j���[�V�����\����

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTexturePlayer[PLAYERSTATE_MAX] = {};		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;				//���_�o�b�t�@�ւ̃|�C���^
Player g_Player;
float g_fJunpSpeed;
float g_fGravity;

//�v���g�^�C�v�錾
void PlayerMove(void);
void MoveReply(void);
void PlayerJump(void);
void Collision(void);

//=======================================
// �v���C���[�̏���������
//=======================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^
	VERTEX_2D* pVtx;								//���_���ւ̃|�C���^

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\Charcter\\cat_wait.png",&g_pTexturePlayer[PLAYERSTATE_NORMAL]);	//�ҋ@
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\Charcter\\cat_walk.png",&g_pTexturePlayer[PLAYERSTATE_WALK]);		//����
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\Charcter\\cat_walk.png",&g_pTexturePlayer[PLAYERSTATE_JUMP]);		//�W�����v
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\Charcter\\cat_walk.png",&g_pTexturePlayer[PLAYERSTATE_AIRJUMP]);	//��i�W�����v
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\Charcter\\cat_walk.png",&g_pTexturePlayer[PLAYERSTATE_FALL]);		//����
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\Charcter\\dead000.png",&g_pTexturePlayer[PLAYERSTATE_DEATH]);	//�����҂�

	//���_�o�b�t�@�̓ǂݍ���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL);

	//�����ʒu�ƈړ��ʂ�ݒ�
	g_Player.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_Player.pos = PLAYER_START_POS;		//�����ʒu
	g_Player.posOld = PLAYER_START_POS;
	g_Player.move = MOVE_ZERO;
	g_Player.fDirectionMove = 0.5f;			//���E�i0 ���� 0.5�j
	g_Player.fWidth = PLAYER_WIDTH;
	g_Player.fHeight = PLAYER_HEIGHT;
	g_Player.pBlock = NULL;
	g_Player.bUse = true;
	g_Player.nDeath = 0;
	g_Player.nCntStart = 0;
	g_fJunpSpeed = JUMP_FORCE;				//�W�����v�̃p���[
	g_fGravity = GRAVITY;				//�d��

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0,(void**)&pVtx, 0);

	//�v���C���[�{�̂̒��_���W��ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_Player.pos.x - g_Player.fWidth, g_Player.pos.y - g_Player.fHeight,0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Player.pos.x + g_Player.fWidth, g_Player.pos.y - g_Player.fHeight,0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Player.pos.x - g_Player.fWidth, g_Player.pos.y,0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Player.pos.x + g_Player.fWidth, g_Player.pos.y,0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

	//�e�N�X�`���̍��W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f,  0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.25f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f,  0.5f);
	pVtx[3].tex = D3DXVECTOR2(0.25f, 0.5f);

	//���_�o�b�t�@�̃��b�N����
	g_pVtxBuffPlayer->Unlock();

}

//=======================================
// �v���C���[�̏I������
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
// �v���C���[�X�V����
//=======================================
void UpdatePlayer(void)
{
	VERTEX_2D* pVtx;								//���_���ւ̃|�C���^

	if(g_Player.state == PLAYERSTATE_DEATH)
	{//����ł�����
		g_Player.nDeath++;
		g_Player.nCntStart = 0;
	}
	else if (g_Player.bUse == false)
	{//����s�\�Ȃ�
		g_Player.pos = PLAYER_START_POS;
		g_Player.move = MOVE_ZERO;
		g_Player.bUse = true;
	}
	else if (g_Player.bUse == true && g_Player.state != PLAYERSTATE_DEATH)
	{//�����Ă��đ���\�Ȃ�

		//�O��̈ʒu�ʒu��ۑ�
		g_Player.posOld = g_Player.pos;

		g_Player.nCntAnim++;
		if (g_Player.nCntAnim % 8 == 0)
		{
			g_Player.nPtnAnim++;
		}

		//�����̊ԑ���s��
		g_Player.nCntStart++;
		if (REVIVALTIME < g_Player.nCntStart)
		{
			//�^�C�}�[���X�^�[�g
			BreakTimer(false);

			//�v���C���[�̑���
			PlayerMove();
		}
		//�v���C���[�ړ�
		MoveReply();
	}
		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

		//�v���C���[�{�̂̒��_���W��ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_Player.pos.x - g_Player.fWidth, g_Player.pos.y - g_Player.fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Player.pos.x + g_Player.fWidth, g_Player.pos.y - g_Player.fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Player.pos.x - g_Player.fWidth, g_Player.pos.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Player.pos.x + g_Player.fWidth, g_Player.pos.y, 0.0f);

		//���_�J���[�ݒ�
		pVtx[0].col = g_Player.col;
		pVtx[1].col = g_Player.col;
		pVtx[2].col = g_Player.col;
		pVtx[3].col = g_Player.col;


		if (g_Player.state == PLAYERSTATE_DEATH)
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
		else
		{
			//�e�N�X�`���̍��W�ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.25f * g_Player.nPtnAnim, g_Player.fDirectionMove);
			pVtx[1].tex = D3DXVECTOR2(0.25f * (g_Player.nPtnAnim + 1), g_Player.fDirectionMove);
			pVtx[2].tex = D3DXVECTOR2(0.25f * g_Player.nPtnAnim, g_Player.fDirectionMove + 0.5f);
			pVtx[3].tex = D3DXVECTOR2(0.25f * (g_Player.nPtnAnim + 1), g_Player.fDirectionMove + 0.5f);
		}

		//���_�o�b�t�@�̃��b�N����
		g_pVtxBuffPlayer->Unlock();
	
#if _DEBUG
	if(GetKeyboardRepeat(DIK_E) == true)
	{
		g_fJunpSpeed += 0.1f;
	}
	if(GetKeyboardRepeat(DIK_Q) == true)
	{
		g_fJunpSpeed -= 0.1f;
	}
	if(GetKeyboardRepeat(DIK_C) == true)
	{
		g_fGravity += 0.02f;
	}
	if(GetKeyboardRepeat(DIK_Z) == true)
	{
		g_fGravity -= 0.02f;
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
// �v���C���[�̍s������
void PlayerMove()
{
	float fAccel = ACCEL;

	//�󒆂ɂ����牡�ړ����x��
	if (g_Player.state == PLAYERSTATE_JUMP)
	{
		fAccel *= 0.8f;
	}
	else if (g_Player.state == PLAYERSTATE_AIRJUMP)
	{
		fAccel *= 0.5f;
	}

	//�ړ�����
	if ((GetKeyboardPress(DIK_A) == true) || (GetKeyboardPress(DIK_D) == true))
	{//�L�[�{�[�h����
		if (GetKeyboardPress(DIK_A) == true)
		{//��
			g_Player.move.x += sinf(-0.5f * D3DX_PI) * fAccel;
			if (g_Player.state != PLAYERSTATE_WALLSIDE)
				g_Player.fDirectionMove = DIRECTION_LEFT;
		}

		if (GetKeyboardPress(DIK_D) == true)
		{//�E
			g_Player.move.x += sinf(0.5f * D3DX_PI) * fAccel;
			if (g_Player.state != PLAYERSTATE_WALLSIDE)
				g_Player.fDirectionMove = DIRECTION_RIGHT;
		}
	}
	else if (GetJoypadStick(LEFT).x != 0 || GetJoypadStick(LEFT).y != 0)
	{//�R���g���[���[����
		g_Player.move.x = GetJoypadStick(LEFT).x * fAccel;
		if (g_Player.move.x < 0)
		{
			g_Player.fDirectionMove = 0.0f;
		}
		if (0 < g_Player.move.x)
		{
			g_Player.fDirectionMove = 0.5f;
		}

		//���x����
		if (MAX_SPEED < g_Player.move.x)
		{
			g_Player.move.x = MAX_SPEED;
		}
		else if (g_Player.move.x < -MAX_SPEED)
		{
			g_Player.move.x = -MAX_SPEED;
		}

	}
	else if (-0.2f <= g_Player.move.x && g_Player.move.x <= 0.2f)
	{//���x�����ȉ��Œ�~
		g_Player.move.x = 0;
	}
	//�W�����v
	PlayerJump();

}

//=======================================
// �v���C���[�̈ړ�����
void MoveReply(void)
{
	//�d�͂̉e��
	g_Player.move.y += g_fGravity;

	//�ړ�
	g_Player.pos += g_Player.move;

	//�����蔻��
	Collision();

	//�u���b�N�̉e�����󂯂��ړ�
	if (g_Player.pBlock != NULL)
	{
		g_Player.move += g_Player.pBlock->move;
		if (g_Player.pBlock->type == BLOCKTYPE_SPIKE)
		{//�Ƃ��ɓ������Ă����ꍇ�͎���
			BreakTimer(true);
			g_Player.state = PLAYERSTATE_DEATH;
			g_Player.bUse = false;
			return;
		}
	}
	if (g_Player.state == PLAYERSTATE_WALLSIDE)
	{
		g_Player.move.y *= 0.8f;
	}

	//����
	g_Player.move.x += (0.0f - g_Player.move.x) * 0.1f;
}

//=======================================
// �v���C���[�̃W�����v����
void PlayerJump(void)
{
	if (GetKeyboardTrigger(DIK_SPACE) == true || GetJoypadTrigger(JOYKEY_A) == true)
	{//�W�����v����
		if (g_Player.state != PLAYERSTATE_JUMP && g_Player.state != PLAYERSTATE_AIRJUMP && g_Player.state != PLAYERSTATE_FALL)
		{
			if (g_Player.state == PLAYERSTATE_WALLSIDE)
			{
				g_Player.state = PLAYERSTATE_JUMP;
				g_Player.move.y = -g_fJunpSpeed* 0.75f;
				g_Player.move.x = (g_Player.fDirectionMove * 4 - 1) * 10.0f;
			}
			else
			{
				g_Player.state = PLAYERSTATE_JUMP;
				g_Player.move.y = -g_fJunpSpeed;
			}
		}
		else if (g_Player.state != PLAYERSTATE_AIRJUMP && g_Player.state != PLAYERSTATE_FALL)
		{
			g_Player.state = PLAYERSTATE_AIRJUMP;
			g_Player.move.y = -g_fJunpSpeed * 0.8f;
		}
	}
}

//=======================================
// �O���̓����蔻��
void Collision(void)
{
	Player* pPlayer = &g_Player;
	int nLand = 0b0000;

	//�O���̓����蔻��
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
		g_Player.state = PLAYERSTATE_DEATH;
		g_Player.bUse = false;
		return;
	}

	//�u���b�N�̓����蔻��
	CollisionBlock(&pPlayer->pos, &pPlayer->posOld, &pPlayer->move, pPlayer->fHeight, pPlayer->fWidth, &pPlayer->pBlock,&nLand);
	if (nLand != BLOCKHIT_NONE)
	{//���n������
		g_Player.bDash = false;
		if (nLand  == BLOCKHIT_LAND)
		{//���n���Ă���
			if (g_Player.move.x != 0)
				g_Player.state = PLAYERSTATE_WALK;
			else
				g_Player.state = PLAYERSTATE_NORMAL;
		}
		else
		{//����ȊO�̕Ԃ�l����������
			g_Player.state = PLAYERSTATE_WALLSIDE;
			g_Player.fDirectionMove = (nLand - 1) * 0.5f;
		}
	}
	else if(g_Player.state != PLAYERSTATE_FALL && g_Player.state != PLAYERSTATE_AIRJUMP)
	{//�󒆂ɂ�����
		g_Player.state = PLAYERSTATE_JUMP;
	}
}

//=======================================
// �v���C���[�`�揈��
//=======================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		g_pVtxBuffPlayer,
		0,
		sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�v���C���[�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTexturePlayer[g_Player.state]);

	//�v���C���[�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=======================================
// �v���C���[CPP����̏��̎擾
//=======================================
void GetPlayer(float* pGravity, float* pJunpSpeed)
{
	*pGravity = g_fGravity;
	*pJunpSpeed = g_fJunpSpeed;
}

//=======================================
// �v���C���[���̎擾
//=======================================
Player* GetPlayer(void)
{
	return &g_Player;
}