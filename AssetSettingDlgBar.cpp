// AssetSettingDlgBar.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "UITool.h"
#include "AssetSettingDlgBar.h"

#include "AssetBase.h"
#include "BasicAsset.h"

#include "DBMgr.h"
#include "ObjMgr.h"

// CAssetSettingDlgBar
#define ASSET_COUNT 10

IMPLEMENT_DYNAMIC(CAssetSettingDlgBar, CDialogBar)

CAssetSettingDlgBar::CAssetSettingDlgBar()
{
	m_iComboBoxIndex = -1;

	m_bEditFocused = false;

	m_strPosX = "0";
	m_strPosY = "0";
	m_strPosZ = "1";
}

CAssetSettingDlgBar::~CAssetSettingDlgBar()
{
	DestroyWindow();
}


BEGIN_MESSAGE_MAP(CAssetSettingDlgBar, CDialogBar)
	ON_WM_CREATE()
	ON_CBN_SELCHANGE(IDC_COMBO_ASSET_ATT, &CAssetSettingDlgBar::OnCbnSelchangeComboAssetAtt)
	ON_CBN_SELCHANGE(IDC_COMBO_ASSET_TYPE, &CAssetSettingDlgBar::OnCbnSelchangeComboAssetType)

	ON_UPDATE_COMMAND_UI(IDC_BUTTON_ASSETLIST_LOAD, &CAssetSettingDlgBar::OnUpdateCmdUI)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_ASSETVIEW_CLEAR, &CAssetSettingDlgBar::OnUpdateCmdUI)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_PICTUREWND_CREATE, &CAssetSettingDlgBar::OnUpdateCmdUI)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_SAVE_EDITOR_ASSET, &CAssetSettingDlgBar::OnUpdateCmdUI)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_LOAD_EDITOR_ASSET, &CAssetSettingDlgBar::OnUpdateCmdUI)
	ON_BN_CLICKED(IDC_BUTTON_ASSETLIST_LOAD, &CAssetSettingDlgBar::OnBnClickedButtonAssetlistLoad)
	ON_BN_CLICKED(IDC_BUTTON_ASSETVIEW_CLEAR, &CAssetSettingDlgBar::OnBnClickedButtonAssetviewClear)
	ON_BN_CLICKED(IDC_BUTTON_PICTUREWND_CREATE, &CAssetSettingDlgBar::OnBnClickedButtonPicturewndCreate)
	ON_LBN_DBLCLK(IDC_ASSET_LIST_EDIT, &CAssetSettingDlgBar::OnLbnDblclkAssetListEdit)

	ON_EN_CHANGE(IDC_EDIT_TEXPOS_X, &CAssetSettingDlgBar::OnEnChangeEditTexposX)
	ON_EN_CHANGE(IDC_EDIT_TEXPOS_Y, &CAssetSettingDlgBar::OnEnChangeEditTexposY)
	ON_EN_CHANGE(IDC_EDIT_TEXPOS_Z, &CAssetSettingDlgBar::OnEnChangeEditTexposZ)
	ON_EN_SETFOCUS(IDC_EDIT_TEXPOS_X, &CAssetSettingDlgBar::OnEnSetfocusEditTexposX)
	ON_EN_SETFOCUS(IDC_EDIT_TEXPOS_Y, &CAssetSettingDlgBar::OnEnSetfocusEditTexposY)
	ON_EN_SETFOCUS(IDC_EDIT_TEXPOS_Z, &CAssetSettingDlgBar::OnEnSetfocusEditTexposZ)
	ON_MESSAGE(WM_ASSET_BASE_FOCUSED, &CAssetSettingDlgBar::OnAssetBaseFocused)

	ON_BN_CLICKED(IDC_BUTTON_SAVE_EDITOR_ASSET, &CAssetSettingDlgBar::OnBnClickedButtonSaveEditorAsset)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_EDITOR_ASSET, &CAssetSettingDlgBar::OnBnClickedButtonLoadEditorAsset)
END_MESSAGE_MAP()


// CAssetSettingDlgBar �޽��� ó�����Դϴ�.

int CAssetSettingDlgBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_AssetListBox = (CListBox*)GetDlgItem(IDC_ASSET_LIST_EDIT);
	m_TypeComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ASSET_TYPE);

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

void CAssetSettingDlgBar::InitializeRrc()
{
	m_AttComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ASSET_ATT);
	m_TypeComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ASSET_TYPE);
	//	���� �����Ϸ��� �����ߴµ�
	//	���ҽ� ����â ���� ������ �ø��� ȭ��ǥ ������ ������ �� ���ȴ��� Ŀ�� �Ѥ�

	for(int i=0; i < ASSET_END; ++i)
	{
		CString cStrAtt(g_strAttribute[i].c_str());
		m_AttComboBox->AddString(cStrAtt);
		m_TypeComboBox->AddString(cStrAtt);
	}

	m_AttComboBox->SetCurSel(0);
	m_TypeComboBox->SetCurSel(0);

	RefreshAssetList();
}

std::string CAssetSettingDlgBar::GetSelAssetType()
{
	m_AttComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ASSET_ATT);
	return g_strAttribute[m_AttComboBox->GetCurSel()];
}


void CAssetSettingDlgBar::OnCbnSelchangeComboAssetAtt()
{
	//	�Ӽ� ������ ���� �� �� Type�ǿ��� �Ӽ��� ������ �� ������ �߻��Ѵ�.
	//	�޺��ڽ����� ������ �Ͼ ���, �������� ���̾�α׹ٰ� ������ �Ǿ���Ѵ�.
	//	�Ӽ��� �°�.
	int iPrevIndex = m_iComboBoxIndex;

	m_AttComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ASSET_ATT);
	m_iComboBoxIndex = m_AttComboBox->GetCurSel();
	//	�Ӽ��� �ٲٸ� ������ �����ϴ� ���� �������.

	if(CObjMgr::GetInstance()->GetTargetMapCount(ASSET_EDITOR_TARGET) != 0)
	{
		int iResult = MessageBox(TEXT("�Ӽ� ����� ���� ���� ������ ������ϴ�. ��� �����ұ��?"), TEXT("Abort"), IDOK);

		if(iResult == IDOK)
		{
			CObjMgr::GetInstance()->ClearTargetMap(ASSET_EDITOR_TARGET);
			//	�Ӽ��� �ٲ�����Ƿ�, �����ʿ� ���÷��� �Ǵ� �Ӽ�â�� �����ϱ� ���� �޼����� ������
		}
		else
		{
			m_AttComboBox->SetCurSel(iPrevIndex);
			m_iComboBoxIndex = iPrevIndex;
			return ;
		}
	}

	::SendMessage(m_AssetEditorhWnd, WM_ASSET_ATT_CHANGE, (WPARAM)m_iComboBoxIndex, NULL);
}

void CAssetSettingDlgBar::OnBnClickedButtonAssetlistLoad()
{
	BrowseFolder();
}


void CAssetSettingDlgBar::BrowseFolder()
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
	pildBrowse    = ::SHBrowseForFolder(&bInfo);

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



void CAssetSettingDlgBar::RefreshAssetList()
{
	//	�ε� ��ư�� ������ ���� �����Ǿ��ִ� ���µ��� � ������ �ִ��� �߰� �ϸ� �ȴ�.
	m_AssetListBox = (CListBox*)GetDlgItem(IDC_ASSET_LIST_EDIT);
	m_TypeComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ASSET_TYPE);

	CDBMgr::GetInstance()->ClearAssetMap(ASSET_EDITOR_TARGET);
	//	���� �� ����Ʈ�� ����Ǿ�� �ϴ� ����Ʈ�� ����ش�
	for(int i = m_AssetListBox->GetCount(); i >= 0; i--)
	{
		m_AssetListBox->DeleteString(i);
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
			iIndex = m_AssetListBox->GetCount();

			CString strCmpAtt;
			m_TypeComboBox->GetWindowText(strCmpAtt);

			CT2CA pszConvertedAnsiString (strCmpAtt);
			std::string _stdCmpAtt(pszConvertedAnsiString);

			CheckAsset = CDBMgr::GetInstance()->LoadBasicAsset(iIndex, finder.GetFilePath(), finder.GetFileName(), ASSET_EDITOR_TARGET);

			if(CheckAsset.GetAssetType() ==  m_TypeComboBox->GetCurSel())
			{
				m_AssetListBox->AddString(finder.GetFileName());	
			}
			else
			{
				CDBMgr::GetInstance()->DeleteBasicAsset(iIndex,ASSET_EDITOR_TARGET);
			}
		}
	}
}

