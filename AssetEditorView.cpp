// AssetEditorView.cpp : ���� �����Դϴ�.
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

// CAssetEditorView �޽��� ó�����Դϴ�.

void CAssetEditorView::OnLButtonDown(UINT nFlags, CPoint point)
{
	POINT tMousePos;
	GetCursorPos(&tMousePos);
	ScreenToClient(&tMousePos);
	if(CObjMgr::GetInstance()->AssetBaseCollisionCheck(tMousePos))
	{
		CObjMgr::GetInstance()->GetFocusAssetBase()->SetState(STATE_PRESSED);
		//	���°� �ٲ���µ� �װ��� �ٷ� ������� �ʴ´�.
		//	�׷��� �ش� ���°� �ٲ���̸� Run�� ���ֱ�� �ߴ��� �ذ��.
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

	// ������ ������ üũ
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

	tme.hwndTrack = m_hWnd; //	���� �ڽ��� ������ �ڵ� ����
	tme.dwHoverTime = 10;	//	10 �и�������� ����

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
		//	�ݵ�� m_hMemdc�� ���� �� ���Ŀ� ȣ��Ǿ�� �Ѵ�
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

		// EditorWnd�� ���� �۾��ؾ��ϴ� �κ��̴�
		// EditorWnd�� �̹����� �������ֱ� ������.
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

		// CxImage�׸���(Cimage�ڵ�)
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	// �׸��� �޽����� ���ؼ��� CWnd::OnPaint()��(��) ȣ������ ���ʽÿ�.
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
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
