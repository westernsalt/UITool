// ScrollDlgBar.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "UITool.h"
#include "ScrollDlgBar.h"
#include "afxdialogex.h"

#include "ScrollBase.h"
#include "ObjMgr.h"
#include "DBMgr.h"


// CScrollDlgBar 대화 상자입니다.

IMPLEMENT_DYNAMIC(CScrollDlgBar, CDialogBar)

CScrollDlgBar::CScrollDlgBar(CWnd* pParent /*=NULL*/)
{
	m_ptNormalTex.x = 0;
	m_ptNormalTex.y = 0;

	m_ptPressedTex.x = 0;
	m_ptPressedTex.y = 0;

	m_ptDisableTex.x = 0;
	m_ptDisableTex.y = 0;

	m_ptHighlightTex.x = 0;
	m_ptHighlightTex.y = 0;

	m_ptBackground.x = 0;
	m_ptBackground.y = 0;
	m_ptBackgroundSize.x = 0;
	m_ptBackgroundSize.y = 0;

	m_ptBtnSize.x = 0;
	m_ptBtnSize.y = 0;
	m_iSelBtn = BTN_BACKGROUND;
	m_strTexPath = "";
	m_bChange = true;
	m_ScrollBase = NULL;
}

CScrollDlgBar::~CScrollDlgBar()
{
}

void CScrollDlgBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);

	m_TexturePathEdit		= (CEdit*)GetDlgItem(IDC_EDIT_SCR_SHOW_TEXTURE_PATH);
	m_NormalLeftPosEdit		= (CEdit*)GetDlgItem(IDC_EDIT_SCR_NM_LEFT);
	m_NormalTopPosEdit		= (CEdit*)GetDlgItem(IDC_EDIT_SCR_NM_TOP);
	m_PressedLeftPosEdit	= (CEdit*)GetDlgItem(IDC_EDIT_SCR_PR_LEFT);
	m_PressedTopPosEdit		= (CEdit*)GetDlgItem(IDC_EDIT_SCR_PR_TOP);
	m_HighlightLeftPosEdit	= (CEdit*)GetDlgItem(IDC_EDIT_SCR_HL_LEFT);
	m_HighlightTopPosEdit	= (CEdit*)GetDlgItem(IDC_EDIT_SCR_HL_TOP);
	m_DisableLeftPosEdit	= (CEdit*)GetDlgItem(IDC_EDIT_SCR_DA_LEFT);
	m_DisableTopPosEdit		= (CEdit*)GetDlgItem(IDC_EDIT_SCR_DA_TOP);

	m_BgLeftEdit		= (CEdit*)GetDlgItem(IDC_EDIT_SCR_BG_LEFT);
	m_BgTopEdit			= (CEdit*)GetDlgItem(IDC_EDIT_SCR_BG_TOP);
	m_BgWidthEdit		= (CEdit*)GetDlgItem(IDC_EDIT_SCR_BG_WIDTH);
	m_BgHeightEdit		= (CEdit*)GetDlgItem(IDC_EDIT_SCR_BG_HEIGHT);

	m_BtnWidthEdit		= (CEdit*)GetDlgItem(IDC_EDIT_SCR_BTN_WIDTH);
	m_BtnHeightEdit		= (CEdit*)GetDlgItem(IDC_EDIT_SCR_BTN_HEIGHT);
	m_NormalRadioBtn	= (CButton*)GetDlgItem(IDC_RADIO_SCR_NM_BTN);
	m_PressedRadioBtn	= (CButton*)GetDlgItem(IDC_RADIO_SCR_PR_BTN);
	m_DisableRadioBtn	= (CButton*)GetDlgItem(IDC_RADIO_SCR_DA_BTN);
	m_HighlightRadioBtn	= (CButton*)GetDlgItem(IDC_RADIO_SCR_HL_BTN);
	m_BackGroundRadioBtn= (CButton*)GetDlgItem(IDC_RADIO_SCR_BG);

	if(CObjMgr::GetInstance()->GetFocusAssetBase() == NULL)
	{
		m_ScrollBase = NULL;
	}
}

