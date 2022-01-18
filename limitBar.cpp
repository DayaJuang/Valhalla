//=============================================================================
//
// �o�[���� [Bar.cpp]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#include "limitBar.h"
#include "sprite.h"
#include "playerUI.h"
#include "battle.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(120)		// �����̃T�C�Y
#define TEXTURE_HEIGHT				(12)	// 
#define TEXTURE_MAX					(2)			// �e�N�X�`���̐�


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[] = {
	"data/BATTLE_UI/ultimateBar.png",
	"data/BATTLE_UI/limitBarMax.png",
};

static BOOL		g_Load = FALSE;		// ���������s�������̃t���O
static BAR		g_Bar;

static PLAYERUI *playerUI = GetPlayerUI();

//=============================================================================
// ����������
//=============================================================================
HRESULT InitBar(void)
{
	ID3D11Device *pDevice = GetDevice();
	PLAYER *player = GetPlayer();

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
	g_Bar.w = player->limitBreak;
	g_Bar.h = TEXTURE_HEIGHT;
	g_Bar.texNo = 0;

	g_Bar.use = TRUE;

	g_Bar.pos = D3DXVECTOR3(playerUI->pos.x + 10.0f, playerUI->pos.y + 110.0f, 0.0f);

	g_Load = TRUE;		// �f�[�^�̏��������s����
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBar(void)
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
void UpdateBar(void)
{
	PLAYER *player = GetPlayer();
	g_Bar.pos = D3DXVECTOR3(playerUI->pos.x + 10.0f, playerUI->pos.y + 110.0f, 0.0f);

	g_Bar.w = player->limitBreak;

	if (g_Bar.w >= WIDTH_MAX)
	{
		g_Bar.w = WIDTH_MAX;
		g_Bar.texNo = 1;
	}
	else
	{
		g_Bar.texNo = 0;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBar(void)
{
	if (playerUI->use == TRUE)
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
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Bar.texNo]);

		// �o�[�̈ʒu��e�N�X�`���[���W�𔽉f
		float px = g_Bar.pos.x;				// �o�[�̕\���ʒuX
		float py = g_Bar.pos.y;				// �o�[�̕\���ʒuY
		float pw = g_Bar.w;					// �o�[�̕\����
		float ph = g_Bar.h;					// �o�[�̕\������

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteLTColor(g_VertexBuffer,
			px, py, pw, ph,
			0.0f, 0.0f, 1.0f, 1.0f,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);

	}
}

void AddLimitBar(int point)
{
	PLAYER *player = GetPlayer();

	player->limitBreak += point;
}


//=============================================================================
// �o�[�����Z����
// ����:add :�ǉ�����_���B�}�C�i�X���\
//=============================================================================
BAR *GetBar(void)
{
	return &g_Bar;
}

