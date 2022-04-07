// PictureDlgBar.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "UITool.h"
#include "PictureDlgBar.h"
#include "DBMgr.h"
#include "PictureWnd.h"
#include "ObjMgr.h"
#include "AssetBase.h"

#define BITMAP_SET 32

// CPictureDlgBar

IMPLEMENT_DYNAMIC(CPictureDlgBar, CDialogBar)

	CPictureDlgBar::CPictureDlgBar()
{
	m_pTexture = NULL;
	m_bChange = true;
}

CPictureDlgBar::~CPictureDlgBar()
{
	DestroyWindow();
}

BEGIN_MESSAGE_MAP(CPictureDlgBar, CDialogBar)
	ON_WM_DROPFILES()
	ON_WM_CREATE()
	ON_LBN_SELCHANGE(IDC_LIST_TEXTUREFILE, &CPictureDlgBar::OnLbnSelchangeListTexturefile)
	ON_MESSAGE(WM_ALLSEL_CHECKED, &CPictureDlgBar::OnAllSelChecked)
	ON_BN_CLICKED(IDC_CHECK_ALL_SELECT, &CPictureDlgBar::OnBnClickedCheckAllSelect)
	ON_BN_CLICKED(IDC_BUTTON_TEXTURE_LOAD, &CPictureDlgBar::OnBnClickedButtonTextureLoad)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_TEXTURE_LOAD, &CPictureDlgBar::OnUpdateCmdUI)
	ON_EN_CHANGE(IDC_EDIT_DRAGBOX_X, &CPictureDlgBar::OnEnChangeEditDragboxX)
	ON_EN_CHANGE(IDC_EDIT_DRAGBOX_Y, &CPictureDlgBar::OnEnChangeEditDragboxY)
	ON_EN_CHANGE(IDC_EDIT_DRAGBOX_WIDTH, &CPictureDlgBar::OnEnChangeEditDragboxWidth)
	ON_EN_CHANGE(IDC_EDIT_DRAGBOX_HEIGHT, &CPictureDlgBar::OnEnChangeEditDragboxHeight)
END_MESSAGE_MAP()

int CPictureDlgBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	// ���̾�α� �ٿ� �����츦 �ޱ� ���� SplitterWnd �۾�
	m_pTextureFileList = (CListBox*)GetDlgItem(IDC_LIST_TEXTUREFILE);
	m_pAllSelectChkBox = (CButton*)GetDlgItem(IDC_CHECK_ALL_SELECT);
	m_pTextureLoadBtn = (CButton*)GetDlgItem(IDC_BUTTON_TEXTURE_LOAD);

	TCHAR strInitPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, strInitPath);
	// �н� ���� : Release 
#if defined _DEBUG || defined RELEASE_DEV_PATH
	_tcscat_s(strInitPath, L"\\Texture");
#else
	_tcscat_s(strInitPath, TGA_FOLDER_PATH);
#endif
	m_strInitPath = strInitPath;
	return 0;
}

void CPictureDlgBar::OnDropFiles(HDROP hDropInfo)
{
	m_pTextureFileList = (CListBox*)GetDlgItem(IDC_LIST_TEXTUREFILE);
	int nFiles;
	int iIndex;
	char szPathName[MAX_PATH];
	CString strFileName;
	CString strName;
	std::string strResult;

	nFiles = ::DragQueryFile(hDropInfo, 0xFFFFFFFF, (LPWSTR)szPathName, MAX_PATH);
	::DragQueryFile(hDropInfo, 0, (LPWSTR)szPathName, MAX_PATH);

	//	������ ��������� �������� ���� �о�´�.
	//	������ �ϳ���� �������� ���������� �����Ͽ� ó���Ѵ�.
	if(nFiles > 1)
	{
		for(int i = nFiles - 1; i >= 0; i--)
		{
			::DragQueryFile(hDropInfo, i, (LPWSTR)szPathName, MAX_PATH);
			iIndex = m_pTextureFileList->GetCount();

			strResult = CDBMgr::GetInstance()->LoadTexture(iIndex, (LPCTSTR)szPathName);
			if(strResult != "")
			{
				strName = strResult.c_str();
				m_pTextureFileList->AddString((LPCTSTR)strName);
			}			
		}
	}
	else if(nFiles <= 1)
	{
		CFileFind  finder;
		bool bReadFolder = true;
		BOOL bWorking;
		int iCount = 0;

		while(szPathName[iCount] >= 0)
		{
			if(szPathName[iCount] == '.')
			{
				//	���� �̰� �ϳ���� ���� �����δ� ���� �ʴ´�.
				bReadFolder = false;
				break;
			}
			++iCount;
		}

		if(bReadFolder)
		{
			//	���� ������ ������, ��ü ������ �е��� �ؾ��Ѵ�.
			//	�н��� 
			PathAppend((LPWSTR)szPathName, L"*.*");
			bWorking = finder.FindFile((LPCTSTR)szPathName);
		}
		else
		{
			bWorking = finder.FindFile((LPCTSTR)szPathName);
		}

		while(bWorking)
		{
			bWorking = finder.FindNextFile();
			if(!finder.IsDots() && !finder.IsDirectory())
			{
				iIndex = m_pTextureFileList->GetCount();
				strResult = CDBMgr::GetInstance()->LoadTexture(iIndex, finder.GetFilePath());
				if(strResult != "")
					m_pTextureFileList->AddString(finder.GetFileName());
			}
		}
	}

	::DragFinish(hDropInfo);

	UpdateData(TRUE);
	CDialogBar::OnDropFiles(hDropInfo);
}

