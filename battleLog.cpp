//=============================================================================
//
// �o�g�����O���� [battleLog.cpp]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#include "battleLog.h"
#include "sprite.h"
#include "limitBar.h"
#include "battleUI.h"
#include "input.h"
#include "battle.h"
#include "battleCursor.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(200)		// �����̃T�C�Y
#define TEXTURE_HEIGHT				(45) 
#define TEXTURE_MAX					(LOG_MAX)	// �e�N�X�`���̐�

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[] = {
	"data/BATTLE_UI/escape_Success.png",
	"data/BATTLE_UI/escape_Failed.png",
	"data/BATTLE_UI/enemyKilled.png",
	"data/BATTLE_UI/maxHP_up.png",
	"data/BATTLE_UI/maxMP_up.png",
	"data/BATTLE_UI/attackUp.png",
	"data/BATTLE_UI/recoverHPMP.png",
};

static BOOL		g_Load = FALSE;		// ���������s�������̃t���O
BATTLELOG			g_BattleLog;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitBattleLog(void)
{
	ID3D11Device *pDevice = GetDevice();
	BATTLE *player = GetBattlePlayer();

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
	g_BattleLog.w = TEXTURE_WIDTH;
	g_BattleLog.h = TEXTURE_HEIGHT;
	g_BattleLog.texNo = 0;

	g_BattleLog.use = FALSE;

	g_BattleLog.pos = D3DXVECTOR3((SCREEN_WIDTH / 2) - (g_BattleLog.w / 2), SCREEN_HEIGHT/2, 0.0f);

	g_Load = TRUE;		// �f�[�^�̏��������s����
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBattleLog(void)
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
void UpdateBattleLog(void)
{
	BATTLECURSOR *cursor = GetBattleCursor();

	if (g_BattleLog.use == TRUE)
	{
		if (GetKeyboardTrigger(DIK_RETURN))
		{
			g_BattleLog.use = FALSE;

			PlaySound(SOUND_LABEL_SE_click);

			switch (g_BattleLog.texNo)
			{
			case ESSCAPE_SUCCESS:
				Escape(ESSCAPE_SUCCESS);
				break;

			case ESCAPE_FAIL:
				Escape(ESCAPE_FAIL);
				break;
			case ENEMY_KILLED:
				ShowBattleUI(POWERUP_UI);
				cursor->canDoAction;
				break;
			default:
				BattleEnd();
				break;
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBattleLog(void)
{
	BATTLEUI *battleUI = GetBattleUI();

	if (g_BattleLog.use == TRUE)
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
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_BattleLog.texNo]);

		// �o�g�����O�̈ʒu��e�N�X�`���[���W�𔽉f
		float px = g_BattleLog.pos.x;				// �o�g�����O�̕\���ʒuX
		float py = g_BattleLog.pos.y;				// �o�g�����O�̕\���ʒuY
		float pw = g_BattleLog.w;					// �o�g�����O�̕\����
		float ph = g_BattleLog.h;					// �o�g�����O�̕\������

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteLTColor(g_VertexBuffer,
			px, py, pw, ph,
			0.0f, 0.0f, 1.0f, 1.0f,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);

	}
}

void ShowBattleLog(int texNo)
{
	g_BattleLog.texNo = texNo;
	g_BattleLog.use = TRUE;
}

BATTLELOG *GetBattleLog(void)
{
	return &g_BattleLog;
}

