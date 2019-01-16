#pragma once
#include "stdafx.h"
#include "CStreamSQ.h"
#include <iostream>


using namespace std;

/**
biref �� ���۸� ���� ũ�⿡ �°� �����Ѵ�.

param sz_byte_buffer : ����� ���� ���� ũ��.
*/
void CStreamSQ::InitialRingBuffer(int sz_byte_buffer) {

	_ring_buffer = new char[sz_byte_buffer];

	//�� ���� ���� �Ҵ�.
	memset(_ring_buffer, 0, sz_byte_buffer);

	//�� ���� �� ������.
	_total_sz_byte_ring_buffer = sz_byte_buffer;
}

/**
biref ������. �⺻ �� ���� ������ 100Byte�� ����.
*/
CStreamSQ::CStreamSQ(void) {

	//�� ���� ���� �Ҵ�.
	InitialRingBuffer(DEFAULT_SIZE_OF_RING_BUFFER);

	_sz_byte_use = 0;
	_sz_byte_free = DEFAULT_SIZE_OF_RING_BUFFER;

	_idx_front = 0;
	_idx_rear = 0;

	total_line = 0;
	total_char = 0;
}


/**
biref ������. ����� ���� ���� ������� ����.

param sz_byte_buffer : ����� ���� ���� ũ��.
*/
CStreamSQ::CStreamSQ(int sz_byte_buffer) {


	//�� ���� ���� �Ҵ�.
	InitialRingBuffer(sz_byte_buffer);

	_sz_byte_use = 0;
	_sz_byte_free = sz_byte_buffer;

	_idx_front = 0;
	_idx_rear = 0;

	total_line = 0;
	total_char = 0;
}


/**
biref �� ���� ������ ��ȯ.
return : �� ���� ������.
*/
int	CStreamSQ::GetTotalBufferSize(void) {

	return _total_sz_byte_ring_buffer;
}


/**
brief ���� ��� ���� ���� �뷮 ���.
return : ���� ��� ���� ���� �뷮.
*/
int	CStreamSQ::GetInsideSizeOfUse(void) {

	return _sz_byte_use - SIZE_OF_BLINK;
}
/**
brief ���� ��� ���� ���� �뷮 ���.
return : ���� ��� ���� ���� �뷮.
*/
int	CStreamSQ::GetSizeOfUse(void) {

	return _sz_byte_use;
}



/**
brief ���� ���� �뷮 ���.
return : ���� ���� �뷮.
*/
int	CStreamSQ::GetSizeOfFree(void) {

	return _total_sz_byte_ring_buffer - _sz_byte_use - SIZE_OF_BLINK;
}


/**
brief ���� �����ͷ� �ܺο��� �ѹ濡 ���� �� �� �ִ� ����.

Return : ���� �����ͷ� �ܺο��� �ѹ濡 ���� �� �� �ִ� ����.
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
brief ���� �����ͷ� �ܺο��� �ѹ濡 ���� �� �� �ִ� ����.

Return : ���� �����ͷ� �ܺο��� �ѹ濡 ���� �� �� �ִ� ����.
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
brief  ����Ÿ �����Ѵ�.

Param  in_data ������ ������.
Param sz_byte_data ������ ũ��.

Return : ���Կ� ������ ������ ũ��.
*/
int	CStreamSQ::Put(char *in_data, int sz_byte_data) {

	//����� �뵵.
	static int xxx = 0;
	if (xxx == 1) {

		int tmp = 3;
	}
	xxx++;

	//�� ���� ���� �� ���� üũ�Ѵ�.
	if (true == IsFull()) {

		return 0;
	}

	//���� �뷮 �缳���Ѵ�.
	if (sz_byte_data > GetSizeOfFree()) {

		sz_byte_data = GetSizeOfFree();
	}

	//==============================================================================
	//2 �� ���� ���� üũ�Ѵ�.
	//==============================================================================
	int next_idx_rear = GetNextIdx(_idx_rear, sz_byte_data);
	if (_idx_front > next_idx_rear) {

		//ù ��° memcpy ���� �뷮 ���� üũ�Ѵ�.
		int sz_left_terminal_partition = GetUnBrokenPutSizeOfBuffer();

		//�ι� memcpy ���� üũ�Ѵ�.
		if (sz_byte_data > sz_left_terminal_partition) {

			//ù ��° memcpy �����Ѵ�.
			memcpy(_ring_buffer + _idx_rear, in_data, sz_left_terminal_partition);
			_idx_rear = GetNextIdx(_idx_rear, sz_left_terminal_partition);

			//�ι� ° memcpy ���� �뷮�� ���Ѵ�.
			int sz_can_put = sz_byte_data - sz_left_terminal_partition;

			//�� ��° memcpy �����Ѵ�.
			memcpy(_ring_buffer + _idx_rear, in_data + sz_left_terminal_partition, sz_can_put);
			_idx_rear = GetNextIdx(_idx_rear, sz_can_put);

			//���� �뷮 �� ���� �뷮 �缳���Ѵ�.
			_sz_byte_use += sz_byte_data;
			_sz_byte_free = _total_sz_byte_ring_buffer - GetInsideSizeOfUse();

			//���� �뷮�� ��ȯ�Ѵ�.
			return sz_byte_data;
		}
	}

	//==============================================================================
	//1 �� ���� ���� üũ�Ѵ�.
	//==============================================================================
	//memcpy 1���� �����Ѵ�.
	memcpy(_ring_buffer + _idx_rear, in_data, sz_byte_data);
	_idx_rear = GetNextIdx(_idx_rear, sz_byte_data);

	//���� �뷮 �� ���� �뷮 �缳���Ѵ�.
	_sz_byte_use += sz_byte_data;
	_sz_byte_free = _total_sz_byte_ring_buffer - GetInsideSizeOfUse();

	//���� �뷮�� ��ȯ�Ѵ�
	return sz_byte_data;
}


