// SlideDlgBar.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "UITool.h"
#include "SliderDlgBar.h"
#include "afxdialogex.h"
#include "ObjMgr.h"
#include "SliderBase.h"
#include "DBMgr.h"


// CSliderDlgBar 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSliderDlgBar, CDialogBar)

CSliderDlgBar::CSliderDlgBar(CWnd* pParent /*=NULL*/)
{
	m_ptNormalTex.x = 0;
	m_ptNormalTex.y = 0;

	m_ptPressedTex.x = 0;
	m_ptPressedTex.y = 0;

	m_ptDisableTex.x = 0;
	m_ptDisableTex.y = 0;

	m_ptHighlightTex.x = 0;
	m_ptHighlightTex.y = 0;

	m_ptSliderGauge.x = 0;
	m_ptSliderGauge.y = 0;
	m_ptSliderBackground.x = 0;
	m_ptSliderBackground.y = 0;
	m_ptSliderGaugeSize.x = 0;
	m_ptSliderGaugeSize.y = 0;
	m_ptSliderBackgroundSize.x = 0;
	m_ptSliderBackgroundSize.y = 0;

	m_ptGaugeIndent.x = 0;
	m_ptGaugeIndent.y = 0;

	m_ptBtnSize.x = 0;
	m_ptBtnSize.y = 0;
	m_iSelBtn = BTN_BACKGROUND;
	m_strTexPath = "";
	m_bChange = true;
	m_SliderBase = NULL;
}

CSliderDlgBar::~CSliderDlgBar()
{
}

void CSliderDlgBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);

	m_TexturePathEdit		= (CEdit*)GetDlgItem(IDC_EDIT_SLD_SHOW_TEXTURE_PATH);
	m_NormalLeftPosEdit		= (CEdit*)GetDlgItem(IDC_EDIT_SLD_NM_LEFT);
	m_NormalTopPosEdit		= (CEdit*)GetDlgItem(IDC_EDIT_SLD_NM_TOP);
	m_PressedLeftPosEdit	= (CEdit*)GetDlgItem(IDC_EDIT_SLD_PR_LEFT);
	m_PressedTopPosEdit		= (CEdit*)GetDlgItem(IDC_EDIT_SLD_PR_TOP);
	m_HighlightLeftPosEdit	= (CEdit*)GetDlgItem(IDC_EDIT_SLD_HL_LEFT);
	m_HighlightTopPosEdit	= (CEdit*)GetDlgItem(IDC_EDIT_SLD_HL_TOP);
	m_DisableLeftPosEdit	= (CEdit*)GetDlgItem(IDC_EDIT_SLD_DA_LEFT);
	m_DisableTopPosEdit		= (CEdit*)GetDlgItem(IDC_EDIT_SLD_DA_TOP);

	m_SliderGaugeLeftEdit	= (CEdit*)GetDlgItem(IDC_EDIT_SLD_GAUGE_LEFT);
	m_SliderGaugeTopEdit	= (CEdit*)GetDlgItem(IDC_EDIT_SLD_GAUGE_TOP);
	m_SliderGaugeWidthEdit	= (CEdit*)GetDlgItem(IDC_EDIT_SLD_GAUGE_WIDTH);
	m_SliderGaugeHeightEdit	= (CEdit*)GetDlgItem(IDC_EDIT_SLD_GAUGE_HEIGHT);

	m_SliderBgLeftEdit		= (CEdit*)GetDlgItem(IDC_EDIT_SLD_BG_LEFT);
	m_SliderBgTopEdit		= (CEdit*)GetDlgItem(IDC_EDIT_SLD_BG_TOP);
	m_SliderBgWidthEdit		= (CEdit*)GetDlgItem(IDC_EDIT_SLD_BG_WIDTH);
	m_SliderBgHeightEdit	= (CEdit*)GetDlgItem(IDC_EDIT_SLD_BG_HEIGHT);

	m_BtnWidthEdit		= (CEdit*)GetDlgItem(IDC_EDIT_SLD_BTN_WIDTH);
	m_BtnHeightEdit		= (CEdit*)GetDlgItem(IDC_EDIT_SLD_BTN_HEIGHT);
	m_NormalRadioBtn	= (CButton*)GetDlgItem(IDC_RADIO_SLD_NM_BTN);
	m_PressedRadioBtn	= (CButton*)GetDlgItem(IDC_RADIO_SLD_PR_BTN);
	m_DisableRadioBtn	= (CButton*)GetDlgItem(IDC_RADIO_SLD_DA_BTN);
	m_HighlightRadioBtn	= (CButton*)GetDlgItem(IDC_RADIO_SLD_HL_BTN);
	m_BackGroundRadioBtn= (CButton*)GetDlgItem(IDC_RADIO_SLD_BG);
	m_GaugeBarRadioBtn	= (CButton*)GetDlgItem(IDC_RADIO_SLD_GAUGEBAR);

	m_GaugeStartXPosEdit= (CEdit*)GetDlgItem(IDC_EDIT_SLD_STARTPOSX);
	m_GaugeStartYPosEdit= (CEdit*)GetDlgItem(IDC_EDIT_SLD_STARTPOSY);

	if(CObjMgr::GetInstance()->GetFocusAssetBase() == NULL)
	{
		m_SliderBase = NULL;
	}
}


