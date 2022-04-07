// ListItemDlgBar.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "UITool.h"
#include "ListItemDlgBar.h"
#include "afxdialogex.h"
#include "ObjMgr.h"
#include "ListItemBase.h"


// CListItemDlgBar ��ȭ �����Դϴ�.

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

void CListItemDlgBar::OnUpdateCmdUI(CCmdUI* pCmdUI) // ���̾�α׹� ��ư Ȱ��ȭ ���� �߰�
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
	//	���� �ε� ��θ� �����Ѵ�
	ITEMIDLIST*	pildBrowse;
	TCHAR		pszPathname[MAX_PATH];
	BROWSEINFO	bInfo;
	memset(&bInfo, 0, sizeof(bInfo));

	LPCTSTR lpszDefaultFolder = m_strAssetListInitPath;

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

	//�޸� ������ �Ѵ�. 
	pMalloc->Free(pildBrowse);
	pMalloc->Release();
}

void CListItemDlgBar::LoadAssetList()
{
	UpdateData(TRUE);

	//	�ε� ��ư�� ������ ���� �����Ǿ��ִ� ���µ��� � ������ �ִ��� �߰� �ϸ� �ȴ�.
	ClearVector();

	//	���� �� ����Ʈ�� ����Ǿ�� �ϴ� ����Ʈ�� ����ش�
	for(int i = m_AssetList->GetCount(); i >= 0; i--)
	{
		m_AssetList->DeleteString(i);
	}

	//	����Ʈ�� ����� ä�������鼭 addString�� �����Ѵ�.
	//	�⺻���� �����Ǿ� �ִ� ���� ������ ���� ������ �о�´�.

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
	//	������ ã�Ƽ� �� ������ ��θ� ���ڷ� �ְ� �ִ�.
	//	������ ���, �̸�, ������ Ÿ��, �׸��� ����Ʈ�ڽ��� �ε�������.
	std::ifstream	ifLoad(pszAssetPath);
	std::string		strInput;

	CBasicAsset*	newBasicAsset = new CBasicAsset;

	if(ifLoad.good())
	{
		while(!ifLoad.eof())
		{
			CBaseInterface* newAsset = NULL;
			//	������ �Է��� ���� InsertAsset�� SetFilePath�� ��ġ

			//	�ε� ���� ������
			//	������Ʈ �Ŵ��������� �� ���ư��� �ڵ尡 ���⼭ �� �ջ��� �߻��ϴ� ����?
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
			//	�׷��ư�� �̺κ� �˾Ƽ� �ϴ°ŷ�.
			//	����� ���� �Լ� ���ο��� ���� ��ư ���� �����ְ�����.
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

// CListItemDlgBar �޽��� ó�����Դϴ�.


int CListItemDlgBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.
	TCHAR strInitPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, strInitPath);
	// �н� ���� : Release 
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
	//	���� �� ����Ʈ�� ����Ǿ�� �ϴ� ����Ʈ�� ����ش�

	for(int i = m_AssetList->GetCount(); i >= 0; i--)
	{
		m_AssetList->DeleteString(i);
	}

	//	����Ʈ�� ����� ä�������鼭 addString�� �����Ѵ�.
	//	�⺻���� �����Ǿ� �ִ� ���� ������ ���� ������ �о�´�.

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
		//	������ �Ҵ��ϰ� �ش� ������ ���θ� ä���ش�.

		CBaseInterface* CopyAsset = cSelBasicAsset->GetAssetBase();
		CBaseInterface* newBase = MakeAdaptableBase(CopyAsset->GetAssetType());
		newBase->SetAssetType(CopyAsset->GetAssetType());

		if(newBase != NULL)
		{
			newBase->Copy(CopyAsset);
			newBase->SetParenthWnd(m_ParenthWnd);
			m_ListItemBase->InsertListItem(newBase);
			//	�������� �Ӽ�â�� �ִ� ��ҵ��� ���Ž����ش�.
			::SendMessageA(m_ParenthWnd, WM_REFRESH_DLGBAR_INFO, (WPARAM)newBase, NULL);

			RefreshDepthSettingList(-1);
		}
	}
}


