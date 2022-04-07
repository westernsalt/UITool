#pragma once


// CComboboxDlgBar 대화 상자입니다.

class CComboBoxBase;

class CComboboxDlgBar : public CDialogBar
{
	DECLARE_DYNAMIC(CComboboxDlgBar)

private:
	//	노말 스테이트 버튼
	CEdit*		m_NormalLeft_Edit;
	CEdit*		m_NormalTop_Edit;
	CEdit*		m_PressedLeft_Edit;
	CEdit*		m_PressedTop_Edit;
	CEdit*		m_HighLightLeft_Edit;
	CEdit*		m_HighLightTop_Edit;
	CEdit*		m_DisableLeft_Edit;
	CEdit*		m_DisableTop_Edit;

	CButton*	m_NormalRadioBtn;
	CButton*	m_PressedRadioBtn;
	CButton*	m_DisableRadioBtn;
	CButton*	m_HighlightRadioBtn;

	//	오픈 스테이트 버튼
	CEdit*		m_OpenNormalLeft_Edit;
	CEdit*		m_OpenNormalTop_Edit;
	CEdit*		m_OpenPressedLeft_Edit;
	CEdit*		m_OpenPressedTop_Edit;
	CEdit*		m_OpenHighLightLeft_Edit;
	CEdit*		m_OpenHIghLightTop_Edit;
	CEdit*		m_OpenDisableLeft_Edit;
	CEdit*		m_OpenDisableTop_Edit;

	CButton*	m_OpenNormalRadioBtn;
	CButton*	m_OpenPressedRadioBtn;
	CButton*	m_OpenDisableRadioBtn;
	CButton*	m_OpenHighlightRadioBtn;

	//	텍스트관련 컨트롤
	CButton*	m_RadioSortLeftBtn;
	CButton*	m_RadioSortMidBtn;
	CButton*	m_RadioSortRightBtn;
	CEdit*		m_TextRGB_R_Edit;
	CEdit*		m_TextRGB_G_Edit;
	CEdit*		m_TextRGB_B_Edit;
	CEdit*		m_TextPreviewEdit;

	CEdit*		m_TextIndentXEdit;
	CEdit*		m_TextIndentYEdit;

	CEdit*		m_DisableRGB_R_Edit;
	CEdit*		m_DisableRGB_G_Edit;
	CEdit*		m_DisableRGB_B_Edit;
	CButton*	m_DisableCheckBox;

	//	버튼 사이즈
	CEdit*		m_BtnWidthEdit;
	CEdit*		m_BtnHeightEdit;

	//	리스트박스 설정 컨트롤
	CEdit*		m_ListBoxPathEdit;
	CButton*	m_SetListBoxBtn;

private:
	POINT		m_ptNormalTex;
	POINT		m_ptPressedTex;
	POINT		m_ptDisableTex;
	POINT		m_ptHighlightTex;

	POINT		m_ptOpenNormalTex;
	POINT		m_ptOpenPressedTex;
	POINT		m_ptOpenDisableTex;
	POINT		m_ptOpenHighlightTex;
	POINT		m_ptTextIndent;

	//	버튼사이즈(공통)
	POINT		m_ptBtnSize;

	//	라디오 버튼 선택
	int			m_iSelBtn;

	//	리스트박스 경로 저장
	std::string	m_strListBoxPath;

	// 버튼 텍스트
	int			m_iSortFlag;
	BYTE		m_byRed;
	BYTE		m_byGreen;
	BYTE		m_byBlue;
	BYTE		m_byDisableRed;
	BYTE		m_byDisableGreen;
	BYTE		m_byDisableBlue;
	CString		m_strPreview;
	std::string	m_strTexFilePath;

private:
	bool		m_bChange;
	HWND		m_ParenthWnd;
	HWND		m_PictureWnd;

private:
	CComboBoxBase* m_ComboBoxBase;

public:
	void		Initialize();

public:
	void SetParenthWnd(HWND _hWnd);
	void SetPictureWnd(HWND _hWnd);
	void SetMemberComboBoxBase(CComboBoxBase* _combobox) { m_ComboBoxBase = _combobox; }

public:
	void CreateAsset();
	void SetComboBoxProperty();
	CBaseInterface* LoadBaseInterface( std::string FilePath, HWND _hWnd );
	void RefreshDlgBar(CComboBoxBase* _LoadBase, POINT& _NormalTex);

public:
	CComboboxDlgBar(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CComboboxDlgBar();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual void OnUpdateCmdUI(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnSelchangeTexturelist(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChangeDragboxInfo(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnEnChangeEditCmbNmLeft();
	afx_msg void OnEnChangeEditCmbNmTop();
	afx_msg void OnEnChangeEditCmbHlLeft();
	afx_msg void OnEnChangeEditCmbHlTop();
	afx_msg void OnEnChangeEditCmbPrLeft();
	afx_msg void OnEnChangeEditCmbPrTop();
	afx_msg void OnEnChangeEditCmbDaLeft();
	afx_msg void OnEnChangeEditCmbDaTop();
	afx_msg void OnEnChangeEditCmbOpenNmLeft();
	afx_msg void OnEnChangeEditCmbOpenNmTop();
	afx_msg void OnEnChangeEditCmbOpenHlLeft();
	afx_msg void OnEnChangeEditCmbOpenHlTop();
	afx_msg void OnEnChangeEditCmbOpenPrLeft();
	afx_msg void OnEnChangeEditCmbOpenPrTop();
	afx_msg void OnEnChangeEditCmbOpenDaLeft();
	afx_msg void OnEnChangeEditCmbOpenDaTop();
	afx_msg void OnEnChangeEditCmbBtnWidth();
	afx_msg void OnEnChangeEditCmbBtnHeight();
	afx_msg void OnBnClickedRadioCmbNmBtn();
	afx_msg void OnBnClickedRadioCmbHlBtn();
	afx_msg void OnBnClickedRadioCmbPrBtn();
	afx_msg void OnBnClickedRadioCmbDaBtn();
	afx_msg void OnBnClickedRadioCmbOpenNmBtn();
	afx_msg void OnBnClickedRadioCmbOpenHlBtn();
	afx_msg void OnBnClickedRadioCmbOpenPrBtn();
	afx_msg void OnBnClickedRadioCmbOpenDaBtn();
	afx_msg void OnBnClickedRadioCmbTextLeft();
	afx_msg void OnBnClickedRadioCmbTextCenter();
	afx_msg void OnBnClickedRadioCmbTextRight();
	afx_msg void OnEnChangeEditCmbTextpreview();
	afx_msg void OnEnChangeEditCmbTextRed();
	afx_msg void OnEnChangeEditCmbTextGreen();
	afx_msg void OnEnChangeEditCmbTextBlue();
	afx_msg void OnBnClickedButtonSetListbox();
	afx_msg void OnEnChangeEditCmbDaTextRed();
	afx_msg void OnEnChangeEditCmbDaTextGreen();
	afx_msg void OnEnChangeEditCmbDaTextBlue();
	afx_msg void OnBnClickedCheckCmbDisablestate();
	afx_msg void OnEnChangeEditCmbIndentX();
	afx_msg void OnEnChangeEditCmbIndentY();
};