/**
brief  ����Ÿ �����Ѵ�.

Param  out_data ��ȯ ��� ������ ������.
Param  sz_byte_data ���� ������ ũ��.

Return : ������ ������ ������ ũ��.
*/
int	CStreamSQ::Get(char *out_data, int sz_byte_data) {


	if (_sz_byte_use == 0) {

		return 0;
	}

	//���� �뷮 �缳���Ѵ�.
	if (_sz_byte_use < sz_byte_data) {

		sz_byte_data = _sz_byte_use;
	}

	//==============================================================================
	//2 �� ���� ���� üũ�Ѵ�.
	//==============================================================================
	if (_idx_rear < _idx_front) {

		//ù ��° memcpy �뷮�� üũ�Ѵ�.
		int sz_left_terminal_partition = 0;
		sz_left_terminal_partition = GetUnBrokenGetSizeOfBuffer();

		//�ι� memcpy ���� üũ�Ѵ�.
		if (sz_byte_data > sz_left_terminal_partition) {

			//ù ��° memcpy�� �����Ѵ�.
			memcpy(out_data, _ring_buffer + _idx_front, sz_left_terminal_partition);
			_idx_front = GetNextIdx(_idx_front, sz_left_terminal_partition);

			//�� ��° memcpy�� �����Ѵ�.
			memcpy(out_data + sz_left_terminal_partition, _ring_buffer + _idx_front, sz_byte_data - sz_left_terminal_partition);
			_idx_front = GetNextIdx(_idx_front, sz_byte_data - sz_left_terminal_partition);

			//���� �뷮 �� ���� �뷮�� �缳���Ѵ�.
			_sz_byte_use -= sz_byte_data;
			_sz_byte_free = _total_sz_byte_ring_buffer - GetInsideSizeOfUse();

			return sz_byte_data;
		}
	}

	//==============================================================================
	//1 �� ���� ���� üũ�Ѵ�.
	//==============================================================================
	memcpy(out_data, _ring_buffer + _idx_front, sz_byte_data);
	_idx_front = GetNextIdx(_idx_front, sz_byte_data);

	//���� �뷮 �� ���� �뷮 �缳���Ѵ�.
	_sz_byte_use -= sz_byte_data;
	_sz_byte_free = _total_sz_byte_ring_buffer - GetInsideSizeOfUse();

	return sz_byte_data;


}


/**
brief  ����Ÿ �б�.

Param  out_data �б� ��� ������ ������.
Param sz_byte_data �б� ������ ũ��.

Return : �б⿡ ������ ������ ũ��.
*/
int	CStreamSQ::Peek(char *out_data, int sz_byte_data) {

	if (_sz_byte_use == 0) {

		return 0;
	}

	//���� �뷮 �缳���Ѵ�.
	if (_sz_byte_use < sz_byte_data) {

		sz_byte_data = _sz_byte_use;
	}

	//peek��
	int _idx_peek = this->_idx_front;

	//==============================================================================
	//2 �� �б� ���� üũ�Ѵ�.
	//==============================================================================
	if (_idx_rear < _idx_peek) {

		//ù ��° memcpy �뷮�� üũ�Ѵ�.
		int sz_left_terminal_partition = 0;
		sz_left_terminal_partition = GetUnBrokenGetSizeOfBuffer();

		//�ι� memcpy ���� üũ�Ѵ�.
		if (sz_byte_data > sz_left_terminal_partition) {

			//ù ��° memcpy�� �����Ѵ�.
			memcpy(out_data, _ring_buffer + _idx_peek, sz_left_terminal_partition);
			_idx_peek = GetNextIdx(_idx_peek, sz_left_terminal_partition);

			//�� ��° memcpy�� �����Ѵ�.
			memcpy(out_data + sz_left_terminal_partition, _ring_buffer + _idx_peek, sz_byte_data - sz_left_terminal_partition);

			return sz_byte_data;
		}
	}

	//==============================================================================
	//1 �� �б� ���� üũ�Ѵ�.
	//==============================================================================
	memcpy(out_data, _ring_buffer + _idx_peek, sz_byte_data);

	return sz_byte_data;
}

/**
brief �� ���۰� ��� �ִ� ���� üũ.

Return : �����۰� ��� �ִ� ����.
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
brief �� ���۰� ���� ���ִ� ���� üũ.

Return :�����۰� ���� �� �ִ� ����.
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
brief ���� �ε��� ���� ��ȯ�Ѵ�.
*/
int CStreamSQ::GetNextIdx(int target_idx, int len_byte_move) {

	return (target_idx + len_byte_move) % _total_sz_byte_ring_buffer;
}


/**
������ ��� ����Ÿ ����.

Parameters: ����.
Return : ����.
*/
void CStreamSQ::ClearRingBuffer(void) {

	_idx_front = 0;
	_idx_rear = 0;

	_sz_byte_use = 0;
	_sz_byte_free = _total_sz_byte_ring_buffer;

	return;
}


/**
������ ������ ����.

Parameters: ����.
Return : (char *)���� ������.
*/
char	*CStreamSQ::GetPtrOfBuffer(void) {

	return _ring_buffer;
}

/**
������ Front ������ ����.

Parameters: ����.
Return : (char *)���� ������.
*/
char	*CStreamSQ::GetFrontPtrOfBuffer(void) {

	return _ring_buffer + _idx_front;
}

/**
������ RearPos ������ ����.

Parameters: ����.
Return : (char *)���� ������.
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