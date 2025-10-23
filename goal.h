//=============================================================================
// 
// �S�[������ [goal.h]
// Author : ENDO HIDETO
// 
//=============================================================================
#ifndef _GOAL_H_
#define _GOAL_H_	//2�d�C���N���[�h�h�~

#include "main.h"

#define GOAL_VER_NUM (4)		//���_��
#define GOAL_SIZE (110)			//�S�[���̑傫��
#define TEXTURE_GOAL (2)		//�e�N�X�`����

//�u���b�N�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;		//���݂̈ʒu
	float fLength;			//�傫��
	float fAngle;			//�`
	float fRot;				//��]
	int nOpen;				//0 �g�p�s�@/1 �g�p��
	bool bUse;				//�g���Ă��邩
}Goal;

//�v���g�^�C�v�錾
void InitGoal(void);
void UninitGoal(void);
void UpdateGoal(void);
void DrawGoal(void);
bool GameFinish(void);

#endif // !_GOAL_H_
