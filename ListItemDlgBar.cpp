// ListItemDlgBar.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "UITool.h"
#include "ListItemDlgBar.h"
#include "afxdialogex.h"
#include "ObjMgr.h"
#include "ListItemBase.h"


// CListItemDlgBar 대화 상자입니다.

IMPLEMENT_DYNAMIC(CListItemDlgBar, CDialogBar)

CListItemDlgBar::CListItemDlgBar(CWnd* pParent /*=NULL*/)
{
	m_AssetList = new CListBox;
	m_AssetAttributeCombo = new CComboBox;
	m_iComboboxIndex = NOTHING_IN_HERE;
	m_iSelectDepthListIndex = -1;
	m_vecAssetList.clear();
}

CListItemDlgBar::~CListItemDlgBar()
{
	SAFE_DELETE(m_AssetList);
	SAFE_DELETE(m_AssetAttributeCombo);

	for(int i = 0; i < m_vecAssetList.size(); i++)
	{
		SAFE_DELETE(m_vecAssetList[i]);
	}

	ClearVector();
}

void CListItemDlgBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	m_AssetList = (CListBox*)GetDlgItem(IDC_LIST_LISTITEM);
	m_AssetAttributeCombo = (CComboBox*)GetDlgItem(IDC_COMBO_LISTITEM_ATT);

	if(CObjMgr::GetInstance()->GetFocusAssetBase() == NULL)
	{
		m_ListItemBase = NULL;
	}
}


BEGIN_MESSAGE_MAP(CListItemDlgBar, CDialogBar)
	ON_WM_CREATE()
	ON_LBN_DBLCLK(IDC_LIST_LISTITEM, &CListItemDlgBar::OnLbnDblclkListListitem)
	ON_CBN_SELCHANGE(IDC_COMBO_LISTITEM_ATT, &CListItemDlgBar::OnCbnSelchangeComboListitemAtt)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_LISTITEM_DIRECTORY, &CListItemDlgBar::OnBnClickedButtonLoadListitemDirectory)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_LOAD_LISTITEM_DIRECTORY, &CListItemDlgBar::OnUpdateCmdUI)
	ON_LBN_SELCHANGE(IDC_LISTITEM_DEPTHSETTING_LIST, &CListItemDlgBar::OnLbnSelchangeListitemDepthsettingList)

	ON_UPDATE_COMMAND_UI(IDC_LISTITEM_BUTTON_DEPTH_TOP, &CListItemDlgBar::OnUpdateCmdUI)
	ON_UPDATE_COMMAND_UI(IDC_LISTITEM_BUTTON_DEPTH_BOTTOM, &CListItemDlgBar::OnUpdateCmdUI)
	ON_UPDATE_COMMAND_UI(IDC_LISTITEM_BUTTON_DEPTH_UP, &CListItemDlgBar::OnUpdateCmdUI)
	ON_UPDATE_COMMAND_UI(IDC_LISTITEM_BUTTON_DEPTH_DOWN, &CListItemDlgBar::OnUpdateCmdUI)
	ON_BN_CLICKED(IDC_LISTITEM_BUTTON_DEPTH_TOP, &CListItemDlgBar::OnBnClickedListitemButtonDepthTop)
	ON_BN_CLICKED(IDC_LISTITEM_BUTTON_DEPTH_BOTTOM, &CListItemDlgBar::OnBnClickedListitemButtonDepthBottom)
	ON_BN_CLICKED(IDC_LISTITEM_BUTTON_DEPTH_UP, &CListItemDlgBar::OnBnClickedListitemButtonDepthUp)
	ON_BN_CLICKED(IDC_LISTITEM_BUTTON_DEPTH_DOWN, &CListItemDlgBar::OnBnClickedListitemButtonDepthDown)
	ON_MESSAGE(WM_ASSET_BASE_FOCUSED, &CListItemDlgBar::OnAssetBaseFocused)
	ON_MESSAGE(WM_ASSETEDITOR_ASSETLIST_DOUBLE_CLICK, &CListItemDlgBar::OnAssetEditorDubleClick)
	ON_MESSAGE(WM_CLEAR_ALL, &CListItemDlgBar::OnAssetEditorAllClear)
	ON_MESSAGE(WM_LISTITEM_ASSET_DELETE, &CListItemDlgBar::OnDeleteListItem)
	
END_MESSAGE_MAP()

