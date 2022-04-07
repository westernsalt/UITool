
// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "UITool.h"

#include "MainFrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "SceneEditDlgBar.h"
#include "SceneSettingDlgBar.h"
#include "AssetEditor.h"
#include "DBMgr.h"
#include "ObjMgr.h"
#include "BasicAsset.h"
#include "SceneAssetPreviewWnd.h"

//#include "BugTrap/BugTrap.h"
//#pragma comment(lib,"BugTrap/BugTrap.lib")

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_OPEN_EDITOR, &CMainFrame::OnOpenEditor)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_BASIC_ASSET_FOCUSED, &CMainFrame::OnBasicAssetFocused)
	ON_MESSAGE(WM_REQUEST_STRING, &CMainFrame::OnRequestString)
	ON_MESSAGE(WM_INIT_STRING, &CMainFrame::OnInitString)
	ON_MESSAGE(WM_UIFRAME_FOCUSED, &CMainFrame::OnUiframeFocused)
	ON_MESSAGE(WM_CHANGE_RESOLUTION, &CMainFrame::OnChangeResolution)

	ON_MESSAGE(WM_ASSETLIST_DOUBLE_CLICK, &CMainFrame::OnAssetListDoubleClick)
	ON_MESSAGE(WM_FRAMELIST_DOUBLE_CLICK, &CMainFrame::OnFrameListDoubleClick)
	ON_MESSAGE(WM_SCENELIST_DOUBLE_CLICK, &CMainFrame::OnSceneListDoubleClick)
	ON_MESSAGE(WM_ASSET_PREVIEW, &CMainFrame::OnSelectAssetPreView)
	ON_MESSAGE(WM_FRAME_PREVIEW, &CMainFrame::OnSelectFramePreView)	
	ON_MESSAGE(WM_SCENE_ASSET_PREVIEW_DESTROY, &CMainFrame::OnSceneAssetWndDestroy)
END_MESSAGE_MAP()

// CMainFrame 생성/소멸
std::string g_strAttribute[ASSET_END];

CMainFrame::CMainFrame()
{
	m_pAssetEditFrame = NULL;
	m_pSceneEditDlgBar = new CSceneEditDlgBar;
	m_pSceneSettingDlgBar = new CSceneSettingDlgBar;
	m_pSceneAssetPreviewWnd = NULL;

	setlocale(LC_ALL, "korean");

	g_strAttribute[ASSET_PICTURE]		= "PICTURE";
	g_strAttribute[ASSET_TEXTBOX]		= "TEXTBOX";
	g_strAttribute[ASSET_BUTTON]		= "BUTTON";
	g_strAttribute[ASSET_CHECKBOX]		= "CHECKBOX";
	g_strAttribute[ASSET_RADIOBUTTON]	= "RADIOBUTTON";
	g_strAttribute[ASSET_INPUTTEXT]		= "INPUTTEXT";
	g_strAttribute[ASSET_LISTBOX]		= "LISTBOX";
	g_strAttribute[ASSET_COMBOBOX]		= "COMBOBOX";
	g_strAttribute[ASSET_SLIDER]		= "SLIDER";
	g_strAttribute[ASSET_SCROLL]		= "SCROLL";
	g_strAttribute[ASSET_LISTITEM]		= "LISTITEM";
	g_strAttribute[ASSET_PROGRESSBAR]	= "PROGRESSBAR";
	g_strAttribute[ASSET_IMAGEANIMATION]= "IMAGEANIMATION";
	g_strAttribute[ASSET_TABBUTTON]		= "TABBUTTON";
	g_strAttribute[ASSET_THIRDGRID]		= "THIRDGRID";

#ifdef _DEBUG
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
#endif
	//BT_SetAppName(_T("UIToolDumpFile"));
	//BT_SetFlags(BTF_DETAILEDMODE | /*BTF_EDITMAIL |*/BTF_SCREENCAPTURE | BTF_ATTACHREPORT);	

	//TCHAR szExePath[250];
	//GetCurrentDirectory(sizeof(szExePath),szExePath);
	//BT_SetReportFilePath(szExePath);

	//BT_SetActivityType(BTA_SAVEREPORT);
}

