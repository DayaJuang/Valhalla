//=============================================================================
//
// Main�w�b�_�[ [main.h]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#pragma once


#pragma warning(push)
#pragma warning(disable:4005)

#define _CRT_SECURE_NO_WARNINGS			// scanf ��warning�h�~
#include <stdio.h>
#include <time.h>		// �� ���Ԑ���n�̊֐��Q

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <d3dx9.h>

#define DIRECTINPUT_VERSION 0x0800		// �x���Ώ�
#include "dinput.h"
#include "mmsystem.h"

#pragma warning(pop)


#include "debugproc.h"


//*****************************************************************************
// ���C�u�����̃����N
//*****************************************************************************
#pragma comment (lib, "d3d11.lib")		
#pragma comment (lib, "d3dcompiler.lib")
#pragma comment (lib, "d3dx11.lib")	
#pragma comment (lib, "d3dx9.lib")	
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "dxerr.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SCREEN_WIDTH	(960)				// �E�C���h�E�̕�
#define SCREEN_HEIGHT	(540)				// �E�C���h�E�̍���
#define SCREEN_CENTER_X	(SCREEN_WIDTH / 2)	// �E�C���h�E�̒��S�w���W
#define SCREEN_CENTER_Y	(SCREEN_HEIGHT / 2)	// �E�C���h�E�̒��S�x���W

#define	NUM_VERTEX		(4)					// ���_��

enum
{
	MODE_TITLE = 0,							// �^�C�g�����
	MODE_TUTORIAL,							// �Q�[���������
	MODE_WORLDMAP,							// �Q�[���������
	MODE_GAME,								// �Q�[�����
	MODE_GAME_OVER,							// �Q�[���I�[�o�[���
	MODE_GAME_CLEAR,
	MODE_END
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
long GetMousePosX(void);
long GetMousePosY(void);
char *GetDebugStr(void);

void SetMode(int mode);
int GetMode(void);

int checkCurrentMap(void);

void SetLoadGame(BOOL flg);

