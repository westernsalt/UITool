// AssetEditor.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "UITool.h"
#include "AssetEditor.h"

#include "AssetEditorView.h"
#include "AssetSettingDlgBar.h"
#include "AssetBase.h"
#include "ObjMgr.h"
#include "DBMgr.h"
#include "PictureWnd.h"

#include "TextBoxDlgBar.h"
#include "PictureDlgBar.h"
#include "ButtonDlgBar.h"
#include "PicPropDlgBar.h"
#include "CheckBoxDlgBar.h"
#include "CheckBoxBase.h"
#include "RadioBtnDlgBar.h"
#include "RadioBtnBase.h"
#include "SliderDlgBar.h"
#include "ScrollDlgBar.h"
#include "ListItemDlgBar.h"
#include "ListItemBase.h"
#include "ListBoxDlgBar.h"
#include "ListBoxBase.h"
#include "ComboboxDlgBar.h"
#include "InputTextDlgBar.h"
#include "InputText.h"
#include "ProgressbarDlgBar.h"
#include "ProgressBarBase.h"
#include "ImageAnimationDlgbar.h"
#include "ImageAnimationBase.h"
#include "TabBtnDlgBar.h"
#include "TabBtnBase.h"
#include "ThirdGridDlgBar.h"
#include "ThirdGridBase.h"
// CAssetEditor

IMPLEMENT_DYNCREATE(CAssetEditor, CFrameWnd)

CAssetEditor::CAssetEditor()
{
	m_pAssetSettingDlgBar = new CAssetSettingDlgBar;
	m_pAssetPropDlgBar = NULL;
	m_pPictureWnd = NULL;
	m_iTextureListIndex = NOTHING_IN_HERE;
}

CAssetEditor::~CAssetEditor()
{
	CObjMgr::GetInstance()->ClearTargetMap(ASSET_EDITOR_TARGET);
	CDBMgr::GetInstance()->ClearAssetMap(ASSET_EDITOR_TARGET);
	CObjMgr::GetInstance()->SetCurDlgBarIndex(-1);

	m_pAssetSettingDlgBar->DestroyWindow();

	if(m_pAssetPropDlgBar != NULL)
		m_pAssetPropDlgBar->DestroyWindow();

	DestroyWindow();
}


BEGIN_MESSAGE_MAP(CAssetEditor, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_LOAD_ASSET, &CAssetEditor::OnLoadAsset)
	ON_COMMAND(ID_SAVE_ASSET, &CAssetEditor::OnSaveAsset)
	ON_COMMAND(ID_EDITOR_EXIT, &CAssetEditor::OnEditorExit)
	ON_MESSAGE(WM_SAVE_ASSET_MSG, &CAssetEditor::OnSaveAssetMsg)
	ON_MESSAGE(WM_ASSET_BASE_FOCUSED, &CAssetEditor::OnAssetBaseFocused)
	ON_MESSAGE(WM_ASSET_ATT_CHANGE, &CAssetEditor::OnAssetAttChange)
	ON_MESSAGE(WM_PICTUREWND_DESTROY, &CAssetEditor::OnPicturewndDestroy)
	ON_MESSAGE(WM_PICTUREWND_CREATE, &CAssetEditor::OnPicturewndCreate)
	ON_MESSAGE(WM_REFRESH_DLGBAR_INFO, &CAssetEditor::OnRefreshDlgbarInfo)
	ON_MESSAGE(WM_CHANGE_ATTCOMBOBOX, &CAssetEditor::OnChangeAttcombobox)
	ON_MESSAGE(WM_CHANGE_GROUPBUTTON_INDEX, &CAssetEditor::OnChangeGroupbuttonIndex)
	ON_MESSAGE(WM_CLEAR_ALL, &CAssetEditor::OnClearAll)
	ON_MESSAGE(WM_SELCHANGE_TEXTURELIST, &CAssetEditor::OnSelchangeTexturelist)
	ON_MESSAGE(WM_REQUEST_STRING, &CAssetEditor::OnRequestString)
	ON_MESSAGE(WM_INIT_STRING, &CAssetEditor::OnInitString)
	ON_MESSAGE(WM_ASSETEDITOR_ASSETLIST_DOUBLE_CLICK, &CAssetEditor::OnAssetEditorDubleClick)

END_MESSAGE_MAP()

int CAssetEditor::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 프레임의 클라이언트 영역을 차지하는 뷰를 만듭니다.
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("뷰 창을 만들지 못했습니다.\n");
		return -1;
	}

	m_wndView.SetParenthWnd(this->m_hWnd);

	m_pAssetSettingDlgBar->Create(this, IDD_DIALOGBAR_ASSET_SETTING, CBRS_ALIGN_BOTTOM, IDD_DIALOGBAR_ASSET_SETTING);
	EnableDocking(CBRS_ALIGN_ANY);
	m_pAssetSettingDlgBar->SetParenthWnd(this->m_hWnd);
	m_pAssetSettingDlgBar->InitializeRrc();
	m_pAssetSettingDlgBar->UpdateEdit();
	m_pAssetSettingDlgBar->SetPicWndBtnEnable(false);

	m_iCurDlgBarIndex = DLGBAR_PICTURE;
	m_pAssetPropDlgBar = new CPicPropDlgBar();
	m_pAssetPropDlgBar->Create(this, IDD_DIALOGBAR_PICTURE, CBRS_ALIGN_RIGHT, IDD_DIALOGBAR_PICTURE);
	EnableDocking(CBRS_ALIGN_ANY);
	((CPicPropDlgBar*)m_pAssetPropDlgBar)->Initialize();
	CreatePictureWnd();
	((CPicPropDlgBar*)m_pAssetPropDlgBar)->SetViewhWnd(m_pPictureWnd->m_hWnd);

	return 0;
}

