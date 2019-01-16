/**
@file lib_CSPRITE_DIB.cpp

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
#include "lib_CSPRITE_DIB.h"

//���� �����̽� �����Ѵ�.
using namespace CSPRITE_DIB;

/**
@brief ������

@brief CSPriteDib �Ŵ����� �����Ѵ�.

@param max_cnt_sprite : �ִ� ���� ������ ��������Ʈ ������(�̹���) ����
@param color_key : ��������Ʈ ��� ���� �� �÷�Ű
*/
CSpriteDib::CSpriteDib(int max_cnt_sprite, DWORD color_key){

	//�ִ� ���� ������ ��������Ʈ ������ �����Ѵ�.
	_max_cnt_sprite = max_cnt_sprite;

	//------------------------------------------------------------------------------
	//	�ִ� �о�� ������ �Ҵ��Ѵ�.
	//------------------------------------------------------------------------------
	_sprites = new SPRITE[max_cnt_sprite];
	memset(_sprites, 0, sizeof(SPRITE) * max_cnt_sprite);

	//���� �÷� �����Ѵ�.
	_color_key = color_key;

	//���� ����� ��������Ʈ ������ 0���̴�.
	_cnt_sprites = 0;

}

/**
@brief �ı���

@brief �Ҵ��� ��� ��������Ʈ �̹����� �����Ѵ�.
*/
CSpriteDib::~CSpriteDib(){

	//��ü ��ȸ�ϸ� ��� ��������Ʈ�� �����Ѵ�.
	for (int idx = 0; idx < _max_cnt_sprite; idx++){

		ReleaseSprite(idx);
	}
}


/**
@brief BMP ������ �о �ϳ��� ���������� �����Ѵ�.

@param idx_sprite : �����ӿ� �ο��� �ε���
@param file_name : �������� ����� bmp ���� ��� �̸�
@param senter_x : �������� ���� x ��ġ
@param senter_y : �������� ���� y ��ġ
*/
BOOL CSpriteDib::LoadDibSprite(int idx_sprite, const WCHAR *bmp_file_path, int senter_x, int setner_y){

	HANDLE h_file;
	DWORD read;
	int pitch;
	int sz_image;
	int cnt;

	BITMAPFILEHEADER bmp_file_header;
	BITMAPINFOHEADER bmp_info_header;
	
	
	//��Ʈ�� ���� �� �����Ѵ�.
	h_file = CreateFile( bmp_file_path, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); 
	if ( INVALID_HANDLE_VALUE == h_file ){

		return FALSE;
	}

	//���� ��������Ʈ�� ���� �� �����Ѵ�.
	ReleaseSprite(idx_sprite);

	// ����������� �д´�.
	ReadFile(h_file, &bmp_file_header, sizeof(BITMAPFILEHEADER), &read, NULL);

	// ���� Ÿ���� ��Ʈ������ "BM" (0x4D42<�򿣵��>) �˻��Ѵ�.
	if (bmp_file_header.bfType != 0x4d42) {
	
		return false;
	}

	// ����������� �д´�.
	ReadFile(h_file, &bmp_info_header, sizeof(BITMAPINFOHEADER),&read, NULL);

	//��Ʈ ī��Ʈ üũ�Ͽ� 32��Ʈ���� Ȯ���Ѵ�.
	if (bmp_info_header.biBitCount != 32){

		CloseHandle(h_file);
		return FALSE;
	}

	//����, ������ ��ġ�� ���Ѵ�.
	pitch = (bmp_info_header.biWidth * (32 / 8) + 3)&~3;
	
	// ��������Ʈ ������ �����Ѵ�.
	_sprites[idx_sprite]._height = bmp_info_header.biHeight;
	_sprites[idx_sprite]._width = bmp_info_header.biWidth;
	_sprites[idx_sprite]._pitch = pitch;

	//	�̹����� ���� ��ü ũ�⸦ ���ϰ�, �޸� �Ҵ��Ѵ�.
	sz_image = pitch * bmp_info_header.biHeight;
	_sprites[idx_sprite]._sprite_buffer = new BYTE[sz_image];

	//------------------------------------------------------------------------------
	//	�̹��� �κ��� ��������Ʈ ���۷� �о�´�.
	//	DIB�� �������� �����Ƿ� �ٽ� ������.
	//	�ӽ� ���ۿ� ���� �ڿ� �������鼭 �����Ѵ�.
	//------------------------------------------------------------------------------
	BYTE *src_buffer = new BYTE[sz_image];                    //���� ������������ ������ ����
	BYTE *dest_buffer = _sprites[idx_sprite]._sprite_buffer;  //���� ������������ ���� �� ���� ����
	BYTE *tern_buff_offset;

	//���� �� ��������Ʈ�� �о� �����Ѵ�.
	ReadFile(h_file, src_buffer, sz_image, &read, NULL);
	CloseHandle(h_file);

	//��������Ʈ�� ������ ��ġ�� ó�� ���������� �����Ѵ�.
	//�ε����� �����ϹǷ� ���� -1�� ���־�� �Ѵ�.
	tern_buff_offset = src_buffer + pitch * (bmp_info_header.biHeight -1);

	//��������Ʈ�� �����Ѵ�.
	for (cnt = 0; cnt < bmp_info_header.biHeight; cnt++){

		memcpy(dest_buffer, tern_buff_offset, pitch);
		dest_buffer += pitch;
		tern_buff_offset -= pitch;
	}

	//�о�� ��������Ʈ�� �����Ѵ�.
	delete []src_buffer;

	//��������Ʈ �����ڿ� ��Ÿ ������ �����Ѵ�.
	_sprites[idx_sprite]._senter_x = senter_x;
	_sprites[idx_sprite]._senter_y = setner_y;
	
	return TRUE;
}


