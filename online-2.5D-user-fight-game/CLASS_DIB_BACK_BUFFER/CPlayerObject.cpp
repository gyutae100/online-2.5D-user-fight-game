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
brief ������1
*/
CPlayerObject::CPlayerObject(){

	_is_main_player = FALSE;

	_is_attacking = FALSE;
	_is_stop_moving = FALSE;
	_is_start_attack = FALSE;

	_is_playing = FALSE;

	//��ü Ÿ�� ����
	_type_object = TYPE_OBJECT::OBJECT_PLAYER;

	//�ʱ� ���� ���۽� ��������Ʈ 
	_cur_player_sprite = SPRITE_PLAYER::PLAYER_STAND_L_1;

	//������ �𷺼� ����
	_last_direction = DIRECTION::DIRECTION_LEFT;

	//��������Ʈ�ִϸ��̼� ����
	SetSpriteFilmFrom(IDX_SPRITE_PLAYER_FILM::IDX_ACTION_STAND_L);

	//�⺻ ���� �׼�
	_cur_action = ACTION_PLAYER::ACTION_STAND;
	
	_pre_input_action_msg = ACTION_PLAYER::ACTION_STAND;
}


/**
brief ������2. ���� ��ġ �����Ѵ�.
*/
CPlayerObject::CPlayerObject(int x, int y) {

	_is_main_player = FALSE;

	_is_start_attack = FALSE;
	_is_stop_moving = FALSE;
	_is_playing = FALSE;

	//��ü Ÿ�� ����
	_type_object = TYPE_OBJECT::OBJECT_PLAYER;

	//���� ��ġ ����
	_cur_x = x;
	_cur_y = y;


	//�ʱ� ���� ���۽� ��������Ʈ
	_cur_player_sprite = SPRITE_PLAYER::PLAYER_STAND_L_1;

	//������ �𷺼� ����
	_last_direction = DIRECTION::DIRECTION_LEFT;

	//��������Ʈ �ִϸ��̼� ����
	SetSpriteFilmFrom(IDX_SPRITE_PLAYER_FILM::IDX_ACTION_STAND_L);

	//�⺻ ���� �׼�
	_cur_action = ACTION_PLAYER::ACTION_STAND;

	_pre_input_action_msg = ACTION_PLAYER::ACTION_STAND;
}

/**
brief �Ҹ���
*/
CPlayerObject::~CPlayerObject() {

}


/**
brief ����� �Է� �׼� ó�� �޽����� �����Ѵ�.
*/
void CPlayerObject::SaveCurActionMsg(ACTION_PLAYER cur_input_action) {

	//���� BOOL ��ȯ �� �� 
	if (cur_input_action < 0) {
		return;
	}

	//�����ִ� ���� ���� ����.
	if (_is_moving == TRUE && cur_input_action == ACTION_PLAYER::ACTION_STAND) {

		_is_stop_moving = TRUE;
	}
	else {
	
		_is_stop_moving = FALSE;
	}


	//-----------------------------------------------------------------------------------------------
	// ���� ��ư�� �������� ������ ��� �߰��߰� ���ٵ� ��ü�� �ѹ� �Ϸ� �� ������ �Ǿ�� �Ѵ�.
	//-----------------------------------------------------------------------------------------------
	if ( FALSE ==_is_end_frame && TRUE == CheckIsAttackAction( cur_input_action) &&TRUE == CheckIsAttackAction(_pre_input_action_msg)) {

		return;
	}
	



	_pre_input_action_msg = _cur_input_action_msg;
	_cur_input_action_msg = cur_input_action;
}

/**
brief �׼� ����� �ݹ��Լ��̴�. ������ ī���� �� �׼� ó�� ������ ��ó�� �׼� �Լ��� �����Ѵ�. 
*/
BOOL  CPlayerObject::OnAction()
{
	//-----------------------------------------------------------------------------------------------
	//������ ī���� ���Ѵ�.
	//-----------------------------------------------------------------------------------------------
	OnNextFrame();

	//-----------------------------------------------------------------------------------------------
	//�׼� ó�� �޽��� ���͸��Ѵ�.
	//-----------------------------------------------------------------------------------------------
	if (TRUE == OnFilteringActionMsg()) {

		//-----------------------------------------------------------------------------------------------
		//�׼� �޽��� ó���� �����Ѵ�.
		//-----------------------------------------------------------------------------------------------
		OnProcActionMsg();
	}

	return true;
}


