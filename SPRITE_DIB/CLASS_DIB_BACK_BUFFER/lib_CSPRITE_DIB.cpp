/**
@file lib_CSPRITE_DIB.cpp

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
#include "lib_CSPRITE_DIB.h"

//네임 스페이스 설정한다.
using namespace CSPRITE_DIB;

/**
@brief 생성자

@brief CSPriteDib 매니저를 생성한다.

@param max_cnt_sprite : 최대 저장 가능한 스프라이트 프레임(이미지) 갯수
@param color_key : 스프라이트 대상에 적용 할 컬러키
*/
CSpriteDib::CSpriteDib(int max_cnt_sprite, DWORD color_key){

	//최대 저장 가능한 스프라이트 갯수를 저장한다.
	_max_cnt_sprite = max_cnt_sprite;

	//------------------------------------------------------------------------------
	//	최대 읽어올 개수를 할당한다.
	//------------------------------------------------------------------------------
	_sprites = new SPRITE[max_cnt_sprite];
	memset(_sprites, 0, sizeof(SPRITE) * max_cnt_sprite);

	//투명 컬러 저장한다.
	_color_key = color_key;

	//현재 저장된 스프라이트 갯수는 0개이다.
	_cnt_sprites = 0;

}

/**
@brief 파괴자

@brief 할당한 모든 스프라이트 이미지를 해제한다.
*/
CSpriteDib::~CSpriteDib(){

	//전체 순회하며 모든 스프라이트를 삭제한다.
	for (int idx = 0; idx < _max_cnt_sprite; idx++){

		ReleaseSprite(idx);
	}
}


/**
@brief BMP 파일을 읽어서 하나의 프레임으로 저장한다.

@param idx_sprite : 프레임에 부여할 인덱스
@param file_name : 프레임이 저장된 bmp 파일 경로 이름
@param senter_x : 프레임의 중점 x 위치
@param senter_y : 프레임의 중점 y 위치
*/
BOOL CSpriteDib::LoadDibSprite(int idx_sprite, const WCHAR *bmp_file_path, int senter_x, int setner_y){

	HANDLE h_file;
	DWORD read;
	int pitch;
	int sz_image;
	int cnt;

	BITMAPFILEHEADER bmp_file_header;
	BITMAPINFOHEADER bmp_info_header;
	
	
	//비트맵 파일 을 오픈한다.
	h_file = CreateFile( bmp_file_path, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); 
	if ( INVALID_HANDLE_VALUE == h_file ){

		return FALSE;
	}

	//기존 스프라이트가 존재 시 해제한다.
	ReleaseSprite(idx_sprite);

	// 파일정보헤드 읽는다.
	ReadFile(h_file, &bmp_file_header, sizeof(BITMAPFILEHEADER), &read, NULL);

	// 파일 타입이 비트맵인지 "BM" (0x4D42<빅엔디안>) 검사한다.
	if (bmp_file_header.bfType != 0x4d42) {
	
		return false;
	}

	// 영상정보헤드 읽는다.
	ReadFile(h_file, &bmp_info_header, sizeof(BITMAPINFOHEADER),&read, NULL);

	//비트 카운트 체크하여 32비트인지 확인한다.
	if (bmp_info_header.biBitCount != 32){

		CloseHandle(h_file);
		return FALSE;
	}

	//한줄, 한줄의 피치값 구한다.
	pitch = (bmp_info_header.biWidth * (32 / 8) + 3)&~3;
	
	// 스프라이트 정보를 저장한다.
	_sprites[idx_sprite]._height = bmp_info_header.biHeight;
	_sprites[idx_sprite]._width = bmp_info_header.biWidth;
	_sprites[idx_sprite]._pitch = pitch;

	//	이미지에 대한 전체 크기를 구하고, 메모리 할당한다.
	sz_image = pitch * bmp_info_header.biHeight;
	_sprites[idx_sprite]._sprite_buffer = new BYTE[sz_image];

	//------------------------------------------------------------------------------
	//	이미지 부분은 스프라이트 버퍼로 읽어온다.
	//	DIB는 뒤집어져 있으므로 다시 뒤집자.
	//	임시 버퍼에 읽은 뒤에 뒤집으면서 복사한다.
	//------------------------------------------------------------------------------
	BYTE *src_buffer = new BYTE[sz_image];                    //읽은 스프라이프를 저장한 공간
	BYTE *dest_buffer = _sprites[idx_sprite]._sprite_buffer;  //읽은 스프라이프를 저장 할 버퍼 공간
	BYTE *tern_buff_offset;

	//저장 할 스프라이트를 읽어 저장한다.
	ReadFile(h_file, src_buffer, sz_image, &read, NULL);
	CloseHandle(h_file);

	//스프라이트의 마지막 피치의 처음 오프셋으로 설정한다.
	//인덱스로 접근하므로 높이 -1을 해주어야 한다.
	tern_buff_offset = src_buffer + pitch * (bmp_info_header.biHeight -1);

	//스프라이트를 복사한다.
	for (cnt = 0; cnt < bmp_info_header.biHeight; cnt++){

		memcpy(dest_buffer, tern_buff_offset, pitch);
		dest_buffer += pitch;
		tern_buff_offset -= pitch;
	}

	//읽어온 스프라이트를 해제한다.
	delete []src_buffer;

	//스프라이트 관리자에 기타 정보를 저장한다.
	_sprites[idx_sprite]._senter_x = senter_x;
	_sprites[idx_sprite]._senter_y = setner_y;
	
	return TRUE;
}


