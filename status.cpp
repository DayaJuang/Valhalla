//=============================================================================
//
// スコア処理 [status.cpp]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#include "status.h"
#include "sprite.h"
#include "playerUI.h"
#include "battle.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(20)	// 数字のサイズ
#define TEXTURE_HEIGHT				(20)	// 
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
	"data/TEXTURE/number.png",
};

static BOOL		g_Load = FALSE;		// 初期化を行ったかのフラグ
static STATUS	g_Status[STATUS_MAX_INDEX];

static PLAYERUI *playerUI = GetPlayerUI();

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitStatus(void)
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

	for (int i = 0; i < STATUS_MAX_INDEX; i++)
	{
		// 変数の初期化
		g_Status[i].w = TEXTURE_WIDTH;
		g_Status[i].h = TEXTURE_HEIGHT;
		g_Status[i].texNo = 0;
		g_Status[i].digit = 3;
		g_Status[i].status = 0;	// 得点を初期化

		g_Status[i].alpha = 1.0f;
		g_Status[i].use = FALSE;
	}

	g_Status[STATUS_HP].pos = D3DXVECTOR3(playerUI->pos.x + 100.0f, playerUI->pos.y + 13.0f, 0.0f);
	g_Status[STATUS_MP].pos = D3DXVECTOR3(playerUI->pos.x + 100.0f, playerUI->pos.y + 63.0f, 0.0f);

	g_Load = TRUE;		// データの初期化を行った
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitStatus(void)
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
void UpdateStatus(void)
{
	BATTLE *player = GetBattlePlayer();

	g_Status[STATUS_HP].status = player->hp;
	g_Status[STATUS_MP].status = player->mp;

	g_Status[STATUS_HP].pos = D3DXVECTOR3(playerUI->pos.x + 100.0f, playerUI->pos.y + 13.0f, 0.0f);
	g_Status[STATUS_MP].pos = D3DXVECTOR3(playerUI->pos.x + 100.0f, playerUI->pos.y + 63.0f, 0.0f);

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawStatus(void)
{
	for (int i = 0; i < STATUS_MAX_INDEX; i++)
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
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Status[i].texNo]);

			// 桁数分処理する

			for (int j = 0; j < g_Status[i].digit; j++)
			{
				// 今回表示する桁の数字
				float x = (float)(g_Status[i].status % 10);

				// 次の桁へ
				g_Status[i].status /= 10;

				// スコアの位置やテクスチャー座標を反映
				float px = g_Status[i].pos.x - g_Status[i].w * j;	// スコアの表示位置X
				float py = g_Status[i].pos.y;				// スコアの表示位置Y
				float pw = g_Status[i].w;					// スコアの表示幅
				float ph = g_Status[i].h;					// スコアの表示高さ

				float tw = 1.0f / 10;					// テクスチャの幅
				float th = 1.0f / 1;					// テクスチャの高さ
				float tx = x * tw;						// テクスチャの左上X座標
				float ty = 0.0f;						// テクスチャの左上Y座標

				// １枚のポリゴンの頂点とテクスチャ座標を設定
				SetSpriteLTColor(g_VertexBuffer,
					px, py, pw, ph,
					tx, ty, tw, th,
					D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Status[i].alpha));

				// ポリゴン描画
				GetDeviceContext()->Draw(4, 0);
			}
		}
	}
}


//=============================================================================
// スコアを加算する
// 引数:add :追加する点数。マイナスも可能
//=============================================================================
STATUS *GetStatusAddress(void)
{
	return &g_Status[0];
}

