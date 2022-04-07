// TextBoxDlgBar.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "UITool.h"
#include "TextBoxDlgBar.h"

#include "ObjMgr.h"
#include "TextBox.h"
#include "AssetBase.h"

// CTextBoxDlgBar

IMPLEMENT_DYNAMIC(CTextBoxDlgBar, CDialogBar)

CTextBoxDlgBar::CTextBoxDlgBar()
{
	m_bRed = 0;
	m_bGreen = 0;
	m_bBlue = 0;
	m_iSortFlag = DT_LEFT | DT_SINGLELINE | DT_VCENTER;
	m_CurTextBox = NULL;
	m_bChange = true;
	m_ptTextIndent.x = 0;
	m_ptTextIndent.y = 0;
	RECT_INITIALIZE(m_tTextBox);
	m_iTextSize = 12;
}

CTextBoxDlgBar::~CTextBoxDlgBar()
{
}

void CTextBoxDlgBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);

	m_TextBoxWidthEdit	= (CEdit*)GetDlgItem(IDC_EDIT_TB_WIDTH);
	m_TextBoxHeightEdit	= (CEdit*)GetDlgItem(IDC_EDIT_TB_HEIGHT);

	m_RedEdit			= (CEdit*)GetDlgItem(IDC_EDIT_RGB_R);
	m_GreenEdit			= (CEdit*)GetDlgItem(IDC_EDIT_RGB_G);
	m_BlueEdit			= (CEdit*)GetDlgItem(IDC_EDIT_RGB_B);

	m_InputBoxEdit		= (CEdit*)GetDlgItem(IDC_EDIT_INPUTBOX);
	m_AlignLeftRadioBtn	= (CButton*)GetDlgItem(IDC_RADIO_LEFT_SORT);
	m_AlignMidRadioBtn	= (CButton*)GetDlgItem(IDC_RADIO_CENTER_SORT);
	m_AlignRightRadioBtn= (CButton*)GetDlgItem(IDC_RADIO_RIGHT_SORT);

	m_TextIndentXEdit	= (CEdit*)GetDlgItem(IDC_EDIT_TXT_INDENT_X);
	m_TextIndentYEdit	= (CEdit*)GetDlgItem(IDC_EDIT_TXT_INDENT_Y);

	m_TextSizeEdit		= (CEdit*)GetDlgItem(IDC_EDIT_TEXT_SCALING);
	m_DisableStringCheckBox = (CButton*)GetDlgItem(IDC_CHECK_TB_DISABLE_STRING);
	m_FontBoldCheckBox = (CButton*)GetDlgItem(IDC_CHECK_RAD_FONT_BOLD);	
	m_TextVerticalIndentEdit = (CEdit*)GetDlgItem(IDC_EDIT_VERTICAL_INDENT);
	if(CObjMgr::GetInstance()->GetFocusAssetBase() == NULL)
	{
		m_CurTextBox = NULL;
	}
}


BEGIN_MESSAGE_MAP(CTextBoxDlgBar, CDialogBar)
	ON_EN_CHANGE(IDC_EDIT_TB_RIGHT,		&CTextBoxDlgBar::OnEnChangeEditTextWidth)
	ON_EN_CHANGE(IDC_EDIT_TB_BOTTOM,	&CTextBoxDlgBar::OnEnChangeEditTextHeight)
	ON_EN_CHANGE(IDC_EDIT_RGB_R,		&CTextBoxDlgBar::OnEnChangeEditRed)
	ON_EN_CHANGE(IDC_EDIT_RGB_G,		&CTextBoxDlgBar::OnEnChangeEditGreen)
	ON_EN_CHANGE(IDC_EDIT_RGB_B,		&CTextBoxDlgBar::OnEnChangeEditBlue)

	ON_EN_CHANGE(IDC_EDIT_INPUTBOX,		&CTextBoxDlgBar::OnEnChangeEditInputbox)
	ON_BN_CLICKED(IDC_RADIO_LEFT_SORT,	&CTextBoxDlgBar::OnBnClickedRadioLeftSort)
	ON_BN_CLICKED(IDC_RADIO_CENTER_SORT,&CTextBoxDlgBar::OnBnClickedRadioMidSort)
	ON_BN_CLICKED(IDC_RADIO_RIGHT_SORT,	&CTextBoxDlgBar::OnBnClickedRadioRightSort)
	ON_EN_CHANGE(IDC_EDIT_TXT_INDENT_X, &CTextBoxDlgBar::OnEnChangeEditTxtIndentX)
	ON_EN_CHANGE(IDC_EDIT_TXT_INDENT_Y, &CTextBoxDlgBar::OnEnChangeEditTxtIndentY)
	ON_EN_CHANGE(IDC_EDIT_TEXT_SCALING, &CTextBoxDlgBar::OnEnChangeEditTextScaling)
	ON_BN_CLICKED(IDC_CHECK_TB_DISABLE_STRING, &CTextBoxDlgBar::OnBnClickedCheckTbDisableString)
	ON_BN_CLICKED(IDC_CHECK_RAD_FONT_BOLD, &CTextBoxDlgBar::OnBnClickedCheckTbFontBold)

	ON_EN_CHANGE(IDC_EDIT_VERTICAL_INDENT, &CTextBoxDlgBar::OnEnChangeEditVerticalIndent)
