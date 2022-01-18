//=============================================================================
//
// �X�R�A���� [bubble.h]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
struct BUBBLE
{
	D3DXVECTOR3		pos;		// �|���S���̍��W
	float			w, h;		// ���ƍ���
	int				texNo;		// �g�p���Ă���e�N�X�`���ԍ�

	BOOL			use;
};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBubble(void);
void UninitBubble(void);
void UpdateBubble(void);
void DrawBubble(void);

void SetBubble(D3DXVECTOR3 targetPos, int textNo);

BUBBLE *GetBubble(void);


