//=============================================================================
// 
// �L�[�{�[�h���� [input.cpp]
// Author : ENDO HIDETO
// 
//=============================================================================
#include "input.h"

//�}�N����`
#define NUM_KEY_MAX (256)					//�L�[�̍ő�l

//�O���[�o���ϐ��錾
LPDIRECTINPUT8 g_pInput = NULL;					//DirectInput�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;		//���̓f�o�C�X(�L�[�{�[�h)�ւ̃|�C���^
LPDIRECTINPUTDEVICE8 g_pDevMouse = NULL;		//���̓f�o�C�X(�}�E�X)�ւ̃|�C���^

BYTE g_aKeyState[NUM_KEY_MAX];					//�L�[�{�[�h�̃v���X���
BYTE g_aKeyStateTrigger[NUM_KEY_MAX];			//�L�[�{�[�h�̃g���K�[���
BYTE g_aKeyStateRelease[NUM_KEY_MAX];			//�L�[�{�[�h�̃����[�X���
BYTE g_aKeyStateRepeat[NUM_KEY_MAX];			//�L�[�{�[�h�̃��s�[�g���
BYTE g_aKeyStateTimer[NUM_KEY_MAX];				//�L�[�{�[�h�̃^�C�}�[���

XINPUT_STATE g_joykeyState;						//�W���C�p�b�h�̃v���X���
XINPUT_STATE g_joykeyStateTrigger;				//�W���C�p�b�h�̃g���K�[���
XINPUT_STATE g_joykeyStateRelease;				//�W���C�p�b�h�̃����[�X���
XINPUT_STATE g_joykeyStateRepeat;				//�W���C�p�b�h�̃��s�[�g
XINPUT_STATE g_joykeyStateTimer;				//�W���C�p�b�h�̃^�C�}�[���

short g_sDeadzone;								//�W���C�X�e�B�b�N�̃f�b�h�]�[��

int g_aKeyCounter[NUM_KEY_MAX] = { 0 };

//=======================================
// ������
//=======================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInput�I�u�W�F�N�g�̐���
	if (FAILED(DirectInput8Create(hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&g_pInput,
		NULL)))
	{	return E_FAIL;	}

	//���̓f�o�C�X�i�L�[�{�[�h�j�̐���
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard,
		&g_pDevKeyboard,
		NULL)))
	{	return E_FAIL;	}

	//�}�E�X�̐���
	if (FAILED(g_pInput->CreateDevice(GUID_SysMouse,
		&g_pDevMouse,
		NULL)))
	{	return E_FAIL;	}

	//�f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{	return E_FAIL;	}
	if (FAILED(g_pDevMouse->SetDataFormat(&c_dfDIMouse)))
	{	return E_FAIL;	}

	//������ݒ�
	if(FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{	return E_FAIL;	}

	if(FAILED(g_pDevMouse->SetCooperativeLevel(hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{	return E_FAIL;	}

	//�L�[�{�[�h�̃A�N�Z�X�����l��
	g_pDevKeyboard->Unacquire();

	//�}�E�X�̃A�N�Z�X�����l��
	g_pDevMouse->Unacquire();

	//�f�b�h�]�[����ݒ�
	g_sDeadzone = 5000;

	return S_OK;
}

//=======================================
// �I��
//=======================================
void UninitKeyboard(void)
{
	//���̓f�o�C�X�̔j��
	if (g_pDevKeyboard != NULL)
	{//�L�[�{�[�h�̔j��
		g_pDevKeyboard->Unacquire();
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}
	if (g_pDevMouse != NULL)
	{//�}�E�X�̔j��
		g_pDevMouse->Unacquire();
		g_pDevMouse->Release();
		g_pDevMouse = NULL;
	}

	//DirectInput�I�u�W�F�N�g�̔j��
	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//***************************************
// �L�[�{�[�h�̍X�V
//***************************************
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];

	//���̓f�o�C�X����f�[�^���l��
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{   
			g_aKeyStateTrigger[nCntKey] = aKeyState[nCntKey] & ~g_aKeyState[nCntKey];		//�L�[�g���K�[����ۑ�
			g_aKeyStateRelease[nCntKey] = ~aKeyState[nCntKey] & g_aKeyState[nCntKey];		//�L�[�����[�X����ۑ�
			g_aKeyStateRepeat[nCntKey] = aKeyState[nCntKey] & ~g_aKeyState[nCntKey];		//�L�[���s�[�g����ۑ�
			if (g_aKeyStateRepeat[nCntKey])
			{	
				g_aKeyCounter[nCntKey] = REPEATTIMER;
			}
			else
			{
				if (0 < g_aKeyCounter[nCntKey] && g_aKeyCounter[nCntKey] <= REPEATTIMER)
				{//�J�E���^�[��i�߂�
					g_aKeyCounter[nCntKey]--;
				}
				else
				{
					g_aKeyStateRepeat[nCntKey] = aKeyState[nCntKey];
				}
			}
			g_aKeyState[nCntKey] = aKeyState[nCntKey];										//�L�[�v���X����ۑ�

		}
	}
	else
	{
		g_pDevKeyboard->Acquire();	//�L�[�{�[�h�A�N�Z�X�����l��
	}
}

//=======================================
// �L�[�{�[�h�̃v���X���̎擾
//=======================================
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true : false;
}

//=======================================
// �L�[�{�[�h�̃g���K�[���̎擾
//=======================================
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//=======================================
// �L�[�{�[�h�̃����[�X���̎擾
//=======================================
bool GetKeyboardRelease(int nKey)
{
	return (g_aKeyStateRelease[nKey] & 0x80) ? true : false;
}


//=======================================
// �L�[�{�[�h�̃��s�[�g���̎擾
//=======================================
bool GetKeyboardRepeat(int nKey)
{
	return (g_aKeyStateRepeat[nKey] & 0x80) ? true : false;
}

