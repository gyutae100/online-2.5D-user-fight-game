
#ifndef  __PACKET__
#define  __PACKET__

#include "stdafx.h"

class CPacket{
public:


	enum SETTING_PACKET
	{
		TOTAL_SZ_BYTE_BUFFER = 1024		///< �⺻ ���� ������.
	};


	//������ ���.
	CPacket();
	CPacket(int iBufferSize);

	//�ı��� ���.
	virtual	~CPacket();
	void	Destroy(void);

	//���� ���.
	void	Clear(void);

	//�� ���� ������ ��ȯ
	int	GetBufferSize(void);

	//���� �� ��Ŷ ������ ��ȯ.
	int		GetDataSize(void);




	//���� ������ ���.
	char	*GetBufferPtr(void);

	//Front �� Rear �ε��� �ű��.
	int		MoveIdxRear(int _idx_offset);
	int		MoveIdxFront(int _idx_offset);






	//--------------------------------------------------------------------------------
	// ������ ���۷�����.
	//--------------------------------------------------------------------------------
	CPacket	&operator = (CPacket &);

	// �ֱ�. �� ���� Ÿ�Ը��� ��� ����.
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


	// ����. �� ���� Ÿ�Ը��� ��� ����.
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





	// ����Ÿ ���.
	int		GetData(char *dest_buffer, int _idx_offset);

	// ����Ÿ ����.
	int		PutData(char *src_buffer, int sz_byte_src);




protected:

	char *_queue_buffer;		///<ť ���� ���� ������

	int _idx_front;				///<����Ʈ �ε���
	int _idx_rear;				///<���� �ε���

	int _total_sz_byte_buffer;  ///<�� ���� ������
	int _total_sz_byte_data;    ///<���� ������� ���� ������.


	
	union TYPE_FLOAT  //<float Ÿ���� char������ �ٲٱ� ���� union ����ü.
	{
		char data[4];
		float value;
	};

	union TYPE_DOUBLE //<double Ÿ���� char������ �ٲٱ� ���� union ����ü.
	{
		char data[8];
		double value;
	};
};

#endif
