// SceneAssetPreviewWnd.cpp : ���� �����Դϴ�.
//
#include "stdafx.h"
#include "SceneAssetPreviewWnd.h"

IMPLEMENT_DYNCREATE(CSceneAssetPreviewWnd, CFrameWnd)
CSceneAssetPreviewWnd::CSceneAssetPreviewWnd()
{
}

CSceneAssetPreviewWnd::~CSceneAssetPreviewWnd()
{
	//���� �̸����� â ����
	::SendMessage(m_pParenthWnd, WM_SCENE_ASSET_PREVIEW_DESTROY, NULL, NULL);
}

//������ ����, ������ ����
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

	// �������� Ŭ���̾�Ʈ ������ �����ϴ� �並 ����ϴ�.
	//����, ������ �̸����� â �����
	if (!m_wndPreview.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("�� â�� ������ ���߽��ϴ�.\n");
		return -1;
	}
	m_wndPreview.MoveWindow(0, 0, WND_WIDTH, WND_WIDTH);
	
	return 0;
}
