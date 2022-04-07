// TabBtnDlgBar.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "UITool.h"
#include "TabBtnDlgBar.h"
#include "afxdialogex.h"

#include "TabBtnBase.h"
#include "GroupButton.h"
#include "ObjMgr.h"
#include "DBMgr.h"

// TabBtnDlgBar 대화 상자입니다.
IMPLEMENT_DYNAMIC(CTabBtnDlgBar, CDialogBar)

CTabBtnDlgBar::CTabBtnDlgBar(CWnd* pParent /*=NULL*/)	
{
	m_ptNormalTex.x = 0;
	m_ptNormalTex.y = 0;

	m_ptPressedTex.x = 0;
	m_ptPressedTex.y = 0;

	m_ptDisableTex.x = 0;
	m_ptDisableTex.y = 0;

	m_ptHighlightTex.x = 0;
	m_ptHighlightTex.y = 0;

	m_ptCurPos.x = 0;
	m_ptCurPos.y = 0;

	m_ptBtnSize.x = 0;
	m_ptBtnSize.y = 0;

	m_iSelBtn = BTN_NORMAL;
	m_strTexPath = "";
	m_bChange = true;
	m_TabGroupBase = NULL;
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
}

CTabBtnDlgBar::~CTabBtnDlgBar()
{
	SAFE_DELETE(m_TabGroupBase);
}

void CTabBtnDlgBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);

	m_TexturePathEdit	= (CEdit*)GetDlgItem(IDC_EDIT_TB_SHOW_TEXTURE_PATH);
	m_NormalLeft_Edit	= (CEdit*)GetDlgItem(IDC_EDIT_TB_NM_LEFT);
	m_NormalTop_Edit	= (CEdit*)GetDlgItem(IDC_EDIT_TB_NM_TOP);
	m_HighLightLeft_Edit= (CEdit*)GetDlgItem(IDC_EDIT_TB_HL_LEFT);
	m_HIghLightTop_Edit	= (CEdit*)GetDlgItem(IDC_EDIT_TB_HL_TOP);
	m_PressedLeft_Edit	= (CEdit*)GetDlgItem(IDC_EDIT_TB_PR_LEFT);
	m_PressedTop_Edit	= (CEdit*)GetDlgItem(IDC_EDIT_TB_PR_TOP);	
	m_DisableLeft_Edit	= (CEdit*)GetDlgItem(IDC_EDIT_TB_DA_LEFT);
	m_DisableTop_Edit	= (CEdit*)GetDlgItem(IDC_EDIT_TB_DA_TOP);
	m_BtnWidthEdit		= (CEdit*)GetDlgItem(IDC_EDIT_TB_BTN_WIDTH);
	m_BtnHeightEdit		= (CEdit*)GetDlgItem(IDC_EDIT_TB_BTN_HEIGHT);
	m_IntervalEdit		= (CEdit*)GetDlgItem(IDC_EDIT_TB_INTERVAL);
	m_RadioBtnColEdit	= (CEdit*)GetDlgItem(IDC_EDIT_TB_COL);
	m_RadioBtnRowEdit	= (CEdit*)GetDlgItem(IDC_EDIT_TB_ROW);
	m_RadioBtnIntervalEdit	= (CEdit*)GetDlgItem(IDC_EDIT_TB_INTERVAL);
	m_NormalRadioBtn	= (CButton*)GetDlgItem(IDC_RADIO_TB_NM_BTN);
	m_HighlightRadioBtn	= (CButton*)GetDlgItem(IDC_RADIO_TB_HL_BTN);
	m_PressedRadioBtn	= (CButton*)GetDlgItem(IDC_RADIO_TB_PR_BTN);
	m_DisableRadioBtn	= (CButton*)GetDlgItem(IDC_RADIO_TB_DA_BTN);	
	m_TextRGB_R_Edit	= (CEdit*)GetDlgItem(IDC_EDIT_TB_RGB_R);
	m_TextRGB_G_Edit	= (CEdit*)GetDlgItem(IDC_EDIT_TB_RGB_G);
	m_TextRGB_B_Edit	= (CEdit*)GetDlgItem(IDC_EDIT_TB_RGB_B);
	m_TextPreviewEdit	= (CEdit*)GetDlgItem(IDC_EDIT_TB_INPUTBOX);
	m_RadioSortLeftBtn	= (CButton*)GetDlgItem(IDC_RADIO_TB_LEFT_SORT);
	m_RadioSortMidBtn	= (CButton*)GetDlgItem(IDC_RADIO_TB_CENTER_SORT);
	m_RadioSortRightBtn	= (CButton*)GetDlgItem(IDC_RADIO_TB_RIGHT_SORT);
	m_DisableRGB_R_Edit	= (CEdit*)GetDlgItem(IDC_EDIT_TB_DA_RGB_R);
	m_DisableRGB_G_Edit	= (CEdit*)GetDlgItem(IDC_EDIT_TB_DA_RGB_G);
	m_DisableRGB_B_Edit	= (CEdit*)GetDlgItem(IDC_EDIT_TB_DA_RGB_B);
	m_DisableCheckBox	= (CButton*)GetDlgItem(IDC_TB_CHECK_DISABLESTATE);
	m_BtnTextVerticalIndentEdit = (CEdit*)GetDlgItem(IDC_EDIT_TABBTN_VERTICAL_INDENT);
}

