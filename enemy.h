//=============================================================================
//
// エネミー処理 [enemy.h]
// Author : 
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ENEMY_MAX		(5)		// エネミーのMax人数

enum
{
	ENEMY_MODE_PATROL,
	ENEMY_MODE_BATTLE,
	ENEMY_MODE_MAX
};

enum
{
	ENEMY_LOOK_DOWN,
	ENEMY_LOOK_RIGHT,
	ENEMY_LOOK_UP,
	ENEMY_LOOK_LEFT,
};

//*****************************************************************************
// 構造体定義
//*****************************************************************************

struct	LINEAR_INTERPOLATION		// 線形補間データの構造体
{
	D3DXVECTOR3		pos;			// 座標
	D3DXVECTOR3		rot;			// 回転量
	D3DXVECTOR3		scl;			// 拡大率
	float			time;			// 移動時間
};


struct ENEMY
{
	D3DXVECTOR3		pos;			// ポリゴンの座標
	D3DXVECTOR3		rot;			// ポリゴンの回転量
	D3DXVECTOR3		scl;			// ポリゴンの拡大率
	BOOL			use;			// true:使っている  false:未使用
	float			w, h;			// 幅と高さ
	int				countAnim;		// アニメーションカウント
	int				patternAnim;	// アニメーションパターンナンバー
	int				texNo;			// テクスチャ番号
	int				dir;			// エネミーの方向
	int				mode;			// 現在のエネミー状態
	D3DXVECTOR3		move;			// 移動速度

	float			time;			// 線形補間用
	int				moveTblNo;		// 行動データのテーブル番号
	int				tblMax;			// そのテーブルのデータ数

	// バトル用の変数
	int				maxHP;						// エネミーの最大HP
	int				maxMP;						// エネミーの最大MP
	int				atkPower;					// エネミーの攻撃力
	int				hp = maxHP;
	int				mp = maxMP;
	int				atkMin, atkMax;					// 最小・最大の攻撃
	int				speed;
	int				exp, minExp, maxExp;			// プレイヤーがもらう経験値
	float			action;

	// エネミーのRGBA
	float r, g, b, a;
};



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
D3DXVECTOR3 FindEnemyPosition(int monsterIndex);

void EnemyDead(int index);

ENEMY *GetEnemy(void);



