#pragma once
#include "stdafx.h"

// 엉망진창 퍼블릭 접근... 고치기에 난 너무 게흘러....
class CSpriteNode {
public:
	
	BOOL _is_main_player;

	WCHAR _nickname[20];	///<닉네임
	DIRECTION _direction;	///<방향
	BOOL _is_main_client;

	DEPTH_PRIORITY _depth_attach_priority;	///<스프라이트 부착 시 깊이 우선순위. 높을 수록 앞에 출력된다.

	BOOL _is_fix_sprite;		   ///<스프라이트 고정 여부.
	BOOL _is_player_sprite;		   ///<게임 플레이어 스트라이트 여부.
	BOOL _is_effect_sprite;		   ///<이펙트 스프라이트 여부.

	CBaseObject *_cur_object_addr; ///<현재 오브젝트 주소.
	TYPE_OBJECT _type_object;	   ///<오브젝트 타입.
	
	BOOL _use_destroy;		  ///<삭제 여부

	SPRITE_FIX _cur_fix_sprite;			  ///<원스 스프라이트인 경우만.
	SPRITE_PLAYER _cur_player_sprite;		  ///<플레이어 스프라이트인 경우만.
	SPRITE_EFFECT _cur_effect_sprite;		  ///<이펙트 스프라이트인 경우만.

	int _cur_x;			  ///<현재 x좌표.
	int _cur_y;			  ///<현재 y좌표.

	INT64 _option_value1;	  ///<옵션 값1.
	INT64 _option_value2;	  ///<옵션 값2.
	INT64 _option_value3;	  ///<옵션 값3.


	//생성자
	CSpriteNode(CBaseObject *target_object, TYPE_OBJECT type_object = TYPE_OBJECT::OBJECT_NULL);



	//스프라이트 노드를 업데이트한다.
	BOOL UpdateSpriteNode();
};