//=======================================
// �W���C�p�b�h�̏���������
//=======================================
HRESULT InitJoypad(void)
{
	//�������̃N���A
	memset(&g_joykeyState,0,sizeof(XINPUT_STATE));

	//XInput�̃X�e�[�g��ݒ�(�L���ɂ���)
	XInputEnable(true);

	return S_OK;
}

//=======================================
// �W���C�p�b�h�̏I������
//=======================================
void UninitJoypad(void)
{
	//XInput�̃X�e�[�g��ݒ�(�����ɂ���)
	XInputEnable(false);
}

//***************************************
// �W���C�p�b�h�̍X�V����
//***************************************
void UpdateJoypad(void)
{
	XINPUT_STATE joykeyState;	//�W���C�p�b�h�̓��͏��

	//�W���C�p�b�h�̏�Ԃ��擾
	if (XInputGetState(0, &joykeyState) == ERROR_SUCCESS)
	{
		g_joykeyStateTrigger.Gamepad.wButtons = joykeyState.Gamepad.wButtons & ~g_joykeyState.Gamepad.wButtons;
		g_joykeyStateTrigger.Gamepad.bLeftTrigger = joykeyState.Gamepad.bLeftTrigger & ~g_joykeyState.Gamepad.bLeftTrigger;
		g_joykeyStateTrigger.Gamepad.bRightTrigger= joykeyState.Gamepad.bRightTrigger & ~g_joykeyState.Gamepad.bRightTrigger;
		g_joykeyStateRelease.Gamepad.wButtons = ~joykeyState.Gamepad.wButtons & g_joykeyState.Gamepad.wButtons;
		g_joykeyState = joykeyState;

		//�f�b�h�]�[�����Ȃ�0��
		if (-g_sDeadzone < joykeyState.Gamepad.sThumbLX && joykeyState.Gamepad.sThumbLX < g_sDeadzone)
			g_joykeyState.Gamepad.sThumbLX = 0;

		if (-g_sDeadzone < joykeyState.Gamepad.sThumbLY && joykeyState.Gamepad.sThumbLY < g_sDeadzone)
			g_joykeyState.Gamepad.sThumbLY = 0;

		if (-g_sDeadzone < joykeyState.Gamepad.sThumbRX && joykeyState.Gamepad.sThumbRX < g_sDeadzone)
			g_joykeyState.Gamepad.sThumbRX = 0;

		if (-g_sDeadzone < joykeyState.Gamepad.sThumbRY && joykeyState.Gamepad.sThumbRY < g_sDeadzone)
			g_joykeyState.Gamepad.sThumbRY = 0;
	}
}

//=======================================
// �W���C�p�b�h�̃v���X���
//=======================================
bool GetJoypadPress(JOYKEY key)
{
	if (key == JOYKEY_LT)
		return (g_joykeyState.Gamepad.bLeftTrigger & 0xff) ? true : false;
	if (key == JOYKEY_RT)
		return (g_joykeyState.Gamepad.bRightTrigger & 0xff) ? true : false;
	else
		return (g_joykeyState.Gamepad.wButtons & 0x01 << key) ? true : false;
}

//=======================================
// �Q�[���p�b�h�̃g���K�[���̎擾
//=======================================
bool GetJoypadTrigger(JOYKEY key)
{
	if (key == JOYKEY_LT)
		return (g_joykeyStateTrigger.Gamepad.bLeftTrigger & 0xff) ? true : false;
	if (key == JOYKEY_RT)
		return (g_joykeyStateTrigger.Gamepad.bRightTrigger & 0xff) ? true : false;
	else
		return (g_joykeyStateTrigger.Gamepad.wButtons & 0x01 << key) ? true : false;
}

//=======================================
// �Q�[���p�b�h�̃����[�X���̎擾
//=======================================
bool GetJoypadRelease(JOYKEY key)
{
	return (g_joykeyStateRelease.Gamepad.wButtons & 0x01 << key) ? true : false;
}

//=======================================
// �Q�[���p�b�h�̃��s�[�g���̎擾
//=======================================
bool GetJoypadRepeat(JOYKEY key)
{
	return (g_joykeyStateRelease.Gamepad.wButtons & 0x01 << key) ? true : false;
}

//=======================================
// �W���C�X�e�B�b�N�̏��
//=======================================
D3DXVECTOR3 GetJoypadStick(int nSide)
{
	if (nSide == LEFT)
		return D3DXVECTOR3((float)g_joykeyState.Gamepad.sThumbLX / 0x7fff, (float)-g_joykeyState.Gamepad.sThumbLY / 0x7fff, -atan2f(-g_joykeyState.Gamepad.sThumbLX, g_joykeyState.Gamepad.sThumbLY));
	else if (nSide == RIGHT)
		return D3DXVECTOR3((float)g_joykeyState.Gamepad.sThumbRX / 0x7fff, (float)-g_joykeyState.Gamepad.sThumbRY / 0x7fff, -atan2f(-g_joykeyState.Gamepad.sThumbRX / 0x7fff, g_joykeyState.Gamepad.sThumbRX / 0x7fff));
	else
		return D3DXVECTOR3(0.0f,0.0f,0.0f);
}

//=======================================
// �f�b�h�]�[���̐ݒ�
//=======================================
void SetDeadzone(short sDeadzone)
{
	g_sDeadzone = sDeadzone;
}

D3DXVECTOR2 GetValue(void)
{
	return D3DXVECTOR2(g_joykeyState.Gamepad.sThumbLX, -g_joykeyState.Gamepad.sThumbLY);
}
int GetTrigger(void)
{
	return g_joykeyState.Gamepad.bRightTrigger;
}