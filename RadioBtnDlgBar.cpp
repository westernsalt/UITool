// RadioBtnDlgBar.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "UITool.h"
#include "RadioBtnDlgBar.h"
#include "afxdialogex.h"

#include "RadioBtnBase.h"
#include "GroupButton.h"
#include "ObjMgr.h"
#include "DBMgr.h"


// CRadioBtnDlgBar 대화 상자입니다.

IMPLEMENT_DYNAMIC(CRadioBtnDlgBar, CDialogBar)

CRadioBtnDlgBar::CRadioBtnDlgBar(CWnd* pParent /*=NULL*/)
{
	m_ptNormalTex.x = 0;
	m_ptNormalTex.y = 0;

	m_ptPressedTex.x = 0;
	m_ptPressedTex.y = 0;

	m_ptDisableTex.x = 0;
	m_ptDisableTex.y = 0;

	m_ptHighlightTex.x = 0;
	m_ptHighlightTex.y = 0;

	m_ptCheckMarkTex.x = 0;
	m_ptCheckMarkTex.y = 0;

	m_ptCurPos.x = 0;
	m_ptCurPos.y = 0;

	m_ptBtnSize.x = 0;
	m_ptBtnSize.y = 0;
	m_ptCheckMarkSize.x = 0;
	m_ptCheckMarkSize.y = 0;
	m_iSelBtn = BTN_NORMAL;
	m_strTexPath = "";
	m_bChange = true;
	m_RadioGroupBase = NULL;
	m_byRed = 0;
	m_byBlue = 0;
	m_byGreen = 0;
	m_iSortFlag = DT_LEFT | DT_SINGLELINE | DT_VCENTER;
	m_iTextBoxInterval = 0;
	m_iRadioBtnInterval = 0;

	m_ptColRow.x = 1;
	m_ptColRow.y = 1;
	m_iArrayType = NOTHING_IN_HERE;

	m_byDisableRed = 0;
	m_byDisableBlue = 0;
	m_byDisableGreen = 0;
	m_ptIconIndent = m_ptCheckMarkSize;
	m_strIconPath = "";
	m_iIconPosition = ICON_FRONT;
}

CRadioBtnDlgBar::~CRadioBtnDlgBar()
{
	SAFE_DELETE(m_RadioGroupBase);
}

void CRadioBtnDlgBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);

	m_TexturePathEdit	= (CEdit*)GetDlgItem(IDC_EDIT_RB_SHOW_TEXTURE_PATH);
	m_NormalLeft_Edit	= (CEdit*)GetDlgItem(IDC_EDIT_RB_NM_LEFT);
	m_NormalTop_Edit	= (CEdit*)GetDlgItem(IDC_EDIT_RB_NM_TOP);
	m_PressedLeft_Edit	= (CEdit*)GetDlgItem(IDC_EDIT_RB_PR_LEFT);
	m_PressedTop_Edit	= (CEdit*)GetDlgItem(IDC_EDIT_RB_PR_TOP);
	m_HighLightLeft_Edit= (CEdit*)GetDlgItem(IDC_EDIT_RB_HL_LEFT);
	m_HIghLightTop_Edit	= (CEdit*)GetDlgItem(IDC_EDIT_RB_HL_TOP);
	m_DisableLeft_Edit	= (CEdit*)GetDlgItem(IDC_EDIT_RB_DA_LEFT);
	m_DisableTop_Edit	= (CEdit*)GetDlgItem(IDC_EDIT_RB_DA_TOP);
	m_BtnWidthEdit		= (CEdit*)GetDlgItem(IDC_EDIT_RB_BTN_WIDTH);
	m_BtnHeightEdit		= (CEdit*)GetDlgItem(IDC_EDIT_RB_BTN_HEIGHT);
	m_CheckMarkLeftEdit	= (CEdit*)GetDlgItem(IDC_EDIT_RB_CM_LEFT);
	m_CheckMarkTopEdit	= (CEdit*)GetDlgItem(IDC_EDIT_RB_CM_TOP);
	m_IntervalEdit		= (CEdit*)GetDlgItem(IDC_EDIT_RB_INTERVAL);

	m_RadioBtnColEdit	= (CEdit*)GetDlgItem(IDC_EDIT_RADIO_COL);
	m_RadioBtnRowEdit	= (CEdit*)GetDlgItem(IDC_EDIT_RADIO_ROW);
	m_RadioBtnIntervalEdit	= (CEdit*)GetDlgItem(IDC_EDIT_RADIO_INTERVAL);

	m_NormalRadioBtn	= (CButton*)GetDlgItem(IDC_RADIO_RB_NM_BTN);
	m_PressedRadioBtn	= (CButton*)GetDlgItem(IDC_RADIO_RB_PR_BTN);
	m_DisableRadioBtn	= (CButton*)GetDlgItem(IDC_RADIO_RB_DA_BTN);
	m_HighlightRadioBtn	= (CButton*)GetDlgItem(IDC_RADIO_RB_HL_BTN);
	m_CheckMarkRadioBtn	= (CButton*)GetDlgItem(IDC_RADIO_RB_CM_BTN);

	m_TextRGB_R_Edit	= (CEdit*)GetDlgItem(IDC_EDIT_RB_RGB_R);
	m_TextRGB_G_Edit	= (CEdit*)GetDlgItem(IDC_EDIT_RB_RGB_G);
	m_TextRGB_B_Edit	= (CEdit*)GetDlgItem(IDC_EDIT_RB_RGB_B);
	m_TextPreviewEdit	= (CEdit*)GetDlgItem(IDC_EDIT_RB_INPUTBOX);

	m_RadioSortLeftBtn	= (CButton*)GetDlgItem(IDC_RADIO_RB_LEFT_SORT);
	m_RadioSortMidBtn	= (CButton*)GetDlgItem(IDC_RADIO_RB_CENTER_SORT);
	m_RadioSortRightBtn	= (CButton*)GetDlgItem(IDC_RADIO_RB_RIGHT_SORT);

	m_DisableRGB_R_Edit	= (CEdit*)GetDlgItem(IDC_EDIT_RB_DA_RGB_R);
	m_DisableRGB_G_Edit	= (CEdit*)GetDlgItem(IDC_EDIT_RB_DA_RGB_G);
	m_DisableRGB_B_Edit	= (CEdit*)GetDlgItem(IDC_EDIT_RB_DA_RGB_B);

	m_DisableCheckBox	= (CButton*)GetDlgItem(IDC_CHECK_DISABLESTATE);

	m_TextSizeEdit		= (CEdit*)GetDlgItem(IDC_EDIT_RAD_TEXTSIZE);
	
	m_IconPathEdit		= (CEdit*)GetDlgItem(IDC_EDIT_RAD_ICON_PATH);
	m_IconHorizontalIndentEdit	= (CEdit*)GetDlgItem(IDC_EDIT_RAD_HORIZON_INDENT);
	m_IconVerticalIndentEdit	= (CEdit*)GetDlgItem(IDC_EDIT_RAD_VERTICAL_INDENT);

	m_FrontIconRadioBtn	= (CButton*)GetDlgItem(IDC_RADIO_RAD_FRONT_ICON);
	m_BackIconRadioBtn	= (CButton*)GetDlgItem(IDC_RADIO_RAD_BACK_ICON);

	m_DisableStringCheckBox = (CButton*)GetDlgItem(IDC_CHECK_RAD_DISABLE_STRING);
	m_FontBoldCheckBox = (CButton*)GetDlgItem(IDC_CHECK_RAD_FONT_BOLD);

	if(CObjMgr::GetInstance()->GetFocusAssetBase() == NULL)
	{
		m_RadioGroupBase = NULL;
	}
}


