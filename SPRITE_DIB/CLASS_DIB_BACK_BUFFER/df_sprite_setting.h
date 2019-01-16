
#pragma once;
#include "stdafx.h"



typedef enum SPRITE_SETTING {     ///< 스프라이트 매니저 셋팅.
	MAX_LOAD_SPRITE = 100	      ///< 최대 스프라이트 갯수.

}SPRITE_SETTING;

typedef enum SCREEN_RESOLUTION {  ///<  화면 해상도 정보.
	SCREEN_HEIGHT = 480, ///< 해상도 높이.
	SCREEN_WIDTH = 640   ///< 해상도 너비.

}SCREEN_RESOLUTION;

typedef enum DEPTH_PRIORITY {	 ///<스프라이트 부착 우선순위. 스프라이트는 _y좌표 기준 정렬 되지만
								 ///<스프라이트 부착 우선순위가 높은 스프라이트는 _y좌표와 관계없이 우선순위가 낮은 스프라이트의 위에 출력된다.

	PRIORITY_NULL= 0,
	PRIORITY_1ST = 1,			 ///<우선순위가 가장 높다.
	PRIORITY_2ND , 
	PRIORITY_3RD , 
	PRIORITY_4TH ,
	PRIORITY_5TH,
	PRIORITY_6TH,
	PRIORITY_7TH,
	PRIORITY_END ,
}DEPTH_PRIORITY;


typedef enum SPRITE_MAP {	   ///<스테이지 종류.
	MAP_NULL = 0,
	MAP_STAGE1 = 1,			///<스테이지 1.
	MAP_END

}SPRITE_MAP;


typedef enum SPRITE_PLAYER {  ///< 플레이어 스트라이트 번호에 이름을 부여한다. 스프라이트 관리자에서 생성하는 스프라이트 번호와 매핑된다.
	PLAYER_NULL = 0,

	//왼쪽 스텐딩.
	PLAYER_STAND_L_1 = MAP_END+1,	///<왼쪽 스텐딩 시작.
	PLAYER_STAND_L_2 ,
	PLAYER_STAND_L_MAX,				///<왼쪽 스텐딩 끝.

	//오른쪽 스텐딩.
	PLAYER_STAND_R_1 ,				///<오른쪽 스텐딩 시작.
	PLAYER_STAND_R_2 , 
	PLAYER_STAND_R_MAX,				///<오른쪽 스텐딩 끝.

	//왼쪽 방향 걷기.
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

	//오른쪽 방향 걷기.
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

	//오른쪽 어텍1 오른쪽 방향.
	PLAYER_ATTACK1_R1,
	PLAYER_ATTACK1_R2,
	PLAYER_ATTACK1_R3,
	PLAYER_ATTACK1_R_MAX,

	//왼쪽 어텍1 왼쪽 방향.
	PLAYER_ATTACK1_L1,
	PLAYER_ATTACK1_L2,
	PLAYER_ATTACK1_L3,
	PLAYER_ATTACK1_L_MAX,

	//오른쪽 어텍2 오른쪽 방향.
	PLAYER_ATTACK2_R1,
	PLAYER_ATTACK2_R2,
	PLAYER_ATTACK2_R3,
	PLAYER_ATTACK2_R_MAX,

	//왼쪽 어텍2 왼쪽 방향.
	PLAYER_ATTACK2_L1,
	PLAYER_ATTACK2_L2,
	PLAYER_ATTACK2_L3,
	PLAYER_ATTACK2_L_MAX,

	//오른쪽 어텍3 오른쪽 방향.
	PLAYER_ATTACK3_R1,
	PLAYER_ATTACK3_R2,
	PLAYER_ATTACK3_R3,
	PLAYER_ATTACK3_R4,
	PLAYER_ATTACK3_R5,
	PLAYER_ATTACK3_R_MAX,

	//왼쪽 어텍3 왼쪽 방향.
	PLAYER_ATTACK3_L1,
	PLAYER_ATTACK3_L2,
	PLAYER_ATTACK3_L3,
	PLAYER_ATTACK3_L4,
	PLAYER_ATTACK3_L5,
	PLAYER_ATTACK3_L_MAX,

	//플레이어 스프레이트 끝.
	PLAYER_END				

}SPRITE_PLAYER;


typedef enum SPRITE_EFFECT {		///<이펙트 스프라이트 애니메이션 번호. 스프라이트 관리자의 스프라이트 번호와 매핑된다.
	
	EFFECT_NULL = 0,
	EFFECT_XSPARK_1 = PLAYER_END+1,
	EFFECT_XSPARK_2,
	EFFECT_XSPARK_3,
	EFFECT_XSPARK_MAX,
	EFFECT_END
}SPRITE_EFFECT;


typedef enum SPRITE_FIX {		   ///<스프라이트가 애니메이션 형식이 아니고 특정 스프라이트에 고정되는 경우. 예 _hp, SHADOW
	
	ONCE_NULL=0,
	ONCE_HP = EFFECT_END +1,
	ONCE_SHADOW,
	ONCE_NICKNAME,
	ONCE_END
}SPRITE_FIX;


typedef enum TYPE_OBJECT{  ///< 오브젝트 타입 정의.
	
	OBJECT_NULL = 0,
	OBJECT_PLAYER = 1,	   ///< 플레이어 오브젝트.
	OBJECT_EFFECT, 		   ///< 이펙트 오브젝트.
	OBJECT_HP,			   ///< _hp 오브젝트.
	OBJECT_SHADOW,
	OBJECT_NICKNAME,	   ///<닉네임
}TYPE_OBJECT;



