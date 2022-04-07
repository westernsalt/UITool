// PictureView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "UITool.h"
#include "PictureView.h"

// CPictureView

IMPLEMENT_DYNAMIC(CPictureView, CWnd)

CPictureView::CPictureView()
{
	m_iFlag = MOUSE_INIT;
	m_pTexture = NULL;
	m_bMouseOver = false;
	m_bTextureDraw = false;
	ResetElement();
}

CPictureView::~CPictureView()
{
}


BEGIN_MESSAGE_MAP(CPictureView, CWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEHOVER()
	ON_WM_PAINT()
END_MESSAGE_MAP()

// CPictureView 메시지 처리기입니다.

void CPictureView::ResetElement()
{
	m_bClicked = false;
	RECT_INITIALIZE(m_rtDrag);
}

void CPictureView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_bTextureDraw &&  m_pTexture != NULL)
	{
		//	현재 선택된 이미지가 없어 프리뷰가 존재하지 않는 경우 동작할 필요가 없다
		CheckDragBoxCollision();

		//	드래그 박스가 클릭되지 않은 경우, 마우스의 좌표를 지정한다
		GetCursorPos(&m_ptMouse);
		ScreenToClient(&m_ptMouse);
		m_tStartPos.x = m_ptMouse.x;
		m_tStartPos.y = m_ptMouse.y;
		if(m_tStartPos.x > m_pTexture->GetWidth() || m_tStartPos.y > m_pTexture->GetHeight())
		{
			return;
		}
		OnPaint();

		m_bClicked = true;
	}

	CWnd::OnLButtonDown(nFlags, point);
}


void CPictureView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_bClicked)
	{
		//	드래그 박스가 클릭되지 않은 경우 드래그박스를 설정하도록 한다
		UpdateWindow();

		GetCursorPos(&m_ptMouse);
		ScreenToClient(&m_ptMouse);
		m_tEndPos.x = m_ptMouse.x;
		m_tEndPos.y = m_ptMouse.y;

		SetDragBox();
		AbortDragBox();

		if(m_rtDrag.right - m_rtDrag.left > 2 &&
			m_rtDrag.bottom - m_rtDrag.top > 2)
		{
			DrawRect();
		}
		//	드래그 박스는 지정하지 않지만 선택된 곳의 좌표는 제공한다.
		::SendMessage(m_ParenthWnd, WM_UPDATE_DRAGBOX_INFO, NULL, DRAGBOX_RECT);

		m_bClicked = false;
	}
	CWnd::OnLButtonUp(nFlags, point);
}


void CPictureView::OnMouseMove(UINT nFlags, CPoint point)
{
	MouseStateCheck();

	if(m_bClicked)
	{
		GetCursorPos(&m_ptMouse);
		ScreenToClient(&m_ptMouse);

		m_tEndPos.x = m_ptMouse.x;
		m_tEndPos.y = m_ptMouse.y;

		SetDragBox();
		AbortDragBox();

		DrawRect();
		DrawDragAreaTexture();
		OnPaint();
	}
	else if(m_bMouseOver)
	{
		// 자국 없애기
		OnPaint();
	}

	CWnd::OnMouseMove(nFlags, point);
}


void CPictureView::OnMouseLeave()
{
	m_iFlag = MOUSE_LEAVE;
	m_bMouseOver = false;

	if(m_bClicked)
	{
		RECT	rtWndSize;
		this->GetWindowRect(&rtWndSize);
		GetCursorPos(&m_ptMouse);
		ScreenToClient(&m_ptMouse);

		int iWidth = rtWndSize.right-rtWndSize.left;
		int iHeight = rtWndSize.bottom - rtWndSize.top;

		//	지정된 범위를 벗어난 경우 보정한다
		if(m_ptMouse.x < 0)
		{
			m_tEndPos.x = 0;
		}
		else if(m_ptMouse.x > iWidth)
		{
			m_tEndPos.x = iWidth;
		}

		if(m_ptMouse.y < 0)
		{
			m_tEndPos.y = 0;
		}
		else if(m_ptMouse.y > iHeight)
		{
			m_tEndPos.y = iHeight;
		}

		SetDragBox();
		AbortDragBox();
		DrawRect();
		m_bClicked = false;
	}

	CWnd::OnMouseLeave();
}


