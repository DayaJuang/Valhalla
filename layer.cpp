//=============================================================================
//
// Layer��ʏ��� [layer.cpp]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#include "layer.h"
#include "main.h"
#include "sprite.h"
#include "bg.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define LAYER_WIDTH					(2400)					// (SCREEN_WIDTH)
#define LAYER_HEIGHT				(2400)					// (SCREEN_HEIGHT)
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
	"data/MAP/wood_layer1.png",
	" ",
};


static BOOL	g_Load = FALSE;		// ���������s�������̃t���O
static LAYER	g_Layer;


// ����������
//=============================================================================
HRESULT InitLayer(void)
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
	int mapToShow = checkCurrentMap();

	g_Layer.w = LAYER_WIDTH;
	g_Layer.h = LAYER_HEIGHT;

	g_Layer.texNo = checkCurrentMap();
	g_Layer.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Layer.r = g_Layer.g = g_Layer.b = 1.0f;

	g_Load = TRUE;	// �f�[�^�̏��������s����
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitLayer(void)
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
void UpdateLayer(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawLayer(void)
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

	// Layer�̔w�i��`��
	{
		BG *bg = GetBG();

		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Layer.texNo]);

		float px = g_Layer.pos.x - bg->pos.x;	// ���C���[�̕\���ʒuX
		float py = g_Layer.pos.y - bg->pos.y;	// ���C���[�̕\���ʒuY
		float pw = g_Layer.w;					// ���C���[�̕\����
		float ph = g_Layer.h;					// ���C���[�̕\������

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteLTColor(g_VertexBuffer,
			px, py, pw, ph, 
			0.0f, 0.0f, 1.0f, 1.0f,
			D3DXCOLOR(g_Layer.r, g_Layer.g, g_Layer.b, 1.0f));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}

}

LAYER *GetLayer(void) {

	return &g_Layer;
}





