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
//	�ۺ� ����.
/////////////////////////////////////////////////////////////////////////////////////////



/**
@brief ������
*/
CBaseObject::CBaseObject() {

	_type_object = TYPE_OBJECT::OBJECT_NULL;
}




/**
@brief �Ҹ���
*/
CBaseObject::~CBaseObject(){

	//�ش� ������Ʈ �����Ѵ�.
	///@todo : �޸� �����Ҵ� ������ ���� ��!!!
}

/**
@brief �ش� ������Ʈ �ı��Ѵ�. �Ҹ��� �������̽��̴�.
*/
BOOL CBaseObject::DestroyThisObject() {

	//�ش� ������Ʈ �����Ѵ�.
	delete(this);

	return true;
}

/**
@brief ���� ��������Ʈ ����� ��ȯ�Ѵ�.
*/
SPRITE_PLAYER CBaseObject::GetCurPlayerSprite() {

	return _cur_player_sprite;
}

/**
@brief ���� ��������Ʈ ����� ��ȯ�Ѵ�.
*/
SPRITE_EFFECT CBaseObject::GetCurEffectSprite() {

	return _cur_effect_sprite;
}



/**
@brief ������Ʈ Ÿ���� ��ȯ�Ѵ�.
*/
TYPE_OBJECT CBaseObject::GetObjectType() {

	return _type_object;
}


/**
@brief �¿� ��Ī�� ��������Ʈ �ִϸ��̼����� �����ϵ� ���� ��������Ʈ ����� ���� �¿� ��Ī�� ��� ������ ���� �����Ѵ�.
	   �ַ� �������� ���� ������ �޶����� �� ��������Ʈ �ִϸ��̼��� �Ų����� �̾���� ���� �̿��Ѵ�.
*/
//�¿� ��Ī�� ��������Ʈ�� �����ϵ� cur_sprite�� +1�� ���� �����Ѵ�.
void CBaseObject::SetSpriteFilmBySymetry() {


	//-----------------------------------------------------------------------------------------------
	//���� ��������Ʈ �����Ѵ�.
	//-----------------------------------------------------------------------------------------------
	SPRITE_PLAYER cur_sprite = _cur_player_sprite;

	//-----------------------------------------------------------------------------------------------
	//���� ��Ʈ����Ʈ �ʸ� ������ �����Ѵ�.
	//-----------------------------------------------------------------------------------------------
	int idx_pre_first_player_sprite = static_cast<int>(_first_player_sprite);
	int idx_pre_max_sprite = static_cast<int>(_last_player_sprite);
	int idx_cur_player_sprite = static_cast<int>(cur_sprite);

	//-----------------------------------------------------------------------------------------------
	//���� ��������Ʈ ��� ������ ���� �������� ���Ѵ�.
	//-----------------------------------------------------------------------------------------------
	idx_cur_player_sprite++;
	idx_cur_player_sprite = (idx_cur_player_sprite - idx_pre_first_player_sprite) % 12;

	//-----------------------------------------------------------------------------------------------
	// �¿� ��Ī�Ǵ� �ݴ� ���� ��������Ʈ �ִϸ��̼����� �ٽ� �����Ѵ�.
	// ���� ���� ��������Ʈ ����� ���� �ٽ� �������ؾ� �Ѵ�.
	// (������ ������ �ٲ���� �� �ڿ������� �ִϸ��̼� ����� ���ؼ�)
	//-----------------------------------------------------------------------------------------------
	if (_first_player_sprite == SPRITE_PLAYER::PLAYER_MOVE_R_1) {

		SetSpriteFilmFrom(IDX_SPRITE_PLAYER_FILM::IDX_ACTION_MOVE_L);
	}
	else if (_first_player_sprite == SPRITE_PLAYER::PLAYER_MOVE_L_1) {

		SetSpriteFilmFrom(IDX_SPRITE_PLAYER_FILM::IDX_ACTION_MOVE_R);
	}

	//-----------------------------------------------------------------------------------------------
	// ���� ��������Ʈ ����� ���� ���� ���� ��������Ʈ ��� ������ �������� �̿��Ͽ� ��Ī�Ǵ� �ݴ� ��������
	// ���� ��������Ʈ ����� ���Ѵ�.
	//-----------------------------------------------------------------------------------------------
	int idx_cur_first_player_sprite = static_cast<int>(_first_player_sprite); //���� �¿� ��Ī�� ����� ù ��������Ʈ ����� ���� �ε���
	idx_cur_player_sprite += idx_cur_first_player_sprite; //�������� �����־� ���� ��������Ʈ ����� �����Ѵ�.

	//-----------------------------------------------------------------------------------------------
	// ���� ��������Ʈ ����� �ٽ� �����Ѵ�.
	//-----------------------------------------------------------------------------------------------
	_cur_player_sprite = static_cast<SPRITE_PLAYER>(idx_cur_player_sprite);
}