/**
@brief �ش� �ε����� ��������Ʈ �����Ѵ�.

@param idx_sprite : ���� �� ��������Ʈ �ε���
*/
void CSpriteDib::ReleaseSprite(int idx_sprite)
{

	//�ִ� �Ҵ�� ��������Ʈ�� �Ѵ��� üũ�Ѵ�.
	if (_max_cnt_sprite < idx_sprite) {

		return;
	}

	if (NULL != _sprites[idx_sprite]._sprite_buffer){

		//���� �� �ʱ�ȭ�Ѵ�.
		delete[] _sprites[idx_sprite]._sprite_buffer;
		//memset(&_sprites[idx_sprite], 0, sizeof(SPRITE));

		//�� ó�� memset�� �����൵ ������.
		_sprites[idx_sprite]._sprite_buffer = NULL;
	}
}


/**
@brief �޸� ����(DIB ����)�� Ư�� ��ġ�� ��������Ʈ�� ����Ѵ�. ( Ŭ���� ó��, Į��Ű ó��)

@param idx_sprite : ������ ��������Ʈ �ε���.
@param draw_start_x : ��������Ʈ�� �׸��� ���� �� x.
@param draw_start_y : ��������Ʈ�� �׸��� ���� �� y.
@param dest_buffer : ��������Ʈ�� ����� �޸� ����(DIB ����).
@param dest_width : ��������Ʈ�� ����� �޸� ������ �ʺ�.
@param dest_height : ��������Ʈ�� ����� �޸� ������ ����.
@param dest_pitch : ��������Ʈ�� ��� �� �޸� ������ ��ġ.
@draw_len : ��� ����.
*/
void CSpriteDib::DrawFullSprite(int idx_sprite, int draw_start_x, int draw_start_y,
							BYTE *dest_buffer, int dest_width, int dest_height, int dest_pitch, int draw_len ){

	//�ִ� ��������Ʈ ������ �ʰ� �ϰų�, �ε���� �ʴ� ��������Ʈ��� �����Ѵ�.
	if (idx_sprite >= _max_cnt_sprite) {

		return;
	}

	//���ۿ� �̹����� �ִ��� üũ�Ѵ�.
	if (NULL == _sprites[idx_sprite]._sprite_buffer) {

		return;
	}

	//��������Ʈ ���� �������� �����Ѵ�.
	SPRITE * current_sprite = &_sprites[idx_sprite];

	//��������Ʈ ����, �ʺ� �����Ѵ�.
	int sprite_width = current_sprite->_width;
	int sprite_height = current_sprite->_height;


	//������ �����Ѵ�. (�⺻ ���� 100�̴�)
	sprite_width = sprite_width * draw_len / 100;

	//��� �� ������ ���� ������(DIB ����)�� ���� ����� �ҽ� �������� �����Ѵ�.
	DWORD *dest_buffer_offset = (DWORD *)dest_buffer;
	DWORD *src_buffer_offset = (DWORD *)(current_sprite->_sprite_buffer);



	//��� ���� ó���Ѵ�.
	draw_start_x = draw_start_x - current_sprite->_senter_x;
	draw_start_y = draw_start_y - current_sprite->_senter_y;


	//��� Ŭ����
	//��� �� ���� ��������Ʈ ��� ��ġ ����Ѵ�. 
	if (0 > draw_start_y){

		//��� ���̸� ���δ�. (©�� �κ��� �������ش�)
		sprite_height = sprite_height - (-draw_start_y);

		//��� ���� �������� �����Ѵ�. (©�� �κ� �̿ܺ��� ����� �����Ѵ�)
		src_buffer_offset = (DWORD*)(current_sprite->_sprite_buffer + current_sprite->_pitch * (-draw_start_y));

		//��� ������ �������Ѵ�.
		draw_start_y = 0;
	}


	//�ϴ� Ŭ����
	//�ϴ� �� ���� ��������Ʈ ��� ��ġ ����Ѵ�. 
	if (dest_height <= draw_start_y + current_sprite->_height ){

		//��� �������� �ٲ��� �ʰ� ��� ���̰� �ٲ��.
		sprite_height -= ((draw_start_y + current_sprite->_height) - dest_height);
	}



	//���� Ŭ����.
	//���� ��� ��ġ ����Ѵ�. 
	if (0 > draw_start_x){

		sprite_width = sprite_width - (-draw_start_x);
		src_buffer_offset = src_buffer_offset + (-draw_start_x);

		//������ ���� �߸��Ƿ� ��� ���� ��ġ�� ���������� �δ�.
		draw_start_x = 0;
	}


	//���� Ŭ����.
	//������ ��� ��ġ ����Ѵ�. 
	if (dest_width <= draw_start_x + current_sprite->_width){

		sprite_width -= ((draw_start_x + current_sprite->_width) - dest_width);

	}

	
	//���� �׸��� ���� ��츦 üũ�Ѵ�.
	if (sprite_width <= 0 || sprite_height <= 0) {
	
		return;
	}


	//ȭ�鿡 ���� ��ġ�� �̵��Ѵ�.
	dest_buffer_offset = (DWORD *)(((BYTE*)(dest_buffer_offset + draw_start_x) + (draw_start_y * dest_pitch)));


	BYTE *dest_buffer_origin = (BYTE*)dest_buffer_offset;
	BYTE *src_buffer_origin = (BYTE*)src_buffer_offset;


	DWORD color_ckey = _color_key;
	int current_sprite_pitch = current_sprite->_pitch;

	//	��ü ũ�⸦ ���鼭 �ȼ����� ����� ó���� �ϸ� �׸� ����Ѵ�.
	for (int idx_y = 0; sprite_height > idx_y; idx_y++){

		for (int idx_x=0; sprite_width > idx_x; idx_x++){

			//�÷�Ű�� �̿��� ��������Ʈ�� ����Ѵ�.
			if (color_ckey != ( *src_buffer_offset & 0x00ffffff) ){

				*dest_buffer_offset = *src_buffer_offset;
			}
			
			dest_buffer_offset++;
			src_buffer_offset++;

		}

		//���� �ٷ� �̵�, ȭ��� ��������Ʈ �����Ѵ�.
		dest_buffer_origin = dest_buffer_origin + dest_pitch;
		src_buffer_origin = src_buffer_origin + current_sprite_pitch;

		//���� ������ �����Ѵ�.
		dest_buffer_offset = (DWORD *)dest_buffer_origin;
		src_buffer_offset = (DWORD *)src_buffer_origin;
	}
}


