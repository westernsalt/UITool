#pragma once

class CTextBox;
// CTextBoxDlgBar

class CTextBoxDlgBar : public CDialogBar
{
	DECLARE_DYNAMIC(CTextBoxDlgBar)
private:
	//	텍스트 인덱스, 사이즈
	CEdit*	m_TextSizeEdit;

	//	텍스트 박스 바운더리
	CEdit*	m_TextBoxWidthEdit;
	CEdit*	m_TextBoxHeightEdit;
	CEdit*	m_TextIndentXEdit;
	CEdit*	m_TextIndentYEdit;
	//	텍스트 컬러
	CEdit*	m_RedEdit;
	CEdit*	m_GreenEdit;
	CEdit*	m_BlueEdit;

	CEdit*		m_InputBoxEdit;
	CEdit*		m_TextVerticalIndentEdit;
	CButton*	m_AlignLeftRadioBtn;
	CButton*	m_AlignMidRadioBtn;
	CButton*	m_AlignRightRadioBtn;
	CButton*	m_DisableStringCheckBox;
	CButton*	m_FontBoldCheckBox;
	CString		m_strInputBox;

private:
	int		m_iVerticalIndent;
	int		m_iTextSize;
	RECT	m_tTextBox;
	UINT	m_iSortFlag;
	BYTE	m_bRed;
	BYTE	m_bGreen;
	BYTE	m_bBlue;
	POINT	m_ptTextIndent;

private:
	CTextBox*	m_CurTextBox;
	HWND		m_EditorViewhWnd;
	bool		m_bChange;


private:
	void	CreateAsset();
	void	EditDataToValue();

public:
	void	RefreshDlgBar(CTextBox* _LoadBase);

public:
	void	Initialize();
	void	SetAssetTextProperty();
	void	SetViewhWnd(HWND _hWnd);
	void	SetMemberTextBox(CTextBox*	_textbox);

public:
	CTextBoxDlgBar();
	virtual ~CTextBoxDlgBar();

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void DoDataExchange(CDataExchange* pDX);
	afx_msg void OnEnChangeEditTextWidth();
	afx_msg void OnEnChangeEditTextHeight();
	afx_msg void OnBnClickedRadioLeftSort();
	afx_msg void OnBnClickedRadioMidSort();
	afx_msg void OnBnClickedRadioRightSort();
	afx_msg void OnEnChangeEditRed();
	afx_msg void OnEnChangeEditGreen();
	afx_msg void OnEnChangeEditBlue();
	afx_msg void OnEnChangeEditInputbox();
	afx_msg void OnEnChangeEditTxtIndentX();
	afx_msg void OnEnChangeEditTxtIndentY();
	afx_msg void OnEnChangeEditTextScaling();
	afx_msg void OnBnClickedCheckTbDisableString();
	afx_msg void OnBnClickedCheckTbFontBold();
	afx_msg void OnEnChangeEditVerticalIndent();
};


