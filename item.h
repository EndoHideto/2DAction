//=============================================================================
// 
// �A�C�e������ [item.h]
// Author : ENDO HIDETO
// 
//=============================================================================
#ifndef _ITEM_H_
#define _ITEM_H_	//2�d�C���N���[�h�h�~

#include "main.h"

#define ITEM_VER_NUM (4)		//���_��
#define GOAL_SIZE (80)			//�A�C�e���̑傫��

//�u���b�N�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;		//���݂̈ʒu
	float fSize;			//�傫��
	bool bUse;				//�g���Ă��邩
}Item;

//�v���g�^�C�v�錾
void InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);

#endif // !_ITEM_H_
