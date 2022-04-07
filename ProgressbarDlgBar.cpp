// ProgressbarDlgBar.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "UITool.h"
#include "ProgressbarDlgBar.h"
#include "afxdialogex.h"
#include "ObjMgr.h"
#include "DBMgr.h"


// CProgressbarDlgBar 대화 상자입니다.

IMPLEMENT_DYNAMIC(CProgressbarDlgBar, CDialogBar)

CProgressbarDlgBar::CProgressbarDlgBar(CWnd* pParent /*=NULL*/)
{
	POINT ptZero;
	ptZero.x = 0;
	ptZero.y = 0;

	m_ptBackgroundStartPos = ptZero;
	m_ptBackgroundSize = ptZero;

	m_ptGaugeStartPos = ptZero;
	m_ptGaugeSize = ptZero;
	m_ptGaugeIndent = ptZero;
	m_ptExtraGaugeStartPos = ptZero;
	m_ptExtraGaugeSize = ptZero;
	m_iExtraGauge = 0;
	m_iGauge = 0;

	m_strTexPath = "";
	m_iSelBtn = BTN_BACKGROUND;
	m_ProgressbarBase = NULL;
	m_bChange = true;
}

CProgressbarDlgBar::~CProgressbarDlgBar()
{
}

void CProgressbarDlgBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);

	m_TexturePathEdit = (CEdit*)GetDlgItem(IDC_EDIT_PRG_SHOW_TEXTURE_PATH);
	m_BackgroundLeftEdit = (CEdit*)GetDlgItem(IDC_EDIT_PRG_BG_LEFT);
	m_BackgroundTopEdit = (CEdit*)GetDlgItem(IDC_EDIT_PRG_BG_TOP);
	m_BackgroundWidthEdit = (CEdit*)GetDlgItem(IDC_EDIT_PRG_BG_WIDTH);
	m_BackgroundHeightEdit = (CEdit*)GetDlgItem(IDC_EDIT_PRG_BG_HEIGHT);

	m_GaugeLeftEdit = (CEdit*)GetDlgItem(IDC_EDIT_PRG_GAUGE_LEFT);
	m_GaugeTopEdit = (CEdit*)GetDlgItem(IDC_EDIT_PRG_GAUGE_TOP);
	m_GaugeWidthEdit = (CEdit*)GetDlgItem(IDC_EDIT_PRG_GAUGE_WIDTH);
	m_GaugeHeightEdit = (CEdit*)GetDlgItem(IDC_EDIT_PRG_GAUGE_HEIGHT);

	m_GaugePercentageEdit = (CEdit*)GetDlgItem(IDC_EDIT_PROGRESSBAR_PERCENTAGE);

	m_BackgroundRadioBtn = (CButton*)GetDlgItem(IDC_RADIO_PRG_BG);
	m_GaugeRadioBtn = (CButton*)GetDlgItem(IDC_RADIO_PRG_GAUGEBAR);
	
	m_GaugeIndentXEdit = (CEdit*)GetDlgItem(IDC_EDIT_PRG_GAUGE_STARTPOSX);
	m_GaugeIndentYEdit = (CEdit*)GetDlgItem(IDC_EDIT_PRG_GAUGE_STARTPOSY);

	m_GaugeBarList = (CListBox*)GetDlgItem(IDC_LIST_MULTIPLE_GAUGE);

	m_ExtraGaugeLeftEdit = (CEdit*)GetDlgItem(IDC_EDIT_PRG_EXTRAGAUGE_LEFT);
	m_ExtraGaugeTopEdit = (CEdit*)GetDlgItem(IDC_EDIT_PRG_EXTRAGAUGE_TOP);
	m_ExtraGaugeWidthEdit = (CEdit*)GetDlgItem(IDC_EDIT_PRG_EXTRAGAUGE_WIDTH);
	m_ExtraGaugeHeightEdit = (CEdit*)GetDlgItem(IDC_EDIT_PRG_EXTRAGAUGE_HEIGHT);
	m_ExtraGaugePercentageEdit = (CEdit*)GetDlgItem(IDC_EDIT_PRG_EXTRAGAUGE_PERCENTAGE);

	if(CObjMgr::GetInstance()->GetFocusAssetBase() == NULL)
	{
		m_ProgressbarBase = NULL;
	}
}

