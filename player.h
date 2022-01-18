//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PLAYER_MAX		(1)		// プレイヤーのMax人数


//*****************************************************************************
// 構造体定義
//*****************************************************************************

struct PLAYER
{
	D3DXVECTOR3		pos;			// ポリゴンの座標
	D3DXVECTOR3		oldPos;			// ポリゴンの座標
	D3DXVECTOR3		rot;			// ポリゴンの回転量
	BOOL			use;			// true:使っている  false:未使用
	float			w, h;			// 幅と高さ
	int				countAnim;		// アニメーションカウント
	int				patternAnim;	// アニメーションパターンナンバー
	int				texNo;			// テクスチャ番号
	int				dir;			// プレイヤー方向
	int				next_Dir;		// プレイヤー方向
	int				mode;			// プレイヤーの状態
	BOOL			idle;			// プレイヤーが動いているかどうか、確認変数
	D3DXVECTOR3		move;			// 移動速度

	BOOL			canMove;		// プレイヤーが移動できるかどうか確認
	BOOL			battleReady;	// バトル準備が良いかどうか
	int				currentLocation;// プレイヤーが現在にいるフィールド

	// バトル用のステータス
	int				level;
	int				currentExp;			// 現在の経験値
	int				expToNextLevel;	// 必要な経験値
	int				maxHP;
	int				maxMP;
	int				atkPower;
	int				hp = maxHP;
	int				mp = maxMP;
	int				atkMin, atkMax;
	float			limitBreak;
	int				speed;
	float			actionTime;

	D3DXVECTOR3		battlePos;
	int				action;
	float			time;

	// プレイヤーのRGBA
	float r, g, b, a;

	BOOL isAlive = FALSE;
};

enum
{
	PLAYER_MODE_EXPLORE,	// 探検モード
	PLAYER_MODE_BATTLE,		// バトルモード
	PLAYER_MODE_MAX
};

enum
{
	PLAYER_LOOK_UP,
	PLAYER_LOOK_LEFT,
	PLAYER_LOOK_DOWN,
	PLAYER_LOOK_RIGHT,

};

enum
{
	PLAYER_WAIT,
	PLAYER_ATTACK,
	PLAYER_CAST_FIREBALL,
	PLAYER_CAST_HEAL,
	PLAYER_LIMIT_BREAK
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
void LevelUp(void);

D3DXVECTOR3 FindPosition(void);
void BattlePreparation(void);

void AttackAnimation(int action);
void MagicAnimation(int playerSprite, int action);
void PlayerDead(void);

PLAYER *GetPlayer(void);



