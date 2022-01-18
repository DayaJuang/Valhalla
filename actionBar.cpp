//=============================================================================
//
// バー処理 [Bar.cpp]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#include "actionBar.h"
#include "actionBarFrame.h"
#include "sprite.h"
#include "playerUI.h"
#include "battle.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_HEIGHT				(8)		// 
#define TEXTURE_MAX					(1)			// テクスチャの数


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[] = {
	"data/BATTLE_UI/actionBar.png",
};

static BOOL		g_Load = FALSE;					// 初期化を行ったかのフラグ
static ACTIONBAR	g_ActionBar[TURN_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitActionBar(void)
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
		g_ActionBar[i].w = 0.0f;
		g_ActionBar[i].h = TEXTURE_HEIGHT;
		g_ActionBar[i].texNo = 0;
		g_ActionBar[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_ActionBar[i].use = FALSE;

	}

	g_Load = TRUE;		// データの初期化を行った
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitActionBar(void)
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
void UpdateActionBar(void)
{
	BATTLE *player = GetBattlePlayer();
	BATTLE *enemy = GetBattleEnemy();

	ACTIONBARFRAME *playerFrame = GetActionBarFrame(PLAYER1_FRAME);
	ACTIONBARFRAME *enemyFrame = GetActionBarFrame(ENEMY1_FRAME);

	g_ActionBar[PLAYER1_BAR].use = playerFrame->use;
	g_ActionBar[ENEMY1_BAR].use = enemyFrame->use;

	g_ActionBar[PLAYER1_BAR].pos = D3DXVECTOR3(playerFrame->pos.x + 2.5f, playerFrame->pos.y, 0.0f);;
	g_ActionBar[ENEMY1_BAR].pos = D3DXVECTOR3(enemyFrame->pos.x + 2.5f, enemyFrame->pos.y, 0.0f);

	g_ActionBar[PLAYER1_BAR].w = player->action;
	g_ActionBar[ENEMY1_BAR].w = enemy->action;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawActionBar(void)
{
	for (int i = 0; i < TURN_MAX; i++)
	{
		if (g_ActionBar[i].use == TRUE)
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
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_ActionBar[i].texNo]);

			// バーの位置やテクスチャー座標を反映
			float px = g_ActionBar[i].pos.x;				// バーの表示位置X
			float py = g_ActionBar[i].pos.y;				// バーの表示位置Y
			float pw = g_ActionBar[i].w;					// バーの表示幅
			float ph = g_ActionBar[i].h;					// バーの表示高さ

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