/**
@brief 해당 인덱스의 스프라이트 해제한다.

@param idx_sprite : 삭제 할 스프라이트 인덱스
*/
void CSpriteDib::ReleaseSprite(int idx_sprite)
{

	//최대 할당된 스프라이트를 넘는지 체크한다.
	if (_max_cnt_sprite < idx_sprite) {

		return;
	}

	if (NULL != _sprites[idx_sprite]._sprite_buffer){

		//삭제 후 초기화한다.
		delete[] _sprites[idx_sprite]._sprite_buffer;
		//memset(&_sprites[idx_sprite], 0, sizeof(SPRITE));

		//위 처럼 memset을 안해줘도 괜찮다.
		_sprites[idx_sprite]._sprite_buffer = NULL;
	}
}


/**
@brief 메모리 버퍼(DIB 버퍼)의 특정 위치에 스프라이트를 출력한다. ( 클리핑 처리, 칼라키 처리)

@param idx_sprite : 선택한 스프라이트 인덱스.
@param draw_start_x : 스프라이트를 그리기 시작 점 x.
@param draw_start_y : 스프라이트를 그리기 시작 점 y.
@param dest_buffer : 스프라이트를 출력할 메모리 버퍼(DIB 버퍼).
@param dest_width : 스프라이트를 출력할 메모리 버퍼의 너비.
@param dest_height : 스프라이트를 출력할 메모리 버퍼의 높이.
@param dest_pitch : 스프라이트를 출력 할 메모리 버퍼의 피치.
@draw_len : 출력 비율.
*/
void CSpriteDib::DrawFullSprite(int idx_sprite, int draw_start_x, int draw_start_y,
							BYTE *dest_buffer, int dest_width, int dest_height, int dest_pitch, int draw_len ){

	//최대 스프라이트 갯수를 초과 하거나, 로드되지 않는 스프라이트라면 무시한다.
	if (idx_sprite >= _max_cnt_sprite) {

		return;
	}

	//버퍼에 이미지가 있는지 체크한다.
	if (NULL == _sprites[idx_sprite]._sprite_buffer) {

		return;
	}

	//스프라이트 시작 오프셋을 저장한다.
	SPRITE * current_sprite = &_sprites[idx_sprite];

	//스프라이트 높이, 너비를 저장한다.
	int sprite_width = current_sprite->_width;
	int sprite_height = current_sprite->_height;


	//비율을 조정한다. (기본 값이 100이다)
	sprite_width = sprite_width * draw_len / 100;

	//출력 할 목적지 버퍼 오프셋(DIB 버퍼)과 복사 대상인 소스 오프셋을 설정한다.
	DWORD *dest_buffer_offset = (DWORD *)dest_buffer;
	DWORD *src_buffer_offset = (DWORD *)(current_sprite->_sprite_buffer);



	//출력 중점 처리한다.
	draw_start_x = draw_start_x - current_sprite->_senter_x;
	draw_start_y = draw_start_y - current_sprite->_senter_y;


	//상단 클리핑
	//상단 에 대한 스프라이트 출력 위치 계산한다. 
	if (0 > draw_start_y){

		//출력 높이를 줄인다. (짤린 부분은 제외해준다)
		sprite_height = sprite_height - (-draw_start_y);

		//출력 시작 오프셋을 변경한다. (짤린 부분 이외부터 출력을 시작한다)
		src_buffer_offset = (DWORD*)(current_sprite->_sprite_buffer + current_sprite->_pitch * (-draw_start_y));

		//출력 시작점 재지정한다.
		draw_start_y = 0;
	}


	//하단 클리핑
	//하단 에 대한 스프라이트 출력 위치 계산한다. 
	if (dest_height <= draw_start_y + current_sprite->_height ){

		//출력 시작점은 바뀌지 않고 출력 높이가 바뀐다.
		sprite_height -= ((draw_start_y + current_sprite->_height) - dest_height);
	}



	//좌측 클리핑.
	//왼쪽 출력 위치 계산한다. 
	if (0 > draw_start_x){

		sprite_width = sprite_width - (-draw_start_x);
		src_buffer_offset = src_buffer_offset + (-draw_start_x);

		//왼쪽이 조금 잘리므로 출력 시작 위치를 오른쪽으로 민다.
		draw_start_x = 0;
	}


	//우측 클리핑.
	//오른쪽 출력 위치 계산한다. 
	if (dest_width <= draw_start_x + current_sprite->_width){

		sprite_width -= ((draw_start_x + current_sprite->_width) - dest_width);

	}

	
	//찍을 그림이 없는 경우를 체크한다.
	if (sprite_width <= 0 || sprite_height <= 0) {
	
		return;
	}


	//화면에 찍을 위치로 이동한다.
	dest_buffer_offset = (DWORD *)(((BYTE*)(dest_buffer_offset + draw_start_x) + (draw_start_y * dest_pitch)));


	BYTE *dest_buffer_origin = (BYTE*)dest_buffer_offset;
	BYTE *src_buffer_origin = (BYTE*)src_buffer_offset;


	DWORD color_ckey = _color_key;
	int current_sprite_pitch = current_sprite->_pitch;

	//	전체 크기를 돌면서 픽셀마다 투명색 처리를 하며 그림 출력한다.
	for (int idx_y = 0; sprite_height > idx_y; idx_y++){

		for (int idx_x=0; sprite_width > idx_x; idx_x++){

			//컬러키는 이외의 스프라이트만 출력한다.
			if (color_ckey != ( *src_buffer_offset & 0x00ffffff) ){

				*dest_buffer_offset = *src_buffer_offset;
			}
			
			dest_buffer_offset++;
			src_buffer_offset++;

		}

		//다음 줄로 이동, 화면과 스프라이트 갱신한다.
		dest_buffer_origin = dest_buffer_origin + dest_pitch;
		src_buffer_origin = src_buffer_origin + current_sprite_pitch;

		//다음 오프셋 설정한다.
		dest_buffer_offset = (DWORD *)dest_buffer_origin;
		src_buffer_offset = (DWORD *)src_buffer_origin;
	}
}


