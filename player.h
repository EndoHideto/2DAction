//=============================================================================
// 
// �v���C���[�̏��� [player.h]
// Author : ENDO HIDETO
// 
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_				//2�d�C���N���[�h�h�~

#include "main.h"
#include "block.h"

//�}�N����`
#define PLAYER_WIDTH		(30.0f)								//�v���C���[�̕�
#define PLAYER_HEIGHT		(60.0f)								//�v���C���[�̍���
#define PLAYER_COL			D3DXCOLOR(1.0f,1.0f,1.0f,1.0f)		//�����̐F
#define PLAYER_START_POS	D3DXVECTOR3(50.0f,600.0f,0.0f)		//�o���ʒu
#define MOVE_ZERO			D3DXVECTOR3(0.0f,0.0f,0.0f)			//move�l���[���ɂ������Ƃ��g��
#define ACCEL				(0.5f)								//�ړ����x
#define MAX_SPEED			(4.0f)								//�ő呬�x
#define DIRECTION_LEFT		(0.0f)								//������
#define DIRECTION_RIGHT		(0.5f)								//�E����
#define JUMP_FORCE			(9.0f)								//�W�����v�͂�
#define DASH_FORCE			(20.0f)								//�_�b�V���̑��x(������)
#define GRAVITY				(0.35f)								//�d��
#define REVIVALTIME			(20)								//������������󂯕t���Ȃ�����

//�v���C���[��ԗ�
typedef enum
{
	PLAYERSTATE_NORMAL = 0,		//�ҋ@
	PLAYERSTATE_WALK,			//����
	PLAYERSTATE_JUMP,			//�W�����v
	PLAYERSTATE_AIRJUMP,		//��i�W�����v
	PLAYERSTATE_WALLSIDE,		//�ǂ���
	PLAYERSTATE_FALL,			//����
	PLAYERSTATE_DEATH,			//���S
	PLAYERSTATE_MAX
}PLAYERSTATE;

//�v���C���[�\���̂̒�`
typedef struct
{
	PLAYERSTATE state;			//�W�����v���
	D3DXCOLOR col;				//�F
	D3DXVECTOR3 pos;			//���݈ʒu
	D3DXVECTOR3 posOld;			//�O��ʒu
	D3DXVECTOR3 move;			//���x
	float fWidth;				//��
	float fHeight;				//����
	int nCntAnim;				//�A�j���[�V�����J�E���^
	int nPtnAnim;				//�A�j���[�V�����p�^�[��
	int nCntStart;				//�J�n���t���[���͈ړ��s��
	float fDirectionMove;		//���E�̌����i�e�N�X�`����i���i�؂�ւ��j
	Block* pBlock;				//�Ώۂ̃u���b�N�ւ̃|�C���^
	int nDeath;					//���S��
	int nRevivalCnter;			//�����܂ł̃J�E���^�[
	bool bUse;					//�����Ă����true
	bool bDash;					//�ːi��true
}Player;

//�v���g�^�C�v�錾
void InitPlayer(void);			//������
void UninitPlayer(void);		//�I��
void UpdatePlayer(void);		//�X�V
void DrawPlayer(void);			//�`��

void GetPlayer(float *pGravity,float*pJunpSpeed);			//���̎擾
Player* GetPlayer(void);									//���̎擾
#endif // !_PLAYER_H_
