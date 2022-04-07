#pragma once


// CScrollDlgBar 대화 상자입니다.

class CScrollBase;

class CScrollDlgBar : public CDialogBar
{
	DECLARE_DYNAMIC(CScrollDlgBar)

	CEdit* m_TexturePathEdit;
	CEdit* m_NormalLeftPosEdit;
	CEdit* m_NormalTopPosEdit;
	CEdit* m_PressedLeftPosEdit;
	CEdit* m_PressedTopPosEdit;
	CEdit* m_DisableLeftPosEdit;
	CEdit* m_DisableTopPosEdit;
	CEdit* m_HighlightLeftPosEdit;
	CEdit* m_HighlightTopPosEdit;
	CEdit* m_BtnWidthEdit;
	CEdit* m_BtnHeightEdit;

	CEdit* m_BgLeftEdit;
	CEdit* m_BgTopEdit;
	CEdit* m_BgWidthEdit;
	CEdit* m_BgHeightEdit;

	CButton* m_NormalRadioBtn;
	CButton* m_PressedRadioBtn;
	CButton* m_DisableRadioBtn;
	CButton* m_HighlightRadioBtn;
	CButton* m_BackGroundRadioBtn;

protected:
	std::string	m_strTexPath;
	POINT		m_ptNormalTex;
	POINT		m_ptPressedTex;
	POINT		m_ptDisableTex;
	POINT		m_ptHighlightTex;
	POINT		m_ptBtnSize;

	POINT		m_ptBackground;
	POINT		m_ptBackgroundSize;

	int			m_iSelBtn;

private:
	CScrollBase* m_ScrollBase;

private:
	SaveData	m_BgData;

private:
	bool	m_bChange;
	HWND	m_ParenthWnd;
	HWND	m_PicturehWnd;

public:
	void	Initialize();
	void	CreateAsset();
	void	SetScrollProperty();
	void	RefreshDlgBar(CScrollBase* _LoadBase, POINT& _normalTex);

public:
	void	SetParenthWnd(HWND _hWnd){	m_ParenthWnd = _hWnd;	}
	void	SetPictureWnd(HWND _hWnd){	m_PicturehWnd = _hWnd;	}
	void	SetScrollBase(CScrollBase*	_base){ m_ScrollBase = _base; }
public:
	CScrollDlgBar(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CScrollDlgBar();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnSelchangeTexturelist(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChangeDragboxInfo(WPARAM wParam, LPARAM lParam);

public:
	afx_msg void OnEnChangeEditSldNmLeft();
	afx_msg void OnEnChangeEditSldNmTop();
	afx_msg void OnEnChangeEditSldPrLeft();
	afx_msg void OnEnChangeEditSldPrTop();
	afx_msg void OnEnChangeEditSldDaLeft();
	afx_msg void OnEnChangeEditSldDaTop();
	afx_msg void OnEnChangeEditSldHlLeft();
	afx_msg void OnEnChangeEditSldHlTop();
	afx_msg void OnEnChangeEditSldBtnWidth();
	afx_msg void OnEnChangeEditSldBtnHeight();
	afx_msg void OnEnChangeEditSldBgLeft();
	afx_msg void OnEnChangeEditSldBgTop();
	afx_msg void OnEnChangeEditSldBgWidth();
	afx_msg void OnEnChangeEditSldBgHeight();
	afx_msg void OnBnClickedRadioSldNmBtn();
	afx_msg void OnBnClickedRadioSldPrBtn();
	afx_msg void OnBnClickedRadioSldDaBtn();
	afx_msg void OnBnClickedRadioSldHlBtn();
	afx_msg void OnBnClickedRadioSldBg();
};
