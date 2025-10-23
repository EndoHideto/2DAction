//=============================================================================
// 
// �S�[������ [goal.cpp]
// Author : ENDO HIDETO
// 
//=============================================================================
//#include "effect.h"
//#include "game.h"
#include "goal.h"
#include "input.h"
#include "player.h"
#include "timer.h"


//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureGoal[TEXTURE_GOAL] = {};	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGoal = NULL;			//���_�o�b�t�@�ւ̃|�C���^
Goal g_goal;											//�I�u�W�F�N�g���
bool g_bFinish;											//�N���A�t���O

//�v���g�^�C�v�錾

//=======================================
// �S�[������������
//=======================================
void InitGoal(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^
	VERTEX_2D* pVtx;								//���_���ւ̃|�C���^

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\object\\close.png",&g_pTextureGoal[1]);
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\object\\goal.png",&g_pTextureGoal[0]);

	//���_�o�b�t�@�̓ǂݍ���
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

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffGoal->Lock(0, 0,(void**)&pVtx, 0);

	//���_���W��ݒ�
	pVtx[0].pos.x = g_goal.pos.x - sinf(-g_goal.fRot + g_goal.fAngle) * g_goal.fLength;
	pVtx[0].pos.y = g_goal.pos.y - cosf(-g_goal.fRot + g_goal.fAngle) * g_goal.fLength;
	pVtx[1].pos.x = g_goal.pos.x + sinf(g_goal.fRot + g_goal.fAngle) * g_goal.fLength;
	pVtx[1].pos.y = g_goal.pos.y - cosf(g_goal.fRot + g_goal.fAngle) * g_goal.fLength;
	pVtx[2].pos.x = g_goal.pos.x - sinf(g_goal.fRot + g_goal.fAngle) * g_goal.fLength;
	pVtx[2].pos.y = g_goal.pos.y + cosf(g_goal.fRot + g_goal.fAngle) * g_goal.fLength;
	pVtx[3].pos.x = g_goal.pos.x + sinf(-g_goal.fRot + g_goal.fAngle) * g_goal.fLength;
	pVtx[3].pos.y = g_goal.pos.y + cosf(-g_goal.fRot + g_goal.fAngle) * g_goal.fLength;

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

	//���_�o�b�t�@�̃��b�N����
	g_pVtxBuffGoal->Unlock();
}

//=======================================
// �S�[���I������
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
// �S�[���X�V����
//=======================================
void UpdateGoal(void)
{
	VERTEX_2D* pVtx;		//���_���ւ̃|�C���^
	Player* pPlayer = GetPlayer();

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffGoal->Lock(0, 0, (void**)&pVtx, 0);

	if (g_goal.pos.y - cosf(g_goal.fAngle) * g_goal.fLength < pPlayer->pos.y
		&& pPlayer->pos.y - pPlayer->fHeight < g_goal.pos.y + cosf(g_goal.fAngle) * g_goal.fLength
		&& g_goal.pos.x - sinf(g_goal.fAngle) * g_goal.fLength < pPlayer->pos.x + pPlayer->fWidth
		&& pPlayer->pos.x - pPlayer->fWidth < g_goal.pos.x + sinf(g_goal.fAngle) * g_goal.fLength)
	{//����������
		StopTimer();
		g_bFinish = true;

		//���_�J���[�ݒ�
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

	//���_�o�b�t�@�̃��b�N����
	g_pVtxBuffGoal->Unlock();
}

//=======================================
// �S�[���`�揈��
//=======================================
void DrawGoal(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		g_pVtxBuffGoal,
		0,
		sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureGoal[g_goal.nOpen]);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

}

//=======================================
// �N���A�t���O�擾
//=======================================
bool GameFinish(void)
{
	return g_bFinish;
}
