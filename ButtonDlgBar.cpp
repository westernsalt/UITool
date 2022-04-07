// ButtonDlgBar.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "UITool.h"
#include "ButtonDlgBar.h"
#include "AssetBase.h"
#include "ObjMgr.h"
#include "BtnBase.h"
#include "DBMgr.h"
#include "TextBoxDlgBar.h"


// CButtonDlgBar

IMPLEMENT_DYNAMIC(CButtonDlgBar, CDialogBar)

CButtonDlgBar::CButtonDlgBar()
{
	m_ptNormalTex.x = 0;
	m_ptNormalTex.y = 0;

	m_ptPressedTex.x = 0;
	m_ptPressedTex.y = 0;

	m_ptDisableTex.x = 0;
	m_ptDisableTex.y = 0;

	m_ptHighlightTex.x = 0;
	m_ptHighlightTex.y = 0;

	m_iTextSize = 12;

	m_ptBtnSize.x = 0;
	m_ptBtnSize.y = 0;
	m_iSelBtn = BTN_NORMAL;
	m_strTexPath = "";
	m_bChange = true;
	m_BtnBase = NULL;
	m_byRed = 0;
	m_byBlue = 0;
	m_byGreen = 0;
	m_iSortFlag = DT_LEFT |DT_SINGLELINE | DT_VCENTER;

	m_byDisableRed = 0;
	m_byDisableBlue = 0;
	m_byDisableGreen = 0;
	m_ptTextIndent.x = 0;
	m_ptTextIndent.y = 0;
}

CButtonDlgBar::~CButtonDlgBar()
{
}


BEGIN_MESSAGE_MAP(CButtonDlgBar, CDialogBar)
	ON_EN_CHANGE(IDC_EDIT_NM_LEFT, &CButtonDlgBar::OnEnChangeEditNmLeft)
	ON_EN_CHANGE(IDC_EDIT_NM_TOP, &CButtonDlgBar::OnEnChangeEditNmTop)
	ON_EN_CHANGE(IDC_EDIT_PR_LEFT, &CButtonDlgBar::OnEnChangeEditPrLeft)
	ON_EN_CHANGE(IDC_EDIT_PR_TOP, &CButtonDlgBar::OnEnChangeEditPrTop)
	ON_EN_CHANGE(IDC_EDIT_DA_LEFT, &CButtonDlgBar::OnEnChangeEditDaLeft)
	ON_EN_CHANGE(IDC_EDIT_DA_TOP, &CButtonDlgBar::OnEnChangeEditDaTop)
	ON_EN_CHANGE(IDC_EDIT_HL_LEFT, &CButtonDlgBar::OnEnChangeEditHlLeft)
	ON_EN_CHANGE(IDC_EDIT_HL_TOP, &CButtonDlgBar::OnEnChangeEditHlTop)
	ON_EN_CHANGE(IDC_EDIT_BTN_WIDTH, &CButtonDlgBar::OnEnChangeEditBtnWidth)
	ON_EN_CHANGE(IDC_EDIT_BTN_HEIGHT, &CButtonDlgBar::OnEnChangeEditBtnHeight)

	ON_BN_CLICKED(IDC_RADIO_NM_BTN, &CButtonDlgBar::OnBnClickedRadioNmBtn)
	ON_BN_CLICKED(IDC_RADIO_PR_BTN, &CButtonDlgBar::OnBnClickedRadioPrBtn)
	ON_BN_CLICKED(IDC_RADIO_DA_BTN, &CButtonDlgBar::OnBnClickedRadioDaBtn)
	ON_BN_CLICKED(IDC_RADIO_HL_BTN, &CButtonDlgBar::OnBnClickedRadioHlBtn)
	ON_MESSAGE(WM_CHANGE_DRAGBOX_INFO, &CButtonDlgBar::OnChangeDragboxInfo)
	ON_MESSAGE(WM_SELCHANGE_TEXTURELIST, &CButtonDlgBar::OnSelchangeTexturelist)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_RADIO_BTN_TEXT_LEFT, &CButtonDlgBar::OnBnClickedRadioBtnTextLeft)
	ON_BN_CLICKED(IDC_RADIO_BTN_TEXT_CENTER, &CButtonDlgBar::OnBnClickedRadioBtnTextCenter)
	ON_BN_CLICKED(IDC_RADIO_BTN_TEXT_RIGHT, &CButtonDlgBar::OnBnClickedRadioBtnTextRight)
	ON_EN_CHANGE(IDC_EDIT_BTN_TEXT_RED, &CButtonDlgBar::OnEnChangeEditBtnTextRed)
	ON_EN_CHANGE(IDC_EDIT_BTN_TEXT_GREEN, &CButtonDlgBar::OnEnChangeEditBtnTextGreen)
	ON_EN_CHANGE(IDC_EDIT_BTN_TEXT_BLUE, &CButtonDlgBar::OnEnChangeEditBtnTextBlue)
	ON_EN_CHANGE(IDC_EDIT_BTN_DA_TEXT_RED, &CButtonDlgBar::OnEnChangeEditBtnDaTextRed)
	ON_EN_CHANGE(IDC_EDIT_BTN_DA_TEXT_GREEN, &CButtonDlgBar::OnEnChangeEditBtnDaTextGreen)
	ON_EN_CHANGE(IDC_EDIT_BTN_DA_TEXT_BLUE, &CButtonDlgBar::OnEnChangeEditBtnDaTextBlue)
	ON_BN_CLICKED(IDC_CHECK_SHOW_DISABLESTATE, &CButtonDlgBar::OnBnClickedCheckShowDisablestate)
	ON_EN_CHANGE(IDC_EDIT_BTN_INDENT_X, &CButtonDlgBar::OnEnChangeEditBtnIndentX)
	ON_EN_CHANGE(IDC_EDIT_BTN_INDENT_Y, &CButtonDlgBar::OnEnChangeEditBtnIndentY)
	ON_EN_CHANGE(IDC_EDIT_BTN_TEXTSIZE, &CButtonDlgBar::OnEnChangeEditBtnTextsize)
	ON_BN_CLICKED(IDC_CHECK_BTN_DISABLE_STRING, &CButtonDlgBar::OnBnClickedCheckBtnDisableString)
	ON_BN_CLICKED(IDC_CHECK_BTN_FONT_BOLD, &CButtonDlgBar::OnBnClickedCheckBtnFontBold)	
	ON_BN_CLICKED(IDC_CHECK_BTN_TEXT_SET_APART, &CButtonDlgBar::OnBnClickedCheckBtnTextSetApart)
	ON_BN_CLICKED(IDC_RADIO_BTN_ICON_FRONT, &CButtonDlgBar::OnBnClickedRadioBtnIconFront)
	ON_BN_CLICKED(IDC_RADIO_BTN_ICON_BACK, &CButtonDlgBar::OnBnClickedRadioBtnIconBack)
	ON_EN_CHANGE(IDC_EDIT_BTN_TEXTBOX_INDENT, &CButtonDlgBar::OnEnChangeEditBtnTextboxIndent)
	ON_EN_CHANGE(IDC_EDIT_BTN_VERTICAL_INDENT, &CButtonDlgBar::OnEnChangeEditBtnVerticalIndent)
