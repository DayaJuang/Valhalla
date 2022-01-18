//=============================================================================
//
// �X�R�A���� [ActionBarFrame.h]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

struct ACTIONBARFRAME
{
	D3DXVECTOR3		pos;		// �|���S���̍��W
	float			w, h;		// ���ƍ���
	int				texNo;		// �g�p���Ă���e�N�X�`���ԍ�

	BOOL			use;
};

enum
{
	PLAYER1_FRAME,
	ENEMY1_FRAME
};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitActionBarFrame(void);
void UninitActionBarFrame(void);
void UpdateActionBarFrame(void);
void DrawActionBarFrame(void);

ACTIONBARFRAME *GetActionBarFrame(int index);


