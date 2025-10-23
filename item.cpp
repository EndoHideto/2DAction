//=============================================================================
// 
// �A�C�e������ [item.cpp]
// Author : ENDO HIDETO
// 
//=============================================================================
#include "item.h"
#include "input.h"
#include "player.h"


//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureItem = NULL;			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffItem = NULL;		//���_�o�b�t�@�ւ̃|�C���^
Item g_item;										//�I�u�W�F�N�g���

//�v���g�^�C�v�錾

//=======================================
// �S�[������������
//=======================================
void InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^
	VERTEX_2D* pVtx;								//���_���ւ̃|�C���^

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\object\\close.png",&g_pTextureItem);

	//���_�o�b�t�@�̓ǂݍ���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)* ITEM_VER_NUM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffItem,
		NULL);

	g_item.pos = D3DXVECTOR3(1200,300,0);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffItem->Lock(0, 0,(void**)&pVtx, 0);

	//���_���W��ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_item.pos.x, g_item.pos.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_item.pos.x + g_item.fSize, g_item.pos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_item.pos.x, g_item.pos.y + g_item.fSize, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_item.pos.x + g_item.fSize, g_item.pos.y + g_item.fSize, 0.0f);

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
	g_pVtxBuffItem->Unlock();
}

//=======================================
// �S�[���I������
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
// �S�[���X�V����
//=======================================
void UpdateItem(void)
{
	VERTEX_2D* pVtx;		//���_���ւ̃|�C���^
	Player* pPlayer = GetPlayer();

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	if (g_item.pos.y < pPlayer->pos.y
		&& pPlayer->pos.y - pPlayer->fHeight < g_item.pos.y + g_item.fSize
		&& g_item.pos.x < pPlayer->pos.x + pPlayer->fWidth
		&& pPlayer->pos.x - pPlayer->fWidth < g_item.pos.x + g_item.fSize)
	{//����������
		pPlayer->bUse = false;

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

	//���_�o�b�t�@�̃��b�N����
	g_pVtxBuffItem->Unlock();
}

//=======================================
// �S�[���`�揈��
//=======================================
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		g_pVtxBuffItem,
		0,
		sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureItem);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

}
