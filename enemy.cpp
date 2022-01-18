//=============================================================================
//
// �G�l�~�[���� [enemy.cpp]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#include "enemy.h"
#include "sprite.h"
#include "input.h"
#include "bg.h"
#include "player.h"
#include "collision.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(85)	// �L�����T�C�Y
#define TEXTURE_HEIGHT				(85)	// 
#define TEXTURE_MAX					(5)		// �e�N�X�`���̐�

#define TEXTURE_PATTERN_DIVIDE_X	(4)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y	(4)		// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define ANIM_WAIT					(8)		// �A�j���[�V�����̐؂�ւ��Wait�l


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/Slime_Medium_Green.png",
	"data/TEXTURE/Slime_Medium_Red.png",
	"data/TEXTURE/Slime_Medium_Blue.png",
	"data/TEXTURE/enemy1.png",

};


static LINEAR_INTERPOLATION g_MoveTbl0[2], g_MoveTbl1[2], g_MoveTbl2[2], g_MoveTbl3[2], g_MoveTbl4[2];

static LINEAR_INTERPOLATION *g_MoveTblAdr[] = 
{
	g_MoveTbl0,
	g_MoveTbl1,
	g_MoveTbl2,
	g_MoveTbl3,
	g_MoveTbl4,

};


static BOOL		g_Load = FALSE;			// ���������s�������̃t���O
static ENEMY	g_Enemy[ENEMY_MAX];		// �G�l�~�[�\����


