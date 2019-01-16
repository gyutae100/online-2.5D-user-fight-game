
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
//	�������� Ŭ����
//------------------------------------------------------------------------------
class CStageScean : public CBaseScean {

private:


	//�׽�Ʈ�� �÷��� ����Ʈ.
	CPlayerObject *_arr_clien_player;


	list<CSpriteNode*>_list_csprite;


	CFrameSkip *_frame_skip;


protected:

	friend class CBaseScean;


	//���� ���۽� �ʿ��� �⺻ �ʱ�ȭ�� ó�� �Ѵ�.
	virtual void InitScean();

	//�� ������ �ʿ��� ��������Ʈ ����� �о� �����Ѵ�.
	virtual void LoadContent();




public:



	CStageScean(HWND cur_hwnd);

	//FPS ����Ѵ�.
	void DisplayFPSOnCaption();

	virtual ~CStageScean();

	virtual void Run();

	//�� �� ��� ��Ҹ� ������Ʈ �Ѵ�.
	virtual void UpdateGame();
};

