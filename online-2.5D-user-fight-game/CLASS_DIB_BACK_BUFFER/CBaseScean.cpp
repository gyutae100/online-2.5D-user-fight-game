
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

//���� �ش� Ŭ���̾�Ʈ�� �޽����� ��ȯ�Ѵ�.
void  CBaseScean::GetCurInputMsgByObject(CPlayerObject *out_plyaer_object) {

	*out_plyaer_object = _arr_player[0];
	
}

//�ٸ� Ű�� ���� ���θ� üũ�Ѵ�.
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

	//���� �÷��̾� ���� üũ�Ѵ�.
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

	//�迭 idx �˻� �� ��������Ʈ �����Ѵ�.
	int cur_idx = 0;
	for (; cur_idx < num_total_player; cur_idx++) {

		if (id == _arr_player[cur_idx]._id) {

			break;
		}
	}

	_arr_player[cur_idx]._is_playing = FALSE;

	//���� ��������Ʈ �����Ѵ�.
	list<CSpriteNode*>::iterator iter = _list_csprite.begin();
	for (;  iter != _list_csprite.end(); iter++) {

		if ((*iter)->_cur_object_addr ==  &_arr_player[cur_idx]) {

			_list_csprite.erase(iter);
		}
	}
}




//������1
CBaseScean::CBaseScean() {

	num_total_player = 100;
}

//������2
CBaseScean::CBaseScean(HWND cur_hwnd) {

	//��ũ�� ��¿� DIB �����Ѵ�.
	g_csprite = new CSPRITE_DIB::CSpriteDib(SPRITE_SETTING::MAX_LOAD_SPRITE, 0x00ffffff);

	//��������Ʈ �Ŵ����� �����Ѵ�.
	g_screen_dib = new CSCREEN_DIB::CScreenDib(SCREEN_RESOLUTION::SCREEN_WIDTH, SCREEN_RESOLUTION::SCREEN_HEIGHT, 32 /**�ȼ� ��Ʈ ��*/);

	//��ũ�� ��� ��� ������ �ڵ��� �����Ѵ�.
	_cur_hwnd = cur_hwnd;

	//���� ���۽� �ʿ��� �⺻ �ʱ�ȭ�� ó���Ѵ�.
	InitScean();

	//�� ������ �ʿ��� ��������Ʈ ����� �о� ���夾����.
	LoadContent();

	num_total_player = 100;

}








/**
brief �� �� ��� ��Ҹ� ������Ʈ �Ѵ�.
*/
void CBaseScean::UpdateGame()
{
	//====================================================================================
	// ���ø����̼� Ȱ��ȭ ���ο� ���� ����� �Է� Ű�� �ش� Ŭ���̾�Ʈ ĳ���Ϳ� �����Ѵ�.
	//====================================================================================
	if (TRUE == g_active_app) {


		HandleClientInputKey();


		//���⼭ �޽����� ������.


	}

	//====================================================================================
	// ���ø����̼� Ȱ��ȭ ���ο� ���� ���� ���� ������Ʈ�� ����Ǿ�� �Ѵ�.
	//====================================================================================

	//====================================================================================
	// ��� Ŭ���̾�Ʈ���� �޽����� ó���Ѵ�.
	//====================================================================================
	ProcClientsMsg();

	//====================================================================================
	// ������ ��ŵ�� ���� �� �� ��� ������Ʈ ��������Ʈ ������Ʈ ���θ� �����Ѵ�.
	//====================================================================================
	FrocDrawByFrameSkip();
}


