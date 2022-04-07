#pragma once
#include "SceneAssetPreview.h"

// CPictureWnd 프레임입니다.
class CSceneAssetPreviewWnd : public CFrameWnd
{
	DECLARE_DYNCREATE(CSceneAssetPreviewWnd)

private:
	HWND				m_pParenthWnd;
	CSceneAssetPreview	m_wndPreview;	//에셋, 프레임 미리보기 뷰

public:
	void	SetParenthWnd(HWND _hWnd) { m_pParenthWnd = _hWnd; }

public:	
	void	SetFocusAssetIndex(int nFocusIndex, eRefreshType assetType);	//선택한 에셋, 프레임 세팅

public:
	CSceneAssetPreviewWnd();
	virtual ~CSceneAssetPreviewWnd();
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


