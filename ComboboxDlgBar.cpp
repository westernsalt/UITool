// ComboboxDlgBar.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "UITool.h"
#include "ComboboxDlgBar.h"
#include "afxdialogex.h"

#include "ComboBoxBase.h"
#include "ObjMgr.h"
#include "DBMgr.h"

// CComboboxDlgBar 대화 상자입니다.

IMPLEMENT_DYNAMIC(CComboboxDlgBar, CDialogBar)

	CComboboxDlgBar::CComboboxDlgBar(CWnd* pParent)
{
	m_ptNormalTex.x = 0;
	m_ptNormalTex.y = 0;
	m_ptPressedTex.x = 0;
	m_ptPressedTex.y = 0;
	m_ptDisableTex.x = 0;
	m_ptDisableTex.y = 0;
	m_ptHighlightTex.x = 0;
	m_ptHighlightTex.y = 0;

	m_ptOpenNormalTex.x = 0;
	m_ptOpenNormalTex.y = 0;
	m_ptOpenPressedTex.x = 0;
	m_ptOpenPressedTex.y = 0;
	m_ptOpenDisableTex.x = 0;
	m_ptOpenDisableTex.y = 0;
	m_ptOpenHighlightTex.x = 0;
	m_ptOpenHighlightTex.y = 0;

	m_ptBtnSize.x = 0;
	m_ptBtnSize.y = 0;

	m_iSelBtn = BTN_NORMAL;
	m_byRed = 0;
	m_byBlue = 0;
	m_byGreen = 0;
	m_strPreview = "";
	m_iSortFlag = DT_LEFT | DT_SINGLELINE | DT_VCENTER;

	m_strListBoxPath = "";
	m_bChange = true;
	m_ComboBoxBase = NULL;

	m_byDisableRed = 0;
	m_byDisableBlue = 0;
	m_byDisableGreen = 0;

	m_ptTextIndent.x = 0;
	m_ptTextIndent.y = 0;
}

CComboboxDlgBar::~CComboboxDlgBar()
{
}

void CComboboxDlgBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);

	m_NormalLeft_Edit		= (CEdit*)GetDlgItem(IDC_EDIT_CMB_NM_LEFT);
	m_NormalTop_Edit		= (CEdit*)GetDlgItem(IDC_EDIT_CMB_NM_TOP);
	m_PressedLeft_Edit		= (CEdit*)GetDlgItem(IDC_EDIT_CMB_PR_LEFT);
	m_PressedTop_Edit		= (CEdit*)GetDlgItem(IDC_EDIT_CMB_PR_TOP);
	m_HighLightLeft_Edit	= (CEdit*)GetDlgItem(IDC_EDIT_CMB_HL_LEFT);
	m_HighLightTop_Edit		= (CEdit*)GetDlgItem(IDC_EDIT_CMB_HL_TOP);
	m_DisableLeft_Edit		= (CEdit*)GetDlgItem(IDC_EDIT_CMB_DA_LEFT);
	m_DisableTop_Edit		= (CEdit*)GetDlgItem(IDC_EDIT_CMB_DA_TOP);

	m_NormalRadioBtn		= (CButton*)GetDlgItem(IDC_RADIO_CMB_NM_BTN);
	m_PressedRadioBtn		= (CButton*)GetDlgItem(IDC_RADIO_CMB_PR_BTN);
	m_DisableRadioBtn		= (CButton*)GetDlgItem(IDC_RADIO_CMB_DA_BTN);
	m_HighlightRadioBtn		= (CButton*)GetDlgItem(IDC_RADIO_CMB_HL_BTN);

	m_OpenNormalLeft_Edit	= (CEdit*)GetDlgItem(IDC_EDIT_CMB_OPEN_NM_LEFT);
	m_OpenNormalTop_Edit	= (CEdit*)GetDlgItem(IDC_EDIT_CMB_OPEN_NM_TOP);
	m_OpenPressedLeft_Edit	= (CEdit*)GetDlgItem(IDC_EDIT_CMB_OPEN_PR_LEFT);
	m_OpenPressedTop_Edit	= (CEdit*)GetDlgItem(IDC_EDIT_CMB_OPEN_PR_TOP);
	m_OpenHighLightLeft_Edit= (CEdit*)GetDlgItem(IDC_EDIT_CMB_OPEN_HL_LEFT);
	m_OpenHIghLightTop_Edit	= (CEdit*)GetDlgItem(IDC_EDIT_CMB_OPEN_HL_TOP);
	m_OpenDisableLeft_Edit	= (CEdit*)GetDlgItem(IDC_EDIT_CMB_OPEN_DA_LEFT);
	m_OpenDisableTop_Edit	= (CEdit*)GetDlgItem(IDC_EDIT_CMB_OPEN_DA_TOP);

	m_OpenNormalRadioBtn	= (CButton*)GetDlgItem(IDC_RADIO_CMB_OPEN_NM_BTN);
	m_OpenPressedRadioBtn	= (CButton*)GetDlgItem(IDC_RADIO_CMB_OPEN_PR_BTN);
	m_OpenDisableRadioBtn	= (CButton*)GetDlgItem(IDC_RADIO_CMB_OPEN_DA_BTN);
	m_OpenHighlightRadioBtn	= (CButton*)GetDlgItem(IDC_RADIO_CMB_OPEN_HL_BTN);

	m_RadioSortLeftBtn		= (CButton*)GetDlgItem(IDC_RADIO_CMB_TEXT_LEFT);
	m_RadioSortMidBtn		= (CButton*)GetDlgItem(IDC_RADIO_CMB_TEXT_CENTER);
	m_RadioSortRightBtn		= (CButton*)GetDlgItem(IDC_RADIO_CMB_TEXT_RIGHT);

	m_TextRGB_R_Edit		= (CEdit*)GetDlgItem(IDC_EDIT_CMB_TEXT_RED);
	m_TextRGB_G_Edit		= (CEdit*)GetDlgItem(IDC_EDIT_CMB_TEXT_GREEN);
	m_TextRGB_B_Edit		= (CEdit*)GetDlgItem(IDC_EDIT_CMB_TEXT_BLUE);
	m_TextPreviewEdit		= (CEdit*)GetDlgItem(IDC_EDIT_CMB_TEXTPREVIEW);

	m_BtnWidthEdit			= (CEdit*)GetDlgItem(IDC_EDIT_CMB_BTN_WIDTH);
	m_BtnHeightEdit			= (CEdit*)GetDlgItem(IDC_EDIT_CMB_BTN_HEIGHT);

	m_ListBoxPathEdit		= (CEdit*)GetDlgItem(IDC_EDIT_LISTBOXPATH);
	m_SetListBoxBtn			= (CButton*)GetDlgItem(IDC_BUTTON_SET_LISTBOX);

	m_DisableRGB_R_Edit		= (CEdit*)GetDlgItem(IDC_EDIT_CMB_DA_TEXT_RED);
	m_DisableRGB_G_Edit		= (CEdit*)GetDlgItem(IDC_EDIT_CMB_DA_TEXT_GREEN);
	m_DisableRGB_B_Edit		= (CEdit*)GetDlgItem(IDC_EDIT_CMB_DA_TEXT_BLUE);

	m_DisableCheckBox		= (CButton*)GetDlgItem(IDC_CHECK_CMB_DISABLESTATE);

	m_TextIndentXEdit		= (CEdit*)GetDlgItem(IDC_EDIT_CMB_INDENT_X);
	m_TextIndentYEdit		= (CEdit*)GetDlgItem(IDC_EDIT_CMB_INDENT_Y);

	if(CObjMgr::GetInstance()->GetFocusAssetBase() == NULL)
	{
		m_ComboBoxBase = NULL;
	}
}


