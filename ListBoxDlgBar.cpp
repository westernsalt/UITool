// ListBoxDlgBar.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "UITool.h"
#include "ListBoxDlgBar.h"
#include "afxdialogex.h"
#include "ObjMgr.h"
#include "ListBoxBase.h"
#include "DBMgr.h"


// CListBoxDlgBar ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CListBoxDlgBar, CDialogBar)

CListBoxDlgBar::CListBoxDlgBar(CWnd* pParent /*=NULL*/)
{
	m_bChange = true;
	POINT ptZero;
	ptZero.x = 0;
	ptZero.y = 0;

	m_strBGTexPath = "";
	m_ptBGPos = ptZero;
	m_ptBGSize = ptZero;

	m_ptListInterval = ptZero;

	m_strScrollPath = "";
	m_strListItemPath = "";

	m_ListBoxBase = NULL;
	m_ListBoxBaseScroll = NULL;
	m_ListBoxBaseItem = NULL;
	m_iListItemCount = 0;

	m_ptListitemStartPos = ptZero;
}

CListBoxDlgBar::~CListBoxDlgBar()
{
	SAFE_DELETE(m_ListBoxBaseItem);
	SAFE_DELETE(m_ListBoxBaseScroll);
}

void CListBoxDlgBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);

	m_BGTexPathEdit			= (CEdit*)GetDlgItem(IDC_EDIT_LST_SHOW_TEXPATH);
	m_BGXPosEdit			= (CEdit*)GetDlgItem(IDC_EDIT_LST_BG_XPOS);
	m_BGYPosEdit			= (CEdit*)GetDlgItem(IDC_EDIT_LST_BG_YPOS);
	m_BGWidthEdit			= (CEdit*)GetDlgItem(IDC_EDIT_LST_BG_WIDTH);
	m_BGHeightEdit			= (CEdit*)GetDlgItem(IDC_EDIT_LST_BG_HEIGHT);
	m_ListIntervalColEdit	= (CEdit*)GetDlgItem(IDC_EDIT_LST_COL_INTERVAL);
	m_ListIntervalRowEdit	= (CEdit*)GetDlgItem(IDC_EDIT_LST_ROW_INTERVAL);
	m_ListItemCountEdit		= (CEdit*)GetDlgItem(IDC_EDIT_LISTITEM_COUNT);
	m_ScrollPathEdit		= (CEdit*)GetDlgItem(IDC_EDIT_LST_SCROLL_PATH);
	m_ListItemPathEdit		= (CEdit*)GetDlgItem(IDC_EDIT_LST_LISTITEM_PATH);
	m_SetScrollBtn			= (CButton*)GetDlgItem(IDC_BUTTON_SET_SCROLL);
	m_SetListItemBtn		= (CButton*)GetDlgItem(IDC_BUTTON_SET_LISTITEM);
	m_ListItemStartXPosEdit	= (CEdit*)GetDlgItem(IDC_EDIT_LST_START_XPOS);
	m_ListItemStartYPosEdit	= (CEdit*)GetDlgItem(IDC_EDIT_LST_START_YPOS);

	if(CObjMgr::GetInstance()->GetFocusAssetBase() == NULL)
	{
		m_ListBoxBase = NULL;
	}
}


