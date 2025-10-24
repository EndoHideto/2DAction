//=============================================================================
// 
// �^�C�}�[�̏��� [timer.h]
// Author : ENDO HIDETO
// 
//=============================================================================
#ifndef TIMER_H_
#define TIMER_H_				//2�d�C���N���[�h�h�~

#include "main.h"
#include <time.h>

//�}�N����`
#define TIMER_TEXTURE (2)		//timer.cpp�Ŏg�p����e�N�X�`����
#define TIMER_PLACE (8)			//�\�����錅��
#define TIMER_WIDTH (30)		//�^�C�}�[�ꌅ�̕�
#define TIMER_HEIGHT (60)		//				����
#define TIMERSTART_OBJPOS D3DXVECTOR3(85.0f,SCREEN_HEIGHT - 200.0f,0.0f)	//�^�C�}�[�J�n�̃I�u�W�F�N�g�̈ʒu

//�^�C�}�[�\����
typedef struct
{
	D3DXVECTOR3 posCounter;		//�^�C���̊�ʒu
	float fWidth;				//�J�E���^�[�̕�
	float fHeight;				//�J�E���^�[�̍���
	D3DXVECTOR3 posStart;		//�v���J�n�I�u�W�F�N�g�̈ʒu
	float fSize;				//�v���J�n�I�u�W�F�N�g�̃T�C�Y
	time_t tStarttime;			//�J�n����
	int nTime;					//�o�߃^�C��(�b)
	bool bTimerStart;			//�^�C�}�[�������Ă��邩�ǂ���
	bool bTimerBreak;			//�^�C�}�[���ꎞ��~���Ă��邩�ǂ���
	time_t breaktime;			//�u���C�N��������������
	time_t tBreak;					//�ꎞ��~���̍���
	time_t tSubTime;				//�����̍��v
}Timer;

//�v���g�^�C�v�錾
void InitTimer(void);			//������
void UninitTimer(void);			//�I��
void UpdateTimer(void);			//�X�V
void DrawTimer(void);			//�`��

void StopTimer(void);			//�^�C�}�[�X�g�b�v
void BreakTimer(bool bTimer);	//�^�C�}�[�ꎞ��~�E���X�^�[�g
Timer GetTimer(void);			//�^�C�}�[���̎擾
#endif // !TIMER_H_
