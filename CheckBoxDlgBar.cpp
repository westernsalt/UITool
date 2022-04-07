// CheckBoxDlgBar.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "UITool.h"
#include "CheckBoxDlgBar.h"
#include "afxdialogex.h"
#include "ObjMgr.h"
#include "CheckBoxBase.h"
#include "DBMgr.h"

// CCheckBoxDlgBar 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCheckBoxDlgBar, CDialogBar)

	CCheckBoxDlgBar::CCheckBoxDlgBar(CWnd* pParent /*=NULL*/)
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

	m_iTextSize = 12;

	m_ptBtnSize.x = 0;
	m_ptBtnSize.y = 0;
	m_ptCheckMarkSize.x = 0;
	m_ptCheckMarkSize.y = 0;
	m_strTexPath = "";
	m_strIconPath = "";
	m_byRed = 0;
	m_byBlue = 0;
	m_byGreen = 0;
	m_iInterval = 0;
	m_byDisableRed = 0;
	m_byDisableGreen = 0;
	m_byDisableBlue = 0;

	m_iSelBtn = BTN_NORMAL;
	m_iSortFlag = DT_LEFT | DT_SINGLELINE | DT_VCENTER;
	m_iIconPosition = ICON_FRONT;
	m_IconAssetBase = NULL;
	m_IconDisableAssetBase = NULL;
	m_CheckBoxBase = NULL;

	m_ptIconIndent = m_ptCheckMarkSize;

	m_bChange = true;
}

CCheckBoxDlgBar::~CCheckBoxDlgBar()
{
	SAFE_DELETE(m_IconAssetBase);
}

void CCheckBoxDlgBar::OnUpdateCmdUI(CCmdUI* pCmdUI) // 다이얼로그바 버튼 활성화 위해 추가
{
	pCmdUI->Enable(TRUE);
}

void CCheckBoxDlgBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);

	m_TexturePathEdit	= (CEdit*)GetDlgItem(IDC_EDIT_CB_SHOW_TEXTURE_PATH);
	m_NormalLeft_Edit	= (CEdit*)GetDlgItem(IDC_EDIT_CB_NM_LEFT);
	m_NormalTop_Edit	= (CEdit*)GetDlgItem(IDC_EDIT_CB_NM_TOP);
	m_PressedLeft_Edit	= (CEdit*)GetDlgItem(IDC_EDIT_CB_PR_LEFT);
	m_PressedTop_Edit	= (CEdit*)GetDlgItem(IDC_EDIT_CB_PR_TOP);
	m_HighLightLeft_Edit= (CEdit*)GetDlgItem(IDC_EDIT_CB_HL_LEFT);
	m_HIghLightTop_Edit	= (CEdit*)GetDlgItem(IDC_EDIT_CB_HL_TOP);
	m_DisableLeft_Edit	= (CEdit*)GetDlgItem(IDC_EDIT_CB_DA_LEFT);
	m_DisableTop_Edit	= (CEdit*)GetDlgItem(IDC_EDIT_CB_DA_TOP);
	m_BtnWidthEdit		= (CEdit*)GetDlgItem(IDC_EDIT_CB_BTN_WIDTH);
	m_BtnHeightEdit		= (CEdit*)GetDlgItem(IDC_EDIT_CB_BTN_HEIGHT);
	m_CheckMarkLeftEdit	= (CEdit*)GetDlgItem(IDC_EDIT_CB_CM_LEFT);
	m_CheckMarkTopEdit	= (CEdit*)GetDlgItem(IDC_EDIT_CB_CM_TOP);
	m_IntervalEdit		= (CEdit*)GetDlgItem(IDC_EDIT_CB_INTERVAL);

	m_NormalRadioBtn	= (CButton*)GetDlgItem(IDC_RADIO_CB_NM_BTN);
	m_PressedRadioBtn	= (CButton*)GetDlgItem(IDC_RADIO_CB_PR_BTN);
	m_DisableRadioBtn	= (CButton*)GetDlgItem(IDC_RADIO_CB_DA_BTN);
	m_HighlightRadioBtn	= (CButton*)GetDlgItem(IDC_RADIO_CB_HL_BTN);
	m_CheckMarkRadioBtn	= (CButton*)GetDlgItem(IDC_RADIO_CB_CM_BTN);

	m_TextRGB_R_Edit	= (CEdit*)GetDlgItem(IDC_EDIT_CB_TEXT_RED);
	m_TextRGB_G_Edit	= (CEdit*)GetDlgItem(IDC_EDIT_CB_TEXT_GREEN);
	m_TextRGB_B_Edit	= (CEdit*)GetDlgItem(IDC_EDIT_CB_TEXT_BLUE);
	m_TextPreviewEdit	= (CEdit*)GetDlgItem(IDC_EDIT_TEXTPREVIEW);

	m_RadioSortLeftBtn	=(CButton*)GetDlgItem(IDC_RADIO_CB_TEXT_LEFT);
	m_RadioSortMidBtn	=(CButton*)GetDlgItem(IDC_RADIO_CB_TEXT_CENTER);
	m_RadioSortRightBtn	=(CButton*)GetDlgItem(IDC_RADIO_CB_TEXT_RIGHT);

	m_DisableRGB_R_Edit	= (CEdit*)GetDlgItem(IDC_EDIT_CB_DA_TEXT_RED);
	m_DisableRGB_G_Edit	= (CEdit*)GetDlgItem(IDC_EDIT_CB_DA_TEXT_GREEN);
	m_DisableRGB_B_Edit	= (CEdit*)GetDlgItem(IDC_EDIT_CB_DA_TEXT_BLUE);

	m_DisableCheckBox	= (CButton*)GetDlgItem(IDC_CHECK_SHOW_DISABLE_CHK);
	m_TextSizeEdit		= (CEdit*)GetDlgItem(IDC_EDIT_CHK_TEXTSIZE);

	m_NormalIconPathEdit= (CEdit*)GetDlgItem(IDC_EDIT_CHK_ICON_NM_PATH);
	m_DisableIconPathEdit=(CEdit*)GetDlgItem(IDC_EDIT_CHK_ICON_DA_PATH);
	m_FrontIconRadioBtn	= (CButton*)GetDlgItem(IDC_RADIO_CHK_FRONT_ICON);
	m_BackIconRadioBtn	= (CButton*)GetDlgItem(IDC_RADIO_CHK_BACK_ICON);
	m_DisableStringCheckBox = (CButton*)GetDlgItem(IDC_CHECK_CHK_DISABLE_STRING);
	m_FontBoldCheckBox = (CButton*)GetDlgItem(IDC_CHECK_CHK_FONT_BOLD);

	m_IconHorizontalIndentEdit	= (CEdit*)GetDlgItem(IDC_EDIT_CHK_HORIZONTAL_INDENT);
	m_IconVerticalIndentEdit	= (CEdit*)GetDlgItem(IDC_EDIT_CHK_VERTICAL_INDENT);	

	if(CObjMgr::GetInstance()->GetFocusAssetBase() == NULL)
	{
		m_CheckBoxBase = NULL;
		m_IconAssetBase = NULL;
	}
}


