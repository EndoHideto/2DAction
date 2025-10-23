//=============================================================================
// 
// �L�[�{�[�h���� [input.h]
// Author : ENDO HIDETO
// 
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

#define STICKMAX (0x7FFF)		//�X�e�B�b�N���ő�܂œ|�����Ƃ��̒l
#define LEFT (0)				//���X�e�B�b�N
#define RIGHT (1)				//�E�X�e�B�b�N
#define REPEATTIMER (20)		//���s�[�g����܂ł̎���

//���͔}�̗񋓌^
typedef enum
{
	INPUTDEVICE_KEYBOAD = 0,	//�L�[�{�[�h����
	INPUTDEVICE_GAMEPAD,		//�Q�[���p�b�h����
	INPUTDEVICE_MAX,
}INPUTDEVICE;

//�W���C�p�b�h����
typedef enum
{
	JOYKEY_UP = 0,		//[ 0]�\���L�[�@��
	JOYKEY_DOWN,		//[ 1]�\���L�[�@��
	JOYKEY_LEFT,		//[ 2]�\���L�[�@��
	JOYKEY_RIGHT,		//[ 3]�\���L�[�@�E
	JOYKEY_START,		//[ 4]START
	JOYKEY_BACK,		//[ 5]BACK
	JOYKEY_LS,			//[ 6]���X�e�B�b�N��������
	JOYKEY_RS,			//[ 7]�E�X�e�B�b�N��������
	JOYKEY_LB,			//[ 8]LB�{�^��
	JOYKEY_RB,			//[ 9]RB�{�^��
	JOYKEY_LT,			//[10]LT�{�^��
	JOYKEY_RT,			//[11]RT�{�^��
	JOYKEY_A,			//[12]A�{�^��
	JOYKEY_B,			//[13]B�{�^��
	JOYKEY_X,			//[14]X�{�^��
	JOYKEY_Y,			//[15]Y�{�^��

	JOYKEY_MAX
}JOYKEY;

//�v���g�^�C�v�錾
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);
void UninitKeyboard(void);
void UpdateKeyboard(void);
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);
bool GetKeyboardRelease(int nKey);
bool GetKeyboardRepeat(int nKey);

HRESULT InitJoypad(void);
void UninitJoypad(void);
void UpdateJoypad(void);
bool GetJoypadPress(JOYKEY key);
bool GetJoypadTrigger(JOYKEY key);
bool GetJoypadRelease(JOYKEY key);
bool GetJoypadRepeat(JOYKEY key);
D3DXVECTOR3 GetJoypadStick(int nSide);

void SetDeadzone(short sDeadzone);

D3DXVECTOR2 GetValue(void);
int GetTrigger(void);
#endif // !_INPUT_H_