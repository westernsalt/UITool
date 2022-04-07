// UIFrameList.cpp : 구현 파일입니다.
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
	//	제대로 스트링이 빠지지 않는 현상이 발견되서 앞에서부터 지워서 그런가 하고 뒤에서부터 지웠더니 됐다.
	//	아무래도 리스트박스이니 만큼, 앞의 항목을 제거하면 노드의 연결이 끊어져서 오류가 생기는 걸지도 모르겠다
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

//	메세지 처리

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
	//	더블클릭 된 경우 해당 포커스된 씬을 리스트에 넣어준다
	/*if(CObjMgr::GetInstance()->GetTargetMapCount(SCENE_EDITOR_TARGET) != 0)
	{
	if(AfxMessageBox(TEXT("편집 중인 Scene을 저장하시겠습니까?"), MB_OKCANCEL) == IDOK)
	{
	::SendMessage(m_ParenthWnd, WM_SAVE_SCENE_MSG, NULL, NULL);
	}

	CObjMgr::GetInstance()->ClearTargetMap(SCENE_EDITOR_TARGET);
	CObjMgr::GetInstance()->ClearFrameMap();
	}*/

	//	상대 경로를 그대로 가져와서 읽으니까 생기는 문제가 있었다.
	if(m_pFocusScene != NULL)
	{
		CUIFrame*	CopyScene = new CUIFrame(m_pFocusScene);
		CopyScene->MakeOriginalFilePath();
		CopyScene->SetMainFrmhWnd(m_MainFrmhWnd);
		CObjMgr::GetInstance()->InsertUIFrame(CopyScene);
	}
	//::SendMessage(m_ParenthWnd, WM_DRAW_BASICASSET, NULL, NULL);
}