void CProgressbarDlgBar::Initialize()
{
	UpdateData(true);

	POINT ptZero;
	ptZero.x = 0;
	ptZero.y = 0;

	m_ptBackgroundStartPos = ptZero;
	m_ptBackgroundSize = ptZero;

	m_ptGaugeStartPos = ptZero;
	m_ptGaugeSize = ptZero;
	m_ptGaugeIndent = ptZero;
	m_ptExtraGaugeStartPos = ptZero;
	m_ptExtraGaugeSize = ptZero;
	m_iExtraGauge = 0;

	m_iGauge = 0;

	m_strTexPath = "";
	m_iSelBtn = BTN_BACKGROUND;
	m_ProgressbarBase = NULL;

	m_bChange = false;

	m_TexturePathEdit->SetWindowText(L"");
	m_BackgroundLeftEdit->SetWindowText(L"0");
	m_BackgroundTopEdit->SetWindowText(L"0");
	m_BackgroundWidthEdit->SetWindowText(L"0");
	m_BackgroundHeightEdit->SetWindowText(L"0");

	m_GaugeLeftEdit->SetWindowText(L"0");
	m_GaugeTopEdit->SetWindowText(L"0");
	m_GaugeWidthEdit->SetWindowText(L"0");
	m_GaugeHeightEdit->SetWindowText(L"0");

	m_GaugePercentageEdit->SetWindowText(L"0");

	m_GaugeRadioBtn->SetCheck(0);
	m_BackgroundRadioBtn->SetCheck(1);
	m_GaugeIndentXEdit->SetWindowText(L"0");
	m_GaugeIndentYEdit->SetWindowText(L"0");

	m_ExtraGaugeLeftEdit->SetWindowText(L"0");
	m_ExtraGaugeTopEdit->SetWindowText(L"0");
	m_ExtraGaugeWidthEdit->SetWindowText(L"0");
	m_ExtraGaugeHeightEdit->SetWindowText(L"0");
	m_ExtraGaugePercentageEdit->SetWindowText(L"0");

	int iListCount = m_GaugeBarList->GetCount();
	for(int i = iListCount; i >= 0; i--)
	{
		m_GaugeBarList->DeleteString(i);
	}

	m_bChange = true;
}