END_MESSAGE_MAP()

// CButtonDlgBar 메시지 처리기입니다.

void CButtonDlgBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);

	m_TexturePathEdit	= (CEdit*)GetDlgItem(IDC_EDIT_SHOW_TEXTURE_PATH);
	m_NormalLeft_Edit	= (CEdit*)GetDlgItem(IDC_EDIT_NM_LEFT);
	m_NormalTop_Edit	= (CEdit*)GetDlgItem(IDC_EDIT_NM_TOP);
	m_PressedLeft_Edit	= (CEdit*)GetDlgItem(IDC_EDIT_PR_LEFT);
	m_PressedTop_Edit	= (CEdit*)GetDlgItem(IDC_EDIT_PR_TOP);
	m_HighLightLeft_Edit= (CEdit*)GetDlgItem(IDC_EDIT_HL_LEFT);
	m_HIghLightTop_Edit	= (CEdit*)GetDlgItem(IDC_EDIT_HL_TOP);
	m_DisableLeft_Edit	= (CEdit*)GetDlgItem(IDC_EDIT_DA_LEFT);
	m_DisableTop_Edit	= (CEdit*)GetDlgItem(IDC_EDIT_DA_TOP);
	m_BtnWidthEdit		= (CEdit*)GetDlgItem(IDC_EDIT_BTN_WIDTH);
	m_BtnHeightEdit		= (CEdit*)GetDlgItem(IDC_EDIT_BTN_HEIGHT);

	m_NormalRadioBtn	= (CButton*)GetDlgItem(IDC_RADIO_NM_BTN);
	m_PressedRadioBtn	= (CButton*)GetDlgItem(IDC_RADIO_PR_BTN);
	m_DisableRadioBtn	= (CButton*)GetDlgItem(IDC_RADIO_DA_BTN);
	m_HighlightRadioBtn	= (CButton*)GetDlgItem(IDC_RADIO_HL_BTN);

	m_TextRGB_R_Edit	= (CEdit*)GetDlgItem(IDC_EDIT_BTN_TEXT_RED);
	m_TextRGB_G_Edit	= (CEdit*)GetDlgItem(IDC_EDIT_BTN_TEXT_GREEN);
	m_TextRGB_B_Edit	= (CEdit*)GetDlgItem(IDC_EDIT_BTN_TEXT_BLUE);

	m_RadioSortLeftBtn	= (CButton*)GetDlgItem(IDC_RADIO_BTN_TEXT_LEFT);
	m_RadioSortMidBtn	= (CButton*)GetDlgItem(IDC_RADIO_BTN_TEXT_CENTER);
	m_RadioSortRightBtn	= (CButton*)GetDlgItem(IDC_RADIO_BTN_TEXT_RIGHT);

	m_DisableRGB_R_Edit = (CEdit*)GetDlgItem(IDC_EDIT_BTN_DA_TEXT_RED);
	m_DisableRGB_G_Edit = (CEdit*)GetDlgItem(IDC_EDIT_BTN_DA_TEXT_GREEN);
	m_DisableRGB_B_Edit = (CEdit*)GetDlgItem(IDC_EDIT_BTN_DA_TEXT_BLUE);

	m_TextIndentXEdit = (CEdit*)GetDlgItem(IDC_EDIT_BTN_INDENT_X);
	m_TextIndentYEdit = (CEdit*)GetDlgItem(IDC_EDIT_BTN_INDENT_Y);

	m_TextSizeEdit	= (CEdit*)GetDlgItem(IDC_EDIT_BTN_TEXTSIZE);
	m_DisableStringCheckBox = (CButton*)GetDlgItem(IDC_CHECK_BTN_DISABLE_STRING);
	m_DisableStateCheckBox = (CButton*)GetDlgItem(IDC_CHECK_SHOW_DISABLESTATE);
	m_FontBoldCheckBox = (CButton*)GetDlgItem(IDC_CHECK_BTN_FONT_BOLD);

	m_TextSetApartCheckBox = (CButton*)GetDlgItem(IDC_CHECK_BTN_TEXT_SET_APART);
	m_IconFrontRadioBtn = (CButton*)GetDlgItem(IDC_RADIO_BTN_ICON_FRONT);
	m_IconBackRadioBtn = (CButton*)GetDlgItem(IDC_RADIO_BTN_ICON_BACK);
	m_IndentEdit = (CEdit*)GetDlgItem(IDC_EDIT_BTN_TEXTBOX_INDENT);

	m_BtnTextVerticalIndentEdit = (CEdit*)GetDlgItem(IDC_EDIT_BTN_VERTICAL_INDENT);

	if(CObjMgr::GetInstance()->GetFocusAssetBase() == NULL)
	{
		m_BtnBase = NULL;
	}
}

