
// MainView.h : CMainView 클래스의 인터페이스
//


#pragma once

//class CSceneEditDlgBar;
//class CSceneSettingDlgBar;

// CMainView 창

class CMainView : public CWnd
{
// 생성입니다.
public:
	CMainView();

protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

public:
	virtual ~CMainView();

	// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

private:
	HWND		m_ParenthWnd;

	POINT		m_PrevMousePos;
	RECT		m_tClientRt;
private:
	bool		m_bClicked;
	bool		m_bFrameClicked;
	int			m_iFlag;
	int			m_iMouseFlag;

public:
	void		RenderObjMgrAsset();
	void		MouseStateCheck();
	void		SetParenthWnd(HWND _hWnd) { m_ParenthWnd = _hWnd; }
	int			GetMouseFlag() { return m_iMouseFlag; }

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

