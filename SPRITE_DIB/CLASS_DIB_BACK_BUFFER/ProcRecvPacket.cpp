#include "stdafx.h"

#include "ProcRecvPacket.h"
#include "CBaseScean.h"
#include "CPacket.h"
#include "df_packet_header.h"


extern CBaseScean *g_game_scean;

void net_recv_Attack1(CPacket *in_packet) {

	DWORD id;
	BYTE direction;
	WORD x;
	WORD y;

	*in_packet >> id >> direction >> x >> y;

	g_game_scean->StartAttack1(id, ACTION_PLAYER::ACTION_ATTACK1, direction, x, y);
}


void net_recv_Attack2(CPacket *in_packet) {

	DWORD id;
	BYTE direction;
	WORD x;
	WORD y;

	*in_packet >> id >> direction >> x >> y;

	g_game_scean->StartAttack2(id, ACTION_PLAYER::ACTION_ATTACK2, direction, x, y);
}


void net_recv_Attack3(CPacket *in_packet) {

	DWORD id;
	BYTE direction;
	WORD x;
	WORD y;

	*in_packet >> id >> direction >> x >> y;

	g_game_scean->StartAttack3(id, ACTION_PLAYER::ACTION_ATTACK3, direction, x, y);
}


void net_recv_StartMoving(CPacket *in_packet) {

	DWORD	id;
	BYTE direction;
	WORD x;
	WORD y;

	*in_packet >> id >> direction >> x >> y;

	g_game_scean->StartMove(id, static_cast<ACTION_PLAYER>(direction), x, y);
}


void net_recv_StopMoving(CPacket *in_packet) {

	DWORD	id;
	BYTE direction;
	WORD x;
	WORD y;

	*in_packet >> id >> direction >> x >> y;


	g_game_scean->StopMove(id, static_cast<DIRECTION>(direction), x, y);
}


void net_recv_Damage(CPacket *in_packet) {


	DWORD attack_id;
	DWORD damage_id;
	BYTE hp;

	*in_packet >> attack_id >> damage_id >> hp;

	// 데미지 차감 진행한다.
	g_game_scean->Damage(damage_id, hp);

	// 이펙트 패킷 생성한다.
	g_game_scean->MakeEffect(attack_id);
}



void net_recv_DeleteWitchCharacter(CPacket *in_packet) {

	DWORD	id;

	//---------------------------------------------------------------
	//	직렬화 버퍼를 이용한 패킷 추출.
	//---------------------------------------------------------------
	*in_packet >> id;

	//---------------------------------------------------------------
	//	해당 플레이어를 삭제한다.
	//---------------------------------------------------------------
	g_game_scean->DeleteClient(id);
}



void net_recv_CreateOtherCharacter(CPacket *in_packet) {

	PACKET_SC_CREATE_OTHER_CHARACTER *packet = (PACKET_SC_CREATE_OTHER_CHARACTER*)in_packet;

	DWORD	id;
	BYTE	direction;
	WORD	x;
	WORD	y;
	BYTE	hp;

	//---------------------------------------------------------------
	//	직렬화 버퍼를 이용한 패킷 추출.
	//---------------------------------------------------------------
	*in_packet >> id >> direction >> x >> y >> hp;

	//사용 중이지 않은 플레이어 인덱스를 검색한다.
	int idx_arr = g_game_scean->FindFreePlayerIdxInList();


	//---------------------------------------------------------------
	//	플레이어 닉네임을 패킷으로 받을 수 없으므로 랜덤으로 설정한다.
	//  공공의 적이 닉네임 대상이다..... 부들부들
	//---------------------------------------------------------------
	static int idx_nickname = -1;
	WCHAR arr_nickname[10][10] = {
	{ L"치킨" },
	{ L"야식" },
	{ L"게임" },
	{ L"예능" },
	{ L"웹툰" },
	{ L"공부" },
	{ L"덕질" },
	{ L"피자" },
	{ L"알바" },
	{ L"과제" },

	};
	idx_nickname++;
	idx_nickname = idx_nickname % 10;

	//---------------------------------------------------------------
	//	플레이어를 등록한다.
	//---------------------------------------------------------------
	g_game_scean->InitNewClient(idx_arr, id, hp, x, y, direction, arr_nickname[idx_nickname]);
}