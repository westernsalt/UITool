// TextureWnd.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "UITool.h"
#include "PictureWnd.h"
#include "DBMgr.h"
// CPictureWnd

#include "PictureDlgBar.h"

IMPLEMENT_DYNCREATE(CPictureWnd, CFrameWnd)

CPictureWnd::CPictureWnd()
{
	m_pPictureDlgBar = new CPictureDlgBar;
}

CPictureWnd::~CPictureWnd()
{
	m_pPictureDlgBar->DestroyWindow();
	::SendMessage(m_pParenthWnd, WM_PICTUREWND_DESTROY, NULL, NULL);
}

BEGIN_MESSAGE_MAP(CPictureWnd, CFrameWnd)
	ON_WM_CREATE()
	ON_MESSAGE(WM_SELCHANGE_TEXTURELIST, &CPictureWnd::OnSelchangeTexturelist)
	ON_MESSAGE(WM_ALLSEL_CHECKED, &CPictureWnd::OnAllselChecked)
	ON_MESSAGE(WM_RESET_PICWND_ELEMENT, &CPictureWnd::OnResetPicwndElement)
	ON_MESSAGE(WM_UPDATE_DRAGBOX_INFO, &CPictureWnd::OnUpdateDragboxInfo)
	ON_MESSAGE(WM_CHANGE_DRAGBOX_INFO, &CPictureWnd::OnChangeDragboxInfo)
	ON_MESSAGE(WM_CHANGE_RADIOBTN, &CPictureWnd::OnChangeRadiobtn)
END_MESSAGE_MAP()

// CPictureWnd �޽��� ó�����Դϴ�.

int CPictureWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// �������� Ŭ���̾�Ʈ ������ �����ϴ� �並 ����ϴ�.
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("�� â�� ������ ���߽��ϴ�.\n");
		return -1;
	}
	m_wndView.MoveWindow(0, 0, WND_WIDTH, WND_HEIGHT);

	m_pPictureDlgBar->Create(this, IDD_DIALOGBAR_PICTURE_EDIT, CBRS_ALIGN_RIGHT, IDD_DIALOGBAR_PICTURE_EDIT);
	EnableDocking(CBRS_ALIGN_ANY);

	TCHAR strInitPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, strInitPath);
	// �н� ���� : Release 
#if defined _DEBUG || defined RELEASE_DEV_PATH
	_tcscat_s(strInitPath, L"\\Texture");
#else
	_tcscat_s(strInitPath, TGA_FOLDER_PATH);
#endif

	m_pPictureDlgBar->LoadTextureList(strInitPath);
	//	���⿡�� �θ� �ڵ��� ����������Ѵ�.
	//	�׸��� �ؽ��� ������� ���̾˷α׹ٸ� ���ؼ� ����ϴ� �κ��� �����Ƿ� �� �κ� ������ ������Ѵ�
	return 0;
}

void CPictureWnd::ResetViewElement()
{
	m_wndView.ResetElement();
}


afx_msg LRESULT CPictureWnd::OnSelchangeTexturelist(WPARAM wParam, LPARAM lParam)
{
	m_wndView.SetpTexture(CDBMgr::GetInstance()->GetTexture((int)wParam));
	m_wndView.OnPaint();
	GetDragBoxRect();
	GetDragBoxStartPos();

	m_wndView.ResetElement();
	m_wndView.DrawTexture();
	::SendMessage(m_pParenthWnd, WM_SELCHANGE_TEXTURELIST, NULL, NULL);
	return 0;
}


afx_msg LRESULT CPictureWnd::OnAllselChecked(WPARAM wParam, LPARAM lParam)
{
	m_wndView.SetAllSelDragBox((int)wParam, (int)lParam);
	m_wndView.DrawRect();
	m_wndView.DrawDragAreaTexture();
	return 0;
}


afx_msg LRESULT CPictureWnd::OnResetPicwndElement(WPARAM wParam, LPARAM lParam)
{
	m_wndView.SetpTexture(NULL);
	m_wndView.ResetElement();
	return 0;
}

void CPictureWnd::SetCheckBoxUnchecked()
{
	m_pPictureDlgBar->SetCheckBoxUnchecked();
}

RECT CPictureWnd::GetTextureWndDragBoxRect()
{
	return m_wndView.GetRtDrag();
}

RECT CPictureWnd::GetDragBoxRect()
{
	m_wndView.SetAlphaBlendValue();
	return m_wndView.GetAlphaBlendBox();
}

POINT CPictureWnd::GetDragBoxStartPos()
{
	m_wndView.SetAlphaBlendValue();
	return m_wndView.GetAlphaBlendPos();
}

void CPictureWnd::SetChildhWnd()
{
	m_wndView.SetAssetEditorhWnd(m_pParenthWnd);
	m_wndView.SetParenthWnd(this->m_hWnd);
	m_pPictureDlgBar->SetParentWnd(this->m_hWnd);
	m_pPictureDlgBar->SetDlgBarhWnd(m_pDlgBarhWnd);
}

int CPictureWnd::GetCurSelTextureListItem()
{
	return m_pPictureDlgBar->GetCurSelTextureListItem();
}


afx_msg LRESULT CPictureWnd::OnUpdateDragboxInfo(WPARAM wParam, LPARAM lParam)
{
	//	�� â���� �巡�� �ڽ��� ������ ��� �� �����͸� ���̾˷α׹ٿ� ������Ʈ ������Ѵ�
	switch((int)lParam)
	{
	case DRAGBOX_LEFT:
	case DRAGBOX_TOP:
	case DRAGBOX_WIDTH:
	case DRAGBOX_HEIGHT:
		m_wndView.SetDragBox((int)wParam, (eDragboxSet)lParam);
		m_pPictureDlgBar->SetEditWnd((int)wParam, (eDragboxSet)lParam);
		break;
	case DRAGBOX_RECT:
		RECT tDragbox = m_wndView.GetRtDrag();
		m_pPictureDlgBar->UpdateEditData(tDragbox);
		break;
	}



	return 0;
}


afx_msg LRESULT CPictureWnd::OnChangeDragboxInfo(WPARAM wParam, LPARAM lParam)
{
	m_wndView.SetDragBox((int)wParam,(eDragboxSet)lParam);
	return 0;
}

void CPictureWnd::SetTextureListIndex(CString _name)
{
	m_pPictureDlgBar->SetTextureListIndex(_name);
}

void CPictureWnd::SetTextureListIndex( int _index )
{
	m_pPictureDlgBar->SetTextureListIndex(_index);
}

void CPictureWnd::SetSize( POINT _size )
{
	m_pPictureDlgBar->SetSize(_size);
}

void CPictureWnd::SetPos( POINT _pos )
{
	m_pPictureDlgBar->SetPos(_pos);
}


afx_msg LRESULT CPictureWnd::OnChangeRadiobtn(WPARAM wParam, LPARAM lParam)
{
	POINT SelPos;
	SelPos.x = (LONG)wParam;
	SelPos.y = (LONG)lParam;
	m_pPictureDlgBar->SetPos(SelPos);
	return 0;
}

void CPictureWnd::Initialize()
{
	ResetViewElement();
	m_wndView.SetpTexture(NULL);
	m_wndView.SetbTextureDraw(false);
	m_pPictureDlgBar->Initialize();
	m_wndView.ClearPictureView();
}

void CPictureWnd::LoadingTextureList( CString _Path )
{
	m_pPictureDlgBar->SetTextureListPath(_Path);
	m_pPictureDlgBar->LoadTextureList(_Path);
}