BEGIN_MESSAGE_MAP(CListBoxDlgBar, CDialogBar)
	ON_EN_CHANGE(IDC_EDIT_LST_SHOW_TEXPATH, &CListBoxDlgBar::OnEnChangeEditLstShowTexpath)
	ON_EN_CHANGE(IDC_EDIT_LST_BG_XPOS, &CListBoxDlgBar::OnEnChangeEditLstBgXpos)
	ON_EN_CHANGE(IDC_EDIT_LST_BG_YPOS, &CListBoxDlgBar::OnEnChangeEditLstBgYpos)
	ON_EN_CHANGE(IDC_EDIT_LST_BG_WIDTH, &CListBoxDlgBar::OnEnChangeEditLstBgWidth)
	ON_EN_CHANGE(IDC_EDIT_LST_BG_HEIGHT, &CListBoxDlgBar::OnEnChangeEditLstBgHeight)
	ON_EN_CHANGE(IDC_EDIT_LST_COL_INTERVAL, &CListBoxDlgBar::OnEnChangeEditLstColInterval)
	ON_EN_CHANGE(IDC_EDIT_LST_ROW_INTERVAL, &CListBoxDlgBar::OnEnChangeEditLstRowInterval)
	ON_EN_CHANGE(IDC_EDIT_LST_SCROLL_PATH, &CListBoxDlgBar::OnEnChangeEditLstScrollPath)
	ON_EN_CHANGE(IDC_EDIT_LST_LISTITEM_PATH, &CListBoxDlgBar::OnEnChangeEditLstListitemPath)
	ON_BN_CLICKED(IDC_BUTTON_SET_SCROLL, &CListBoxDlgBar::OnBnClickedButtonSetScroll)
	ON_BN_CLICKED(IDC_BUTTON_SET_LISTITEM, &CListBoxDlgBar::OnBnClickedButtonSetListitem)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_SET_SCROLL, &CListBoxDlgBar::OnUpdateCmdUI)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_SET_LISTITEM, &CListBoxDlgBar::OnUpdateCmdUI)
	ON_EN_CHANGE(IDC_EDIT_LISTITEM_COUNT, &CListBoxDlgBar::OnEnChangeEditListitemCount)
	ON_MESSAGE(WM_SELCHANGE_TEXTURELIST, &CListBoxDlgBar::OnSelchangeTexturelist)
	ON_MESSAGE(WM_CHANGE_DRAGBOX_INFO, &CListBoxDlgBar::OnChangeDragboxInfo)
	ON_EN_CHANGE(IDC_EDIT_LST_START_XPOS, &CListBoxDlgBar::OnEnChangeEditLstStartXpos)
	ON_EN_CHANGE(IDC_EDIT_LST_START_YPOS, &CListBoxDlgBar::OnEnChangeEditLstStartYpos)
END_MESSAGE_MAP()


// CListBoxDlgBar �޽��� ó�����Դϴ�.


void CListBoxDlgBar::OnEnChangeEditLstShowTexpath()
{
	if(m_bChange)
	{
		UpdateData(true);
		//	������ �� ���ϰ�θ� ���⿡�ٰ� ����־��ֱ�.
		//	������ ���ϰ�θ� ������ ��ȯ �� ��� ������ ����־��ش�.
		m_strBGTexPath = GetEditTextStdString(m_BGTexPathEdit);
	}

	SetListBoxProperty();
}


void CListBoxDlgBar::OnEnChangeEditLstBgXpos()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptBGPos.x = GetEditText(m_BGXPosEdit);
		::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBGPos.x, (LPARAM)DRAGBOX_LEFT);
	}
	SetListBoxProperty();
}


void CListBoxDlgBar::OnEnChangeEditLstBgYpos()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptBGPos.y = GetEditText(m_BGYPosEdit);
		::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBGPos.y, (LPARAM)DRAGBOX_TOP);
	}
	SetListBoxProperty();
}


void CListBoxDlgBar::OnEnChangeEditLstBgWidth()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptBGSize.x = GetEditText(m_BGWidthEdit);

		if(m_strBGTexPath != "")
			::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBGSize.x, (LPARAM)DRAGBOX_WIDTH);
	}
	SetListBoxProperty();
}


void CListBoxDlgBar::OnEnChangeEditLstBgHeight()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptBGSize.y = GetEditText(m_BGHeightEdit);

		std::cout << "Height�����Դ�" << std::endl;

		if(m_strBGTexPath != "")
			::SendMessage(m_PictureWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptBGSize.y, (LPARAM)DRAGBOX_HEIGHT);
	}
	SetListBoxProperty();
}


void CListBoxDlgBar::OnEnChangeEditLstColInterval()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptListInterval.x = GetEditText(m_ListIntervalColEdit);
	}
	SetListBoxProperty();
}


void CListBoxDlgBar::OnEnChangeEditLstRowInterval()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptListInterval.y = GetEditText(m_ListIntervalRowEdit);
	}
	SetListBoxProperty();
}


void CListBoxDlgBar::OnEnChangeEditLstScrollPath()
{
	if(m_bChange)
	{
		UpdateData(true);

		m_strScrollPath = GetEditTextStdString(m_ScrollPathEdit);

		if(m_ListBoxBaseScroll != NULL)
		{
			CreateAsset();
			m_ListBoxBase->SetScrollBase(m_ListBoxBaseScroll);
			m_ListBoxBase->SetScrollPath(m_strScrollPath);
		}
	}
}


void CListBoxDlgBar::OnEnChangeEditLstListitemPath()
{
	if(m_bChange)
	{
		UpdateData(true);

		m_strListItemPath = GetEditTextStdString(m_ListItemPathEdit);

		if(m_ListBoxBaseItem != NULL)
		{
			CreateAsset();
			m_ListBoxBase->SetListItemBase(m_ListBoxBaseItem);
			m_ListBoxBase->SetListItemPath(m_strListItemPath);
		}
	}
}