void CTabBtnDlgBar::Initialize()
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

	m_ptCurPos.x = 0;
	m_ptCurPos.y = 0;
	m_iSelBtn = BTN_NORMAL;
	m_strTexPath = "";
	m_bChange = true;
	m_TabGroupBase = NULL;
	m_byRed = 0;
	m_byBlue = 0;
	m_byGreen = 0;
	m_iSortFlag = DT_LEFT | DT_SINGLELINE | DT_VCENTER;
	m_iTextBoxInterval = 0;
	m_iRadioBtnInterval = 0;

	m_ptColRow.x = 1;
	m_ptColRow.y = 1;
	m_iArrayType = NOTHING_IN_HERE;

	m_NormalLeft_Edit->SetWindowText(L"0");
	m_NormalTop_Edit->SetWindowText(L"0");
	m_PressedLeft_Edit->SetWindowText(L"0");
	m_PressedTop_Edit->SetWindowText(L"0");
	m_HighLightLeft_Edit->SetWindowText(L"0");
	m_HIghLightTop_Edit->SetWindowText(L"0");
	m_DisableLeft_Edit->SetWindowText(L"0");
	m_DisableTop_Edit->SetWindowText(L"0");
	m_RadioBtnIntervalEdit->SetWindowText(L"0");

	m_NormalRadioBtn->SetCheck(1);
	m_PressedRadioBtn->SetCheck(0);
	m_HighlightRadioBtn->SetCheck(0);
	m_DisableRadioBtn->SetCheck(0);

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

	m_bChange = true;
}

void CTabBtnDlgBar::SetMemberTabGroupBase(CGroupButton* _base)
{
	m_TabGroupBase = _base;
}

void CTabBtnDlgBar::SetTabBtnProperty()
{
	//	col row의 곱만큼을 에셋을 만들되, m_ptcurPos를 조절하며 포지션을 조절할 것
	if(m_TabGroupBase == NULL && m_strTexPath != "")
	{
		CreateAsset();
	}

	if(strcmp(m_strTexPath.c_str(), ""))
	{
		POINT ptTemp;
		ptTemp.x = 0;
		ptTemp.y = 0;

		//	텍스트박스를 제외하고는 전부 적용되도록 수정
		m_TabGroupBase->SetFilePath(m_strTexPath);
		//	여기서 체크박스와 텍스트박스간의 인터벌을 조정하고
		m_TabGroupBase->SetGroupBtnProperty(m_ptNormalTex, m_ptPressedTex, m_ptDisableTex, m_ptHighlightTex, ptTemp, m_ptBtnSize, 
											ptTemp, m_iTextBoxInterval, ASSET_TABBUTTON);
		m_TabGroupBase->SetText(m_byRed, m_byGreen, m_byBlue, m_iSortFlag, ASSET_TABBUTTON);
		//	여기서 버튼간의 인터벌을 조정한다.
		m_TabGroupBase->SetBtnInterval(m_iRadioBtnInterval);
		m_TabGroupBase->AdjustGroupBtnPos();
		m_TabGroupBase->SetDisableText(m_byDisableRed, m_byDisableGreen, m_byDisableBlue);
	}
}