/**
@brief 메모리 버퍼(DIB 버퍼)의 특정 위치에 스프라이트를 출력한다. ( 클리핑 처리, 칼라키 처리)

@param idx_sprite : 선택한 스프라이트 인덱스.
@param draw_start_x : 스프라이트를 그리기 시작 점 x.
@param draw_start_y : 스프라이트를 그리기 시작 점 y.
@param dest_buffer : 스프라이트를 출력할 메모리 버퍼(DIB 버퍼).
@param dest_width : 스프라이트를 출력할 메모리 버퍼의 너비.
@param dest_height : 스프라이트를 출력할 메모리 버퍼의 높이.
@param dest_pitch : 스프라이트를 출력 할 메모리 버퍼의 피치.
@draw_len : 출력 비율.
*/
void CSpriteDib::DrawFullSpriteLikeMonster(int idx_sprite, int draw_start_x, int draw_start_y,
	BYTE *dest_buffer, int dest_width, int dest_height, int dest_pitch, int draw_len) {

	//최대 스프라이트 갯수를 초과 하거나, 로드되지 않는 스프라이트라면 무시한다.
	if (idx_sprite >= _max_cnt_sprite) {

		return;
	}

	//버퍼에 이미지가 있는지 체크한다.
	if (NULL == _sprites[idx_sprite]._sprite_buffer) {

		return;
	}

	//스프라이트 시작 오프셋을 저장한다.
	SPRITE * current_sprite = &_sprites[idx_sprite];

	//스프라이트 높이, 너비를 저장한다.
	int sprite_width = current_sprite->_width;
	int sprite_height = current_sprite->_height;


	//비율을 조정한다. (기본 값이 100이다)
	sprite_width = sprite_width * draw_len / 100;

	//출력 할 목적지 버퍼 오프셋(DIB 버퍼)과 복사 대상인 소스 오프셋을 설정한다.
	DWORD *dest_buffer_offset = (DWORD *)dest_buffer;
	DWORD *src_buffer_offset = (DWORD *)(current_sprite->_sprite_buffer);



	//출력 중점 처리한다.
	draw_start_x = draw_start_x - current_sprite->_senter_x;
	draw_start_y = draw_start_y - current_sprite->_senter_y;


	//상단 클리핑
	//상단 에 대한 스프라이트 출력 위치 계산한다. 
	if (0 > draw_start_y) {

		//출력 높이를 줄인다. (짤린 부분은 제외해준다)
		sprite_height = sprite_height - (-draw_start_y);

		//출력 시작 오프셋을 변경한다. (짤린 부분 이외부터 출력을 시작한다)
		src_buffer_offset = (DWORD*)(current_sprite->_sprite_buffer + current_sprite->_pitch * (-draw_start_y));

		//출력 시작점 재지정한다.
		draw_start_y = 0;
	}


	//하단 클리핑
	//하단 에 대한 스프라이트 출력 위치 계산한다. 
	if (dest_height <= draw_start_y + current_sprite->_height) {

		//출력 시작점은 바뀌지 않고 출력 높이가 바뀐다.
		sprite_height -= ((draw_start_y + current_sprite->_height) - dest_height);
	}



	//좌측 클리핑.
	//왼쪽 출력 위치 계산한다. 
	if (0 > draw_start_x) {

		sprite_width = sprite_width - (-draw_start_x);
		src_buffer_offset = src_buffer_offset + (-draw_start_x);

		//왼쪽이 조금 잘리므로 출력 시작 위치를 오른쪽으로 민다.
		draw_start_x = 0;
	}


	//우측 클리핑.
	//오른쪽 출력 위치 계산한다. 
	if (dest_width <= draw_start_x + current_sprite->_width) {

		sprite_width -= ((draw_start_x + current_sprite->_width) - dest_width);

	}


	//찍을 그림이 없는 경우를 체크한다.
	if (sprite_width <= 0 || sprite_height <= 0) {

		return;
	}


	//화면에 찍을 위치로 이동한다.
	dest_buffer_offset = (DWORD *)(((BYTE*)(dest_buffer_offset + draw_start_x) + (draw_start_y * dest_pitch)));


	BYTE *dest_buffer_origin = (BYTE*)dest_buffer_offset;
	BYTE *src_buffer_origin = (BYTE*)src_buffer_offset;


	DWORD color_ckey = _color_key;
	int current_sprite_pitch = current_sprite->_pitch;

	//	전체 크기를 돌면서 픽셀마다 투명색 처리를 하며 그림 출력한다.
	for (int idx_y = 0; sprite_height > idx_y; idx_y++) {

		for (int idx_x = 0; sprite_width > idx_x; idx_x++) {

			//컬러키는 이외의 스프라이트만 출력한다.
			if (color_ckey != (*src_buffer_offset & 0x00ffffff)) {

				*((BYTE*)dest_buffer_offset) = GetRValue(src_buffer_offset);
				*((BYTE*)dest_buffer_offset+1) = GetGValue(src_buffer_offset) / 2;
				*((BYTE*)dest_buffer_offset+2) = 50;
			}

			dest_buffer_offset++;
			src_buffer_offset++;

		}

		//다음 줄로 이동, 화면과 스프라이트 갱신한다.
		dest_buffer_origin = dest_buffer_origin + dest_pitch;
		src_buffer_origin = src_buffer_origin + current_sprite_pitch;

		//다음 오프셋 설정한다.
		dest_buffer_offset = (DWORD *)dest_buffer_origin;
		src_buffer_offset = (DWORD *)src_buffer_origin;
	}
}







