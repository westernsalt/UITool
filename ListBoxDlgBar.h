#pragma once


// CListBoxDlgBar 대화 상자입니다.

class CListBoxBase;

class CListBoxDlgBar : public CDialogBar
{
	DECLARE_DYNAMIC(CListBoxDlgBar)
	
	CEdit*		m_BGTexPathEdit;
	CEdit*		m_BGXPosEdit;
	CEdit*		m_BGYPosEdit;
	CEdit*		m_BGWidthEdit;
	CEdit*		m_BGHeightEdit;

	CEdit*		m_ListItemStartXPosEdit;
	CEdit*		m_ListItemStartYPosEdit;
	CEdit*		m_ListIntervalColEdit;
	CEdit*		m_ListIntervalRowEdit;
	CEdit*		m_ListItemCountEdit;

	CEdit*		m_ScrollPathEdit;
	CEdit*		m_ListItemPathEdit;

	CButton*	m_SetScrollBtn;
	CButton*	m_SetListItemBtn;

private:
	bool		m_bChange;

private:
	std::string	m_strBGTexPath;
	POINT		m_ptBGPos;
	POINT		m_ptBGSize;
	POINT		m_ptListitemStartPos;

	POINT		m_ptListInterval;
	int			m_iListItemCount;

	std::string		m_strScrollPath;
	std::string		m_strListItemPath;


private:	
	CListBoxBase*	m_ListBoxBase;
	CScrollBase*	m_ListBoxBaseScroll;
	CListItemBase*	m_ListBoxBaseItem;

private:
	HWND		m_PictureWnd;
	HWND		m_ParenthWnd;

private:
	CBaseInterface*	LoadBaseInterface(std::string FilePath, HWND _hWnd);

public:
	void	SetParenthWnd(HWND _hWnd);
	void	SetPicturehWnd(HWND _hWnd);

public:
	void	Initialize();
	void	CreateAsset();
	void	RefreshDlgBar(CListBoxBase* _LoadBase, POINT& _BgTex);
	void	SetListBoxProperty();
	void	SetMemberListBoxBase(CListBoxBase* _base);

public:
	CListBoxDlgBar(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CListBoxDlgBar();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnEnChangeEditLstShowTexpath();
	afx_msg void OnEnChangeEditLstBgXpos();
	afx_msg void OnEnChangeEditLstBgYpos();
	afx_msg void OnEnChangeEditLstBgWidth();
	afx_msg void OnEnChangeEditLstBgHeight();
	afx_msg void OnEnChangeEditLstColInterval();
	afx_msg void OnEnChangeEditLstRowInterval();
	afx_msg void OnEnChangeEditLstScrollPath();
	afx_msg void OnEnChangeEditLstListitemPath();
	afx_msg void OnBnClickedButtonSetScroll();
	afx_msg void OnBnClickedButtonSetListitem();
	virtual void OnUpdateCmdUI(CCmdUI* pCmdUI);
	afx_msg void OnEnChangeEditListitemCount();
	afx_msg LRESULT OnSelchangeTexturelist(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChangeDragboxInfo(WPARAM wParam, LPARAM lParam);
	afx_msg void OnEnChangeEditLstStartXpos();
	afx_msg void OnEnChangeEditLstStartYpos();
};