BEGIN_MESSAGE_MAP(CRadioBtnDlgBar, CDialogBar)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_RAD_LOAD_ICON, &CRadioBtnDlgBar::OnUpdateCmdUI)

	ON_EN_CHANGE(IDC_EDIT_RB_NM_LEFT, &CRadioBtnDlgBar::OnEnChangeEditRbNmLeft)
	ON_EN_CHANGE(IDC_EDIT_RB_NM_TOP, &CRadioBtnDlgBar::OnEnChangeEditRbNmTop)
	ON_EN_CHANGE(IDC_EDIT_RB_PR_LEFT, &CRadioBtnDlgBar::OnEnChangeEditRbPrLeft)
	ON_EN_CHANGE(IDC_EDIT_RB_PR_TOP, &CRadioBtnDlgBar::OnEnChangeEditRbPrTop)
	ON_EN_CHANGE(IDC_EDIT_RB_DA_LEFT, &CRadioBtnDlgBar::OnEnChangeEditRbDaLeft)
	ON_EN_CHANGE(IDC_EDIT_RB_DA_TOP, &CRadioBtnDlgBar::OnEnChangeEditRbDaTop)
	ON_EN_CHANGE(IDC_EDIT_RB_HL_LEFT, &CRadioBtnDlgBar::OnEnChangeEditRbHlLeft)
	ON_EN_CHANGE(IDC_EDIT_RB_HL_TOP, &CRadioBtnDlgBar::OnEnChangeEditRbHlTop)
	ON_EN_CHANGE(IDC_EDIT_RB_CM_LEFT, &CRadioBtnDlgBar::OnEnChangeEditRbCmLeft)
	ON_EN_CHANGE(IDC_EDIT_RB_CM_TOP, &CRadioBtnDlgBar::OnEnChangeEditRbCmTop)
	ON_EN_CHANGE(IDC_EDIT_RB_BTN_WIDTH, &CRadioBtnDlgBar::OnEnChangeEditRbBtnWidth)
	ON_EN_CHANGE(IDC_EDIT_RB_BTN_HEIGHT, &CRadioBtnDlgBar::OnEnChangeEditRbBtnHeight)
	ON_EN_CHANGE(IDC_EDIT_RADIO_COL, &CRadioBtnDlgBar::OnEnChangeEditRadioCol)
	ON_EN_CHANGE(IDC_EDIT_RADIO_ROW, &CRadioBtnDlgBar::OnEnChangeEditRadioRow)
	ON_EN_CHANGE(IDC_EDIT_RADIO_INTERVAL, &CRadioBtnDlgBar::OnEnChangeEditRadioInterval)
	ON_BN_CLICKED(IDC_RADIO_RB_LEFT_SORT, &CRadioBtnDlgBar::OnBnClickedRadioRbLeftSort)
	ON_BN_CLICKED(IDC_RADIO_RB_CENTER_SORT, &CRadioBtnDlgBar::OnBnClickedRadioRbCenterSort)
	ON_BN_CLICKED(IDC_RADIO_RB_RIGHT_SORT, &CRadioBtnDlgBar::OnBnClickedRadioRbRightSort)
	ON_EN_CHANGE(IDC_EDIT_RB_INPUTBOX, &CRadioBtnDlgBar::OnEnChangeEditRbInputbox)
	ON_EN_CHANGE(IDC_EDIT_RB_TEXTBOXINTERVAL, &CRadioBtnDlgBar::OnEnChangeEditRbTextboxinterval)
	ON_EN_CHANGE(IDC_EDIT_RB_RGB_R, &CRadioBtnDlgBar::OnEnChangeEditRbRgbR)
	ON_EN_CHANGE(IDC_EDIT_RB_RGB_G, &CRadioBtnDlgBar::OnEnChangeEditRbRgbG)
	ON_EN_CHANGE(IDC_EDIT_RB_RGB_B, &CRadioBtnDlgBar::OnEnChangeEditRbRgbB)
	ON_BN_CLICKED(IDC_RADIO_RB_NM_BTN, &CRadioBtnDlgBar::OnBnClickedRadioRbNmBtn)
	ON_BN_CLICKED(IDC_RADIO_RB_PR_BTN, &CRadioBtnDlgBar::OnBnClickedRadioRbPrBtn)
	ON_BN_CLICKED(IDC_RADIO_RB_DA_BTN, &CRadioBtnDlgBar::OnBnClickedRadioRbDaBtn)
	ON_BN_CLICKED(IDC_RADIO_RB_HL_BTN, &CRadioBtnDlgBar::OnBnClickedRadioRbHlBtn)
	ON_BN_CLICKED(IDC_RADIO_RB_CM_BTN, &CRadioBtnDlgBar::OnBnClickedRadioRbCmBtn)
	ON_MESSAGE(WM_CHANGE_DRAGBOX_INFO, &CRadioBtnDlgBar::OnChangeDragboxInfo)
	ON_MESSAGE(WM_SELCHANGE_TEXTURELIST, &CRadioBtnDlgBar::OnSelchangeTexturelist)
	ON_EN_CHANGE(IDC_EDIT_RB_DA_RGB_R, &CRadioBtnDlgBar::OnEnChangeEditRbDaRgbR)
	ON_EN_CHANGE(IDC_EDIT_RB_DA_RGB_G, &CRadioBtnDlgBar::OnEnChangeEditRbDaRgbG)
	ON_EN_CHANGE(IDC_EDIT_RB_DA_RGB_B, &CRadioBtnDlgBar::OnEnChangeEditRbDaRgbB)
	ON_BN_CLICKED(IDC_CHECK_DISABLESTATE, &CRadioBtnDlgBar::OnBnClickedCheckDisablestate)
	ON_EN_CHANGE(IDC_EDIT_RAD_TEXTSIZE, &CRadioBtnDlgBar::OnEnChangeEditRadTextsize)
	ON_EN_CHANGE(IDC_EDIT_RAD_ICON_PATH, &CRadioBtnDlgBar::OnEnChangeEditRadIconPath)
	ON_BN_CLICKED(IDC_BUTTON_RAD_LOAD_ICON, &CRadioBtnDlgBar::OnBnClickedButtonRadLoadIcon)
	ON_BN_CLICKED(IDC_RADIO_RAD_FRONT_ICON, &CRadioBtnDlgBar::OnBnClickedRadioRadFrontIcon)
	ON_BN_CLICKED(IDC_RADIO_RAD_BACK_ICON, &CRadioBtnDlgBar::OnBnClickedRadioRadBackIcon)
	ON_EN_CHANGE(IDC_EDIT_RAD_HORIZON_INDENT, &CRadioBtnDlgBar::OnEnChangeEditRadHorizonIndent)
	ON_EN_CHANGE(IDC_EDIT_RAD_VERTICAL_INDENT, &CRadioBtnDlgBar::OnEnChangeEditRadVerticalIndent)
	ON_BN_CLICKED(IDC_CHECK_RAD_DISABLE_STRING, &CRadioBtnDlgBar::OnBnClickedCheckRadDisableString)
	ON_BN_CLICKED(IDC_CHECK_RAD_FONT_BOLD, &CRadioBtnDlgBar::OnBnClickedCheckInputFontBold)	
