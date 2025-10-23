//=============================================================================
// 
// �Q�[����� [game.h]
// Author : ENDO HIDETO
// 
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"

typedef enum
{
	GAME_NONE = 0,		//�Q�[�������Ă��Ȃ�
	GAME_START,			//�N��
	GAME_RUNNING,		//�i�s��
	GAME_RESPAWN,		//���X�|�[��
	GAME_DEAD,			//��
	GAME_CLEAR,			//�N���A
	GAME_MAX
}GAMESTATE;

//�v���g�^�C�v�錾
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);

GAMESTATE GetGameState(void);			//�Q�[���̏�Ԃ��擾
void SetGameState(GAMESTATE state);		//�Q�[���̏�Ԃ�ݒ�
#endif // !_GAME_H_
