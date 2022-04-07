// SceneEditDlgBar.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "UITool.h"
#include "SceneEditDlgBar.h"

#include "UIFrameFileList.h"
#include "ObjMgr.h"
#include "DBMgr.h"
#include "BasicAsset.h"
#include "UIFrame.h"
#include "UIScene.h"

// CSceneEditDlgBar

IMPLEMENT_DYNAMIC(CSceneEditDlgBar, CDialogBar)

CSceneEditDlgBar::CSceneEditDlgBar()
{
}

CSceneEditDlgBar::~CSceneEditDlgBar()
{
	DestroyWindow();
}

BEGIN_MESSAGE_MAP(CSceneEditDlgBar, CDialogBar)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_LOAD_SCENE, &CSceneEditDlgBar::OnUpdateCmdUI)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_SAVE_SCENE, &CSceneEditDlgBar::OnUpdateCmdUI)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_ASSET_LOAD, &CSceneEditDlgBar::OnUpdateCmdUI)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_ASSET_REFRESH, &CSceneEditDlgBar::OnUpdateCmdUI)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_FRAME_LOAD, &CSceneEditDlgBar::OnUpdateCmdUI)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_FRAME_SAVE, &CSceneEditDlgBar::OnUpdateCmdUI)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_LOAD_BG_ASSET, &CSceneEditDlgBar::OnUpdateCmdUI)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_LOAD_BLUR_ASSET, &CSceneEditDlgBar::OnUpdateCmdUI)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_SCENE_ALLCLEAR, &CSceneEditDlgBar::OnUpdateCmdUI)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_LOAD_REVISE_FRAME, &CSceneEditDlgBar::OnUpdateCmdUI)
	
	ON_BN_CLICKED(IDC_BUTTON_ASSET_LOAD, &CSceneEditDlgBar::OnBnClickedButtonAssetlistLoad)
	ON_BN_CLICKED(IDC_BUTTON_ASSET_REFRESH, &CSceneEditDlgBar::OnBnClickedButtonAssetlistRefresh)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_SCENE, &CSceneEditDlgBar::OnBnClickedButtonSceneLoad)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_SCENE, &CSceneEditDlgBar::OnBnClickedButtonSceneSave)

	ON_LBN_SELCHANGE(IDC_LIST_ASSET, &CSceneEditDlgBar::OnLbnSelchangeListAsset)
	ON_LBN_DBLCLK(IDC_LIST_ASSET, &CSceneEditDlgBar::OnLbnDblclkListAsset)

	ON_LBN_SELCHANGE(IDC_LIST_FRAME, &CSceneEditDlgBar::OnLbnSelchangeListFrame)
	ON_LBN_DBLCLK(IDC_LIST_FRAME, &CSceneEditDlgBar::OnLbnDblclkListFrame)

	ON_LBN_SELCHANGE(IDC_LIST_SCENE, &CSceneEditDlgBar::OnLbnSelchangeListScene)
	ON_LBN_DBLCLK(IDC_LIST_SCENE, &CSceneEditDlgBar::OnLbnDblclkListScene)

	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTON_FRAME_LOAD, &CSceneEditDlgBar::OnBnClickedButtonFrameLoad)
	ON_BN_CLICKED(IDC_BUTTON_FRAME_SAVE, &CSceneEditDlgBar::OnBnClickedButtonFrameSave)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_BG_ASSET, &CSceneEditDlgBar::OnBnClickedButtonLoadBgAsset)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_BLUR_ASSET, &CSceneEditDlgBar::OnBnClickedButtonLoadBlurAsset)
	ON_BN_CLICKED(IDC_BUTTON_SCENE_ALLCLEAR, &CSceneEditDlgBar::OnBnClickedButtonSceneAllclear)
	ON_EN_CHANGE(IDC_EDIT_LOAD_REVISE_FRAME, &CSceneEditDlgBar::OnEnChangeEditLoadReviseFrame)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_REVISE_FRAME, &CSceneEditDlgBar::OnBnClickedButtonLoadReviseFrame)
