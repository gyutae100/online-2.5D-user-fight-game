#pragma once

#include "CBaseObject.h"
#include "stdafx.h"


class CPlayerObject : public CBaseObject {

private :
	

	int _max_hp;
	int _cur_hp;


	BOOL _is_moving;				///<���� ��ü�� �����̰� �ִ� ���� ǥ��.  
	BOOL _is_standing;			    ///<���� ��ü�� ���ִ� ���� ǥ��.



public:


	BOOL _is_attacking;				//���� ���� ���� ���� ǥ��.
	BOOL _is_start_attack;			//���� ���� ���� ���� ǥ��.
	BOOL _is_stop_moving;			///<���� ��ü�� �޸��ٰ� ���� ���� ǥ��.


	int _id;			//id;
	BOOL _is_playing;	//���� �÷��̾� ���� ���� ǥ��.
	ACTION_PLAYER  _cur_input_action_msg; ///< ���� Ŭ���̾�Ʈ �Է� �޽���. ���� ������ �׼��� ����.
	ACTION_PLAYER  _pre_input_action_msg; ///< ���� Ŭ���̾�Ʈ �Է� �޽���. ������ ������ �׼� ����.

	friend class CStageScean;

	//������1
	CPlayerObject(int x, int y);

	//������2
	CPlayerObject();

	///@todo : ������3���� ��. ���� �� hp ���õ� ���� ���� ��!!!

	//����� �Է� �׼� �޽����� �����Ѵ�.
	void SaveCurActionMsg(ACTION_PLAYER cur_input_action);

	//�׼� ����� �ݹ� �Լ��̴�.
	BOOL OnAction();

	//�Ҹ���
	virtual ~CPlayerObject();


	void SetNickName(CONST WCHAR *nickname);
	void InitPlayerDirection(DIRECTION cur_direction);
	int GetHp();
	void SetHp(int hp);
	void InitPlayerInfo(int id, int hp, int x, int y, DIRECTION cur_direction, CONST WCHAR *nickname);



protected:
	
	//����� �Է� �׼� ��ó�� ������ �����Ѵ�.
	//����ڰ� ���� ���� �׼� �� ����� �ȵǴ� �׼�(�� ����)���� �׼��� ��� ����� �Է� �׼� �޽����� �����ϴ� ó���� ����Ѵ�.
	BOOL OnFilteringActionMsg(void);

	//����� ���� �Է� �׼� �޽����� ó���Ѵ�.
	BOOL OnProcActionMsg();


	//���� ��ü�� �ȴ� ��� ���¿����� üũ�Ѵ�.
	BOOL CheckKeepMoving();

	//���� ������ ������ �ٸ��� ���ߴ��� üũ�Ѵ�.
	BOOL CheckChangeDirection();

	//�׼� ���� ó���Ѵ�. 
	void OnActionMove();

	//�׼� ���� �����Ѵ�.
	void BeginActionMove();

	//�׼� ���ٵ� �����Ѵ�.
	void BeginActionStand();

	//�׼� ���� �����Ѵ�.
	void BeginActionAttack();



	//�ش� �޽����� ���� �޽������� üũ�Ѵ�
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


	//������2
	CEffectObject(int x, int y);

	int GetHp() {

		return -1;
	}
	BOOL OnAction();
};

