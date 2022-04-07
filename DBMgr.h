#pragma once

#include "stdafx.h"

class CBasicAsset;
class CUIFrame;
class CUIScene;

class CDBMgr
{
	typedef int INDEX;

	DECLARE_SINGLETON(CDBMgr);

	enum eTextureType
	{
		TEXTURE_BTN,
		TEXTURE_PIC,
	};

private:
	typedef stdext::hash_map<INDEX, CxImage*>				TEX_MAP;
	typedef stdext::hash_map<INDEX, std::string>			PATH_MAP;
	typedef stdext::hash_map<INDEX, CBasicAsset*>			ASSET_MAP;
	typedef stdext::hash_map<INDEX, CUIFrame*>				FRAME_MAP;
	typedef stdext::hash_map<INDEX, CUIScene*>				SCENE_MAP;

	typedef TEX_MAP::iterator		MAP_TEX_ITER;
	typedef TEX_MAP::value_type		MAP_TEX_VALUETYPE;

	typedef PATH_MAP::iterator		MAP_PATH_ITER;
	typedef PATH_MAP::value_type	MAP_PATH_VALUETYPE;

	typedef ASSET_MAP::iterator		MAP_ASSET_ITER;
	typedef ASSET_MAP::value_type	MAP_ASSET_VALUETYPE;

	//여기 아직 이름 안바뀜.
	typedef FRAME_MAP::iterator		MAP_FRAME_ITER;
	typedef FRAME_MAP::value_type	MAP_FRAME_VALUETYPE;

	typedef SCENE_MAP::iterator		MAP_SCENE_ITER;
	typedef SCENE_MAP::value_type	MAP_SCENE_VALUETYPE;

	TEX_MAP		m_MapTexture;
	PATH_MAP	m_MapPath;
	FRAME_MAP	m_MapUIFrame;
	SCENE_MAP	m_MapUIScene;

	ASSET_MAP	m_vecSceneEditorAsset;
	ASSET_MAP	m_MapAssetEditorAsset;

public:
	std::string		LoadTexture(INDEX _index, LPCTSTR pszFileName);
	CBasicAsset		LoadBasicAsset(INDEX _index, LPCTSTR pszAssetPath, LPCTSTR pszAssetName, eAssetTarget _eTarget);
	bool			LoadUIFrame(INDEX _index, LPCTSTR pszFramePath, HWND _hWnd);
	bool			LoadUIScene(INDEX _index, LPCTSTR pszScenePath, HWND _hWnd);
	
	void			DeleteBasicAsset(INDEX _index, eAssetTarget _eTarget);

	void			ClearMap();
	void			ClearTextureMap();
	void			ClearFrameMap();
	void			ClearPathMap();
	void			ClearAssetMap(eAssetTarget _eTarget);
	void			ClearSceneMap();
	void			MapClearFunc(ASSET_MAP& _target);

	TEX_MAP*		GetTexMap() { return &m_MapTexture; }
	PATH_MAP*		GetPathMap() { return &m_MapPath; }
	ASSET_MAP*		GetBasicAssetMap() { return &m_vecSceneEditorAsset; }

	CxImage*		GetTexture(INDEX _index);
	CBasicAsset*	GetBasicAsset(INDEX _index, eAssetTarget _eTarget);
	CUIFrame*		GetUIFrame(INDEX _index);
	CUIScene*		GetUIScene(INDEX _index);

private:
	CDBMgr(void);
	~CDBMgr(void);
};

