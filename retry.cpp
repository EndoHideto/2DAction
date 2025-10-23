//=============================================================================
// 
// �������̏��� [retry.cpp]
// Author : ENDO HIDETO
// 
//=============================================================================
#include "input.h"
#include "retry.h"
#include "player.h"
#include "timer.h"

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureRetry = NULL;				//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRetry = NULL;			//���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posRetry;									//�\���ʒu

//=======================================
// �w�i�̏���������
//=======================================
void InitRetry(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//�f�o�C�X�ւ̃|�C���^
	VERTEX_2D* pVtx;									//���_���ւ̃|�C���^

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\BG\\pause001.png",&g_pTextureRetry);


	//���_�o�b�t�@�̓ǂݍ���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRetry,
		NULL);

	//�ʒu���i�[
	g_posRetry = D3DXVECTOR3(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 0.0f);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRetry->Lock(0, 0,(void**)&pVtx, 0);

	//���_���W��ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_posRetry.x - RETRY_WIDTH, g_posRetry.y - RETRY_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posRetry.x + RETRY_WIDTH, g_posRetry.y - RETRY_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posRetry.x - RETRY_WIDTH, g_posRetry.y + RETRY_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posRetry.x + RETRY_WIDTH, g_posRetry.y + RETRY_HEIGHT, 0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

	//�e�N�X�`���̍��W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@�̃��b�N����
	g_pVtxBuffRetry->Unlock();
}

//=======================================
// �w�i�̏I������
//=======================================
void UninitRetry(void)
{
	if (g_pTextureRetry != NULL)
	{
		g_pTextureRetry->Release();
		g_pTextureRetry = NULL;
	}

	if (g_pVtxBuffRetry != NULL)
	{
		g_pVtxBuffRetry->Release();
		g_pVtxBuffRetry = NULL;
	}
}

//=======================================
// �w�i�X�V����
//=======================================
void UpdateRetry(void)
{
	VERTEX_2D* pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRetry->Lock(0, 0, (void**)&pVtx, 0);

	if (GetPlayer()->state == PLAYERSTATE_DEATH)
	{//���񂾂烊�g���C�ƕ\��
		//���_�J���[�ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		if (GetKeyboardTrigger(DIK_SPACE) == true || GetKeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_A) == true)
		{//�X�y�[�X�E�G���^�[�EA�{�^���ŕ���
			GetPlayer()->state = PLAYERSTATE_NORMAL;	//��Ԃ𕜊�
			RestartTimer();								//�^�C�}�[���ĊJ
		}
	}
	else
	{
		//���_�J���[�ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	}

	//���_�o�b�t�@�̃��b�N����
	g_pVtxBuffRetry->Unlock();

}

//=======================================
// �w�i�`�揈��
//=======================================
void DrawRetry(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		g_pVtxBuffRetry,
		0,
		sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureRetry);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
