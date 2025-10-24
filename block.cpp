//=============================================================================
// 
// �u���b�N���� [block.cpp]
// Author : ENDO HIDETO
// 
//=============================================================================
//#include "effect.h"
//#include "game.h"
#include "block.h"
#include "input.h"

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureBlock[BLOCKTYPE_MAX] = {};			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBlock = NULL;					//���_�o�b�t�@�ւ̃|�C���^
Block g_aBlock[MAX_BLOCK];										//�I�u�W�F�N�g���
Edit g_Edit;													//�G�f�B�b�g���
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
int g_nSetBlock;											//�ݒu�ς݃u���b�N
int g_nLoadBlock;											//�ǂݍ��񂾃u���b�N

//�v���g�^�C�v�錾
void BlockEdit(VERTEX_2D* pVtx);	//�G�f�B�b�g�֘A
void PrintBlock(void);				//�u���b�N�ݒu�����e�L�X�g�t�@�C���ɏo��
void OpenFile(void);				//�t�@�C������ǂݍ���

//=======================================
// �u���b�N����������
//=======================================
void InitBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^
	VERTEX_2D* pVtx;								//���_���ւ̃|�C���^

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\Object\\block000.jpg",&g_pTextureBlock[BLOCKTYPE_NORMAL]);
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\Object\\spike.png", &g_pTextureBlock[BLOCKTYPE_SPIKE]);
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\Object\\block004.jpg",&g_pTextureBlock[BLOCKTYPE_ICE]);

	//���_�o�b�t�@�̓ǂݍ���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * BLOCK_VER_NUM * (MAX_BLOCK + 1),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBlock,
		NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK + 1; nCntBlock++, pVtx += 4)
	{
		//�e�u���b�N�\���̏�����
		g_aBlock[nCntBlock].bUse = false;

		//���_���W��ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`���̍��W�ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	}
	//���_�o�b�t�@�̃��b�N����
	g_pVtxBuffBlock->Unlock();

	//�G�f�B�b�g���[�h�I�t
	g_Edit.bEdit = false;
	g_Edit.nSetBlock = 0;
	g_Edit.nTagBlock = 0;
	g_nSetBlock = 0;
	g_nLoadBlock = 9;

	//�t�@�C���ǂݍ���
	OpenFile();

	//�u���b�N�ݒu
	for (int nCntBlock = 0; nCntBlock < g_nLoadBlock; nCntBlock++)
	{
		SetBlock(g_aBlockInfo[nCntBlock].pos, g_aBlockInfo[nCntBlock].fWidth,
			g_aBlockInfo[nCntBlock].fHeight, g_aBlockInfo[nCntBlock].move, g_aBlockInfo[nCntBlock].fRange,g_aBlockInfo[nCntBlock].type,g_aBlockInfo[nCntBlock].fDirection);
	}

}
//=======================================
// �u���b�N�I������
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
// �u���b�N�X�V����
//=======================================
void UpdateBlock(void)
{
	VERTEX_2D* pVtx;		//���_���ւ̃|�C���^
	Block* pBlock;			//�u���b�N�|�C���^
	int nId;				//�����o�����b�Z�[�W�Ԃ�l

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

#if _DEBUG
	//�G�f�B�b�g�L��/����
	if (GetKeyboardTrigger(DIK_0) == true)
		g_Edit.bEdit = g_Edit.bEdit ^ 1;

	//�����o��
	if (GetKeyboardTrigger(DIK_9) == true)
	{
		PrintBlock();
	}

	//�ǂݍ���
	if (GetKeyboardTrigger(DIK_8) == true)
	{
		//�ݒu�ς݃u���b�N�𖢎g�p�ɂ���
		for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
		{
			if (g_aBlock[nCntBlock].bUse == true)
			{
				g_nSetBlock--;
				g_aBlock[nCntBlock].bUse = false;
			}
		}

		//�t�@�C������ݒu����ǂݍ���
		OpenFile();

		//�ǂݍ��񂾐������ݒu����
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

		//���_�o�b�t�@�̃��b�N����
		g_pVtxBuffBlock->Unlock();

		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);
	}
	else
	{

		for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++, pVtx += 4)
		{
			if (g_aBlock[nCntBlock].bUse)
			{
				pBlock = &g_aBlock[nCntBlock];
				pBlock->posOld = pBlock->pos;		//�ߋ��̈ʒu��ۑ�

				//�u���b�N�ړ�
				pBlock->pos += pBlock->move;

				if (pBlock->pos.x < pBlock->posOrigin.x || pBlock->posOrigin.x + pBlock->fRange < pBlock->pos.x)
				{
					pBlock->move.x *= -1;
				}

				if (pBlock->pos.y < pBlock->posOrigin.y || pBlock->posOrigin.y + pBlock->fRange < pBlock->pos.y)
				{
					pBlock->move.y *= -1;
				}

				//���_���W��ݒ�
				pVtx[0].pos = D3DXVECTOR3(pBlock->pos.x, pBlock->pos.y, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(pBlock->pos.x + sin(D3DX_PI * 0.5f + pBlock->fDirection) * pBlock->fWidth, pBlock->pos.y + cos(D3DX_PI * 0.5f + pBlock->fDirection) * pBlock->fHeight, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pBlock->pos.x + sin(D3DX_PI * 0.0f + pBlock->fDirection) * pBlock->fWidth, pBlock->pos.y + cos(D3DX_PI * 0.0f + pBlock->fDirection) * pBlock->fHeight, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pBlock->pos.x + sin(pBlock->fAngle + pBlock->fDirection) * pBlock->fLength, pBlock->pos.y + cos(pBlock->fAngle + pBlock->fDirection) * pBlock->fLength, 0.0f);
#if 0
				//width��height�ł̕��@
				pVtx[0].pos = D3DXVECTOR3(pBlock->pos.x, pBlock->pos.y, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(pBlock->pos.x + pBlock->fWidth, pBlock->pos.y, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pBlock->pos.x, pBlock->pos.y + pBlock->fHeight, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pBlock->pos.x + pBlock->fWidth, pBlock->pos.y + pBlock->fHeight, 0.0f);
#endif

				//���_�J���[�ݒ�
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

	//���_�o�b�t�@�̃��b�N����
	g_pVtxBuffBlock->Unlock();
}

//=======================================
// �u���b�N�G�f�B�b�g����
void BlockEdit(VERTEX_2D* pVtx)
{
	Block* pBlock = &g_aBlock[g_Edit.nTagBlock];			//�u���b�N�|�C���^
	float fTmp;
	float fTmpWidth,fTmpHeight;

	//�u���b�N�ݒu
	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		SetBlock(D3DXVECTOR3(100, 100, 0), 100, 100, D3DXVECTOR3(0, 0, 0), 0, BLOCKTYPE_NORMAL,0.0f);
	}
	//�u���b�N�폜
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

	//���̃u���b�N��
	if (GetKeyboardTrigger(DIK_7) == true)
	{
		while (1)
		{
			g_Edit.nTagBlock++;
			if (g_Edit.nTagBlock >= MAX_BLOCK)
				g_Edit.nTagBlock = 0;
			if (g_aBlock[g_Edit.nTagBlock].bUse == true)
			{//�g�p���̃u���b�N������������
				pBlock = &g_aBlock[g_Edit.nTagBlock];
				fTmpWidth = pBlock->fWidth;
				fTmpHeight = pBlock->fHeight;
				break;
			}
		}
	}
	//�O�̃u���b�N��
	if (GetKeyboardTrigger(DIK_6) == true)
	{
		while (1)
		{
			g_Edit.nTagBlock--;
			if (g_Edit.nTagBlock < 0)
				g_Edit.nTagBlock = MAX_BLOCK-1;
			if (g_aBlock[g_Edit.nTagBlock].bUse == true)
			{//�g�p���̃u���b�N������������
				pBlock = &g_aBlock[g_Edit.nTagBlock];
				fTmpWidth = pBlock->fWidth;
				fTmpHeight = pBlock->fHeight;
				break;
			}

		}
	}

	//�ړ�
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

	//�傫��
	if (GetKeyboardRepeat(DIK_UP) == true && GetKeyboardPress(DIK_LSHIFT) == true)
	{
		if (g_Edit.pBlock->fHeight < SCREEN_HEIGHT)
			g_Edit.pBlock->fHeight += 5;
		//�Ίp�����C��
		pBlock->fAngle = atan2f(pBlock->fWidth, pBlock->fHeight);
		pBlock->fLength = sqrt(pow(pBlock->fWidth, 2) + pow(pBlock->fHeight, 2));
	}
	if (GetKeyboardRepeat(DIK_DOWN) == true && GetKeyboardPress(DIK_LSHIFT) == true)
	{
		if (0 < g_Edit.pBlock->fHeight)
			g_Edit.pBlock->fHeight -= 5;
		//�Ίp�����C��
		pBlock->fAngle = atan2f(pBlock->fWidth, pBlock->fHeight);
		pBlock->fLength = sqrt(pow(pBlock->fWidth, 2) + pow(pBlock->fHeight, 2));
	}
	if (GetKeyboardRepeat(DIK_LEFT) == true && GetKeyboardPress(DIK_LSHIFT) == true)
	{
		if (0 < g_Edit.pBlock->fWidth)
			g_Edit.pBlock->fWidth -= 5;
		//�Ίp�����C��
		pBlock->fAngle = atan2f(pBlock->fWidth, pBlock->fHeight);
		pBlock->fLength = sqrt(pow(pBlock->fWidth, 2) + pow(pBlock->fHeight, 2));
	}
	if (GetKeyboardRepeat(DIK_RIGHT) == true && GetKeyboardPress(DIK_LSHIFT) == true)
	{
		if (g_Edit.pBlock->fWidth < SCREEN_WIDTH)
			g_Edit.pBlock->fWidth += 5;
		//�Ίp�����C��
		pBlock->fAngle = atan2f(pBlock->fWidth, pBlock->fHeight);
		pBlock->fLength = sqrt(pow(pBlock->fWidth, 2) + pow(pBlock->fHeight, 2));
	}

	//��]
	if (GetKeyboardRepeat(DIK_O) == true)
	{//�����v���@�X�O�x����
		pBlock->fDirection += D3DX_PI * 0.5f;
		fTmp = pBlock->fWidth;
		pBlock->fWidth = pBlock->fHeight;
		pBlock->fHeight = fTmp;
	}
	if (GetKeyboardRepeat(DIK_K) == true)
	{//*���v���@�X�O�x����
		pBlock->fDirection -= D3DX_PI * 0.5f;
		fTmp = pBlock->fWidth;
		pBlock->fWidth = pBlock->fHeight;
		pBlock->fHeight = fTmp;
	}

	if (GetKeyboardRepeat(DIK_M) == true)
	{//���ɖ߂��Ȃ�
		pBlock->fDirection = 0;
	}

	//���x�E�ړ��͈�
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

	//���
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

			//���_���W��ݒ�
			pVtx[0].pos = D3DXVECTOR3(pBlock->pos.x, pBlock->pos.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pBlock->pos.x + sin(D3DX_PI * 0.5f + pBlock->fDirection) * pBlock->fWidth, pBlock->pos.y + cos(D3DX_PI * 0.5f + pBlock->fDirection) * pBlock->fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pBlock->pos.x + sin(D3DX_PI * 0.0f + pBlock->fDirection) * pBlock->fWidth, pBlock->pos.y + cos(D3DX_PI * 0.0f + pBlock->fDirection) * pBlock->fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pBlock->pos.x + sin(pBlock->fAngle + pBlock->fDirection) * pBlock->fLength, pBlock->pos.y + cos(pBlock->fAngle + pBlock->fDirection) * pBlock->fLength, 0.0f);

			if (nCntBlock == g_Edit.nTagBlock)
			{//�I�𒆂̃u���b�N�����F������
				//���_�J���[�ݒ�
				pVtx[0].col = D3DXCOLOR(0.5f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.5f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.5f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(0.5f, 1.0f, 1.0f, 1.0f);
			}
			else
			{//����ȊO�͒ʏ�F
				//���_�J���[�ݒ�
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
		}
	}
}

//=======================================
// �u���b�N�`�揈��
//=======================================
void DrawBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		g_pVtxBuffBlock,
		0,
		sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureBlock[g_aBlock[nCntBlock].type]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntBlock, 2);
		}
	}
}