/**
brief 	����� �Է� �׼� �޼����� ���� ��ó��(���͸�) ������ �����Ѵ�.
		����ڰ� ���� ���� �׼� �� ����� �ȵǴ� �׼�(�� ����)���� �׼��� ��� ����� �Է� �׼� �޽����� �����ϴ� ó���� ����Ѵ�.
		����� �׼��� 1ȸ�� �׼��� ��� �׼��� ���� �� �⺻ �ڼ��� ���ٵ� �ڼ��� ���ư��� �Ѵ�.
*/
BOOL CPlayerObject:: OnFilteringActionMsg(void){

	switch (_cur_action){

	//-----------------------------------------------------------------------------------------------
	// ���� ������ �Է� Ű���� �켱������ ����.
	// ���� �޽����� ������ ��������Ʈ ����̸鼭 ������ ������ ī�����̱� ������ 
	// ��� ��������Ʈ �ִϸ��̼��� ����Ǿ�� �Ѵ�.
	//-----------------------------------------------------------------------------------------------
	case ACTION_PLAYER::ACTION_ATTACK1:
	case ACTION_PLAYER::ACTION_ATTACK2:
	case ACTION_PLAYER::ACTION_ATTACK3:

		//-----------------------------------------------------------------------------------------------
		//��������Ʈ �ִϸ��̼��� ���� ���θ� üũ�Ѵ�.
		//-----------------------------------------------------------------------------------------------
		if (TRUE == IsEndSpriteFilm() ){

			//-----------------------------------------------------------------------------------------------
			//���� �׼��� �׼��� ���� �� ���ٵ� �ڼ��� �����Ѵ�. (�⺻ �ڼ�)
			//-----------------------------------------------------------------------------------------------
			_cur_input_action_msg = ACTION_PLAYER::ACTION_STAND;
			return TRUE;
		}
		return FALSE;
	}

	return true;
}


/**
brief 	���� ��ü�� �ȴ� ��� ���¿����� üũ�Ѵ�.
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
brief 	���� ������ ������ �ٸ��� ���ߴ��� üũ�Ѵ�.

return : ������ ���� �� ��� TRUE, �ƴ� ��� FALSE�� ��ȯ
*/
BOOL CPlayerObject::CheckChangeDirection(){

	//-----------------------------------------------------------------------------------------------
	//���� �̵� ������ ������ �ȴ� ����� ������ üũ�Ѵ�.
	//-----------------------------------------------------------------------------------------------
	switch (_cur_input_action_msg) {

	//-----------------------------------------------------------------------------------------------
	//��, �Ʒ� �̵��� ���� ����� �����ϴ�.
	//-----------------------------------------------------------------------------------------------
	case ACTION_PLAYER::ACTION_MOVE_UU:
	case ACTION_PLAYER::ACTION_MOVE_DD: {

		return false;
	}

	//-----------------------------------------------------------------------------------------------
	//���� ����� ���� �޽����� ���� �������� ���
	//-----------------------------------------------------------------------------------------------
	case ACTION_PLAYER::ACTION_MOVE_LL:
	case ACTION_PLAYER::ACTION_MOVE_LU:
	case ACTION_PLAYER::ACTION_MOVE_LD: {

		//-----------------------------------------------------------------------------------------------
		//���� �������� �������� ���
		//�̵� �������� �ٲ����.
		//-----------------------------------------------------------------------------------------------
		if (_last_direction == DIRECTION::DIRECTION_RIGHT) {

			//������ ���ʹ������� �̵��Ѵ�.
			_last_direction = DIRECTION::DIRECTION_LEFT;
			return true;
		}

		//���� �̵� ����� ����.
		if (_last_direction == DIRECTION::DIRECTION_LEFT) {

			return false;
		}
	}

	//-----------------------------------------------------------------------------------------------
	//���� ����� ���� �޽����� ������ �������� ���
	//-----------------------------------------------------------------------------------------------
	case ACTION_PLAYER::ACTION_MOVE_RR:
	case ACTION_PLAYER::ACTION_MOVE_RU:
	case ACTION_PLAYER::ACTION_MOVE_RD: {

		//���� �������� ������ ���
		if (_last_direction == DIRECTION::DIRECTION_LEFT) {

			//������ ������ �������� �̵��Ѵ�.
			_last_direction = DIRECTION::DIRECTION_RIGHT;
			return true;
		}

		//���� �̵� ����� ����.
		if (_last_direction == DIRECTION::DIRECTION_RIGHT) {

			return false;
		}
	}
	}

	return true;
}



