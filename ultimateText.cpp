//=============================================================================
//
// バー処理 [UText.cpp]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#include "ultimateText.h"
#include "sprite.h"
#include "limitBar.h"
#include "battleUI.h"
#include "playerUI.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(100)		// 数字のサイズ
#define TEXTURE_HEIGHT				(28) 
#define TEXTURE_MAX					(1)			// テクスチャの数

#define TEXTURE_PATTERN_DIVIDE_X	(4)		// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y	(1)		// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define ANIM_WAIT					(16)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[] = {
	"data/TEXTURE/ultimateText.png",
};

static BOOL		g_Load = FALSE;		// 初期化を行ったかのフラグ
UTEXT			g_UText;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitUltimateText(void)
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

	// 変数の初期化
	g_UText.w = TEXTURE_WIDTH;
	g_UText.h = TEXTURE_HEIGHT;
	g_UText.texNo = 0;
	g_UText.countAnim = 0;
	g_UText.patternAnim = 0;

	g_UText.use = TRUE;

	g_UText.pos = D3DXVECTOR3(battleUI->pos.x + 10.0f, battleUI->pos.y - 40.0f, 0.0f);

	g_Load = TRUE;		// データの初期化を行った
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitUltimateText(void)
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
void UpdateUltimateText(void)
{
	BAR *limitBar = GetBar();
	BATTLEUI *battleUI = GetBattleUI();
	PLAYERUI *playerUI = GetPlayerUI();

	g_UText.pos = D3DXVECTOR3(battleUI->pos.x + 10.0f, battleUI->pos.y - 40.0f, 0.0f);

	if (limitBar->w >= WIDTH_MAX && playerUI->use == TRUE)
		g_UText.use = TRUE;
	else
		g_UText.use = FALSE;

	if (g_UText.use == TRUE)
	{
		// アニメーション  
		g_UText.countAnim++;
		if ((g_UText.countAnim % ANIM_WAIT) == 0)
		{
			g_UText.countAnim = 0;
			g_UText.patternAnim = ((g_UText.patternAnim + 1) % ANIM_PATTERN_NUM);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawUltimateText(void)
{
	BATTLEUI *battleUI = GetBattleUI();

	if (g_UText.use == TRUE)
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
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_UText.texNo]);

		// バーの位置やテクスチャー座標を反映
		float px = g_UText.pos.x;				// バーの表示位置X
		float py = g_UText.pos.y;				// バーの表示位置Y
		float pw = g_UText.w;					// バーの表示幅
		float ph = g_UText.h;					// バーの表示高さ

		float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X;	// テクスチャの幅
		float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// テクスチャの高さ
		float tx = (float)(g_UText.patternAnim % TEXTURE_PATTERN_DIVIDE_X) * tw;		// テクスチャの左上X座標
		float ty = (float)(g_UText.patternAnim / TEXTURE_PATTERN_DIVIDE_X) * th;		// テクスチャの左上Y座標

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteLTColor(g_VertexBuffer,
			px, py, pw, ph,
			 tx, ty, tw, th,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);

	}
#ifdef _DEBUG
	// デバッグ表示
	PrintDebugProc("UText X:%f Y:%f \n", g_UText.pos.x, g_UText.pos.y);
	PrintDebugProc("battleUI X:%f Y:%f \n", battleUI->pos.x, battleUI->pos.y);
#endif

}

//=============================================================================
// バーを加算する
// 引数:add :追加する点数。マイナスも可能
//=============================================================================
UTEXT *GetUltimateText(void)
{
	return &g_UText;
}

