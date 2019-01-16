#pragma once

#include "CBaseObject.h"
#include "stdafx.h"


class CPlayerObject : public CBaseObject {

private :
	

	int _max_hp;
	int _cur_hp;


	BOOL _is_moving;				///<현재 객체가 움직이고 있는 상태 표시.  
	BOOL _is_standing;			    ///<현재 객체가 서있는 상태 표시.



public:


	BOOL _is_attacking;				//어텍 중인 여부 상태 표시.
	BOOL _is_start_attack;			//어텍 시작 여부 상태 표시.
	BOOL _is_stop_moving;			///<현재 객체가 달리다가 멈춘 상태 표시.


	int _id;			//id;
	BOOL _is_playing;	//현재 플레이어 여부 상태 표시.
	ACTION_PLAYER  _cur_input_action_msg; ///< 현재 클라이언트 입력 메시지. 다음 실행할 액션을 정의.
	ACTION_PLAYER  _pre_input_action_msg; ///< 이전 클라이언트 입력 메시지. 이전에 실행한 액션 정의.

	friend class CStageScean;

	//생성자1
	CPlayerObject(int x, int y);

	//생성자2
	CPlayerObject();

	///@todo : 생성자3만들 것. 만들 시 hp 세팅도 같이 해줄 것!!!

	//사용자 입력 액션 메시지를 저장한다.
	void SaveCurActionMsg(ACTION_PLAYER cur_input_action);

	//액션 진행용 콜백 함수이다.
	BOOL OnAction();

	//소멸자
	virtual ~CPlayerObject();


	void SetNickName(CONST WCHAR *nickname);
	void InitPlayerDirection(DIRECTION cur_direction);
	int GetHp();
	void SetHp(int hp);
	void InitPlayerInfo(int id, int hp, int x, int y, DIRECTION cur_direction, CONST WCHAR *nickname);



protected:
	
	//사용자 입력 액션 전처리 과정을 진행한다.
	//사용자가 진행 중인 액션 중 끊기면 안되는 액션(예 공격)같은 액션인 경우 사용자 입력 액션 메시지를 무시하는 처리를 담당한다.
	BOOL OnFilteringActionMsg(void);

	//사용자 지정 입력 액션 메시지를 처리한다.
	BOOL OnProcActionMsg();


	//현재 객체가 걷는 계속 상태였는지 체크한다.
	BOOL CheckKeepMoving();

	//현재 방향이 이전과 다르게 변했는지 체크한다.
	BOOL CheckChangeDirection();

	//액션 무브 처리한다. 
	void OnActionMove();

	//액션 무브 시작한다.
	void BeginActionMove();

	//액션 스텐드 시작한다.
	void BeginActionStand();

	//액션 어텍 시작한다.
	void BeginActionAttack();



	//해당 메시지가 어텍 메시지인지 체크한다
	BOOL CheckIsAttackAction(ACTION_PLAYER in_action) {

		if (in_action == ACTION_ATTACK1)
		{
			return TRUE;
		}
		else if (in_action == ACTION_ATTACK2) {

			return TRUE;
		}
		else if (in_action == ACTION_ATTACK3) {

			return TRUE;
		}

		return FALSE;

	}


};



class CEffectObject : public CBaseObject {

public:


	//생성자2
	CEffectObject(int x, int y);

	int GetHp() {

		return -1;
	}
	BOOL OnAction();
};

