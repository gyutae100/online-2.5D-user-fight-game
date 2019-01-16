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


/////////////////////////////////////////////////////////////////////////////////////////
//	퍼블릭 영역.
/////////////////////////////////////////////////////////////////////////////////////////



/**
@brief 생성자
*/
CBaseObject::CBaseObject() {

	_type_object = TYPE_OBJECT::OBJECT_NULL;
}




/**
@brief 소멸자
*/
CBaseObject::~CBaseObject(){

	//해당 오브젝트 삭제한다.
	///@todo : 메모리 동적할당 해제를 해줄 것!!!
}

/**
@brief 해당 오브젝트 파괴한다. 소멸자 인터페이스이다.
*/
BOOL CBaseObject::DestroyThisObject() {

	//해당 오브젝트 삭제한다.
	delete(this);

	return true;
}

/**
@brief 현재 스프라이트 장면을 반환한다.
*/
SPRITE_PLAYER CBaseObject::GetCurPlayerSprite() {

	return _cur_player_sprite;
}

/**
@brief 현재 스프라이트 장면을 반환한다.
*/
SPRITE_EFFECT CBaseObject::GetCurEffectSprite() {

	return _cur_effect_sprite;
}



/**
@brief 오브젝트 타입을 반환한다.
*/
TYPE_OBJECT CBaseObject::GetObjectType() {

	return _type_object;
}


/**
@brief 좌우 대칭된 스프라이트 애니메이션으로 변경하되 현재 스프라이트 장면은 이전 좌우 대칭된 장면 이후의 값을 적용한다.
	   주로 움직임의 진행 방향이 달라졌을 때 스프라이트 애니메이션이 매끄럽게 이어가지기 위해 이용한다.
*/
//좌우 대칭된 스프라이트로 설정하되 cur_sprite는 +1한 값을 적용한다.
void CBaseObject::SetSpriteFilmBySymetry() {


	//-----------------------------------------------------------------------------------------------
	//현재 스프라이트 저장한다.
	//-----------------------------------------------------------------------------------------------
	SPRITE_PLAYER cur_sprite = _cur_player_sprite;

	//-----------------------------------------------------------------------------------------------
	//이전 스트라이트 필름 정보를 저장한다.
	//-----------------------------------------------------------------------------------------------
	int idx_pre_first_player_sprite = static_cast<int>(_first_player_sprite);
	int idx_pre_max_sprite = static_cast<int>(_last_player_sprite);
	int idx_cur_player_sprite = static_cast<int>(cur_sprite);

	//-----------------------------------------------------------------------------------------------
	//현재 스프라이트 장면 이후의 값의 오프셋을 구한다.
	//-----------------------------------------------------------------------------------------------
	idx_cur_player_sprite++;
	idx_cur_player_sprite = (idx_cur_player_sprite - idx_pre_first_player_sprite) % 12;

	//-----------------------------------------------------------------------------------------------
	// 좌우 대칭되는 반대 방향 스프라이트 애니메이션으로 다시 세팅한다.
	// 이중 현재 스프라이트 장면은 이후 다시 재지정해야 한다.
	// (이유는 방향이 바뀌었을 때 자연스러운 애니메이션 모션을 위해서)
	//-----------------------------------------------------------------------------------------------
	if (_first_player_sprite == SPRITE_PLAYER::PLAYER_MOVE_R_1) {

		SetSpriteFilmFrom(IDX_SPRITE_PLAYER_FILM::IDX_ACTION_MOVE_L);
	}
	else if (_first_player_sprite == SPRITE_PLAYER::PLAYER_MOVE_L_1) {

		SetSpriteFilmFrom(IDX_SPRITE_PLAYER_FILM::IDX_ACTION_MOVE_R);
	}

	//-----------------------------------------------------------------------------------------------
	// 현재 스프라이트 장면은 위에 구한 현재 스프라이트 장면 이후의 오프셋을 이용하여 대칭되는 반대 움직임의
	// 현재 스프라이트 장면을 구한다.
	//-----------------------------------------------------------------------------------------------
	int idx_cur_first_player_sprite = static_cast<int>(_first_player_sprite); //현재 좌우 대칭된 장면의 첫 스프라이트 장면의 시작 인덱스
	idx_cur_player_sprite += idx_cur_first_player_sprite; //오프셋을 더해주어 현재 스프라이트 장면을 변경한다.

	//-----------------------------------------------------------------------------------------------
	// 현재 스프라이트 장면을 다시 저장한다.
	//-----------------------------------------------------------------------------------------------
	_cur_player_sprite = static_cast<SPRITE_PLAYER>(idx_cur_player_sprite);
}