BEGIN_MESSAGE_MAP(CComboboxDlgBar, CDialogBar)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_SET_LISTBOX, &CComboboxDlgBar::OnUpdateCmdUI)
	ON_MESSAGE(WM_SELCHANGE_TEXTURELIST, &CComboboxDlgBar::OnSelchangeTexturelist)
	ON_MESSAGE(WM_CHANGE_DRAGBOX_INFO, &CComboboxDlgBar::OnChangeDragboxInfo)
	ON_EN_CHANGE(IDC_EDIT_CMB_NM_LEFT, &CComboboxDlgBar::OnEnChangeEditCmbNmLeft)
	ON_EN_CHANGE(IDC_EDIT_CMB_NM_TOP, &CComboboxDlgBar::OnEnChangeEditCmbNmTop)
	ON_EN_CHANGE(IDC_EDIT_CMB_HL_LEFT, &CComboboxDlgBar::OnEnChangeEditCmbHlLeft)
	ON_EN_CHANGE(IDC_EDIT_CMB_HL_TOP, &CComboboxDlgBar::OnEnChangeEditCmbHlTop)
	ON_EN_CHANGE(IDC_EDIT_CMB_PR_LEFT, &CComboboxDlgBar::OnEnChangeEditCmbPrLeft)
	ON_EN_CHANGE(IDC_EDIT_CMB_PR_TOP, &CComboboxDlgBar::OnEnChangeEditCmbPrTop)
	ON_EN_CHANGE(IDC_EDIT_CMB_DA_LEFT, &CComboboxDlgBar::OnEnChangeEditCmbDaLeft)
	ON_EN_CHANGE(IDC_EDIT_CMB_DA_TOP, &CComboboxDlgBar::OnEnChangeEditCmbDaTop)
	ON_EN_CHANGE(IDC_EDIT_CMB_OPEN_NM_LEFT, &CComboboxDlgBar::OnEnChangeEditCmbOpenNmLeft)
	ON_EN_CHANGE(IDC_EDIT_CMB_OPEN_NM_TOP, &CComboboxDlgBar::OnEnChangeEditCmbOpenNmTop)
	ON_EN_CHANGE(IDC_EDIT_CMB_OPEN_HL_LEFT, &CComboboxDlgBar::OnEnChangeEditCmbOpenHlLeft)
	ON_EN_CHANGE(IDC_EDIT_CMB_OPEN_HL_TOP, &CComboboxDlgBar::OnEnChangeEditCmbOpenHlTop)
	ON_EN_CHANGE(IDC_EDIT_CMB_OPEN_PR_LEFT, &CComboboxDlgBar::OnEnChangeEditCmbOpenPrLeft)
	ON_EN_CHANGE(IDC_EDIT_CMB_OPEN_PR_TOP, &CComboboxDlgBar::OnEnChangeEditCmbOpenPrTop)
	ON_EN_CHANGE(IDC_EDIT_CMB_OPEN_DA_LEFT, &CComboboxDlgBar::OnEnChangeEditCmbOpenDaLeft)
	ON_EN_CHANGE(IDC_EDIT_CMB_OPEN_DA_TOP, &CComboboxDlgBar::OnEnChangeEditCmbOpenDaTop)
	ON_EN_CHANGE(IDC_EDIT_CMB_BTN_WIDTH, &CComboboxDlgBar::OnEnChangeEditCmbBtnWidth)
	ON_EN_CHANGE(IDC_EDIT_CMB_BTN_HEIGHT, &CComboboxDlgBar::OnEnChangeEditCmbBtnHeight)
	ON_BN_CLICKED(IDC_RADIO_CMB_NM_BTN, &CComboboxDlgBar::OnBnClickedRadioCmbNmBtn)
	ON_BN_CLICKED(IDC_RADIO_CMB_HL_BTN, &CComboboxDlgBar::OnBnClickedRadioCmbHlBtn)
	ON_BN_CLICKED(IDC_RADIO_CMB_PR_BTN, &CComboboxDlgBar::OnBnClickedRadioCmbPrBtn)
	ON_BN_CLICKED(IDC_RADIO_CMB_DA_BTN, &CComboboxDlgBar::OnBnClickedRadioCmbDaBtn)
	ON_BN_CLICKED(IDC_RADIO_CMB_OPEN_NM_BTN, &CComboboxDlgBar::OnBnClickedRadioCmbOpenNmBtn)
	ON_BN_CLICKED(IDC_RADIO_CMB_OPEN_HL_BTN, &CComboboxDlgBar::OnBnClickedRadioCmbOpenHlBtn)
	ON_BN_CLICKED(IDC_RADIO_CMB_OPEN_PR_BTN, &CComboboxDlgBar::OnBnClickedRadioCmbOpenPrBtn)
	ON_BN_CLICKED(IDC_RADIO_CMB_OPEN_DA_BTN, &CComboboxDlgBar::OnBnClickedRadioCmbOpenDaBtn)
	ON_BN_CLICKED(IDC_RADIO_CMB_TEXT_LEFT, &CComboboxDlgBar::OnBnClickedRadioCmbTextLeft)
	ON_BN_CLICKED(IDC_RADIO_CMB_TEXT_CENTER, &CComboboxDlgBar::OnBnClickedRadioCmbTextCenter)
	ON_BN_CLICKED(IDC_RADIO_CMB_TEXT_RIGHT, &CComboboxDlgBar::OnBnClickedRadioCmbTextRight)
	ON_EN_CHANGE(IDC_EDIT_CMB_TEXTPREVIEW, &CComboboxDlgBar::OnEnChangeEditCmbTextpreview)
	ON_EN_CHANGE(IDC_EDIT_CMB_TEXT_RED, &CComboboxDlgBar::OnEnChangeEditCmbTextRed)
	ON_EN_CHANGE(IDC_EDIT_CMB_TEXT_GREEN, &CComboboxDlgBar::OnEnChangeEditCmbTextGreen)
	ON_EN_CHANGE(IDC_EDIT_CMB_TEXT_BLUE, &CComboboxDlgBar::OnEnChangeEditCmbTextBlue)
	ON_BN_CLICKED(IDC_BUTTON_SET_LISTBOX, &CComboboxDlgBar::OnBnClickedButtonSetListbox)
	ON_EN_CHANGE(IDC_EDIT_CMB_DA_TEXT_RED, &CComboboxDlgBar::OnEnChangeEditCmbDaTextRed)
	ON_EN_CHANGE(IDC_EDIT_CMB_DA_TEXT_GREEN, &CComboboxDlgBar::OnEnChangeEditCmbDaTextGreen)
	ON_EN_CHANGE(IDC_EDIT_CMB_DA_TEXT_BLUE, &CComboboxDlgBar::OnEnChangeEditCmbDaTextBlue)
	ON_BN_CLICKED(IDC_CHECK_CMB_DISABLESTATE, &CComboboxDlgBar::OnBnClickedCheckCmbDisablestate)
	ON_EN_CHANGE(IDC_EDIT_CMB_INDENT_X, &CComboboxDlgBar::OnEnChangeEditCmbIndentX)
	ON_EN_CHANGE(IDC_EDIT_CMB_INDENT_Y, &CComboboxDlgBar::OnEnChangeEditCmbIndentY)
