#pragma once
#include "afxwin.h"


// CProgressbarDlgBar 대화 상자입니다.

class CProgressbarDlgBar : public CDialogBar
{
	DECLARE_DYNAMIC(CProgressbarDlgBar)

private:
	CEdit* m_TexturePathEdit;
	CEdit* m_BackgroundLeftEdit;
	CEdit* m_BackgroundTopEdit;
	CEdit* m_BackgroundWidthEdit;
	CEdit* m_BackgroundHeightEdit;

	CEdit* m_GaugeLeftEdit;
	CEdit* m_GaugeTopEdit;
	CEdit* m_GaugeWidthEdit;
	CEdit* m_GaugeHeightEdit;

	CEdit* m_ExtraGaugeLeftEdit;
	CEdit* m_ExtraGaugeTopEdit;
	CEdit* m_ExtraGaugeWidthEdit;
	CEdit* m_ExtraGaugeHeightEdit;

	CEdit* m_GaugePercentageEdit;
	CEdit* m_ExtraGaugePercentageEdit;
	
	CEdit* m_GaugeIndentXEdit;
	CEdit* m_GaugeIndentYEdit;

	CButton*	m_BackgroundRadioBtn;
	CButton*	m_GaugeRadioBtn;
	CButton*	m_ExtraGaugeRadioBtn;

private:
	CProgressBarBase* m_ProgressbarBase;

	// 복수의 프로그래스바 생성을 위한 게이지 설정
private:
	CButton*	m_DeleteGaugeBtn;
	CListBox*	m_GaugeBarList;
	int			m_iSelectGageBarIndex;

private:
	int			m_iSelBtn;
	bool		m_bChange;
	std::string m_strTexPath;

private:
	POINT		m_ptBackgroundStartPos;
	POINT		m_ptBackgroundSize;

	POINT		m_ptGaugeStartPos;
	POINT		m_ptGaugeSize;
	POINT		m_ptGaugeIndent;
	POINT		m_ptExtraGaugeStartPos;
	POINT		m_ptExtraGaugeSize;

	int			m_iGauge;
	int			m_iExtraGauge;

private:
	HWND	m_ParenthWnd;
	HWND	m_PicturehWnd;

public:
	void	Initialize();
	void	CreateAsset();
	void	SetProgressbarProperty();
	void	RefreshDlgBar(CProgressBarBase* _LoadBase, POINT& _normalTex);
	void	RefreshGaugeBarList();

public:
	void	SetParenthWnd(HWND _hWnd){	m_ParenthWnd = _hWnd;	}
	void	SetPictureWnd(HWND _hWnd){	m_PicturehWnd = _hWnd;	}
	void	SetProgressBarBase(CProgressBarBase* _base) { m_ProgressbarBase = _base; }

public:
	CProgressbarDlgBar(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CProgressbarDlgBar();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnUpdateCmdUI(CCmdUI* pCmdUI);
	afx_msg void OnEnChangeEditPrgBgLeft();
	afx_msg void OnEnChangeEditPrgBgTop();
	afx_msg void OnEnChangeEditPrgBgWidth();
	afx_msg void OnEnChangeEditPrgBgHeight();
	afx_msg void OnEnChangeEditPrgGaugeLeft();
	afx_msg void OnEnChangeEditPrgGaugeTop();
	afx_msg void OnEnChangeEditPrgGaugeWidth();
	afx_msg void OnEnChangeEditPrgGaugeHeight();
	afx_msg void OnEnChangeProgressBarPreview();
	afx_msg void OnBnClickedRadioPrgGaugebar();
	afx_msg void OnBnClickedRadioPrgBg();
	afx_msg void OnEnChangeEditPrgShowTexturePath();

protected:
	afx_msg LRESULT OnSelchangeTexturelist(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChangeDragboxInfo(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnEnChangeEditPrgGaugeStartposx();
	afx_msg void OnEnChangeEditPrgGaugeStartposy();
	afx_msg void OnLbnSelchangeListMultipleGauge();
	afx_msg void OnBnClickedButtonGaugebarUp();
	afx_msg void OnBnClickedButtonGaugebarDown();
	afx_msg void OnBnClickedButtonDeleteGaugebar();
	afx_msg void OnBnClickedButtonAddGaugebar();
	afx_msg void OnEnChangeEditPrgExtragaugeLeft();
	afx_msg void OnEnChangeEditPrgExtragaugeTop();
	afx_msg void OnEnChangeEditPrgExtragaugeWidth();
	afx_msg void OnEnChangeEditPrgExtragaugeHeight();
	afx_msg void OnEnChangeEditPrgExtragaugePercentage();
	afx_msg void OnBnClickedRadioExtraGauge();
};
