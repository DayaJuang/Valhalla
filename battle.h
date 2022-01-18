/*******************************************************************************
* �^�C�g��:		Battle�w�b�_�[
* �v���O������:	battle.h
* �쐬��:		GP11A132 21 Daya Juang Mahaputra
*******************************************************************************/

#pragma once	// �C���N���[�h�K�[�h�i���d�C���N���[�h�h�~�j

/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/
#include "player.h"
#include "enemy.h"


/*******************************************************************************
* �}�N����`
*******************************************************************************/
#define TURN_MAX		(2)		// �v���C���[��Max�l��


/*******************************************************************************
* �\���̒�`
*******************************************************************************/
struct BATTLE
{
	int damage;

	// �o�g���p�����o�[�ϐ�
	int hp, hp_max;		// HP / HP Max
	int mp, mp_max;		// HP / HP Max
	int atk, atk_max;	// �U����
	int speed;

	int enemyIndex;		// �G�l�~�[�p�̔ԍ�
	int exp;
	int roll;
	float action;
	float h, w;			// ���ƍ���

	D3DXVECTOR3		pos;			// �|���S���̍��W
	BOOL			use;			// �g���Ă��邩�ǂ����̕ϐ�
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
* �v���g�^�C�v�錾
*******************************************************************************/
void InitBattle(void);
void UninitBattle(void);
void UpdateBattle(void);
void DrawBattle(void);

// �o�g���p�̊֐�
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

// �o�g���ԍ��������ɖ���Ă�������Ƀo�g���̏��������s��
void SetBattle(int enemyNo);

BATTLE *GetBattlePlayer(void);
BATTLE *GetBattleEnemy(void);