// CAssetEditor 메시지 처리기입니다.




void CAssetEditor::OnLoadAsset()
{
	//	로드에셋이라고 하는 메뉴바의 요소가 선택되었을 때
	CObjMgr::GetInstance()->LoadAssetByFileDlg(this->m_hWnd);
}

void CAssetEditor::OnSaveAsset()
{
	//	세이브에셋이라고 하는 메뉴바의 요소가 선택되었을 때
	if(CObjMgr::GetInstance()->SaveAssetByFileDlg() == false)
	{
		MessageBox(TEXT("저장에 실패했습니다."));
	}

	m_pAssetSettingDlgBar->RefreshAssetList();
}


void CAssetEditor::OnEditorExit()
{
	//	나간다고 하는 것은 그냥 단순하게 창을 끄는 행위
	this->DestroyWindow();
}

BOOL CAssetEditor::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_DELETE)
	{
		if(CObjMgr::GetInstance()->GetFocusAssetBase() != NULL)
		{
			if(m_wndView.GetMouseFlag() != MOUSE_LEAVE)
			{
				if(CObjMgr::GetInstance()->GetFocusAssetBase()->GetAssetType() == ASSET_LISTITEM)
				{
					//	delete를 누르면 내부의 에셋 중에 포커스 된 아이가 사라지도록.
					bool bResult = true;
					CListItemBase* FocusAsset = (CListItemBase*)(CObjMgr::GetInstance()->GetFocusAssetBase());
					bResult = FocusAsset->DeleteFocusItem();
					if(bResult)
					{
						CObjMgr::GetInstance()->DeleteAssetBase(CObjMgr::GetInstance()->GetFocusAssetBase());
						CObjMgr::GetInstance()->SetFocusAssetBaseNULL();						
					}
					else
					{
						if(m_iCurDlgBarIndex == DLGBAR_LISTITEM)
							::SendMessage(m_pAssetPropDlgBar->m_hWnd, WM_LISTITEM_ASSET_DELETE, NULL ,NULL);
					}
				}
				else
				{
					CObjMgr::GetInstance()->DeleteAssetBase(CObjMgr::GetInstance()->GetFocusAssetBase());
					CObjMgr::GetInstance()->SetFocusAssetBaseNULL();
					//	에셋을 지우면 다이알로그바의 속성들도 초기화시키도록한다.
					InitializeAssetPropDlgBar();
					InitializePictureWnd();
				}

				m_wndView.OnPaint();
			}
		}
	}

	if (pMsg->message == WM_CHAR)
	{
		if(m_wndView.GetMouseFlag() == MOUSE_HOVER)
		{
			int nCharIndex;
			nCharIndex = m_strInput.GetLength();

			if(pMsg->wParam == VK_BACK)
			{
				m_strInput.Delete(nCharIndex - 1,1);
			}
			else
			{
				m_strInput.AppendChar(pMsg->wParam);
			}
		}
	}

	return CFrameWnd::PreTranslateMessage(pMsg);
}


afx_msg LRESULT CAssetEditor::OnSaveAssetMsg(WPARAM wParam, LPARAM lParam)
{
	OnSaveAsset();
	return 0;
}


afx_msg LRESULT CAssetEditor::OnAssetBaseFocused(WPARAM wParam, LPARAM lParam)
{
	//리스트 아이템일 경우에는 뎁스세팅한테 알려주자
	if(m_iCurDlgBarIndex == DLGBAR_LISTITEM)
		::SendMessage(m_pAssetPropDlgBar->m_hWnd, WM_ASSET_BASE_FOCUSED, NULL ,NULL);

	::SendMessage(m_pAssetSettingDlgBar->m_hWnd, WM_ASSET_BASE_FOCUSED, NULL ,NULL);
	return 0;
}