/**
@brief �޸� ����(DIB ����)�� Ư�� ��ġ�� ��������Ʈ�� ����Ѵ�. ( Ŭ���� ó��, Į��Ű ó��)

@param idx_sprite : ������ ��������Ʈ �ε���.
@param draw_start_x : ��������Ʈ�� �׸��� ���� �� x.
@param draw_start_y : ��������Ʈ�� �׸��� ���� �� y.
@param dest_buffer : ��������Ʈ�� ����� �޸� ����(DIB ����).
@param dest_width : ��������Ʈ�� ����� �޸� ������ �ʺ�.
@param dest_height : ��������Ʈ�� ����� �޸� ������ ����.
@param dest_pitch : ��������Ʈ�� ��� �� �޸� ������ ��ġ.
@draw_len : ��� ����.
*/
void CSpriteDib::DrawFullSpriteLikeMonster(int idx_sprite, int draw_start_x, int draw_start_y,
	BYTE *dest_buffer, int dest_width, int dest_height, int dest_pitch, int draw_len) {

	//�ִ� ��������Ʈ ������ �ʰ� �ϰų�, �ε���� �ʴ� ��������Ʈ��� �����Ѵ�.
	if (idx_sprite >= _max_cnt_sprite) {

		return;
	}

	//���ۿ� �̹����� �ִ��� üũ�Ѵ�.
	if (NULL == _sprites[idx_sprite]._sprite_buffer) {

		return;
	}

	//��������Ʈ ���� �������� �����Ѵ�.
	SPRITE * current_sprite = &_sprites[idx_sprite];

	//��������Ʈ ����, �ʺ� �����Ѵ�.
	int sprite_width = current_sprite->_width;
	int sprite_height = current_sprite->_height;


	//������ �����Ѵ�. (�⺻ ���� 100�̴�)
	sprite_width = sprite_width * draw_len / 100;

	//��� �� ������ ���� ������(DIB ����)�� ���� ����� �ҽ� �������� �����Ѵ�.
	DWORD *dest_buffer_offset = (DWORD *)dest_buffer;
	DWORD *src_buffer_offset = (DWORD *)(current_sprite->_sprite_buffer);



	//��� ���� ó���Ѵ�.
	draw_start_x = draw_start_x - current_sprite->_senter_x;
	draw_start_y = draw_start_y - current_sprite->_senter_y;


	//��� Ŭ����
	//��� �� ���� ��������Ʈ ��� ��ġ ����Ѵ�. 
	if (0 > draw_start_y) {

		//��� ���̸� ���δ�. (©�� �κ��� �������ش�)
		sprite_height = sprite_height - (-draw_start_y);

		//��� ���� �������� �����Ѵ�. (©�� �κ� �̿ܺ��� ����� �����Ѵ�)
		src_buffer_offset = (DWORD*)(current_sprite->_sprite_buffer + current_sprite->_pitch * (-draw_start_y));

		//��� ������ �������Ѵ�.
		draw_start_y = 0;
	}


	//�ϴ� Ŭ����
	//�ϴ� �� ���� ��������Ʈ ��� ��ġ ����Ѵ�. 
	if (dest_height <= draw_start_y + current_sprite->_height) {

		//��� �������� �ٲ��� �ʰ� ��� ���̰� �ٲ��.
		sprite_height -= ((draw_start_y + current_sprite->_height) - dest_height);
	}



	//���� Ŭ����.
	//���� ��� ��ġ ����Ѵ�. 
	if (0 > draw_start_x) {

		sprite_width = sprite_width - (-draw_start_x);
		src_buffer_offset = src_buffer_offset + (-draw_start_x);

		//������ ���� �߸��Ƿ� ��� ���� ��ġ�� ���������� �δ�.
		draw_start_x = 0;
	}


	//���� Ŭ����.
	//������ ��� ��ġ ����Ѵ�. 
	if (dest_width <= draw_start_x + current_sprite->_width) {

		sprite_width -= ((draw_start_x + current_sprite->_width) - dest_width);

	}


	//���� �׸��� ���� ��츦 üũ�Ѵ�.
	if (sprite_width <= 0 || sprite_height <= 0) {

		return;
	}


	//ȭ�鿡 ���� ��ġ�� �̵��Ѵ�.
	dest_buffer_offset = (DWORD *)(((BYTE*)(dest_buffer_offset + draw_start_x) + (draw_start_y * dest_pitch)));


	BYTE *dest_buffer_origin = (BYTE*)dest_buffer_offset;
	BYTE *src_buffer_origin = (BYTE*)src_buffer_offset;


	DWORD color_ckey = _color_key;
	int current_sprite_pitch = current_sprite->_pitch;

	//	��ü ũ�⸦ ���鼭 �ȼ����� ����� ó���� �ϸ� �׸� ����Ѵ�.
	for (int idx_y = 0; sprite_height > idx_y; idx_y++) {

		for (int idx_x = 0; sprite_width > idx_x; idx_x++) {

			//�÷�Ű�� �̿��� ��������Ʈ�� ����Ѵ�.
			if (color_ckey != (*src_buffer_offset & 0x00ffffff)) {

				*((BYTE*)dest_buffer_offset) = GetRValue(src_buffer_offset);
				*((BYTE*)dest_buffer_offset+1) = GetGValue(src_buffer_offset) / 2;
				*((BYTE*)dest_buffer_offset+2) = 50;
			}

			dest_buffer_offset++;
			src_buffer_offset++;

		}

		//���� �ٷ� �̵�, ȭ��� ��������Ʈ �����Ѵ�.
		dest_buffer_origin = dest_buffer_origin + dest_pitch;
		src_buffer_origin = src_buffer_origin + current_sprite_pitch;

		//���� ������ �����Ѵ�.
		dest_buffer_offset = (DWORD *)dest_buffer_origin;
		src_buffer_offset = (DWORD *)src_buffer_origin;
	}
}