void CListBoxDlgBar::OnBnClickedButtonSetScroll()
{
	//	��ũ���� �����ϴ� ���ϴ��̾˷α׸� �����ش�.
	if(m_bChange)
	{
		UpdateData(true);

		CString strPathName;
		std::string strRai = "rai";
		CA2T wt(strRai.c_str());
		CFileDialog FileDlg(TRUE, wt, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, this);

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
			//	���� ���ִ� Asset���� Ŭ�����ϴ� �Լ�.
			strPathName = FileDlg.GetPathName();
			CT2CA pszConvertedAnsiString (strPathName);
			std::string _Path(pszConvertedAnsiString);
			//	�θ��ڵ��� ����� ���������� �׻� üũ�� ��.
			newBase = LoadBaseInterface(_Path, m_ParenthWnd);
		}
		else
		{
			return;
		}

		if(newBase->GetAssetType() != ASSET_SCROLL)
		{
			MessageBox(L"������ ����� ���õ��� �ʾҽ��ϴ�.\n��ũ�� ������ �������ֽʽÿ�.");
			SAFE_DELETE(newBase);
		}
		else
		{
			//	�н��� �����ϸ鼭 ��ũ���� newbase �����͸� ����Ʈ�ڽ� ���ο� �־��ֵ��� ����.
			SetListBoxProperty();
			POINT ptZero;
			ptZero.x = 0;
			ptZero.y = 0;
			newBase->SetPosition(ptZero);
			m_ListBoxBaseScroll = (CScrollBase*)newBase;
			CT2CA pszConvertedText (strPathName);
			std::string _strConvertText(pszConvertedText);

			m_strScrollPath = _strConvertText;
			m_ScrollPathEdit->SetWindowText(strPathName);
		}
	}

	SetListBoxProperty();
}


void CListBoxDlgBar::OnBnClickedButtonSetListitem()
{
	//	����Ʈ�������� �����ϴ� ���ϴ��̾˷α׸� �����ش�.
	if(m_bChange)
	{
		UpdateData(true);

		CString strPathName;
		std::string strRai = "rai";
		CA2T wt(strRai.c_str());
		CFileDialog FileDlg(TRUE, wt, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, this);

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
			//	���� ���ִ� Asset���� Ŭ�����ϴ� �Լ�.
			strPathName = FileDlg.GetPathName();
			CT2CA pszConvertedAnsiString (strPathName);
			std::string _Path(pszConvertedAnsiString);
			//	�θ��ڵ��� ����� ���������� �׻� üũ�� ��.
			newBase = LoadBaseInterface(_Path, m_ParenthWnd);
		}
		else
		{
			return;
		}

		if(newBase->GetAssetType() != ASSET_LISTITEM)
		{
			MessageBox(L"������ ����� ���õ��� �ʾҽ��ϴ�.\n����Ʈ������ ������ �������ֽʽÿ�.");
			SAFE_DELETE(newBase);
		}
		else
		{
			//	���⿡�� �����ؼ� �ڵ����� �������ְ� ����.
			SetListBoxProperty();
			//	���⿡�� �н��� �����ϸ鼭 newbase�� ����Ʈ�ڽ� ���ο� �־�����.

			m_ListBoxBaseItem = (CListItemBase*)newBase;
			
			CT2CA pszConvertedText (strPathName);
			std::string _strConvertText(pszConvertedText);

			m_strListItemPath = _strConvertText;
			m_ListItemPathEdit->SetWindowText(strPathName);
		}
	}
}

void CListBoxDlgBar::Initialize()
{
	UpdateData(true);

	m_bChange = false;
	POINT ptZero;
	ptZero.x = 0;
	ptZero.y = 0;

	m_strBGTexPath = "";
	m_ptBGPos = ptZero;
	m_ptBGSize = ptZero;

	m_ptListInterval = ptZero;

	m_strScrollPath = "";
	m_strListItemPath = "";
	m_ptListitemStartPos = ptZero;

	SAFE_DELETE(m_ListBoxBaseItem);
	SAFE_DELETE(m_ListBoxBaseScroll);

	m_BGTexPathEdit->SetWindowText(L"");
	m_BGXPosEdit->SetWindowText(L"0");
	m_BGYPosEdit->SetWindowText(L"0");
	m_BGWidthEdit->SetWindowText(L"0");
	m_BGHeightEdit->SetWindowText(L"0");

	m_ListIntervalColEdit->SetWindowText(L"0");
	m_ListIntervalRowEdit->SetWindowText(L"0");
	m_ListItemCountEdit->SetWindowText(L"0");

	m_ListItemStartXPosEdit->SetWindowText(L"0");
	m_ListItemStartYPosEdit->SetWindowText(L"0");

	m_ScrollPathEdit->SetWindowText(L"");
	m_ListItemPathEdit->SetWindowText(L"");

	m_bChange = true;
}

