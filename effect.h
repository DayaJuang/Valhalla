//=============================================================================
//
// �o���b�g���� [effect.h]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

// �o���b�g�\����
struct EFFECT
{
	BOOL					use;					// true:�g���Ă���  false:���g�p
	float					w, h;					// ���ƍ���
	D3DXVECTOR3				pos;					// �o���b�g�̍��W
	D3DXVECTOR3				rot;					// �o���b�g�̉�]��
	D3DXVECTOR3				move;					// �o���b�g�̈ړ���
	int						countAnim;				// �A�j���[�V�����J�E���g
	int						patternAnim;			// �A�j���[�V�����p�^�[���i���o�[
	int						texNo;					// ���Ԗڂ̃e�N�X�`���[���g�p����̂�

};

enum
{
	ATTACK_EFFECT,
	ENEMY_ATTACK_EFFECT,
	FIRE_EFFECT,
	HEAL_EFFECT,
	LIMIT_BREAK_EFFECT,

	EFFECT_MAX
};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);

EFFECT *GetEffect(void);
void SetEffect(D3DXVECTOR3 pos, int texNo);

