
#pragma once
#include "lib_CSPRITE_DIB.h"
#include "lib_CSCREEN_DIB.h"

#include "df_sprite_setting.h"
#include "CPlayerObject.h"
#include "CBaseScean.h"
#include "CFrameSkip.h"
#include "CSpriteNode.h"

//------------------------------------------------------------------------------
//	Ŭ���� ������
//
//			     CSean
//		       /         
//		  CStage1  
//
//------------------------------------------------------------------------------

/**
@class CBaseScean

@brief ��� ���� ���̽��� �Ǵ� Ŭ����.

@todo : ���� h, cpp �и� �� ��!
*/
class CBaseScean {

private:

	//��������Ʈ ����.
	list<CSpriteNode*>_list_csprite;	///<��������Ʈ ����Ʈ
	int num_total_player;				///<�ִ� �̿� ���� �÷��̾� ����

	//������ ��ŵ.
	CFrameSkip *_frame_skip;		   

public:

	//�÷��� ����Ʈ.
	CPlayerObject * _arr_player;


	//������1
	CBaseScean();

	//������2
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

	HWND _cur_hwnd;		///<��ũ�� ��� ��� ������ �ڵ�.

	//------------------------------------------------------------------------------
	//	���� �̱��� DIB ��ũ�� �����Ѵ�.
	//------------------------------------------------------------------------------
	CSCREEN_DIB::CScreenDib *g_screen_dib;

	//------------------------------------------------------------------------------
	//	SPRITE ������ �����Ѵ�.
	//------------------------------------------------------------------------------
	CSPRITE_DIB::CSpriteDib *g_csprite;


	void InitScean();
	void LoadContent();
	void FrocDrawByFrameSkip();
	BOOL HandleClientInputKey();

	void DisplayFPSOnCaption();



	//��� ��������Ʈ�� �ε��Ѵ�.
	BOOL LoadSprites();

	//�� �� �����ϴ� �÷��̾ ��������Ʈ�� ����Ѵ�.
	BOOL AddChildSpriteNode(CPlayerObject *player_object);

	//�� �ȿ� ��� ��������Ʈ ��带 ����ۿ� �׷��ش�.
	BOOL DrawAllChildSpriteNode();


	//����� DIB�� �׸� ���� ����Ѵ�.
	void FlipScean();

	//����� DIB�� ���� �׸���.
	void DrawMap();

	//Ŭ���̾�Ʈ �޽����� ó���Ѵ�.
	void ProcClientsMsg();

	void DisplayAllNickName();


};

#include "stdafx.h"