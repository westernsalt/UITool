#pragma once
#include "afxwin.h"


class CGroupButton;
// CRadioBtnDlgBar 대화 상자입니다.

class CRadioBtnDlgBar : public CDialogBar 
{
	DECLARE_DYNAMIC(CRadioBtnDlgBar)

private:
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

	//	라디오 버튼 행렬 입력 에디트 컨트롤
	CEdit*		m_RadioBtnColEdit;
	CEdit*		m_RadioBtnRowEdit;
	CEdit*		m_RadioBtnIntervalEdit;

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
	CEdit*		m_TextSizeEdit;

	CEdit*		m_DisableRGB_R_Edit;
	CEdit*		m_DisableRGB_G_Edit;
	CEdit*		m_DisableRGB_B_Edit;

	CEdit*		m_TextPreviewEdit;

private:
	//	아이콘 추가를 위한 장치
	CEdit*		m_IconPathEdit;
	CEdit*		m_IconHorizontalIndentEdit;
	CEdit*		m_IconVerticalIndentEdit;
	CButton*	m_FrontIconRadioBtn;
	CButton*	m_BackIconRadioBtn;

	std::string m_strIconPath;
	POINT		m_ptIconIndent;
	int			m_iIconPosition;

private:
	CGroupButton* m_RadioGroupBase;

private:
	POINT		m_ptNormalTex;
	POINT		m_ptPressedTex;
	POINT		m_ptDisableTex;
	POINT		m_ptHighlightTex;
	POINT		m_ptCheckMarkTex;

	POINT		m_ptColRow;
	int			m_iArrayType;
	int			m_iRadioBtnInterval;

	bool		m_bChange;
	int			m_iTextSize;
	int			m_iSelBtn;
	int			m_iTextBoxInterval;

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
	HWND	m_ParenthWnd;
	HWND	m_PictureWnd;

public:
	POINT	m_ptCurPos;

public:
	void	SetPictureWnd(HWND _hWnd) { m_PictureWnd = _hWnd; }
	void	SetParenthWnd(HWND _hWnd) { m_ParenthWnd = _hWnd; }
	void	SetMemberRadioGroupBase(CGroupButton* _base);
	void	SetCheckMarkSize(POINT _size);
	void	Initialize();
	void	SetRadioBtnProperty();
	void	CreateAsset();
	void	CreateRadioBtn();
	void	RePositioningBtn();
	void	SettingControl( void* _data, eRadioBtnDlgBarControl _controlID );
	void	RefreshDlgBar(CGroupButton* _LoadBase, POINT& _NormalTex);

public:
	CRadioBtnDlgBar(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CRadioBtnDlgBar();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnUpdateCmdUI(CCmdUI* pCmdUI);
	afx_msg void OnEnChangeEditRbNmLeft();
	afx_msg void OnEnChangeEditRbNmTop();
	afx_msg void OnEnChangeEditRbPrLeft();
	afx_msg void OnEnChangeEditRbPrTop();
	afx_msg void OnEnChangeEditRbDaLeft();
	afx_msg void OnEnChangeEditRbDaTop();
	afx_msg void OnEnChangeEditRbHlLeft();
	afx_msg void OnEnChangeEditRbHlTop();
	afx_msg void OnEnChangeEditRbCmLeft();
	afx_msg void OnEnChangeEditRbCmTop();
	afx_msg void OnEnChangeEditRbBtnWidth();
	afx_msg void OnEnChangeEditRbBtnHeight();
	afx_msg void OnEnChangeEditRadioCol();
	afx_msg void OnEnChangeEditRadioRow();
	afx_msg void OnEnChangeEditRadioInterval();
	afx_msg void OnBnClickedRadioRbLeftSort();
	afx_msg void OnBnClickedRadioRbCenterSort();
	afx_msg void OnBnClickedRadioRbRightSort();
	afx_msg void OnEnChangeEditRbInputbox();
	afx_msg void OnEnChangeEditRbTextboxinterval();
	afx_msg void OnEnChangeEditRbRgbR();
	afx_msg void OnEnChangeEditRbRgbG();
	afx_msg void OnEnChangeEditRbRgbB();
	afx_msg void OnBnClickedRadioRbNmBtn();
	afx_msg void OnBnClickedRadioRbPrBtn();
	afx_msg void OnBnClickedRadioRbDaBtn();
	afx_msg void OnBnClickedRadioRbHlBtn();
	afx_msg void OnBnClickedRadioRbCmBtn();
	afx_msg LRESULT OnChangeDragboxInfo(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSelchangeTexturelist(WPARAM wParam, LPARAM lParam);
	afx_msg void OnEnChangeEditRbDaRgbR();
	afx_msg void OnEnChangeEditRbDaRgbG();
	afx_msg void OnEnChangeEditRbDaRgbB();
	afx_msg void OnBnClickedCheckDisablestate();
	afx_msg void OnEnChangeEditRadTextsize();
	afx_msg void OnEnChangeEditRadIconPath();
	afx_msg void OnBnClickedButtonRadLoadIcon();
	afx_msg void OnBnClickedRadioRadFrontIcon();
	afx_msg void OnBnClickedRadioRadBackIcon();
	afx_msg void OnEnChangeEditRadHorizonIndent();
	afx_msg void OnEnChangeEditRadVerticalIndent();
	afx_msg void OnBnClickedCheckRadDisableString();
	afx_msg void OnBnClickedCheckInputFontBold();
};
