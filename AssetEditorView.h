#pragma once


// CAssetEditorView

class CAssetEditDlg;

class CAssetEditorView : public CWnd
{
	DECLARE_DYNAMIC(CAssetEditorView)
	
	bool	m_bClicked;
	bool	m_bDragBox;
	int		m_iFlag;

	POINT	m_PrevMousePos;
	RECT	m_tClientRt;

	HWND	m_AssetEditorhWnd;

	CString	m_strInput;

public:
	void	MouseStateCheck();
	void	DrawAlphaBlendDragBox(CxImage* _Texture, RECT _DragBox, POINT _Pos);

public:
	void	SetPrevMousePos(POINT _pos) { m_PrevMousePos = _pos; }
	void	SetParenthWnd(HWND _hWnd) { m_AssetEditorhWnd = _hWnd; }

	bool	GetAssetClicked() { return m_bClicked; }
	POINT	GetPrevMousePos() { return m_PrevMousePos; }
	bool	GetbDragBox() { return m_bDragBox; }
	RECT	GetClientRT() { this->GetClientRect(&m_tClientRt); return m_tClientRt; }
	int		GetMouseFlag() { return m_iFlag; }
	CString	GetInputString() { return m_strInput; }

public:
	CAssetEditorView();
	virtual ~CAssetEditorView();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
protected:
	afx_msg LRESULT OnDragboxSelect(WPARAM wParam, LPARAM lParam);

public:
	void		RenderObjMgrAsset();
	afx_msg void OnPaint();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
};


