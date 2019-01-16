
#ifndef  __PACKET__
#define  __PACKET__

#include "stdafx.h"

class CPacket{
public:


	enum SETTING_PACKET
	{
		TOTAL_SZ_BYTE_BUFFER = 1024		///< 기본 버퍼 사이즈.
	};


	//생성자 목록.
	CPacket();
	CPacket(int iBufferSize);

	//파괴자 목록.
	virtual	~CPacket();
	void	Destroy(void);

	//버퍼 비움.
	void	Clear(void);

	//총 버퍼 사이즈 반환
	int	GetBufferSize(void);

	//버퍼 내 패킷 사이즈 반환.
	int		GetDataSize(void);




	//버퍼 포인터 얻기.
	char	*GetBufferPtr(void);

	//Front 및 Rear 인덱스 옮기기.
	int		MoveIdxRear(int _idx_offset);
	int		MoveIdxFront(int _idx_offset);






	//--------------------------------------------------------------------------------
	// 연산자 오퍼레이터.
	//--------------------------------------------------------------------------------
	CPacket	&operator = (CPacket &);

	// 넣기. 각 변수 타입마다 모두 만듬.
	//1Byte.
	CPacket	&operator << (BYTE );
	CPacket	&operator << (char );

	//2Byte.
	CPacket	&operator << (short);
	CPacket	&operator << (WORD );

	//4Byte.
	CPacket	&operator << (int );
	CPacket	&operator << (DWORD );
	CPacket	&operator << (float );

	//8Byte.
	CPacket	&operator << (__int64 );
	CPacket	&operator << (double );


	// 빼기. 각 변수 타입마다 모두 만듬.
	//1Byte.
	CPacket	&operator >> (BYTE &);
	CPacket	&operator >> (char &);

	//2Byte.
	CPacket	&operator >> (short &);
	CPacket	&operator >> (WORD &);

	//4Byte.
	CPacket	&operator >> (int &);
	CPacket	&operator >> (DWORD &);
	CPacket	&operator >> (float &);

	//8Byte.
	CPacket	&operator >> (__int64 &);
	CPacket	&operator >> (double &);





	// 데이타 얻기.
	int		GetData(char *dest_buffer, int _idx_offset);

	// 데이타 삽입.
	int		PutData(char *src_buffer, int sz_byte_src);




protected:

	char *_queue_buffer;		///<큐 버퍼 시작 포인터

	int _idx_front;				///<프론트 인덱스
	int _idx_rear;				///<리어 인덱스

	int _total_sz_byte_buffer;  ///<총 버퍼 사이즈
	int _total_sz_byte_data;    ///<현재 사용중인 버퍼 사이즈.


	
	union TYPE_FLOAT  //<float 타입을 char형으로 바꾸기 위한 union 구조체.
	{
		char data[4];
		float value;
	};

	union TYPE_DOUBLE //<double 타입을 char형으로 바꾸기 위한 union 구조체.
	{
		char data[8];
		double value;
	};
};

#endif
