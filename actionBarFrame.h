//=============================================================================
//
// スコア処理 [ActionBarFrame.h]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

struct ACTIONBARFRAME
{
	D3DXVECTOR3		pos;		// ポリゴンの座標
	float			w, h;		// 幅と高さ
	int				texNo;		// 使用しているテクスチャ番号

	BOOL			use;
};

enum
{
	PLAYER1_FRAME,
	ENEMY1_FRAME
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitActionBarFrame(void);
void UninitActionBarFrame(void);
void UpdateActionBarFrame(void);
void DrawActionBarFrame(void);

ACTIONBARFRAME *GetActionBarFrame(int index);


