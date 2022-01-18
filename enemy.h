//=============================================================================
//
// �G�l�~�[���� [enemy.h]
// Author : 
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define ENEMY_MAX		(5)		// �G�l�~�[��Max�l��

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
// �\���̒�`
//*****************************************************************************

struct	LINEAR_INTERPOLATION		// ���`��ԃf�[�^�̍\����
{
	D3DXVECTOR3		pos;			// ���W
	D3DXVECTOR3		rot;			// ��]��
	D3DXVECTOR3		scl;			// �g�嗦
	float			time;			// �ړ�����
};


struct ENEMY
{
	D3DXVECTOR3		pos;			// �|���S���̍��W
	D3DXVECTOR3		rot;			// �|���S���̉�]��
	D3DXVECTOR3		scl;			// �|���S���̊g�嗦
	BOOL			use;			// true:�g���Ă���  false:���g�p
	float			w, h;			// ���ƍ���
	int				countAnim;		// �A�j���[�V�����J�E���g
	int				patternAnim;	// �A�j���[�V�����p�^�[���i���o�[
	int				texNo;			// �e�N�X�`���ԍ�
	int				dir;			// �G�l�~�[�̕���
	int				mode;			// ���݂̃G�l�~�[���
	D3DXVECTOR3		move;			// �ړ����x

	float			time;			// ���`��ԗp
	int				moveTblNo;		// �s���f�[�^�̃e�[�u���ԍ�
	int				tblMax;			// ���̃e�[�u���̃f�[�^��

	// �o�g���p�̕ϐ�
	int				maxHP;						// �G�l�~�[�̍ő�HP
	int				maxMP;						// �G�l�~�[�̍ő�MP
	int				atkPower;					// �G�l�~�[�̍U����
	int				hp = maxHP;
	int				mp = maxMP;
	int				atkMin, atkMax;					// �ŏ��E�ő�̍U��
	int				speed;
	int				exp, minExp, maxExp;			// �v���C���[�����炤�o���l
	float			action;

	// �G�l�~�[��RGBA
	float r, g, b, a;
};



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
D3DXVECTOR3 FindEnemyPosition(int monsterIndex);

void EnemyDead(int index);

ENEMY *GetEnemy(void);



