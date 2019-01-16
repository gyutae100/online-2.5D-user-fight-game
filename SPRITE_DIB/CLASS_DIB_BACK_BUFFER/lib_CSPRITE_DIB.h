/**
@file lib_CSPRITE_DIB.h

@date 2018/06/02

@brief SPRITE �̹��� �����ڸ� Ŭ����ȭ �Ѵ�.
@brief SPRITE�� ��������Ʈ�� ȭ�鿡 �׷��� �׸� ���� �׸��� �ռ�, ��� �ϰ� �̵��� �� �ִ� ����� �ǹ��Ѵ�.
@brief BMP ������ �о SPRITE �̹����� �����Ѵ�.
@brief SPRITE �̹����� DIB ���ۿ� �ȼ� ������ ����Ѵ�.
@brief SPRITE �̹����� ��� �ÿ��� �÷� Ű(��������Ʈ�� ��� ��)�� DIB ���� ��� �� ���� �Ѵ�.
@brief SPRITE �̹����� DIB ���ۿ� ��� �� ���� ���� �� �ʺ� ����� ��� CLLIPING(�ɰ���)�� �����Ѵ�.
*/


#pragma once
#include "stdafx.h"

//------------------------------------------------------------------------------
//	namespace : CSPRITE_DIB
//------------------------------------------------------------------------------
namespace CSPRITE_DIB {

	/**
	@class CSpriteDib

	@brief CPRITE �̹��� �����ڸ� Ŭ����ȭ �Ѵ�.
	@brief BMP ������ �о SPRITE �̹����� �����Ѵ�.
	@brief CPRITE �̹����� DIB ���ۿ� �ȼ� ������ ����Ѵ�.
	@brief CPRITE �̹����� ��� �ÿ��� �÷� Ű(��������Ʈ�� ��� ��)�� DIB ���� ��� �� ���� �Ѵ�.
	@brief CPRITE �̹����� DIB ���ۿ� ��� �� ���� ���� �� �ʺ� ����� ��� CLLIPING(�ɰ���)�� �����Ѵ�.
	*/
	class CSpriteDib {

	private:


		//typedef struct SPRITE	///< ��������Ʈ ������ ���� ����ü.
		//{
		//	BYTE *_sprite_buffer;   ///< ��������Ʈ ����. (�÷�Ű�� �����Ѵ�.)
		//	int _width;				///< ��������Ʈ �ʺ�.
		//	int _height;			///< ��������Ʈ ����.
		//	int _pitch;				///< ��������Ʈ ��ġ.

		//	int _senter_x;	///< ��������Ʈ ����x.
		//	int _senter_y;	///< ��������Ʈ ����y.
		//}SPRITE;


	public:

		typedef struct SPRITE	///< ��������Ʈ ������ ���� ����ü
		{
			BYTE *_sprite_buffer;   ///< ��������Ʈ ����. (�÷�Ű�� �����Ѵ�.)
			int _width;				///< ��������Ʈ �ʺ�.
			int _height;			///< ��������Ʈ ����.
			int _pitch;				///< ��������Ʈ ��ġ.

			int _senter_x;	///< ��������Ʈ ����x.
			int _senter_y;	///< ��������Ʈ ����y.
		}SPRITE;

		SPRITE *_sprites;		  ///< ��������Ʈ ������ �迭. 

		//������.
		CSpriteDib(int max_cnt_sprite, DWORD color_key);

		//�Ҹ���.
		virtual ~CSpriteDib();

		//BMP ������ �о �ϳ��� ��������Ʈ�� �����Ѵ�.
		BOOL LoadDibSprite(int idx_sprite, const WCHAR *file_name, int center_point_x, int cneter_point_y);

		//�ش� �ε��� ��������Ʈ �����Ѵ�.
		void ReleaseSprite(int idx_sprite);

		//Ư�� �޸� ��ġ�� ��������Ʈ ����Ѵ�. ( Ŭ����, Į��Ű ó�� ����)
		void DrawFullSprite(int idx_sprite, int draw_x, int draw_y, BYTE *p_dest_img,
			int dest_width, int dest_height, int dest_pitch, int draw_len = 100);

		//���� ������ ����Ѵ�.
		void DrawFullSpriteLikeMonster(int idx_sprite, int draw_x, int draw_y, BYTE *p_dest_img,
			int dest_width, int dest_height, int dest_pitch, int draw_len = 100);

		//Ư�� �޸� ��ġ�� ��������Ʈ ����Ѵ�. ( Ŭ����, Į��Ű ó�� ����)
		void DrawVariationSprite(int idx_sprite, int draw_start_x, int draw_start_y,
			BYTE *dest_buffer, int dest_width, int dest_height, int dest_pitch, int draw_len);

		//Ư�� �޸� ��ġ�� ��������Ʈ ��� ( Ŭ���� ó����)
		void DrawImage(int idx_sprite, int draw_x, int draw_y, BYTE *p_dest_img,
			int dest_width, int dest_height, int dest_pitch, int draw_len = 100);



	protected:

		//------------------------------------------------------------------------------
		//	��������Ʈ �迭 ����.
		//------------------------------------------------------------------------------
		int _max_cnt_sprite; ///< �ִ� ��������Ʈ ������ ���� ����.
//		SPRITE *_sprites;		  ///< ��������Ʈ ������ �迭.
		int _cnt_sprites;	  ///< ���� ���Ե� ��������Ʈ ������ ����.

		DWORD _color_key;		  ///< ���� ���� ���� �÷�.
	};
}