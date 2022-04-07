#pragma once


// CPictureView

class CPictureView : public CWnd
{
#define MINIMUM_SIZE		5
#define DRAGRECT_INTERVAL	1

	DECLARE_DYNAMIC(CPictureView)
private:
	RECT	m_rtDrag;
	RECT	m_tClientRT;

	POINT	m_ptMouse;
	POINT	m_tStartPos;
	POINT	m_tEndPos;

	CxImage*	m_pTexture;

	int		m_iFlag;

	bool	m_bClicked;
	bool	m_bMouseOver;
	bool	m_bTextureDraw;

	HWND	m_AssetEditorhWnd;
	HWND	m_ParenthWnd;

public:
	void	ResetElement();
	void	DrawRect(bool _bAllSel = false);
	void	SetDragBox();
	void	SetDragBox(int _EditNum, eDragboxSet _set);
	void	AbortDragBox();
	void	SetAllSelDragBox(int _Width, int _Height);
	void	CheckDragBoxCollision();
	void	MouseStateCheck();
	void	ClearPictureView();

public:
	//	Setter 함수
	void	SetParenthWnd(HWND _hWnd)
	{
		m_ParenthWnd = _hWnd;
	}

	void	SetAssetEditorhWnd(HWND _hWnd)
	{
		m_AssetEditorhWnd = _hWnd;
	}
	void	SetbTextureDraw(bool _bTextureDraw)
	{
		m_bTextureDraw = _bTextureDraw;
	}
	void	SetpTexture(CxImage*	_Texture)
	{
		m_pTexture = _Texture;
	}


	//	Getter 함수

	RECT	GetRtDrag()
	{
		return m_rtDrag;
	}

public:
	void		DrawTexture();
	void		DrawDragAreaTexture();
	RECT		GetCalculateRectByRate(RECT rect, double dValue);

private:
	RECT	m_rtAlphaBlendBox;
	POINT	m_tAlphaBlendPos;

public:
	RECT	GetAlphaBlendBox() { return m_rtAlphaBlendBox; }
	POINT	GetAlphaBlendPos() { return m_tAlphaBlendPos; }
	void	SetAlphaBlendValue();

public:
	CPictureView();
	virtual ~CPictureView();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
};


