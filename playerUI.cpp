//=============================================================================
//
// PlayerUI��ʏ��� [playerUI.cpp]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#include "playerUI.h"
#include "main.h"
#include "sprite.h"
#include "battleUI.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(140)	// �L�����T�C�Y
#define TEXTURE_HEIGHT				(140)	// 

#define TEXTURE_MAX					(1)						// �e�N�X�`���̐�


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[TEXTURE_MAX] = {
	"data/BATTLE_UI/playerDetails.png"

};

static BOOL	g_Load = FALSE;		// ���������s�������̃t���O
static PLAYERUI	g_PlayerUI;


// ����������
//=============================================================================
HRESULT InitPlayerUI(void)
{
	ID3D11Device *pDevice = GetDevice();
	BATTLEUI *battleUI = GetBattleUI();

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

	g_PlayerUI.w = TEXTURE_WIDTH;
	g_PlayerUI.h = TEXTURE_HEIGHT;

	g_PlayerUI.texNo = 0;
	g_PlayerUI.pos = D3DXVECTOR3(battleUI->pos.x + 140.0f, battleUI->pos.y, 0.0f);

	g_PlayerUI.use = FALSE;

	g_Load = TRUE;	// �f�[�^�̏��������s����
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayerUI(void)
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
void UpdatePlayerUI(void)
{
	BATTLEUI *battleUI = GetBattleUI();

	g_PlayerUI.use = battleUI->use;

	if (battleUI->texNo == POWERUP_UI)
		g_PlayerUI.use = FALSE;

	g_PlayerUI.pos = D3DXVECTOR3(battleUI->pos.x + 140.0f, battleUI->pos.y, 0.0f);
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayerUI(void)
{
	if (g_PlayerUI.use == TRUE)
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

		// PlayerUI�̔w�i��`��
		{
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_PlayerUI.texNo]);

			float px = g_PlayerUI.pos.x;	// �\���ʒuX
			float py = g_PlayerUI.pos.y;	// �\���ʒuY
			float pw = g_PlayerUI.w;		// �\����
			float ph = g_PlayerUI.h;		// �\������

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

//=============================================================================
// PlayerUI�\���̂̐擪�A�h���X���擾
//=============================================================================
PLAYERUI *GetPlayerUI(void)
{
	return &g_PlayerUI;
}





