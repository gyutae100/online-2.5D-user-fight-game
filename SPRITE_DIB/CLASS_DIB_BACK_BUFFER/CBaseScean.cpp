
#include "stdafx.h"
#include "CBaseScean.h"

extern BOOL g_active_app;


void CBaseScean::StartAttack1(int id, ACTION_PLAYER msg, int direction, int x, int y) {

	for (int idx = 0; idx < num_total_player; idx++) {

		if (id == _arr_player[idx]._id) {

			_arr_player[idx].SaveCurActionMsg(msg);
			_arr_player[idx].SetCurXY(x, y);
			_arr_player[idx]._last_direction = static_cast<DIRECTION> (direction);
		}
	}
}

void CBaseScean::StartAttack2(int id, ACTION_PLAYER msg, int direction, int x, int y) {

	for (int idx = 0; idx < num_total_player; idx++) {

		if (id == _arr_player[idx]._id) {

			_arr_player[idx].SaveCurActionMsg(msg);
			_arr_player[idx].SetCurXY(x, y);
			_arr_player[idx]._last_direction = static_cast<DIRECTION> (direction);
		}
	}

}


void CBaseScean::StartMove(int id, ACTION_PLAYER msg,  int x, int y) {

	for (int idx = 0; idx < num_total_player; idx++) {

		if (id == _arr_player[idx]._id) {

			_arr_player[idx].SaveCurActionMsg(msg);
			_arr_player[idx].SetCurXY(x, y);
		}
	}

}

void CBaseScean::StopMove(int id, DIRECTION direction, int x, int y) {

	for (int idx = 0; idx < num_total_player; idx++) {

		if (id == _arr_player[idx]._id) {

			_arr_player[idx].SaveCurActionMsg(ACTION_PLAYER::ACTION_STAND);
			_arr_player[idx]._last_direction = direction;
			_arr_player[idx].SetCurXY(x, y);
		}
	}

}


void CBaseScean::Damage(int id, int damage_hp) {

	for (int idx = 0; idx < num_total_player; idx++) {

		if (id == _arr_player[idx]._id) {

			_arr_player[idx].SetHp(damage_hp);
		}
	}

}

void CBaseScean::StartAttack3(int id, ACTION_PLAYER msg, int direction, int x, int y) {

	for (int idx = 0; idx < num_total_player; idx++) {

		if (id == _arr_player[idx]._id) {

			_arr_player[idx].SaveCurActionMsg(msg);
			_arr_player[idx].SetCurXY(x, y);
			_arr_player[idx]._last_direction = static_cast<DIRECTION> (direction);
		}
	}

}

//현재 해당 클라이언트의 메시지를 반환한다.
void  CBaseScean::GetCurInputMsgByObject(CPlayerObject *out_plyaer_object) {

	*out_plyaer_object = _arr_player[0];
	
}

//다른 키가 눌린 여부를 체크한다.
BOOL CBaseScean::CheckDifferentInputKeyBefore() {

	if (_arr_player[0]._cur_input_action_msg != _arr_player[0]._pre_input_action_msg) {

		return TRUE;

	}
	else {

		return FALSE;
	}
}

int CBaseScean::FindFreePlayerIdxInList() {

	for (int idx = 0; idx < num_total_player; idx++) {

		BOOL is_playing = _arr_player[idx]._is_playing;
		if (FALSE == is_playing) {

			return idx;
		}
	}

	return -1;
}

void CBaseScean::InitNewClient(int idx, int id, int hp, int x, int y, int direction, CONST WCHAR *nickname) {

	DIRECTION cur_direction;
	if (DIRECTION::DIRECTION_LEFT == direction) {

		cur_direction = DIRECTION::DIRECTION_LEFT;
	}
	if (DIRECTION::DIRECTION_RIGHT == direction) {

		cur_direction = DIRECTION::DIRECTION_RIGHT;
	}

	//메인 플레이어 여부 체크한다.
	if (idx == 0) {

		_arr_player[idx]._is_main_player = TRUE;
	}
	else {

		_arr_player[idx]._is_main_player = FALSE;
	}


	_arr_player[idx].InitPlayerInfo(id, hp, x, y, cur_direction, nickname);
	_arr_player[idx].InitPlayerDirection(cur_direction);
	_arr_player[idx].SaveCurActionMsg(ACTION_PLAYER::ACTION_STAND);


	AddChildSpriteNode(&_arr_player[idx]);
}

