#pragma once
#include "stdafx.h"
#include "CStreamSQ.h"
#include <iostream>


using namespace std;

/**
biref 링 버퍼를 정한 크기에 맞게 생성한다.

param sz_byte_buffer : 사용자 지정 버퍼 크기.
*/
void CStreamSQ::InitialRingBuffer(int sz_byte_buffer) {

	_ring_buffer = new char[sz_byte_buffer];

	//링 버퍼 공간 할당.
	memset(_ring_buffer, 0, sz_byte_buffer);

	//링 버퍼 총 사이즈.
	_total_sz_byte_ring_buffer = sz_byte_buffer;
}

/**
biref 생성자. 기본 링 버퍼 사이즈 100Byte로 생성.
*/
CStreamSQ::CStreamSQ(void) {

	//링 버퍼 공간 할당.
	InitialRingBuffer(DEFAULT_SIZE_OF_RING_BUFFER);

	_sz_byte_use = 0;
	_sz_byte_free = DEFAULT_SIZE_OF_RING_BUFFER;

	_idx_front = 0;
	_idx_rear = 0;

	total_line = 0;
	total_char = 0;
}


/**
biref 생성자. 사용자 지정 버퍼 사이즈로 생성.

param sz_byte_buffer : 사용자 지정 버퍼 크기.
*/
CStreamSQ::CStreamSQ(int sz_byte_buffer) {


	//링 버퍼 공간 할당.
	InitialRingBuffer(sz_byte_buffer);

	_sz_byte_use = 0;
	_sz_byte_free = sz_byte_buffer;

	_idx_front = 0;
	_idx_rear = 0;

	total_line = 0;
	total_char = 0;
}


/**
biref 총 버퍼 사이즈 반환.
return : 총 버퍼 사이즈.
*/
int	CStreamSQ::GetTotalBufferSize(void) {

	return _total_sz_byte_ring_buffer;
}


/**
brief 현재 사용 중인 버퍼 용량 얻기.
return : 현재 사용 중인 버퍼 용량.
*/
int	CStreamSQ::GetInsideSizeOfUse(void) {

	return _sz_byte_use - SIZE_OF_BLINK;
}
/**
brief 현재 사용 중인 버퍼 용량 얻기.
return : 현재 사용 중인 버퍼 용량.
*/
int	CStreamSQ::GetSizeOfUse(void) {

	return _sz_byte_use;
}



/**
brief 남은 버퍼 용량 얻기.
return : 남은 버퍼 용량.
*/
int	CStreamSQ::GetSizeOfFree(void) {

	return _total_sz_byte_ring_buffer - _sz_byte_use - SIZE_OF_BLINK;
}


/**
brief 버퍼 포인터로 외부에서 한방에 읽을 쓸 수 있는 길이.

Return : 버퍼 포인터로 외부에서 한방에 읽을 쓸 수 있는 길이.
*/
int	CStreamSQ::GetUnBrokenGetSizeOfBuffer(void) {

	if (_idx_front > _idx_rear) {

		return _total_sz_byte_ring_buffer - _idx_front;

	}
	else if (_idx_front < _idx_rear) {


		return _idx_rear - _idx_front;
	}
	else if (_idx_front == _idx_rear) {

		return 0;
	}
}

/**
brief 버퍼 포인터로 외부에서 한방에 삽입 할 수 있는 길이.

Return : 버퍼 포인터로 외부에서 한방에 삽입 할 수 있는 길이.
*/
int	CStreamSQ::GetUnBrokenPutSizeOfBuffer(void) {

	if (_idx_front > _idx_rear) {

		return _idx_front - _idx_rear;
	}
	else if (_idx_front < _idx_rear) {

		return _total_sz_byte_ring_buffer - _idx_rear;
	}

	else if (_idx_front == _idx_rear) {

		return _total_sz_byte_ring_buffer - _idx_rear;
	}
}


