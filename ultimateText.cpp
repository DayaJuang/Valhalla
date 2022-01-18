//=============================================================================
//
// �o�[���� [UText.cpp]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#include "ultimateText.h"
#include "sprite.h"
#include "limitBar.h"
#include "battleUI.h"
#include "playerUI.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(100)		// �����̃T�C�Y
#define TEXTURE_HEIGHT				(28) 
#define TEXTURE_MAX					(1)			// �e�N�X�`���̐�

#define TEXTURE_PATTERN_DIVIDE_X	(4)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y	(1)		// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define ANIM_WAIT					(16)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[] = {
	"data/TEXTURE/ultimateText.png",
};

static BOOL		g_Load = FALSE;		// ���������s�������̃t���O
UTEXT			g_UText;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitUltimateText(void)
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

	// �ϐ��̏�����
	g_UText.w = TEXTURE_WIDTH;
	g_UText.h = TEXTURE_HEIGHT;
	g_UText.texNo = 0;
	g_UText.countAnim = 0;
	g_UText.patternAnim = 0;

	g_UText.use = TRUE;

	g_UText.pos = D3DXVECTOR3(battleUI->pos.x + 10.0f, battleUI->pos.y - 40.0f, 0.0f);

	g_Load = TRUE;		// �f�[�^�̏��������s����
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitUltimateText(void)
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
void UpdateUltimateText(void)
{
	BAR *limitBar = GetBar();
	BATTLEUI *battleUI = GetBattleUI();
	PLAYERUI *playerUI = GetPlayerUI();

	g_UText.pos = D3DXVECTOR3(battleUI->pos.x + 10.0f, battleUI->pos.y - 40.0f, 0.0f);

	if (limitBar->w >= WIDTH_MAX && playerUI->use == TRUE)
		g_UText.use = TRUE;
	else
		g_UText.use = FALSE;

	if (g_UText.use == TRUE)
	{
		// �A�j���[�V����  
		g_UText.countAnim++;
		if ((g_UText.countAnim % ANIM_WAIT) == 0)
		{
			g_UText.countAnim = 0;
			g_UText.patternAnim = ((g_UText.patternAnim + 1) % ANIM_PATTERN_NUM);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawUltimateText(void)
{
	BATTLEUI *battleUI = GetBattleUI();

	if (g_UText.use == TRUE)
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
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_UText.texNo]);

		// �o�[�̈ʒu��e�N�X�`���[���W�𔽉f
		float px = g_UText.pos.x;				// �o�[�̕\���ʒuX
		float py = g_UText.pos.y;				// �o�[�̕\���ʒuY
		float pw = g_UText.w;					// �o�[�̕\����
		float ph = g_UText.h;					// �o�[�̕\������

		float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X;	// �e�N�X�`���̕�
		float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// �e�N�X�`���̍���
		float tx = (float)(g_UText.patternAnim % TEXTURE_PATTERN_DIVIDE_X) * tw;		// �e�N�X�`���̍���X���W
		float ty = (float)(g_UText.patternAnim / TEXTURE_PATTERN_DIVIDE_X) * th;		// �e�N�X�`���̍���Y���W

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteLTColor(g_VertexBuffer,
			px, py, pw, ph,
			 tx, ty, tw, th,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);

	}
#ifdef _DEBUG
	// �f�o�b�O�\��
	PrintDebugProc("UText X:%f Y:%f \n", g_UText.pos.x, g_UText.pos.y);
	PrintDebugProc("battleUI X:%f Y:%f \n", battleUI->pos.x, battleUI->pos.y);
#endif

}

//=============================================================================
// �o�[�����Z����
// ����:add :�ǉ�����_���B�}�C�i�X���\
//=============================================================================
UTEXT *GetUltimateText(void)
{
	return &g_UText;
}