void CBaseScean::DeleteClient(int id) {

	//배열 idx 검색 후 스프라이트 삭제한다.
	int cur_idx = 0;
	for (; cur_idx < num_total_player; cur_idx++) {

		if (id == _arr_player[cur_idx]._id) {

			break;
		}
	}

	_arr_player[cur_idx]._is_playing = FALSE;

	//관련 스프라이트 삭제한다.
	list<CSpriteNode*>::iterator iter = _list_csprite.begin();
	for (;  iter != _list_csprite.end(); iter++) {

		if ((*iter)->_cur_object_addr ==  &_arr_player[cur_idx]) {

			_list_csprite.erase(iter);
		}
	}
}




//생성자1
CBaseScean::CBaseScean() {

	num_total_player = 100;
}

//생성자2
CBaseScean::CBaseScean(HWND cur_hwnd) {

	//스크린 출력용 DIB 생성한다.
	g_csprite = new CSPRITE_DIB::CSpriteDib(SPRITE_SETTING::MAX_LOAD_SPRITE, 0x00ffffff);

	//스프라이트 매니저를 생성한다.
	g_screen_dib = new CSCREEN_DIB::CScreenDib(SCREEN_RESOLUTION::SCREEN_WIDTH, SCREEN_RESOLUTION::SCREEN_HEIGHT, 32 /**픽셀 비트 값*/);

	//스크린 출력 대상 윈도우 핸들을 저장한다.
	_cur_hwnd = cur_hwnd;

	//게임 시작시 필요한 기본 초기화를 처리한다.
	InitScean();

	//씬 구성에 필요한 스프라이트 목록을 읽어 저장ㅎ나다.
	LoadContent();

	num_total_player = 100;

}








/**
brief 씬 내 모든 요소를 업데이트 한다.
*/
void CBaseScean::UpdateGame()
{
	//====================================================================================
	// 어플리케이션 활성화 여부에 따라 사용자 입력 키를 해당 클라이언트 캐릭터에 적용한다.
	//====================================================================================
	if (TRUE == g_active_app) {


		HandleClientInputKey();


		//여기서 메시지를 보낸다.


	}

	//====================================================================================
	// 어플리케이션 활성화 여부와 관계 없이 게임 업데이트는 진행되어야 한다.
	//====================================================================================

	//====================================================================================
	// 모든 클라이언트들의 메시지를 처리한다.
	//====================================================================================
	ProcClientsMsg();

	//====================================================================================
	// 프레임 스킵에 의해 씬 내 모든 오브젝트 스프라이트 업데이트 여부를 결정한다.
	//====================================================================================
	FrocDrawByFrameSkip();
}


/**
brief FPS 출력한다.
*/
void CBaseScean::DisplayFPSOnCaption() {

	//추후 프레임 스킵으로 변경한다.
	WCHAR temp_string[20];
	float fps;
	BOOL is_working;

	is_working = _frame_skip->GetFPS(&fps);

	HDC hdc;
	hdc = GetDC(_cur_hwnd);

	_itow_s((int)fps, temp_string, 10);
	TextOut(hdc, 0, 0, temp_string, 2);

	ReleaseDC(_cur_hwnd, hdc);
}

/**
brief 클라이언트 메시지를 처리한다. 프레임 스킵 발생해도 로직은 정상적으로 처리가 되어야 한다.
*/
void CBaseScean::ProcClientsMsg() {

	//접속한 유저만 처리하게 한다.
	//해당 클라이언트 객체. 

	for (int idx = 0; idx < num_total_player; idx++) {
	

		if( TRUE == _arr_player[idx]._is_playing )
		
			_arr_player[idx].OnAction();
	}

	//나머지 클라이언트 키보드 입력 처리한다.
	//_arr_player[1].OnAction();
	//_arr_player[2].OnAction();
}


//프로텍티드

