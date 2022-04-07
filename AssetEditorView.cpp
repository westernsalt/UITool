// AssetEditorView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "UITool.h"
#include "AssetEditorView.h"
#include "ObjMgr.h"
#include "AssetBase.h"

// CAssetEditorView

IMPLEMENT_DYNAMIC(CAssetEditorView, CWnd)

CAssetEditorView::CAssetEditorView()
{
	m_bDragBox = false;
	m_bClicked = false;
	m_iFlag = -1;
}

CAssetEditorView::~CAssetEditorView()
{
	
}


BEGIN_MESSAGE_MAP(CAssetEditorView, CWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEHOVER()
	ON_MESSAGE(WM_DRAGBOX_SELECT, &CAssetEditorView::OnDragboxSelect)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CAssetEditorView 메시지 처리기입니다.

void CAssetEditorView::OnLButtonDown(UINT nFlags, CPoint point)
{
	POINT tMousePos;
	GetCursorPos(&tMousePos);
	ScreenToClient(&tMousePos);
	if(CObjMgr::GetInstance()->AssetBaseCollisionCheck(tMousePos))
	{
		CObjMgr::GetInstance()->GetFocusAssetBase()->SetState(STATE_PRESSED);
		//	상태가 바뀌었는데 그것이 바로 적용되지 않는다.
		//	그래서 해당 상태가 바뀐아이만 Run을 해주기로 했더니 해결됨.
		CObjMgr::GetInstance()->GetFocusAssetBase()->Run();
		m_bClicked = true;
		m_PrevMousePos = tMousePos;
		OnPaint();
		::SendMessage(m_AssetEditorhWnd, WM_ASSET_BASE_FOCUSED, NULL, NULL);
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void CAssetEditorView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_bClicked)
	{
		CObjMgr::GetInstance()->GetFocusAssetBase()->SetState(STATE_NORMAL);
		OnPaint();
	}

	m_bClicked = false;
	
	if(m_bDragBox)
	{
		m_bDragBox = false;

		HDC			hMemDC;
		HBITMAP		hMembit;
		HBITMAP		hOldMembit;

		CDC* pDC = this->GetDC();
		GetClientRect(&m_tClientRt);
		hMemDC= CreateCompatibleDC(HDC(*pDC));
		hMembit = MakeDIBSection(*pDC, m_tClientRt.right, m_tClientRt.bottom);
		hOldMembit = (HBITMAP)SelectObject(hMemDC, hMembit);

		PatBlt(hMemDC, 0, 0, m_tClientRt.right, m_tClientRt.bottom, WHITENESS);
		CObjMgr::GetInstance()->Render(ASSET_EDITOR_TARGET, hMemDC);

		BitBlt(HDC(*pDC), 0, 0, m_tClientRt.right, m_tClientRt.bottom,
			hMemDC, 0, 0, SRCCOPY);	
		SelectObject(hMemDC, hOldMembit);
		DeleteDC(hMemDC);
		DeleteObject( hMembit );
		this->ReleaseDC(pDC);

		::SendMessage(m_AssetEditorhWnd, WM_CREATE_ASSET, NULL, NULL);
	}

	CWnd::OnLButtonUp(nFlags, point);
}


void CAssetEditorView::OnMouseLeave()
{
	m_bClicked = false;
	m_iFlag = MOUSE_LEAVE;

	if(m_bDragBox == true)
	{
		m_bDragBox = false;

		HDC			hMemDC;
		HBITMAP		hMembit;
		HBITMAP		hOldMembit;

		CDC* pDC = this->GetDC();
		GetClientRect(&m_tClientRt);
		hMemDC= CreateCompatibleDC(HDC(*pDC));
		hMembit = MakeDIBSection(*pDC, m_tClientRt.right, m_tClientRt.bottom);
		hOldMembit = (HBITMAP)SelectObject(hMemDC, hMembit);

		PatBlt(hMemDC, 0, 0, m_tClientRt.right, m_tClientRt.bottom, WHITENESS);
		CObjMgr::GetInstance()->Render(ASSET_EDITOR_TARGET, hMemDC);

		BitBlt(HDC(*pDC), 0, 0, m_tClientRt.right, m_tClientRt.bottom,
			hMemDC, 0, 0, SRCCOPY);	
		SelectObject(hMemDC, hOldMembit);
		DeleteDC(hMemDC);
		DeleteObject( hMembit );
		this->ReleaseDC(pDC);
	}

	CWnd::OnMouseLeave();
}


void CAssetEditorView::OnMouseMove(UINT nFlags, CPoint point)
{
	MouseStateCheck();

	// 윈도우 사이즈 체크
	/*RECT rtSize;
	GetClientRect(&rtSize);
	std::cout << rtSize.left << "," << rtSize.top << "," << rtSize.right << ","
		<< rtSize.bottom << std::endl;*/

	if(m_bClicked && !m_bDragBox)
	{	
		POINT tPosNum;
		POINT tMousePos;

		tPosNum.x = 0;
		tPosNum.y = 0;

		GetCursorPos(&tMousePos);
		ScreenToClient(&tMousePos);
		tPosNum.x = m_PrevMousePos.x - tMousePos.x;
		tPosNum.y = m_PrevMousePos.y - tMousePos.y;

		CObjMgr::GetInstance()->GetFocusAssetBase()->SubstractPosition(tPosNum);
		
		m_PrevMousePos = tMousePos;
		::SendMessage(m_AssetEditorhWnd, WM_ASSET_BASE_FOCUSED, NULL, NULL);
	}

	if(m_bDragBox)
	{
		::SendMessage(m_AssetEditorhWnd, WM_DRAW_ALPHABLEND_DRAGBOX, NULL, NULL);
	}

	CWnd::OnMouseMove(nFlags, point);
}


void CAssetEditorView::OnMouseHover(UINT nFlags, CPoint point)
{
	m_iFlag = MOUSE_HOVER;

	if(m_bDragBox)
	{
		::SendMessage(m_AssetEditorhWnd, WM_DRAW_ALPHABLEND_DRAGBOX, NULL, NULL);
	}

	CWnd::OnMouseHover(nFlags, point);
}

void CAssetEditorView::MouseStateCheck()
{
	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(TRACKMOUSEEVENT);

	if(m_iFlag == 0)
		tme.dwFlags = TME_LEAVE;
	else
		tme.dwFlags = TME_HOVER;

	tme.hwndTrack = m_hWnd; //	현재 자신의 윈도우 핸들 지정
	tme.dwHoverTime = 10;	//	10 밀리세컨드로 지정

	_TrackMouseEvent( &tme );
}


afx_msg LRESULT CAssetEditorView::OnDragboxSelect(WPARAM wParam, LPARAM lParam)
{
	m_bDragBox = true;
	return 0;
}

void CAssetEditorView::DrawAlphaBlendDragBox(CxImage* _Texture, RECT _DragBox, POINT _Pos)
{
	MouseStateCheck();
	if(m_iFlag == MOUSE_HOVER)
	{
		//	반드시 m_hMemdc가 셋팅 된 이후에 호출되어야 한다
		POINT	tMousePos;
		GetCursorPos(&tMousePos);
		ScreenToClient(&tMousePos);

		RECT rtDraw = _DragBox;

		HDC			hMemDC;
		HBITMAP		hMembit;
		HBITMAP		hOldMembit;

		CDC* pDC = this->GetDC();
		GetClientRect(&m_tClientRt);
		hMemDC= CreateCompatibleDC(HDC(*pDC));
		hMembit = MakeDIBSection(*pDC, m_tClientRt.right, m_tClientRt.bottom);
		hOldMembit = (HBITMAP)SelectObject(hMemDC, hMembit);

		// EditorWnd와 병행 작업해야하는 부분이다
		// EditorWnd가 이미지를 세팅해주기 때문임.
		PatBlt(hMemDC, 0, 0, m_tClientRt.right, m_tClientRt.bottom, WHITENESS);
		CObjMgr::GetInstance()->Render(ASSET_EDITOR_TARGET, hMemDC);
		
		CxImage CropImage = (*_Texture);
		CropImage.Crop(_Pos.x, _Pos.y, _Pos.x + _DragBox.right, _Pos.y + _DragBox.bottom);
		CropImage.AlphaCreate();
		CropImage.AlphaSet(ALPHA_AMOUNT);
		CropImage.Draw(hMemDC, tMousePos.x, tMousePos.y, _DragBox.right, _DragBox.bottom);

		BitBlt(HDC(*pDC), 0, 0, m_tClientRt.right, m_tClientRt.bottom,
			hMemDC, 0, 0, SRCCOPY);	
		SelectObject(hMemDC, hOldMembit);
		DeleteDC(hMemDC);
		DeleteObject( hMembit );
		this->ReleaseDC(pDC);

		// CxImage그리기(Cimage코드)
		/*_Texture->AlphaBlend(m_hMemDC,
			tMousePos.x, tMousePos.y,
			_DragBox.right, _DragBox.bottom,
			_Pos.x, _Pos.y,
			_DragBox.right, _DragBox.bottom, ALPHA_AMOUNT);*/
	}
}

void CAssetEditorView::RenderObjMgrAsset()
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
	CObjMgr::GetInstance()->Render(ASSET_EDITOR_TARGET, hMemDC);	
	BitBlt(HDC(*pDC), 0, 0, m_tClientRt.right, m_tClientRt.bottom,
		hMemDC, 0, 0, SRCCOPY);	
	SelectObject(hMemDC, hOldMembit);
	DeleteDC(hMemDC);
	DeleteObject( hMembit );
	this->ReleaseDC(pDC);
}

void CAssetEditorView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CWnd::OnPaint()을(를) 호출하지 마십시오.
	RenderObjMgrAsset();
}

void CAssetEditorView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	int nCharIndex;
	nCharIndex = m_strInput.GetLength();

	if(nChar == VK_BACK)
	{
		m_strInput.Delete(nCharIndex - 1,1);
	}
	else
	{
		m_strInput.AppendChar(nChar);
	}

	::SendMessage(m_AssetEditorhWnd, WM_SEND_REQUEST_STRING, nChar, NULL);
	CWnd::OnChar(nChar, nRepCnt, nFlags);
}


void CAssetEditorView::OnTimer(UINT_PTR nIDEvent)
{
	static int iCount;
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(nIDEvent == DRAW_ASSET_TIMER)
	{
		RenderObjMgrAsset();
		CObjMgr::GetInstance()->Run(ASSET_EDITOR_TARGET);
	}

	CWnd::OnTimer(nIDEvent);
}


int CAssetEditorView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetTimer(DRAW_ASSET_TIMER, 10, 0);

	return 0;
}


void CAssetEditorView::OnDestroy()
{
	CWnd::OnDestroy();

	KillTimer(DRAW_ASSET_TIMER);
}
