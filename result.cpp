//=============================================================================
// 
// ���U���g��ʂ̏��� [result.cpp]
// Author : ENDO HIDETO
// 
//=============================================================================
#include "result.h"
#include "fade.h"
#include "game.h"
//#include "sound.h"

//�w�i�\���̂̒�`

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureResult[RESULT_TEXTURE] = { NULL };	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;				//���_�o�b�t�@�ւ̃|�C���^
Result g_Result;												//���U���g���

//�v���g�^�C�v�錾
void ResultAppear(VERTEX_2D* pVtx);								//���U���g���o�Ă��鏈��
void ResultCtrl(VERTEX_2D* pVtx);								//���U���g��ʂł̑���

//=======================================
// ����������
//=======================================
void InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^
	VERTEX_2D* pVtx;								//���_���ւ̃|�C���^

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\BG\\ResultBG.png",&g_pTextureResult[0]);
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\Word\\number001.png",&g_pTextureResult[1]);
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\Word\\pause001.png",&g_pTextureResult[2]);
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\Word\\pause002.png",&g_pTextureResult[3]);

	//���_�o�b�t�@�̓ǂݍ���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)* 4 * RESULT_THING,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResult,
		NULL);

	//�J�E���^�[�N���A
	g_Result.nCounter = 0;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResult->Lock(0, 0,(void**)&pVtx, 0);

	//��ʒu�ۑ�
	g_Result.posBox = D3DXVECTOR3((SCREEN_WIDTH - RESULT_WIDTH) * 0.5f,-RESULT_HEIGHT, 0.0f);
	g_Result.posTime = D3DXVECTOR3((SCREEN_WIDTH - TIMER_PLACE * RESULTTIMER_WIDTH) * 0.5f, g_Result.posBox.y + RESULTTIMER_LINE, 0.0f);
	g_Result.posOperate[RESULTCONTINUE_RETRY] = D3DXVECTOR3(g_Result.posBox.x + (RESULT_WIDTH - OPERATE_WIDTH * 2) * 0.25f, g_Result.posBox.y + RESULTOPERATE_LINE, 0.0f);
	g_Result.posOperate[RESULTCONTINUE_QUIT] = D3DXVECTOR3(((SCREEN_WIDTH - OPERATE_WIDTH + RESULT_WIDTH*0.5f) * 0.5f), g_Result.posBox.y + RESULTOPERATE_LINE, 0.0f);
	g_Result.nChose = 0;
	g_Result.bMove = false;

	for (int nCntResult = 0; nCntResult < RESULT_THING; nCntResult++, pVtx += 4)
	{
		//���_���W��ݒ�
		pVtx[0].pos = g_Result.posBox;
		pVtx[1].pos = g_Result.posBox;
		pVtx[2].pos = g_Result.posBox;
		pVtx[3].pos = g_Result.posBox;

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
	g_pVtxBuffResult->Unlock();

	//���y�Đ�
	//PlaySound(SOUND_LABEL_BGM004);
}

//=======================================
// �I������
//=======================================
void UninitResult(void)
{
	for (int nCntTex = 0; nCntTex < RESULT_TEXTURE; nCntTex++)
	{
		if (g_pTextureResult[nCntTex] != NULL)
		{
			g_pTextureResult[nCntTex]->Release();
			g_pTextureResult[nCntTex] = NULL;
		}
	}

	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}

	//���y��~
	//StopSound();
}

//=======================================
// �X�V����
//=======================================
void UpdateResult(void)
{
	VERTEX_2D* pVtx;		//���_���ւ̃|�C���^

	g_Result.nCounter++;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);


	if (g_Result.nCounter >= COUNTER_RESURT)
	{//��莞�Ԍo�߂�����L�[�������ꂽ��

		//���ɓ�����
		ResultAppear(pVtx);

		if (g_Result.bMove == false)
		{
			//�I��
			ResultCtrl(pVtx);
		}
	}

	//�J�E���g���I���܂Ői�߂�
	if ((GetKeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_A) == true) && g_Result.nCounter < COUNTER_RESURT)
	{
		g_Result.nCounter += COUNTER_RESURT;
	}
	
	//���_�o�b�t�@�̃��b�N����
	g_pVtxBuffResult->Unlock();
}