BEGIN_MESSAGE_MAP(CCheckBoxDlgBar, CDialogBar)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_CHK_LOAD_NM_ICON, &CCheckBoxDlgBar::OnUpdateCmdUI)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_CHK_LOAD_DA_ICON, &CCheckBoxDlgBar::OnUpdateCmdUI)

	ON_EN_CHANGE(IDC_EDIT_CB_NM_LEFT, &CCheckBoxDlgBar::OnEnChangeEditNmLeft)
	ON_EN_CHANGE(IDC_EDIT_CB_NM_TOP, &CCheckBoxDlgBar::OnEnChangeEditNmTop)
	ON_EN_CHANGE(IDC_EDIT_CB_PR_LEFT, &CCheckBoxDlgBar::OnEnChangeEditPrLeft)
	ON_EN_CHANGE(IDC_EDIT_CB_PR_TOP, &CCheckBoxDlgBar::OnEnChangeEditPrTop)
	ON_EN_CHANGE(IDC_EDIT_CB_DA_LEFT, &CCheckBoxDlgBar::OnEnChangeEditDaLeft)
	ON_EN_CHANGE(IDC_EDIT_CB_DA_TOP, &CCheckBoxDlgBar::OnEnChangeEditDaTop)
	ON_EN_CHANGE(IDC_EDIT_CB_HL_LEFT, &CCheckBoxDlgBar::OnEnChangeEditHlLeft)
	ON_EN_CHANGE(IDC_EDIT_CB_HL_TOP, &CCheckBoxDlgBar::OnEnChangeEditHlTop)
	ON_EN_CHANGE(IDC_EDIT_CB_BTN_WIDTH, &CCheckBoxDlgBar::OnEnChangeEditBtnWidth)
	ON_EN_CHANGE(IDC_EDIT_CB_BTN_HEIGHT, &CCheckBoxDlgBar::OnEnChangeEditBtnHeight)

	ON_BN_CLICKED(IDC_RADIO_CB_NM_BTN, &CCheckBoxDlgBar::OnBnClickedRadioNmBtn)
	ON_BN_CLICKED(IDC_RADIO_CB_PR_BTN, &CCheckBoxDlgBar::OnBnClickedRadioPrBtn)
	ON_BN_CLICKED(IDC_RADIO_CB_DA_BTN, &CCheckBoxDlgBar::OnBnClickedRadioDaBtn)
	ON_BN_CLICKED(IDC_RADIO_CB_HL_BTN, &CCheckBoxDlgBar::OnBnClickedRadioHlBtn)
	ON_MESSAGE(WM_CHANGE_DRAGBOX_INFO, &CCheckBoxDlgBar::OnChangeDragboxInfo)
	ON_MESSAGE(WM_SELCHANGE_TEXTURELIST, &CCheckBoxDlgBar::OnSelchangeTexturelist)
	ON_BN_CLICKED(IDC_RADIO_CB_TEXT_LEFT, &CCheckBoxDlgBar::OnBnClickedRadioBtnTextLeft)
	ON_BN_CLICKED(IDC_RADIO_CB_TEXT_CENTER, &CCheckBoxDlgBar::OnBnClickedRadioBtnTextCenter)
	ON_BN_CLICKED(IDC_RADIO_CB_TEXT_RIGHT, &CCheckBoxDlgBar::OnBnClickedRadioBtnTextRight)
	ON_EN_CHANGE(IDC_EDIT_CB_TEXT_RED, &CCheckBoxDlgBar::OnEnChangeEditBtnTextRed)
	ON_EN_CHANGE(IDC_EDIT_CB_TEXT_GREEN, &CCheckBoxDlgBar::OnEnChangeEditBtnTextGreen)
	ON_EN_CHANGE(IDC_EDIT_CB_TEXT_BLUE, &CCheckBoxDlgBar::OnEnChangeEditBtnTextBlue)
	ON_EN_CHANGE(IDC_EDIT_CB_INTERVAL, &CCheckBoxDlgBar::OnEnChangeEditCbInterval)
	//	체크마크 관련 함수
	ON_BN_CLICKED(IDC_RADIO_CB_CM_BTN, &CCheckBoxDlgBar::OnBnClickedRadioCbCmBtn)
	ON_EN_CHANGE(IDC_EDIT_CB_CM_LEFT, &CCheckBoxDlgBar::OnEnChangeEditCbCmLeft)
	ON_EN_CHANGE(IDC_EDIT_CB_CM_TOP, &CCheckBoxDlgBar::OnEnChangeEditCbCmTop)
	ON_EN_CHANGE(IDC_EDIT_TEXTPREVIEW, &CCheckBoxDlgBar::OnEnChangeEditTextpreview)
	ON_BN_CLICKED(IDC_CHECK_SHOW_DISABLE_CHK, &CCheckBoxDlgBar::OnBnClickedCheckShowDisableChk)
	ON_BN_CLICKED(IDC_CHECK_CHK_FONT_BOLD, &CCheckBoxDlgBar::OnBnClickedFontBold)
	ON_EN_CHANGE(IDC_EDIT_CB_DA_TEXT_RED, &CCheckBoxDlgBar::OnEnChangeEditCbDaTextRed)
	ON_EN_CHANGE(IDC_EDIT_CB_DA_TEXT_GREEN, &CCheckBoxDlgBar::OnEnChangeEditCbDaTextGreen)
	ON_EN_CHANGE(IDC_EDIT_CB_DA_TEXT_BLUE, &CCheckBoxDlgBar::OnEnChangeEditCbDaTextBlue)
	ON_EN_CHANGE(IDC_EDIT_CHK_TEXTSIZE, &CCheckBoxDlgBar::OnEnChangeEditChkTextsize)
	ON_BN_CLICKED(IDC_BUTTON_CHK_LOAD_NM_ICON, &CCheckBoxDlgBar::OnBnClickedButtonChkLoadIcon)
	ON_BN_CLICKED(IDC_RADIO_CHK_BACK_ICON, &CCheckBoxDlgBar::OnBnClickedRadioChkBackIcon)
	ON_BN_CLICKED(IDC_RADIO_CHK_FRONT_ICON, &CCheckBoxDlgBar::OnBnClickedRadioChkFrontIcon)
	ON_EN_CHANGE(IDC_EDIT_CHK_ICON_NM_PATH, &CCheckBoxDlgBar::OnEnChangeEditChkIconPath)
	ON_EN_CHANGE(IDC_EDIT_CHK_HORIZONTAL_INDENT, &CCheckBoxDlgBar::OnEnChangeEditChkHorizontalIndent)
	ON_EN_CHANGE(IDC_EDIT_CHK_VERTICAL_INDENT, &CCheckBoxDlgBar::OnEnChangeEditChkVerticalIndent)	
	
	ON_BN_CLICKED(IDC_BUTTON_CHK_LOAD_DA_ICON, &CCheckBoxDlgBar::OnBnClickedButtonChkLoadDaIcon)
	ON_EN_CHANGE(IDC_EDIT_CHK_ICON_DA_PATH, &CCheckBoxDlgBar::OnEnChangeEditChkIconDaPath)
	ON_BN_CLICKED(IDC_CHECK_CHK_DISABLE_STRING, &CCheckBoxDlgBar::OnBnClickedCheckChkDisableString)
