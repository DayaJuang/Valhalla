//=============================================================================
//
// Main処理 [main.cpp]
// Author : GP11A132 21 DAYA MAHAPUTRA
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "camera.h"
#include "input.h"
#include "title.h"
#include "bg.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "score.h"
#include "result.h"
#include "sound.h"
#include "fade.h"
#include "file.h"
#include "layer.h"
#include "worldmap.h"
#include "iconWM.h"
#include "cursorWM.h"
#include "battleUI.h"
#include "battleCursor.h"
#include "damage.h"
#include "status.h"
#include "playerUI.h"
#include "battle.h"
#include "effect.h"
#include "bubble.h"
#include "limitBar.h"
#include "ultimateText.h"
#include "battleLog.h"
#include "actionBar.h"
#include "actionBarFrame.h"
#include "tutorial.h"
#include "orb.h"
#include "gameClear.h"
#include "waterfall.h"
#include "titleText.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CLASS_NAME			"AppClass"				// ウインドウのクラス名
#define WINDOW_NAME			"Valhalla"		// ウインドウのキャプション名

//*****************************************************************************
// 構造体定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);

void CheckGameClear(void);


//*****************************************************************************
// グローバル変数:
//*****************************************************************************
long g_MouseX = 0;
long g_MouseY = 0;


#ifdef _DEBUG
int		g_CountFPS;							// FPSカウンタ
char	g_DebugStr[2048] = WINDOW_NAME;		// デバッグ文字表示用

#endif

int g_Mode = MODE_TITLE;					// 起動時の画面を設定

BOOL g_LoadGame = FALSE;					// NewGame

//=============================================================================
// メイン関数
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//UNREFERENCED_PARAMETER(hPrevInstance);	// 無くても良いけど、警告が出る（未使用宣言）
	//UNREFERENCED_PARAMETER(lpCmdLine);		// 無くても良いけど、警告が出る（未使用宣言）

	// 時間計測用
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;
	
	WNDCLASSEX	wcex = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW+1),
		NULL,
		CLASS_NAME,
		NULL
	};
	HWND		hWnd;
	MSG			msg;
	
	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// ウィンドウの作成
	hWnd = CreateWindow(CLASS_NAME,
						WINDOW_NAME,
						WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT,																		// ウィンドウの左座標
						CW_USEDEFAULT,																		// ウィンドウの上座標
						SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME)*2,									// ウィンドウ横幅
						SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME)*2+GetSystemMetrics(SM_CYCAPTION),	// ウィンドウ縦幅
						NULL,
						NULL,
						hInstance,
						NULL);

	// ウィンドウモードかフルスクリーンモードかの処理
	bool mode = true;

	int id = MessageBox(NULL, "Windowモードでプレイしますか？", "起動モード", MB_YESNOCANCEL | MB_ICONQUESTION);
	switch (id)
	{
	case IDYES:		// YesならWindowモードで起動
		mode = true;
		break;
	case IDNO:		// Noならフルスクリーンモードで起動
		mode = false;
		break;
	case IDCANCEL:	// CANCELなら終了
	default:
		return -1;
		break;
	}

	// DirectXの初期化(ウィンドウを作成してから行う)
	if(FAILED(Init(hInstance, hWnd, mode)))
	{
		return -1;
	}

	// フレームカウント初期化
	timeBeginPeriod(1);	// 分解能を設定
	dwExecLastTime = dwFPSLastTime = timeGetTime();	// システム時刻をミリ秒単位で取得
	dwCurrentTime = dwFrameCount = 0;
	
	// ウインドウの表示(Init()の後に呼ばないと駄目)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	
	// メッセージループ
	while(1)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳とディスパッチ
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();					// システム時刻を取得

			if ((dwCurrentTime - dwFPSLastTime) >= 1000)	// 1秒ごとに実行
			{
#ifdef _DEBUG
				g_CountFPS = dwFrameCount;
#endif
				dwFPSLastTime = dwCurrentTime;				// FPSを測定した時刻を保存
				dwFrameCount = 0;							// カウントをクリア
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))	// 1/60秒ごとに実行
			{
				dwExecLastTime = dwCurrentTime;	// 処理した時刻を保存

#ifdef _DEBUG	// デバッグ版の時だけFPSを表示する
				wsprintf(g_DebugStr, WINDOW_NAME);
				wsprintf(&g_DebugStr[strlen(g_DebugStr)], " FPS:%d", g_CountFPS);
#endif

				Update();			// 更新処理
				Draw();				// 描画処理

#ifdef _DEBUG	// デバッグ版の時だけ表示する
				wsprintf(&g_DebugStr[strlen(g_DebugStr)], " MX:%d MY:%d", GetMousePosX(), GetMousePosY());
				SetWindowText(hWnd, g_DebugStr);
#endif

				dwFrameCount++;		// 処理回数のカウントを加算
			}
		}
	}
	
	timeEndPeriod(1);				// 分解能を戻す

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// 終了処理
	Uninit();

	return (int)msg.wParam;
}

