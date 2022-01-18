//=============================================================================
//
// WM��ʏ��� [battleCursor.cpp]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#include "battleCursor.h"
#include "main.h"
#include "sprite.h"
#include "player.h"
#include "battleUI.h"
#include "input.h"
#include "battle.h"
#include "damage.h"
#include "limitBar.h"
#include "battleLog.h"
#include "tutorial.h"
#include "sound.h"

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
static BATTLECURSOR g_Cursor;

// ����������
//=============================================================================
HRESULT InitBattleCursor(void)
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

	g_Cursor.w = 50;
	g_Cursor.h = 50;

	g_Cursor.texNo = 0;
	g_Cursor.posIndex = 0;

	g_Cursor.mode = MENU_SELECT;
	g_Cursor.canDoAction = TRUE;

	g_Cursor.pos[0] = D3DXVECTOR3(battleUI->pos.x - 30.0f, 390.0f, 0.0f);
	g_Cursor.pos[1] = D3DXVECTOR3(battleUI->pos.x - 30.0f, 435.0f, 0.0f);
	g_Cursor.pos[2] = D3DXVECTOR3(battleUI->pos.x - 30.0f, 480.0f, 0.0f);

	g_Cursor.powerUpPos[0] = D3DXVECTOR3(battleUI->pos.x - 30.0f, 290.0f, 0.0f);
	g_Cursor.powerUpPos[1] = D3DXVECTOR3(battleUI->pos.x - 30.0f, 310.0f, 0.0f);
	g_Cursor.powerUpPos[2] = D3DXVECTOR3(battleUI->pos.x - 30.0f, 333.0f, 0.0f);
	g_Cursor.powerUpPos[3] = D3DXVECTOR3(battleUI->pos.x - 30.0f, 355.0f, 0.0f);

	g_Load = TRUE;	// �f�[�^�̏��������s����
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBattleCursor(void)
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
void UpdateBattleCursor(void)
{
	PLAYER *player = GetPlayer();
	BATTLEUI *battleUI = GetBattleUI();
	TUTORIAL *tutorial = GetTutorial();

	if (tutorial->use)
		g_Cursor.canDoAction = FALSE;

	if (battleUI->use && g_Cursor.canDoAction)
	{
		if (g_Cursor.mode == MENU_SELECT)
		{
			g_Cursor.pos[0] = D3DXVECTOR3(battleUI->pos.x - 30.0f, 390.0f, 0.0f);
			g_Cursor.pos[1] = D3DXVECTOR3(battleUI->pos.x - 30.0f, 435.0f, 0.0f);
			g_Cursor.pos[2] = D3DXVECTOR3(battleUI->pos.x - 30.0f, 480.0f, 0.0f);

			if (GetKeyboardTrigger(DIK_X))
			{
				if (player->limitBreak >= WIDTH_MAX)
				{
					PlaySound(SOUND_LABEL_SE_click);

					// LimitBreak���g��
					PlayerLimitBreak();
				}
			}

			if (GetKeyboardTrigger(DIK_UP))
			{
				PlaySound(SOUND_LABEL_SE_click);

				g_Cursor.posIndex = (g_Cursor.posIndex - 1) % 3;

				if (g_Cursor.posIndex < 0)
					g_Cursor.posIndex = CURSOR_MAX_POSITION - 1;
			}
			else if (GetKeyboardTrigger(DIK_DOWN))
			{
				PlaySound(SOUND_LABEL_SE_click);

				g_Cursor.posIndex = (g_Cursor.posIndex + 1) % CURSOR_MAX_POSITION;
			}

			if (GetKeyboardTrigger(DIK_RETURN))
			{// Enter��������
				PlaySound(SOUND_LABEL_SE_click);

				if (battleUI->texNo == BATTLE_UI)
				{
					switch (g_Cursor.posIndex)
					{
					case 0:
						g_Cursor.mode = ATTACK_ENEMY;
						SelectTargetEnemy();
						break;
					case 1:
						g_Cursor.posIndex = 0;
						ShowBattleUI(MAGIC_UI);
						break;
					case 2:
						EscapeCheck();
						g_Cursor.posIndex = 0;
						break;
					default:
						break;
					}
				}
				else
				{
					switch (g_Cursor.posIndex)
					{
					case 0:
						if (CheckMP(6))
						{
							g_Cursor.mode = USE_FIREBALL;
							SelectTargetEnemy();
						}
						break;
					case 1:
						if (CheckMP(10))
						{
							g_Cursor.mode = HEAL_ALLY;
							g_Cursor.posIndex = 0;
							SelectAlly();
						}
						break;
					case 2:
						g_Cursor.posIndex = 0;
						ShowBattleUI(BATTLE_UI);
						break;
					default:
						break;
					}
				}
			}
		}
		else if(g_Cursor.mode == ATTACK_ENEMY)
		{
			if (GetKeyboardTrigger(DIK_RETURN))
			{// Enter��������
				PlaySound(SOUND_LABEL_SE_click);

				PlayerAttack();
				g_Cursor.mode = MENU_SELECT;
			}
			else if (GetKeyboardTrigger(DIK_BACKSPACE))
			{
				PlaySound(SOUND_LABEL_SE_click);

				g_Cursor.mode = MENU_SELECT;
			}
		}
		else if (g_Cursor.mode == USE_FIREBALL)
		{
			if (GetKeyboardTrigger(DIK_RETURN))
			{
				PlaySound(SOUND_LABEL_SE_click);

				// �t�@�C���[�{�[�����g���܂�
				FireBall();
				ShowBattleUI(BATTLE_UI);
				g_Cursor.mode = MENU_SELECT;
			}
			else if (GetKeyboardTrigger(DIK_BACKSPACE))
			{
				PlaySound(SOUND_LABEL_SE_click);

				ShowBattleUI(BATTLE_UI);
				g_Cursor.mode = MENU_SELECT;
			}
		}
		else if (g_Cursor.mode == HEAL_ALLY)
		{
			if (GetKeyboardTrigger(DIK_RETURN))
			{
				PlaySound(SOUND_LABEL_SE_click);

				// �L���A���g���܂�
				Cure();
				ShowBattleUI(BATTLE_UI);
				g_Cursor.mode = MENU_SELECT;
			}
			else if (GetKeyboardTrigger(DIK_BACKSPACE))
			{
				g_Cursor.mode = MENU_SELECT;
			}
		}
		else if (g_Cursor.mode == POWERUP_SELECT)
		{
			g_Cursor.powerUpPos[0] = D3DXVECTOR3(battleUI->pos.x - 30.0f, 290.0f, 0.0f);
			g_Cursor.powerUpPos[1] = D3DXVECTOR3(battleUI->pos.x - 30.0f, 310.0f, 0.0f);
			g_Cursor.powerUpPos[2] = D3DXVECTOR3(battleUI->pos.x - 30.0f, 333.0f, 0.0f);
			g_Cursor.powerUpPos[3] = D3DXVECTOR3(battleUI->pos.x - 30.0f, 355.0f, 0.0f);

			if (GetKeyboardTrigger(DIK_UP))
			{
				PlaySound(SOUND_LABEL_SE_click);

				g_Cursor.posIndex = (g_Cursor.posIndex - 1) % 4;

				if (g_Cursor.posIndex < 0)
					g_Cursor.posIndex = POWER_UP_MAX_POSITION - 1;
			}
			else if (GetKeyboardTrigger(DIK_DOWN))
			{
				PlaySound(SOUND_LABEL_SE_click);

				g_Cursor.posIndex = (g_Cursor.posIndex + 1) % POWER_UP_MAX_POSITION;
			}

			if (GetKeyboardTrigger(DIK_RETURN))
			{
				PlaySound(SOUND_LABEL_SE_click);

				battleUI->use = FALSE;

				switch (g_Cursor.posIndex)
				{
				case HP_UP:
					ShowBattleLog(MAX_HP_UP_LOG);
					player->maxHP += 3;
					break;
				case MP_UP:
					ShowBattleLog(MAX_MP_UP_LOG);
					player->maxMP += 3;
					break;
				case ATK_UP:
					ShowBattleLog(DAMAGE_UP_LOG);
					player->atkPower += 2;
					break;
				case RECOVERALL:
					ShowBattleLog(RECOVER_LOG);
					player->hp = player->maxHP;
					player->mp = player->maxMP;
					break;
				default:
					break;
				}
			}
		}

	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBattleCursor(void)
{
	BATTLEUI *battleUI = GetBattleUI();

	if(battleUI->use && g_Cursor.canDoAction)
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

			float px = g_Cursor.pos[g_Cursor.posIndex].x;
			float py = g_Cursor.pos[g_Cursor.posIndex].y;

			if (g_Cursor.mode == POWERUP_SELECT)
			{
				px = g_Cursor.powerUpPos[g_Cursor.posIndex].x;
				py = g_Cursor.powerUpPos[g_Cursor.posIndex].y;
			}

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteLTColor(g_VertexBuffer,
				px, py, g_Cursor.w, g_Cursor.h,
				0.0f, 0.0f, 1.0f, 1.0f,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}
	}
}

void SelectTargetEnemy(void)
{
	BATTLE *battleEnemy = GetBattleEnemy();
	g_Cursor.pos[0] = D3DXVECTOR3(battleEnemy->pos.x - 50.0f, battleEnemy->pos.y - 20.0f, 0.0f);
}

void SelectAlly(void)
{
	BATTLE *battlePlayer = GetBattlePlayer();
	g_Cursor.pos[0] = D3DXVECTOR3(battlePlayer->pos.x - 50.0f, battlePlayer->pos.y - 20.0f, 0.0f);
}

BATTLECURSOR *GetBattleCursor(void)
{
	return &g_Cursor;
}




