#pragma once
#include "stdafx.h"

#include "lib_CSPRITE_DIB.h"
#include "lib_CSCREEN_DIB.h"

#include "df_sprite_setting.h"
#include "CFrameSkip.h"
#include "CBaseObject.h"
#include "CPlayerObject.h"
#include "CBaseScean.h"
#include "CStageScean.h"
#include "CSpriteNode.h"

/**
brief 생성자1
*/
CPlayerObject::CPlayerObject(){

	_is_main_player = FALSE;

	_is_attacking = FALSE;
	_is_stop_moving = FALSE;
	_is_start_attack = FALSE;

	_is_playing = FALSE;

	//객체 타입 설정
	_type_object = TYPE_OBJECT::OBJECT_PLAYER;

	//초기 게임 시작시 스프라이트 
	_cur_player_sprite = SPRITE_PLAYER::PLAYER_STAND_L_1;

	//마지막 디렉션 설정
	_last_direction = DIRECTION::DIRECTION_LEFT;

	//스프라이트애니메이션 설정
	SetSpriteFilmFrom(IDX_SPRITE_PLAYER_FILM::IDX_ACTION_STAND_L);

	//기본 진행 액션
	_cur_action = ACTION_PLAYER::ACTION_STAND;
	
	_pre_input_action_msg = ACTION_PLAYER::ACTION_STAND;
}


/**
brief 생성자2. 시작 위치 지정한다.
*/
CPlayerObject::CPlayerObject(int x, int y) {

	_is_main_player = FALSE;

	_is_start_attack = FALSE;
	_is_stop_moving = FALSE;
	_is_playing = FALSE;

	//객체 타입 설정
	_type_object = TYPE_OBJECT::OBJECT_PLAYER;

	//시작 위치 지정
	_cur_x = x;
	_cur_y = y;


	//초기 게임 시작시 스프라이트
	_cur_player_sprite = SPRITE_PLAYER::PLAYER_STAND_L_1;

	//마지막 디렉션 설정
	_last_direction = DIRECTION::DIRECTION_LEFT;

	//스프라이트 애니메이션 설정
	SetSpriteFilmFrom(IDX_SPRITE_PLAYER_FILM::IDX_ACTION_STAND_L);

	//기본 진행 액션
	_cur_action = ACTION_PLAYER::ACTION_STAND;

	_pre_input_action_msg = ACTION_PLAYER::ACTION_STAND;
}

/**
brief 소멸자
*/
CPlayerObject::~CPlayerObject() {

}


/**
brief 사용자 입력 액션 처리 메시지를 저장한다.
*/
void CPlayerObject::SaveCurActionMsg(ACTION_PLAYER cur_input_action) {

	//추후 BOOL 반환 할 것 
	if (cur_input_action < 0) {
		return;
	}

	//멈춰있는 상태 여부 설정.
	if (_is_moving == TRUE && cur_input_action == ACTION_PLAYER::ACTION_STAND) {

		_is_stop_moving = TRUE;
	}
	else {
	
		_is_stop_moving = FALSE;
	}


	//-----------------------------------------------------------------------------------------------
	// 공격 버튼을 연속으로 누르는 경우 중간중간 스텐드 자체가 한번 완료 후 어텍이 되어야 한다.
	//-----------------------------------------------------------------------------------------------
	if ( FALSE ==_is_end_frame && TRUE == CheckIsAttackAction( cur_input_action) &&TRUE == CheckIsAttackAction(_pre_input_action_msg)) {

		return;
	}
	



	_pre_input_action_msg = _cur_input_action_msg;
	_cur_input_action_msg = cur_input_action;
}