END_MESSAGE_MAP()


// CTextBoxDlgBar 메시지 처리기입니다.

void CTextBoxDlgBar::OnEnChangeEditTextWidth()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_tTextBox.right = GetEditText(m_TextBoxWidthEdit) + m_tTextBox.left;
		SetAssetTextProperty(); 
	}
}


void CTextBoxDlgBar::OnEnChangeEditTextHeight()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_tTextBox.bottom = GetEditText(m_TextBoxHeightEdit) + m_tTextBox.top;
		SetAssetTextProperty();
	}
}


void CTextBoxDlgBar::OnBnClickedRadioLeftSort()
{
	if(m_bChange)
	{
		m_iSortFlag = DT_LEFT | DT_VCENTER | DT_SINGLELINE;
		SetAssetTextProperty();
	}
}


void CTextBoxDlgBar::OnBnClickedRadioMidSort()
{
	if(m_bChange)
	{
		m_iSortFlag = DT_CENTER | DT_VCENTER | DT_SINGLELINE;
		SetAssetTextProperty();
	}
}


void CTextBoxDlgBar::OnBnClickedRadioRightSort()
{
	if(m_bChange)
	{
		m_iSortFlag = DT_RIGHT | DT_VCENTER | DT_SINGLELINE;
		SetAssetTextProperty();
	}
}


void CTextBoxDlgBar::OnEnChangeEditRed()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_bRed = GetEditText(m_RedEdit);
		SetAssetTextProperty();
	}
}


void CTextBoxDlgBar::OnEnChangeEditGreen()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_bGreen = GetEditText(m_GreenEdit);
		SetAssetTextProperty();
	}
}


void CTextBoxDlgBar::OnEnChangeEditBlue()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_bBlue = GetEditText(m_BlueEdit);
		SetAssetTextProperty();
	}
}

void CTextBoxDlgBar::SetAssetTextProperty()
{
	if(m_CurTextBox == NULL)
	{
		CreateAsset();
	}

	m_CurTextBox->SetTextProperty(m_tTextBox, m_bRed, m_bGreen, m_bBlue, m_iSortFlag);
	m_CurTextBox->SetTextIndent(m_ptTextIndent);
	m_CurTextBox->SetFontSize(m_iTextSize);
	m_CurTextBox->SetVerticalIndent(m_iVerticalIndent);
}

void CTextBoxDlgBar::CreateAsset()
{
	m_bChange = false;
	UpdateData(true);

	//	ObjMgr의 Map에 넣기 위한 동적할당, 추후에 Mgr 파괴할 때 Free된다.
	m_CurTextBox = new CTextBox;

	std::string strPath = "";

	POINT ptInitPos;
	ptInitPos.x = m_tTextBox.left;
	ptInitPos.y = m_tTextBox.top;

	//	에셋의 포지션을 설정한다
	((CTextBox*)m_CurTextBox)->SetPosition(ptInitPos);
	((CTextBox*)m_CurTextBox)->SetAssetType(ASSET_TEXTBOX);

	//	에셋의 멤버 변수들을 셋팅해준다
	((CTextBox*)m_CurTextBox)->SetTextProperty(m_tTextBox,m_bRed, m_bGreen, m_bBlue, m_iSortFlag);

	//	만들어진 에셋을 Map에 저장하는 과정
	CObjMgr::GetInstance()->InsertAssetBase(m_CurTextBox);
	m_bChange = true;
}