/**
brief 	����� ���� �Է� �׼� �޽����� ó���Ѵ�.
*/
BOOL CPlayerObject::OnProcActionMsg() {

	//-----------------------------------------------------------------------------------------------
	//���� �׼� �����Ѵ�.
	//-----------------------------------------------------------------------------------------------
	_last_action = _cur_input_action_msg;

	//-----------------------------------------------------------------------------------------------
	//���� �Էµ� �׼��� �����Ѵ�.
	//-----------------------------------------------------------------------------------------------
	ACTION_PLAYER input_cur_action = _cur_input_action_msg;
	int cur_x = _cur_x;
	int cur_y = _cur_y;

	int inc_x = 0;
	int inc_y = 0;

	//-----------------------------------------------------------------------------------------------
	//����� ���� �Է� �޽����� ���� �б� ó��.
	//-----------------------------------------------------------------------------------------------
	switch (input_cur_action)
	{

	//-----------------------------------------------------------------------------------------------
	//���� �̵��� ���.
	//-----------------------------------------------------------------------------------------------
	case ACTION_PLAYER::ACTION_MOVE_LL: {
	
		inc_x -= 3;
		OnActionMove();
		break;
	}

	//-----------------------------------------------------------------------------------------------
	//������ �̵��� ���.
	//-----------------------------------------------------------------------------------------------
	case ACTION_PLAYER::ACTION_MOVE_RR: {

		inc_x += 3;
		OnActionMove();
		break;
	}

	//-----------------------------------------------------------------------------------------------
	//���� �̵��� ���.
	//-----------------------------------------------------------------------------------------------
	case ACTION_PLAYER::ACTION_MOVE_UU: {

		inc_y -= 2;
		OnActionMove();
		break;
	}

	//-----------------------------------------------------------------------------------------------
	//�Ʒ��� �̵��� ���.
	//-----------------------------------------------------------------------------------------------
	case ACTION_PLAYER::ACTION_MOVE_DD: {

		inc_y += 2;
		OnActionMove();
		break;
	}

	//-----------------------------------------------------------------------------------------------
	//���� �� �̵��� ���.
	//-----------------------------------------------------------------------------------------------
	case ACTION_PLAYER::ACTION_MOVE_LU: {

		inc_y -= 2;
		inc_x -= 3;
		OnActionMove();
		break;
	}
	//-----------------------------------------------------------------------------------------------
	//������ �� �̵��� ���.
	//-----------------------------------------------------------------------------------------------
	case ACTION_PLAYER::ACTION_MOVE_RU: {

		inc_y -= 2;
		inc_x += 3;
		OnActionMove();
		break;
	}
	//-----------------------------------------------------------------------------------------------
	//������ �Ʒ� �̵��� ���.
	//-----------------------------------------------------------------------------------------------
	case ACTION_PLAYER::ACTION_MOVE_RD: {

		inc_y += 2;
		inc_x += 3;
		OnActionMove();
		break;
	}
	//-----------------------------------------------------------------------------------------------
	//���� �Ʒ� �̵��� ���.
	//-----------------------------------------------------------------------------------------------
	case ACTION_PLAYER::ACTION_MOVE_LD: {

		inc_y += 2;
		inc_x -= 3;
		OnActionMove();
		break;
	}

	//-----------------------------------------------------------------------------------------------
	//����1, ����2, ����3 ���� ó��.
	//-----------------------------------------------------------------------------------------------
	case ACTION_PLAYER::ACTION_ATTACK1: 
	case ACTION_PLAYER::ACTION_ATTACK2: 
	case ACTION_PLAYER::ACTION_ATTACK3: {

		BeginActionAttack();
		break;
	}


	//-----------------------------------------------------------------------------------------------
	//���ĵ� �ڼ��� ���.
	//-----------------------------------------------------------------------------------------------
	case ACTION_PLAYER::ACTION_STAND: {

		BeginActionStand();
		break;
	}
	}

	//-----------------------------------------------------------------------------------------------
	//��ǥ �̵� �� �̵� ���� �����Ѵ�.
	//-----------------------------------------------------------------------------------------------
	cur_x += inc_x;
	cur_y += inc_y;
	SetCurXY(cur_x, cur_y);

	return true;
}