void CTabBtnDlgBar::CreateAsset()
{
	if(m_strTexPath != "")
	{
		//	그룹버튼을 만들어서 내부에 버튼배열을 동적할당 해준후, 해당 그룹버튼을 벡터에 집어넣는다.
		m_TabGroupBase = new CGroupButton;
		m_ptCurPos.x = 0;
		m_ptCurPos.y = 0;

		//	그룹버튼의 내부를 채워줄 라디오 버튼을 세팅한다.
		if( (m_ptColRow.x != 0) && (m_ptColRow.y != 0) )
		{
			m_TabGroupBase->Create(ASSET_TABBUTTON, m_ptColRow.x, m_ptColRow.y);
			for(int i = 0; i < (m_ptColRow.x * m_ptColRow.y); i++)
			{
				//	텍스파일의 위치가 설정되어있지 않으므로 문제가 생기고 있는것
				//	라디오 버튼이 생성이 잘 되고 있지 않은 현상.
				//	텍스파일이 설정되지 않으면 만들어지지 않게 했다.
				CreateTabBtn();
			}
		}

		m_TabGroupBase->SetParenthWnd(m_ParenthWnd);

		CObjMgr::GetInstance()->InsertAssetBase(m_TabGroupBase);
	}
}

void CTabBtnDlgBar::CreateTabBtn()
{	
	POINT ptTemp;
	ptTemp.x = 0;
	ptTemp.y = 0;

	//	에셋의 포지션을 설정한다
	CTabBtnBase newAsset;	
	newAsset.SetPosition(m_ptCurPos);
	newAsset.SetAssetType(ASSET_TABBUTTON);
	newAsset.SetFilePath(m_strTexPath);

	//	에셋의 멤버 변수들을 셋팅해준다
	newAsset.SetButtonProperty(m_ptNormalTex, m_ptPressedTex, m_ptDisableTex, m_ptHighlightTex, m_ptBtnSize);
	//	만들어진 에셋을 Map에 저장하는 과정
	m_TabGroupBase->InsertGroupedBtn(&newAsset, m_ParenthWnd);

	m_TabGroupBase->SetFilePath(m_strTexPath);
	m_TabGroupBase->SetGroupBtnProperty(m_ptNormalTex, m_ptPressedTex, m_ptDisableTex, m_ptHighlightTex, ptTemp, m_ptBtnSize, 
										ptTemp, m_iTextBoxInterval, ASSET_TABBUTTON);
	m_TabGroupBase->SetText(m_byRed, m_byGreen, m_byBlue, m_iSortFlag, ASSET_TABBUTTON);

	switch(m_iArrayType)
	{
	case ARRAYTYPE_WIDTH:
		m_ptCurPos.x = m_ptCurPos.x + m_TabGroupBase->GetRecentBtnSize().x + m_iRadioBtnInterval;
		break;
	case ARRAYTYPE_HEIGHT:
		m_ptCurPos.y = m_ptCurPos.y + m_TabGroupBase->GetRecentBtnSize().y + m_iRadioBtnInterval;
		break;
	}
}

