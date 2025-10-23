//=============================================================================
// 
// キーボード操作 [input.cpp]
// Author : ENDO HIDETO
// 
//=============================================================================
#include "input.h"

//マクロ定義
#define NUM_KEY_MAX (256)					//キーの最大値

//グローバル変数宣言
LPDIRECTINPUT8 g_pInput = NULL;					//DirectInputオブジェクトへのポインタ
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;		//入力デバイス(キーボード)へのポインタ
LPDIRECTINPUTDEVICE8 g_pDevMouse = NULL;		//入力デバイス(マウス)へのポインタ

BYTE g_aKeyState[NUM_KEY_MAX];					//キーボードのプレス情報
BYTE g_aKeyStateTrigger[NUM_KEY_MAX];			//キーボードのトリガー情報
BYTE g_aKeyStateRelease[NUM_KEY_MAX];			//キーボードのリリース情報
BYTE g_aKeyStateRepeat[NUM_KEY_MAX];			//キーボードのリピート情報
BYTE g_aKeyStateTimer[NUM_KEY_MAX];				//キーボードのタイマー情報

XINPUT_STATE g_joykeyState;						//ジョイパッドのプレス情報
XINPUT_STATE g_joykeyStateTrigger;				//ジョイパッドのトリガー情報
XINPUT_STATE g_joykeyStateRelease;				//ジョイパッドのリリース情報
XINPUT_STATE g_joykeyStateRepeat;				//ジョイパッドのリピート
XINPUT_STATE g_joykeyStateTimer;				//ジョイパッドのタイマー情報

short g_sDeadzone;								//ジョイスティックのデッドゾーン

int g_aKeyCounter[NUM_KEY_MAX] = { 0 };

//=======================================
// 初期化
//=======================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&g_pInput,
		NULL)))
	{	return E_FAIL;	}

	//入力デバイス（キーボード）の生成
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard,
		&g_pDevKeyboard,
		NULL)))
	{	return E_FAIL;	}

	//マウスの生成
	if (FAILED(g_pInput->CreateDevice(GUID_SysMouse,
		&g_pDevMouse,
		NULL)))
	{	return E_FAIL;	}

	//データフォーマットを設定
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{	return E_FAIL;	}
	if (FAILED(g_pDevMouse->SetDataFormat(&c_dfDIMouse)))
	{	return E_FAIL;	}

	//協調を設定
	if(FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{	return E_FAIL;	}

	if(FAILED(g_pDevMouse->SetCooperativeLevel(hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{	return E_FAIL;	}

	//キーボードのアクセス権を獲得
	g_pDevKeyboard->Unacquire();

	//マウスのアクセス権を獲得
	g_pDevMouse->Unacquire();

	//デッドゾーンを設定
	g_sDeadzone = 5000;

	return S_OK;
}

//=======================================
// 終了
//=======================================
void UninitKeyboard(void)
{
	//入力デバイスの破棄
	if (g_pDevKeyboard != NULL)
	{//キーボードの破棄
		g_pDevKeyboard->Unacquire();
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}
	if (g_pDevMouse != NULL)
	{//マウスの破棄
		g_pDevMouse->Unacquire();
		g_pDevMouse->Release();
		g_pDevMouse = NULL;
	}

	//DirectInputオブジェクトの破棄
	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//***************************************
// キーボードの更新
//***************************************
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];

	//入力デバイスからデータを獲得
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{   
			g_aKeyStateTrigger[nCntKey] = aKeyState[nCntKey] & ~g_aKeyState[nCntKey];		//キートリガー情報を保存
			g_aKeyStateRelease[nCntKey] = ~aKeyState[nCntKey] & g_aKeyState[nCntKey];		//キーリリース情報を保存
			g_aKeyStateRepeat[nCntKey] = aKeyState[nCntKey] & ~g_aKeyState[nCntKey];		//キーリピート情報を保存
			if (g_aKeyStateRepeat[nCntKey])
			{	
				g_aKeyCounter[nCntKey] = REPEATTIMER;
			}
			else
			{
				if (0 < g_aKeyCounter[nCntKey] && g_aKeyCounter[nCntKey] <= REPEATTIMER)
				{//カウンターを進める
					g_aKeyCounter[nCntKey]--;
				}
				else
				{
					g_aKeyStateRepeat[nCntKey] = aKeyState[nCntKey];
				}
			}
			g_aKeyState[nCntKey] = aKeyState[nCntKey];										//キープレス情報を保存

		}
	}
	else
	{
		g_pDevKeyboard->Acquire();	//キーボードアクセス権を獲得
	}
}

//=======================================
// キーボードのプレス情報の取得
//=======================================
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true : false;
}

//=======================================
// キーボードのトリガー情報の取得
//=======================================
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//=======================================
// キーボードのリリース情報の取得
//=======================================
bool GetKeyboardRelease(int nKey)
{
	return (g_aKeyStateRelease[nKey] & 0x80) ? true : false;
}


//=======================================
// キーボードのリピート情報の取得
//=======================================
bool GetKeyboardRepeat(int nKey)
{
	return (g_aKeyStateRepeat[nKey] & 0x80) ? true : false;
}

//=======================================
// ジョイパッドの初期化処理
//=======================================
HRESULT InitJoypad(void)
{
	//メモリのクリア
	memset(&g_joykeyState,0,sizeof(XINPUT_STATE));

	//XInputのステートを設定(有効にする)
	XInputEnable(true);

	return S_OK;
}

//=======================================
// ジョイパッドの終了処理
//=======================================
void UninitJoypad(void)
{
	//XInputのステートを設定(無効にする)
	XInputEnable(false);
}

//***************************************
// ジョイパッドの更新処理
//***************************************
void UpdateJoypad(void)
{
	XINPUT_STATE joykeyState;	//ジョイパッドの入力情報

	//ジョイパッドの状態を取得
	if (XInputGetState(0, &joykeyState) == ERROR_SUCCESS)
	{
		g_joykeyStateTrigger.Gamepad.wButtons = joykeyState.Gamepad.wButtons & ~g_joykeyState.Gamepad.wButtons;
		g_joykeyStateTrigger.Gamepad.bLeftTrigger = joykeyState.Gamepad.bLeftTrigger & ~g_joykeyState.Gamepad.bLeftTrigger;
		g_joykeyStateTrigger.Gamepad.bRightTrigger= joykeyState.Gamepad.bRightTrigger & ~g_joykeyState.Gamepad.bRightTrigger;
		g_joykeyStateRelease.Gamepad.wButtons = ~joykeyState.Gamepad.wButtons & g_joykeyState.Gamepad.wButtons;
		g_joykeyState = joykeyState;

		//デッドゾーン内なら0に
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
// ジョイパッドのプレス情報
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
// ゲームパッドのトリガー情報の取得
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
// ゲームパッドのリリース情報の取得
//=======================================
bool GetJoypadRelease(JOYKEY key)
{
	return (g_joykeyStateRelease.Gamepad.wButtons & 0x01 << key) ? true : false;
}

//=======================================
// ゲームパッドのリピート情報の取得
//=======================================
bool GetJoypadRepeat(JOYKEY key)
{
	return (g_joykeyStateRelease.Gamepad.wButtons & 0x01 << key) ? true : false;
}

//=======================================
// ジョイスティックの情報
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
// デッドゾーンの設定
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