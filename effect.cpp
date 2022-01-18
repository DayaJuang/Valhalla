//=============================================================================
//
// エフェクト処理 [effect.cpp]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#include "effect.h"
#include "sprite.h"
#include "enemy.h"
#include "collision.h"
#include "score.h"
#include "sound.h"
#include "bg.h"
#include "damage.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(65)	// エフェクトサイズ
#define TEXTURE_HEIGHT				(65)	// 
#define ULTIMATE_TEXTURE_WIDTH		(SCREEN_WIDTH)	// Ultimateのエフェクトサイズ
#define ULTIMATE_TEXTURE_HEIGHT		(SCREEN_HEIGHT)	// 
#define TEXTURE_MAX					(EFFECT_MAX)		// テクスチャの数

#define TEXTURE_PATTERN_DIVIDE_X	(6)		// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y	(1)		// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define ANIM_WAIT					(5)		// アニメーションの切り替わるWait値

#define ULTIMATE_TEXTURE_PATTERN_DIVIDE_X	(1)			// アニメパターンのテクスチャ内分割数（X)
#define ULTIMATE_TEXTURE_PATTERN_DIVIDE_Y	(12)		// アニメパターンのテクスチャ内分割数（Y)
#define ULTIMATE_ANIM_PATTERN_NUM			(ULTIMATE_TEXTURE_PATTERN_DIVIDE_X*ULTIMATE_TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define ULTIMATE_ANIM_WAIT					(12)		// アニメーションの切り替わるWait値


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;				// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[] = {
	"data/EFFECT/slash_effect.png",
	"data/EFFECT/enemy_attack.png",
	"data/EFFECT/fire_effect.png",
	"data/EFFECT/heal_effect.png",
	"data/EFFECT/ultimate.png",
};

static BOOL	  g_Load = FALSE;		// 初期化を行ったかのフラグ
static EFFECT g_Effect;				// エフェクト構造体


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEffect(void)
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

	g_Effect.use = FALSE;			// 未使用（発射されていない弾）
	g_Effect.w = TEXTURE_WIDTH;
	g_Effect.h = TEXTURE_HEIGHT;
	g_Effect.pos = D3DXVECTOR3(300, 300.0f, 0.0f);
	g_Effect.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Effect.texNo = 0;

	g_Effect.countAnim = 0;
	g_Effect.patternAnim = 0;

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEffect(void)
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
void UpdateEffect(void)
{
	BG *bg = GetBG();

	if (g_Effect.use == TRUE)	// このエフェクトが使われている？
	{								// Yes
		// アニメーション  
		g_Effect.countAnim++;
		if (g_Effect.texNo == LIMIT_BREAK_EFFECT)
		{
			if ((g_Effect.countAnim % ULTIMATE_ANIM_WAIT) == 0)
			{
				PlaySound(SOUND_LABEL_SE_laser);

				// パターンの切り替え
				g_Effect.patternAnim = (g_Effect.patternAnim + 1) % ULTIMATE_ANIM_PATTERN_NUM;

				if (g_Effect.patternAnim >= ULTIMATE_TEXTURE_PATTERN_DIVIDE_Y - 1)
				{
					g_Effect.use = FALSE;
					ShowDamage();
				}
			}
		}
		else
		{
			if ((g_Effect.countAnim % ANIM_WAIT) == 0)
			{
				// パターンの切り替え
				g_Effect.patternAnim = (g_Effect.patternAnim + 1) % ANIM_PATTERN_NUM;

				if (g_Effect.patternAnim >= TEXTURE_PATTERN_DIVIDE_X - 1)
				{
					g_Effect.use = FALSE;
					ShowDamage();
				}
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEffect(void)
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

	BG *bg = GetBG();

	if (g_Effect.use == TRUE)		// このエフェクトが使われている？
	{									// Yes
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Effect.texNo]);

		//エフェクトの位置やテクスチャー座標を反映
		float px = g_Effect.pos.x;	// エフェクトの表示位置X
		float py = g_Effect.pos.y;	// エフェクトの表示位置Y
		float pw = g_Effect.w;		// エフェクトの表示幅
		float ph = g_Effect.h;		// エフェクトの表示高さ

		float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X;	// テクスチャの幅
		float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// テクスチャの高さ
		float tx = (float)(g_Effect.patternAnim % TEXTURE_PATTERN_DIVIDE_X) * tw;	// テクスチャの左上X座標
		float ty = (float)(g_Effect.patternAnim / TEXTURE_PATTERN_DIVIDE_X) * th;	// テクスチャの左上Y座標

		if (g_Effect.texNo == LIMIT_BREAK_EFFECT)
		{
			pw = ULTIMATE_TEXTURE_WIDTH;		// エフェクトの表示幅
			ph = ULTIMATE_TEXTURE_HEIGHT;		// エフェクトの表示高さ

			tw = 1.0f / ULTIMATE_TEXTURE_PATTERN_DIVIDE_X;	// テクスチャの幅
			th = 1.0f / ULTIMATE_TEXTURE_PATTERN_DIVIDE_Y;	// テクスチャの高さ
			tx = (float)(g_Effect.patternAnim % ULTIMATE_TEXTURE_PATTERN_DIVIDE_X) * tw;	// テクスチャの左上X座標
			ty = (float)(g_Effect.patternAnim / ULTIMATE_TEXTURE_PATTERN_DIVIDE_X) * th;	// テクスチャの左上Y座標
		}

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColorRotation(g_VertexBuffer,
			px, py, pw, ph,
			tx, ty, tw, th,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			g_Effect.rot.z);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}


}


//=============================================================================
// エフェクト構造体の先頭アドレスを取得
//=============================================================================
EFFECT *GetEffect(void)
{
	return &g_Effect;
}


//=============================================================================
// エフェクトの設定
//=============================================================================
void SetEffect(D3DXVECTOR3 pos, int texNo)
{
	DAMAGE *damage = GetDamageAddress();

	if (damage->damage > 0)
	{
		switch (texNo)
		{
		case ATTACK_EFFECT:
			PlaySound(SOUND_LABEL_SE_hit);
			break;
		case ENEMY_ATTACK_EFFECT:
			PlaySound(SOUND_LABEL_SE_enemyHit);
			break;
		case FIRE_EFFECT:
			PlaySound(SOUND_LABEL_SE_explode);
			break;
		case HEAL_EFFECT:
			PlaySound(SOUND_LABEL_SE_cure);
			break;
		default:
			break;
		}
	}

	g_Effect.pos = pos;
	g_Effect.texNo = texNo;
	g_Effect.use = TRUE;
}

