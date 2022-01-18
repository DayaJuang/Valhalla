//=============================================================================
//
// バレット処理 [bullet.h]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BULLET_MAX		(1)			// バレットのMax数
#define BULLET_SPEED	(4.0f)		// バレットの移動スピード


// バレット構造体
struct BULLET
{
	BOOL					use;					// true:使っている  false:未使用
	float					w, h;					// 幅と高さ
	D3DXVECTOR3				pos;					// バレットの座標
	D3DXVECTOR3				targetPos;				// ターゲットの座標
	D3DXVECTOR3				rot;					// バレットの回転量
	int						countAnim;				// アニメーションカウント
	int						patternAnim;			// アニメーションパターンナンバー
	int						texNo;					// 何番目のテクスチャーを使用するのか

};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);

BULLET *GetBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 targetPos);

