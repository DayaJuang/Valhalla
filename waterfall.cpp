//=============================================================================
//
// Waterfall画面処理 [waterfall.cpp]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#include "waterfall.h"
#include "main.h"
#include "sprite.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define Waterfall_WIDTH					(SCREEN_WIDTH)					// (SCREEN_WIDTH)
#define Waterfall_HEIGHT				(SCREEN_HEIGHT)					// (SCREEN_HEIGHT)
#define TEXTURE_MAX						(1)								// テクスチャの数

#define TEXTURE_PATTERN_DIVIDE_X	(5)													// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y	(1)													// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define ANIM_WAIT					(16)												// アニメーションの切り替わるWait値



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/waterFall.png"
};


static BOOL	g_Load = FALSE;		// 初期化を行ったかのフラグ
static WATERFALL	g_Waterfall;


// 初期化処理
//=============================================================================
HRESULT InitWaterfall(void)
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
	g_Waterfall.w = Waterfall_WIDTH;
	g_Waterfall.h = Waterfall_HEIGHT;

	g_Waterfall.texNo = checkCurrentMap();
	g_Waterfall.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Waterfall.countAnim = 0;
	g_Waterfall.patternAnim = 0;

	g_Load = TRUE;	// データの初期化を行った
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitWaterfall(void)
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
void UpdateWaterfall(void)
{
	// アニメーション  
	g_Waterfall.countAnim++;
	if ((g_Waterfall.countAnim % ANIM_WAIT) == 0)
	{
		// パターンの切り替え
		g_Waterfall.patternAnim = ((g_Waterfall.patternAnim + 1) % TEXTURE_PATTERN_DIVIDE_X);
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawWaterfall(void)
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

	// Waterfallの背景を描画
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Waterfall.texNo]);

		float px = g_Waterfall.pos.x;	// 表示位置X
		float py = g_Waterfall.pos.y;	// 表示位置Y
		float pw = g_Waterfall.w;		// 表示幅
		float ph = g_Waterfall.h;		// 表示高さ

		// アニメーション用
		float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X;	// テクスチャの幅
		float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// テクスチャの高さ
		float tx = (float)(g_Waterfall.patternAnim % TEXTURE_PATTERN_DIVIDE_X) * tw;	// テクスチャの左上X座標
		float ty = (float)(g_Waterfall.patternAnim / TEXTURE_PATTERN_DIVIDE_X) * th;	// テクスチャの左上Y座標


		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteLTColor(g_VertexBuffer,
			px, py, pw, ph, 
			tx, ty, tw, th,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}

}




