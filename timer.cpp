//=============================================================================
// 
// �^�C�}�[�̏��� [timer.h]
// Author : ENDO HIDETO
// 
//=============================================================================
#include "timer.h"
#include "player.h"

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTexturTimer[TIMER_TEXTURE] = { NULL };		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTimer = NULL;						//���_�o�b�t�@�ւ̃|�C���^
Timer g_timer;

//=======================================
// �^�C�}�[����������
//=======================================
void InitTimer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^
	VERTEX_2D* pVtx;								//���_���ւ̃|�C���^

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\object\\timer.png", &g_pTexturTimer[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Word\\number001.png", &g_pTexturTimer[1]);

	//���_�o�b�t�@�̓ǂݍ���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * (TIMER_PLACE + 1),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTimer,
		NULL);

	//�^�C�}�[�\���̏�����
	g_timer.posCounter = D3DXVECTOR3((SCREEN_WIDTH - TIMER_PLACE * TIMER_WIDTH) * 0.5f, 40, 0);	//�ʒu
	g_timer.fWidth = TIMER_WIDTH;				//��
	g_timer.fHeight = TIMER_HEIGHT;				//����
	g_timer.posStart = TIMERSTART_OBJPOS;		//�X�^�[�g�I�u�W�F�N�g�̈ʒu
	g_timer.fSize = 50;							//�����T�C�Y
	g_timer.bTimerStart = false;				//�X�^�[�g�t���O
	g_timer.bTimerBreak = false;				//�^�C�}�[�ꎞ��~�t���O
	g_timer.nBreak = 0;							//�^�C�}�[�ꎞ��~��K���̍���
	g_timer.nSubTime = 0;						//�^�C�}�[�ꎞ��~���v

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	//�v���J�n�I�u�W�F�N�g�̐ݒ�
	//���_���W��ݒ�
	pVtx[0].pos = g_timer.posStart;
	pVtx[1].pos = D3DXVECTOR3(g_timer.posStart.x + g_timer.fSize, g_timer.posStart.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_timer.posStart.x, g_timer.posStart.y + g_timer.fSize, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_timer.posStart.x + g_timer.fSize, g_timer.posStart.y + g_timer.fSize, 0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�ݒ�
	pVtx[0].col = D3DXCOLOR(0.8f, 0.8f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(0.8f, 0.8f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(0.8f, 0.8f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(0.8f, 0.8f, 1.0f, 1.0f);

	//�e�N�X�`���̍��W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx += 4;

	//�^�C�}�[�̐ݒ�
	for (int nCntPlace = 0; nCntPlace < TIMER_PLACE; nCntPlace++, pVtx += 4)
	{
		//���_���W��ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_timer.posCounter.x + (g_timer.fWidth * nCntPlace), g_timer.posCounter.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_timer.posCounter.x + (g_timer.fWidth * (nCntPlace + 1)), g_timer.posCounter.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_timer.posCounter.x + (g_timer.fWidth * nCntPlace), g_timer.posCounter.y + g_timer.fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_timer.posCounter.x + (g_timer.fWidth * (nCntPlace + 1)), g_timer.posCounter.y + g_timer.fHeight, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�ݒ�
		pVtx[0].col = D3DXCOLOR(0.8f, 0.8f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.8f, 0.8f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.8f, 0.8f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.8f, 0.8f, 1.0f, 1.0f);

		if (nCntPlace == 2)
		{
			//�e�N�X�`���̍��W�ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.5f);
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.5f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
		}
		else if (nCntPlace == 5)
		{
			//�e�N�X�`���̍��W�ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.1f, 0.5f);
			pVtx[1].tex = D3DXVECTOR2(0.2f, 0.5f);
			pVtx[2].tex = D3DXVECTOR2(0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.2f, 1.0f);
		}
		else
		{			//�e�N�X�`���̍��W�ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
		}
	}
	//���_�o�b�t�@�̃��b�N����
	g_pVtxBuffTimer->Unlock();
}

//=======================================
// �^�C�}�[�I������
//=======================================
void UninitTimer(void)
{
	for (int nCntTex = 0; nCntTex < TIMER_TEXTURE; nCntTex++)
	{
		if (g_pTexturTimer[nCntTex] != NULL)
		{
			g_pTexturTimer[nCntTex]->Release();
			g_pTexturTimer[nCntTex] = NULL;
		}
	}

	if (g_pVtxBuffTimer != NULL)
	{
		g_pVtxBuffTimer->Release();
		g_pVtxBuffTimer = NULL;
	}
}

//=======================================
// �^�C�}�[�X�V����
//=======================================
void UpdateTimer(void)
{
	Player* pPlayer = GetPlayer();	//�v���C���[���
	VERTEX_2D* pVtx;				//���_���ւ̃|�C���^
	time_t nowtime;					//���ݎ���
	int nDigit[TIMER_PLACE];		//�ꌅ����

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	if (g_timer.bTimerStart == false)
	{
		if (GetPlayer()->bUse == true)
		{
			if (g_timer.posStart.y < pPlayer->pos.y
				&& pPlayer->pos.y - pPlayer->fHeight < g_timer.posStart.y + g_timer.fSize
				&& g_timer.posStart.x < pPlayer->pos.x + pPlayer->fWidth
				&& pPlayer->pos.x - pPlayer->fWidth < g_timer.posStart.x + g_timer.fSize)
			{//����������^�C�}�[�X�^�[�g
				g_timer.bTimerStart = true;
				g_timer.starttime = timeGetTime();

				//���v�𓧖��ɂ���
				pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
				pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
				pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
				pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			}
		}
	}
	else
	{
		nowtime = timeGetTime();
		if (g_timer.bTimerBreak == false)
		{//��Ɏ�����ۑ��@�~�߂��u�ԍX�V���~�܂�
			g_timer.breaktime = nowtime;
		}

		//�ꎞ��~���Ԃ��v�����A��������
		g_timer.nBreak = (nowtime - g_timer.breaktime);
		g_timer.nTime = (nowtime - g_timer.starttime - g_timer.nBreak - g_timer.nSubTime) * 0.1f;
		if (g_timer.nTime < 600000)
		{
			nDigit[0] = g_timer.nTime / 60000;			//X0:00.00
			nDigit[1] = g_timer.nTime / 6000 % 10;		//0X:00.00
														//00_00.00
			nDigit[3] = g_timer.nTime % 6000 / 1000;	//00:X0.00
			nDigit[4] = g_timer.nTime % 60000 / 100;	//00:0X.00
														//00:00_00
			nDigit[6] = g_timer.nTime % 600000 / 10;	//00:00.X0
			nDigit[7] = g_timer.nTime % 6000000;		//00:00.0X
		}
		else
		{//�\�����E����
			for (int nCntNum = 0; nCntNum < TIMER_PLACE; nCntNum++)
				nDigit[nCntNum] = 9;
		}

		for (int nCntNum = 0; nCntNum < TIMER_PLACE; nCntNum++)
		{
			pVtx += 4;
			if (nCntNum != 2 && nCntNum != 5)
			{
				//�e�N�X�`���̍��W�ݒ�
				pVtx[0].tex = D3DXVECTOR2(0.1f * nDigit[nCntNum], 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.1f * (nDigit[nCntNum] + 1), 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.1f * nDigit[nCntNum], 0.5f);
				pVtx[3].tex = D3DXVECTOR2(0.1f * (nDigit[nCntNum] + 1), 0.5f);
			}
		}
	}

	//���_�o�b�t�@�̃��b�N����
	g_pVtxBuffTimer->Unlock();
}

//=======================================
// �^�C�}�[�`�揈��
//=======================================
void DrawTimer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		g_pVtxBuffTimer,
		0,
		sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_timer.bTimerStart)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexturTimer[1]);

		//�|���S���̕`��
		for (int nCntResult = 0; nCntResult < TIMER_PLACE; nCntResult++)
		{
			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * (nCntResult + 1), 2);
		}
	}
	else
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexturTimer[0]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}

}

//=======================================
//�^�C�}�[�X�g�b�v
//=======================================
void StopTimer(void)
{
	g_timer.bTimerStart = false;
}

//=======================================
//�^�C�}�[�ꎞ��~�E���X�^�[�g
//=======================================
void BreakTimer(bool bTimer) 
{
	g_timer.bTimerBreak = bTimer;
	if (g_timer.bTimerBreak)
	{//�ꎞ��~������
		g_timer.nSubTime += g_timer.nBreak;	//�O��̈ꎞ��~���Ԃ����v�ɉ��Z
		g_timer.nBreak = 0;					//������
	}
}

//=======================================
//�^�C�}�[���̎擾
//=======================================
Timer GetTimer(void)
{
	return g_timer;
}