BEGIN_MESSAGE_MAP(CProgressbarDlgBar, CDialogBar)
	ON_EN_CHANGE(IDC_EDIT_PRG_BG_LEFT, &CProgressbarDlgBar::OnEnChangeEditPrgBgLeft)
	ON_EN_CHANGE(IDC_EDIT_PRG_BG_TOP, &CProgressbarDlgBar::OnEnChangeEditPrgBgTop)
	ON_EN_CHANGE(IDC_EDIT_PRG_BG_WIDTH, &CProgressbarDlgBar::OnEnChangeEditPrgBgWidth)
	ON_EN_CHANGE(IDC_EDIT_PRG_BG_HEIGHT, &CProgressbarDlgBar::OnEnChangeEditPrgBgHeight)
	ON_EN_CHANGE(IDC_EDIT_PRG_GAUGE_LEFT, &CProgressbarDlgBar::OnEnChangeEditPrgGaugeLeft)
	ON_EN_CHANGE(IDC_EDIT_PRG_GAUGE_TOP, &CProgressbarDlgBar::OnEnChangeEditPrgGaugeTop)
	ON_EN_CHANGE(IDC_EDIT_PRG_GAUGE_WIDTH, &CProgressbarDlgBar::OnEnChangeEditPrgGaugeWidth)
	ON_EN_CHANGE(IDC_EDIT_PRG_GAUGE_HEIGHT, &CProgressbarDlgBar::OnEnChangeEditPrgGaugeHeight)
	ON_EN_CHANGE(IDC_EDIT_PROGRESSBAR_PERCENTAGE, &CProgressbarDlgBar::OnEnChangeProgressBarPreview)
	ON_BN_CLICKED(IDC_RADIO_PRG_GAUGEBAR, &CProgressbarDlgBar::OnBnClickedRadioPrgGaugebar)
	ON_BN_CLICKED(IDC_RADIO_PRG_BG, &CProgressbarDlgBar::OnBnClickedRadioPrgBg)
	ON_EN_CHANGE(IDC_EDIT_PRG_SHOW_TEXTURE_PATH, &CProgressbarDlgBar::OnEnChangeEditPrgShowTexturePath)
	ON_MESSAGE(WM_SELCHANGE_TEXTURELIST, CProgressbarDlgBar::OnSelchangeTexturelist)
	ON_MESSAGE(WM_CHANGE_DRAGBOX_INFO, CProgressbarDlgBar::OnChangeDragboxInfo)
	ON_EN_CHANGE(IDC_EDIT_PRG_GAUGE_STARTPOSX, &CProgressbarDlgBar::OnEnChangeEditPrgGaugeStartposx)
	ON_EN_CHANGE(IDC_EDIT_PRG_GAUGE_STARTPOSY, &CProgressbarDlgBar::OnEnChangeEditPrgGaugeStartposy)
	ON_LBN_SELCHANGE(IDC_LIST_MULTIPLE_GAUGE, &CProgressbarDlgBar::OnLbnSelchangeListMultipleGauge)
	ON_BN_CLICKED(IDC_BUTTON_GAUGEBAR_UP, &CProgressbarDlgBar::OnBnClickedButtonGaugebarUp)
	ON_BN_CLICKED(IDC_BUTTON_GAUGEBAR_DOWN, &CProgressbarDlgBar::OnBnClickedButtonGaugebarDown)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_GAUGEBAR, &CProgressbarDlgBar::OnBnClickedButtonDeleteGaugebar)
	ON_BN_CLICKED(IDC_BUTTON_ADD_GAUGEBAR, &CProgressbarDlgBar::OnBnClickedButtonAddGaugebar)

	ON_UPDATE_COMMAND_UI(IDC_BUTTON_GAUGEBAR_UP, &CProgressbarDlgBar::OnUpdateCmdUI)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_GAUGEBAR_DOWN, &CProgressbarDlgBar::OnUpdateCmdUI)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_DELETE_GAUGEBAR, &CProgressbarDlgBar::OnUpdateCmdUI)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_ADD_GAUGEBAR, &CProgressbarDlgBar::OnUpdateCmdUI)
	ON_EN_CHANGE(IDC_EDIT_PRG_EXTRAGAUGE_LEFT, &CProgressbarDlgBar::OnEnChangeEditPrgExtragaugeLeft)
	ON_EN_CHANGE(IDC_EDIT_PRG_EXTRAGAUGE_TOP, &CProgressbarDlgBar::OnEnChangeEditPrgExtragaugeTop)
	ON_EN_CHANGE(IDC_EDIT_PRG_EXTRAGAUGE_WIDTH, &CProgressbarDlgBar::OnEnChangeEditPrgExtragaugeWidth)
	ON_EN_CHANGE(IDC_EDIT_PRG_EXTRAGAUGE_HEIGHT, &CProgressbarDlgBar::OnEnChangeEditPrgExtragaugeHeight)
	ON_EN_CHANGE(IDC_EDIT_PRG_EXTRAGAUGE_PERCENTAGE, &CProgressbarDlgBar::OnEnChangeEditPrgExtragaugePercentage)
	ON_BN_CLICKED(IDC_RADIO_EXTRA_GAUGE, &CProgressbarDlgBar::OnBnClickedRadioExtraGauge)
END_MESSAGE_MAP()