BEGIN_MESSAGE_MAP(CSliderDlgBar, CDialogBar)
	ON_EN_CHANGE(IDC_EDIT_SLD_NM_LEFT, CSliderDlgBar::OnEnChangeEditSldNmLeft)
	ON_EN_CHANGE(IDC_EDIT_SLD_NM_TOP, CSliderDlgBar::OnEnChangeEditSldNmTop)
	ON_EN_CHANGE(IDC_EDIT_SLD_PR_LEFT, CSliderDlgBar::OnEnChangeEditSldPrLeft)
	ON_EN_CHANGE(IDC_EDIT_SLD_PR_TOP, CSliderDlgBar::OnEnChangeEditSldPrTop)
	ON_EN_CHANGE(IDC_EDIT_SLD_DA_LEFT, CSliderDlgBar::OnEnChangeEditSldDaLeft)
	ON_EN_CHANGE(IDC_EDIT_SLD_DA_TOP, CSliderDlgBar::OnEnChangeEditSldDaTop)
	ON_EN_CHANGE(IDC_EDIT_SLD_HL_LEFT, CSliderDlgBar::OnEnChangeEditSldHlLeft)
	ON_EN_CHANGE(IDC_EDIT_SLD_HL_TOP, CSliderDlgBar::OnEnChangeEditSldHlTop)
	ON_EN_CHANGE(IDC_EDIT_SLD_BTN_WIDTH, CSliderDlgBar::OnEnChangeEditSldBtnWidth)
	ON_EN_CHANGE(IDC_EDIT_SLD_BTN_HEIGHT, CSliderDlgBar::OnEnChangeEditSldBtnHeight)
	ON_EN_CHANGE(IDC_EDIT_SLD_GAUGE_LEFT, CSliderDlgBar::OnEnChangeEditSldGaugeLeft)
	ON_EN_CHANGE(IDC_EDIT_SLD_GAUGE_TOP, CSliderDlgBar::OnEnChangeEditSldGaugeTop)
	ON_EN_CHANGE(IDC_EDIT_SLD_GAUGE_WIDTH, CSliderDlgBar::OnEnChangeEditSldGaugeWidth)
	ON_EN_CHANGE(IDC_EDIT_SLD_GAUGE_HEIGHT, CSliderDlgBar::OnEnChangeEditSldGaugeHeight)
	ON_EN_CHANGE(IDC_EDIT_SLD_BG_LEFT, CSliderDlgBar::OnEnChangeEditSldBgLeft)
	ON_EN_CHANGE(IDC_EDIT_SLD_BG_TOP, CSliderDlgBar::OnEnChangeEditSldBgTop)
	ON_EN_CHANGE(IDC_EDIT_SLD_BG_WIDTH, CSliderDlgBar::OnEnChangeEditSldBgWidth)
	ON_EN_CHANGE(IDC_EDIT_SLD_BG_HEIGHT, CSliderDlgBar::OnEnChangeEditSldBgHeight)
	ON_BN_CLICKED(IDC_RADIO_SLD_NM_BTN, CSliderDlgBar::OnBnClickedRadioSldNmBtn)
	ON_BN_CLICKED(IDC_RADIO_SLD_PR_BTN, CSliderDlgBar::OnBnClickedRadioSldPrBtn)
	ON_BN_CLICKED(IDC_RADIO_SLD_DA_BTN, CSliderDlgBar::OnBnClickedRadioSldDaBtn)
	ON_BN_CLICKED(IDC_RADIO_SLD_HL_BTN, CSliderDlgBar::OnBnClickedRadioSldHlBtn)
	ON_BN_CLICKED(IDC_RADIO_SLD_GAUGEBAR, CSliderDlgBar::OnBnClickedRadioSldGaugebar)
	ON_BN_CLICKED(IDC_RADIO_SLD_BG, CSliderDlgBar::OnBnClickedRadioSldBg)
	ON_MESSAGE(WM_SELCHANGE_TEXTURELIST, CSliderDlgBar::OnSelchangeTexturelist)
	ON_MESSAGE(WM_CHANGE_DRAGBOX_INFO, CSliderDlgBar::OnChangeDragboxInfo)
	ON_EN_CHANGE(IDC_EDIT_SLD_STARTPOSX, &CSliderDlgBar::OnEnChangeEditSldStartposx)
	ON_EN_CHANGE(IDC_EDIT_SLD_STARTPOSY, &CSliderDlgBar::OnEnChangeEditSldStartposy)
