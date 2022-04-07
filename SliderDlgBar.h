#pragma once
#include "afxwin.h"


class CSliderBase;
// CSliderDlgBar 대화 상자입니다.

class CSliderDlgBar : public CDialogBar
{
	DECLARE_DYNAMIC(CSliderDlgBar)

protected:
	DECLARE_MESSAGE_MAP()
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
	
	CEdit* m_SliderGaugeLeftEdit;
	CEdit* m_SliderGaugeTopEdit;
	CEdit* m_SliderGaugeWidthEdit;
	CEdit* m_SliderGaugeHeightEdit;
	CEdit* m_SliderBgLeftEdit;
	CEdit* m_SliderBgTopEdit;
	CEdit* m_SliderBgWidthEdit;
	CEdit* m_SliderBgHeightEdit;

	CEdit* m_GaugeStartXPosEdit;
	CEdit* m_GaugeStartYPosEdit;

	CButton* m_NormalRadioBtn;
	CButton* m_PressedRadioBtn;
	CButton* m_DisableRadioBtn;
	CButton* m_HighlightRadioBtn;
	CButton* m_GaugeBarRadioBtn;
	CButton* m_BackGroundRadioBtn;

protected:
	std::string	m_strTexPath;
	POINT		m_ptNormalTex;
	POINT		m_ptPressedTex;
	POINT		m_ptDisableTex;
	POINT		m_ptHighlightTex;
	POINT		m_ptBtnSize;
	POINT		m_ptGaugeIndent;

	POINT		m_ptSliderGauge;
	POINT		m_ptSliderGaugeSize;
	POINT		m_ptSliderBackground;
	POINT		m_ptSliderBackgroundSize;

	int			m_iSelBtn;

private:
	CSliderBase* m_SliderBase;

private:
	SaveData	m_SliderBgData;
	SaveData	m_SliderGaugeData;

private:
	bool	m_bChange;
	HWND	m_ParenthWnd;
	HWND	m_PicturehWnd;

public:
	void	Initialize();
	void	CreateAsset();
	void	SetSliderProperty();

public:
	void	SetParenthWnd(HWND _hWnd){	m_ParenthWnd = _hWnd;	}
	void	SetPictureWnd(HWND _hWnd){	m_PicturehWnd = _hWnd;	}
	void	SetSliderBase(CSliderBase*	_base){ m_SliderBase = _base; }
	void	RefreshDlgBar(CSliderBase* _LoadBase, POINT& _normalTex);
	
public:
	CSliderDlgBar(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSliderDlgBar();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
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
	afx_msg void OnEnChangeEditSldGaugeLeft();
	afx_msg void OnEnChangeEditSldGaugeTop();
	afx_msg void OnEnChangeEditSldGaugeWidth();
	afx_msg void OnEnChangeEditSldGaugeHeight();
	afx_msg void OnEnChangeEditSldBgLeft();
	afx_msg void OnEnChangeEditSldBgTop();
	afx_msg void OnEnChangeEditSldBgWidth();
	afx_msg void OnEnChangeEditSldBgHeight();
	afx_msg void OnBnClickedRadioSldNmBtn();
	afx_msg void OnBnClickedRadioSldPrBtn();
	afx_msg void OnBnClickedRadioSldDaBtn();
	afx_msg void OnBnClickedRadioSldHlBtn();
	afx_msg void OnBnClickedRadioSldGaugebar();
	afx_msg void OnBnClickedRadioSldBg();
protected:
	afx_msg LRESULT OnSelchangeTexturelist(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChangeDragboxInfo(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnEnChangeEditSldStartposx();
	afx_msg void OnEnChangeEditSldStartposy();
};