/**
brief 액션 진행용 콜백함수이다. 프레임 카운터 및 액션 처리 이전의 전처리 액션 함수를 실행한다. 
*/
BOOL  CPlayerObject::OnAction()
{
	//-----------------------------------------------------------------------------------------------
	//프레임 카운터 업한다.
	//-----------------------------------------------------------------------------------------------
	OnNextFrame();

	//-----------------------------------------------------------------------------------------------
	//액션 처리 메시지 필터링한다.
	//-----------------------------------------------------------------------------------------------
	if (TRUE == OnFilteringActionMsg()) {

		//-----------------------------------------------------------------------------------------------
		//액션 메시지 처리를 진행한다.
		//-----------------------------------------------------------------------------------------------
		OnProcActionMsg();
	}

	return true;
}


/**
brief 	사용자 입력 액션 메세지에 대한 전처리(필터링) 과정을 진행한다.
		사용자가 진행 중인 액션 중 끊기면 안되는 액션(예 공격)같은 액션인 경우 사용자 입력 액션 메시지를 무시하는 처리를 담당한다.
		사용자 액션이 1회성 액션인 경우 액션이 끝난 후 기본 자세인 스텐드 자세로 돌아가게 한다.
*/
BOOL CPlayerObject:: OnFilteringActionMsg(void){

	switch (_cur_action){

	//-----------------------------------------------------------------------------------------------
	// 어텍 진행이 입력 키보다 우선순위가 높다.
	// 어텍 메시지는 마지막 스프라이트 장면이면서 마지막 프레임 카운터이기 전까지 
	// 계속 스프라이트 애니메이션이 진행되어야 한다.
	//-----------------------------------------------------------------------------------------------
	case ACTION_PLAYER::ACTION_ATTACK1:
	case ACTION_PLAYER::ACTION_ATTACK2:
	case ACTION_PLAYER::ACTION_ATTACK3:

		//-----------------------------------------------------------------------------------------------
		//스프라이트 애니메이션이 끝난 여부를 체크한다.
		//-----------------------------------------------------------------------------------------------
		if (TRUE == IsEndSpriteFilm() ){

			//-----------------------------------------------------------------------------------------------
			//공격 액션은 액션이 끝난 후 스텐드 자세로 변경한다. (기본 자세)
			//-----------------------------------------------------------------------------------------------
			_cur_input_action_msg = ACTION_PLAYER::ACTION_STAND;
			return TRUE;
		}
		return FALSE;
	}

	return true;
}


/**
brief 	현재 객체가 걷는 계속 상태였는지 체크한다.
*/
BOOL CPlayerObject::CheckKeepMoving()
{
	if (false == _is_moving) {

		return false;
	}
	else {

		return true;
	}
}


/**
brief 	현재 방향이 이전과 다르게 변했는지 체크한다.

return : 방향이 변경 된 경우 TRUE, 아닌 경우 FALSE를 반환
*/
BOOL CPlayerObject::CheckChangeDirection(){

	//-----------------------------------------------------------------------------------------------
	//현재 이동 방향이 이전에 걷는 방향과 같은가 체크한다.
	//-----------------------------------------------------------------------------------------------
	switch (_cur_input_action_msg) {

	//-----------------------------------------------------------------------------------------------
	//위, 아래 이동은 이전 방향과 동일하다.
	//-----------------------------------------------------------------------------------------------
	case ACTION_PLAYER::ACTION_MOVE_UU:
	case ACTION_PLAYER::ACTION_MOVE_DD: {

		return false;
	}

	//-----------------------------------------------------------------------------------------------
	//현재 사용자 지정 메시지가 왼쪽 움직임인 경우
	//-----------------------------------------------------------------------------------------------
	case ACTION_PLAYER::ACTION_MOVE_LL:
	case ACTION_PLAYER::ACTION_MOVE_LU:
	case ACTION_PLAYER::ACTION_MOVE_LD: {

		//-----------------------------------------------------------------------------------------------
		//이전 움직임이 오른쪽인 경우
		//이동 움직임이 바뀌었다.
		//-----------------------------------------------------------------------------------------------
		if (_last_direction == DIRECTION::DIRECTION_RIGHT) {

			//이제는 왼쪽방향으로 이동한다.
			_last_direction = DIRECTION::DIRECTION_LEFT;
			return true;
		}

		//이전 이동 방향과 같다.
		if (_last_direction == DIRECTION::DIRECTION_LEFT) {

			return false;
		}
	}

	//-----------------------------------------------------------------------------------------------
	//현재 사용자 지정 메시지가 오른쪽 움직임인 경우
	//-----------------------------------------------------------------------------------------------
	case ACTION_PLAYER::ACTION_MOVE_RR:
	case ACTION_PLAYER::ACTION_MOVE_RU:
	case ACTION_PLAYER::ACTION_MOVE_RD: {

		//이전 움직임이 왼쪽인 경우
		if (_last_direction == DIRECTION::DIRECTION_LEFT) {

			//이제는 오른쪽 방향으로 이동한다.
			_last_direction = DIRECTION::DIRECTION_RIGHT;
			return true;
		}

		//이전 이동 방향과 같다.
		if (_last_direction == DIRECTION::DIRECTION_RIGHT) {

			return false;
		}
	}
	}

	return true;
}