//=======================================
// �u���b�N�����蔻��
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
			{//�ォ��̂߂肱��
				if (g_aBlock[nCntBlock].pos.x < (*pPosOld).x + fTagWidth
					&& (*pPosOld).x - fTagWidth < g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth)
				{//�ߋ��̈ʒu���C������Ă遁�ǂɉ����Ĉړ����ł���
					if ((*pPosOld).y <= g_aBlock[nCntBlock].pos.y)
					{//�O��̈ʒu�Ɣ�r
						(*pPos).y = g_aBlock[nCntBlock].pos.y;
						(*pMove).y = 0;
						*pBlock = &g_aBlock[nCntBlock];
						*pLand = BLOCKHIT_LAND;			//���n
					}
				}
			}

			if ((*pPos).y - fTagHeight <= g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight
				&& g_aBlock[nCntBlock].pos.x < (*pPos).x + fTagWidth
				&& (*pPos).x - fTagWidth < g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth)
			{//������̂߂肱��
				if (g_aBlock[nCntBlock].pos.x < (*pPosOld).x + fTagWidth
					&& (*pPosOld).x - fTagWidth < g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth)
				{//�ߋ��̈ʒu���C������Ă遁�ǂɉ����Ĉړ����ł���
					if (g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight <= (*pPosOld).y - fTagHeight)
					{//�O��̈ʒu�Ɣ�r
						(*pPos).y = g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight + fTagHeight;
						(*pMove).y *= 0.5f;
					}
				}
			}

			if (g_aBlock[nCntBlock].pos.x <= (*pPos).x + fTagWidth
				&& g_aBlock[nCntBlock].pos.y < (*pPos).y
				&& (*pPos).y - fTagHeight < g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight)
			{//������̂߂肱��
				if ((*pPosOld).x + fTagWidth <= g_aBlock[nCntBlock].pos.x)
				{//�O��̈ʒu�Ɣ�r
					(*pPos).x = g_aBlock[nCntBlock].pos.x - fTagWidth;
					*pBlock = &g_aBlock[nCntBlock];	//�ǂ̑��x��n��
					if (*pLand == BLOCKHIT_NONE)
					{
						*pLand = BLOCKHIT_LEFT;
					}
				}
			}

			if ((*pPos).x - fTagWidth <= g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth
				&& g_aBlock[nCntBlock].pos.y < (*pPos).y
				&& (*pPos).y - fTagHeight < g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight)
			{//�E����̂߂肱��
				if (g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth <= (*pPosOld).x - fTagWidth)
				{//�O��̈ʒu�Ɣ�r
					(*pPos).x = g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth + fTagWidth;
					*pBlock = &g_aBlock[nCntBlock];	//�ǂ̑��x��n��
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
// �u���b�N�ݒu����
//=======================================
void SetBlock(D3DXVECTOR3 pos, float fWidth, float fHeight, D3DXVECTOR3 move,float fRange, BlockType type,float fDirection)
{
	float fRot;
	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == false)
		{
			Block* pBlock = &g_aBlock[nCntBlock];

			//�l��ۑ�
			pBlock->pos = pos;
			pBlock->fWidth = fWidth;
			pBlock->fHeight = fHeight;
			pBlock->move = move;
			pBlock->fRange = fRange;
			pBlock->type = type;
			pBlock->fDirection = fDirection;

			//�Ίp��
			pBlock->fAngle = atan2f(fWidth,fHeight);
			pBlock->fLength = sqrt(pow(fWidth, 2) + pow(fHeight, 2));

			fRot = atan2f(pBlock->move.x, pBlock->move.y);	//�ړ�������ۑ�
			pBlock->posOrigin = pos;

			g_nSetBlock++;
			g_Edit.nSetBlock = g_nSetBlock;
			pBlock->bUse = true;
			break;
		}
	}
}