END_MESSAGE_MAP()

void CComboboxDlgBar::Initialize()
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

	m_ptOpenNormalTex.x = 0;
	m_ptOpenNormalTex.y = 0;
	m_ptOpenPressedTex.x = 0;
	m_ptOpenPressedTex.y = 0;
	m_ptOpenDisableTex.x = 0;
	m_ptOpenDisableTex.y = 0;
	m_ptOpenHighlightTex.x = 0;
	m_ptOpenHighlightTex.y = 0;

	m_ptTextIndent.x = 0;
	m_ptTextIndent.y = 0;

	m_ptBtnSize.x = 0;
	m_ptBtnSize.y = 0;

	m_iSelBtn = BTN_NORMAL;
	m_byRed = 0;
	m_byBlue = 0;
	m_byGreen = 0;
	m_byDisableRed = 0;
	m_byDisableBlue = 0;
	m_byDisableGreen = 0;
	m_strPreview = "";
	m_iSortFlag = DT_LEFT | DT_SINGLELINE | DT_VCENTER;

	m_strListBoxPath = "";

	//	버튼 에디트 설정
	m_NormalLeft_Edit->SetWindowText(L"0");
	m_NormalTop_Edit->SetWindowText(L"0");
	m_PressedLeft_Edit->SetWindowText(L"0");
	m_PressedTop_Edit->SetWindowText(L"0");
	m_HighLightLeft_Edit->SetWindowText(L"0");
	m_HighLightTop_Edit->SetWindowText(L"0");
	m_DisableLeft_Edit->SetWindowText(L"0");
	m_DisableTop_Edit->SetWindowText(L"0");

	m_OpenNormalLeft_Edit->SetWindowText(L"0");
	m_OpenNormalTop_Edit->SetWindowText(L"0");
	m_OpenPressedLeft_Edit->SetWindowText(L"0");
	m_OpenPressedTop_Edit->SetWindowText(L"0");
	m_OpenHighLightLeft_Edit->SetWindowText(L"0");
	m_OpenHIghLightTop_Edit->SetWindowText(L"0");
	m_OpenDisableLeft_Edit->SetWindowText(L"0");
	m_OpenDisableTop_Edit->SetWindowText(L"0");

	m_BtnWidthEdit->SetWindowText(L"0");
	m_BtnHeightEdit->SetWindowText(L"0");

	m_TextIndentXEdit->SetWindowText(L"0");
	m_TextIndentYEdit->SetWindowText(L"0");

	//	라디오 버튼 초기화
	m_NormalRadioBtn->SetCheck(1);
	m_PressedRadioBtn->SetCheck(0);
	m_HighlightRadioBtn->SetCheck(0);
	m_DisableRadioBtn->SetCheck(0);
	m_OpenNormalRadioBtn->SetCheck(0);
	m_OpenPressedRadioBtn->SetCheck(0);
	m_OpenHighlightRadioBtn->SetCheck(0);
	m_OpenDisableRadioBtn->SetCheck(0);

	m_TextRGB_R_Edit->SetWindowText(L"0");
	m_TextRGB_G_Edit->SetWindowText(L"0");
	m_TextRGB_B_Edit->SetWindowText(L"0");
	m_ListBoxPathEdit->SetWindowText(L"");

	m_RadioSortLeftBtn->SetCheck(1);
	m_RadioSortMidBtn->SetCheck(0);
	m_RadioSortRightBtn->SetCheck(0);

	m_DisableRGB_R_Edit->SetWindowText(L"0");
	m_DisableRGB_G_Edit->SetWindowText(L"0");
	m_DisableRGB_B_Edit->SetWindowText(L"0");

	m_bChange = true;
}

