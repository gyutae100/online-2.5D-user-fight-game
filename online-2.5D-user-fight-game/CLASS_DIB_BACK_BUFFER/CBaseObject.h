#pragma once

/**
@class CBaseObject

@brief �����ӿ� ���� ��ȭ�ϴ� ��������Ʈ �ִϸ��̼��� ���� ������Ʈ�� �⺻ Ŭ�����̴�. ����Ͽ� �̿��Ѵ�.
	   ��������Ʈ �ִϸ��̼� ȿ�� �� �ִϸ��̼��� ������ ������ ��� ����Ѵ�.
*/
class CBaseObject {

public:

	DIRECTION _last_direction;		///< ������ ����. ��, �� ����.

	BOOL _is_main_player;

	WCHAR _nickname[20];	///<�г���


private:

protected:


	int _cur_x;					///<���� ��ǥx.
	int _cur_y;					///<���� ��ǥy.
	TYPE_OBJECT _type_object;	///< �ش� ������Ʈ Ÿ��.
	
	ACTION_PLAYER  _cur_action;	///< ���� �������� �׼�.
	ACTION_PLAYER  _last_action;///< ������ ������ �׼�.

	//�÷��̾� ��������Ʈ ����.
	SPRITE_PLAYER _first_player_sprite;	///<���� ��������Ʈ.
	SPRITE_PLAYER _cur_player_sprite;	///<���� �������� ��������Ʈ.
	SPRITE_PLAYER _last_player_sprite;	///<������ ��������Ʈ.

	//����Ʈ ��������Ʈ ����.
	SPRITE_EFFECT _first_effect_sprite;	///<���� ��������Ʈ.
	SPRITE_EFFECT _cur_effect_sprite;	///<���� �������� ��������Ʈ.
	SPRITE_EFFECT _last_effect_sprite;	///<������ ��������Ʈ.


	int _max_delay_each_frame;	///<������ ������.
	int _cur_frame;				///<���� ������.
	BOOL _is_end_frame;			///<������ ������ ����. 





public:

	//������
	CBaseObject();

	//�Ҹ���
	virtual ~CBaseObject();

	//�ش� ������Ʈ �ı��Ѵ�. �Ҹ��� �������̽��̴�.
	BOOL DestroyThisObject();

	//���� ��������Ʈ ����� ��ȯ�Ѵ�.
	SPRITE_PLAYER GetCurPlayerSprite();

	SPRITE_EFFECT GetCurEffectSprite();


	//������Ʈ ��ü�� ������ �����ϴ� �����Լ��� �ݹ��Լ��̴�.
	virtual BOOL  OnAction()=0;

	//ü�� ��ȯ�Ѵ�.
	virtual int GetHp() = 0;

	//�ش� ������Ʈ Ÿ�� ��ȯ�Ѵ�.
	TYPE_OBJECT GetObjectType();

public:
	

	//��ǥ xy�� �����Ѵ�.
	void SetCurXY(int cur_x, int cur_y);

	//��ǥ xy�� ��ȯ�Ѵ�.
	void GetCurXY(int *out_cur_x, int *out_cur_y);





protected:

	//�¿� ��Ī�� ��������Ʈ �ִϸ��̼����� �����ϵ� ���� ��������Ʈ ����� ���� �¿� ��Ī�� ��� ������ ���� �����Ѵ�.
	void SetSpriteFilmBySymetry();

	//�������̵�. ��������Ʈ �ʸ� ������ �����Ѵ�.
	BOOL SetSpriteFilmFrom(IDX_SPRITE_PLAYER_FILM idx_cur_player_sprite_PLAYER_FILM);

	BOOL SetSpriteFilmFrom(IDX_SPRITE_EFFECT idx_cur_player_sprite_EFFECT_FILM);

	//�������̵�. ��������Ʈ �ʸ� ������ �����Ѵ�.
	BOOL SetSpriteFilmFrom(SPRITE_PLAYER first_sprite, SPRITE_PLAYER last_sprite, int delay_last_frame);

	//���� �������� ī��Ʈ�Ѵ�.
	BOOL OnNextFrame();

	//����Ʈ�� ������ ī����.
	BOOL OnNextFrameByEffect();

	//������ ������ ���θ� Ȯ���Ѵ�.
	BOOL IsEndSpriteFilm();
};
