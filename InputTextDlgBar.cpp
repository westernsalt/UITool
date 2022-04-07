// InputTextDlgBar.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "UITool.h"
#include "InputTextDlgBar.h"
#include "afxdialogex.h"

#include "InputText.h"
#include "ObjMgr.h"
#include "DBMgr.h"

// CInputTextDlgBar 대화 상자입니다.

IMPLEMENT_DYNAMIC(CInputTextDlgBar, CDialogBar)

	CInputTextDlgBar::CInputTextDlgBar(CWnd* pParent /*=NULL*/)
{
	m_tBackgroundStartPos.x = 0;
	m_tBackgroundStartPos.y = 0;

	m_tBackgroundSize.x = 0;
	m_tBackgroundSize.y = 0;

	m_iInterval = 0;
	m_strBackgroundPath = "";
	m_bChange = true;
	m_bPassword = false;
	m_bFontBold = false;
	m_InputTextBase = NULL;
	m_byRed = 0;
	m_byBlue = 0;
	m_byGreen = 0;
	m_iSortFlag = DT_LEFT |DT_SINGLELINE | DT_VCENTER;

	m_byDisableRed = 0;
	m_byDisableBlue = 0;
	m_byDisableGreen = 0;
	m_iTextSize = 12;
}

CInputTextDlgBar::~CInputTextDlgBar()
{
}

void CInputTextDlgBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);

	m_BackgroundPathEdit	= (CEdit*)GetDlgItem(IDC_EDIT_INPUT_BACKGROUND);
	m_BackgroundXPosEdit	= (CEdit*)GetDlgItem(IDC_EDIT_INPUT_XPOS);
	m_BackgroundYPosEdit	= (CEdit*)GetDlgItem(IDC_EDIT_INPUT_YPOS);
	m_BackgroundWidthEdit	= (CEdit*)GetDlgItem(IDC_EDIT_INPUT_WIDTH);;
	m_BackgroundHeightEdit	= (CEdit*)GetDlgItem(IDC_EDIT_INPUT_HEIGHT);
	m_WritingIntervalEdit	= (CEdit*)GetDlgItem(IDC_EDIT_INPUT_INTERVAL);
	m_ColorRedEdit			= (CEdit*)GetDlgItem(IDC_EDIT_INPUT_RGB_R);
	m_ColorGreenEdit		= (CEdit*)GetDlgItem(IDC_EDIT_INPUT_RGB_G);
	m_ColorBlueEdit			= (CEdit*)GetDlgItem(IDC_EDIT_INPUT_RGB_B);
	m_PasswordCheckBox		= (CButton*)GetDlgItem(IDC_CHECK_INPUT_PASSWORD);
	m_FontBoldCheckBox		= (CButton*)GetDlgItem(IDC_CHECK_INPUT_FONT_BOLD);	
	m_RadioSortLeftBtn		= (CButton*)GetDlgItem(IDC_RADIO_INPUT_LEFT_SORT);
	m_RadioSortCenterBtn	= (CButton*)GetDlgItem(IDC_RADIO_INPUT_CENTER_SORT);
	m_RadioSortRightBtn		= (CButton*)GetDlgItem(IDC_RADIO_INPUT_RIGHT_SORT);
	m_DisableRGB_R_Edit		= (CEdit*)GetDlgItem(IDC_EDIT_INPUT_DA_RGB_R);
	m_DisableRGB_G_Edit		= (CEdit*)GetDlgItem(IDC_EDIT_INPUT_DA_RGB_G);
	m_DisableRGB_B_Edit		= (CEdit*)GetDlgItem(IDC_EDIT_INPUT_DA_RGB_B);
	m_DisableCheckBox		= (CButton*)GetDlgItem(IDC_CHECK_INPUT_DISABLESTATE);
	m_TextSizeEdit			= (CEdit*)GetDlgItem(IDC_EDIT_INPUT_TEXTSIZE);

	if(CObjMgr::GetInstance()->GetFocusAssetBase() == NULL)
	{
		m_InputTextBase = NULL;
	}
}


