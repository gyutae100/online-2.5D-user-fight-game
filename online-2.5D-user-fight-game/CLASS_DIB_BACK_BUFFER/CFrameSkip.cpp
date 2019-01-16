#pragma once

#include "stdafx.h"
#include "CFrameSkip.h"

#pragma comment(lib, "winmm.lib") 

using namespace std;


/**
brief 	생성자

param max_ms_frame : 프레임 길이 설정한다.
*/
CFrameSkip::CFrameSkip(unsigned long max_ms_frame)
{

	//타이머 정밀도
	timeBeginPeriod(1);

	_max_ms_frame = max_ms_frame;

	_accumulate_ms_frame=0;

	_measure_ms_frame;

	_is_measure_tick = false;

	_is_over_frame = false;

	//고해상도 타이머의 주파수를 얻는다.
	QueryPerformanceFrequency(&f);

}

CFrameSkip::~CFrameSkip(){

}


/**
brief 	프레임 측정 시작 또는 프레임 측정 끝을 선택하는 함수이다.
*/
void CFrameSkip::SelectBeginOrEndTick() {

	if (false == _is_measure_tick) {

		BeginTick();
	}
	else {

		EndTick();
	}
}

/**
brief 	프레임 측정 시작한다.
*/
void CFrameSkip::BeginTick() {

	_is_measure_tick = true;

	QueryPerformanceCounter(&_start_tick);
}

/**
brief 	프레임 측정 완료한다.
*/
void CFrameSkip::EndTick() {

	_is_measure_tick = false;

	QueryPerformanceCounter(&_end_tick);

	_measure_ms_frame = (_end_tick.QuadPart - _start_tick.QuadPart) / (f.QuadPart / 1000);

	_accumulate_ms_frame += _measure_ms_frame;

	timeEndPeriod(1);
}

/**
brief 	프레임 오버 여부 체크한다.
*/
BOOL CFrameSkip::CheckOverFrame() {

	if (_max_ms_frame <= _accumulate_ms_frame) {

		_is_over_frame = true;
	}

	return _max_ms_frame <= _accumulate_ms_frame;
}

/**
brief 	프레임 스킵한다.
*/
void CFrameSkip::OnSleep() {

	if (TRUE == _is_over_frame) {
	
		_accumulate_ms_frame -= _measure_ms_frame;
		return;
	}

	_is_over_frame = false;
		
	Sleep(((_max_ms_frame - _accumulate_ms_frame))-1);

	_accumulate_ms_frame = 0;
}

/**
brief 	프레임 스킵 여부 판단 후 프레임 스킵이 필요한 경우 스킵한다.
*/
BOOL CFrameSkip::CheckFrameSkip() {

	if (TRUE == CheckOverFrame()) {

		_is_over_frame = true;

		_accumulate_ms_frame -= _measure_ms_frame;

		//슬립 후 다시 측정해줘야 한다.
		_is_measure_tick = false;
		BeginTick();

		return true;
	}
	else {

		_is_over_frame = false;
		OnSleep();

		//슬립 후 다시 측정해줘야 한다.
		_is_measure_tick = false;
		BeginTick();

		return false;
	}
}

/**
brief 	현재 FPS 값을 반환한다.
*/
BOOL CFrameSkip::GetFPS(float *out_fps) {

	static DWORD frameCount = 0;            //프레임 카운트수
	static float timeElapsed = 0.0f;            //흐른 시간
	static DWORD lastTime = timeGetTime();   //마지막 시간(temp변수)

	static float fps;

	DWORD curTime = timeGetTime();      //현재 시간
	float timeDelta = (curTime - lastTime)*0.001f;        //timeDelta(1번생성후 흐른 시간) 1초단위로 바꿔준다.

	timeElapsed += timeDelta;

	frameCount++;

	if (timeElapsed >= 1.0f)         //흐른시간이 1초이상이면 내가 하고싶은것 처리
	{
	

		//올림한다.
		fps = ceil((float)frameCount / timeElapsed) ;

		*out_fps = fps;

		frameCount = 0;

		lastTime = curTime;

		float tmp = timeElapsed - 1.0f;
		timeElapsed = tmp;

		return TRUE;
	}
	else
	{
		*out_fps = fps;

		lastTime = curTime;
		return FALSE;
	}
}