/**
@brief �޸� ����(DIB ����)�� Ư�� ��ġ�� ��������Ʈ�� ����Ѵ�. ( Ŭ���� ó��, Į��Ű ó��)

@param idx_sprite : ������ ��������Ʈ �ε���.
@param draw_start_x : ��������Ʈ�� �׸��� ���� �� x.
@param draw_start_y : ��������Ʈ�� �׸��� ���� �� y.
@param dest_buffer : ��������Ʈ�� ����� �޸� ����(DIB ����).
@param dest_width : ��������Ʈ�� ����� �޸� ������ �ʺ�.
@param dest_height : ��������Ʈ�� ����� �޸� ������ ����.
@param dest_pitch : ��������Ʈ�� ��� �� �޸� ������ ��ġ.
@draw_len : ��� ����
*/
void CSpriteDib::DrawVariationSprite(int idx_sprite, int draw_start_x, int draw_start_y,
	BYTE *dest_buffer, int dest_width, int dest_height, int dest_pitch, int draw_len) {

	//�ִ� ��������Ʈ ������ �ʰ� �ϰų�, �ε���� �ʴ� ��������Ʈ��� �����Ѵ�.
	if (idx_sprite >= _max_cnt_sprite) {

		return;
	}

	//���ۿ� �̹����� �ִ��� üũ�Ѵ�.
	if (NULL == _sprites[idx_sprite]._sprite_buffer) {

		return;
	}

	//��������Ʈ ���� �������� �����Ѵ�.
	SPRITE * current_sprite = &_sprites[idx_sprite];

	//��������Ʈ ����, �ʺ� �����Ѵ�.
	int sprite_width = current_sprite->_width;
	int sprite_height = current_sprite->_height;


	//������ �����Ѵ�. (�⺻ ���� 100�̴�)
	sprite_width = sprite_width * draw_len / 100;

	//��� �� ������ ���� ������(DIB ����)�� ���� ����� �ҽ� �������� �����Ѵ�.
	DWORD *dest_buffer_offset = (DWORD *)dest_buffer;
	DWORD *src_buffer_offset = (DWORD *)(current_sprite->_sprite_buffer);



	//��� ���� ó���Ѵ�.
	draw_start_x = draw_start_x - current_sprite->_senter_x;
	draw_start_y = draw_start_y - current_sprite->_senter_y;


	//��� Ŭ����.
	//��� �� ���� ��������Ʈ ��� ��ġ ����Ѵ�. 
	if (0 > draw_start_y) {

		//��� ���̸� ���δ�. (©�� �κ��� �������ش�)
		sprite_height = sprite_height - (-draw_start_y);

		//��� ���� �������� �����Ѵ�. (©�� �κ� �̿ܺ��� ����� �����Ѵ�)
		src_buffer_offset = (DWORD*)(current_sprite->_sprite_buffer + current_sprite->_pitch * (-draw_start_y));

		//��� ������ �������Ѵ�.
		draw_start_y = 0;
	}


	//�ϴ� Ŭ����.
	//�ϴ� �� ���� ��������Ʈ ��� ��ġ ����Ѵ�. 
	if (dest_height <= draw_start_y + current_sprite->_height) {

		//��� �������� �ٲ��� �ʰ� ��� ���̰� �ٲ��.
		sprite_height -= ((draw_start_y + current_sprite->_height) - dest_height);
	}



	//���� Ŭ����.
	//���� ��� ��ġ ����Ѵ�. 
	if (0 > draw_start_x) {

		sprite_width = sprite_width - (-draw_start_x);
		src_buffer_offset = src_buffer_offset + (-draw_start_x);

		//������ ���� �߸��Ƿ� ��� ���� ��ġ�� ���������� �δ�.
		draw_start_x = 0;
	}


	//���� Ŭ����.
	//������ ��� ��ġ ����Ѵ�. 
	if (dest_width <= draw_start_x + current_sprite->_width) {

		sprite_width -= ((draw_start_x + current_sprite->_width) - dest_width);

	}


	//���� �׸��� ���� ��츦 üũ�Ѵ�.
	if (sprite_width <= 0 || sprite_height <= 0) {

		return;
	}


	//ȭ�鿡 ���� ��ġ�� �̵��Ѵ�.
	dest_buffer_offset = (DWORD *)(((BYTE*)(dest_buffer_offset + draw_start_x) + (draw_start_y * dest_pitch)));


	BYTE *dest_buffer_origin = (BYTE*)dest_buffer_offset;
	BYTE *src_buffer_origin = (BYTE*)src_buffer_offset;


	DWORD color_ckey = _color_key;
	int current_sprite_pitch = current_sprite->_pitch;

	//	��ü ũ�⸦ ���鼭 �ȼ����� ����� ó���� �ϸ� �׸� ����Ѵ�.
	for (int idx_y = 0; sprite_height > idx_y; idx_y++) {

		for (int idx_x = 0; sprite_width > idx_x; idx_x++) {

			//�÷�Ű�� �̿��� ��������Ʈ�� ����Ѵ�.
			if (color_ckey != (*src_buffer_offset & 0x00ffffff)) {

				*dest_buffer_offset = *src_buffer_offset;
			}

			dest_buffer_offset++;
			src_buffer_offset++;

		}

		//���� �ٷ� �̵�, ȭ��� ��������Ʈ �����Ѵ�.
		dest_buffer_origin = dest_buffer_origin + dest_pitch;
		src_buffer_origin = src_buffer_origin + current_sprite_pitch;

		//���� ������ �����Ѵ�.
		dest_buffer_offset = (DWORD *)dest_buffer_origin;
		src_buffer_offset = (DWORD *)src_buffer_origin;
	}
}