void CButtonDlgBar::OnEnChangeEditNmLeft()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptNormalTex.x = GetEditText(m_NormalLeft_Edit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptNormalTex.x, (LPARAM)DRAGBOX_LEFT);
	}
	SetButtonProperty();
}


void CButtonDlgBar::OnEnChangeEditNmTop()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptNormalTex.y = GetEditText(m_NormalTop_Edit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptNormalTex.y, (LPARAM)DRAGBOX_TOP);
	}
	SetButtonProperty();
}


void CButtonDlgBar::OnEnChangeEditPrLeft()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptPressedTex.x = GetEditText(m_PressedLeft_Edit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptPressedTex.x, (LPARAM)DRAGBOX_LEFT);
	}
	SetButtonProperty();
}


void CButtonDlgBar::OnEnChangeEditPrTop()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptPressedTex.y = GetEditText(m_PressedTop_Edit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptPressedTex.y, (LPARAM)DRAGBOX_TOP);
	}
	SetButtonProperty();
}


void CButtonDlgBar::OnEnChangeEditDaLeft()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptDisableTex.x = GetEditText(m_DisableLeft_Edit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptDisableTex.x, (LPARAM)DRAGBOX_LEFT);
	}
	SetButtonProperty();
}


void CButtonDlgBar::OnEnChangeEditDaTop()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptDisableTex.y = GetEditText(m_DisableTop_Edit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptDisableTex.y, (LPARAM)DRAGBOX_TOP);
	}
	SetButtonProperty();
}


void CButtonDlgBar::OnEnChangeEditHlLeft()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptHighlightTex.x = GetEditText(m_HighLightLeft_Edit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptHighlightTex.x, (LPARAM)DRAGBOX_LEFT);
	}
	SetButtonProperty();
}


