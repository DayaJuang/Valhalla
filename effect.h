//=============================================================================
//
// バレット処理 [effect.h]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

// バレット構造体
struct EFFECT
{
	BOOL					use;					// true:使っている  false:未使用
	float					w, h;					// 幅と高さ
	D3DXVECTOR3				pos;					// バレットの座標
	D3DXVECTOR3				rot;					// バレットの回転量
	D3DXVECTOR3				move;					// バレットの移動量
	int						countAnim;				// アニメーションカウント
	int						patternAnim;			// アニメーションパターンナンバー
	int						texNo;					// 何番目のテクスチャーを使用するのか

};

enum
{
	ATTACK_EFFECT,
	ENEMY_ATTACK_EFFECT,
	FIRE_EFFECT,
	HEAL_EFFECT,
	LIMIT_BREAK_EFFECT,

	EFFECT_MAX
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);

EFFECT *GetEffect(void);
void SetEffect(D3DXVECTOR3 pos, int texNo);

