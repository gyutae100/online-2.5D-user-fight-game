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

	// ������ ���� �����Ѵ�.
	g_game_scean->Damage(damage_id, hp);

	// ����Ʈ ��Ŷ �����Ѵ�.
	g_game_scean->MakeEffect(attack_id);
}



void net_recv_DeleteWitchCharacter(CPacket *in_packet) {

	DWORD	id;

	//---------------------------------------------------------------
	//	����ȭ ���۸� �̿��� ��Ŷ ����.
	//---------------------------------------------------------------
	*in_packet >> id;

	//---------------------------------------------------------------
	//	�ش� �÷��̾ �����Ѵ�.
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
	//	����ȭ ���۸� �̿��� ��Ŷ ����.
	//---------------------------------------------------------------
	*in_packet >> id >> direction >> x >> y >> hp;

	//��� ������ ���� �÷��̾� �ε����� �˻��Ѵ�.
	int idx_arr = g_game_scean->FindFreePlayerIdxInList();


	//---------------------------------------------------------------
	//	�÷��̾� �г����� ��Ŷ���� ���� �� �����Ƿ� �������� �����Ѵ�.
	//  ������ ���� �г��� ����̴�..... �ε�ε�
	//---------------------------------------------------------------
	static int idx_nickname = -1;
	WCHAR arr_nickname[10][10] = {
	{ L"ġŲ" },
	{ L"�߽�" },
	{ L"����" },
	{ L"����" },
	{ L"����" },
	{ L"����" },
	{ L"����" },
	{ L"����" },
	{ L"�˹�" },
	{ L"����" },

	};
	idx_nickname++;
	idx_nickname = idx_nickname % 10;

	//---------------------------------------------------------------
	//	�÷��̾ ����Ѵ�.
	//---------------------------------------------------------------
	g_game_scean->InitNewClient(idx_arr, id, hp, x, y, direction, arr_nickname[idx_nickname]);
}