/**
@brief 오버라이딩. 플레이어 스프라이트 필름 단위로 저장한다.

@param idx_cur_player_sprite_PLAYER_FILM :해당 애니메이션 필름 테이블의 인덱스(애니메이션)

@return : 성공 여부
*/
BOOL CBaseObject::SetSpriteFilmFrom(IDX_SPRITE_PLAYER_FILM idx_cur_player_sprite_PLAYER_FILM) {

	//-----------------------------------------------------------------------------------------------
	// 해당 플레이어 애니메이션 필름 테이블의 인덱스를 이용해 시작 스프라이트, 현재 스프라이트, 마지막 스프라이트를 지정한다.
	//-----------------------------------------------------------------------------------------------
	_first_player_sprite = TABLE_SPRITE_PLAYER[idx_cur_player_sprite_PLAYER_FILM]._first_player_sprite;
	_cur_player_sprite = TABLE_SPRITE_PLAYER[idx_cur_player_sprite_PLAYER_FILM]._first_player_sprite;
	_last_player_sprite = TABLE_SPRITE_PLAYER[idx_cur_player_sprite_PLAYER_FILM]._last_player_sprite;

	//-----------------------------------------------------------------------------------------------
	// 각 스프라이트 당 딜레이 프레임 제어에 대한 값을 초기화한다.
	//-----------------------------------------------------------------------------------------------
	_max_delay_each_frame = TABLE_SPRITE_PLAYER[idx_cur_player_sprite_PLAYER_FILM]._max_delay_each_fram;
	_cur_frame = 0;
	_is_end_frame = FALSE;

	return true;
}


/**
@brief 오버라이딩. 이펙트 스프라이트 필름 단위로 저장한다.

@param idx_cur_player_sprite_PLAYER_FILM :해당 애니메이션 필름 테이블의 인덱스(애니메이션)

@return : 성공 여부
*/
BOOL CBaseObject::SetSpriteFilmFrom(IDX_SPRITE_EFFECT idx_cur_player_sprite_PLAYER_FILM) {

	//-----------------------------------------------------------------------------------------------
	// 해당 플레이어 애니메이션 필름 테이블의 인덱스를 이용해 시작 스프라이트, 현재 스프라이트, 마지막 스프라이트를 지정한다.
	//-----------------------------------------------------------------------------------------------
	_first_effect_sprite = TABLE_SPRITE_EFFECT[idx_cur_player_sprite_PLAYER_FILM]._first_player_sprite;
	_cur_effect_sprite = TABLE_SPRITE_EFFECT[idx_cur_player_sprite_PLAYER_FILM]._first_player_sprite;
	_last_effect_sprite = TABLE_SPRITE_EFFECT[idx_cur_player_sprite_PLAYER_FILM]._last_player_sprite;

	//-----------------------------------------------------------------------------------------------
	// 각 스프라이트 당 딜레이 프레임 제어에 대한 값을 초기화한다.
	//-----------------------------------------------------------------------------------------------
	_max_delay_each_frame = TABLE_SPRITE_EFFECT[idx_cur_player_sprite_PLAYER_FILM]._max_delay_each_fram;
	_cur_frame = 0;
	_is_end_frame = FALSE;

	return true;
}



/**
@brief 오버라이딩 된 SetSpriteFilmFrom(IDX_SPRITE_PLAYER_FILM idx_cur_player_sprite_PLAYER_FILM)이 이 함수를 대체한다.
*/
BOOL CBaseObject::SetSpriteFilmFrom(SPRITE_PLAYER first_sprite, SPRITE_PLAYER last_sprite, int delay_last_frame) {

	if (SPRITE_PLAYER::PLAYER_NULL > first_sprite || SPRITE_PLAYER::PLAYER_END <= first_sprite) {

		return false;
	}

	if (SPRITE_PLAYER::PLAYER_NULL > last_sprite || SPRITE_PLAYER::PLAYER_END <= last_sprite) {

		return false;
	}

	_first_player_sprite = first_sprite;
	_cur_player_sprite = first_sprite;
	_last_player_sprite = last_sprite;


	_max_delay_each_frame = delay_last_frame;
	_cur_frame = 0;
	_is_end_frame = FALSE;
}


