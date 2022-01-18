//=============================================================================
//
// バー処理 [Bar.cpp]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#include "actionBarFrame.h"
#include "actionBar.h"
#include "sprite.h"
#include "battle.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(ACTION_BAR_MAX)		// 数字のサイズ
#define TEXTURE_HEIGHT				(8) 
#define TEXTURE_MAX					(1)						// テクスチャの数


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[] = {
	"data/BATTLE_UI/actioBarFrame.png",
};

static BOOL		g_Load = FALSE;					// 初期化を行ったかのフラグ
static ACTIONBARFRAME	g_BarFrame[TURN_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitActionBarFrame(void)
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

	for (int i = 0; i < TURN_MAX; i++)
	{
		// 変数の初期化
		g_BarFrame[i].w = TEXTURE_WIDTH;
		g_BarFrame[i].h = TEXTURE_HEIGHT;
		g_BarFrame[i].texNo = 0;
		g_BarFrame[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_BarFrame[i].use = FALSE;

	}

	g_Load = TRUE;		// データの初期化を行った
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitActionBarFrame(void)
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
void UpdateActionBarFrame(void)
{
	PLAYER *player = GetPlayer();
	BATTLE *battlePlayer = GetBattlePlayer();
	BATTLE *enemy = GetBattleEnemy();

	for (int i = 0; i < TURN_MAX; i++)
	{
		if (player->battleReady && battlePlayer->hp > 0 && enemy->hp > 0)
		{
			g_BarFrame[i].use = TRUE;
		}
		else
		{
			g_BarFrame[i].use = FALSE;
		}
	}

	g_BarFrame[PLAYER1_FRAME].pos = D3DXVECTOR3(battlePlayer->pos.x - g_BarFrame[PLAYER1_FRAME].w / 2, battlePlayer->pos.y - 30.0f, 0.0f);
	g_BarFrame[ENEMY1_FRAME].pos = D3DXVECTOR3(enemy->pos.x - g_BarFrame[PLAYER1_FRAME].w / 2, enemy->pos.y - 30.0f, 0.0f);
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawActionBarFrame(void)
{
	for (int i = 0; i < TURN_MAX; i++)
	{
		if (g_BarFrame[i].use == TRUE)
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

			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_BarFrame[i].texNo]);

			// バーの位置やテクスチャー座標を反映
			float px = g_BarFrame[i].pos.x;				// バーの表示位置X
			float py = g_BarFrame[i].pos.y;				// バーの表示位置Y
			float pw = g_BarFrame[i].w;					// バーの表示幅
			float ph = g_BarFrame[i].h;					// バーの表示高さ

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

ACTIONBARFRAME *GetActionBarFrame(int index)
{
	return &g_BarFrame[index];
}