void CButtonDlgBar::OnEnChangeEditHlTop()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptHighlightTex.y = GetEditText(m_HIghLightTop_Edit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptHighlightTex.y, (LPARAM)DRAGBOX_TOP);
	}
	SetButtonProperty();
}


void CButtonDlgBar::OnEnChangeEditBtnWidth()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptBtnSize.x = GetEditText(m_BtnWidthEdit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.x, (LPARAM)DRAGBOX_WIDTH);
	}
	SetButtonProperty();
}


void CButtonDlgBar::OnEnChangeEditBtnHeight()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptBtnSize.y = GetEditText(m_BtnHeightEdit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.y, (LPARAM)DRAGBOX_HEIGHT);
	}
	SetButtonProperty();
}

void CButtonDlgBar::CreateAsset()
{
	if(m_strTexPath == "")
	{
		return;
	}
	//	ObjMgr의 Map에 넣기 위한 동적할당, 추후에 Mgr 파괴할 때 Free된다.
	m_BtnBase = new CBtnBase;
	POINT ptZeroPos;
	ptZeroPos.x = 0;
	ptZeroPos.y = 0;

	//	에셋의 포지션을 설정한다
	m_BtnBase->SetPosition(ptZeroPos);
	m_BtnBase->SetAssetType(ASSET_BUTTON);
	m_BtnBase->SetFilePath(m_strTexPath);
	m_BtnBase->SetParenthWnd(m_pParenthWnd);
	//	에셋의 멤버 변수들을 셋팅해준다
	((CBtnBase*)m_BtnBase)->SetButtonProperty(m_ptNormalTex, m_ptPressedTex, m_ptDisableTex, m_ptHighlightTex, m_ptBtnSize);

	//	만들어진 에셋을 Map에 저장하는 과정
	CObjMgr::GetInstance()->InsertAssetBase(m_BtnBase);
}

void CButtonDlgBar::SetButtonProperty()
{
	//	 실시간으로 갱신하기 위한 함수
	if(m_BtnBase == NULL)
	{
		CreateAsset();
	}

	if(strcmp(m_strTexPath.c_str(), ""))
	{
		m_BtnBase->SetFilePath(m_strTexPath);
		m_BtnBase->SetButtonProperty(m_ptNormalTex, m_ptPressedTex, m_ptDisableTex, m_ptHighlightTex, m_ptBtnSize);
		m_BtnBase->SetText(m_byRed, m_byGreen, m_byBlue, m_iSortFlag);
		m_BtnBase->SetDisableText(m_byDisableRed, m_byDisableGreen, m_byDisableBlue);
		m_BtnBase->SetTextIndent(m_ptTextIndent);
		m_BtnBase->SetTextSize(m_iTextSize);
	}
}

void CButtonDlgBar::OnBnClickedRadioNmBtn()
{
	//	선택되었을때 픽쳐다이알로그의 xpos와 ypos를 수정해주면 될 것 같다
	m_iSelBtn = BTN_NORMAL;
	//	포인터를 보내준 다음 해당 값을 참조해서 복사를 하게 하고 그거를 다이알로그바에 세팅하게 하면
	::SendMessage(m_PicturehWnd, WM_CHANGE_RADIOBTN, (WPARAM)m_ptNormalTex.x, (LPARAM)m_ptNormalTex.y);
}


void CButtonDlgBar::OnBnClickedRadioPrBtn()
{
	m_iSelBtn = BTN_PRESSED;
	::SendMessage(m_PicturehWnd, WM_CHANGE_RADIOBTN, (WPARAM)m_ptPressedTex.x, (LPARAM)m_ptPressedTex.y);
}


void CButtonDlgBar::OnBnClickedRadioDaBtn()
{
	m_iSelBtn = BTN_DISABLE;
	::SendMessage(m_PicturehWnd, WM_CHANGE_RADIOBTN, (WPARAM)m_ptDisableTex.x, (LPARAM)m_ptDisableTex.y);
}


void CButtonDlgBar::OnBnClickedRadioHlBtn()
{
	m_iSelBtn = BTN_HIGHLIGHT;
	::SendMessage(m_PicturehWnd, WM_CHANGE_RADIOBTN, (WPARAM)m_ptHighlightTex.x, (LPARAM)m_ptHighlightTex.y);
}

