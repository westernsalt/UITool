#pragma once


// CUIFrameFileList

class CUIFrame;

class CUIFrameFileList : public CListBox
{
	DECLARE_DYNAMIC(CUIFrameFileList)

private:
	CUIFrame*		m_pFocusScene;
	int				m_iFocusIndex;
	HWND			m_ParenthWnd;
	HWND			m_MainFrmhWnd;

public:
	void			LoadUIFrameList(CString _path);

public:
	CUIFrameFileList();
	virtual ~CUIFrameFileList();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLbnSelchange();
	afx_msg void OnLbnDblclk();

public:
	void		SetParentWnd(HWND _hWnd) { m_ParenthWnd = _hWnd; }
	void		SetMainFrmhWnd(HWND _hWnd) { m_MainFrmhWnd = _hWnd; }
};