END_MESSAGE_MAP()

void CSliderDlgBar::Initialize()
{
	m_bChange = false;
	UpdateData(true);

	m_strTexPath = "";
	m_TexturePathEdit->SetWindowText(L"");

	m_ptNormalTex.x = 0;
	m_ptNormalTex.y = 0;

	m_ptPressedTex.x = 0;
	m_ptPressedTex.y = 0;

	m_ptDisableTex.x = 0;
	m_ptDisableTex.y = 0;

	m_ptHighlightTex.x = 0;
	m_ptHighlightTex.y = 0;

	m_ptSliderGauge.x = 0;
	m_ptSliderGauge.y = 0;
	m_ptSliderBackground.x = 0;
	m_ptSliderBackground.y = 0;
	m_ptSliderGaugeSize.x = 0;
	m_ptSliderGaugeSize.y = 0;
	m_ptSliderBackgroundSize.x = 0;
	m_ptSliderBackgroundSize.y = 0;
	m_ptGaugeIndent.x = 0;
	m_ptGaugeIndent.y = 0;

	m_ptBtnSize.x = 0;
	m_ptBtnSize.y = 0;
	m_iSelBtn = BTN_BACKGROUND;
	m_strTexPath = "";
	m_bChange = true;
	m_SliderBase = NULL;

	m_NormalLeftPosEdit->SetWindowText(L"0");
	m_NormalTopPosEdit->SetWindowText(L"0");
	m_PressedLeftPosEdit->SetWindowText(L"0");
	m_PressedTopPosEdit->SetWindowText(L"0");
	m_HighlightLeftPosEdit->SetWindowText(L"0");
	m_HighlightTopPosEdit->SetWindowText(L"0");
	m_DisableLeftPosEdit->SetWindowText(L"0");
	m_DisableTopPosEdit->SetWindowText(L"0");
	m_DisableLeftPosEdit->SetWindowText(L"0");
	m_DisableTopPosEdit->SetWindowText(L"0");
	m_BtnWidthEdit->SetWindowText(L"0");
	m_BtnHeightEdit->SetWindowText(L"0");

	m_SliderBgLeftEdit->SetWindowText(L"0");
	m_SliderBgTopEdit->SetWindowText(L"0");
	m_SliderBgWidthEdit->SetWindowText(L"0");
	m_SliderBgHeightEdit->SetWindowText(L"0");

	m_SliderGaugeLeftEdit->SetWindowText(L"0");
	m_SliderGaugeTopEdit->SetWindowText(L"0");
	m_SliderGaugeWidthEdit->SetWindowText(L"0");
	m_SliderGaugeHeightEdit->SetWindowText(L"0");

	m_NormalRadioBtn->SetCheck(0);
	m_PressedRadioBtn->SetCheck(0);
	m_HighlightRadioBtn->SetCheck(0);
	m_DisableRadioBtn->SetCheck(0);
	m_GaugeBarRadioBtn->SetCheck(0);
	m_BackGroundRadioBtn->SetCheck(1);

	m_GaugeStartXPosEdit->SetWindowText(L"0");
	m_GaugeStartYPosEdit->SetWindowText(L"0");

	m_bChange = true;
}