END_MESSAGE_MAP()

void CCheckBoxDlgBar::SetParenthWnd( HWND _hWnd )
{
	m_ParenthWnd = _hWnd;
}

void CCheckBoxDlgBar::SetPictureWnd( HWND _hWnd )
{
	m_PictureWnd = _hWnd;
}

void CCheckBoxDlgBar::Initialize()
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

	m_ptCheckMarkTex.x = 0;
	m_ptCheckMarkTex.y = 0;

	m_ptIconIndent = m_ptCheckMarkSize;

	m_ptBtnSize.x = 0;
	m_ptBtnSize.y = 0;
	m_ptCheckMarkSize.x = 0;
	m_ptCheckMarkSize.y = 0;
	m_iTextSize = 12;
	m_iSelBtn = BTN_NORMAL;
	m_strTexPath = "";
	m_CheckBoxBase = NULL;
	m_IconAssetBase = NULL;
	m_IconDisableAssetBase = NULL;
	m_byRed = 0;
	m_byBlue = 0;
	m_byGreen = 0;
	m_iSortFlag = DT_LEFT | DT_SINGLELINE | DT_VCENTER;
	m_iInterval = 0;
	m_iIconPosition = ICON_FRONT;
	m_strIconPath = "";
	m_strIconDisablePath = "";

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

	m_NormalRadioBtn->SetCheck(1);
	m_PressedRadioBtn->SetCheck(0);
	m_HighlightRadioBtn->SetCheck(0);
	m_DisableRadioBtn->SetCheck(0);
	m_CheckMarkRadioBtn->SetCheck(0);

	m_FrontIconRadioBtn->SetCheck(1);
	m_BackIconRadioBtn->SetCheck(0);
	m_FontBoldCheckBox->SetCheck(0);

	m_TextRGB_R_Edit->SetWindowText(L"0");
	m_TextRGB_G_Edit->SetWindowText(L"0");
	m_TextRGB_B_Edit->SetWindowText(L"0");
	m_IntervalEdit->SetWindowText(L"0");
	m_TexturePathEdit->SetWindowText(L"");
	m_TextSizeEdit->SetWindowText(L"12");

	m_RadioSortLeftBtn->SetCheck(1);
	m_RadioSortMidBtn->SetCheck(0);
	m_RadioSortRightBtn->SetCheck(0);

	m_DisableRGB_R_Edit->SetWindowText(L"0");
	m_DisableRGB_G_Edit->SetWindowText(L"0");
	m_DisableRGB_B_Edit->SetWindowText(L"0");

	m_IconHorizontalIndentEdit->SetWindowText(L"0");
	m_IconVerticalIndentEdit->SetWindowText(L"0");
	m_NormalIconPathEdit->SetWindowText(L"");
	m_DisableIconPathEdit->SetWindowText(L"");

	m_bChange = true;
}
void CCheckBoxDlgBar::OnEnChangeEditNmLeft()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptNormalTex.x = GetEditText(m_NormalLeft_Edit);
		::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptNormalTex.x, (LPARAM)DRAGBOX_LEFT);
	}

	SetCheckBoxProperty();
}


