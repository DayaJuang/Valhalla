//=============================================================================
//
// �X�R�A���� [battleLog.h]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
struct BATTLELOG
{
	D3DXVECTOR3		pos;		// �|���S���̍��W
	float			w, h;		// ���ƍ���
	int				texNo;		// �g�p���Ă���e�N�X�`���ԍ�

	BOOL			use;
};

enum
{
	ESSCAPE_SUCCESS,
	ESCAPE_FAIL,
	ENEMY_KILLED,
	MAX_HP_UP_LOG,
	MAX_MP_UP_LOG,
	DAMAGE_UP_LOG,
	RECOVER_LOG,
	LOG_MAX
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBattleLog(void);
void UninitBattleLog(void);
void UpdateBattleLog(void);
void DrawBattleLog(void);
void ShowBattleLog(int texNo);

BATTLELOG *GetBattleLog(void);