afx_msg LRESULT CAssetEditor::OnAssetAttChange(WPARAM wParam, LPARAM lParam)
{
	//	이전의 컨트롤바를 삭제하고 걸맞는 컨트롤바를 세팅한다
	if(m_pAssetPropDlgBar != NULL)
	{
		m_pAssetPropDlgBar->DestroyWindow();
		m_pAssetPropDlgBar = NULL;
	}

	CObjMgr::GetInstance()->SetCurDlgBarIndex(wParam);

	switch(wParam)
	{
	case DLGBAR_PICTURE:
		m_iCurDlgBarIndex = DLGBAR_PICTURE;
		m_pAssetPropDlgBar = new CPicPropDlgBar();
		m_pAssetPropDlgBar->Create(this, IDD_DIALOGBAR_PICTURE, CBRS_ALIGN_RIGHT, IDD_DIALOGBAR_PICTURE);
		EnableDocking(CBRS_ALIGN_ANY);
		((CPicPropDlgBar*)m_pAssetPropDlgBar)->Initialize();
		CreatePictureWnd();
		m_pPictureWnd->SetDlgBarhWnd(((CPicPropDlgBar*)m_pAssetPropDlgBar)->m_hWnd);
		((CPicPropDlgBar*)m_pAssetPropDlgBar)->SetViewhWnd(m_pPictureWnd->m_hWnd);
		m_pPictureWnd->SetChildhWnd();
		break;
	case DLGBAR_TEXTBOX:
		m_iCurDlgBarIndex = DLGBAR_TEXTBOX;
		m_pAssetPropDlgBar = new CTextBoxDlgBar();
		m_pAssetPropDlgBar->Create(this, IDD_DIALOGBAR_TEXTBOX, CBRS_ALIGN_RIGHT, IDD_DIALOGBAR_TEXTBOX);
		EnableDocking(CBRS_ALIGN_ANY);
		((CTextBoxDlgBar*)m_pAssetPropDlgBar)->SetViewhWnd(m_hWnd);
		((CTextBoxDlgBar*)m_pAssetPropDlgBar)->Initialize();
		m_pAssetSettingDlgBar->SetPicWndBtnEnable(false);
		if(m_pPictureWnd != NULL)
		{
			m_pPictureWnd->DestroyWindow();
			m_pPictureWnd = NULL;
		}
		break;
	case DLGBAR_BUTTON:
		m_iCurDlgBarIndex = DLGBAR_BUTTON;
		m_pAssetPropDlgBar = new CButtonDlgBar();
		m_pAssetPropDlgBar->Create(this, IDD_DIALOGBAR_BTN, CBRS_ALIGN_RIGHT, IDD_DIALOGBAR_BTN);
		EnableDocking(CBRS_ALIGN_ANY);
		((CButtonDlgBar*)m_pAssetPropDlgBar)->SetParenthWnd(this->m_hWnd);
		((CButtonDlgBar*)m_pAssetPropDlgBar)->Initialize();

		CreatePictureWnd();
		m_pPictureWnd->SetDlgBarhWnd(((CButtonDlgBar*)m_pAssetPropDlgBar)->m_hWnd);
		((CButtonDlgBar*)m_pAssetPropDlgBar)->SetPictureWnd(m_pPictureWnd->m_hWnd);
		m_pPictureWnd->SetChildhWnd();
		break;
	case DLGBAR_SLIDER:
		m_iCurDlgBarIndex = DLGBAR_SLIDER;
		m_pAssetPropDlgBar = new CSliderDlgBar();
		m_pAssetPropDlgBar->Create(this, IDD_DIALOGBAR_SLIDER, CBRS_ALIGN_RIGHT, IDD_DIALOGBAR_SLIDER);
		EnableDocking(CBRS_ALIGN_ANY);
		((CSliderDlgBar*)m_pAssetPropDlgBar)->SetParenthWnd(this->m_hWnd);
		((CSliderDlgBar*)m_pAssetPropDlgBar)->Initialize();

		CreatePictureWnd();
		m_pPictureWnd->SetDlgBarhWnd(((CSliderDlgBar*)m_pAssetPropDlgBar)->m_hWnd);
		((CSliderDlgBar*)m_pAssetPropDlgBar)->SetPictureWnd(m_pPictureWnd->m_hWnd);
		m_pPictureWnd->SetChildhWnd();
		break;
	case DLGBAR_SCROLL:
		m_iCurDlgBarIndex = DLGBAR_SCROLL;
		m_pAssetPropDlgBar = new CScrollDlgBar();
		m_pAssetPropDlgBar->Create(this, IDD_DIALOGBAR_SCROLL, CBRS_ALIGN_RIGHT, IDD_DIALOGBAR_SCROLL);
		EnableDocking(CBRS_ALIGN_ANY);
		((CScrollDlgBar*)m_pAssetPropDlgBar)->SetParenthWnd(this->m_hWnd);
		((CScrollDlgBar*)m_pAssetPropDlgBar)->Initialize();

		CreatePictureWnd();
		m_pPictureWnd->SetDlgBarhWnd(((CScrollDlgBar*)m_pAssetPropDlgBar)->m_hWnd);
		((CScrollDlgBar*)m_pAssetPropDlgBar)->SetPictureWnd(m_pPictureWnd->m_hWnd);
		m_pPictureWnd->SetChildhWnd();
		break;
	case DLGBAR_CHECKBOX:
		m_iCurDlgBarIndex = DLGBAR_CHECKBOX;
		m_pAssetPropDlgBar = new CCheckBoxDlgBar;
		m_pAssetPropDlgBar->Create(this, IDD_DIALOGBAR_CHECKBOX, CBRS_ALIGN_RIGHT, IDD_DIALOGBAR_CHECKBOX);
		EnableDocking(CBRS_ALIGN_ANY);
		((CCheckBoxDlgBar*)m_pAssetPropDlgBar)->SetParenthWnd(this->m_hWnd);
		((CCheckBoxDlgBar*)m_pAssetPropDlgBar)->Initialize();
		CreatePictureWnd();
		m_pPictureWnd->SetDlgBarhWnd(((CCheckBoxDlgBar*)m_pAssetPropDlgBar)->m_hWnd);
		((CCheckBoxDlgBar*)m_pAssetPropDlgBar)->SetPictureWnd(m_pPictureWnd->m_hWnd);
		m_pPictureWnd->SetChildhWnd();
		break;
	case DLGBAR_RADIOBUTTON:
		m_iCurDlgBarIndex = DLGBAR_RADIOBUTTON;
		m_pAssetPropDlgBar = new CRadioBtnDlgBar;
		m_pAssetPropDlgBar->Create(this, IDD_DIALOGBAR_RADIOBTN, CBRS_ALIGN_RIGHT, IDD_DIALOGBAR_RADIOBTN);
		EnableDocking(CBRS_ALIGN_ANY);
		((CRadioBtnDlgBar*)m_pAssetPropDlgBar)->SetParenthWnd(this->m_hWnd);
		((CRadioBtnDlgBar*)m_pAssetPropDlgBar)->Initialize();
		CreatePictureWnd();
		m_pPictureWnd->SetDlgBarhWnd(((CRadioBtnDlgBar*)m_pAssetPropDlgBar)->m_hWnd);
		((CRadioBtnDlgBar*)m_pAssetPropDlgBar)->SetPictureWnd(m_pPictureWnd->m_hWnd);
		m_pPictureWnd->SetChildhWnd();
		break;
	case DLGBAR_LISTITEM:
		//	리스트 아이템은 픽쳐윈도우를 열지 않기로 한다.
		//	이미 완성되어있는 에셋을 바탕으로 만들기로 했으니까.
		m_iCurDlgBarIndex = DLGBAR_LISTITEM;
		m_pAssetPropDlgBar = new CListItemDlgBar;
		m_pAssetPropDlgBar->Create(this, IDD_DIALOGBAR_LISTITEM, CBRS_ALIGN_RIGHT, IDD_DIALOGBAR_LISTITEM);
		EnableDocking(CBRS_ALIGN_RIGHT);
		((CListItemDlgBar*)m_pAssetPropDlgBar)->SetParenthWnd(this->m_hWnd);
		((CListItemDlgBar*)m_pAssetPropDlgBar)->Initialize();
		if(m_pPictureWnd != NULL)
		{
			m_pPictureWnd->DestroyWindow();
			m_pPictureWnd = NULL;
		}
		break;
	case DLGBAR_LISTBOX:
		m_iCurDlgBarIndex = DLGBAR_LISTBOX;
		m_pAssetPropDlgBar = new CListBoxDlgBar;
		m_pAssetPropDlgBar->Create(this, IDD_DIALOGBAR_LISTBOX, CBRS_ALIGN_RIGHT, IDD_DIALOGBAR_LISTBOX);
		EnableDocking(CBRS_ALIGN_RIGHT);
		((CListBoxDlgBar*)m_pAssetPropDlgBar)->SetParenthWnd(this->m_hWnd);
		((CListBoxDlgBar*)m_pAssetPropDlgBar)->Initialize();
		CreatePictureWnd();
		m_pPictureWnd->SetDlgBarhWnd(((CListBoxDlgBar*)m_pAssetPropDlgBar)->m_hWnd);
		((CListBoxDlgBar*)m_pAssetPropDlgBar)->SetPicturehWnd(m_pPictureWnd->m_hWnd);
		m_pPictureWnd->SetChildhWnd();
		break;
	case DLGBAR_COMBOBOX:
		m_iCurDlgBarIndex = DLGBAR_COMBOBOX;
		m_pAssetPropDlgBar = new CComboboxDlgBar;
		m_pAssetPropDlgBar->Create(this, IDD_DIALOGBAR_COMBOBOX, CBRS_ALIGN_RIGHT, IDD_DIALOGBAR_COMBOBOX);
		EnableDocking(CBRS_ALIGN_RIGHT);
		((CComboboxDlgBar*)m_pAssetPropDlgBar)->SetParenthWnd(this->m_hWnd);
		((CComboboxDlgBar*)m_pAssetPropDlgBar)->Initialize();
		CreatePictureWnd();
		m_pPictureWnd->SetDlgBarhWnd(((CComboboxDlgBar*)m_pAssetPropDlgBar)->m_hWnd);
		((CComboboxDlgBar*)m_pAssetPropDlgBar)->SetPictureWnd(m_pPictureWnd->m_hWnd);
		m_pPictureWnd->SetChildhWnd();
		break;
	case DLGBAR_INPUTTEXT:
		m_iCurDlgBarIndex = DLGBAR_INPUTTEXT;
		m_pAssetPropDlgBar = new CInputTextDlgBar;
		m_pAssetPropDlgBar->Create(this, IDD_DIALOGBAR_INPUTTEXT, CBRS_ALIGN_RIGHT, IDD_DIALOGBAR_INPUTTEXT);
		EnableDocking(CBRS_ALIGN_RIGHT);
		((CInputTextDlgBar*)m_pAssetPropDlgBar)->SetParenthWnd(this->m_hWnd);
		((CInputTextDlgBar*)m_pAssetPropDlgBar)->Initialize();
		CreatePictureWnd();
		m_pPictureWnd->SetDlgBarhWnd(((CInputTextDlgBar*)m_pAssetPropDlgBar)->m_hWnd);
		((CInputTextDlgBar*)m_pAssetPropDlgBar)->SetPictureWnd(m_pPictureWnd->m_hWnd);
		m_pPictureWnd->SetChildhWnd();
		break;
	case DLGBAR_PROGRESSBAR:
		m_iCurDlgBarIndex = DLGBAR_PROGRESSBAR;
		m_pAssetPropDlgBar = new CProgressbarDlgBar;
		m_pAssetPropDlgBar->Create(this, IDD_DIALOGBAR_PROGRESSBAR, CBRS_ALIGN_RIGHT, IDD_DIALOGBAR_PROGRESSBAR);
		EnableDocking(CBRS_ALIGN_RIGHT);
		((CProgressbarDlgBar*)m_pAssetPropDlgBar)->SetParenthWnd(this->m_hWnd);
		((CProgressbarDlgBar*)m_pAssetPropDlgBar)->Initialize();
		CreatePictureWnd();
		m_pPictureWnd->SetDlgBarhWnd(((CProgressbarDlgBar*)m_pAssetPropDlgBar)->m_hWnd);
		((CProgressbarDlgBar*)m_pAssetPropDlgBar)->SetPictureWnd(m_pPictureWnd->m_hWnd);
		m_pPictureWnd->SetChildhWnd();
		break;

	case DLGBAR_IMAGEANIMATION:
		m_iCurDlgBarIndex = DLGBAR_IMAGEANIMATION;
		m_pAssetPropDlgBar = new CImageAnimationDlgbar;
		m_pAssetPropDlgBar->Create(this, IDD_DIALOGBAR_IMAGEANIMATION, CBRS_ALIGN_RIGHT, IDD_DIALOGBAR_IMAGEANIMATION);
		EnableDocking(CBRS_ALIGN_RIGHT);
		((CImageAnimationDlgbar*)m_pAssetPropDlgBar)->SetParenthWnd(this->m_hWnd);
		((CImageAnimationDlgbar*)m_pAssetPropDlgBar)->Initialize();
		CreatePictureWnd();
		m_pPictureWnd->SetDlgBarhWnd(((CImageAnimationDlgbar*)m_pAssetPropDlgBar)->m_hWnd);
		((CImageAnimationDlgbar*)m_pAssetPropDlgBar)->SetPictureWnd(m_pPictureWnd->m_hWnd);
		m_pPictureWnd->SetChildhWnd();
		break;
	case DLGBAR_TABBUTTON:
		m_iCurDlgBarIndex = DLGBAR_TABBUTTON;
		m_pAssetPropDlgBar = new CTabBtnDlgBar;
		m_pAssetPropDlgBar->Create(this, IDD_DIALOGBAR_TABBUTTON, CBRS_ALIGN_RIGHT, IDD_DIALOGBAR_TABBUTTON);
		EnableDocking(CBRS_ALIGN_ANY);
		((CTabBtnDlgBar*)m_pAssetPropDlgBar)->SetParenthWnd(this->m_hWnd);
		((CTabBtnDlgBar*)m_pAssetPropDlgBar)->Initialize();
		CreatePictureWnd();
		m_pPictureWnd->SetDlgBarhWnd(((CTabBtnDlgBar*)m_pAssetPropDlgBar)->m_hWnd);
		((CTabBtnDlgBar*)m_pAssetPropDlgBar)->SetPictureWnd(m_pPictureWnd->m_hWnd);
		m_pPictureWnd->SetChildhWnd();
		break;

	case DLGBAR_THIRDGRID:
		m_iCurDlgBarIndex = DLGBAR_THIRDGRID;
		m_pAssetPropDlgBar = new CThirdGridDlgBar;
		m_pAssetPropDlgBar->Create(this, IDD_DIALOGBAR_THIRD_GRID, CBRS_ALIGN_RIGHT, IDD_DIALOGBAR_THIRD_GRID);
		EnableDocking(CBRS_ALIGN_RIGHT);
		((CThirdGridDlgBar*)m_pAssetPropDlgBar)->SetParenthWnd(this->m_hWnd);
		((CThirdGridDlgBar*)m_pAssetPropDlgBar)->Initialize();
		CreatePictureWnd();
		m_pPictureWnd->SetDlgBarhWnd(((CThirdGridDlgBar*)m_pAssetPropDlgBar)->m_hWnd);
		((CThirdGridDlgBar*)m_pAssetPropDlgBar)->SetPictureWnd(m_pPictureWnd->m_hWnd);
		m_pPictureWnd->SetChildhWnd();
		break;
	}

	ShowControlBar(m_pAssetPropDlgBar, 1, 0);
	return 0;
}

