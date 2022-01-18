/*******************************************************************************
* タイトル:		Battleプログラム
* プログラム名:	battle.cpp
* 作成者:		GP11A132 21 Daya Juang Mahaputra
*******************************************************************************/

/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "main.h"
#include "battle.h"
#include "battleUI.h"
#include "bg.h"
#include "damage.h"
#include "player.h"
#include "battleCursor.h"
#include "effect.h"
#include "bg.h"
#include "layer.h"
#include "bubble.h"
#include "limitBar.h"
#include "battleLog.h"
#include "actionBar.h"
#include "tutorial.h"
#include "sound.h"

/*******************************************************************************
* マクロ定義
*******************************************************************************/


/*******************************************************************************
* 構造体定義
*******************************************************************************/


/*******************************************************************************
* プロトタイプ宣言(こんな関数を用意してありますよ宣言/関数の説明書)
*******************************************************************************/



/*******************************************************************************
* グローバル変数
*******************************************************************************/

BATTLE		g_BPlayer;			// バトル用のプレイヤーワーク
BATTLE		g_BEnemy;			// バトル用のエネミーワーク

// 必要なオブジェクトのアドレスを取得
DAMAGE		*damageScore = GetDamageAddress();
BATTLECURSOR *cursor = GetBattleCursor();
PLAYER *player = GetPlayer();
ENEMY *enemy = GetEnemy();
BG *bg = GetBG();
LAYER *layer = GetLayer();

static BOOL enemy_turn;
static int waitTurn;
static int playerOdd = 0;
static BOOL firstTime = TRUE;

// バトルの初期化処理
void InitBattle(void)
{

}


// バトルの終了処理
void UninitBattle(void)
{

}


// バトルの更新処理
void UpdateBattle(void)
{
	TUTORIAL *tutorial = GetTutorial();

	if (player[0].battleReady)	// まだバトル中かどうか
	{
		if (firstTime)
		{
			firstTime = FALSE;
			ShowTutorial(BATTLE_TUTORIAL1, BATTLE_TUTORIAL);
		}
		else if(!tutorial->use)
		{
			if (g_BPlayer.stop == FALSE && g_BPlayer.hp > 0 && g_BEnemy.hp > 0)
			{
				g_BPlayer.action += (0.1f * g_BPlayer.speed);

				if (g_BPlayer.action >= ACTION_BAR_MAX - 3.0f)
				{
					g_BPlayer.action = ACTION_BAR_MAX - 3.0f;
					cursor->canDoAction = TRUE;
				}
				else
				{
					cursor->canDoAction = FALSE;
				}
			}

			if (g_BEnemy.stop == FALSE && g_BPlayer.hp > 0 && g_BEnemy.hp > 0)
			{
				g_BEnemy.action += (0.1f * g_BEnemy.speed);
			}

			if (g_BEnemy.action >= ACTION_BAR_MAX - 3.0f)
			{
				g_BEnemy.action = ACTION_BAR_MAX - 3.0f;
				cursor->canDoAction = FALSE;
				enemy_turn = TRUE;
			}
			else
			{
				enemy_turn = FALSE;
			}

			if (g_BEnemy.canDoAction && enemy_turn) // エネミーのターンなら
			{
				EnemyAttack();
				waitTurn = 0;
			}
		}
	}

#ifdef _DEBUG
	// デバッグ表示
	PrintDebugProc("PlayerAction:%f EnemyAction:%f \n", g_BPlayer.action, g_BEnemy.action);
#endif

}

// バトル描画処理
void DrawBattle(void)
{

}

// バトルする前に必要なデータを取得変数・準備
void SetBattle(int enemyNo)
{
	BG *bg = GetBG();

	// バトルプレイヤーのデータを登録
	g_BPlayer.pos = player[0].battlePos - bg->pos;
	g_BPlayer.hp = player[0].hp;
	g_BPlayer.hp_max = player[0].maxHP;

	g_BPlayer.mp = player[0].mp;
	g_BPlayer.mp_max = player[0].maxMP;
	g_BPlayer.atk = player[0].atkMin;
	g_BPlayer.atk_max = player[0].atkMax;
	g_BPlayer.use = player[0].use;
	g_BPlayer.h = player[0].h;
	g_BPlayer.w = player[0].w;
	g_BPlayer.speed = player[0].speed;
	g_BPlayer.action = player[0].actionTime;
	g_BPlayer.roll = 0;

	// バトルエネミーのデータを登録
	g_BEnemy.pos = enemy[enemyNo].pos - bg->pos;
	g_BEnemy.hp = enemy[enemyNo].hp;
	g_BEnemy.hp_max = enemy[enemyNo].maxHP;
	g_BEnemy.mp = enemy[enemyNo].mp;
	g_BEnemy.mp_max = enemy[enemyNo].maxMP;
	g_BEnemy.atk = enemy[enemyNo].atkMin;
	g_BEnemy.atk_max = enemy[enemyNo].atkMax;
	g_BEnemy.use = enemy[enemyNo].use;
	g_BEnemy.exp = enemy[enemyNo].exp;
	g_BEnemy.h = enemy[enemyNo].h;
	g_BEnemy.w = enemy[enemyNo].w;
	g_BEnemy.speed = enemy[enemyNo].speed;
	g_BEnemy.action = enemy[enemyNo].action;
	g_BEnemy.roll = 0;
	g_BEnemy.canDoAction = TRUE;

	g_BEnemy.stop = FALSE;
	g_BPlayer.stop = FALSE;

	g_BEnemy.enemyIndex = enemyNo;

	waitTurn = 0;					// ターンを待つ時間
}