void CComboboxDlgBar::SetParenthWnd( HWND _hWnd )
{
	m_ParenthWnd = _hWnd;
}

void CComboboxDlgBar::SetPictureWnd( HWND _hWnd )
{
	m_PictureWnd = _hWnd;
}

void CComboboxDlgBar::CreateAsset()
{
	if(m_ComboBoxBase == NULL)
	{
		m_ComboBoxBase = new CComboBoxBase;
		m_ComboBoxBase->SetParenthWnd(m_ParenthWnd);

		POINT ptZeroPos;
		ptZeroPos.x = 0;
		ptZeroPos.y = 0;

		//	에셋의 포지션을 설정한다
		m_ComboBoxBase->SetPosition(ptZeroPos);
		m_ComboBoxBase->SetAssetType(ASSET_COMBOBOX);
		m_ComboBoxBase->SetFilePath(m_strTexFilePath);
		m_ComboBoxBase->SetParenthWnd(m_ParenthWnd);

		//	만들어진 에셋을 Map에 저장하는 과정
		CObjMgr::GetInstance()->InsertAssetBase(m_ComboBoxBase);
	}
}

void CComboboxDlgBar::SetComboBoxProperty()
{
	if(m_strTexFilePath != "")
	{
		CreateAsset();

		RECT ptSize;
		ptSize.left = 0;
		ptSize.top = 0;
		ptSize.right = m_ptBtnSize.x;
		ptSize.bottom = m_ptBtnSize.y;
		//	에셋의 멤버 변수들을 셋팅해준다

		m_ComboBoxBase->SetNormalBtnProperty(m_ptNormalTex, m_ptPressedTex, m_ptDisableTex,
			m_ptHighlightTex, m_ptBtnSize);
		m_ComboBoxBase->SetOpenBtnProperty(m_ptOpenNormalTex, m_ptOpenPressedTex, m_ptOpenDisableTex,
			m_ptOpenHighlightTex, m_ptBtnSize);
		m_ComboBoxBase->SetComboBoxProperty(m_byRed, m_byGreen, m_byBlue, m_iSortFlag);
		m_ComboBoxBase->SetParenthWnd(m_ParenthWnd);
		m_ComboBoxBase->SetDisableText(m_byDisableRed, m_byDisableGreen, m_byDisableBlue);
		m_ComboBoxBase->SetTextIndent(m_ptTextIndent);
	}
}

