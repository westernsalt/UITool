// SceneAssetPreviewWnd.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "SceneAssetPreview.h"
#include "ObjMgr.h"
#include "DBMgr.h"
#include "BasicAsset.h"
#include "UIFrame.h" 


IMPLEMENT_DYNCREATE(CSceneAssetPreview, CWnd)
CSceneAssetPreview::CSceneAssetPreview()
{
	CopyAsset = NULL;
	CopyFrame = NULL;
	RECT_INITIALIZE(m_tClientRt);
}

CSceneAssetPreview::~CSceneAssetPreview()
{
	
}

//선택한 에셋 보여주기
void CSceneAssetPreview::SetAssetPreview(int nFocusIndex)
{
	DeleteAsset();
		
	CBasicAsset* FocusAsset =  CDBMgr::GetInstance()->GetBasicAsset(nFocusIndex, SCENE_EDITOR_TARGET);
	if(FocusAsset != NULL)
	{
		CopyAsset = new CBasicAsset(FocusAsset);
		CopyAsset->Run();
	}	
}

//선택한 프레임 보여주기
void CSceneAssetPreview::SetFramePreview(int nFocusIndex)
{
	DeleteAsset();

	CUIFrame* FocusFrame = CDBMgr::GetInstance()->GetUIFrame(nFocusIndex);	
	if(FocusFrame != NULL)
	{
		POINT initPos;
		initPos.x = 0;
		initPos.y = 0;		
		CopyFrame = new CUIFrame(FocusFrame);
		CopyFrame->SetPosition(initPos);
		CopyFrame->Run();		
	}	
}

//복사한 에셋, 프레임 초기화
void CSceneAssetPreview::DeleteAsset()
{
	if(CopyAsset != NULL)
		SAFE_DELETE(CopyAsset);

	if(CopyFrame != NULL)
		SAFE_DELETE(CopyFrame);	
}

BEGIN_MESSAGE_MAP(CSceneAssetPreview, CWnd)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

//실시간 렌더링하기위해서 세팅
void CSceneAssetPreview::OnTimer(UINT_PTR nIDEvent)
{
	CWnd::OnTimer(nIDEvent);

	if(nIDEvent == DRAW_ASSET_PREVIEW_TIMER)
	{
		HDC			hMemDC;
		HBITMAP		hMembit;
		HBITMAP		hOldMembit;
		CDC* pDC = this->GetDC();
		GetClientRect(&m_tClientRt);
		hMemDC= CreateCompatibleDC(HDC(*pDC));
		hMembit = MakeDIBSection(*pDC, m_tClientRt.right, m_tClientRt.bottom);
		hOldMembit = (HBITMAP)SelectObject(hMemDC, hMembit);

		PatBlt(hMemDC, 0, 0, m_tClientRt.right, m_tClientRt.bottom, WHITENESS);	
		if(CopyAsset != NULL)
		{
			CopyAsset->Run();
			CopyAsset->Render(hMemDC, SCENE_EDITOR_TARGET);
		}

		if(CopyFrame != NULL)
		{
			CopyFrame->Run();
			CopyFrame->Render(hMemDC, SCENE_EDITOR_TARGET, false);
		}

		BitBlt(HDC(*pDC), 0, 0, m_tClientRt.right, m_tClientRt.bottom,
			hMemDC, 0, 0, SRCCOPY);	
		SelectObject(hMemDC, hOldMembit);
		DeleteDC(hMemDC);
		DeleteObject(hMembit);
		this->ReleaseDC(pDC);
	}
}

int CSceneAssetPreview::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;	

	//타이머 설정
	SetTimer(DRAW_ASSET_PREVIEW_TIMER, 10, 0);

	return 0;
}

void CSceneAssetPreview::OnDestroy()
{
	CWnd::OnDestroy();

	KillTimer(DRAW_ASSET_PREVIEW_TIMER);

	if(CopyAsset != NULL)
		SAFE_DELETE(CopyAsset);

	if(CopyFrame != NULL)
		SAFE_DELETE(CopyFrame);
}
