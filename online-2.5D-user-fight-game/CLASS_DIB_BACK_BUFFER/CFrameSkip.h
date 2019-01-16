#pragma once
#include "stdafx.h"

/**
class CFrameSkip

brief 프레임 스킵 클래스. FPS 50 프레임인 경우 모든 로직이 20ms안에 처리되어야 한다. 이 때 만일 드로우 이전까지 20ms을 초과하는 경우 혹은 드로우 시 20ms를 초과하는 경우
	  다음 드로우는 초과분량이 20ms미만 일 때 까지 반복한다.

*/
class CFrameSkip {

private:

	int _times_tick;				///<시간 측정에 쓰이는 변수.


	LARGE_INTEGER f;

	LARGE_INTEGER _end_tick;		///<측정 마지막 틱.
	LARGE_INTEGER _start_tick;		///<측정 시작 틱.

	unsigned long _max_ms_frame;	///<최대 프레임.
	
	__int64 _accumulate_ms_frame;	///<누적 측정 시간.

	__int64 _measure_ms_frame;		///<측정 시간. 측정 마지막 틱- 측정 시작 틱

	BOOL _is_measure_tick;			///<측정 시작한 경우 true. 측정 끝난 경우 false

	unsigned long _tmp_test;		///<테스트용 변수1		   
	unsigned long _tmp_test2;		///<테스트용 변수2			


	BOOL _is_over_frame; ///< true시 프레임 스킵한다.


public:

	//생성자
	CFrameSkip(unsigned long max_fps);

	//소멸자
	~CFrameSkip();

	//프레임 측정 시작 또는 프레임 측정 끝을 선택하는 함수이다.
	void SelectBeginOrEndTick();

	//측정 시작한다.
	void BeginTick();

	//측정을 완료 한다.
	void EndTick();

	//프레임 오버여부 체크한다.
	BOOL CheckOverFrame();

	//프레임 맞추기 위해 남는 시간은 슬립한다.
	void OnSleep();

	//프레임 스킵 여부 판단 후 프레임 스킵이 필요한 경우 스킵한다.
	BOOL CheckFrameSkip();

	//현재 FPS 값을 반환한다.
	BOOL GetFPS(float *out_fps);
};


