//=============================================================================
//
// WM画面処理 [cursorWM.cpp]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#include "cursorWM.h"
#include "iconWM.h"
#include "main.h"
#include "sprite.h"
#include "player.h"
#include "input.h"
#include "fade.h"
#include "layer.h"
#include "sound.h"
#include "tutorial.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_MAX					(1)						// テクスチャの数



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/cursor.png"
};

static BOOL	g_Load = FALSE;		// 初期化を行ったかのフラグ
static CURSORWM g_Cursor;
static int currentLocation;
static BOOL firstTime = TRUE;


// 初期化処理
//=============================================================================
HRESULT InitCursorWM(void)
{
	ID3D11Device *pDevice = GetDevice();
	currentLocation = checkCurrentMap();

	ICONWM *icon = GetIconWM(currentLocation);


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

	g_Cursor.w = 50;
	g_Cursor.h = 50;

	g_Cursor.texNo = 0;

	g_Cursor.pos = D3DXVECTOR3(icon->pos.x - 35.0f, icon->pos.y + 10.0f, 0.0f);

	g_Load = TRUE;	// データの初期化を行った
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitCursorWM(void)
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
void UpdateCursorWM(void)
{
	TUTORIAL *tutorial = GetTutorial();

	if (firstTime)
	{
		firstTime = FALSE;
		ShowTutorial(WORLDMAP_TUTORIAL1, WORLDMAP_TUTORIAL);
	}
	else if (!tutorial->use)
	{
		int old_currentLocation = currentLocation;

		if (GetKeyboardTrigger(DIK_RIGHT) || GetKeyboardTrigger(DIK_LEFT))
		{
			PlaySound(SOUND_LABEL_SE_click);

			currentLocation = (currentLocation + 1) % 2;
			ICONWM *icon = GetIconWM(currentLocation);

			if (icon->use == FALSE)
			{
				currentLocation = old_currentLocation;
				return;
			}

			g_Cursor.pos = g_Cursor.pos = D3DXVECTOR3(icon->pos.x - 35.0f, icon->pos.y, 0.0f);
		}

		if (GetKeyboardTrigger(DIK_RETURN))
		{
			PlaySound(SOUND_LABEL_SE_click);

			PLAYER *player = GetPlayer();
			player->currentLocation = currentLocation;
			SetFade(FADE_OUT, MODE_GAME);
		}
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawCursorWM(void)
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

		// WMの背景を描画
		{
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Cursor.texNo]);

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteLTColor(g_VertexBuffer,
				g_Cursor.pos.x, g_Cursor.pos.y, g_Cursor.w, g_Cursor.h,
				0.0f, 0.0f, 1.0f, 1.0f,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}

}




