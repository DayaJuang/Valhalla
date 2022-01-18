//=============================================================================
//
// Layer画面処理 [layer.cpp]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#include "layer.h"
#include "main.h"
#include "sprite.h"
#include "bg.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define LAYER_WIDTH					(2400)					// (SCREEN_WIDTH)
#define LAYER_HEIGHT				(2400)					// (SCREEN_HEIGHT)
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
	"data/MAP/wood_layer1.png",
	" ",
};


static BOOL	g_Load = FALSE;		// 初期化を行ったかのフラグ
static LAYER	g_Layer;


// 初期化処理
//=============================================================================
HRESULT InitLayer(void)
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
	int mapToShow = checkCurrentMap();

	g_Layer.w = LAYER_WIDTH;
	g_Layer.h = LAYER_HEIGHT;

	g_Layer.texNo = checkCurrentMap();
	g_Layer.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Layer.r = g_Layer.g = g_Layer.b = 1.0f;

	g_Load = TRUE;	// データの初期化を行った
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitLayer(void)
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
void UpdateLayer(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawLayer(void)
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

	// Layerの背景を描画
	{
		BG *bg = GetBG();

		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Layer.texNo]);

		float px = g_Layer.pos.x - bg->pos.x;	// レイヤーの表示位置X
		float py = g_Layer.pos.y - bg->pos.y;	// レイヤーの表示位置Y
		float pw = g_Layer.w;					// レイヤーの表示幅
		float ph = g_Layer.h;					// レイヤーの表示高さ

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteLTColor(g_VertexBuffer,
			px, py, pw, ph, 
			0.0f, 0.0f, 1.0f, 1.0f,
			D3DXCOLOR(g_Layer.r, g_Layer.g, g_Layer.b, 1.0f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}

}

LAYER *GetLayer(void) {

	return &g_Layer;
}





