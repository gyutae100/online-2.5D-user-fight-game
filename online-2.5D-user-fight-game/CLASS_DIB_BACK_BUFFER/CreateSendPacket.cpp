
#include "stdafx.h"
#include "CreateSendPacket.h"
#include "CBaseScean.h"
#include "df_packet_header.h"
#include "CPacket.h"

extern CBaseScean *g_game_scean;
extern BOOL net_send_SendPacket(CPacket *packet);


void net_send_StopMoving(BYTE direction, WORD x, WORD y) {

	///@todo : ���߿� ���İ�Ƽ �ڵ� ���� �� ��.
	if (ACTION_PLAYER::ACTION_ATTACK1 <= g_game_scean->_arr_player[0]._pre_input_action_msg &&
		ACTION_PLAYER::ACTION_ATTACK3 >= g_game_scean->_arr_player[0]._pre_input_action_msg)
	{
		return;
	}


	//------------------------------------------------------------------------------------------
	//����ȭ ��� �̿�.
	//------------------------------------------------------------------------------------------
	CPacket packet;

	//��� ����ȭ ���ۿ� ����.
	packet << ((BYTE)0x89) << (BYTE)sizeof(PACKET_CS_MOVE_STOP) << (BYTE)dfPACKET_CS_MOVE_STOP << (BYTE)-1;
	//���̷ε� ����ȭ ���ۿ� ����.
	packet << (BYTE)direction << (WORD)x << (WORD)y;
	//���� �ڵ� ����.
	packet << ((BYTE)0x79);

	net_send_SendPacket(&packet);

}


void net_send_StartMoving(BYTE direction, WORD x, WORD y) {



	//------------------------------------------------------------------------------------------
	//����ȭ ��� �̿�.
	//------------------------------------------------------------------------------------------
	CPacket packet;

	//��� ����ȭ ���ۿ� ����.
	packet << ((BYTE)0x89) << (BYTE)sizeof(PACKET_CS_MOVE_START) << (BYTE)dfPACKET_CS_MOVE_START << (BYTE)-1;
	//���̷ε� ����ȭ ���ۿ� ����.
	packet << (BYTE)direction << (WORD)x << (WORD)y;
	//���� �ڵ� ����.
	packet << ((BYTE)0x79);

	net_send_SendPacket(&packet);
}


void net_send_Attack1(BYTE direction, WORD x, WORD y) {


	//------------------------------------------------------------------------------------------
	//����ȭ ��� �̿�.
	//------------------------------------------------------------------------------------------
	CPacket packet;

	//��� ����ȭ ���ۿ� ����.
	packet << ((BYTE)0x89) << (BYTE)sizeof(PACKET_CS_ATTACK1) << (BYTE)dfPACKET_CS_ATTACK1 << (BYTE)-1;
	//���̷ε� ����ȭ ���ۿ� ����.
	packet << (BYTE)direction << (WORD)x << (WORD)y;
	//���� �ڵ� ����
	packet << ((BYTE)0x79);

	net_send_SendPacket(&packet);
}


void net_send_Attack2(BYTE direction, WORD x, WORD y) {


	//------------------------------------------------------------------------------------------
	//����ȭ ��� �̿�.
	//------------------------------------------------------------------------------------------
	CPacket packet;

	//��� ����ȭ ���ۿ� ����.
	packet << ((BYTE)0x89) << (BYTE)sizeof(PACKET_CS_ATTACK2) << (BYTE)dfPACKET_CS_ATTACK2 << (BYTE)-1;
	//���̷ε� ����ȭ ���ۿ� ����.
	packet << (BYTE)direction << (WORD)x << (WORD)y;
	//���� �ڵ� ����
	packet << ((BYTE)0x79);

	net_send_SendPacket(&packet);
}


void net_send_Attack3(BYTE direction, WORD x, WORD y) {


	//------------------------------------------------------------------------------------------
	//����ȭ ��� �̿�.
	//------------------------------------------------------------------------------------------
	CPacket packet;

	//��� ����ȭ ���ۿ� ����.
	packet << ((BYTE)0x89) << (BYTE)sizeof(PACKET_CS_ATTACK3) << (BYTE)dfPACKET_CS_ATTACK3 << (BYTE)-1;
	//���̷ε� ����ȭ ���ۿ� ����.
	packet << (BYTE)direction << (WORD)x << (WORD)y;
	//���� �ڵ� ����.
	packet << ((BYTE)0x79);

	net_send_SendPacket(&packet);
}