/**
@brief 다음 프레임을 카운트한다.
*/
BOOL CBaseObject::OnNextFrame(){

	//-----------------------------------------------------------------------------------------------
	// 현재 스프라이트 장면이 없거나 혹은 인덱스 범위를 벗어났는지 체크한다.
	//-----------------------------------------------------------------------------------------------
	if (SPRITE_PLAYER::PLAYER_NULL > _cur_player_sprite || SPRITE_PLAYER::PLAYER_END <= _cur_player_sprite) {

		return FALSE;
	}

	//-----------------------------------------------------------------------------------------------
	// 프레임 카운트 한다.
	//-----------------------------------------------------------------------------------------------
	_cur_frame++;

	//-----------------------------------------------------------------------------------------------
	// 프레임 카운트 값이 각 애니메이션의 딜레이 프레임보다 큰 경우 다음 애니메이션으로 넘기고
	// 프레임 카운트 값은 0으로 초기화한다.
	//-----------------------------------------------------------------------------------------------
	if (_cur_frame >= _max_delay_each_frame) {

		
		_cur_frame = 0;

		int num_sprite = static_cast<int>(_cur_player_sprite);
		num_sprite++;
		SPRITE_PLAYER cur_sprite = static_cast<SPRITE_PLAYER>(num_sprite);

		_cur_player_sprite = cur_sprite;

		//-----------------------------------------------------------------------------------------------
		// 현재 스프라이트 장면이 마지막 스프라이트 장면을 넘어섰는지 체크한다.
		// 만일 넘은 경우 is_end_frame으로 체크해준다. (일회성 애니메이션인 경우 애니메이션이 끝났음을 알린다.)
		// 일회성 애니메이션 동작이 아닌 동작이 이어지는 스프라이트 애니메이션(걷기)같은 경우나 스탠드 액션인 경우
		// 다시 원래 스프라이트 애니메이션을 반복해야 한다.
		//-----------------------------------------------------------------------------------------------
		if (cur_sprite > _last_player_sprite) {

			_cur_player_sprite = _first_player_sprite;
			_is_end_frame = TRUE;
		}
	}

	return TRUE;
}



/**
@brief 다음 프레임을 카운트한다.
*/
BOOL CBaseObject::OnNextFrameByEffect() {


	//-----------------------------------------------------------------------------------------------
	// 프레임 카운트 한다.
	//-----------------------------------------------------------------------------------------------
	_cur_frame++;

	//-----------------------------------------------------------------------------------------------
	// 프레임 카운트 값이 각 애니메이션의 딜레이 프레임보다 큰 경우 다음 애니메이션으로 넘기고
	// 프레임 카운트 값은 0으로 초기화한다.
	//-----------------------------------------------------------------------------------------------
	if (_cur_frame >= _max_delay_each_frame) {


		_cur_frame = 0;

		int num_effect = static_cast<int>(_cur_effect_sprite);
		num_effect++;
		SPRITE_EFFECT cur_sprite = static_cast<SPRITE_EFFECT>(num_effect);

		_cur_effect_sprite = cur_sprite;

		//-----------------------------------------------------------------------------------------------
		// 현재 스프라이트 장면이 마지막 스프라이트 장면을 넘어섰는지 체크한다.
		// 만일 넘은 경우 is_end_frame으로 체크해준다. (일회성 애니메이션인 경우 애니메이션이 끝났음을 알린다.)
		// 일회성 애니메이션 동작이 아닌 동작이 이어지는 스프라이트 애니메이션(걷기)같은 경우나 스탠드 액션인 경우 
		// 다시 원래 스프라이트 애니메이션을 반복해야 한다.
		//-----------------------------------------------------------------------------------------------
		if (_cur_effect_sprite > _last_effect_sprite) {

			_is_end_frame = TRUE;
		}
	}
	return TRUE;
}


/**
@brief 마지막 프레임 여부를 확인한다. 스프라이트 애니메이션이 1회성인 경우(예: 공격)시 동작이 반복되지 않도록 끊어주는 역활을 한다.
*/
BOOL CBaseObject::IsEndSpriteFilm(){

	if (TRUE == _is_end_frame) {

		return true;
	}
	else {

		return false;
	}
}



/**
@brief 좌표 xy를 반환한다.
*/
void CBaseObject::GetCurXY(int *out_cur_x, int *out_cur_y) {
	
	*out_cur_x = _cur_x;
	*out_cur_y = _cur_y;
}


/**
@brief 좌표 xy를 설정한다.
*/
void CBaseObject::SetCurXY(int cur_x, int cur_y) {
	
	_cur_x = cur_x;
	_cur_y = cur_y;
}