void CCheckBoxDlgBar::OnEnChangeEditNmTop()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptNormalTex.y = GetEditText(m_NormalTop_Edit);
		::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptNormalTex.y, (LPARAM)DRAGBOX_TOP);
	}
	SetCheckBoxProperty();
}


void CCheckBoxDlgBar::OnEnChangeEditPrLeft()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptPressedTex.x = GetEditText(m_PressedLeft_Edit);
		::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptPressedTex.x, (LPARAM)DRAGBOX_LEFT);
	}
	SetCheckBoxProperty();
}


void CCheckBoxDlgBar::OnEnChangeEditPrTop()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptPressedTex.y = GetEditText(m_PressedTop_Edit);
		::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptPressedTex.y, (LPARAM)DRAGBOX_TOP);
	}
	SetCheckBoxProperty();
}


void CCheckBoxDlgBar::OnEnChangeEditDaLeft()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptDisableTex.x = GetEditText(m_DisableLeft_Edit);
		::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptDisableTex.x, (LPARAM)DRAGBOX_LEFT);
	}
	SetCheckBoxProperty();
}


void CCheckBoxDlgBar::OnEnChangeEditDaTop()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptDisableTex.y = GetEditText(m_DisableTop_Edit);
		::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptDisableTex.y, (LPARAM)DRAGBOX_TOP);
	}
	SetCheckBoxProperty();
}


void CCheckBoxDlgBar::OnEnChangeEditHlLeft()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptHighlightTex.x = GetEditText(m_HighLightLeft_Edit);
		::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptHighlightTex.x, (LPARAM)DRAGBOX_LEFT);
	}
	SetCheckBoxProperty();
}


void CCheckBoxDlgBar::OnEnChangeEditHlTop()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptHighlightTex.y = GetEditText(m_HIghLightTop_Edit);
		::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptHighlightTex.y, (LPARAM)DRAGBOX_TOP);
	}
	SetCheckBoxProperty();
}

void CCheckBoxDlgBar::OnEnChangeEditCbCmLeft()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptCheckMarkTex.x = GetEditText(m_CheckMarkLeftEdit);
		::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptCheckMarkTex.x, (LPARAM)DRAGBOX_LEFT);
	}
	SetCheckBoxProperty();
}


void CCheckBoxDlgBar::OnEnChangeEditCbCmTop()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptCheckMarkTex.y = GetEditText(m_CheckMarkTopEdit);
		::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptCheckMarkTex.y, (LPARAM)DRAGBOX_TOP);
	}
	SetCheckBoxProperty();
}



void CCheckBoxDlgBar::OnEnChangeEditBtnWidth()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptBtnSize.x = GetEditText(m_BtnWidthEdit);
		::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.x, (LPARAM)DRAGBOX_WIDTH);
	}
	SetCheckBoxProperty();
}


