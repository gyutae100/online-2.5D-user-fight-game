///@mainpage    TCP FIGHETER 2.5D 액션 게임 클라이언트 제작.
///@section intro 소개
///- 부분1 : 스프라이트 DIB 관리자 및 백버퍼 DIB를 포함한 스프라이트 관리 및 출력 엔진이 존재한다.
///- 부분2 : 씬 내에서 스프라이트 엔진에 대한 모든 처리(로드, 스프라이트 부착, 플립 등)와 모든 게임 로직(키보드 입력, 패킷 처리, 액션 등)을 처리한다.
///- 부분3 : 게임 설정값은 df_sprite_setting.h 경로에 존재한다. 쉽게 수정이 가능하다.
///- 부분4 : 게임 플레이 캐릭터 중 자신의 캐릭만 키보드를 통해 입력을 받아 조작이 가능하다.
///- 부분5 : 그 외의 게임 플레이 캐릭터는 게임 서버에서 로직을 처리하여 패킷 전달을 통해 조작한다.
///- 부분6 : 링 버퍼를 통해 패킷을 관리한다.
///- 부분7 : 직렬화 버퍼를 이용한 패킷을 처리한다. 

///@section Program 프로그램명
///- 프로그램 명  :   TCP FIGHETER 2.5D 액션 게임 클라이언트 제작.
///- 프로그램 내용    : 링 버퍼 및 직렬화 버퍼를 이용한 네트워크 로직 개선 중.
///@section CREATEINFO 작성정보
///- 작성자      :   김규태(gyutae100@gmail.com) / 참고 : 프로카데미 학원 자료.
///- 작성일      :   2018/06/02
///@section MODIFYINFO 수정정보
///- 수정자/수정일   : 작성자와 동일 / 참고 : 프로카데미 학원 자료. 
///- 2018.06.02    : Doxygen 주석화 및 리펙토링.
///- 2018.06.14    : 리펙토링 재작업.
///- 2018.07.08    : 네트워크 로직 추가 이전의 마지막 리펙토링 재작업.
///- 2018.07.10    : 링 버퍼를 이용한 네트워크 로직 추가. ( 클라이언트로서 기능 가능.)
///- 2018.07.11    : 잔버그 몇 가지 수정....
///- 2018.07.17    : 네트워크 로직 내 직렬화 버퍼 추가.
///-@version 1.0


/**
@file main.cpp

@brief  게임 로직 루프 진행한다. 게임 씬 생성 및 지속적인 윈도우 메시지 처리 내에서 네트워크 로직에 의한 게임 씬 업데이트를 진행한다.
*/

#include "stdafx.h"
#include "main.h"

//---------------------------------------------------------------
//게임 로직 관련 헤더파일.
//---------------------------------------------------------------
#include "CStageScean.h"
#include "df_sprite_setting.h"

//---------------------------------------------------------------
//네트워크 관련 헤더파일.
//---------------------------------------------------------------
#include "CStreamSQ.h"
#include "df_packet_header.h"
#include "CPacket.h"
#include "CreateSendPacket.h"
#include "ProcRecvPacket.h"

//---------------------------------------------------------------
//게임 씬 관련 전역 변수 목록.
//---------------------------------------------------------------
static HWND g_hwnd;			///<게임 윈도우 핸들.
BOOL g_active_app;			///<현재 윈도우 어플리케이션 활성화 여부를 지속적으로 체크한다.
CBaseScean *g_game_scean;	///<게임 씬.

//---------------------------------------------------------------
//게임 씬 관련 함수 목록.
//---------------------------------------------------------------
void UpdateGame(void);	///<게임 업데이트 한다.

//---------------------------------------------------------------
//네트워크 주소 정보.
//---------------------------------------------------------------
#define SERVER_PORT 20000	
#define SERVER_IP L"127.0.0.1"

//---------------------------------------------------------------
//네트워크 관련 전역 변수 목록.
//---------------------------------------------------------------
SOCKET g_socket;					///<서버와 연결된 소켓. 			
BOOL g_is_connected_with_server;	///<서버와 연결 여부.