void CPictureView::OnMouseHover(UINT nFlags, CPoint point)
{
	m_iFlag = MOUSE_HOVER;
	m_bMouseOver = true;
	CWnd::OnMouseHover(nFlags, point);
}

void CPictureView::DrawRect(bool _bAllSel)
{
	//	드래그하는 라인을 그려준다.
	CDC* pDC = this->GetDC();
	HPEN CustomPen, OldPen;

	HBRUSH myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH oldBrush = (HBRUSH)SelectObject(HDC(*pDC), myBrush);

	CustomPen = CreatePen(PS_DOT, 1, BLACK_PEN);
	OldPen = (HPEN)SelectObject(HDC(*pDC),CustomPen);
	
	Rectangle(HDC(*pDC), m_rtDrag.left - DRAGRECT_INTERVAL, m_rtDrag.top - DRAGRECT_INTERVAL,
		m_rtDrag.right + DRAGRECT_INTERVAL, m_rtDrag.bottom + DRAGRECT_INTERVAL);

	UpdateWindow();
	SelectObject(HDC(*pDC), OldPen);
	SelectObject(HDC(*pDC), oldBrush);
	DeleteObject(myBrush);
	DeleteObject(CustomPen);
	this->ReleaseDC(pDC);
}

void CPictureView::SetDragBox()
{
	//	드래그박스 지정을 위한 좌표 보정
	if(m_tStartPos.x > m_tEndPos.x)
	{
		m_rtDrag.left = m_tEndPos.x;
		m_rtDrag.right = m_tStartPos.x;
	}
	else
	{
		m_rtDrag.left = m_tStartPos.x;
		m_rtDrag.right = m_tEndPos.x;
	}

	if(m_tStartPos.y > m_tEndPos.y)
	{
		m_rtDrag.bottom = m_tStartPos.y;
		m_rtDrag.top = m_tEndPos.y;
	}
	else
	{
		m_rtDrag.bottom = m_tEndPos.y;
		m_rtDrag.top = m_tStartPos.y;
	}
}

void CPictureView::SetDragBox( int _EditNum, eDragboxSet _set )
{
	//	PictureDlg에서 드래그박스를 설정한 경우
	if(m_pTexture)
	{
		switch(_set)
		{
		case DRAGBOX_LEFT:
			{
				if(_EditNum < 0)
				{
					_EditNum = 0;
				}
				int iWidth = m_rtDrag.right - m_rtDrag.left;
				m_rtDrag.left = _EditNum;
				m_rtDrag.right = m_rtDrag.left + iWidth;
			}
			break;
		case DRAGBOX_TOP:
			{
				if(_EditNum < 0)
				{
					_EditNum = 0;
				}
				int iHeight = m_rtDrag.bottom - m_rtDrag.top;
				m_rtDrag.top = _EditNum;
				m_rtDrag.bottom = m_rtDrag.top + iHeight;
			}
			break;
		case DRAGBOX_WIDTH:
			m_rtDrag.right = _EditNum + m_rtDrag.left;
			if(m_rtDrag.right > m_pTexture->GetWidth())
			{
				m_rtDrag.right = m_pTexture->GetWidth();
			}
			break;
		case DRAGBOX_HEIGHT:
			m_rtDrag.bottom = _EditNum + m_rtDrag.top;
			if(m_rtDrag.bottom > m_pTexture->GetHeight())
			{
				m_rtDrag.bottom = m_pTexture->GetHeight();
			}
			break;
		}

		OnPaint();
		DrawRect();
		DrawDragAreaTexture();
		m_bClicked = false;
	}
}

