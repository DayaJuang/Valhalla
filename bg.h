//=============================================================================
//
// BG��ʏ��� [bg.h]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
struct BG
{
	D3DXVECTOR3		pos;		// �|���S���̍��W
	float			w, h;		// ���ƍ���
	int				texNo;		// �g�p���Ă���e�N�X�`���ԍ�
	float			r, g, b;	// BG��RGB

};

enum
{
	PASS,
	COLLISION,
	PLAYER_POS,
	TO_WORLDMAP,
	TO_WORLDMAP_NEXT_AREA,
	ENEMY_POS
};

enum
{
	FOREST,
	DUNGEON
};



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBG(void);
void UninitBG(void);
void UpdateBG(void);
void DrawBG(void);
int CheckMap(float x, float y);
int GetMapIndex(int y, int x);

BG *GetBG(void);