//---------------------------------------------------------------
//네트워크 링 버퍼 목록.
//---------------------------------------------------------------
CStreamSQ g_recv_ring_buffer;		///<리시브 전용 링버퍼.
CStreamSQ g_send_ring_buffer;		///<센드 전용 링버퍼.

//---------------------------------------------------------------
//네트워크 연결 및 해제 함수 목록.
//---------------------------------------------------------------
BOOL net_InitNetwork(SOCKET *out_socket, HWND in_hwnd);		//네트워크 초기화 및 서버에 연결.
///@todo : Disconnect 함수 생성 하자.

//---------------------------------------------------------------
//네트워크 이벤트 처리용 함수 목록.
//---------------------------------------------------------------
BOOL net_HandleNetworkEvent(WPARAM wParam, LPARAM lParam);  // 네트워크 이벤트에 따른 분기 처리를 한다.
/*RECV*/BOOL net_OnRecvEvent(void);
/*RECV*/void net_recv_BeginRecvProcByPacketType(BYTE type_packet, CPacket *packet);	//받은 패킷을 해석 후 해당 처리 함수를 호출한다.
/*SEND*/void net_PreSendEvent(CPlayerObject *in_player_object);
/*SEND*/BOOL net_OnSendEvent(void);
/*SEND*/BOOL net_send_SendPacket(CPacket *packet);