void CPictureView::SetAllSelDragBox(int _Width, int _Height)
{
	RECT tClientRT;
	GetClientRect(&tClientRT);

	double dResizeRate;

	m_rtDrag.left = 0;
	m_rtDrag.top = 0;
	m_rtDrag.right = _Width;
	m_rtDrag.bottom = _Height;

	//	텍스쳐의 원본 크기가 그대로 들어왔었기 때문에 문제가 생긴 것이다.
	if(tClientRT.bottom < _Height || tClientRT.right < _Width)
	{
		double dRate = 0.0;
		double dClientRatio = tClientRT.right / (double)tClientRT.bottom;
		double dResourceRatio = _Width / (double)_Height;

		dRate = dClientRatio / dResourceRatio;

		if(dResourceRatio > 1)
		{// 가로 길이가 모자란 경우(fRatio가 1보다 크다.)
			dResizeRate = _Width / (double)m_tClientRT.right;
		}
		else if(dResourceRatio <= 1)
		{// 세로 길이가 모자란 경우 (fRatio가 1보다 작다.)
			dResizeRate = _Height / (double)m_tClientRT.bottom;
		}

		m_rtDrag.top = (LONG)(m_rtDrag.top / dResizeRate);
		m_rtDrag.left = (LONG)(m_rtDrag.left / dResizeRate);
		m_rtDrag.right = (LONG)(m_rtDrag.right / dResizeRate);
		m_rtDrag.bottom = (LONG)(m_rtDrag.bottom / dResizeRate);
	}

	::SendMessage(m_ParenthWnd, WM_UPDATE_DRAGBOX_INFO, NULL, DRAGBOX_RECT);

	m_bClicked = false;
}

void CPictureView::CheckDragBoxCollision()
{
	if(m_rtDrag.right - m_rtDrag.left > MINIMUM_SIZE
		&& m_rtDrag.bottom - m_rtDrag.top > MINIMUM_SIZE)
	{
		//	드래그 박스가 클릭되었다면 새로 영역을 설정하는 것이 아니고 마우스 업이 일어날때까지
		//	해당 텍스쳐를 마우스 좌표를 중심으로 해서 그려야한다.
		POINT	ptCurMousePos;
		GetCursorPos(&ptCurMousePos);
		ScreenToClient(&ptCurMousePos);
	}
}

void CPictureView::MouseStateCheck()
{
	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(TRACKMOUSEEVENT);

	if(m_iFlag == MOUSE_HOVER)
		tme.dwFlags = TME_LEAVE;
	else
		tme.dwFlags = TME_HOVER;

	tme.hwndTrack = m_hWnd; //	현재 자신의 윈도우 핸들 지정
	tme.dwHoverTime = 10;	//	10 밀리세컨드로 타이밍 지정

	_TrackMouseEvent( &tme );
}

//	2018.08.16
//	EditDlgBar에서 진행되던 Draw파트를 이식해왔다

void CPictureView::DrawTexture()
{
	if(m_pTexture == NULL)
	{
		return;
	}
	//	여기의 ClientRt는 변동의 여지가 있는 RECT이기 때문에 m_tClientRT를 사용하지 않는다.
	RECT	tClientRT;
	RECT	tResourceRT;

	GetClientRect(&m_tClientRT);
	RECT_COPY(tClientRT, m_tClientRT);

	HDC			hTexWndMemDC;
	HBITMAP		hMembit;
	HBITMAP		hOldMembit;

	CDC* pDC = this->GetDC();

	hTexWndMemDC= CreateCompatibleDC(HDC(*pDC));
	hMembit = MakeDIBSection(*pDC, m_tClientRT.right, m_tClientRT.bottom);
	hOldMembit = (HBITMAP)SelectObject(hTexWndMemDC, hMembit);

	m_bTextureDraw = true;

	tResourceRT.left = 0;
	tResourceRT.top = 0;
	tResourceRT.right = m_pTexture->GetWidth();
	tResourceRT.bottom = m_pTexture->GetHeight();

	//	보간법을 적용해 준다.
	//	미적용시 그림파일 깨짐.
	::SetStretchBltMode(hTexWndMemDC, COLORONCOLOR);

	if(tClientRT.bottom < tResourceRT.bottom || tClientRT.right < tResourceRT.right)
	{
		double dRate = 0.0;
		double dClientRatio = tClientRT.right / (double)tClientRT.bottom;
		double dResourceRatio = tResourceRT.right / (double)tResourceRT.bottom;

		dRate = dClientRatio / dResourceRatio;

		if(dResourceRatio > 1)
		{// 가로 길이가 모자란 경우(fRatio가 1보다 크다.)
			double dBottom = (double)tClientRT.bottom;
			dBottom *= dRate;
			tClientRT.bottom = (LONG)dBottom;
		}
		else if(dResourceRatio <= 1)
		{// 세로 길이가 모자란 경우 (fRatio가 1보다 작다.)
			double dRight = tClientRT.right;
			dRight /= dRate;
			tClientRT.right = (LONG)dRight;
		}

		PatBlt(hTexWndMemDC, 0, 0, m_tClientRT.right, m_tClientRT.bottom, WHITENESS);
		//	텍스쳐 경계선
		Rectangle(hTexWndMemDC, 0, 0, tResourceRT.right, tResourceRT.bottom);
		m_pTexture->Draw(hTexWndMemDC, tClientRT, &tResourceRT);
	}
	else
	{
		POINT tPos;
		tPos.x = 0;
		tPos.y = 0;
		PatBlt(hTexWndMemDC, 0, 0, m_tClientRT.right, m_tClientRT.bottom, WHITENESS);
		//	텍스쳐 경계선
		Rectangle(hTexWndMemDC, 0, 0, tResourceRT.right, tResourceRT.bottom);
		// CxImage그리기
		m_pTexture->Draw(hTexWndMemDC, tResourceRT);
	}

	BitBlt(HDC(*pDC), 0, 0, m_tClientRT.right, m_tClientRT.bottom, hTexWndMemDC, 0, 0, SRCCOPY);	
	SelectObject(hTexWndMemDC, hOldMembit);
	DeleteDC(hTexWndMemDC);
	DeleteObject( hMembit );
	this->ReleaseDC(pDC);
}