void CCheckBoxDlgBar::OnEnChangeEditBtnHeight()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptBtnSize.y = GetEditText(m_BtnHeightEdit);
		::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.y, (LPARAM)DRAGBOX_HEIGHT);
	}
	SetCheckBoxProperty();
}

void CCheckBoxDlgBar::OnBnClickedRadioNmBtn()
{
	//	선택되었을때 픽쳐다이알로그의 xpos와 ypos를 수정해주면 될 것 같다
	m_iSelBtn = BTN_NORMAL;
	//	포인터를 보내준 다음 해당 값을 참조해서 복사를 하게 하고 그거를 다이알로그바에 세팅하게 하면
	::SendMessage(m_PictureWnd, WM_CHANGE_RADIOBTN, (WPARAM)m_ptNormalTex.x, (LPARAM)m_ptNormalTex.y);
	::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.x, (LPARAM)DRAGBOX_WIDTH);
	::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.y, (LPARAM)DRAGBOX_HEIGHT);
}


void CCheckBoxDlgBar::OnBnClickedRadioPrBtn()
{
	m_iSelBtn = BTN_PRESSED;
	::SendMessage(m_PictureWnd, WM_CHANGE_RADIOBTN, (WPARAM)m_ptPressedTex.x, (LPARAM)m_ptPressedTex.y);
	::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.x, (LPARAM)DRAGBOX_WIDTH);
	::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.y, (LPARAM)DRAGBOX_HEIGHT);
}


void CCheckBoxDlgBar::OnBnClickedRadioDaBtn()
{
	m_iSelBtn = BTN_DISABLE;
	::SendMessage(m_PictureWnd, WM_CHANGE_RADIOBTN, (WPARAM)m_ptDisableTex.x, (LPARAM)m_ptDisableTex.y);
	::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.x, (LPARAM)DRAGBOX_WIDTH);
	::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.y, (LPARAM)DRAGBOX_HEIGHT);
}


void CCheckBoxDlgBar::OnBnClickedRadioHlBtn()
{
	m_iSelBtn = BTN_HIGHLIGHT;
	::SendMessage(m_PictureWnd, WM_CHANGE_RADIOBTN, (WPARAM)m_ptHighlightTex.x, (LPARAM)m_ptHighlightTex.y);
	::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.x, (LPARAM)DRAGBOX_WIDTH);
	::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.y, (LPARAM)DRAGBOX_HEIGHT);
}

void CCheckBoxDlgBar::OnBnClickedRadioCbCmBtn()
{
	m_iSelBtn = BTN_CHECKMARK;
	::SendMessage(m_PictureWnd, WM_CHANGE_RADIOBTN, (WPARAM)m_ptCheckMarkTex.x, (LPARAM)m_ptCheckMarkTex.y);
	::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptCheckMarkSize.x, (LPARAM)DRAGBOX_WIDTH);
	::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptCheckMarkSize.y, (LPARAM)DRAGBOX_HEIGHT);
}


void CCheckBoxDlgBar::OnBnClickedRadioBtnTextLeft()
{
	m_iSortFlag = DT_LEFT | DT_VCENTER | DT_SINGLELINE;
	SetCheckBoxProperty();
}


void CCheckBoxDlgBar::OnBnClickedRadioBtnTextCenter()
{
	m_iSortFlag = DT_CENTER | DT_VCENTER | DT_SINGLELINE;
	SetCheckBoxProperty();
}


void CCheckBoxDlgBar::OnBnClickedRadioBtnTextRight()
{
	m_iSortFlag = DT_RIGHT | DT_VCENTER | DT_SINGLELINE;
	SetCheckBoxProperty();
}

void CCheckBoxDlgBar::OnEnChangeEditBtnTextRed()
{
	UpdateData(true);
	m_byRed = GetEditText(m_TextRGB_R_Edit);
	SetCheckBoxProperty();
}


void CCheckBoxDlgBar::OnEnChangeEditBtnTextGreen()
{
	UpdateData(true);
	m_byGreen = GetEditText(m_TextRGB_G_Edit);
	SetCheckBoxProperty();
}


void CCheckBoxDlgBar::OnEnChangeEditBtnTextBlue()
{
	UpdateData(true);
	m_byBlue = GetEditText(m_TextRGB_B_Edit);
	SetCheckBoxProperty();
}


void CCheckBoxDlgBar::OnEnChangeEditCbInterval()
{
	UpdateData(true);
	m_iInterval = GetEditText(m_IntervalEdit);
	SetCheckBoxProperty();
}