void CProgressbarDlgBar::OnUpdateCmdUI( CCmdUI* pCmdUI )
{
	pCmdUI->Enable(TRUE);
}

// CProgressbarDlgBar 메시지 처리기입니다.


void CProgressbarDlgBar::OnEnChangeEditPrgBgLeft()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptBackgroundStartPos.x = GetEditText(m_BackgroundLeftEdit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBackgroundStartPos.x, (LPARAM)DRAGBOX_LEFT);
	}

	SetProgressbarProperty();
}


void CProgressbarDlgBar::OnEnChangeEditPrgBgTop()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptBackgroundStartPos.y = GetEditText(m_BackgroundTopEdit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBackgroundStartPos.y, (LPARAM)DRAGBOX_TOP);
	}

	SetProgressbarProperty();
}


void CProgressbarDlgBar::OnEnChangeEditPrgBgWidth()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptBackgroundSize.x = GetEditText(m_BackgroundWidthEdit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBackgroundSize.x, (LPARAM)DRAGBOX_WIDTH);
	}

	SetProgressbarProperty();
}


void CProgressbarDlgBar::OnEnChangeEditPrgBgHeight()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptBackgroundSize.y = GetEditText(m_BackgroundHeightEdit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBackgroundSize.y, (LPARAM)DRAGBOX_HEIGHT);
	}
	SetProgressbarProperty();
}


void CProgressbarDlgBar::OnEnChangeEditPrgGaugeLeft()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptGaugeStartPos.x = GetEditText(m_GaugeLeftEdit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptGaugeStartPos.x, (LPARAM)DRAGBOX_LEFT);
	}
	SetProgressbarProperty();
}


void CProgressbarDlgBar::OnEnChangeEditPrgGaugeTop()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptGaugeStartPos.y = GetEditText(m_GaugeTopEdit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptGaugeStartPos.y, (LPARAM)DRAGBOX_TOP);
	}

	SetProgressbarProperty();
}


void CProgressbarDlgBar::OnEnChangeEditPrgGaugeWidth()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptGaugeSize.x = GetEditText(m_GaugeWidthEdit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptGaugeSize.x, (LPARAM)DRAGBOX_WIDTH);
	}

	SetProgressbarProperty();
}


void CProgressbarDlgBar::OnEnChangeEditPrgGaugeHeight()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptGaugeSize.y = GetEditText(m_GaugeHeightEdit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptGaugeSize.y, (LPARAM)DRAGBOX_HEIGHT);
	}

	SetProgressbarProperty();
}


void CProgressbarDlgBar::OnBnClickedRadioPrgGaugebar()
{
	m_iSelBtn = BTN_GAUGEBAR;
	::SendMessage(m_PicturehWnd, WM_CHANGE_RADIOBTN, (WPARAM)m_ptGaugeStartPos.x, (LPARAM)m_ptGaugeStartPos.y);
	::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptGaugeSize.x, (LPARAM)DRAGBOX_WIDTH);
	::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptGaugeSize.y, (LPARAM)DRAGBOX_HEIGHT);
}


void CProgressbarDlgBar::OnBnClickedRadioPrgBg()
{
	m_iSelBtn = BTN_BACKGROUND;
	::SendMessage(m_PicturehWnd, WM_CHANGE_RADIOBTN, (WPARAM)m_ptBackgroundStartPos.x, (LPARAM)m_ptBackgroundStartPos.y);
	::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBackgroundSize.x, (LPARAM)DRAGBOX_WIDTH);
	::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBackgroundSize.y, (LPARAM)DRAGBOX_HEIGHT);
}

void CProgressbarDlgBar::OnBnClickedRadioExtraGauge()
{
	m_iSelBtn = BTN_EXTRAGAUGEBAR;
	::SendMessage(m_PicturehWnd, WM_CHANGE_RADIOBTN, (WPARAM)m_ptExtraGaugeStartPos.x, (LPARAM)m_ptExtraGaugeStartPos.y);
	::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptExtraGaugeSize.x, (LPARAM)DRAGBOX_WIDTH);
	::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptExtraGaugeSize.y, (LPARAM)DRAGBOX_HEIGHT);
}