void CPictureView::DrawDragAreaTexture()
{
	if(m_pTexture == NULL)
	{
		return;
	}

	RECT	tResourceRT;
	double	dResizeRate;

	GetClientRect(&m_tClientRT);

	HDC			hTexWndMemDC;
	HBITMAP		hMembit;
	HBITMAP		hOldMembit;

	CDC* pDC = this->GetDC();

	hTexWndMemDC= CreateCompatibleDC(HDC(*pDC));
	hMembit = MakeDIBSection(*pDC, m_tClientRT.right, m_tClientRT.bottom);
	hOldMembit = (HBITMAP)SelectObject(hTexWndMemDC, hMembit);

	tResourceRT.left = 0;
	tResourceRT.top = 0;
	tResourceRT.right = m_pTexture->GetWidth();
	tResourceRT.bottom = m_pTexture->GetHeight();

	double dRate = 0.0;
	double dClientRatio = m_tClientRT.right / (double)m_tClientRT.bottom;
	double dResourceRatio = tResourceRT.right / (double)tResourceRT.bottom;

	//	보간법을 적용해 준다.
	//	미적용시 그림파일 깨짐.
	::SetStretchBltMode(hTexWndMemDC, COLORONCOLOR);

	if(m_tClientRT.bottom < tResourceRT.bottom || m_tClientRT.right < tResourceRT.right)
	{
		dRate = dClientRatio / dResourceRatio;

		if(dResourceRatio > 1)
		{// 가로 길이가 모자란 경우(fRatio가 1보다 크다.)
			dResizeRate = tResourceRT.right / (double)m_tClientRT.right;
		}
		else if(dResourceRatio <= 1)
		{// 세로 길이가 모자란 경우 (fRatio가 1보다 작다.)
			dResizeRate = tResourceRT.bottom / (double)m_tClientRT.bottom;
		}

		/*
		20180904 06 : 52
		CxImage와 Cimage의 레퍼런스가 달라서 서로 다르게 적용을 해주어야한다.
		*/

		//	중앙 사각형 그리기
		RECT rtResize = GetCalculateRectByRate(m_rtDrag, dResizeRate);

		//	깜박임 현상이 어디서 나나 봤는데 DrawRect였다 ;
		PatBlt(hTexWndMemDC, m_rtDrag.left, m_rtDrag.top, m_rtDrag.right, m_rtDrag.bottom, WHITENESS);
		CxImage CropImage = (*m_pTexture);
		CropImage.Crop(rtResize);
		CropImage.Draw(hTexWndMemDC, m_rtDrag);
	}
	else
	{
		PatBlt(hTexWndMemDC, m_rtDrag.left, m_rtDrag.top, m_rtDrag.right, m_rtDrag.bottom, WHITENESS);
		CxImage CropImage = (*m_pTexture);
		CropImage.Crop(m_rtDrag);
		CropImage.Draw(hTexWndMemDC, m_rtDrag);
	}
	//	여기서는 일부분만 그려주는 것이기 때문에 bitblt의 x1, y1을 내가 그려준 부분으로만 산정을 해야한다.
	BitBlt(HDC(*pDC), m_rtDrag.left, m_rtDrag.top, m_rtDrag.right-m_rtDrag.left, m_rtDrag.bottom-m_rtDrag.top,
		hTexWndMemDC, m_rtDrag.left, m_rtDrag.top, SRCCOPY);
	SelectObject(hTexWndMemDC, hOldMembit);
	DeleteDC(hTexWndMemDC);
	DeleteObject( hMembit );
	this->ReleaseDC(pDC);
}

