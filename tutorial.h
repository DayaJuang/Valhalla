//=============================================================================
//
// スコア処理 [Tutorial.h]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FIELD_TUTORIAL_MAX			(2)
#define BATTLE_TUTORIAL_MAX			(3)
#define WORLDMAP_TUTORIAL_MAX		(1)
#define BATTLE_TUTORIAL_MAXINDEX	(5)

struct TUTORIAL
{
	D3DXVECTOR3		pos;		// ポリゴンの座標
	float			w, h;		// 幅と高さ
	int				texNo;		// 使用しているテクスチャ番号
	int				flag;

	BOOL			use;
};

enum
{
	FIELD_TUTORIAL1,
	FIELD_TUTORIAL2,
	BATTLE_TUTORIAL1,
	BATTLE_TUTORIAL2,
	BATTLE_TUTORIAL3,
	WORLDMAP_TUTORIAL1
};

enum
{
	FIELD_TUTORIAL,
	BATTLE_TUTORIAL,
	WORLDMAP_TUTORIAL
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTutorial(void);
void UninitTutorial(void);
void UpdateTutorial(void);
void DrawTutorial(void);
void ShowTutorial(int texNo, int flag);

TUTORIAL *GetTutorial(void);


