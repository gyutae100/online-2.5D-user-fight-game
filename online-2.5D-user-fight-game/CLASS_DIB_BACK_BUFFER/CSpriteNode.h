#pragma once
#include "stdafx.h"

// ������â �ۺ� ����... ��ġ�⿡ �� �ʹ� ���귯....
class CSpriteNode {
public:
	
	BOOL _is_main_player;

	WCHAR _nickname[20];	///<�г���
	DIRECTION _direction;	///<����
	BOOL _is_main_client;

	DEPTH_PRIORITY _depth_attach_priority;	///<��������Ʈ ���� �� ���� �켱����. ���� ���� �տ� ��µȴ�.

	BOOL _is_fix_sprite;		   ///<��������Ʈ ���� ����.
	BOOL _is_player_sprite;		   ///<���� �÷��̾� ��Ʈ����Ʈ ����.
	BOOL _is_effect_sprite;		   ///<����Ʈ ��������Ʈ ����.

	CBaseObject *_cur_object_addr; ///<���� ������Ʈ �ּ�.
	TYPE_OBJECT _type_object;	   ///<������Ʈ Ÿ��.
	
	BOOL _use_destroy;		  ///<���� ����

	SPRITE_FIX _cur_fix_sprite;			  ///<���� ��������Ʈ�� ��츸.
	SPRITE_PLAYER _cur_player_sprite;		  ///<�÷��̾� ��������Ʈ�� ��츸.
	SPRITE_EFFECT _cur_effect_sprite;		  ///<����Ʈ ��������Ʈ�� ��츸.

	int _cur_x;			  ///<���� x��ǥ.
	int _cur_y;			  ///<���� y��ǥ.

	INT64 _option_value1;	  ///<�ɼ� ��1.
	INT64 _option_value2;	  ///<�ɼ� ��2.
	INT64 _option_value3;	  ///<�ɼ� ��3.


	//������
	CSpriteNode(CBaseObject *target_object, TYPE_OBJECT type_object = TYPE_OBJECT::OBJECT_NULL);



	//��������Ʈ ��带 ������Ʈ�Ѵ�.
	BOOL UpdateSpriteNode();
};