//=======================================
// �I�u�W�F�N�g�����擾
//=======================================
Block* GetBlock(void)
{
	return &g_aBlock[0];
}

//=======================================
//�G�f�B�b�g�����擾
//=======================================
Edit GetEdit(void)
{
	g_Edit.pBlock = &g_aBlock[g_Edit.nTagBlock];
	return g_Edit;
}

//=======================================
//�u���b�N�ݒu�����t�@�C���ɏo��
void PrintBlock(void)
{
	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{
			g_aBlockInfo[nCntBlock].pos = g_aBlock[nCntBlock].pos;				//�ʒu
			g_aBlockInfo[nCntBlock].fWidth = g_aBlock[nCntBlock].fWidth;		//��
			g_aBlockInfo[nCntBlock].fHeight = g_aBlock[nCntBlock].fHeight;		//����
			g_aBlockInfo[nCntBlock].move = g_aBlock[nCntBlock].move;			//�ړ����x
			g_aBlockInfo[nCntBlock].fRange = g_aBlock[nCntBlock].fRange;		//�ړ�����͈�
			g_aBlockInfo[nCntBlock].type = g_aBlock[nCntBlock].type;			//�u���b�N�̎��
		}
	}

	FILE* pFile;
	pFile = fopen(FILE_SAVEDATA, "wb");
	if (pFile != NULL)
	{	//�t�@�C�����J������
		fwrite(&g_nSetBlock, sizeof(int), 1, pFile);
		fwrite(&g_aBlockInfo[0],sizeof(BlockInfo),MAX_BLOCK,pFile);

		//�t�@�C�������
		fclose(pFile);
	}
#if _DEBUG
	else
	{//�J���Ȃ�������
		MessageBox(GetActiveWindow(), "�J���܂���ł���", "fileerror", MB_OK);
	}
#endif
}

//=======================================
//�u���b�N�ݒu�����t�@�C���ǂݍ���
void OpenFile(void)
{
	FILE* pFile;
	HWND hWnd;
	pFile = fopen(FILE_SAVEDATA, "rb");
	if (pFile != NULL)
	{	//�t�@�C�����J������
		fread(&g_nLoadBlock, sizeof(int), 1, pFile);
		fread(&g_aBlockInfo[0], sizeof(BlockInfo), MAX_BLOCK, pFile);

	}
#if _DEBUG
	else
	{//�J���Ȃ�������
		MessageBox(GetActiveWindow(), "�J���܂���ł���", "fileerror", MB_OK);
	}
#endif
}