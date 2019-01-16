#pragma once
#include "stdafx.h"

/**
class CFrameSkip

brief ������ ��ŵ Ŭ����. FPS 50 �������� ��� ��� ������ 20ms�ȿ� ó���Ǿ�� �Ѵ�. �� �� ���� ��ο� �������� 20ms�� �ʰ��ϴ� ��� Ȥ�� ��ο� �� 20ms�� �ʰ��ϴ� ���
	  ���� ��ο�� �ʰ��з��� 20ms�̸� �� �� ���� �ݺ��Ѵ�.

*/
class CFrameSkip {

private:

	int _times_tick;				///<�ð� ������ ���̴� ����.


	LARGE_INTEGER f;

	LARGE_INTEGER _end_tick;		///<���� ������ ƽ.
	LARGE_INTEGER _start_tick;		///<���� ���� ƽ.

	unsigned long _max_ms_frame;	///<�ִ� ������.
	
	__int64 _accumulate_ms_frame;	///<���� ���� �ð�.

	__int64 _measure_ms_frame;		///<���� �ð�. ���� ������ ƽ- ���� ���� ƽ

	BOOL _is_measure_tick;			///<���� ������ ��� true. ���� ���� ��� false

	unsigned long _tmp_test;		///<�׽�Ʈ�� ����1		   
	unsigned long _tmp_test2;		///<�׽�Ʈ�� ����2			


	BOOL _is_over_frame; ///< true�� ������ ��ŵ�Ѵ�.


public:

	//������
	CFrameSkip(unsigned long max_fps);

	//�Ҹ���
	~CFrameSkip();

	//������ ���� ���� �Ǵ� ������ ���� ���� �����ϴ� �Լ��̴�.
	void SelectBeginOrEndTick();

	//���� �����Ѵ�.
	void BeginTick();

	//������ �Ϸ� �Ѵ�.
	void EndTick();

	//������ �������� üũ�Ѵ�.
	BOOL CheckOverFrame();

	//������ ���߱� ���� ���� �ð��� �����Ѵ�.
	void OnSleep();

	//������ ��ŵ ���� �Ǵ� �� ������ ��ŵ�� �ʿ��� ��� ��ŵ�Ѵ�.
	BOOL CheckFrameSkip();

	//���� FPS ���� ��ȯ�Ѵ�.
	BOOL GetFPS(float *out_fps);
};


