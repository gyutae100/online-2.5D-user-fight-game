
#include "stdafx.h"
#include "CPacket.h"

CPacket::CPacket() {

	_queue_buffer= new char[SETTING_PACKET::TOTAL_SZ_BYTE_BUFFER];
	_total_sz_byte_buffer = TOTAL_SZ_BYTE_BUFFER;

	_total_sz_byte_data = 0;

	_idx_front = 0;
	_idx_rear = 0;
}

CPacket::CPacket(int iBufferSize) {

	_queue_buffer = new char[iBufferSize];
	_total_sz_byte_buffer = iBufferSize;

	_total_sz_byte_data = 0;

	_idx_front = 0;
	_idx_rear = 0;
}

CPacket::~CPacket() {

	Destroy();
}

void CPacket::Destroy(void) {

	delete _queue_buffer;
}

void CPacket::Clear(void) {

	_total_sz_byte_data = 0;

	_idx_front = 0;
	_idx_rear = 0;
}


int	CPacket::GetBufferSize(void) {
	
	return _total_sz_byte_buffer;
}

int	CPacket::GetDataSize(void) {

	return _total_sz_byte_data;
}

char *CPacket::GetBufferPtr(void) {

	return _queue_buffer;
}

int	CPacket::MoveIdxRear(int idx_offset) {

	if (0 > idx_offset) {

		return 0;
	}

	if ( _total_sz_byte_buffer < (_idx_rear + idx_offset) ) {

		return 0;
	}

	_total_sz_byte_data +=idx_offset;

	_idx_rear +=idx_offset;

	return idx_offset;
}

int	CPacket::MoveIdxFront(int idx_offset) {

	if (0 > idx_offset) {

		return 0;
	}

	if ( _total_sz_byte_data < idx_offset) {

		return 0;
	}


	_total_sz_byte_data -= idx_offset;
	_idx_front += idx_offset;


	return idx_offset;
}

int CPacket::GetData(char *dest_buffer, int sz_byte_data) {

	if (_total_sz_byte_data < sz_byte_data) {

		return 0;
	}
	
	memcpy(dest_buffer, _queue_buffer+ _idx_front, sz_byte_data);
	int success_offset = MoveIdxFront(sz_byte_data);

	if (success_offset == sz_byte_data) {

		return sz_byte_data;
	}
	else {

		//에러 출력한다.
		return -1;
	}
}

int CPacket::PutData(char *src_buffer, int sz_byte_data) {

	if (_total_sz_byte_buffer - _total_sz_byte_data < sz_byte_data) {

		return 0;
	}

	memcpy(_queue_buffer + _idx_rear, src_buffer, sz_byte_data);
	int success_offset = MoveIdxRear(sz_byte_data);

	if (success_offset == sz_byte_data) {

		return sz_byte_data;
	}
	else {

		//에러 출력한다.
		return -1;
	}


}

CPacket& CPacket::operator = (CPacket &clSrcPacket) {

	//???
	return *this;
}

CPacket& CPacket::operator << (BYTE in_value) {

	PutData((char *)&in_value, 1);
	return *this;

}
CPacket& CPacket::operator << (char in_value) {

	PutData((char *)&in_value, 1);
	return *this;
}

CPacket& CPacket::operator << (short in_value) {

	PutData((char *)&in_value, 2);
	return *this;
}

CPacket& CPacket::operator << (WORD in_value) {

	PutData((char *)&in_value, 2);
	return *this;
}

CPacket& CPacket::operator << (int in_value) {

	PutData((char *)&in_value, 4);
	return *this;
}

CPacket& CPacket::operator << (DWORD in_value) {

	PutData((char *)&in_value, 4);
	return *this;
}

CPacket& CPacket::operator << (float in_value) {

	TYPE_FLOAT data_float;
	data_float.value = in_value;
	PutData((char *)&data_float.data, 4);
	return *this;
}

CPacket& CPacket::operator << (__int64 in_value) {

	PutData((char *)&in_value, 8);
	return *this;
}

CPacket& CPacket::operator << (double in_value) {

	TYPE_DOUBLE data_double;
	data_double.value = in_value;
	PutData((char *)&data_double.data, 8);
	return *this;
}


CPacket& CPacket::operator >> (BYTE &out_value) {

	GetData((char *)&out_value, 1);
	return *this;
}

CPacket& CPacket::operator >> (char &out_value) {

	GetData((char *)&out_value, 1);
	return *this;
}

CPacket& CPacket::operator >> (short &out_value) {

	GetData((char *)&out_value, 2);
	return *this;
}

CPacket& CPacket::operator >> (WORD &out_value) {

	GetData((char *)&out_value, 2);
	return *this;
}

CPacket& CPacket::operator >> (int &out_value) {

	GetData((char *)&out_value, 4);
	return *this;
}

CPacket& CPacket::operator >> (DWORD &out_value) {

	GetData((char *)&out_value, 4);
	return *this;
}
CPacket& CPacket::operator >> (float &out_value) {

	TYPE_FLOAT data_float;
	GetData((char *)&data_float.data, 4);
	out_value = data_float.value;
	return *this;
}

CPacket& CPacket::operator >> (__int64 &out_value) {

	GetData((char *)&out_value, 8);
	return *this;
}

CPacket& CPacket::operator >> (double &out_value) {

	TYPE_DOUBLE data_double;
	GetData((char *)&data_double.data, 8);
	out_value = data_double.value;
	return *this;
}