void CListItemDlgBar::OnUpdateCmdUI(CCmdUI* pCmdUI) // 다이얼로그바 버튼 활성화 위해 추가
{
	pCmdUI->Enable(TRUE);
}

void CListItemDlgBar::SetParenthWnd( HWND _hWnd )
{
	m_ParenthWnd = _hWnd;
}

void CListItemDlgBar::Initialize()
{
	UpdateData(true);

	m_iComboboxIndex = NOTHING_IN_HERE;

	int j = 0;
	for(int i = 0; i < ASSET_END; ++i)
	{
		if(i == ASSET_PICTURE || i == ASSET_BUTTON || i == ASSET_CHECKBOX
			|| i == ASSET_TEXTBOX || i == ASSET_PROGRESSBAR || i == ASSET_THIRDGRID)
		{
			CString cStrAtt(g_strAttribute[i].c_str());
			m_AssetAttributeCombo->AddString(cStrAtt);
			m_AssetType[j] = i;
			j++;
		}
	}

	m_AssetAttributeCombo->SetCurSel(0);
	RefreshAssetList();
}

int CALLBACK CListItemDlgBar::BrowseCallbackProc( HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData )
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

void CListItemDlgBar::BrowseFolder()
{
	//	에셋 로드 경로를 설정한다
	ITEMIDLIST*	pildBrowse;
	TCHAR		pszPathname[MAX_PATH];
	BROWSEINFO	bInfo;
	memset(&bInfo, 0, sizeof(bInfo));

	LPCTSTR lpszDefaultFolder = m_strAssetListInitPath;

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
	bInfo.lpfn				= BrowseCallbackProc; // NULL;
	bInfo.lParam			= (LPARAM)lpszDefaultFolder;
	pildBrowse				= ::SHBrowseForFolder(&bInfo);

	if(pildBrowse)
	{
		SHGetPathFromIDList(pildBrowse, pszPathname);
		CString strTemp(pszPathname);
		m_strAssetListInitPath = strTemp;
		RefreshAssetList();
	}

	IMalloc*		pMalloc = NULL;

	if (::SHGetMalloc(&pMalloc) != NOERROR)
	{
		return;
	}

	//메모리 해제를 한다. 
	pMalloc->Free(pildBrowse);
	pMalloc->Release();
}

void CListItemDlgBar::LoadAssetList()
{
	UpdateData(TRUE);

	//	로드 버튼이 눌리면 현재 생성되어있는 에셋들이 어떤 종류가 있는지 뜨게 하면 된다.
	ClearVector();

	//	현재 이 리스트에 노출되어야 하는 리스트를 비워준다
	for(int i = m_AssetList->GetCount(); i >= 0; i--)
	{
		m_AssetList->DeleteString(i);
	}

	//	리스트에 목록을 채워넣으면서 addString을 진행한다.
	//	기본으로 지정되어 있는 에셋 폴더를 들어가서 파일을 읽어온다.

	TCHAR szLoadingPath[MAX_PATH] = L"";
	CFileFind finder;
	int iIndex;
	bool bWorking;

	if(m_strAssetListInitPath != "")
	{
		_tcscpy_s(szLoadingPath, _countof(szLoadingPath), m_strAssetListInitPath.GetBuffer());
		_tcscat_s(szLoadingPath, L"\\*.rai");
		bWorking = finder.FindFile(szLoadingPath);
	}
	else
	{
		CString strAssetInit = ASSET_FOLDER_PATH;
		bWorking = finder.FindFile(strAssetInit);
	}

	while(bWorking)
	{
		CBasicAsset CheckAsset;
		bWorking = finder.FindNextFile();
		if(!finder.IsDots() && !finder.IsDirectory())
		{
			iIndex = m_AssetList->GetCount();
			CheckAsset = LoadBasicAsset(finder.GetFilePath(), finder.GetFileName());

			if(CheckAsset.GetAssetType() == m_AssetAttributeCombo->GetCurSel())
			{
				m_AssetList->AddString(finder.GetFileName());	
			}
			else
			{
				//CDBMgr::GetInstance()->DeleteBasicAsset(iIndex,ASSET_EDITOR_TARGET);
				DeleteVectorAsset(iIndex);
			}
		}
	}
}