void CButtonDlgBar::SetBtnPosition( int _x, int _y )
{
	switch(m_iSelBtn)
	{
	case BTN_NORMAL:
		m_ptNormalTex.x = _x;
		m_ptNormalTex.y = _y;
		break;
	case BTN_PRESSED:
		m_ptPressedTex.x = _x;
		m_ptPressedTex.y = _y;
		break;
	case BTN_HIGHLIGHT:
		m_ptHighlightTex.x = _x;
		m_ptHighlightTex.y = _y;
		break;
	case BTN_DISABLE:
		m_ptDisableTex.x = _x;
		m_ptDisableTex.y = _y;
		break;
	}
}

afx_msg LRESULT CButtonDlgBar::OnChangeDragboxInfo(WPARAM wParam, LPARAM lParam)
{
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
			SetEditWndText(m_NormalLeft_Edit, m_ptNormalTex.x);
			break;
		case DRAGBOX_TOP:
			m_ptNormalTex.y = (int)wParam;
			SetEditWndText(m_NormalTop_Edit, m_ptNormalTex.y);
			break;
		}
		break;
	case BTN_DISABLE:
		switch((int)lParam)
		{
		case DRAGBOX_LEFT:
			m_ptDisableTex.x = (int)wParam;
			SetEditWndText(m_DisableLeft_Edit, m_ptDisableTex.x);
			break;
		case DRAGBOX_TOP:
			m_ptDisableTex.y = (int)wParam;
			SetEditWndText(m_DisableTop_Edit, m_ptDisableTex.y);
			break;
		}
		break;
	case BTN_HIGHLIGHT:
		switch((int)lParam)
		{
		case DRAGBOX_LEFT:
			m_ptHighlightTex.x = (int)wParam;
			SetEditWndText(m_HighLightLeft_Edit, m_ptHighlightTex.x);
			break;
		case DRAGBOX_TOP:
			m_ptHighlightTex.y = (int)wParam;
			SetEditWndText(m_HIghLightTop_Edit, m_ptHighlightTex.y);
			break;
		}
		break;
	case BTN_PRESSED:
		switch((int)lParam)
		{
		case DRAGBOX_LEFT:
			m_ptPressedTex.x = (int)wParam;
			SetEditWndText(m_PressedLeft_Edit, m_ptPressedTex.x);
			break;
		case DRAGBOX_TOP:
			m_ptPressedTex.y = (int)wParam;
			SetEditWndText(m_PressedTop_Edit, m_ptPressedTex.y);
			break;
		}
		break;
	}

	switch((int)lParam)
	{
	case DRAGBOX_WIDTH:
		m_ptBtnSize.x = (int)wParam;
		SetEditWndText(m_BtnWidthEdit, m_ptBtnSize.x);
		break;
	case DRAGBOX_HEIGHT:
		m_ptBtnSize.y = (int)wParam;
		SetEditWndText(m_BtnHeightEdit, m_ptBtnSize.y);
		break;
	}

	m_bChange = true;

	return 0;
}

afx_msg LRESULT CButtonDlgBar::OnSelchangeTexturelist(WPARAM wParam, LPARAM lParam)
{
	m_strTexPath = (CDBMgr::GetInstance()->GetPathMap()->find((int)wParam)->second);
	m_TexturePathEdit = (CEdit*)GetDlgItem(IDC_EDIT_SHOW_TEXTURE_PATH);

	CA2T wt(m_strTexPath.c_str());
	m_TexturePathEdit->SetWindowText(wt);

	if(m_BtnBase != NULL)
	{
		m_BtnBase->SetFilePath(m_strTexPath);
	}
	return 0;
}


int CButtonDlgBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}


void CButtonDlgBar::OnBnClickedRadioBtnTextLeft()
{
	m_iSortFlag = DT_LEFT | DT_VCENTER | DT_SINGLELINE;
	SetButtonProperty();
}


void CButtonDlgBar::OnBnClickedRadioBtnTextCenter()
{
	m_iSortFlag = DT_CENTER | DT_VCENTER | DT_SINGLELINE;
	SetButtonProperty();
}


void CButtonDlgBar::OnBnClickedRadioBtnTextRight()
{
	m_iSortFlag = DT_RIGHT | DT_VCENTER | DT_SINGLELINE;
	SetButtonProperty();
}

void CButtonDlgBar::OnEnChangeEditBtnTextRed()
{
	UpdateData(true);
	m_byRed = GetEditText(m_TextRGB_R_Edit);
	SetButtonProperty();
}


void CButtonDlgBar::OnEnChangeEditBtnTextGreen()
{
	UpdateData(true);
	m_byGreen = GetEditText(m_TextRGB_G_Edit);
	SetButtonProperty();
}


void CButtonDlgBar::OnEnChangeEditBtnTextBlue()
{
	UpdateData(true);
	m_byBlue = GetEditText(m_TextRGB_B_Edit);
	SetButtonProperty();
}

