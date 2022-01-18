//=============================================================================
//
// �Q�[���N���A��ʏ��� [GameClear.cpp]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#include "gameClear.h"
#include "sprite.h"
#include "input.h"
#include "score.h"
#include "fade.h"
#include "sound.h"
#include "player.h"
#include "iconWM.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// �w�i�T�C�Y
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// 
#define TEXTURE_MAX					(2)				// �e�N�X�`���̐�


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/gameEnd.png",
};


static BOOL			g_Load = FALSE;		// ���������s�������̃t���O
static GAMECLEAR	g_GameClear;

// ����������
//=============================================================================
HRESULT InitGameClear(void)
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
	g_GameClear.w     = TEXTURE_WIDTH;
	g_GameClear.h     = TEXTURE_HEIGHT;
	g_GameClear.pos   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_GameClear.texNo = 0;

	g_Load = TRUE;	// �f�[�^�̏��������s����
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitGameClear(void)
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
void UpdateGameClear(void)
{
	PLAYER *player = GetPlayer();

	if (GetKeyboardTrigger(DIK_RETURN))
	{// Enter��������A�X�e�[�W��؂�ւ���
		PlaySound(SOUND_LABEL_SE_click);
		SetFade(FADE_OUT, MODE_TITLE);

		// ���Z�b�g���܂�
		player->isAlive = FALSE;
		player->currentLocation = 0;

		for (int i = 0; i < ICON_MAX; i++)
		{
			ICONWM *icon = GetIconWM(i);

			if (i != 0)
				icon->use = FALSE;
		}
	}
	// �Q�[���p�b�h�œ��͏���
	else if (IsButtonTriggered(0, BUTTON_B))
	{
		PlaySound(SOUND_LABEL_SE_click);

		SetFade(FADE_OUT, MODE_TITLE);

		player->isAlive = FALSE;
		player->currentLocation = 0;

		for (int i = 0; i < ICON_MAX; i++)
		{
			ICONWM *icon = GetIconWM(i);

			if (i != 0)
				icon->use = FALSE;
		}

	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawGameClear(void)
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

	// �Q�[���N���A�̔w�i��`��
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_GameClear.texNo]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteLTColor(g_VertexBuffer,
			g_GameClear.pos.x, g_GameClear.pos.y, g_GameClear.w, g_GameClear.h, 
			0.0f, 0.0f, 1.0f, 1.0f,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}
}





