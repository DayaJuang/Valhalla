//=============================================================================
//
// Main���� [main.cpp]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "camera.h"
#include "input.h"
#include "title.h"
#include "bg.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "score.h"
#include "result.h"
#include "sound.h"
#include "fade.h"
#include "file.h"
#include "layer.h"
#include "worldmap.h"
#include "iconWM.h"
#include "cursorWM.h"
#include "battleUI.h"
#include "battleCursor.h"
#include "damage.h"
#include "status.h"
#include "playerUI.h"
#include "battle.h"
#include "effect.h"
#include "bubble.h"
#include "limitBar.h"
#include "ultimateText.h"
#include "battleLog.h"
#include "actionBar.h"
#include "actionBarFrame.h"
#include "tutorial.h"
#include "orb.h"
#include "gameClear.h"
#include "waterfall.h"
#include "titleText.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CLASS_NAME			"AppClass"				// �E�C���h�E�̃N���X��
#define WINDOW_NAME			"Valhalla"		// �E�C���h�E�̃L���v�V������

//*****************************************************************************
// �\���̒�`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);

void CheckGameClear(void);


//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************
long g_MouseX = 0;
long g_MouseY = 0;


#ifdef _DEBUG
int		g_CountFPS;							// FPS�J�E���^
char	g_DebugStr[2048] = WINDOW_NAME;		// �f�o�b�O�����\���p

#endif

int g_Mode = MODE_TITLE;					// �N�����̉�ʂ�ݒ�

BOOL g_LoadGame = FALSE;					// NewGame

//=============================================================================
// ���C���֐�
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//UNREFERENCED_PARAMETER(hPrevInstance);	// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j
	//UNREFERENCED_PARAMETER(lpCmdLine);		// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j

	// ���Ԍv���p
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;
	
	WNDCLASSEX	wcex = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW+1),
		NULL,
		CLASS_NAME,
		NULL
	};
	HWND		hWnd;
	MSG			msg;
	
	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �E�B���h�E�̍쐬
	hWnd = CreateWindow(CLASS_NAME,
						WINDOW_NAME,
						WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT,																		// �E�B���h�E�̍����W
						CW_USEDEFAULT,																		// �E�B���h�E�̏���W
						SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME)*2,									// �E�B���h�E����
						SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME)*2+GetSystemMetrics(SM_CYCAPTION),	// �E�B���h�E�c��
						NULL,
						NULL,
						hInstance,
						NULL);

	// �E�B���h�E���[�h���t���X�N���[�����[�h���̏���
	bool mode = true;

	int id = MessageBox(NULL, "Window���[�h�Ńv���C���܂����H", "�N�����[�h", MB_YESNOCANCEL | MB_ICONQUESTION);
	switch (id)
	{
	case IDYES:		// Yes�Ȃ�Window���[�h�ŋN��
		mode = true;
		break;
	case IDNO:		// No�Ȃ�t���X�N���[�����[�h�ŋN��
		mode = false;
		break;
	case IDCANCEL:	// CANCEL�Ȃ�I��
	default:
		return -1;
		break;
	}

	// DirectX�̏�����(�E�B���h�E���쐬���Ă���s��)
	if(FAILED(Init(hInstance, hWnd, mode)))
	{
		return -1;
	}

	// �t���[���J�E���g������
	timeBeginPeriod(1);	// ����\��ݒ�
	dwExecLastTime = dwFPSLastTime = timeGetTime();	// �V�X�e���������~���b�P�ʂŎ擾
	dwCurrentTime = dwFrameCount = 0;
	
	// �E�C���h�E�̕\��(Init()�̌�ɌĂ΂Ȃ��Ƒʖ�)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	
	// ���b�Z�[�W���[�v
	while(1)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{// PostQuitMessage()���Ă΂ꂽ�烋�[�v�I��
				break;
			}
			else
			{
				// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();					// �V�X�e���������擾

			if ((dwCurrentTime - dwFPSLastTime) >= 1000)	// 1�b���ƂɎ��s
			{
#ifdef _DEBUG
				g_CountFPS = dwFrameCount;
#endif
				dwFPSLastTime = dwCurrentTime;				// FPS�𑪒肵��������ۑ�
				dwFrameCount = 0;							// �J�E���g���N���A
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))	// 1/60�b���ƂɎ��s
			{
				dwExecLastTime = dwCurrentTime;	// ��������������ۑ�

#ifdef _DEBUG	// �f�o�b�O�ł̎�����FPS��\������
				wsprintf(g_DebugStr, WINDOW_NAME);
				wsprintf(&g_DebugStr[strlen(g_DebugStr)], " FPS:%d", g_CountFPS);
#endif

				Update();			// �X�V����
				Draw();				// �`�揈��

#ifdef _DEBUG	// �f�o�b�O�ł̎������\������
				wsprintf(&g_DebugStr[strlen(g_DebugStr)], " MX:%d MY:%d", GetMousePosX(), GetMousePosY());
				SetWindowText(hWnd, g_DebugStr);
#endif

				dwFrameCount++;		// �����񐔂̃J�E���g�����Z
			}
		}
	}
	
	timeEndPeriod(1);				// ����\��߂�

	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// �I������
	Uninit();

	return (int)msg.wParam;
}

