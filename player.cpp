//=============================================================================
//
// プレイヤー処理 [player.cpp]
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
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(140/2)	// キャラサイズ
#define TEXTURE_HEIGHT				(140/2)	// 
#define TEXTURE_MAX					(2)		// テクスチャの数

#define TEXTURE_PATTERN_DIVIDE_X	(8)		// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y	(5)		// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数

// バトル用のアニメーション
#define BATTLE_TEXTURE_PATTERN_DIVIDE_X	(9)		// アニメパターンのテクスチャ内分割数（X)
#define BATTLE_TEXTURE_PATTERN_DIVIDE_Y	(12)		// アニメパターンのテクスチャ内分割数（Y)
#define BATTLE_ANIM_PATTERN_NUM			(BATTLE_TEXTURE_PATTERN_DIVIDE_X*BATTLE_TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数

#define ANIM_WAIT						(4)		// アニメーションの切り替わるWait値

// プレイヤーの画面内配置座標
#define PLAYER_DISP_X				(SCREEN_WIDTH/2)
#define PLAYER_DISP_Y				(SCREEN_HEIGHT/2 + TEXTURE_HEIGHT)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/Character2.png",
	"data/TEXTURE/battle_Character.png"
};

static LINEAR_INTERPOLATION g_MoveTbl;

static BOOL		g_Load = FALSE;			// 初期化を行ったかのフラグ
static PLAYER	g_Player[PLAYER_MAX];	// プレイヤー構造体
static int waitTime = 0;
static BOOL firstTime = TRUE;			// 初めてゲームをプレイするかどうか確認変数

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(void)
{
	ID3D11Device *pDevice = GetDevice();

	//テクスチャ生成
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


	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	// プレイヤー構造体の初期化
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

		g_Player[i].move = D3DXVECTOR3(3.0f, 0.0f, 0.0f);		// 移動量

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
			// バトル用のステータス
			g_Player[i].level = 1;
			g_Player[i].currentExp = 0;			// 現在の経験値
			g_Player[i].expToNextLevel = 35;	// 必要な経験値
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

	g_Load = TRUE;	// データの初期化を行った
	return S_OK;
}