//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemy(void)
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
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		g_Enemy[i].use = TRUE;
		g_Enemy[i].pos = FindEnemyPosition(i);	// ���S�_����\��
		g_Enemy[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Enemy[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Enemy[i].w   = TEXTURE_WIDTH;
		g_Enemy[i].h   = TEXTURE_HEIGHT;

		g_Enemy[i].hp = g_Enemy[i].maxHP = 50 + rand() % ((50 + 15 * player->level) - 50);
		g_Enemy[i].atkPower = (15 + rand() % 5) * player->level;
		g_Enemy[i].atkMin = g_Enemy[i].atkPower - 4;
		g_Enemy[i].atkMax = g_Enemy[i].atkPower + 2;
		g_Enemy[i].minExp = 5 * player->level;
		g_Enemy[i].maxExp = 10 * player->level;
		g_Enemy[i].exp = g_Enemy[i].minExp + rand() % (g_Enemy[i].maxExp - g_Enemy[i].minExp);
		g_Enemy[i].speed = 2 + rand() % 3;
		g_Enemy[i].action = 0.0f;

		if (player->currentLocation == 0)
			g_Enemy[i].texNo = rand() % 3;
		else if (player->currentLocation == 1)
			g_Enemy[i].texNo = 3;

		g_Enemy[i].dir = 0;
		g_Enemy[i].mode = ENEMY_MODE_PATROL;

		g_Enemy[i].countAnim = 0;
		g_Enemy[i].patternAnim = 0;

		g_Enemy[i].r = g_Enemy[i].g = g_Enemy[i].b = g_Enemy[i].a = 1.0f;

		g_Enemy[i].move = D3DXVECTOR3(4.0f, 0.0f, 0.0f);

		// �s���e�[�u����`
		g_MoveTbl0[0] = 
			//���W																	��]��							�g�嗦							����
			{ D3DXVECTOR3(g_Enemy[0].pos.x - 50.0f,  g_Enemy[0].pos.y, 0.0f),		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),	0.005f };

		g_MoveTbl0[1] =
			//���W																	��]��							�g�嗦							����
			{ D3DXVECTOR3(g_Enemy[0].pos.x + 50.0f,  g_Enemy[0].pos.y, 0.0f),		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),	0.005f };

		g_MoveTbl1[0] =
			//���W																	��]��							�g�嗦							����
			{ D3DXVECTOR3(g_Enemy[1].pos.x - 50.0f,  g_Enemy[1].pos.y, 0.0f),		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),	0.005f };

		g_MoveTbl1[1] =
			//���W																	��]��							�g�嗦							����
			{ D3DXVECTOR3(g_Enemy[1].pos.x + 50.0f,  g_Enemy[1].pos.y, 0.0f),		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),	0.005f };


		g_MoveTbl2[0] =
			//���W																	��]��							�g�嗦							����
			{ D3DXVECTOR3(g_Enemy[2].pos.x - 50.0f,  g_Enemy[2].pos.y, 0.0f),		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),	0.005f };

		g_MoveTbl2[1] =
			//���W																	��]��							�g�嗦							����
			{ D3DXVECTOR3(g_Enemy[2].pos.x + 50.0f,  g_Enemy[2].pos.y, 0.0f),		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),	0.005f };


		g_MoveTbl3[0] =
			//���W																	��]��							�g�嗦							����
			{ D3DXVECTOR3(g_Enemy[3].pos.x - 50.0f,  g_Enemy[3].pos.y, 0.0f),		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),	0.005f };

		g_MoveTbl3[1] =
			//���W																	��]��							�g�嗦							����
			{ D3DXVECTOR3(g_Enemy[3].pos.x + 50.0f,  g_Enemy[3].pos.y, 0.0f),		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),	0.005f };


		g_MoveTbl4[0] =
			//���W																	��]��							�g�嗦							����
			{ D3DXVECTOR3(g_Enemy[4].pos.x - 50.0f,  g_Enemy[4].pos.y, 0.0f),		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),	0.005f };

		g_MoveTbl4[1] =
			//���W																	��]��							�g�嗦							����
			{ D3DXVECTOR3(g_Enemy[4].pos.x + 50.0f,  g_Enemy[4].pos.y, 0.0f),		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f),	0.005f };

		// �s���p�^�[����������
		g_Enemy[i].time		= 0.0f;			// ���`��ԗp
		g_Enemy[i].moveTblNo= i;			// �f�[�^�e�[�u��
		g_Enemy[i].tblMax = sizeof(g_MoveTbl0) / sizeof(LINEAR_INTERPOLATION);// ���`��ԗp
	}

	g_Load = TRUE;	// �f�[�^�̏��������s����
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEnemy(void)
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
void UpdateEnemy(void)
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (g_Enemy[i].use == TRUE && g_Enemy[i].mode == ENEMY_MODE_PATROL)	// ���̃G�l�~�[���g���Ă���H
		{	
			if (g_Enemy[i].hp <= 0)
			{
				EnemyDead(i);
			}
			
			// Yes
			// �n�`�Ƃ̓����蔻��p�ɍ��W�̃o�b�N�A�b�v������Ă���
			D3DXVECTOR3 pos_old = g_Enemy[i].pos;

			// �ړ����� 
			{
				// �s���e�[�u���ɏ]���č��W�ړ��i���`��ԁj
				int nowNo = (int)g_Enemy[i].time;			// �������ł���e�[�u���ԍ������o���Ă���
				int maxNo = g_Enemy[i].tblMax;				// �o�^�e�[�u�����𐔂��Ă���
				int nextNo = (nowNo + 1) % maxNo;			// �ړ���e�[�u���̔ԍ������߂Ă���
				LINEAR_INTERPOLATION *tbl = g_MoveTblAdr[ g_Enemy[i].moveTblNo ];	// �s���e�[�u���̃A�h���X���擾
				D3DXVECTOR3	pos = tbl[nextNo].pos - tbl[nowNo].pos;	// XYZ�ړ��ʂ��v�Z���Ă���
				D3DXVECTOR3	rot = tbl[nextNo].rot - tbl[nowNo].rot;	// XYZ��]�ʂ��v�Z���Ă���
				D3DXVECTOR3	scl = tbl[nextNo].scl - tbl[nowNo].scl;	// XYZ�g�嗦���v�Z���Ă���
				float nowTime = g_Enemy[i].time - nowNo;	// ���ԕ����ł��鏭�������o���Ă���
				pos *= nowTime;								// ���݂̈ړ��ʂ��v�Z���Ă���
				rot *= nowTime;								// ���݂̉�]�ʂ��v�Z���Ă���
				scl *= nowTime;								// ���݂̊g�嗦���v�Z���Ă���

				// �v�Z���ċ��߂��ړ��ʂ����݂̈ړ��e�[�u��XYZ�ɑ����Ă��遁�\�����W�����߂Ă���
				g_Enemy[i].pos = tbl[nowNo].pos + pos;

				// �v�Z���ċ��߂���]�ʂ����݂̈ړ��e�[�u���ɑ����Ă���
				g_Enemy[i].rot = tbl[nowNo].rot + rot;

				// �v�Z���ċ��߂��g�嗦�����݂̈ړ��e�[�u���ɑ����Ă���
				g_Enemy[i].scl = tbl[nowNo].scl + scl;
				g_Enemy[i].w = TEXTURE_WIDTH * g_Enemy[i].scl.x;
				g_Enemy[i].h = TEXTURE_HEIGHT * g_Enemy[i].scl.y;

				// frame���g�Ď��Ԍo�ߏ���������
				g_Enemy[i].time += tbl[nowNo].time;			// ���Ԃ�i�߂Ă���
				if ((int)g_Enemy[i].time >= maxNo)			// �o�^�e�[�u���Ō�܂ňړ��������H
				{
					g_Enemy[i].time -= maxNo;				// �O�ԖڂɃ��Z�b�g�������������������p���ł���
				}

				float xPos_differ = tbl[nextNo].pos.x - tbl[nowNo].pos.x;	// ���̃e�[�u���͉E�����������A�m�F�ϐ�
				float yPos_differ = tbl[nextNo].pos.y - tbl[nowNo].pos.y;	// ���̃e�[�u���͏㑤���������A�m�F�ϐ�
				int ans = abs((int)xPos_differ) - abs((int)yPos_differ);

				if (xPos_differ >= 0 && yPos_differ >= 0)
				{
					if (ans > 0)
						g_Enemy[i].dir = ENEMY_LOOK_RIGHT;
					else
						g_Enemy[i].dir = ENEMY_LOOK_DOWN;
				}
				else if (xPos_differ >= 0 && yPos_differ <= 0)
				{
					if (ans > 0)
						g_Enemy[i].dir = ENEMY_LOOK_RIGHT;
					else
						g_Enemy[i].dir = ENEMY_LOOK_UP;
				}
				else if (xPos_differ <= 0 && yPos_differ >= 0)
				{
					if (ans > 0)
						g_Enemy[i].dir = ENEMY_LOOK_LEFT;
					else
						g_Enemy[i].dir = ENEMY_LOOK_DOWN;
				}
				else
				{
					if (ans > 0)
						g_Enemy[i].dir = ENEMY_LOOK_LEFT;
					else
						g_Enemy[i].dir = ENEMY_LOOK_UP;
				}

				// �A�j���[�V����  
				g_Enemy[i].countAnim++;
				if ((g_Enemy[i].countAnim % ANIM_WAIT) == 0)
				{
					// �p�^�[���̐؂�ւ�
					g_Enemy[i].patternAnim = (g_Enemy[i].dir * TEXTURE_PATTERN_DIVIDE_X) + ((g_Enemy[i].patternAnim + 1) % TEXTURE_PATTERN_DIVIDE_X);
				}
			}
		}
		else if (g_Enemy[i].use == TRUE && g_Enemy[i].mode == ENEMY_MODE_BATTLE)	// ���̃G�l�~�[���g���Ă���H
		{
			PLAYER *player = GetPlayer();
			g_Enemy[i].countAnim++;
			if ((g_Enemy[i].countAnim % ANIM_WAIT) == 0)
			{
				// �p�^�[���̐؂�ւ�
				g_Enemy[i].patternAnim = (g_Enemy[i].dir * TEXTURE_PATTERN_DIVIDE_X) + ((g_Enemy[i].patternAnim + 1) % TEXTURE_PATTERN_DIVIDE_X);
			}
			
			// �o�g���̏���
			if (player->battleReady)			// �o�g���̏������I���?
			{
				switch (player->dir)
				{
				case PLAYER_LOOK_UP:
					g_Enemy[i].dir = ENEMY_LOOK_DOWN;
					break;
				case PLAYER_LOOK_LEFT:
					g_Enemy[i].dir = ENEMY_LOOK_RIGHT;
					break;
				case PLAYER_LOOK_DOWN:
					g_Enemy[i].dir = ENEMY_LOOK_UP;
					break;
				case PLAYER_LOOK_RIGHT:
					g_Enemy[i].dir = ENEMY_LOOK_LEFT;
					break;
				}
			}
		}

