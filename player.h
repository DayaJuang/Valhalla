//=============================================================================
//
// �v���C���[���� [player.h]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PLAYER_MAX		(1)		// �v���C���[��Max�l��


//*****************************************************************************
// �\���̒�`
//*****************************************************************************

struct PLAYER
{
	D3DXVECTOR3		pos;			// �|���S���̍��W
	D3DXVECTOR3		oldPos;			// �|���S���̍��W
	D3DXVECTOR3		rot;			// �|���S���̉�]��
	BOOL			use;			// true:�g���Ă���  false:���g�p
	float			w, h;			// ���ƍ���
	int				countAnim;		// �A�j���[�V�����J�E���g
	int				patternAnim;	// �A�j���[�V�����p�^�[���i���o�[
	int				texNo;			// �e�N�X�`���ԍ�
	int				dir;			// �v���C���[����
	int				next_Dir;		// �v���C���[����
	int				mode;			// �v���C���[�̏��
	BOOL			idle;			// �v���C���[�������Ă��邩�ǂ����A�m�F�ϐ�
	D3DXVECTOR3		move;			// �ړ����x

	BOOL			canMove;		// �v���C���[���ړ��ł��邩�ǂ����m�F
	BOOL			battleReady;	// �o�g���������ǂ����ǂ���
	int				currentLocation;// �v���C���[�����݂ɂ���t�B�[���h

	// �o�g���p�̃X�e�[�^�X
	int				level;
	int				currentExp;			// ���݂̌o���l
	int				expToNextLevel;	// �K�v�Ȍo���l
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

	// �v���C���[��RGBA
	float r, g, b, a;

	BOOL isAlive = FALSE;
};

enum
{
	PLAYER_MODE_EXPLORE,	// �T�����[�h
	PLAYER_MODE_BATTLE,		// �o�g�����[�h
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
// �v���g�^�C�v�錾
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



