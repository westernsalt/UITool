// SceneAssetPreviewWnd.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "SceneAssetPreviewWnd.h"

IMPLEMENT_DYNCREATE(CSceneAssetPreviewWnd, CFrameWnd)
CSceneAssetPreviewWnd::CSceneAssetPreviewWnd()
{
}

CSceneAssetPreviewWnd::~CSceneAssetPreviewWnd()
{
	//에셋 미리보기 창 삭제
	::SendMessage(m_pParenthWnd, WM_SCENE_ASSET_PREVIEW_DESTROY, NULL, NULL);
}

//선택한 에셋, 프레임 세팅
void CSceneAssetPreviewWnd::SetFocusAssetIndex(int nFocusIndex, eRefreshType assetType)
{
	if(assetType == REFRESH_ASSET)
		m_wndPreview.SetAssetPreview(nFocusIndex);
	else if(assetType = REFRESH_FRAME)
		m_wndPreview.SetFramePreview(nFocusIndex);
}

BEGIN_MESSAGE_MAP(CSceneAssetPreviewWnd, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()


int CSceneAssetPreviewWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 프레임의 클라이언트 영역을 차지하는 뷰를 만듭니다.
	//에셋, 프레임 미리보기 창 만들기
	if (!m_wndPreview.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("뷰 창을 만들지 못했습니다.\n");
		return -1;
	}
	m_wndPreview.MoveWindow(0, 0, WND_WIDTH, WND_WIDTH);
	
	return 0;
}