/**
brief 게임 시작시 필요한 기본 초기화를 처리 한다.
*/
void CBaseScean::InitScean() {

	//프레임 스킵을 동적 할당한다.
	_frame_skip = new CFrameSkip(20);

	num_total_player = 100;

	//유저를 동적 할당한다.
	_arr_player = new CPlayerObject[num_total_player];

	for (int i = 0; i < num_total_player; i++) {

		_arr_player[i]._is_playing = FALSE;
	}

}

/**
brief 씬 구성에 필요한 스트라이트 목록을 읽어 저장한다.
*/
void CBaseScean::LoadContent() {

	//스프라이트 로드한다.
	LoadSprites();
}

/**
brief 씬 안에 플레이어를 스프라이트 노드로 등록한다.
*/
BOOL CBaseScean::AddChildSpriteNode(CPlayerObject *player_object) {

	if (NULL == player_object) {

		return false;
	}

	//-----------------------------------------------------------------------------------------------
	// 메인 플레이어인 경우.
	//-----------------------------------------------------------------------------------------------
	if (player_object->_is_main_player) {

		//플레이어는 _hp 스프라이트, 플레이어 스프라이트, shadow 스프라이트로 구성된다.
		_list_csprite.push_front(new CSpriteNode(player_object) );
		_list_csprite.push_front(new CSpriteNode(player_object, TYPE_OBJECT::OBJECT_HP));
		_list_csprite.push_front(new CSpriteNode(player_object, TYPE_OBJECT::OBJECT_SHADOW));
		_list_csprite.push_front(new CSpriteNode(player_object, TYPE_OBJECT::OBJECT_NICKNAME));

	}

	//-----------------------------------------------------------------------------------------------
	// 그 외 플레이어인 경우.
	//-----------------------------------------------------------------------------------------------
	else {

		//플레이어는 _hp 스프라이트, 플레이어 스프라이트, shadow 스프라이트로 구성된다.
		_list_csprite.push_front(new CSpriteNode(player_object));
		_list_csprite.push_front(new CSpriteNode(player_object, TYPE_OBJECT::OBJECT_HP));
		_list_csprite.push_front(new CSpriteNode(player_object, TYPE_OBJECT::OBJECT_SHADOW));
		_list_csprite.push_front(new CSpriteNode(player_object, TYPE_OBJECT::OBJECT_NICKNAME));

	}

	///@todo : 추후 닉네임도 추가해주자.

	return true;
}


//y 축 순서로 오름차순 함수이다.
struct SortYAscending {

	bool operator() (const CSpriteNode *pObject1, const CSpriteNode *pObject2) const {

		return (pObject1->_cur_y < pObject2->_cur_y);
	}
};

