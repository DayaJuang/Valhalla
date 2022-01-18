//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#include "player.h"
#include "sprite.h"
#include "input.h"
#include "bg.h"
#include "bullet.h"
#include "enemy.h"
#include "collision.h"
#include "damage.h"
#include "sound.h"
#include "file.h"
#include "fade.h"
#include "iconWM.h"
#include "battleUI.h"
#include "battleCursor.h"
#include "battle.h"
#include "effect.h"
#include "tutorial.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(140/2)	// �L�����T�C�Y
#define TEXTURE_HEIGHT				(140/2)	// 
#define TEXTURE_MAX					(2)		// �e�N�X�`���̐�

#define TEXTURE_PATTERN_DIVIDE_X	(8)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y	(5)		// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����

// �o�g���p�̃A�j���[�V����
#define BATTLE_TEXTURE_PATTERN_DIVIDE_X	(9)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define BATTLE_TEXTURE_PATTERN_DIVIDE_Y	(12)		// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define BATTLE_ANIM_PATTERN_NUM			(BATTLE_TEXTURE_PATTERN_DIVIDE_X*BATTLE_TEXTURE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����

#define ANIM_WAIT						(4)		// �A�j���[�V�����̐؂�ւ��Wait�l

// �v���C���[�̉�ʓ��z�u���W
#define PLAYER_DISP_X				(SCREEN_WIDTH/2)
#define PLAYER_DISP_Y				(SCREEN_HEIGHT/2 + TEXTURE_HEIGHT)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/Character2.png",
	"data/TEXTURE/battle_Character.png"
};

static LINEAR_INTERPOLATION g_MoveTbl;

static BOOL		g_Load = FALSE;			// ���������s�������̃t���O
static PLAYER	g_Player[PLAYER_MAX];	// �v���C���[�\����
static int waitTime = 0;
static BOOL firstTime = TRUE;			// ���߂ăQ�[�����v���C���邩�ǂ����m�F�ϐ�

//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(void)
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


	// �v���C���[�\���̂̏�����
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		g_Player[i].use = TRUE;
		g_Player[i].pos = FindPosition();
		g_Player[i].battlePos = g_Player[i].pos;
		g_Player[i].oldPos = g_Player[i].pos;
		g_Player[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Player[i].w   = TEXTURE_WIDTH;
		g_Player[i].h   = TEXTURE_HEIGHT;
		g_Player[i].texNo = 0;
		g_Player[i].a = 1.0f;

		g_Player[i].countAnim = 0;
		g_Player[i].patternAnim = 0;

		g_Player[i].move = D3DXVECTOR3(3.0f, 0.0f, 0.0f);		// �ړ���

		g_Player[i].dir = PLAYER_LOOK_RIGHT;
		g_Player[i].idle = TRUE;
		g_Player[i].mode = PLAYER_MODE_EXPLORE;

		g_Player[i].canMove = TRUE;
		g_Player[i].battleReady = FALSE;

		g_Player[i].atkMin = g_Player[i].atkPower - 2;
		g_Player[i].atkMax = g_Player[i].atkPower + 2;
		g_Player[i].speed = 4;

		g_Player[i].time = 0.0f;
		g_Player[i].actionTime = 0.0f;

		g_Player[i].action = PLAYER_WAIT;

		if (!g_Player[i].isAlive)
		{
			// �o�g���p�̃X�e�[�^�X
			g_Player[i].level = 1;
			g_Player[i].currentExp = 0;			// ���݂̌o���l
			g_Player[i].expToNextLevel = 35;	// �K�v�Ȍo���l
			g_Player[i].maxHP = 75;
			g_Player[i].maxMP = 42;
			g_Player[i].atkPower = 15;
			g_Player[i].hp = g_Player[i].maxHP;
			g_Player[i].mp = g_Player[i].maxMP;
			g_Player[i].limitBreak = 0.0f;
			g_Player[i].currentLocation = 0;

			g_Player[i].isAlive = TRUE;
		}
	}

	g_Load = TRUE;	// �f�[�^�̏��������s����
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
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
void UpdatePlayer(void)
{
	TUTORIAL *tutorial = GetTutorial();

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		// �`���[�g���A����\��
		if (firstTime)
		{
			firstTime = FALSE;
			g_Player[i].canMove = FALSE;
			ShowTutorial(FIELD_TUTORIAL1, FIELD_TUTORIAL);
		}
		else
		{
			// �v���C���[�̍U�����X�V
			g_Player[i].atkMin = g_Player[i].atkPower - 2;
			g_Player[i].atkMax = g_Player[i].atkPower + 2;

			// �v���C���[���܂������Ă��邩�ǂ���
			if (g_Player[i].hp <= 0)
			{
				PlayerDead();	// ���ɂ܂�
			}

			// �T���̃��[�h
			if (g_Player[i].mode == PLAYER_MODE_EXPLORE)
			{
				float speed = g_Player[i].move.x;	// �ړ��X�s�[�h

				// �`���[�g���A�����܂��\�����Ă���Ƃ��A�ړ��ł��܂���
				if(!tutorial->use)
					g_Player[i].canMove = TRUE;

				// �̍��W�̓o�^
				g_Player[i].oldPos.x = g_Player[i].pos.x;
				g_Player[i].oldPos.y = g_Player[i].pos.y;

				// �����Ă�v���C���[��������������
				if (g_Player[i].use == TRUE && g_Player[i].canMove)
				{
					// �n�`�Ƃ̓����蔻��p�ɍ��W�̃o�b�N�A�b�v������Ă���
					D3DXVECTOR3 pos_old = g_Player[i].pos;

					// �A�j���[�V����  
					g_Player[i].countAnim += 1;

					if (g_Player[i].countAnim > ANIM_WAIT)
					{
						g_Player[i].countAnim = 0;
						// �p�^�[���̐؂�ւ�
						g_Player[i].patternAnim = (g_Player[i].patternAnim + 1) % ANIM_PATTERN_NUM;

						// �����̉��̏���
						if (((g_Player[i].patternAnim + 1) % ANIM_WAIT) == 0  && g_Player[i].idle == FALSE)
						{
							PlaySound(SOUND_LABEL_SE_footstep);
						}
					}

					// �L�[���͂ňړ� 
					{
						if (GetKeyboardPress(DIK_DOWN))
						{
							if (GetKeyboardPress(DIK_RIGHT) || GetKeyboardPress(DIK_LEFT))
								speed *= 0.7f;	// �΂߂̃X�s�[�h����1�{�ɂ���

							g_Player[i].idle = FALSE;
							g_Player[i].pos.y += speed;
							g_Player[i].dir = 2;

						}
						else if (GetKeyboardPress(DIK_UP))
						{
							if (GetKeyboardPress(DIK_RIGHT) || GetKeyboardPress(DIK_LEFT))
								speed *= 0.7f;	// �΂߂̃X�s�[�h����1�{�ɂ���

							g_Player[i].idle = FALSE;
							g_Player[i].pos.y -= speed;
							g_Player[i].dir = PLAYER_LOOK_UP;
						}

						if (GetKeyboardPress(DIK_RIGHT))
						{
							g_Player[i].idle = FALSE;
							g_Player[i].pos.x += speed;
							g_Player[i].dir = PLAYER_LOOK_RIGHT;
						}
						else if (GetKeyboardPress(DIK_LEFT))
						{
							g_Player[i].idle = FALSE;
							g_Player[i].pos.x -= speed;
							g_Player[i].dir = PLAYER_LOOK_LEFT;
						}

						// �Q�[���p�b�h�łňړ�����
						if (IsButtonPressed(0, BUTTON_DOWN))
						{
							if (IsButtonPressed(0, BUTTON_RIGHT) || IsButtonPressed(0, BUTTON_LEFT))
								speed *= 0.7f;	// �΂߂̃X�s�[�h����1�{�ɂ���

							g_Player[i].idle = FALSE;
							g_Player[i].pos.y += speed;
							g_Player[i].dir = PLAYER_LOOK_DOWN;
						}
						else if (IsButtonPressed(0, BUTTON_UP))
						{
							if (IsButtonPressed(0, BUTTON_RIGHT) || IsButtonPressed(0, BUTTON_LEFT))
								speed *= 0.7f;	// �΂߂̃X�s�[�h����1�{�ɂ���

							g_Player[i].idle = FALSE;
							g_Player[i].pos.y -= speed;
							g_Player[i].dir = PLAYER_LOOK_UP;
						}

						if (IsButtonPressed(0, BUTTON_RIGHT))
						{
							g_Player[i].idle = FALSE;
							g_Player[i].pos.x += speed;
							g_Player[i].dir = PLAYER_LOOK_RIGHT;
						}
						else if (IsButtonPressed(0, BUTTON_LEFT))
						{
							g_Player[i].idle = FALSE;
							g_Player[i].pos.x -= speed;
							g_Player[i].dir = PLAYER_LOOK_LEFT;
						}

						// ���������ĂȂ��Ƃ��A�v���C���[���A�C�h���̏�ԂɂȂ�
						if ((!IsButtonPressed(0, BUTTON_DOWN) && !IsButtonPressed(0, BUTTON_UP) && !IsButtonPressed(0, BUTTON_RIGHT) && !IsButtonPressed(0, BUTTON_LEFT))
							&& (GetKeyboardRelease(DIK_UP) || GetKeyboardRelease(DIK_DOWN) || GetKeyboardRelease(DIK_RIGHT) || GetKeyboardRelease(DIK_LEFT)))
							g_Player[i].idle = TRUE;

						// MAP�O�`�F�b�N
						BG *bg = GetBG();

						if (g_Player[i].pos.x < g_Player[i].w / 5)
						{
							g_Player[i].pos.x = g_Player[i].w / 5;
						}

						if (g_Player[i].pos.x > bg->w - g_Player[i].w / 5)
						{
							g_Player[i].pos.x = bg->w - g_Player[i].w / 5;
						}

						if (g_Player[i].pos.y < g_Player[i].h / 5)
						{
							g_Player[i].pos.y = g_Player[i].h / 5;
						}

						if (g_Player[i].pos.y > bg->h - g_Player[i].h / 2)
						{
							g_Player[i].pos.y = bg->h - g_Player[i].h / 2;
						}

						// �v���C���[�̓����蔻��`�F�b�N
						int checkTop = CheckMap(g_Player[i].pos.x, g_Player[i].pos.y);
						int checkLeft = CheckMap(g_Player[i].pos.x - g_Player[i].w / 5, g_Player[i].pos.y);
						int checkRight = CheckMap(g_Player[i].pos.x + g_Player[i].w / 5, g_Player[i].pos.y);
						int checkBot = CheckMap(g_Player[i].pos.x, g_Player[i].pos.y + g_Player[i].h / 2);

						// �ʂ�Ȃ�
						if ((checkTop == COLLISION) || (checkLeft == COLLISION) || (checkRight == COLLISION) || (checkBot == COLLISION))
						{
							g_Player[i].pos.x = g_Player[i].oldPos.x;
							g_Player[i].pos.y = g_Player[i].oldPos.y;
						}
						else if ((checkTop == TO_WORLDMAP) || (checkLeft == TO_WORLDMAP) || (checkRight == TO_WORLDMAP) || (checkBot == TO_WORLDMAP))
						{
							// ���[�h�}�b�v�Ɉړ�
							g_Player[i].idle = TRUE;
							g_Player[i].canMove = FALSE;
							g_Player[i].pos.x = g_Player[i].oldPos.x;
							g_Player[i].pos.y = g_Player[i].oldPos.y;
							SetFade(FADE_OUT, MODE_WORLDMAP);
						}
						else if ((checkTop == TO_WORLDMAP_NEXT_AREA) || (checkLeft == TO_WORLDMAP_NEXT_AREA) || (checkRight == TO_WORLDMAP_NEXT_AREA) || (checkBot == TO_WORLDMAP_NEXT_AREA))
						{
							// ���[�h�}�b�v�Ɉړ����āA���̃t�B�[���h��\������
							ICONWM *icon = GetIconWM(g_Player[i].currentLocation + 1);
							icon->use = TRUE;
							g_Player[i].idle = TRUE;
							g_Player[i].canMove = FALSE;
							g_Player[i].pos.x = g_Player[i].oldPos.x;
							g_Player[i].pos.y = g_Player[i].oldPos.y;
							SetFade(FADE_OUT, MODE_WORLDMAP);
						}

						// �v���C���[�̗����ʒu����MAP�̃X�N���[�����W���v�Z����
						bg->pos.x = g_Player[i].pos.x - PLAYER_DISP_X;
						if (bg->pos.x < 0) bg->pos.x = 0;
						if (bg->pos.x > bg->w - SCREEN_WIDTH) bg->pos.x = bg->w - SCREEN_WIDTH;

						bg->pos.y = g_Player[i].pos.y - PLAYER_DISP_Y;
						if (bg->pos.y < 0) bg->pos.y = 0;
						if (bg->pos.y > bg->h - SCREEN_HEIGHT) bg->pos.y = bg->h - SCREEN_HEIGHT;

						// �ړ����I�������G�l�~�[�Ƃ̓����蔻��
						{
							ENEMY *enemy = GetEnemy();

							// �G�l�~�[�̐��������蔻����s��
							for (int j = 0; j < ENEMY_MAX; j++)
							{
								// �����Ă�G�l�~�[�Ɠ����蔻�������
								if (enemy[j].use == TRUE)
								{
									BOOL ans = CollisionBB(g_Player[i].pos, g_Player[i].w, g_Player[i].h,
										enemy[j].pos, enemy[j].w, enemy[j].h);
									// �������Ă���H
									if (ans == TRUE)
									{
										PlaySound(SOUND_LABEL_SE_battleStart);

										// �v���C���[��G�l�~�[�̃��[�h�̐ؑ�
										g_Player[i].mode = PLAYER_MODE_BATTLE;
										enemy[j].mode = ENEMY_MODE_BATTLE;

										// �v���C���[�̃o�g�����W�����߂�
										float posDiffX = g_Player[i].pos.x - enemy[j].pos.x;
										float posDiffY = g_Player[i].pos.y - enemy[j].pos.y;
										int ans = abs((int)posDiffX) - abs((int)posDiffY);

										if (posDiffX >= 0 && posDiffY >= 0)
										{
											if (ans > 0)
											{
												g_Player[i].next_Dir = PLAYER_LOOK_RIGHT;
												g_MoveTbl =
													//���W														��]��					Z		�g�嗦			X		Y		����
												{ D3DXVECTOR3(enemy[j].pos.x + 150.0f, enemy[j].pos.y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 0.02f };

											}
											else
											{
												g_Player[i].next_Dir = PLAYER_LOOK_DOWN;
												g_MoveTbl =
													//���W														��]��					Z		�g�嗦			X		Y		����
												{ D3DXVECTOR3(enemy[j].pos.x, enemy[j].pos.y + 150.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 0.02f };
											}
										}
										else if (posDiffX >= 0 && posDiffY <= 0)
										{
											if (ans > 0)
											{
												g_Player[i].next_Dir = PLAYER_LOOK_RIGHT;
												g_MoveTbl =
													//���W														��]��					Z		�g�嗦			X		Y		����
												{ D3DXVECTOR3(enemy[j].pos.x + 150.0f, enemy[j].pos.y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 0.02f };
											}
											else
											{
												g_Player[i].next_Dir = PLAYER_LOOK_UP;
												g_MoveTbl =
													//���W														��]��					Z		�g�嗦			X		Y		����
												{ D3DXVECTOR3(enemy[j].pos.x, enemy[j].pos.y - 150.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 0.02f };
											}
										}
										else if (posDiffX <= 0 && posDiffY >= 0)
										{
											if (ans > 0)
											{
												g_Player[i].next_Dir = PLAYER_LOOK_LEFT;
												g_MoveTbl =
													//���W														��]��					Z		�g�嗦			X		Y		����
												{ D3DXVECTOR3(enemy[j].pos.x - 150.0f, enemy[j].pos.y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 0.02f };
											}
											else
											{
												g_Player[i].next_Dir = PLAYER_LOOK_DOWN;
												g_MoveTbl =
													//���W														��]��					Z		�g�嗦			X		Y		����
												{ D3DXVECTOR3(enemy[j].pos.x, enemy[j].pos.y + 150.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 0.02f };
											}
										}
										else
										{
											if (ans > 0)
											{
												g_Player[i].next_Dir = PLAYER_LOOK_LEFT;
												g_MoveTbl =
													//���W														��]��					Z		�g�嗦			X		Y		����
												{ D3DXVECTOR3(enemy[j].pos.x - 150.0f, enemy[j].pos.y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 0.02f };
											}
											else
											{
												g_Player[i].next_Dir = PLAYER_LOOK_UP;
												g_MoveTbl =
													//���W														��]��					Z		�g�嗦			X		Y		����
												{ D3DXVECTOR3(enemy[j].pos.x, enemy[j].pos.y - 150.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 0.02f };
											}
										}

										// �o�g������
										g_Player[i].battlePos = g_MoveTbl.pos;
										SetBattle(j);
									}
								}
							}
						}

						// �o���b�g����
						if (GetKeyboardTrigger(DIK_SPACE))
						{// Enter��������A�X�e�[�W��؂�ւ���
							D3DXVECTOR3 pos = g_Player[i].pos;

						}
						if (IsButtonTriggered(0, BUTTON_B))
						{// Enter��������A�X�e�[�W��؂�ւ���
							D3DXVECTOR3 pos = g_Player[i].pos;
						}

					}
				}
			}
			else if (g_Player[i].mode == PLAYER_MODE_BATTLE)	// �o�g�����[�h�̏���
			{
				// �o�g���̏���
				if (!g_Player[i].battleReady)
				{
					BattlePreparation();
				}
				else
				{
					// �o�g���̏���
					BATTLEUI *battleUI = GetBattleUI();
					BATTLE *enemy = GetBattleEnemy();

					// ���݂͂ǂ�ȃA�N�V����������Ă��邩?
					switch (g_Player[i].action)
					{
					case PLAYER_WAIT:
						g_Player[i].patternAnim = (g_Player[i].dir * BATTLE_TEXTURE_PATTERN_DIVIDE_X);
						break;
					case PLAYER_ATTACK:
						AttackAnimation(g_Player[i].dir);
						break;
					case PLAYER_CAST_FIREBALL:
						MagicAnimation(g_Player[i].dir, g_Player[i].action);
						break;
					case PLAYER_CAST_HEAL:
						MagicAnimation(g_Player[i].dir, g_Player[i].action);
						break;
					case PLAYER_LIMIT_BREAK:
						MagicAnimation(g_Player[i].dir, g_Player[i].action);
						break;
					default:
						break;
					}
				}
			}

#ifdef _DEBUG
			// �f�o�b�O�\��
			PrintDebugProc("Player No%d  X:%f Y:%f \n", i, g_Player[i].pos.x, g_Player[i].pos.y);
#endif

		}
	}
	
}


//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
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

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (g_Player[i].use == TRUE)		// ���̃v���C���[���g���Ă���H
		{									// Yes
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Player[i].texNo]);

			//�v���C���[�̈ʒu��e�N�X�`���[���W�𔽉f
			float px = g_Player[i].pos.x - bg->pos.x;	// �v���C���[�̕\���ʒuX
			float py = g_Player[i].pos.y - bg->pos.y;	// �v���C���[�̕\���ʒuY
			float pw = g_Player[i].w;		// �v���C���[�̕\����
			float ph = g_Player[i].h;		// �v���C���[�̕\������

				// �A�j���[�V�����p
			float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X;	// �e�N�X�`���̕�
			float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// �e�N�X�`���̍���

			float tx = 0.0f;	// �e�N�X�`���̍���X���W
			float ty = 0.0f;	// �e�N�X�`���̍���Y���W

			float gridWide = 0.0f;	// �f�ނ̃O���b�h��

			if (g_Player[i].idle)	// Idle�A�j���[�V����
			{
				gridWide = 0.125f;

				tx = gridWide * g_Player[i].dir;
				ty = 0.0f;
			}
			else					// ����A�j���[�V����
			{
				gridWide = 0.2f;

				tx = (float)(g_Player[i].patternAnim % TEXTURE_PATTERN_DIVIDE_X) * tw;	// �e�N�X�`���̍���X���W
				ty = gridWide*(g_Player[i].dir + 1);										// �e�N�X�`���̍���Y���W
			}

			// �o�g���v���C���[�̃A�j���[�V����
			if (g_Player[i].battleReady)
			{
				tw = 1.0f / BATTLE_TEXTURE_PATTERN_DIVIDE_X;	// �e�N�X�`���̕�
				th = 1.0f / BATTLE_TEXTURE_PATTERN_DIVIDE_Y;	// �e�N�X�`���̍���
				tx = (float)(g_Player[i].patternAnim % BATTLE_TEXTURE_PATTERN_DIVIDE_X) * tw;	// �e�N�X�`���̍���X���W
				ty = (float)(g_Player[i].patternAnim / BATTLE_TEXTURE_PATTERN_DIVIDE_X) * th;	// �e�N�X�`���̍���Y���W
			}

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Player[i].a),
				g_Player[i].rot.z);

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}
	}
}

// �v���C���[�̍��W���߂�֐�
D3DXVECTOR3 FindPosition(void)
{
	int mapSize = 2400;
	int mapChipSize = 32;
	float x, y;

	for (int i = 0; i < mapSize/mapChipSize; i++)
	{
		for (int j = 0; j < mapSize/mapChipSize; j++)
		{
			if (GetMapIndex(j,i) == PLAYER_POS)
			{
				x = (float) i * mapChipSize;
				y = (float) j * mapChipSize;

				return D3DXVECTOR3(x , y, 0.0f);
			}
		}
	}

	return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

// �o�g���̏���
void BattlePreparation(void)
{
	BATTLECURSOR *cursor = GetBattleCursor();

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		g_Player[i].canMove = FALSE;

		waitTime++;		// �����҂�����

		if (waitTime >= 60)
		{
			// �s���e�[�u���ɏ]���č��W�ړ��i���`��ԁj
			int nowNo = (int)g_Player[i].time;			// �������ł���e�[�u���ԍ������o���Ă���
			int maxNo = 1;								// �o�^�e�[�u�����𐔂��Ă���
			int nextNo = (nowNo + 1) % maxNo;			// �ړ���e�[�u���̔ԍ������߂Ă���
			D3DXVECTOR3	pos = g_MoveTbl.pos - g_Player[i].pos;	// XYZ�ړ��ʂ��v�Z���Ă���
			float nowTime = g_Player[i].time - nowNo;	// ���ԕ����ł��鏭�������o���Ă���
			pos *= nowTime;								// ���݂̈ړ��ʂ��v�Z���Ă���

			// �v�Z���ċ��߂��ړ��ʂ����݂̈ړ��e�[�u��XYZ�ɑ����Ă��遁�\�����W�����߂Ă���
			g_Player[i].pos += pos;

			g_Player[i].dir = g_Player[i].next_Dir;

			g_Player[i].countAnim++;
			if ((g_Player[i].countAnim % ANIM_WAIT) == 0)
			{
				// �p�^�[���̐؂�ւ�
				g_Player[i].patternAnim = (g_Player[i].dir * TEXTURE_PATTERN_DIVIDE_X) + ((g_Player[i].patternAnim + 1) % TEXTURE_PATTERN_DIVIDE_X);
			}

			// frame���g�Ď��Ԍo�ߏ���������
			g_Player[i].time += g_MoveTbl.time;						// ���Ԃ�i�߂Ă���
			if ((int)g_Player[i].time >= maxNo)						// �������I���������
			{
				// �v���C���[���������߂�
				switch (g_Player[i].dir)
				{
				case PLAYER_LOOK_UP:
					g_Player[i].dir = PLAYER_LOOK_DOWN;
					break;
				case PLAYER_LOOK_LEFT:
					g_Player[i].dir = PLAYER_LOOK_RIGHT;
					break;
				case PLAYER_LOOK_DOWN:
					g_Player[i].dir = PLAYER_LOOK_UP;
					break;
				case PLAYER_LOOK_RIGHT:
					g_Player[i].dir = PLAYER_LOOK_LEFT;
					break;
				}
				
				g_Player[i].texNo = 1;			// �o�g���̑f�ނɐ؂�ւ��

				g_Player[i].idle = TRUE;		// �v���C���[���A�C�h���ɂ���
				g_Player[i].battleReady = TRUE;// �������I������t���O��ON�ɂ���

				// �ϐ������Z�b�g����
				waitTime = 0;
				g_Player[i].time = 0;
				ShowBattleUI(BATTLE_UI);
				cursor->posIndex = 0;
				cursor->mode = MENU_SELECT;

				StopSound();
				PlaySound(SOUND_LABEL_BGM_battle);				
			}
		}
	}
}

// �U���A�j���[�V����
void AttackAnimation(int playerSprite)
{
	BATTLE *player = GetBattlePlayer();
	BATTLE *enemy = GetBattleEnemy();

	int attackAnim = playerSprite + 4;	// �v���C���[�̍U���A�j���[�V�������Z�b�g����

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		g_Player[i].countAnim++;
		if ((g_Player[i].countAnim % ANIM_WAIT) == 0)
		{
			// �p�^�[���̐؂�ւ�
			g_Player[i].patternAnim = ((attackAnim) * BATTLE_TEXTURE_PATTERN_DIVIDE_X) + ((g_Player[i].patternAnim + 1) % BATTLE_TEXTURE_PATTERN_DIVIDE_X);

			if (((g_Player[i].patternAnim + 1) % BATTLE_TEXTURE_PATTERN_DIVIDE_X) >= BATTLE_TEXTURE_PATTERN_DIVIDE_X - 1)
			{
				SetEffect(enemy->pos, ATTACK_EFFECT);	// �G�t�F�N�g��ݒ肷��
				g_Player[i].idle = TRUE;
				g_Player[i].action = PLAYER_WAIT;

				// �A�N�V�������X�g�b�v����
				enemy->stop = FALSE;
				player->stop = FALSE;
			}
		}
	}
}

