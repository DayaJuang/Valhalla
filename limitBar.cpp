//=============================================================================
//
// バー処理 [Bar.cpp]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#include "limitBar.h"
#include "sprite.h"
#include "playerUI.h"
#include "battle.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(120)		// 数字のサイズ
#define TEXTURE_HEIGHT				(12)	// 
#define TEXTURE_MAX					(2)			// テクスチャの数


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[] = {
	"data/BATTLE_UI/ultimateBar.png",
	"data/BATTLE_UI/limitBarMax.png",
};

static BOOL		g_Load = FALSE;		// 初期化を行ったかのフラグ
static BAR		g_Bar;

static PLAYERUI *playerUI = GetPlayerUI();

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBar(void)
{
	ID3D11Device *pDevice = GetDevice();
	PLAYER *player = GetPlayer();

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

	// 変数の初期化
	g_Bar.w = player->limitBreak;
	g_Bar.h = TEXTURE_HEIGHT;
	g_Bar.texNo = 0;

	g_Bar.use = TRUE;

	g_Bar.pos = D3DXVECTOR3(playerUI->pos.x + 10.0f, playerUI->pos.y + 110.0f, 0.0f);

	g_Load = TRUE;		// データの初期化を行った
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBar(void)
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
void UpdateBar(void)
{
	PLAYER *player = GetPlayer();
	g_Bar.pos = D3DXVECTOR3(playerUI->pos.x + 10.0f, playerUI->pos.y + 110.0f, 0.0f);

	g_Bar.w = player->limitBreak;

	if (g_Bar.w >= WIDTH_MAX)
	{
		g_Bar.w = WIDTH_MAX;
		g_Bar.texNo = 1;
	}
	else
	{
		g_Bar.texNo = 0;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBar(void)
{
	if (playerUI->use == TRUE)
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
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Bar.texNo]);

		// バーの位置やテクスチャー座標を反映
		float px = g_Bar.pos.x;				// バーの表示位置X
		float py = g_Bar.pos.y;				// バーの表示位置Y
		float pw = g_Bar.w;					// バーの表示幅
		float ph = g_Bar.h;					// バーの表示高さ

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteLTColor(g_VertexBuffer,
			px, py, pw, ph,
			0.0f, 0.0f, 1.0f, 1.0f,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);

	}
}

void AddLimitBar(int point)
{
	PLAYER *player = GetPlayer();

	player->limitBreak += point;
}


//=============================================================================
// バーを加算する
// 引数:add :追加する点数。マイナスも可能
//=============================================================================
BAR *GetBar(void)
{
	return &g_Bar;
}