CBasicAsset CListItemDlgBar::LoadBasicAsset(LPCTSTR pszAssetPath, LPCTSTR pszAssetName)
{
	//	파일을 찾아서 그 파일의 경로를 인자로 주고 있다.
	//	에셋의 경로, 이름, 저장할 타겟, 그리고 리스트박스의 인덱스까지.
	std::ifstream	ifLoad(pszAssetPath);
	std::string		strInput;

	CBasicAsset*	newBasicAsset = new CBasicAsset;

	if(ifLoad.good())
	{
		while(!ifLoad.eof())
		{
			CBaseInterface* newAsset = NULL;
			//	복수의 입력을 위한 InsertAsset과 SetFilePath의 배치

			//	로드 에셋 데이터
			//	오브젝트 매니저에서는 잘 돌아가는 코드가 여기서 힙 손상이 발생하는 이유?
			std::getline(ifLoad, strInput, '\n');

			if(!strcmp(strInput.c_str(), "[END]"))
			{
				newAsset = NULL;
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
				newBasicAsset->InsertAsset(newAsset);
				newAsset = NULL;
			}
		}

		CString _csPath = pszAssetPath;
		CT2CA pszConvertedAnsiString (_csPath);
		std::string _stdPath(pszConvertedAnsiString);

		CString _csName = pszAssetName;
		CT2CA pszConvertedName (_csName);
		std::string _stdName(pszConvertedName);

		newBasicAsset->SetFilePath(_stdPath);
		newBasicAsset->SetFileName(_stdName);
		newBasicAsset->Initialize();

		if(newBasicAsset->GetAssetType() != ASSET_RADIOBUTTON || newBasicAsset->GetAssetType() != ASSET_TABBUTTON)
		{
			//	그룹버튼은 이부분 알아서 하는거로.
			//	현재는 밑의 함수 내부에서 라디오 버튼 따로 굴려주고있음.
			newBasicAsset->AssetBasePathSetting();
		}

		if(newBasicAsset != NULL)
		{
			m_vecAssetList.push_back(newBasicAsset);
		}
		else
		{
			SAFE_DELETE(newBasicAsset);
		}

		ifLoad.close();
		CBasicAsset cResultAsset = *newBasicAsset;
		newBasicAsset = NULL;
		return cResultAsset;
	}

	SAFE_DELETE(newBasicAsset);
	ifLoad.close();
	return NULL;
}

void CListItemDlgBar::ClearVector()
{
	int iVecCount = m_vecAssetList.size();

	if(iVecCount != 0)
	{
		for(int i = 0; i < iVecCount;i++)
		{
			SAFE_DELETE(m_vecAssetList[i]);
		}
		//m_iFocusAssetBase = NOTHING_IN_HERE;
		m_vecAssetList.clear();
	}
}

void CListItemDlgBar::DeleteVectorAsset( int _iIndex )
{
	VEC_ASSETLIST_ITER iter_begin = m_vecAssetList.begin();
	VEC_ASSETLIST_ITER iter_end = m_vecAssetList.end();

	for(iter_begin; iter_begin != iter_end; iter_begin++)
	{
		if(((CBasicAsset*)*iter_begin) == m_vecAssetList[_iIndex])
		{
			SAFE_DELETE(m_vecAssetList[_iIndex]);
			m_vecAssetList.erase(iter_begin);
			return;
		}
	}
}

// CListItemDlgBar 메시지 처리기입니다.


int CListItemDlgBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	TCHAR strInitPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, strInitPath);
	// 패스 변경 : Release 
#if defined _DEBUG || defined RELEASE_DEV_PATH
	_tcscat_s(strInitPath, L"\\Asset");
#else
		_tcscat_s(strInitPath, ASSET_FOLDER_PATH);
#endif
	m_strAssetListInitPath = strInitPath;
	return 0;
}