void CTextBoxDlgBar::SetViewhWnd(HWND _hWnd)
{
	m_EditorViewhWnd = _hWnd;
}


void CTextBoxDlgBar::OnEnChangeEditInputbox()
{
	UpdateData(TRUE);
	if(CObjMgr::GetInstance()->GetFocusAssetBase() == NULL)
	{
		m_CurTextBox = NULL;
	}

	m_InputBoxEdit->GetWindowText(m_strInputBox);

	if(m_CurTextBox == NULL)
	{
		CreateAsset();
	}

	m_CurTextBox->SetTextPreview(m_strInputBox);
	
}

void CTextBoxDlgBar::EditDataToValue()
{
	//	전체 에디터를 갱신해서 값을 받아온다.
	OnEnChangeEditTextWidth();
	OnEnChangeEditTextHeight();
	OnEnChangeEditRed();
	OnEnChangeEditGreen();
	OnEnChangeEditBlue();
}

void CTextBoxDlgBar::Initialize()
{
	m_bChange = false;
	UpdateData(true);

	m_TextBoxWidthEdit->SetWindowText(L"0");
	m_TextBoxHeightEdit->SetWindowText(L"0");
	m_AlignLeftRadioBtn->SetCheck(1);
	m_AlignMidRadioBtn->SetCheck(0);
	m_AlignRightRadioBtn->SetCheck(0);
	m_DisableStringCheckBox->SetCheck(0);
	m_FontBoldCheckBox->SetCheck(0);

	m_RedEdit->SetWindowText(L"0");
	m_GreenEdit->SetWindowText(L"0");
	m_BlueEdit->SetWindowText(L"0");

	m_TextIndentXEdit->SetWindowText(L"0");
	m_TextIndentYEdit->SetWindowText(L"0");
	m_TextSizeEdit->SetWindowText(L"12");
	m_TextVerticalIndentEdit->SetWindowText(L"0");

	m_bRed = 0;
	m_bGreen = 0;
	m_bBlue = 0;
	m_ptTextIndent.x = 0;
	m_ptTextIndent.y = 0;
	m_iSortFlag = DT_LEFT | DT_SINGLELINE | DT_VCENTER;
	m_CurTextBox = NULL;
	m_bChange = true;
	m_iTextSize = 12;
	RECT_INITIALIZE(m_tTextBox);
	m_bChange = true;

	m_iVerticalIndent = 0;
}

void CTextBoxDlgBar::SetMemberTextBox( CTextBox* _textbox )
{
	m_CurTextBox = _textbox;
}

