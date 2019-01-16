/**
@file lib_CSCREEN_DIB.h

@date 2018/06/02

@brief DIB( Device Independent Bitmap ) 클래스화 한다.
@brief DIB는 게임에서 주로 백버퍼 용도로 이용한다.
@brief DIB 클래스는 장치에 종속되지 않는 독립적인 비트맵 버퍼이다. 대표적으로 BMP 파일이 존재한다.
@brief 최종 윈도우 화면 출력시에는 GDI를 통해 화면에 출력하므로 DDB로 변환이 필요하다.
*/


#pragma once
#include "stdafx.h"

//------------------------------------------------------------------------------
//	namespace : CSCREEN_DIB
//------------------------------------------------------------------------------
namespace CSCREEN_DIB {


	/**
	@class CScreenDib

	@brief 화면의 백버퍼를 DIB( Device Independent Bitmap ) 클래스화 한다.
	@brief DIB 클래스는 장치에 종속되지 않는 독립적인 비트맵 버퍼이다. 대표적으로 BMP 파일이 존재한다.
	@brief 최종 윈도우 화면 출력시에는 GDI를 통해 화면에 출력하므로 DDB로 변환이 필요하다.
	*/
	class CScreenDib
	{


	public:

		//생성자
		CScreenDib(int iWidth, int iHeight, int iColorBit);

		//소멸자
		virtual ~CScreenDib();

	protected:

		//버퍼를 생성한다.
		void CreateDibBuffer(int iWidth, int iHeight, int iColorBit);

		//버퍼를 해제한다.
		void ReleaseDibBuffer(void);


	public:

		//버퍼를 화면에 출력한다.
		//DIB를 GDI에 출력하기 위해 DDB로 변환한다.
		void DrawBuffer(HWND hWnd, int iX=0 , int iY=0);



		//------------------------------------------------------------------------------
		//	스크린에 대한 정보를 얻기 위한 함수 목록이다.
		//------------------------------------------------------------------------------
		//버퍼를 반환한다.
		BYTE *GetDibBuffer(void);

		//화면의 길이를 반환한다.
		int GetWidth(void);

		//화면의 높이를 반환한다.
		int GetHeight(void);

		//화면의 피치를 반환한다.
		int GetPitch(void);


	protected:

		//------------------------------------------------------------------------------
		//	스크린에 대한 정보 목록
		//------------------------------------------------------------------------------
		BITMAPINFO _dib_info;		///< DIB 비트맵 자체에 대한 정보

		BYTE *_image_buffer; ///< 픽셀 데이터 정보를 담은 백버퍼 
		int _sz_buffer;	      ///< 백버퍼 사이즈

		int _width;			  ///< 픽셀 단위의 스크린 너비
		int _height;		  ///< 픽셀 단위의 스크린 높이
		int _pitch;			  ///< 픽셀 단위의 스크린 피치
		int _color_bit;		  ///< 픽셀 단위
	};
}