#pragma once

class CPictureWnd;

class CPictureDlgBar : public CDialogBar
{
	DECLARE_DYNAMIC(CPictureDlgBar)

	CListBox*	m_pTextureFileList;
	CButton*	m_pAllSelectChkBox;
	CButton*	m_pTextureLoadBtn;
	CEdit*		m_pDragBoxPos_X_Edit;
	CEdit*		m_pDragBoxPos_Y_Edit;
	CEdit*		m_pDragBoxWidthEdit;
	CEdit*		m_pDragBoxHeightEdit;

	CxImage*	m_pTexture;

	POINT		m_tDragBoxPos;
	POINT		m_tDragBoxSize;

private:
	HWND		m_pParentWnd;
	HWND		m_pDlgBarhWnd;

private:
	bool		m_bChange;
	CString		m_strTextureListPath;
	CString		m_strInitPath;

public:
	void	Initialize();
	void	BrowseFolder(CString& _strPath);
	void	LoadTextureList(CString _strPath);
	void	UpdateEditData(RECT _rtDragBox);
	void	SetEditWnd(int _num, eDragboxSet _set);

	void	SetTextureListPath(CString _Path);
	void	SetTextureListIndex(CString _name);
	void	SetTextureListIndex(int _index);
	void	SetPos(POINT _pos);
	void	SetSize(POINT _size);

public:
	RECT	GetCalculateRectByRate(RECT rect, double dValue);
	int		GetCurSelTextureListItem();
	void	SetCheckBoxUnchecked();
	void	SetParentWnd(HWND _hWnd){ m_pParentWnd = _hWnd;}
	void	SetDlgBarhWnd(HWND _hWnd){ m_pDlgBarhWnd = _hWnd; }
public:
	CPictureDlgBar();
	virtual ~CPictureDlgBar();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLbnSelchangeListTexturefile();

protected:
	afx_msg LRESULT OnAllSelChecked(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedCheckAllSelect();
	afx_msg void OnBnClickedButtonTextureLoad();
	afx_msg void OnBnClickedButtonAssetComposition();

	afx_msg void OnUpdateCmdUI(CCmdUI* pCmdUI);

public:
	static int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
	afx_msg void OnEnChangeEditDragboxX();
	afx_msg void OnEnChangeEditDragboxY();
	afx_msg void OnEnChangeEditDragboxWidth();
	afx_msg void OnEnChangeEditDragboxHeight();
};


