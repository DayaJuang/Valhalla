//=============================================================================
//
// エネミー処理 [orb.h]
// Author : 
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define		ORB_MAX		(1)
#define		ORB_POS		(10)
//*****************************************************************************
// 構造体定義
//*****************************************************************************

struct ORB
{
	D3DXVECTOR3		pos;			// ポリゴンの座標
	D3DXVECTOR3		rot;			// ポリゴンの回転量
	D3DXVECTOR3		scl;			// ポリゴンの拡大率
	BOOL			use;			// true:使っている  false:未使用
	float			w, h;			// 幅と高さ
	int				countAnim;		// アニメーションカウント
	int				patternAnim;	// アニメーションパターンナンバー
	int				texNo;			// テクスチャ番号
};



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitOrb(void);
void UninitOrb(void);
void UpdateOrb(void);
void DrawOrb(void);
D3DXVECTOR3 FindOrbPosition(void);

ORB *GetOrb(void);



