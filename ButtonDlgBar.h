#pragma once

// CButtonDlgBar
class CBtnBase;

class CButtonDlgBar : public CDialogBar
{
	DECLARE_DYNAMIC(CButtonDlgBar)

	CEdit*		m_TexturePathEdit;
	CEdit*		m_NormalLeft_Edit;
	CEdit*		m_NormalTop_Edit;
	CEdit*		m_PressedLeft_Edit;
	CEdit*		m_PressedTop_Edit;
	CEdit*		m_HighLightLeft_Edit;
	CEdit*		m_HIghLightTop_Edit;
	CEdit*		m_DisableLeft_Edit;
	CEdit*		m_DisableTop_Edit;
	CEdit*		m_BtnWidthEdit;
	CEdit*		m_BtnHeightEdit;
	CEdit*		m_BtnTextVerticalIndentEdit;

	CButton*	m_NormalRadioBtn;
	CButton*	m_PressedRadioBtn;
	CButton*	m_DisableRadioBtn;
	CButton*	m_HighlightRadioBtn;

	CButton*	m_RadioSortLeftBtn;
	CButton*	m_RadioSortMidBtn;
	CButton*	m_RadioSortRightBtn;
	CButton*	m_DisableStateCheckBox;
	CButton*	m_DisableStringCheckBox;
	CButton*	m_FontBoldCheckBox;			//폰트 볼드 적용 여부

	CButton*	m_TextSetApartCheckBox;
	CButton*	m_IconBackRadioBtn;
	CButton*	m_IconFrontRadioBtn;
	CEdit*		m_IndentEdit;

private:
	//	버튼 텍스트
	CEdit*		m_TextRGB_R_Edit;
	CEdit*		m_TextRGB_G_Edit;
	CEdit*		m_TextRGB_B_Edit;

	CEdit*		m_TextSizeEdit;

	CEdit*		m_DisableRGB_R_Edit;
	CEdit*		m_DisableRGB_G_Edit;
	CEdit*		m_DisableRGB_B_Edit;

	CEdit*		m_TextIndentXEdit;
	CEdit*		m_TextIndentYEdit;

private:
	CBtnBase*	m_BtnBase;

private:
	POINT		m_ptNormalTex;
	POINT		m_ptPressedTex;
	POINT		m_ptDisableTex;
	POINT		m_ptHighlightTex;

	int			m_iSelBtn;
	int			m_iTextSize;
	bool		m_bChange;
	POINT		m_ptBtnSize;
	POINT		m_ptTextIndent;
	std::string	m_strTexPath;

	// 버튼 텍스트
	int			m_iSortFlag;
	BYTE		m_byRed;
	BYTE		m_byGreen;
	BYTE		m_byBlue;

	BYTE		m_byDisableRed;
	BYTE		m_byDisableGreen;
	BYTE		m_byDisableBlue;

private:
	HWND		m_PicturehWnd;
	HWND		m_pParenthWnd;

public:
	void		SetParenthWnd(HWND _hWnd){	m_pParenthWnd = _hWnd;	}
	void		SetPictureWnd(HWND _hWnd){	m_PicturehWnd = _hWnd;	}
	void		SetBtnPosition(int _x, int _y);
	void		SetMemberBtnBase(CBtnBase* _btn) { m_BtnBase = _btn; }
	void		SetBtnbaseNULL();

public:
	void		Initialize();
	void		CreateAsset();
	void		RefreshDlgBar(CBtnBase* _LoadBase, POINT& _NormalTex);
	void		SetButtonProperty();

public:
	CButtonDlgBar();
	virtual ~CButtonDlgBar();

protected:
	DECLARE_MESSAGE_MAP()
	virtual void DoDataExchange(CDataExchange* pDX);
public:
	afx_msg void OnEnChangeEditNmLeft();
	afx_msg void OnEnChangeEditNmTop();
	afx_msg void OnEnChangeEditPrLeft();
	afx_msg void OnEnChangeEditPrTop();
	afx_msg void OnEnChangeEditDaLeft();
	afx_msg void OnEnChangeEditDaTop();
	afx_msg void OnEnChangeEditHlLeft();
	afx_msg void OnEnChangeEditHlTop();
	afx_msg void OnEnChangeEditBtnWidth();
	afx_msg void OnEnChangeEditBtnHeight();

	afx_msg void OnBnClickedRadioNmBtn();
	afx_msg void OnBnClickedRadioPrBtn();
	afx_msg void OnBnClickedRadioDaBtn();
	afx_msg void OnBnClickedRadioHlBtn();
protected:
	afx_msg LRESULT OnChangeDragboxInfo(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSelchangeTexturelist(WPARAM wParam, LPARAM lParam);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedRadioBtnTextLeft();
	afx_msg void OnBnClickedRadioBtnTextCenter();
	afx_msg void OnBnClickedRadioBtnTextRight();
	afx_msg void OnEnChangeEditBtnTextRed();
	afx_msg void OnEnChangeEditBtnTextGreen();
	afx_msg void OnEnChangeEditBtnTextBlue();

	afx_msg void OnEnChangeEditBtnDaTextRed();
	afx_msg void OnEnChangeEditBtnDaTextGreen();
	afx_msg void OnEnChangeEditBtnDaTextBlue();
	afx_msg void OnBnClickedCheckShowDisablestate();
	afx_msg void OnEnChangeEditBtnIndentX();
	afx_msg void OnEnChangeEditBtnIndentY();
	afx_msg void OnEnChangeEditBtnTextsize();
	afx_msg void OnBnClickedCheckBtnDisableString();
	afx_msg void OnBnClickedCheckBtnFontBold();			//폰트 볼드 적용 여부
	afx_msg void OnBnClickedCheckBtnTextSetApart();
	afx_msg void OnBnClickedRadioBtnIconFront();
	afx_msg void OnBnClickedRadioBtnIconBack();
	afx_msg void OnEnChangeEditBtnTextboxIndent();
	afx_msg void OnEnChangeEditBtnVerticalIndent();
};


