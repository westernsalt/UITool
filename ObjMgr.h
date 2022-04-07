#pragma once

#include "StdAfx.h"

class CBaseInterface;
class CBasicAsset;
class CUIFrame;
class CUIScene;

class CObjMgr
{
	DECLARE_SINGLETON(CObjMgr)

	typedef int	INDEX;

	typedef std::vector<CBaseInterface*>		VEC_INTERFACE;
	typedef VEC_INTERFACE::iterator				VEC_INTERFACE_ITER;
	typedef VEC_INTERFACE::reverse_iterator		VEC_INTERFACE_REVERSE_ITER;

	typedef std::vector<CBasicAsset*>			VEC_BASICASSET;
	typedef VEC_BASICASSET::iterator			VEC_BASICASSET_ITER;
	typedef VEC_BASICASSET::reverse_iterator	VEC_BASICASSET_REVERSE_ITER;

	typedef std::vector<CUIFrame*>				VEC_FRAME;
	typedef VEC_FRAME::iterator					VEC_FRAME_ITER;
	typedef VEC_FRAME::reverse_iterator			VEC_FRAME_REVERSE_ITER;

	typedef std::vector<CUIScene*>				VEC_SCENE;
	typedef VEC_SCENE::iterator					VEC_SCENE_ITER;
	typedef VEC_SCENE::reverse_iterator			VEC_SCENE_REVERSE_ITER;
	
	VEC_INTERFACE	m_vecBaseInterface;
	VEC_BASICASSET	m_vecBasicAsset;
	VEC_FRAME		m_vecUIFrame;
	VEC_SCENE		m_vecUIScene;

	int				m_iFocusAssetBase;
	int				m_iFocusBasicAsset;
	int				m_iFocusUIFrame;
	int				m_iFocusUIScene;

	std::string		m_strAtt[ASSET_END];
	std::string		m_strCmpAtt[ASSET_END];

	bool			m_bShowID;
	// 800*600 �ػ� ���� [6/3/2019 Gabrielle]
	bool			m_b800x600;
	int				m_iCurDlgBarIndex;

	//	���� �� �̹��� ����
private:
	CAssetBase*		m_BackgroundBase;
	std::string		m_strBackgroundAssetPath;
	CAssetBase*		m_BlurImageBase;
	std::string		m_strBlurImageAssetPath;

	std::string		m_strSaveEditAssetFileName;		//������ ���� ���� �̸�
	std::string		m_strSaveSceneFileName;			//�� ���� �̸�
	std::string		m_strSaveFrameFileName;			//������ �̸�

public:
	void	ShowObjectID();
	void	HideObjectID();

public:
	bool	LoadBackgroundAsset(std::string _Path, HWND _hWnd, HWND _ViewhWnd);
	bool	LoadBlurImageAsset(std::string _Path, HWND _hWnd, HWND _ViewhWnd);
	void	SetSaveFileName(std::string strValue);
	void	SetSaveSceneFileName(std::string strValue);
	void	SetSaveFrameFileName(std::string strValue);
	void	Setb800x600(bool _bOn);

public:
	void	Run(eAssetTarget _target);
	void	Render(eAssetTarget _target, HDC _DC);
	void	ClearMap();
	void	ClearFrameMap();
	void	ClearSceneMap();
	void	ClearTargetMap(eAssetTarget _eTarget);

	POINT	CalculateAssetMinPos();	//	������ ��ǥ�� 0, 0 �������� �����ϰ��� �Ҷ� �ʿ��� ��.
	POINT	CalculateBaseMinPos(); //	���¿����Ϳ��� �����Ҷ� ���� ���� ��ǥ�� ������ 0,0 ���� �����ϱ� ���� ��ȯ�Լ�
	
public:
	bool	AssetBaseCollisionCheck(POINT _MousePos);
	bool	BasicAssetCollisionCheck(POINT _pos);
	bool	UIFrameCollisionCheck(POINT _pos);
		
	void	SetFocusAssetIndex(int nIndex);		//��Ŀ���� ���� �ε��� ����
	void	SetFocusFrameIndex(int nIndex);		//��Ŀ���� ������ �ε��� ����

	void	SetFocusAssetBaseNULL() { m_iFocusAssetBase = NOTHING_IN_HERE; }
	void	SetFocusBasicAssetNULL() { m_iFocusBasicAsset = NOTHING_IN_HERE; }
	void	SetFocusUIFrameNULL(){ m_iFocusUIFrame = NOTHING_IN_HERE; }

	CBaseInterface*	FindAssetBaseByIndex(INDEX _index);
	CBasicAsset*	FindBasicAssetByIndex(INDEX _index);
	CUIFrame*		FindUIFrameByIndex(INDEX _index);

	void	InsertAssetBase(CBaseInterface* _asset);
	void	InsertBasicAsset(CBasicAsset* _asset);
	void	InsertUIFrame(CUIFrame* _frame);
	void	InsertUIScene(CUIScene* _scene);

	void	DeleteAssetBase(CBaseInterface* _asset);
	void	DeleteBasicAsset(CBasicAsset* _asset);
	void	DeleteUIFrame(CUIFrame* _frame);

	void    ChangeFrameListDepth(int nIndex, int iChangePos);		//������ ������ ����Ʈ ���� ����
	void    ChangeBasicAssetListDepth(int nIndex, int iChangePos);	//������ BasicAsset ����Ʈ ���� ����
	
	void	SaveAsset(std::string FilePath);
	void	SaveFrame(std::string FilePath);				//	Frame Save
	void	SaveScene(std::string FilePath);
	
	void	LoadAsset(std::string FilePath, HWND _hWnd);
	void	LoadFrame(std::string FilePath, HWND _hWnd);	//	Frame Load
	void	LoadScene(std::string FilePath, HWND _hWnd, HWND _ViewhWnd);

	bool	SaveAssetByFileDlg();
	bool	SaveFrameByFileDlg();
	bool	SaveSceneByFileDlg();

	void	LoadAssetByFileDlg(HWND _hWnd);
	bool	LoadFrameByFileDlg(HWND _hWnd);
	bool	LoadSceneByFileDlg(HWND _hWnd, HWND _ViewhWnd);

	void	EditAssetFileName(std::string& _PathName);

	void	SetCurDlgBarIndex(int nIndex);					//���� ���� �����Ϳ��� ���µǾ� �ִ� ���̾�α� Ÿ�� �����ϱ�

public:
	int					GetTargetMapCount(eAssetTarget _Target);
	CBaseInterface*		GetFocusAssetBase();
	CBasicAsset*		GetFocusBasicAsset();
	int					GetFocusBasicAssetIndex();		//��Ŀ���� BasicAsset �ε��� ��������
	CUIFrame*			GetFocusUIFrame();
	int					GetFocusUIFrameIndex();			//��Ŀ���� ������ �ε��� ��������
	std::vector<CUIFrame*> 	GetUIFrameList();			//������ ����Ʈ ��������
	std::vector<CBasicAsset*> GetBasicAssetList();		//������ ���� ����Ʈ ��������
	int					GetCurDlgBarIndex();			//���� ���� �����Ϳ��� ���µǾ� �ִ� ���̾�α� Ÿ�� ��������

private:
	CObjMgr(void);
	~CObjMgr(void);
};

