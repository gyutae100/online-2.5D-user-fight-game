/**
@file lib_CSCREEN_DIB.cpp

@date 2018/06/02

@brief DIB( Device Independent Bitmap ) Ŭ����ȭ �Ѵ�.
@brief DIB�� ���ӿ��� �ַ� ����� �뵵�� �̿��Ѵ�.
@brief DIB Ŭ������ ��ġ�� ���ӵ��� �ʴ� �������� ��Ʈ�� �����̴�. ��ǥ������ BMP ������ �����Ѵ�.
@brief ���� ������ ȭ�� ��½ÿ��� GDI�� ���� ȭ�鿡 ����ϹǷ� DDB�� ��ȯ�� �ʿ��ϴ�.
*/


#pragma once
#include "stdafx.h"
#include "lib_CSCREEN_DIB.h"

//���� �����̽� �����Ѵ�.
using namespace CSCREEN_DIB;

/**
@brief ������

@brief DIB�� �����Ѵ�.

@param width : ��ũ�� �ʺ� ==�ȼ� ���� �ʺ�.
@param height : ��ũ�� ���� ==�ȼ� ���� ����.
@param color_bit : DIB ���� ����. �� ���α׷��� 32��Ʈ�� �����Ѵ�.
*/
CScreenDib::CScreenDib(int width, int height, int color_bit = 32){

	_width = width;
	_height = height;
	_pitch = (width * (color_bit / 8) + 3)&~3; //��ġ ���� ���Ѵ�.
	_color_bit = color_bit;
	_sz_buffer = width * height * ((color_bit) / 8); //���� ����� ���Ѵ�.


	//��ũ�� ���۸� �����Ѵ�.
	CreateDibBuffer(width, height, color_bit);
}


/**
@brief �Ҹ���

@brief DIB ���� ���� �Ҵ�� ���۸� �Ҹ��Ѵ�.
*/
CScreenDib::~CScreenDib(){

	ReleaseDibBuffer();
}


/**
@brief DIB ���� ������ �����Ѵ�.
@brief ���ӿ����� ���� ���۸��� �� ���� �뵵�� �̿�ȴ�.
@brief DIB�� �ʿ��� BITMAPINFO ����� �����Ѵ�.
@brief �̹��� ������ ��� ���۸� �����Ѵ�.

@param width : ��ũ�� �ʺ� ==�ȼ� ���� �ʺ�.
@param height : ��ũ�� ���� ==�ȼ� ���� ����.
@param color_bit : DIB ���� ����. �� ���α׷��� 32��Ʈ�� �����Ѵ�.
*/
void CScreenDib::CreateDibBuffer(int width, int height, int color_bit){

	//BITMAPINFO (BITMAPINFOHEADER)�� �����Ѵ�.
	_dib_info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	_dib_info.bmiHeader.biWidth = width;		   //�׸��� �ʺ�
	_dib_info.bmiHeader.biHeight = -height;		   //���̰� ������ ��� �׸��� �Ųٷ� �������� �ʴ´�.
	_dib_info.bmiHeader.biPlanes = 1;
	_dib_info.bmiHeader.biBitCount = color_bit;    //�÷� ��Ʈ = 32��Ʈ�� �⺻�̴�.
	_dib_info.bmiHeader.biCompression = BI_RGB; 
	_dib_info.bmiHeader.biSizeImage = _sz_buffer;  //�̹��� ������ == ���� ������
	_dib_info.bmiHeader.biXPelsPerMeter = 0;
	_dib_info.bmiHeader.biYPelsPerMeter = 0;
	_dib_info.bmiHeader.biClrUsed =0;
	_dib_info.bmiHeader.biClrImportant = 0;

	//�̹����� ���� �� ���۸� �����Ѵ�.
	_image_buffer = new BYTE[_sz_buffer];
}


/**
@brief DIB ���� �������Ѵ�.
*/
void CScreenDib::ReleaseDibBuffer(void) {

	//���� ������ �����Ѵ�.
	//�Ҹ� �Ǿ����Ƿ� ��� 0�̴�.
	_width = 0;
	_height = 0;
	_pitch = 0;
	_sz_buffer = 0;

	//���� ���ۿ� ����� �׸��� ��� 0x00 (���)���� �����Ѵ�.
	//memset(_image_buffer, 0x00, sizeof(BITMAPINFO));

	//�޸� �����Ѵ�.
	if (NULL != _image_buffer) {
	
		delete _image_buffer;
	}

	_image_buffer = NULL;
}

/**
@brief DIB ���� �� �����ϴ� �̹��� �ȼ��� DDB�� �����Ͽ� ����Ѵ�.

@param hWnd : ��� ������ �ڵ�.
@param x : x ��� ������.
@param y : y ��� ������.
*/
void CScreenDib::DrawBuffer(HWND hWnd, int x , int y ){

	if (NULL == _image_buffer) {

		return;
	}

	HDC hdc;

	//�Է¹��� hWnd �ڵ��� DC�� ��´�
	hdc = GetDC(hWnd);

	//------------------------------------------------------------------------------
	//	DIB ������ GDI �Լ��� ����Ͽ� DDBȭ �Ͽ� ����� �����쿡 ����Ѵ�.
	//------------------------------------------------------------------------------
	StretchDIBits( hdc, 
		x, y, _width, _height,
		x, y, _width, _height,
		_image_buffer,
		(LPBITMAPINFO)&_dib_info,
		DIB_RGB_COLORS,
		SRCCOPY
	);

	//DC�� ��ȯ�Ѵ�.
	ReleaseDC(hWnd, hdc);
}



//------------------------------------------------------------------------------
//	��ũ�� ���� ��� �Լ� ����̴�.
//------------------------------------------------------------------------------
/**
@brief �̹��� ���۸� ��ȯ�Ѵ�.
*/
BYTE *CScreenDib::GetDibBuffer(void) {

	return _image_buffer;
}
	
/**
@brief �̹��� �ʺ� ��ȯ�Ѵ�.
*/
int CScreenDib::GetWidth(void) {

	return _width;
}

/**
@brief �̹��� ���̸� ��ȯ�Ѵ�.
*/
int CScreenDib::GetHeight(void) {

	return _height;
}

/**
@brief �̹��� ��ġ�� ��ȯ�Ѵ�.
*/
int CScreenDib::GetPitch(void) {

	return _pitch;
}

