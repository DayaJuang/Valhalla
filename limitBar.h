//=============================================================================
//
// スコア処理 [bar.h]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define WIDTH_MAX					(120)

struct BAR
{
	D3DXVECTOR3		pos;		// ポリゴンの座標
	float			w, h;		// 幅と高さ
	int				texNo;		// 使用しているテクスチャ番号

	BOOL			use;
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBar(void);
void UninitBar(void);
void UpdateBar(void);
void DrawBar(void);

void AddLimitBar(int point);

BAR *GetBar(void);


