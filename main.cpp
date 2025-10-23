//==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*
// 
// シューティング [main.cpp]
// Author : ENDO HIDETO
// 
//==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*
#include "main.h"
#include "input.h"
#include "fade.h"
//#include "sound.h"
#include "title.h"
#include "game.h"
#include "player.h"
#include "block.h"
#include "timer.h"
#include "result.h"
#include "pause.h"

//マクロ定義
#define CLASS_NAME	"WindowClass"			//ウィンドウクラスの名前
#define WINDOW_NAME	"アクション"			//ウィンドウの名前（キャプション）

//プロトタイプ宣言
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uinit(void);
void Update(void);
void Draw(void);
void DrawDebug(void);

//グローバル変数宣言
LPDIRECT3D9 g_pD3D = NULL;					//DirectX3Dオブジェクトへのポインタ
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;		//DirectX3Dデバイスへのポインタ
MODE g_mode = MODE_TITLE;					//現在のモード
LPD3DXFONT g_pFont = NULL;					//フォントのポインタ
int g_nCountFPS = 0;						//FPSカウンター

//=======================================
// メイン関数
//=======================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	DWORD dwCurrentTime;						//現在時刻
	DWORD dwExecLastTime;						//最後に処理した時刻
	DWORD dwFramCount;							//フレームカウント
	DWORD dwFPSLastTime;						//最後にFPSを計測した時刻

	//ランダムシードの生成
	srand(timeGetTime());

	//画面サイズの構造体
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	//デバッグ表示用変数の初期化
	dwFramCount = 0;
	dwFPSLastTime = timeGetTime();

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),						//WNDCLASSEXのメモリサイズ
		CS_CLASSDC,								//ウィンドウのスタイル
		WindowProc,								//ウィンドウプロシージャ
		0,										//通常は使用しない
		0,										//同上
		hInstance,								//インスタンスハンドル
		LoadIcon(NULL, IDI_APPLICATION),		//タスクバーのアイコン（自作可能
		LoadCursor(NULL,IDC_ARROW),				//カーソル（自作可能
		(HBRUSH)(COLOR_WINDOW + 1),				//背景色
		NULL,									//メニューバー
		CLASS_NAME,								//ウィンドウクラスの名前
		LoadIcon(NULL,IDI_APPLICATION),			//ファイル上のアイコン
	};

	HWND hWnd;
	MSG  msg;

	//ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	//クライアント領域を指定サイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//ウィンドウを生成
	hWnd = CreateWindowEx(0,					//拡張ウィンドウスタイル
		CLASS_NAME,								//ウィンドウクラスの名前
		WINDOW_NAME,							//ウィンドウの名前
		WS_OVERLAPPEDWINDOW,					//ウィンドウスタイル
		CW_USEDEFAULT,							//左上X座標
		CW_USEDEFAULT,							//左上Y座標
		(rect.right - rect.left),				//幅
		(rect.bottom - rect.top),				//高さ
		NULL,									//親ウィンドウのハンドル（このウィンドウが最上位なのでなし
		NULL,									//メニューハンドル（子ウィンドウID
		hInstance,								//インスタンスハンドル
		NULL);									//ウィンドウ作成データ

	//初期化処理
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{//初期化に失敗したら
		return -1;
	}

	//分解能を設定
	timeBeginPeriod(1);
	dwCurrentTime = 0;
	dwExecLastTime = timeGetTime();

	//ウィンドウ表示
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//メッセージループ
	while(1)
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE) != 0)
		{//windowの処理
			if (msg.message == WM_QUIT)
			{//WM_QUITメッセージを受け取ったらループを抜ける
				break;
			}
			else
			{
				//メッセージの設定
				TranslateMessage(&msg);
				DispatchMessage(&msg);				
			}
		}
		else
		{//DirectXの処理
			dwCurrentTime = timeGetTime();
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5秒
				//FPSを計測
				g_nCountFPS = (dwFramCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;		//FPSを計測した時刻を保存
				dwFramCount = 0;					//カウントクリア
			}

			if (dwCurrentTime - dwExecLastTime >= 1000 / 60)
			{//1/60秒
					dwExecLastTime = dwCurrentTime;

					//更新処理
					Update();

					//描画処理
					Draw();

					//フレームカウント
					dwFramCount++;
			}
		}
	}
	//ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//=======================================