void CAssetEditor::CreatePictureWnd()
{
	//	텍스쳐 윈도우를 만들어보자.
	if(m_pPictureWnd ==  NULL)
	{
		bool bWndCreate = false;
		m_pPictureWnd = new CPictureWnd;

		bWndCreate = m_pPictureWnd->Create(NULL, L"Texture", WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
			| WS_BORDER | WS_SYSMENU, CRect(0,0,1,1), this);

		if(bWndCreate)
		{
			RECT rtEditorWnd;
			GetWindowRect(&rtEditorWnd);
			m_pPictureWnd->ShowWindow(SW_SHOW);
			m_pPictureWnd->MoveWindow(rtEditorWnd.right, rtEditorWnd.top, WND_WIDTH + 360, WND_HEIGHT + 289);
			m_pPictureWnd->SetParenthWnd(m_hWnd);
			m_pPictureWnd->SetDlgBarhWnd(m_pAssetPropDlgBar->m_hWnd);
			//m_pPictureWnd->SetTextureListIndex(m_iTextureListIndex);
			m_pPictureWnd->SetChildhWnd();
		}
		else
		{
			m_pPictureWnd->DestroyWindow();
			m_pPictureWnd = NULL;
		}

		m_pAssetSettingDlgBar->SetPicWndBtnEnable(false);
	}
}

