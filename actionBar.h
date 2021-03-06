//=============================================================================
//
// スコア処理 [ActionBar.h]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ACTION_BAR_MAX (100)

struct ACTIONBAR
{
	D3DXVECTOR3		pos;		// ポリゴンの座標
	float			w, h;		// 幅と高さ
	int				texNo;		// 使用しているテクスチャ番号

	BOOL			use;
};

enum
{
	PLAYER1_BAR,
	ENEMY1_BAR
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitActionBar(void);
void UninitActionBar(void);
void UpdateActionBar(void);
void DrawActionBar(void);