//=============================================================================
// プロシージャ
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch( message )
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:					// [ESC]キーが押された
			DestroyWindow(hWnd);		// ウィンドウを破棄するよう指示する
			break;
		}
		break;

	case WM_MOUSEMOVE:
		g_MouseX = LOWORD(lParam);
		g_MouseY = HIWORD(lParam);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// レンダリング処理の初期化
	InitRenderer(hInstance, hWnd, bWindow);

	// カメラ処理の初期化
	InitCamera();

	// 入力処理の初期化
	InitInput(hInstance, hWnd);

	// サウンド処理の初期化
	InitSound(hWnd);

	// フェード処理の初期化
	InitFade();

	// 最初のモードをセットして、この中でInitする
	SetMode(g_Mode);	// ここはSetModeのままで！

	return S_OK;
}


//=============================================================================
// 終了処理
//=============================================================================
void Uninit(void)
{
	// ゲーム的な物はこの中でUninitする
	SetMode(MODE_END);	// ここはSetModeのままで！

	// フェードの終了処理
	UninitFade();

	// サウンドの終了処理
	UninitSound();

	// 入力の終了処理
	UninitInput();

	// カメラの終了処理
	UninitCamera();

	// レンダリングの終了処理
	UninitRenderer();

}

//=============================================================================
// 更新処理
//=============================================================================
void Update(void)
{
	// 入力の更新処理
	UpdateInput();

	// カメラの更新処理
	UpdateCamera();

	// モードによって処理を分ける
	switch (g_Mode)
	{
	case MODE_TITLE:		// タイトル画面の更新
		UpdateTitle();
		UpdateWaterfall();
		UpdateTitleText();
		break;

	case MODE_WORLDMAP:		// ワードマップ画面の更新
		UpdateWM();
		UpdateIconWM();
		UpdateCursorWM();
		UpdateTutorial();
		break;

	case MODE_GAME:			// ゲーム画面の更新
		UpdateBG();
		UpdateBubble();
		UpdatePlayer();
		UpdateEnemy();
		UpdateOrb();
		UpdateLayer();
		UpdateBullet();
		UpdateScore();
		UpdateDamage();
		UpdateBattleUI();
		UpdateBattleLog();
		UpdateUltimateText();
		UpdateBattleCursor();
		UpdatePlayerUI();
		UpdateStatus();
		UpdateBar();
		UpdateEffect();
		UpdateBattle();
		UpdateActionBar();
		UpdateActionBarFrame();
		UpdateTutorial();
		break;

	case MODE_GAME_OVER:	// ゲームオーバー画面の更新
		UpdateResult();
		break;

	case MODE_GAME_CLEAR:	// ゲームクリア画面の更新
		UpdateGameClear();
		break;

	}

	UpdateFade();			// フェードの更新処理
}

//=============================================================================
// 描画処理
//=============================================================================
void Draw(void)
{
	// バックバッファクリア
	Clear();

	// カメラをセット
	SetCamera();

	// 2D描画なので深度無効
	SetDepthEnable(false);
	
	// モードによって処理を分ける
	switch (g_Mode)
	{
	case MODE_TITLE:		// タイトル画面の描画
		DrawTitle();
		DrawWaterfall();
		DrawTitleText();
		break;

	case MODE_WORLDMAP:
		// ワードマップ画面の描画
		DrawWM();
		DrawIconWM();
		DrawCursorWM();
		DrawTutorial();
		break;

	case MODE_GAME:			// ゲーム画面の描画
		DrawBG();
		DrawBullet();
		DrawEnemy();
		DrawOrb();
		DrawPlayer();
		DrawBubble();
		DrawLayer();
		DrawDamage();
		DrawBattleUI();
		DrawBattleLog();
		DrawUltimateText();
		DrawBattleCursor();
		DrawPlayerUI();
		DrawStatus();
		DrawBar();
		DrawEffect();
		DrawBattle();
		DrawActionBarFrame();
		DrawActionBar();
		DrawTutorial();
		break;

	case MODE_GAME_OVER:		// ゲームオーバー画面の描画
		DrawResult();
		break;

	case MODE_GAME_CLEAR:		// ゲームクリア画面の描画
		DrawGameClear();
		break;

	}

	DrawFade();				// フェード画面の描画


#ifdef _DEBUG
	// デバッグ表示
	DrawDebugProc();
#endif

	// バックバッファ、フロントバッファ入れ替え
	Present();

}


