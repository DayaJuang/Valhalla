//=============================================================================
//
// BattleUI��ʏ��� [battleUI.h]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
struct BATTLEUI
{
	D3DXVECTOR3		pos;		// �|���S���̍��W
	float			w, h;		// ���ƍ���
	int				texNo;		// �g�p���Ă���e�N�X�`���ԍ�

	BOOL			use;

};

enum
{
	BATTLE_UI_LEFT,
	BATTLE_UI_RIGHT,
	POWER_UP_PANEL
};

enum
{
	BATTLE_UI,
	MAGIC_UI,
	POWERUP_UI
};



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBattleUI(void);
void UninitBattleUI(void);
void UpdateBattleUI(void);
void DrawBattleUI(void);
void ShowBattleUI(int texNo);

BATTLEUI *GetBattleUI(void);

