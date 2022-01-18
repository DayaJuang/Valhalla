//=============================================================================
//
// BG��ʏ��� [battleCursor.h]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CURSOR_MAX_POSITION		(3)
#define POWER_UP_MAX_POSITION	(4)

struct BATTLECURSOR
{
	D3DXVECTOR3		pos[CURSOR_MAX_POSITION];			// �|���S���̍��W
	D3DXVECTOR3		powerUpPos[POWER_UP_MAX_POSITION];	// �����p�̃|���S���̍��W
	float			w, h;		// ���ƍ���
	int				texNo;		// �g�p���Ă���e�N�X�`���ԍ�
	int				mode;
	int				posIndex;

	BOOL			canDoAction;
};

enum
{
	HP_UP,
	MP_UP,
	ATK_UP,
	RECOVERALL
};

enum {
	MENU_SELECT,
	ATTACK_ENEMY,
	USE_FIREBALL,
	HEAL_ALLY,
	POWERUP_SELECT
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBattleCursor(void);
void UninitBattleCursor(void);
void UpdateBattleCursor(void);
void DrawBattleCursor(void);

void SelectTargetEnemy(void);
void SelectAlly(void);

BATTLECURSOR *GetBattleCursor(void);