//=============================================================================
// �v���V�[�W��
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch( message )
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:					// [ESC]�L�[�������ꂽ
			DestroyWindow(hWnd);		// �E�B���h�E��j������悤�w������
			break;
		}
		break;

	case WM_MOUSEMOVE:
		g_MouseX = LOWORD(lParam);
		g_MouseY = HIWORD(lParam);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// �����_�����O�����̏�����
	InitRenderer(hInstance, hWnd, bWindow);

	// �J���������̏�����
	InitCamera();

	// ���͏����̏�����
	InitInput(hInstance, hWnd);

	// �T�E���h�����̏�����
	InitSound(hWnd);

	// �t�F�[�h�����̏�����
	InitFade();

	// �ŏ��̃��[�h���Z�b�g���āA���̒���Init����
	SetMode(g_Mode);	// ������SetMode�̂܂܂ŁI

	return S_OK;
}


//=============================================================================
// �I������
//=============================================================================
void Uninit(void)
{
	// �Q�[���I�ȕ��͂��̒���Uninit����
	SetMode(MODE_END);	// ������SetMode�̂܂܂ŁI

	// �t�F�[�h�̏I������
	UninitFade();

	// �T�E���h�̏I������
	UninitSound();

	// ���͂̏I������
	UninitInput();

	// �J�����̏I������
	UninitCamera();

	// �����_�����O�̏I������
	UninitRenderer();

}

//=============================================================================
// �X�V����
//=============================================================================
void Update(void)
{
	// ���͂̍X�V����
	UpdateInput();

	// �J�����̍X�V����
	UpdateCamera();

	// ���[�h�ɂ���ď����𕪂���
	switch (g_Mode)
	{
	case MODE_TITLE:		// �^�C�g����ʂ̍X�V
		UpdateTitle();
		UpdateWaterfall();
		UpdateTitleText();
		break;

	case MODE_WORLDMAP:		// ���[�h�}�b�v��ʂ̍X�V
		UpdateWM();
		UpdateIconWM();
		UpdateCursorWM();
		UpdateTutorial();
		break;

	case MODE_GAME:			// �Q�[����ʂ̍X�V
		UpdateBG();
		UpdateBubble();
		UpdatePlayer();
		UpdateEnemy();
		UpdateOrb();
		UpdateLayer();
		UpdateBullet();
		UpdateScore();
		UpdateDamage();
		UpdateBattleUI();
		UpdateBattleLog();
		UpdateUltimateText();
		UpdateBattleCursor();
		UpdatePlayerUI();
		UpdateStatus();
		UpdateBar();
		UpdateEffect();
		UpdateBattle();
		UpdateActionBar();
		UpdateActionBarFrame();
		UpdateTutorial();
		break;

	case MODE_GAME_OVER:	// �Q�[���I�[�o�[��ʂ̍X�V
		UpdateResult();
		break;

	case MODE_GAME_CLEAR:	// �Q�[���N���A��ʂ̍X�V
		UpdateGameClear();
		break;

	}

	UpdateFade();			// �t�F�[�h�̍X�V����
}

//=============================================================================
// �`�揈��
//=============================================================================
void Draw(void)
{
	// �o�b�N�o�b�t�@�N���A
	Clear();

	// �J�������Z�b�g
	SetCamera();

	// 2D�`��Ȃ̂Ő[�x����
	SetDepthEnable(false);
	
	// ���[�h�ɂ���ď����𕪂���
	switch (g_Mode)
	{
	case MODE_TITLE:		// �^�C�g����ʂ̕`��
		DrawTitle();
		DrawWaterfall();
		DrawTitleText();
		break;

	case MODE_WORLDMAP:
		// ���[�h�}�b�v��ʂ̕`��
		DrawWM();
		DrawIconWM();
		DrawCursorWM();
		DrawTutorial();
		break;

	case MODE_GAME:			// �Q�[����ʂ̕`��
		DrawBG();
		DrawBullet();
		DrawEnemy();
		DrawOrb();
		DrawPlayer();
		DrawBubble();
		DrawLayer();
		DrawDamage();
		DrawBattleUI();
		DrawBattleLog();
		DrawUltimateText();
		DrawBattleCursor();
		DrawPlayerUI();
		DrawStatus();
		DrawBar();
		DrawEffect();
		DrawBattle();
		DrawActionBarFrame();
		DrawActionBar();
		DrawTutorial();
		break;

	case MODE_GAME_OVER:		// �Q�[���I�[�o�[��ʂ̕`��
		DrawResult();
		break;

	case MODE_GAME_CLEAR:		// �Q�[���N���A��ʂ̕`��
		DrawGameClear();
		break;

	}

	DrawFade();				// �t�F�[�h��ʂ̕`��


#ifdef _DEBUG
	// �f�o�b�O�\��
	DrawDebugProc();
#endif

	// �o�b�N�o�b�t�@�A�t�����g�o�b�t�@����ւ�
	Present();

}


