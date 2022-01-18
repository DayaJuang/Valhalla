//=============================================================================
//
// �X�R�A���� [damage.h]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define DAMAGE_MAX			(999)		// �X�R�A�̍ő�l

#define	DAMAGE_DX			(500.0f)	// �\���ʒu
#define	DAMAGE_DY			(20.0f)		// 
#define	DAMAGE_DW			(16.0f)		// 
#define	DAMAGE_DH			(32.0f)		// 

struct DAMAGE
{
	D3DXVECTOR3		pos;		// �|���S���̍��W
	float			w, h;		// ���ƍ���
	int				texNo;		// �g�p���Ă���e�N�X�`���ԍ�

	int				damage;		// ���_
	int				digit;

	float			alpha;

	BOOL			use;
};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitDamage(void);
void UninitDamage(void);
void UpdateDamage(void);
void DrawDamage(void);

int GetDamage(void);
void SetDamageDigit(int damage);
void SetDamagePosition(D3DXVECTOR3 targetPos);
void SetDamage(int Damage);
void ShowDamage(void);

DAMAGE *GetDamageAddress(void);


