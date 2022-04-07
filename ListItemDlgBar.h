#pragma once
#include "afxwin.h"

#include "BasicAsset.h"

class CListItemBase;

// CListItemDlgBar 대화 상자입니다.


class CListItemDlgBar : public CDialogBar
{
	DECLARE_DYNAMIC(CListItemDlgBar)
#define LISTITEMASSET_COUNT		6
private:
	CListBox*	m_AssetList;
	CComboBox*	m_AssetAttributeCombo;
	int			m_iComboboxIndex;
	CString		m_strAssetListInitPath;
	int			m_AssetType[LISTITEMASSET_COUNT];

private:
	typedef		std::vector<CBasicAsset*>			VEC_ASSETLIST;
	typedef		std::vector<CBasicAsset*>::iterator	VEC_ASSETLIST_ITER;

	VEC_ASSETLIST	m_vecAssetList;

private:	
	CListItemBase*	m_ListItemBase;

	CListBox*		m_pDepthSettingListBox;		//뎁스 세팅 리스트
	int				m_iSelectDepthListIndex;	//선택 Asset 인덱스	

private:
	HWND		m_ParenthWnd;

public:
	void		BrowseFolder();
	CBasicAsset	LoadBasicAsset(LPCTSTR pszAssetPath, LPCTSTR pszAssetName);
	
public:
	void		RefreshAssetList();
	void		LoadAssetList();
	void		DeleteVectorAsset(int _iIndex);
	void		ClearVector();
	void		CreateAsset();
	void		RefreshDepthSettingList(int nIndex, bool bAllClear=false); //뎁스 세팅 리스트 갱신	

private:	
	void		SelectDepthListIndex(int nSelectIndex); //뎁스 옮기는 버튼 눌렀을 경우 리스트 포커스가 날라가서 강제로 포커스해줌.

public:
	void	SetParenthWnd(HWND _hWnd);
	void	Initialize();

public:
	CListItemDlgBar(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CListItemDlgBar();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	static int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnUpdateCmdUI(CCmdUI* pCmdUI);
	afx_msg void OnLbnDblclkListListitem();
	afx_msg void OnCbnSelchangeComboListitemAtt();
	afx_msg void OnBnClickedButtonLoadListitemDirectory();
	afx_msg void OnLbnSelchangeListitemDepthsettingList();	//리스트 아이템 리스트 클릭 이벤트
	afx_msg void OnBnClickedListitemButtonDepthTop();		//뎁스세팅(최상위)
	afx_msg void OnBnClickedListitemButtonDepthBottom();	//뎁스세팅(최하위)
	afx_msg void OnBnClickedListitemButtonDepthUp();		//뎁스세팅(한칸위)
	afx_msg void OnBnClickedListitemButtonDepthDown();		//뎁스세팅(한칸아래)
	afx_msg LRESULT OnAssetBaseFocused(WPARAM wParam, LPARAM lParam);		//에셋 에디터에서 포커스 됐을때 뎁스세팅에 알려주기 위한 이벤트
	afx_msg LRESULT OnAssetEditorDubleClick(WPARAM wParam, LPARAM lParam);	//에셋 에디터 다이얼로그 리스트에서 더블클릭 했을때 뎁스세팅에 알려주기 위한 이벤트
	afx_msg LRESULT OnAssetEditorAllClear(WPARAM wParam, LPARAM lParam);	//에디터 세팅에서 화면 클리어 버튼을 클릭시 뎁스세팅에 알려주기 위한 이벤트
	afx_msg LRESULT OnDeleteListItem(WPARAM wParam, LPARAM lParam);			//에디터 화면에서 에셋을 삭제했을 때 뎁스세팅에 알려주기 위한 이벤트
};
