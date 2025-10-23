//=============================================================================
// 
// �t�F�[�h�̏��� [fade.cpp]
// Author : ENDO HIDETO
// 
//=============================================================================
#include "fade.h"
//#include "pause.h"

//�w�i�\���̂̒�`
typedef struct
{
	FADE mode;
	float fAlpha;
	MODE nextmode;
}Fade;

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureFade = NULL;			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFade = NULL;		//���_�o�b�t�@�ւ̃|�C���^
Fade g_aFade;										//�t�F�[�h���

//=======================================
// �t�F�[�h�̏���������
//=======================================
void InitFade(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^
	VERTEX_2D* pVtx;								//���_���ւ̃|�C���^

	//�e�N�X�`���ǂݍ���

	//���_�o�b�t�@�̓ǂݍ���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*FADE_VER_NUM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffFade,
		NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffFade->Lock(0, 0,(void**)&pVtx, 0);

	//�t�F�[�h���g�p�E�����ɐݒ�
	g_aFade.mode = FADE_NONE;
	g_aFade.fAlpha = 0.0f;

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
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_aFade.fAlpha);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_aFade.fAlpha);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_aFade.fAlpha);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_aFade.fAlpha);

	//�e�N�X�`���̍��W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
  	
	//���_�o�b�t�@�̃��b�N����
	g_pVtxBuffFade->Unlock();
}

//=======================================
// �t�F�[�h�̂̏I������
//=======================================
void UninitFade(void)
{
	if (g_pTextureFade != NULL)
	{
		g_pTextureFade->Release();
		g_pTextureFade = NULL;
	}

	if (g_pVtxBuffFade != NULL)
	{
		g_pVtxBuffFade->Release();
		g_pVtxBuffFade = NULL;
	}
}

//=======================================
// �t�F�[�h�̍X�V����
//=======================================
void UpdateFade(void)
{
	MODE mode = GetMode();				//���݂̃��[�h�擾
	VERTEX_2D* pVtx;					//���_���ւ̃|�C���^

	if (g_aFade.mode!= FADE_NONE)
	{
		if (g_aFade.mode == FADE_IN)
		{//�t�F�[�h�C���@�����ɂ���
			g_aFade.fAlpha -= 0.05f;

			if (g_aFade.fAlpha <= 0.0f)
			{//�����x���O�ȉ��ɂȂ�����
				g_aFade.fAlpha = 0.0f;
				g_aFade.mode = FADE_NONE;
			}
		}
		else if (g_aFade.mode == FADE_OUT)
		{//�t�F�[�h�A�E�g�@�Â�����
			g_aFade.fAlpha += 0.05f;

			if (g_aFade.fAlpha >= 1.0f)
			{//�����x���P�ȏ�ɂȂ�����
				g_aFade.fAlpha = 1.0f;
				g_aFade.mode = FADE_IN;

				//�����[�h�Z�b�g
				SetMode(g_aFade.nextmode);
			}
		}

		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

		//���_�J���[�ݒ�
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_aFade.fAlpha);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_aFade.fAlpha);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_aFade.fAlpha);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_aFade.fAlpha);

		//���_�o�b�t�@�̃��b�N����

		g_pVtxBuffFade->Unlock();
	}
}

//=======================================
// �t�F�[�h�̕`�揈��
//=======================================
void DrawFade(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		g_pVtxBuffFade,
		0,
		sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);

}

//=======================================
// �t�F�[�h�ݒ菈��
//=======================================
void SetFade(MODE nextmode)
{
	//�t�F�[�h�A�E�g�J�n
	g_aFade.mode = FADE_OUT;

	//���̃��[�h��ۑ�
	g_aFade.nextmode = nextmode;

	//�t�F�[�h�C��
	if (g_aFade.fAlpha >= 1.0f)
	{//�����x���P�ȏ�ɂȂ�����
		g_aFade.mode = FADE_IN;
	}
}

//=======================================
// �t�F�[�h��Ԏ擾
//=======================================
FADE GetFade(void)
{
	return g_aFade.mode;
}