BEGIN_MESSAGE_MAP(CTabBtnDlgBar, CDialogBar)
	ON_EN_CHANGE(IDC_EDIT_TB_SHOW_TEXTURE_PATH, &CTabBtnDlgBar::OnEnChangeEditTbShowTexturePath)
	ON_EN_CHANGE(IDC_EDIT_TB_NM_LEFT, &CTabBtnDlgBar::OnEnChangeEditTbNmLeft)
	ON_EN_CHANGE(IDC_EDIT_TB_NM_TOP, &CTabBtnDlgBar::OnEnChangeEditTbNmTop)
	ON_BN_CLICKED(IDC_RADIO_TB_NM_BTN, &CTabBtnDlgBar::OnBnClickedRadioTbNmBtn)
	ON_EN_CHANGE(IDC_EDIT_TB_HL_LEFT, &CTabBtnDlgBar::OnEnChangeEditTbHlLeft)
	ON_EN_CHANGE(IDC_EDIT_TB_HL_TOP, &CTabBtnDlgBar::OnEnChangeEditTbHlTop)
	ON_BN_CLICKED(IDC_RADIO_TB_HL_BTN, &CTabBtnDlgBar::OnBnClickedRadioTbHlBtn)
	ON_EN_CHANGE(IDC_EDIT_TB_PR_LEFT, &CTabBtnDlgBar::OnEnChangeEditTbPrLeft)
	ON_EN_CHANGE(IDC_EDIT_TB_PR_TOP, &CTabBtnDlgBar::OnEnChangeEditTbPrTop)	
	ON_BN_CLICKED(IDC_RADIO_TB_PR_BTN, &CTabBtnDlgBar::OnBnClickedRadioTbPrBtn)	
	ON_EN_CHANGE(IDC_EDIT_TB_DA_LEFT, &CTabBtnDlgBar::OnEnChangeEditTbDaLeft)
	ON_EN_CHANGE(IDC_EDIT_TB_DA_TOP, &CTabBtnDlgBar::OnEnChangeEditTbDaTop)
	ON_BN_CLICKED(IDC_RADIO_TB_DA_BTN, &CTabBtnDlgBar::OnBnClickedRadioTbDaBtn)
	ON_EN_CHANGE(IDC_EDIT_TB_BTN_WIDTH, &CTabBtnDlgBar::OnEnChangeEditTbBtnWidth)
	ON_EN_CHANGE(IDC_EDIT_TB_BTN_HEIGHT, &CTabBtnDlgBar::OnEnChangeEditTbBtnHeight)
	ON_EN_CHANGE(IDC_EDIT_TB_COL, &CTabBtnDlgBar::OnEnChangeEditTbCol)
	ON_EN_CHANGE(IDC_EDIT_TB_ROW, &CTabBtnDlgBar::OnEnChangeEditTbRow)
	ON_EN_CHANGE(IDC_EDIT_TB_INTERVAL, &CTabBtnDlgBar::OnEnChangeEditTbInterval)
	ON_BN_CLICKED(IDC_TB_CHECK_DISABLESTATE, &CTabBtnDlgBar::OnBnClickedTbCheckDisablestate)
	ON_BN_CLICKED(IDC_RADIO_TB_LEFT_SORT, &CTabBtnDlgBar::OnBnClickedRadioTbLeftSort)
	ON_BN_CLICKED(IDC_RADIO_TB_CENTER_SORT, &CTabBtnDlgBar::OnBnClickedRadioTbCenterSort)
	ON_BN_CLICKED(IDC_RADIO_TB_RIGHT_SORT, &CTabBtnDlgBar::OnBnClickedRadioTbRightSort)
	ON_EN_CHANGE(IDC_EDIT_TB_INPUTBOX, &CTabBtnDlgBar::OnEnChangeEditTbInputbox)
	ON_EN_CHANGE(IDC_EDIT_TB_TEXTBOXINTERVAL, &CTabBtnDlgBar::OnEnChangeEditTbTextboxinterval)
	ON_EN_CHANGE(IDC_EDIT_TB_RGB_R, &CTabBtnDlgBar::OnEnChangeEditTbRgbR)
	ON_EN_CHANGE(IDC_EDIT_TB_RGB_G, &CTabBtnDlgBar::OnEnChangeEditTbRgbG)
	ON_EN_CHANGE(IDC_EDIT_TB_RGB_B, &CTabBtnDlgBar::OnEnChangeEditTbRgbB)
	ON_EN_CHANGE(IDC_EDIT_TB_DA_RGB_R, &CTabBtnDlgBar::OnEnChangeEditTbDaRgbR)
	ON_EN_CHANGE(IDC_EDIT_TB_DA_RGB_G, &CTabBtnDlgBar::OnEnChangeEditTbDaRgbG)
	ON_EN_CHANGE(IDC_EDIT_TB_DA_RGB_B, &CTabBtnDlgBar::OnEnChangeEditTbDaRgbB)	
	ON_MESSAGE(WM_CHANGE_DRAGBOX_INFO, &CTabBtnDlgBar::OnChangeDragboxInfo)
	ON_MESSAGE(WM_SELCHANGE_TEXTURELIST, &CTabBtnDlgBar::OnSelchangeTexturelist)	
	ON_EN_CHANGE(IDC_EDIT_TABBTN_VERTICAL_INDENT, &CTabBtnDlgBar::OnEnChangeEditTabbtnVerticalIndent)
