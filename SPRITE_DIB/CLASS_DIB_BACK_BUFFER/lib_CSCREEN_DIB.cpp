/**
@file lib_CSCREEN_DIB.cpp

@date 2018/06/02

@brief DIB( Device Independent Bitmap ) 클래스화 한다.
@brief DIB는 게임에서 주로 백버퍼 용도로 이용한다.
@brief DIB 클래스는 장치에 종속되지 않는 독립적인 비트맵 버퍼이다. 대표적으로 BMP 파일이 존재한다.
@brief 최종 윈도우 화면 출력시에는 GDI를 통해 화면에 출력하므로 DDB로 변환이 필요하다.
*/


#pragma once
#include "stdafx.h"
#include "lib_CSCREEN_DIB.h"

//네임 스페이스 설정한다.
using namespace CSCREEN_DIB;

/**
@brief 생성자

@brief DIB를 생성한다.

@param width : 스크린 너비 ==픽셀 단위 너비.
@param height : 스크린 높이 ==픽셀 단위 높이.
@param color_bit : DIB 색상 깊이. 본 프로그램은 32비트만 지원한다.
*/
CScreenDib::CScreenDib(int width, int height, int color_bit = 32){

	_width = width;
	_height = height;
	_pitch = (width * (color_bit / 8) + 3)&~3; //피치 값을 구한다.
	_color_bit = color_bit;
	_sz_buffer = width * height * ((color_bit) / 8); //버퍼 사이즈를 구한다.


	//스크린 버퍼를 생성한다.
	CreateDibBuffer(width, height, color_bit);
}


/**
@brief 소멸자

@brief DIB 내부 동적 할당된 버퍼를 소멸한다.
*/
CScreenDib::~CScreenDib(){

	ReleaseDibBuffer();
}


/**
@brief DIB 버퍼 생성를 생성한다.
@brief 게임에서는 더블 버퍼링용 백 버퍼 용도로 이용된다.
@brief DIB에 필요한 BITMAPINFO 헤더를 설정한다.
@brief 이미지 정보를 담는 버퍼를 생성한다.

@param width : 스크린 너비 ==픽셀 단위 너비.
@param height : 스크린 높이 ==픽셀 단위 높이.
@param color_bit : DIB 색상 깊이. 본 프로그램은 32비트만 지원한다.
*/
void CScreenDib::CreateDibBuffer(int width, int height, int color_bit){

	//BITMAPINFO (BITMAPINFOHEADER)를 셋팅한다.
	_dib_info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	_dib_info.bmiHeader.biWidth = width;		   //그림의 너비
	_dib_info.bmiHeader.biHeight = -height;		   //높이가 음수인 경우 그림을 거꾸로 저장하지 않는다.
	_dib_info.bmiHeader.biPlanes = 1;
	_dib_info.bmiHeader.biBitCount = color_bit;    //컬러 비트 = 32비트가 기본이다.
	_dib_info.bmiHeader.biCompression = BI_RGB; 
	_dib_info.bmiHeader.biSizeImage = _sz_buffer;  //이미지 사이즈 == 버퍼 사이즈
	_dib_info.bmiHeader.biXPelsPerMeter = 0;
	_dib_info.bmiHeader.biYPelsPerMeter = 0;
	_dib_info.bmiHeader.biClrUsed =0;
	_dib_info.bmiHeader.biClrImportant = 0;

	//이미지를 저장 할 버퍼를 생성한다.
	_image_buffer = new BYTE[_sz_buffer];
}


/**
@brief DIB 버퍼 릴리즈한다.
*/
void CScreenDib::ReleaseDibBuffer(void) {

	//버퍼 정보를 수정한다.
	//소멸 되었으므로 모두 0이다.
	_width = 0;
	_height = 0;
	_pitch = 0;
	_sz_buffer = 0;

	//기존 버퍼에 저장된 그림을 모두 0x00 (흰색)으로 변경한다.
	//memset(_image_buffer, 0x00, sizeof(BITMAPINFO));

	//메모리 해제한다.
	if (NULL != _image_buffer) {
	
		delete _image_buffer;
	}

	_image_buffer = NULL;
}

/**
@brief DIB 버퍼 내 존재하는 이미지 픽셀을 DDB로 변경하여 출력한다.

@param hWnd : 출력 윈도우 핸들.
@param x : x 출력 시작점.
@param y : y 출력 시작점.
*/
void CScreenDib::DrawBuffer(HWND hWnd, int x , int y ){

	if (NULL == _image_buffer) {

		return;
	}

	HDC hdc;

	//입력받은 hWnd 핸들의 DC를 얻는다
	hdc = GetDC(hWnd);

	//------------------------------------------------------------------------------
	//	DIB 파일을 GDI 함수를 사용하여 DDB화 하여 출력할 윈도우에 출력한다.
	//------------------------------------------------------------------------------
	StretchDIBits( hdc, 
		x, y, _width, _height,
		x, y, _width, _height,
		_image_buffer,
		(LPBITMAPINFO)&_dib_info,
		DIB_RGB_COLORS,
		SRCCOPY
	);

	//DC를 반환한다.
	ReleaseDC(hWnd, hdc);
}



//------------------------------------------------------------------------------
//	스크린 정보 얻는 함수 목록이다.
//------------------------------------------------------------------------------
/**
@brief 이미지 버퍼를 반환한다.
*/
BYTE *CScreenDib::GetDibBuffer(void) {

	return _image_buffer;
}
	
/**
@brief 이미지 너비를 반환한다.
*/
int CScreenDib::GetWidth(void) {

	return _width;
}

/**
@brief 이미지 높이를 반환한다.
*/
int CScreenDib::GetHeight(void) {

	return _height;
}

/**
@brief 이미지 피치를 반환한다.
*/
int CScreenDib::GetPitch(void) {

	return _pitch;
}