BEGIN_MESSAGE_MAP(CInputTextDlgBar, CDialogBar)
	ON_EN_CHANGE(IDC_EDIT_INPUT_BACKGROUND, &CInputTextDlgBar::OnEnChangeEditInputBackground)
	ON_EN_CHANGE(IDC_EDIT_INPUT_XPOS, &CInputTextDlgBar::OnEnChangeEditInputXpos)
	ON_EN_CHANGE(IDC_EDIT_INPUT_WIDTH, &CInputTextDlgBar::OnEnChangeEditInputWidth)
	ON_EN_CHANGE(IDC_EDIT_INPUT_YPOS, &CInputTextDlgBar::OnEnChangeEditInputYpos)
	ON_EN_CHANGE(IDC_EDIT_INPUT_HEIGHT, &CInputTextDlgBar::OnEnChangeEditInputHeight)
	ON_BN_CLICKED(IDC_CHECK_INPUT_CHECKBOX, &CInputTextDlgBar::OnBnClickedCheckInputCheckbox)
	ON_BN_CLICKED(IDC_CHECK_INPUT_FONT_BOLD, &CInputTextDlgBar::OnBnClickedCheckInputFontBold)
	ON_EN_CHANGE(IDC_EDIT_INPUT_INTERVAL, &CInputTextDlgBar::OnEnChangeEditInputInterval)
	ON_BN_CLICKED(IDC_RADIO_INPUT_LEFT_SORT, &CInputTextDlgBar::OnBnClickedRadioInputLeftSort)
	ON_BN_CLICKED(IDC_RADIO_INPUT_CENTER_SORT, &CInputTextDlgBar::OnBnClickedRadioInputCenterSort)
	ON_BN_CLICKED(IDC_RADIO_INPUT_RIGHT_SORT, &CInputTextDlgBar::OnBnClickedRadioInputRightSort)
	ON_EN_CHANGE(IDC_EDIT_INPUT_RGB_R, &CInputTextDlgBar::OnEnChangeEditInputRgbR)
	ON_EN_CHANGE(IDC_EDIT_INPUT_RGB_G, &CInputTextDlgBar::OnEnChangeEditInputRgbG)
	ON_EN_CHANGE(IDC_EDIT_INPUT_RGB_B, &CInputTextDlgBar::OnEnChangeEditInputRgbB)
	ON_MESSAGE(WM_SELCHANGE_TEXTURELIST, &CInputTextDlgBar::OnSelchangeTexturelist)
	ON_MESSAGE(WM_CHANGE_DRAGBOX_INFO, &CInputTextDlgBar::OnChangeDragboxInfo)
	ON_EN_CHANGE(IDC_EDIT_INPUT_DA_RGB_R, &CInputTextDlgBar::OnEnChangeEditInputDaRgbR)
	ON_EN_CHANGE(IDC_EDIT_INPUT_DA_RGB_G, &CInputTextDlgBar::OnEnChangeEditInputDaRgbG)
	ON_EN_CHANGE(IDC_EDIT_INPUT_DA_RGB_B, &CInputTextDlgBar::OnEnChangeEditInputDaRgbB)
	ON_BN_CLICKED(IDC_CHECK_INPUT_DISABLESTATE, &CInputTextDlgBar::OnBnClickedCheckInputDisablestate)
	ON_EN_CHANGE(IDC_EDIT_INPUT_TEXTSIZE, &CInputTextDlgBar::OnEnChangeEditInputTextsize)
END_MESSAGE_MAP()


// CInputTextDlgBar 메시지 처리기입니다.


void CInputTextDlgBar::OnEnChangeEditInputBackground()
{

}


void CInputTextDlgBar::OnEnChangeEditInputXpos()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_tBackgroundStartPos.x = GetEditText(m_BackgroundXPosEdit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_tBackgroundStartPos.x, (LPARAM)DRAGBOX_LEFT);
	}
	SetInputTextProperty();
}

void CInputTextDlgBar::OnEnChangeEditInputYpos()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_tBackgroundStartPos.y = GetEditText(m_BackgroundYPosEdit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_tBackgroundStartPos.y, (LPARAM)DRAGBOX_TOP);
	}
	SetInputTextProperty();
}

void CInputTextDlgBar::OnEnChangeEditInputWidth()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_tBackgroundSize.x = GetEditText(m_BackgroundWidthEdit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_tBackgroundSize.x, (LPARAM)DRAGBOX_WIDTH);
	}
	SetInputTextProperty();
}

void CInputTextDlgBar::OnEnChangeEditInputHeight()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_tBackgroundSize.y = GetEditText(m_BackgroundHeightEdit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_tBackgroundSize.y, (LPARAM)DRAGBOX_HEIGHT);
	}
	SetInputTextProperty();
}