/**
brief 씬 안에 모든 스프라이트 노드를 백버퍼에 그려준다.
*/
BOOL CBaseScean::DrawAllChildSpriteNode() {

	//-----------------------------------------------------------------------------------------------
	//씬 안에 모든 스프라이트 노드를 갱신한다.
	//-----------------------------------------------------------------------------------------------
	list<CSpriteNode *>::iterator iter;
	for (iter = _list_csprite.begin(); iter != _list_csprite.end(); iter++) {

		CSpriteNode *tmp = *iter;

		tmp->UpdateSpriteNode();

	}
	//-----------------------------------------------------------------------------------------------
	//y측 기준으로 정렬한다.
	//-----------------------------------------------------------------------------------------------
	_list_csprite.sort(SortYAscending());

	//------------------------------------------------------------------------------
	//	출력 버퍼 포인터 및 정보얻는다.
	//------------------------------------------------------------------------------
	BYTE *image_buffer = g_screen_dib->GetDibBuffer();
	int dest_width = g_screen_dib->GetWidth();
	int dest_height = g_screen_dib->GetHeight();
	int dest_pitch = g_screen_dib->GetPitch();


	int cur_x = -1;
	int cur_y = -1;

	SPRITE_PLAYER cur_sprite = SPRITE_PLAYER::PLAYER_NULL;
	SPRITE_FIX cur_fix_sprite = SPRITE_FIX::ONCE_NULL;


	//-----------------------------------------------------------------------------------------------
	//스프라이트를 출력 깊이 우선순위와 y축 기준에 따라 출력한다.
	//-----------------------------------------------------------------------------------------------
	int start_depth = static_cast<int>(DEPTH_PRIORITY::PRIORITY_7TH);
	int end_depth = static_cast<int>(DEPTH_PRIORITY::PRIORITY_NULL);
	int cur_depth;
	for (int start_depth = static_cast<int>(DEPTH_PRIORITY::PRIORITY_7TH); start_depth > end_depth; start_depth--) {

		for (iter = _list_csprite.begin(); iter != _list_csprite.end(); iter++) {

			CSpriteNode *tmp = *iter;

			cur_x = tmp->_cur_x;
			cur_y = tmp->_cur_y;

			//-----------------------------------------------------------------------------------------------
			// 출력 깊이 계산.
			//-----------------------------------------------------------------------------------------------
			cur_depth = static_cast<int>(tmp->_depth_attach_priority);
			if (cur_depth != start_depth) {

				continue;
			}

			//-----------------------------------------------------------------------------------------------
			//	출력 스프라이트가 플레이어인 경우.
			//-----------------------------------------------------------------------------------------------
			if (tmp->_type_object == TYPE_OBJECT::OBJECT_PLAYER) {
				SPRITE_PLAYER cur_sprite = tmp->_cur_player_sprite;

				//-----------------------------------------------------------------------------------------------
				// 메인 플레이어 유무에 따라 스프라이트 색상이 변경된다.
				//-----------------------------------------------------------------------------------------------
				if (TRUE == tmp->_is_main_player) {
					g_csprite->DrawFullSprite(cur_sprite, cur_x, cur_y, image_buffer, dest_width, dest_height, dest_pitch);
				}
				else {

					g_csprite->DrawFullSpriteLikeMonster(cur_sprite, cur_x, cur_y, image_buffer, dest_width, dest_height, dest_pitch);
				}
			}

			///@ todo: 닉네임 추후 할 것
			else if (tmp->_type_object == TYPE_OBJECT::OBJECT_NICKNAME) {

				//PrintNickName(tmp->_nickname, cur_x, cur_y);
				
			}

			//-----------------------------------------------------------------------------------------------
			// 출력 스프라이트가 HP인 경우.
			//-----------------------------------------------------------------------------------------------
			else if (tmp->_type_object == TYPE_OBJECT::OBJECT_HP) {

				
				cur_fix_sprite = tmp->_cur_fix_sprite;
				g_csprite->DrawFullSprite(cur_fix_sprite, cur_x, cur_y, image_buffer, dest_width, dest_height, dest_pitch, tmp->_option_value1);
			}

			//-----------------------------------------------------------------------------------------------
			// 출력 스프라이트가 그림자인 경우.
			//-----------------------------------------------------------------------------------------------
			else if (tmp->_type_object == TYPE_OBJECT::OBJECT_SHADOW) {

				cur_fix_sprite = tmp->_cur_fix_sprite;
				g_csprite->DrawFullSprite(cur_fix_sprite, cur_x, cur_y, image_buffer, dest_width, dest_height, dest_pitch);
			}

			//-----------------------------------------------------------------------------------------------
			// 출력 스프라이트가 이펙트인 경우
			//-----------------------------------------------------------------------------------------------
			else if (tmp->_type_object == TYPE_OBJECT::OBJECT_EFFECT) {


				if (FALSE == (*iter)->_cur_object_addr->OnAction()) {

					_list_csprite.remove(*iter);
					continue;
				}

				SPRITE_EFFECT cur_effect_sprite = tmp->_cur_effect_sprite;

				int x= (*iter)->_cur_x;
				int y= (*iter)->_cur_y;
				
				//-----------------------------------------------------------------------------------------------
				// 메인 플레이어 유무에 따라 스프라이트 색상이 변경된다.
				//-----------------------------------------------------------------------------------------------
				if (TRUE == tmp->_is_main_player) {

					g_csprite->DrawFullSprite(cur_effect_sprite, cur_x, cur_y, image_buffer, dest_width, dest_height, dest_pitch);
				}
				else {

					g_csprite->DrawFullSpriteLikeMonster(cur_effect_sprite, cur_x, cur_y, image_buffer, dest_width, dest_height, dest_pitch);
				}
			}

		}
	}

	return true;
}