void CButtonDlgBar::Initialize()
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

	m_ptBtnSize.x = 0;
	m_ptBtnSize.y = 0;
	m_iSelBtn = BTN_NORMAL;
	m_strTexPath = "";
	m_bChange = true;
	m_BtnBase = NULL;
	m_byRed = 0;
	m_byBlue = 0;
	m_byGreen = 0;
	m_TextIndentXEdit->SetWindowText(L"0");
	m_TextIndentYEdit->SetWindowText(L"0");
	m_ptTextIndent.x = 0;
	m_ptTextIndent.y = 0;
	m_iSortFlag = DT_LEFT |DT_SINGLELINE | DT_VCENTER;

	m_byDisableRed = 0;
	m_byDisableBlue = 0;
	m_byDisableGreen = 0;

	m_NormalLeft_Edit->SetWindowText(L"0");
	m_NormalTop_Edit->SetWindowText(L"0");
	m_PressedLeft_Edit->SetWindowText(L"0");
	m_PressedTop_Edit->SetWindowText(L"0");
	m_HighLightLeft_Edit->SetWindowText(L"0");
	m_HIghLightTop_Edit->SetWindowText(L"0");
	m_DisableLeft_Edit->SetWindowText(L"0");
	m_DisableTop_Edit->SetWindowText(L"0");
	m_BtnWidthEdit->SetWindowText(L"0");
	m_BtnHeightEdit->SetWindowText(L"0");
	m_TexturePathEdit->SetWindowText(L"");
	m_strTexPath = "";

	m_NormalRadioBtn->SetCheck(1);
	m_PressedRadioBtn->SetCheck(0);
	m_HighlightRadioBtn->SetCheck(0);
	m_DisableRadioBtn->SetCheck(0);
	m_TextSizeEdit->SetWindowText(L"12");

	m_bChange = true;

	m_TextRGB_R_Edit->SetWindowText(L"0");
	m_TextRGB_G_Edit->SetWindowText(L"0");
	m_TextRGB_B_Edit->SetWindowText(L"0");
	m_RadioSortLeftBtn->SetCheck(1);
	m_RadioSortMidBtn->SetCheck(0);
	m_RadioSortRightBtn->SetCheck(0);
	m_DisableRGB_R_Edit->SetWindowText(L"0");
	m_DisableRGB_G_Edit->SetWindowText(L"0");
	m_DisableRGB_B_Edit->SetWindowText(L"0");
	m_TextSetApartCheckBox->SetCheck(0);

	m_IconBackRadioBtn->EnableWindow(false);
	m_IconFrontRadioBtn->EnableWindow(false);

	m_FontBoldCheckBox->SetCheck(0);
}

void CButtonDlgBar::SetBtnbaseNULL()
{
	m_BtnBase = NULL;
}