void CProgressbarDlgBar::OnEnChangeEditPrgShowTexturePath()
{

}

LRESULT CProgressbarDlgBar::OnSelchangeTexturelist( WPARAM wParam, LPARAM lParam )
{
	UpdateData(true);

	m_strTexPath = (CDBMgr::GetInstance()->GetPathMap()->find((int)wParam)->second);
	CA2T wt(m_strTexPath.c_str());
	m_TexturePathEdit->SetWindowText(wt);

	if(m_ProgressbarBase != NULL)
	{
		m_ProgressbarBase->SetFilePath(m_strTexPath);
	}
	return 0;
}

LRESULT CProgressbarDlgBar::OnChangeDragboxInfo( WPARAM wParam, LPARAM lParam )
{
	m_bChange = false;

	UpdateData(true);

	switch(m_iSelBtn)
	{
	case BTN_BACKGROUND:
		switch((int)lParam)
		{
		case DRAGBOX_LEFT:
			m_ptBackgroundStartPos.x = (int)wParam;
			SetEditWndText(m_BackgroundLeftEdit, m_ptBackgroundStartPos.x);
			break;
		case DRAGBOX_TOP:
			m_ptBackgroundStartPos.y = (int)wParam;
			SetEditWndText(m_BackgroundTopEdit, m_ptBackgroundStartPos.y);
			break;
		}
		break;
	case BTN_GAUGEBAR:
		switch((int)lParam)
		{
		case DRAGBOX_LEFT:
			m_ptGaugeStartPos.x = (int)wParam;
			SetEditWndText(m_GaugeLeftEdit, m_ptGaugeStartPos.x);
			break;
		case DRAGBOX_TOP:
			m_ptGaugeStartPos.y = (int)wParam;
			SetEditWndText(m_GaugeTopEdit, m_ptGaugeStartPos.y);
			break;
		}
		break;
	case BTN_EXTRAGAUGEBAR:
		switch((int)lParam)
		{
		case DRAGBOX_LEFT:
			m_ptExtraGaugeStartPos.x = (int)wParam;
			SetEditWndText(m_ExtraGaugeLeftEdit, m_ptExtraGaugeStartPos.x);
			break;
		case DRAGBOX_TOP:
			m_ptExtraGaugeStartPos.y = (int)wParam;
			SetEditWndText(m_ExtraGaugeTopEdit, m_ptExtraGaugeStartPos.y);
			break;
		}
		break;
	}


	switch((int)lParam)
	{
	case DRAGBOX_WIDTH:
		if(m_iSelBtn == BTN_GAUGEBAR)
		{
			m_ptGaugeSize.x = (int)wParam;
			SetEditWndText(m_GaugeWidthEdit, m_ptGaugeSize.x);
		}
		else if(m_iSelBtn == BTN_BACKGROUND)
		{
			m_ptBackgroundSize.x = (int)wParam;
			SetEditWndText(m_BackgroundWidthEdit, m_ptBackgroundSize.x);
		}
		else if (m_iSelBtn == BTN_EXTRAGAUGEBAR)
		{
			m_ptExtraGaugeSize.x = (int)wParam;
			SetEditWndText(m_ExtraGaugeWidthEdit, m_ptExtraGaugeSize.x);
		}
		break;
	case DRAGBOX_HEIGHT:
		if(m_iSelBtn == BTN_GAUGEBAR)
		{
			m_ptGaugeSize.y = (int)wParam;
			SetEditWndText(m_GaugeHeightEdit, m_ptGaugeSize.y);
		}
		else if(m_iSelBtn == BTN_BACKGROUND)
		{
			m_ptBackgroundSize.y = (int)wParam;
			SetEditWndText(m_BackgroundHeightEdit, m_ptBackgroundSize.y);
		}
		else if (m_iSelBtn == BTN_EXTRAGAUGEBAR)
		{
			m_ptExtraGaugeSize.y = (int)wParam;
			SetEditWndText(m_ExtraGaugeHeightEdit, m_ptExtraGaugeSize.y);
		}
		break;
	}

	m_bChange = true;

	SetProgressbarProperty();
	//	게이지바가 갱신이 제대로 되고 있지않은 것 같아 삽입.

	return 0;
}

