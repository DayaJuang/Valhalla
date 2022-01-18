/*******************************************************************************
* �^�C�g��:		Battle�v���O����
* �v���O������:	battle.cpp
* �쐬��:		GP11A132 21 Daya Juang Mahaputra
*******************************************************************************/

/*******************************************************************************
* �C���N���[�h�t�@�C��
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
* �}�N����`
*******************************************************************************/


/*******************************************************************************
* �\���̒�`
*******************************************************************************/


/*******************************************************************************
* �v���g�^�C�v�錾(����Ȋ֐���p�ӂ��Ă���܂���錾/�֐��̐�����)
*******************************************************************************/



/*******************************************************************************
* �O���[�o���ϐ�
*******************************************************************************/

BATTLE		g_BPlayer;			// �o�g���p�̃v���C���[���[�N
BATTLE		g_BEnemy;			// �o�g���p�̃G�l�~�[���[�N

// �K�v�ȃI�u�W�F�N�g�̃A�h���X���擾
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

// �o�g���̏���������
void InitBattle(void)
{

}


// �o�g���̏I������
void UninitBattle(void)
{

}


// �o�g���̍X�V����
void UpdateBattle(void)
{
	TUTORIAL *tutorial = GetTutorial();

	if (player[0].battleReady)	// �܂��o�g�������ǂ���
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

			if (g_BEnemy.canDoAction && enemy_turn) // �G�l�~�[�̃^�[���Ȃ�
			{
				EnemyAttack();
				waitTurn = 0;
			}
		}
	}

#ifdef _DEBUG
	// �f�o�b�O�\��
	PrintDebugProc("PlayerAction:%f EnemyAction:%f \n", g_BPlayer.action, g_BEnemy.action);
#endif

}

// �o�g���`�揈��
void DrawBattle(void)
{

}

// �o�g������O�ɕK�v�ȃf�[�^���擾�ϐ��E����
void SetBattle(int enemyNo)
{
	BG *bg = GetBG();

	// �o�g���v���C���[�̃f�[�^��o�^
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

	// �o�g���G�l�~�[�̃f�[�^��o�^
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

	waitTurn = 0;					// �^�[����҂���
}

// �v���C���[�̍U���֐�
void PlayerAttack(void)
{
	PLAYER *player = GetPlayer();

	g_BPlayer.action = 0.0f;
	g_BEnemy.stop = TRUE;
	g_BPlayer.stop = TRUE;
	g_BEnemy.canDoAction = FALSE;
	cursor->canDoAction = FALSE;
	cursor->posIndex = 0;

	// �U���͓����邩�ǂ������f
	g_BPlayer.roll = (1 + rand() % 6) + playerOdd;
	g_BEnemy.roll = (1 + rand() % 6);

	if (g_BPlayer.roll >= g_BEnemy.roll)
	{
		g_BPlayer.damage = g_BPlayer.atk + rand() % (g_BPlayer.atk_max - g_BPlayer.atk);	// �_���[�W���擾
		playerOdd = 0;
	}
	else
	{
		g_BPlayer.damage = 0;
		playerOdd++;
	}

	// �_���[�W�\����ݒ�
	SetDamage(g_BPlayer.damage);
	SetDamageDigit(g_BPlayer.damage);
	SetDamagePosition(g_BEnemy.pos);

	// �v���C���[�̍U���A�j���[�V����
	player->idle = FALSE;
	player->action = PLAYER_ATTACK;
	SetBubble(g_BPlayer.pos, 0);

	bg->r = bg->g = bg->b = layer->r = layer->g = layer->b = 0.2f;

	g_BEnemy.hp -= g_BPlayer.damage;	// �G�l�~�[���_���[�W���󂯂�

	AddLimitBar(8);		// �v���C���[��LIMIT��8�オ��

	PlaySound(SOUND_LABEL_SE_swing);
}

// �G�l�~�[�̍U��
void EnemyAttack(void)
{
	EFFECT *effect = GetEffect();
	g_BEnemy.action = 0.0f;
	g_BPlayer.stop = TRUE;
	g_BEnemy.stop = TRUE;
	cursor->canDoAction = FALSE;

	// �U���͓����邩�ǂ������f
	g_BPlayer.roll = (1 + rand() % 6);
	g_BEnemy.roll = (1 + rand() % 6);

	if (g_BEnemy.roll >= g_BPlayer.roll)
		g_BEnemy.damage = g_BEnemy.atk + rand() % (g_BEnemy.atk_max - g_BEnemy.atk);	// �_���[�W���擾
	else
		g_BEnemy.damage = 0;

	// �_���[�W�\����ݒ�
	SetDamage(g_BEnemy.damage);
	SetDamageDigit(g_BEnemy.damage);
	SetDamagePosition(g_BPlayer.pos);
	SetBubble(g_BEnemy.pos, 0);

	bg->r = bg->g = bg->b = layer->r = layer->g = layer->b = 0.2f;

	// �G�t�F�N�g��ݒ�
	SetEffect(g_BPlayer.pos, ENEMY_ATTACK_EFFECT);

	ShowDamage();

	g_BPlayer.hp -= g_BEnemy.damage;	// �v���C���[���_���[�W���󂯂�

	AddLimitBar(4);		// �v���C���[��LIMIT��4�オ��
}