afx_msg LRESULT CAssetEditor::OnPicturewndDestroy(WPARAM wParam, LPARAM lParam)
{
	m_pPictureWnd = NULL;
	m_pAssetSettingDlgBar->SetPicWndBtnEnable(true);
	return 0;
}


afx_msg LRESULT CAssetEditor::OnPicturewndCreate(WPARAM wParam, LPARAM lParam)
{
	CreatePictureWnd();
	return 0;
}


afx_msg LRESULT CAssetEditor::OnRefreshDlgbarInfo(WPARAM wParam, LPARAM lParam)
{
	CBaseInterface* LoadBase = (CBaseInterface*)wParam;
	POINT tStartTex;

	switch(m_iCurDlgBarIndex)
	{
	case DLGBAR_PICTURE:
		((CPicPropDlgBar*)m_pAssetPropDlgBar)->RefreshDlgBar((CAssetBase*)LoadBase, tStartTex);
		SetPictureDlgBarControl(((CAssetBase*)LoadBase)->GetFilePath(),
			((CAssetBase*)LoadBase)->GetFileSize(), ((CAssetBase*)LoadBase)->GetStartPos());
		break;
	case DLGBAR_TEXTBOX:
		((CTextBoxDlgBar*)m_pAssetPropDlgBar)->RefreshDlgBar((CTextBox*)LoadBase);
		break;
	case DLGBAR_BUTTON:
		{
			RECT tButtonSize;
			RECT_INITIALIZE(tButtonSize);
			tButtonSize.right = ((CBtnBase*)LoadBase)->GetSize().x;
			tButtonSize.bottom = ((CBtnBase*)LoadBase)->GetSize().y;
			((CButtonDlgBar*)m_pAssetPropDlgBar)->RefreshDlgBar((CBtnBase*)LoadBase, tStartTex);
			SetPictureDlgBarControl(((CBtnBase*)LoadBase)->GetFilePath(),
				tButtonSize, tStartTex);
		}
		break;
	case DLGBAR_CHECKBOX:
		((CCheckBoxDlgBar*)m_pAssetPropDlgBar)->RefreshDlgBar((CCheckBoxBase*)LoadBase, tStartTex);
		SetPictureDlgBarControl(((CAssetBase*)LoadBase)->GetFilePath(),
			((CAssetBase*)LoadBase)->GetFileSize(), tStartTex);
		break;
	case DLGBAR_RADIOBUTTON:
		{
			CRadioBtnBase* GroupBase = (CRadioBtnBase*)(((CGroupButton*)LoadBase)->GetGroupBase());
			((CRadioBtnDlgBar*)m_pAssetPropDlgBar)->RefreshDlgBar((CGroupButton*)LoadBase, tStartTex);
			SetPictureDlgBarControl(GroupBase[0].GetFilePath(),
				GroupBase[0].GetFileSize(), tStartTex);
		}
		break;
	case DLGBAR_SLIDER:
		((CSliderDlgBar*)m_pAssetPropDlgBar)->RefreshDlgBar((CSliderBase*)LoadBase, tStartTex);
		SetPictureDlgBarControl(((CSliderBase*)LoadBase)->GetBackgroundBar()->GetFilePath(),
			((CSliderBase*)LoadBase)->GetBackgroundBar()->GetFileSize(), tStartTex);
		break;
	case DLGBAR_SCROLL:
		((CScrollDlgBar*)m_pAssetPropDlgBar)->RefreshDlgBar((CScrollBase*)LoadBase, tStartTex);
		SetPictureDlgBarControl(((CScrollBase*)LoadBase)->GetBackgroundBar()->GetFilePath(),
			((CScrollBase*)LoadBase)->GetBackgroundBar()->GetFileSize(), tStartTex);
		break;
	case DLGBAR_LISTITEM:
		//	동기화시키는건데... 이거는 바뀔 일이 딱히 없는 것 같다.
		//	리스트 더블클릭했을때 뎁스세팅 갱신해주자.
		//((CListItemDlgBar*)m_pAssetPropDlgBar)->RefreshDepthSettingList(-1);
		break;
	case DLGBAR_LISTBOX:
		((CListBoxDlgBar*)m_pAssetPropDlgBar)->RefreshDlgBar((CListBoxBase*)LoadBase, tStartTex);
		SetPictureDlgBarControl(((CListBoxBase*)LoadBase)->GetFilePath(),
			((CListBoxBase*)LoadBase)->GetBackgroundSize(), tStartTex);
		break;
	case DLGBAR_COMBOBOX:
		((CComboboxDlgBar*)m_pAssetPropDlgBar)->RefreshDlgBar((CComboBoxBase*)LoadBase, tStartTex);
		SetPictureDlgBarControl(((CComboBoxBase*)LoadBase)->GetFilePath(),
			((CComboBoxBase*)LoadBase)->GetFileSize(), tStartTex);
		break;
	case DLGBAR_INPUTTEXT:
		((CInputTextDlgBar*)m_pAssetPropDlgBar)->RefreshDlgBar((CInputText*)LoadBase, tStartTex);
		SetPictureDlgBarControl(((CInputText*)LoadBase)->GetFilePath(),
			((CInputText*)LoadBase)->GetBackgroundSize(), tStartTex);
		break;
	case DLGBAR_PROGRESSBAR:
		((CProgressbarDlgBar*)m_pAssetPropDlgBar)->RefreshDlgBar((CProgressBarBase*)LoadBase, tStartTex);
		SetPictureDlgBarControl(((CProgressBarBase*)LoadBase)->GetFilePath(),
			((CProgressBarBase*)LoadBase)->GetBackgroundBar()->GetFileSize(), tStartTex);
		break;
	case DLGBAR_IMAGEANIMATION:
		((CImageAnimationDlgbar*)m_pAssetPropDlgBar)->RefreshDlgBar((CImageAnimationBase*)LoadBase, tStartTex);
		SetPictureDlgBarControl(((CImageAnimationBase*)LoadBase)->GetFilePath(),
			((CImageAnimationBase*)LoadBase)->GetSingleImageSizeToRect(), tStartTex);
		break;
	case DLGBAR_TABBUTTON:
		{
		 CTabBtnBase* GroupBase = (CTabBtnBase*)(((CGroupButton*)LoadBase)->GetGroupBase());
		((CTabBtnDlgBar*)m_pAssetPropDlgBar)->RefreshDlgBar((CGroupButton*)LoadBase, tStartTex);
		SetPictureDlgBarControl(GroupBase[0].GetFilePath(),
			GroupBase[0].GetFileSize(), tStartTex);
		}
		break;
	case DLGBAR_THIRDGRID:
		((CThirdGridDlgBar*)m_pAssetPropDlgBar)->RefreshDlgBar((CThirdGridBase*)LoadBase, tStartTex);
		SetPictureDlgBarControl(((CThirdGridBase*)LoadBase)->GetFilePath(), ((CThirdGridBase*)LoadBase)->GetImageSizeToRect(), tStartTex);
		break;
	}
	//	다이알로그의 정보를 갱신해야한다는 메세지 (로드를 했을 경우)
	return 0;
}