void CInputTextDlgBar::OnBnClickedCheckInputCheckbox()
{
	if(m_bChange)
	{
		m_bPassword = !m_bPassword;

		if(m_bPassword)
		{
			std::cout << "체크박스가 클릭되어 패스워드 속성이 켜졌습니다." << std::endl;
		}
	}
	SetInputTextProperty();
}

void CInputTextDlgBar::OnBnClickedCheckInputFontBold()
{
	if(m_bChange)
	{
		m_bFontBold = !m_bFontBold;
	}
	SetInputTextProperty();
}

void CInputTextDlgBar::OnEnChangeEditInputInterval()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_iInterval = GetEditText(m_WritingIntervalEdit);
	}
	SetInputTextProperty();
}


void CInputTextDlgBar::OnBnClickedRadioInputLeftSort()
{
	m_iSortFlag = DT_LEFT | DT_VCENTER | DT_SINGLELINE;
	SetInputTextProperty();
}


void CInputTextDlgBar::OnBnClickedRadioInputCenterSort()
{
	m_iSortFlag = DT_CENTER | DT_VCENTER | DT_SINGLELINE;
	SetInputTextProperty();
}


void CInputTextDlgBar::OnBnClickedRadioInputRightSort()
{
	m_iSortFlag = DT_RIGHT | DT_VCENTER | DT_SINGLELINE;
	SetInputTextProperty();
}


void CInputTextDlgBar::OnEnChangeEditInputRgbR()
{
	UpdateData(true);
	m_byRed = GetEditText(m_ColorRedEdit);
	SetInputTextProperty();
}


void CInputTextDlgBar::OnEnChangeEditInputRgbG()
{
	UpdateData(true);
	m_byGreen = GetEditText(m_ColorGreenEdit);
	SetInputTextProperty();
}


void CInputTextDlgBar::OnEnChangeEditInputRgbB()
{
	UpdateData(true);
	m_byBlue = GetEditText(m_ColorBlueEdit);
	SetInputTextProperty();
}

void CInputTextDlgBar::Initialize()
{
	UpdateData(true);

	m_tBackgroundStartPos.x = 0;
	m_tBackgroundStartPos.y = 0;

	m_tBackgroundSize.x = 0;
	m_tBackgroundSize.y = 0;

	m_strBackgroundPath = "";
	m_bPassword = false;
	m_bFontBold = false;
	m_InputTextBase = NULL;
	m_byRed = 0;
	m_byBlue = 0;
	m_byGreen = 0;
	m_iTextSize = 12;
	m_iSortFlag = DT_LEFT |DT_SINGLELINE | DT_VCENTER;

	m_bChange = false;

	m_BackgroundPathEdit->SetWindowText(L"");

	m_BackgroundXPosEdit->SetWindowText(L"0");
	m_BackgroundYPosEdit->SetWindowText(L"0");

	m_BackgroundWidthEdit->SetWindowText(L"0");
	m_BackgroundHeightEdit->SetWindowText(L"0");

	m_WritingIntervalEdit->SetWindowText(L"0");
	m_ColorRedEdit->SetWindowText(L"0");
	m_ColorGreenEdit->SetWindowText(L"0");
	m_ColorBlueEdit->SetWindowText(L"0");

	m_RadioSortLeftBtn->SetCheck(1);
	m_RadioSortCenterBtn->SetCheck(0);
	m_RadioSortRightBtn->SetCheck(0);
	m_PasswordCheckBox->SetCheck(0);
	m_FontBoldCheckBox->SetCheck(0);
	m_TextSizeEdit->SetWindowText(L"12");

	m_DisableRGB_R_Edit->SetWindowText(L"0");
	m_DisableRGB_G_Edit->SetWindowText(L"0");
	m_DisableRGB_B_Edit->SetWindowText(L"0");

	m_FontBoldCheckBox->SetCheck(0);

	m_bChange = true;
}

