
#pragma once;
#include "stdafx.h"



typedef enum SPRITE_SETTING {     ///< ��������Ʈ �Ŵ��� ����.
	MAX_LOAD_SPRITE = 100	      ///< �ִ� ��������Ʈ ����.

}SPRITE_SETTING;

typedef enum SCREEN_RESOLUTION {  ///<  ȭ�� �ػ� ����.
	SCREEN_HEIGHT = 480, ///< �ػ� ����.
	SCREEN_WIDTH = 640   ///< �ػ� �ʺ�.

}SCREEN_RESOLUTION;

typedef enum DEPTH_PRIORITY {	 ///<��������Ʈ ���� �켱����. ��������Ʈ�� _y��ǥ ���� ���� ������
								 ///<��������Ʈ ���� �켱������ ���� ��������Ʈ�� _y��ǥ�� ������� �켱������ ���� ��������Ʈ�� ���� ��µȴ�.

	PRIORITY_NULL= 0,
	PRIORITY_1ST = 1,			 ///<�켱������ ���� ����.
	PRIORITY_2ND , 
	PRIORITY_3RD , 
	PRIORITY_4TH ,
	PRIORITY_5TH,
	PRIORITY_6TH,
	PRIORITY_7TH,
	PRIORITY_END ,
}DEPTH_PRIORITY;


typedef enum SPRITE_MAP {	   ///<�������� ����.
	MAP_NULL = 0,
	MAP_STAGE1 = 1,			///<�������� 1.
	MAP_END

}SPRITE_MAP;


typedef enum SPRITE_PLAYER {  ///< �÷��̾� ��Ʈ����Ʈ ��ȣ�� �̸��� �ο��Ѵ�. ��������Ʈ �����ڿ��� �����ϴ� ��������Ʈ ��ȣ�� ���εȴ�.
	PLAYER_NULL = 0,

	//���� ���ٵ�.
	PLAYER_STAND_L_1 = MAP_END+1,	///<���� ���ٵ� ����.
	PLAYER_STAND_L_2 ,
	PLAYER_STAND_L_MAX,				///<���� ���ٵ� ��.

	//������ ���ٵ�.
	PLAYER_STAND_R_1 ,				///<������ ���ٵ� ����.
	PLAYER_STAND_R_2 , 
	PLAYER_STAND_R_MAX,				///<������ ���ٵ� ��.

	//���� ���� �ȱ�.
	PLAYER_MOVE_L_1 ,			
	PLAYER_MOVE_L_2,
	PLAYER_MOVE_L_3,
	PLAYER_MOVE_L_4,
	PLAYER_MOVE_L_5,
	PLAYER_MOVE_L_6,
	PLAYER_MOVE_L_7,
	PLAYER_MOVE_L_8,
	PLAYER_MOVE_L_9,
	PLAYER_MOVE_L_10,
	PLAYER_MOVE_L_11,
	PLAYER_MOVE_L_MAX ,

	//������ ���� �ȱ�.
	PLAYER_MOVE_R_1 ,
	PLAYER_MOVE_R_2,
	PLAYER_MOVE_R_3,
	PLAYER_MOVE_R_4,
	PLAYER_MOVE_R_5,
	PLAYER_MOVE_R_6,
	PLAYER_MOVE_R_7,
	PLAYER_MOVE_R_8,
	PLAYER_MOVE_R_9,
	PLAYER_MOVE_R_10,
	PLAYER_MOVE_R_11,
	PLAYER_MOVE_R_MAX ,

	//������ ����1 ������ ����.
	PLAYER_ATTACK1_R1,
	PLAYER_ATTACK1_R2,
	PLAYER_ATTACK1_R3,
	PLAYER_ATTACK1_R_MAX,

	//���� ����1 ���� ����.
	PLAYER_ATTACK1_L1,
	PLAYER_ATTACK1_L2,
	PLAYER_ATTACK1_L3,
	PLAYER_ATTACK1_L_MAX,

	//������ ����2 ������ ����.
	PLAYER_ATTACK2_R1,
	PLAYER_ATTACK2_R2,
	PLAYER_ATTACK2_R3,
	PLAYER_ATTACK2_R_MAX,

	//���� ����2 ���� ����.
	PLAYER_ATTACK2_L1,
	PLAYER_ATTACK2_L2,
	PLAYER_ATTACK2_L3,
	PLAYER_ATTACK2_L_MAX,

	//������ ����3 ������ ����.
	PLAYER_ATTACK3_R1,
	PLAYER_ATTACK3_R2,
	PLAYER_ATTACK3_R3,
	PLAYER_ATTACK3_R4,
	PLAYER_ATTACK3_R5,
	PLAYER_ATTACK3_R_MAX,

	//���� ����3 ���� ����.
	PLAYER_ATTACK3_L1,
	PLAYER_ATTACK3_L2,
	PLAYER_ATTACK3_L3,
	PLAYER_ATTACK3_L4,
	PLAYER_ATTACK3_L5,
	PLAYER_ATTACK3_L_MAX,

	//�÷��̾� ��������Ʈ ��.
	PLAYER_END				

}SPRITE_PLAYER;


