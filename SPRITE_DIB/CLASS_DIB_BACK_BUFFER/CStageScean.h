
#pragma once
#include "stdafx.h"

#include "lib_CSPRITE_DIB.h"
#include "lib_CSCREEN_DIB.h"

#include "df_sprite_setting.h"
#include "CPlayerObject.h"
#include "CBaseScean.h"
#include "CFrameSkip.h"
#include "CSpriteNode.h"



//------------------------------------------------------------------------------
//	스테이지 클래스
//------------------------------------------------------------------------------
class CStageScean : public CBaseScean {

private:


	//테스트용 플레이 리스트.
	CPlayerObject *_arr_clien_player;


	list<CSpriteNode*>_list_csprite;


	CFrameSkip *_frame_skip;


protected:

	friend class CBaseScean;


	//게임 시작시 필요한 기본 초기화를 처리 한다.
	virtual void InitScean();

	//씬 구성에 필요한 스프라이트 목록을 읽어 저장한다.
	virtual void LoadContent();




public:



	CStageScean(HWND cur_hwnd);

	//FPS 출력한다.
	void DisplayFPSOnCaption();

	virtual ~CStageScean();

	virtual void Run();

	//씬 내 모든 요소를 업데이트 한다.
	virtual void UpdateGame();
};

