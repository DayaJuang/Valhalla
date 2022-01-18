//=============================================================================
//
// エネミー処理 [Orb.cpp]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#include "orb.h"
#include "sprite.h"
#include "bg.h"
#include "player.h"
#include "collision.h"
#include "fade.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(40)	// キャラサイズ
#define TEXTURE_HEIGHT				(40)	// 
#define TEXTURE_MAX					(1)		// テクスチャの数

#define TEXTURE_PATTERN_DIVIDE_X	(3)		// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y	(1)		// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define ANIM_WAIT					(30)		// アニメーションの切り替わるWait値


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/orb.png"
};

static BOOL		g_Load = FALSE;			// 初期化を行ったかのフラグ
static ORB	g_Orb[ORB_MAX];		// エネミー構造体


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitOrb(void)
{
	ID3D11Device *pDevice = GetDevice();
	srand((unsigned)time(NULL));

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

	PLAYER *player = GetPlayer();

	// エネミー構造体の初期化
	for (int i = 0; i < ORB_MAX; i++)
	{
		g_Orb[i].use = TRUE;
		g_Orb[i].pos = FindOrbPosition();	// 中心点から表示
		g_Orb[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Orb[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Orb[i].w   = TEXTURE_WIDTH;
		g_Orb[i].h   = TEXTURE_HEIGHT;
	}

	g_Load = TRUE;	// データの初期化を行った
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitOrb(void)
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
void UpdateOrb(void)
{
	for (int i = 0; i < ORB_MAX; i++)
	{
		if (g_Orb->pos == D3DXVECTOR3(0.0f, 0.0f, 0.0f))
			g_Orb->use = FALSE;
 
		{
			// アニメーション  
			g_Orb[i].countAnim++;
			if ((g_Orb[i].countAnim % ANIM_WAIT) == 0)
			{
				// パターンの切り替え
				g_Orb[i].patternAnim = (g_Orb[i].countAnim * TEXTURE_PATTERN_DIVIDE_X) + ((g_Orb[i].patternAnim + 1) % TEXTURE_PATTERN_DIVIDE_X);
			}
		}

		// 当たり判定処理
		{
			PLAYER *player = GetPlayer();

			// エネミーの数分当たり判定を行う
			for (int j = 0; j < PLAYER_MAX; j++)
			{
				// 生きてるエネミーと当たり判定をする
				if (player[j].use == TRUE)
				{
					BOOL ans = CollisionBB(g_Orb[i].pos, g_Orb[i].w, g_Orb[i].h,
						player[j].pos, player[j].w, player[j].h);
					// 当たっている？
					if (ans == TRUE)
					{
						// ↓この処理を入れないと貫通弾になってしまう
						g_Orb[i].use = FALSE;

						// 当たった時の処理
						SetFade(FADE_OUT, MODE_GAME_CLEAR);

						break;
					}
				}
			}
		}


#ifdef _DEBUG
		// デバッグ表示
		PrintDebugProc("Orb No%d  X:%f Y:%f\n", i, g_Orb[i].pos.x, g_Orb[i].pos.y);
#endif

	}
}


//=============================================================================
// 描画処理
//=============================================================================
void DrawOrb(void)
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

	for (int i = 0; i < ORB_MAX; i++)
	{
		if (g_Orb[i].use == TRUE)		// このエネミーが使われている？
		{								// Yes
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Orb[i].texNo]);

			//エネミーの位置やテクスチャー座標を反映
			float px = g_Orb[i].pos.x - bg->pos.x;	// エネミーの表示位置X
			float py = g_Orb[i].pos.y - bg->pos.y;	// エネミーの表示位置Y
			float pw = g_Orb[i].w;		// エネミーの表示幅
			float ph = g_Orb[i].h;		// エネミーの表示高さ

			// アニメーション用
			float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X;	// テクスチャの幅
			float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// テクスチャの高さ
			float tx = (float)(g_Orb[i].patternAnim % TEXTURE_PATTERN_DIVIDE_X) * tw;	// テクスチャの左上X座標
			float ty = (float)(g_Orb[i].patternAnim / TEXTURE_PATTERN_DIVIDE_X) * th;	// テクスチャの左上Y座標

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
				g_Orb[i].rot.z);

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}
	}
}

D3DXVECTOR3 FindOrbPosition(void)
{
	int mapSize = 2400;
	int mapChipSize = 32;
	float x, y;

	for (int i = 0; i < mapSize / mapChipSize; i++)
	{
		for (int j = 0; j < mapSize / mapChipSize; j++)
		{

			if (GetMapIndex(j, i) == ORB_POS)
			{
				x = (float)i * mapChipSize;
				y = (float)j * mapChipSize;

				return D3DXVECTOR3(x, y, 0.0f);
			}
		}
	}

	return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}


//=============================================================================
// エネミー構造体の先頭アドレスを取得
//=============================================================================
ORB *GetOrb(void)
{

	return &g_Orb[0];
}




