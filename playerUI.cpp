//=============================================================================
//
// PlayerUI画面処理 [playerUI.cpp]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#include "playerUI.h"
#include "main.h"
#include "sprite.h"
#include "battleUI.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(140)	// キャラサイズ
#define TEXTURE_HEIGHT				(140)	// 

#define TEXTURE_MAX					(1)						// テクスチャの数


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[TEXTURE_MAX] = {
	"data/BATTLE_UI/playerDetails.png"

};

static BOOL	g_Load = FALSE;		// 初期化を行ったかのフラグ
static PLAYERUI	g_PlayerUI;


// 初期化処理
//=============================================================================
HRESULT InitPlayerUI(void)
{
	ID3D11Device *pDevice = GetDevice();
	BATTLEUI *battleUI = GetBattleUI();

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

	g_PlayerUI.w = TEXTURE_WIDTH;
	g_PlayerUI.h = TEXTURE_HEIGHT;

	g_PlayerUI.texNo = 0;
	g_PlayerUI.pos = D3DXVECTOR3(battleUI->pos.x + 140.0f, battleUI->pos.y, 0.0f);

	g_PlayerUI.use = FALSE;

	g_Load = TRUE;	// データの初期化を行った
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayerUI(void)
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
void UpdatePlayerUI(void)
{
	BATTLEUI *battleUI = GetBattleUI();

	g_PlayerUI.use = battleUI->use;

	if (battleUI->texNo == POWERUP_UI)
		g_PlayerUI.use = FALSE;

	g_PlayerUI.pos = D3DXVECTOR3(battleUI->pos.x + 140.0f, battleUI->pos.y, 0.0f);
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayerUI(void)
{
	if (g_PlayerUI.use == TRUE)
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

		// PlayerUIの背景を描画
		{
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_PlayerUI.texNo]);

			float px = g_PlayerUI.pos.x;	// 表示位置X
			float py = g_PlayerUI.pos.y;	// 表示位置Y
			float pw = g_PlayerUI.w;		// 表示幅
			float ph = g_PlayerUI.h;		// 表示高さ

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

//=============================================================================
// PlayerUI構造体の先頭アドレスを取得
//=============================================================================
PLAYERUI *GetPlayerUI(void)
{
	return &g_PlayerUI;
}





