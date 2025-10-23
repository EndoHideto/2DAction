//=============================================================================
// 
// キーボード操作 [input.h]
// Author : ENDO HIDETO
// 
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

#define STICKMAX (0x7FFF)		//スティックを最大まで倒したときの値
#define LEFT (0)				//左スティック
#define RIGHT (1)				//右スティック
#define REPEATTIMER (20)		//リピートするまでの時間

//入力媒体列挙型
typedef enum
{
	INPUTDEVICE_KEYBOAD = 0,	//キーボード操作
	INPUTDEVICE_GAMEPAD,		//ゲームパッド操作
	INPUTDEVICE_MAX,
}INPUTDEVICE;

//ジョイパッド操作
typedef enum
{
	JOYKEY_UP = 0,		//[ 0]十字キー　上
	JOYKEY_DOWN,		//[ 1]十字キー　下
	JOYKEY_LEFT,		//[ 2]十字キー　左
	JOYKEY_RIGHT,		//[ 3]十字キー　右
	JOYKEY_START,		//[ 4]START
	JOYKEY_BACK,		//[ 5]BACK
	JOYKEY_LS,			//[ 6]左スティック押し込み
	JOYKEY_RS,			//[ 7]右スティック押し込み
	JOYKEY_LB,			//[ 8]LBボタン
	JOYKEY_RB,			//[ 9]RBボタン
	JOYKEY_LT,			//[10]LTボタン
	JOYKEY_RT,			//[11]RTボタン
	JOYKEY_A,			//[12]Aボタン
	JOYKEY_B,			//[13]Bボタン
	JOYKEY_X,			//[14]Xボタン
	JOYKEY_Y,			//[15]Yボタン

	JOYKEY_MAX
}JOYKEY;

//プロトタイプ宣言
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