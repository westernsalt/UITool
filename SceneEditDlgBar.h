#pragma once

// CSceneEditDlgBar

#include "UIFrameFileList.h"

class CSceneEditDlgBar : public CDialogBar
{
	enum eListType
	{
		TYPE_ASSETLIST,
		TYPE_FRAMELIST,
		TYPE_SCENELIST,
	};

	DECLARE_DYNAMIC(CSceneEditDlgBar)

	CListBox*	m_AssetList;
	CListBox*	m_UIFrameList;
	CListBox*	m_SceneList;
	CEdit*		m_BackgroundPathEdit;
	CEdit*		m_BlurPathEdit;
	CEdit*		m_ReviseFramePathEdit;

private:
	HWND		m_ParenthWnd;
	HWND		m_ViewhWnd;

	//	BrowseFolder ¸â¹ö º¯¼ö
	CString		m_strAssetListPath;
	CString		m_strSceneListPath;
	CString		m_strFrameListPath;
	CString		m_strBackgroundPath;
	CString		m_strBlurPath;
	CString		m_strReviseFramePath;

	int			m_iFocusAssetIndex;
	int			m_iFocusFrameIndex;
	int			m_iFocusSceneIndex;

public:
	void		Initialize();
	void		BrowseFolder(CString& _strPath, eListType _type);		
	void		RefreshAssetList();
	
	void		LoadBackgroundAsset();
	void		LoadAssetList(CString _path);
	void		LoadUIFrameList(CString _path);
	void		LoadSceneList(CString _path);

public:
	void		SetParenthWnd(HWND _hWnd);
	void		SetViewhWnd(HWND _hWnd);

public:
	CSceneEditDlgBar();
	virtual ~CSceneEditDlgBar();

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnUpdateCmdUI(CCmdUI* pCmdUI);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedButtonAssetlistLoad();
	afx_msg void OnBnClickedButtonSceneLoad();
	afx_msg void OnBnClickedButtonSceneSave();
	afx_msg LRESULT OnSaveSceneMsg(WPARAM wParam, LPARAM lParam);
	static int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
	afx_msg void OnBnClickedButtonAssetlistRefresh();
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg void OnLbnSelchangeListAsset();
	afx_msg void OnLbnDblclkListAsset();
	afx_msg void OnLbnSelchangeListFrame();
	afx_msg void OnLbnDblclkListFrame();
	afx_msg void OnLbnSelchangeListScene();
	afx_msg void OnLbnDblclkListScene();
	afx_msg void OnBnClickedButtonFrameLoad();
	afx_msg void OnBnClickedButtonFrameSave();
	afx_msg void OnBnClickedButtonLoadBgAsset();
	afx_msg void OnBnClickedButtonLoadBlurAsset();
	afx_msg void OnBnClickedButtonSceneAllclear();
	afx_msg void OnEnChangeEditLoadReviseFrame();
	afx_msg void OnBnClickedButtonLoadReviseFrame();
	afx_msg void OnEnChangeEditFrameScaleX();
};