/**
brief 클라이언트(본인) 입력 키 처리한다.
*/
BOOL CBaseScean::HandleClientInputKey() {

	ACTION_PLAYER cur_action_msg = ACTION_PLAYER::ACTION_NULL;

	//==============================================================================
	//	이동 키 파트.
	//==============================================================================

	//==============================================================================
	//	플레이어 왼쪽 관련 방향키 입력.
	//==============================================================================
	if (GetAsyncKeyState(VK_LEFT) & 0x8000) {

		//왼쪽 위.
		if (GetAsyncKeyState(VK_UP) & 0x8000) {

			cur_action_msg = ACTION_PLAYER::ACTION_MOVE_LU;
		}

		//왼쪽 아래.
		else if (GetAsyncKeyState(VK_DOWN) & 0x8000) {

			cur_action_msg = ACTION_PLAYER::ACTION_MOVE_LD;
		}
		//왼쪽. 
		else {

			//왼쪽 방향키
			cur_action_msg = ACTION_PLAYER::ACTION_MOVE_LL;
		}
	}


	//==============================================================================
	//	플레이어 오른쪽 관련 방향키 입력.
	//==============================================================================
	else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {

		//오른쪽 위.
		if (GetAsyncKeyState(VK_UP) & 0x8000) {

			cur_action_msg = ACTION_PLAYER::ACTION_MOVE_RU;
		}

		//오른쪽 아래.
		else if (GetAsyncKeyState(VK_DOWN) & 0x8000) {

			cur_action_msg = ACTION_PLAYER::ACTION_MOVE_RD;
		}
		//오른쪽.
		else {

			cur_action_msg = ACTION_PLAYER::ACTION_MOVE_RR;
		}
	}


	//==============================================================================
	//	플레이어 아래쪽 방향키 입력.
	//==============================================================================
	else if (GetAsyncKeyState(VK_DOWN) & 0x8000) {

		cur_action_msg = ACTION_PLAYER::ACTION_MOVE_DD;
	}


	//==============================================================================
	//	플레이어 위쪽 방향키 입력
	//==============================================================================
	else if (GetAsyncKeyState(VK_UP) & 0x8000) {

		cur_action_msg = ACTION_PLAYER::ACTION_MOVE_UU;
	}


	//==============================================================================
	//	서있는 상태. 아무 방향키도 안 누른 상태.
	//==============================================================================
	else {

		cur_action_msg = ACTION_PLAYER::ACTION_STAND;
	}


	//==============================================================================
	//	공격 키 파트. 이동 키보다 우선순위가 높다.
	//==============================================================================

	//------------------------------------------------------------------------------
	//	플레이어 Z키 어텍 1 입력.
	//------------------------------------------------------------------------------
	if (GetAsyncKeyState(0x5A) & 0x8000) {

		cur_action_msg = ACTION_PLAYER::ACTION_ATTACK1;
	}

	//------------------------------------------------------------------------------
	//	플레이어 _x키 어텍 2 입력.
	//------------------------------------------------------------------------------
	if (GetAsyncKeyState(0x58) & 0x8000) {

		cur_action_msg = ACTION_PLAYER::ACTION_ATTACK2;
	}

	//------------------------------------------------------------------------------
	//	플레이어 C키 어텍 3 입력.
	//------------------------------------------------------------------------------
	if (GetAsyncKeyState(0x43) & 0x8000) {

		cur_action_msg = ACTION_PLAYER::ACTION_ATTACK3;
	}


	//해당 클라이언트 객체에게 사용자 클라이언트 액션을 보낸다.
	_arr_player[0].SaveCurActionMsg(cur_action_msg);


	return true;
}

/**
brief 씬 안에 모든 구성 요소를 프레임 스킵 여부에 따라 그린다.
*/
void CBaseScean::FrocDrawByFrameSkip()
{

	DisplayFPSOnCaption();

	//------------------------------------------------------------------------------
	//	시간 측정 및 FPS를 출력한다.
	//------------------------------------------------------------------------------
	_frame_skip->SelectBeginOrEndTick();

	//------------------------------------------------------------------------------
	//	프레임 스킵 여부 체크 후 맞으면 프레임 체크한다.
	//------------------------------------------------------------------------------
	if (TRUE == _frame_skip->CheckFrameSkip()) {

		return;
	}

	//------------------------------------------------------------------------------
	//	맵을 그린다.
	//------------------------------------------------------------------------------
	DrawMap();

	//------------------------------------------------------------------------------
	//	모든 스프라이트를 그린다.
	//------------------------------------------------------------------------------
	DrawAllChildSpriteNode();

	//------------------------------------------------------------------------------
	//	씬을 플립한다.
	//------------------------------------------------------------------------------
	FlipScean();


}


