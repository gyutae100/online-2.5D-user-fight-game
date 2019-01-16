
#include "stdafx.h"


//------------------------------------------------------------------------------------------
//	send 패킷 생성 함수 목록.
//------------------------------------------------------------------------------------------
void net_send_StopMoving(BYTE direction, WORD x, WORD y);
void net_send_StartMoving(BYTE direction, WORD x, WORD y);
void net_send_Attack1(BYTE direction, WORD x, WORD y);
void net_send_Attack2(BYTE direction, WORD x, WORD y);
void net_send_Attack3(BYTE direction, WORD x, WORD y);