END_MESSAGE_MAP()


// CRadioBtnDlgBar 메시지 처리기입니다.
void CRadioBtnDlgBar::OnUpdateCmdUI(CCmdUI* pCmdUI) // 다이얼로그바 버튼 활성화 위해 추가
{
	pCmdUI->Enable(TRUE);
}

void CRadioBtnDlgBar::OnEnChangeEditRbNmLeft()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptNormalTex.x = GetEditText(m_NormalLeft_Edit);
		::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptNormalTex.x, (LPARAM)DRAGBOX_LEFT);
	}

	SetRadioBtnProperty();
}


void CRadioBtnDlgBar::OnEnChangeEditRbNmTop()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptNormalTex.y = GetEditText(m_NormalTop_Edit);
		::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptNormalTex.y, (LPARAM)DRAGBOX_TOP);
	}
	SetRadioBtnProperty();
}


void CRadioBtnDlgBar::OnEnChangeEditRbPrLeft()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptPressedTex.x = GetEditText(m_PressedLeft_Edit);
		::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptPressedTex.x, (LPARAM)DRAGBOX_LEFT);
	}
	SetRadioBtnProperty();
}


void CRadioBtnDlgBar::OnEnChangeEditRbPrTop()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptPressedTex.y = GetEditText(m_PressedTop_Edit);
		::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptPressedTex.y, (LPARAM)DRAGBOX_TOP);
	}
	SetRadioBtnProperty();
}


void CRadioBtnDlgBar::OnEnChangeEditRbDaLeft()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptDisableTex.x = GetEditText(m_DisableLeft_Edit);
		::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptDisableTex.x, (LPARAM)DRAGBOX_LEFT);
	}
	SetRadioBtnProperty();
}


void CRadioBtnDlgBar::OnEnChangeEditRbDaTop()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptDisableTex.y = GetEditText(m_DisableTop_Edit);
		::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptDisableTex.y, (LPARAM)DRAGBOX_TOP);
	}
	SetRadioBtnProperty();
}


void CRadioBtnDlgBar::OnEnChangeEditRbHlLeft()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptHighlightTex.x = GetEditText(m_HighLightLeft_Edit);
		::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptHighlightTex.x, (LPARAM)DRAGBOX_LEFT);
	}
	SetRadioBtnProperty();
}


void CRadioBtnDlgBar::OnEnChangeEditRbHlTop()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptHighlightTex.y = GetEditText(m_HIghLightTop_Edit);
		::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptHighlightTex.y, (LPARAM)DRAGBOX_TOP);
	}
	SetRadioBtnProperty();
}


void CRadioBtnDlgBar::OnEnChangeEditRbCmLeft()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptCheckMarkTex.x = GetEditText(m_CheckMarkLeftEdit);
		::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptCheckMarkTex.x, (LPARAM)DRAGBOX_LEFT);
	}
	SetRadioBtnProperty();
}


void CRadioBtnDlgBar::OnEnChangeEditRbCmTop()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptCheckMarkTex.y = GetEditText(m_CheckMarkTopEdit);
		::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptCheckMarkTex.y, (LPARAM)DRAGBOX_TOP);
	}
	SetRadioBtnProperty();
}


void CRadioBtnDlgBar::OnEnChangeEditRbBtnWidth()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptBtnSize.x = GetEditText(m_BtnWidthEdit);
		::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.x, (LPARAM)DRAGBOX_WIDTH);
	}
	SetRadioBtnProperty();
}


void CRadioBtnDlgBar::OnEnChangeEditRbBtnHeight()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptBtnSize.y = GetEditText(m_BtnHeightEdit);
		::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.y, (LPARAM)DRAGBOX_HEIGHT);
	}
	SetRadioBtnProperty();
}


void CRadioBtnDlgBar::OnEnChangeEditRadioCol()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptColRow.x = GetEditText(m_RadioBtnColEdit);

		if(m_ptColRow.x > m_ptColRow.y)
		{
			//	가로타입 라디오 버튼
			m_iArrayType = ARRAYTYPE_WIDTH;
		}
		else if(m_ptColRow.x < m_ptColRow.y)
		{
			//	세로타입 라디오 버튼
			m_iArrayType = ARRAYTYPE_HEIGHT;
		}

		CObjMgr::GetInstance()->DeleteAssetBase(m_RadioGroupBase);
		m_RadioGroupBase = NULL;
		SetRadioBtnProperty();
	}
}


