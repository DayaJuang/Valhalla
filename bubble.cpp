//=============================================================================
//
// スコア処理 [Bubble.cpp]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#include "bubble.h"
#include "sprite.h"
#include "battle.h"
#include "bg.h"
#include "layer.h"
#include "actionBarFrame.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(50)	// 数字のサイズ
#define TEXTURE_HEIGHT				(50)
#define TEXTURE_MAX					(3)		// テクスチャの数


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[] = {
	"data/BATTLE_UI/attackBubble.png",
	"data/BATTLE_UI/fireballBubble.png",
	"data/BATTLE_UI/cureBubble.png",

};

static BOOL		g_Load = FALSE;		// 初期化を行ったかのフラグ
static BUBBLE	g_Bubble;

static float floatTime = 0;


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBubble(void)
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


	// 変数の初期化
	g_Bubble.w = TEXTURE_WIDTH;
	g_Bubble.h = TEXTURE_HEIGHT;
	g_Bubble.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Bubble.texNo = 0;

	g_Bubble.use = FALSE;

	g_Load = TRUE;		// データの初期化を行った
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBubble(void)
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
void UpdateBubble(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBubble(void)
{
	if (g_Bubble.use == TRUE)
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
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Bubble.texNo]);

		// スコアの位置やテクスチャー座標を反映
		float px = g_Bubble.pos.x;
		float py = g_Bubble.pos.y - 75.0f;
		float pw = g_Bubble.w;
		float ph = g_Bubble.h;

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteLTColor(g_VertexBuffer,
			px, py, pw, ph,
			0.0f, 0.0f, 1.0f, 1.0f,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);

	}

}

void SetBubble(D3DXVECTOR3 targetPos, int textNo)
{
	g_Bubble.pos = targetPos;
	g_Bubble.texNo = textNo;
	g_Bubble.use = TRUE;
}

BUBBLE *GetBubble(void)
{
	return &g_Bubble;
}

