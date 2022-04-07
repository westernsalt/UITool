// SceneEditDlgBar.cpp : ���� �����Դϴ�.
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

void CSceneEditDlgBar::OnUpdateCmdUI(CCmdUI* pCmdUI) // ���̾�α׹� ��ư Ȱ��ȭ ���� �߰�
{
	pCmdUI->Enable(TRUE);
}

// CSceneEditDlgBar �޽��� ó�����Դϴ�.

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
	//	�ҷ����⸦ ���� ���� ���⼭ â�� ����ְ� �۾��� ������Ѵ�
	CString strPathName;
	std::string strRai = "rai";
	CA2T wt(strRai.c_str());
	CFileDialog FileDlg(TRUE, wt, NULL);

	TCHAR strInitPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, strInitPath);
	// �н� ���� : Release 
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

		//	��׶��� ������ �ε��ϴµ�, ����ε� ������ ��쿡�� ���ͼ� �����Ѵ�.
		if(CObjMgr::GetInstance()->LoadBackgroundAsset(_Path, m_ParenthWnd, m_ViewhWnd))
		{
			m_strBackgroundPath = strPathName;
			m_BackgroundPathEdit->SetWindowText(m_strBackgroundPath);
		}
	}
}

void CSceneEditDlgBar::OnBnClickedButtonLoadBlurAsset()
{
	//	�ҷ����⸦ ���� ���� ���⼭ â�� ����ְ� �۾��� ������Ѵ�
	CString strPathName;
	std::string strRai = "rai";
	CA2T wt(strRai.c_str());
	CFileDialog FileDlg(TRUE, wt, NULL);

	TCHAR strInitPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, strInitPath);
	// �н� ���� : Release 
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

		//	�� ������ �ε��ϴµ�, ����ε� ������ ��쿡�� ���ͼ� �����Ѵ�.
		if(CObjMgr::GetInstance()->LoadBlurImageAsset(_Path, m_ParenthWnd, m_ViewhWnd))
		{
			m_strBlurPath = strPathName;
			m_BlurPathEdit->SetWindowText(m_strBlurPath);
		}
	}
}


void CSceneEditDlgBar::BrowseFolder(CString& _strPath, eListType _type)
{
	//	���� �ε� ��θ� �����Ѵ�
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


	//	��Ʈ ��� ���� ��ƾ
	//**********************************************************************
	TCHAR sPath[MAX_PATH] = {0};
	::GetCurrentDirectory(MAX_PATH, sPath);
	// �н� ���� : Release 
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

	//	����ü ����
	bInfo.hwndOwner			= GetSafeHwnd();
	bInfo.pidlRoot			= pStartDir;
	bInfo.pszDisplayName	= pszPathname;
	bInfo.lpszTitle			= _T("���丮�� �����ϼ���.");
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

	//�޸� ������ �Ѵ�. 
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

	//���� ������
	::SendMessage(m_ParenthWnd, WM_ASSET_PREVIEW, m_iFocusAssetIndex, NULL);
}


void CSceneEditDlgBar::OnLbnDblclkListAsset()
{
	UpdateData(true);
	//	�����ͺ��̽��Ŵ����� ���� �����͸� ���������� ������ �ִµ� ���������� �Ҹ��ϸ鼭 
	//	���������Ϳ� �����ϰ� �ִ� ������ ����������.
	CBasicAsset* FocusAsset = CDBMgr::GetInstance()->GetBasicAsset(m_iFocusAssetIndex, SCENE_EDITOR_TARGET);
	CBasicAsset* CopyAsset = new CBasicAsset(FocusAsset);
	CopyAsset->Initialize();
	CopyAsset->SetHitBox();
	CopyAsset->SetParenthWnd(m_ParenthWnd);
	CObjMgr::GetInstance()->InsertBasicAsset(CopyAsset);
	//�������ø���Ʈ ����
	::SendMessage(m_ParenthWnd, WM_ASSETLIST_DOUBLE_CLICK, NULL, NULL);
}


void CSceneEditDlgBar::OnLbnSelchangeListFrame()
{
	UpdateData(true);
	m_iFocusFrameIndex = m_UIFrameList->GetCurSel();

	//������ ������
	::SendMessage(m_ParenthWnd, WM_FRAME_PREVIEW, m_iFocusFrameIndex, NULL);
}

//�������� ������ �����Ӹ���Ʈ ����Ŭ��
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

		//�ҷ��� ������ ���� �̸� ����
		CObjMgr::GetInstance()->SetSaveFrameFileName(CopyFrame->GetFrameFileName());

		//�������ø���Ʈ ����
		::SendMessage(m_ParenthWnd, WM_FRAMELIST_DOUBLE_CLICK, NULL, NULL);
	}
}

