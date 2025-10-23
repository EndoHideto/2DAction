//=============================================================================
// 
// ���U���g [result.h]
// Author : ENDO HIDETO
// 
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_	//2�d�C���N���[�h�h�~

#include "main.h"
#include "timer.h"
#include "input.h"

#define RESULT_THING (TIMER_PLACE + 3)		//���U���g��ʂɕ\������l�p�̐�
#define RESULT_TEXTURE (4)					//���U���g��ʂŎg���e�N�X�`���̐�
#define COUNTER_RESURT (50)					//���U���g��ʂ�\����������t���[����
#define RESULT_WIDTH (400)					//�g�̕�
#define RESULT_HEIGHT (500)					//�g�̍���
#define RESULTTIMER_LINE (120.0f)			//���U���g�g���̃^�C�}�[�̈ʒu
#define RESULTTIMER_WIDTH (45)					//�^�C�}�[�ꌅ�̕�
#define RESULTTIMER_HEIGHT (90)					//�^�C�}�[�ꌅ�̍���
#define RESULTOPERATE_LINE (400.0f)			//������E��߂�@�̈ʒu
#define OPERATE_WIDTH (160)					//������E��߂�@�̕�
#define OPERATE_HEIGHT (40)					//������E��߂�@�̍���
#define RESULT_MOVE (10.0f)					//�~��Ă��鑬��
#define RESULT_CHOICE (8.0f)				//�I�𒆂̋����\��

typedef enum
{
	RESULTCONTINUE_RETRY = 0,				//������x
	RESULTCONTINUE_QUIT,					//�I��
	RESULTCONTINUE_MAX
}ResultContinue;

typedef struct
{
	D3DXVECTOR3 posBox;										//�g�\���ʒu
	D3DXVECTOR3 posTime;									//�^�C���\���ʒu
	D3DXVECTOR3 posOperate[RESULTCONTINUE_MAX];				//���U���g��ʂł̑���\���ʒu
	int nCounter;											//�\������
	int nTime;												//�N���A�^�C��
	int nChose;												//�I�𒆂̔ԍ�
	bool bMove;												//�����Ă��邩
}Result;

//�v���g�^�C�v�錾
void InitResult(void);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);
void SetResult(void);
#endif // !_RESULT_H_