void CBaseScean::DisplayAllNickName() {

	list<CSpriteNode *>::iterator iter;

	for (iter = _list_csprite.begin(); iter != _list_csprite.end(); iter++) {

		CSpriteNode *tmp = *iter;


		int cur_x = tmp->_cur_x;
		int cur_y = tmp->_cur_y;
		DIRECTION cur_direction = tmp->_direction;


		if (tmp->_type_object == TYPE_OBJECT::OBJECT_NICKNAME) {

			//-----------------------------------------------------------------------------------------------
			// 메인 플레이어 유무에 따라 닉네임 색상이 변경된다.
			// 플레이어 객체 방향에 따라 닉네임 출력 위치 보정값이 필요하다.
			//-----------------------------------------------------------------------------------------------
			if (&_arr_player[0] == tmp->_cur_object_addr) {


				if (DIRECTION::DIRECTION_LEFT == cur_direction) {

					PrintNickName(tmp->_nickname, cur_x - 30, cur_y - 110, TRUE);
				}
				else {

					PrintNickName(tmp->_nickname, cur_x - 10, cur_y - 110, TRUE);
				}
			}
			else {

				if (DIRECTION::DIRECTION_LEFT == cur_direction) {

					PrintNickName(tmp->_nickname, cur_x - 30, cur_y - 110, FALSE);
				}
				else {

					PrintNickName(tmp->_nickname, cur_x - 10, cur_y - 110, FALSE);
				}
			}
		}
	}

}

/**
brief 백버퍼 DIB에 그린 씬을 출력한다.
*/
void CBaseScean::FlipScean() {

	//------------------------------------------------------------------------------
	//	ScreenDib를 화면으로 플립한다.
	//------------------------------------------------------------------------------
	g_screen_dib->DrawBuffer(_cur_hwnd);

	//------------------------------------------------------------------------------
	//	모든 플레이어들의 닉네임을 출력한다.
	//------------------------------------------------------------------------------
	DisplayAllNickName();


}

/**
brief 백버퍼 DIB에 맵을 그린다.
*/
void CBaseScean::DrawMap() {

	//------------------------------------------------------------------------------
	//	출력 버퍼 포인터 및 정보얻는다.
	//------------------------------------------------------------------------------
	BYTE *image_buffer = g_screen_dib->GetDibBuffer();
	int dest_width = g_screen_dib->GetWidth();
	int dest_height = g_screen_dib->GetHeight();
	int dest_pitch = g_screen_dib->GetPitch();

	//------------------------------------------------------------------------------
	//	1. 맵 화면 출력한다.
	//------------------------------------------------------------------------------
	g_csprite->DrawImage(SPRITE_MAP::MAP_STAGE1, 0, 0, image_buffer, dest_width, dest_height, dest_pitch);
}

