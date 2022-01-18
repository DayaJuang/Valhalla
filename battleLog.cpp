//=============================================================================
//
// バトルログ処理 [battleLog.cpp]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#include "battleLog.h"
#include "sprite.h"
#include "limitBar.h"
#include "battleUI.h"
#include "input.h"
#include "battle.h"
#include "battleCursor.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(200)		// 数字のサイズ
#define TEXTURE_HEIGHT				(45) 
#define TEXTURE_MAX					(LOG_MAX)	// テクスチャの数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[] = {
	"data/BATTLE_UI/escape_Success.png",
	"data/BATTLE_UI/escape_Failed.png",
	"data/BATTLE_UI/enemyKilled.png",
	"data/BATTLE_UI/maxHP_up.png",
	"data/BATTLE_UI/maxMP_up.png",
	"data/BATTLE_UI/attackUp.png",
	"data/BATTLE_UI/recoverHPMP.png",
};

static BOOL		g_Load = FALSE;		// 初期化を行ったかのフラグ
BATTLELOG			g_BattleLog;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBattleLog(void)
{
	ID3D11Device *pDevice = GetDevice();
	BATTLE *player = GetBattlePlayer();

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
	g_BattleLog.w = TEXTURE_WIDTH;
	g_BattleLog.h = TEXTURE_HEIGHT;
	g_BattleLog.texNo = 0;

	g_BattleLog.use = FALSE;

	g_BattleLog.pos = D3DXVECTOR3((SCREEN_WIDTH / 2) - (g_BattleLog.w / 2), SCREEN_HEIGHT/2, 0.0f);

	g_Load = TRUE;		// データの初期化を行った
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBattleLog(void)
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
void UpdateBattleLog(void)
{
	BATTLECURSOR *cursor = GetBattleCursor();

	if (g_BattleLog.use == TRUE)
	{
		if (GetKeyboardTrigger(DIK_RETURN))
		{
			g_BattleLog.use = FALSE;

			PlaySound(SOUND_LABEL_SE_click);

			switch (g_BattleLog.texNo)
			{
			case ESSCAPE_SUCCESS:
				Escape(ESSCAPE_SUCCESS);
				break;

			case ESCAPE_FAIL:
				Escape(ESCAPE_FAIL);
				break;
			case ENEMY_KILLED:
				ShowBattleUI(POWERUP_UI);
				cursor->canDoAction;
				break;
			default:
				BattleEnd();
				break;
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBattleLog(void)
{
	BATTLEUI *battleUI = GetBattleUI();

	if (g_BattleLog.use == TRUE)
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
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_BattleLog.texNo]);

		// バトルログの位置やテクスチャー座標を反映
		float px = g_BattleLog.pos.x;				// バトルログの表示位置X
		float py = g_BattleLog.pos.y;				// バトルログの表示位置Y
		float pw = g_BattleLog.w;					// バトルログの表示幅
		float ph = g_BattleLog.h;					// バトルログの表示高さ

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteLTColor(g_VertexBuffer,
			px, py, pw, ph,
			0.0f, 0.0f, 1.0f, 1.0f,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);

	}
}

void ShowBattleLog(int texNo)
{
	g_BattleLog.texNo = texNo;
	g_BattleLog.use = TRUE;
}

BATTLELOG *GetBattleLog(void)
{
	return &g_BattleLog;
}