void CInputTextDlgBar::RefreshDlgBar( CInputText* _LoadBase, POINT& _NormalTex )
{
	m_bChange = false;

	UpdateData(true);
	m_tBackgroundStartPos.x = _LoadBase->GetBackgroundStartPos().x;
	EditTextSetting(m_tBackgroundStartPos.x, m_BackgroundXPosEdit);
	m_tBackgroundStartPos.y = _LoadBase->GetBackgroundStartPos().y;
	EditTextSetting(m_tBackgroundStartPos.y, m_BackgroundYPosEdit);
	_NormalTex = m_tBackgroundStartPos;

	m_tBackgroundSize.x = _LoadBase->GetBackgroundSize().right;
	EditTextSetting(m_tBackgroundSize.x, m_BackgroundWidthEdit);
	m_tBackgroundSize.y = _LoadBase->GetBackgroundSize().bottom;
	EditTextSetting(m_tBackgroundSize.y, m_BackgroundHeightEdit);

	m_byRed =  _LoadBase->GetRGB(RGB_R);
	EditTextSetting(m_byRed, m_ColorRedEdit);

	m_byGreen = _LoadBase->GetRGB(RGB_G);
	EditTextSetting(m_byGreen, m_ColorGreenEdit);

	m_byBlue = _LoadBase->GetRGB(RGB_B);
	EditTextSetting(m_byBlue, m_ColorBlueEdit);

	m_byDisableRed =  _LoadBase->GetDisableRGB(RGB_R);
	EditTextSetting(m_byDisableRed, m_DisableRGB_R_Edit);

	m_byDisableGreen = _LoadBase->GetDisableRGB(RGB_G);
	EditTextSetting(m_byDisableGreen, m_DisableRGB_G_Edit);

	m_byDisableBlue = _LoadBase->GetDisableRGB(RGB_B);
	EditTextSetting(m_byDisableBlue, m_DisableRGB_B_Edit);

	m_iInterval = _LoadBase->GetInterval();
	EditTextSetting(m_iInterval, m_WritingIntervalEdit);

	m_iTextSize = _LoadBase->GetFontSize();
	EditTextSetting(m_iTextSize, m_TextSizeEdit);

	m_bPassword = _LoadBase->GetbPassword();
	m_PasswordCheckBox->SetCheck(m_bPassword);
	m_bFontBold = _LoadBase->GetFontBold();
	m_FontBoldCheckBox->SetCheck(m_bFontBold);

	UINT uiSort = _LoadBase->GetSortFlag();
	if(uiSort == (DT_VCENTER | DT_LEFT | DT_SINGLELINE))
	{
		m_RadioSortLeftBtn->SetCheck(1);
		m_RadioSortCenterBtn->SetCheck(0);
		m_RadioSortRightBtn->SetCheck(0);
		m_iSortFlag = DT_LEFT | DT_VCENTER | DT_SINGLELINE;
	}
	else if(uiSort == (DT_VCENTER | DT_CENTER | DT_SINGLELINE))
	{
		m_RadioSortLeftBtn->SetCheck(0);
		m_RadioSortCenterBtn->SetCheck(1);
		m_RadioSortRightBtn->SetCheck(0);
		m_iSortFlag = DT_CENTER | DT_VCENTER | DT_SINGLELINE;
	}
	else if(uiSort == (DT_VCENTER | DT_RIGHT | DT_SINGLELINE))
	{
		m_RadioSortLeftBtn->SetCheck(0);
		m_RadioSortCenterBtn->SetCheck(0);
		m_RadioSortRightBtn->SetCheck(1);
		m_iSortFlag = DT_RIGHT | DT_VCENTER | DT_SINGLELINE;
	}

	SetMemberInputTextBase(_LoadBase);

	m_strBackgroundPath = _LoadBase->GetFilePath();
	CString TexPath(m_strBackgroundPath.c_str());
	m_BackgroundPathEdit->SetWindowText(TexPath);

	m_bChange = true;
}

afx_msg LRESULT CInputTextDlgBar::OnSelchangeTexturelist(WPARAM wParam, LPARAM lParam)
{
	UpdateData(true);

	m_strBackgroundPath = (CDBMgr::GetInstance()->GetPathMap()->find((int)wParam)->second);
	CA2T wt(m_strBackgroundPath.c_str());
	m_BackgroundPathEdit->SetWindowText(wt);

	if(m_InputTextBase != NULL)
	{
		m_InputTextBase->SetFilePath(m_strBackgroundPath);
	}
	return 0;
}


afx_msg LRESULT CInputTextDlgBar::OnChangeDragboxInfo(WPARAM wParam, LPARAM lParam)
{
	//	크리티컬 세션처럼 쓴다.
	m_bChange = false;

	UpdateData(true);

	switch((int)lParam)
	{
	case DRAGBOX_LEFT:
		m_tBackgroundStartPos.x = (int)wParam;
		SetEditWndText(m_BackgroundXPosEdit, m_tBackgroundStartPos.x);
		break;
	case DRAGBOX_TOP:
		m_tBackgroundStartPos.y = (int)wParam;
		SetEditWndText(m_BackgroundYPosEdit, m_tBackgroundStartPos.y);
		break;
	case DRAGBOX_WIDTH:
		m_tBackgroundSize.x = (int)wParam;
		SetEditWndText(m_BackgroundWidthEdit, m_tBackgroundSize.x);
		break;
	case DRAGBOX_HEIGHT:
		m_tBackgroundSize.y = (int)wParam;
		SetEditWndText(m_BackgroundHeightEdit, m_tBackgroundSize.y);
		break;
	}

	m_bChange = true;
	return 0;
}