void CComboboxDlgBar::RefreshDlgBar( CComboBoxBase* _LoadBase, POINT& _NormalTex )
{
	m_bChange = false;

	SetMemberComboBoxBase(_LoadBase);

	m_ptNormalTex.x = _LoadBase->GetNormalBtn()->GetNormalPos().x;
	EditTextSetting(m_ptNormalTex.x, m_NormalLeft_Edit);
	m_ptNormalTex.y = _LoadBase->GetNormalBtn()->GetNormalPos().y;
	EditTextSetting(m_ptNormalTex.y, m_NormalTop_Edit);
	_NormalTex = m_ptNormalTex;

	m_ptHighlightTex.x = _LoadBase->GetNormalBtn()->GetHighlightPos().x;
	EditTextSetting(m_ptHighlightTex.x, m_HighLightLeft_Edit);
	m_ptHighlightTex.y = _LoadBase->GetNormalBtn()->GetHighlightPos().y;
	EditTextSetting(m_ptHighlightTex.y, m_HighLightTop_Edit);

	m_ptPressedTex.x = _LoadBase->GetNormalBtn()->GetPressedPos().x;
	EditTextSetting(m_ptPressedTex.x, m_PressedLeft_Edit);
	m_ptPressedTex.y = _LoadBase->GetNormalBtn()->GetPressedPos().y;
	EditTextSetting(m_ptPressedTex.y, m_PressedTop_Edit);

	m_ptDisableTex.x = _LoadBase->GetNormalBtn()->GetDisablePos().x;
	EditTextSetting(m_ptDisableTex.x, m_DisableLeft_Edit);
	m_ptDisableTex.y = _LoadBase->GetNormalBtn()->GetDisablePos().y;
	EditTextSetting(m_ptDisableTex.y, m_DisableTop_Edit);

	m_ptOpenNormalTex.x = _LoadBase->GetOpenBtn()->GetNormalPos().x;
	EditTextSetting(m_ptOpenNormalTex.x, m_OpenNormalLeft_Edit);
	m_ptOpenNormalTex.y = _LoadBase->GetOpenBtn()->GetNormalPos().y;
	EditTextSetting(m_ptOpenNormalTex.y, m_OpenNormalTop_Edit);

	m_ptOpenHighlightTex.x = _LoadBase->GetOpenBtn()->GetHighlightPos().x;
	EditTextSetting(m_ptOpenHighlightTex.x, m_OpenHighLightLeft_Edit);
	m_ptOpenHighlightTex.y = _LoadBase->GetOpenBtn()->GetHighlightPos().y;
	EditTextSetting(m_ptOpenHighlightTex.y, m_OpenHIghLightTop_Edit);

	m_ptOpenPressedTex.x = _LoadBase->GetOpenBtn()->GetPressedPos().x;
	EditTextSetting(m_ptOpenPressedTex.x, m_OpenPressedLeft_Edit);
	m_ptOpenPressedTex.y = _LoadBase->GetOpenBtn()->GetPressedPos().y;
	EditTextSetting(m_ptOpenPressedTex.y, m_OpenPressedTop_Edit);

	m_ptOpenDisableTex.x = _LoadBase->GetOpenBtn()->GetDisablePos().x;
	EditTextSetting(m_ptOpenDisableTex.x, m_OpenDisableLeft_Edit);
	m_ptOpenDisableTex.y  = _LoadBase->GetOpenBtn()->GetDisablePos().y;
	EditTextSetting(m_ptOpenDisableTex.y, m_OpenDisableTop_Edit);

	m_ptTextIndent.x = _LoadBase->GetTextIndent().x;
	EditTextSetting(m_ptTextIndent.x, m_TextIndentXEdit);
	m_ptTextIndent.y  = _LoadBase->GetTextIndent().y;
	EditTextSetting(m_ptTextIndent.y, m_TextIndentYEdit);

	m_ptBtnSize.x = _LoadBase->GetNormalBtn()->GetSize().x;
	EditTextSetting(m_ptBtnSize.x, m_BtnWidthEdit);
	m_ptBtnSize.y = _LoadBase->GetNormalBtn()->GetSize().y;
	EditTextSetting(m_ptBtnSize.y, m_BtnHeightEdit);
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

	//	ListBoxPath
	m_strListBoxPath = _LoadBase->GetListBoxPath();
	CString TexPath(m_strListBoxPath.c_str());
	m_ListBoxPathEdit->SetWindowText(TexPath);

	m_strTexFilePath = _LoadBase->GetFilePath();
	m_bChange = true;
}

void CComboboxDlgBar::OnUpdateCmdUI( CCmdUI* pCmdUI )
{
	//	DlgBar에 버튼을 업데이트하기 위한 작업
	pCmdUI->Enable(TRUE);
}

// CComboboxDlgBar 메시지 처리기입니다.

afx_msg LRESULT CComboboxDlgBar::OnSelchangeTexturelist(WPARAM wParam, LPARAM lParam)
{
	UpdateData(true);

	m_strTexFilePath = (CDBMgr::GetInstance()->GetPathMap()->find((int)wParam)->second);
	CA2T wt(m_strTexFilePath.c_str());

	if(m_ComboBoxBase != NULL)
	{
		m_ComboBoxBase->SetFilePath(m_strTexFilePath);

	}
	return 0;
}