void CSliderDlgBar::CreateAsset()
{
	if((m_SliderBase == NULL) && (m_strTexPath != ""))
	{
		m_bChange = false;
		UpdateData(true);

		m_SliderBase = new CSliderBase;

		std::string strPath = "";

		POINT ptInitPos;
		ptInitPos.x = 0;
		ptInitPos.y = 0;

		//	에셋의 속성을 설정한다
		m_SliderBase->SetFilePath(m_strTexPath);
		m_SliderBase->SetPosition(ptInitPos);
		m_SliderBase->SetAssetType(ASSET_SLIDER);
		m_SliderBase->SetParenthWnd(m_ParenthWnd);
		//	에셋의 멤버 변수들을 셋팅해준다
		SetSliderProperty();
		//	만들어진 에셋을 Map에 저장하는 과정
		CObjMgr::GetInstance()->InsertAssetBase(m_SliderBase);

		//	ObjMgr Render Call
		
		m_bChange = true;
	}
}

void CSliderDlgBar::SetSliderProperty()
{
	if(m_SliderBase == NULL && m_strTexPath != "")
	{
		CreateAsset();
	}

	if(m_SliderBase != NULL)
	{
		m_SliderBgData.m_szFilePath = m_strTexPath;
		m_SliderBgData.m_tStartPos.x = m_ptSliderBackground.x;
		m_SliderBgData.m_tStartPos.y = m_ptSliderBackground.y;

		m_SliderGaugeData.m_szFilePath = m_strTexPath;
		m_SliderGaugeData.m_tStartPos.x = m_ptSliderGauge.x;
		m_SliderGaugeData.m_tStartPos.y = m_ptSliderGauge.y;

		m_SliderBase->SetSliderGauge(m_SliderGaugeData, m_ptSliderGaugeSize, m_ptGaugeIndent);
		m_SliderBase->SetBackGround(m_SliderBgData, m_ptSliderBackgroundSize);
		m_SliderBase->SetBtn(m_ptNormalTex, m_ptPressedTex, m_ptDisableTex, m_ptHighlightTex, m_ptBtnSize);
		m_SliderBase->SetSliderProperty();
	}
}

// CSliderDlgBar 메시지 처리기입니다.

void CSliderDlgBar::OnEnChangeEditSldNmLeft()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptNormalTex.x = GetEditText(m_NormalLeftPosEdit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptNormalTex.x, (LPARAM)DRAGBOX_LEFT);
	}
	SetSliderProperty();
}


void CSliderDlgBar::OnEnChangeEditSldNmTop()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptNormalTex.y = GetEditText(m_NormalTopPosEdit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptNormalTex.y, (LPARAM)DRAGBOX_TOP);
	}
	SetSliderProperty();
}


void CSliderDlgBar::OnEnChangeEditSldPrLeft()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptPressedTex.x = GetEditText(m_PressedLeftPosEdit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptPressedTex.x, (LPARAM)DRAGBOX_LEFT);
	}
	SetSliderProperty();
}


void CSliderDlgBar::OnEnChangeEditSldPrTop()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptPressedTex.y = GetEditText(m_PressedTopPosEdit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptPressedTex.y, (LPARAM)DRAGBOX_TOP);
	}
	SetSliderProperty();
}


void CSliderDlgBar::OnEnChangeEditSldDaLeft()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptDisableTex.x = GetEditText(m_DisableLeftPosEdit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptDisableTex.x, (LPARAM)DRAGBOX_LEFT);
	}
	SetSliderProperty();
}


void CSliderDlgBar::OnEnChangeEditSldDaTop()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptDisableTex.y = GetEditText(m_DisableTopPosEdit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptDisableTex.y, (LPARAM)DRAGBOX_TOP);
	}
	SetSliderProperty();
}


