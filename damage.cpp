//=============================================================================
//
// ダメージ処理 [damage.cpp]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#include "damage.h"
#include "sprite.h"
#include "battle.h"
#include "bg.h"
#include "layer.h"
#include "bubble.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(23)	// 数字のサイズ
#define TEXTURE_HEIGHT				(23)
#define TEXTURE_MAX					(2)		// テクスチャの数


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
	"data/TEXTURE/missText.png",
};

static BOOL		g_Load = FALSE;		// 初期化を行ったかのフラグ
static DAMAGE	g_Damage;

static float floatTime = 0;


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitDamage(void)
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
	g_Damage.w = TEXTURE_WIDTH;
	g_Damage.h = TEXTURE_HEIGHT;
	g_Damage.pos = D3DXVECTOR3(DAMAGE_DX, DAMAGE_DY, 0.0f);
	g_Damage.texNo = 0;
	g_Damage.digit = 0;
	g_Damage.damage = 0;	// 得点を初期化

	g_Damage.alpha = 1.0f;
	g_Damage.use = FALSE;

	g_Load = TRUE;		// データの初期化を行った
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitDamage(void)
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
void UpdateDamage(void)
{
	BG *bg = GetBG();
	BUBBLE *bubble = GetBubble();
	LAYER *layer = GetLayer();
	BATTLE *player = GetBattlePlayer();
	BATTLE *enemy = GetBattleEnemy();

	// ダメージの表示
	if (g_Damage.use == TRUE)
	{
		floatTime++;

		if (floatTime >= 60)
		{
			g_Damage.use = FALSE;
			bubble->use = FALSE;
			floatTime = 0;
			player->stop = enemy->stop = FALSE;

			bg->r = bg->g = bg->b = layer->r = layer->g = layer->b = 1.0f;

			CheckBattleResult();
		}
	}
#ifdef _DEBUG
	// デバッグ表示
	PrintDebugProc("Damage :%d TexNo:%d %d\n", g_Damage.damage, g_Damage.texNo, g_Damage.use);

#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawDamage(void)
{
	if (g_Damage.use == TRUE)
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
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Damage.texNo]);

		// 桁数分処理する
		int number = g_Damage.damage;
		int digit = g_Damage.digit;

		if (g_Damage.texNo == 1)
			digit = 1;

		for (int i = 0; i < digit; i++)
		{
			// 今回表示する桁の数字
			float x = (float)(number % 10);

			// 次の桁へ
			number /= 10;

			// ダメージの位置やテクスチャー座標を反映
			float px = g_Damage.pos.x - (g_Damage.w/1.5f)*i;	// ダメージの表示位置X
			float py = g_Damage.pos.y;							// ダメージの表示位置Y
			float pw = g_Damage.w;								// ダメージの表示幅
			float ph = g_Damage.h;								// ダメージの表示高さ

			float tw = 1.0f / 10;					// テクスチャの幅
			float th = 1.0f / 1;					// テクスチャの高さ
			float tx = x * tw;						// テクスチャの左上X座標
			float ty = 0.0f;						// テクスチャの左上Y座標

			if (g_Damage.texNo == 1)
			{
				px = g_Damage.pos.x;	// ダメージの表示位置X
				py = g_Damage.pos.y;	// ダメージの表示位置Y
				pw = g_Damage.w * 1.5f;		// ダメージの表示幅
				ph = g_Damage.h * 1.5f;		// ダメージの表示高さ

				tw = 1.0f;					// テクスチャの幅
				th = 1.0f;					// テクスチャの高さ
				tx = 0.0f;					// テクスチャの左上X座標
				ty = 0.0f;					// テクスチャの左上Y座標
			}

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteLTColor(g_VertexBuffer,
				px, py, pw, ph,
				tx, ty, tw, th,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Damage.alpha));

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}
	}

}

int GetDamage(void)
{
	return g_Damage.damage;
}

void SetDamageDigit(int damage)
{
	int digit = 0;

	while (damage != 0)
	{
		damage /= 10;

		++digit;
	}

	g_Damage.digit = digit;

}

void ShowDamage(void)
{
	g_Damage.use = TRUE;
}

void SetDamagePosition(D3DXVECTOR3 targetPos)
{
	g_Damage.pos = targetPos;
}

void SetDamage(int damage)
{
	g_Damage.damage = damage;

	if (damage == 0)
	{
		g_Damage.texNo = 1;
	}
	else
	{
		g_Damage.texNo = 0;
	}
}

DAMAGE *GetDamageAddress(void)
{
	return &g_Damage;
}