void CProgressbarDlgBar::OnEnChangeProgressBarPreview()
{
	if(m_bChange)
	{
		UpdateData(true);
		int iResult = GetEditText(m_GaugePercentageEdit);
		if(m_ProgressbarBase != NULL)
		{
			m_ProgressbarBase->SetGaugeAmount(iResult);
		}
	}
	SetProgressbarProperty();
}

void CProgressbarDlgBar::SetProgressbarProperty()
{
	if(m_ProgressbarBase == NULL)
	{
		CreateAsset();
	}

	if(m_ProgressbarBase != NULL)
	{
		m_ProgressbarBase->SetProgressBar(m_strTexPath, m_ptBackgroundSize, m_ptBackgroundStartPos);
		m_ProgressbarBase->SetProgressbarProperty();
		m_ProgressbarBase->SetIndent(m_ptGaugeIndent);
		m_ProgressbarBase->SetExtraGauge(m_strTexPath, m_ptExtraGaugeSize, m_ptExtraGaugeStartPos);
		m_ProgressbarBase->SetExtraGaugeAmount(m_iExtraGauge);
		//RefreshGaugeBarList();
	}
}

void CProgressbarDlgBar::CreateAsset()
{
	if(m_strTexPath != "")
	{
		m_bChange = false;
		UpdateData(TRUE);
		
		m_ProgressbarBase = new CProgressBarBase;
		m_ProgressbarBase->SetAssetType(ASSET_PROGRESSBAR);

		POINT ptZero;
		ptZero.x = 0;
		ptZero.y = 0;

		m_ProgressbarBase->SetPosition(ptZero);
		m_ProgressbarBase->SetFilePath(m_strTexPath);
		m_ProgressbarBase->SetParenthWnd(m_ParenthWnd);

		CObjMgr::GetInstance()->InsertAssetBase(m_ProgressbarBase);
		m_bChange = true;
	}
}