void CAssetSettingDlgBar::OnCbnSelchangeComboAssetType()
{
	m_TypeComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ASSET_TYPE);
	m_TypeComboBox->SetCurSel(m_TypeComboBox->GetCurSel());

	RefreshAssetList();
}

void CAssetSettingDlgBar::InitComboBox()
{
	m_AttComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ASSET_ATT);
	m_TypeComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ASSET_TYPE);

	m_AttComboBox->SetCurSel(0);
	m_TypeComboBox->SetCurSel(0);
	RefreshAssetList();

	//::SendMessage(m_AssetEditorhWnd, WM_CHANGE_ATTCOMBOBOX, NULL, NULL);
}


void CAssetSettingDlgBar::OnBnClickedButtonAssetviewClear()
{
	if(MessageBox(TEXT("���� ���� �ؽ��ĵ��� ��� �����Ͻðڽ��ϱ�?"), L"Abort", MB_YESNO) == IDYES)
	{
		CObjMgr::GetInstance()->SetSaveFileName("");
		CObjMgr::GetInstance()->ClearTargetMap(ASSET_EDITOR_TARGET);
		//InitComboBox();
		::SendMessage(m_AssetEditorhWnd,WM_CLEAR_ALL, NULL, NULL);

	}
}

//	AssetEditor�󿡼� ����Ʈ��Ʈ�ѷ� ���� ���� ����Ǿ��� ���.

void CAssetSettingDlgBar::OnEnChangeEditTexposX()
{
	if( m_bEditFocused == false)
		return;

	AdaptedChangedPosEdit();
}


void CAssetSettingDlgBar::OnEnChangeEditTexposY()
{
	if( m_bEditFocused == false)
		return;

	AdaptedChangedPosEdit();
}


void CAssetSettingDlgBar::OnEnChangeEditTexposZ()
{
	if( m_bEditFocused == false)
		return;

	m_pPos_Z_Edit = (CEdit*)GetDlgItem(IDC_EDIT_TEXPOS_Z);
	m_pPos_Z_Edit->GetWindowText(m_strPosZ);
}


void CAssetSettingDlgBar::OnEnSetfocusEditTexposX()
{
	m_bEditFocused = true;
}


void CAssetSettingDlgBar::OnEnSetfocusEditTexposY()
{
	m_bEditFocused = true;
}


void CAssetSettingDlgBar::OnEnSetfocusEditTexposZ()
{
	m_bEditFocused = true;
}

void CAssetSettingDlgBar::AdaptedChangedPosEdit()
{
	//	����Ʈ�� ��Ŀ���� �����ִ� ���¿��� ������ Ŭ���Ǹ� bEditFocused�� True�� ������ �ʴ� ����
	m_pPos_X_Edit = (CEdit*)GetDlgItem(IDC_EDIT_TEXPOS_X);
	m_pPos_Y_Edit = (CEdit*)GetDlgItem(IDC_EDIT_TEXPOS_Y);

	if(CObjMgr::GetInstance()->GetFocusAssetBase() != NULL && m_bEditFocused)
	{
		POINT	ptMoveDist;

		ptMoveDist.x = GetEditText(m_pPos_X_Edit);
		ptMoveDist.y = GetEditText(m_pPos_Y_Edit);

		m_AttComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ASSET_ATT);
		if(g_strAttribute[m_AttComboBox->GetCurSel()] == "LISTITEM")
		{
			CListItemBase* Focusbase = (CListItemBase*)CObjMgr::GetInstance()->GetFocusAssetBase();
			Focusbase->SetFocusAssetPos(ptMoveDist);
		}
		else
		{
			CObjMgr::GetInstance()->GetFocusAssetBase()->SetPosition(ptMoveDist);
		}
	}
}

