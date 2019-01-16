#include "stdafx.h"
#include "CPacket.h"

//--------------------------------------------------------------------------
//recv 패킷 처리 함수 목록.
//--------------------------------------------------------------------------
void net_recv_CreateMyCharacter(CPacket *packet);
void net_recv_CreateOtherCharacter(CPacket *packet);
void net_recv_DeleteWitchCharacter(CPacket *packet);
void net_recv_Attack1(CPacket *in_packet);
void net_recv_Attack2(CPacket *in_packet);
void net_recv_Attack3(CPacket *in_packet);
void net_recv_StartMoving(CPacket *in_packet);
void net_recv_StopMoving(CPacket *in_packet);
void net_recv_Damage(CPacket *in_packet);