void CProgressbarDlgBar::RefreshDlgBar( CProgressBarBase* _LoadBase, POINT& _normalTex )
{
	m_bChange = false;

	// 게이지 수치
	m_iGauge  = _LoadBase->GetGaugePercentage();
	EditTextSetting(m_iGauge, m_GaugePercentageEdit);

	m_iExtraGauge = _LoadBase->GetExtraGaugePercentage();
	EditTextSetting(m_iExtraGauge, m_ExtraGaugePercentageEdit);

	m_ptExtraGaugeStartPos.x= _LoadBase->GetExtraGaugeBar()->GetStartPos().x;
	EditTextSetting(m_ptExtraGaugeStartPos.x, m_ExtraGaugeLeftEdit);
	m_ptExtraGaugeStartPos.y= _LoadBase->GetExtraGaugeBar()->GetStartPos().y;
	EditTextSetting(m_ptExtraGaugeStartPos.y, m_ExtraGaugeTopEdit);
	m_ptExtraGaugeSize.x= _LoadBase->GetExtraGaugeBar()->GetFileSize().right;
	EditTextSetting(m_ptExtraGaugeSize.x, m_ExtraGaugeWidthEdit);
	m_ptExtraGaugeSize.y= _LoadBase->GetExtraGaugeBar()->GetFileSize().bottom;
	EditTextSetting(m_ptExtraGaugeSize.y, m_ExtraGaugeHeightEdit);

	m_ptBackgroundSize.x = _LoadBase->GetBackgroundBar()->GetFileSize().right;
	EditTextSetting(m_ptBackgroundSize.x, m_BackgroundWidthEdit);
	m_ptBackgroundSize.y = _LoadBase->GetBackgroundBar()->GetFileSize().bottom;
	EditTextSetting(m_ptBackgroundSize.y, m_BackgroundHeightEdit);

	m_ptBackgroundStartPos.x = _LoadBase->GetBackgroundBar()->GetStartPos().x;
	EditTextSetting(m_ptBackgroundStartPos.x, m_BackgroundLeftEdit);
	m_ptBackgroundStartPos.y = _LoadBase->GetBackgroundBar()->GetStartPos().y;
	EditTextSetting(m_ptBackgroundStartPos.y, m_BackgroundTopEdit);

	_normalTex = m_ptBackgroundStartPos;

	m_ptGaugeIndent.x = _LoadBase->GetGaugeIndent().x;
	EditTextSetting(m_ptGaugeIndent.x, m_GaugeIndentXEdit);
	m_ptGaugeIndent.y = _LoadBase->GetGaugeIndent().y;
	EditTextSetting(m_ptGaugeIndent.y, m_GaugeIndentYEdit);

	//	베이스를 다이알로그바에 셋팅
	SetProgressBarBase(_LoadBase);

	m_strTexPath = _LoadBase->GetFilePath();
	CString TexPath(m_strTexPath.c_str());
	m_TexturePathEdit->SetWindowText(TexPath);

	//	게이지
	CAssetBase* CurGaugeAsset = (*(m_ProgressbarBase->GetGaugeBarVector()))[0];

	m_ptGaugeSize.x = CurGaugeAsset->GetFileSize().right;
	EditTextSetting(m_ptGaugeSize.x, m_GaugeWidthEdit);
	m_ptGaugeSize.y  = CurGaugeAsset->GetFileSize().bottom;
	EditTextSetting(m_ptGaugeSize.y, m_GaugeHeightEdit);

	m_ptGaugeStartPos.x = CurGaugeAsset->GetStartPos().x;
	EditTextSetting(m_ptGaugeStartPos.x, m_GaugeLeftEdit);
	m_ptGaugeStartPos.y = CurGaugeAsset->GetStartPos().y;
	EditTextSetting(m_ptGaugeStartPos.y, m_GaugeTopEdit);

	m_bChange = true;

	RefreshGaugeBarList();
}

void CProgressbarDlgBar::OnEnChangeEditPrgGaugeStartposx()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptGaugeIndent.x = GetEditText(m_GaugeIndentXEdit);
	}
	SetProgressbarProperty();
}


void CProgressbarDlgBar::OnEnChangeEditPrgGaugeStartposy()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptGaugeIndent.y = GetEditText(m_GaugeIndentYEdit);
	}
	SetProgressbarProperty();
}


void CProgressbarDlgBar::OnLbnSelchangeListMultipleGauge()
{
	if(m_bChange)
	{
		//	셀체인지가 일어났을 경우 픽쳐 윈도우에 드래그박스 정보를 넘겨준다.
		UpdateData(true);
		m_iSelectGageBarIndex = m_GaugeBarList->GetCurSel();

		if(m_ProgressbarBase != NULL)
		{
			//	하단에 디스플레이 해줄거라면 여기 밑에서 작업하면 된다.
			CAssetBase* CurGaugeAsset = (*(m_ProgressbarBase->GetGaugeBarVector()))[m_iSelectGageBarIndex];
			m_ptGaugeStartPos.x = CurGaugeAsset->GetStartPos().x;
			m_ptGaugeStartPos.y = CurGaugeAsset->GetStartPos().y;
			m_ptGaugeSize.x = CurGaugeAsset->GetFileSize().right;
			m_ptGaugeSize.y = CurGaugeAsset->GetFileSize().bottom;

			SetEditWndText(m_GaugeLeftEdit, m_ptGaugeStartPos.x);
			SetEditWndText(m_GaugeTopEdit, m_ptGaugeStartPos.y);
			SetEditWndText(m_GaugeWidthEdit, m_ptGaugeSize.x);
			SetEditWndText(m_GaugeHeightEdit, m_ptGaugeSize.y);

			::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptGaugeStartPos.x, (LPARAM)DRAGBOX_LEFT);
			::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptGaugeStartPos.y, (LPARAM)DRAGBOX_TOP);
			::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptGaugeSize.x, (LPARAM)DRAGBOX_WIDTH);
			::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptGaugeSize.y, (LPARAM)DRAGBOX_HEIGHT);
		}
	}
}