void CListBoxDlgBar::SetParenthWnd(HWND _hWnd)
{
	m_ParenthWnd = _hWnd;
}

void CListBoxDlgBar::SetPicturehWnd( HWND _hWnd )
{
	m_PictureWnd = _hWnd;
}

CBaseInterface* CListBoxDlgBar::LoadBaseInterface( std::string FilePath, HWND _hWnd )
{
	std::ifstream	ifLoad(FilePath.c_str());
	std::string		strInput;	

	CBaseInterface* newAsset = NULL;

	while(!ifLoad.eof())
	{
		//	������ �Է��� ���� InsertAsset�� SetFilePath�� ��ġ
		//	�ε� ���� ������
		std::getline(ifLoad, strInput, '\n');
		if(!strcmp(strInput.c_str(), "[END]\t"))
		{
			break;
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
			newAsset->SetParenthWnd(_hWnd);
		}
	}

	//	newAsset�� ���� ��쿡�� ����� ������ ���� ���� ���̴�
	ifLoad.close();
	return newAsset;
}


void CListBoxDlgBar::OnUpdateCmdUI(CCmdUI* pCmdUI)
{
	//	DlgBar�� ��ư�� ������Ʈ�ϱ� ���� �۾�
	pCmdUI->Enable(TRUE);
}

void CListBoxDlgBar::CreateAsset()
{
	std::cout << "��¥ �����" << std::endl;
	if(m_ListBoxBase == NULL)
	{
		std::cout << "�����Ҵ��Ѵ�" << std::endl;
		m_ListBoxBase = new CListBoxBase;

		std::cout << "�Ҵ��ߴ�" << std::endl;
		POINT ptZeroPos;
		ptZeroPos.x = 0;
		ptZeroPos.y = 0;

		//	������ �������� �����Ѵ�
		m_ListBoxBase->SetPosition(ptZeroPos);

		std::cout << "������ �����ߴ�" << std::endl;
		m_ListBoxBase->SetAssetType(ASSET_LISTBOX);
		std::cout << "����Ÿ�� �����ߴ�" << std::endl;
		m_ListBoxBase->SetFilePath(m_strBGTexPath);
		std::cout << "���� �н� �����ߴ�" << std::endl;
		std::cout << "�з�Ʈ �����Ѵ�" << std::endl;
		m_ListBoxBase->SetParenthWnd(m_ParenthWnd);

		//	������� ������ Map�� �����ϴ� ����
		CObjMgr::GetInstance()->InsertAssetBase(m_ListBoxBase);
	}
}

void CListBoxDlgBar::SetListBoxProperty()
{
	if(m_ptBGSize.x != 0 && m_ptBGSize.y != 0)
	{
		std::cout << "���¸���� " << std::endl;
		CreateAsset();

		std::cout << "���� �������" << std::endl;

		RECT ptSize;
		ptSize.left = 0;
		ptSize.top = 0;
		ptSize.right = m_ptBGSize.x;
		ptSize.bottom = m_ptBGSize.y;
		//	������ ��� �������� �������ش�

		std::cout << "����Ʈ�ڽ� ������Ƽ �����Ѵ�" << std::endl;

		m_ListBoxBase->SetListItemCount(m_iListItemCount);
		m_ListBoxBase->SetListBoxProperty(m_iListItemCount, m_strBGTexPath, m_ptBGPos, ptSize, m_ptListInterval, m_ptListitemStartPos);;
		m_ListBoxBase->Run();
	}
}


void CListBoxDlgBar::OnEnChangeEditListitemCount()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_iListItemCount = GetEditText(m_ListItemCountEdit);
	}

	SetListBoxProperty();
}