void CSliderDlgBar::OnEnChangeEditSldHlLeft()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptHighlightTex.x = GetEditText(m_HighlightLeftPosEdit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptHighlightTex.x, (LPARAM)DRAGBOX_LEFT);
	}
	SetSliderProperty();
}


void CSliderDlgBar::OnEnChangeEditSldHlTop()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptHighlightTex.y = GetEditText(m_HighlightTopPosEdit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptHighlightTex.y, (LPARAM)DRAGBOX_TOP);
	}
	SetSliderProperty();
}


void CSliderDlgBar::OnEnChangeEditSldBtnWidth()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptBtnSize.x = GetEditText(m_BtnWidthEdit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.x, (LPARAM)DRAGBOX_WIDTH);
	}
	SetSliderProperty();
}


void CSliderDlgBar::OnEnChangeEditSldBtnHeight()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptBtnSize.y = GetEditText(m_BtnHeightEdit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.y, (LPARAM)DRAGBOX_HEIGHT);
	}
	SetSliderProperty();
}


void CSliderDlgBar::OnEnChangeEditSldGaugeLeft()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptSliderGauge.x = GetEditText(m_SliderGaugeLeftEdit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptSliderGauge.x, (LPARAM)DRAGBOX_LEFT);
	}
	SetSliderProperty();
}


void CSliderDlgBar::OnEnChangeEditSldGaugeTop()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptSliderGauge.y = GetEditText(m_SliderGaugeTopEdit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptSliderGauge.y, (LPARAM)DRAGBOX_TOP);
	}
	SetSliderProperty();
}


void CSliderDlgBar::OnEnChangeEditSldGaugeWidth()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptSliderGaugeSize.x = GetEditText(m_SliderGaugeWidthEdit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptSliderGaugeSize.x, (LPARAM)DRAGBOX_WIDTH);
	}
	SetSliderProperty();
}


void CSliderDlgBar::OnEnChangeEditSldGaugeHeight()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptSliderGaugeSize.y = GetEditText(m_SliderGaugeHeightEdit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptSliderGaugeSize.y, (LPARAM)DRAGBOX_HEIGHT);
	}
	SetSliderProperty();
}


void CSliderDlgBar::OnEnChangeEditSldBgLeft()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptSliderBackground.x = GetEditText(m_SliderBgLeftEdit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptSliderBackground.x, (LPARAM)DRAGBOX_LEFT);
	}
	SetSliderProperty();
}

void CSliderDlgBar::OnEnChangeEditSldBgTop()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptSliderBackground.y = GetEditText(m_SliderBgTopEdit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptSliderBackground.y, (LPARAM)DRAGBOX_TOP);
	}
	SetSliderProperty();
}


void CSliderDlgBar::OnEnChangeEditSldBgWidth()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptSliderBackgroundSize.x = GetEditText(m_SliderBgWidthEdit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptSliderBackgroundSize.x, (LPARAM)DRAGBOX_WIDTH);
	}
	SetSliderProperty();
}


void CSliderDlgBar::OnEnChangeEditSldBgHeight()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptSliderBackgroundSize.y = GetEditText(m_SliderBgHeightEdit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptSliderBackgroundSize.y, (LPARAM)DRAGBOX_HEIGHT);
	}
	SetSliderProperty();
}


void CSliderDlgBar::OnBnClickedRadioSldNmBtn()
{
	m_iSelBtn = BTN_NORMAL;
	::SendMessage(m_PicturehWnd, WM_CHANGE_RADIOBTN, (WPARAM)m_ptNormalTex.x, (LPARAM)m_ptNormalTex.y);
	::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.x, (LPARAM)DRAGBOX_WIDTH);
	::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.y, (LPARAM)DRAGBOX_HEIGHT);
}


void CSliderDlgBar::OnBnClickedRadioSldPrBtn()
{
	m_iSelBtn = BTN_PRESSED;
	::SendMessage(m_PicturehWnd, WM_CHANGE_RADIOBTN, (WPARAM)m_ptPressedTex.x, (LPARAM)m_ptPressedTex.y);
	::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.x, (LPARAM)DRAGBOX_WIDTH);
	::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.y, (LPARAM)DRAGBOX_HEIGHT);
}


