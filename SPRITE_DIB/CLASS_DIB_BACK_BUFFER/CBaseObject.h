#pragma once

/**
@class CBaseObject

@brief 프레임에 따라 변화하는 스프라이트 애니메이션을 가진 오브젝트의 기본 클래스이다. 상속하여 이용한다.
	   스프라이트 애니메이션 효과 및 애니메이션의 각각의 프레임 제어를 담당한다.
*/
class CBaseObject {

public:

	DIRECTION _last_direction;		///< 마지막 방향. 좌, 우 존재.

	BOOL _is_main_player;

	WCHAR _nickname[20];	///<닉네임


private:

protected:


	int _cur_x;					///<현재 좌표x.
	int _cur_y;					///<현재 좌표y.
	TYPE_OBJECT _type_object;	///< 해당 오브젝트 타입.
	
	ACTION_PLAYER  _cur_action;	///< 현재 진행중인 액션.
	ACTION_PLAYER  _last_action;///< 마지막 실행한 액션.

	//플레이어 스프라이트 저장.
	SPRITE_PLAYER _first_player_sprite;	///<시작 스프라이트.
	SPRITE_PLAYER _cur_player_sprite;	///<현재 진행중인 스프라이트.
	SPRITE_PLAYER _last_player_sprite;	///<마지막 스프라이트.

	//이펙트 스프라이트 저장.
	SPRITE_EFFECT _first_effect_sprite;	///<시작 스프라이트.
	SPRITE_EFFECT _cur_effect_sprite;	///<현재 진행중인 스프라이트.
	SPRITE_EFFECT _last_effect_sprite;	///<마지막 스프라이트.


	int _max_delay_each_frame;	///<마지막 프레임.
	int _cur_frame;				///<현재 프레임.
	BOOL _is_end_frame;			///<마지막 프레임 여부. 





public:

	//생성자
	CBaseObject();

	//소멸자
	virtual ~CBaseObject();

	//해당 오브젝트 파괴한다. 소멸자 인터페이스이다.
	BOOL DestroyThisObject();

	//현재 스프라이트 장면을 반환한다.
	SPRITE_PLAYER GetCurPlayerSprite();

	SPRITE_EFFECT GetCurEffectSprite();


	//오브젝트 객체의 동작을 실행하는 가상함수용 콜백함수이다.
	virtual BOOL  OnAction()=0;

	//체력 반환한다.
	virtual int GetHp() = 0;

	//해당 오브젝트 타입 반환한다.
	TYPE_OBJECT GetObjectType();

public:
	

	//좌표 xy를 설정한다.
	void SetCurXY(int cur_x, int cur_y);

	//좌표 xy를 반환한다.
	void GetCurXY(int *out_cur_x, int *out_cur_y);





protected:

	//좌우 대칭된 스프라이트 애니메이션으로 변경하되 현재 스프라이트 장면은 이전 좌우 대칭된 장면 이후의 값을 적용한다.
	void SetSpriteFilmBySymetry();

	//오버라이딩. 스프라이트 필름 단위로 저장한다.
	BOOL SetSpriteFilmFrom(IDX_SPRITE_PLAYER_FILM idx_cur_player_sprite_PLAYER_FILM);

	BOOL SetSpriteFilmFrom(IDX_SPRITE_EFFECT idx_cur_player_sprite_EFFECT_FILM);

	//오버라이딩. 스프라이트 필름 단위로 저장한다.
	BOOL SetSpriteFilmFrom(SPRITE_PLAYER first_sprite, SPRITE_PLAYER last_sprite, int delay_last_frame);

	//다음 프레임을 카운트한다.
	BOOL OnNextFrame();

	//이펙트용 프레임 카운터.
	BOOL OnNextFrameByEffect();

	//마지막 프레임 여부를 확인한다.
	BOOL IsEndSpriteFilm();
};