void CRadioBtnDlgBar::OnEnChangeEditRadioRow()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptColRow.y = GetEditText(m_RadioBtnRowEdit);

		if(m_ptColRow.x > m_ptColRow.y)
		{
			//	가로타입 라디오 버튼
			m_iArrayType = ARRAYTYPE_WIDTH;
		}
		else if(m_ptColRow.x < m_ptColRow.y)
		{
			//	세로타입 라디오 버튼
			m_iArrayType = ARRAYTYPE_HEIGHT;
		}

		CObjMgr::GetInstance()->DeleteAssetBase(m_RadioGroupBase);
		m_RadioGroupBase = NULL;
		SetRadioBtnProperty();
	}
}


void CRadioBtnDlgBar::OnEnChangeEditRadioInterval()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_iRadioBtnInterval = GetEditText(m_RadioBtnIntervalEdit);
	}
	SetRadioBtnProperty();
}

void CRadioBtnDlgBar::OnBnClickedRadioRbLeftSort()
{
	m_iSortFlag = DT_LEFT | DT_VCENTER | DT_SINGLELINE;
	SetRadioBtnProperty();
}


void CRadioBtnDlgBar::OnBnClickedRadioRbCenterSort()
{
	m_iSortFlag = DT_CENTER | DT_VCENTER | DT_SINGLELINE;
	SetRadioBtnProperty();
}


void CRadioBtnDlgBar::OnBnClickedRadioRbRightSort()
{
	m_iSortFlag = DT_RIGHT | DT_VCENTER | DT_SINGLELINE;
	SetRadioBtnProperty();
}


void CRadioBtnDlgBar::OnEnChangeEditRbInputbox()
{
	UpdateData(TRUE);
	if(CObjMgr::GetInstance()->GetFocusAssetBase() == NULL)
	{
		m_RadioGroupBase = NULL;
	}

	m_TextPreviewEdit->GetWindowText(m_strPreview);

	if(m_RadioGroupBase != NULL)
	{
		//	여기서 텍스트 박스 요소를 세팅
		m_RadioGroupBase->SetTextPreview(m_strPreview, ASSET_RADIOBUTTON);
	}
}


void CRadioBtnDlgBar::OnEnChangeEditRbTextboxinterval()
{
	UpdateData(true);
	m_iTextBoxInterval = GetEditText(m_IntervalEdit);
	SetRadioBtnProperty();
}


void CRadioBtnDlgBar::OnEnChangeEditRbRgbR()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_byRed = GetEditText(m_TextRGB_R_Edit);
		SetRadioBtnProperty();
	}
}


void CRadioBtnDlgBar::OnEnChangeEditRbRgbG()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_byGreen = GetEditText(m_TextRGB_G_Edit);
		SetRadioBtnProperty();
	}
}


void CRadioBtnDlgBar::OnEnChangeEditRbRgbB()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_byBlue = GetEditText(m_TextRGB_B_Edit);
		SetRadioBtnProperty();
	}
}

void CRadioBtnDlgBar::Initialize()
{
	UpdateData(true);

	m_ptNormalTex.x = 0;
	m_ptNormalTex.y = 0;

	m_ptPressedTex.x = 0;
	m_ptPressedTex.y = 0;

	m_ptDisableTex.x = 0;
	m_ptDisableTex.y = 0;

	m_ptHighlightTex.x = 0;
	m_ptHighlightTex.y = 0;

	m_ptCheckMarkTex.x = 0;
	m_ptCheckMarkTex.y = 0;

	m_iTextSize = 12;

	m_ptCurPos.x = 0;
	m_ptCurPos.y = 0;

	m_ptBtnSize.x = 0;
	m_ptBtnSize.y = 0;
	m_ptCheckMarkSize.x = 0;
	m_ptCheckMarkSize.y = 0;
	m_iSelBtn = BTN_NORMAL;
	m_strTexPath = "";
	m_bChange = true;
	m_RadioGroupBase = NULL;
	m_byRed = 0;
	m_byBlue = 0;
	m_byGreen = 0;
	m_iSortFlag = DT_LEFT | DT_SINGLELINE | DT_VCENTER;
	m_iTextBoxInterval = 0;
	m_iRadioBtnInterval = 0;

	m_ptColRow.x = 1;
	m_ptColRow.y = 1;
	m_iArrayType = NOTHING_IN_HERE;


	m_ptIconIndent = m_ptCheckMarkSize;
	m_strIconPath = "";
	m_iIconPosition = ICON_FRONT;

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
	m_CheckMarkLeftEdit->SetWindowText(L"0");
	m_CheckMarkTopEdit->SetWindowText(L"0");
	m_RadioBtnIntervalEdit->SetWindowText(L"0");

	m_NormalRadioBtn->SetCheck(1);
	m_PressedRadioBtn->SetCheck(0);
	m_HighlightRadioBtn->SetCheck(0);
	m_DisableRadioBtn->SetCheck(0);
	m_CheckMarkRadioBtn->SetCheck(0);

	m_TextRGB_R_Edit->SetWindowText(L"0");
	m_TextRGB_G_Edit->SetWindowText(L"0");
	m_TextRGB_B_Edit->SetWindowText(L"0");
	m_RadioBtnColEdit->SetWindowText(L"1");
	m_RadioBtnRowEdit->SetWindowText(L"1");
	m_IntervalEdit->SetWindowText(L"0");
	m_TexturePathEdit->SetWindowText(L"");

	m_RadioSortLeftBtn->SetCheck(1);
	m_RadioSortMidBtn->SetCheck(0);
	m_RadioSortRightBtn->SetCheck(0);

	m_DisableRGB_R_Edit->SetWindowText(L"0");
	m_DisableRGB_G_Edit->SetWindowText(L"0");
	m_DisableRGB_B_Edit->SetWindowText(L"0");
	
	m_TextSizeEdit->SetWindowText(L"12");
	m_IconPathEdit->SetWindowText(L"");
	m_IconHorizontalIndentEdit->SetWindowText(L"0");
	m_IconVerticalIndentEdit->SetWindowText(L"0");

	m_FrontIconRadioBtn->SetCheck(1);
	m_BackIconRadioBtn->SetCheck(0);

	m_FontBoldCheckBox->SetCheck(0);

	m_bChange = true;
}