#ifdef _DEBUG
		// �f�o�b�O�\��
		PrintDebugProc("Enemy No%d  X:%f Y:%f\n", i, g_Enemy[i].pos.x, g_Enemy[i].pos.y);
#endif

	}
}


//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemy(void)
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

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (g_Enemy[i].use == TRUE)		// ���̃G�l�~�[���g���Ă���H
		{								// Yes
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Enemy[i].texNo]);

			//�G�l�~�[�̈ʒu��e�N�X�`���[���W�𔽉f
			float px = g_Enemy[i].pos.x - bg->pos.x;	// �G�l�~�[�̕\���ʒuX
			float py = g_Enemy[i].pos.y - bg->pos.y;	// �G�l�~�[�̕\���ʒuY
			float pw = g_Enemy[i].w;		// �G�l�~�[�̕\����
			float ph = g_Enemy[i].h;		// �G�l�~�[�̕\������

			// �A�j���[�V�����p
			float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X;	// �e�N�X�`���̕�
			float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// �e�N�X�`���̍���
			float tx = (float)(g_Enemy[i].patternAnim % TEXTURE_PATTERN_DIVIDE_X) * tw;	// �e�N�X�`���̍���X���W
			float ty = (float)(g_Enemy[i].patternAnim / TEXTURE_PATTERN_DIVIDE_X) * th;	// �e�N�X�`���̍���Y���W

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				D3DXCOLOR(g_Enemy[i].r, g_Enemy[i].g, g_Enemy[i].b, g_Enemy[i].a),
				g_Enemy[i].rot.z);

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}
	}
}

D3DXVECTOR3 FindEnemyPosition(int monsterIndex)
{
	int mapSize = 2400;
	int mapChipSize = 32;
	float x, y;

	for (int i = 0; i < mapSize / mapChipSize; i++)
	{
		for (int j = 0; j < mapSize / mapChipSize; j++)
		{

			if (GetMapIndex(j, i) == ENEMY_POS + monsterIndex)
			{
				x = (float)i * mapChipSize;
				y = (float)j * mapChipSize;

				return D3DXVECTOR3(x, y, 0.0f);
			}
		}
	}

	return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

void EnemyDead(int index)
{
	g_Enemy[index].g = g_Enemy[index].b = 0.0f;
	g_Enemy[index].a -= 0.1f;

	if (g_Enemy[index].a <= 0.0f)
	{
		g_Enemy[index].use = FALSE;
	}
}



//=============================================================================
// �G�l�~�[�\���̂̐擪�A�h���X���擾
//=============================================================================
ENEMY *GetEnemy(void)
{

	return &g_Enemy[0];
}