afx_msg LRESULT CComboboxDlgBar::OnChangeDragboxInfo(WPARAM wParam, LPARAM lParam)
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
			SetEditWndText(m_HighLightTop_Edit, m_ptHighlightTex.y);
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
		//	여기부터는 오픈용.
	case BTN_OPEN_NORMAL:
		switch((int)lParam)
		{
		case DRAGBOX_LEFT:
			m_ptOpenNormalTex.x = (int)wParam;
			SetEditWndText(m_OpenNormalLeft_Edit, m_ptOpenNormalTex.x);
			break;
		case DRAGBOX_TOP:
			m_ptOpenNormalTex.y = (int)wParam;
			SetEditWndText(m_OpenNormalTop_Edit, m_ptOpenNormalTex.y);
			break;
		}
		break;
	case BTN_OPEN_DISABLE:
		switch((int)lParam)
		{
		case DRAGBOX_LEFT:
			m_ptOpenDisableTex.x = (int)wParam;
			SetEditWndText(m_OpenDisableLeft_Edit, m_ptOpenDisableTex.x);
			break;
		case DRAGBOX_TOP:
			m_ptOpenDisableTex.y = (int)wParam;
			SetEditWndText(m_OpenDisableTop_Edit, m_ptOpenDisableTex.y);
			break;
		}
		break;
	case BTN_OPEN_HIGHLIGHT:
		switch((int)lParam)
		{
		case DRAGBOX_LEFT:
			m_ptOpenHighlightTex.x = (int)wParam;
			SetEditWndText(m_OpenHighLightLeft_Edit, m_ptOpenHighlightTex.x);
			break;
		case DRAGBOX_TOP:
			m_ptOpenHighlightTex.y = (int)wParam;
			SetEditWndText(m_OpenHIghLightTop_Edit, m_ptOpenHighlightTex.y);
			break;
		}
		break;
	case BTN_OPEN_PRESSED:
		switch((int)lParam)
		{
		case DRAGBOX_LEFT:
			m_ptOpenPressedTex.x = (int)wParam;
			SetEditWndText(m_OpenPressedLeft_Edit, m_ptOpenPressedTex.x);
			break;
		case DRAGBOX_TOP:
			m_ptOpenPressedTex.y = (int)wParam;
			SetEditWndText(m_OpenPressedTop_Edit, m_ptOpenPressedTex.y);
			break;
		}
		break;
	}


	switch((int)lParam)
	{
	case DRAGBOX_WIDTH:
		{
			m_ptBtnSize.x = (int)wParam;
			SetEditWndText(m_BtnWidthEdit, m_ptBtnSize.x);
		}
		break;
	case DRAGBOX_HEIGHT:
		{
			m_ptBtnSize.y = (int)wParam;
			SetEditWndText(m_BtnHeightEdit, m_ptBtnSize.y);
		}
		break;
	}

	m_bChange = true;
	return 0;
}


void CComboboxDlgBar::OnEnChangeEditCmbNmLeft()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptNormalTex.x = GetEditText(m_NormalLeft_Edit);
		::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptNormalTex.x, (LPARAM)DRAGBOX_LEFT);
	}
	SetComboBoxProperty();
}


void CComboboxDlgBar::OnEnChangeEditCmbNmTop()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptNormalTex.y = GetEditText(m_NormalTop_Edit);
		::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptNormalTex.y, (LPARAM)DRAGBOX_TOP);
	}
	SetComboBoxProperty();
}


void CComboboxDlgBar::OnEnChangeEditCmbHlLeft()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptHighlightTex.x = GetEditText(m_HighLightLeft_Edit);
		::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptHighlightTex.x, (LPARAM)DRAGBOX_LEFT);
	}
	SetComboBoxProperty();
}


void CComboboxDlgBar::OnEnChangeEditCmbHlTop()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptHighlightTex.y = GetEditText(m_HighLightTop_Edit);
		::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptHighlightTex.y, (LPARAM)DRAGBOX_TOP);
	}
	SetComboBoxProperty();
}


void CComboboxDlgBar::OnEnChangeEditCmbPrLeft()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptPressedTex.x = GetEditText(m_PressedLeft_Edit);
		::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptPressedTex.x, (LPARAM)DRAGBOX_LEFT);
	}
	SetComboBoxProperty();
}


void CComboboxDlgBar::OnEnChangeEditCmbPrTop()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptPressedTex.y = GetEditText(m_PressedTop_Edit);
		::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptPressedTex.y, (LPARAM)DRAGBOX_TOP);
	}
	SetComboBoxProperty();
}


void CComboboxDlgBar::OnEnChangeEditCmbDaLeft()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptDisableTex.x = GetEditText(m_DisableLeft_Edit);
		::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptDisableTex.x, (LPARAM)DRAGBOX_LEFT);
	}
	SetComboBoxProperty();
}


void CComboboxDlgBar::OnEnChangeEditCmbDaTop()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptDisableTex.y = GetEditText(m_DisableTop_Edit);
		::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptDisableTex.y, (LPARAM)DRAGBOX_TOP);
	}
	SetComboBoxProperty();
}


void CComboboxDlgBar::OnEnChangeEditCmbOpenNmLeft()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptOpenNormalTex.x = GetEditText(m_OpenNormalLeft_Edit);
		::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptOpenNormalTex.x, (LPARAM)DRAGBOX_LEFT);
	}
	SetComboBoxProperty();
}


void CComboboxDlgBar::OnEnChangeEditCmbOpenNmTop()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptOpenNormalTex.y = GetEditText(m_OpenNormalTop_Edit);
		::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptOpenNormalTex.y, (LPARAM)DRAGBOX_TOP);
	}
	SetComboBoxProperty();
}


