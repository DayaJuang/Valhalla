//=============================================================================
//
// WM��ʏ��� [cursorWM.cpp]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#include "cursorWM.h"
#include "iconWM.h"
#include "main.h"
#include "sprite.h"
#include "player.h"
#include "input.h"
#include "fade.h"
#include "layer.h"
#include "sound.h"
#include "tutorial.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
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
	"data/TEXTURE/cursor.png"
};

static BOOL	g_Load = FALSE;		// ���������s�������̃t���O
static CURSORWM g_Cursor;
static int currentLocation;
static BOOL firstTime = TRUE;


// ����������
//=============================================================================
HRESULT InitCursorWM(void)
{
	ID3D11Device *pDevice = GetDevice();
	currentLocation = checkCurrentMap();

	ICONWM *icon = GetIconWM(currentLocation);


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

	g_Cursor.w = 50;
	g_Cursor.h = 50;

	g_Cursor.texNo = 0;

	g_Cursor.pos = D3DXVECTOR3(icon->pos.x - 35.0f, icon->pos.y + 10.0f, 0.0f);

	g_Load = TRUE;	// �f�[�^�̏��������s����
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitCursorWM(void)
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
void UpdateCursorWM(void)
{
	TUTORIAL *tutorial = GetTutorial();

	if (firstTime)
	{
		firstTime = FALSE;
		ShowTutorial(WORLDMAP_TUTORIAL1, WORLDMAP_TUTORIAL);
	}
	else if (!tutorial->use)
	{
		int old_currentLocation = currentLocation;

		if (GetKeyboardTrigger(DIK_RIGHT) || GetKeyboardTrigger(DIK_LEFT))
		{
			PlaySound(SOUND_LABEL_SE_click);

			currentLocation = (currentLocation + 1) % 2;
			ICONWM *icon = GetIconWM(currentLocation);

			if (icon->use == FALSE)
			{
				currentLocation = old_currentLocation;
				return;
			}

			g_Cursor.pos = g_Cursor.pos = D3DXVECTOR3(icon->pos.x - 35.0f, icon->pos.y, 0.0f);
		}

		if (GetKeyboardTrigger(DIK_RETURN))
		{
			PlaySound(SOUND_LABEL_SE_click);

			PLAYER *player = GetPlayer();
			player->currentLocation = currentLocation;
			SetFade(FADE_OUT, MODE_GAME);
		}
	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawCursorWM(void)
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

		// WM�̔w�i��`��
		{
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Cursor.texNo]);

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteLTColor(g_VertexBuffer,
				g_Cursor.pos.x, g_Cursor.pos.y, g_Cursor.w, g_Cursor.h,
				0.0f, 0.0f, 1.0f, 1.0f,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}

}