void CTextBoxDlgBar::RefreshDlgBar(CTextBox* _LoadBase)
{
	m_bChange = false;

	m_tTextBox.right = _LoadBase->GetFileSize().right;
	EditTextSetting(m_tTextBox.right, m_TextBoxWidthEdit);
	m_tTextBox.bottom =_LoadBase->GetFileSize().bottom;
	EditTextSetting(m_tTextBox.bottom, m_TextBoxHeightEdit);

	m_bRed = ((CTextBox*)_LoadBase)->GetRGB(RGB_R);
	EditTextSetting(m_bRed, m_RedEdit);
	m_bGreen = ((CTextBox*)_LoadBase)->GetRGB(RGB_G);
	EditTextSetting(m_bGreen, m_GreenEdit);
	m_bBlue = ((CTextBox*)_LoadBase)->GetRGB(RGB_B);
	EditTextSetting(m_bBlue, m_BlueEdit);

	m_ptTextIndent.x = ((CTextBox*)_LoadBase)->GetTextIndent().x;
	EditTextSetting(m_ptTextIndent.x, m_TextIndentXEdit);
	m_ptTextIndent.y = ((CTextBox*)_LoadBase)->GetTextIndent().y;
	EditTextSetting(m_ptTextIndent.y, m_TextIndentYEdit);

	m_iTextSize = ((CTextBox*)_LoadBase)->GetFontSize();
	EditTextSetting(m_iTextSize, m_TextSizeEdit);

	// 행간 조절 추가 [7/16/2019 Gabrielle]
	EditTextSetting(((CTextBox*)_LoadBase)->GetVerticalIndent(), m_TextVerticalIndentEdit);

	//Sort
	UINT uiSort = ((CTextBox*)_LoadBase)->GetSortFlag();
	if(uiSort == (DT_VCENTER | DT_LEFT | DT_SINGLELINE))
	{
		m_AlignLeftRadioBtn->SetCheck(1);
		m_AlignMidRadioBtn->SetCheck(0);
		m_AlignRightRadioBtn->SetCheck(0);
		m_iSortFlag = DT_LEFT | DT_VCENTER | DT_SINGLELINE;
	}
	else if(uiSort == (DT_VCENTER | DT_CENTER | DT_SINGLELINE))
	{
		m_AlignLeftRadioBtn->SetCheck(0);
		m_AlignMidRadioBtn->SetCheck(1);
		m_AlignRightRadioBtn->SetCheck(0);
		m_iSortFlag = DT_CENTER | DT_VCENTER | DT_SINGLELINE;
	}
	else if(uiSort == (DT_VCENTER | DT_RIGHT | DT_SINGLELINE))
	{
		m_AlignLeftRadioBtn->SetCheck(0);
		m_AlignMidRadioBtn->SetCheck(0);
		m_AlignRightRadioBtn->SetCheck(1);
		m_iSortFlag = DT_RIGHT | DT_VCENTER | DT_SINGLELINE;
	}

	bool bDefaultShow = _LoadBase->GetbDefaultShow();
	if(!bDefaultShow)
	{
		m_DisableStringCheckBox->SetCheck(1);
	}
	else
	{
		m_DisableStringCheckBox->SetCheck(0);
	}

	bool bdFontBold = _LoadBase->GetFontBold();
	if(bdFontBold)
	{
		m_FontBoldCheckBox->SetCheck(1);
	}
	else
	{
		m_FontBoldCheckBox->SetCheck(0);
	}

	SetMemberTextBox((CTextBox*)_LoadBase);
	m_bChange = true;
}


void CTextBoxDlgBar::OnEnChangeEditTxtIndentX()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptTextIndent.x = GetEditText(m_TextIndentXEdit);
		SetAssetTextProperty();
	}
}


void CTextBoxDlgBar::OnEnChangeEditTxtIndentY()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptTextIndent.y = GetEditText(m_TextIndentYEdit);
		SetAssetTextProperty();
	}
}

void CTextBoxDlgBar::OnEnChangeEditTextScaling()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_iTextSize = GetEditText(m_TextSizeEdit);
		SetAssetTextProperty();
	}
}


void CTextBoxDlgBar::OnBnClickedCheckTbDisableString()
{
	UpdateData(true);
	if(m_DisableStringCheckBox->GetCheck() == BST_CHECKED)
	{
		if(m_CurTextBox != NULL)
			m_CurTextBox->SetDefaultShow(false);
	}
	else if(m_DisableStringCheckBox->GetCheck() == BST_UNCHECKED)
	{
		if(m_CurTextBox != NULL)
			m_CurTextBox->SetDefaultShow(true);
	}
}

void CTextBoxDlgBar::OnBnClickedCheckTbFontBold()
{
	UpdateData(true);
	if(m_FontBoldCheckBox->GetCheck() == BST_CHECKED)
	{
		if(m_CurTextBox != NULL)
			m_CurTextBox->SetFontBold(true);
	}
	else if(m_FontBoldCheckBox->GetCheck() == BST_UNCHECKED)
	{
		if(m_CurTextBox != NULL)
			m_CurTextBox->SetFontBold(false);
	}
}

void CTextBoxDlgBar::OnEnChangeEditVerticalIndent()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_iVerticalIndent = GetEditText(m_TextVerticalIndentEdit);
		SetAssetTextProperty();
	}
}