/**
brief 	사용자 지정 입력 액션 메시지를 처리한다.
*/
BOOL CPlayerObject::OnProcActionMsg() {

	//-----------------------------------------------------------------------------------------------
	//이전 액션 저장한다.
	//-----------------------------------------------------------------------------------------------
	_last_action = _cur_input_action_msg;

	//-----------------------------------------------------------------------------------------------
	//현재 입력된 액션을 저장한다.
	//-----------------------------------------------------------------------------------------------
	ACTION_PLAYER input_cur_action = _cur_input_action_msg;
	int cur_x = _cur_x;
	int cur_y = _cur_y;

	int inc_x = 0;
	int inc_y = 0;

	//-----------------------------------------------------------------------------------------------
	//사용자 지정 입력 메시지에 대한 분기 처리.
	//-----------------------------------------------------------------------------------------------
	switch (input_cur_action)
	{

	//-----------------------------------------------------------------------------------------------
	//왼쪽 이동한 경우.
	//-----------------------------------------------------------------------------------------------
	case ACTION_PLAYER::ACTION_MOVE_LL: {
	
		inc_x -= 3;
		OnActionMove();
		break;
	}

	//-----------------------------------------------------------------------------------------------
	//오른쪽 이동한 경우.
	//-----------------------------------------------------------------------------------------------
	case ACTION_PLAYER::ACTION_MOVE_RR: {

		inc_x += 3;
		OnActionMove();
		break;
	}

	//-----------------------------------------------------------------------------------------------
	//위쪽 이동한 경우.
	//-----------------------------------------------------------------------------------------------
	case ACTION_PLAYER::ACTION_MOVE_UU: {

		inc_y -= 2;
		OnActionMove();
		break;
	}

	//-----------------------------------------------------------------------------------------------
	//아래쪽 이동한 경우.
	//-----------------------------------------------------------------------------------------------
	case ACTION_PLAYER::ACTION_MOVE_DD: {

		inc_y += 2;
		OnActionMove();
		break;
	}

	//-----------------------------------------------------------------------------------------------
	//왼쪽 위 이동한 경우.
	//-----------------------------------------------------------------------------------------------
	case ACTION_PLAYER::ACTION_MOVE_LU: {

		inc_y -= 2;
		inc_x -= 3;
		OnActionMove();
		break;
	}
	//-----------------------------------------------------------------------------------------------
	//오른쪽 위 이동한 경우.
	//-----------------------------------------------------------------------------------------------
	case ACTION_PLAYER::ACTION_MOVE_RU: {

		inc_y -= 2;
		inc_x += 3;
		OnActionMove();
		break;
	}
	//-----------------------------------------------------------------------------------------------
	//오른쪽 아래 이동한 경우.
	//-----------------------------------------------------------------------------------------------
	case ACTION_PLAYER::ACTION_MOVE_RD: {

		inc_y += 2;
		inc_x += 3;
		OnActionMove();
		break;
	}
	//-----------------------------------------------------------------------------------------------
	//왼쪽 아래 이동한 경우.
	//-----------------------------------------------------------------------------------------------
	case ACTION_PLAYER::ACTION_MOVE_LD: {

		inc_y += 2;
		inc_x -= 3;
		OnActionMove();
		break;
	}

	//-----------------------------------------------------------------------------------------------
	//어텍1, 어텍2, 어텍3 통합 처리.
	//-----------------------------------------------------------------------------------------------
	case ACTION_PLAYER::ACTION_ATTACK1: 
	case ACTION_PLAYER::ACTION_ATTACK2: 
	case ACTION_PLAYER::ACTION_ATTACK3: {

		BeginActionAttack();
		break;
	}


	//-----------------------------------------------------------------------------------------------
	//스탠드 자세인 경우.
	//-----------------------------------------------------------------------------------------------
	case ACTION_PLAYER::ACTION_STAND: {

		BeginActionStand();
		break;
	}
	}

	//-----------------------------------------------------------------------------------------------
	//좌표 이동 시 이동 값을 적용한다.
	//-----------------------------------------------------------------------------------------------
	cur_x += inc_x;
	cur_y += inc_y;
	SetCurXY(cur_x, cur_y);

	return true;
}