// プレイヤーの攻撃関数
void PlayerAttack(void)
{
	PLAYER *player = GetPlayer();

	g_BPlayer.action = 0.0f;
	g_BEnemy.stop = TRUE;
	g_BPlayer.stop = TRUE;
	g_BEnemy.canDoAction = FALSE;
	cursor->canDoAction = FALSE;
	cursor->posIndex = 0;

	// 攻撃は当たるかどうか判断
	g_BPlayer.roll = (1 + rand() % 6) + playerOdd;
	g_BEnemy.roll = (1 + rand() % 6);

	if (g_BPlayer.roll >= g_BEnemy.roll)
	{
		g_BPlayer.damage = g_BPlayer.atk + rand() % (g_BPlayer.atk_max - g_BPlayer.atk);	// ダメージを取得
		playerOdd = 0;
	}
	else
	{
		g_BPlayer.damage = 0;
		playerOdd++;
	}

	// ダメージ表示を設定
	SetDamage(g_BPlayer.damage);
	SetDamageDigit(g_BPlayer.damage);
	SetDamagePosition(g_BEnemy.pos);

	// プレイヤーの攻撃アニメーション
	player->idle = FALSE;
	player->action = PLAYER_ATTACK;
	SetBubble(g_BPlayer.pos, 0);

	bg->r = bg->g = bg->b = layer->r = layer->g = layer->b = 0.2f;

	g_BEnemy.hp -= g_BPlayer.damage;	// エネミーがダメージを受ける

	AddLimitBar(8);		// プレイヤーのLIMITが8上がる

	PlaySound(SOUND_LABEL_SE_swing);
}

// エネミーの攻撃
void EnemyAttack(void)
{
	EFFECT *effect = GetEffect();
	g_BEnemy.action = 0.0f;
	g_BPlayer.stop = TRUE;
	g_BEnemy.stop = TRUE;
	cursor->canDoAction = FALSE;

	// 攻撃は当たるかどうか判断
	g_BPlayer.roll = (1 + rand() % 6);
	g_BEnemy.roll = (1 + rand() % 6);

	if (g_BEnemy.roll >= g_BPlayer.roll)
		g_BEnemy.damage = g_BEnemy.atk + rand() % (g_BEnemy.atk_max - g_BEnemy.atk);	// ダメージを取得
	else
		g_BEnemy.damage = 0;

	// ダメージ表示を設定
	SetDamage(g_BEnemy.damage);
	SetDamageDigit(g_BEnemy.damage);
	SetDamagePosition(g_BPlayer.pos);
	SetBubble(g_BEnemy.pos, 0);

	bg->r = bg->g = bg->b = layer->r = layer->g = layer->b = 0.2f;

	// エフェクトを設定
	SetEffect(g_BPlayer.pos, ENEMY_ATTACK_EFFECT);

	ShowDamage();

	g_BPlayer.hp -= g_BEnemy.damage;	// プレイヤーがダメージを受ける

	AddLimitBar(4);		// プレイヤーのLIMITが4上がる
}

void PlayerLimitBreak(void)
{
	PLAYER *player = GetPlayer();

	g_BPlayer.action = 0.0f;
	g_BEnemy.stop = TRUE;
	g_BPlayer.stop = TRUE;
	cursor->canDoAction = FALSE;
	cursor->posIndex = 0;

	int baseDamage = 50*player->level;	// ベースダメージ
	player->limitBreak = 0;

	// ダメージを取得
	g_BPlayer.damage = g_BPlayer.atk + rand() % (g_BPlayer.atk_max - g_BPlayer.atk) + baseDamage;

	// ダメージ表示を設定
	SetDamage(g_BPlayer.damage);
	SetDamageDigit(g_BPlayer.damage);
	SetDamagePosition(g_BEnemy.pos);

	// プレイヤーのアニメーション
	player->idle = FALSE;
	player->action = PLAYER_LIMIT_BREAK;

	bg->r = bg->g = bg->b = layer->r = layer->g = layer->b = 0.2f;

	g_BEnemy.hp -= g_BPlayer.damage;
}