/**
brief 	�׼� ���� �����Ѵ�.
*/
void CPlayerObject::BeginActionMove() {

	_is_standing = false;
	_is_moving = true;

	_cur_action = _cur_input_action_msg;
	//-----------------------------------------------------------------------------------------------
	//���� �׼ǿ� ���� �б� ó��.
	//-----------------------------------------------------------------------------------------------
	switch (_cur_action){

	//-----------------------------------------------------------------------------------------------
	//�������� �����̴� ���.
	//-----------------------------------------------------------------------------------------------
	case ACTION_MOVE_LD:
	case ACTION_MOVE_LL:
	case ACTION_MOVE_LU: {

		//������ ���� ������ �������� �Ѵ�.
		_last_direction = DIRECTION::DIRECTION_LEFT;
		SetSpriteFilmFrom(IDX_SPRITE_PLAYER_FILM::IDX_ACTION_MOVE_L);
		break;
	}
	//-----------------------------------------------------------------------------------------------
	//���������� �����̴� ���.
	//-----------------------------------------------------------------------------------------------
	case ACTION_MOVE_RD:
	case ACTION_MOVE_RR:
	case ACTION_MOVE_RU: {

		//������ ���� ������ ���������� �Ѵ�.
		_last_direction = DIRECTION::DIRECTION_RIGHT;
		SetSpriteFilmFrom(IDX_SPRITE_PLAYER_FILM::IDX_ACTION_MOVE_R);
		break;
	}
	//-----------------------------------------------------------------------------------------------
	//�� �Ǵ� �Ʒ��� �����̴� ���.
	//-----------------------------------------------------------------------------------------------
	case ACTION_MOVE_UU:
	case ACTION_MOVE_DD: {

		//���� ����� �����ϰ� ��������Ʈ �ִϸ��̼� �� ������ ���� ������ ���ش�.
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
brief 	�׼� ���ٵ� �����Ѵ�.
*/
void CPlayerObject::BeginActionStand() {

	//�̵� ���°� �ƴϴ�.
	_is_moving = FALSE;

	//���� ���°� �ƴϴ�
	_is_attacking = FALSE;

	//�����̴� ���°� �ƴϴ�.
	_is_stop_moving = TRUE;

	//-----------------------------------------------------------------------------------------------
	//������ ���ٵ� �ڼ��� �ƴ��� üũ�Ѵ�.
	//-----------------------------------------------------------------------------------------------
	if (false == _is_standing){

		//���� �׼��� ���ĵ�� ����ȴ�.
		_cur_action = ACTION_PLAYER::ACTION_STAND;

		//���ĵ� �����̴�.
		_is_standing = TRUE;

		//-----------------------------------------------------------------------------------------------
		//������ ���⿡ ���� ���ĵ� ������ ���󰣴�.
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
brief 	�׼� ���� �����Ѵ�.
*/
void CPlayerObject::BeginActionAttack() {

	_is_standing = FALSE;

	//�̵� ���¿� ���ٵ� ���°� �ƴϴ�.
	_is_moving = FALSE;

	_is_start_attack = TRUE;
	_is_attacking = TRUE;

	//���� �׼��� ���� �׼����� ����ȴ�.
	_cur_action = _cur_input_action_msg;

	//-----------------------------------------------------------------------------------------------
	// ������ ���� ���⿡ ���� ���� ���⵵ �����ϴ�.
	// �� ���� ������ �����̴�.
	//-----------------------------------------------------------------------------------------------
	if (_last_direction == DIRECTION::DIRECTION_RIGHT) {

		//���� �޽��� ������ ���� �з��ȴ�.
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
	// ������ ���� ���⿡ ���� ���� ���⵵ �����ϴ�.
	// �� ���� ���� �����̴�.
	//-----------------------------------------------------------------------------------------------
	else if (_last_direction == DIRECTION::DIRECTION_LEFT) {

		//���� �޽��� ������ ���� �з��ȴ�.
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
brief 	�׼� ���� ó���Ѵ�. 
*/
void CPlayerObject::OnActionMove() {
	//-----------------------------------------------------------------------------------------------
	//��� �̵� ���� üũ�Ѵ�.
	//-----------------------------------------------------------------------------------------------
	if (TRUE == CheckKeepMoving()){

		//-----------------------------------------------------------------------------------------------
		//�̵� ������ �ٲ������ üũ�Ѵ�.
		//-----------------------------------------------------------------------------------------------
		if (TRUE == CheckChangeDirection()) {

			//-----------------------------------------------------------------------------------------------
			// �̵� ������ �ٲ� ��� �̵� ��������Ʈ ���� ���� �ٲ��� �Ѵ�.
			// �� ��� ���� �̵� ��������Ʈ�� ������ �ٲ� ��������Ʈ �ִϸ��̼ǿ����� �ڿ�������
			// �̵��ϴ� ������ �����Ǿ�� �Ѵ�.
			//-----------------------------------------------------------------------------------------------
			SetSpriteFilmBySymetry();

			return;
		}

		//-----------------------------------------------------------------------------------------------
		//������ ���� �������� �ȴ� ���.
		//-----------------------------------------------------------------------------------------------
		else {

			//���� �ٲ� �ʿ����.
			return;
		}

	}

	//-----------------------------------------------------------------------------------------------
	// ��� �̵��� ��찡 �ƴ� ���.
	// ���� ���� �����Ѵ�.
	//-----------------------------------------------------------------------------------------------
	BeginActionMove();

	return;
}





void CPlayerObject::InitPlayerDirection(DIRECTION cur_direction) {

	if (cur_direction == DIRECTION::DIRECTION_LEFT)
	{
		//������ �𷺼� ����
		_last_direction = DIRECTION::DIRECTION_LEFT;

		//��������Ʈ �ִϸ��̼� ����
		SetSpriteFilmFrom(IDX_SPRITE_PLAYER_FILM::IDX_ACTION_STAND_L);
	}

	if (cur_direction == DIRECTION::DIRECTION_RIGHT)
	{
		//������ �𷺼� ����
		_last_direction = DIRECTION::DIRECTION_RIGHT;

		//��������Ʈ �ִϸ��̼� ����
		SetSpriteFilmFrom(IDX_SPRITE_PLAYER_FILM::IDX_ACTION_STAND_R);
	}

}



//�г����� �����Ѵ�.
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
brief ������2.���� ��ġ �����Ѵ�.
*/
CEffectObject::CEffectObject(int x, int y) {

	//��ü Ÿ�� ����
	_type_object = TYPE_OBJECT::OBJECT_EFFECT;

	//���� ��ġ ����
	_cur_x = x;
	_cur_y = y;

	//��������Ʈ �ִϸ��̼� ����
	SetSpriteFilmFrom(IDX_SPRITE_EFFECT::IDX_EFFECT1);

}





/**
brief �׼� ����� �ݹ��Լ��̴�. ������ ī���� �� �׼� ó�� ������ ��ó�� �׼� �Լ��� �����Ѵ�.
*/
BOOL  CEffectObject::OnAction(){

	//-----------------------------------------------------------------------------------------------
	//������ ī���� ���Ѵ�.
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