void CRadioBtnDlgBar::SetRadioBtnProperty()
{
	//	col row의 곱만큼을 에셋을 만들되, m_ptcurPos를 조절하며 포지션을 조절할 것
	if(m_RadioGroupBase == NULL && m_strTexPath != "")
	{
		CreateAsset();
	}

	if(strcmp(m_strTexPath.c_str(),""))
	{
		//	텍스트박스를 제외하고는 전부 적용되도록 수정
		m_RadioGroupBase->SetFilePath(m_strTexPath);
		//	여기서 체크박스와 텍스트박스간의 인터벌을 조정하고
		m_RadioGroupBase->SetGroupBtnProperty(m_ptNormalTex, m_ptPressedTex, m_ptDisableTex, m_ptHighlightTex, m_ptCheckMarkTex, m_ptBtnSize,m_ptCheckMarkSize, m_iTextBoxInterval, ASSET_RADIOBUTTON);
		m_RadioGroupBase->SetText(m_byRed, m_byGreen, m_byBlue, m_iSortFlag, ASSET_RADIOBUTTON);
		//	여기서 버튼간의 인터벌을 조정한다.
		m_RadioGroupBase->SetBtnInterval(m_iRadioBtnInterval);
		m_RadioGroupBase->AdjustGroupBtnPos();
		m_RadioGroupBase->SetDisableText(m_byDisableRed, m_byDisableGreen, m_byDisableBlue);
		m_RadioGroupBase->SetTextSize(m_iTextSize);
		m_RadioGroupBase->SetIconIndent(m_ptIconIndent);
	}
}

//	라디오버튼을 바꾸면 픽쳐윈도우의 width, height도 수정을 해준다.

void CRadioBtnDlgBar::OnBnClickedRadioRbNmBtn()
{
	//	선택되었을때 픽쳐다이알로그의 xpos와 ypos를 수정해주면 될 것 같다
	m_iSelBtn = BTN_NORMAL;
	//	포인터를 보내준 다음 해당 값을 참조해서 복사를 하게 하고 그거를 다이알로그바에 세팅하게 하면
	::SendMessage(m_PictureWnd, WM_CHANGE_RADIOBTN, (WPARAM)m_ptNormalTex.x, (LPARAM)m_ptNormalTex.y);
	::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.x, (LPARAM)DRAGBOX_WIDTH);
	::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.y, (LPARAM)DRAGBOX_HEIGHT);
}


void CRadioBtnDlgBar::OnBnClickedRadioRbPrBtn()
{
	m_iSelBtn = BTN_PRESSED;
	::SendMessage(m_PictureWnd, WM_CHANGE_RADIOBTN, (WPARAM)m_ptPressedTex.x, (LPARAM)m_ptPressedTex.y);
	::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.x, (LPARAM)DRAGBOX_WIDTH);
	::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.y, (LPARAM)DRAGBOX_HEIGHT);
}


void CRadioBtnDlgBar::OnBnClickedRadioRbDaBtn()
{
	m_iSelBtn = BTN_DISABLE;
	::SendMessage(m_PictureWnd, WM_CHANGE_RADIOBTN, (WPARAM)m_ptDisableTex.x, (LPARAM)m_ptDisableTex.y);
	::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.x, (LPARAM)DRAGBOX_WIDTH);
	::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.y, (LPARAM)DRAGBOX_HEIGHT);
}


void CRadioBtnDlgBar::OnBnClickedRadioRbHlBtn()
{
	m_iSelBtn = BTN_HIGHLIGHT;
	::SendMessage(m_PictureWnd, WM_CHANGE_RADIOBTN, (WPARAM)m_ptHighlightTex.x, (LPARAM)m_ptHighlightTex.y);
	::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.x, (LPARAM)DRAGBOX_WIDTH);
	::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.y, (LPARAM)DRAGBOX_HEIGHT);
}


void CRadioBtnDlgBar::OnBnClickedRadioRbCmBtn()
{
	m_iSelBtn = BTN_CHECKMARK;
	::SendMessage(m_PictureWnd, WM_CHANGE_RADIOBTN, (WPARAM)m_ptCheckMarkTex.x, (LPARAM)m_ptCheckMarkTex.y);
	::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptCheckMarkSize.x, (LPARAM)DRAGBOX_WIDTH);
	::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptCheckMarkSize.y, (LPARAM)DRAGBOX_HEIGHT);
}

void CRadioBtnDlgBar::CreateAsset()
{
	if(m_strTexPath != "")
	{
		//	그룹버튼을 만들어서 내부에 버튼배열을 동적할당 해준후, 해당 그룹버튼을 벡터에 집어넣는다.
		m_RadioGroupBase = new CGroupButton;
		m_ptCurPos.x = 0;
		m_ptCurPos.y = 0;

		//	그룹버튼의 내부를 채워줄 라디오 버튼을 세팅한다.
		if( (m_ptColRow.x != 0) && (m_ptColRow.y != 0) )
		{
			m_RadioGroupBase->Create(ASSET_RADIOBUTTON, m_ptColRow.x, m_ptColRow.y);
			for(int i = 0; i < (m_ptColRow.x * m_ptColRow.y); i++)
			{
				//	텍스파일의 위치가 설정되어있지 않으므로 문제가 생기고 있는것
				//	라디오 버튼이 생성이 잘 되고 있지 않은 현상.
				//	텍스파일이 설정되지 않으면 만들어지지 않게 했다.
				CreateRadioBtn();
			}
		}

		m_RadioGroupBase->SetParenthWnd(m_ParenthWnd);

		CObjMgr::GetInstance()->InsertAssetBase(m_RadioGroupBase);
	}
}