/**
@brief 메모리 버퍼(DIB 버퍼)의 특정 위치에 스프라이트를 출력한다. ( 클리핑 처리, 칼라키 처리)

@param idx_sprite : 선택한 스프라이트 인덱스.
@param draw_start_x : 스프라이트를 그리기 시작 점 x.
@param draw_start_y : 스프라이트를 그리기 시작 점 y.
@param dest_buffer : 스프라이트를 출력할 메모리 버퍼(DIB 버퍼).
@param dest_width : 스프라이트를 출력할 메모리 버퍼의 너비.
@param dest_height : 스프라이트를 출력할 메모리 버퍼의 높이.
@param dest_pitch : 스프라이트를 출력 할 메모리 버퍼의 피치.
@draw_len : 출력 비율
*/
void CSpriteDib::DrawVariationSprite(int idx_sprite, int draw_start_x, int draw_start_y,
	BYTE *dest_buffer, int dest_width, int dest_height, int dest_pitch, int draw_len) {

	//최대 스프라이트 갯수를 초과 하거나, 로드되지 않는 스프라이트라면 무시한다.
	if (idx_sprite >= _max_cnt_sprite) {

		return;
	}

	//버퍼에 이미지가 있는지 체크한다.
	if (NULL == _sprites[idx_sprite]._sprite_buffer) {

		return;
	}

	//스프라이트 시작 오프셋을 저장한다.
	SPRITE * current_sprite = &_sprites[idx_sprite];

	//스프라이트 높이, 너비를 저장한다.
	int sprite_width = current_sprite->_width;
	int sprite_height = current_sprite->_height;


	//비율을 조정한다. (기본 값이 100이다)
	sprite_width = sprite_width * draw_len / 100;

	//출력 할 목적지 버퍼 오프셋(DIB 버퍼)과 복사 대상인 소스 오프셋을 설정한다.
	DWORD *dest_buffer_offset = (DWORD *)dest_buffer;
	DWORD *src_buffer_offset = (DWORD *)(current_sprite->_sprite_buffer);



	//출력 중점 처리한다.
	draw_start_x = draw_start_x - current_sprite->_senter_x;
	draw_start_y = draw_start_y - current_sprite->_senter_y;


	//상단 클리핑.
	//상단 에 대한 스프라이트 출력 위치 계산한다. 
	if (0 > draw_start_y) {

		//출력 높이를 줄인다. (짤린 부분은 제외해준다)
		sprite_height = sprite_height - (-draw_start_y);

		//출력 시작 오프셋을 변경한다. (짤린 부분 이외부터 출력을 시작한다)
		src_buffer_offset = (DWORD*)(current_sprite->_sprite_buffer + current_sprite->_pitch * (-draw_start_y));

		//출력 시작점 재지정한다.
		draw_start_y = 0;
	}


	//하단 클리핑.
	//하단 에 대한 스프라이트 출력 위치 계산한다. 
	if (dest_height <= draw_start_y + current_sprite->_height) {

		//출력 시작점은 바뀌지 않고 출력 높이가 바뀐다.
		sprite_height -= ((draw_start_y + current_sprite->_height) - dest_height);
	}



	//좌측 클리핑.
	//왼쪽 출력 위치 계산한다. 
	if (0 > draw_start_x) {

		sprite_width = sprite_width - (-draw_start_x);
		src_buffer_offset = src_buffer_offset + (-draw_start_x);

		//왼쪽이 조금 잘리므로 출력 시작 위치를 오른쪽으로 민다.
		draw_start_x = 0;
	}


	//우측 클리핑.
	//오른쪽 출력 위치 계산한다. 
	if (dest_width <= draw_start_x + current_sprite->_width) {

		sprite_width -= ((draw_start_x + current_sprite->_width) - dest_width);

	}


	//찍을 그림이 없는 경우를 체크한다.
	if (sprite_width <= 0 || sprite_height <= 0) {

		return;
	}


	//화면에 찍을 위치로 이동한다.
	dest_buffer_offset = (DWORD *)(((BYTE*)(dest_buffer_offset + draw_start_x) + (draw_start_y * dest_pitch)));


	BYTE *dest_buffer_origin = (BYTE*)dest_buffer_offset;
	BYTE *src_buffer_origin = (BYTE*)src_buffer_offset;


	DWORD color_ckey = _color_key;
	int current_sprite_pitch = current_sprite->_pitch;

	//	전체 크기를 돌면서 픽셀마다 투명색 처리를 하며 그림 출력한다.
	for (int idx_y = 0; sprite_height > idx_y; idx_y++) {

		for (int idx_x = 0; sprite_width > idx_x; idx_x++) {

			//컬러키는 이외의 스프라이트만 출력한다.
			if (color_ckey != (*src_buffer_offset & 0x00ffffff)) {

				*dest_buffer_offset = *src_buffer_offset;
			}

			dest_buffer_offset++;
			src_buffer_offset++;

		}

		//다음 줄로 이동, 화면과 스프라이트 갱신한다.
		dest_buffer_origin = dest_buffer_origin + dest_pitch;
		src_buffer_origin = src_buffer_origin + current_sprite_pitch;

		//다음 오프셋 설정한다.
		dest_buffer_offset = (DWORD *)dest_buffer_origin;
		src_buffer_offset = (DWORD *)src_buffer_origin;
	}
}




