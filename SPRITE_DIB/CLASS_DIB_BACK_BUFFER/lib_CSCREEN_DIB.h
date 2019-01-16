/**
@file lib_CSCREEN_DIB.h

@date 2018/06/02

@brief DIB( Device Independent Bitmap ) Ŭ����ȭ �Ѵ�.
@brief DIB�� ���ӿ��� �ַ� ����� �뵵�� �̿��Ѵ�.
@brief DIB Ŭ������ ��ġ�� ���ӵ��� �ʴ� �������� ��Ʈ�� �����̴�. ��ǥ������ BMP ������ �����Ѵ�.
@brief ���� ������ ȭ�� ��½ÿ��� GDI�� ���� ȭ�鿡 ����ϹǷ� DDB�� ��ȯ�� �ʿ��ϴ�.
*/


#pragma once
#include "stdafx.h"

//------------------------------------------------------------------------------
//	namespace : CSCREEN_DIB
//------------------------------------------------------------------------------
namespace CSCREEN_DIB {


	/**
	@class CScreenDib

	@brief ȭ���� ����۸� DIB( Device Independent Bitmap ) Ŭ����ȭ �Ѵ�.
	@brief DIB Ŭ������ ��ġ�� ���ӵ��� �ʴ� �������� ��Ʈ�� �����̴�. ��ǥ������ BMP ������ �����Ѵ�.
	@brief ���� ������ ȭ�� ��½ÿ��� GDI�� ���� ȭ�鿡 ����ϹǷ� DDB�� ��ȯ�� �ʿ��ϴ�.
	*/
	class CScreenDib
	{


	public:

		//������
		CScreenDib(int iWidth, int iHeight, int iColorBit);

		//�Ҹ���
		virtual ~CScreenDib();

	protected:

		//���۸� �����Ѵ�.
		void CreateDibBuffer(int iWidth, int iHeight, int iColorBit);

		//���۸� �����Ѵ�.
		void ReleaseDibBuffer(void);


	public:

		//���۸� ȭ�鿡 ����Ѵ�.
		//DIB�� GDI�� ����ϱ� ���� DDB�� ��ȯ�Ѵ�.
		void DrawBuffer(HWND hWnd, int iX=0 , int iY=0);



		//------------------------------------------------------------------------------
		//	��ũ���� ���� ������ ��� ���� �Լ� ����̴�.
		//------------------------------------------------------------------------------
		//���۸� ��ȯ�Ѵ�.
		BYTE *GetDibBuffer(void);

		//ȭ���� ���̸� ��ȯ�Ѵ�.
		int GetWidth(void);

		//ȭ���� ���̸� ��ȯ�Ѵ�.
		int GetHeight(void);

		//ȭ���� ��ġ�� ��ȯ�Ѵ�.
		int GetPitch(void);


	protected:

		//------------------------------------------------------------------------------
		//	��ũ���� ���� ���� ���
		//------------------------------------------------------------------------------
		BITMAPINFO _dib_info;		///< DIB ��Ʈ�� ��ü�� ���� ����

		BYTE *_image_buffer; ///< �ȼ� ������ ������ ���� ����� 
		int _sz_buffer;	      ///< ����� ������

		int _width;			  ///< �ȼ� ������ ��ũ�� �ʺ�
		int _height;		  ///< �ȼ� ������ ��ũ�� ����
		int _pitch;			  ///< �ȼ� ������ ��ũ�� ��ġ
		int _color_bit;		  ///< �ȼ� ����
	};
}