afx_msg LRESULT CCheckBoxDlgBar::OnChangeDragboxInfo(WPARAM wParam, LPARAM lParam)
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
	case BTN_CHECKMARK:
		switch((int)lParam)
		{
		case DRAGBOX_LEFT:
			m_ptCheckMarkTex.x = (int)wParam;
			SetEditWndText(m_CheckMarkLeftEdit, m_ptCheckMarkTex.x);
			break;
		case DRAGBOX_TOP:
			m_ptCheckMarkTex.y = (int)wParam;
			SetEditWndText(m_CheckMarkTopEdit, m_ptCheckMarkTex.y);
			break;
		}
		break;
	}


	switch((int)lParam)
	{
	case DRAGBOX_WIDTH:
		if(m_iSelBtn == BTN_CHECKMARK)
		{
			m_ptCheckMarkSize.x = (int)wParam;
			m_CheckBoxBase->SetCheckBoxProperty(m_ptNormalTex, m_ptPressedTex, m_ptDisableTex,
				m_ptHighlightTex, m_ptCheckMarkTex, m_ptBtnSize, m_ptCheckMarkSize, m_iInterval);
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
			m_CheckBoxBase->SetCheckBoxProperty(m_ptNormalTex, m_ptPressedTex, m_ptDisableTex,
				m_ptHighlightTex, m_ptCheckMarkTex, m_ptBtnSize, m_ptCheckMarkSize, m_iInterval);
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

afx_msg LRESULT CCheckBoxDlgBar::OnSelchangeTexturelist(WPARAM wParam, LPARAM lParam)
{
	UpdateData(true);

	m_strTexPath = (CDBMgr::GetInstance()->GetPathMap()->find((int)wParam)->second);
	CA2T wt(m_strTexPath.c_str());
	m_TexturePathEdit->SetWindowText(wt);

	if(m_CheckBoxBase != NULL)
	{
		m_CheckBoxBase->SetFilePath(m_strTexPath);
	}
	return 0;
}


void CCheckBoxDlgBar::SetCheckBoxProperty()
{
	if(m_CheckBoxBase == NULL && m_strTexPath != "")
	{
		CreateAsset();
	}

	if(strcmp(m_strTexPath.c_str(), ""))
	{
		m_CheckBoxBase->SetFilePath(m_strTexPath);
		m_CheckBoxBase->SetCheckBoxProperty(m_ptNormalTex, m_ptPressedTex, m_ptDisableTex, m_ptHighlightTex, m_ptCheckMarkTex, m_ptBtnSize, m_ptCheckMarkSize, m_iInterval);
		m_CheckBoxBase->SetText(m_byRed, m_byGreen, m_byBlue, m_iSortFlag);
		m_CheckBoxBase->SetDisableText(m_byDisableRed, m_byDisableGreen, m_byDisableBlue);
		m_CheckBoxBase->SetTextSize(m_iTextSize);
		m_CheckBoxBase->SetIconIndent(m_ptIconIndent);
	}
}

void CCheckBoxDlgBar::CreateAsset()
{
	if(m_strTexPath == "")
	{
		return;
	}
	//	ObjMgr의 Map에 넣기 위한 동적할당, 추후에 Mgr 파괴할 때 Free된다.
	m_CheckBoxBase = new CCheckBoxBase;
	POINT ptZeroPos;
	ptZeroPos.x = 0;
	ptZeroPos.y = 0;

	//	에셋의 포지션을 설정한다
	m_CheckBoxBase->SetPosition(ptZeroPos);
	m_CheckBoxBase->SetAssetType(ASSET_CHECKBOX);
	m_CheckBoxBase->SetFilePath(m_strTexPath);
	m_CheckBoxBase->SetParenthWnd(m_ParenthWnd);

	//	에셋의 멤버 변수들을 셋팅해준다
	((CCheckBoxBase*)m_CheckBoxBase)->SetCheckBoxProperty(m_ptNormalTex, m_ptPressedTex, m_ptDisableTex, m_ptHighlightTex, m_ptCheckMarkTex, m_ptBtnSize, m_ptCheckMarkSize,m_iInterval);

	//	만들어진 에셋을 Map에 저장하는 과정
	CObjMgr::GetInstance()->InsertAssetBase(m_CheckBoxBase);
}

void CCheckBoxDlgBar::SetBoxPosition( int _x, int _y )
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

// CCheckBoxDlgBar 메시지 처리기입니다.



void CCheckBoxDlgBar::OnEnChangeEditTextpreview()
{
	UpdateData(TRUE);
	if(CObjMgr::GetInstance()->GetFocusAssetBase() == NULL)
	{
		m_CheckBoxBase = NULL;
	}

	m_TextPreviewEdit->GetWindowText(m_strPreview);

	if(m_CheckBoxBase != NULL)
	{
		m_CheckBoxBase->GetpTextBox()->SetTextPreview(m_strPreview);
	}
}

void CCheckBoxDlgBar::SetCheckMarkSize( POINT _size )
{
	m_ptCheckMarkSize.x = _size.x;
	m_ptCheckMarkSize.y = _size.y;
}

void CCheckBoxDlgBar::SetCheckBoxbaseNULL()
{
	m_CheckBoxBase = NULL;
}

void CCheckBoxDlgBar::RefreshDlgBar( CCheckBoxBase* _LoadBase, POINT& _NormalTex )
{
	UpdateData(true);
	m_bChange = false;

	m_ptNormalTex.x = _LoadBase->GetNormalPos().x;
	EditTextSetting(m_ptNormalTex.x, m_NormalLeft_Edit);
	m_ptNormalTex.y = _LoadBase->GetNormalPos().y;
	EditTextSetting(m_ptNormalTex.y, m_NormalTop_Edit);

	_NormalTex = m_ptNormalTex;

	m_ptPressedTex.x =_LoadBase->GetPressedPos().x;
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

	m_ptCheckMarkTex.x = _LoadBase->GetCheckMarkPos().x;
	EditTextSetting(m_ptCheckMarkTex.x, m_CheckMarkLeftEdit);
	m_ptCheckMarkTex.y = _LoadBase->GetCheckMarkPos().y;
	EditTextSetting(m_ptCheckMarkTex.y, m_CheckMarkTopEdit);

	//	checkbox Interval
	m_iInterval = _LoadBase->GetInterval();
	EditTextSetting(m_iInterval, m_IntervalEdit);

	m_byRed = _LoadBase->GetTextBox().GetRGB(RGB_R);
	EditTextSetting(m_byRed, m_TextRGB_R_Edit);

	m_byGreen = _LoadBase->GetTextBox().GetRGB(RGB_G);
	EditTextSetting(m_byGreen, m_TextRGB_G_Edit);

	m_byBlue = _LoadBase->GetTextBox().GetRGB(RGB_B);
	EditTextSetting(m_byBlue, m_TextRGB_B_Edit);

	m_byDisableRed = _LoadBase->GetTextBox().GetDisableRGB(RGB_R);
	EditTextSetting(m_byDisableRed, m_DisableRGB_R_Edit);

	m_byDisableGreen = _LoadBase->GetTextBox().GetDisableRGB(RGB_G);
	EditTextSetting(m_byDisableGreen, m_DisableRGB_G_Edit);

	m_byDisableBlue = _LoadBase->GetTextBox().GetDisableRGB(RGB_B);
	EditTextSetting(m_byDisableBlue, m_DisableRGB_B_Edit);

	m_iTextSize = _LoadBase->GetTextSize();
	EditTextSetting(m_iTextSize, m_TextSizeEdit);

	m_ptIconIndent.x = _LoadBase->GetIconIndent().x;
	EditTextSetting(m_ptIconIndent.x, m_IconHorizontalIndentEdit);

	m_ptIconIndent.y = _LoadBase->GetIconIndent().y;
	EditTextSetting(m_ptIconIndent.y, m_IconVerticalIndentEdit);

	m_strIconPath = _LoadBase->GetIconAssetPath();
	CString strIconPath(m_strIconPath.c_str());
	m_NormalIconPathEdit->SetWindowText(strIconPath);

	if(m_strIconPath != "")
		m_IconAssetBase = (CAssetBase*)(LoadBaseInterface(m_strIconPath, m_ParenthWnd));

	m_strIconDisablePath = _LoadBase->GetIconDisablePath();
	CString strIconDiablePath(m_strIconDisablePath.c_str());
	m_DisableIconPathEdit->SetWindowText(strIconDiablePath);

	if(m_strIconDisablePath != "")
		m_IconDisableAssetBase = (CAssetBase*)(LoadBaseInterface(m_strIconDisablePath, m_ParenthWnd));

	bool bIconFront = _LoadBase->GetbIconFront();
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

	SetCheckMarkSize(_LoadBase->GetCheckMarkSize());
	SetMemberBtnBase(_LoadBase);

	m_strTexPath = _LoadBase->GetFilePath();
	CString TexPath(m_strTexPath.c_str());
	m_TexturePathEdit->SetWindowText(TexPath);

	m_bChange = true;
}


void CCheckBoxDlgBar::OnBnClickedCheckShowDisableChk()
{
	UpdateData(true);
	if(m_DisableCheckBox->GetCheck() == BST_CHECKED)
	{
		m_CheckBoxBase->SetbDisable(true);
	}
	else if(m_DisableCheckBox->GetCheck() == BST_UNCHECKED)
	{
		m_CheckBoxBase->SetbDisable(false);
	}
}


void CCheckBoxDlgBar::OnEnChangeEditCbDaTextRed()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_byDisableRed = GetEditText(m_DisableRGB_R_Edit);
	}
	SetCheckBoxProperty();
}


