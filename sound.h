//=============================================================================
//
// サウンド処理 [sound.h]
//
//=============================================================================
#pragma once

#include <windows.h>
#include "xaudio2.h"						// サウンド処理で必要

//*****************************************************************************
// サウンドファイル
//*****************************************************************************
enum 
{
	SOUND_LABEL_BGM_title,	
	SOUND_LABEL_BGM_forest,	
	SOUND_LABEL_BGM_dungeon,
	SOUND_LABEL_BGM_worldmap,
	SOUND_LABEL_BGM_battle,	
	SOUND_LABEL_BGM_gameclear,
	SOUND_LABEL_SE_battleStart,
	SOUND_LABEL_SE_swing,	
	SOUND_LABEL_SE_hit,		
	SOUND_LABEL_SE_enemyHit,
	SOUND_LABEL_SE_death,	
	SOUND_LABEL_SE_cure,	
	SOUND_LABEL_SE_fireball,
	SOUND_LABEL_SE_explode,	
	SOUND_LABEL_SE_laser,	
	SOUND_LABEL_SE_victory,	
	SOUND_LABEL_SE_click,	
	SOUND_LABEL_SE_gameover,
	SOUND_LABEL_SE_footstep,

	SOUND_LABEL_MAX,
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
bool InitSound(HWND hWnd);
void UninitSound(void);
void PlaySound(int label);
void StopSound(int label);
void StopSound(void);