typedef enum SPRITE_EFFECT {		///<����Ʈ ��������Ʈ �ִϸ��̼� ��ȣ. ��������Ʈ �������� ��������Ʈ ��ȣ�� ���εȴ�.
	
	EFFECT_NULL = 0,
	EFFECT_XSPARK_1 = PLAYER_END+1,
	EFFECT_XSPARK_2,
	EFFECT_XSPARK_3,
	EFFECT_XSPARK_MAX,
	EFFECT_END
}SPRITE_EFFECT;


typedef enum SPRITE_FIX {		   ///<��������Ʈ�� �ִϸ��̼� ������ �ƴϰ� Ư�� ��������Ʈ�� �����Ǵ� ���. �� _hp, SHADOW
	
	ONCE_NULL=0,
	ONCE_HP = EFFECT_END +1,
	ONCE_SHADOW,
	ONCE_NICKNAME,
	ONCE_END
}SPRITE_FIX;


typedef enum TYPE_OBJECT{  ///< ������Ʈ Ÿ�� ����.
	
	OBJECT_NULL = 0,
	OBJECT_PLAYER = 1,	   ///< �÷��̾� ������Ʈ.
	OBJECT_EFFECT, 		   ///< ����Ʈ ������Ʈ.
	OBJECT_HP,			   ///< _hp ������Ʈ.
	OBJECT_SHADOW,
	OBJECT_NICKNAME,	   ///<�г���
}TYPE_OBJECT;



typedef enum ACTION_PLAYER {  ///< �÷��̾� �׼� ����. ���� Ŭ���̾�Ʈ �������ݷ� �̿��Ѵ�.
	
	//���ൿ ����
	ACTION_NULL = -1,

	//���⿡ ���� ������ ����
	ACTION_MOVE_LL = 0,
	ACTION_MOVE_LU = 1,
	ACTION_MOVE_UU = 2,
	ACTION_MOVE_RU = 3,
	ACTION_MOVE_RR = 4,
	ACTION_MOVE_RD = 5,
	ACTION_MOVE_DD = 6,
	ACTION_MOVE_LD = 7,

	//���ִ� ������ ����
	ACTION_STAND = 8,

	//���� ������ ����
	ACTION_ATTACK1 = 9,
	ACTION_ATTACK2 = 10,
	ACTION_ATTACK3 = 11
}ACTION_PLAYER;


typedef enum DIRECTION {	//<���� ����. ��, �� �����Ѵ�.  

	DIRECTION_LEFT=0,
	DIRECTION_RIGHT=4
}DIRECTION;



typedef struct SPRITE_PLAYER_FILM {	///<��������Ʈ �ִϸ��̼�. ó�� �ִϸ��̼� ��ȣ�� ������ �ִϸ��̼� ��ȣ. �׸��� ������ ��������Ʈ�� ������ ������.

	SPRITE_PLAYER _first_player_sprite;
	SPRITE_PLAYER _last_player_sprite;
	int  _max_delay_each_fram;
}SPRITE_PLAYER_FILM;