//=======================================
// ���U���g�̏o��
void ResultAppear(VERTEX_2D* pVtx)
{
	//���_���W��ݒ�
	pVtx[0].pos = g_Result.posBox;//�g
	pVtx[1].pos = D3DXVECTOR3(g_Result.posBox.x + RESULT_WIDTH, g_Result.posBox.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Result.posBox.x, g_Result.posBox.y + RESULT_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Result.posBox.x + RESULT_WIDTH, g_Result.posBox.y + RESULT_HEIGHT, 0.0f);
	pVtx += 4;

	for (int nCntNum = 0; nCntNum < TIMER_PLACE; nCntNum++, pVtx += 4)
	{//�^�C��
		pVtx[0].pos = D3DXVECTOR3(g_Result.posTime.x + (RESULTTIMER_WIDTH * nCntNum), g_Result.posTime.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Result.posTime.x + (RESULTTIMER_WIDTH * (nCntNum + 1)), g_Result.posTime.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Result.posTime.x + (RESULTTIMER_WIDTH * nCntNum), g_Result.posTime.y + RESULTTIMER_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Result.posTime.x + (RESULTTIMER_WIDTH * (nCntNum + 1)), g_Result.posTime.y + RESULTTIMER_HEIGHT, 0.0f);
	}

	//������
	pVtx[0].pos = D3DXVECTOR3(g_Result.posOperate[RESULTCONTINUE_RETRY].x, g_Result.posOperate[RESULTCONTINUE_RETRY].y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Result.posOperate[RESULTCONTINUE_RETRY].x + OPERATE_WIDTH, g_Result.posOperate[RESULTCONTINUE_RETRY].y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Result.posOperate[RESULTCONTINUE_RETRY].x, g_Result.posOperate[RESULTCONTINUE_RETRY].y + OPERATE_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Result.posOperate[RESULTCONTINUE_RETRY].x + OPERATE_WIDTH, g_Result.posOperate[RESULTCONTINUE_RETRY].y + OPERATE_HEIGHT, 0.0f);
	pVtx += 4;
	//��߂�
	pVtx[0].pos = D3DXVECTOR3(g_Result.posOperate[RESULTCONTINUE_QUIT].x, g_Result.posOperate[RESULTCONTINUE_QUIT].y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Result.posOperate[RESULTCONTINUE_QUIT].x + OPERATE_WIDTH, g_Result.posOperate[RESULTCONTINUE_QUIT].y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Result.posOperate[RESULTCONTINUE_QUIT].x, g_Result.posOperate[RESULTCONTINUE_QUIT].y + OPERATE_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Result.posOperate[RESULTCONTINUE_QUIT].x + OPERATE_WIDTH, g_Result.posOperate[RESULTCONTINUE_QUIT].y + OPERATE_HEIGHT, 0.0f);

	//���ɓ�����
	if (g_Result.posBox.y < (SCREEN_HEIGHT - RESULT_HEIGHT) * 0.5f)
	{
		g_Result.posBox.y += RESULT_MOVE;
		g_Result.posTime.y += RESULT_MOVE;
		g_Result.posOperate[RESULTCONTINUE_RETRY].y += RESULT_MOVE;
		g_Result.posOperate[RESULTCONTINUE_QUIT].y += RESULT_MOVE;
	}
	else
	{//��苗���Ŏ~�߂�
		g_Result.posBox.y = (SCREEN_HEIGHT - RESULT_HEIGHT) * 0.5f;
		g_Result.posTime.y = g_Result.posBox.y + RESULTTIMER_LINE;
		g_Result.posOperate[RESULTCONTINUE_RETRY].y = g_Result.posBox.y + RESULTOPERATE_LINE;
		g_Result.posOperate[RESULTCONTINUE_QUIT].y = g_Result.posBox.y + RESULTOPERATE_LINE;
		g_Result.bMove = false;

	}

}