END_MESSAGE_MAP()

void CSceneEditDlgBar::OnUpdateCmdUI(CCmdUI* pCmdUI) // 다이얼로그바 버튼 활성화 위해 추가
{
	pCmdUI->Enable(TRUE);
}

// CSceneEditDlgBar 메시지 처리기입니다.

int CSceneEditDlgBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CSceneEditDlgBar::OnBnClickedButtonAssetlistLoad()
{
	BrowseFolder(m_strAssetListPath, TYPE_ASSETLIST);
	LoadAssetList(m_strAssetListPath);
}


void CSceneEditDlgBar::OnBnClickedButtonSceneLoad()
{
	BrowseFolder(m_strSceneListPath, TYPE_SCENELIST);
	LoadSceneList(m_strSceneListPath);
}


void CSceneEditDlgBar::OnBnClickedButtonSceneSave()
{
	if(CObjMgr::GetInstance()->GetTargetMapCount(SCENE_EDITOR_FRAME_TARGET)  == 0)
	{
		return;
	}

	CObjMgr::GetInstance()->SaveSceneByFileDlg();
	LoadSceneList(m_strSceneListPath);
}

void CSceneEditDlgBar::OnBnClickedButtonFrameLoad()
{
	BrowseFolder(m_strFrameListPath, TYPE_FRAMELIST);
	LoadUIFrameList(m_strFrameListPath);
}


void CSceneEditDlgBar::OnBnClickedButtonFrameSave()
{
	if(CObjMgr::GetInstance()->GetTargetMapCount(SCENE_EDITOR_TARGET)  == 0)
	{
		return;
	}
	CObjMgr::GetInstance()->SaveFrameByFileDlg();
	LoadUIFrameList(m_strFrameListPath);
}

void CSceneEditDlgBar::OnBnClickedButtonLoadBgAsset()
{
	//	불러오기를 했을 때에 여기서 창을 띄워주고 작업을 해줘야한다
	CString strPathName;
	std::string strRai = "rai";
	CA2T wt(strRai.c_str());
	CFileDialog FileDlg(TRUE, wt, NULL);

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
		UpdateData(true);
		CString FileName = FileDlg.GetFileName();
		strPathName = FileDlg.GetPathName();
		CT2CA pszConvertedAnsiString (strPathName);
		std::string _Path(pszConvertedAnsiString);

		//	백그라운드 에셋을 로딩하는데, 제대로된 에셋인 경우에만 들어와서 셋팅한다.
		if(CObjMgr::GetInstance()->LoadBackgroundAsset(_Path, m_ParenthWnd, m_ViewhWnd))
		{
			m_strBackgroundPath = strPathName;
			m_BackgroundPathEdit->SetWindowText(m_strBackgroundPath);
		}
	}
}

void CSceneEditDlgBar::OnBnClickedButtonLoadBlurAsset()
{
	//	불러오기를 했을 때에 여기서 창을 띄워주고 작업을 해줘야한다
	CString strPathName;
	std::string strRai = "rai";
	CA2T wt(strRai.c_str());
	CFileDialog FileDlg(TRUE, wt, NULL);

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
		UpdateData(true);
		CString FileName = FileDlg.GetFileName();
		strPathName = FileDlg.GetPathName();
		CT2CA pszConvertedAnsiString (strPathName);
		std::string _Path(pszConvertedAnsiString);

		//	블러 에셋을 로딩하는데, 제대로된 에셋인 경우에만 들어와서 셋팅한다.
		if(CObjMgr::GetInstance()->LoadBlurImageAsset(_Path, m_ParenthWnd, m_ViewhWnd))
		{
			m_strBlurPath = strPathName;
			m_BlurPathEdit->SetWindowText(m_strBlurPath);
		}
	}
}