void CSliderDlgBar::OnBnClickedRadioSldDaBtn()
{
	m_iSelBtn = BTN_DISABLE;
	::SendMessage(m_PicturehWnd, WM_CHANGE_RADIOBTN, (WPARAM)m_ptDisableTex.x, (LPARAM)m_ptDisableTex.y);
	::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.x, (LPARAM)DRAGBOX_WIDTH);
	::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.y, (LPARAM)DRAGBOX_HEIGHT);
}


void CSliderDlgBar::OnBnClickedRadioSldHlBtn()
{
	m_iSelBtn = BTN_HIGHLIGHT;
	::SendMessage(m_PicturehWnd, WM_CHANGE_RADIOBTN, (WPARAM)m_ptHighlightTex.x, (LPARAM)m_ptHighlightTex.y);
	::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.x, (LPARAM)DRAGBOX_WIDTH);
	::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.y, (LPARAM)DRAGBOX_HEIGHT);
}


void CSliderDlgBar::OnBnClickedRadioSldGaugebar()
{
	m_iSelBtn = BTN_GAUGEBAR;
	::SendMessage(m_PicturehWnd, WM_CHANGE_RADIOBTN, (WPARAM)m_ptSliderGauge.x, (LPARAM)m_ptSliderGauge.y);
	::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptSliderGaugeSize.x, (LPARAM)DRAGBOX_WIDTH);
	::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptSliderGaugeSize.y, (LPARAM)DRAGBOX_HEIGHT);
}


void CSliderDlgBar::OnBnClickedRadioSldBg()
{
	m_iSelBtn = BTN_BACKGROUND;
	::SendMessage(m_PicturehWnd, WM_CHANGE_RADIOBTN, (WPARAM)m_ptSliderBackground.x, (LPARAM)m_ptSliderBackground.y);
	::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptSliderBackgroundSize.x, (LPARAM)DRAGBOX_WIDTH);
	::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptSliderBackgroundSize.y, (LPARAM)DRAGBOX_HEIGHT);
}

afx_msg LRESULT CSliderDlgBar::OnSelchangeTexturelist(WPARAM wParam, LPARAM lParam)
{
	UpdateData(true);

	m_strTexPath = (CDBMgr::GetInstance()->GetPathMap()->find((int)wParam)->second);
	CA2T wt(m_strTexPath.c_str());
	m_TexturePathEdit->SetWindowText(wt);

	if(m_SliderBase != NULL)
	{
		m_SliderBase->SetFilePath(m_strTexPath);
		
	}
	return 0;
}


