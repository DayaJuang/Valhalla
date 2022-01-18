//=============================================================================
//
// �X�R�A���� [status.cpp]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#include "status.h"
#include "sprite.h"
#include "playerUI.h"
#include "battle.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(20)	// �����̃T�C�Y
#define TEXTURE_HEIGHT				(20)	// 
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
	"data/TEXTURE/number.png",
};

static BOOL		g_Load = FALSE;		// ���������s�������̃t���O
static STATUS	g_Status[STATUS_MAX_INDEX];

static PLAYERUI *playerUI = GetPlayerUI();

//=============================================================================
// ����������
//=============================================================================
HRESULT InitStatus(void)
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

	for (int i = 0; i < STATUS_MAX_INDEX; i++)
	{
		// �ϐ��̏�����
		g_Status[i].w = TEXTURE_WIDTH;
		g_Status[i].h = TEXTURE_HEIGHT;
		g_Status[i].texNo = 0;
		g_Status[i].digit = 3;
		g_Status[i].status = 0;	// ���_��������

		g_Status[i].alpha = 1.0f;
		g_Status[i].use = FALSE;
	}

	g_Status[STATUS_HP].pos = D3DXVECTOR3(playerUI->pos.x + 100.0f, playerUI->pos.y + 13.0f, 0.0f);
	g_Status[STATUS_MP].pos = D3DXVECTOR3(playerUI->pos.x + 100.0f, playerUI->pos.y + 63.0f, 0.0f);

	g_Load = TRUE;		// �f�[�^�̏��������s����
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitStatus(void)
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
void UpdateStatus(void)
{
	BATTLE *player = GetBattlePlayer();

	g_Status[STATUS_HP].status = player->hp;
	g_Status[STATUS_MP].status = player->mp;

	g_Status[STATUS_HP].pos = D3DXVECTOR3(playerUI->pos.x + 100.0f, playerUI->pos.y + 13.0f, 0.0f);
	g_Status[STATUS_MP].pos = D3DXVECTOR3(playerUI->pos.x + 100.0f, playerUI->pos.y + 63.0f, 0.0f);

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawStatus(void)
{
	for (int i = 0; i < STATUS_MAX_INDEX; i++)
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
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Status[i].texNo]);

			// ��������������

			for (int j = 0; j < g_Status[i].digit; j++)
			{
				// ����\�����錅�̐���
				float x = (float)(g_Status[i].status % 10);

				// ���̌���
				g_Status[i].status /= 10;

				// �X�R�A�̈ʒu��e�N�X�`���[���W�𔽉f
				float px = g_Status[i].pos.x - g_Status[i].w * j;	// �X�R�A�̕\���ʒuX
				float py = g_Status[i].pos.y;				// �X�R�A�̕\���ʒuY
				float pw = g_Status[i].w;					// �X�R�A�̕\����
				float ph = g_Status[i].h;					// �X�R�A�̕\������

				float tw = 1.0f / 10;					// �e�N�X�`���̕�
				float th = 1.0f / 1;					// �e�N�X�`���̍���
				float tx = x * tw;						// �e�N�X�`���̍���X���W
				float ty = 0.0f;						// �e�N�X�`���̍���Y���W

				// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
				SetSpriteLTColor(g_VertexBuffer,
					px, py, pw, ph,
					tx, ty, tw, th,
					D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Status[i].alpha));

				// �|���S���`��
				GetDeviceContext()->Draw(4, 0);
			}
		}
	}
}


//=============================================================================
// �X�R�A�����Z����
// ����:add :�ǉ�����_���B�}�C�i�X���\
//=============================================================================
STATUS *GetStatusAddress(void)
{
	return &g_Status[0];
}