END_MESSAGE_MAP()


// TabBtnDlgBar 메시지 처리기입니다.
//텍스쳐 경로
void CTabBtnDlgBar::OnEnChangeEditTbShowTexturePath()
{
}

void CTabBtnDlgBar::OnEnChangeEditTbNmLeft()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptNormalTex.x = GetEditText(m_NormalLeft_Edit);
		::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptNormalTex.x, (LPARAM)DRAGBOX_LEFT);
	}
	SetTabBtnProperty();
}

void CTabBtnDlgBar::OnEnChangeEditTbNmTop()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptNormalTex.y = GetEditText(m_NormalTop_Edit);
		::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptNormalTex.y, (LPARAM)DRAGBOX_TOP);
	}
	SetTabBtnProperty();
}

void CTabBtnDlgBar::OnEnChangeEditTbHlLeft()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptHighlightTex.x = GetEditText(m_HighLightLeft_Edit);
		::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptHighlightTex.x, (LPARAM)DRAGBOX_LEFT);
	}
	SetTabBtnProperty();
}

void CTabBtnDlgBar::OnEnChangeEditTbHlTop()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptHighlightTex.y = GetEditText(m_HIghLightTop_Edit);
		::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptHighlightTex.y, (LPARAM)DRAGBOX_TOP);
	}
	SetTabBtnProperty();
}

void CTabBtnDlgBar::OnEnChangeEditTbPrLeft()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptPressedTex.x = GetEditText(m_PressedLeft_Edit);
		::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptPressedTex.x, (LPARAM)DRAGBOX_LEFT);
	}
	SetTabBtnProperty();
}

void CTabBtnDlgBar::OnEnChangeEditTbPrTop()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptPressedTex.y = GetEditText(m_PressedTop_Edit);
		::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptPressedTex.y, (LPARAM)DRAGBOX_TOP);
	}
	SetTabBtnProperty();
}

void CTabBtnDlgBar::OnBnClickedRadioTbHlBtn()
{
	m_iSelBtn = BTN_HIGHLIGHT;
	::SendMessage(m_PictureWnd, WM_CHANGE_RADIOBTN, (WPARAM)m_ptHighlightTex.x, (LPARAM)m_ptHighlightTex.y);
	::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.x, (LPARAM)DRAGBOX_WIDTH);
	::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.y, (LPARAM)DRAGBOX_HEIGHT);
}

void CTabBtnDlgBar::OnBnClickedRadioTbPrBtn()
{
	m_iSelBtn = BTN_PRESSED;
	::SendMessage(m_PictureWnd, WM_CHANGE_RADIOBTN, (WPARAM)m_ptPressedTex.x, (LPARAM)m_ptPressedTex.y);
	::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.x, (LPARAM)DRAGBOX_WIDTH);
	::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.y, (LPARAM)DRAGBOX_HEIGHT);
}

void CTabBtnDlgBar::OnBnClickedRadioTbNmBtn()
{
	//	선택되었을때 픽쳐다이알로그의 xpos와 ypos를 수정해주면 될 것 같다
	m_iSelBtn = BTN_NORMAL;
	//	포인터를 보내준 다음 해당 값을 참조해서 복사를 하게 하고 그거를 다이알로그바에 세팅하게 하면
	::SendMessage(m_PictureWnd, WM_CHANGE_RADIOBTN, (WPARAM)m_ptNormalTex.x, (LPARAM)m_ptNormalTex.y);
	::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.x, (LPARAM)DRAGBOX_WIDTH);
	::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.y, (LPARAM)DRAGBOX_HEIGHT);
}

void CTabBtnDlgBar::OnEnChangeEditTbDaLeft()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptDisableTex.x = GetEditText(m_DisableLeft_Edit);
		::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptDisableTex.x, (LPARAM)DRAGBOX_LEFT);
	}
	SetTabBtnProperty();
}

