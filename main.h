//=============================================================================
//
// Mainヘッダー [main.h]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#pragma once


#pragma warning(push)
#pragma warning(disable:4005)

#define _CRT_SECURE_NO_WARNINGS			// scanf のwarning防止
#include <stdio.h>
#include <time.h>		// ← 時間制御系の関数群

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <d3dx9.h>

#define DIRECTINPUT_VERSION 0x0800		// 警告対処
#include "dinput.h"
#include "mmsystem.h"

#pragma warning(pop)


#include "debugproc.h"


//*****************************************************************************
// ライブラリのリンク
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
// マクロ定義
//*****************************************************************************
#define SCREEN_WIDTH	(960)				// ウインドウの幅
#define SCREEN_HEIGHT	(540)				// ウインドウの高さ
#define SCREEN_CENTER_X	(SCREEN_WIDTH / 2)	// ウインドウの中心Ｘ座標
#define SCREEN_CENTER_Y	(SCREEN_HEIGHT / 2)	// ウインドウの中心Ｙ座標

#define	NUM_VERTEX		(4)					// 頂点数

enum
{
	MODE_TITLE = 0,							// タイトル画面
	MODE_TUTORIAL,							// ゲーム説明画面
	MODE_WORLDMAP,							// ゲーム説明画面
	MODE_GAME,								// ゲーム画面
	MODE_GAME_OVER,							// ゲームオーバー画面
	MODE_GAME_CLEAR,
	MODE_END
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
long GetMousePosX(void);
long GetMousePosY(void);
char *GetDebugStr(void);

void SetMode(int mode);
int GetMode(void);

int checkCurrentMap(void);

void SetLoadGame(BOOL flg);

