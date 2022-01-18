//=============================================================================
//
// �X�R�A���� [UltimateText.h]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
struct UTEXT
{
	D3DXVECTOR3		pos;		// �|���S���̍��W
	float			w, h;		// ���ƍ���
	int				texNo;		// �g�p���Ă���e�N�X�`���ԍ�
	int				countAnim;
	int				patternAnim;

	BOOL			use;
};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitUltimateText(void);
void UninitUltimateText(void);
void UpdateUltimateText(void);
void DrawUltimateText(void);

UTEXT *GetUltimateText(void);