void CPictureDlgBar::OnLbnSelchangeListTexturefile()
{
	m_pTextureFileList = (CListBox*)GetDlgItem(IDC_LIST_TEXTUREFILE);
	if(m_pTextureFileList->GetCount() != 0)
	{
		//���⼭ �̴ϼȶ�����
		m_pAllSelectChkBox = (CButton*)GetDlgItem(IDC_CHECK_ALL_SELECT);
		m_pAllSelectChkBox->SetCheck(BST_UNCHECKED);
		OnBnClickedCheckAllSelect();

		int iIndex = m_pTextureFileList->GetCurSel();
		m_pTexture = CDBMgr::GetInstance()->GetTexture(iIndex);
		::SendMessage(m_pParentWnd, WM_SELCHANGE_TEXTURELIST, (WPARAM)iIndex, NULL);
		::SendMessage(m_pDlgBarhWnd, WM_SELCHANGE_TEXTURELIST, (WPARAM)iIndex, NULL);

		UpdateData(true);
	}
}

RECT CPictureDlgBar::GetCalculateRectByRate(RECT rect, double dValue)
{
	RECT result;
	result.top = (LONG)(rect.top * dValue);
	result.left = (LONG)(rect.left * dValue);
	result.right = (LONG)(rect.right * dValue);
	result.bottom = (LONG)(rect.bottom * dValue);

	return result;
}

int CPictureDlgBar::GetCurSelTextureListItem()
{
	m_pTextureFileList = (CListBox*)GetDlgItem(IDC_LIST_TEXTUREFILE);
	return m_pTextureFileList->GetCurSel();
}

afx_msg LRESULT CPictureDlgBar::OnAllSelChecked( WPARAM wParam, LPARAM lParam )
{
	m_pAllSelectChkBox = (CButton*)GetDlgItem(IDC_CHECK_ALL_SELECT);
	// �ؽ��İ� �����ϰ� üũ�ڽ��� üũ�Ǿ�����
	if(m_pAllSelectChkBox->GetCheck() == BST_CHECKED)
	{
		::SendMessage(m_pParentWnd, WM_ALLSEL_CHECKED, (WPARAM)m_pTexture->GetWidth(), (LPARAM)m_pTexture->GetHeight());
	}
	return 0;
}

void CPictureDlgBar::OnBnClickedCheckAllSelect()
{
	m_pAllSelectChkBox = (CButton*)GetDlgItem(IDC_CHECK_ALL_SELECT);
	m_pTextureFileList = (CListBox*)GetDlgItem(IDC_LIST_TEXTUREFILE);

	int iChkBoxState = m_pAllSelectChkBox->GetCheck();
	if(iChkBoxState == BST_CHECKED && m_pTexture != NULL)
	{
		OnAllSelChecked(NULL,NULL);
	}
	else if(iChkBoxState == BST_UNCHECKED && m_pTexture != NULL)
	{
		//	�ؽ��İ� �����ϰ� üũ�ڽ��� �����Ǿ�����.
		::SendMessage(m_pParentWnd, WM_PAINT, NULL, NULL);
		::SendMessage(m_pParentWnd, WM_RESET_PICWND_ELEMENT, NULL, NULL);
	}
}

