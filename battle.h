/*******************************************************************************
* タイトル:		Battleヘッダー
* プログラム名:	battle.h
* 作成者:		GP11A132 21 Daya Juang Mahaputra
*******************************************************************************/

#pragma once	// インクルードガード（多重インクルード防止）

/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "player.h"
#include "enemy.h"


/*******************************************************************************
* マクロ定義
*******************************************************************************/
#define TURN_MAX		(2)		// プレイヤーのMax人数


/*******************************************************************************
* 構造体定義
*******************************************************************************/
struct BATTLE
{
	int damage;

	// バトル用メンバー変数
	int hp, hp_max;		// HP / HP Max
	int mp, mp_max;		// HP / HP Max
	int atk, atk_max;	// 攻撃力
	int speed;

	int enemyIndex;		// エネミー用の番号
	int exp;
	int roll;
	float action;
	float h, w;			// 幅と高さ

	D3DXVECTOR3		pos;			// ポリゴンの座標
	BOOL			use;			// 使っているかどうかの変数
	BOOL			canDoAction;
	BOOL			stop;
};

enum
{
	BATTLING,
	PLAYER_WIN,
	ENEMY_WIN
};


/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
void InitBattle(void);
void UninitBattle(void);
void UpdateBattle(void);
void DrawBattle(void);

// バトル用の関数
void PlayerAttack(void);
void EnemyAttack(void);
void FireBall(void);
void Cure(void);
void PlayerLimitBreak(void);

void Escape(int isSuccess);
void EscapeCheck(void);

BOOL CheckMP(int mpToCheck);

void BattleEnd(void);
void CheckBattleResult(void);

// バトル番号を引数に貰ってそれを元にバトルの初期化を行う
void SetBattle(int enemyNo);

BATTLE *GetBattlePlayer(void);
BATTLE *GetBattleEnemy(void);