afx_msg LRESULT CAssetEditor::OnChangeAttcombobox(WPARAM wParam, LPARAM lParam)
{
	CBaseInterface* newBase = (CBaseInterface*)wParam;
	int iIndex = newBase->GetAssetType();
	m_pAssetSettingDlgBar->SetAttCombobox(iIndex);
	return 0;
}

void CAssetEditor::InitializeAssetPropDlgBar()
{
	switch(m_iCurDlgBarIndex)
	{
	case DLGBAR_PICTURE:
		((CPicPropDlgBar*)m_pAssetPropDlgBar)->Initialize();
		break;
	case DLGBAR_TEXTBOX:
		((CTextBoxDlgBar*)m_pAssetPropDlgBar)->Initialize();
		break;
	case DLGBAR_BUTTON:
		((CButtonDlgBar*)m_pAssetPropDlgBar)->Initialize();
		break;
	case DLGBAR_LISTBOX:
		((CListBoxDlgBar*)m_pAssetPropDlgBar)->Initialize();
		break;
	case DLGBAR_SLIDER:
		((CSliderDlgBar*)m_pAssetPropDlgBar)->Initialize();
		break;
	case DLGBAR_SCROLL:
		((CScrollDlgBar*)m_pAssetPropDlgBar)->Initialize();
		break;
	case DLGBAR_COMBOBOX:
		((CComboboxDlgBar*)m_pAssetPropDlgBar)->Initialize();
		break;
	case DLGBAR_CHECKBOX:
		((CCheckBoxDlgBar*)m_pAssetPropDlgBar)->Initialize();
		break;
	case DLGBAR_RADIOBUTTON:
		((CRadioBtnDlgBar*)m_pAssetPropDlgBar)->Initialize();
		break;
	case DLGBAR_INPUTTEXT:
		((CInputTextDlgBar*)m_pAssetPropDlgBar)->Initialize();
		break;
	case DLGBAR_PROGRESSBAR:
		((CProgressbarDlgBar*)m_pAssetPropDlgBar)->Initialize();
		break;
	case DLGBAR_IMAGEANIMATION:
		((CImageAnimationDlgbar*)m_pAssetPropDlgBar)->Initialize();
		break;
	case DLGBAR_TABBUTTON:
		((CTabBtnDlgBar*)m_pAssetPropDlgBar)->Initialize();
		break;
	case DLGBAR_THIRDGRID:
		((CThirdGridDlgBar*)m_pAssetPropDlgBar)->Initialize();
		break;
	}
}

