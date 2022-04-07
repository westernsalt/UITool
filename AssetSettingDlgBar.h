#pragma once


// CAssetSettingDlgBar

class CComboBox;

class CAssetSettingDlgBar : public CDialogBar
{
	DECLARE_DYNAMIC(CAssetSettingDlgBar)

private:
	HWND		m_AssetEditorhWnd;
	int			m_iComboBoxIndex;
	bool		m_bEditFocused;

	CEdit*		m_pPos_X_Edit;
	CEdit*		m_pPos_Y_Edit;
	CEdit*		m_pPos_Z_Edit;

	CString		m_strPosX;
	CString		m_strPosY;
	CString		m_strPosZ;

	CComboBox*	m_AttComboBox;
	CComboBox*	m_TypeComboBox;

	CListBox*	m_AssetListBox;
	CButton*	m_AssetListLoadBtn;

	//	BrowseFolder ¸â¹ö º¯¼ö
	CString		m_strAssetListInitPath;

	bool		m_bPictureWnd;


public:
	void		InitializeRrc();
	void		InitComboBox();
	void		UpdateEdit();
	void		RefreshAssetList();
public:
	void		BrowseFolder();
	void		SetPicWndBtnEnable(bool _bEnable);
	void		AdaptedChangedPosEdit();

public:
	std::string	GetSelAssetType();
	void		SetParenthWnd(HWND _hWnd) { m_AssetEditorhWnd = _hWnd; }
	void		SetPosEdit(CString _pos, eAxisType _type);
	void		SetAttCombobox(int _index);

	
public:
	CAssetSettingDlgBar();
	virtual ~CAssetSettingDlgBar();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnCbnSelchangeComboAssetAtt();
	afx_msg void OnCbnSelchangeComboAssetType();
	afx_msg void OnUpdateCmdUI(CCmdUI* pCmdUI);
	afx_msg void OnBnClickedButtonAssetlistLoad();
	afx_msg void OnBnClickedButtonAssetviewClear();
	afx_msg void OnEnChangeEditTexposX();
	afx_msg void OnEnChangeEditTexposY();
	afx_msg void OnEnChangeEditTexposZ();

	afx_msg void OnEnSetfocusEditTexposX();
	afx_msg void OnEnSetfocusEditTexposY();
	afx_msg void OnEnSetfocusEditTexposZ();
	static int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
	afx_msg LRESULT OnAssetBaseFocused(WPARAM wParam, LPARAM lParam);
	afx_msg void OnLbnDblclkAssetListEdit();
	afx_msg void OnBnClickedButtonPicturewndCreate();
	afx_msg void OnBnClickedButtonSaveEditorAsset();
	afx_msg void OnBnClickedButtonLoadEditorAsset();
};