//=======================================
// ���U���g�̑���
void ResultCtrl(VERTEX_2D* pVtx)
{
	pVtx += 4 * (TIMER_PLACE + 1);

	//���j���[�ʒu�ݒ�
	for (int nCntResult = 0; nCntResult < RESULTCONTINUE_MAX; nCntResult++, pVtx += 4)
	{

		if (g_Result.nChose % RESULTCONTINUE_MAX == nCntResult)
		{//�I�𒆂̃��j���[�̋���
			//���_���W
			pVtx[0].pos = D3DXVECTOR3(g_Result.posOperate[nCntResult].x - RESULT_CHOICE, g_Result.posOperate[nCntResult].y - RESULT_CHOICE, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_Result.posOperate[nCntResult].x + OPERATE_WIDTH + RESULT_CHOICE, g_Result.posOperate[nCntResult].y - RESULT_CHOICE, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_Result.posOperate[nCntResult].x - RESULT_CHOICE, g_Result.posOperate[nCntResult].y + OPERATE_HEIGHT + RESULT_CHOICE, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_Result.posOperate[nCntResult].x + OPERATE_WIDTH + RESULT_CHOICE, g_Result.posOperate[nCntResult].y + OPERATE_HEIGHT + RESULT_CHOICE, 0.0f);

			//���_�J���[�ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		}
		else
		{
			//���_���W
			pVtx[0].pos = g_Result.posOperate[nCntResult];
			pVtx[1].pos = D3DXVECTOR3(g_Result.posOperate[nCntResult].x + OPERATE_WIDTH, g_Result.posOperate[nCntResult].y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_Result.posOperate[nCntResult].x, g_Result.posOperate[nCntResult].y + OPERATE_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_Result.posOperate[nCntResult].x + OPERATE_WIDTH, g_Result.posOperate[nCntResult].y + OPERATE_HEIGHT, 0.0f);

			//���_�J���[�ݒ�
			pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[2].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		}
	}

	//���j���[����
	if (GetKeyboardTrigger(DIK_A) == true || GetJoypadTrigger(JOYKEY_RIGHT) == true)
	{
		g_Result.nChose += (int)RESULTCONTINUE_MAX - 1;
	}
	if (GetKeyboardTrigger(DIK_D) == true || GetJoypadTrigger(JOYKEY_LEFT) == true)
	{
		g_Result.nChose++;
	}

	//���j���[��������s
	if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_SPACE) == true|| GetJoypadTrigger(JOYKEY_A) == true)
	{
		switch (g_Result.nChose % RESULTCONTINUE_MAX)
		{

		case RESULTCONTINUE_RETRY:
			g_Result.nChose = 0;
			SetFade(MODE_GAME);
			break;

		case RESULTCONTINUE_QUIT:
			g_Result.nChose = 0;
			SetFade(MODE_TITLE);
			break;

		default:
			break;
		}
	}

}

//=======================================
// �`�揈��
//=======================================
void DrawResult(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		g_pVtxBuffResult,
		0,
		sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�w�i�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureResult[0]);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//�����e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureResult[1]);

	for (int nCntResult = 0; nCntResult < TIMER_PLACE; nCntResult++)
	{
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * (nCntResult + 1), 2);
	}

	//CONTINUE�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureResult[2]);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * (TIMER_PLACE + 1), 2);

	//QUIT�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureResult[3]);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * (TIMER_PLACE + 2), 2);

}

//=======================================
// ���U���g���̐ݒ�
//=======================================
void SetResult(void)
{
	VERTEX_2D* pVtx;		//���_���ւ̃|�C���^
	int nDigit[TIMER_PLACE];

	//�N���A�^�C���̎擾	
	g_Result.nTime = GetTimer().nTime;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);
	pVtx += 4;
	//�ꌅ���i�[
	if (g_Result.nTime < 600000)
	{
		nDigit[0] = g_Result.nTime / 60000;			//X0:00.00
		nDigit[1] = g_Result.nTime / 6000 % 10;		//0X:00.00
													//00_00.00
		nDigit[3] = g_Result.nTime % 6000 / 1000;		//00:X0.00
		nDigit[4] = g_Result.nTime % 60000 / 100;		//00:0X.00
													//00:00_00
		nDigit[6] = g_Result.nTime % 600000 / 10;		//00:00.X0
		nDigit[7] = g_Result.nTime % 6000000;			//00:00.0X
	}
	else
	{//�\�����E����
		for (int nCntNum = 0; nCntNum < TIMER_PLACE; nCntNum++)
			nDigit[nCntNum] = 9;
	}

	//�^�C���\��
	for (int nCntNum = 0; nCntNum < TIMER_PLACE; nCntNum++, pVtx += 4)
	{
		if (nCntNum == 2)
		{//���F�b
			//�e�N�X�`���̍��W�ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.5f);
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.5f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
		}
		else if (nCntNum == 5)
		{//����.����
			//�e�N�X�`���̍��W�ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.1f, 0.5f);
			pVtx[1].tex = D3DXVECTOR2(0.2f, 0.5f);
			pVtx[2].tex = D3DXVECTOR2(0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.2f, 1.0f);
		}
		else
		{//�e�N�X�`���̍��W�ݒ�
			//�e�N�X�`���̍��W�ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.1f * nDigit[nCntNum], 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f * (nDigit[nCntNum] + 1), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.1f * nDigit[nCntNum], 0.5f);
			pVtx[3].tex = D3DXVECTOR2(0.1f * (nDigit[nCntNum] + 1), 0.5f);
		}
	}

	//�������Ă悵
	g_Result.bMove = true;

	//���_�o�b�t�@�̃��b�N����
	g_pVtxBuffResult->Unlock();

}