/**
@brief �������̵�. �÷��̾� ��������Ʈ �ʸ� ������ �����Ѵ�.

@param idx_cur_player_sprite_PLAYER_FILM :�ش� �ִϸ��̼� �ʸ� ���̺��� �ε���(�ִϸ��̼�)

@return : ���� ����
*/
BOOL CBaseObject::SetSpriteFilmFrom(IDX_SPRITE_PLAYER_FILM idx_cur_player_sprite_PLAYER_FILM) {

	//-----------------------------------------------------------------------------------------------
	// �ش� �÷��̾� �ִϸ��̼� �ʸ� ���̺��� �ε����� �̿��� ���� ��������Ʈ, ���� ��������Ʈ, ������ ��������Ʈ�� �����Ѵ�.
	//-----------------------------------------------------------------------------------------------
	_first_player_sprite = TABLE_SPRITE_PLAYER[idx_cur_player_sprite_PLAYER_FILM]._first_player_sprite;
	_cur_player_sprite = TABLE_SPRITE_PLAYER[idx_cur_player_sprite_PLAYER_FILM]._first_player_sprite;
	_last_player_sprite = TABLE_SPRITE_PLAYER[idx_cur_player_sprite_PLAYER_FILM]._last_player_sprite;

	//-----------------------------------------------------------------------------------------------
	// �� ��������Ʈ �� ������ ������ ��� ���� ���� �ʱ�ȭ�Ѵ�.
	//-----------------------------------------------------------------------------------------------
	_max_delay_each_frame = TABLE_SPRITE_PLAYER[idx_cur_player_sprite_PLAYER_FILM]._max_delay_each_fram;
	_cur_frame = 0;
	_is_end_frame = FALSE;

	return true;
}


/**
@brief �������̵�. ����Ʈ ��������Ʈ �ʸ� ������ �����Ѵ�.

@param idx_cur_player_sprite_PLAYER_FILM :�ش� �ִϸ��̼� �ʸ� ���̺��� �ε���(�ִϸ��̼�)

@return : ���� ����
*/
BOOL CBaseObject::SetSpriteFilmFrom(IDX_SPRITE_EFFECT idx_cur_player_sprite_PLAYER_FILM) {

	//-----------------------------------------------------------------------------------------------
	// �ش� �÷��̾� �ִϸ��̼� �ʸ� ���̺��� �ε����� �̿��� ���� ��������Ʈ, ���� ��������Ʈ, ������ ��������Ʈ�� �����Ѵ�.
	//-----------------------------------------------------------------------------------------------
	_first_effect_sprite = TABLE_SPRITE_EFFECT[idx_cur_player_sprite_PLAYER_FILM]._first_player_sprite;
	_cur_effect_sprite = TABLE_SPRITE_EFFECT[idx_cur_player_sprite_PLAYER_FILM]._first_player_sprite;
	_last_effect_sprite = TABLE_SPRITE_EFFECT[idx_cur_player_sprite_PLAYER_FILM]._last_player_sprite;

	//-----------------------------------------------------------------------------------------------
	// �� ��������Ʈ �� ������ ������ ��� ���� ���� �ʱ�ȭ�Ѵ�.
	//-----------------------------------------------------------------------------------------------
	_max_delay_each_frame = TABLE_SPRITE_EFFECT[idx_cur_player_sprite_PLAYER_FILM]._max_delay_each_fram;
	_cur_frame = 0;
	_is_end_frame = FALSE;

	return true;
}