void CButtonDlgBar::RefreshDlgBar( CBtnBase* _LoadBase, POINT& _NormalTex )
{
	m_bChange = false;

	m_ptNormalTex.x  = _LoadBase->GetNormalPos().x;
	EditTextSetting(m_ptNormalTex.x, m_NormalLeft_Edit);
	m_ptNormalTex.y = _LoadBase->GetNormalPos().y;
	EditTextSetting(m_ptNormalTex.y, m_NormalTop_Edit);
	_NormalTex = m_ptNormalTex;

	m_ptPressedTex.x = _LoadBase->GetPressedPos().x;
	EditTextSetting(m_ptPressedTex.x, m_PressedLeft_Edit);
	m_ptPressedTex.y = _LoadBase->GetPressedPos().y;
	EditTextSetting(m_ptPressedTex.y, m_PressedTop_Edit);

	m_ptDisableTex.x = _LoadBase->GetDisablePos().x;
	EditTextSetting(m_ptDisableTex.x, m_DisableLeft_Edit);
	m_ptDisableTex.y = _LoadBase->GetDisablePos().y;
	EditTextSetting(m_ptDisableTex.y, m_DisableTop_Edit);

	m_ptHighlightTex.x = _LoadBase->GetHighlightPos().x;
	EditTextSetting(m_ptHighlightTex.x, m_HighLightLeft_Edit);
	m_ptHighlightTex.y = _LoadBase->GetHighlightPos().y;
	EditTextSetting(m_ptHighlightTex.y, m_HIghLightTop_Edit);

	m_ptBtnSize.x = _LoadBase->GetSize().x;
	EditTextSetting(m_ptBtnSize.x, m_BtnWidthEdit);
	m_ptBtnSize.y =  _LoadBase->GetSize().y;
	EditTextSetting(m_ptBtnSize.y, m_BtnHeightEdit);

	m_byRed = _LoadBase->GetTextBox().GetRGB(RGB_R);
	EditTextSetting(m_byRed, m_TextRGB_R_Edit);
	m_byGreen = _LoadBase->GetTextBox().GetRGB(RGB_G);
	EditTextSetting(m_byGreen, m_TextRGB_G_Edit);
	m_byBlue = _LoadBase->GetTextBox().GetRGB(RGB_B);
	EditTextSetting(m_byBlue, m_TextRGB_B_Edit);

	m_ptTextIndent.x = _LoadBase->GetTextBox().GetTextIndent().x;
	EditTextSetting(m_ptTextIndent.x, m_TextIndentXEdit);
	m_ptTextIndent.y = _LoadBase->GetTextBox().GetTextIndent().y;
	EditTextSetting(m_ptTextIndent.y, m_TextIndentYEdit);

	m_byDisableRed = _LoadBase->GetTextBox().GetDisableRGB(RGB_R);
	EditTextSetting(m_byDisableRed, m_DisableRGB_R_Edit);
	m_byDisableGreen = _LoadBase->GetTextBox().GetDisableRGB(RGB_G);
	EditTextSetting(m_byDisableGreen, m_DisableRGB_G_Edit);
	m_byDisableBlue = _LoadBase->GetTextBox().GetDisableRGB(RGB_B);
	EditTextSetting(m_byDisableBlue, m_DisableRGB_B_Edit);

	m_iTextSize = _LoadBase->GetTextSize();
	EditTextSetting(m_iTextSize, m_TextSizeEdit);

	// 행간 조절 추가 [7/16/2019 Gabrielle]
	EditTextSetting(_LoadBase->GetTextBox().GetVerticalIndent(), m_BtnTextVerticalIndentEdit);

	bool bDefaultShow = _LoadBase->GetDefaultShow();
	if(!bDefaultShow)
	{
		m_DisableStringCheckBox->SetCheck(1);
	}
	else
	{
		m_DisableStringCheckBox->SetCheck(0);
	}

	bool bFontBold = _LoadBase->GetFontBold();
	if(bFontBold)
	{
		m_FontBoldCheckBox->SetCheck(1);
	}
	else
	{
		m_FontBoldCheckBox->SetCheck(0);
	}

	UINT uiSort = _LoadBase->GetTextBox().GetSortFlag();

	if(uiSort == (DT_VCENTER | DT_LEFT | DT_SINGLELINE))
	{
		m_RadioSortLeftBtn->SetCheck(1);
		m_RadioSortMidBtn->SetCheck(0);
		m_RadioSortRightBtn->SetCheck(0);
		m_iSortFlag = DT_LEFT | DT_VCENTER | DT_SINGLELINE;
	}
	else if(uiSort == (DT_VCENTER | DT_CENTER | DT_SINGLELINE))
	{
		m_RadioSortLeftBtn->SetCheck(0);
		m_RadioSortMidBtn->SetCheck(1);
		m_RadioSortRightBtn->SetCheck(0);
		m_iSortFlag = DT_CENTER | DT_VCENTER | DT_SINGLELINE;
	}
	else if(uiSort == (DT_VCENTER | DT_RIGHT | DT_SINGLELINE))
	{
		m_RadioSortLeftBtn->SetCheck(0);
		m_RadioSortMidBtn->SetCheck(0);
		m_RadioSortRightBtn->SetCheck(1);
		m_iSortFlag = DT_RIGHT | DT_VCENTER | DT_SINGLELINE;
	}

	EditTextSetting(_LoadBase->GetIconIndent(), m_IndentEdit);

	if(_LoadBase->GetTextBoxSetApart())
	{
		m_TextSetApartCheckBox->SetCheck(1);
		m_IconBackRadioBtn->EnableWindow(true);
		m_IconFrontRadioBtn->EnableWindow(true);
		if(_LoadBase->GetIconFront())
		{
			m_IconFrontRadioBtn->SetCheck(1);
			m_IconBackRadioBtn->SetCheck(0);
		}
		else
		{
			m_IconFrontRadioBtn->SetCheck(0);
			m_IconBackRadioBtn->SetCheck(1);
		}
	}
	else
	{
		m_TextSetApartCheckBox->SetCheck(0);
	}

	m_bChange = true;

	SetMemberBtnBase(_LoadBase);

	m_strTexPath = _LoadBase->GetFilePath();
	CString TexPath(m_strTexPath.c_str());
	m_TexturePathEdit->SetWindowText(TexPath);
}


