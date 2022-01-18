//=============================================================================
//
// BattleUI画面処理 [BattleUI.cpp]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#include "battleUI.h"
#include "main.h"
#include "sprite.h"
#include "player.h"
#include "bg.h"
#include "battle.h"
#include "battleCursor.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(140)	// キャラサイズ
#define TEXTURE_HEIGHT				(140)	// 

#define TEXTURE_MAX					(3)						// テクスチャの数


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[TEXTURE_MAX] = {
	"data/BATTLE_UI/battleUI2.png",
	"data/BATTLE_UI/magicUI2.png",
	"data/BATTLE_UI/powerUPLog.png",
};

static BOOL	g_Load = FALSE;		// 初期化を行ったかのフラグ
static BATTLEUI	g_BattleUI;
static D3DXVECTOR3 positions[3] =
{
	D3DXVECTOR3(30.0f, 390.0f, 0.0f),
	D3DXVECTOR3(670.0f, 390.0f, 0.0f),
	D3DXVECTOR3((SCREEN_WIDTH / 2) - (TEXTURE_WIDTH / 2), SCREEN_HEIGHT / 2, 0.0f)
};


// 初期化処理
//=============================================================================
HRESULT InitBattleUI(void)
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

	g_BattleUI.w = TEXTURE_WIDTH;
	g_BattleUI.h = TEXTURE_HEIGHT;

	g_BattleUI.texNo = 0;
	g_BattleUI.pos = positions[0];

	g_BattleUI.use = FALSE;

	g_Load = TRUE;	// データの初期化を行った
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBattleUI(void)
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
void UpdateBattleUI(void)
{
	PLAYER *player = GetPlayer();
	BATTLE *battlePlayer = GetBattlePlayer();
	BATTLE *enemy = GetBattleEnemy();
	BATTLECURSOR *cursor = GetBattleCursor();
	BG *bg = GetBG();

	if (g_BattleUI.texNo == POWERUP_UI)
	{
		cursor->canDoAction = TRUE;
		cursor->mode = POWERUP_SELECT;
		g_BattleUI.pos = positions[POWER_UP_PANEL];
		return;
	}
	else if (player->mode != PLAYER_MODE_BATTLE && (enemy->hp <= 0 || battlePlayer->hp <= 0))
	{
		g_BattleUI.use = FALSE;
	}
	else
	{
		// プレイヤーやエネミーの座標が画面の右にあるか、左にあるか
		if ((((player->pos.x + bg->pos.x)) < SCREEN_WIDTH / 2 && ((player->pos.y + bg->pos.y)) > SCREEN_HEIGHT / 2)
			|| (((enemy->pos.x + bg->pos.x)) < SCREEN_WIDTH / 2 && ((enemy->pos.y + bg->pos.y)) > SCREEN_HEIGHT / 2))
		{
			g_BattleUI.pos = positions[BATTLE_UI_RIGHT];
		}
		else
		{
			g_BattleUI.pos = positions[BATTLE_UI_LEFT];
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBattleUI(void)
{
	if (g_BattleUI.use == TRUE)
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

		// BattleUIの背景を描画
		{
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_BattleUI.texNo]);

			float px = g_BattleUI.pos.x;	// 表示位置X
			float py = g_BattleUI.pos.y;	// 表示位置Y
			float pw = g_BattleUI.w;		// 表示幅
			float ph = g_BattleUI.h;		// 表示高さ

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteLTColor(g_VertexBuffer,
				px, py, pw, ph,
				0.0f, 0.0f, 1.0f, 1.0f,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}

	}

}

void ShowBattleUI(int texNo)
{
	BATTLECURSOR *cursor = GetBattleCursor();

	g_BattleUI.texNo = texNo;
	g_BattleUI.use = TRUE;
	cursor->posIndex = 0;
}

//=============================================================================
// BattleUI構造体の先頭アドレスを取得
//=============================================================================
BATTLEUI *GetBattleUI(void)
{
	return &g_BattleUI;
}