afx_msg LRESULT CAssetSettingDlgBar::OnAssetBaseFocused(WPARAM wParam, LPARAM lParam)
{
	m_bEditFocused = false;

	CString strResult;
	char szITOA[20];

	CBaseInterface* FocusAsset = CObjMgr::GetInstance()->GetFocusAssetBase();

	m_pPos_X_Edit = (CEdit*)GetDlgItem(IDC_EDIT_TEXPOS_X);
	m_pPos_Y_Edit = (CEdit*)GetDlgItem(IDC_EDIT_TEXPOS_Y);

	(CStatic*)GetDlgItem(IDC_STATIC_ASSET_SETTING_POSITION)->SetFocus();

	_itoa_s(FocusAsset->GetPosition().x, szITOA, 10);
	std::string szResult = szITOA;
	strResult = szResult.c_str();
	m_pPos_X_Edit->SetWindowText(strResult);

	_itoa_s(FocusAsset->GetPosition().y, szITOA, 10);
	szResult = szITOA;
	strResult = szResult.c_str();
	m_pPos_Y_Edit->SetWindowText(strResult);

	return 0;
}

void CAssetSettingDlgBar::SetPosEdit( CString _pos, eAxisType _type )
{
	m_pPos_X_Edit = (CEdit*)GetDlgItem(IDC_EDIT_TEXPOS_X);
	m_pPos_Y_Edit = (CEdit*)GetDlgItem(IDC_EDIT_TEXPOS_Y);
	m_pPos_Z_Edit = (CEdit*)GetDlgItem(IDC_EDIT_TEXPOS_Z);

	switch(_type)
	{
	case AXIS_X:
		m_strPosX = _pos;
		m_pPos_X_Edit->SetWindowText(m_strPosX);
		break;
	case AXIS_Y:
		m_strPosY = _pos;
		m_pPos_Y_Edit->SetWindowText(m_strPosY);
		break;
	case AXIS_Z:
		m_strPosZ = _pos;
		m_pPos_Z_Edit->SetWindowText(m_strPosZ);
		break;
	}
}

void CAssetSettingDlgBar::UpdateEdit()
{
	m_pPos_X_Edit = (CEdit*)GetDlgItem(IDC_EDIT_TEXPOS_X);
	m_pPos_Y_Edit = (CEdit*)GetDlgItem(IDC_EDIT_TEXPOS_Y);
	m_pPos_Z_Edit = (CEdit*)GetDlgItem(IDC_EDIT_TEXPOS_Z);

	m_pPos_X_Edit->SetWindowText(m_strPosX);
	m_pPos_Y_Edit->SetWindowText(m_strPosY);
	m_pPos_Z_Edit->SetWindowText(m_strPosZ);
}


