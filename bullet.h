//=============================================================================
//
// �o���b�g���� [bullet.h]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BULLET_MAX		(1)			// �o���b�g��Max��
#define BULLET_SPEED	(4.0f)		// �o���b�g�̈ړ��X�s�[�h


// �o���b�g�\����
struct BULLET
{
	BOOL					use;					// true:�g���Ă���  false:���g�p
	float					w, h;					// ���ƍ���
	D3DXVECTOR3				pos;					// �o���b�g�̍��W
	D3DXVECTOR3				targetPos;				// �^�[�Q�b�g�̍��W
	D3DXVECTOR3				rot;					// �o���b�g�̉�]��
	int						countAnim;				// �A�j���[�V�����J�E���g
	int						patternAnim;			// �A�j���[�V�����p�^�[���i���o�[
	int						texNo;					// ���Ԗڂ̃e�N�X�`���[���g�p����̂�

};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);

BULLET *GetBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 targetPos);
