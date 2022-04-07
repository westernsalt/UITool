#pragma once


// CCheckBoxDlgBar 대화 상자입니다.

class CCheckBoxBase;

class CCheckBoxDlgBar : public CDialogBar
{
	DECLARE_DYNAMIC(CCheckBoxDlgBar)

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
	CEdit*		m_CheckMarkLeftEdit;
	CEdit*		m_CheckMarkTopEdit;

	CEdit*		m_TextSizeEdit;

	CButton*	m_NormalRadioBtn;
	CButton*	m_PressedRadioBtn;
	CButton*	m_DisableRadioBtn;
	CButton*	m_HighlightRadioBtn;
	CButton*	m_CheckMarkRadioBtn;

	CButton*	m_RadioSortLeftBtn;
	CButton*	m_RadioSortMidBtn;
	CButton*	m_RadioSortRightBtn;

	CButton*	m_DisableCheckBox;
	CButton*	m_DisableStringCheckBox;
	CButton*	m_FontBoldCheckBox;

private:
	//	버튼 텍스트
	CEdit*		m_TextRGB_R_Edit;
	CEdit*		m_TextRGB_G_Edit;
	CEdit*		m_TextRGB_B_Edit;
	CEdit*		m_IntervalEdit;

	CEdit*		m_DisableRGB_R_Edit;
	CEdit*		m_DisableRGB_G_Edit;
	CEdit*		m_DisableRGB_B_Edit;

	CEdit*		m_TextPreviewEdit;
	

private:
	CCheckBoxBase* m_CheckBoxBase;

private:
	POINT		m_ptNormalTex;
	POINT		m_ptPressedTex;
	POINT		m_ptDisableTex;
	POINT		m_ptHighlightTex;
	POINT		m_ptCheckMarkTex;

	int			m_iTextSize;
	int			m_iSelBtn;
	int			m_iInterval;
	bool		m_bChange;
	POINT		m_ptBtnSize;
	POINT		m_ptCheckMarkSize;
	std::string	m_strTexPath;

	// 버튼 텍스트
	int			m_iSortFlag;
	BYTE		m_byRed;
	BYTE		m_byGreen;
	BYTE		m_byBlue;

	BYTE		m_byDisableRed;
	BYTE		m_byDisableGreen;
	BYTE		m_byDisableBlue;
	
	CString		m_strPreview;

private:
	//	아이콘 등록 위한 변수
	CEdit*		m_NormalIconPathEdit;
	CEdit*		m_DisableIconPathEdit;
	CEdit*		m_IconHorizontalIndentEdit;
	CEdit*		m_IconVerticalIndentEdit;

	CButton*	m_FrontIconRadioBtn;
	CButton*	m_BackIconRadioBtn;

	int			m_iIconPosition;
	POINT		m_ptIconIndent;
	std::string	m_strIconPath;
	std::string m_strIconDisablePath;

	CAssetBase*	m_IconAssetBase;
	CAssetBase* m_IconDisableAssetBase;

private:
	HWND	m_ParenthWnd;
	HWND	m_PictureWnd;

public:
	void SetParenthWnd(HWND _hWnd);
	void SetPictureWnd(HWND _hWnd);
	void SetBoxPosition(int _x, int _y);
	void SetMemberBtnBase(CCheckBoxBase* _btn) { m_CheckBoxBase = _btn; }
	void SetCheckMarkSize(POINT _size);
	void SetCheckBoxbaseNULL();

public:
	void SetCheckBoxProperty();
	void Initialize();
	void CreateAsset();
	void RefreshDlgBar(CCheckBoxBase* _LoadBase, POINT& _NormalTex);

private:
	CBaseInterface* LoadBaseInterface( std::string FilePath, HWND _hWnd );

public:
	CCheckBoxDlgBar(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CCheckBoxDlgBar();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnUpdateCmdUI(CCmdUI* pCmdUI);
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

	afx_msg void OnBnClickedRadioBtnTextLeft();
	afx_msg void OnBnClickedRadioBtnTextCenter();
	afx_msg void OnBnClickedRadioBtnTextRight();
	afx_msg void OnEnChangeEditBtnTextRed();
	afx_msg void OnEnChangeEditBtnTextGreen();
	afx_msg void OnEnChangeEditBtnTextBlue();

	afx_msg LRESULT OnChangeDragboxInfo(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSelchangeTexturelist(WPARAM wParam, LPARAM lParam);

	afx_msg void OnEnChangeEditCbInterval();
	afx_msg void OnBnClickedRadioCbCmBtn();
	afx_msg void OnEnChangeEditCbCmLeft();
	afx_msg void OnEnChangeEditCbCmTop();
	afx_msg void OnEnChangeEditTextpreview();
	afx_msg void OnBnClickedCheckShowDisableChk();
	afx_msg void OnEnChangeEditCbDaTextRed();
	afx_msg void OnEnChangeEditCbDaTextGreen();
	afx_msg void OnEnChangeEditCbDaTextBlue();
	afx_msg void OnEnChangeEditChkTextsize();
	afx_msg void OnBnClickedButtonChkLoadIcon();
	afx_msg void OnBnClickedRadioChkBackIcon();
	afx_msg void OnBnClickedFontBold();
	afx_msg void OnBnClickedRadioChkFrontIcon();
	afx_msg void OnEnChangeEditChkIconPath();
	afx_msg void OnEnChangeEditChkHorizontalIndent();
	afx_msg void OnEnChangeEditChkVerticalIndent();
	afx_msg void OnBnClickedButtonChkLoadDaIcon();
	afx_msg void OnEnChangeEditChkIconDaPath();
	afx_msg void OnBnClickedCheckChkDisableString();
};