void CTabBtnDlgBar::OnEnChangeEditTbDaTop()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptDisableTex.y = GetEditText(m_DisableTop_Edit);
		::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptDisableTex.y, (LPARAM)DRAGBOX_TOP);
	}
	SetTabBtnProperty();
}

void CTabBtnDlgBar::OnBnClickedRadioTbDaBtn()
{
	m_iSelBtn = BTN_DISABLE;
	::SendMessage(m_PictureWnd, WM_CHANGE_RADIOBTN, (WPARAM)m_ptDisableTex.x, (LPARAM)m_ptDisableTex.y);
	::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.x, (LPARAM)DRAGBOX_WIDTH);
	::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.y, (LPARAM)DRAGBOX_HEIGHT);
}

void CTabBtnDlgBar::OnEnChangeEditTbBtnWidth()
{	
	if(m_bChange)
	{
		UpdateData(true);
		m_ptBtnSize.x = GetEditText(m_BtnWidthEdit);
		::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.x, (LPARAM)DRAGBOX_WIDTH);
	}
	SetTabBtnProperty();
}

void CTabBtnDlgBar::OnEnChangeEditTbBtnHeight()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptBtnSize.y = GetEditText(m_BtnHeightEdit);
		::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBtnSize.y, (LPARAM)DRAGBOX_HEIGHT);
	}
	SetTabBtnProperty();
}

void CTabBtnDlgBar::OnEnChangeEditTbCol()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptColRow.x = GetEditText(m_RadioBtnColEdit);

		if(m_ptColRow.x > m_ptColRow.y)
		{
			//가로타입 라디오 버튼
			m_iArrayType = ARRAYTYPE_WIDTH;
		}
		else if(m_ptColRow.x < m_ptColRow.y)
		{
			//세로타입 라디오 버튼
			m_iArrayType = ARRAYTYPE_HEIGHT;
		}

		CObjMgr::GetInstance()->DeleteAssetBase(m_TabGroupBase);
		m_TabGroupBase = NULL;
		SetTabBtnProperty();
	}
}

void CTabBtnDlgBar::OnEnChangeEditTbRow()
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

		CObjMgr::GetInstance()->DeleteAssetBase(m_TabGroupBase);
		m_TabGroupBase = NULL;
		SetTabBtnProperty();
	}
}

void CTabBtnDlgBar::OnEnChangeEditTbInterval()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_iRadioBtnInterval = GetEditText(m_RadioBtnIntervalEdit);
	}
	SetTabBtnProperty();
}

void CTabBtnDlgBar::OnBnClickedTbCheckDisablestate()
{
	UpdateData(true);
	if(m_DisableCheckBox->GetCheck() == BST_CHECKED)
	{
		if(m_TabGroupBase != NULL)
		{
			m_TabGroupBase->SetbDisable(true);
		}
	}
	else if(m_DisableCheckBox->GetCheck() == BST_UNCHECKED)
	{
		if(m_TabGroupBase != NULL)
		{
			m_TabGroupBase->SetbDisable(false);
		}
	}
}

void CTabBtnDlgBar::OnBnClickedRadioTbLeftSort()
{
	m_iSortFlag = DT_LEFT | DT_VCENTER | DT_SINGLELINE;
	SetTabBtnProperty();
}

void CTabBtnDlgBar::OnBnClickedRadioTbCenterSort()
{
	m_iSortFlag = DT_CENTER | DT_VCENTER | DT_SINGLELINE;
	SetTabBtnProperty();
}

void CTabBtnDlgBar::OnBnClickedRadioTbRightSort()
{
	m_iSortFlag = DT_RIGHT | DT_VCENTER | DT_SINGLELINE;
	SetTabBtnProperty();
}

void CTabBtnDlgBar::OnEnChangeEditTbInputbox()
{
	UpdateData(TRUE);
	if(CObjMgr::GetInstance()->GetFocusAssetBase() == NULL)
	{
		m_TabGroupBase = NULL;
	}

	m_TextPreviewEdit->GetWindowText(m_strPreview);

	if(m_TabGroupBase != NULL)
	{
		//	여기서 텍스트 박스 요소를 세팅
		m_TabGroupBase->SetTextPreview(m_strPreview, ASSET_TABBUTTON);
	}
}

