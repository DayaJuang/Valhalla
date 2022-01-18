//=============================================================================
//
// �G�t�F�N�g���� [effect.cpp]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#include "effect.h"
#include "sprite.h"
#include "enemy.h"
#include "collision.h"
#include "score.h"
#include "sound.h"
#include "bg.h"
#include "damage.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(65)	// �G�t�F�N�g�T�C�Y
#define TEXTURE_HEIGHT				(65)	// 
#define ULTIMATE_TEXTURE_WIDTH		(SCREEN_WIDTH)	// Ultimate�̃G�t�F�N�g�T�C�Y
#define ULTIMATE_TEXTURE_HEIGHT		(SCREEN_HEIGHT)	// 
#define TEXTURE_MAX					(EFFECT_MAX)		// �e�N�X�`���̐�

#define TEXTURE_PATTERN_DIVIDE_X	(6)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y	(1)		// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define ANIM_WAIT					(5)		// �A�j���[�V�����̐؂�ւ��Wait�l

#define ULTIMATE_TEXTURE_PATTERN_DIVIDE_X	(1)			// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define ULTIMATE_TEXTURE_PATTERN_DIVIDE_Y	(12)		// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ULTIMATE_ANIM_PATTERN_NUM			(ULTIMATE_TEXTURE_PATTERN_DIVIDE_X*ULTIMATE_TEXTURE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define ULTIMATE_ANIM_WAIT					(12)		// �A�j���[�V�����̐؂�ւ��Wait�l


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;				// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[] = {
	"data/EFFECT/slash_effect.png",
	"data/EFFECT/enemy_attack.png",
	"data/EFFECT/fire_effect.png",
	"data/EFFECT/heal_effect.png",
	"data/EFFECT/ultimate.png",
};

static BOOL	  g_Load = FALSE;		// ���������s�������̃t���O
static EFFECT g_Effect;				// �G�t�F�N�g�\����


//=============================================================================
// ����������
//=============================================================================
HRESULT InitEffect(void)
{
	ID3D11Device *pDevice = GetDevice();

	//�e�N�X�`������
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TexturName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}


	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	g_Effect.use = FALSE;			// ���g�p�i���˂���Ă��Ȃ��e�j
	g_Effect.w = TEXTURE_WIDTH;
	g_Effect.h = TEXTURE_HEIGHT;
	g_Effect.pos = D3DXVECTOR3(300, 300.0f, 0.0f);
	g_Effect.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Effect.texNo = 0;

	g_Effect.countAnim = 0;
	g_Effect.patternAnim = 0;

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEffect(void)
{
	if (g_Load == FALSE) return;

	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Texture[i])
		{
			g_Texture[i]->Release();
			g_Texture[i] = NULL;
		}
	}

	g_Load = FALSE;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEffect(void)
{
	BG *bg = GetBG();

	if (g_Effect.use == TRUE)	// ���̃G�t�F�N�g���g���Ă���H
	{								// Yes
		// �A�j���[�V����  
		g_Effect.countAnim++;
		if (g_Effect.texNo == LIMIT_BREAK_EFFECT)
		{
			if ((g_Effect.countAnim % ULTIMATE_ANIM_WAIT) == 0)
			{
				PlaySound(SOUND_LABEL_SE_laser);

				// �p�^�[���̐؂�ւ�
				g_Effect.patternAnim = (g_Effect.patternAnim + 1) % ULTIMATE_ANIM_PATTERN_NUM;

				if (g_Effect.patternAnim >= ULTIMATE_TEXTURE_PATTERN_DIVIDE_Y - 1)
				{
					g_Effect.use = FALSE;
					ShowDamage();
				}
			}
		}
		else
		{
			if ((g_Effect.countAnim % ANIM_WAIT) == 0)
			{
				// �p�^�[���̐؂�ւ�
				g_Effect.patternAnim = (g_Effect.patternAnim + 1) % ANIM_PATTERN_NUM;

				if (g_Effect.patternAnim >= TEXTURE_PATTERN_DIVIDE_X - 1)
				{
					g_Effect.use = FALSE;
					ShowDamage();
				}
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEffect(void)
{
	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �}�g���N�X�ݒ�
	SetWorldViewProjection2D();

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	BG *bg = GetBG();

	if (g_Effect.use == TRUE)		// ���̃G�t�F�N�g���g���Ă���H
	{									// Yes
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Effect.texNo]);

		//�G�t�F�N�g�̈ʒu��e�N�X�`���[���W�𔽉f
		float px = g_Effect.pos.x;	// �G�t�F�N�g�̕\���ʒuX
		float py = g_Effect.pos.y;	// �G�t�F�N�g�̕\���ʒuY
		float pw = g_Effect.w;		// �G�t�F�N�g�̕\����
		float ph = g_Effect.h;		// �G�t�F�N�g�̕\������

		float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X;	// �e�N�X�`���̕�
		float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// �e�N�X�`���̍���
		float tx = (float)(g_Effect.patternAnim % TEXTURE_PATTERN_DIVIDE_X) * tw;	// �e�N�X�`���̍���X���W
		float ty = (float)(g_Effect.patternAnim / TEXTURE_PATTERN_DIVIDE_X) * th;	// �e�N�X�`���̍���Y���W

		if (g_Effect.texNo == LIMIT_BREAK_EFFECT)
		{
			pw = ULTIMATE_TEXTURE_WIDTH;		// �G�t�F�N�g�̕\����
			ph = ULTIMATE_TEXTURE_HEIGHT;		// �G�t�F�N�g�̕\������

			tw = 1.0f / ULTIMATE_TEXTURE_PATTERN_DIVIDE_X;	// �e�N�X�`���̕�
			th = 1.0f / ULTIMATE_TEXTURE_PATTERN_DIVIDE_Y;	// �e�N�X�`���̍���
			tx = (float)(g_Effect.patternAnim % ULTIMATE_TEXTURE_PATTERN_DIVIDE_X) * tw;	// �e�N�X�`���̍���X���W
			ty = (float)(g_Effect.patternAnim / ULTIMATE_TEXTURE_PATTERN_DIVIDE_X) * th;	// �e�N�X�`���̍���Y���W
		}

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteColorRotation(g_VertexBuffer,
			px, py, pw, ph,
			tx, ty, tw, th,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			g_Effect.rot.z);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}


}


//=============================================================================
// �G�t�F�N�g�\���̂̐擪�A�h���X���擾
//=============================================================================
EFFECT *GetEffect(void)
{
	return &g_Effect;
}


//=============================================================================
// �G�t�F�N�g�̐ݒ�
//=============================================================================
void SetEffect(D3DXVECTOR3 pos, int texNo)
{
	DAMAGE *damage = GetDamageAddress();

	if (damage->damage > 0)
	{
		switch (texNo)
		{
		case ATTACK_EFFECT:
			PlaySound(SOUND_LABEL_SE_hit);
			break;
		case ENEMY_ATTACK_EFFECT:
			PlaySound(SOUND_LABEL_SE_enemyHit);
			break;
		case FIRE_EFFECT:
			PlaySound(SOUND_LABEL_SE_explode);
			break;
		case HEAL_EFFECT:
			PlaySound(SOUND_LABEL_SE_cure);
			break;
		default:
			break;
		}
	}

	g_Effect.pos = pos;
	g_Effect.texNo = texNo;
	g_Effect.use = TRUE;
}