/**
brief 	액션 무브 시작한다.
*/
void CPlayerObject::BeginActionMove() {

	_is_standing = false;
	_is_moving = true;

	_cur_action = _cur_input_action_msg;
	//-----------------------------------------------------------------------------------------------
	//현재 액션에 대한 분기 처리.
	//-----------------------------------------------------------------------------------------------
	switch (_cur_action){

	//-----------------------------------------------------------------------------------------------
	//왼쪽으로 움직이는 경우.
	//-----------------------------------------------------------------------------------------------
	case ACTION_MOVE_LD:
	case ACTION_MOVE_LL:
	case ACTION_MOVE_LU: {

		//마지막 방향 설정을 왼쪽으로 한다.
		_last_direction = DIRECTION::DIRECTION_LEFT;
		SetSpriteFilmFrom(IDX_SPRITE_PLAYER_FILM::IDX_ACTION_MOVE_L);
		break;
	}
	//-----------------------------------------------------------------------------------------------
	//오른쪽으로 움직이는 경우.
	//-----------------------------------------------------------------------------------------------
	case ACTION_MOVE_RD:
	case ACTION_MOVE_RR:
	case ACTION_MOVE_RU: {

		//마지막 방향 설정을 오른쪽으로 한다.
		_last_direction = DIRECTION::DIRECTION_RIGHT;
		SetSpriteFilmFrom(IDX_SPRITE_PLAYER_FILM::IDX_ACTION_MOVE_R);
		break;
	}
	//-----------------------------------------------------------------------------------------------
	//위 또는 아래로 움직이는 경우.
	//-----------------------------------------------------------------------------------------------
	case ACTION_MOVE_UU:
	case ACTION_MOVE_DD: {

		//이전 방향과 동일하게 스프라이트 애니메이션 및 마지막 방향 설정을 해준다.
		if (_last_direction == DIRECTION::DIRECTION_LEFT) {

			_last_direction = DIRECTION::DIRECTION_LEFT;
			SetSpriteFilmFrom(IDX_SPRITE_PLAYER_FILM::IDX_ACTION_MOVE_L);
		}
		else if (_last_direction == DIRECTION::DIRECTION_RIGHT) {

			_last_direction = DIRECTION::DIRECTION_RIGHT;
			SetSpriteFilmFrom(IDX_SPRITE_PLAYER_FILM::IDX_ACTION_MOVE_R);
		}
		break;
	}
	}
}