typedef enum IDX_SPRITE_PLAYER_FILM {	///<��������Ʈ �ִϸ��̼ǿ� �ε����� �ο�.

	//�÷��̾� ���ϸ��̼� 
	IDX_ACTION_MOVE_L= 0,
	IDX_ACTION_MOVE_R= 1,
	
	IDX_ACTION_STAND_L,
	IDX_ACTION_STAND_R,

	IDX_ACTION_ATTACK1_L,
	IDX_ACTION_ATTACK2_L,
	IDX_ACTION_ATTACK3_L,

	IDX_ACTION_ATTACK1_R,
	IDX_ACTION_ATTACK2_R,
	IDX_ACTION_ATTACK3_R,

	
}IDX_SPRITE_PLAYER_FILM;



static SPRITE_PLAYER_FILM TABLE_SPRITE_PLAYER[30] = {		///<��������Ʈ �ִϸ��̼� ���̺��̴�. IDX_SPRITE_PLAYER_FILM�� �ε����� ���εȴ�.

	//���� �̵� ������ ����
	/*ACTION_MOVE_L*/	{ SPRITE_PLAYER::PLAYER_MOVE_L_1,   SPRITE_PLAYER::PLAYER_MOVE_L_MAX,     4 },

	//������ �̵� ������ ����
	/*ACTION_MOVE_R*/	{ SPRITE_PLAYER::PLAYER_MOVE_R_1,   SPRITE_PLAYER::PLAYER_MOVE_R_MAX,     4 },

	//���� ���ٵ� ������ ����
	/*ACTION_STAND_L*/	{ SPRITE_PLAYER::PLAYER_STAND_L_1,  SPRITE_PLAYER::PLAYER_STAND_L_MAX,    5 },

	//������ ���ٵ� ������ ����
	/*ACTION_STAND_R*/	{ SPRITE_PLAYER::PLAYER_STAND_R_1,  SPRITE_PLAYER::PLAYER_STAND_R_MAX,    5 },

	//���� ���� ������ ����
	/*ACTION_ATTACK1_L*/{ SPRITE_PLAYER::PLAYER_ATTACK1_L1, SPRITE_PLAYER::PLAYER_ATTACK1_L_MAX,  3 },
	/*ACTION_ATTACK2_L*/{ SPRITE_PLAYER::PLAYER_ATTACK2_L1, SPRITE_PLAYER::PLAYER_ATTACK2_L_MAX,  4 },
	/*ACTION_ATTACK3_L*/{ SPRITE_PLAYER::PLAYER_ATTACK3_L1, SPRITE_PLAYER::PLAYER_ATTACK3_L_MAX,  4 },

	//������ ���� ������ ����
	/*ACTION_ATTACK1_R*/{ SPRITE_PLAYER::PLAYER_ATTACK1_R1, SPRITE_PLAYER::PLAYER_ATTACK1_R_MAX,  3 },
	/*ACTION_ATTACK2_R*/{ SPRITE_PLAYER::PLAYER_ATTACK2_R1, SPRITE_PLAYER::PLAYER_ATTACK2_R_MAX,  4 },
	/*ACTION_ATTACK3_R*/{ SPRITE_PLAYER::PLAYER_ATTACK3_R1, SPRITE_PLAYER::PLAYER_ATTACK3_R_MAX,  4 },	


};

typedef struct SPRITE_EFFECT_FILM {	///<��������Ʈ �ִϸ��̼�. ó�� �ִϸ��̼� ��ȣ�� ������ �ִϸ��̼� ��ȣ. �׸��� ������ ��������Ʈ�� ������ ������.

	SPRITE_EFFECT _first_player_sprite;
	SPRITE_EFFECT _last_player_sprite;
	int  _max_delay_each_fram;
}SPRITE_EFFECT_FILM;

typedef enum IDX_SPRITE_EFFECT {	///<��������Ʈ �ִϸ��̼ǿ� �ε����� �ο�.

	IDX_EFFECT1 =0,
}IDX_SPRITE_EFFECT;

static SPRITE_EFFECT_FILM TABLE_SPRITE_EFFECT[30] = {		///<��������Ʈ �ִϸ��̼� ���̺��̴�. IDX_SPRITE_PLAYER_FILM�� �ε����� ���εȴ�.

													//���� �̵� ������ ����
	/*ACTION_MOVE_L*/{ SPRITE_EFFECT::EFFECT_XSPARK_1,   SPRITE_EFFECT::EFFECT_XSPARK_MAX,     3 },

};





