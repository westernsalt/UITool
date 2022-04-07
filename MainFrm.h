
// MainFrm.h : CMainFrame 클래스의 인터페이스
//

#pragma once
#include "MainView.h"
class CAssetEditor;
class CSceneEditDlgBar;
class CSceneSettingDlgBar;
class CSceneAssetPreviewWnd;

#include "InputText.h"

class CMainFrame : public CFrameWnd
{
private:
	CAssetEditor*			m_pAssetEditFrame;

	CWnd*					m_pEditDlgBarFrame;

	CSceneEditDlgBar*		m_pSceneEditDlgBar;
	CSceneSettingDlgBar*	m_pSceneSettingDlgBar;
	CSceneAssetPreviewWnd*	m_pSceneAssetPreviewWnd;

	CString m_strInput;

public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// 구현입니다.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	CMainView			m_wndView;

public:
	void	CreateSceneAssetPreviewWnd(int nFocusIndex, eRefreshType assetType);

// 생성된 메시지 맵 함수
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnOpenEditor();
	afx_msg void OnDestroy();
	afx_msg LRESULT OnBasicAssetFocused(WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
protected:
	afx_msg LRESULT OnRequestString(WPARAM wParam, LPARAM lParam);
protected:
	afx_msg LRESULT OnInitString(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUiframeFocused(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChangeResolution(WPARAM wParam, LPARAM lParam);	
	afx_msg LRESULT OnAssetListDoubleClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFrameListDoubleClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSceneListDoubleClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSelectAssetPreView(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSelectFramePreView(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSceneAssetWndDestroy(WPARAM wParam, LPARAM lParam);
};


