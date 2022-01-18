//=============================================================================
//
// �G�l�~�[���� [Orb.cpp]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#include "orb.h"
#include "sprite.h"
#include "bg.h"
#include "player.h"
#include "collision.h"
#include "fade.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(40)	// �L�����T�C�Y
#define TEXTURE_HEIGHT				(40)	// 
#define TEXTURE_MAX					(1)		// �e�N�X�`���̐�

#define TEXTURE_PATTERN_DIVIDE_X	(3)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y	(1)		// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define ANIM_WAIT					(30)		// �A�j���[�V�����̐؂�ւ��Wait�l


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/orb.png"
};

static BOOL		g_Load = FALSE;			// ���������s�������̃t���O
static ORB	g_Orb[ORB_MAX];		// �G�l�~�[�\����


//=============================================================================
// ����������
//=============================================================================
HRESULT InitOrb(void)
{
	ID3D11Device *pDevice = GetDevice();
	srand((unsigned)time(NULL));

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

	PLAYER *player = GetPlayer();

	// �G�l�~�[�\���̂̏�����
	for (int i = 0; i < ORB_MAX; i++)
	{
		g_Orb[i].use = TRUE;
		g_Orb[i].pos = FindOrbPosition();	// ���S�_����\��
		g_Orb[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Orb[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Orb[i].w   = TEXTURE_WIDTH;
		g_Orb[i].h   = TEXTURE_HEIGHT;
	}

	g_Load = TRUE;	// �f�[�^�̏��������s����
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitOrb(void)
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
void UpdateOrb(void)
{
	for (int i = 0; i < ORB_MAX; i++)
	{
		if (g_Orb->pos == D3DXVECTOR3(0.0f, 0.0f, 0.0f))
			g_Orb->use = FALSE;
 
		{
			// �A�j���[�V����  
			g_Orb[i].countAnim++;
			if ((g_Orb[i].countAnim % ANIM_WAIT) == 0)
			{
				// �p�^�[���̐؂�ւ�
				g_Orb[i].patternAnim = (g_Orb[i].countAnim * TEXTURE_PATTERN_DIVIDE_X) + ((g_Orb[i].patternAnim + 1) % TEXTURE_PATTERN_DIVIDE_X);
			}
		}

		// �����蔻�菈��
		{
			PLAYER *player = GetPlayer();

			// �G�l�~�[�̐��������蔻����s��
			for (int j = 0; j < PLAYER_MAX; j++)
			{
				// �����Ă�G�l�~�[�Ɠ����蔻�������
				if (player[j].use == TRUE)
				{
					BOOL ans = CollisionBB(g_Orb[i].pos, g_Orb[i].w, g_Orb[i].h,
						player[j].pos, player[j].w, player[j].h);
					// �������Ă���H
					if (ans == TRUE)
					{
						// �����̏��������Ȃ��Ɗђʒe�ɂȂ��Ă��܂�
						g_Orb[i].use = FALSE;

						// �����������̏���
						SetFade(FADE_OUT, MODE_GAME_CLEAR);

						break;
					}
				}
			}
		}


#ifdef _DEBUG
		// �f�o�b�O�\��
		PrintDebugProc("Orb No%d  X:%f Y:%f\n", i, g_Orb[i].pos.x, g_Orb[i].pos.y);
#endif

	}
}


//=============================================================================
// �`�揈��
//=============================================================================
void DrawOrb(void)
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

	BG *bg = GetBG();

	for (int i = 0; i < ORB_MAX; i++)
	{
		if (g_Orb[i].use == TRUE)		// ���̃G�l�~�[���g���Ă���H
		{								// Yes
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Orb[i].texNo]);

			//�G�l�~�[�̈ʒu��e�N�X�`���[���W�𔽉f
			float px = g_Orb[i].pos.x - bg->pos.x;	// �G�l�~�[�̕\���ʒuX
			float py = g_Orb[i].pos.y - bg->pos.y;	// �G�l�~�[�̕\���ʒuY
			float pw = g_Orb[i].w;		// �G�l�~�[�̕\����
			float ph = g_Orb[i].h;		// �G�l�~�[�̕\������

			// �A�j���[�V�����p
			float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X;	// �e�N�X�`���̕�
			float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// �e�N�X�`���̍���
			float tx = (float)(g_Orb[i].patternAnim % TEXTURE_PATTERN_DIVIDE_X) * tw;	// �e�N�X�`���̍���X���W
			float ty = (float)(g_Orb[i].patternAnim / TEXTURE_PATTERN_DIVIDE_X) * th;	// �e�N�X�`���̍���Y���W

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
				g_Orb[i].rot.z);

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}
	}
}

D3DXVECTOR3 FindOrbPosition(void)
{
	int mapSize = 2400;
	int mapChipSize = 32;
	float x, y;

	for (int i = 0; i < mapSize / mapChipSize; i++)
	{
		for (int j = 0; j < mapSize / mapChipSize; j++)
		{

			if (GetMapIndex(j, i) == ORB_POS)
			{
				x = (float)i * mapChipSize;
				y = (float)j * mapChipSize;

				return D3DXVECTOR3(x, y, 0.0f);
			}
		}
	}

	return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}


//=============================================================================
// �G�l�~�[�\���̂̐擪�A�h���X���擾
//=============================================================================
ORB *GetOrb(void)
{

	return &g_Orb[0];
}




