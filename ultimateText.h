//=============================================================================
//
// スコア処理 [UltimateText.h]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
struct UTEXT
{
	D3DXVECTOR3		pos;		// ポリゴンの座標
	float			w, h;		// 幅と高さ
	int				texNo;		// 使用しているテクスチャ番号
	int				countAnim;
	int				patternAnim;

	BOOL			use;
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitUltimateText(void);
void UninitUltimateText(void);
void UpdateUltimateText(void);
void DrawUltimateText(void);

UTEXT *GetUltimateText(void);


