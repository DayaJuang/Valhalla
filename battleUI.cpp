//=============================================================================
//
// BattleUI��ʏ��� [BattleUI.cpp]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#include "battleUI.h"
#include "main.h"
#include "sprite.h"
#include "player.h"
#include "bg.h"
#include "battle.h"
#include "battleCursor.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(140)	// �L�����T�C�Y
#define TEXTURE_HEIGHT				(140)	// 

#define TEXTURE_MAX					(3)						// �e�N�X�`���̐�


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[TEXTURE_MAX] = {
	"data/BATTLE_UI/battleUI2.png",
	"data/BATTLE_UI/magicUI2.png",
	"data/BATTLE_UI/powerUPLog.png",
};

static BOOL	g_Load = FALSE;		// ���������s�������̃t���O
static BATTLEUI	g_BattleUI;
static D3DXVECTOR3 positions[3] =
{
	D3DXVECTOR3(30.0f, 390.0f, 0.0f),
	D3DXVECTOR3(670.0f, 390.0f, 0.0f),
	D3DXVECTOR3((SCREEN_WIDTH / 2) - (TEXTURE_WIDTH / 2), SCREEN_HEIGHT / 2, 0.0f)
};


// ����������
//=============================================================================
HRESULT InitBattleUI(void)
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

	g_BattleUI.w = TEXTURE_WIDTH;
	g_BattleUI.h = TEXTURE_HEIGHT;

	g_BattleUI.texNo = 0;
	g_BattleUI.pos = positions[0];

	g_BattleUI.use = FALSE;

	g_Load = TRUE;	// �f�[�^�̏��������s����
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBattleUI(void)
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
void UpdateBattleUI(void)
{
	PLAYER *player = GetPlayer();
	BATTLE *battlePlayer = GetBattlePlayer();
	BATTLE *enemy = GetBattleEnemy();
	BATTLECURSOR *cursor = GetBattleCursor();
	BG *bg = GetBG();

	if (g_BattleUI.texNo == POWERUP_UI)
	{
		cursor->canDoAction = TRUE;
		cursor->mode = POWERUP_SELECT;
		g_BattleUI.pos = positions[POWER_UP_PANEL];
		return;
	}
	else if (player->mode != PLAYER_MODE_BATTLE && (enemy->hp <= 0 || battlePlayer->hp <= 0))
	{
		g_BattleUI.use = FALSE;
	}
	else
	{
		// �v���C���[��G�l�~�[�̍��W����ʂ̉E�ɂ��邩�A���ɂ��邩
		if ((((player->pos.x + bg->pos.x)) < SCREEN_WIDTH / 2 && ((player->pos.y + bg->pos.y)) > SCREEN_HEIGHT / 2)
			|| (((enemy->pos.x + bg->pos.x)) < SCREEN_WIDTH / 2 && ((enemy->pos.y + bg->pos.y)) > SCREEN_HEIGHT / 2))
		{
			g_BattleUI.pos = positions[BATTLE_UI_RIGHT];
		}
		else
		{
			g_BattleUI.pos = positions[BATTLE_UI_LEFT];
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBattleUI(void)
{
	if (g_BattleUI.use == TRUE)
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

		// BattleUI�̔w�i��`��
		{
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_BattleUI.texNo]);

			float px = g_BattleUI.pos.x;	// �\���ʒuX
			float py = g_BattleUI.pos.y;	// �\���ʒuY
			float pw = g_BattleUI.w;		// �\����
			float ph = g_BattleUI.h;		// �\������

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

void ShowBattleUI(int texNo)
{
	BATTLECURSOR *cursor = GetBattleCursor();

	g_BattleUI.texNo = texNo;
	g_BattleUI.use = TRUE;
	cursor->posIndex = 0;
}

//=============================================================================
// BattleUI�\���̂̐擪�A�h���X���擾
//=============================================================================
BATTLEUI *GetBattleUI(void)
{
	return &g_BattleUI;
}