void CAssetEditor::InitializePictureWnd()
{
	if(m_pPictureWnd != NULL)
	{
		m_pPictureWnd->Initialize();
	}
}


afx_msg LRESULT CAssetEditor::OnChangeGroupbuttonIndex(WPARAM wParam, LPARAM lParam)
{
	//	wParam으로 보내줘야한다, 포인터를.
	//	lParam은 focus된 index를 보내도록한다.
	if(m_iCurDlgBarIndex == DLGBAR_RADIOBUTTON)
	{
		CBaseInterface* LoadBase = (CBaseInterface*)wParam;
		CRadioBtnBase* GroupBase = (CRadioBtnBase*)((CGroupButton*)LoadBase)->GetGroupBase();

		int iNum = -1;
		iNum = GroupBase[lParam].GetTextBox().GetRGB(RGB_R);
		((CRadioBtnDlgBar*)m_pAssetPropDlgBar)->SettingControl(&iNum, RADIOBTNDLGBAR_RGB_R);
		iNum = GroupBase[lParam].GetTextBox().GetRGB(RGB_G);
		((CRadioBtnDlgBar*)m_pAssetPropDlgBar)->SettingControl(&iNum, RADIOBTNDLGBAR_RGB_G);
		iNum = GroupBase[lParam].GetTextBox().GetRGB(RGB_B);
		((CRadioBtnDlgBar*)m_pAssetPropDlgBar)->SettingControl(&iNum, RADIOBTNDLGBAR_RGB_B);

		iNum = GroupBase[lParam].GetIconIndent().x;
		((CRadioBtnDlgBar*)m_pAssetPropDlgBar)->SettingControl(&iNum, RADIOBTNDLGBAR_INDENT_X);
		iNum = GroupBase[lParam].GetIconIndent().y;
		((CRadioBtnDlgBar*)m_pAssetPropDlgBar)->SettingControl(&iNum, RADIOBTNDLGBAR_INDENT_Y);

		std::string Path = GroupBase[lParam].GetIconAssetPath();
		((CRadioBtnDlgBar*)m_pAssetPropDlgBar)->SettingControl(&Path, RADIOBTNDLGBAR_ICON_PATH);

		bool bFront = GroupBase[lParam].GetbIconFront();
		((CRadioBtnDlgBar*)m_pAssetPropDlgBar)->SettingControl(&bFront, RADIOBTNDLGBAR_ICON_POSITION);

		bool bDefaultShow = GroupBase[lParam].GetDefaultShow();
		((CRadioBtnDlgBar*)m_pAssetPropDlgBar)->SettingControl(&bDefaultShow, RADIOBTNDLGBAR_DEFAULT_SHOW);

		bool bFontBold = GroupBase[lParam].GetFontBold();
		((CRadioBtnDlgBar*)m_pAssetPropDlgBar)->SettingControl(&bFontBold, RADIOBTNDLGBAR_FONT_BOLD);
	}
	else if(m_iCurDlgBarIndex == DLGBAR_TABBUTTON)
	{
		CBaseInterface* LoadBase = (CBaseInterface*)wParam;
		CTabBtnBase* GroupBase = (CTabBtnBase*)((CGroupButton*)LoadBase)->GetGroupBase();
	}
	return 0;
}

