
#pragma once
#include "lib_CSPRITE_DIB.h"
#include "lib_CSCREEN_DIB.h"

#include "df_sprite_setting.h"
#include "CPlayerObject.h"
#include "CBaseScean.h"
#include "CFrameSkip.h"
#include "CSpriteNode.h"

//------------------------------------------------------------------------------
//	클래스 계층도
//
//			     CSean
//		       /         
//		  CStage1  
//
//------------------------------------------------------------------------------

/**
@class CBaseScean

@brief 모든 씬의 베이스가 되는 클래스.

@todo : 추후 h, cpp 분리 할 것!
*/
class CBaseScean {

private:

	//스프라이트 관련.
	list<CSpriteNode*>_list_csprite;	///<스프라이트 리스트
	int num_total_player;				///<최대 이용 가능 플레이어 갯수

	//프레임 스킵.
	CFrameSkip *_frame_skip;		   

public:

	//플레이 리스트.
	CPlayerObject * _arr_player;


	//생성자1
	CBaseScean();

	//생성자2
	CBaseScean(HWND cur_hwnd);

	void UpdateGame();


	int FindFreePlayerIdxInList();
	void InitNewClient(int idx, int id, int hp, int x, int y, int direction, CONST WCHAR *nickname);
	void DeleteClient(int idx);

	BOOL CheckDifferentInputKeyBefore();
	void GetCurInputMsgByObject(CPlayerObject *out_plyaer_object);
	void StartAttack1(int id, ACTION_PLAYER msg, int direction, int x, int y);
	void StartAttack2(int id, ACTION_PLAYER msg, int direction, int x, int y);
	void StartAttack3(int id, ACTION_PLAYER msg, int direction, int x, int y);

	void StartMove(int id, ACTION_PLAYER msg, int x, int y);
	void StopMove(int id, DIRECTION msg, int x, int y);
	void Damage(int id, int damage_hp);
	
	void MakeEffect(int id);
	void PrintNickName(CONST WCHAR *in_text, int x, int y, BOOL this_client);

protected:

	HWND _cur_hwnd;		///<스크린 출력 대상 윈도우 핸들.

	//------------------------------------------------------------------------------
	//	전역 싱글톤 DIB 스크린 생성한다.
	//------------------------------------------------------------------------------
	CSCREEN_DIB::CScreenDib *g_screen_dib;

	//------------------------------------------------------------------------------
	//	SPRITE 관리자 생성한다.
	//------------------------------------------------------------------------------
	CSPRITE_DIB::CSpriteDib *g_csprite;


	void InitScean();
	void LoadContent();
	void FrocDrawByFrameSkip();
	BOOL HandleClientInputKey();

	void DisplayFPSOnCaption();



	//모든 스프라이트를 로드한다.
	BOOL LoadSprites();

	//씬 안 존재하는 플레이어를 스프라이트로 등록한다.
	BOOL AddChildSpriteNode(CPlayerObject *player_object);

	//씬 안에 모든 스프라이트 노드를 백버퍼에 그려준다.
	BOOL DrawAllChildSpriteNode();


	//백버퍼 DIB에 그린 씬을 출력한다.
	void FlipScean();

	//백버퍼 DIB에 맵을 그린다.
	void DrawMap();

	//클라이언트 메시지를 처리한다.
	void ProcClientsMsg();

	void DisplayAllNickName();


};

#include "stdafx.h"