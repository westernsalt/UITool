
// MainFrm.h : CMainFrame Ŭ������ �������̽�
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

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// �����Դϴ�.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	CMainView			m_wndView;

public:
	void	CreateSceneAssetPreviewWnd(int nFocusIndex, eRefreshType assetType);

// ������ �޽��� �� �Լ�
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