void CRadioBtnDlgBar::CreateRadioBtn()
{
	CRadioBtnBase newAsset;
	std::string strAtt = "RADIOBUTTON";

	//	에셋의 포지션을 설정한다
	newAsset.SetPosition(m_ptCurPos);
	newAsset.SetAssetType(ASSET_RADIOBUTTON);
	newAsset.SetFilePath(m_strTexPath);

	//	에셋의 멤버 변수들을 셋팅해준다
	newAsset.SetCheckBoxProperty(m_ptNormalTex, m_ptPressedTex, m_ptDisableTex, m_ptHighlightTex, m_ptCheckMarkTex, m_ptBtnSize, m_ptCheckMarkSize,m_iTextBoxInterval);
	//	만들어진 에셋을 Map에 저장하는 과정
	m_RadioGroupBase->InsertGroupedBtn(&newAsset, m_ParenthWnd);

	m_RadioGroupBase->SetFilePath(m_strTexPath);
	m_RadioGroupBase->SetGroupBtnProperty(m_ptNormalTex, m_ptPressedTex, m_ptDisableTex, m_ptHighlightTex, m_ptCheckMarkTex, m_ptBtnSize, m_ptCheckMarkSize, m_iTextBoxInterval, ASSET_RADIOBUTTON);
	m_RadioGroupBase->SetText(m_byRed, m_byGreen, m_byBlue, m_iSortFlag, ASSET_RADIOBUTTON);

	switch(m_iArrayType)
	{
	case ARRAYTYPE_HEIGHT:
		m_ptCurPos.y = m_ptCurPos.y + m_RadioGroupBase->GetRecentBtnSize().y + m_iRadioBtnInterval;
		break;
	case ARRAYTYPE_WIDTH:
		m_ptCurPos.x = m_ptCurPos.x + m_RadioGroupBase->GetRecentBtnSize().x + m_iRadioBtnInterval;
		break;
	}
}


