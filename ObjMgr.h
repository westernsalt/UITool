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
	// 800*600 해상도 관련 [6/3/2019 Gabrielle]
	bool			m_b800x600;
	int				m_iCurDlgBarIndex;

	//	배경과 블러 이미지 저장
private:
	CAssetBase*		m_BackgroundBase;
	std::string		m_strBackgroundAssetPath;
	CAssetBase*		m_BlurImageBase;
	std::string		m_strBlurImageAssetPath;

	std::string		m_strSaveEditAssetFileName;		//에디터 에셋 파일 이름
	std::string		m_strSaveSceneFileName;			//신 파일 이름
	std::string		m_strSaveFrameFileName;			//프레임 이름

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

	POINT	CalculateAssetMinPos();	//	에셋의 좌표를 0, 0 기준으로 정렬하고자 할때 필요한 값.
	POINT	CalculateBaseMinPos(); //	에셋에디터에서 저장할때 가장 왼쪽 좌표의 에셋을 0,0 기준 저장하기 위한 반환함수
	
public:
	bool	AssetBaseCollisionCheck(POINT _MousePos);
	bool	BasicAssetCollisionCheck(POINT _pos);
	bool	UIFrameCollisionCheck(POINT _pos);
		
	void	SetFocusAssetIndex(int nIndex);		//포커스한 에셋 인덱스 저장
	void	SetFocusFrameIndex(int nIndex);		//포커스한 프레임 인덱스 저장

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

	void    ChangeFrameListDepth(int nIndex, int iChangePos);		//씬에서 프레임 리스트 뎁스 변경
	void    ChangeBasicAssetListDepth(int nIndex, int iChangePos);	//씬에서 BasicAsset 리스트 뎁스 변경
	
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

	void	SetCurDlgBarIndex(int nIndex);					//현재 에셋 에디터에서 오픈되어 있는 다이얼로그 타입 저장하기

public:
	int					GetTargetMapCount(eAssetTarget _Target);
	CBaseInterface*		GetFocusAssetBase();
	CBasicAsset*		GetFocusBasicAsset();
	int					GetFocusBasicAssetIndex();		//포커스된 BasicAsset 인덱스 가져오기
	CUIFrame*			GetFocusUIFrame();
	int					GetFocusUIFrameIndex();			//포커스된 프레임 인덱스 가져오기
	std::vector<CUIFrame*> 	GetUIFrameList();			//프레임 리스트 가져오기
	std::vector<CBasicAsset*> GetBasicAssetList();		//베이직 에셋 리스트 가져오기
	int					GetCurDlgBarIndex();			//현재 에셋 에디터에서 오픈되어 있는 다이얼로그 타입 가져오기

private:
	CObjMgr(void);
	~CObjMgr(void);
};

