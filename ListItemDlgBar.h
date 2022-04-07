#pragma once
#include "afxwin.h"

#include "BasicAsset.h"

class CListItemBase;

// CListItemDlgBar ��ȭ �����Դϴ�.


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

	CListBox*		m_pDepthSettingListBox;		//���� ���� ����Ʈ
	int				m_iSelectDepthListIndex;	//���� Asset �ε���	

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
	void		RefreshDepthSettingList(int nIndex, bool bAllClear=false); //���� ���� ����Ʈ ����	

private:	
	void		SelectDepthListIndex(int nSelectIndex); //���� �ű�� ��ư ������ ��� ����Ʈ ��Ŀ���� ���󰡼� ������ ��Ŀ������.

public:
	void	SetParenthWnd(HWND _hWnd);
	void	Initialize();

public:
	CListItemDlgBar(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CListItemDlgBar();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	static int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnUpdateCmdUI(CCmdUI* pCmdUI);
	afx_msg void OnLbnDblclkListListitem();
	afx_msg void OnCbnSelchangeComboListitemAtt();
	afx_msg void OnBnClickedButtonLoadListitemDirectory();
	afx_msg void OnLbnSelchangeListitemDepthsettingList();	//����Ʈ ������ ����Ʈ Ŭ�� �̺�Ʈ
	afx_msg void OnBnClickedListitemButtonDepthTop();		//��������(�ֻ���)
	afx_msg void OnBnClickedListitemButtonDepthBottom();	//��������(������)
	afx_msg void OnBnClickedListitemButtonDepthUp();		//��������(��ĭ��)
	afx_msg void OnBnClickedListitemButtonDepthDown();		//��������(��ĭ�Ʒ�)
	afx_msg LRESULT OnAssetBaseFocused(WPARAM wParam, LPARAM lParam);		//���� �����Ϳ��� ��Ŀ�� ������ �������ÿ� �˷��ֱ� ���� �̺�Ʈ
	afx_msg LRESULT OnAssetEditorDubleClick(WPARAM wParam, LPARAM lParam);	//���� ������ ���̾�α� ����Ʈ���� ����Ŭ�� ������ �������ÿ� �˷��ֱ� ���� �̺�Ʈ
	afx_msg LRESULT OnAssetEditorAllClear(WPARAM wParam, LPARAM lParam);	//������ ���ÿ��� ȭ�� Ŭ���� ��ư�� Ŭ���� �������ÿ� �˷��ֱ� ���� �̺�Ʈ
	afx_msg LRESULT OnDeleteListItem(WPARAM wParam, LPARAM lParam);			//������ ȭ�鿡�� ������ �������� �� �������ÿ� �˷��ֱ� ���� �̺�Ʈ
};