void CScrollDlgBar::CreateAsset()
{
	if((m_ScrollBase == NULL) && (m_strTexPath != ""))
	{
		m_bChange = false;
		UpdateData(true);

		m_ScrollBase = new CScrollBase;

		std::string strPath = "";

		//	현재 설정되어있는 에셋의 속성을 받아온다
		std::string strAtt =  "SCROLL";

		POINT ptInitPos;
		ptInitPos.x = 0;
		ptInitPos.y = 0;

		//	에셋의 포지션을 설정한다

		m_ScrollBase->SetFilePath(m_strTexPath);
		m_ScrollBase->SetPosition(ptInitPos);
		m_ScrollBase->SetAssetType(ASSET_SCROLL);
		m_ScrollBase->SetParenthWnd(m_ParenthWnd);
		//	에셋의 멤버 변수들을 셋팅해준다
		SetScrollProperty();
		//	만들어진 에셋을 Map에 저장하는 과정
		CObjMgr::GetInstance()->InsertAssetBase(m_ScrollBase);

		//	ObjMgr Render Call
		
		m_bChange = true;
	}
}

void CScrollDlgBar::Initialize()
{
	m_bChange = false;
	UpdateData(true);

	m_ptNormalTex.x = 0;
	m_ptNormalTex.y = 0;

	m_ptPressedTex.x = 0;
	m_ptPressedTex.y = 0;

	m_ptDisableTex.x = 0;
	m_ptDisableTex.y = 0;

	m_ptHighlightTex.x = 0;
	m_ptHighlightTex.y = 0;

	m_ptBackground.x = 0;
	m_ptBackground.y = 0;
	m_ptBackgroundSize.x = 0;
	m_ptBackgroundSize.y = 0;

	m_ptBtnSize.x = 0;
	m_ptBtnSize.y = 0;
	m_iSelBtn = BTN_BACKGROUND;
	m_strTexPath = "";
	m_ScrollBase = NULL;

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

	m_BgLeftEdit->SetWindowText(L"0");
	m_BgTopEdit->SetWindowText(L"0");
	m_BgWidthEdit->SetWindowText(L"0");
	m_BgHeightEdit->SetWindowText(L"0");

	m_NormalRadioBtn->SetCheck(0);
	m_PressedRadioBtn->SetCheck(0);
	m_HighlightRadioBtn->SetCheck(0);
	m_DisableRadioBtn->SetCheck(0);
	m_BackGroundRadioBtn->SetCheck(1);

	m_strTexPath = "";
	m_TexturePathEdit->SetWindowText(L"");
	m_bChange = true;
}

BEGIN_MESSAGE_MAP(CScrollDlgBar, CDialogBar)
	ON_MESSAGE(WM_SELCHANGE_TEXTURELIST, &CScrollDlgBar::OnSelchangeTexturelist)
	ON_MESSAGE(WM_CHANGE_DRAGBOX_INFO, &CScrollDlgBar::OnChangeDragboxInfo)

	ON_EN_CHANGE(IDC_EDIT_SCR_NM_LEFT, CScrollDlgBar::OnEnChangeEditSldNmLeft)
	ON_EN_CHANGE(IDC_EDIT_SCR_NM_TOP, CScrollDlgBar::OnEnChangeEditSldNmTop)
	ON_EN_CHANGE(IDC_EDIT_SCR_PR_LEFT, CScrollDlgBar::OnEnChangeEditSldPrLeft)
	ON_EN_CHANGE(IDC_EDIT_SCR_PR_TOP, CScrollDlgBar::OnEnChangeEditSldPrTop)
	ON_EN_CHANGE(IDC_EDIT_SCR_DA_LEFT, CScrollDlgBar::OnEnChangeEditSldDaLeft)
	ON_EN_CHANGE(IDC_EDIT_SCR_DA_TOP, CScrollDlgBar::OnEnChangeEditSldDaTop)
	ON_EN_CHANGE(IDC_EDIT_SCR_HL_LEFT, CScrollDlgBar::OnEnChangeEditSldHlLeft)
	ON_EN_CHANGE(IDC_EDIT_SCR_HL_TOP, CScrollDlgBar::OnEnChangeEditSldHlTop)
	ON_EN_CHANGE(IDC_EDIT_SCR_BTN_WIDTH, CScrollDlgBar::OnEnChangeEditSldBtnWidth)
	ON_EN_CHANGE(IDC_EDIT_SCR_BTN_HEIGHT, CScrollDlgBar::OnEnChangeEditSldBtnHeight)
	ON_EN_CHANGE(IDC_EDIT_SCR_BG_LEFT, CScrollDlgBar::OnEnChangeEditSldBgLeft)
	ON_EN_CHANGE(IDC_EDIT_SCR_BG_TOP, CScrollDlgBar::OnEnChangeEditSldBgTop)
	ON_EN_CHANGE(IDC_EDIT_SCR_BG_WIDTH, CScrollDlgBar::OnEnChangeEditSldBgWidth)
	ON_EN_CHANGE(IDC_EDIT_SCR_BG_HEIGHT, CScrollDlgBar::OnEnChangeEditSldBgHeight)
	ON_BN_CLICKED(IDC_RADIO_SCR_NM_BTN, CScrollDlgBar::OnBnClickedRadioSldNmBtn)
	ON_BN_CLICKED(IDC_RADIO_SCR_PR_BTN, CScrollDlgBar::OnBnClickedRadioSldPrBtn)
	ON_BN_CLICKED(IDC_RADIO_SCR_DA_BTN, CScrollDlgBar::OnBnClickedRadioSldDaBtn)
	ON_BN_CLICKED(IDC_RADIO_SCR_HL_BTN, CScrollDlgBar::OnBnClickedRadioSldHlBtn)
	ON_BN_CLICKED(IDC_RADIO_SCR_BG, CScrollDlgBar::OnBnClickedRadioSldBg)