void CInputTextDlgBar::CreateAsset()
{
	if(m_InputTextBase == NULL && m_strBackgroundPath != "")
	{
		m_InputTextBase = new CInputText;

		std::string strAtt =  "INPUTTEXT";
		POINT ptZeroPos;
		ptZeroPos.x = 0;
		ptZeroPos.y = 0;

		//	에셋의 포지션을 설정한다
		m_InputTextBase->SetPosition(ptZeroPos);
		m_InputTextBase->SetAssetType(ASSET_INPUTTEXT);
		m_InputTextBase->SetFilePath(m_strBackgroundPath);
		m_InputTextBase->SetParenthWnd(m_pParenthWnd);

		RECT rtTextBox;
		rtTextBox.left = 0;
		rtTextBox.top = 0;
		rtTextBox.right = m_tBackgroundSize.x;
		rtTextBox.bottom = m_tBackgroundSize.y;

		//	에셋의 멤버 변수들을 셋팅해준다
		((CInputText*)m_InputTextBase)->SetParenthWnd(m_pParenthWnd);
		((CInputText*)m_InputTextBase)->SetBackground(m_strBackgroundPath, m_tBackgroundStartPos, m_tBackgroundSize);
		((CInputText*)m_InputTextBase)->SetInputTextProperty(rtTextBox, m_byRed, m_byGreen, m_byBlue, m_iSortFlag);

		((CInputText*)m_InputTextBase)->SetFocus();
		//	만들어진 에셋을 Map에 저장하는 과정
		CObjMgr::GetInstance()->InsertAssetBase(m_InputTextBase);

	}
}

void CInputTextDlgBar::SetInputTextProperty()
{
	CreateAsset();

	if(m_strBackgroundPath != "")
	{
		RECT rtTextBox;
		rtTextBox.left = 0;
		rtTextBox.top = 0;
		rtTextBox.right = m_tBackgroundSize.x;
		rtTextBox.bottom = m_tBackgroundSize.y;

		((CInputText*)m_InputTextBase)->SetInterval(m_iInterval);
		((CInputText*)m_InputTextBase)->SetbPassword(m_bPassword);
		((CInputText*)m_InputTextBase)->SetFontBold(m_bFontBold);		
		((CInputText*)m_InputTextBase)->SetBackground(m_strBackgroundPath, m_tBackgroundStartPos, m_tBackgroundSize);
		((CInputText*)m_InputTextBase)->SetInputTextProperty(rtTextBox, m_byRed, m_byGreen, m_byBlue, m_iSortFlag);
		((CInputText*)m_InputTextBase)->SetDisableTextColor(m_byDisableRed, m_byDisableGreen, m_byDisableBlue);
		((CInputText*)m_InputTextBase)->SetFontSize(m_iTextSize);
	}
}


void CInputTextDlgBar::OnEnChangeEditInputDaRgbR()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_byDisableRed = GetEditText(m_DisableRGB_R_Edit);
	}
	SetInputTextProperty();
}


void CInputTextDlgBar::OnEnChangeEditInputDaRgbG()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_byDisableGreen = GetEditText(m_DisableRGB_G_Edit);
	}
	SetInputTextProperty();
}


void CInputTextDlgBar::OnEnChangeEditInputDaRgbB()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_byDisableBlue = GetEditText(m_DisableRGB_B_Edit);
	}
	SetInputTextProperty();
}


void CInputTextDlgBar::OnBnClickedCheckInputDisablestate()
{
	UpdateData(true);
	if(m_DisableCheckBox->GetCheck() == BST_CHECKED)
	{
		m_InputTextBase->SetbDisable(true);
	}
	else if(m_DisableCheckBox->GetCheck() == BST_UNCHECKED)
	{
		m_InputTextBase->SetbDisable(false);
	}
}


void CInputTextDlgBar::OnEnChangeEditInputTextsize()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_iTextSize = GetEditText(m_TextSizeEdit);
	}
	SetInputTextProperty();
}
