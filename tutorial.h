//=============================================================================
//
// �X�R�A���� [Tutorial.h]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define FIELD_TUTORIAL_MAX			(2)
#define BATTLE_TUTORIAL_MAX			(3)
#define WORLDMAP_TUTORIAL_MAX		(1)
#define BATTLE_TUTORIAL_MAXINDEX	(5)

struct TUTORIAL
{
	D3DXVECTOR3		pos;		// �|���S���̍��W
	float			w, h;		// ���ƍ���
	int				texNo;		// �g�p���Ă���e�N�X�`���ԍ�
	int				flag;

	BOOL			use;
};

enum
{
	FIELD_TUTORIAL1,
	FIELD_TUTORIAL2,
	BATTLE_TUTORIAL1,
	BATTLE_TUTORIAL2,
	BATTLE_TUTORIAL3,
	WORLDMAP_TUTORIAL1
};

enum
{
	FIELD_TUTORIAL,
	BATTLE_TUTORIAL,
	WORLDMAP_TUTORIAL
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitTutorial(void);
void UninitTutorial(void);
void UpdateTutorial(void);
void DrawTutorial(void);
void ShowTutorial(int texNo, int flag);

TUTORIAL *GetTutorial(void);


