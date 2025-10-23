//==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*
// 
// �V���[�e�B���O [main.cpp]
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

//�}�N����`
#define CLASS_NAME	"WindowClass"			//�E�B���h�E�N���X�̖��O
#define WINDOW_NAME	"�A�N�V����"			//�E�B���h�E�̖��O�i�L���v�V�����j

//�v���g�^�C�v�錾
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uinit(void);
void Update(void);
void Draw(void);
void DrawDebug(void);

//�O���[�o���ϐ��錾
LPDIRECT3D9 g_pD3D = NULL;					//DirectX3D�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;		//DirectX3D�f�o�C�X�ւ̃|�C���^
MODE g_mode = MODE_TITLE;					//���݂̃��[�h
LPD3DXFONT g_pFont = NULL;					//�t�H���g�̃|�C���^
int g_nCountFPS = 0;						//FPS�J�E���^�[

//=======================================
// ���C���֐�
//=======================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	DWORD dwCurrentTime;						//���ݎ���
	DWORD dwExecLastTime;						//�Ō�ɏ�����������
	DWORD dwFramCount;							//�t���[���J�E���g
	DWORD dwFPSLastTime;						//�Ō��FPS���v����������

	//�����_���V�[�h�̐���
	srand(timeGetTime());

	//��ʃT�C�Y�̍\����
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	//�f�o�b�O�\���p�ϐ��̏�����
	dwFramCount = 0;
	dwFPSLastTime = timeGetTime();

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),						//WNDCLASSEX�̃������T�C�Y
		CS_CLASSDC,								//�E�B���h�E�̃X�^�C��
		WindowProc,								//�E�B���h�E�v���V�[�W��
		0,										//�ʏ�͎g�p���Ȃ�
		0,										//����
		hInstance,								//�C���X�^���X�n���h��
		LoadIcon(NULL, IDI_APPLICATION),		//�^�X�N�o�[�̃A�C�R���i����\
		LoadCursor(NULL,IDC_ARROW),				//�J�[�\���i����\
		(HBRUSH)(COLOR_WINDOW + 1),				//�w�i�F
		NULL,									//���j���[�o�[
		CLASS_NAME,								//�E�B���h�E�N���X�̖��O
		LoadIcon(NULL,IDI_APPLICATION),			//�t�@�C����̃A�C�R��
	};

	HWND hWnd;
	MSG  msg;

	//�E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	//�N���C�A���g�̈���w��T�C�Y�ɒ���
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//�E�B���h�E�𐶐�
	hWnd = CreateWindowEx(0,					//�g���E�B���h�E�X�^�C��
		CLASS_NAME,								//�E�B���h�E�N���X�̖��O
		WINDOW_NAME,							//�E�B���h�E�̖��O
		WS_OVERLAPPEDWINDOW,					//�E�B���h�E�X�^�C��
		CW_USEDEFAULT,							//����X���W
		CW_USEDEFAULT,							//����Y���W
		(rect.right - rect.left),				//��
		(rect.bottom - rect.top),				//����
		NULL,									//�e�E�B���h�E�̃n���h���i���̃E�B���h�E���ŏ�ʂȂ̂łȂ�
		NULL,									//���j���[�n���h���i�q�E�B���h�EID
		hInstance,								//�C���X�^���X�n���h��
		NULL);									//�E�B���h�E�쐬�f�[�^

	//����������
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{//�������Ɏ��s������
		return -1;
	}

	//����\��ݒ�
	timeBeginPeriod(1);
	dwCurrentTime = 0;
	dwExecLastTime = timeGetTime();

	//�E�B���h�E�\��
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//���b�Z�[�W���[�v
	while(1)
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE) != 0)
		{//window�̏���
			if (msg.message == WM_QUIT)
			{//WM_QUIT���b�Z�[�W���󂯎�����烋�[�v�𔲂���
				break;
			}
			else
			{
				//���b�Z�[�W�̐ݒ�
				TranslateMessage(&msg);
				DispatchMessage(&msg);				
			}
		}
		else
		{//DirectX�̏���
			dwCurrentTime = timeGetTime();
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5�b
				//FPS���v��
				g_nCountFPS = (dwFramCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;		//FPS���v������������ۑ�
				dwFramCount = 0;					//�J�E���g�N���A
			}

			if (dwCurrentTime - dwExecLastTime >= 1000 / 60)
			{//1/60�b
					dwExecLastTime = dwCurrentTime;

					//�X�V����
					Update();

					//�`�揈��
					Draw();

					//�t���[���J�E���g
					dwFramCount++;
			}
		}
	}
	//�E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//=======================================
