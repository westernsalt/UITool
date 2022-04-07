// AssetSettingDlgBar.cpp : 구현 파일입니다.
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


// CAssetSettingDlgBar 메시지 처리기입니다.

int CAssetSettingDlgBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_AssetListBox = (CListBox*)GetDlgItem(IDC_ASSET_LIST_EDIT);
	m_TypeComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ASSET_TYPE);

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

void CAssetSettingDlgBar::InitializeRrc()
{
	m_AttComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ASSET_ATT);
	m_TypeComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ASSET_TYPE);
	//	높이 조절하려고 삽질했는데
	//	리소스 편집창 가서 밑으로 늘리는 화살표 누르고 밑으로 쭉 내렸더니 커짐 ㅡㅡ

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
	//	속성 변경을 먼저 한 후 Type탭에서 속성을 변경할 시 에러가 발생한다.
	//	콤보박스에서 변경이 일어난 경우, 오른쪽의 다이얼로그바가 변경이 되어야한다.
	//	속성에 맞게.
	int iPrevIndex = m_iComboBoxIndex;

	m_AttComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ASSET_ATT);
	m_iComboBoxIndex = m_AttComboBox->GetCurSel();
	//	속성을 바꾸면 이전에 편집하던 것이 사라진다.

	if(CObjMgr::GetInstance()->GetTargetMapCount(ASSET_EDITOR_TARGET) != 0)
	{
		int iResult = MessageBox(TEXT("속성 변경시 편집 중인 에셋이 사라집니다. 계속 진행할까요?"), TEXT("Abort"), IDOK);

		if(iResult == IDOK)
		{
			CObjMgr::GetInstance()->ClearTargetMap(ASSET_EDITOR_TARGET);
			//	속성이 바뀌었으므로, 오른쪽에 디스플레이 되는 속성창을 변경하기 위해 메세지를 보낸다
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

	//메모리 해제를 한다. 
	pMalloc->Free(pildBrowse);
	pMalloc->Release();
}



void CAssetSettingDlgBar::RefreshAssetList()
{
	//	로드 버튼이 눌리면 현재 생성되어있는 에셋들이 어떤 종류가 있는지 뜨게 하면 된다.
	m_AssetListBox = (CListBox*)GetDlgItem(IDC_ASSET_LIST_EDIT);
	m_TypeComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ASSET_TYPE);

	CDBMgr::GetInstance()->ClearAssetMap(ASSET_EDITOR_TARGET);
	//	현재 이 리스트에 노출되어야 하는 리스트를 비워준다
	for(int i = m_AssetListBox->GetCount(); i >= 0; i--)
	{
		m_AssetListBox->DeleteString(i);
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
	if(MessageBox(TEXT("에셋 뷰의 텍스쳐들을 모두 삭제하시겠습니까?"), L"Abort", MB_YESNO) == IDYES)
	{
		CObjMgr::GetInstance()->SetSaveFileName("");
		CObjMgr::GetInstance()->ClearTargetMap(ASSET_EDITOR_TARGET);
		//InitComboBox();
		::SendMessage(m_AssetEditorhWnd,WM_CLEAR_ALL, NULL, NULL);

	}
}

//	AssetEditor상에서 에디트컨트롤로 인해 값이 변경되었을 경우.

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
	//	에디트에 포커스가 들어와있는 상태에서 에셋이 클릭되면 bEditFocused가 True로 들어오지 않는 현상
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

	//	베이직 에셋을 받아온다.
	CBasicAsset* cSelBasicAsset = CDBMgr::GetInstance()->GetBasicAsset(iIndex, ASSET_EDITOR_TARGET);

	if(cSelBasicAsset != NULL)
	{		
		CObjMgr::GetInstance()->SetSaveFileName(cSelBasicAsset->GetFileName());

		//여기서 combobox의 속성을 현재 선택된 애로 바꿔주면 됨
		if(CObjMgr::GetInstance()->GetTargetMapCount(ASSET_EDITOR_TARGET) != ZEROBASE)
		{
			/*
			//	중간 저장을 물어보는 구문 주석 처리
			if(AfxMessageBox(TEXT("편집 중인 에셋이 있습니다. 저장하시겠습니까?"), MB_OKCANCEL) == IDOK)
			{
			::SendMessage(m_AssetEditorhWnd,WM_SAVE_ASSET_MSG, NULL, NULL);
			}
			*/
			CObjMgr::GetInstance()->ClearTargetMap(ASSET_EDITOR_TARGET);
		}

		m_AttComboBox->SetCurSel(m_TypeComboBox->GetCurSel());
		OnCbnSelchangeComboAssetAtt();

		//	에셋을 할당하고 해당 에셋의 내부를 채워준다.

		CBaseInterface* CopyAsset = cSelBasicAsset->GetAssetBase();

		CBaseInterface* newBase = MakeAdaptableBase(CopyAsset->GetAssetType());
		newBase->SetAssetType(CopyAsset->GetAssetType());
		//	여기서 속성들을 전부 카피해서 넘겨줘야한다.
		//	virtual 함수로 해서 자신들이 알아서 카피를 시키게 하자.
		//	기존에 생성자에서 해주던 작업들을 함수로 빼서 하면 될 것 같다.
		if(newBase != NULL)
		{
			newBase->Copy(CopyAsset);
			newBase->SetParenthWnd(m_AssetEditorhWnd);
			CObjMgr::GetInstance()->InsertAssetBase(newBase);

			//	에디터의 속성창에 있는 요소들을 갱신시켜준다.
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
업캐스팅을 활용하여 각자가 알아서 자신의 카피를 가상함수를 이용해 호출하게끔 만들었다
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

void CAssetSettingDlgBar::OnUpdateCmdUI(CCmdUI* pCmdUI) // 다이얼로그바 버튼 활성화 위해 추가
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


//	여기에서 팝업을 띄워주는데 간헐적으로 SceneEditor가 위로 올라오는 현상 발생
void CAssetSettingDlgBar::OnBnClickedButtonSaveEditorAsset()
{
	if(CObjMgr::GetInstance()->SaveAssetByFileDlg() == false)
	{
		MessageBox(TEXT("저장에 실패했습니다."));
	}
	RefreshAssetList();
}


void CAssetSettingDlgBar::OnBnClickedButtonLoadEditorAsset()
{
	CObjMgr::GetInstance()->LoadAssetByFileDlg(m_AssetEditorhWnd);
	RefreshAssetList();
}