END_MESSAGE_MAP()


// CScrollDlgBar 메시지 처리기입니다.

void CScrollDlgBar::OnEnChangeEditSldNmLeft()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptNormalTex.x = GetEditText(m_NormalLeftPosEdit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptNormalTex.x, (LPARAM)DRAGBOX_LEFT);
	}
	SetScrollProperty();
}


void CScrollDlgBar::OnEnChangeEditSldNmTop()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptNormalTex.y = GetEditText(m_NormalTopPosEdit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptNormalTex.y, (LPARAM)DRAGBOX_TOP);
	}
	SetScrollProperty();
}


void CScrollDlgBar::OnEnChangeEditSldPrLeft()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptPressedTex.x = GetEditText(m_PressedLeftPosEdit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptPressedTex.x, (LPARAM)DRAGBOX_LEFT);
	}
	SetScrollProperty();
}


void CScrollDlgBar::OnEnChangeEditSldPrTop()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptPressedTex.y = GetEditText(m_PressedTopPosEdit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptPressedTex.y, (LPARAM)DRAGBOX_TOP);
	}
	SetScrollProperty();
}


void CScrollDlgBar::OnEnChangeEditSldDaLeft()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptDisableTex.x = GetEditText(m_DisableLeftPosEdit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptDisableTex.x, (LPARAM)DRAGBOX_LEFT);
	}
	SetScrollProperty();
}


void CScrollDlgBar::OnEnChangeEditSldDaTop()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptDisableTex.y = GetEditText(m_DisableTopPosEdit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptDisableTex.y, (LPARAM)DRAGBOX_TOP);
	}
	SetScrollProperty();
}


void CScrollDlgBar::OnEnChangeEditSldHlLeft()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptHighlightTex.x = GetEditText(m_HighlightLeftPosEdit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptHighlightTex.x, (LPARAM)DRAGBOX_LEFT);
	}
	SetScrollProperty();
}


void CScrollDlgBar::OnEnChangeEditSldHlTop()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptHighlightTex.y = GetEditText(m_HighlightTopPosEdit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptHighlightTex.y, (LPARAM)DRAGBOX_TOP);
	}
	SetScrollProperty();
}


void CScrollDlgBar::OnEnChangeEditSldBtnWidth()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptBtnSize.x = GetEditText(m_BtnWidthEdit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.x, (LPARAM)DRAGBOX_WIDTH);
	}
	SetScrollProperty();
}


void CScrollDlgBar::OnEnChangeEditSldBtnHeight()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptBtnSize.y = GetEditText(m_BtnHeightEdit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.y, (LPARAM)DRAGBOX_HEIGHT);
	}
	SetScrollProperty();
}



void CScrollDlgBar::OnEnChangeEditSldBgLeft()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptBackground.x = GetEditText(m_BgLeftEdit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBackground.x, (LPARAM)DRAGBOX_LEFT);
	}
	SetScrollProperty();
}

void CScrollDlgBar::OnEnChangeEditSldBgTop()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptBackground.y = GetEditText(m_BgTopEdit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBackground.y, (LPARAM)DRAGBOX_TOP);
	}
	SetScrollProperty();
}


