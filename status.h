//=============================================================================
//
// スコア処理 [status.h]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define STATUS_MAX			(999)		// スコアの最大値

#define	STATUS_DX			(500.0f)	// 表示位置
#define	STATUS_DY			(20.0f)		// 
#define	STATUS_DW			(16.0f)		// 
#define	STATUS_DH			(32.0f)		// 

enum {
	STATUS_HP,
	STATUS_MP,
	STATUS_MAX_INDEX
};

struct STATUS
{
	D3DXVECTOR3		pos;		// ポリゴンの座標
	float			w, h;		// 幅と高さ
	int				texNo;		// 使用しているテクスチャ番号

	int				status;		// 得点
	int				digit;		// 桁

	float			alpha;		// 透明さ

	BOOL			use;
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitStatus(void);
void UninitStatus(void);
void UpdateStatus(void);
void DrawStatus(void);

int GetStatus(void);
void SetStatusDigit(int Status);
void SetStatusPosition(D3DXVECTOR3 targetPos);
void AddStatus(int add);
void SetStatus(int Status);

STATUS *GetStatusAddress(void);