void CAssetEditor::SetPictureDlgBarControl( std::string _path, RECT _size, POINT _pos )
{
	if(m_pPictureWnd != NULL)
	{
		if(_path != "")
		{
			CString szFilePath(_path.c_str());
			CA2T	RemoveFileSpecPath(_path.c_str());
			PathRemoveFileSpec(RemoveFileSpecPath);
			CString szConvertPath(RemoveFileSpecPath);
			m_pPictureWnd->LoadingTextureList(szConvertPath);

			CString szFilename = PathFindFileName(szFilePath);
			m_pPictureWnd->SetTextureListIndex(szFilename);
			POINT ptSize;
			ptSize.x = _size.right;
			ptSize.y = _size.bottom;
			m_pPictureWnd->SetSize(ptSize);
			m_pPictureWnd->SetPos(_pos);
		}
	}
}


afx_msg LRESULT CAssetEditor::OnClearAll(WPARAM wParam, LPARAM lParam)
{
	InitializePictureWnd();
	InitializeAssetPropDlgBar();

	if(m_iCurDlgBarIndex == DLGBAR_LISTITEM)
		::SendMessage(m_pAssetPropDlgBar->m_hWnd, WM_CLEAR_ALL, wParam ,NULL);

	return 0;
}


afx_msg LRESULT CAssetEditor::OnSelchangeTexturelist(WPARAM wParam, LPARAM lParam)
{
	m_iTextureListIndex = m_pPictureWnd->GetCurSelTextureListItem();
	return 0;
}

afx_msg LRESULT CAssetEditor::OnRequestString(WPARAM wParam, LPARAM lParam)
{
	CString* szCopy = (CString*)wParam;
	(*szCopy) = m_strInput;
	return 0;
}


afx_msg LRESULT CAssetEditor::OnInitString(WPARAM wParam, LPARAM lParam)
{
	m_strInput = "";
	return 0;
}

//에셋 에디터 다이얼로그 리스트에서 더블클릭
afx_msg LRESULT CAssetEditor::OnAssetEditorDubleClick(WPARAM wParam, LPARAM lParam)
{
	if(m_iCurDlgBarIndex == DLGBAR_LISTITEM)
		::SendMessage(m_pAssetPropDlgBar->m_hWnd, WM_ASSETEDITOR_ASSETLIST_DOUBLE_CLICK, wParam ,NULL);
	return 0;
}