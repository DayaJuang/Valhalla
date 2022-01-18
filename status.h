//=============================================================================
//
// �X�R�A���� [status.h]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define STATUS_MAX			(999)		// �X�R�A�̍ő�l

#define	STATUS_DX			(500.0f)	// �\���ʒu
#define	STATUS_DY			(20.0f)		// 
#define	STATUS_DW			(16.0f)		// 
#define	STATUS_DH			(32.0f)		// 

enum {
	STATUS_HP,
	STATUS_MP,
	STATUS_MAX_INDEX
};

struct STATUS
{
	D3DXVECTOR3		pos;		// �|���S���̍��W
	float			w, h;		// ���ƍ���
	int				texNo;		// �g�p���Ă���e�N�X�`���ԍ�

	int				status;		// ���_
	int				digit;		// ��

	float			alpha;		// ������

	BOOL			use;
};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitStatus(void);
void UninitStatus(void);
void UpdateStatus(void);
void DrawStatus(void);

int GetStatus(void);
void SetStatusDigit(int Status);
void SetStatusPosition(D3DXVECTOR3 targetPos);
void AddStatus(int add);
void SetStatus(int Status);

STATUS *GetStatusAddress(void);


