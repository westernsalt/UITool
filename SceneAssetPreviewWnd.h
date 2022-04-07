#pragma once
#include "SceneAssetPreview.h"

// CPictureWnd �������Դϴ�.
class CSceneAssetPreviewWnd : public CFrameWnd
{
	DECLARE_DYNCREATE(CSceneAssetPreviewWnd)

private:
	HWND				m_pParenthWnd;
	CSceneAssetPreview	m_wndPreview;	//����, ������ �̸����� ��

public:
	void	SetParenthWnd(HWND _hWnd) { m_pParenthWnd = _hWnd; }

public:	
	void	SetFocusAssetIndex(int nFocusIndex, eRefreshType assetType);	//������ ����, ������ ����

public:
	CSceneAssetPreviewWnd();
	virtual ~CSceneAssetPreviewWnd();
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