/**
brief  데이타 삽입한다.

Param  in_data 데이터 포인터.
Param sz_byte_data 데이터 크기.

Return : 삽입에 성공한 데이터 크기.
*/
int	CStreamSQ::Put(char *in_data, int sz_byte_data) {

	//디버기 용도.
	static int xxx = 0;
	if (xxx == 1) {

		int tmp = 3;
	}
	xxx++;

	//링 버퍼 가득 찬 여부 체크한다.
	if (true == IsFull()) {

		return 0;
	}

	//삽입 용량 재설정한다.
	if (sz_byte_data > GetSizeOfFree()) {

		sz_byte_data = GetSizeOfFree();
	}

	//==============================================================================
	//2 번 삽입 여부 체크한다.
	//==============================================================================
	int next_idx_rear = GetNextIdx(_idx_rear, sz_byte_data);
	if (_idx_front > next_idx_rear) {

		//첫 번째 memcpy 저장 용량 공간 체크한다.
		int sz_left_terminal_partition = GetUnBrokenPutSizeOfBuffer();

		//두번 memcpy 여부 체크한다.
		if (sz_byte_data > sz_left_terminal_partition) {

			//첫 번째 memcpy 진행한다.
			memcpy(_ring_buffer + _idx_rear, in_data, sz_left_terminal_partition);
			_idx_rear = GetNextIdx(_idx_rear, sz_left_terminal_partition);

			//두번 째 memcpy 예정 용량을 구한다.
			int sz_can_put = sz_byte_data - sz_left_terminal_partition;

			//두 번째 memcpy 진행한다.
			memcpy(_ring_buffer + _idx_rear, in_data + sz_left_terminal_partition, sz_can_put);
			_idx_rear = GetNextIdx(_idx_rear, sz_can_put);

			//삽입 용량 및 남은 용량 재설정한다.
			_sz_byte_use += sz_byte_data;
			_sz_byte_free = _total_sz_byte_ring_buffer - GetInsideSizeOfUse();

			//삽입 용량을 반환한다.
			return sz_byte_data;
		}
	}

	//==============================================================================
	//1 번 삽입 여부 체크한다.
	//==============================================================================
	//memcpy 1번만 진행한다.
	memcpy(_ring_buffer + _idx_rear, in_data, sz_byte_data);
	_idx_rear = GetNextIdx(_idx_rear, sz_byte_data);

	//삽입 용량 및 남은 용량 재설정한다.
	_sz_byte_use += sz_byte_data;
	_sz_byte_free = _total_sz_byte_ring_buffer - GetInsideSizeOfUse();

	//삽입 용량을 반환한다
	return sz_byte_data;
}


/**
brief  데이타 삭제한다.

Param  out_data 반환 대상 데이터 포인터.
Param  sz_byte_data 삭제 데이터 크기.

Return : 삭제에 성공한 데이터 크기.
*/
int	CStreamSQ::Get(char *out_data, int sz_byte_data) {


	if (_sz_byte_use == 0) {

		return 0;
	}

	//삭제 용량 재설정한다.
	if (_sz_byte_use < sz_byte_data) {

		sz_byte_data = _sz_byte_use;
	}

	//==============================================================================
	//2 번 삭제 여부 체크한다.
	//==============================================================================
	if (_idx_rear < _idx_front) {

		//첫 번째 memcpy 용량을 체크한다.
		int sz_left_terminal_partition = 0;
		sz_left_terminal_partition = GetUnBrokenGetSizeOfBuffer();

		//두번 memcpy 여부 체크한다.
		if (sz_byte_data > sz_left_terminal_partition) {

			//첫 번째 memcpy를 진행한다.
			memcpy(out_data, _ring_buffer + _idx_front, sz_left_terminal_partition);
			_idx_front = GetNextIdx(_idx_front, sz_left_terminal_partition);

			//두 번째 memcpy를 진행한다.
			memcpy(out_data + sz_left_terminal_partition, _ring_buffer + _idx_front, sz_byte_data - sz_left_terminal_partition);
			_idx_front = GetNextIdx(_idx_front, sz_byte_data - sz_left_terminal_partition);

			//삽입 용량 및 남은 용량을 재설정한다.
			_sz_byte_use -= sz_byte_data;
			_sz_byte_free = _total_sz_byte_ring_buffer - GetInsideSizeOfUse();

			return sz_byte_data;
		}
	}

	//==============================================================================
	//1 번 삭제 여부 체크한다.
	//==============================================================================
	memcpy(out_data, _ring_buffer + _idx_front, sz_byte_data);
	_idx_front = GetNextIdx(_idx_front, sz_byte_data);

	//삭제 용량 및 남은 용량 재설정한다.
	_sz_byte_use -= sz_byte_data;
	_sz_byte_free = _total_sz_byte_ring_buffer - GetInsideSizeOfUse();

	return sz_byte_data;


}


