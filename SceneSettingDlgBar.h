#pragma once


// CSceneSettingDlgBar

class CSceneSettingDlgBar : public CDialogBar
{
	enum eResolution
	{
		RESOLUTION_1024_768,
		RESOLUTION_800_600,
		RESOLUTION_END,
	};

	DECLARE_DYNAMIC(CSceneSettingDlgBar)

	CComboBox*		m_DepthComboBox;
	CComboBox*		m_ResolutionComboBox;	

	CEdit*		m_pPos_X_Edit;
	CEdit*		m_pPos_Y_Edit;
	CEdit*		m_pPos_Z_Edit;

	CEdit*		m_pFrameScaleXEdit;
	CEdit*		m_pFrameScaleYEdit;

	CComboBox*	m_pResolutionCombobox;

	CListBox*	m_pDepthSettingListBox;		//뎁스 세팅 리스트

	CString		m_strPosX;
	CString		m_strPosY;
	CString		m_strPosZ;

	CString		m_strScaleX;
	CString		m_strScaleY;
	float		m_fScaleRatio;

	HWND		m_ParenthWnd;

	bool		m_bEditFocused;
	bool		m_bScaleChange;

	int			m_iSelectDepthListIndex;	//선택 Asset And Frame 인덱스
	eRefreshType m_eRefreshType;			//Asset인지 Frame인지 구부하는 타입
	int			m_nFocusIndex;				//포커된 인덱스(Asset/Frame) 저장

public:
	CString		m_strResolution[RESOLUTION_END];

public:
	void		SetParenthWnd(HWND _hWnd) { m_ParenthWnd = _hWnd; }
	bool		GetbEditFocused(){ return m_bEditFocused; }


public:
	CSceneSettingDlgBar();
	virtual ~CSceneSettingDlgBar();

public:
	void		AdaptedChangedPosEdit();
	void		RefreshPosEdit(eRefreshType _type);
	void		RefreshDepthSettingList(int nIndex); //뎁스 세팅 리스트 갱신	
	void		SelectDepthListIndex(int nSelectIndex); //뎁스 옮기는 버튼 눌렀을 경우 리스트 포커스가 날라가서 강제로 포커스해줌.

	void		UpdateEdit();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnEnChangeEditPosX();
	afx_msg void OnEnChangeEditPosY();
	afx_msg void OnEnChangeEditPosZ();
	afx_msg void OnEnSetfocusEditPosX();
	afx_msg void OnEnSetfocusEditPosY();
	afx_msg void OnEnSetfocusEditPosZ();
	afx_msg void OnUpdateCmdUI(CCmdUI* pCmdUI);
	afx_msg void OnCbnSelchangeComboSceneResolution();

	afx_msg LRESULT OnBasicAssetFocused(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUiframeFocused(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAssetListDoubleClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFrameListDoubleClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSceneListDoubleClick(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnLbnSelchangeDepthsettingList();
	afx_msg void OnBnClickedButtonDepthTop();
	afx_msg void OnBnClickedButtonDepthBottom();
	afx_msg void OnBnClickedButtonDepthUp();
	afx_msg void OnBnClickedButtonDepthDown();
	afx_msg void OnBnClickedCheckSceneIdShow();
	afx_msg void OnEnChangeEditFrameScaleX();
	afx_msg void OnEnChangeEditFrameScaleY();
};


