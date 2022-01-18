//=============================================================================
//
// �o�g�����O���� [Tutorial.cpp]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#include "tutorial.h"
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
#define TEXTURE_WIDTH				(840)		// �����̃T�C�Y
#define TEXTURE_HEIGHT				(480) 
#define TEXTURE_MAX					(FIELD_TUTORIAL_MAX + BATTLE_TUTORIAL_MAX + WORLDMAP_TUTORIAL_MAX)	// �e�N�X�`���̐�

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[] = {
	"data/TUTORIAL/fieldTutorial.png",
	"data/TUTORIAL/fieldTutorial2.png",
	"data/TUTORIAL/battleTutorial1.png",
	"data/TUTORIAL/battleTutorial2.png",
	"data/TUTORIAL/battleTutorial3.png",
	"data/TUTORIAL/worldMapTutorial.png",
};

static BOOL		g_Load = FALSE;		// ���������s�������̃t���O
TUTORIAL		g_Tutorial;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitTutorial(void)
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
	g_Tutorial.w = TEXTURE_WIDTH;
	g_Tutorial.h = TEXTURE_HEIGHT;
	g_Tutorial.texNo = 0;

	g_Tutorial.use = FALSE;

	g_Tutorial.pos = D3DXVECTOR3(60.0f, 30.0f, 0.0f);

	g_Load = TRUE;		// �f�[�^�̏��������s����
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitTutorial(void)
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
void UpdateTutorial(void)
{
	PLAYER *player = GetPlayer();

	if (g_Tutorial.use == TRUE)
	{
		if (GetKeyboardTrigger(DIK_RETURN))
		{			
			PlaySound(SOUND_LABEL_SE_click);

			if (g_Tutorial.flag == FIELD_TUTORIAL)
			{
				g_Tutorial.texNo++;

				if (g_Tutorial.texNo > FIELD_TUTORIAL_MAX - 1)
				{
					g_Tutorial.use = FALSE;
					player->canMove = TRUE;
				}
			}
			else if (g_Tutorial.flag == BATTLE_TUTORIAL)
			{
				g_Tutorial.texNo++;

				if (g_Tutorial.texNo > (BATTLE_TUTORIAL_MAXINDEX) - 1)
				{
					g_Tutorial.use = FALSE;
				}
			}
			else
			{
				g_Tutorial.use = FALSE;
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTutorial(void)
{
	BATTLEUI *battleUI = GetBattleUI();

	if (g_Tutorial.use == TRUE)
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
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Tutorial.texNo]);

		// �o�g�����O�̈ʒu��e�N�X�`���[���W�𔽉f
		float px = g_Tutorial.pos.x;				// �o�g�����O�̕\���ʒuX
		float py = g_Tutorial.pos.y;				// �o�g�����O�̕\���ʒuY
		float pw = g_Tutorial.w;					// �o�g�����O�̕\����
		float ph = g_Tutorial.h;					// �o�g�����O�̕\������

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteLTColor(g_VertexBuffer,
			px, py, pw, ph,
			0.0f, 0.0f, 1.0f, 1.0f,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);

	}
}

void ShowTutorial(int texNo, int flag)
{
	g_Tutorial.texNo = texNo;
	g_Tutorial.use = TRUE;
	g_Tutorial.flag = flag;
}

TUTORIAL *GetTutorial(void)
{
	return &g_Tutorial;
}