/**
brief 	액션 스텐드 시작한다.
*/
void CPlayerObject::BeginActionStand() {

	//이동 상태가 아니다.
	_is_moving = FALSE;

	//공격 상태가 아니다
	_is_attacking = FALSE;

	//움직이는 상태가 아니다.
	_is_stop_moving = TRUE;

	//-----------------------------------------------------------------------------------------------
	//이전에 스텐딩 자세가 아닌지 체크한다.
	//-----------------------------------------------------------------------------------------------
	if (false == _is_standing){

		//진행 액션은 스탠드로 변경된다.
		_cur_action = ACTION_PLAYER::ACTION_STAND;

		//스탠드 상태이다.
		_is_standing = TRUE;

		//-----------------------------------------------------------------------------------------------
		//마지막 방향에 따라 스탠드 방향이 따라간다.
		//-----------------------------------------------------------------------------------------------
		if (_last_direction == DIRECTION::DIRECTION_LEFT) {

			SetSpriteFilmFrom(IDX_SPRITE_PLAYER_FILM::IDX_ACTION_STAND_L);
		}


		else if (_last_direction == DIRECTION::DIRECTION_RIGHT) {

			SetSpriteFilmFrom(IDX_SPRITE_PLAYER_FILM::IDX_ACTION_STAND_R);
		}
	}
}


/**
brief 	액션 어텍 시작한다.
*/
void CPlayerObject::BeginActionAttack() {

	_is_standing = FALSE;

	//이동 상태와 스텐드 상태가 아니다.
	_is_moving = FALSE;

	_is_start_attack = TRUE;
	_is_attacking = TRUE;

	//현재 액션이 어텍 액션으로 변경된다.
	_cur_action = _cur_input_action_msg;

	//-----------------------------------------------------------------------------------------------
	// 마지막 진행 방향에 따라 어텍 방향도 동일하다.
	// 이 경우는 오른쪽 방향이다.
	//-----------------------------------------------------------------------------------------------
	if (_last_direction == DIRECTION::DIRECTION_RIGHT) {

		//어텍 메시지 종류에 따라 분류된다.
		switch (_cur_input_action_msg){

		case ACTION_PLAYER::ACTION_ATTACK1: {

			SetSpriteFilmFrom(IDX_SPRITE_PLAYER_FILM::IDX_ACTION_ATTACK1_R);
			break;
		}
		case ACTION_PLAYER::ACTION_ATTACK2: {

			SetSpriteFilmFrom(IDX_SPRITE_PLAYER_FILM::IDX_ACTION_ATTACK2_R);
			break;
		}

		case ACTION_PLAYER::ACTION_ATTACK3: {

			SetSpriteFilmFrom(IDX_SPRITE_PLAYER_FILM::IDX_ACTION_ATTACK3_R);
			break;
		}

		}

	}
	//-----------------------------------------------------------------------------------------------
	// 마지막 진행 방향에 따라 어텍 방향도 동일하다.
	// 이 경우는 왼쪽 방향이다.
	//-----------------------------------------------------------------------------------------------
	else if (_last_direction == DIRECTION::DIRECTION_LEFT) {

		//어텍 메시지 종류에 따라 분류된다.
		switch (_cur_input_action_msg){

		case ACTION_PLAYER::ACTION_ATTACK1: {

			SetSpriteFilmFrom(IDX_SPRITE_PLAYER_FILM::IDX_ACTION_ATTACK1_L);
			break;
		}
		case ACTION_PLAYER::ACTION_ATTACK2: {

			SetSpriteFilmFrom(IDX_SPRITE_PLAYER_FILM::IDX_ACTION_ATTACK2_L);
			break;
		}

		case ACTION_PLAYER::ACTION_ATTACK3: {

			SetSpriteFilmFrom(IDX_SPRITE_PLAYER_FILM::IDX_ACTION_ATTACK3_L);
			break;
		}
		}
	}
}


