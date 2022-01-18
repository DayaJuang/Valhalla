//=============================================================================
//
// スコア処理 [battleLog.h]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
struct BATTLELOG
{
	D3DXVECTOR3		pos;		// ポリゴンの座標
	float			w, h;		// 幅と高さ
	int				texNo;		// 使用しているテクスチャ番号

	BOOL			use;
};

enum
{
	ESSCAPE_SUCCESS,
	ESCAPE_FAIL,
	ENEMY_KILLED,
	MAX_HP_UP_LOG,
	MAX_MP_UP_LOG,
	DAMAGE_UP_LOG,
	RECOVER_LOG,
	LOG_MAX
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBattleLog(void);
void UninitBattleLog(void);
void UpdateBattleLog(void);
void DrawBattleLog(void);
void ShowBattleLog(int texNo);

BATTLELOG *GetBattleLog(void);