afx_msg LRESULT CRadioBtnDlgBar::OnChangeDragboxInfo(WPARAM wParam, LPARAM lParam)
{
	//	드래그 박스 인포메이션이 변경되었을 경우.
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
	case BTN_CHECKMARK:
		switch((int)lParam)
		{
		case DRAGBOX_LEFT:
			m_ptCheckMarkTex.x = (int)wParam;
			SetEditWndText(m_CheckMarkLeftEdit, m_ptCheckMarkTex.x);
		case DRAGBOX_TOP:
			m_ptCheckMarkTex.y = (int)wParam;
			SetEditWndText(m_CheckMarkTopEdit, m_ptCheckMarkTex.y);
		}
		break;
	}

	switch((int)lParam)
	{
	case DRAGBOX_WIDTH:
		if(m_iSelBtn == BTN_CHECKMARK)
		{
			m_ptCheckMarkSize.x = (int)wParam;
			m_RadioGroupBase->SetGroupBtnProperty(m_ptNormalTex, m_ptPressedTex, m_ptDisableTex, m_ptHighlightTex,
				m_ptCheckMarkTex, m_ptBtnSize,m_ptCheckMarkSize, m_iTextBoxInterval, ASSET_RADIOBUTTON);
		}
		else
		{
			m_ptBtnSize.x = (int)wParam;
			SetEditWndText(m_BtnWidthEdit, m_ptBtnSize.x);
		}
		break;
	case DRAGBOX_HEIGHT:
		if(m_iSelBtn == BTN_CHECKMARK)
		{
			m_ptCheckMarkSize.y = (int)wParam;
			m_RadioGroupBase->SetGroupBtnProperty(m_ptNormalTex, m_ptPressedTex, m_ptDisableTex, m_ptHighlightTex,
				m_ptCheckMarkTex, m_ptBtnSize,m_ptCheckMarkSize, m_iTextBoxInterval, ASSET_RADIOBUTTON);
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


afx_msg LRESULT CRadioBtnDlgBar::OnSelchangeTexturelist(WPARAM wParam, LPARAM lParam)
{
	UpdateData(true);

	m_strTexPath = (CDBMgr::GetInstance()->GetPathMap()->find((int)wParam)->second);
	CA2T wt(m_strTexPath.c_str());
	m_TexturePathEdit->SetWindowText(wt);

	if(m_RadioGroupBase != NULL)
	{
		m_RadioGroupBase->SetFilePath(m_strTexPath);

	}
	return 0;
}

void CRadioBtnDlgBar::SettingControl( void* _data, eRadioBtnDlgBarControl _controlID )
{
	m_bChange = false;
	UpdateData(true);

	switch(_controlID)
	{
	case RADIOBTNDLGBAR_RGB_R:
		m_byRed = *((BYTE*)_data);
		EditTextSetting(m_byRed, m_TextRGB_R_Edit);
		break;
	case RADIOBTNDLGBAR_RGB_G:
		m_byGreen = *((BYTE*)_data);
		EditTextSetting(m_byGreen, m_TextRGB_G_Edit);
		break;
	case RADIOBTNDLGBAR_RGB_B:
		m_byBlue = *((BYTE*)_data);
		EditTextSetting(m_byBlue, m_TextRGB_B_Edit);
		break;
	case RADIOBTNDLGBAR_INDENT_X:
		m_ptIconIndent.x = *((int*)_data);
		EditTextSetting(m_ptIconIndent.x, m_IconHorizontalIndentEdit);
		break;
	case RADIOBTNDLGBAR_INDENT_Y:
		m_ptIconIndent.y = *((int*)_data);
		EditTextSetting(m_ptIconIndent.y, m_IconVerticalIndentEdit);
		break;
	case RADIOBTNDLGBAR_ICON_PATH:
		{
			m_strIconPath = *((std::string*)_data);
			CString szPath(m_strIconPath.c_str());
			m_IconPathEdit->SetWindowText(szPath);
		}
		break;
	case RADIOBTNDLGBAR_ICON_POSITION:
		{
			bool bIconFront = *((bool*)_data);
			if(bIconFront)
			{
				m_iIconPosition = ICON_FRONT;
				m_FrontIconRadioBtn->SetCheck(1);
				m_BackIconRadioBtn->SetCheck(0);
			}
			else
			{
				m_iIconPosition = ICON_BACK;
				m_FrontIconRadioBtn->SetCheck(0);
				m_BackIconRadioBtn->SetCheck(1);
			}
		}
		break;
	case RADIOBTNDLGBAR_DEFAULT_SHOW:
		{
			bool bDefaultShow = *((bool*)_data);
			if(!bDefaultShow)
			{
				m_DisableStringCheckBox->SetCheck(1);
			}
			else
			{
				m_DisableStringCheckBox->SetCheck(0);
			}
		}

	case RADIOBTNDLGBAR_FONT_BOLD:
		{
			bool bdFontBold = *((bool*)_data);
			if(bdFontBold)
			{
				m_FontBoldCheckBox->SetCheck(1);
			}
			else
			{
				m_FontBoldCheckBox->SetCheck(0);
			}
		}
	}
	m_bChange = true;
}

void CRadioBtnDlgBar::SetMemberRadioGroupBase( CGroupButton* _base )
{
	m_RadioGroupBase = _base;
}

void CRadioBtnDlgBar::SetCheckMarkSize( POINT _size )
{
	m_ptCheckMarkSize.x = _size.x;
	m_ptCheckMarkSize.y = _size.y;
}

void CRadioBtnDlgBar::RefreshDlgBar( CGroupButton* _LoadBase, POINT& _NormalTex )
{
	m_bChange = false;

	SetMemberRadioGroupBase(_LoadBase);

	std::string Path = _LoadBase->GetFilePath();

	m_iRadioBtnInterval = _LoadBase->GetGroupInterval();
	EditTextSetting(m_iRadioBtnInterval, m_RadioBtnIntervalEdit);

	CRadioBtnBase* GroupBase = (CRadioBtnBase*)(_LoadBase->GetGroupBase());
	SetCheckMarkSize(GroupBase[0].GetCheckMarkSize());

	m_ptNormalTex.x = GroupBase[0].GetNormalPos().x;
	EditTextSetting(m_ptNormalTex.x, m_NormalLeft_Edit);
	m_ptNormalTex.y = GroupBase[0].GetNormalPos().y;
	EditTextSetting(m_ptNormalTex.y, m_NormalTop_Edit);
	_NormalTex = m_ptNormalTex;

	m_ptPressedTex.x = GroupBase[0].GetPressedPos().x;
	EditTextSetting(m_ptPressedTex.x, m_PressedLeft_Edit);
	m_ptPressedTex.y = GroupBase[0].GetPressedPos().y;
	EditTextSetting(m_ptPressedTex.y, m_PressedTop_Edit);

	m_ptDisableTex.x = GroupBase[0].GetDisablePos().x;
	EditTextSetting(m_ptDisableTex.x, m_DisableLeft_Edit);
	m_ptDisableTex.y = GroupBase[0].GetDisablePos().y;
	EditTextSetting(m_ptDisableTex.y, m_DisableTop_Edit);

	m_ptHighlightTex.x = GroupBase[0].GetHighlightPos().x;
	EditTextSetting(m_ptHighlightTex.x, m_HighLightLeft_Edit);
	m_ptHighlightTex.y = GroupBase[0].GetHighlightPos().y;
	EditTextSetting(m_ptHighlightTex.y, m_HIghLightTop_Edit);

	//	checkmark refresh
	m_ptCheckMarkTex.x = GroupBase[0].GetCheckMarkPos().x;
	EditTextSetting(m_ptCheckMarkTex.x, m_CheckMarkLeftEdit);
	m_ptCheckMarkTex.y = GroupBase[0].GetCheckMarkPos().y;
	EditTextSetting(m_ptCheckMarkTex.y, m_CheckMarkTopEdit);

	m_ptBtnSize.x = GroupBase[0].GetFileSize().right;
	EditTextSetting(m_ptBtnSize.x, m_BtnWidthEdit);

	m_ptBtnSize.y = GroupBase[0].GetFileSize().bottom;
	EditTextSetting(m_ptBtnSize.y, m_BtnHeightEdit);

	//	focus되어있는 인덱스의 버튼에 접근
	m_byRed = GroupBase[0].GetTextBox().GetRGB(RGB_R);
	EditTextSetting(m_byRed, m_TextRGB_R_Edit);
	m_byGreen = GroupBase[0].GetTextBox().GetRGB(RGB_G);
	EditTextSetting(m_byGreen, m_TextRGB_G_Edit);
	m_byBlue = GroupBase[0].GetTextBox().GetRGB(RGB_B);
	EditTextSetting(m_byBlue, m_TextRGB_B_Edit);

	m_iTextSize = GroupBase[0].GetTextSize();
	EditTextSetting(m_iTextSize, m_TextSizeEdit);

	bool bDefaultShow = GroupBase[0].GetDefaultShow();
	if(!bDefaultShow)
	{
		m_DisableStringCheckBox->SetCheck(1);
	}
	else
	{
		m_DisableStringCheckBox->SetCheck(0);
	}

	bool bdFontBold = GroupBase[0].GetFontBold();
	if(bdFontBold)
	{
		m_FontBoldCheckBox->SetCheck(1);
	}
	else
	{
		m_FontBoldCheckBox->SetCheck(0);
	}

	bool bIconFront = GroupBase[0].GetbIconFront();
	if(bIconFront)
	{
		m_iIconPosition = ICON_FRONT;
		m_FrontIconRadioBtn->SetCheck(1);
		m_BackIconRadioBtn->SetCheck(0);
	}
	else
	{
		m_iIconPosition = ICON_BACK;
		m_FrontIconRadioBtn->SetCheck(0);
		m_BackIconRadioBtn->SetCheck(1);
	}

	m_strIconPath = GroupBase[0].GetIconAssetPath();
	CString strIconPath(m_strIconPath.c_str());
	m_IconPathEdit->SetWindowText(strIconPath);
	if(m_strIconPath != "")	//	라디오 버튼 읽어오면서 에셋 베이스 셋팅
	{
		m_RadioGroupBase->SetIconAsset(((CAssetBase*)(LoadBaseInterface(m_strIconPath, m_ParenthWnd))),
		(eIconPosition)m_iIconPosition, m_strIconPath);
	}

	m_ptIconIndent.x = GroupBase[0].GetIconIndent().x;
	EditTextSetting(m_ptIconIndent.x, m_IconHorizontalIndentEdit);
	m_ptIconIndent.y = GroupBase[0].GetIconIndent().y;
	EditTextSetting(m_ptIconIndent.y, m_IconVerticalIndentEdit);

	m_byDisableRed = GroupBase[0].GetTextBox().GetDisableRGB(RGB_R);
	EditTextSetting(m_byDisableRed, m_DisableRGB_R_Edit);
	m_byDisableGreen = GroupBase[0].GetTextBox().GetDisableRGB(RGB_G);
	EditTextSetting(m_byDisableGreen, m_DisableRGB_G_Edit);
	m_byDisableBlue = GroupBase[0].GetTextBox().GetDisableRGB(RGB_B);
	EditTextSetting(m_byDisableBlue, m_DisableRGB_B_Edit);

	UINT uiSort = GroupBase[0].GetTextBox().GetSortFlag();
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

	m_ptColRow.x = _LoadBase->GetBtnCol();
	EditTextSetting(m_ptColRow.x, m_RadioBtnColEdit);
	m_ptColRow.y = _LoadBase->GetBtnRow();
	EditTextSetting(m_ptColRow.y, m_RadioBtnRowEdit);

	m_iTextBoxInterval = _LoadBase->GetTextBoxInterval();
	EditTextSetting(m_iTextBoxInterval, m_IntervalEdit);

	//	상대적 위치를 고려한 버튼 위치 재설정
	_LoadBase->AdjustGroupBtnPos();
	//	state체크를 위한 핸들 설정
	_LoadBase->SetParenthWnd(m_ParenthWnd);
	_LoadBase->SetParenthWndToGroup();

	//	초기 선택 버튼 지정
	_LoadBase->SetFocusIndex(0);

	m_bChange = true;
}


void CRadioBtnDlgBar::OnEnChangeEditRbDaRgbR()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_byDisableRed = GetEditText(m_DisableRGB_R_Edit);
	}
	SetRadioBtnProperty();
}