void CProgressbarDlgBar::OnBnClickedButtonGaugebarUp()
{
	if(m_bChange)
	{
		UpdateData(true);
		if(m_ProgressbarBase != NULL)
		{
			int iIndex = m_GaugeBarList->GetCurSel();
			m_ProgressbarBase->SelectGaugeBarUp(iIndex);
		}
	}
}


void CProgressbarDlgBar::OnBnClickedButtonGaugebarDown()
{
	if(m_bChange)
	{
		UpdateData(true);
		if(m_ProgressbarBase != NULL)
		{
			int iIndex = m_GaugeBarList->GetCurSel();
			m_ProgressbarBase->SelectGaugeBarDown(iIndex);
			
		}
	}
}


void CProgressbarDlgBar::OnBnClickedButtonDeleteGaugebar()
{
	if(m_bChange)
	{
		UpdateData(true);
		if(m_ProgressbarBase != NULL)
		{
			int iIndex = m_GaugeBarList->GetCurSel();
			m_ProgressbarBase->DeleteGaugeBar(iIndex);
			RefreshGaugeBarList();
		}
	}
}


void CProgressbarDlgBar::OnBnClickedButtonAddGaugebar()
{
	if(m_bChange)
	{
		UpdateData(true);
		if(m_ProgressbarBase != NULL)
		{
			m_ProgressbarBase->SetGauge(m_strTexPath, m_ptGaugeSize, m_ptGaugeStartPos, m_ptGaugeIndent);
			RefreshGaugeBarList();
		}
	}
}

void CProgressbarDlgBar::RefreshGaugeBarList()
{
	if(m_bChange)
	{
		UpdateData(true);
		if(m_ProgressbarBase != NULL)
		{
			int iListCount = m_GaugeBarList->GetCount();
			for(int i = iListCount; i >= 0; i--)
			{
				m_GaugeBarList->DeleteString(i);
			}

			std::vector<CAssetBase*>* vecProgressBar = m_ProgressbarBase->GetGaugeBarVector();
			for(int i = 0; i < vecProgressBar->size(); i++)
			{
				char szString[MAX_PATH] = {0,};
				sprintf(szString, "%d%% ~ %d%% 구간", i * 100, (i + 1) * 100);
				CString szAddstring(szString);
				m_GaugeBarList->AddString(szAddstring);
			}
		}
	}
}

void CProgressbarDlgBar::OnEnChangeEditPrgExtragaugeLeft()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptExtraGaugeStartPos.x = GetEditText(m_ExtraGaugeLeftEdit);
		SetProgressbarProperty();
	}
}


void CProgressbarDlgBar::OnEnChangeEditPrgExtragaugeTop()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptExtraGaugeStartPos.y = GetEditText(m_ExtraGaugeTopEdit);
		SetProgressbarProperty();
	}
}


void CProgressbarDlgBar::OnEnChangeEditPrgExtragaugeWidth()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptExtraGaugeSize.x = GetEditText(m_ExtraGaugeWidthEdit);
		SetProgressbarProperty();
	}
}


void CProgressbarDlgBar::OnEnChangeEditPrgExtragaugeHeight()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptExtraGaugeSize.y = GetEditText(m_ExtraGaugeHeightEdit);
		SetProgressbarProperty();
	}
}


void CProgressbarDlgBar::OnEnChangeEditPrgExtragaugePercentage()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_iExtraGauge = GetEditText(m_ExtraGaugePercentageEdit);
		SetProgressbarProperty();
	}
}