afx_msg LRESULT CSliderDlgBar::OnChangeDragboxInfo(WPARAM wParam, LPARAM lParam)
{
	//	슬라이더의 속성을 변경시킨다.
	//	크리티컬 세션처럼 쓴다.
	m_bChange = false;

	UpdateData(true);

	switch(m_iSelBtn)
	{
	case BTN_NORMAL:
		switch((int)lParam)
		{
		case DRAGBOX_LEFT:
			m_ptNormalTex.x = (int)wParam;
			SetEditWndText(m_NormalLeftPosEdit, m_ptNormalTex.x);
			break;
		case DRAGBOX_TOP:
			m_ptNormalTex.y = (int)wParam;
			SetEditWndText(m_NormalTopPosEdit, m_ptNormalTex.y);
			break;
		}
		break;
	case BTN_DISABLE:
		switch((int)lParam)
		{
		case DRAGBOX_LEFT:
			m_ptDisableTex.x = (int)wParam;
			SetEditWndText(m_DisableLeftPosEdit, m_ptDisableTex.x);
			break;
		case DRAGBOX_TOP:
			m_ptDisableTex.y = (int)wParam;
			SetEditWndText(m_DisableTopPosEdit, m_ptDisableTex.y);
			break;
		}
		break;
	case BTN_HIGHLIGHT:
		switch((int)lParam)
		{
		case DRAGBOX_LEFT:
			m_ptHighlightTex.x = (int)wParam;
			SetEditWndText(m_HighlightLeftPosEdit, m_ptHighlightTex.x);
			break;
		case DRAGBOX_TOP:
			m_ptHighlightTex.y = (int)wParam;
			SetEditWndText(m_HighlightTopPosEdit, m_ptHighlightTex.y);
			break;
		}
		break;
	case BTN_PRESSED:
		switch((int)lParam)
		{
		case DRAGBOX_LEFT:
			m_ptPressedTex.x = (int)wParam;
			SetEditWndText(m_PressedLeftPosEdit, m_ptPressedTex.x);
			break;
		case DRAGBOX_TOP:
			m_ptPressedTex.y = (int)wParam;
			SetEditWndText(m_PressedTopPosEdit, m_ptPressedTex.y);
			break;
		}
		break;
	case BTN_BACKGROUND:
		switch((int)lParam)
		{
		case DRAGBOX_LEFT:
			m_ptSliderBackground.x = (int)wParam;
			SetEditWndText(m_SliderBgLeftEdit, m_ptSliderBackground.x);
			break;
		case DRAGBOX_TOP:
			m_ptSliderBackground.y = (int)wParam;
			SetEditWndText(m_SliderBgTopEdit, m_ptSliderBackground.y);
			break;
		}
		break;
	case BTN_GAUGEBAR:
		switch((int)lParam)
		{
		case DRAGBOX_LEFT:
			m_ptSliderGauge.x = (int)wParam;
			SetEditWndText(m_SliderGaugeLeftEdit, m_ptSliderGauge.x);
			break;
		case DRAGBOX_TOP:
			m_ptSliderGauge.y = (int)wParam;
			SetEditWndText(m_SliderGaugeTopEdit, m_ptSliderGauge.y);
			break;
		}
		break;
	}


	switch((int)lParam)
	{
	case DRAGBOX_WIDTH:
		if(m_iSelBtn == BTN_GAUGEBAR)
		{
			m_ptSliderGaugeSize.x = (int)wParam;
			SetEditWndText(m_SliderGaugeWidthEdit, m_ptSliderGaugeSize.x);
		}
		else if(m_iSelBtn == BTN_BACKGROUND)
		{
			m_ptSliderBackgroundSize.x = (int)wParam;
			SetEditWndText(m_SliderBgWidthEdit, m_ptSliderBackgroundSize.x);
		}
		else
		{
			m_ptBtnSize.x = (int)wParam;
			SetEditWndText(m_BtnWidthEdit, m_ptBtnSize.x);
		}
		break;
	case DRAGBOX_HEIGHT:
		if(m_iSelBtn == BTN_GAUGEBAR)
		{
			m_ptSliderGaugeSize.y = (int)wParam;
			SetEditWndText(m_SliderGaugeHeightEdit, m_ptSliderGaugeSize.y);
		}
		else if(m_iSelBtn == BTN_BACKGROUND)
		{
			m_ptSliderBackgroundSize.y = (int)wParam;
			SetEditWndText(m_SliderBgHeightEdit, m_ptSliderBackgroundSize.y);
		}
		else
		{
			m_ptBtnSize.y = (int)wParam;
			SetEditWndText(m_BtnHeightEdit, m_ptBtnSize.y);
		}
		break;
	}

	m_bChange = true;

	return 0;
}

