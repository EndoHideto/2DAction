//=============================================================================
// 
// �^�C�g�� [title.h]
// Author : ENDO HIDETO
// 
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_	//2�d�C���N���[�h�h�~

#include "main.h"

#define TITLE_THING		(6)						//�ݒu�������
#define TITLE_TEXTURE	(5)						//�e�N�X�`����
#define TITLE_POS		(410.0f,700.0f,0.0f)	//�^�C�g���̈ʒu
#define TITLE_WIDTH		(360.0f)				//			���̔���
#define TITLE_HEIGHT	(400.0f)				//			����
#define TITLE_POS_START	()
#define TITLE_POS_END	()
#define TITLE_POS_LIGHT (65.0f, 720.0f, 0.0f)
#define LIGHT_WIDTH		(200.0f)				//���C�g�i����j�̕��̔���
#define LIGHT_HEIGHT	(500.0f)				//���C�g�i����j�̍���

typedef struct
{
	D3DXVECTOR3 pos;
	float fWidth;
	float fHeight;
	float fAngle;
	float fLength;
}TitileObject;

//�v���g�^�C�v�錾
void InitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);

#endif // !_TITLE_H_
