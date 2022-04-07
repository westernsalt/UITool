#pragma once


// CAssetEditor 프레임입니다.

#include "AssetEditorView.h"

class CAssetSettingDlgBar;
class CAssetEditWnd;
class CPictureWnd;

class CAssetEditor : public CFrameWnd
{
	enum eEditorWnd
	{
		EDITWND_WIDTH = WINDOW_SIZE_X / 2,
		EDITWND_HEIGHT = WINDOW_SIZE_Y / 2,
		EDITWND_START_X = 0,
		EDITWND_START_Y = 0,
	};

	DECLARE_DYNCREATE(CAssetEditor)

private:
	CAssetEditorView		m_wndView;				

	// DlgBar
	CAssetSettingDlgBar*	m_pAssetSettingDlgBar;
	CDialogBar*				m_pAssetPropDlgBar;
	CPictureWnd*			m_pPictureWnd;

	int		m_iCurDlgBarIndex;
	int		m_iTextureListIndex;

	CString m_strInput;

public:
	void	CreatePictureWnd();
	void	InitializeAssetPropDlgBar();
	void	InitializePictureWnd();
	void	SetPictureDlgBarControl(std::string _path, RECT _size, POINT _pos);

public:
	CAssetEditor();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CAssetEditor();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnLoadAsset();
	afx_msg void OnSaveAsset();
	afx_msg void OnEditorExit();

protected:
	afx_msg LRESULT OnDrawAssetBase(WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	afx_msg LRESULT OnSaveAssetMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAssetBaseFocused(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAssetAttChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPicturewndDestroy(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPicturewndCreate(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRefreshDlgbarInfo(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChangeAttcombobox(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChangeGroupbuttonIndex(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClearAll(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSelchangeTexturelist(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRequestString(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnInitString(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAssetEditorDubleClick(WPARAM wParam, LPARAM lParam);	//에셋 에디터 다이얼로그 리스트에서 더블클릭
};