//=============================================================================
// モードの設定
//=============================================================================
void SetMode(int mode)
{
	// モードを変える前に全部メモリを解放しちゃう
	StopSound();			// まず曲を止める

	// タイトルに関する終了処理
	UninitTitle();
	UninitWaterfall();
	UninitTitleText();

	// BGの終了処理
	UninitBG();

	// ワードマップ画面の終了処理
	UninitWM();

	// アイコンの終了処理
	UninitIconWM();

	// カーソルの終了処理
	UninitCursorWM();

	// テキストの終了処理
	UninitUltimateText();

	// バトルUIの終了処理
	UninitBattleUI();

	// バトルログの終了処理
	UninitBattleLog();

	// ダメージ数字の終了処理
	UninitDamage();

	// バーブルの終了処理
	UninitBubble();

	// バトル用カーソルの終了処理
	UninitBattleCursor();

	// プレイヤーステータスUIの終了処理
	UninitPlayerUI();

	// HPとMP表示数字の終了処理
	UninitStatus();

	// エフェクトの終了処理
	UninitEffect();

	// プレイヤーの終了処理
	UninitPlayer();

	// エネミーの終了処理
	UninitEnemy();

	// オーブの終了処理
	UninitOrb();

	// バレットの終了処理
	UninitBullet();

	// スコアの終了処理
	UninitScore();

	// バーの終了処理
	UninitBar();

	// ゲームオーバーの終了処理
	UninitResult();

	// バトルの終了処理
	UninitBattle();

	// アクションバーに関する終了処理
	UninitActionBar();
	UninitActionBarFrame();

	// チュートリアルの終了処理
	UninitTutorial();

	// ゲームクリアの終了処理
	UninitGameClear();

	g_Mode = mode;	// 次のモードをセットしている

	switch (g_Mode)
	{
	case MODE_TITLE:
		// タイトル画面の初期化
		InitTitle();
		InitWaterfall();
		InitTitleText();
		PlaySound(SOUND_LABEL_BGM_title);
		break;

	case MODE_WORLDMAP:
		// ワードマップ画面の初期化
		InitWM();
		InitIconWM();
		InitCursorWM();
		InitTutorial();
		PlaySound(SOUND_LABEL_BGM_worldmap);
		break;

	case MODE_GAME:
		// ゲーム画面の初期化
		InitBG();
		InitPlayer();
		InitBubble();
		InitEnemy();
		InitOrb();
		InitLayer();
		InitBullet();
		InitScore();
		InitDamage();
		InitBattleUI();
		InitBattleLog();
		InitUltimateText();
		InitBattleCursor();
		InitPlayerUI();
		InitStatus();
		InitBar();
		InitEffect();
		InitBattle();
		InitActionBar();
		InitActionBarFrame();
		InitTutorial();

		// ロードゲームだったらすべての初期化が終わった後にセーブデータを読み込む
		if (g_LoadGame == TRUE)
		{
			LoadData();
			g_LoadGame = FALSE;		// ロードしたからフラグをClearする
		}

		//PlaySound(SOUND_LABEL_BGM_sample001);
		break;

	case MODE_GAME_OVER:
		// ゲームオーバー画面の初期化
		InitResult();
		PlaySound(SOUND_LABEL_SE_gameover);
		break;

	case MODE_GAME_CLEAR:
		// ゲームクリア画面の初期化
		InitGameClear();
		PlaySound(SOUND_LABEL_BGM_gameclear);
		break;

	case MODE_END:
		// ゲーム終了時の処理
		StopSound();
		break;
	}
}

//=============================================================================
// モードの取得
//=============================================================================
int GetMode(void)
{
	return g_Mode;
}


//=============================================================================
// マウス座標の取得
//=============================================================================
long GetMousePosX(void)
{
	return g_MouseX;
}


long GetMousePosY(void)
{
	return g_MouseY;
}


#ifdef _DEBUG
char *GetDebugStr(void)
{
	return g_DebugStr;
}
#endif

int checkCurrentMap(void)
{
	PLAYER *player = GetPlayer();

	return player->currentLocation;
}


//=============================================================================
// ニューゲームかロードゲームかをセットする
//=============================================================================
void SetLoadGame(BOOL flg)
{
	g_LoadGame = flg;
}





