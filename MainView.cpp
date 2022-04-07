
// MainView.cpp : CMainView 클래스의 구현
//

#include "stdafx.h"
#include "UITool.h"
#include "MainView.h"

#include "ObjMgr.h"
#include "BasicAsset.h"
#include "UIFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainView

CMainView::CMainView()
{
	m_bClicked = false;
	m_bFrameClicked = false;
	m_iMouseFlag = MOUSE_INIT;
	RECT_INITIALIZE(m_tClientRt);
	ZeroMemory(&m_PrevMousePos, sizeof(POINT));
}

CMainView::~CMainView()
{
}


BEGIN_MESSAGE_MAP(CMainView, CWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()



// CMainView 메시지 처리기

BOOL CMainView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

int CMainView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetTimer(DRAW_SCENE_TIMER, 10, 0);

	return 0;
}


void CMainView::RenderObjMgrAsset()
{
	HDC			hMemDC;
	HBITMAP		hMembit;
	HBITMAP		hOldMembit;

	CDC* pDC = this->GetDC();
	GetClientRect(&m_tClientRt);
	hMemDC= CreateCompatibleDC(HDC(*pDC));
	hMembit = MakeDIBSection(*pDC, m_tClientRt.right, m_tClientRt.bottom);
	hOldMembit = (HBITMAP)SelectObject(hMemDC, hMembit);

	PatBlt(hMemDC, 0, 0, m_tClientRt.right, m_tClientRt.bottom, WHITENESS);
	CObjMgr::GetInstance()->Render(SCENE_EDITOR_TARGET, hMemDC);

	BitBlt(HDC(*pDC), 0, 0, m_tClientRt.right, m_tClientRt.bottom,
		hMemDC, 0, 0, SRCCOPY);	
	SelectObject(hMemDC, hOldMembit);
	DeleteDC(hMemDC);
	DeleteObject( hMembit );
	this->ReleaseDC(pDC);
}

void CMainView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CObjMgr::GetInstance()->Run(SCENE_EDITOR_TARGET);
	//	뷰 내에서 마우스 클릭이 일어난 경우, 에셋과 충돌했는지를 확인한다
	if(m_bClicked == false)
	{
		POINT tMousePos;
		GetCursorPos(&tMousePos);
		ScreenToClient(&tMousePos);
		if(CObjMgr::GetInstance()->BasicAssetCollisionCheck(tMousePos))
		{
			if(CObjMgr::GetInstance()->GetFocusBasicAsset() != NULL)
			CObjMgr::GetInstance()->GetFocusBasicAsset()->SetState(STATE_PRESSED);
			m_bClicked = true;
			m_PrevMousePos = tMousePos;
			::SendMessage(m_ParenthWnd, WM_BASIC_ASSET_FOCUSED, NULL, NULL);
		}

		if(CObjMgr::GetInstance()->UIFrameCollisionCheck(tMousePos))
		{
			if(CObjMgr::GetInstance()->GetFocusUIFrame() != NULL)
			CObjMgr::GetInstance()->GetFocusUIFrame()->SetState(STATE_PRESSED);
			m_bFrameClicked = true;
			m_PrevMousePos = tMousePos;
			//	이 메세지에 대한 처리는 Pos값 변화와scale값 변화에 있다.
			::SendMessage(m_ParenthWnd, WM_UIFRAME_FOCUSED, NULL, NULL);
		}
	}
	CWnd::OnLButtonDown(nFlags, point);
}


void CMainView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_bClicked)
	{
		if(CObjMgr::GetInstance()->GetFocusBasicAsset() != NULL)
			CObjMgr::GetInstance()->GetFocusBasicAsset()->SetState(STATE_NORMAL);
	}

	if(m_bFrameClicked)
	{
		if(CObjMgr::GetInstance()->GetFocusUIFrame() != NULL)
			CObjMgr::GetInstance()->GetFocusUIFrame()->SetState(STATE_NORMAL);
	}

	m_bClicked = false;
	m_bFrameClicked = false;

	CWnd::OnLButtonUp(nFlags, point);
}

