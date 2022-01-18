//=============================================================================
//
// �o�[���� [Bar.cpp]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#include "actionBar.h"
#include "actionBarFrame.h"
#include "sprite.h"
#include "playerUI.h"
#include "battle.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_HEIGHT				(8)		// 
#define TEXTURE_MAX					(1)			// �e�N�X�`���̐�


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[] = {
	"data/BATTLE_UI/actionBar.png",
};

static BOOL		g_Load = FALSE;					// ���������s�������̃t���O
static ACTIONBAR	g_ActionBar[TURN_MAX];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitActionBar(void)
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
		g_ActionBar[i].w = 0.0f;
		g_ActionBar[i].h = TEXTURE_HEIGHT;
		g_ActionBar[i].texNo = 0;
		g_ActionBar[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_ActionBar[i].use = FALSE;

	}

	g_Load = TRUE;		// �f�[�^�̏��������s����
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitActionBar(void)
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
void UpdateActionBar(void)
{
	BATTLE *player = GetBattlePlayer();
	BATTLE *enemy = GetBattleEnemy();

	ACTIONBARFRAME *playerFrame = GetActionBarFrame(PLAYER1_FRAME);
	ACTIONBARFRAME *enemyFrame = GetActionBarFrame(ENEMY1_FRAME);

	g_ActionBar[PLAYER1_BAR].use = playerFrame->use;
	g_ActionBar[ENEMY1_BAR].use = enemyFrame->use;

	g_ActionBar[PLAYER1_BAR].pos = D3DXVECTOR3(playerFrame->pos.x + 2.5f, playerFrame->pos.y, 0.0f);;
	g_ActionBar[ENEMY1_BAR].pos = D3DXVECTOR3(enemyFrame->pos.x + 2.5f, enemyFrame->pos.y, 0.0f);

	g_ActionBar[PLAYER1_BAR].w = player->action;
	g_ActionBar[ENEMY1_BAR].w = enemy->action;
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawActionBar(void)
{
	for (int i = 0; i < TURN_MAX; i++)
	{
		if (g_ActionBar[i].use == TRUE)
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
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_ActionBar[i].texNo]);

			// �o�[�̈ʒu��e�N�X�`���[���W�𔽉f
			float px = g_ActionBar[i].pos.x;				// �o�[�̕\���ʒuX
			float py = g_ActionBar[i].pos.y;				// �o�[�̕\���ʒuY
			float pw = g_ActionBar[i].w;					// �o�[�̕\����
			float ph = g_ActionBar[i].h;					// �o�[�̕\������

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