typedef enum ACTION_PLAYER {  ///< 플레이어 액션 정의. 실제 클라이언트 프로토콜로 이용한다.
	
	//무행동 정의
	ACTION_NULL = -1,

	//방향에 따른 움직임 정의
	ACTION_MOVE_LL = 0,
	ACTION_MOVE_LU = 1,
	ACTION_MOVE_UU = 2,
	ACTION_MOVE_RU = 3,
	ACTION_MOVE_RR = 4,
	ACTION_MOVE_RD = 5,
	ACTION_MOVE_DD = 6,
	ACTION_MOVE_LD = 7,

	//서있는 움직임 정의
	ACTION_STAND = 8,

	//공격 움직임 정의
	ACTION_ATTACK1 = 9,
	ACTION_ATTACK2 = 10,
	ACTION_ATTACK3 = 11
}ACTION_PLAYER;


typedef enum DIRECTION {	//<방향 설정. 좌, 우 존재한다.  

	DIRECTION_LEFT=0,
	DIRECTION_RIGHT=4
}DIRECTION;



typedef struct SPRITE_PLAYER_FILM {	///<스프라이트 애니메이션. 처음 애니메이션 번호와 마지막 애니메이션 번호. 그리고 각각의 스프라이트의 프레임 딜레이.

	SPRITE_PLAYER _first_player_sprite;
	SPRITE_PLAYER _last_player_sprite;
	int  _max_delay_each_fram;
}SPRITE_PLAYER_FILM;



typedef enum IDX_SPRITE_PLAYER_FILM {	///<스프라이트 애니메이션에 인덱스를 부여.

	//플레이어 에니메이션 
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



static SPRITE_PLAYER_FILM TABLE_SPRITE_PLAYER[30] = {		///<스프라이트 애니메이션 테이블이다. IDX_SPRITE_PLAYER_FILM의 인덱스와 매핑된다.

	//왼쪽 이동 움직임 정의
	/*ACTION_MOVE_L*/	{ SPRITE_PLAYER::PLAYER_MOVE_L_1,   SPRITE_PLAYER::PLAYER_MOVE_L_MAX,     4 },

	//오른쪽 이동 움직임 정의
	/*ACTION_MOVE_R*/	{ SPRITE_PLAYER::PLAYER_MOVE_R_1,   SPRITE_PLAYER::PLAYER_MOVE_R_MAX,     4 },

	//왼쪽 스텐드 움직임 정의
	/*ACTION_STAND_L*/	{ SPRITE_PLAYER::PLAYER_STAND_L_1,  SPRITE_PLAYER::PLAYER_STAND_L_MAX,    5 },

	//오른쪽 스텐드 움직임 정의
	/*ACTION_STAND_R*/	{ SPRITE_PLAYER::PLAYER_STAND_R_1,  SPRITE_PLAYER::PLAYER_STAND_R_MAX,    5 },

	//왼쪽 공격 움직임 정의
	/*ACTION_ATTACK1_L*/{ SPRITE_PLAYER::PLAYER_ATTACK1_L1, SPRITE_PLAYER::PLAYER_ATTACK1_L_MAX,  3 },
	/*ACTION_ATTACK2_L*/{ SPRITE_PLAYER::PLAYER_ATTACK2_L1, SPRITE_PLAYER::PLAYER_ATTACK2_L_MAX,  4 },
	/*ACTION_ATTACK3_L*/{ SPRITE_PLAYER::PLAYER_ATTACK3_L1, SPRITE_PLAYER::PLAYER_ATTACK3_L_MAX,  4 },

	//오른쪽 공격 움직임 정의
	/*ACTION_ATTACK1_R*/{ SPRITE_PLAYER::PLAYER_ATTACK1_R1, SPRITE_PLAYER::PLAYER_ATTACK1_R_MAX,  3 },
	/*ACTION_ATTACK2_R*/{ SPRITE_PLAYER::PLAYER_ATTACK2_R1, SPRITE_PLAYER::PLAYER_ATTACK2_R_MAX,  4 },
	/*ACTION_ATTACK3_R*/{ SPRITE_PLAYER::PLAYER_ATTACK3_R1, SPRITE_PLAYER::PLAYER_ATTACK3_R_MAX,  4 },	


};

typedef struct SPRITE_EFFECT_FILM {	///<스프라이트 애니메이션. 처음 애니메이션 번호와 마지막 애니메이션 번호. 그리고 각각의 스프라이트의 프레임 딜레이.

	SPRITE_EFFECT _first_player_sprite;
	SPRITE_EFFECT _last_player_sprite;
	int  _max_delay_each_fram;
}SPRITE_EFFECT_FILM;

typedef enum IDX_SPRITE_EFFECT {	///<스프라이트 애니메이션에 인덱스를 부여.

	IDX_EFFECT1 =0,
}IDX_SPRITE_EFFECT;

static SPRITE_EFFECT_FILM TABLE_SPRITE_EFFECT[30] = {		///<스프라이트 애니메이션 테이블이다. IDX_SPRITE_PLAYER_FILM의 인덱스와 매핑된다.

													//왼쪽 이동 움직임 정의
	/*ACTION_MOVE_L*/{ SPRITE_EFFECT::EFFECT_XSPARK_1,   SPRITE_EFFECT::EFFECT_XSPARK_MAX,     3 },

};