void CComboboxDlgBar::OnEnChangeEditCmbOpenHlLeft()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptOpenHighlightTex.x = GetEditText(m_OpenHighLightLeft_Edit);
		::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptOpenHighlightTex.x, (LPARAM)DRAGBOX_LEFT);
	}
	SetComboBoxProperty();
}


void CComboboxDlgBar::OnEnChangeEditCmbOpenHlTop()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptOpenHighlightTex.y = GetEditText(m_OpenHIghLightTop_Edit);
		::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptOpenHighlightTex.y, (LPARAM)DRAGBOX_TOP);
	}
	SetComboBoxProperty();
}


void CComboboxDlgBar::OnEnChangeEditCmbOpenPrLeft()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptOpenPressedTex.x = GetEditText(m_OpenPressedLeft_Edit);
		::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptOpenPressedTex.x, (LPARAM)DRAGBOX_LEFT);
	}
	SetComboBoxProperty();
}


void CComboboxDlgBar::OnEnChangeEditCmbOpenPrTop()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptOpenPressedTex.y = GetEditText(m_OpenPressedTop_Edit);
		::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptOpenPressedTex.y, (LPARAM)DRAGBOX_TOP);
	}
	SetComboBoxProperty();
}


void CComboboxDlgBar::OnEnChangeEditCmbOpenDaLeft()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptOpenDisableTex.x = GetEditText(m_OpenDisableLeft_Edit);
		::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptOpenDisableTex.x, (LPARAM)DRAGBOX_LEFT);
	}
	SetComboBoxProperty();
}


void CComboboxDlgBar::OnEnChangeEditCmbOpenDaTop()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptOpenDisableTex.y = GetEditText(m_OpenDisableTop_Edit);
		::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptOpenDisableTex.y, (LPARAM)DRAGBOX_TOP);
	}
	SetComboBoxProperty();
}


void CComboboxDlgBar::OnEnChangeEditCmbBtnWidth()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptBtnSize.x = GetEditText(m_BtnWidthEdit);
		::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.x, (LPARAM)DRAGBOX_WIDTH);
	}
	SetComboBoxProperty();
}


void CComboboxDlgBar::OnEnChangeEditCmbBtnHeight()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptBtnSize.y = GetEditText(m_BtnHeightEdit);
		::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.y, (LPARAM)DRAGBOX_HEIGHT);
	}
	SetComboBoxProperty();
}


void CComboboxDlgBar::OnBnClickedRadioCmbNmBtn()
{
	m_iSelBtn = BTN_NORMAL;
	::SendMessage(m_PictureWnd, WM_CHANGE_RADIOBTN, (WPARAM)m_ptNormalTex.x, (LPARAM)m_ptNormalTex.y);
	::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.x, (LPARAM)DRAGBOX_WIDTH);
	::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.y, (LPARAM)DRAGBOX_HEIGHT);
}


void CComboboxDlgBar::OnBnClickedRadioCmbHlBtn()
{
	m_iSelBtn = BTN_HIGHLIGHT;
	::SendMessage(m_PictureWnd, WM_CHANGE_RADIOBTN, (WPARAM)m_ptHighlightTex.x, (LPARAM)m_ptHighlightTex.y);
	::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.x, (LPARAM)DRAGBOX_WIDTH);
	::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.y, (LPARAM)DRAGBOX_HEIGHT);
}


void CComboboxDlgBar::OnBnClickedRadioCmbPrBtn()
{
	m_iSelBtn = BTN_PRESSED;
	::SendMessage(m_PictureWnd, WM_CHANGE_RADIOBTN, (WPARAM)m_ptPressedTex.x, (LPARAM)m_ptPressedTex.y);
	::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.x, (LPARAM)DRAGBOX_WIDTH);
	::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.y, (LPARAM)DRAGBOX_HEIGHT);
}


void CComboboxDlgBar::OnBnClickedRadioCmbDaBtn()
{
	m_iSelBtn = BTN_DISABLE;
	::SendMessage(m_PictureWnd, WM_CHANGE_RADIOBTN, (WPARAM)m_ptDisableTex.x, (LPARAM)m_ptDisableTex.y);
	::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.x, (LPARAM)DRAGBOX_WIDTH);
	::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.y, (LPARAM)DRAGBOX_HEIGHT);
}


void CComboboxDlgBar::OnBnClickedRadioCmbOpenNmBtn()
{
	m_iSelBtn = BTN_OPEN_NORMAL;
	::SendMessage(m_PictureWnd, WM_CHANGE_RADIOBTN, (WPARAM)m_ptOpenNormalTex.x, (LPARAM)m_ptOpenNormalTex.y);
	::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.x, (LPARAM)DRAGBOX_WIDTH);
	::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.y, (LPARAM)DRAGBOX_HEIGHT);
}


void CComboboxDlgBar::OnBnClickedRadioCmbOpenHlBtn()
{
	m_iSelBtn = BTN_OPEN_HIGHLIGHT;
	::SendMessage(m_PictureWnd, WM_CHANGE_RADIOBTN, (WPARAM)m_ptOpenHighlightTex.x, (LPARAM)m_ptOpenHighlightTex.y);
	::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.x, (LPARAM)DRAGBOX_WIDTH);
	::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.y, (LPARAM)DRAGBOX_HEIGHT);
}