void CPictureDlgBar::OnBnClickedButtonTextureLoad()
{
	// ����ִ� �ؽ��ĸ� �׷��ִ� ���� �ذ� ���ؼ�
	//  [11/29/2018 Gabrielle]
	::SendMessage(m_pParentWnd, WM_RESET_PICWND_ELEMENT, NULL, NULL);
	m_pTextureFileList = (CListBox*)GetDlgItem(IDC_LIST_TEXTUREFILE);
	BrowseFolder(m_strTextureListPath);
	LoadTextureList(m_strTextureListPath);
}

void CPictureDlgBar::LoadTextureList( CString _strPath )
{
	m_pTextureFileList = (CListBox*)GetDlgItem(IDC_LIST_TEXTUREFILE);
	m_pTextureFileList->SetCurSel(-1);

	CDBMgr::GetInstance()->ClearTextureMap();
	CDBMgr::GetInstance()->ClearPathMap();

	for(int i = m_pTextureFileList->GetCount(); i >= 0; i--)
	{
		m_pTextureFileList->DeleteString(i);
	}

	CString strInputPath(_strPath);

	TCHAR szLoadingPath[MAX_PATH] = L"";
	_tcscpy_s(szLoadingPath, _countof(szLoadingPath), strInputPath.GetBuffer());
	_tcscat_s(szLoadingPath, L"\\*.tga");

	CFileFind finder;
	bool bWorking = finder.FindFile((LPCTSTR)szLoadingPath);

	while(bWorking)
	{
		bWorking = finder.FindNextFile();
		if(!finder.IsDots() && !finder.IsDirectory())
		{
			int iIndex = m_pTextureFileList->GetCount();
			CString	FilePath = finder.GetFilePath();
			std::string strResult = CDBMgr::GetInstance()->LoadTexture(iIndex, FilePath);
			if(strResult != "")
				m_pTextureFileList->AddString(finder.GetFileName());
		}
	}
}

void CPictureDlgBar::OnUpdateCmdUI(CCmdUI* pCmdUI) // ���̾�α׹� ��ư Ȱ��ȭ ���� �߰�
{
	pCmdUI->Enable(TRUE);
}