// ウィンドウプロシージャ
//=======================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;		//返り値用

	switch (uMsg)
	{
	case WM_DESTROY:// > ウィンドウ破棄のメッセージなら
			//WM_QUITメッセージを送る
			PostQuitMessage(0);
		break;// > DESTROY

	case WM_KEYDOWN:// > キーが押されたら
		switch (wParam)
		{
		case VK_ESCAPE:	// >> [ESC]キーなら
			//WM_DESTROYメッセージを送る
			nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);
			if (nID == IDYES)
				//ウィンドウ破棄メッセージ
				DestroyWindow(hWnd);
			break;// >> ESC
		}
		break;// > KEYDOWN
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//=======================================
// 初期化処理
//=======================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			//ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;	//プレゼンテーションパラメータ

	//Direct3Dオブジェクトの生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp)); //パラメータのゼロクリア

	d3dpp.BackBufferWidth = SCREEN_WIDTH;							//ゲーム画面の幅
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;							//高さ
	d3dpp.BackBufferFormat = d3ddm.Format;							//バックバッファの形式
	d3dpp.BackBufferCount = 1;										//		〃		の数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;						//ダブルバッファの切り替え（映像信号に同期
	d3dpp.EnableAutoDepthStencil = TRUE;							//デプスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;						//デプスバッファとして16bitを使う	
	d3dpp.Windowed = bWindow;										//ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		//リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		//インターバル

	//Direct3Dデバイスの生成
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{//描画・頂点処理をハードウェアでできないなら
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{//頂点処理をCPUにしてもだめなら
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{//描画もCPUにしてダメなら
				return E_FAIL;//あきらめて？
			}
		}
	}	

	//レンダーステートの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);

	//サンプラーステートの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);			//拡縮補完
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);			//	〃
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);			//テクスチャ繰り返し
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);			//	〃

	//テクスチャステージステートの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);			//テクスチャアルファブレンドの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//キーボードの初期化
	if (FAILED(InitKeyboard(hInstance, hWnd)))
		return E_FAIL;	//失敗したら終了

	//ジョイパッドの初期化
	if (FAILED(InitJoypad()))
		return E_FAIL;	//失敗したら終了

	//デバッグ表示用フォントの生成
	D3DXCreateFont(g_pD3DDevice,18,0,0,0,FALSE,SHIFTJIS_CHARSET,
					OUT_DEFAULT_PRECIS,DEFAULT_QUALITY,
					DEFAULT_PITCH,"Terminal",&g_pFont);

	//サウンドの初期化
	//InitSound(hWnd);

	//フェード初期化
	InitFade();

	//モードの設定
	SetMode(g_mode);

	return S_OK;
}

//=======================================
// 終了処理
//=======================================
void Uinit(void)
{
	//サウンド停止・終了
	//StopSound();
	//UninitSound();

	//タイトル終了
	UninitTitle();

	//ゲーム画面終了
	UninitGame();

	//リザルト終了
	UninitResult();

	//フェード終了
	UninitFade();

	//キーボード終了
	UninitKeyboard();	

	//ジョイパッド終了
	UninitJoypad();

	//デバッグ表示用フォントの破棄
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}

	//Direct3Dデバイスの破棄
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	//Direct3Dオブジェクトの破棄
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}

	//分解能を戻す
	timeEndPeriod(1);
}