void CListItemDlgBar::RefreshAssetList()
{
	UpdateData(true);

	ClearVector();
	//	현재 이 리스트에 노출되어야 하는 리스트를 비워준다

	for(int i = m_AssetList->GetCount(); i >= 0; i--)
	{
		m_AssetList->DeleteString(i);
	}

	//	리스트에 목록을 채워넣으면서 addString을 진행한다.
	//	기본으로 지정되어 있는 에셋 폴더를 들어가서 파일을 읽어온다.

	TCHAR szLoadingPath[MAX_PATH] = L"";
	CFileFind finder;
	int iIndex;
	bool bWorking;

	if(m_strAssetListInitPath != "")
	{
		_tcscpy_s(szLoadingPath, _countof(szLoadingPath), m_strAssetListInitPath.GetBuffer());
		_tcscat_s(szLoadingPath, L"\\*.rai");
		bWorking = finder.FindFile(szLoadingPath);
	}
	else
	{
		CString strAssetInit = ASSET_FOLDER_PATH;
		bWorking = finder.FindFile(strAssetInit);
	}

	while(bWorking)
	{
		CBasicAsset CheckAsset;
		bWorking = finder.FindNextFile();
		if(!finder.IsDots() && !finder.IsDirectory())
		{
			iIndex = m_AssetList->GetCount();
			CheckAsset = LoadBasicAsset(finder.GetFilePath(), finder.GetFileName());
			int iCurSel = m_AssetAttributeCombo->GetCurSel();
			if(CheckAsset.GetAssetType() == m_AssetType[iCurSel])
			{
				m_AssetList->AddString(finder.GetFileName());	
			}
			else
			{
				//CDBMgr::GetInstance()->DeleteBasicAsset(iIndex,ASSET_EDITOR_TARGET);
				DeleteVectorAsset(iIndex);
			}
		}
	}
}


void CListItemDlgBar::OnLbnDblclkListListitem()
{
	UpdateData(TRUE);

	CreateAsset();

	int iIndex = m_AssetList->GetCurSel();
	CBasicAsset* cSelBasicAsset = m_vecAssetList[iIndex];	
	if(cSelBasicAsset != NULL)
	{
		//	에셋을 할당하고 해당 에셋의 내부를 채워준다.

		CBaseInterface* CopyAsset = cSelBasicAsset->GetAssetBase();
		CBaseInterface* newBase = MakeAdaptableBase(CopyAsset->GetAssetType());
		newBase->SetAssetType(CopyAsset->GetAssetType());

		if(newBase != NULL)
		{
			newBase->Copy(CopyAsset);
			newBase->SetParenthWnd(m_ParenthWnd);
			m_ListItemBase->InsertListItem(newBase);
			//	에디터의 속성창에 있는 요소들을 갱신시켜준다.
			::SendMessageA(m_ParenthWnd, WM_REFRESH_DLGBAR_INFO, (WPARAM)newBase, NULL);

			RefreshDepthSettingList(-1);
		}
	}
}


void CListItemDlgBar::OnCbnSelchangeComboListitemAtt()
{
	//	에셋 속성을 바꿨을 경우
	UpdateData(TRUE);
	m_iComboboxIndex = m_AssetAttributeCombo->GetCurSel();

	RefreshAssetList();
}

void CListItemDlgBar::CreateAsset()
{
	if(m_ListItemBase == NULL)
	{
		m_ListItemBase = new CListItemBase;
		m_ListItemBase->SetParenthWnd(m_ParenthWnd);
		CObjMgr::GetInstance()->InsertAssetBase(m_ListItemBase);
	}
}

//뎁스 세팅 리스트 갱신
void CListItemDlgBar::RefreshDepthSettingList(int nIndex, bool bAllClear)
{
	m_pDepthSettingListBox = (CListBox*)GetDlgItem(IDC_LISTITEM_DEPTHSETTING_LIST);	

	//리스트에 있는 항목을 비워준다.
	if(m_pDepthSettingListBox->GetCount() != 0)
	{
		for(int i = m_pDepthSettingListBox->GetCount(); i >= 0; i--)
		{
			m_pDepthSettingListBox->DeleteString(i);
		}
	}	

	//리스트 박스에 정보 보여주기
	if(bAllClear == FALSE && m_ListItemBase != NULL)
	{
		std::vector<CBaseInterface*> listItemList = m_ListItemBase->GetListItemList();		
		if(listItemList.size() != 0)
		{
			char szIndex[50] = {0,};		
			for(int i=0; i<listItemList.size(); ++i)
			{
				CBaseInterface* pAsset = listItemList[i];			
				itoa(i, szIndex, 10);
				CA2T wt(szIndex);
				m_pDepthSettingListBox->AddString(wt + L"Asset");
			}
		}
	}	

	SelectDepthListIndex(nIndex);
}

