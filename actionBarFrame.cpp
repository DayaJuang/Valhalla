//=============================================================================
//
// �o�[���� [Bar.cpp]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#include "actionBarFrame.h"
#include "actionBar.h"
#include "sprite.h"
#include "battle.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(ACTION_BAR_MAX)		// �����̃T�C�Y
#define TEXTURE_HEIGHT				(8) 
#define TEXTURE_MAX					(1)						// �e�N�X�`���̐�


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[] = {
	"data/BATTLE_UI/actioBarFrame.png",
};

static BOOL		g_Load = FALSE;					// ���������s�������̃t���O
static ACTIONBARFRAME	g_BarFrame[TURN_MAX];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitActionBarFrame(void)
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

	for (int i = 0; i < TURN_MAX; i++)
	{
		// �ϐ��̏�����
		g_BarFrame[i].w = TEXTURE_WIDTH;
		g_BarFrame[i].h = TEXTURE_HEIGHT;
		g_BarFrame[i].texNo = 0;
		g_BarFrame[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_BarFrame[i].use = FALSE;

	}

	g_Load = TRUE;		// �f�[�^�̏��������s����
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitActionBarFrame(void)
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
void UpdateActionBarFrame(void)
{
	PLAYER *player = GetPlayer();
	BATTLE *battlePlayer = GetBattlePlayer();
	BATTLE *enemy = GetBattleEnemy();

	for (int i = 0; i < TURN_MAX; i++)
	{
		if (player->battleReady && battlePlayer->hp > 0 && enemy->hp > 0)
		{
			g_BarFrame[i].use = TRUE;
		}
		else
		{
			g_BarFrame[i].use = FALSE;
		}
	}

	g_BarFrame[PLAYER1_FRAME].pos = D3DXVECTOR3(battlePlayer->pos.x - g_BarFrame[PLAYER1_FRAME].w / 2, battlePlayer->pos.y - 30.0f, 0.0f);
	g_BarFrame[ENEMY1_FRAME].pos = D3DXVECTOR3(enemy->pos.x - g_BarFrame[PLAYER1_FRAME].w / 2, enemy->pos.y - 30.0f, 0.0f);
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawActionBarFrame(void)
{
	for (int i = 0; i < TURN_MAX; i++)
	{
		if (g_BarFrame[i].use == TRUE)
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
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_BarFrame[i].texNo]);

			// �o�[�̈ʒu��e�N�X�`���[���W�𔽉f
			float px = g_BarFrame[i].pos.x;				// �o�[�̕\���ʒuX
			float py = g_BarFrame[i].pos.y;				// �o�[�̕\���ʒuY
			float pw = g_BarFrame[i].w;					// �o�[�̕\����
			float ph = g_BarFrame[i].h;					// �o�[�̕\������

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteLTColor(g_VertexBuffer,
				px, py, pw, ph,
				0.0f, 0.0f, 1.0f, 1.0f,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);

		}
	}

}

ACTIONBARFRAME *GetActionBarFrame(int index)
{
	return &g_BarFrame[index];
}