// 전역 변수:
HINSTANCE hInst;												 ///< 현재 인스턴스입니다.
WCHAR szTitle[SPRITE_SETTING::MAX_LOAD_SPRITE];                  ///< 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[SPRITE_SETTING::MAX_LOAD_SPRITE];            ///< 기본 창 클래스 이름입니다.

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.
	//콘솔창을 오픈한다.
	//AllocConsole();
	//freopen("CONOUT$", "wt", stdout);

	_wsetlocale(LC_ALL, L"korean");

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, SPRITE_SETTING::MAX_LOAD_SPRITE);
    LoadStringW(hInstance, IDC_CLASSDIBBACKBUFFER, szWindowClass, SPRITE_SETTING::MAX_LOAD_SPRITE);
    MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLASSDIBBACKBUFFER));

    MSG msg;


	//게임 처리를 위한 WinMain 함수의 메시지 루프.
	while (TRUE){

		//---------------------------------------------------------------
		// 기본 메시지 루프입니다.
		// PeekMessage 함수는 현재 윈도우에 입력된 메시지를 검색한다.
		// PeekMessage 함수는 메시지의 존재 유무를 리턴 값으로 반환한다.
		// PeekMessage 함수가 처리할 메시지가 없는 경우 아래 분기의 게임 처리 함수를 호출한다.
		//---------------------------------------------------------------
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (WM_QUIT == msg.message) {

				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		
		//---------------------------------------------------------------
		//게임 처리 함수 호출한다.
		//게임은 일정 시간마다 매번 화면을 업데이트 시켜줘야 한다.
		//---------------------------------------------------------------
		else {
				//---------------------------------------------------------------
				//게임을 업데이트 한다.
				//---------------------------------------------------------------	
				UpdateGame();
		}
	}

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLASSDIBBACKBUFFER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CLASSDIBBACKBUFFER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   //---------------------------------------------------------------------
   //윈도우 화면 생성한다.
   //---------------------------------------------------------------------
   HWND hWnd = CreateWindowW(szWindowClass, 
	   szTitle, 
	   WS_SYSMENU |WS_CAPTION |WS_MINIMIZEBOX,
       CW_USEDEFAULT,
	   CW_USEDEFAULT,
	   SCREEN_RESOLUTION::SCREEN_WIDTH, 
	   SCREEN_RESOLUTION::SCREEN_HEIGHT,
	   nullptr, 
	   nullptr, 
	   hInstance, 
	   nullptr);


   //전역 헨들 저장
   g_hwnd = hWnd;
   g_game_scean = new CBaseScean(hWnd);

   //---------------------------------------------------------------
   //서버와의 연결을 초기화한다.
   //---------------------------------------------------------------
   if (!net_InitNetwork(&g_socket, g_hwnd)) {

	   puts("서버와의 연결 실패");
	   g_is_connected_with_server = false;
	   return FALSE;
   }

   puts("서버와의 연결 성공");

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){

    switch (message){
	
	//네트워크 로직 파트.
	case UM_NETWORK:

		net_HandleNetworkEvent(wParam, lParam);
		break;


	case WM_CREATE:

		//---------------------------------------------------------------
		///@todo : 
		//한글 입력 IME 창 없애기 (영문모드 강제 전환)
		//안된다 나중에 찾자
		//ImmAssociateContext(hWnd, NULL);
		//---------------------------------------------------------------
		break;

	//어플리케이션 활성화 여부 체크.
	case WM_ACTIVATEAPP: {

		g_active_app = (BOOL)wParam; //wParama에 true가 들어오면 어플리케이션 활성화 상태. false시 비활성화 상태.
		break;
	}

    case WM_COMMAND: {

            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다.

            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다.
			
			//Update();

            EndPaint(hWnd, &ps);
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

/**
	brief 게임 업데이트한다.
*/
void UpdateGame(void){

	if (TRUE == g_is_connected_with_server) {
	
		g_game_scean->UpdateGame();

		//---------------------------------------------------------------
		//	여기서 패킷을 서버에 보낼 여부를 체크한다.
		//	센드 패킷을 보낼 상황인 경우 센드용 링버퍼에 해당 패킷을 저장한다.
		//---------------------------------------------------------------
		if (TRUE == g_game_scean->CheckDifferentInputKeyBefore()) {

			CPlayerObject this_object;
			g_game_scean->GetCurInputMsgByObject(&this_object);

			//센드용 링버퍼에 해당 패킷을 저장한다.
			net_PreSendEvent(&this_object);
		}
	}
}



// 네트워크 이벤트에 따른 분기 처리를 한다.
BOOL net_HandleNetworkEvent(WPARAM wParam, LPARAM lParam) {

	//---------------------------------------------------------------
	// 에러 발생시 에러처리.
	// 소켓 이벤트 확인 전에 에러처리를 먼저 해줘야만 한다.
	//---------------------------------------------------------------
	if (0 != WSAGETSELECTERROR(lParam)) {

		return FALSE;
	}

	int retval = -1;

	//---------------------------------------------------------------------------------------------
	// 네트워크 이벤트 메시지 처리 분기문.
	//---------------------------------------------------------------------------------------------
	switch (WSAGETSELECTEVENT(lParam)) {

	//---------------------------------------------------------------------------------------------
	//	가능한 수신 버퍼에 있는 데이터를 링버퍼에 저장 후 가능한 많은 Recv 패킷에 대한 처리를 반복 진행한다. 
	//---------------------------------------------------------------------------------------------
	case FD_READ:

		net_OnRecvEvent();
		return true;


	//---------------------------------------------------------------------------------------------
	//	상대방 수신 버퍼에 여유 공간이 생긴 경우 send를 하여 수신용 링버퍼에 존재하는 패킷 단위로 최대한 전송한다.
	//---------------------------------------------------------------------------------------------
	case FD_WRITE:

		net_OnSendEvent();
		return true;

	case FD_CONNECT:

		g_is_connected_with_server = true;
		return true;

	//---------------------------------------------------------------------------------------------
	//	클라이언트 종료 위해 종료 메시지를 보낸다.
	//---------------------------------------------------------------------------------------------
	case FD_CLOSE: {

		if (TRUE == g_is_connected_with_server) {

			PostQuitMessage(0);
		}
		return false;
	}
	}
	return true;
}



//네트워크 초기화 및 서버에 연결.
BOOL net_InitNetwork(SOCKET *out_socket, HWND in_hwnd) {

	WSADATA wsa;
	SOCKADDR_IN serveraddr;

	//---------------------------------------------------------------------------------------------
	// 윈도우 소켓 시작 설정 및 TCP 소켓 생성.
	//---------------------------------------------------------------------------------------------
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 0;

	*out_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (*out_socket == INVALID_SOCKET)
	{
		return 0;
	}

	//---------------------------------------------------------------------------------------------
	// WSAAsync 모델로 변환.
	//---------------------------------------------------------------------------------------------
	WSAAsyncSelect(*out_socket, in_hwnd, UM_NETWORK, FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE);

	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	InetPton(AF_INET, SERVER_IP, &serveraddr.sin_addr);
	serveraddr.sin_port = htons(SERVER_PORT);


	//---------------------------------------------------------------------------------------------
	// 네이글 알고리즘 중지.
	// :빠른 응답 속도를 위해 버퍼링 기능을 끈다.
	// :단점으로 통신량이 증가하지만 TCP Fighter게임 특징상 빠른 반응속도가 더 우선된다.
	//---------------------------------------------------------------------------------------------
	bool flag_true = false;
	setsockopt(*out_socket, IPPROTO_TCP, TCP_NODELAY, (char *)&flag_true, sizeof(bool));

	//---------------------------------------------------------------------------------------------
	//서버에 연결요청.
	//---------------------------------------------------------------------------------------------
	int code_err;
	code_err = connect(*out_socket, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (code_err == SOCKET_ERROR) {

		//에러 발생시 프로그램 종료.
		if (WSAGetLastError() != WSAEWOULDBLOCK) {

			return FALSE;
		}
	}

	return TRUE;
}

BOOL net_OnRecvEvent(void) {


	int retval = -1;

	//---------------------------------------------------------------------------------------------
	// 수신 버퍼의 데이터를 최대 리시브용 링 버퍼의 빈공간 만큼 RECV한다.
	//---------------------------------------------------------------------------------------------
	int sz_byte_free_recv_ring_buffer = g_recv_ring_buffer.GetSizeOfFree();
	retval = recv(g_socket, g_recv_ring_buffer.GetRearPtrOfBuffer(), sz_byte_free_recv_ring_buffer, NULL);
	
	//---------------------------------------------------------------------------------------------
	// Fin 패킷을 받은 경우.
	// 연결을 끊고 프로그램을 종료시킨다.
	//---------------------------------------------------------------------------------------------
	if (0 == retval) {

		//종료한다.
		puts("Fin 패킷 수신");
		///@todo : Disconnect를 넣어준다.
		//exit(1);
	}

	// 종료한다.
	if (SOCKET_ERROR == retval) {

		puts("OnRecvEvent recv 에러");
		//exit(1);
	}

	// 혹시나 싶은 마음에.... 예외처리
	// 수신 버퍼에서 리시브한 사이즈가 리시브 링버퍼 남은 용량 크기보다 큰 경우.
	if (retval > sz_byte_free_recv_ring_buffer) {

		//종료한다.
		//exit(1);
	}
	//---------------------------------------------------------------------------------------------
	// 수신 버퍼의 데이터가 정상적으로 링버퍼에 옮겨졌다.
	// 수신 버퍼의 Rear Index를 증가시킨다.
	//---------------------------------------------------------------------------------------------
	g_recv_ring_buffer.MoveRearIdx(retval);

	//---------------------------------------------------------------------------------------------
	// 리시브 링 버퍼에 존재하는 패킷을 처리하는 부분.
	// 패킷이 완료 패킷인 경우만 처리하고 그 외 경우 링 버퍼에 그대로 두고 다음 RECV 시 완성 패킷이 되게 만든다.
	//---------------------------------------------------------------------------------------------
	for ( ; ; ) {

		//---------------------------------------------------------------------------------------------
		// 1. 리시브 링버퍼에 최소 헤더 사이즈만큼 데이터가 존재하는지 체크한다. 
		//---------------------------------------------------------------------------------------------
		int retval = -1;
		retval = g_recv_ring_buffer.GetSizeOfUse(); 

		//---------------------------------------------------------------------------------------------
		// 헤더 사이즈보다 링 버퍼 데이터 크기가 적은 경우는 다음 Recv 이후를 기약한다.
		//---------------------------------------------------------------------------------------------
		if (retval < sizeof(NETWORK_PACKET_HEADER)) {

			return false;
		}

		//---------------------------------------------------------------------------------------------
		// 초반에 Peek하는 이유 : 리시브 링버퍼에서 데이터를 꺼낼 때 패킷 단위로 꺼내야 하기 때문이다.
		//---------------------------------------------------------------------------------------------
		
		//---------------------------------------------------------------------------------------------
		// 2. 리시브 링버퍼에서 헤더 사이즈 만큼의 데이터를 Peek한다.
		//---------------------------------------------------------------------------------------------
		NETWORK_PACKET_HEADER cur_header;
		retval = g_recv_ring_buffer.Peek((char *)&cur_header, sizeof(NETWORK_PACKET_HEADER));

		//---------------------------------------------------------------------------------------------
		// 3. 헤더 code 확인한다.
		//---------------------------------------------------------------------------------------------
		if (cur_header._code != NETWORK_PACKET_CODE) {

			return false;
		}

		//---------------------------------------------------------------------------------------------
		//4. 패킷 하나 단위의 크기만큼 리시브 링버퍼에 데이터 크기가 존재하는지 체크한다.
		// 패킷 하나 단위 크기는 헤더 크기(4Byte) + 페이로드 크기(Random) + 엔드코드(1Byte)이다.
		//---------------------------------------------------------------------------------------------
		if ( (cur_header._sz_byte_payload + sizeof(NETWORK_PACKET_HEADER) + sizeof(NETWORK_PACKET_END) ) > g_recv_ring_buffer.GetSizeOfUse() ){

			return false;
		}

		//---------------------------------------------------------------------------------------------
		// 5. 리시브 링버퍼에서 패킷 단위 삭제가 가능함이 증명되었다. 
		//---------------------------------------------------------------------------------------------
		g_recv_ring_buffer.MoveFrontIdx(sizeof(NETWORK_PACKET_HEADER));
		
		//---------------------------------------------------------------------------------------------
		// 6. 리시브 링버퍼에서 페이로드 단위 크기 직렬화 버퍼 패킷에 복사한다.
		//---------------------------------------------------------------------------------------------
		CPacket packet;
		retval =g_recv_ring_buffer.Get(packet.GetBufferPtr(), cur_header._sz_byte_payload);
		packet.MoveIdxRear(retval);
		
		// 혹시나 싶은 마음에.... 예외처리
		if (retval != packet.GetDataSize()) {

			//에러 발생.
		}
		//---------------------------------------------------------------------------------------------
		// 7. END CODE 체크한다.
		//---------------------------------------------------------------------------------------------
		BYTE end_code;
		retval= g_recv_ring_buffer.Get((char*)&end_code, sizeof(NETWORK_PACKET_END));

		//---------------------------------------------------------------------------------------------
		// 8. 엔드코드 확인.
		//---------------------------------------------------------------------------------------------
		if (end_code != NETWORK_PACKET_END) {

			// 종료해야 하는가?? 로그를 남겨야 하는가?? 
			// 클라에서는 어떻게 대처를 해야 하는가??
			puts("end_code 불일치 에러");
			//exit(1);
		}
		
		//---------------------------------------------------------------------------------------------
		// 9. 헤더의 타입에 따른 분기를 통한 패킷을 인자로 하는 프로시저를 호출한다.
		//---------------------------------------------------------------------------------------------
		net_recv_BeginRecvProcByPacketType(cur_header._type_packet, &packet);
	}

	return true;

}


void net_recv_BeginRecvProcByPacketType(BYTE type_packet, CPacket *in_packet ) {

	
	switch (type_packet) {
	
	//---------------------------------------------------------------------------------------------
	//내 캐릭터 생성.
	//---------------------------------------------------------------------------------------------
	case dfPACKET_SC_CREATE_MY_CHARACTER:
		net_recv_CreateMyCharacter(in_packet);
		break;

	//---------------------------------------------------------------------------------------------
	//다른 캐릭터 생성.
	//---------------------------------------------------------------------------------------------
	case dfPACKET_SC_CREATE_OTHER_CHARACTER:
		net_recv_CreateOtherCharacter(in_packet);
		break;

	//---------------------------------------------------------------------------------------------
	//캐릭터 삭제.
	//---------------------------------------------------------------------------------------------
	case dfPACKET_SC_DELETE_CHARACTER:
		net_recv_DeleteWitchCharacter(in_packet);
		break;

	//---------------------------------------------------------------------------------------------	
	//캐릭터 공격1. 
	//---------------------------------------------------------------------------------------------
	case dfPACKET_SC_ATTACK1:
		net_recv_Attack1(in_packet);
		break;

	//---------------------------------------------------------------------------------------------
	//캐릭터 공격 2.
	//---------------------------------------------------------------------------------------------
	case dfPACKET_SC_ATTACK2:
		net_recv_Attack2(in_packet);
		break;

	//---------------------------------------------------------------------------------------------
	//캐릭터 공격3.
	//---------------------------------------------------------------------------------------------
	case dfPACKET_SC_ATTACK3:
		net_recv_Attack3(in_packet);
		break;

	//---------------------------------------------------------------------------------------------
	//캐릭터 이동.
	//---------------------------------------------------------------------------------------------
	case dfPACKET_SC_MOVE_START:
		net_recv_StartMoving(in_packet);
		break;

	//---------------------------------------------------------------------------------------------
	//캐릭터 멈춤.
	//---------------------------------------------------------------------------------------------
	case dfPACKET_SC_MOVE_STOP:
		net_recv_StopMoving(in_packet);
		break;

	//---------------------------------------------------------------------------------------------
	//데미지 받음.
	//---------------------------------------------------------------------------------------------
	case dfPACKET_SC_DAMAGE:
		net_recv_Damage(in_packet);
		break;
	}
}



void net_recv_CreateMyCharacter(CPacket *in_packet) {


	DWORD	id;
	BYTE	direction;
	WORD	x;
	WORD	y;
	BYTE	hp;

	//---------------------------------------------------------------
	//	직렬화 버퍼를 이용한 패킷 추출.
	//---------------------------------------------------------------
	*in_packet >> id >> direction >>x >> y >> hp;

	//사용 중이지 않은 플레이어 인덱스를 검색한다.
	int idx_arr = g_game_scean->FindFreePlayerIdxInList();
	if (idx_arr == -1) {

		puts("더이상 할당 할 플레이어 인덱스가 존재하지 않습니다. ");
	}

	//---------------------------------------------------------------
	//	플레이어를 등록한다.
	//---------------------------------------------------------------
	g_game_scean->InitNewClient(idx_arr, id, hp, x, y, direction, L"당신");
}

BOOL net_send_SendPacket(CPacket *packet) {

	//---------------------------------------------------------------
	// 직렬화 버퍼에 존재하는 패킷들(최소 1개)을 센드 링버퍼에 삽입한다.
	//---------------------------------------------------------------
	g_send_ring_buffer.Put((char*)packet->GetBufferPtr(), packet->GetDataSize());

	//---------------------------------------------------------------
	// 센드용 링버퍼에 존재하는 패킷들을(최소 1개) Send 시도한다.
	//---------------------------------------------------------------
	if (TRUE == net_OnSendEvent()){
		
		//비우자!
		packet->Clear();
	}

	return TRUE;
}



BOOL net_OnSendEvent(void) {

	for (; ;) {

		if (TRUE ==g_send_ring_buffer.IsEmpty()) {

			return false;
		}

		// 센드용 링버퍼에서 한번에 복사할 수 있는 최대 사이즈를 구한다.
		int sz_byte_once_send = g_send_ring_buffer.GetUnBrokenGetSizeOfBuffer();

		// 센드용 링버퍼에 존재하는 최대 복사 가능 사이즈를 구한다.
		int total_sz_byte_send = g_send_ring_buffer.GetSizeOfUse();

		//---------------------------------------------------------------------
		// 센드용 링버퍼에서 데이터 send시 두 번 send해야 하는 경우를 체크한다.
		// 위 두 변수 값이 다르다면 두 번 send를 시도해야 할 확률이 존재한다.
		//---------------------------------------------------------------------
		if (total_sz_byte_send > sz_byte_once_send) {

			//---------------------------------------------------------------------
			// 첫 번째 센드 시도.
			//---------------------------------------------------------------------
			int retval = send(g_socket, g_send_ring_buffer.GetFrontPtrOfBuffer(), sz_byte_once_send, NULL);

			//제대로 센드 된 여부를 체크한다.
			if (sz_byte_once_send == retval) {

				g_send_ring_buffer.MoveFrontIdx(sz_byte_once_send);
			}

			//---------------------------------------------------------------------
			// 상대방 수신 버퍼 문제로 센드 실패!
			// 다음을 기약하자.
			//---------------------------------------------------------------------
			if (WSAEWOULDBLOCK == retval) {


				return false;
			}

			//---------------------------------------------------------------------
			// 두 번째 센드 시도.
			//---------------------------------------------------------------------
			sz_byte_once_send = g_send_ring_buffer.GetUnBrokenGetSizeOfBuffer();
			retval = send(g_socket, g_send_ring_buffer.GetFrontPtrOfBuffer(), sz_byte_once_send, NULL);

			//---------------------------------------------------------------------
			//정상적으로 보낸 경우.
			//---------------------------------------------------------------------
			if (sz_byte_once_send == retval) {

				g_send_ring_buffer.MoveFrontIdx(sz_byte_once_send);
			}
			//---------------------------------------------------------------------
			// 상대방 수신 버퍼 문제로 센드 실패!
			// 다음을 기약하자.
			//---------------------------------------------------------------------
			if (WSAEWOULDBLOCK == retval) {

				return false;
			}

		}
		//---------------------------------------------------------------------
		// 한 번 send로 모든 패킷을 서버측에 보낼 가능성이 있는 경우.
		//---------------------------------------------------------------------
		else {

			//---------------------------------------------------------------------
			// send 시도
			//---------------------------------------------------------------------
			int retval = send(g_socket, g_send_ring_buffer.GetFrontPtrOfBuffer(), total_sz_byte_send, NULL);

			//---------------------------------------------------------------------
			// 정상적으로 보낸 경우.
			//---------------------------------------------------------------------
			if (total_sz_byte_send == retval) {

				g_send_ring_buffer.MoveFrontIdx(total_sz_byte_send);
			}

			//---------------------------------------------------------------------
			// 보내기에 실패한 경우.
			// 다음을 기약한다.
			//---------------------------------------------------------------------
			if (WSAEWOULDBLOCK == retval) {

				return false;
			}
		}
	}
	return true;
}


//현재 플레이어 액션 메시지에 맞게 헤더와 페이로드를 생성한다.
void net_PreSendEvent(CPlayerObject *in_player_object) {

	//---------------------------------------------------------------------
	// 현재 플레이어 액션 메시지 및 현재 플레이어 객체의 상태를 저장한다.
	//---------------------------------------------------------------------
	ACTION_PLAYER cur_input_msg = in_player_object->_cur_input_action_msg;
	BOOL _is_stop_move = in_player_object->_is_stop_moving;
	BOOL _is_start_attack = in_player_object->_is_start_attack;
	BOOL _is_attacking = in_player_object->_is_attacking;

	int x;
	int y;
	in_player_object->GetCurXY(&x, &y);

	DIRECTION direction;
	direction= in_player_object->_last_direction;

	//---------------------------------------------------------------------
	// 공격을 시작한 경우.
	//---------------------------------------------------------------------
	if (TRUE == _is_start_attack) {

		switch (cur_input_msg) {

		//---------------------------------------------------------------------
		// 공격1.
		//---------------------------------------------------------------------
		case 	ACTION_ATTACK1:

			net_send_Attack1(direction, x, y);
			return;

		//---------------------------------------------------------------------
		// 공격2.
		//---------------------------------------------------------------------
		case	ACTION_ATTACK2:

			net_send_Attack2(direction, x, y);
			return;

		//---------------------------------------------------------------------
		// 공격3.
		//---------------------------------------------------------------------
		case	ACTION_ATTACK3:

			net_send_Attack3(direction, x, y);
			return;
		}
	}

	//---------------------------------------------------------------------
	// 움직임이 멈춘 경우.
	//---------------------------------------------------------------------
	if (TRUE == _is_stop_move) {

		net_send_StopMoving(direction, x, y);
		return;
	}

	//---------------------------------------------------------------------
	// 움직임.
	//---------------------------------------------------------------------
	switch (cur_input_msg) {

	case 	ACTION_MOVE_LL :
	case	ACTION_MOVE_LU :
	case	ACTION_MOVE_UU :
	case	ACTION_MOVE_RU :
	case	ACTION_MOVE_RR : 
	case	ACTION_MOVE_RD : 
	case	ACTION_MOVE_DD :
	case	ACTION_MOVE_LD :
		
		if (FALSE == _is_attacking) {

			net_send_StartMoving(cur_input_msg, x, y);
			return;
		}
	}
}


