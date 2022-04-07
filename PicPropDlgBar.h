#pragma once

class CAssetBase;

// CPicPropDlgBar ��ȭ �����Դϴ�.

class CPicPropDlgBar : public CDialogBar
{
	DECLARE_DYNAMIC(CPicPropDlgBar)

	CEdit*	m_TexturePathEdit;
	CEdit*	m_PictureXPosEdit;
	CEdit*	m_PictureYPosEdit;
	CEdit*	m_PictureWidthEdit;
	CEdit*	m_PictureHeightEdit;
	CEdit*	m_PictureScaleWidthEdit;
	CEdit*	m_PictureScaleHeightEdit;

private:
	std::string m_strTexturePath;
	POINT	m_ptPosition;
	POINT	m_ptSize;
	POINT	m_ptScaleSize;

private:
	CAssetBase*	m_PictureAssetBase;

private:
	bool	m_bChange;
	HWND	m_ParenthWnd;
	HWND	m_PicturehWnd;

private:
	void CreateAsset();
	void SetPictureProperty();

public:
	void Initialize();
	void RefreshDlgBar(CAssetBase* _LoadBase, POINT& _NormalTex);
	void SetParenthWnd(HWND _hWnd);
	void SetViewhWnd(HWND _hWnd){ m_PicturehWnd = _hWnd; }
	void SetMemberBase(CAssetBase* _base);

public:
	CPicPropDlgBar(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPicPropDlgBar();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnEnChangeEditPictureTexturePath();
	afx_msg void OnEnChangeEditPicXPos();
	afx_msg void OnEnChangeEditPicYPos();
	afx_msg void OnEnChangeEditPicWidth();
	afx_msg void OnEnChangeEditPicHeight();
protected:
	afx_msg LRESULT OnSelchangeTexturelist(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChangeDragboxInfo(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnEnChangeEditPicScaleWidth();
	afx_msg void OnEnChangeEditPicScaleHeight();
};
