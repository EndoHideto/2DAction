//=============================================================================
// 
// �u���b�N�̏��� [block.h]
// Author : ENDO HIDETO
// 
//=============================================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_	//2�d�C���N���[�h�h�~

#include "main.h"

#define BLOCK_VER_NUM		(4)							//���_��
#define MAX_BLOCK			(255)						//�u���b�N�ő吔
#define FILE_SAVEDATA		"data\\STAGE\\block.bin"	//�u���b�N����ۑ�����t�@�C��
#define BLOCKTEX_INTERVAL	(30.0f)						//�u���b�N�e�N�X�`�����J��Ԃ���

#define BLOCKHIT_NONE		(0b0000)					//�u���b�N�ɓ������ĂȂ�
#define BLOCKHIT_LAND		(0b1000)					//�n�ʂɗ������Ƃ�
#define BLOCKHIT_HEAD		(0b0100)					//��������������
#define BLOCKHIT_RIGHT		(0b0010)					//�E���炠����
#define BLOCKHIT_LEFT		(0b0001)					//�����炠����

//�u���b�N�̎�ޗ񋓌^
typedef enum
{
	BLOCKTYPE_NORMAL = 0,
	BLOCKTYPE_SPIKE,
	BLOCKTYPE_ICE,
	BLOCKTYPE_MAX
}BlockType;



//�u���b�N�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 posOrigin;	//�ݒu�ʒu
	D3DXVECTOR3 pos;		//���݂̈ʒu
	D3DXVECTOR3 posOld;		//�O�̈ʒu
	float fWidth;			//��
	float fHeight;			//����
	float fAngle;			//�Ίp���̊p�x
	float fLength;			//�Ίp���̒���
	D3DXVECTOR3 move;		//�ړ����x
	float fRange;			//�ړ�����͈�
	BlockType type;			//�u���b�N�̎��
	float fDirection;		//�����i90�x���݁j
	bool bUse;				//�g���Ă��邩
}Block;

//�u���b�N�ݒu���\����
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	float fWidth;			//��
	float fHeight;			//����
	D3DXVECTOR3 move;		//�ړ����x
	float fRange;			//�ړ�����͈�
	BlockType type;			//�u���b�N�̎��
	float fDirection;		//�e�N�X�`������
}BlockInfo;

//�G�f�B�b�g���̍\����
typedef struct
{
	int nSetBlock;			//�ݒu�ς݃u���b�N��
	int nTagBlock;			//���ݑI�𒆂̃u���b�N�̔ԍ�
	Block* pBlock;			//���̃|�C���^
	bool bEdit;				//�G�f�B�b�g���[�hON�EOFF
}Edit;

//�v���g�^�C�v�錾
void InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);
void CollisionBlock(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fTagHeight, float fTagWidth, Block** pBlock,int *pLand);
void SetBlock(D3DXVECTOR3 pos,float fWidth, float fHeight,D3DXVECTOR3 move, float fRange,BlockType type,float fDirection);
Block* GetBlock(void);


Edit GetEdit(void);

#endif // !_BLOCK_H_
