//=============================================================================
// 
// �w�i�̏��� [bg.h]
// Author : ENDO HIDETO
// 
//=============================================================================
#ifndef _BG_H_
#define _BG_H_	//2�d�C���N���[�h�h�~

#include "main.h"

#define BG_VER_NUM (4)		//���_��

typedef enum
{
	BG_0 = 0,
	BG_1,
	BG_2,
	BGLAYER_MAX
}Bglayer;

//�v���g�^�C�v�錾
void InitBG(void);
void UninitBG(void);
void UpdateBG(void);
void DrawBG(void);

#endif // !_BG_H_