// ���@�̃A�j���[�V����
void MagicAnimation(int playerSprite, int action)
{
	BATTLE *player = GetBattlePlayer();
	BATTLE *enemy = GetBattleEnemy();

	int castAnim = playerSprite + 8;

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		g_Player[i].countAnim++;
		if ((g_Player[i].countAnim % ANIM_WAIT) == 0)
		{
			// �p�^�[���̐؂�ւ�
			g_Player[i].patternAnim = ((castAnim) * BATTLE_TEXTURE_PATTERN_DIVIDE_X) + ((g_Player[i].patternAnim + 1) % BATTLE_TEXTURE_PATTERN_DIVIDE_X);

			if (((g_Player[i].patternAnim + 1) % BATTLE_TEXTURE_PATTERN_DIVIDE_X) >= BATTLE_TEXTURE_PATTERN_DIVIDE_X - 1)
			{
				g_Player[i].idle = TRUE;
				g_Player[i].action = PLAYER_WAIT;

				// �ǂ�Ȗ��@���g�����A�m�F
				if (action == PLAYER_CAST_FIREBALL)
				{
					SetBullet(player->pos, enemy->pos);
				}
				else if (action == PLAYER_CAST_HEAL)
				{
					SetEffect(player->pos, HEAL_EFFECT);
					g_Player[i].idle = TRUE;
					g_Player[i].action = PLAYER_WAIT;
				}
				else if (action == PLAYER_LIMIT_BREAK)
				{
					SetEffect(enemy->pos, LIMIT_BREAK_EFFECT);
					g_Player[i].idle = TRUE;
					g_Player[i].action = PLAYER_WAIT;

					// �A�N�V�������X�g�b�v����
					enemy->stop = FALSE;
					player->stop = FALSE;

				}
			}
		}
	}

}

// �v���C���[�����ɂ܂�
void PlayerDead(void)
{
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		// �����������炵�A�Ԃɂ���
		g_Player[i].g = g_Player[i].b = 0.0f;
		g_Player[i].a -= 0.1f;

		if (g_Player[i].a <= 0.0f)
		{
			g_Player[i].use = FALSE;
			SetFade(FADE_OUT, MODE_GAME_OVER);
		}
	}
}

// ���x���A�b�v
void LevelUp(void)
{
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		g_Player[i].level++;
		g_Player[i].expToNextLevel *= 2;
	}
}


//=============================================================================
// �v���C���[�\���̂̐擪�A�h���X���擾
//=============================================================================
PLAYER *GetPlayer(void)
{

	return &g_Player[0];
}




