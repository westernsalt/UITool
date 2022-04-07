// UIFrameList.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "UITool.h"
#include "UIFrameFileList.h"
#include "DBMgr.h"
#include "UIFrame.h"
#include "ObjMgr.h"


// CUIFrameFileList

IMPLEMENT_DYNAMIC(CUIFrameFileList, CListBox)

CUIFrameFileList::CUIFrameFileList()
{

}

CUIFrameFileList::~CUIFrameFileList()
{
}


BEGIN_MESSAGE_MAP(CUIFrameFileList, CListBox)
	ON_WM_LBUTTONDOWN()
	ON_CONTROL_REFLECT(LBN_SELCHANGE, &CUIFrameFileList::OnLbnSelchange)
	ON_CONTROL_REFLECT(LBN_DBLCLK, &CUIFrameFileList::OnLbnDblclk)
END_MESSAGE_MAP()

void CUIFrameFileList::LoadUIFrameList(CString _path)
{
	//	����� ��Ʈ���� ������ �ʴ� ������ �߰ߵǼ� �տ������� ������ �׷��� �ϰ� �ڿ������� �������� �ƴ�.
	//	�ƹ����� ����Ʈ�ڽ��̴� ��ŭ, ���� �׸��� �����ϸ� ����� ������ �������� ������ ����� ������ �𸣰ڴ�
	for(int i = GetCount(); i >= 0; i--)
	{
		DeleteString(i);
	}

	CDBMgr::GetInstance()->ClearFrameMap();

	CString strInputPath(_path);

	TCHAR szLoadingPath[MAX_PATH] = L"";
	_tcscpy_s(szLoadingPath, _countof(szLoadingPath), strInputPath.GetBuffer());
	_tcscat_s(szLoadingPath, L"\\*.rsi");

	CFileFind finder;
	int iIndex;
	bool bWorking = finder.FindFile((LPCTSTR)szLoadingPath);

	while(bWorking)
	{
		bWorking = finder.FindNextFile();
		if(!finder.IsDots() && !finder.IsDirectory())
		{
			iIndex = GetCount();
			CDBMgr::GetInstance()->LoadUIFrame(iIndex, finder.GetFilePath(), m_MainFrmhWnd);
		}

		AddString(finder.GetFileName());
	}
}

//	�޼��� ó��

void CUIFrameFileList::OnLButtonDown(UINT nFlags, CPoint point)
{
	CListBox::OnLButtonDown(nFlags, point);
}

void CUIFrameFileList::OnLbnSelchange()
{
	m_iFocusIndex = GetCurSel();
	m_pFocusScene = CDBMgr::GetInstance()->GetBasicScene(m_iFocusIndex);
}


void CUIFrameFileList::OnLbnDblclk()
{
	//	����Ŭ�� �� ��� �ش� ��Ŀ���� ���� ����Ʈ�� �־��ش�
	/*if(CObjMgr::GetInstance()->GetTargetMapCount(SCENE_EDITOR_TARGET) != 0)
	{
	if(AfxMessageBox(TEXT("���� ���� Scene�� �����Ͻðڽ��ϱ�?"), MB_OKCANCEL) == IDOK)
	{
	::SendMessage(m_ParenthWnd, WM_SAVE_SCENE_MSG, NULL, NULL);
	}

	CObjMgr::GetInstance()->ClearTargetMap(SCENE_EDITOR_TARGET);
	CObjMgr::GetInstance()->ClearFrameMap();
	}*/

	//	��� ��θ� �״�� �����ͼ� �����ϱ� ����� ������ �־���.
	if(m_pFocusScene != NULL)
	{
		CUIFrame*	CopyScene = new CUIFrame(m_pFocusScene);
		CopyScene->MakeOriginalFilePath();
		CopyScene->SetMainFrmhWnd(m_MainFrmhWnd);
		CObjMgr::GetInstance()->InsertUIFrame(CopyScene);
	}
	//::SendMessage(m_ParenthWnd, WM_DRAW_BASICASSET, NULL, NULL);
}