void CRadioBtnDlgBar::OnEnChangeEditRbDaRgbG()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_byDisableGreen = GetEditText(m_DisableRGB_G_Edit);
	}
	SetRadioBtnProperty();
}


void CRadioBtnDlgBar::OnEnChangeEditRbDaRgbB()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_byDisableBlue = GetEditText(m_DisableRGB_B_Edit);
	}
	SetRadioBtnProperty();
}

void CRadioBtnDlgBar::OnBnClickedCheckDisablestate()
{
	UpdateData(true);
	if(m_DisableCheckBox->GetCheck() == BST_CHECKED)
	{
		m_RadioGroupBase->SetbDisable(true);
	}
	else if(m_DisableCheckBox->GetCheck() == BST_UNCHECKED)
	{
		m_RadioGroupBase->SetbDisable(false);
	}
}


void CRadioBtnDlgBar::OnEnChangeEditRadTextsize()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_iTextSize = GetEditText(m_TextSizeEdit);
	}
	SetRadioBtnProperty();
}


void CRadioBtnDlgBar::OnEnChangeEditRadIconPath()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_strIconPath = GetEditText(m_IconPathEdit);
		SetRadioBtnProperty();
	}
}

void CRadioBtnDlgBar::OnBnClickedButtonRadLoadIcon()
{
	if(m_bChange)
	{
		UpdateData(true);

		CString strPathName;
		std::string strRai = "rai";
		CA2T wt(strRai.c_str());
		CFileDialog FileDlg(TRUE, wt, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, this);

		TCHAR strInitPath[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, strInitPath);
		// 패스 변경 : Release 
#if defined _DEBUG || defined RELEASE_DEV_PATH
		_tcscat_s(strInitPath, L"\\Asset");
#else
		_tcscat_s(strInitPath, ASSET_FOLDER_PATH);
#endif
		FileDlg.m_ofn.lpstrInitialDir = strInitPath;

		int iResult = FileDlg.DoModal();
		CBaseInterface* newBase = NULL;

		if(IDOK == iResult)
		{
			//	현재 떠있는 Asset들을 클리어하는 함수.
			strPathName = FileDlg.GetPathName();
			CT2CA pszConvertedAnsiString (strPathName);
			std::string _Path(pszConvertedAnsiString);
			//	부모핸들이 제대로 잡히는지를 항상 체크할 것.
			newBase = LoadBaseInterface(_Path, m_ParenthWnd);
		}
		else
		{
			return;
		}

		if(newBase->GetAssetType() != ASSET_PICTURE)
		{
			MessageBox(L"에셋이 제대로 선택되지 않았습니다.\n픽쳐 에셋을 선택해주십시오.");
			SAFE_DELETE(newBase);
		}
		else
		{
			//	패스를 세팅하면서 checkbox의 newbase 포인터를 내부에 넣어주도록 하자.
			POINT ptZero;
			ptZero.x = 0;
			ptZero.y = 0;
			newBase->SetPosition(ptZero);
			CT2CA pszConvertedText (strPathName);
			std::string _strConvertText(pszConvertedText);

			//	여기서 그룹버튼에 에셋아이콘 할당
			m_RadioGroupBase->SetIconAsset((CAssetBase*)newBase, (eIconPosition)m_iIconPosition, _strConvertText);

			m_strIconPath = _strConvertText;
			m_IconPathEdit->SetWindowText(strPathName);
		}
	}
}


void CRadioBtnDlgBar::OnBnClickedRadioRadFrontIcon()
{
	UpdateData(true);
	m_iIconPosition = ICON_FRONT;
	if(m_RadioGroupBase != NULL)
	{
		m_RadioGroupBase->SetIconPosition((eIconPosition)m_iIconPosition);
	}
}


void CRadioBtnDlgBar::OnBnClickedRadioRadBackIcon()
{
	UpdateData(true);
	m_iIconPosition = ICON_BACK;
	if(m_RadioGroupBase != NULL)
	{
		m_RadioGroupBase->SetIconPosition((eIconPosition)m_iIconPosition);
	}
}


void CRadioBtnDlgBar::OnEnChangeEditRadHorizonIndent()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptIconIndent.x = GetEditText(m_IconHorizontalIndentEdit);
		SetRadioBtnProperty();
	}
}


void CRadioBtnDlgBar::OnEnChangeEditRadVerticalIndent()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptIconIndent.y = GetEditText(m_IconVerticalIndentEdit);
		SetRadioBtnProperty();
	}
}


void CRadioBtnDlgBar::OnBnClickedCheckRadDisableString()
{
	UpdateData(true);
	if(m_DisableStringCheckBox->GetCheck() == BST_CHECKED)
	{
		if(m_RadioGroupBase != NULL)
			m_RadioGroupBase->SetDefaultShow(false);
	}
	else if(m_DisableStringCheckBox->GetCheck() == BST_UNCHECKED)
	{
		if(m_RadioGroupBase != NULL)
			m_RadioGroupBase->SetDefaultShow(true);
	}
}

void CRadioBtnDlgBar::OnBnClickedCheckInputFontBold()
{
	UpdateData(true);
	if(m_FontBoldCheckBox->GetCheck() == BST_CHECKED)
	{
		if(m_RadioGroupBase != NULL)
			m_RadioGroupBase->SetFontBold(true);
	}
	else if(m_FontBoldCheckBox->GetCheck() == BST_UNCHECKED)
	{
		if(m_RadioGroupBase != NULL)
			m_RadioGroupBase->SetFontBold(false);
	}
}