void CMainView::OnMouseMove(UINT nFlags, CPoint point)
{
	MouseStateCheck();

	//	뷰 크기 측정
	/*RECT rtSize;
	GetClientRect(&rtSize);
	std::cout << rtSize.left << "," << rtSize.top << "," << rtSize.right << ","
		<< rtSize.bottom << std::endl;*/

	POINT tPosNum;
	POINT tMousePos;

	tPosNum.x = 0;
	tPosNum.y = 0;

	if(m_bClicked || m_bFrameClicked)
	{
		GetCursorPos(&tMousePos);
		ScreenToClient(&tMousePos);
		tPosNum.x = m_PrevMousePos.x - tMousePos.x;
		tPosNum.y = m_PrevMousePos.y - tMousePos.y;
	}

	if(m_bClicked)
	{	
		POINT ptMovingPos;

		CBasicAsset* FocusAsset = CObjMgr::GetInstance()->GetFocusBasicAsset();

		if(FocusAsset != NULL)
		{
			ptMovingPos.x = FocusAsset->GetPosition().x;
			ptMovingPos.y = FocusAsset->GetPosition().y;

			ptMovingPos.x -= tPosNum.x;
			ptMovingPos.y -= tPosNum.y;

			CObjMgr::GetInstance()->GetFocusBasicAsset()->SubsAssetBasePos(tPosNum, ptMovingPos);

			m_PrevMousePos = tMousePos;
			::SendMessage(m_ParenthWnd, WM_BASIC_ASSET_FOCUSED, NULL, NULL);
		}
	}
	
	if(m_bFrameClicked)
	{
		//	히트박스는 제대로 존재하는데 좌우로만 움직이는 현상.
		POINT ptMovingPos;

		CUIFrame* FocusFrame = CObjMgr::GetInstance()->GetFocusUIFrame();

		if(FocusFrame != NULL)
		{
			ptMovingPos.x = FocusFrame->GetPosition().x;
			ptMovingPos.y = FocusFrame->GetPosition().y;

			ptMovingPos.x -= tPosNum.x;
			ptMovingPos.y -= tPosNum.y;

			CObjMgr::GetInstance()->GetFocusUIFrame()->SubstractPosition(tPosNum);

			m_PrevMousePos = tMousePos;
			::SendMessage(m_ParenthWnd, WM_UIFRAME_FOCUSED, NULL, NULL);
		}
	}

	CWnd::OnMouseMove(nFlags, point);
}

void CMainView::OnMouseLeave()
{
	m_iMouseFlag = MOUSE_LEAVE;
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CWnd::OnMouseLeave();
}


void CMainView::OnMouseHover(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_iMouseFlag = MOUSE_HOVER;
	CWnd::OnMouseHover(nFlags, point);
}

void CMainView::MouseStateCheck()
{
	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(TRACKMOUSEEVENT);

	if(m_iFlag == 0)
	{
		tme.dwFlags = TME_LEAVE;
	}
	else
	{
		tme.dwFlags = TME_HOVER;
	}

	tme.hwndTrack = m_hWnd; //	현재 자신의 윈도우 핸들 지정
	tme.dwHoverTime = 10;	//	10 밀리세컨드로 지정

	_TrackMouseEvent( &tme );
}

void CMainView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	RenderObjMgrAsset();
}

void CMainView::OnDestroy()
{
	CWnd::OnDestroy();

	KillTimer(DRAW_SCENE_TIMER);
}


void CMainView::OnTimer(UINT_PTR nIDEvent)
{
	CWnd::OnTimer(nIDEvent);

	if(nIDEvent == DRAW_SCENE_TIMER)
	{
		RenderObjMgrAsset();
		CObjMgr::GetInstance()->Run(SCENE_EDITOR_TARGET);
	}
}