void CTabBtnDlgBar::OnEnChangeEditTbTextboxinterval()
{
	UpdateData(true);
	m_iTextBoxInterval = GetEditText(m_IntervalEdit);
	SetTabBtnProperty();
}

void CTabBtnDlgBar::OnEnChangeEditTbRgbR()
{
	UpdateData(true);
	m_byRed = GetEditText(m_TextRGB_R_Edit);
	SetTabBtnProperty();
}

void CTabBtnDlgBar::OnEnChangeEditTbRgbG()
{
	UpdateData(true);
	m_byGreen = GetEditText(m_TextRGB_G_Edit);
	SetTabBtnProperty();
}

void CTabBtnDlgBar::OnEnChangeEditTbRgbB()
{
	UpdateData(true);
	m_byBlue = GetEditText(m_TextRGB_B_Edit);
	SetTabBtnProperty();
}

void CTabBtnDlgBar::OnEnChangeEditTbDaRgbR()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_byDisableRed = GetEditText(m_DisableRGB_R_Edit);
	}
	SetTabBtnProperty();
}

void CTabBtnDlgBar::OnEnChangeEditTbDaRgbG()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_byDisableGreen = GetEditText(m_DisableRGB_G_Edit);
	}
	SetTabBtnProperty();
}

void CTabBtnDlgBar::OnEnChangeEditTbDaRgbB()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_byDisableBlue = GetEditText(m_DisableRGB_B_Edit);
	}
	SetTabBtnProperty();
}

void CTabBtnDlgBar::RefreshDlgBar( CGroupButton* _LoadBase, POINT& _NormalTex )
{
	m_bChange = false;

	SetMemberTabGroupBase(_LoadBase);

	std::string Path = _LoadBase->GetFilePath();

	m_iRadioBtnInterval = _LoadBase->GetGroupInterval();
	EditTextSetting(m_iRadioBtnInterval, m_RadioBtnIntervalEdit);

	CTabBtnBase* GroupBase = (CTabBtnBase*)(_LoadBase->GetGroupBase());
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

	//	focus되어있는 인덱스의 버튼에 접근
	m_byRed = GroupBase[0].GetTextBox().GetRGB(RGB_R);
	EditTextSetting(m_byRed, m_TextRGB_R_Edit);
	m_byGreen = GroupBase[0].GetTextBox().GetRGB(RGB_G);
	EditTextSetting(m_byGreen, m_TextRGB_G_Edit);
	m_byBlue = GroupBase[0].GetTextBox().GetRGB(RGB_B);
	EditTextSetting(m_byBlue, m_TextRGB_B_Edit);

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

	EditTextSetting(_LoadBase->GetBtnVerticalIndent(), m_BtnTextVerticalIndentEdit);

	//	상대적 위치를 고려한 버튼 위치 재설정
	_LoadBase->AdjustGroupBtnPos();
	//	state체크를 위한 핸들 설정
	_LoadBase->SetParenthWnd(m_ParenthWnd);
	_LoadBase->SetParenthWndToGroup();

	//	초기 선택 버튼 지정
	_LoadBase->SetFocusIndex(0);

	m_bChange = true;
}

LRESULT CTabBtnDlgBar::OnChangeDragboxInfo( WPARAM wParam, LPARAM lParam )
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

LRESULT CTabBtnDlgBar::OnSelchangeTexturelist( WPARAM wParam, LPARAM lParam )
{
	UpdateData(true);

	m_strTexPath = (CDBMgr::GetInstance()->GetPathMap()->find((int)wParam)->second);
	CA2T wt(m_strTexPath.c_str());
	m_TexturePathEdit->SetWindowText(wt);

	if(m_TabGroupBase != NULL)
	{
		m_TabGroupBase->SetFilePath(m_strTexPath);
	}
	return 0;
}

void CTabBtnDlgBar::OnEnChangeEditTabbtnVerticalIndent()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_TabGroupBase->SetTextBoxVerticalIndent(GetEditText(m_BtnTextVerticalIndentEdit));
	}

	SetTabBtnProperty();
}
