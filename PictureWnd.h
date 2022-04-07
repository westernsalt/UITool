#pragma once
#include "PictureView.h"


// CPictureWnd �������Դϴ�.

class CPictureDlgBar;

class CPictureWnd : public CFrameWnd
{
	DECLARE_DYNCREATE(CPictureWnd)

private:
	CPictureDlgBar*	m_pPictureDlgBar;
	CPictureView	m_wndView;

private:
	HWND			m_pParenthWnd;
	HWND			m_pDlgBarhWnd; //	���� ������� ������ ���̾˷α׹��� �ڵ�

public:
	void	ResetViewElement();
	void	SetCheckBoxUnchecked();
	void	SetTextureWndDragBoxClicked(bool _bClicked);

	RECT	GetTextureWndDragBoxRect();
	RECT	GetDragBoxRect();
	POINT	GetDragBoxStartPos();
	int		GetCurSelTextureListItem();

public:
	void	Initialize();

public:
	void	SetParenthWnd(HWND _hWnd) { m_pParenthWnd = _hWnd; }
	void	SetDlgBarhWnd(HWND _hWnd) { m_pDlgBarhWnd = _hWnd; }
	void	SetChildhWnd();

	//	���̾˷α׹� ��� �Լ�
	void	LoadingTextureList(CString _Path);
	void	SetTextureListIndex(CString _name);
	void	SetTextureListIndex(int _index);
	void	SetSize(POINT _size);
	void	SetPos(POINT _pos);


public:
	CPictureWnd();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CPictureWnd();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
	afx_msg LRESULT OnSelchangeTexturelist(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAllselChecked(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnResetPicwndElement(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdateDragboxInfo(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChangeDragboxInfo(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChangeRadiobtn(WPARAM wParam, LPARAM lParam);
};