// プレイヤーの火球の技
void FireBall(void)
{
	PLAYER *player = GetPlayer();
	g_BPlayer.action = 0.0f;
	g_BEnemy.stop = TRUE;
	g_BPlayer.stop = TRUE;
	cursor->canDoAction = FALSE;
	cursor->posIndex = 0;

	int mpNeeded = 6;		//　必要なMP
	int baseDamage = 10;	// ベースダメージ

	// ダメージを取得
	g_BPlayer.damage = g_BPlayer.atk + rand() % (g_BPlayer.atk_max - g_BPlayer.atk) + baseDamage;
	g_BPlayer.mp -= mpNeeded;

	// ダメージ表示を設定
	SetDamage(g_BPlayer.damage);
	SetDamageDigit(g_BPlayer.damage);
	SetDamagePosition(g_BEnemy.pos);
	SetBubble(g_BPlayer.pos, 1);

	// プレイヤーのアニメーション
	player->idle = FALSE;
	player->action = PLAYER_CAST_FIREBALL;

	bg->r = bg->g = bg->b = layer->r = layer->g = layer->b = 0.2f;

	g_BEnemy.hp -= g_BPlayer.damage;

	AddLimitBar(8);

}

// プレイヤーのキュア技
void Cure(void)
{
	PLAYER *player = GetPlayer();
	g_BPlayer.action = 0.0f;
	g_BEnemy.stop = TRUE;
	g_BPlayer.stop = TRUE;
	cursor->canDoAction = FALSE;
	cursor->posIndex = 0;

	int mpNeeded = 10;
	int baseDamage = 15;

	g_BPlayer.damage = g_BPlayer.atk + rand() % (g_BPlayer.atk_max - g_BPlayer.atk) + baseDamage;
	g_BPlayer.mp -= mpNeeded;

	// ダメージ表示を設定
	SetDamage(g_BPlayer.damage);
	SetDamageDigit(g_BPlayer.damage);
	SetDamagePosition(g_BPlayer.pos);
	SetBubble(g_BPlayer.pos, 2);

	// プレイヤーの攻撃アニメーション
	player->idle = FALSE;
	player->action = PLAYER_CAST_HEAL;

	bg->r = bg->g = bg->b = layer->r = layer->g = layer->b = 0.2f;

	g_BPlayer.hp += g_BPlayer.damage;

	if (g_BPlayer.hp > g_BPlayer.hp_max)
		g_BPlayer.hp = g_BPlayer.hp_max;

	AddLimitBar(8);

}

// プレイヤーの逃げる関数
void EscapeCheck(void)
{
	int chance = 50;	//　逃げるの確率
	int flee = rand() % 100;

	g_BEnemy.stop = g_BPlayer.stop = TRUE;
	cursor->canDoAction = FALSE;

	// 逃げられますか
	if (flee > chance)
	{
		// YESだったら、バトルが終わる
		ShowBattleLog(ESSCAPE_SUCCESS);
	}
	else
	{
		// NO だったら、エネミーのターンになる
		ShowBattleLog(ESCAPE_FAIL);
	}
}

void Escape(int isSuccess)
{
	BATTLEUI *battleUI = GetBattleUI();
	g_BPlayer.action = 0.0f;

	if (isSuccess == ESSCAPE_SUCCESS)
	{
		player->hp = g_BPlayer.hp;
		player->mp = g_BPlayer.mp;

		enemy[g_BEnemy.enemyIndex].hp = 0;
		enemy[g_BEnemy.enemyIndex].mode = ENEMY_MODE_PATROL;

		battleUI->use = FALSE;

		BattleEnd();
	}
	else
	{
		g_BEnemy.stop = FALSE;
		g_BPlayer.stop = FALSE;
	}

}

// 技を使う前にプレイヤーのMPをチェック
BOOL CheckMP(int mpToCheck)
{
	if (g_BPlayer.mp >= mpToCheck)
	{
		return TRUE;		// 足りる
	}

	return FALSE;			// 足りない
}

// バトルのチェック関数
void CheckBattleResult(void)
{
	BATTLEUI *battleUI = GetBattleUI();

	// バトルが終わるかどうか
	if (g_BPlayer.hp <= 0)
	{
		battleUI->use = FALSE;

		g_BPlayer.hp = 0;
		player->hp = g_BPlayer.hp;

		return;
	}
	else if (g_BEnemy.hp <= 0)
	{
		battleUI->use = FALSE;

		StopSound();
		PlaySound(SOUND_LABEL_SE_victory);

		// プレイヤーのHPとMP戻す
		player->hp = g_BPlayer.hp;
		player->mp = g_BPlayer.mp;
		player->currentExp += g_BEnemy.exp;

		player->currentExp += g_BEnemy.exp;

		if (player->currentExp >= player->expToNextLevel)
		{
			LevelUp();
		}

		enemy[g_BEnemy.enemyIndex].hp = 0;
		enemy[g_BEnemy.enemyIndex].mode = ENEMY_MODE_PATROL;

		ShowBattleLog(ENEMY_KILLED);

		return;
	}

	g_BEnemy.canDoAction = TRUE;

}

void BattleEnd(void)
{
	BATTLEUI *battleUI = GetBattleUI();

	player->mode = PLAYER_MODE_EXPLORE;
	player->idle = TRUE;
	player->battleReady = FALSE;
	player->texNo = 0;

	StopSound();

	if(bg->texNo == FOREST)
		PlaySound(SOUND_LABEL_BGM_forest);
	else
		PlaySound(SOUND_LABEL_BGM_dungeon);

}

BATTLE *GetBattlePlayer(void)
{
	return &g_BPlayer;
}

BATTLE *GetBattleEnemy(void)
{
	return &g_BEnemy;
}