/**
brief 씬 구성에 필요한 스프라이트 목록을 읽어 저장한다.
*/
BOOL CBaseScean::LoadSprites() {

	//맵 초기화 한다.
	g_csprite->LoadDibSprite(SPRITE_MAP::MAP_STAGE1, L"SpriteData\\_Map.bmp", 0, 0);

	//왼쪽 스텐드 스프라이트 초기화한다.
	g_csprite->LoadDibSprite(3, L"SpriteData\\STAND_L_01.bmp", 71, 90);
	g_csprite->LoadDibSprite(4, L"SpriteData\\STAND_L_02.bmp", 71, 90);
	g_csprite->LoadDibSprite(5, L"SpriteData\\STAND_L_03.bmp", 71, 90);


	//오른쪽 스텐드 스프라이트 초기화한다.
	g_csprite->LoadDibSprite(6, L"SpriteData\\STAND_R_01.bmp", 71, 90);
	g_csprite->LoadDibSprite(7, L"SpriteData\\STAND_R_02.bmp", 71, 90);
	g_csprite->LoadDibSprite(8, L"SpriteData\\STAND_R_03.bmp", 71, 90);


	//왼쪽 걷기 스프라이트 초기화 한다.
	g_csprite->LoadDibSprite(9, L"SpriteData\\Move_L_01.bmp", 71, 90);
	g_csprite->LoadDibSprite(10, L"SpriteData\\Move_L_02.bmp", 71, 90);
	g_csprite->LoadDibSprite(11, L"SpriteData\\Move_L_03.bmp", 71, 90);
	g_csprite->LoadDibSprite(12, L"SpriteData\\Move_L_04.bmp", 71, 90);
	g_csprite->LoadDibSprite(13, L"SpriteData\\Move_L_05.bmp", 71, 90);
	g_csprite->LoadDibSprite(14, L"SpriteData\\Move_L_06.bmp", 71, 90);
	g_csprite->LoadDibSprite(15, L"SpriteData\\Move_L_07.bmp", 71, 90);
	g_csprite->LoadDibSprite(16, L"SpriteData\\Move_L_08.bmp", 71, 90);
	g_csprite->LoadDibSprite(17, L"SpriteData\\Move_L_09.bmp", 71, 90);
	g_csprite->LoadDibSprite(18, L"SpriteData\\Move_L_10.bmp", 71, 90);
	g_csprite->LoadDibSprite(19, L"SpriteData\\Move_L_11.bmp", 71, 90);
	g_csprite->LoadDibSprite(20, L"SpriteData\\Move_L_12.bmp", 71, 90);


	//오른쪽 걷기 스프라이트 초기화 한다.
	g_csprite->LoadDibSprite(21, L"SpriteData\\Move_R_01.bmp", 71, 90);
	g_csprite->LoadDibSprite(22, L"SpriteData\\Move_R_02.bmp", 71, 90);
	g_csprite->LoadDibSprite(23, L"SpriteData\\Move_R_03.bmp", 71, 90);
	g_csprite->LoadDibSprite(24, L"SpriteData\\Move_R_04.bmp", 71, 90);
	g_csprite->LoadDibSprite(25, L"SpriteData\\Move_R_05.bmp", 71, 90);
	g_csprite->LoadDibSprite(26, L"SpriteData\\Move_R_06.bmp", 71, 90);
	g_csprite->LoadDibSprite(27, L"SpriteData\\Move_R_07.bmp", 71, 90);
	g_csprite->LoadDibSprite(28, L"SpriteData\\Move_R_08.bmp", 71, 90);
	g_csprite->LoadDibSprite(29, L"SpriteData\\Move_R_09.bmp", 71, 90);
	g_csprite->LoadDibSprite(30, L"SpriteData\\Move_R_10.bmp", 71, 90);
	g_csprite->LoadDibSprite(31, L"SpriteData\\Move_R_11.bmp", 71, 90);
	g_csprite->LoadDibSprite(32, L"SpriteData\\Move_R_12.bmp", 71, 90);


	//오른쪽 공격1 스프라이트 초기화 한다.
	g_csprite->LoadDibSprite(33, L"SpriteData\\Attack1_R_01.bmp", 71, 90);
	g_csprite->LoadDibSprite(34, L"SpriteData\\Attack1_R_02.bmp", 71, 90);
	g_csprite->LoadDibSprite(35, L"SpriteData\\Attack1_R_03.bmp", 71, 90);
	g_csprite->LoadDibSprite(36, L"SpriteData\\Attack1_R_04.bmp", 71, 90);



	//왼쪽 공격1 스프라이트 초기화 한다.
	g_csprite->LoadDibSprite(37, L"SpriteData\\Attack1_L_01.bmp", 71, 90);
	g_csprite->LoadDibSprite(38, L"SpriteData\\Attack1_L_02.bmp", 71, 90);
	g_csprite->LoadDibSprite(39, L"SpriteData\\Attack1_L_03.bmp", 71, 90);
	g_csprite->LoadDibSprite(40, L"SpriteData\\Attack1_L_04.bmp", 71, 90);


	//오른쪽 공격2 스프라이트 초기화 한다.
	g_csprite->LoadDibSprite(41, L"SpriteData\\Attack2_R_01.bmp", 71, 90);
	g_csprite->LoadDibSprite(42, L"SpriteData\\Attack2_R_02.bmp", 71, 90);
	g_csprite->LoadDibSprite(43, L"SpriteData\\Attack2_R_03.bmp", 71, 90);
	g_csprite->LoadDibSprite(44, L"SpriteData\\Attack2_R_04.bmp", 71, 90);


	//왼쪽 공격2 스프라이트 초기화 한다.
	g_csprite->LoadDibSprite(45, L"SpriteData\\Attack2_L_01.bmp", 71, 90);
	g_csprite->LoadDibSprite(46, L"SpriteData\\Attack2_L_02.bmp", 71, 90);
	g_csprite->LoadDibSprite(47, L"SpriteData\\Attack2_L_03.bmp", 71, 90);
	g_csprite->LoadDibSprite(48, L"SpriteData\\Attack2_L_04.bmp", 71, 90);


	//오른쪽 공격3 스프라이트 초기화 한다.
	g_csprite->LoadDibSprite(49, L"SpriteData\\Attack3_R_01.bmp", 71, 90);
	g_csprite->LoadDibSprite(50, L"SpriteData\\Attack3_R_02.bmp", 71, 90);
	g_csprite->LoadDibSprite(51, L"SpriteData\\Attack3_R_03.bmp", 71, 90);
	g_csprite->LoadDibSprite(52, L"SpriteData\\Attack3_R_04.bmp", 71, 90);
	g_csprite->LoadDibSprite(53, L"SpriteData\\Attack3_R_05.bmp", 71, 90);
	g_csprite->LoadDibSprite(54, L"SpriteData\\Attack3_R_06.bmp", 71, 90);


	//왼쪽 공격3 스프라이트 초기화 한다.
	g_csprite->LoadDibSprite(55, L"SpriteData\\Attack3_L_01.bmp", 71, 90);
	g_csprite->LoadDibSprite(56, L"SpriteData\\Attack3_L_02.bmp", 71, 90);
	g_csprite->LoadDibSprite(57, L"SpriteData\\Attack3_L_03.bmp", 71, 90);
	g_csprite->LoadDibSprite(58, L"SpriteData\\Attack3_L_04.bmp", 71, 90);
	g_csprite->LoadDibSprite(59, L"SpriteData\\Attack3_L_05.bmp", 71, 90);
	g_csprite->LoadDibSprite(60, L"SpriteData\\Attack3_L_06.bmp", 71, 90);



	//이펙트 스프라이트 초기화한다.
	g_csprite->LoadDibSprite(62, L"SpriteData\\xSpark_1.bmp", 70, 70);
	g_csprite->LoadDibSprite(63, L"SpriteData\\xSpark_2.bmp", 70, 70);
	g_csprite->LoadDibSprite(64, L"SpriteData\\xSpark_3.bmp", 70, 70);
	g_csprite->LoadDibSprite(65, L"SpriteData\\xSpark_4.bmp", 70, 70);

	//체력 스프라이트 초기화한다.
	g_csprite->LoadDibSprite(67, L"SpriteData\\HPGuage.bmp", +35, -15);

	//그림자 스프라이트 초기화한다.
	g_csprite->LoadDibSprite(68, L"SpriteData\\Shadow.bmp", 32, 4);


	return true;
}


void CBaseScean::MakeEffect(int id) {

	for (int idx = 0; idx < num_total_player; idx++) {

		if (_arr_player[idx]._id == id) {
			_list_csprite.push_front(new CSpriteNode(&_arr_player[idx], TYPE_OBJECT::OBJECT_EFFECT));
		}
	}
}


//닉네임을 출력한다.
void CBaseScean::PrintNickName(CONST WCHAR *in_text, int x, int y, BOOL this_client) {

	HDC hdc;

	hdc = GetDC(_cur_hwnd);

	if (TRUE == this_client) {
		SetTextColor(hdc, RGB(0, 255, 0));
	}
	else {

		SetTextColor(hdc, RGB(255, 0, 0));
	}

	SetBkMode(hdc, TRANSPARENT);

	TextOut(hdc, x, y, in_text, (int)wcslen(in_text));
	ReleaseDC(_cur_hwnd, hdc);
}