void CSceneEditDlgBar::OnLbnSelchangeListScene()
{
	UpdateData(true);
	m_iFocusSceneIndex = m_SceneList->GetCurSel();
}

//�������� ������ ������Ʈ ����Ŭ��
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
		//�ҷ��� �� ���� �̸� ����
		CObjMgr::GetInstance()->SetSaveSceneFileName(FocusScene->GetSceneFileName());
		CUIScene*	CopyScene = new CUIScene(FocusScene);

		std::cout << "Scene ī�� �� ����н� ���� ����" << std::endl;

		CString szBGPath(CopyScene->GetBackgroundPath().c_str());
		m_strBackgroundPath = szBGPath;
		m_BackgroundPathEdit->SetWindowText(m_strBackgroundPath);
		if(m_strBackgroundPath != "")
			CObjMgr::GetInstance()->LoadBackgroundAsset(CopyScene->GetBackgroundPath().c_str(), m_ParenthWnd, m_ViewhWnd);

		std::cout << "Scene ī�� �� ����н� ���� �Ϸ�" << std::endl;

		std::cout << "Scene ī�� �� �� ���� ����" << std::endl;
		CString szBlurPath(CopyScene->GetBlurImagePath().c_str());
		m_strBlurPath = szBlurPath;
		m_BlurPathEdit->SetWindowText(m_strBlurPath);
		std::cout << "Scene ī�� �� �� ���� �Ϸ�" << std::endl;

		if(m_strBlurPath != "")
			CObjMgr::GetInstance()->LoadBlurImageAsset(CopyScene->GetBlurImagePath().c_str(), m_ParenthWnd, m_ViewhWnd);

		std::cout << "Scene Frame ������Ʈ �Ŵ����� ���Ϳ� ����" << std::endl;
		CopyScene->SetParenthWnd(m_ParenthWnd);
		CopyScene->InsertFrameToObjMgr();
		CObjMgr::GetInstance()->InsertUIScene(CopyScene);
		std::cout << "Scene�� ������Ʈ �Ŵ��� ���Ϳ� ���� �Ϸ�." << std::endl;
		::SendMessage(m_ParenthWnd, WM_SCENELIST_DOUBLE_CLICK, NULL, NULL);
	}

	std::cout << "Scene���� ����" << std::endl;
}

void CSceneEditDlgBar::Initialize()
{
	TCHAR strInitPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, strInitPath);
	// �н� ���� : Release 
#if defined _DEBUG || defined RELEASE_DEV_PATH
	_tcscat_s(strInitPath, L"\\Asset");
#else
	_tcscat_s(strInitPath, ASSET_FOLDER_PATH);
#endif
	LoadAssetList(strInitPath);
	m_strAssetListPath = strInitPath;

	GetCurrentDirectory(MAX_PATH, strInitPath);
	// �н� ���� : Release 
#if defined _DEBUG || defined RELEASE_DEV_PATH
	_tcscat_s(strInitPath, L"\\Frame");
#else
	_tcscat_s(strInitPath, FRAME_FOLDER_PATH);
#endif
	LoadUIFrameList(strInitPath);
	m_strFrameListPath = strInitPath;

	GetCurrentDirectory(MAX_PATH, strInitPath);
	// �н� ���� : Release 
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

	if(AfxMessageBox(TEXT("���� ���µ��� ��� �����Ͻðڽ��ϱ�?"), MB_OKCANCEL) == IDOK)
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
		//�������� ����Ʈ �ʱ�ȭ
		::SendMessage(m_ParenthWnd, WM_SCENELIST_DOUBLE_CLICK, NULL, NULL);
	}
}


void CSceneEditDlgBar::OnEnChangeEditLoadReviseFrame()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogBar::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CSceneEditDlgBar::OnBnClickedButtonLoadReviseFrame()
{
	CString strPathName;
	std::string strRai = "rfi";
	CA2T wt(strRai.c_str());
	CFileDialog FileDlg(TRUE, wt, NULL);

	TCHAR strInitPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, strInitPath);
	// �н� ���� : Release 
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

		//�������� ����Ʈ �ʱ�ȭ
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

				// �������� ����ϴ� ������ ��ġ��
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

	//�������ø���Ʈ ����
	::SendMessage(m_ParenthWnd, WM_ASSETLIST_DOUBLE_CLICK, NULL, NULL);
}