/**
@brief �������̵� �� SetSpriteFilmFrom(IDX_SPRITE_PLAYER_FILM idx_cur_player_sprite_PLAYER_FILM)�� �� �Լ��� ��ü�Ѵ�.
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
@brief ���� �������� ī��Ʈ�Ѵ�.
*/
BOOL CBaseObject::OnNextFrame(){

	//-----------------------------------------------------------------------------------------------
	// ���� ��������Ʈ ����� ���ų� Ȥ�� �ε��� ������ ������� üũ�Ѵ�.
	//-----------------------------------------------------------------------------------------------
	if (SPRITE_PLAYER::PLAYER_NULL > _cur_player_sprite || SPRITE_PLAYER::PLAYER_END <= _cur_player_sprite) {

		return FALSE;
	}

	//-----------------------------------------------------------------------------------------------
	// ������ ī��Ʈ �Ѵ�.
	//-----------------------------------------------------------------------------------------------
	_cur_frame++;

	//-----------------------------------------------------------------------------------------------
	// ������ ī��Ʈ ���� �� �ִϸ��̼��� ������ �����Ӻ��� ū ��� ���� �ִϸ��̼����� �ѱ��
	// ������ ī��Ʈ ���� 0���� �ʱ�ȭ�Ѵ�.
	//-----------------------------------------------------------------------------------------------
	if (_cur_frame >= _max_delay_each_frame) {

		
		_cur_frame = 0;

		int num_sprite = static_cast<int>(_cur_player_sprite);
		num_sprite++;
		SPRITE_PLAYER cur_sprite = static_cast<SPRITE_PLAYER>(num_sprite);

		_cur_player_sprite = cur_sprite;

		//-----------------------------------------------------------------------------------------------
		// ���� ��������Ʈ ����� ������ ��������Ʈ ����� �Ѿ���� üũ�Ѵ�.
		// ���� ���� ��� is_end_frame���� üũ���ش�. (��ȸ�� �ִϸ��̼��� ��� �ִϸ��̼��� �������� �˸���.)
		// ��ȸ�� �ִϸ��̼� ������ �ƴ� ������ �̾����� ��������Ʈ �ִϸ��̼�(�ȱ�)���� ��쳪 ���ĵ� �׼��� ���
		// �ٽ� ���� ��������Ʈ �ִϸ��̼��� �ݺ��ؾ� �Ѵ�.
		//-----------------------------------------------------------------------------------------------
		if (cur_sprite > _last_player_sprite) {

			_cur_player_sprite = _first_player_sprite;
			_is_end_frame = TRUE;
		}
	}

	return TRUE;
}



/**
@brief ���� �������� ī��Ʈ�Ѵ�.
*/
BOOL CBaseObject::OnNextFrameByEffect() {


	//-----------------------------------------------------------------------------------------------
	// ������ ī��Ʈ �Ѵ�.
	//-----------------------------------------------------------------------------------------------
	_cur_frame++;

	//-----------------------------------------------------------------------------------------------
	// ������ ī��Ʈ ���� �� �ִϸ��̼��� ������ �����Ӻ��� ū ��� ���� �ִϸ��̼����� �ѱ��
	// ������ ī��Ʈ ���� 0���� �ʱ�ȭ�Ѵ�.
	//-----------------------------------------------------------------------------------------------
	if (_cur_frame >= _max_delay_each_frame) {


		_cur_frame = 0;

		int num_effect = static_cast<int>(_cur_effect_sprite);
		num_effect++;
		SPRITE_EFFECT cur_sprite = static_cast<SPRITE_EFFECT>(num_effect);

		_cur_effect_sprite = cur_sprite;

		//-----------------------------------------------------------------------------------------------
		// ���� ��������Ʈ ����� ������ ��������Ʈ ����� �Ѿ���� üũ�Ѵ�.
		// ���� ���� ��� is_end_frame���� üũ���ش�. (��ȸ�� �ִϸ��̼��� ��� �ִϸ��̼��� �������� �˸���.)
		// ��ȸ�� �ִϸ��̼� ������ �ƴ� ������ �̾����� ��������Ʈ �ִϸ��̼�(�ȱ�)���� ��쳪 ���ĵ� �׼��� ��� 
		// �ٽ� ���� ��������Ʈ �ִϸ��̼��� �ݺ��ؾ� �Ѵ�.
		//-----------------------------------------------------------------------------------------------
		if (_cur_effect_sprite > _last_effect_sprite) {

			_is_end_frame = TRUE;
		}
	}
	return TRUE;
}


/**
@brief ������ ������ ���θ� Ȯ���Ѵ�. ��������Ʈ �ִϸ��̼��� 1ȸ���� ���(��: ����)�� ������ �ݺ����� �ʵ��� �����ִ� ��Ȱ�� �Ѵ�.
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
@brief ��ǥ xy�� ��ȯ�Ѵ�.
*/
void CBaseObject::GetCurXY(int *out_cur_x, int *out_cur_y) {
	
	*out_cur_x = _cur_x;
	*out_cur_y = _cur_y;
}


/**
@brief ��ǥ xy�� �����Ѵ�.
*/
void CBaseObject::SetCurXY(int cur_x, int cur_y) {
	
	_cur_x = cur_x;
	_cur_y = cur_y;
}