void CComboboxDlgBar::OnBnClickedRadioCmbOpenPrBtn()
{
	m_iSelBtn = BTN_OPEN_PRESSED;
	::SendMessage(m_PictureWnd, WM_CHANGE_RADIOBTN, (WPARAM)m_ptOpenPressedTex.x, (LPARAM)m_ptOpenPressedTex.y);
	::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.x, (LPARAM)DRAGBOX_WIDTH);
	::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.y, (LPARAM)DRAGBOX_HEIGHT);
}


void CComboboxDlgBar::OnBnClickedRadioCmbOpenDaBtn()
{
	m_iSelBtn = BTN_OPEN_DISABLE;
	::SendMessage(m_PictureWnd, WM_CHANGE_RADIOBTN, (WPARAM)m_ptOpenDisableTex.x, (LPARAM)m_ptOpenDisableTex.y);
	::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.x, (LPARAM)DRAGBOX_WIDTH);
	::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.y, (LPARAM)DRAGBOX_HEIGHT);
}


void CComboboxDlgBar::OnBnClickedRadioCmbTextLeft()
{
	m_iSortFlag = DT_LEFT | DT_VCENTER | DT_SINGLELINE;
	SetComboBoxProperty();
}


void CComboboxDlgBar::OnBnClickedRadioCmbTextCenter()
{
	m_iSortFlag = DT_CENTER | DT_VCENTER | DT_SINGLELINE;
	SetComboBoxProperty();
}


void CComboboxDlgBar::OnBnClickedRadioCmbTextRight()
{
	m_iSortFlag = DT_RIGHT | DT_VCENTER | DT_SINGLELINE;
	SetComboBoxProperty();
}


void CComboboxDlgBar::OnEnChangeEditCmbTextpreview()
{
	UpdateData(TRUE);

	m_TextPreviewEdit->GetWindowText(m_strPreview);

	if(m_ComboBoxBase != NULL)
	{
		//	텍스트 프리뷰를 여기서 볼수 있게 해줘야한다.
		m_ComboBoxBase->SetTextPreview(m_strPreview);
	}
}


void CComboboxDlgBar::OnEnChangeEditCmbTextRed()
{
	UpdateData(true);
	m_byRed = GetEditText(m_TextRGB_R_Edit);
	SetComboBoxProperty();
}


void CComboboxDlgBar::OnEnChangeEditCmbTextGreen()
{
	UpdateData(true);
	m_byGreen = GetEditText(m_TextRGB_G_Edit);
	SetComboBoxProperty();
}


void CComboboxDlgBar::OnEnChangeEditCmbTextBlue()
{
	UpdateData(true);
	m_byBlue = GetEditText(m_TextRGB_B_Edit);
	SetComboBoxProperty();
}


void CComboboxDlgBar::OnBnClickedButtonSetListbox()
{
	//	리스트박스를 셋팅하는 파일다이알로그를 열어준다.
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

		if(newBase->GetAssetType() != ASSET_LISTBOX)
		{
			MessageBox(L"에셋이 제대로 선택되지 않았습니다.\n리스트 에셋을 선택해주십시오.");
		}
		else
		{
			//	여기에서 생성해서 자동으로 세팅해주게 하자.
			SetComboBoxProperty();

			if(m_ComboBoxBase != NULL)
			{
				m_ComboBoxBase->SetListBoxBase((CListBoxBase*)newBase);

				CT2CA pszConvertedText (strPathName);
				std::string _strConvertText(pszConvertedText);

				m_strListBoxPath = _strConvertText;
				m_ListBoxPathEdit->SetWindowText(strPathName);
			}
		}
	}
}

CBaseInterface* CComboboxDlgBar::LoadBaseInterface( std::string FilePath, HWND _hWnd )
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


void CComboboxDlgBar::OnEnChangeEditCmbDaTextRed()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_byDisableRed = GetEditText(m_DisableRGB_R_Edit);
	}
	SetComboBoxProperty();
}


void CComboboxDlgBar::OnEnChangeEditCmbDaTextGreen()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_byDisableGreen = GetEditText(m_DisableRGB_G_Edit);
	}
	SetComboBoxProperty();
}


void CComboboxDlgBar::OnEnChangeEditCmbDaTextBlue()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_byDisableBlue = GetEditText(m_DisableRGB_B_Edit);
	}
	SetComboBoxProperty();
}


void CComboboxDlgBar::OnBnClickedCheckCmbDisablestate()
{
	UpdateData(true);
	if(m_DisableCheckBox->GetCheck() == BST_CHECKED)
	{
		if(m_ComboBoxBase != NULL)
			m_ComboBoxBase->SetbDisable(true);
	}
	else if(m_DisableCheckBox->GetCheck() == BST_UNCHECKED)
	{
		if(m_ComboBoxBase != NULL)
			m_ComboBoxBase->SetbDisable(false);
	}
}


void CComboboxDlgBar::OnEnChangeEditCmbIndentX()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptTextIndent.x = GetEditText(m_TextIndentXEdit);
	}
	SetComboBoxProperty();
}


void CComboboxDlgBar::OnEnChangeEditCmbIndentY()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptTextIndent.y = GetEditText(m_TextIndentYEdit);
	}
	SetComboBoxProperty();
}
