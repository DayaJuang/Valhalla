//=============================================================================
//
// BattleUI画面処理 [battleUI.h]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
struct BATTLEUI
{
	D3DXVECTOR3		pos;		// ポリゴンの座標
	float			w, h;		// 幅と高さ
	int				texNo;		// 使用しているテクスチャ番号

	BOOL			use;

};

enum
{
	BATTLE_UI_LEFT,
	BATTLE_UI_RIGHT,
	POWER_UP_PANEL
};

enum
{
	BATTLE_UI,
	MAGIC_UI,
	POWERUP_UI
};



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBattleUI(void);
void UninitBattleUI(void);
void UpdateBattleUI(void);
void DrawBattleUI(void);
void ShowBattleUI(int texNo);

BATTLEUI *GetBattleUI(void);

