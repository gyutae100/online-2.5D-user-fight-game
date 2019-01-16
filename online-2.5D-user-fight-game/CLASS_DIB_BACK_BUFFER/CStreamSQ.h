
#pragma once
#include "stdafx.h"
#include <iostream>

using namespace std;

/**
class ������ Ŭ����

brief
*/
class CStreamSQ {

private:
	enum OPTION_RING_BUFFER {

		DEFAULT_SIZE_OF_RING_BUFFER = 20000,	 ///<�� ���� ����Ʈ ������
		SIZE_OF_BLINK = 1
	};

	char * _ring_buffer;			///<�� ���� 

	int _total_sz_byte_ring_buffer;	///<�� ���� �� ������
	int _sz_byte_use;				///<���� ��� ���� �� ���� �뷮.
	int _sz_byte_free;				///<���� ���� ������ �뷮

	int _idx_front;
	int _idx_rear;

	int _sz_input_data;				///<����� ��
	double total_line;				///<����� ��
	double total_char;				///<����� ��


private:

	void InitialRingBuffer(int sz_byte_buffer);	///<���۸� ���� ũ�⿡ �°� �����Ѵ�.

public:


	CStreamSQ(void);				///<������. �⺻ ���� ������ 100Byte�� ����.

	CStreamSQ(int sz_byte_buffer); ///<������. ����� ���� ���� ������� ����.

	int	GetTotalBufferSize(void);	 ///< �Ҵ� �� ���� ������ ��ȯ.
	int	GetInsideSizeOfUse(void);			 ///<���� ��� ���� ���� �뷮 ���.
	int	GetSizeOfFree(void);		 ///< ���ۿ� ���� �뷮 ���.
	int GetSizeOfUse(void);

	int	GetUnBrokenGetSizeOfBuffer(void); ///<���� �����ͷ� �ܺο��� �ѹ濡 �а�, �� �� �ִ� ����.
	int	GetUnBrokenPutSizeOfBuffer(void); ///<���� �����ͷ� �ܺο��� �ѹ濡 �а�, �� �� �ִ� ����.


	int	Put(char *in_data, int sz_data);	///< WritePos �� ����Ÿ ����.
	int	Get(char *out_data, int sz_data);	///<ReadPos ���� ����Ÿ ������.ReadPos �̵�
	int	Peek(char *out_data, int sz_data);  ///< ReadPos ���� ����Ÿ �о��.ReadPos ����.


	bool IsEmpty();	///<�� ���۰� ��� �ִ� ���� üũ.
	bool IsFull();	///<�� ���۰� ���� ���ִ� ���� üũ.



	int GetNextIdx(int target_idx, int len_byte_move);	 ///<���� �ε��� ���� ��ȯ�Ѵ�.
	void MoveRearIdx(int sz_data);						 ///<���ϴ� ���̸�ŭ �б���ġ ���� ���� / ���� ��ġ �̵�
	void MoveFrontIdx(int sz_data);					 ///<���ϴ� ���̸�ŭ �б���ġ ���� ���� / ���� ��ġ �̵�


	void ClearRingBuffer(void);			 ///<������ ��� ����Ÿ ����


	char *GetPtrOfBuffer(void);			 ///<������ ������ ����.
	char *GetFrontPtrOfBuffer(void);	 ///<������ Front ������ ����.
	char *GetRearPtrOfBuffer(void);		 ///<������ RearPos ������ ����.

	void DisplayInfo1(void);				 ///<���� ��Ʈ�� ���� ���� ������ �ܼ�â Ÿ��Ʋ â�� ǥ���Ѵ�.
	void DisplayInfo2(void);				 ///<���� ��Ʈ�� ���� ���� ������ �ܼ�â Ÿ��Ʋ â�� ǥ���Ѵ�.


};