RECT CPictureView::GetCalculateRectByRate(RECT rect, double dValue)
{
	RECT result;
	result.top = (LONG)(rect.top * dValue);
	result.left = (LONG)(rect.left * dValue);
	result.right = (LONG)(rect.right * dValue);
	result.bottom = (LONG)(rect.bottom * dValue);

	return result;
}

void CPictureView::SetAlphaBlendValue()
{
	if(m_pTexture == NULL)
	{
		return;
	}
	//	종횡비와 비율을 계산하기 위한 RECT 구조체
	RECT	tClientRT;
	RECT	tResourceRT;
	RECT	rtResult;
	RECT	rtDraw;	//	그려질 곳의 영역을 지정하기 위한 RECT 구조체
	POINT	tPos;	//	시작점을 알려주기 위한 Point 구조체
	double	dResizeRate;

	//	계산 전 크기 할당
	GetClientRect(&tClientRT);

	//	계산 전 변수 밑 구조체 초기화 작업
	//	텍스쳐의 크기를 담는 RECT 구조체를 초기화
	tResourceRT.left = 0;
	tResourceRT.top = 0;
	tResourceRT.right = m_pTexture->GetWidth();
	tResourceRT.bottom = m_pTexture->GetHeight();

	double dRate = 0.0;
	double dClientRatio = tClientRT.right / (double)tClientRT.bottom;
	double dResourceRatio = tResourceRT.right / (double)tResourceRT.bottom;

	//	종횡비와 비율 계산
	if(tClientRT.bottom < tResourceRT.bottom || tClientRT.right < tResourceRT.right)
	{
		dRate = dClientRatio / dResourceRatio;

		if(dResourceRatio > 1)
		{// 가로 길이가 모자란 경우(fRatio가 1보다 크다.)
			dResizeRate = tResourceRT.right / (double)tClientRT.right;
		}
		else if(dResourceRatio <= 1)
		{// 세로 길이가 모자란 경우 (fRatio가 1보다 작다.)
			dResizeRate = tResourceRT.bottom / (double)tClientRT.bottom;
		}

		rtResult = GetCalculateRectByRate(m_rtDrag, dResizeRate);

		rtDraw.left = 0;
		rtDraw.right = rtResult.right-rtResult.left;
		rtDraw.top = 0;
		rtDraw.bottom = rtResult.bottom-rtResult.top;

		//	그려지는 영역의 시작점을 설정
		tPos.x = rtResult.left;
		tPos.y = rtResult.top;
	}
	else
	{
		tPos.x = m_rtDrag.left;
		tPos.y = m_rtDrag.top;

		rtDraw.left = 0;
		rtDraw.right = m_rtDrag.right - m_rtDrag.left;
		rtDraw.top = 0;
		rtDraw.bottom = m_rtDrag.bottom - m_rtDrag.top;
	}

	//	그려지는 영역의 사각형을  지정.
	m_tAlphaBlendPos = tPos;
	m_rtAlphaBlendBox = rtDraw;
}

void CPictureView::OnPaint()
{
	CPaintDC dc(this);

	if(m_pTexture!=NULL)
	{
		DrawTexture();
		DrawRect();
		DrawDragAreaTexture(); 
	}
}

void CPictureView::ClearPictureView()
{
	CDC* pDC = this->GetDC();
	PatBlt((HDC)*pDC, 0, 0, m_tClientRT.right, m_tClientRT.bottom, WHITENESS);
	ReleaseDC(pDC);
}

void CPictureView::AbortDragBox()
{
	if(m_pTexture != NULL)
	{
		//	드래그박스 영역이 텍스쳐 영역을 넘지 못하게 제한하는 것
		if(m_rtDrag.right > m_pTexture->GetWidth())
		{
			m_rtDrag.right = m_pTexture->GetWidth();
		}

		if(m_rtDrag.bottom > m_pTexture->GetHeight())
		{
			m_rtDrag.bottom = m_pTexture->GetHeight();
		}
	}
}
