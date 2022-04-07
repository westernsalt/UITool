#pragma once
#include "afxwin.h"


// CInputTextDlgBar 대화 상자입니다.

class CInputText;

class CInputTextDlgBar : public CDialogBar
{
	DECLARE_DYNAMIC(CInputTextDlgBar)

private:
	CEdit*	m_BackgroundPathEdit;
	CEdit*	m_BackgroundXPosEdit;
	CEdit*	m_BackgroundYPosEdit;
	CEdit*	m_BackgroundWidthEdit;
	CEdit*	m_BackgroundHeightEdit;

	CEdit*	m_WritingIntervalEdit;
	CEdit*	m_ColorRedEdit;
	CEdit*	m_ColorGreenEdit;
	CEdit*	m_ColorBlueEdit;

	CButton*	m_PasswordCheckBox;
	CButton*	m_RadioSortLeftBtn;
	CButton*	m_RadioSortCenterBtn;
	CButton*	m_RadioSortRightBtn;
	CButton*	m_FontBoldCheckBox;

	CButton*	m_DisableCheckBox;

	CEdit*		m_DisableRGB_R_Edit;
	CEdit*		m_DisableRGB_G_Edit;
	CEdit*		m_DisableRGB_B_Edit;

	CEdit*		m_TextSizeEdit;
	

private:
	POINT	m_tBackgroundStartPos;
	POINT	m_tBackgroundSize;
	
	UINT	m_iSortFlag;
	int		m_iInterval;
	int		m_iTextSize;
	BYTE	m_byRed;
	BYTE	m_byGreen;
	BYTE	m_byBlue;

	BYTE		m_byDisableRed;
	BYTE		m_byDisableGreen;
	BYTE		m_byDisableBlue;

	bool	m_bChange;
	bool	m_bPassword;
	bool	m_bFontBold;

	std::string	m_strBackgroundPath;

private:
	CInputText*	m_InputTextBase;

private:
	HWND	m_pParenthWnd;
	HWND	m_PicturehWnd;

public:
	void		SetParenthWnd(HWND _hWnd){	m_pParenthWnd = _hWnd;	}
	void		SetPictureWnd(HWND _hWnd){	m_PicturehWnd = _hWnd;	}
	void		SetMemberInputTextBase(CInputText* _base) { m_InputTextBase = _base; }

public:
	void	Initialize();
	void	CreateAsset();
	void	SetInputTextProperty();
	void	RefreshDlgBar(CInputText* _LoadBase, POINT& _NormalTex);

public:
	CInputTextDlgBar(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CInputTextDlgBar();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnEnChangeEditInputBackground();
	afx_msg void OnEnChangeEditInputXpos();
	afx_msg void OnEnChangeEditInputWidth();
	afx_msg void OnEnChangeEditInputYpos();
	afx_msg void OnEnChangeEditInputHeight();
	afx_msg void OnBnClickedCheckInputCheckbox();
	afx_msg void OnBnClickedCheckInputFontBold();
	afx_msg void OnEnChangeEditInputInterval();
	afx_msg void OnBnClickedRadioInputLeftSort();
	afx_msg void OnBnClickedRadioInputCenterSort();
	afx_msg void OnBnClickedRadioInputRightSort();
	afx_msg void OnEnChangeEditInputRgbR();
	afx_msg void OnEnChangeEditInputRgbG();
	afx_msg void OnEnChangeEditInputRgbB();
protected:
	afx_msg LRESULT OnSelchangeTexturelist(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChangeDragboxInfo(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnEnChangeEditInputDaRgbR();
	afx_msg void OnEnChangeEditInputDaRgbG();
	afx_msg void OnEnChangeEditInputDaRgbB();
	afx_msg void OnBnClickedCheckInputDisablestate();
	afx_msg void OnEnChangeEditInputTextsize();
};