void CSceneEditDlgBar::BrowseFolder(CString& _strPath, eListType _type)
{
	//	에셋 로드 경로를 설정한다
	ITEMIDLIST*	pildBrowse;
	TCHAR		pszPathname[MAX_PATH];
	BROWSEINFO	bInfo;
	memset(&bInfo, 0, sizeof(bInfo));

	LPCTSTR lpszDefaultFolder = NULL;

	switch(_type)
	{
	case TYPE_ASSETLIST:
		lpszDefaultFolder = m_strAssetListPath;
		break;
	case TYPE_FRAMELIST:
		lpszDefaultFolder = m_strFrameListPath;
		break;
	case TYPE_SCENELIST:
		lpszDefaultFolder = m_strSceneListPath;
		break;
	}


	//	루트 경로 지정 루틴
	//**********************************************************************
	TCHAR sPath[MAX_PATH] = {0};
	::GetCurrentDirectory(MAX_PATH, sPath);
	// 패스 변경 : Release 
#ifdef RELEASE_PATH
	PathAppend(sPath, L"\\..\\DataSetup");
#endif
	LPITEMIDLIST pStartDir;
	ULONG nCharsParsed = 0;
	LPSHELLFOLDER pShellFolder = NULL;
	SHGetDesktopFolder(&pShellFolder);
	HRESULT hr = pShellFolder->ParseDisplayName(NULL, NULL, sPath, &nCharsParsed, &pStartDir, NULL);
	pShellFolder->Release();
	//**********************************************************************

	//	구조체 설정
	bInfo.hwndOwner			= GetSafeHwnd();
	bInfo.pidlRoot			= pStartDir;
	bInfo.pszDisplayName	= pszPathname;
	bInfo.lpszTitle			= _T("디렉토리를 선택하세요.");
	bInfo.ulFlags			= BIF_RETURNONLYFSDIRS;
	bInfo.lpfn				= BrowseCallbackProc;
	bInfo.lParam			= (LPARAM)lpszDefaultFolder;
	pildBrowse    = ::SHBrowseForFolder(&bInfo);

	if(pildBrowse)
	{
		SHGetPathFromIDList(pildBrowse, pszPathname);
		CString strTemp(pszPathname);
		_strPath = strTemp;

		switch(_type)
		{
		case TYPE_ASSETLIST:
			m_strAssetListPath = _strPath;
			break;
		case TYPE_FRAMELIST:
			m_strFrameListPath = _strPath;
			break;
		case TYPE_SCENELIST:
			m_strSceneListPath = _strPath;
			break;
		}
	}

	IMalloc* pMalloc = NULL;

	if (::SHGetMalloc(&pMalloc) != NOERROR)
	{
		return;
	}

	//메모리 해제를 한다. 
	pMalloc->Free(pildBrowse);
	pMalloc->Release();
}

int CALLBACK CSceneEditDlgBar::BrowseCallbackProc( HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData )
{
	CHAR lpszDir[MAX_PATH];
	switch (uMsg)
	{
	case BFFM_INITIALIZED:
		::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
		break;
	case BFFM_SELCHANGED:
		if(SHGetPathFromIDList((LPITEMIDLIST)lParam, (LPWSTR)lpszDir))
		{
			::SendMessage(hwnd, BFFM_SETSTATUSTEXT, 0, (LPARAM)lpszDir);
		}
	}
	return 0;
}

void CSceneEditDlgBar::SetParenthWnd( HWND _hWnd )
{
	m_ParenthWnd = _hWnd; 
}


void CSceneEditDlgBar::OnBnClickedButtonAssetlistRefresh()
{
	LoadAssetList(m_strAssetListPath);
}


void CSceneEditDlgBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);

	m_AssetList = (CListBox*)GetDlgItem(IDC_LIST_ASSET);
	m_UIFrameList = (CListBox*)GetDlgItem(IDC_LIST_FRAME);
	m_SceneList = (CListBox*)GetDlgItem(IDC_LIST_SCENE);
	m_BackgroundPathEdit = (CEdit*)GetDlgItem(IDC_EDIT_SCENE_BACKGROUND_PATH);
	m_BlurPathEdit = (CEdit*)GetDlgItem(IDC_EDIT_SCENE_BLUR_PATH);
}

void CSceneEditDlgBar::LoadAssetList(CString _path)
{
	UpdateData(TRUE);

	CDBMgr::GetInstance()->ClearAssetMap(SCENE_EDITOR_TARGET);

	for(int i = m_AssetList->GetCount(); i >= 0; i--)
	{
		m_AssetList->DeleteString(i);
	}

	CString strInputPath(_path);

	TCHAR szLoadingPath[MAX_PATH] = L"";
	_tcscpy_s(szLoadingPath, _countof(szLoadingPath), strInputPath.GetBuffer());
	_tcscat_s(szLoadingPath, L"\\*.rai");

	CFileFind finder;
	int iIndex;
	bool bWorking = finder.FindFile((LPCTSTR)szLoadingPath);

	while(bWorking)
	{
		bWorking = finder.FindNextFile();
		CBasicAsset LoadBasicAsset;
		if(!finder.IsDots() && !finder.IsDirectory())
		{
			iIndex = m_AssetList->GetCount();
			LoadBasicAsset = CDBMgr::GetInstance()->LoadBasicAsset(iIndex, finder.GetFilePath(), finder.GetFileName(), SCENE_EDITOR_TARGET);
			if(/*LoadBasicAsset.GetAssetType() != ASSET_LISTITEM &&*/ LoadBasicAsset.GetAssetType() != ASSET_SCROLL)
			{
				m_AssetList->AddString(finder.GetFileName());
			}
			else 
			{
				CDBMgr::GetInstance()->DeleteBasicAsset(iIndex, SCENE_EDITOR_TARGET);
			}
		}
	}
}

void CSceneEditDlgBar::LoadUIFrameList(CString _path)
{
	UpdateData(TRUE);

	for(int i = m_UIFrameList->GetCount(); i >= 0; i--)
	{
		m_UIFrameList->DeleteString(i);
	}

	CDBMgr::GetInstance()->ClearFrameMap();

	CString strInputPath(_path);

	TCHAR szLoadingPath[MAX_PATH] = L"";
	_tcscpy_s(szLoadingPath, _countof(szLoadingPath), strInputPath.GetBuffer());
	_tcscat_s(szLoadingPath, L"\\*.rfi");

	CFileFind finder;
	int iIndex;
	bool bWorking = finder.FindFile((LPCTSTR)szLoadingPath);
	bool bAddOkay = false;
	while(bWorking)
	{
		bWorking = finder.FindNextFile();
		if(!finder.IsDots() && !finder.IsDirectory())
		{
			iIndex = m_UIFrameList->GetCount();
			bAddOkay = CDBMgr::GetInstance()->LoadUIFrame(iIndex, finder.GetFilePath(), m_ParenthWnd);
		}

		if(bAddOkay)
		{
			m_UIFrameList->AddString(finder.GetFileName());
		}
	}
}

void CSceneEditDlgBar::LoadSceneList( CString _path )
{
	UpdateData(TRUE);

	for(int i = m_SceneList->GetCount(); i >= 0; i--)
	{
		m_SceneList->DeleteString(i);
	}

	CDBMgr::GetInstance()->ClearSceneMap();

	CString strInputPath(_path);

	TCHAR szLoadingPath[MAX_PATH] = L"";
	_tcscpy_s(szLoadingPath, _countof(szLoadingPath), strInputPath.GetBuffer());
	_tcscat_s(szLoadingPath, L"\\*.rsi");

	CFileFind finder;
	int iIndex;
	bool bWorking = finder.FindFile((LPCTSTR)szLoadingPath);
	bool bAddOkay = false;

	while(bWorking)
	{
		bWorking = finder.FindNextFile();
		if(!finder.IsDots() && !finder.IsDirectory())
		{
			iIndex = m_SceneList->GetCount();
			bAddOkay = CDBMgr::GetInstance()->LoadUIScene(iIndex, finder.GetFilePath(), m_ParenthWnd);
		}

		if(bAddOkay)
		{
			m_SceneList->AddString(finder.GetFileName());
		}
	}
}