//뎁스 옮기는 버튼 눌렀을 경우 리스트 포커스가 날라가서 강제로 포커스해줌.
void CListItemDlgBar::SelectDepthListIndex(int nSelectIndex)
{
	if(nSelectIndex != -1)
	{
		m_pDepthSettingListBox = (CListBox*)GetDlgItem(IDC_LISTITEM_DEPTHSETTING_LIST);
		m_iSelectDepthListIndex = nSelectIndex;
		m_pDepthSettingListBox->SetCurSel(m_iSelectDepthListIndex);
	}
}

void CListItemDlgBar::OnBnClickedButtonLoadListitemDirectory()
{
	BrowseFolder();
}

//리스트 아이템 리스트 클릭 이벤트
void CListItemDlgBar::OnLbnSelchangeListitemDepthsettingList()
{
	m_pDepthSettingListBox = (CListBox*)GetDlgItem(IDC_LISTITEM_DEPTHSETTING_LIST);
	m_iSelectDepthListIndex = m_pDepthSettingListBox->GetCurSel();
	m_ListItemBase->SetFocusItemIndex(m_iSelectDepthListIndex);
	::SendMessage(m_ParenthWnd, WM_ASSET_BASE_FOCUSED, NULL ,NULL);
}

//뎁스세팅(최상위)
void CListItemDlgBar::OnBnClickedListitemButtonDepthTop()
{
	m_ListItemBase->ChangeListitemDepth(m_iSelectDepthListIndex, 0);
	RefreshDepthSettingList(0);
}

//뎁스세팅(최하위)
void CListItemDlgBar::OnBnClickedListitemButtonDepthBottom()
{
	m_pDepthSettingListBox = (CListBox*)GetDlgItem(IDC_LISTITEM_DEPTHSETTING_LIST);
	m_ListItemBase->ChangeListitemDepth(m_iSelectDepthListIndex, m_pDepthSettingListBox->GetCount()-1);
	RefreshDepthSettingList(m_pDepthSettingListBox->GetCount()-1);
}

//뎁스세팅(한칸위)
void CListItemDlgBar::OnBnClickedListitemButtonDepthUp()
{
	int nChangePos = m_iSelectDepthListIndex-1;
	if(nChangePos <= 0)
		nChangePos = 0;

	m_ListItemBase->ChangeListitemDepth(m_iSelectDepthListIndex, nChangePos);
	RefreshDepthSettingList(nChangePos);
}

//뎁스세팅(한칸아래)
void CListItemDlgBar::OnBnClickedListitemButtonDepthDown()
{
	m_pDepthSettingListBox = (CListBox*)GetDlgItem(IDC_LISTITEM_DEPTHSETTING_LIST);
	int nChangePos = m_iSelectDepthListIndex+1;
	if(nChangePos >= m_pDepthSettingListBox->GetCount()-1)
		nChangePos = m_pDepthSettingListBox->GetCount()-1;

	m_ListItemBase->ChangeListitemDepth(m_iSelectDepthListIndex, nChangePos);
	RefreshDepthSettingList(nChangePos);
}

//에셋 에디터에서 포커스 됐을때 뎁스세팅에 알려주기 위한 이벤트
afx_msg LRESULT CListItemDlgBar::OnAssetBaseFocused(WPARAM wParam, LPARAM lParam)
{
	SelectDepthListIndex(m_ListItemBase->GetFocusItemIndex());

	return 0;
}

//에셋 에디터 다이얼로그 리스트에서 더블클릭 했을때 뎁스세팅에 알려주기 위한 이벤트
afx_msg LRESULT CListItemDlgBar::OnAssetEditorDubleClick(WPARAM wParam, LPARAM lParam)
{	
	if(m_ListItemBase == NULL)
	{
		m_ListItemBase = (CListItemBase*)wParam;
		RefreshDepthSettingList(-1);
	}

	return 0;
}

//에디터 세팅에서 화면 클리어 버튼을 클릭시 뎁스세팅에 알려주기 위한 이벤트
afx_msg LRESULT CListItemDlgBar::OnAssetEditorAllClear(WPARAM wParam, LPARAM lParam)
{
	RefreshDepthSettingList(-1, TRUE);
	return 0;
}

//에디터 화면에서 에셋을 삭제했을 때 뎁스세팅에 알려주기 위한 이벤트
afx_msg LRESULT CListItemDlgBar::OnDeleteListItem(WPARAM wParam, LPARAM lParam)
{
	RefreshDepthSettingList(-1);
	return 0;
}
