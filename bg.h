//=============================================================================
//
// BG画面処理 [bg.h]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
struct BG
{
	D3DXVECTOR3		pos;		// ポリゴンの座標
	float			w, h;		// 幅と高さ
	int				texNo;		// 使用しているテクスチャ番号
	float			r, g, b;	// BGのRGB

};

enum
{
	PASS,
	COLLISION,
	PLAYER_POS,
	TO_WORLDMAP,
	TO_WORLDMAP_NEXT_AREA,
	ENEMY_POS
};

enum
{
	FOREST,
	DUNGEON
};



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBG(void);
void UninitBG(void);
void UpdateBG(void);
void DrawBG(void);
int CheckMap(float x, float y);
int GetMapIndex(int y, int x);

BG *GetBG(void);

