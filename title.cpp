//=============================================================================
// 
// �^�C�g�� [title.cpp]
// Author : ENDO HIDETO
// 
//=============================================================================
#include "input.h"
#include "title.h"
#include "fade.h"
//#include "sound.h"

//�\���̂̒�`

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureTitle[TITLE_TEXTURE] = {};		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;				//���_�o�b�t�@�ւ̃|�C���^
int g_nCouonterTitle;										//�^�C�g����ʂ̃J�E���^�[
TitileObject g_titleObj[TITLE_THING];						//�^�C�g����ʂɕ\�����镨

D3DXVECTOR3 g_posTitle;										//�^�C�g���̈ʒu
D3DXVECTOR3 g_posLight[2];									//�Ɩ��̈ʒu
D3DXCOLOR g_pressAlpha;										//Press Enter�̐F
float g_fLenght;											//�Ίp���̒���
float g_fAngle;												//�Ίp���̊p�x
float g_fRot;												//�Ίp���̌���

//�v���g�^�C�v�錾
void TitleSet(void);										//������

//=======================================
// �w�i�̏���������
//=======================================
void InitTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^
	VERTEX_2D* pVtx;								//���_���ւ̃|�C���^

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\BG\\town.png",&g_pTextureTitle[0]);				//�w�i
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\Word\\Title_Jump Cat.png",&g_pTextureTitle[1]);	//�^�C�g��
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\Word\\end.png",&g_pTextureTitle[2]);				//�n�߂�
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\Word\\end.png",&g_pTextureTitle[3]);				//�I��
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\Object\\light.png",&g_pTextureTitle[4]);			//����


	//���_�o�b�t�@�̓ǂݍ���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * TITLE_THING,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitle,
		NULL);

	//�^�C�g���̈ʒu
	g_posTitle = D3DXVECTOR3 TITLE_POS;
	//���C�g�̑Ίp��
	g_fLenght =(float)sqrt(200*200 + 500*500);
	g_fAngle  = atan2f(200,500);
	g_fRot = 0;
	g_posLight[0] = D3DXVECTOR3(65.0f, 720.0f, 0.0f);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitle->Lock(0, 0,(void**)&pVtx, 0);

	for (int nCntTitle = 0; nCntTitle < TITLE_THING; nCntTitle++, pVtx += 4)
	{
		//�w�i���_���W��ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//�e�N�X�`���̍��W�ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//���_�J���[�ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
	}
	//���_�o�b�t�@�̃��b�N����
	g_pVtxBuffTitle->Unlock();

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);
	//�w�i���_���W��ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//�^�C�g��
	pVtx[4].pos = D3DXVECTOR3(g_posTitle.x - TITLE_WIDTH,g_posTitle.y - TITLE_HEIGHT, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(g_posTitle.x + TITLE_WIDTH,g_posTitle.y - TITLE_HEIGHT, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(g_posTitle.x - TITLE_WIDTH,g_posTitle.y,0.0f);
	pVtx[7].pos = D3DXVECTOR3(g_posTitle.x + TITLE_WIDTH,g_posTitle.y,0.0f);

	//�Ɩ�
	pVtx[ 8].pos = D3DXVECTOR3(g_posLight[0].x - sin(g_fAngle) * g_fLenght, g_posLight[0].y - cos(g_fAngle) * g_fLenght, 0.0f);
	pVtx[ 9].pos = D3DXVECTOR3(g_posLight[0].x + sin(g_fAngle) * g_fLenght, g_posLight[0].y - cos(g_fAngle) * g_fLenght, 0.0f);
	pVtx[10].pos = D3DXVECTOR3(g_posLight[0].x - sin(g_fAngle) * g_fLenght, g_posLight[0].y, 0.0f);
	pVtx[11].pos = D3DXVECTOR3(g_posLight[0].x + sin(g_fAngle) * g_fLenght, g_posLight[0].y, 0.0f);

	//���_�o�b�t�@�̃��b�N����
	g_pVtxBuffTitle->Unlock();

	//���y�Đ�
	//PlaySound(SOUND_LABEL_BGM003);

	//�J�E���^�[������
	g_nCouonterTitle = 0;
}

//=======================================
// �w�i�̏I������
//=======================================
void UninitTitle(void)
{
	for (int nCntTex = 0; nCntTex < TITLE_TEXTURE; nCntTex++)
	{
		if (g_pTextureTitle[nCntTex] != NULL)
		{
			g_pTextureTitle[nCntTex]->Release();
			g_pTextureTitle[nCntTex] = NULL;
		}
	}

	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}

	//���y��~
	//StopSound();
}

//=======================================
// �w�i�X�V����
//=======================================
void UpdateTitle(void)
{
	VERTEX_2D* pVtx;		//���_���ւ̃|�C���^
	
	
	if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_A) == true)
	{//����L�[�������ꂽ��
		//���[�h�J��
		SetFade(MODE_GAME);
	}

	if (GetKeyboardPress(DIK_3) == true)
	{
		g_fRot -=	0.01f;
	}
	if (GetKeyboardPress(DIK_4) == true)
	{
		g_fRot +=	0.01f;
	}

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	//�Ɩ�
	pVtx[ 8].pos = D3DXVECTOR3(g_posLight[0].x - sin(g_fAngle - g_fRot) * g_fLenght, g_posLight[0].y - cos(g_fAngle - g_fRot) * g_fLenght, 0.0f);
	pVtx[ 9].pos = D3DXVECTOR3(g_posLight[0].x + sin(g_fAngle + g_fRot) * g_fLenght, g_posLight[0].y - cos(g_fAngle + g_fRot) * g_fLenght, 0.0f);
	pVtx[10].pos = D3DXVECTOR3(g_posLight[0].x - sin(D3DX_PI * 0.5 + g_fRot) * 200, g_posLight[0].y + cos(D3DX_PI * 0.5 + g_fRot) * 200, 0.0f);
	pVtx[11].pos = D3DXVECTOR3(g_posLight[0].x + sin(D3DX_PI * 0.5 - g_fRot) * 200, g_posLight[0].y + cos(D3DX_PI * 0.5 - g_fRot) * 200, 0.0f);

	//���_�o�b�t�@�̃��b�N����
	g_pVtxBuffTitle->Unlock();
}

//=======================================
// �w�i�`�揈��
//=======================================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		g_pVtxBuffTitle,
		0,
		sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureTitle[0]);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureTitle[1]);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, 2);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureTitle[2]);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 8, 2);

}

//=======================================
// �������i���₷���̂��ߊ֐�����
void TitleSet(void)
{
	g_titleObj[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�w�i
	g_titleObj[1].pos = D3DXVECTOR3 TITLE_POS;				//�Q�[���^�C�g��
	g_titleObj[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�n�߂�
	g_titleObj[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�����
	g_titleObj[4].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//���胉�C�g�P
	g_titleObj[5].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//���胉�C�g�Q

}