void CListBoxDlgBar::RefreshDlgBar(CListBoxBase* _LoadBase, POINT& _BgTex)
{
	m_bChange = false;

	SetMemberListBoxBase(_LoadBase);

	m_ptBGPos.x = _LoadBase->GetBackgroundPos().x;
	EditTextSetting(m_ptBGPos.x, m_BGXPosEdit);
	m_ptBGPos.y = _LoadBase->GetBackgroundPos().y;
	EditTextSetting(m_ptBGPos.y, m_BGYPosEdit);
	_BgTex = m_ptBGPos;

	m_ptListInterval.x  = _LoadBase->GetListBoxInterval().x;
	EditTextSetting(m_ptListInterval.x, m_ListIntervalColEdit);
	m_ptListInterval.y = _LoadBase->GetListBoxInterval().y;
	EditTextSetting(m_ptListInterval.y, m_ListIntervalRowEdit);

	m_ptListitemStartPos.x = _LoadBase->GetListItemStartPos().x;
	EditTextSetting(m_ptListitemStartPos.x, m_ListItemStartXPosEdit);
	m_ptListitemStartPos.y = _LoadBase->GetListItemStartPos().y;
	EditTextSetting(m_ptListitemStartPos.y, m_ListItemStartYPosEdit);

	m_iListItemCount = _LoadBase->GetListBoxItemCount();
	EditTextSetting(m_iListItemCount, m_ListItemCountEdit);

	m_strScrollPath = _LoadBase->GetScrollPath();
	CString strScrollPath(m_strScrollPath.c_str());
	m_ScrollPathEdit->SetWindowText(strScrollPath);

	m_strListItemPath = _LoadBase->GetListItemPath();
	CString strListItemPath(m_strListItemPath.c_str());
	m_ListItemPathEdit->SetWindowText(strListItemPath);


	std::string Path = _LoadBase->GetFilePath();
	if(Path != "")
	{
		m_strBGTexPath = Path;
		CString TexPath(m_strBGTexPath.c_str());
		m_BGTexPathEdit->SetWindowText(TexPath);
		m_ptBGSize.x = _LoadBase->GetBackgroundSize().right;
		EditTextSetting(m_ptBGSize.x, m_BGWidthEdit);
		m_ptBGSize.y = _LoadBase->GetBackgroundSize().bottom;
		EditTextSetting(m_ptBGSize.y, m_BGHeightEdit);
	}
	else
	{
		m_ptBGSize.x = _LoadBase->GetListBoxAreaSize().right;
		EditTextSetting(m_ptBGSize.x, m_BGWidthEdit);
		m_ptBGSize.y = _LoadBase->GetListBoxAreaSize().bottom;
		EditTextSetting(m_ptBGSize.y, m_BGHeightEdit);
	}

	m_bChange = true;
}

void CListBoxDlgBar::SetMemberListBoxBase(CListBoxBase* _base)
{
	m_ListBoxBase = _base;
}


afx_msg LRESULT CListBoxDlgBar::OnSelchangeTexturelist(WPARAM wParam, LPARAM lParam)
{
	UpdateData(true);

	m_strBGTexPath = (CDBMgr::GetInstance()->GetPathMap()->find((int)wParam)->second);
	CA2T wt(m_strBGTexPath.c_str());
	m_BGTexPathEdit->SetWindowText(wt);

	if(m_ListBoxBase != NULL)
	{
		m_ListBoxBase->SetFilePath(m_strBGTexPath);
		
	}
	return 0;
}


afx_msg LRESULT CListBoxDlgBar::OnChangeDragboxInfo(WPARAM wParam, LPARAM lParam)
{
	//	�巡�� �ڽ� �������̼��� ����Ǿ��� ���.
	//	ũ��Ƽ�� ����ó�� ����.
	m_bChange = false;

	UpdateData(true);

	switch((int)lParam)
	{
	case DRAGBOX_LEFT:
		m_ptBGPos.x = (int)wParam;
		SetEditWndText(m_BGXPosEdit, m_ptBGPos.x);
		break;
	case DRAGBOX_TOP:
		m_ptBGPos.y = (int)wParam;
		SetEditWndText(m_BGYPosEdit, m_ptBGPos.y);
		break;
	case DRAGBOX_WIDTH:
		m_ptBGSize.x = (int)wParam;
		SetEditWndText(m_BGWidthEdit, m_ptBGSize.x);
		break;
	case DRAGBOX_HEIGHT:
		m_ptBGSize.y = (int)wParam;
		SetEditWndText(m_BGHeightEdit, m_ptBGSize.y);
		break;
	}

	m_bChange = true;
	return 0;
}


void CListBoxDlgBar::OnEnChangeEditLstStartXpos()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptListitemStartPos.x = GetEditText(m_ListItemStartXPosEdit);
	}
	SetListBoxProperty();
}


void CListBoxDlgBar::OnEnChangeEditLstStartYpos()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptListitemStartPos.y = GetEditText(m_ListItemStartYPosEdit);
	}
	SetListBoxProperty();
}
