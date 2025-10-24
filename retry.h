//=============================================================================
// 
// �������̏��� [retry.h]
// Author : ENDO HIDETO
// 
//=============================================================================
#ifndef _RETRY_H_
#define _RETRY_H_	//2�d�C���N���[�h�h�~

#include "main.h"

#define RETRY_WIDTH			(80.0f)		//���g���C�̕\����
#define RETRY_HEIGHT		(40.0f)		//���g���C�̕\������
#define RETRY_COUTER		(10)		//���肵�Ă�����ۂɓ�����܂ł̃J�E���^�[
#define RETRY_TEXTURE		(2)			//�e�N�X�`����

//�v���g�^�C�v�錾
void InitRetry(void);
void UninitRetry(void);
void UpdateRetry(void);
void DrawRetry(void);

#endif // !_RETRY_H_
