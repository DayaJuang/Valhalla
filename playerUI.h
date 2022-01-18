//=============================================================================
//
// PlayerUI画面処理 [PlayerUI.h]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
struct PLAYERUI
{
	D3DXVECTOR3		pos;		// ポリゴンの座標
	float			w, h;		// 幅と高さ
	int				texNo;		// 使用しているテクスチャ番号

	BOOL			use;

};



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayerUI(void);
void UninitPlayerUI(void);
void UpdatePlayerUI(void);
void DrawPlayerUI(void);

PLAYERUI *GetPlayerUI(void);