void CScrollDlgBar::OnEnChangeEditSldBgWidth()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptBackgroundSize.x = GetEditText(m_BgWidthEdit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBackgroundSize.x, (LPARAM)DRAGBOX_WIDTH);
	}
	SetScrollProperty();
}


void CScrollDlgBar::OnEnChangeEditSldBgHeight()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptBackgroundSize.y = GetEditText(m_BgHeightEdit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBackgroundSize.y, (LPARAM)DRAGBOX_HEIGHT);
	}
	SetScrollProperty();
}


void CScrollDlgBar::OnBnClickedRadioSldNmBtn()
{
	m_iSelBtn = BTN_NORMAL;
	::SendMessage(m_PicturehWnd, WM_CHANGE_RADIOBTN, (WPARAM)m_ptNormalTex.x, (LPARAM)m_ptNormalTex.y);
	::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.x, (LPARAM)DRAGBOX_WIDTH);
	::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.y, (LPARAM)DRAGBOX_HEIGHT);
}


void CScrollDlgBar::OnBnClickedRadioSldPrBtn()
{
	m_iSelBtn = BTN_PRESSED;
	::SendMessage(m_PicturehWnd, WM_CHANGE_RADIOBTN, (WPARAM)m_ptPressedTex.x, (LPARAM)m_ptPressedTex.y);
	::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.x, (LPARAM)DRAGBOX_WIDTH);
	::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.y, (LPARAM)DRAGBOX_HEIGHT);
}


void CScrollDlgBar::OnBnClickedRadioSldDaBtn()
{
	m_iSelBtn = BTN_DISABLE;
	::SendMessage(m_PicturehWnd, WM_CHANGE_RADIOBTN, (WPARAM)m_ptDisableTex.x, (LPARAM)m_ptDisableTex.y);
	::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.x, (LPARAM)DRAGBOX_WIDTH);
	::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.y, (LPARAM)DRAGBOX_HEIGHT);
}


void CScrollDlgBar::OnBnClickedRadioSldHlBtn()
{
	m_iSelBtn = BTN_HIGHLIGHT;
	::SendMessage(m_PicturehWnd, WM_CHANGE_RADIOBTN, (WPARAM)m_ptHighlightTex.x, (LPARAM)m_ptHighlightTex.y);
	::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.x, (LPARAM)DRAGBOX_WIDTH);
	::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.y, (LPARAM)DRAGBOX_HEIGHT);
}


void CScrollDlgBar::OnBnClickedRadioSldBg()
{
	m_iSelBtn = BTN_BACKGROUND;
	::SendMessage(m_PicturehWnd, WM_CHANGE_RADIOBTN, (WPARAM)m_ptBackground.x, (LPARAM)m_ptBackground.y);
	::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBackgroundSize.x, (LPARAM)DRAGBOX_WIDTH);
	::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBackgroundSize.y, (LPARAM)DRAGBOX_HEIGHT);
}




afx_msg LRESULT CScrollDlgBar::OnSelchangeTexturelist(WPARAM wParam, LPARAM lParam)
{
	UpdateData(true);

	m_strTexPath = (CDBMgr::GetInstance()->GetPathMap()->find((int)wParam)->second);
	CA2T wt(m_strTexPath.c_str());
	m_TexturePathEdit->SetWindowText(wt);

	if(m_ScrollBase != NULL)
	{
		m_ScrollBase->SetFilePath(m_strTexPath);
		
	}
	return 0;
}