/**
brief FPS ����Ѵ�.
*/
void CBaseScean::DisplayFPSOnCaption() {

	//���� ������ ��ŵ���� �����Ѵ�.
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
brief Ŭ���̾�Ʈ �޽����� ó���Ѵ�. ������ ��ŵ �߻��ص� ������ ���������� ó���� �Ǿ�� �Ѵ�.
*/
void CBaseScean::ProcClientsMsg() {

	//������ ������ ó���ϰ� �Ѵ�.
	//�ش� Ŭ���̾�Ʈ ��ü. 

	for (int idx = 0; idx < num_total_player; idx++) {
	

		if( TRUE == _arr_player[idx]._is_playing )
		
			_arr_player[idx].OnAction();
	}

	//������ Ŭ���̾�Ʈ Ű���� �Է� ó���Ѵ�.
	//_arr_player[1].OnAction();
	//_arr_player[2].OnAction();
}


//������Ƽ��

/**
brief ���� ���۽� �ʿ��� �⺻ �ʱ�ȭ�� ó�� �Ѵ�.
*/
void CBaseScean::InitScean() {

	//������ ��ŵ�� ���� �Ҵ��Ѵ�.
	_frame_skip = new CFrameSkip(20);

	num_total_player = 100;

	//������ ���� �Ҵ��Ѵ�.
	_arr_player = new CPlayerObject[num_total_player];

	for (int i = 0; i < num_total_player; i++) {

		_arr_player[i]._is_playing = FALSE;
	}

}

/**
brief �� ������ �ʿ��� ��Ʈ����Ʈ ����� �о� �����Ѵ�.
*/
void CBaseScean::LoadContent() {

	//��������Ʈ �ε��Ѵ�.
	LoadSprites();
}

/**
brief �� �ȿ� �÷��̾ ��������Ʈ ���� ����Ѵ�.
*/
BOOL CBaseScean::AddChildSpriteNode(CPlayerObject *player_object) {

	if (NULL == player_object) {

		return false;
	}

	//-----------------------------------------------------------------------------------------------
	// ���� �÷��̾��� ���.
	//-----------------------------------------------------------------------------------------------
	if (player_object->_is_main_player) {

		//�÷��̾�� _hp ��������Ʈ, �÷��̾� ��������Ʈ, shadow ��������Ʈ�� �����ȴ�.
		_list_csprite.push_front(new CSpriteNode(player_object) );
		_list_csprite.push_front(new CSpriteNode(player_object, TYPE_OBJECT::OBJECT_HP));
		_list_csprite.push_front(new CSpriteNode(player_object, TYPE_OBJECT::OBJECT_SHADOW));
		_list_csprite.push_front(new CSpriteNode(player_object, TYPE_OBJECT::OBJECT_NICKNAME));

	}

	//-----------------------------------------------------------------------------------------------
	// �� �� �÷��̾��� ���.
	//-----------------------------------------------------------------------------------------------
	else {

		//�÷��̾�� _hp ��������Ʈ, �÷��̾� ��������Ʈ, shadow ��������Ʈ�� �����ȴ�.
		_list_csprite.push_front(new CSpriteNode(player_object));
		_list_csprite.push_front(new CSpriteNode(player_object, TYPE_OBJECT::OBJECT_HP));
		_list_csprite.push_front(new CSpriteNode(player_object, TYPE_OBJECT::OBJECT_SHADOW));
		_list_csprite.push_front(new CSpriteNode(player_object, TYPE_OBJECT::OBJECT_NICKNAME));

	}

	///@todo : ���� �г��ӵ� �߰�������.

	return true;
}


//y �� ������ �������� �Լ��̴�.
struct SortYAscending {

	bool operator() (const CSpriteNode *pObject1, const CSpriteNode *pObject2) const {

		return (pObject1->_cur_y < pObject2->_cur_y);
	}
};

/**
brief �� �ȿ� ��� ��������Ʈ ��带 ����ۿ� �׷��ش�.
*/
BOOL CBaseScean::DrawAllChildSpriteNode() {

	//-----------------------------------------------------------------------------------------------
	//�� �ȿ� ��� ��������Ʈ ��带 �����Ѵ�.
	//-----------------------------------------------------------------------------------------------
	list<CSpriteNode *>::iterator iter;
	for (iter = _list_csprite.begin(); iter != _list_csprite.end(); iter++) {

		CSpriteNode *tmp = *iter;

		tmp->UpdateSpriteNode();

	}
	//-----------------------------------------------------------------------------------------------
	//y�� �������� �����Ѵ�.
	//-----------------------------------------------------------------------------------------------
	_list_csprite.sort(SortYAscending());

	//------------------------------------------------------------------------------
	//	��� ���� ������ �� ������´�.
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
	//��������Ʈ�� ��� ���� �켱������ y�� ���ؿ� ���� ����Ѵ�.
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
			// ��� ���� ���.
			//-----------------------------------------------------------------------------------------------
			cur_depth = static_cast<int>(tmp->_depth_attach_priority);
			if (cur_depth != start_depth) {

				continue;
			}

			//-----------------------------------------------------------------------------------------------
			//	��� ��������Ʈ�� �÷��̾��� ���.
			//-----------------------------------------------------------------------------------------------
			if (tmp->_type_object == TYPE_OBJECT::OBJECT_PLAYER) {
				SPRITE_PLAYER cur_sprite = tmp->_cur_player_sprite;

				//-----------------------------------------------------------------------------------------------
				// ���� �÷��̾� ������ ���� ��������Ʈ ������ ����ȴ�.
				//-----------------------------------------------------------------------------------------------
				if (TRUE == tmp->_is_main_player) {
					g_csprite->DrawFullSprite(cur_sprite, cur_x, cur_y, image_buffer, dest_width, dest_height, dest_pitch);
				}
				else {

					g_csprite->DrawFullSpriteLikeMonster(cur_sprite, cur_x, cur_y, image_buffer, dest_width, dest_height, dest_pitch);
				}
			}

			///@ todo: �г��� ���� �� ��
			else if (tmp->_type_object == TYPE_OBJECT::OBJECT_NICKNAME) {

				//PrintNickName(tmp->_nickname, cur_x, cur_y);
				
			}

			//-----------------------------------------------------------------------------------------------
			// ��� ��������Ʈ�� HP�� ���.
			//-----------------------------------------------------------------------------------------------
			else if (tmp->_type_object == TYPE_OBJECT::OBJECT_HP) {

				
				cur_fix_sprite = tmp->_cur_fix_sprite;
				g_csprite->DrawFullSprite(cur_fix_sprite, cur_x, cur_y, image_buffer, dest_width, dest_height, dest_pitch, tmp->_option_value1);
			}

			//-----------------------------------------------------------------------------------------------
			// ��� ��������Ʈ�� �׸����� ���.
			//-----------------------------------------------------------------------------------------------
			else if (tmp->_type_object == TYPE_OBJECT::OBJECT_SHADOW) {

				cur_fix_sprite = tmp->_cur_fix_sprite;
				g_csprite->DrawFullSprite(cur_fix_sprite, cur_x, cur_y, image_buffer, dest_width, dest_height, dest_pitch);
			}

			//-----------------------------------------------------------------------------------------------
			// ��� ��������Ʈ�� ����Ʈ�� ���
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
				// ���� �÷��̾� ������ ���� ��������Ʈ ������ ����ȴ�.
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
brief Ŭ���̾�Ʈ(����) �Է� Ű ó���Ѵ�.
*/
BOOL CBaseScean::HandleClientInputKey() {

	ACTION_PLAYER cur_action_msg = ACTION_PLAYER::ACTION_NULL;

	//==============================================================================
	//	�̵� Ű ��Ʈ.
	//==============================================================================

	//==============================================================================
	//	�÷��̾� ���� ���� ����Ű �Է�.
	//==============================================================================
	if (GetAsyncKeyState(VK_LEFT) & 0x8000) {

		//���� ��.
		if (GetAsyncKeyState(VK_UP) & 0x8000) {

			cur_action_msg = ACTION_PLAYER::ACTION_MOVE_LU;
		}

		//���� �Ʒ�.
		else if (GetAsyncKeyState(VK_DOWN) & 0x8000) {

			cur_action_msg = ACTION_PLAYER::ACTION_MOVE_LD;
		}
		//����. 
		else {

			//���� ����Ű
			cur_action_msg = ACTION_PLAYER::ACTION_MOVE_LL;
		}
	}


	//==============================================================================
	//	�÷��̾� ������ ���� ����Ű �Է�.
	//==============================================================================
	else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {

		//������ ��.
		if (GetAsyncKeyState(VK_UP) & 0x8000) {

			cur_action_msg = ACTION_PLAYER::ACTION_MOVE_RU;
		}

		//������ �Ʒ�.
		else if (GetAsyncKeyState(VK_DOWN) & 0x8000) {

			cur_action_msg = ACTION_PLAYER::ACTION_MOVE_RD;
		}
		//������.
		else {

			cur_action_msg = ACTION_PLAYER::ACTION_MOVE_RR;
		}
	}


	//==============================================================================
	//	�÷��̾� �Ʒ��� ����Ű �Է�.
	//==============================================================================
	else if (GetAsyncKeyState(VK_DOWN) & 0x8000) {

		cur_action_msg = ACTION_PLAYER::ACTION_MOVE_DD;
	}


	//==============================================================================
	//	�÷��̾� ���� ����Ű �Է�
	//==============================================================================
	else if (GetAsyncKeyState(VK_UP) & 0x8000) {

		cur_action_msg = ACTION_PLAYER::ACTION_MOVE_UU;
	}


	//==============================================================================
	//	���ִ� ����. �ƹ� ����Ű�� �� ���� ����.
	//==============================================================================
	else {

		cur_action_msg = ACTION_PLAYER::ACTION_STAND;
	}


	//==============================================================================
	//	���� Ű ��Ʈ. �̵� Ű���� �켱������ ����.
	//==============================================================================

	//------------------------------------------------------------------------------
	//	�÷��̾� ZŰ ���� 1 �Է�.
	//------------------------------------------------------------------------------
	if (GetAsyncKeyState(0x5A) & 0x8000) {

		cur_action_msg = ACTION_PLAYER::ACTION_ATTACK1;
	}

	//------------------------------------------------------------------------------
	//	�÷��̾� _xŰ ���� 2 �Է�.
	//------------------------------------------------------------------------------
	if (GetAsyncKeyState(0x58) & 0x8000) {

		cur_action_msg = ACTION_PLAYER::ACTION_ATTACK2;
	}

	//------------------------------------------------------------------------------
	//	�÷��̾� CŰ ���� 3 �Է�.
	//------------------------------------------------------------------------------
	if (GetAsyncKeyState(0x43) & 0x8000) {

		cur_action_msg = ACTION_PLAYER::ACTION_ATTACK3;
	}


	//�ش� Ŭ���̾�Ʈ ��ü���� ����� Ŭ���̾�Ʈ �׼��� ������.
	_arr_player[0].SaveCurActionMsg(cur_action_msg);


	return true;
}

/**
brief �� �ȿ� ��� ���� ��Ҹ� ������ ��ŵ ���ο� ���� �׸���.
*/
void CBaseScean::FrocDrawByFrameSkip()
{

	DisplayFPSOnCaption();

	//------------------------------------------------------------------------------
	//	�ð� ���� �� FPS�� ����Ѵ�.
	//------------------------------------------------------------------------------
	_frame_skip->SelectBeginOrEndTick();

	//------------------------------------------------------------------------------
	//	������ ��ŵ ���� üũ �� ������ ������ üũ�Ѵ�.
	//------------------------------------------------------------------------------
	if (TRUE == _frame_skip->CheckFrameSkip()) {

		return;
	}

	//------------------------------------------------------------------------------
	//	���� �׸���.
	//------------------------------------------------------------------------------
	DrawMap();

	//------------------------------------------------------------------------------
	//	��� ��������Ʈ�� �׸���.
	//------------------------------------------------------------------------------
	DrawAllChildSpriteNode();

	//------------------------------------------------------------------------------
	//	���� �ø��Ѵ�.
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
			// ���� �÷��̾� ������ ���� �г��� ������ ����ȴ�.
			// �÷��̾� ��ü ���⿡ ���� �г��� ��� ��ġ �������� �ʿ��ϴ�.
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
brief ����� DIB�� �׸� ���� ����Ѵ�.
*/
void CBaseScean::FlipScean() {

	//------------------------------------------------------------------------------
	//	ScreenDib�� ȭ������ �ø��Ѵ�.
	//------------------------------------------------------------------------------
	g_screen_dib->DrawBuffer(_cur_hwnd);

	//------------------------------------------------------------------------------
	//	��� �÷��̾���� �г����� ����Ѵ�.
	//------------------------------------------------------------------------------
	DisplayAllNickName();


}

/**
brief ����� DIB�� ���� �׸���.
*/
void CBaseScean::DrawMap() {

	//------------------------------------------------------------------------------
	//	��� ���� ������ �� ������´�.
	//------------------------------------------------------------------------------
	BYTE *image_buffer = g_screen_dib->GetDibBuffer();
	int dest_width = g_screen_dib->GetWidth();
	int dest_height = g_screen_dib->GetHeight();
	int dest_pitch = g_screen_dib->GetPitch();

	//------------------------------------------------------------------------------
	//	1. �� ȭ�� ����Ѵ�.
	//------------------------------------------------------------------------------
	g_csprite->DrawImage(SPRITE_MAP::MAP_STAGE1, 0, 0, image_buffer, dest_width, dest_height, dest_pitch);
}

/**
brief �� ������ �ʿ��� ��������Ʈ ����� �о� �����Ѵ�.
*/
BOOL CBaseScean::LoadSprites() {

	//�� �ʱ�ȭ �Ѵ�.
	g_csprite->LoadDibSprite(SPRITE_MAP::MAP_STAGE1, L"SpriteData\\_Map.bmp", 0, 0);

	//���� ���ٵ� ��������Ʈ �ʱ�ȭ�Ѵ�.
	g_csprite->LoadDibSprite(3, L"SpriteData\\STAND_L_01.bmp", 71, 90);
	g_csprite->LoadDibSprite(4, L"SpriteData\\STAND_L_02.bmp", 71, 90);
	g_csprite->LoadDibSprite(5, L"SpriteData\\STAND_L_03.bmp", 71, 90);


	//������ ���ٵ� ��������Ʈ �ʱ�ȭ�Ѵ�.
	g_csprite->LoadDibSprite(6, L"SpriteData\\STAND_R_01.bmp", 71, 90);
	g_csprite->LoadDibSprite(7, L"SpriteData\\STAND_R_02.bmp", 71, 90);
	g_csprite->LoadDibSprite(8, L"SpriteData\\STAND_R_03.bmp", 71, 90);


	//���� �ȱ� ��������Ʈ �ʱ�ȭ �Ѵ�.
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


	//������ �ȱ� ��������Ʈ �ʱ�ȭ �Ѵ�.
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


	//������ ����1 ��������Ʈ �ʱ�ȭ �Ѵ�.
	g_csprite->LoadDibSprite(33, L"SpriteData\\Attack1_R_01.bmp", 71, 90);
	g_csprite->LoadDibSprite(34, L"SpriteData\\Attack1_R_02.bmp", 71, 90);
	g_csprite->LoadDibSprite(35, L"SpriteData\\Attack1_R_03.bmp", 71, 90);
	g_csprite->LoadDibSprite(36, L"SpriteData\\Attack1_R_04.bmp", 71, 90);



	//���� ����1 ��������Ʈ �ʱ�ȭ �Ѵ�.
	g_csprite->LoadDibSprite(37, L"SpriteData\\Attack1_L_01.bmp", 71, 90);
	g_csprite->LoadDibSprite(38, L"SpriteData\\Attack1_L_02.bmp", 71, 90);
	g_csprite->LoadDibSprite(39, L"SpriteData\\Attack1_L_03.bmp", 71, 90);
	g_csprite->LoadDibSprite(40, L"SpriteData\\Attack1_L_04.bmp", 71, 90);


	//������ ����2 ��������Ʈ �ʱ�ȭ �Ѵ�.
	g_csprite->LoadDibSprite(41, L"SpriteData\\Attack2_R_01.bmp", 71, 90);
	g_csprite->LoadDibSprite(42, L"SpriteData\\Attack2_R_02.bmp", 71, 90);
	g_csprite->LoadDibSprite(43, L"SpriteData\\Attack2_R_03.bmp", 71, 90);
	g_csprite->LoadDibSprite(44, L"SpriteData\\Attack2_R_04.bmp", 71, 90);


	//���� ����2 ��������Ʈ �ʱ�ȭ �Ѵ�.
	g_csprite->LoadDibSprite(45, L"SpriteData\\Attack2_L_01.bmp", 71, 90);
	g_csprite->LoadDibSprite(46, L"SpriteData\\Attack2_L_02.bmp", 71, 90);
	g_csprite->LoadDibSprite(47, L"SpriteData\\Attack2_L_03.bmp", 71, 90);
	g_csprite->LoadDibSprite(48, L"SpriteData\\Attack2_L_04.bmp", 71, 90);


	//������ ����3 ��������Ʈ �ʱ�ȭ �Ѵ�.
	g_csprite->LoadDibSprite(49, L"SpriteData\\Attack3_R_01.bmp", 71, 90);
	g_csprite->LoadDibSprite(50, L"SpriteData\\Attack3_R_02.bmp", 71, 90);
	g_csprite->LoadDibSprite(51, L"SpriteData\\Attack3_R_03.bmp", 71, 90);
	g_csprite->LoadDibSprite(52, L"SpriteData\\Attack3_R_04.bmp", 71, 90);
	g_csprite->LoadDibSprite(53, L"SpriteData\\Attack3_R_05.bmp", 71, 90);
	g_csprite->LoadDibSprite(54, L"SpriteData\\Attack3_R_06.bmp", 71, 90);


	//���� ����3 ��������Ʈ �ʱ�ȭ �Ѵ�.
	g_csprite->LoadDibSprite(55, L"SpriteData\\Attack3_L_01.bmp", 71, 90);
	g_csprite->LoadDibSprite(56, L"SpriteData\\Attack3_L_02.bmp", 71, 90);
	g_csprite->LoadDibSprite(57, L"SpriteData\\Attack3_L_03.bmp", 71, 90);
	g_csprite->LoadDibSprite(58, L"SpriteData\\Attack3_L_04.bmp", 71, 90);
	g_csprite->LoadDibSprite(59, L"SpriteData\\Attack3_L_05.bmp", 71, 90);
	g_csprite->LoadDibSprite(60, L"SpriteData\\Attack3_L_06.bmp", 71, 90);



	//����Ʈ ��������Ʈ �ʱ�ȭ�Ѵ�.
	g_csprite->LoadDibSprite(62, L"SpriteData\\xSpark_1.bmp", 70, 70);
	g_csprite->LoadDibSprite(63, L"SpriteData\\xSpark_2.bmp", 70, 70);
	g_csprite->LoadDibSprite(64, L"SpriteData\\xSpark_3.bmp", 70, 70);
	g_csprite->LoadDibSprite(65, L"SpriteData\\xSpark_4.bmp", 70, 70);

	//ü�� ��������Ʈ �ʱ�ȭ�Ѵ�.
	g_csprite->LoadDibSprite(67, L"SpriteData\\HPGuage.bmp", +35, -15);

	//�׸��� ��������Ʈ �ʱ�ȭ�Ѵ�.
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


//�г����� ����Ѵ�.
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
