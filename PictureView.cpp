// PictureView.cpp : ���� �����Դϴ�.
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

// CPictureView �޽��� ó�����Դϴ�.

void CPictureView::ResetElement()
{
	m_bClicked = false;
	RECT_INITIALIZE(m_rtDrag);
}

void CPictureView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_bTextureDraw &&  m_pTexture != NULL)
	{
		//	���� ���õ� �̹����� ���� �����䰡 �������� �ʴ� ��� ������ �ʿ䰡 ����
		CheckDragBoxCollision();

		//	�巡�� �ڽ��� Ŭ������ ���� ���, ���콺�� ��ǥ�� �����Ѵ�
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
		//	�巡�� �ڽ��� Ŭ������ ���� ��� �巡�׹ڽ��� �����ϵ��� �Ѵ�
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
		//	�巡�� �ڽ��� �������� ������ ���õ� ���� ��ǥ�� �����Ѵ�.
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
		// �ڱ� ���ֱ�
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

		//	������ ������ ��� ��� �����Ѵ�
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
	//	�巡���ϴ� ������ �׷��ش�.
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
	//	�巡�׹ڽ� ������ ���� ��ǥ ����
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
	//	PictureDlg���� �巡�׹ڽ��� ������ ���
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

	//	�ؽ����� ���� ũ�Ⱑ �״�� ���Ծ��� ������ ������ ���� ���̴�.
	if(tClientRT.bottom < _Height || tClientRT.right < _Width)
	{
		double dRate = 0.0;
		double dClientRatio = tClientRT.right / (double)tClientRT.bottom;
		double dResourceRatio = _Width / (double)_Height;

		dRate = dClientRatio / dResourceRatio;

		if(dResourceRatio > 1)
		{// ���� ���̰� ���ڶ� ���(fRatio�� 1���� ũ��.)
			dResizeRate = _Width / (double)m_tClientRT.right;
		}
		else if(dResourceRatio <= 1)
		{// ���� ���̰� ���ڶ� ��� (fRatio�� 1���� �۴�.)
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
		//	�巡�� �ڽ��� Ŭ���Ǿ��ٸ� ���� ������ �����ϴ� ���� �ƴϰ� ���콺 ���� �Ͼ������
		//	�ش� �ؽ��ĸ� ���콺 ��ǥ�� �߽����� �ؼ� �׷����Ѵ�.
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

	tme.hwndTrack = m_hWnd; //	���� �ڽ��� ������ �ڵ� ����
	tme.dwHoverTime = 10;	//	10 �и�������� Ÿ�̹� ����

	_TrackMouseEvent( &tme );
}

//	2018.08.16
//	EditDlgBar���� ����Ǵ� Draw��Ʈ�� �̽��ؿԴ�

void CPictureView::DrawTexture()
{
	if(m_pTexture == NULL)
	{
		return;
	}
	//	������ ClientRt�� ������ ������ �ִ� RECT�̱� ������ m_tClientRT�� ������� �ʴ´�.
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

	//	�������� ������ �ش�.
	//	������� �׸����� ����.
	::SetStretchBltMode(hTexWndMemDC, COLORONCOLOR);

	if(tClientRT.bottom < tResourceRT.bottom || tClientRT.right < tResourceRT.right)
	{
		double dRate = 0.0;
		double dClientRatio = tClientRT.right / (double)tClientRT.bottom;
		double dResourceRatio = tResourceRT.right / (double)tResourceRT.bottom;

		dRate = dClientRatio / dResourceRatio;

		if(dResourceRatio > 1)
		{// ���� ���̰� ���ڶ� ���(fRatio�� 1���� ũ��.)
			double dBottom = (double)tClientRT.bottom;
			dBottom *= dRate;
			tClientRT.bottom = (LONG)dBottom;
		}
		else if(dResourceRatio <= 1)
		{// ���� ���̰� ���ڶ� ��� (fRatio�� 1���� �۴�.)
			double dRight = tClientRT.right;
			dRight /= dRate;
			tClientRT.right = (LONG)dRight;
		}

		PatBlt(hTexWndMemDC, 0, 0, m_tClientRT.right, m_tClientRT.bottom, WHITENESS);
		//	�ؽ��� ��輱
		Rectangle(hTexWndMemDC, 0, 0, tResourceRT.right, tResourceRT.bottom);
		m_pTexture->Draw(hTexWndMemDC, tClientRT, &tResourceRT);
	}
	else
	{
		POINT tPos;
		tPos.x = 0;
		tPos.y = 0;
		PatBlt(hTexWndMemDC, 0, 0, m_tClientRT.right, m_tClientRT.bottom, WHITENESS);
		//	�ؽ��� ��輱
		Rectangle(hTexWndMemDC, 0, 0, tResourceRT.right, tResourceRT.bottom);
		// CxImage�׸���
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

	//	�������� ������ �ش�.
	//	������� �׸����� ����.
	::SetStretchBltMode(hTexWndMemDC, COLORONCOLOR);

	if(m_tClientRT.bottom < tResourceRT.bottom || m_tClientRT.right < tResourceRT.right)
	{
		dRate = dClientRatio / dResourceRatio;

		if(dResourceRatio > 1)
		{// ���� ���̰� ���ڶ� ���(fRatio�� 1���� ũ��.)
			dResizeRate = tResourceRT.right / (double)m_tClientRT.right;
		}
		else if(dResourceRatio <= 1)
		{// ���� ���̰� ���ڶ� ��� (fRatio�� 1���� �۴�.)
			dResizeRate = tResourceRT.bottom / (double)m_tClientRT.bottom;
		}

		/*
		20180904 06 : 52
		CxImage�� Cimage�� ���۷����� �޶� ���� �ٸ��� ������ ���־���Ѵ�.
		*/

		//	�߾� �簢�� �׸���
		RECT rtResize = GetCalculateRectByRate(m_rtDrag, dResizeRate);

		//	������ ������ ��� ���� �ôµ� DrawRect���� ;
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
	//	���⼭�� �Ϻκи� �׷��ִ� ���̱� ������ bitblt�� x1, y1�� ���� �׷��� �κ����θ� ������ �ؾ��Ѵ�.
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
	//	��Ⱦ��� ������ ����ϱ� ���� RECT ����ü
	RECT	tClientRT;
	RECT	tResourceRT;
	RECT	rtResult;
	RECT	rtDraw;	//	�׷��� ���� ������ �����ϱ� ���� RECT ����ü
	POINT	tPos;	//	�������� �˷��ֱ� ���� Point ����ü
	double	dResizeRate;

	//	��� �� ũ�� �Ҵ�
	GetClientRect(&tClientRT);

	//	��� �� ���� �� ����ü �ʱ�ȭ �۾�
	//	�ؽ����� ũ�⸦ ��� RECT ����ü�� �ʱ�ȭ
	tResourceRT.left = 0;
	tResourceRT.top = 0;
	tResourceRT.right = m_pTexture->GetWidth();
	tResourceRT.bottom = m_pTexture->GetHeight();

	double dRate = 0.0;
	double dClientRatio = tClientRT.right / (double)tClientRT.bottom;
	double dResourceRatio = tResourceRT.right / (double)tResourceRT.bottom;

	//	��Ⱦ��� ���� ���
	if(tClientRT.bottom < tResourceRT.bottom || tClientRT.right < tResourceRT.right)
	{
		dRate = dClientRatio / dResourceRatio;

		if(dResourceRatio > 1)
		{// ���� ���̰� ���ڶ� ���(fRatio�� 1���� ũ��.)
			dResizeRate = tResourceRT.right / (double)tClientRT.right;
		}
		else if(dResourceRatio <= 1)
		{// ���� ���̰� ���ڶ� ��� (fRatio�� 1���� �۴�.)
			dResizeRate = tResourceRT.bottom / (double)tClientRT.bottom;
		}

		rtResult = GetCalculateRectByRate(m_rtDrag, dResizeRate);

		rtDraw.left = 0;
		rtDraw.right = rtResult.right-rtResult.left;
		rtDraw.top = 0;
		rtDraw.bottom = rtResult.bottom-rtResult.top;

		//	�׷����� ������ �������� ����
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

	//	�׷����� ������ �簢����  ����.
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
		//	�巡�׹ڽ� ������ �ؽ��� ������ ���� ���ϰ� �����ϴ� ��
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
