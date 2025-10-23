//=============================================================================
// 
// �|�[�Y���� [pause.h]
// Author : ENDO HIDETO
// 
//=============================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "main.h"

//�}�N����`
#define PAUSE_TEXTURE (4)		//�|�[�Y�����Ŏg�p����e�N�X�`�����
#define PAUSE_WIDTH (250.0f)	//�I������Ă��Ȃ��|�[�Y�I�����̉���
#define PAUSE_HEIGHT (50.0f)	//�I������Ă��Ȃ��|�[�Y�I�����̍���
#define PAUSE_CHOICE (10.0f)	//�I�����ꂽ�Ƃ��̊g��
#define PAUSE_LINE (60.0f)		//�|�[�Y�I�����̊Ԋu

//�|�[�Y���j���[�񋓌^
typedef enum
{
	PAUSE_CONTINUE = 0,			//�R���e�B�j���[
	PAUSE_RETRY,				//���g���C
	PAUSE_QUIT,					//�^�C�g����
	PAUSE_MAX			
}PAUSEMENU;


//�v���g�^�C�v�錾
void InitPause(void);		//������
void UninitPause(void);		//�I��
void UpdatePause(void);		//�X�V
void DrawPause(void);		//�`��
bool GetPause(void);		//�|�[�Y�����ǂ����擾

#endif // !_PAUSE_H_