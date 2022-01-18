//=============================================================================
//
// スコア処理 [damage.h]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define DAMAGE_MAX			(999)		// スコアの最大値

#define	DAMAGE_DX			(500.0f)	// 表示位置
#define	DAMAGE_DY			(20.0f)		// 
#define	DAMAGE_DW			(16.0f)		// 
#define	DAMAGE_DH			(32.0f)		// 

struct DAMAGE
{
	D3DXVECTOR3		pos;		// ポリゴンの座標
	float			w, h;		// 幅と高さ
	int				texNo;		// 使用しているテクスチャ番号

	int				damage;		// 得点
	int				digit;

	float			alpha;

	BOOL			use;
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitDamage(void);
void UninitDamage(void);
void UpdateDamage(void);
void DrawDamage(void);

int GetDamage(void);
void SetDamageDigit(int damage);
void SetDamagePosition(D3DXVECTOR3 targetPos);
void SetDamage(int Damage);
void ShowDamage(void);

DAMAGE *GetDamageAddress(void);