void CCheckBoxDlgBar::OnEnChangeEditCbDaTextGreen()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_byDisableGreen = GetEditText(m_DisableRGB_G_Edit);
	}
	SetCheckBoxProperty();
}


void CCheckBoxDlgBar::OnEnChangeEditCbDaTextBlue()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_byDisableBlue = GetEditText(m_DisableRGB_B_Edit);
	}
	SetCheckBoxProperty();
}


void CCheckBoxDlgBar::OnEnChangeEditChkTextsize()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_iTextSize = GetEditText(m_TextSizeEdit);
	}
	SetCheckBoxProperty();
}


void CCheckBoxDlgBar::OnBnClickedButtonChkLoadIcon()
{
	//	여기서 패스만 셋팅을 해주고, Edit가 Change 상태일때 들어오는 함수에서
	//	읽어온 Base를 체크박스에 셋팅해준다
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
			m_IconAssetBase = (CAssetBase*)newBase;
			m_IconAssetBase->SetAssetType(ASSET_PICTURE);
			CT2CA pszConvertedText (strPathName);
			std::string _strConvertText(pszConvertedText);

			m_strIconPath = _strConvertText;
			m_NormalIconPathEdit->SetWindowText(strPathName);
		}
	}
}


void CCheckBoxDlgBar::OnBnClickedRadioChkBackIcon()
{
	m_iIconPosition = ICON_BACK;

	if(m_CheckBoxBase != NULL && m_strIconPath != "")
	{
		m_CheckBoxBase->SetIconAsset(m_IconAssetBase, (eIconPosition)m_iIconPosition);
		m_CheckBoxBase->SetIconAssetPath(m_strIconPath);
	}
}

