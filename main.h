//=============================================================================
//
// ���C���w�b�_�[ [main.h]
// Author : ENDO HIDETO
//
//=============================================================================
#ifndef _MAIN_H_							//���̃}�N������`����ĂȂ�������
#define _MAIN_H_							//2�d�C���N���[�h�h�~�p�}�N��

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "d3dx9.h"
#define DIRECTINPUT_VERSION (0x0800)		//�r���h���̌x���Ώ��p
#include "dinput.h"							//���͏����ɕK�v
#include "xaudio2.h"						//�T�E���h�����ɕK�v
#include "xinput.h"							//�W���C�p�b�h�����ɕK�v

//���C�u�����̃����N
#pragma comment (lib,"d3d9.lib")
#pragma comment (lib,"d3dx9.lib")
#pragma comment (lib,"dxguid.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "dinput8.lib")		//���͏����p
#pragma comment (lib, "xinput.lib")			//�W���C�L�[�����ɕK�v

//�}�N����`
#define SCREEN_WIDTH (1280)					//�E�B���h�E�̕�
#define SCREEN_HEIGHT (720)					//�E�B���h�E�̍���
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)		//���_�t�H�[�}�b�g

//��ʑJ�ځi���[�h�j�̎��
typedef enum
{
	MODE_TITLE = 0,		//�^�C�g��
	MODE_SELECT,		//�X�e�[�W�Z���N�g
	MODE_GAME,			//�Q�[��
	MODE_MAX
}MODE;

//���_���[2D]�̍\���̒�`
typedef struct
{
	D3DXVECTOR3 pos;		//���_���W
	float rhw;				//���W�ϊ��p�W��
	D3DCOLOR col;			//�J���[
	D3DXVECTOR2 tex;		//�e�N�X�`�����W
}VERTEX_2D;

//�v���g�^�C�v�錾
LPDIRECT3DDEVICE9 GetDevice(void);	//�f�o�C�X�̎擾
void SetMode(MODE mode);			//���[�h�̐ݒ�
MODE GetMode(void);					//���[�h�̎擾

#endif // !_MAIN_H_