afx_msg LRESULT CScrollDlgBar::OnChangeDragboxInfo(WPARAM wParam, LPARAM lParam)
{
	//	스크롤의 속성을 변경시킨다.
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
			m_ptBackground.x = (int)wParam;
			SetEditWndText(m_BgLeftEdit, m_ptBackground.x);
			break;
		case DRAGBOX_TOP:
			m_ptBackground.y = (int)wParam;
			SetEditWndText(m_BgTopEdit, m_ptBackground.y);
			break;
		}
		break;
	}


	switch((int)lParam)
	{
	case DRAGBOX_WIDTH:
		if(m_iSelBtn == BTN_BACKGROUND)
		{
			m_ptBackgroundSize.x = (int)wParam;
			SetEditWndText(m_BgWidthEdit, m_ptBackgroundSize.x);
		}
		else
		{
			m_ptBtnSize.x = (int)wParam;
			SetEditWndText(m_BtnWidthEdit, m_ptBtnSize.x);
		}
		break;
	case DRAGBOX_HEIGHT:
		if(m_iSelBtn == BTN_BACKGROUND)
		{
			m_ptBackgroundSize.y = (int)wParam;
			SetEditWndText(m_BgHeightEdit, m_ptBackgroundSize.y);
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

void CScrollDlgBar::SetScrollProperty()
{
	if(m_ScrollBase == NULL && m_strTexPath != "")
	{
		CreateAsset();
	}

	if(m_ScrollBase != NULL)
	{
		m_BgData.m_szFilePath = m_strTexPath;
		m_BgData.m_tStartPos.x = m_ptBackground.x;
		m_BgData.m_tStartPos.y = m_ptBackground.y;

		//	사이즈가 영일때 만들어져서 문제가 생긴다.
		m_ScrollBase->SetBackGround(m_BgData, m_ptBackgroundSize);
		m_ScrollBase->SetBtn(m_ptNormalTex, m_ptPressedTex, m_ptDisableTex, m_ptHighlightTex, m_ptBtnSize);
		m_ScrollBase->SetScrollProperty();
		
	}
}

void CScrollDlgBar::RefreshDlgBar( CScrollBase* _LoadBase, POINT& _StartTex )
{
	m_bChange = false;

	m_ptNormalTex.x = _LoadBase->GetSliderBtnBase()->GetNormalPos().x;
	EditTextSetting(m_ptNormalTex.x, m_NormalLeftPosEdit);
	m_ptNormalTex.y  = _LoadBase->GetSliderBtnBase()->GetNormalPos().y;
	EditTextSetting(m_ptNormalTex.y, m_NormalTopPosEdit);

	m_ptPressedTex.x = _LoadBase->GetSliderBtnBase()->GetPressedPos().x;
	EditTextSetting(m_ptPressedTex.x, m_PressedLeftPosEdit);
	m_ptPressedTex.y  = _LoadBase->GetSliderBtnBase()->GetPressedPos().y;
	EditTextSetting(m_ptPressedTex.y, m_PressedTopPosEdit);

	m_ptDisableTex.x = _LoadBase->GetSliderBtnBase()->GetDisablePos().x;
	EditTextSetting(m_ptDisableTex.x, m_DisableLeftPosEdit);
	m_ptDisableTex.y  = _LoadBase->GetSliderBtnBase()->GetDisablePos().y;
	EditTextSetting(m_ptDisableTex.y, m_DisableTopPosEdit);

	m_ptHighlightTex.x = _LoadBase->GetSliderBtnBase()->GetHighlightPos().x;
	EditTextSetting(m_ptHighlightTex.x, m_HighlightLeftPosEdit);
	m_ptHighlightTex.y = _LoadBase->GetSliderBtnBase()->GetHighlightPos().y;
	EditTextSetting(m_ptHighlightTex.y, m_HighlightTopPosEdit);

	m_ptBtnSize.x = _LoadBase->GetSliderBtnBase()->GetFileSize().right;
	EditTextSetting(m_ptBtnSize.x, m_BtnWidthEdit);
	m_ptBtnSize.y = _LoadBase->GetSliderBtnBase()->GetFileSize().bottom;
	EditTextSetting(m_ptBtnSize.y, m_BtnHeightEdit);

	m_ptBackgroundSize.x = _LoadBase->GetBackgroundBar()->GetFileSize().right;
	EditTextSetting(m_ptBackgroundSize.x, m_BgWidthEdit);
	m_ptBackgroundSize.y = _LoadBase->GetBackgroundBar()->GetFileSize().bottom;
	EditTextSetting(m_ptBackgroundSize.y, m_BgHeightEdit);

	m_ptBackground.x = _LoadBase->GetBackgroundBar()->GetStartPos().x;
	EditTextSetting(m_ptBackground.x, m_BgLeftEdit);
	m_ptBackground.y  = _LoadBase->GetBackgroundBar()->GetStartPos().y;
	EditTextSetting(m_ptBackground.y, m_BgTopEdit);
	_StartTex = m_ptBackground;

	//	스크롤베이스를 다이알로그바에 셋팅
	SetScrollBase(_LoadBase);

	m_strTexPath = _LoadBase->GetFilePath();
	CString TexPath(m_strTexPath.c_str());
	m_TexturePathEdit->SetWindowText(TexPath);

	m_bChange = true;
}