void  CCheckBoxDlgBar::OnBnClickedFontBold()
{
	UpdateData(true);
	if(m_FontBoldCheckBox->GetCheck() == BST_CHECKED)
	{
		m_CheckBoxBase->SetFontBold(true);
	}
	else if(m_FontBoldCheckBox->GetCheck() == BST_UNCHECKED)
	{
		m_CheckBoxBase->SetFontBold(false);
	}
}


void CCheckBoxDlgBar::OnBnClickedRadioChkFrontIcon()
{
	m_iIconPosition = ICON_FRONT;

	if(m_CheckBoxBase != NULL && m_strIconPath != "")
	{
		m_CheckBoxBase->SetIconAsset(m_IconAssetBase, (eIconPosition)m_iIconPosition);
		m_CheckBoxBase->SetIconAssetPath(m_strIconPath);
	}
}

CBaseInterface* CCheckBoxDlgBar::LoadBaseInterface( std::string FilePath, HWND _hWnd )
{
	std::ifstream	ifLoad(FilePath.c_str());
	std::string		strInput;	

	CBaseInterface* newAsset = NULL;

	while(!ifLoad.eof())
	{
		//	복수의 입력을 위한 InsertAsset과 SetFilePath의 배치
		//	로드 에셋 데이터
		std::getline(ifLoad, strInput, '\n');
		if(!strcmp(strInput.c_str(), "[END]\t"))
		{
			break;
		}

		if(!strcmp(strInput.c_str(), "[START]"))
		{
			std::getline(ifLoad, strInput, '\t');
			std::getline(ifLoad, strInput, '\n');
			strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());

			eAssetType LoadAssetType = (eAssetType)atoi(strInput.c_str());
			newAsset = MakeAdaptableBase(LoadAssetType);
			newAsset->SetAssetType(LoadAssetType);
		}

		if(newAsset != NULL)
		{
			newAsset->Load(ifLoad);
			newAsset->SetParenthWnd(_hWnd);
		}
	}

	//	newAsset이 널일 경우에는 제대로 셋팅이 되지 않은 것이다
	ifLoad.close();
	return newAsset;
}


void CCheckBoxDlgBar::OnEnChangeEditChkIconPath()
{
	if(m_bChange)
	{
		UpdateData(true);

		if(m_CheckBoxBase != NULL && m_strIconPath != "")
		{
			m_CheckBoxBase->SetIconAsset(m_IconAssetBase, (eIconPosition)m_iIconPosition);
			m_CheckBoxBase->SetIconAssetPath(m_strIconPath);
		}
	}
}


void CCheckBoxDlgBar::OnEnChangeEditChkHorizontalIndent()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptIconIndent.x = GetEditText(m_IconHorizontalIndentEdit);
		SetCheckBoxProperty();
	}
}


void CCheckBoxDlgBar::OnEnChangeEditChkVerticalIndent()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptIconIndent.y = GetEditText(m_IconVerticalIndentEdit);
		SetCheckBoxProperty();
	}
}


void CCheckBoxDlgBar::OnBnClickedButtonChkLoadDaIcon()
{
	//	여기서 패스만 셋팅을 해주고, Edit가 Change 상태일때 들어오는 함수에서
	//	읽어온 Base를 체크박스에 셋팅해준다
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
			m_IconDisableAssetBase = (CAssetBase*)newBase;
			m_IconDisableAssetBase->SetAssetType(ASSET_PICTURE);
			CT2CA pszConvertedText (strPathName);
			std::string _strConvertText(pszConvertedText);

			m_strIconDisablePath = _strConvertText;
			m_DisableIconPathEdit->SetWindowText(strPathName);
		}
	}
}

void CCheckBoxDlgBar::OnEnChangeEditChkIconDaPath()
{
	if(m_bChange)
	{
		UpdateData(true);

		if(m_CheckBoxBase != NULL && m_strIconDisablePath != "")
		{
			m_CheckBoxBase->SetDisableIconAsset(m_IconDisableAssetBase, (eIconPosition)m_iIconPosition);
			m_CheckBoxBase->SetIconDisableAssetPath(m_strIconDisablePath);
		}
	}
}


void CCheckBoxDlgBar::OnBnClickedCheckChkDisableString()
{
	UpdateData(true);

	if(m_DisableStringCheckBox->GetCheck() == BST_CHECKED)
	{
		if(m_CheckBoxBase != NULL)
			m_CheckBoxBase->SetDefaultShow(false);
	}
	else if(m_DisableStringCheckBox->GetCheck() == BST_UNCHECKED)
	{
		if(m_CheckBoxBase != NULL)
			m_CheckBoxBase->SetDefaultShow(true);
	}
}
