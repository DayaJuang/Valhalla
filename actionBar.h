//=============================================================================
//
// �X�R�A���� [ActionBar.h]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define ACTION_BAR_MAX (100)

struct ACTIONBAR
{
	D3DXVECTOR3		pos;		// �|���S���̍��W
	float			w, h;		// ���ƍ���
	int				texNo;		// �g�p���Ă���e�N�X�`���ԍ�

	BOOL			use;
};

enum
{
	PLAYER1_BAR,
	ENEMY1_BAR
};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitActionBar(void);
void UninitActionBar(void);
void UpdateActionBar(void);
void DrawActionBar(void);