/**
brief 	액션 무브 처리한다. 
*/
void CPlayerObject::OnActionMove() {
	//-----------------------------------------------------------------------------------------------
	//계속 이동 여부 체크한다.
	//-----------------------------------------------------------------------------------------------
	if (TRUE == CheckKeepMoving()){

		//-----------------------------------------------------------------------------------------------
		//이동 방향이 바뀌었는지 체크한다.
		//-----------------------------------------------------------------------------------------------
		if (TRUE == CheckChangeDirection()) {

			//-----------------------------------------------------------------------------------------------
			// 이동 방향이 바뀐 경우 이동 스프라이트 방향 또한 바뀌어야 한다.
			// 이 경우 현재 이동 스프라이트는 방향이 바뀐 스프라이트 애니메이션에서도 자연스럽게
			// 이동하는 것으로 설정되어야 한다.
			//-----------------------------------------------------------------------------------------------
			SetSpriteFilmBySymetry();

			return;
		}

		//-----------------------------------------------------------------------------------------------
		//이전과 같은 방향으로 걷는 경우.
		//-----------------------------------------------------------------------------------------------
		else {

			//굳이 바꿀 필요없다.
			return;
		}

	}

	//-----------------------------------------------------------------------------------------------
	// 계속 이동한 경우가 아닌 경우.
	// 이제 무브 시작한다.
	//-----------------------------------------------------------------------------------------------
	BeginActionMove();

	return;
}





void CPlayerObject::InitPlayerDirection(DIRECTION cur_direction) {

	if (cur_direction == DIRECTION::DIRECTION_LEFT)
	{
		//마지막 디렉션 설정
		_last_direction = DIRECTION::DIRECTION_LEFT;

		//스프라이트 애니메이션 설정
		SetSpriteFilmFrom(IDX_SPRITE_PLAYER_FILM::IDX_ACTION_STAND_L);
	}

	if (cur_direction == DIRECTION::DIRECTION_RIGHT)
	{
		//마지막 디렉션 설정
		_last_direction = DIRECTION::DIRECTION_RIGHT;

		//스프라이트 애니메이션 설정
		SetSpriteFilmFrom(IDX_SPRITE_PLAYER_FILM::IDX_ACTION_STAND_R);
	}

}



//닉네임을 설정한다.
void CPlayerObject::SetNickName(CONST WCHAR *nickname) {

	int len = sizeof(_nickname) / sizeof(WCHAR);
	wcscpy_s(_nickname, len, nickname);
	int mpp = 3;
}

int CPlayerObject::GetHp() {

	return _cur_hp;
}

void CPlayerObject::SetHp(int hp) {

	_cur_hp = hp;
}

void CPlayerObject::InitPlayerInfo(int id, int hp, int x, int y, DIRECTION cur_direction, CONST WCHAR *nickname) {

	_id = id;
	_max_hp = hp;
	_cur_hp = hp;
	_cur_x = x;
	_cur_y = y;
	_last_direction = cur_direction;
	SetNickName(nickname);


	_is_start_attack = FALSE;
	_is_stop_moving = FALSE;
	_is_playing = TRUE;
	_pre_input_action_msg = ACTION_PLAYER::ACTION_STAND;
}






////////////////////////////////////////////////////////////////////////////////////////
/**
brief 생성자2.시작 위치 지정한다.
*/
CEffectObject::CEffectObject(int x, int y) {

	//객체 타입 설정
	_type_object = TYPE_OBJECT::OBJECT_EFFECT;

	//시작 위치 지정
	_cur_x = x;
	_cur_y = y;

	//스프라이트 애니메이션 설정
	SetSpriteFilmFrom(IDX_SPRITE_EFFECT::IDX_EFFECT1);

}





/**
brief 액션 진행용 콜백함수이다. 프레임 카운터 및 액션 처리 이전의 전처리 액션 함수를 실행한다.
*/
BOOL  CEffectObject::OnAction(){

	//-----------------------------------------------------------------------------------------------
	//프레임 카운터 업한다.
	//-----------------------------------------------------------------------------------------------
	OnNextFrameByEffect();

	if (TRUE == _is_end_frame)
	{
		return false;
	}
	else {

		return true;
	}
}