/**
@brief �޸� ����(DIB ����)�� Ư�� ��ġ�� ��������Ʈ�� ����Ѵ�. ( Ŭ���� ó��)

@param idx_sprite : ������ ��������Ʈ �ε���.
@param draw_start_x : ��������Ʈ�� �׸��� ���� �� x.
@param draw_start_y : ��������Ʈ�� �׸��� ���� �� y.
@param dest_buffer : ��������Ʈ�� ����� �޸� ����(DIB ����).
@param dest_width : ��������Ʈ�� ����� �޸� ������ �ʺ�.
@param dest_height : ��������Ʈ�� ����� �޸� ������ ����.
@param dest_pitch : ��������Ʈ�� ��� �� �޸� ������ ��ġ.
@draw_len : ��� ����.
*/
void CSpriteDib::DrawImage(int idx_sprite, int draw_start_x, int draw_start_y, 
	BYTE *dest_buffer, int dest_width, int dest_height, int dest_pitch, int draw_len ){

	//�ִ� ��������Ʈ ������ �ʰ� �ϰų�, �ε���� �ʴ� ��������Ʈ��� �����Ѵ�.
	if (idx_sprite >= _max_cnt_sprite) {
	
		return;
	}

	//���ۿ� �̹����� �ִ��� üũ�Ѵ�.
	if (NULL == _sprites[idx_sprite]._sprite_buffer) {

		return;
	}

	//��������Ʈ ���� �������� �����Ѵ�.
	SPRITE *current_sprite = &_sprites[idx_sprite];

	//��������Ʈ ����, �ʺ� �����Ѵ�.
	int sprite_width = current_sprite->_width;
	int sprite_height = current_sprite->_height;

	//������ �����Ѵ�. (�⺻ ���� 100�̴�)
	sprite_width = sprite_width * draw_len / 100;

	//��� �� ������ ���� ������(DIB ����)�� ���� ����� �ҽ� �������� �����Ѵ�.
	DWORD *dest_buffer_offset = (DWORD *)dest_buffer;
	DWORD *src_buffer_offset = (DWORD *)(current_sprite->_sprite_buffer);


	//��� Ŭ����.
	//��� �� ���� ��������Ʈ ��� ��ġ ����Ѵ�. 
	if (0 > draw_start_y){

		//��� ���̸� ���δ�. (©�� �κ��� �������ش�)
		sprite_height = sprite_height - (-draw_start_y);

		//��� ���� �������� �����Ѵ�. (©�� �κ� �̿ܺ��� ����� �����Ѵ�)
		src_buffer_offset = (DWORD*)(current_sprite->_sprite_buffer + current_sprite->_pitch * (-draw_start_y));

		//��� ������ �������Ѵ�.
		draw_start_y = 0;
	}

	//�ϴ� Ŭ����.
	//�ϴ� �� ���� ��������Ʈ ��� ��ġ ����Ѵ�. 
	if (dest_height < draw_start_y + current_sprite->_height){

		//��� �������� �ٲ��� �ʰ� ��� ���̰� �ٲ��.
		sprite_height -= ((draw_start_y + current_sprite->_height) - dest_height);
	}


	//���� Ŭ����.
	//���� ��� ��ġ ����Ѵ�. 
	if (0 >draw_start_x){

		sprite_width = sprite_width - (-draw_start_x);
		src_buffer_offset = src_buffer_offset + (-draw_start_x);


		//������ ���� �߸��Ƿ� ��� ���� ��ġ�� ���������� �δ�.
		draw_start_x = 0;
	}

	//���� Ŭ����.
	//������ ��� ��ġ ����Ѵ�. 
	if (dest_width < draw_start_x + current_sprite->_width){

		sprite_width -= ((draw_start_x + current_sprite->_width) - dest_width);
	}


	//���� �׸��� ���� ��츦 üũ�Ѵ�.
	if (sprite_width <= 0 || sprite_height <= 0) {
	
		//�׸��� �ʰ� �����Ѵ�.
		return;
	}

	//ȭ�鿡 ���� ��ġ�� �̵��Ѵ�.
	dest_buffer_offset = (DWORD *)(((BYTE*)(dest_buffer_offset + draw_start_x) + (draw_start_y * dest_pitch)));


	//��ü ũ�⸦ ���鼭 �ȼ����� ����� ó���� �ϸ� �׸� ����Ѵ�.
	for (int height = 0; sprite_height > height; height++){

		//�����Ѵ�.
		memcpy(dest_buffer_offset, src_buffer_offset, sprite_width * 4);

		//�������� �����Ѵ�.
		dest_buffer_offset = (DWORD *)((BYTE *)dest_buffer_offset + dest_pitch);
		src_buffer_offset = (DWORD *)((BYTE *)src_buffer_offset + current_sprite->_pitch);
	}
}