//=======================================
// 更新処理
//=======================================
void Update(void)
{
	UpdateFade();			//フェード更新
	UpdateKeyboard();		//キーボード更新
	UpdateJoypad();			//ジョイパッド更新
	

	switch (g_mode)
	{
	case MODE_TITLE:		//タイトル
		UpdateTitle();
		break;
	case MODE_GAME:			//ゲーム画面
		UpdateGame();
		break;
	default:				//例外
		break;
	}

}

//=======================================
// 描画処理
//=======================================
void Draw(void)
{
	//画面クリア（バックバッファとZバッファのクリア
	g_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	//描画開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{//各種描画

		switch (g_mode)
		{
		case MODE_TITLE:		//タイトル
			DrawTitle();
			break;
		case MODE_GAME:			//ゲーム画面
			DrawGame();
			break;
		default:				//例外
			break;
		}
		//フェード描画
		DrawFade();
	}

#ifdef _DEBUG

	//デバッグ表示
	DrawDebug();

#endif

	//描画終了
	g_pD3DDevice->EndScene();
	
	//バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=======================================
// モードの設定
//=======================================
void SetMode(MODE mode)
{

	switch (g_mode)
	{
	case MODE_TITLE:		//タイトル
		UninitTitle();
		break;

	case MODE_GAME:			//ゲーム画面
		UninitGame();
		break;

	default:				//例外
		break;
	}

	switch (mode)
	{
	case MODE_TITLE:		//タイトル
		InitTitle();
		break;

	case MODE_GAME:			//ゲーム画面
		InitGame();
		break;

	default:				//例外
		break;
	}

	//モードの保存
	g_mode = mode;
}

//=======================================
// モードの取得
//=======================================
MODE GetMode(void)
{
	return g_mode;
}

//=======================================
// デバイスの取得（他のcppファイルでもD3DDeviceを呼び出せるようにする
//=======================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//=======================================
// デバッグ描画
//=======================================
void DrawDebug(void)
{
	char aStr[512];
	char aCtrl[512];
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	Edit edit = GetEdit();
	char aEdit[10] = {};
	float fGravity, fJunpSpeed;
	Player player = *GetPlayer();
	GetPlayer(&fGravity, &fJunpSpeed);
	Timer time = GetTimer();
	char sPause[32] = {};

	if (edit.bEdit)
	{
		strcpy(&aEdit[0], "ON");
	}
	else
	{
		strcpy(&aEdit[0], "OFF");
	}
	if (GetPause())
	{
		strcpy(&sPause[0], "F1:ポーズ画面非表示\n");
	}

	//テキストを代入
	sprintf(&aStr[0], "FPS:%d\nEDIT:%s\nSetBlock:%d\nEditNumber:%d(6↓/7↑)\n"
		"\nジャンプ力:%.2f(Q↓E↑)\n重力:%.2f(Z↓C↑)\ncol:r %.1f,g %.1f,b %.1f\nタイム:%3d:%2d.%2d\n"
		, g_nCountFPS,&aEdit,edit.nSetBlock,edit.nTagBlock+1,
		fJunpSpeed, fGravity,player.col.r,player.col.g,player.col.b,time.nTime/6000,time.nTime%6000 /100,time.nTime %100);

	sprintf(&aCtrl[0], "WASDキャラ行動\n↑/↓: posX : %4.1f\n←/→: posY %4.1f\nI/J: moveX:%2.1f\nShift I/J: moveY:%2.1f\nU/H: Range:%3.1f\nShift←/→: Width:%4.1f\nShift↑/↓:Height:%4.1f\n%s"
				,edit.pBlock->pos.x, edit.pBlock->pos.y, edit.pBlock->move.x, edit.pBlock->move.y, edit.pBlock->fRange, edit.pBlock->fWidth, edit.pBlock->fHeight/10, &sPause);

	//テキスト描画
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(0,128,255,255));
	
	//テキスト描画
	g_pFont->DrawText(NULL, &aCtrl[0], -1, &rect, DT_RIGHT, D3DCOLOR_RGBA(0,128,255,255));
}