/**
brief  데이타 읽기.

Param  out_data 읽기 대상 데이터 포인터.
Param sz_byte_data 읽기 데이터 크기.

Return : 읽기에 성공한 데이터 크기.
*/
int	CStreamSQ::Peek(char *out_data, int sz_byte_data) {

	if (_sz_byte_use == 0) {

		return 0;
	}

	//삭제 용량 재설정한다.
	if (_sz_byte_use < sz_byte_data) {

		sz_byte_data = _sz_byte_use;
	}

	//peek용
	int _idx_peek = this->_idx_front;

	//==============================================================================
	//2 번 읽기 여부 체크한다.
	//==============================================================================
	if (_idx_rear < _idx_peek) {

		//첫 번째 memcpy 용량을 체크한다.
		int sz_left_terminal_partition = 0;
		sz_left_terminal_partition = GetUnBrokenGetSizeOfBuffer();

		//두번 memcpy 여부 체크한다.
		if (sz_byte_data > sz_left_terminal_partition) {

			//첫 번째 memcpy를 진행한다.
			memcpy(out_data, _ring_buffer + _idx_peek, sz_left_terminal_partition);
			_idx_peek = GetNextIdx(_idx_peek, sz_left_terminal_partition);

			//두 번째 memcpy를 진행한다.
			memcpy(out_data + sz_left_terminal_partition, _ring_buffer + _idx_peek, sz_byte_data - sz_left_terminal_partition);

			return sz_byte_data;
		}
	}

	//==============================================================================
	//1 번 읽기 여부 체크한다.
	//==============================================================================
	memcpy(out_data, _ring_buffer + _idx_peek, sz_byte_data);

	return sz_byte_data;
}

/**
brief 링 버퍼가 비어 있는 여부 체크.

Return : 링버퍼가 비어 있는 여부.
*/
bool CStreamSQ::IsEmpty() {

	if (_idx_rear == _idx_front) {

		return true;
	}
	else {

		return false;
	}
}

/**
brief 링 버퍼가 가득 차있는 여부 체크.

Return :링버퍼가 가득 차 있는 여부.
*/
bool CStreamSQ::IsFull() {

	if (((_idx_rear + 1) % _total_sz_byte_ring_buffer) == _idx_front) {

		return true;
	}
	else {

		return false;
	}
}

void CStreamSQ::MoveRearIdx(int len_byte_move) {

	_sz_byte_use += len_byte_move;
	_sz_byte_free -= len_byte_move;
	_idx_rear = (_idx_rear + len_byte_move) % _total_sz_byte_ring_buffer;
}

void	CStreamSQ::MoveFrontIdx(int len_byte_move) {

	_sz_byte_use -= len_byte_move;
	_sz_byte_free += len_byte_move;
	_idx_front = (_idx_front + len_byte_move) % _total_sz_byte_ring_buffer;
}


/**
brief 다음 인덱스 값을 반환한다.
*/
int CStreamSQ::GetNextIdx(int target_idx, int len_byte_move) {

	return (target_idx + len_byte_move) % _total_sz_byte_ring_buffer;
}


/**
버퍼의 모든 데이타 삭제.

Parameters: 없음.
Return : 없음.
*/
void CStreamSQ::ClearRingBuffer(void) {

	_idx_front = 0;
	_idx_rear = 0;

	_sz_byte_use = 0;
	_sz_byte_free = _total_sz_byte_ring_buffer;

	return;
}


/**
버퍼의 포인터 얻음.

Parameters: 없음.
Return : (char *)버퍼 포인터.
*/
char	*CStreamSQ::GetPtrOfBuffer(void) {

	return _ring_buffer;
}

/**
버퍼의 Front 포인터 얻음.

Parameters: 없음.
Return : (char *)버퍼 포인터.
*/
char	*CStreamSQ::GetFrontPtrOfBuffer(void) {

	return _ring_buffer + _idx_front;
}

/**
버퍼의 RearPos 포인터 얻음.

Parameters: 없음.
Return : (char *)버퍼 포인터.
*/
char	*CStreamSQ::GetRearPtrOfBuffer(void) {

	return _ring_buffer + _idx_rear;
}




void CStreamSQ::DisplayInfo1(void) {

	char info[1000];
	sprintf_s(info, "title get / front = %6d / rear = %6d /  use = %6d / free = %6d / use+free= %6d", _idx_front, _idx_rear, GetSizeOfUse(), GetSizeOfFree(), GetSizeOfUse() + GetSizeOfFree());

	//char info2[1000];
	//strcpy_s(info2, _ring_buffer);
	//info2[5] = NULL;
	//strcat_s(info, info2);

	system(info);
}

void CStreamSQ::DisplayInfo2(void) {

	char info[1000];
	sprintf_s(info, "title get / front = %6d / rear = %6d /  use = %6d / free = %6d / use+free= %6d", _idx_front, _idx_rear, GetSizeOfUse(), GetSizeOfFree(), GetSizeOfUse() + GetSizeOfFree());

	//char info2[1000];
	//strcpy_s(info2, _ring_buffer);
	//info2[5] = NULL;

	//strcat_s(info, info2);

	system(info);
}