// �E�B���h�E�v���V�[�W��
//=======================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;		//�Ԃ�l�p

	switch (uMsg)
	{
	case WM_DESTROY:// > �E�B���h�E�j���̃��b�Z�[�W�Ȃ�
			//WM_QUIT���b�Z�[�W�𑗂�
			PostQuitMessage(0);
		break;// > DESTROY

	case WM_KEYDOWN:// > �L�[�������ꂽ��
		switch (wParam)
		{
		case VK_ESCAPE:	// >> [ESC]�L�[�Ȃ�
			//WM_DESTROY���b�Z�[�W�𑗂�
			nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO);
			if (nID == IDYES)
				//�E�B���h�E�j�����b�Z�[�W
				DestroyWindow(hWnd);
			break;// >> ESC
		}
		break;// > KEYDOWN
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//=======================================
// ����������
//=======================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			//�f�B�X�v���C���[�h
	D3DPRESENT_PARAMETERS d3dpp;	//�v���[���e�[�V�����p�����[�^

	//Direct3D�I�u�W�F�N�g�̐���
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//�f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp)); //�p�����[�^�̃[���N���A

	d3dpp.BackBufferWidth = SCREEN_WIDTH;							//�Q�[����ʂ̕�
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;							//����
	d3dpp.BackBufferFormat = d3ddm.Format;							//�o�b�N�o�b�t�@�̌`��
	d3dpp.BackBufferCount = 1;										//		�V		�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;						//�_�u���o�b�t�@�̐؂�ւ��i�f���M���ɓ���
	d3dpp.EnableAutoDepthStencil = TRUE;							//�f�v�X�o�b�t�@�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;						//�f�v�X�o�b�t�@�Ƃ���16bit���g��	
	d3dpp.Windowed = bWindow;										//�E�B���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		//���t���b�V�����[�g
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		//�C���^�[�o��

	//Direct3D�f�o�C�X�̐���
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{//�`��E���_�������n�[�h�E�F�A�łł��Ȃ��Ȃ�
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{//���_������CPU�ɂ��Ă����߂Ȃ�
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{//�`���CPU�ɂ��ă_���Ȃ�
				return E_FAIL;//������߂āH
			}
		}
	}	

	//�����_�[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);

	//�T���v���[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);			//�g�k�⊮
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);			//	�V
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);			//�e�N�X�`���J��Ԃ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);			//	�V

	//�e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);			//�e�N�X�`���A���t�@�u�����h�̐ݒ�
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//�L�[�{�[�h�̏�����
	if (FAILED(InitKeyboard(hInstance, hWnd)))
		return E_FAIL;	//���s������I��

	//�W���C�p�b�h�̏�����
	if (FAILED(InitJoypad()))
		return E_FAIL;	//���s������I��

	//�f�o�b�O�\���p�t�H���g�̐���
	D3DXCreateFont(g_pD3DDevice,18,0,0,0,FALSE,SHIFTJIS_CHARSET,
					OUT_DEFAULT_PRECIS,DEFAULT_QUALITY,
					DEFAULT_PITCH,"Terminal",&g_pFont);

	//�T�E���h�̏�����
	//InitSound(hWnd);

	//�t�F�[�h������
	InitFade();

	//���[�h�̐ݒ�
	SetMode(g_mode);

	return S_OK;
}

//=======================================
// �I������
//=======================================
void Uinit(void)
{
	//�T�E���h��~�E�I��
	//StopSound();
	//UninitSound();

	//�^�C�g���I��
	UninitTitle();

	//�Q�[����ʏI��
	UninitGame();

	//���U���g�I��
	UninitResult();

	//�t�F�[�h�I��
	UninitFade();

	//�L�[�{�[�h�I��
	UninitKeyboard();	

	//�W���C�p�b�h�I��
	UninitJoypad();

	//�f�o�b�O�\���p�t�H���g�̔j��
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}

	//Direct3D�f�o�C�X�̔j��
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	//Direct3D�I�u�W�F�N�g�̔j��
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}

	//����\��߂�
	timeEndPeriod(1);
}