//=============================================================================
// 終了処理
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
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	TUTORIAL *tutorial = GetTutorial();

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		// チュートリアルを表示
		if (firstTime)
		{
			firstTime = FALSE;
			g_Player[i].canMove = FALSE;
			ShowTutorial(FIELD_TUTORIAL1, FIELD_TUTORIAL);
		}
		else
		{
			// プレイヤーの攻撃を更新
			g_Player[i].atkMin = g_Player[i].atkPower - 2;
			g_Player[i].atkMax = g_Player[i].atkPower + 2;

			// プレイヤーがまだ生きているかどうか
			if (g_Player[i].hp <= 0)
			{
				PlayerDead();	// 死にます
			}

			// 探検のモード
			if (g_Player[i].mode == PLAYER_MODE_EXPLORE)
			{
				float speed = g_Player[i].move.x;	// 移動スピード

				// チュートリアルがまだ表示しているとき、移動できません
				if(!tutorial->use)
					g_Player[i].canMove = TRUE;

				// 昔座標の登録
				g_Player[i].oldPos.x = g_Player[i].pos.x;
				g_Player[i].oldPos.y = g_Player[i].pos.y;

				// 生きてるプレイヤーだけ処理をする
				if (g_Player[i].use == TRUE && g_Player[i].canMove)
				{
					// 地形との当たり判定用に座標のバックアップを取っておく
					D3DXVECTOR3 pos_old = g_Player[i].pos;

					// アニメーション  
					g_Player[i].countAnim += 1;

					if (g_Player[i].countAnim > ANIM_WAIT)
					{
						g_Player[i].countAnim = 0;
						// パターンの切り替え
						g_Player[i].patternAnim = (g_Player[i].patternAnim + 1) % ANIM_PATTERN_NUM;

						// 足元の音の処理
						if (((g_Player[i].patternAnim + 1) % ANIM_WAIT) == 0  && g_Player[i].idle == FALSE)
						{
							PlaySound(SOUND_LABEL_SE_footstep);
						}
					}

					// キー入力で移動 
					{
						if (GetKeyboardPress(DIK_DOWN))
						{
							if (GetKeyboardPress(DIK_RIGHT) || GetKeyboardPress(DIK_LEFT))
								speed *= 0.7f;	// 斜めのスピードが約1倍にする

							g_Player[i].idle = FALSE;
							g_Player[i].pos.y += speed;
							g_Player[i].dir = 2;

						}
						else if (GetKeyboardPress(DIK_UP))
						{
							if (GetKeyboardPress(DIK_RIGHT) || GetKeyboardPress(DIK_LEFT))
								speed *= 0.7f;	// 斜めのスピードが約1倍にする

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

						// ゲームパッドでで移動処理
						if (IsButtonPressed(0, BUTTON_DOWN))
						{
							if (IsButtonPressed(0, BUTTON_RIGHT) || IsButtonPressed(0, BUTTON_LEFT))
								speed *= 0.7f;	// 斜めのスピードが約1倍にする

							g_Player[i].idle = FALSE;
							g_Player[i].pos.y += speed;
							g_Player[i].dir = PLAYER_LOOK_DOWN;
						}
						else if (IsButtonPressed(0, BUTTON_UP))
						{
							if (IsButtonPressed(0, BUTTON_RIGHT) || IsButtonPressed(0, BUTTON_LEFT))
								speed *= 0.7f;	// 斜めのスピードが約1倍にする

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

						// 何も押してないとき、プレイヤーがアイドルの状態になる
						if ((!IsButtonPressed(0, BUTTON_DOWN) && !IsButtonPressed(0, BUTTON_UP) && !IsButtonPressed(0, BUTTON_RIGHT) && !IsButtonPressed(0, BUTTON_LEFT))
							&& (GetKeyboardRelease(DIK_UP) || GetKeyboardRelease(DIK_DOWN) || GetKeyboardRelease(DIK_RIGHT) || GetKeyboardRelease(DIK_LEFT)))
							g_Player[i].idle = TRUE;

						// MAP外チェック
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

						// プレイヤーの当たり判定チェック
						int checkTop = CheckMap(g_Player[i].pos.x, g_Player[i].pos.y);
						int checkLeft = CheckMap(g_Player[i].pos.x - g_Player[i].w / 5, g_Player[i].pos.y);
						int checkRight = CheckMap(g_Player[i].pos.x + g_Player[i].w / 5, g_Player[i].pos.y);
						int checkBot = CheckMap(g_Player[i].pos.x, g_Player[i].pos.y + g_Player[i].h / 2);

						// 通れない
						if ((checkTop == COLLISION) || (checkLeft == COLLISION) || (checkRight == COLLISION) || (checkBot == COLLISION))
						{
							g_Player[i].pos.x = g_Player[i].oldPos.x;
							g_Player[i].pos.y = g_Player[i].oldPos.y;
						}
						else if ((checkTop == TO_WORLDMAP) || (checkLeft == TO_WORLDMAP) || (checkRight == TO_WORLDMAP) || (checkBot == TO_WORLDMAP))
						{
							// ワードマップに移動
							g_Player[i].idle = TRUE;
							g_Player[i].canMove = FALSE;
							g_Player[i].pos.x = g_Player[i].oldPos.x;
							g_Player[i].pos.y = g_Player[i].oldPos.y;
							SetFade(FADE_OUT, MODE_WORLDMAP);
						}
						else if ((checkTop == TO_WORLDMAP_NEXT_AREA) || (checkLeft == TO_WORLDMAP_NEXT_AREA) || (checkRight == TO_WORLDMAP_NEXT_AREA) || (checkBot == TO_WORLDMAP_NEXT_AREA))
						{
							// ワードマップに移動して、次のフィールドを表示する
							ICONWM *icon = GetIconWM(g_Player[i].currentLocation + 1);
							icon->use = TRUE;
							g_Player[i].idle = TRUE;
							g_Player[i].canMove = FALSE;
							g_Player[i].pos.x = g_Player[i].oldPos.x;
							g_Player[i].pos.y = g_Player[i].oldPos.y;
							SetFade(FADE_OUT, MODE_WORLDMAP);
						}

						// プレイヤーの立ち位置からMAPのスクロール座標を計算する
						bg->pos.x = g_Player[i].pos.x - PLAYER_DISP_X;
						if (bg->pos.x < 0) bg->pos.x = 0;
						if (bg->pos.x > bg->w - SCREEN_WIDTH) bg->pos.x = bg->w - SCREEN_WIDTH;

						bg->pos.y = g_Player[i].pos.y - PLAYER_DISP_Y;
						if (bg->pos.y < 0) bg->pos.y = 0;
						if (bg->pos.y > bg->h - SCREEN_HEIGHT) bg->pos.y = bg->h - SCREEN_HEIGHT;

						// 移動が終わったらエネミーとの当たり判定
						{
							ENEMY *enemy = GetEnemy();

							// エネミーの数分当たり判定を行う
							for (int j = 0; j < ENEMY_MAX; j++)
							{
								// 生きてるエネミーと当たり判定をする
								if (enemy[j].use == TRUE)
								{
									BOOL ans = CollisionBB(g_Player[i].pos, g_Player[i].w, g_Player[i].h,
										enemy[j].pos, enemy[j].w, enemy[j].h);
									// 当たっている？
									if (ans == TRUE)
									{
										PlaySound(SOUND_LABEL_SE_battleStart);

										// プレイヤーやエネミーのモードの切替
										g_Player[i].mode = PLAYER_MODE_BATTLE;
										enemy[j].mode = ENEMY_MODE_BATTLE;

										// プレイヤーのバトル座標を決める
										float posDiffX = g_Player[i].pos.x - enemy[j].pos.x;
										float posDiffY = g_Player[i].pos.y - enemy[j].pos.y;
										int ans = abs((int)posDiffX) - abs((int)posDiffY);

										if (posDiffX >= 0 && posDiffY >= 0)
										{
											if (ans > 0)
											{
												g_Player[i].next_Dir = PLAYER_LOOK_RIGHT;
												g_MoveTbl =
													//座標														回転率					Z		拡大率			X		Y		時間
												{ D3DXVECTOR3(enemy[j].pos.x + 150.0f, enemy[j].pos.y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 0.02f };

											}
											else
											{
												g_Player[i].next_Dir = PLAYER_LOOK_DOWN;
												g_MoveTbl =
													//座標														回転率					Z		拡大率			X		Y		時間
												{ D3DXVECTOR3(enemy[j].pos.x, enemy[j].pos.y + 150.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 0.02f };
											}
										}
										else if (posDiffX >= 0 && posDiffY <= 0)
										{
											if (ans > 0)
											{
												g_Player[i].next_Dir = PLAYER_LOOK_RIGHT;
												g_MoveTbl =
													//座標														回転率					Z		拡大率			X		Y		時間
												{ D3DXVECTOR3(enemy[j].pos.x + 150.0f, enemy[j].pos.y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 0.02f };
											}
											else
											{
												g_Player[i].next_Dir = PLAYER_LOOK_UP;
												g_MoveTbl =
													//座標														回転率					Z		拡大率			X		Y		時間
												{ D3DXVECTOR3(enemy[j].pos.x, enemy[j].pos.y - 150.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 0.02f };
											}
										}
										else if (posDiffX <= 0 && posDiffY >= 0)
										{
											if (ans > 0)
											{
												g_Player[i].next_Dir = PLAYER_LOOK_LEFT;
												g_MoveTbl =
													//座標														回転率					Z		拡大率			X		Y		時間
												{ D3DXVECTOR3(enemy[j].pos.x - 150.0f, enemy[j].pos.y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 0.02f };
											}
											else
											{
												g_Player[i].next_Dir = PLAYER_LOOK_DOWN;
												g_MoveTbl =
													//座標														回転率					Z		拡大率			X		Y		時間
												{ D3DXVECTOR3(enemy[j].pos.x, enemy[j].pos.y + 150.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 0.02f };
											}
										}
										else
										{
											if (ans > 0)
											{
												g_Player[i].next_Dir = PLAYER_LOOK_LEFT;
												g_MoveTbl =
													//座標														回転率					Z		拡大率			X		Y		時間
												{ D3DXVECTOR3(enemy[j].pos.x - 150.0f, enemy[j].pos.y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 0.02f };
											}
											else
											{
												g_Player[i].next_Dir = PLAYER_LOOK_UP;
												g_MoveTbl =
													//座標														回転率					Z		拡大率			X		Y		時間
												{ D3DXVECTOR3(enemy[j].pos.x, enemy[j].pos.y - 150.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 0.02f };
											}
										}

										// バトル準備
										g_Player[i].battlePos = g_MoveTbl.pos;
										SetBattle(j);
									}
								}
							}
						}

						// バレット処理
						if (GetKeyboardTrigger(DIK_SPACE))
						{// Enter押したら、ステージを切り替える
							D3DXVECTOR3 pos = g_Player[i].pos;

						}
						if (IsButtonTriggered(0, BUTTON_B))
						{// Enter押したら、ステージを切り替える
							D3DXVECTOR3 pos = g_Player[i].pos;
						}

					}
				}
			}
			else if (g_Player[i].mode == PLAYER_MODE_BATTLE)	// バトルモードの処理
			{
				// バトルの準備
				if (!g_Player[i].battleReady)
				{
					BattlePreparation();
				}
				else
				{
					// バトルの処理
					BATTLEUI *battleUI = GetBattleUI();
					BATTLE *enemy = GetBattleEnemy();

					// 現在はどんなアクションをやっているか?
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
			// デバッグ表示
			PrintDebugProc("Player No%d  X:%f Y:%f \n", i, g_Player[i].pos.x, g_Player[i].pos.y);
#endif

		}
	}
	
}


//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// マトリクス設定
	SetWorldViewProjection2D();

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	BG *bg = GetBG();

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (g_Player[i].use == TRUE)		// このプレイヤーが使われている？
		{									// Yes
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Player[i].texNo]);

			//プレイヤーの位置やテクスチャー座標を反映
			float px = g_Player[i].pos.x - bg->pos.x;	// プレイヤーの表示位置X
			float py = g_Player[i].pos.y - bg->pos.y;	// プレイヤーの表示位置Y
			float pw = g_Player[i].w;		// プレイヤーの表示幅
			float ph = g_Player[i].h;		// プレイヤーの表示高さ

				// アニメーション用
			float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X;	// テクスチャの幅
			float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// テクスチャの高さ

			float tx = 0.0f;	// テクスチャの左上X座標
			float ty = 0.0f;	// テクスチャの左上Y座標

			float gridWide = 0.0f;	// 素材のグリッド幅

			if (g_Player[i].idle)	// Idleアニメーション
			{
				gridWide = 0.125f;

				tx = gridWide * g_Player[i].dir;
				ty = 0.0f;
			}
			else					// 走るアニメーション
			{
				gridWide = 0.2f;

				tx = (float)(g_Player[i].patternAnim % TEXTURE_PATTERN_DIVIDE_X) * tw;	// テクスチャの左上X座標
				ty = gridWide*(g_Player[i].dir + 1);										// テクスチャの左上Y座標
			}

			// バトルプレイヤーのアニメーション
			if (g_Player[i].battleReady)
			{
				tw = 1.0f / BATTLE_TEXTURE_PATTERN_DIVIDE_X;	// テクスチャの幅
				th = 1.0f / BATTLE_TEXTURE_PATTERN_DIVIDE_Y;	// テクスチャの高さ
				tx = (float)(g_Player[i].patternAnim % BATTLE_TEXTURE_PATTERN_DIVIDE_X) * tw;	// テクスチャの左上X座標
				ty = (float)(g_Player[i].patternAnim / BATTLE_TEXTURE_PATTERN_DIVIDE_X) * th;	// テクスチャの左上Y座標
			}

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Player[i].a),
				g_Player[i].rot.z);

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}
	}
}

// プレイヤーの座標決める関数
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

// バトルの準備
void BattlePreparation(void)
{
	BATTLECURSOR *cursor = GetBattleCursor();

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		g_Player[i].canMove = FALSE;

		waitTime++;		// 少し待ち処理

		if (waitTime >= 60)
		{
			// 行動テーブルに従って座標移動（線形補間）
			int nowNo = (int)g_Player[i].time;			// 整数分であるテーブル番号を取り出している
			int maxNo = 1;								// 登録テーブル数を数えている
			int nextNo = (nowNo + 1) % maxNo;			// 移動先テーブルの番号を求めている
			D3DXVECTOR3	pos = g_MoveTbl.pos - g_Player[i].pos;	// XYZ移動量を計算している
			float nowTime = g_Player[i].time - nowNo;	// 時間部分である少数を取り出している
			pos *= nowTime;								// 現在の移動量を計算している

			// 計算して求めた移動量を現在の移動テーブルXYZに足している＝表示座標を求めている
			g_Player[i].pos += pos;

			g_Player[i].dir = g_Player[i].next_Dir;

			g_Player[i].countAnim++;
			if ((g_Player[i].countAnim % ANIM_WAIT) == 0)
			{
				// パターンの切り替え
				g_Player[i].patternAnim = (g_Player[i].dir * TEXTURE_PATTERN_DIVIDE_X) + ((g_Player[i].patternAnim + 1) % TEXTURE_PATTERN_DIVIDE_X);
			}

			// frameを使て時間経過処理をする
			g_Player[i].time += g_MoveTbl.time;						// 時間を進めている
			if ((int)g_Player[i].time >= maxNo)						// 準備が終わった処理
			{
				// プレイヤー方向を決める
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
				
				g_Player[i].texNo = 1;			// バトルの素材に切り替わる

				g_Player[i].idle = TRUE;		// プレイヤーがアイドルにする
				g_Player[i].battleReady = TRUE;// 準備が終わったフラグがONにする

				// 変数をリセットする
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

// 攻撃アニメーション
void AttackAnimation(int playerSprite)
{
	BATTLE *player = GetBattlePlayer();
	BATTLE *enemy = GetBattleEnemy();

	int attackAnim = playerSprite + 4;	// プレイヤーの攻撃アニメーションをセットする

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		g_Player[i].countAnim++;
		if ((g_Player[i].countAnim % ANIM_WAIT) == 0)
		{
			// パターンの切り替え
			g_Player[i].patternAnim = ((attackAnim) * BATTLE_TEXTURE_PATTERN_DIVIDE_X) + ((g_Player[i].patternAnim + 1) % BATTLE_TEXTURE_PATTERN_DIVIDE_X);

			if (((g_Player[i].patternAnim + 1) % BATTLE_TEXTURE_PATTERN_DIVIDE_X) >= BATTLE_TEXTURE_PATTERN_DIVIDE_X - 1)
			{
				SetEffect(enemy->pos, ATTACK_EFFECT);	// エフェクトを設定する
				g_Player[i].idle = TRUE;
				g_Player[i].action = PLAYER_WAIT;

				// アクションをストップする
				enemy->stop = FALSE;
				player->stop = FALSE;
			}
		}
	}
}

// 魔法のアニメーション
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
			// パターンの切り替え
			g_Player[i].patternAnim = ((castAnim) * BATTLE_TEXTURE_PATTERN_DIVIDE_X) + ((g_Player[i].patternAnim + 1) % BATTLE_TEXTURE_PATTERN_DIVIDE_X);

			if (((g_Player[i].patternAnim + 1) % BATTLE_TEXTURE_PATTERN_DIVIDE_X) >= BATTLE_TEXTURE_PATTERN_DIVIDE_X - 1)
			{
				g_Player[i].idle = TRUE;
				g_Player[i].action = PLAYER_WAIT;

				// どんな魔法を使うか、確認
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

					// アクションをストップする
					enemy->stop = FALSE;
					player->stop = FALSE;

				}
			}
		}
	}

}

// プレイヤーが死にます
void PlayerDead(void)
{
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		// 透明さを減らし、赤にする
		g_Player[i].g = g_Player[i].b = 0.0f;
		g_Player[i].a -= 0.1f;

		if (g_Player[i].a <= 0.0f)
		{
			g_Player[i].use = FALSE;
			SetFade(FADE_OUT, MODE_GAME_OVER);
		}
	}
}

// レベルアップ
void LevelUp(void)
{
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		g_Player[i].level++;
		g_Player[i].expToNextLevel *= 2;
	}
}


//=============================================================================
// プレイヤー構造体の先頭アドレスを取得
//=============================================================================
PLAYER *GetPlayer(void)
{

	return &g_Player[0];
}




