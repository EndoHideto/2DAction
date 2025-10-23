//=============================================================================
// 
// �Q�[����� [game.cpp]
// Author : ENDO HIDETO
// 
//=============================================================================
#include "bg.h"
#include "block.h"
//#include "effect.h"
//#include "explosion.h"
#include "fade.h"
#include "game.h"
#include "goal.h"
#include "input.h"
//#include "particle.h"
#include "pause.h"
#include "player.h"
#include "result.h"
#include "retry.h"
//#include "score.h"
//#include "sound.h"
//#include "stage.h"
#include "timer.h"

//�O���[�o���ϐ��錾
bool g_bDrawPause = true;
GAMESTATE g_gameState = GAME_NONE;
bool g_bTrans = true;				//��ʑJ��
int g_nSelectStage;

//�v���g�^�C�v�錾
void GameRunning(void);					//�Q�[���i�s���̏���

//=======================================
// �Q�[����ʏ���������
//=======================================
void InitGame(void)
{
	//�|�[�Y������
	InitPause();

	//�w�i�̏�����
	InitBG();

	//�v���C���[������
	InitPlayer();

	//�u���b�N������
	InitBlock();

	//�S�[��������
	InitGoal();

	//���g���C�����̏�����
	InitRetry();

	//�^�C�}�[������
	InitTimer();

	//���U���g������
	InitResult();

#if 0
	//�X�e�[�W������
	InitStage();

	//�G�t�F�N�g�̏�����
	InitEffect();

	//�p�[�e�B�N���̏�����
	InitParticle();
	//�T�E���h�̍Đ�
	PlaySound(SOUND_LABEL_BGM000);
#endif
#if _DEBUG
	g_bTrans = false;
#endif
	g_gameState = GAME_START;
	g_bDrawPause = true;		//F1�Ń|�[�Y���j���[�������i�f�o�b�O�p
}

//=======================================
// �Q�[����ʏI������
//=======================================
void UninitGame(void)
{
	//�T�E���h��~
	//StopSound();

	//�|�[�Y�I��
	UninitPause();

	//�v���C���[�I��
	UninitPlayer();		

	//�u���b�N�I��
	UninitBlock();

	//�S�[���I��
	UninitGoal();

	//���g���C�����I��
	UninitRetry();

	//�^�C�}�[�I��
	UninitTimer();
#if 0
	//�X�e�[�W�I��
	UninitStage();

	//�G�t�F�N�g�I��
	UninitEffect();

	//�p�[�e�B�N���I��
	UninitParticle();

#endif
	//�w�i�I��
	UninitBG();

	//���U���g�I��
	UninitResult();
}

//=======================================
// �Q�[����ʍX�V����
//=======================================
void UpdateGame(void)
{
	switch (g_gameState)
	{
	case GAME_START:	//�N��
		g_gameState = GAME_RUNNING;
		break;

	case GAME_RUNNING:	//�i�s��
		GameRunning();
		break;

	case GAME_RESPAWN:	//���X�|�[��
		g_gameState = GAME_RUNNING;
		break;

	case GAME_CLEAR:	//�N���A
		UpdateResult();
		break;

	default:
		break;
	}
}

//=======================================
// �Q�[���i�s��
void GameRunning(void)
{
		
	//�N���A����
	if (GameFinish() == true)
	{//goal.cpp�ŏI������
		g_gameState = GAME_CLEAR;
		SetResult();
	}

	UpdatePause();			//�|�[�Y��ʍX�V

	if (GetPause() == false)
	{//�Q�[�����s��
		UpdatePlayer();			//�v���C���[�X�V
		UpdateRetry();			//���g���C�X�V
		UpdateBlock();			//�u���b�N�X�V
		UpdateGoal();			//�S�[���X�V
		UpdateTimer();			//�^�C�}�[�X�V
		UpdateBG();				//�w�i�X�V

		//UpdateStage();			//�X�e�[�W�X�V
		//UpdateParticle();		//�p�[�e�B�N���X�V
		//UpdateEffect();			//�G�t�F�N�g�X�V
	}

	//�|�[�Y�؂�ւ�
	if (GetKeyboardTrigger(DIK_F1) == true)
		g_bDrawPause = g_bDrawPause ^ 1;

#if _DEBUG
	//�f�o�b�O�p�R�}���h
	if (GetKeyboardTrigger(DIK_F2) == true)
	{//�N���A
		g_gameState = GAME_CLEAR;
		SetResult();
	}
#endif
}

//=======================================
// �Q�[����ʕ`�揈��
//=======================================
void DrawGame(void)
{
	DrawBG();			//�w�i�`��
	DrawPlayer();		//�v���C���[�`��
	DrawBlock();		//�u���b�N�`��
	DrawGoal();			//�S�[���`��
	DrawTimer();		//�^�C�}�[�`��
	DrawRetry();		//���g���C�`��
	DrawResult();		//���U���g�`��
#if 0
	DrawStage();		//�X�e�[�W�`��
	DrawParticle();		//�p�[�e�B�N���`��
	DrawEffect();		//�G�t�F�N�g�`��
#endif

	if (GetPause() == true && g_bDrawPause)
	{//�|�[�Y��ʂ̕`��
		DrawPause();
	}
}

//=======================================
//  �Q�[����Ԏ擾
//=======================================
GAMESTATE GetGameState(void)
{
	return g_gameState;
}

//=======================================
// �Q�[����Ԑݒ�
//=======================================
void SetGameState(GAMESTATE state)
{
	g_gameState = state;
}
