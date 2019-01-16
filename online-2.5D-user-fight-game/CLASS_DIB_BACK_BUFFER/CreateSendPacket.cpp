
#include "stdafx.h"
#include "CreateSendPacket.h"
#include "CBaseScean.h"
#include "df_packet_header.h"
#include "CPacket.h"

extern CBaseScean *g_game_scean;
extern BOOL net_send_SendPacket(CPacket *packet);


void net_send_StopMoving(BYTE direction, WORD x, WORD y) {

	///@todo : 나중에 스파게티 코드 수정 할 것.
	if (ACTION_PLAYER::ACTION_ATTACK1 <= g_game_scean->_arr_player[0]._pre_input_action_msg &&
		ACTION_PLAYER::ACTION_ATTACK3 >= g_game_scean->_arr_player[0]._pre_input_action_msg)
	{
		return;
	}


	//------------------------------------------------------------------------------------------
	//직렬화 방식 이용.
	//------------------------------------------------------------------------------------------
	CPacket packet;

	//헤더 직렬화 버퍼에 삽입.
	packet << ((BYTE)0x89) << (BYTE)sizeof(PACKET_CS_MOVE_STOP) << (BYTE)dfPACKET_CS_MOVE_STOP << (BYTE)-1;
	//페이로드 직렬화 버퍼에 삽입.
	packet << (BYTE)direction << (WORD)x << (WORD)y;
	//엔드 코드 삽입.
	packet << ((BYTE)0x79);

	net_send_SendPacket(&packet);

}


void net_send_StartMoving(BYTE direction, WORD x, WORD y) {



	//------------------------------------------------------------------------------------------
	//직렬화 방식 이용.
	//------------------------------------------------------------------------------------------
	CPacket packet;

	//헤더 직렬화 버퍼에 삽입.
	packet << ((BYTE)0x89) << (BYTE)sizeof(PACKET_CS_MOVE_START) << (BYTE)dfPACKET_CS_MOVE_START << (BYTE)-1;
	//페이로드 직렬화 버퍼에 삽입.
	packet << (BYTE)direction << (WORD)x << (WORD)y;
	//엔드 코드 삽입.
	packet << ((BYTE)0x79);

	net_send_SendPacket(&packet);
}


void net_send_Attack1(BYTE direction, WORD x, WORD y) {


	//------------------------------------------------------------------------------------------
	//직렬화 방식 이용.
	//------------------------------------------------------------------------------------------
	CPacket packet;

	//헤더 직렬화 버퍼에 삽입.
	packet << ((BYTE)0x89) << (BYTE)sizeof(PACKET_CS_ATTACK1) << (BYTE)dfPACKET_CS_ATTACK1 << (BYTE)-1;
	//페이로드 직렬화 버퍼에 삽입.
	packet << (BYTE)direction << (WORD)x << (WORD)y;
	//엔드 코드 삽입
	packet << ((BYTE)0x79);

	net_send_SendPacket(&packet);
}


void net_send_Attack2(BYTE direction, WORD x, WORD y) {


	//------------------------------------------------------------------------------------------
	//직렬화 방식 이용.
	//------------------------------------------------------------------------------------------
	CPacket packet;

	//헤더 직렬화 버퍼에 삽입.
	packet << ((BYTE)0x89) << (BYTE)sizeof(PACKET_CS_ATTACK2) << (BYTE)dfPACKET_CS_ATTACK2 << (BYTE)-1;
	//페이로드 직렬화 버퍼에 삽입.
	packet << (BYTE)direction << (WORD)x << (WORD)y;
	//엔드 코드 삽입
	packet << ((BYTE)0x79);

	net_send_SendPacket(&packet);
}


void net_send_Attack3(BYTE direction, WORD x, WORD y) {


	//------------------------------------------------------------------------------------------
	//직렬화 방식 이용.
	//------------------------------------------------------------------------------------------
	CPacket packet;

	//헤더 직렬화 버퍼에 삽입.
	packet << ((BYTE)0x89) << (BYTE)sizeof(PACKET_CS_ATTACK3) << (BYTE)dfPACKET_CS_ATTACK3 << (BYTE)-1;
	//페이로드 직렬화 버퍼에 삽입.
	packet << (BYTE)direction << (WORD)x << (WORD)y;
	//엔드 코드 삽입.
	packet << ((BYTE)0x79);

	net_send_SendPacket(&packet);
}