//=============================================================================
// ���[�h�̐ݒ�
//=============================================================================
void SetMode(int mode)
{
	// ���[�h��ς���O�ɑS������������������Ⴄ
	StopSound();			// �܂��Ȃ��~�߂�

	// �^�C�g���Ɋւ���I������
	UninitTitle();
	UninitWaterfall();
	UninitTitleText();

	// BG�̏I������
	UninitBG();

	// ���[�h�}�b�v��ʂ̏I������
	UninitWM();

	// �A�C�R���̏I������
	UninitIconWM();

	// �J�[�\���̏I������
	UninitCursorWM();

	// �e�L�X�g�̏I������
	UninitUltimateText();

	// �o�g��UI�̏I������
	UninitBattleUI();

	// �o�g�����O�̏I������
	UninitBattleLog();

	// �_���[�W�����̏I������
	UninitDamage();

	// �o�[�u���̏I������
	UninitBubble();

	// �o�g���p�J�[�\���̏I������
	UninitBattleCursor();

	// �v���C���[�X�e�[�^�XUI�̏I������
	UninitPlayerUI();

	// HP��MP�\�������̏I������
	UninitStatus();

	// �G�t�F�N�g�̏I������
	UninitEffect();

	// �v���C���[�̏I������
	UninitPlayer();

	// �G�l�~�[�̏I������
	UninitEnemy();

	// �I�[�u�̏I������
	UninitOrb();

	// �o���b�g�̏I������
	UninitBullet();

	// �X�R�A�̏I������
	UninitScore();

	// �o�[�̏I������
	UninitBar();

	// �Q�[���I�[�o�[�̏I������
	UninitResult();

	// �o�g���̏I������
	UninitBattle();

	// �A�N�V�����o�[�Ɋւ���I������
	UninitActionBar();
	UninitActionBarFrame();

	// �`���[�g���A���̏I������
	UninitTutorial();

	// �Q�[���N���A�̏I������
	UninitGameClear();

	g_Mode = mode;	// ���̃��[�h���Z�b�g���Ă���

	switch (g_Mode)
	{
	case MODE_TITLE:
		// �^�C�g����ʂ̏�����
		InitTitle();
		InitWaterfall();
		InitTitleText();
		PlaySound(SOUND_LABEL_BGM_title);
		break;

	case MODE_WORLDMAP:
		// ���[�h�}�b�v��ʂ̏�����
		InitWM();
		InitIconWM();
		InitCursorWM();
		InitTutorial();
		PlaySound(SOUND_LABEL_BGM_worldmap);
		break;

	case MODE_GAME:
		// �Q�[����ʂ̏�����
		InitBG();
		InitPlayer();
		InitBubble();
		InitEnemy();
		InitOrb();
		InitLayer();
		InitBullet();
		InitScore();
		InitDamage();
		InitBattleUI();
		InitBattleLog();
		InitUltimateText();
		InitBattleCursor();
		InitPlayerUI();
		InitStatus();
		InitBar();
		InitEffect();
		InitBattle();
		InitActionBar();
		InitActionBarFrame();
		InitTutorial();

		// ���[�h�Q�[���������炷�ׂĂ̏��������I�������ɃZ�[�u�f�[�^��ǂݍ���
		if (g_LoadGame == TRUE)
		{
			LoadData();
			g_LoadGame = FALSE;		// ���[�h��������t���O��Clear����
		}

		//PlaySound(SOUND_LABEL_BGM_sample001);
		break;

	case MODE_GAME_OVER:
		// �Q�[���I�[�o�[��ʂ̏�����
		InitResult();
		PlaySound(SOUND_LABEL_SE_gameover);
		break;

	case MODE_GAME_CLEAR:
		// �Q�[���N���A��ʂ̏�����
		InitGameClear();
		PlaySound(SOUND_LABEL_BGM_gameclear);
		break;

	case MODE_END:
		// �Q�[���I�����̏���
		StopSound();
		break;
	}
}

//=============================================================================
// ���[�h�̎擾
//=============================================================================
int GetMode(void)
{
	return g_Mode;
}


//=============================================================================
// �}�E�X���W�̎擾
//=============================================================================
long GetMousePosX(void)
{
	return g_MouseX;
}


long GetMousePosY(void)
{
	return g_MouseY;
}


#ifdef _DEBUG
char *GetDebugStr(void)
{
	return g_DebugStr;
}
#endif

int checkCurrentMap(void)
{
	PLAYER *player = GetPlayer();

	return player->currentLocation;
}


//=============================================================================
// �j���[�Q�[�������[�h�Q�[�������Z�b�g����
//=============================================================================
void SetLoadGame(BOOL flg)
{
	g_LoadGame = flg;
}