CMainFrame::~CMainFrame()
{
	m_pSceneEditDlgBar->DestroyWindow();
	m_pSceneSettingDlgBar->DestroyWindow();

	if(m_pSceneAssetPreviewWnd != NULL)
		m_pSceneAssetPreviewWnd->DestroyWindow();

	DestroyWindow();
#ifdef _DEBUG
	FreeConsole();
#endif
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	return TRUE;
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
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

	CDBMgr::Initialize();
	CObjMgr::Initialize();
	
	// DlgBar 붙이기.
	if(!m_pSceneEditDlgBar->Create(this, IDD_DIALOGBAR_LIST, CBRS_RIGHT, IDD_DIALOGBAR_LIST))
		return -1;
	m_pSceneEditDlgBar->EnableDocking(CBRS_ALIGN_ANY);
	m_pSceneEditDlgBar->SetParenthWnd(this->m_hWnd);
	m_pSceneEditDlgBar->SetViewhWnd(m_wndView.m_hWnd);
	m_pSceneEditDlgBar->Initialize();

	if(!m_pSceneSettingDlgBar->Create(this, IDD_DIALOGBAR_SETTING, CBRS_LEFT|CBRS_BOTTOM, IDD_DIALOGBAR_SETTING))
		return -1;
	m_pSceneSettingDlgBar->EnableDocking(CBRS_ALIGN_ANY);
	m_pSceneSettingDlgBar->SetParenthWnd(this->m_hWnd);
	m_pSceneSettingDlgBar->UpdateEdit();

#ifdef _DEBUG
	CreateDirectory(_T("Scene"), NULL);
	CreateDirectory(_T("Asset"), NULL);
	CreateDirectory(_T("Frame"), NULL);
	CreateDirectory(_T("Texture"), NULL);
	CreateDirectory(_T("FontFile"), NULL);
	CreateDirectory(_T("Scene800x600"), NULL);
	CreateDirectory(_T("Frame800x600"), NULL);
#endif

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		 | WS_BORDER | WS_SYSMENU;
	//	WS_THICKFRAME을 WS_BORDER를 대체할 경우 사이즈를 조절할 수 있게 된다
	//	WS_MINIMIZEBOX | WS_MAXIMIZEBOX |

	cs.cx = WINDOW_SIZE_X + SCENE_EDIT_DLGBAR_X;
	cs.cy = WINDOW_SIZE_Y + SCENE_SETTING_DLGBAR_Y;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// 뷰 창으로 포커스를 이동합니다.
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// 뷰에서 첫째 크랙이 해당 명령에 나타나도록 합니다.
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// 그렇지 않으면 기본 처리합니다.
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnOpenEditor()
{	
	m_pAssetEditFrame = new CAssetEditor();
	m_pAssetEditFrame->Create(NULL, L"AssetEditor",WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		| WS_BORDER | WS_SYSMENU, CRect(0, 0, WINDOW_SIZE_X + ASSET_EDIT_DLGBAR_X,
		WINDOW_SIZE_Y + ASSET_SETTING_DLGBAR_Y ), CWnd::GetDesktopWindow(), MAKEINTRESOURCE(IDR_MENU_EDITOR));
	m_pAssetEditFrame->ShowWindow(SW_SHOW);
	m_pAssetEditFrame->UpdateWindow();

	return;
}

void CMainFrame::OnDestroy()
{
	CObjMgr::Destroy();
	CDBMgr::Destroy();
	CFrameWnd::OnDestroy();
}

void CMainFrame::CreateSceneAssetPreviewWnd(int nFocusIndex, eRefreshType assetType)
{
	//	텍스쳐 윈도우를 만들어보자.
	if(m_pSceneAssetPreviewWnd ==  NULL)
	{
		bool bWndCreate = false;
		m_pSceneAssetPreviewWnd = new CSceneAssetPreviewWnd;

		bWndCreate = m_pSceneAssetPreviewWnd->Create(NULL, L"AssetPreview", WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
			| WS_BORDER | WS_SYSMENU, CRect(0,0,1,1), this);

		if(bWndCreate)
		{
			RECT rtEditorWnd;
			GetWindowRect(&rtEditorWnd);
			m_pSceneAssetPreviewWnd->ShowWindow(SW_SHOW);
			m_pSceneAssetPreviewWnd->MoveWindow(rtEditorWnd.right, rtEditorWnd.top, WND_WIDTH + 360, WND_HEIGHT + 289);
			m_pSceneAssetPreviewWnd->SetParenthWnd(this->m_hWnd);
			m_pSceneAssetPreviewWnd->SetFocusAssetIndex(nFocusIndex, assetType);
		}
		else
		{
			m_pSceneAssetPreviewWnd->DestroyWindow();
			m_pSceneAssetPreviewWnd = NULL;
		}
	}
	else
		m_pSceneAssetPreviewWnd->SetFocusAssetIndex(nFocusIndex, assetType);
}

afx_msg LRESULT CMainFrame::OnBasicAssetFocused(WPARAM wParam, LPARAM lParam)
{
	::SendMessage(m_pSceneSettingDlgBar->m_hWnd, WM_BASIC_ASSET_FOCUSED, NULL, NULL);
	return 0;
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN
		&& m_pSceneSettingDlgBar->GetbEditFocused() == false)
	{
		POINT ps;
		ps.x = 0;
		ps.y = 0;
		CBasicAsset* FocusAsset = NULL;
		CUIFrame* FocusFrame = NULL;

		FocusAsset = CObjMgr::GetInstance()->GetFocusBasicAsset();
		FocusFrame = CObjMgr::GetInstance()->GetFocusUIFrame();

		switch(pMsg->wParam)
		{
		case VK_DELETE:
			{
				if(FocusAsset != NULL)
				{
					CObjMgr::GetInstance()->DeleteBasicAsset(FocusAsset);
					CObjMgr::GetInstance()->SetFocusBasicAssetNULL();
					::SendMessage(m_pSceneSettingDlgBar->m_hWnd, WM_ASSETLIST_DOUBLE_CLICK, NULL, NULL);
				}
				
				if(FocusFrame != NULL)
				{
					CObjMgr::GetInstance()->DeleteUIFrame(FocusFrame);
					CObjMgr::GetInstance()->SetFocusUIFrameNULL();
					::SendMessage(m_pSceneSettingDlgBar->m_hWnd, WM_SCENELIST_DOUBLE_CLICK, NULL, NULL);
				}
				return CFrameWnd::PreTranslateMessage(pMsg);
			}
		}

		if(FocusAsset != NULL)
		{
			CObjMgr::GetInstance()->GetFocusBasicAsset()->AddPosition(ps);
			CObjMgr::GetInstance()->GetFocusBasicAsset()->AddAssetBasePos(ps);
		}
		
		m_pSceneSettingDlgBar->RefreshPosEdit(REFRESH_ASSET);
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

afx_msg LRESULT CMainFrame::OnRequestString(WPARAM wParam, LPARAM lParam)
{
	CString* szCopy = (CString*)wParam;
	(*szCopy) = m_strInput;
	return 0;
}


afx_msg LRESULT CMainFrame::OnInitString(WPARAM wParam, LPARAM lParam)
{
	m_strInput = "";
	return 0;
}


afx_msg LRESULT CMainFrame::OnUiframeFocused(WPARAM wParam, LPARAM lParam)
{
	::SendMessage(m_pSceneSettingDlgBar->m_hWnd, WM_UIFRAME_FOCUSED, NULL, NULL);
	return 0;
}


afx_msg LRESULT CMainFrame::OnChangeResolution(WPARAM wParam, LPARAM lParam)
{
	// wParam과 lParam으로 바뀌어야하는 해상도값이 들어올 것이다.
	if(wParam == WND_WIDTH)
	{
		this->SetWindowPos(NULL, 0, 0, WINDOW_SIZE_X + SCENE_EDIT_DLGBAR_X,
			WINDOW_SIZE_Y + SCENE_SETTING_DLGBAR_Y, SWP_NOMOVE);
	}
	else if(wParam == WND_SMALL_WIDTH)
	{
		this->SetWindowPos(NULL, 0, 0, WINDOW_SMALL_SIZE_X + SCENE_EDIT_DLGBAR_X,
			WINDOW_SMALL_SIZE_Y + SCENE_SETTING_DLGBAR_Y, SWP_NOMOVE);
	}
	return 0;
}

afx_msg LRESULT CMainFrame::OnAssetListDoubleClick(WPARAM wParam, LPARAM lParam)
{
	::SendMessage(m_pSceneSettingDlgBar->m_hWnd, WM_ASSETLIST_DOUBLE_CLICK, NULL, NULL);
	return 0;
}

afx_msg LRESULT CMainFrame::OnFrameListDoubleClick(WPARAM wParam, LPARAM lParam)
{
	::SendMessage(m_pSceneSettingDlgBar->m_hWnd, WM_FRAMELIST_DOUBLE_CLICK, NULL, NULL);
	return 0;
}

afx_msg LRESULT CMainFrame::OnSceneListDoubleClick(WPARAM wParam, LPARAM lParam)
{
	::SendMessage(m_pSceneSettingDlgBar->m_hWnd, WM_SCENELIST_DOUBLE_CLICK, NULL, NULL);
	return 0;
}

afx_msg LRESULT CMainFrame::OnSelectAssetPreView(WPARAM wParam, LPARAM lParam)
{
	CreateSceneAssetPreviewWnd((int)wParam, REFRESH_ASSET);
	return 0;
}

afx_msg LRESULT CMainFrame::OnSelectFramePreView(WPARAM wParam, LPARAM lParam)
{
	CreateSceneAssetPreviewWnd((int)wParam, REFRESH_FRAME);
	return 0;
}

afx_msg LRESULT CMainFrame::OnSceneAssetWndDestroy(WPARAM wParam, LPARAM lParam)
{
	m_pSceneAssetPreviewWnd->DestroyWindow();
	m_pSceneAssetPreviewWnd = NULL;
	return 0;
}