void CSceneEditDlgBar::OnLbnSelchangeListAsset()
{
	UpdateData(true);
	m_iFocusAssetIndex = m_AssetList->GetCurSel();

	//에셋 프리뷰
	::SendMessage(m_ParenthWnd, WM_ASSET_PREVIEW, m_iFocusAssetIndex, NULL);
}


void CSceneEditDlgBar::OnLbnDblclkListAsset()
{
	UpdateData(true);
	//	데이터베이스매니저의 원본 포인터를 지역변수로 가지고 있는데 지역변수가 소멸하면서 
	//	원본데이터와 공유하고 있던 정보를 날려버린다.
	CBasicAsset* FocusAsset = CDBMgr::GetInstance()->GetBasicAsset(m_iFocusAssetIndex, SCENE_EDITOR_TARGET);
	CBasicAsset* CopyAsset = new CBasicAsset(FocusAsset);
	CopyAsset->Initialize();
	CopyAsset->SetHitBox();
	CopyAsset->SetParenthWnd(m_ParenthWnd);
	CObjMgr::GetInstance()->InsertBasicAsset(CopyAsset);
	//뎁스세팅리스트 갱신
	::SendMessage(m_ParenthWnd, WM_ASSETLIST_DOUBLE_CLICK, NULL, NULL);
}


void CSceneEditDlgBar::OnLbnSelchangeListFrame()
{
	UpdateData(true);
	m_iFocusFrameIndex = m_UIFrameList->GetCurSel();

	//프레임 프리뷰
	::SendMessage(m_ParenthWnd, WM_FRAME_PREVIEW, m_iFocusFrameIndex, NULL);
}

//씬에디터 오른쪽 프레임리스트 더블클릭
void CSceneEditDlgBar::OnLbnDblclkListFrame()
{
	UpdateData(true);

	CUIFrame* FocusFrame = CDBMgr::GetInstance()->GetUIFrame(m_iFocusFrameIndex);
	FocusFrame->Initialize();
	if(FocusFrame != NULL)
	{
		CUIFrame*	CopyFrame = new CUIFrame(FocusFrame);
		CopyFrame->MakeOriginalFilePath();
		CopyFrame->SetMainFrmhWnd(m_ParenthWnd);
		CObjMgr::GetInstance()->InsertUIFrame(CopyFrame);

		//불러온 프레임 파일 이름 저장
		CObjMgr::GetInstance()->SetSaveFrameFileName(CopyFrame->GetFrameFileName());

		//뎁스세팅리스트 갱신
		::SendMessage(m_ParenthWnd, WM_FRAMELIST_DOUBLE_CLICK, NULL, NULL);
	}
}

void CSceneEditDlgBar::OnLbnSelchangeListScene()
{
	UpdateData(true);
	m_iFocusSceneIndex = m_SceneList->GetCurSel();
}