void CPictureDlgBar::BrowseFolder( CString& _strPath )
{
	//	���� �ε� ��θ� �����Ѵ�
	ITEMIDLIST*	pildBrowse;
	TCHAR		pszPathname[MAX_PATH];
	BROWSEINFO	bInfo;
	memset(&bInfo, 0, sizeof(bInfo));

	LPCTSTR lpszDefaultFolder = m_strTextureListPath;

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
	::OleInitialize(NULL);

	pildBrowse    = ::SHBrowseForFolder(&bInfo);

	if(pildBrowse)
	{
		SHGetPathFromIDList(pildBrowse, pszPathname);
		CString strTemp(pszPathname);
		_strPath = strTemp;
		m_strTextureListPath = _strPath;
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

void CPictureDlgBar::Initialize()
{
	m_bChange = false;

	m_pTextureFileList = (CListBox*)GetDlgItem(IDC_LIST_TEXTUREFILE);
	LoadTextureList(m_strInitPath);

	m_pDragBoxPos_X_Edit = (CEdit*)GetDlgItem(IDC_EDIT_DRAGBOX_X);
	m_pDragBoxPos_Y_Edit = (CEdit*)GetDlgItem(IDC_EDIT_DRAGBOX_Y);
	m_pDragBoxWidthEdit = (CEdit*)GetDlgItem(IDC_EDIT_DRAGBOX_WIDTH);
	m_pDragBoxHeightEdit = (CEdit*)GetDlgItem(IDC_EDIT_DRAGBOX_HEIGHT);

	m_tDragBoxPos.x = 0;
	m_tDragBoxPos.y = 0;
	m_tDragBoxSize.x = 0;
	m_tDragBoxSize.y = 0;

	m_pDragBoxPos_X_Edit->SetWindowText(L"0");
	m_pDragBoxPos_Y_Edit->SetWindowText(L"0");
	m_pDragBoxWidthEdit->SetWindowText(L"0");
	m_pDragBoxHeightEdit->SetWindowText(L"0");
	//	�ʱ�ȭ ��ų�� Ŀ��Ʈ ����Ʈ �ʱ�ȭ.
	m_pTextureFileList->SetCurSel(-1);

	m_bChange = true;
}

int CALLBACK CPictureDlgBar::BrowseCallbackProc( HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData )
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

//	���⼭ ���� �����ϸ� �̰Ÿ� ���ĺ信�� �ݿ��� ��������Ѵ�.

void CPictureDlgBar::OnEnChangeEditDragboxX()
{
	if(m_bChange)
	{
		m_pDragBoxPos_X_Edit = (CEdit*)GetDlgItem(IDC_EDIT_DRAGBOX_X);
		m_tDragBoxPos.x = GetEditText(m_pDragBoxPos_X_Edit);
		::SendMessage(m_pParentWnd, WM_CHANGE_DRAGBOX_INFO, (WPARAM)m_tDragBoxPos.x, (LPARAM)DRAGBOX_LEFT);
		::SendMessage(m_pDlgBarhWnd, WM_CHANGE_DRAGBOX_INFO, (WPARAM)m_tDragBoxPos.x, (LPARAM)DRAGBOX_LEFT);
	}
}


void CPictureDlgBar::OnEnChangeEditDragboxY()
{
	if(m_bChange)
	{
		m_pDragBoxPos_Y_Edit = (CEdit*)GetDlgItem(IDC_EDIT_DRAGBOX_Y);
		m_tDragBoxPos.y = GetEditText(m_pDragBoxPos_Y_Edit);
		::SendMessage(m_pParentWnd, WM_CHANGE_DRAGBOX_INFO, (WPARAM)m_tDragBoxPos.y, (LPARAM)DRAGBOX_TOP);
		::SendMessage(m_pDlgBarhWnd, WM_CHANGE_DRAGBOX_INFO, (WPARAM)m_tDragBoxPos.y, (LPARAM)DRAGBOX_TOP);
	}
}


void CPictureDlgBar::OnEnChangeEditDragboxWidth()
{
	if(m_bChange)
	{
		m_pDragBoxWidthEdit = (CEdit*)GetDlgItem(IDC_EDIT_DRAGBOX_WIDTH);
		m_tDragBoxSize.x = GetEditText(m_pDragBoxWidthEdit);
		::SendMessage(m_pParentWnd, WM_CHANGE_DRAGBOX_INFO,
			(WPARAM)m_tDragBoxSize.x, (LPARAM)DRAGBOX_WIDTH);
		::SendMessage(m_pDlgBarhWnd, WM_CHANGE_DRAGBOX_INFO,
			(WPARAM)m_tDragBoxSize.x, (LPARAM)DRAGBOX_WIDTH);
	}
}


void CPictureDlgBar::OnEnChangeEditDragboxHeight()
{
	if(m_bChange)
	{
		m_pDragBoxHeightEdit = (CEdit*)GetDlgItem(IDC_EDIT_DRAGBOX_HEIGHT);
		m_tDragBoxSize.y = GetEditText(m_pDragBoxHeightEdit);
		::SendMessage(m_pParentWnd, WM_CHANGE_DRAGBOX_INFO,
			(WPARAM)m_tDragBoxSize.y, (LPARAM)DRAGBOX_HEIGHT);
		::SendMessage(m_pDlgBarhWnd, WM_CHANGE_DRAGBOX_INFO,
			(WPARAM)m_tDragBoxSize.y, (LPARAM)DRAGBOX_HEIGHT);
	}
}

void CPictureDlgBar::UpdateEditData( RECT _rtDragBox )
{
	m_pDragBoxPos_X_Edit = (CEdit*)GetDlgItem(IDC_EDIT_DRAGBOX_X);
	m_pDragBoxPos_Y_Edit = (CEdit*)GetDlgItem(IDC_EDIT_DRAGBOX_Y);
	m_pDragBoxWidthEdit = (CEdit*)GetDlgItem(IDC_EDIT_DRAGBOX_WIDTH);
	m_pDragBoxHeightEdit = (CEdit*)GetDlgItem(IDC_EDIT_DRAGBOX_HEIGHT);

	CString strResult;
	char szITOA[20];

	_itoa_s(_rtDragBox.left, szITOA, 10);
	std::string szResult = szITOA;
	strResult = szResult.c_str();
	m_pDragBoxPos_X_Edit->SetWindowText(strResult);

	_itoa_s(_rtDragBox.top, szITOA, 10);
	szResult = szITOA;
	strResult = szResult.c_str();
	m_pDragBoxPos_Y_Edit->SetWindowText(strResult);

	_itoa_s(_rtDragBox.right - _rtDragBox.left, szITOA, 10);
	szResult = szITOA;
	strResult = szResult.c_str();
	m_pDragBoxWidthEdit->SetWindowText(strResult);

	_itoa_s(_rtDragBox.bottom - _rtDragBox.top, szITOA, 10);
	szResult = szITOA;
	strResult = szResult.c_str();
	m_pDragBoxHeightEdit->SetWindowText(strResult);
}

void CPictureDlgBar::SetEditWnd( int _num, eDragboxSet _set )
{
	//	�ణ ũ��Ƽ�� ���� ó��.
	m_bChange = false;

	m_pDragBoxPos_X_Edit = (CEdit*)GetDlgItem(IDC_EDIT_DRAGBOX_X);
	m_pDragBoxPos_Y_Edit = (CEdit*)GetDlgItem(IDC_EDIT_DRAGBOX_Y);
	m_pDragBoxWidthEdit = (CEdit*)GetDlgItem(IDC_EDIT_DRAGBOX_WIDTH);
	m_pDragBoxHeightEdit = (CEdit*)GetDlgItem(IDC_EDIT_DRAGBOX_HEIGHT);

	switch(_set)
	{
	case DRAGBOX_LEFT:
		m_tDragBoxPos.x = _num;
		SetEditWndText(m_pDragBoxPos_X_Edit, _num);
		break;
	case DRAGBOX_TOP:
		m_tDragBoxPos.y = _num;
		SetEditWndText(m_pDragBoxPos_Y_Edit, _num);
		break;
	case DRAGBOX_WIDTH:
		m_tDragBoxSize.x = _num;
		SetEditWndText(m_pDragBoxWidthEdit, _num);
		break;
	case DRAGBOX_HEIGHT:
		m_tDragBoxSize.y = _num;
		SetEditWndText(m_pDragBoxHeightEdit, _num);
		break;
	}

	m_bChange = true;
}

void CPictureDlgBar::SetTextureListIndex(CString _name)
{
	m_pTextureFileList = (CListBox*)GetDlgItem(IDC_LIST_TEXTUREFILE);
	int iIndex = m_pTextureFileList->FindString(0, _name);
	m_pTextureFileList->SetCurSel(iIndex);
	OnLbnSelchangeListTexturefile();
}

void CPictureDlgBar::SetTextureListIndex( int _index )
{
	m_pTextureFileList = (CListBox*)GetDlgItem(IDC_LIST_TEXTUREFILE);
	m_pTextureFileList->SetCurSel(_index);
	OnLbnSelchangeListTexturefile();
}

void CPictureDlgBar::SetPos( POINT _pos )
{
	m_pDragBoxPos_X_Edit = (CEdit*)GetDlgItem(IDC_EDIT_DRAGBOX_X);
	m_pDragBoxPos_Y_Edit = (CEdit*)GetDlgItem(IDC_EDIT_DRAGBOX_Y);
	
	CString strPos;
	char szPos[30];

	_itoa_s(_pos.x, szPos, 10);
	strPos = szPos;
	m_pDragBoxPos_X_Edit->SetWindowText(strPos);

	_itoa_s(_pos.y, szPos, 10);
	strPos = szPos;
	m_pDragBoxPos_Y_Edit->SetWindowText(strPos);
}

void CPictureDlgBar::SetSize( POINT _size )
{
	m_pDragBoxWidthEdit = (CEdit*)GetDlgItem(IDC_EDIT_DRAGBOX_WIDTH);
	m_pDragBoxHeightEdit = (CEdit*)GetDlgItem(IDC_EDIT_DRAGBOX_HEIGHT);

	CString strPos;
	char szPos[30];

	_itoa_s(_size.x, szPos, 10);
	strPos = szPos;
	m_pDragBoxWidthEdit->SetWindowText(strPos);

	_itoa_s(_size.y, szPos, 10);
	strPos = szPos;
	m_pDragBoxHeightEdit->SetWindowText(strPos);
}

void CPictureDlgBar::SetCheckBoxUnchecked()
{
	m_pAllSelectChkBox = (CButton*)GetDlgItem(IDC_CHECK_ALL_SELECT);
	m_pAllSelectChkBox->SetCheck(BST_UNCHECKED);
}

void CPictureDlgBar::SetTextureListPath( CString _Path )
{
	m_strTextureListPath = _Path;
}
