
#pragma once
#include "stdafx.h"
#include <iostream>

using namespace std;

/**
class 링버퍼 클래스

brief
*/
class CStreamSQ {

private:
	enum OPTION_RING_BUFFER {

		DEFAULT_SIZE_OF_RING_BUFFER = 20000,	 ///<링 버퍼 디폴트 사이즈
		SIZE_OF_BLINK = 1
	};

	char * _ring_buffer;			///<링 버퍼 

	int _total_sz_byte_ring_buffer;	///<링 버퍼 총 사이즈
	int _sz_byte_use;				///<현재 사용 중인 링 버퍼 용량.
	int _sz_byte_free;				///<현재 남은 링버퍼 용량

	int _idx_front;
	int _idx_rear;

	int _sz_input_data;				///<디버깅 용
	double total_line;				///<디버깅 용
	double total_char;				///<디버깅 용


private:

	void InitialRingBuffer(int sz_byte_buffer);	///<버퍼를 정한 크기에 맞게 생성한다.

public:


	CStreamSQ(void);				///<생성자. 기본 버퍼 사이즈 100Byte로 생성.

	CStreamSQ(int sz_byte_buffer); ///<생성자. 사용자 지정 버퍼 사이즈로 생성.

	int	GetTotalBufferSize(void);	 ///< 할당 된 버퍼 사이즈 반환.
	int	GetInsideSizeOfUse(void);			 ///<현재 사용 중인 버퍼 용량 얻기.
	int	GetSizeOfFree(void);		 ///< 버퍼에 남은 용량 얻기.
	int GetSizeOfUse(void);

	int	GetUnBrokenGetSizeOfBuffer(void); ///<버퍼 포인터로 외부에서 한방에 읽고, 쓸 수 있는 길이.
	int	GetUnBrokenPutSizeOfBuffer(void); ///<버퍼 포인터로 외부에서 한방에 읽고, 쓸 수 있는 길이.


	int	Put(char *in_data, int sz_data);	///< WritePos 에 데이타 넣음.
	int	Get(char *out_data, int sz_data);	///<ReadPos 에서 데이타 가져옴.ReadPos 이동
	int	Peek(char *out_data, int sz_data);  ///< ReadPos 에서 데이타 읽어옴.ReadPos 고정.


	bool IsEmpty();	///<링 버퍼가 비어 있는 여부 체크.
	bool IsFull();	///<링 버퍼가 가득 차있는 여부 체크.



	int GetNextIdx(int target_idx, int len_byte_move);	 ///<다음 인덱스 값을 반환한다.
	void MoveRearIdx(int sz_data);						 ///<원하는 길이만큼 읽기위치 에서 삭제 / 쓰기 위치 이동
	void MoveFrontIdx(int sz_data);					 ///<원하는 길이만큼 읽기위치 에서 삭제 / 쓰기 위치 이동


	void ClearRingBuffer(void);			 ///<버퍼의 모든 데이타 삭제


	char *GetPtrOfBuffer(void);			 ///<버퍼의 포인터 얻음.
	char *GetFrontPtrOfBuffer(void);	 ///<버퍼의 Front 포인터 얻음.
	char *GetRearPtrOfBuffer(void);		 ///<버퍼의 RearPos 포인터 얻음.

	void DisplayInfo1(void);				 ///<현재 스트림 버퍼 내부 정보를 콘솔창 타이틀 창에 표기한다.
	void DisplayInfo2(void);				 ///<현재 스트림 버퍼 내부 정보를 콘솔창 타이틀 창에 표기한다.


};