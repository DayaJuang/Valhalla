//=============================================================================
//
// スコア処理 [bubble.h]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
struct BUBBLE
{
	D3DXVECTOR3		pos;		// ポリゴンの座標
	float			w, h;		// 幅と高さ
	int				texNo;		// 使用しているテクスチャ番号

	BOOL			use;
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBubble(void);
void UninitBubble(void);
void UpdateBubble(void);
void DrawBubble(void);

void SetBubble(D3DXVECTOR3 targetPos, int textNo);

BUBBLE *GetBubble(void);


