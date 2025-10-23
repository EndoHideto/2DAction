//=============================================================================
// 
// �w�i�̏��� [bg.cpp]
// Author : ENDO HIDETO
// 
//=============================================================================
#include "bg.h"
#include "player.h"

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureBG[BGLAYER_MAX] = {};		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBG = NULL;			//���_�o�b�t�@�ւ̃|�C���^

//=======================================
// �w�i�̏���������
//=======================================
void InitBG(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//�f�o�C�X�ւ̃|�C���^
	VERTEX_2D* pVtx;									//���_���ւ̃|�C���^

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\BG\\town000.png",&g_pTextureBG[BG_0]);
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\BG\\town001.png",&g_pTextureBG[BG_1]);
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\BG\\town002.png",&g_pTextureBG[BG_2]);


	//���_�o�b�t�@�̓ǂݍ���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * BG_VER_NUM * BGLAYER_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBG,
		NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBG->Lock(0, 0,(void**)&pVtx, 0);

	for (int nCntBG = 0; nCntBG < BGLAYER_MAX; nCntBG++,pVtx+=4)
	{
		//���_���W��ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

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
		pVtx[1].tex = D3DXVECTOR2(1.2f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.2f, 1.0f);
	}

	//���_�o�b�t�@�̃��b�N����
	g_pVtxBuffBG->Unlock();
}

//=======================================
// �w�i�̏I������
//=======================================
void UninitBG(void)
{
	for (int nCntTex = 0; nCntTex < BGLAYER_MAX; nCntTex++)
	{
		if (g_pTextureBG[nCntTex] != NULL)
		{
			g_pTextureBG[nCntTex]->Release();
			g_pTextureBG[nCntTex] = NULL;
		}
	}

	if (g_pVtxBuffBG != NULL)
	{
		g_pVtxBuffBG->Release();
		g_pVtxBuffBG = NULL;
	}
}

//=======================================
// �w�i�X�V����
//=======================================
void UpdateBG(void)
{
	VERTEX_2D* pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBG = 0; nCntBG < BGLAYER_MAX ; nCntBG++, pVtx += 4)
	{
		//�e�N�X�`���̍��W�ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f + GetPlayer()->pos.x * 0.00005 * (BGLAYER_MAX - nCntBG), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.2f + GetPlayer()->pos.x * 0.00005 * (BGLAYER_MAX - nCntBG), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + GetPlayer()->pos.x * 0.00005 * (BGLAYER_MAX - nCntBG), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.2f + GetPlayer()->pos.x * 0.00005 * (BGLAYER_MAX - nCntBG), 1.0f);
	}

	//���_�o�b�t�@�̃��b�N����
	g_pVtxBuffBG->Unlock();

}

//=======================================
// �w�i�`�揈��
//=======================================
void DrawBG(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		g_pVtxBuffBG,
		0,
		sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTex = 0; nCntTex < BGLAYER_MAX; nCntTex++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureBG [nCntTex] );

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntTex, 2);
	}
}