void CSliderDlgBar::RefreshDlgBar( CSliderBase* _LoadBase, POINT& _StartTex )
{
	m_bChange = false;

	m_ptNormalTex.x = _LoadBase->GetSliderBtnBase()->GetNormalPos().x;
	EditTextSetting(m_ptNormalTex.x, m_NormalLeftPosEdit);
	m_ptNormalTex.y = _LoadBase->GetSliderBtnBase()->GetNormalPos().y;
	EditTextSetting(m_ptNormalTex.y, m_NormalTopPosEdit);

	m_ptPressedTex.x = _LoadBase->GetSliderBtnBase()->GetPressedPos().x;
	EditTextSetting(m_ptPressedTex.x, m_PressedLeftPosEdit);
	m_ptPressedTex.y = _LoadBase->GetSliderBtnBase()->GetPressedPos().y;
	EditTextSetting(m_ptPressedTex.y, m_PressedTopPosEdit);

	m_ptDisableTex.x = _LoadBase->GetSliderBtnBase()->GetDisablePos().x;
	EditTextSetting(m_ptDisableTex.x, m_DisableLeftPosEdit);
	m_ptDisableTex.y = _LoadBase->GetSliderBtnBase()->GetDisablePos().y;
	EditTextSetting(m_ptDisableTex.y, m_DisableTopPosEdit);

	m_ptHighlightTex.x = _LoadBase->GetSliderBtnBase()->GetHighlightPos().x;
	EditTextSetting(m_ptHighlightTex.x, m_HighlightLeftPosEdit);
	m_ptHighlightTex.y = _LoadBase->GetSliderBtnBase()->GetHighlightPos().y;
	EditTextSetting(m_ptHighlightTex.y, m_HighlightTopPosEdit);

	m_ptBtnSize.x = _LoadBase->GetSliderBtnBase()->GetFileSize().right;
	EditTextSetting(m_ptBtnSize.x, m_BtnWidthEdit);
	m_ptBtnSize.y = _LoadBase->GetSliderBtnBase()->GetFileSize().bottom;
	EditTextSetting(m_ptBtnSize.y, m_BtnHeightEdit);

	m_ptSliderGaugeSize.x = _LoadBase->GetGaugeBar()->GetFileSize().right;
	EditTextSetting(m_ptSliderGaugeSize.x, m_SliderGaugeWidthEdit);
	m_ptSliderGaugeSize.y = _LoadBase->GetGaugeBar()->GetFileSize().bottom;
	EditTextSetting(m_ptSliderGaugeSize.y, m_SliderGaugeHeightEdit);

	m_ptSliderGauge.x = _LoadBase->GetGaugeBar()->GetStartPos().x;
	EditTextSetting(m_ptSliderGauge.x, m_SliderGaugeLeftEdit);
	m_ptSliderGauge.y = _LoadBase->GetGaugeBar()->GetStartPos().y;
	EditTextSetting(m_ptSliderGauge.y, m_SliderGaugeTopEdit);

	m_ptSliderBackground.x = _LoadBase->GetBackgroundBar()->GetStartPos().x;
	EditTextSetting(m_ptSliderBackground.x, m_SliderBgLeftEdit);
	m_ptSliderBackground.y = _LoadBase->GetBackgroundBar()->GetStartPos().y;
	EditTextSetting(m_ptSliderBackground.y, m_SliderBgTopEdit);
	m_ptSliderBackgroundSize.x = _LoadBase->GetBackgroundBar()->GetFileSize().right;
	EditTextSetting(m_ptSliderBackgroundSize.x, m_SliderBgWidthEdit);
	m_ptSliderBackgroundSize.y = _LoadBase->GetBackgroundBar()->GetFileSize().bottom;
	EditTextSetting(m_ptSliderBackgroundSize.y, m_SliderBgHeightEdit);

	m_ptGaugeIndent.x = _LoadBase->GetGaugeStartPos().x;
	EditTextSetting(m_ptGaugeIndent.x, m_GaugeStartXPosEdit);
	m_ptGaugeIndent.y = _LoadBase->GetGaugeStartPos().y;
	EditTextSetting(m_ptGaugeIndent.y, m_GaugeStartYPosEdit);

	_StartTex = m_ptSliderBackground;

	//	슬라이더베이스를 다이알로그바에 셋팅
	SetSliderBase(_LoadBase);

	m_strTexPath = _LoadBase->GetFilePath();
	CString TexPath(m_strTexPath.c_str());
	m_TexturePathEdit->SetWindowText(TexPath);

	m_bChange = true;
}


void CSliderDlgBar::OnEnChangeEditSldStartposx()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptGaugeIndent.x = GetEditText(m_GaugeStartXPosEdit);
	}
	SetSliderProperty();
}


void CSliderDlgBar::OnEnChangeEditSldStartposy()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptGaugeIndent.y = GetEditText(m_GaugeStartYPosEdit);
	}
	SetSliderProperty();
}
