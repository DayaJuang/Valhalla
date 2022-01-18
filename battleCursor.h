//=============================================================================
//
// BG画面処理 [battleCursor.h]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CURSOR_MAX_POSITION		(3)
#define POWER_UP_MAX_POSITION	(4)

struct BATTLECURSOR
{
	D3DXVECTOR3		pos[CURSOR_MAX_POSITION];			// ポリゴンの座標
	D3DXVECTOR3		powerUpPos[POWER_UP_MAX_POSITION];	// 強化用のポリゴンの座標
	float			w, h;		// 幅と高さ
	int				texNo;		// 使用しているテクスチャ番号
	int				mode;
	int				posIndex;

	BOOL			canDoAction;
};

enum
{
	HP_UP,
	MP_UP,
	ATK_UP,
	RECOVERALL
};

enum {
	MENU_SELECT,
	ATTACK_ENEMY,
	USE_FIREBALL,
	HEAL_ALLY,
	POWERUP_SELECT
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBattleCursor(void);
void UninitBattleCursor(void);
void UpdateBattleCursor(void);
void DrawBattleCursor(void);

void SelectTargetEnemy(void);
void SelectAlly(void);

BATTLECURSOR *GetBattleCursor(void);