//=======================================
// �X�V����
//=======================================
void Update(void)
{
	UpdateFade();			//�t�F�[�h�X�V
	UpdateKeyboard();		//�L�[�{�[�h�X�V
	UpdateJoypad();			//�W���C�p�b�h�X�V
	

	switch (g_mode)
	{
	case MODE_TITLE:		//�^�C�g��
		UpdateTitle();
		break;
	case MODE_GAME:			//�Q�[�����
		UpdateGame();
		break;
	default:				//��O
		break;
	}

}

//=======================================
// �`�揈��
//=======================================
void Draw(void)
{
	//��ʃN���A�i�o�b�N�o�b�t�@��Z�o�b�t�@�̃N���A
	g_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	//�`��J�n
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{//�e��`��

		switch (g_mode)
		{
		case MODE_TITLE:		//�^�C�g��
			DrawTitle();
			break;
		case MODE_GAME:			//�Q�[�����
			DrawGame();
			break;
		default:				//��O
			break;
		}
		//�t�F�[�h�`��
		DrawFade();
	}

#ifdef _DEBUG

	//�f�o�b�O�\��
	DrawDebug();

#endif

	//�`��I��
	g_pD3DDevice->EndScene();
	
	//�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=======================================
// ���[�h�̐ݒ�
//=======================================
void SetMode(MODE mode)
{

	switch (g_mode)
	{
	case MODE_TITLE:		//�^�C�g��
		UninitTitle();
		break;

	case MODE_GAME:			//�Q�[�����
		UninitGame();
		break;

	default:				//��O
		break;
	}

	switch (mode)
	{
	case MODE_TITLE:		//�^�C�g��
		InitTitle();
		break;

	case MODE_GAME:			//�Q�[�����
		InitGame();
		break;

	default:				//��O
		break;
	}

	//���[�h�̕ۑ�
	g_mode = mode;
}

//=======================================
// ���[�h�̎擾
//=======================================
MODE GetMode(void)
{
	return g_mode;
}

//=======================================
// �f�o�C�X�̎擾�i����cpp�t�@�C���ł�D3DDevice���Ăяo����悤�ɂ���
//=======================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//=======================================
// �f�o�b�O�`��
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
		strcpy(&sPause[0], "F1:�|�[�Y��ʔ�\��\n");
	}

	//�e�L�X�g����
	sprintf(&aStr[0], "FPS:%d\nEDIT:%s\nSetBlock:%d\nEditNumber:%d(6��/7��)\n"
		"\n�W�����v��:%.2f(Q��E��)\n�d��:%.2f(Z��C��)\ncol:r %.1f,g %.1f,b %.1f\n�^�C��:%3d:%2d.%2d\n"
		, g_nCountFPS,&aEdit,edit.nSetBlock,edit.nTagBlock+1,
		fJunpSpeed, fGravity,player.col.r,player.col.g,player.col.b,time.nTime/6000,time.nTime%6000 /100,time.nTime %100);

	sprintf(&aCtrl[0], "WASD�L�����s��\n��/��: posX : %4.1f\n��/��: posY %4.1f\nI/J: moveX:%2.1f\nShift I/J: moveY:%2.1f\nU/H: Range:%3.1f\nShift��/��: Width:%4.1f\nShift��/��:Height:%4.1f\n%s"
				,edit.pBlock->pos.x, edit.pBlock->pos.y, edit.pBlock->move.x, edit.pBlock->move.y, edit.pBlock->fRange, edit.pBlock->fWidth, edit.pBlock->fHeight/10, &sPause);

	//�e�L�X�g�`��
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(0,128,255,255));
	
	//�e�L�X�g�`��
	g_pFont->DrawText(NULL, &aCtrl[0], -1, &rect, DT_RIGHT, D3DCOLOR_RGBA(0,128,255,255));
}