void CButtonDlgBar::OnEnChangeEditBtnDaTextRed()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_byDisableRed = GetEditText(m_DisableRGB_R_Edit);
	}
	SetButtonProperty();
}


void CButtonDlgBar::OnEnChangeEditBtnDaTextGreen()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_byDisableGreen = GetEditText(m_DisableRGB_G_Edit);
	}
	SetButtonProperty();
}


void CButtonDlgBar::OnEnChangeEditBtnDaTextBlue()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_byDisableBlue = GetEditText(m_DisableRGB_B_Edit);
	}
	SetButtonProperty();
}


void CButtonDlgBar::OnBnClickedCheckShowDisablestate()
{
	UpdateData(true);
	if(m_DisableStateCheckBox->GetCheck() == BST_CHECKED)
	{
		m_BtnBase->SetbDisable(true);
	}
	else if(m_DisableStateCheckBox->GetCheck() == BST_UNCHECKED)
	{
		m_BtnBase->SetbDisable(false);
	}
}

void CButtonDlgBar::OnEnChangeEditBtnIndentX()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptTextIndent.x = GetEditText(m_TextIndentXEdit);
		SetButtonProperty();
	}
}


void CButtonDlgBar::OnEnChangeEditBtnIndentY()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptTextIndent.y = GetEditText(m_TextIndentYEdit);
		SetButtonProperty();
	}
}


void CButtonDlgBar::OnEnChangeEditBtnTextsize()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_iTextSize = GetEditText(m_TextSizeEdit);
		SetButtonProperty();
	}
}

void CButtonDlgBar::OnBnClickedCheckBtnDisableString()
{
	UpdateData(true);
	if(m_DisableStringCheckBox->GetCheck() == BST_CHECKED)
	{
		if(m_BtnBase != NULL)
			m_BtnBase->SetDefaultShow(false);
	}
	else if(m_DisableStringCheckBox->GetCheck() == BST_UNCHECKED)
	{
		if(m_BtnBase != NULL)
			m_BtnBase->SetDefaultShow(true);
	}
}

void CButtonDlgBar::OnBnClickedCheckBtnFontBold()
{
	UpdateData(true);
	if(m_FontBoldCheckBox->GetCheck() == BST_CHECKED)
	{
		if(m_BtnBase != NULL)
			m_BtnBase->SetFontBold(true);
	}
	else if(m_FontBoldCheckBox->GetCheck() == BST_UNCHECKED)
	{
		if(m_BtnBase != NULL)
			m_BtnBase->SetFontBold(false);
	}	
}

void CButtonDlgBar::OnBnClickedCheckBtnTextSetApart()
{
	UpdateData(true);
	if(m_bChange)
	{
		if(m_TextSetApartCheckBox->GetCheck() == BST_CHECKED)
		{
			if(m_BtnBase != NULL)
				m_BtnBase->SetbTextBoxSetApart(true);

			m_IconBackRadioBtn->EnableWindow(true);
			m_IconFrontRadioBtn->EnableWindow(true);
			m_IconFrontRadioBtn->SetCheck(1);
			m_IconBackRadioBtn->SetCheck(0);
		}
		else if(m_TextSetApartCheckBox->GetCheck() == BST_UNCHECKED)
		{
			if(m_BtnBase != NULL)
				m_BtnBase->SetbTextBoxSetApart(false);

			m_IconBackRadioBtn->EnableWindow(false);
			m_IconFrontRadioBtn->EnableWindow(false);
			m_IconFrontRadioBtn->SetCheck(0);
			m_IconBackRadioBtn->SetCheck(0);
		}
	}
}


void CButtonDlgBar::OnBnClickedRadioBtnIconFront()
{
	if(m_bChange)
	{
		if(m_BtnBase != NULL)
			m_BtnBase->SetbIconFont(true);
	}
}


void CButtonDlgBar::OnBnClickedRadioBtnIconBack()
{
	if(m_bChange)
	{
		if(m_BtnBase != NULL)
			m_BtnBase->SetbIconFont(false);
	}
}


void CButtonDlgBar::OnEnChangeEditBtnTextboxIndent()
{
	if(m_bChange)
	{
		UpdateData(true);
		if(m_BtnBase != NULL)
		{
			m_BtnBase->SetTextboxIndent(GetEditText(m_IndentEdit));
		}
	}
}


void CButtonDlgBar::OnEnChangeEditBtnVerticalIndent()
{
	if(m_bChange)
	{
		UpdateData(true);
		if(m_BtnBase != NULL)
		{
			m_BtnBase->SetTextBoxVerticalIndent(GetEditText(m_BtnTextVerticalIndentEdit));
		}
	}
}