/**
@file lib_CSPRITE_DIB.h

@date 2018/06/02

@brief SPRITE 이미지 관리자를 클래스화 한다.
@brief SPRITE란 스프라이트란 화면에 그려진 그림 위에 그림을 합성, 출력 하고 이동할 수 있는 기능을 의미한다.
@brief BMP 파일을 읽어서 SPRITE 이미지를 관리한다.
@brief SPRITE 이미지를 DIB 버퍼에 픽셀 단위로 출력한다.
@brief SPRITE 이미지를 출력 시에는 컬러 키(스프라이트의 배경 색)는 DIB 버퍼 출력 시 제외 한다.
@brief SPRITE 이미지를 DIB 버퍼에 출력 시 버퍼 높이 및 너비를 벗어나는 경우 CLLIPING(쪼개다)을 지원한다.
*/


#pragma once
#include "stdafx.h"

//------------------------------------------------------------------------------
//	namespace : CSPRITE_DIB
//------------------------------------------------------------------------------
namespace CSPRITE_DIB {

	/**
	@class CSpriteDib

	@brief CPRITE 이미지 관리자를 클래스화 한다.
	@brief BMP 파일을 읽어서 SPRITE 이미지를 관리한다.
	@brief CPRITE 이미지를 DIB 버퍼에 픽셀 단위로 출력한다.
	@brief CPRITE 이미지를 출력 시에는 컬러 키(스프라이트의 배경 색)는 DIB 버퍼 출력 시 제외 한다.
	@brief CPRITE 이미지를 DIB 버퍼에 출력 시 버퍼 높이 및 너비를 벗어나는 경우 CLLIPING(쪼개다)을 지원한다.
	*/
	class CSpriteDib {

	private:


		//typedef struct SPRITE	///< 스프라이트 정보를 담은 구조체.
		//{
		//	BYTE *_sprite_buffer;   ///< 스프라이트 버퍼. (컬러키를 포함한다.)
		//	int _width;				///< 스프라이트 너비.
		//	int _height;			///< 스프라이트 높이.
		//	int _pitch;				///< 스프라이트 피치.

		//	int _senter_x;	///< 스프라이트 중점x.
		//	int _senter_y;	///< 스프라이트 중점y.
		//}SPRITE;


	public:

		typedef struct SPRITE	///< 스프라이트 정보를 담은 구조체
		{
			BYTE *_sprite_buffer;   ///< 스프라이트 버퍼. (컬러키를 포함한다.)
			int _width;				///< 스프라이트 너비.
			int _height;			///< 스프라이트 높이.
			int _pitch;				///< 스프라이트 피치.

			int _senter_x;	///< 스프라이트 중점x.
			int _senter_y;	///< 스프라이트 중점y.
		}SPRITE;

		SPRITE *_sprites;		  ///< 스프라이트 프레임 배열. 

		//생성자.
		CSpriteDib(int max_cnt_sprite, DWORD color_key);

		//소멸자.
		virtual ~CSpriteDib();

		//BMP 파일을 읽어서 하나의 스프라이트로 저장한다.
		BOOL LoadDibSprite(int idx_sprite, const WCHAR *file_name, int center_point_x, int cneter_point_y);

		//해당 인덱스 스프라이트 해제한다.
		void ReleaseSprite(int idx_sprite);

		//특정 메모리 위치에 스프라이트 출력한다. ( 클리핑, 칼라키 처리 동시)
		void DrawFullSprite(int idx_sprite, int draw_x, int draw_y, BYTE *p_dest_img,
			int dest_width, int dest_height, int dest_pitch, int draw_len = 100);

		//레드 색으로 출력한다.
		void DrawFullSpriteLikeMonster(int idx_sprite, int draw_x, int draw_y, BYTE *p_dest_img,
			int dest_width, int dest_height, int dest_pitch, int draw_len = 100);

		//특정 메모리 위치에 스프라이트 출력한다. ( 클리핑, 칼라키 처리 동시)
		void DrawVariationSprite(int idx_sprite, int draw_start_x, int draw_start_y,
			BYTE *dest_buffer, int dest_width, int dest_height, int dest_pitch, int draw_len);

		//특정 메모리 위치에 스프라이트 출력 ( 클리핑 처리만)
		void DrawImage(int idx_sprite, int draw_x, int draw_y, BYTE *p_dest_img,
			int dest_width, int dest_height, int dest_pitch, int draw_len = 100);



	protected:

		//------------------------------------------------------------------------------
		//	스프라이트 배열 정보.
		//------------------------------------------------------------------------------
		int _max_cnt_sprite; ///< 최대 스프라이트 프레임 저장 갯수.
//		SPRITE *_sprites;		  ///< 스프라이트 프레임 배열.
		int _cnt_sprites;	  ///< 현재 삽입된 스프라이트 프레임 갯수.

		DWORD _color_key;		  ///< 투명 색상 지정 컬러.
	};
}