void CListItemDlgBar::OnCbnSelchangeComboListitemAtt()
{
	//	���� �Ӽ��� �ٲ��� ���
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

//���� ���� ����Ʈ ����
void CListItemDlgBar::RefreshDepthSettingList(int nIndex, bool bAllClear)
{
	m_pDepthSettingListBox = (CListBox*)GetDlgItem(IDC_LISTITEM_DEPTHSETTING_LIST);	

	//����Ʈ�� �ִ� �׸��� ����ش�.
	if(m_pDepthSettingListBox->GetCount() != 0)
	{
		for(int i = m_pDepthSettingListBox->GetCount(); i >= 0; i--)
		{
			m_pDepthSettingListBox->DeleteString(i);
		}
	}	

	//����Ʈ �ڽ��� ���� �����ֱ�
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

//���� �ű�� ��ư ������ ��� ����Ʈ ��Ŀ���� ���󰡼� ������ ��Ŀ������.
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

//����Ʈ ������ ����Ʈ Ŭ�� �̺�Ʈ
void CListItemDlgBar::OnLbnSelchangeListitemDepthsettingList()
{
	m_pDepthSettingListBox = (CListBox*)GetDlgItem(IDC_LISTITEM_DEPTHSETTING_LIST);
	m_iSelectDepthListIndex = m_pDepthSettingListBox->GetCurSel();
	m_ListItemBase->SetFocusItemIndex(m_iSelectDepthListIndex);
	::SendMessage(m_ParenthWnd, WM_ASSET_BASE_FOCUSED, NULL ,NULL);
}

//��������(�ֻ���)
void CListItemDlgBar::OnBnClickedListitemButtonDepthTop()
{
	m_ListItemBase->ChangeListitemDepth(m_iSelectDepthListIndex, 0);
	RefreshDepthSettingList(0);
}

//��������(������)
void CListItemDlgBar::OnBnClickedListitemButtonDepthBottom()
{
	m_pDepthSettingListBox = (CListBox*)GetDlgItem(IDC_LISTITEM_DEPTHSETTING_LIST);
	m_ListItemBase->ChangeListitemDepth(m_iSelectDepthListIndex, m_pDepthSettingListBox->GetCount()-1);
	RefreshDepthSettingList(m_pDepthSettingListBox->GetCount()-1);
}

//��������(��ĭ��)
void CListItemDlgBar::OnBnClickedListitemButtonDepthUp()
{
	int nChangePos = m_iSelectDepthListIndex-1;
	if(nChangePos <= 0)
		nChangePos = 0;

	m_ListItemBase->ChangeListitemDepth(m_iSelectDepthListIndex, nChangePos);
	RefreshDepthSettingList(nChangePos);
}

//��������(��ĭ�Ʒ�)
void CListItemDlgBar::OnBnClickedListitemButtonDepthDown()
{
	m_pDepthSettingListBox = (CListBox*)GetDlgItem(IDC_LISTITEM_DEPTHSETTING_LIST);
	int nChangePos = m_iSelectDepthListIndex+1;
	if(nChangePos >= m_pDepthSettingListBox->GetCount()-1)
		nChangePos = m_pDepthSettingListBox->GetCount()-1;

	m_ListItemBase->ChangeListitemDepth(m_iSelectDepthListIndex, nChangePos);
	RefreshDepthSettingList(nChangePos);
}

//���� �����Ϳ��� ��Ŀ�� ������ �������ÿ� �˷��ֱ� ���� �̺�Ʈ
afx_msg LRESULT CListItemDlgBar::OnAssetBaseFocused(WPARAM wParam, LPARAM lParam)
{
	SelectDepthListIndex(m_ListItemBase->GetFocusItemIndex());

	return 0;
}

//���� ������ ���̾�α� ����Ʈ���� ����Ŭ�� ������ �������ÿ� �˷��ֱ� ���� �̺�Ʈ
afx_msg LRESULT CListItemDlgBar::OnAssetEditorDubleClick(WPARAM wParam, LPARAM lParam)
{	
	if(m_ListItemBase == NULL)
	{
		m_ListItemBase = (CListItemBase*)wParam;
		RefreshDepthSettingList(-1);
	}

	return 0;
}

//������ ���ÿ��� ȭ�� Ŭ���� ��ư�� Ŭ���� �������ÿ� �˷��ֱ� ���� �̺�Ʈ
afx_msg LRESULT CListItemDlgBar::OnAssetEditorAllClear(WPARAM wParam, LPARAM lParam)
{
	RefreshDepthSettingList(-1, TRUE);
	return 0;
}

//������ ȭ�鿡�� ������ �������� �� �������ÿ� �˷��ֱ� ���� �̺�Ʈ
afx_msg LRESULT CListItemDlgBar::OnDeleteListItem(WPARAM wParam, LPARAM lParam)
{
	RefreshDepthSettingList(-1);
	return 0;
}
