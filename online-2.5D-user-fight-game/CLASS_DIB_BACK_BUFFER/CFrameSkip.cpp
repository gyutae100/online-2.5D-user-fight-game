#pragma once

#include "stdafx.h"
#include "CFrameSkip.h"

#pragma comment(lib, "winmm.lib") 

using namespace std;


/**
brief 	������

param max_ms_frame : ������ ���� �����Ѵ�.
*/
CFrameSkip::CFrameSkip(unsigned long max_ms_frame)
{

	//Ÿ�̸� ���е�
	timeBeginPeriod(1);

	_max_ms_frame = max_ms_frame;

	_accumulate_ms_frame=0;

	_measure_ms_frame;

	_is_measure_tick = false;

	_is_over_frame = false;

	//���ػ� Ÿ�̸��� ���ļ��� ��´�.
	QueryPerformanceFrequency(&f);

}

CFrameSkip::~CFrameSkip(){

}


/**
brief 	������ ���� ���� �Ǵ� ������ ���� ���� �����ϴ� �Լ��̴�.
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
brief 	������ ���� �����Ѵ�.
*/
void CFrameSkip::BeginTick() {

	_is_measure_tick = true;

	QueryPerformanceCounter(&_start_tick);
}

/**
brief 	������ ���� �Ϸ��Ѵ�.
*/
void CFrameSkip::EndTick() {

	_is_measure_tick = false;

	QueryPerformanceCounter(&_end_tick);

	_measure_ms_frame = (_end_tick.QuadPart - _start_tick.QuadPart) / (f.QuadPart / 1000);

	_accumulate_ms_frame += _measure_ms_frame;

	timeEndPeriod(1);
}

/**
brief 	������ ���� ���� üũ�Ѵ�.
*/
BOOL CFrameSkip::CheckOverFrame() {

	if (_max_ms_frame <= _accumulate_ms_frame) {

		_is_over_frame = true;
	}

	return _max_ms_frame <= _accumulate_ms_frame;
}

/**
brief 	������ ��ŵ�Ѵ�.
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
brief 	������ ��ŵ ���� �Ǵ� �� ������ ��ŵ�� �ʿ��� ��� ��ŵ�Ѵ�.
*/
BOOL CFrameSkip::CheckFrameSkip() {

	if (TRUE == CheckOverFrame()) {

		_is_over_frame = true;

		_accumulate_ms_frame -= _measure_ms_frame;

		//���� �� �ٽ� ��������� �Ѵ�.
		_is_measure_tick = false;
		BeginTick();

		return true;
	}
	else {

		_is_over_frame = false;
		OnSleep();

		//���� �� �ٽ� ��������� �Ѵ�.
		_is_measure_tick = false;
		BeginTick();

		return false;
	}
}

/**
brief 	���� FPS ���� ��ȯ�Ѵ�.
*/
BOOL CFrameSkip::GetFPS(float *out_fps) {

	static DWORD frameCount = 0;            //������ ī��Ʈ��
	static float timeElapsed = 0.0f;            //�帥 �ð�
	static DWORD lastTime = timeGetTime();   //������ �ð�(temp����)

	static float fps;

	DWORD curTime = timeGetTime();      //���� �ð�
	float timeDelta = (curTime - lastTime)*0.001f;        //timeDelta(1�������� �帥 �ð�) 1�ʴ����� �ٲ��ش�.

	timeElapsed += timeDelta;

	frameCount++;

	if (timeElapsed >= 1.0f)         //�帥�ð��� 1���̻��̸� ���� �ϰ������ ó��
	{
	

		//�ø��Ѵ�.
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
