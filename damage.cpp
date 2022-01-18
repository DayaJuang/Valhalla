//=============================================================================
//
// �_���[�W���� [damage.cpp]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#include "damage.h"
#include "sprite.h"
#include "battle.h"
#include "bg.h"
#include "layer.h"
#include "bubble.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(23)	// �����̃T�C�Y
#define TEXTURE_HEIGHT				(23)
#define TEXTURE_MAX					(2)		// �e�N�X�`���̐�


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[] = {
	"data/TEXTURE/number.png",
	"data/TEXTURE/missText.png",
};

static BOOL		g_Load = FALSE;		// ���������s�������̃t���O
static DAMAGE	g_Damage;

static float floatTime = 0;


//=============================================================================
// ����������
//=============================================================================
HRESULT InitDamage(void)
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


	// �ϐ��̏�����
	g_Damage.w = TEXTURE_WIDTH;
	g_Damage.h = TEXTURE_HEIGHT;
	g_Damage.pos = D3DXVECTOR3(DAMAGE_DX, DAMAGE_DY, 0.0f);
	g_Damage.texNo = 0;
	g_Damage.digit = 0;
	g_Damage.damage = 0;	// ���_��������

	g_Damage.alpha = 1.0f;
	g_Damage.use = FALSE;

	g_Load = TRUE;		// �f�[�^�̏��������s����
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitDamage(void)
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
void UpdateDamage(void)
{
	BG *bg = GetBG();
	BUBBLE *bubble = GetBubble();
	LAYER *layer = GetLayer();
	BATTLE *player = GetBattlePlayer();
	BATTLE *enemy = GetBattleEnemy();

	// �_���[�W�̕\��
	if (g_Damage.use == TRUE)
	{
		floatTime++;

		if (floatTime >= 60)
		{
			g_Damage.use = FALSE;
			bubble->use = FALSE;
			floatTime = 0;
			player->stop = enemy->stop = FALSE;

			bg->r = bg->g = bg->b = layer->r = layer->g = layer->b = 1.0f;

			CheckBattleResult();
		}
	}
#ifdef _DEBUG
	// �f�o�b�O�\��
	PrintDebugProc("Damage :%d TexNo:%d %d\n", g_Damage.damage, g_Damage.texNo, g_Damage.use);

#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawDamage(void)
{
	if (g_Damage.use == TRUE)
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

		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Damage.texNo]);

		// ��������������
		int number = g_Damage.damage;
		int digit = g_Damage.digit;

		if (g_Damage.texNo == 1)
			digit = 1;

		for (int i = 0; i < digit; i++)
		{
			// ����\�����錅�̐���
			float x = (float)(number % 10);

			// ���̌���
			number /= 10;

			// �_���[�W�̈ʒu��e�N�X�`���[���W�𔽉f
			float px = g_Damage.pos.x - (g_Damage.w/1.5f)*i;	// �_���[�W�̕\���ʒuX
			float py = g_Damage.pos.y;							// �_���[�W�̕\���ʒuY
			float pw = g_Damage.w;								// �_���[�W�̕\����
			float ph = g_Damage.h;								// �_���[�W�̕\������

			float tw = 1.0f / 10;					// �e�N�X�`���̕�
			float th = 1.0f / 1;					// �e�N�X�`���̍���
			float tx = x * tw;						// �e�N�X�`���̍���X���W
			float ty = 0.0f;						// �e�N�X�`���̍���Y���W

			if (g_Damage.texNo == 1)
			{
				px = g_Damage.pos.x;	// �_���[�W�̕\���ʒuX
				py = g_Damage.pos.y;	// �_���[�W�̕\���ʒuY
				pw = g_Damage.w * 1.5f;		// �_���[�W�̕\����
				ph = g_Damage.h * 1.5f;		// �_���[�W�̕\������

				tw = 1.0f;					// �e�N�X�`���̕�
				th = 1.0f;					// �e�N�X�`���̍���
				tx = 0.0f;					// �e�N�X�`���̍���X���W
				ty = 0.0f;					// �e�N�X�`���̍���Y���W
			}

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteLTColor(g_VertexBuffer,
				px, py, pw, ph,
				tx, ty, tw, th,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Damage.alpha));

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}
	}

}

int GetDamage(void)
{
	return g_Damage.damage;
}

void SetDamageDigit(int damage)
{
	int digit = 0;

	while (damage != 0)
	{
		damage /= 10;

		++digit;
	}

	g_Damage.digit = digit;

}

void ShowDamage(void)
{
	g_Damage.use = TRUE;
}

void SetDamagePosition(D3DXVECTOR3 targetPos)
{
	g_Damage.pos = targetPos;
}

void SetDamage(int damage)
{
	g_Damage.damage = damage;

	if (damage == 0)
	{
		g_Damage.texNo = 1;
	}
	else
	{
		g_Damage.texNo = 0;
	}
}

DAMAGE *GetDamageAddress(void)
{
	return &g_Damage;
}

