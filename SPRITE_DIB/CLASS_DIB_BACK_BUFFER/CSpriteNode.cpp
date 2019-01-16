#pragma once
#include "stdafx.h"

#include "lib_CSPRITE_DIB.h"
#include "lib_CSCREEN_DIB.h"

#include "df_sprite_setting.h"
#include "CFrameSkip.h"
#include "CBaseObject.h"
#include "CPlayerObject.h"
#include "CSpriteNode.h"

/**
brief 생성자
*/
CSpriteNode::CSpriteNode(CBaseObject *target_object, TYPE_OBJECT type_object){
	
	if (TRUE == target_object->_is_main_player) {

		_is_main_player = TRUE;
	}
	else {

		_is_main_player = FALSE;
	}

		int cur_x = 0;
		int cur_y = 0;

		int inc_x = 0;
		int inc_y = 0;

		//현재 오브젝트 주소를 저장한다.
		_cur_object_addr = target_object;


		//현재 좌표 x,y 저장한다.
		target_object->GetCurXY(&cur_x, &cur_y);

		_use_destroy = false;

		//플레이어인 경우
		if (type_object == TYPE_OBJECT::OBJECT_NULL) {

			_depth_attach_priority = DEPTH_PRIORITY::PRIORITY_3RD;

			//현재 오브젝트 주소를 저장한다.
			_cur_object_addr = target_object;

			//타입 지정한다.
			_type_object = target_object->GetObjectType();

			if (_type_object == TYPE_OBJECT::OBJECT_PLAYER) {
				_is_player_sprite = true;

				_cur_player_sprite = target_object->GetCurPlayerSprite();

				_cur_x = cur_x;
				_cur_y = cur_y;
			}

			return;
		}


		if (type_object == TYPE_OBJECT::OBJECT_HP) {

			_depth_attach_priority = DEPTH_PRIORITY::PRIORITY_2ND;
			_cur_fix_sprite = SPRITE_FIX::ONCE_HP;
			inc_x = 35;
			inc_y = -30;


		}

		else if (type_object == TYPE_OBJECT::OBJECT_SHADOW) {

			_depth_attach_priority = DEPTH_PRIORITY::PRIORITY_3RD;
			_cur_fix_sprite = SPRITE_FIX::ONCE_SHADOW;
			inc_x = 0;
			inc_y = 0;
		}

		else if (type_object == TYPE_OBJECT::OBJECT_NICKNAME) {

			_depth_attach_priority = DEPTH_PRIORITY::PRIORITY_1ST;
			_cur_fix_sprite = SPRITE_FIX::ONCE_NICKNAME;
			wcscpy_s(_nickname, sizeof(_nickname)/sizeof(WCHAR), target_object->_nickname);
			inc_x = 0;
			inc_y = 0;
			int tmp = 3;

		}

		else if (type_object == TYPE_OBJECT::OBJECT_EFFECT) {

			_depth_attach_priority = DEPTH_PRIORITY::PRIORITY_1ST;
			_cur_effect_sprite = SPRITE_EFFECT::EFFECT_XSPARK_1;

			_is_effect_sprite = TRUE;
			_type_object = type_object;

			int cur_x;
			int cur_y;
			DIRECTION cur_direction;
			target_object->GetCurXY(&cur_x, &cur_y);
			cur_direction = target_object->_last_direction;

			if (TRUE == target_object->_is_main_player )
			{
				_is_main_client = TRUE;
			}
			else {

				_is_main_client = FALSE;
			}
		
			if (cur_direction == DIRECTION::DIRECTION_LEFT) {

				_cur_object_addr = new CEffectObject(cur_x-55, cur_y-60);
			}
			else {


				_cur_object_addr = new CEffectObject(cur_x + 55, cur_y - 60);
			}
			return;
		}


		_type_object = type_object;

		//hp. shadow인 경우
		_is_fix_sprite = true;

		_cur_x = cur_x + inc_x;
		_cur_y = cur_y + inc_y;

}


/**
brief 스프라이트 노드를 업데이트한다.
*/
BOOL CSpriteNode::UpdateSpriteNode() {

	///@todo : 추후 _use_destroy를 체크해준다.

	//현재 플레이어 오브젝트의 위치값.
	int cur_x = 0;
	int cur_y = 0;

	//스프라이트 노드의 위치 값.
	int inc_x = 0;
	int inc_y = 0;

	//함수 바깥에서 삭제해준다.
	if (TRUE == _use_destroy) {

		return false;
	}

	//좌표를 재설정해준다.
	//모든 스프라이트 노드는 좌표가 플레이어 기준으로 변경된다.
	_cur_object_addr->GetCurXY(&cur_x, &cur_y);

	if (_type_object == TYPE_OBJECT::OBJECT_SHADOW) {

		//보정 위치값.
		inc_x = 0;
		inc_y = 0;
	}

	else if (_type_object == TYPE_OBJECT::OBJECT_HP) {

		//보정 위치값.
		inc_x = 0;
		inc_y = 0;
		_option_value1 = _cur_object_addr->GetHp();
	}
	else if (_type_object == TYPE_OBJECT::OBJECT_PLAYER) {

		//보정 위치값.
		inc_x = 0;
		inc_y = 0;
		_cur_player_sprite = _cur_object_addr->GetCurPlayerSprite();
	}

	else if (_type_object == TYPE_OBJECT::OBJECT_EFFECT) {

		_cur_effect_sprite = _cur_object_addr->GetCurEffectSprite();

	}

	else if (_type_object == TYPE_OBJECT::OBJECT_NICKNAME) {

		_direction = _cur_object_addr->_last_direction;

	}


	_cur_x = cur_x;
	_cur_y = cur_y;

	return true;
}