void PlayerLimitBreak(void)
{
	PLAYER *player = GetPlayer();

	g_BPlayer.action = 0.0f;
	g_BEnemy.stop = TRUE;
	g_BPlayer.stop = TRUE;
	cursor->canDoAction = FALSE;
	cursor->posIndex = 0;

	int baseDamage = 50*player->level;	// �x�[�X�_���[�W
	player->limitBreak = 0;

	// �_���[�W���擾
	g_BPlayer.damage = g_BPlayer.atk + rand() % (g_BPlayer.atk_max - g_BPlayer.atk) + baseDamage;

	// �_���[�W�\����ݒ�
	SetDamage(g_BPlayer.damage);
	SetDamageDigit(g_BPlayer.damage);
	SetDamagePosition(g_BEnemy.pos);

	// �v���C���[�̃A�j���[�V����
	player->idle = FALSE;
	player->action = PLAYER_LIMIT_BREAK;

	bg->r = bg->g = bg->b = layer->r = layer->g = layer->b = 0.2f;

	g_BEnemy.hp -= g_BPlayer.damage;
}

// �v���C���[�̉΋��̋Z
void FireBall(void)
{
	PLAYER *player = GetPlayer();
	g_BPlayer.action = 0.0f;
	g_BEnemy.stop = TRUE;
	g_BPlayer.stop = TRUE;
	cursor->canDoAction = FALSE;
	cursor->posIndex = 0;

	int mpNeeded = 6;		//�@�K�v��MP
	int baseDamage = 10;	// �x�[�X�_���[�W

	// �_���[�W���擾
	g_BPlayer.damage = g_BPlayer.atk + rand() % (g_BPlayer.atk_max - g_BPlayer.atk) + baseDamage;
	g_BPlayer.mp -= mpNeeded;

	// �_���[�W�\����ݒ�
	SetDamage(g_BPlayer.damage);
	SetDamageDigit(g_BPlayer.damage);
	SetDamagePosition(g_BEnemy.pos);
	SetBubble(g_BPlayer.pos, 1);

	// �v���C���[�̃A�j���[�V����
	player->idle = FALSE;
	player->action = PLAYER_CAST_FIREBALL;

	bg->r = bg->g = bg->b = layer->r = layer->g = layer->b = 0.2f;

	g_BEnemy.hp -= g_BPlayer.damage;

	AddLimitBar(8);

}

// �v���C���[�̃L���A�Z
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

	// �_���[�W�\����ݒ�
	SetDamage(g_BPlayer.damage);
	SetDamageDigit(g_BPlayer.damage);
	SetDamagePosition(g_BPlayer.pos);
	SetBubble(g_BPlayer.pos, 2);

	// �v���C���[�̍U���A�j���[�V����
	player->idle = FALSE;
	player->action = PLAYER_CAST_HEAL;

	bg->r = bg->g = bg->b = layer->r = layer->g = layer->b = 0.2f;

	g_BPlayer.hp += g_BPlayer.damage;

	if (g_BPlayer.hp > g_BPlayer.hp_max)
		g_BPlayer.hp = g_BPlayer.hp_max;

	AddLimitBar(8);

}

// �v���C���[�̓�����֐�
void EscapeCheck(void)
{
	int chance = 50;	//�@������̊m��
	int flee = rand() % 100;

	g_BEnemy.stop = g_BPlayer.stop = TRUE;
	cursor->canDoAction = FALSE;

	// �������܂���
	if (flee > chance)
	{
		// YES��������A�o�g�����I���
		ShowBattleLog(ESSCAPE_SUCCESS);
	}
	else
	{
		// NO ��������A�G�l�~�[�̃^�[���ɂȂ�
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

// �Z���g���O�Ƀv���C���[��MP���`�F�b�N
BOOL CheckMP(int mpToCheck)
{
	if (g_BPlayer.mp >= mpToCheck)
	{
		return TRUE;		// �����
	}

	return FALSE;			// ����Ȃ�
}

// �o�g���̃`�F�b�N�֐�
void CheckBattleResult(void)
{
	BATTLEUI *battleUI = GetBattleUI();

	// �o�g�����I��邩�ǂ���
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

		// �v���C���[��HP��MP�߂�
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