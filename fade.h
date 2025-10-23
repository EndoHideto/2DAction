//=============================================================================
// 
// �ӂ��`�ǂ̏��� [fade.h]
// Author : ENDO HIDETO
// 
//=============================================================================
#ifndef _FADE_H_
#define _FADE_H_				//2�d�C���N���[�h�h�~

#include "main.h"

#define FADE_VER_NUM (4)		//���_��
#define FADE_LAYER (3)			//�t�F�[�h���C���[��

//�t�F�[�h�񋓌^��`
typedef enum
{
	FADE_NONE = 0,				//�������ĂȂ����
	FADE_IN,					//�t�F�[�h�C��
	FADE_OUT,					//�t�F�[�h�A�E�g
	FADE_MAX
}FADE;

//�v���g�^�C�v�錾
void InitFade(void);
void UninitFade(void);
void UpdateFade(void);
void DrawFade(void);
void SetFade(MODE nextmode);
FADE GetFade(void);

#endif // !_FADE_H_