/**
@brief 메모리 버퍼(DIB 버퍼)의 특정 위치에 스프라이트를 출력한다. ( 클리핑 처리)

@param idx_sprite : 선택한 스프라이트 인덱스.
@param draw_start_x : 스프라이트를 그리기 시작 점 x.
@param draw_start_y : 스프라이트를 그리기 시작 점 y.
@param dest_buffer : 스프라이트를 출력할 메모리 버퍼(DIB 버퍼).
@param dest_width : 스프라이트를 출력할 메모리 버퍼의 너비.
@param dest_height : 스프라이트를 출력할 메모리 버퍼의 높이.
@param dest_pitch : 스프라이트를 출력 할 메모리 버퍼의 피치.
@draw_len : 출력 비율.
*/
void CSpriteDib::DrawImage(int idx_sprite, int draw_start_x, int draw_start_y, 
	BYTE *dest_buffer, int dest_width, int dest_height, int dest_pitch, int draw_len ){

	//최대 스프라이트 갯수를 초과 하거나, 로드되지 않는 스프라이트라면 무시한다.
	if (idx_sprite >= _max_cnt_sprite) {
	
		return;
	}

	//버퍼에 이미지가 있는지 체크한다.
	if (NULL == _sprites[idx_sprite]._sprite_buffer) {

		return;
	}

	//스프라이트 시작 오프셋을 저장한다.
	SPRITE *current_sprite = &_sprites[idx_sprite];

	//스프라이트 높이, 너비를 저장한다.
	int sprite_width = current_sprite->_width;
	int sprite_height = current_sprite->_height;

	//비율을 조정한다. (기본 값이 100이다)
	sprite_width = sprite_width * draw_len / 100;

	//출력 할 목적지 버퍼 오프셋(DIB 버퍼)과 복사 대상인 소스 오프셋을 설정한다.
	DWORD *dest_buffer_offset = (DWORD *)dest_buffer;
	DWORD *src_buffer_offset = (DWORD *)(current_sprite->_sprite_buffer);


	//상단 클리핑.
	//상단 에 대한 스프라이트 출력 위치 계산한다. 
	if (0 > draw_start_y){

		//출력 높이를 줄인다. (짤린 부분은 제외해준다)
		sprite_height = sprite_height - (-draw_start_y);

		//출력 시작 오프셋을 변경한다. (짤린 부분 이외부터 출력을 시작한다)
		src_buffer_offset = (DWORD*)(current_sprite->_sprite_buffer + current_sprite->_pitch * (-draw_start_y));

		//출력 시작점 재지정한다.
		draw_start_y = 0;
	}

	//하단 클리핑.
	//하단 에 대한 스프라이트 출력 위치 계산한다. 
	if (dest_height < draw_start_y + current_sprite->_height){

		//출력 시작점은 바뀌지 않고 출력 높이가 바뀐다.
		sprite_height -= ((draw_start_y + current_sprite->_height) - dest_height);
	}


	//좌측 클리핑.
	//왼쪽 출력 위치 계산한다. 
	if (0 >draw_start_x){

		sprite_width = sprite_width - (-draw_start_x);
		src_buffer_offset = src_buffer_offset + (-draw_start_x);


		//왼쪽이 조금 잘리므로 출력 시작 위치를 오른쪽으로 민다.
		draw_start_x = 0;
	}

	//우측 클리핑.
	//오른쪽 출력 위치 계산한다. 
	if (dest_width < draw_start_x + current_sprite->_width){

		sprite_width -= ((draw_start_x + current_sprite->_width) - dest_width);
	}


	//찍을 그림이 없는 경우를 체크한다.
	if (sprite_width <= 0 || sprite_height <= 0) {
	
		//그리지 않고 종료한다.
		return;
	}

	//화면에 찍을 위치로 이동한다.
	dest_buffer_offset = (DWORD *)(((BYTE*)(dest_buffer_offset + draw_start_x) + (draw_start_y * dest_pitch)));


	//전체 크기를 돌면서 픽셀마다 투명색 처리를 하며 그림 출력한다.
	for (int height = 0; sprite_height > height; height++){

		//복사한다.
		memcpy(dest_buffer_offset, src_buffer_offset, sprite_width * 4);

		//오프셋을 갱신한다.
		dest_buffer_offset = (DWORD *)((BYTE *)dest_buffer_offset + dest_pitch);
		src_buffer_offset = (DWORD *)((BYTE *)src_buffer_offset + current_sprite->_pitch);
	}
}
