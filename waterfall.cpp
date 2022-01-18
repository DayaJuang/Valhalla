//=============================================================================
//
// Waterfall��ʏ��� [waterfall.cpp]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#include "waterfall.h"
#include "main.h"
#include "sprite.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define Waterfall_WIDTH					(SCREEN_WIDTH)					// (SCREEN_WIDTH)
#define Waterfall_HEIGHT				(SCREEN_HEIGHT)					// (SCREEN_HEIGHT)
#define TEXTURE_MAX						(1)								// �e�N�X�`���̐�

#define TEXTURE_PATTERN_DIVIDE_X	(5)													// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y	(1)													// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define ANIM_WAIT					(16)												// �A�j���[�V�����̐؂�ւ��Wait�l



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/waterFall.png"
};


static BOOL	g_Load = FALSE;		// ���������s�������̃t���O
static WATERFALL	g_Waterfall;


// ����������
//=============================================================================
HRESULT InitWaterfall(void)
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
	g_Waterfall.w = Waterfall_WIDTH;
	g_Waterfall.h = Waterfall_HEIGHT;

	g_Waterfall.texNo = checkCurrentMap();
	g_Waterfall.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Waterfall.countAnim = 0;
	g_Waterfall.patternAnim = 0;

	g_Load = TRUE;	// �f�[�^�̏��������s����
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitWaterfall(void)
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
void UpdateWaterfall(void)
{
	// �A�j���[�V����  
	g_Waterfall.countAnim++;
	if ((g_Waterfall.countAnim % ANIM_WAIT) == 0)
	{
		// �p�^�[���̐؂�ւ�
		g_Waterfall.patternAnim = ((g_Waterfall.patternAnim + 1) % TEXTURE_PATTERN_DIVIDE_X);
	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawWaterfall(void)
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

	// Waterfall�̔w�i��`��
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Waterfall.texNo]);

		float px = g_Waterfall.pos.x;	// �\���ʒuX
		float py = g_Waterfall.pos.y;	// �\���ʒuY
		float pw = g_Waterfall.w;		// �\����
		float ph = g_Waterfall.h;		// �\������

		// �A�j���[�V�����p
		float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X;	// �e�N�X�`���̕�
		float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// �e�N�X�`���̍���
		float tx = (float)(g_Waterfall.patternAnim % TEXTURE_PATTERN_DIVIDE_X) * tw;	// �e�N�X�`���̍���X���W
		float ty = (float)(g_Waterfall.patternAnim / TEXTURE_PATTERN_DIVIDE_X) * th;	// �e�N�X�`���̍���Y���W


		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteLTColor(g_VertexBuffer,
			px, py, pw, ph, 
			tx, ty, tw, th,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}

}