void CAssetSettingDlgBar::OnLbnDblclkAssetListEdit()
{
	m_AssetListBox = (CListBox*)GetDlgItem(IDC_ASSET_LIST_EDIT);
	m_AttComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ASSET_ATT);
	m_TypeComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ASSET_TYPE);
	int iIndex = m_AssetListBox->GetCurSel();

	//	������ ������ �޾ƿ´�.
	CBasicAsset* cSelBasicAsset = CDBMgr::GetInstance()->GetBasicAsset(iIndex, ASSET_EDITOR_TARGET);

	if(cSelBasicAsset != NULL)
	{		
		CObjMgr::GetInstance()->SetSaveFileName(cSelBasicAsset->GetFileName());

		//���⼭ combobox�� �Ӽ��� ���� ���õ� �ַ� �ٲ��ָ� ��
		if(CObjMgr::GetInstance()->GetTargetMapCount(ASSET_EDITOR_TARGET) != ZEROBASE)
		{
			/*
			//	�߰� ������ ����� ���� �ּ� ó��
			if(AfxMessageBox(TEXT("���� ���� ������ �ֽ��ϴ�. �����Ͻðڽ��ϱ�?"), MB_OKCANCEL) == IDOK)
			{
			::SendMessage(m_AssetEditorhWnd,WM_SAVE_ASSET_MSG, NULL, NULL);
			}
			*/
			CObjMgr::GetInstance()->ClearTargetMap(ASSET_EDITOR_TARGET);
		}

		m_AttComboBox->SetCurSel(m_TypeComboBox->GetCurSel());
		OnCbnSelchangeComboAssetAtt();

		//	������ �Ҵ��ϰ� �ش� ������ ���θ� ä���ش�.

		CBaseInterface* CopyAsset = cSelBasicAsset->GetAssetBase();

		CBaseInterface* newBase = MakeAdaptableBase(CopyAsset->GetAssetType());
		newBase->SetAssetType(CopyAsset->GetAssetType());
		//	���⼭ �Ӽ����� ���� ī���ؼ� �Ѱ�����Ѵ�.
		//	virtual �Լ��� �ؼ� �ڽŵ��� �˾Ƽ� ī�Ǹ� ��Ű�� ����.
		//	������ �����ڿ��� ���ִ� �۾����� �Լ��� ���� �ϸ� �� �� ����.
		if(newBase != NULL)
		{
			newBase->Copy(CopyAsset);
			newBase->SetParenthWnd(m_AssetEditorhWnd);
			CObjMgr::GetInstance()->InsertAssetBase(newBase);

			//	�������� �Ӽ�â�� �ִ� ��ҵ��� ���Ž����ش�.
			::SendMessageA(m_AssetEditorhWnd, WM_REFRESH_DLGBAR_INFO, (WPARAM)newBase, NULL);
			::SendMessageA(m_AssetEditorhWnd, WM_ASSETEDITOR_ASSETLIST_DOUBLE_CLICK, (WPARAM)newBase, NULL);			
		}
	}
}

int CALLBACK CAssetSettingDlgBar::BrowseCallbackProc( HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData )
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


/*
20180914 : Gabrielle
��ĳ������ Ȱ���Ͽ� ���ڰ� �˾Ƽ� �ڽ��� ī�Ǹ� �����Լ��� �̿��� ȣ���ϰԲ� �������
*/

void CAssetSettingDlgBar::OnBnClickedButtonPicturewndCreate()
{
	::SendMessage(m_AssetEditorhWnd, WM_PICTUREWND_CREATE, NULL, NULL);
	SetPicWndBtnEnable(false);
}

void CAssetSettingDlgBar::SetPicWndBtnEnable( bool _bEnable )
{	
	if(_bEnable)
	{
		m_bPictureWnd = true;
	}
	else
	{
		m_bPictureWnd = false;
	}
}

void CAssetSettingDlgBar::OnUpdateCmdUI(CCmdUI* pCmdUI) // ���̾�α׹� ��ư Ȱ��ȭ ���� �߰�
{
	switch(pCmdUI->m_nID)
	{
	case IDC_BUTTON_PICTUREWND_CREATE:
		pCmdUI->Enable(m_bPictureWnd);
		break;
	default:
		pCmdUI->Enable(TRUE);
		break;
	}
}

void CAssetSettingDlgBar::SetAttCombobox( int _index )
{
	m_AttComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ASSET_ATT);
	m_AttComboBox->SetCurSel(_index);
	OnCbnSelchangeComboAssetAtt();
}


//	���⿡�� �˾��� ����ִµ� ���������� SceneEditor�� ���� �ö���� ���� �߻�
void CAssetSettingDlgBar::OnBnClickedButtonSaveEditorAsset()
{
	if(CObjMgr::GetInstance()->SaveAssetByFileDlg() == false)
	{
		MessageBox(TEXT("���忡 �����߽��ϴ�."));
	}
	RefreshAssetList();
}


void CAssetSettingDlgBar::OnBnClickedButtonLoadEditorAsset()
{
	CObjMgr::GetInstance()->LoadAssetByFileDlg(m_AssetEditorhWnd);
	RefreshAssetList();
}
