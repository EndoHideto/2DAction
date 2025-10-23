//=============================================================================
// 
// �|�[�Y���� [pause.cpp]
// Author : ENDO HIDETO
// 
//=============================================================================
#include "fade.h"
#include "input.h"
#include "pause.h"

//�w�i�\���̂̒�`

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTexturePause[PAUSE_TEXTURE] = { NULL };		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;						//���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_PauseMenupos;											//���j���[��\������ʒu�̊�_
int g_nChose;														//�I������Ă��郁�j���[
bool g_bPause = false;												//�|�[�Y��Ԃ̐؂�ւ�

//=======================================
// ����������
//=======================================
void InitPause(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^
	VERTEX_2D* pVtx;								//���_���ւ̃|�C���^

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\BG\\pause101.png",
		&g_pTexturePause[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Word\\pause000.png",
		&g_pTexturePause[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Word\\pause001.png",
		&g_pTexturePause[2]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Word\\pause002.png",
		&g_pTexturePause[3]);

	//���_�o�b�t�@�̓ǂݍ���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * (PAUSE_TEXTURE + 1),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause,
		NULL);

	//�|�[�Y���j���[�I��������
	g_nChose = 0;

	//�|�[�Y���j���[��ʒu
	g_PauseMenupos = D3DXVECTOR3((SCREEN_WIDTH - PAUSE_WIDTH) * 0.5f, (SCREEN_HEIGHT - (PAUSE_LINE * PAUSE_MAX)) * 0.5f, 0.0f);

	//�|�[�Y����
	g_bPause = false;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	//UI�w�i ���_���W��ݒ�
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
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f);

	//�e�N�X�`���̍��W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.625f, 1.0f);
	pVtx += 4;

	//--------------
	//�|�[�Y���̑I����
	for (int nCntPause = 0; nCntPause < PAUSE_MAX; nCntPause++,pVtx += 4)
	{
		//�e�e�L�X�g���_���W
		pVtx[0].pos = g_PauseMenupos;
		pVtx[1].pos = D3DXVECTOR3(g_PauseMenupos.x + PAUSE_WIDTH, g_PauseMenupos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_PauseMenupos.x, g_PauseMenupos.y + PAUSE_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_PauseMenupos.x + PAUSE_WIDTH, g_PauseMenupos.y + PAUSE_HEIGHT, 0.0f);

		//���̃��j���[��ʒu
		g_PauseMenupos.y += PAUSE_LINE;

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�ݒ�
		pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);

		//�e�N�X�`���̍��W�ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	
	//���_�o�b�t�@�̃��b�N����
	g_pVtxBuffPause->Unlock();
}

//=======================================
// �I������
//=======================================
void UninitPause(void)
{
	for (int nCntPause = 0; nCntPause < PAUSE_TEXTURE; nCntPause++)
	{
		if (g_pTexturePause[nCntPause] != NULL)
		{
			g_pTexturePause[nCntPause]->Release();
			g_pTexturePause[nCntPause] = NULL;
		}
	}

	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}
}

//=======================================
// �X�V����
//=======================================
void UpdatePause(void)
{
	if (GetFade() == FADE_NONE)
	{
		if (GetKeyboardTrigger(DIK_P) == true || GetJoypadTrigger(JOYKEY_START) == true)
		{
			g_bPause = g_bPause ^ 1;
			g_nChose = 0;
		}
	}

	if (g_bPause == true)
	{
		VERTEX_2D* pVtx;	//���_���ւ̃|�C���^


		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

		//�|�[�Y���j���[��ʒu
		g_PauseMenupos = D3DXVECTOR3((SCREEN_WIDTH - PAUSE_WIDTH) * 0.5f, (SCREEN_HEIGHT - (PAUSE_LINE * PAUSE_MAX)) * 0.5f, 0.0f);

		//���j���[�ʒu�ݒ�
		for (int nCntPause = 0; nCntPause < PAUSE_MAX; nCntPause++)
		{
			pVtx += 4;

			if (g_nChose % PAUSE_MAX == nCntPause)
			{//�I�𒆂̃��j���[�̋���
				//���_���W
				pVtx[0].pos = D3DXVECTOR3(g_PauseMenupos.x - PAUSE_CHOICE				, g_PauseMenupos.y - PAUSE_CHOICE, 0.0f);;
				pVtx[1].pos = D3DXVECTOR3(g_PauseMenupos.x + PAUSE_WIDTH + PAUSE_CHOICE	, g_PauseMenupos.y - PAUSE_CHOICE, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_PauseMenupos.x - PAUSE_CHOICE				, g_PauseMenupos.y + PAUSE_HEIGHT + PAUSE_CHOICE, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_PauseMenupos.x + PAUSE_WIDTH + PAUSE_CHOICE	, g_PauseMenupos.y + PAUSE_HEIGHT + PAUSE_CHOICE, 0.0f);

				//���_�J���[�ݒ�
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			}
			else
			{
				//���_���W
				pVtx[0].pos = g_PauseMenupos;
				pVtx[1].pos = D3DXVECTOR3(g_PauseMenupos.x + 250, g_PauseMenupos.y, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_PauseMenupos.x, g_PauseMenupos.y + 50, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_PauseMenupos.x + 250, g_PauseMenupos.y + 50, 0.0f);

				//���_�J���[�ݒ�
				pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
				pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			}

			//���̃��j���[��ʒu
			g_PauseMenupos.y += 60.0f;
		}

		//���j���[����
		if (GetKeyboardTrigger(DIK_W) == true || GetJoypadTrigger(JOYKEY_UP) == true)
		{
			g_nChose += (int)PAUSE_MAX - 1;
		}
		if (GetKeyboardTrigger(DIK_S) == true || GetJoypadTrigger(JOYKEY_DOWN) == true)
		{
			g_nChose++;
		}

		//���j���[��������s
		if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_A) == true)
		{
			switch (g_nChose % PAUSE_MAX)
			{
			case PAUSE_CONTINUE:
				g_nChose = 0;			//�|�[�Y���j���[�I��������
				g_bPause = false;
				break;

			case PAUSE_RETRY:
				g_bPause = false;
				g_nChose = 0;
				SetFade(MODE_GAME);
				break;

			case PAUSE_QUIT:
				g_bPause = false;
				g_nChose = 0;
				SetFade(MODE_TITLE);
				break;

			default:
				break;
			}
		}
		//���_�o�b�t�@�̃��b�N����
		g_pVtxBuffPause->Unlock();
	}
}

//=======================================
// �`�揈��
//=======================================
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		g_pVtxBuffPause,
		0,
		sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntPause = 0; nCntPause < PAUSE_MAX + 1; nCntPause++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexturePause[nCntPause]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,nCntPause * 4, 2);
	}
}

//=======================================
// �|�[�Y�����ǂ����擾
//=======================================
bool GetPause(void)
{
	return g_bPause;
}