//씬에디터 오른쪽 씬리스트 더블클릭
void CSceneEditDlgBar::OnLbnDblclkListScene()
{
	UpdateData(true);

	CObjMgr::GetInstance()->ClearTargetMap(SCENE_EDITOR_TARGET);

	m_BackgroundPathEdit->SetWindowText(L"");
	m_BlurPathEdit->SetWindowText(L"");

	CUIScene* FocusScene = CDBMgr::GetInstance()->GetUIScene(m_iFocusSceneIndex);
	FocusScene->Initialize();
	if(FocusScene != NULL)
	{
		//불러온 씬 파일 이름 저장
		CObjMgr::GetInstance()->SetSaveSceneFileName(FocusScene->GetSceneFileName());
		CUIScene*	CopyScene = new CUIScene(FocusScene);

		std::cout << "Scene 카피 후 배경패스 세팅 시작" << std::endl;

		CString szBGPath(CopyScene->GetBackgroundPath().c_str());
		m_strBackgroundPath = szBGPath;
		m_BackgroundPathEdit->SetWindowText(m_strBackgroundPath);
		if(m_strBackgroundPath != "")
			CObjMgr::GetInstance()->LoadBackgroundAsset(CopyScene->GetBackgroundPath().c_str(), m_ParenthWnd, m_ViewhWnd);

		std::cout << "Scene 카피 후 배경패스 세팅 완료" << std::endl;

		std::cout << "Scene 카피 후 블러 세팅 시작" << std::endl;
		CString szBlurPath(CopyScene->GetBlurImagePath().c_str());
		m_strBlurPath = szBlurPath;
		m_BlurPathEdit->SetWindowText(m_strBlurPath);
		std::cout << "Scene 카피 후 블러 세팅 완료" << std::endl;

		if(m_strBlurPath != "")
			CObjMgr::GetInstance()->LoadBlurImageAsset(CopyScene->GetBlurImagePath().c_str(), m_ParenthWnd, m_ViewhWnd);

		std::cout << "Scene Frame 오브젝트 매니저의 벡터에 삽입" << std::endl;
		CopyScene->SetParenthWnd(m_ParenthWnd);
		CopyScene->InsertFrameToObjMgr();
		CObjMgr::GetInstance()->InsertUIScene(CopyScene);
		std::cout << "Scene을 오브젝트 매니저 벡터에 삽입 완료." << std::endl;
		::SendMessage(m_ParenthWnd, WM_SCENELIST_DOUBLE_CLICK, NULL, NULL);
	}

	std::cout << "Scene복사 성공" << std::endl;
}

void CSceneEditDlgBar::Initialize()
{
	TCHAR strInitPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, strInitPath);
	// 패스 변경 : Release 
#if defined _DEBUG || defined RELEASE_DEV_PATH
	_tcscat_s(strInitPath, L"\\Asset");
#else
	_tcscat_s(strInitPath, ASSET_FOLDER_PATH);
#endif
	LoadAssetList(strInitPath);
	m_strAssetListPath = strInitPath;

	GetCurrentDirectory(MAX_PATH, strInitPath);
	// 패스 변경 : Release 
#if defined _DEBUG || defined RELEASE_DEV_PATH
	_tcscat_s(strInitPath, L"\\Frame");
#else
	_tcscat_s(strInitPath, FRAME_FOLDER_PATH);
#endif
	LoadUIFrameList(strInitPath);
	m_strFrameListPath = strInitPath;

	GetCurrentDirectory(MAX_PATH, strInitPath);
	// 패스 변경 : Release 
#if defined _DEBUG || defined RELEASE_DEV_PATH
	_tcscat_s(strInitPath, L"\\Scene");
#else
	_tcscat_s(strInitPath, SCENE_FOLDER_PATH);
#endif
	LoadSceneList(strInitPath);
	m_strSceneListPath = strInitPath;
}

void CSceneEditDlgBar::SetViewhWnd( HWND _hWnd )
{
	m_ViewhWnd = _hWnd;
}


void CSceneEditDlgBar::OnBnClickedButtonSceneAllclear()
{
	UpdateData(true);

	if(AfxMessageBox(TEXT("뷰의 에셋들을 모두 삭제하시겠습니까?"), MB_OKCANCEL) == IDOK)
	{
		CObjMgr::GetInstance()->ClearFrameMap();
		CObjMgr::GetInstance()->ClearTargetMap(SCENE_EDITOR_TARGET);
		m_ReviseFramePathEdit = (CEdit*)GetDlgItem(IDC_EDIT_LOAD_REVISE_FRAME);
		m_ReviseFramePathEdit->SetWindowText(L"");

		m_BackgroundPathEdit->SetWindowText(L"");
		m_BlurPathEdit->SetWindowText(L"");
		m_strBackgroundPath = "";
		m_strBlurPath = "";

		//CObjMgr::GetInstance()->
		//뎁스세팅 리스트 초기화
		::SendMessage(m_ParenthWnd, WM_SCENELIST_DOUBLE_CLICK, NULL, NULL);
	}
}


void CSceneEditDlgBar::OnEnChangeEditLoadReviseFrame()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogBar::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CSceneEditDlgBar::OnBnClickedButtonLoadReviseFrame()
{
	CString strPathName;
	std::string strRai = "rfi";
	CA2T wt(strRai.c_str());
	CFileDialog FileDlg(TRUE, wt, NULL);

	TCHAR strInitPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, strInitPath);
	// 패스 변경 : Release 
#if defined _DEBUG || defined RELEASE_DEV_PATH
	_tcscat_s(strInitPath, L"\\Frame");
#else
	_tcscat_s(strInitPath, FRAME_FOLDER_PATH);
#endif
	FileDlg.m_ofn.lpstrInitialDir = strInitPath;

	int iResult = FileDlg.DoModal();
	if(IDOK == iResult)
	{
		CObjMgr::GetInstance()->ClearFrameMap();
		CObjMgr::GetInstance()->ClearTargetMap(SCENE_EDITOR_TARGET);

		//뎁스세팅 리스트 초기화
		::SendMessage(m_ParenthWnd, WM_SCENELIST_DOUBLE_CLICK, NULL, NULL);
		CString	pszFramePath = FileDlg.GetPathName();
		std::ifstream	ifLoad(pszFramePath);
		std::string		strInput;

		CBasicAsset* newBasicAsset = NULL;
		POINT ptMinPos;
		ptMinPos.x = 0;
		ptMinPos.y = 0;

		if(ifLoad.good())
		{
			while(!ifLoad.eof())
			{
				std::getline(ifLoad, strInput, '\n');

				// 툴에서만 사용하는 프레임 위치값
				if(!strcmp(strInput.c_str(), "[REVISE_FRAME]"))
				{
					std::getline(ifLoad, strInput, ',');
					strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
					strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
					ptMinPos.x = atoi(strInput.c_str());

					std::getline(ifLoad, strInput, '\n');
					strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
					strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
					ptMinPos.y = atoi(strInput.c_str());
				}

				if(!strcmp(strInput.c_str(), "[START]"))
				{
					newBasicAsset = new CBasicAsset;

					if(newBasicAsset->Load(ifLoad) != true)
					{
						SAFE_DELETE(newBasicAsset);
					}
					else
					{
						//  [1/17/2019 Gabrielle]
						if(ptMinPos.x != 0 || ptMinPos.y != 0)
						{
							newBasicAsset->AddPosition(ptMinPos);
						}

						newBasicAsset->MakeOriginalFilePath();
						newBasicAsset->Run();
						newBasicAsset->SetParenthWnd(m_ParenthWnd);

						CObjMgr::GetInstance()->InsertBasicAsset(newBasicAsset);
					}
				}
			}
		}

		m_strReviseFramePath = pszFramePath;
		m_ReviseFramePathEdit = (CEdit*)GetDlgItem(IDC_EDIT_LOAD_REVISE_FRAME);
		m_ReviseFramePathEdit->SetWindowText(m_strReviseFramePath);
		ifLoad.close();
	}

	//뎁스세팅리스트 갱신
	::SendMessage(m_ParenthWnd, WM_ASSETLIST_DOUBLE_CLICK, NULL, NULL);
}