#include "StdAfx.h"
#include "DBMgr.h"

#include "AssetBase.h"
#include "BasicAsset.h"
#include "UIFrame.h"
#include "TextBox.h"
#include "BtnBase.h"
#include "BaseInterface.h"
#include "CheckBoxBase.h"
#include "GroupButton.h"
#include "UIScene.h"

//싱글톤 매니저

IMPLEMENT_SINGLETON(CDBMgr);

CDBMgr::CDBMgr(void)
{
}


CDBMgr::~CDBMgr(void)
{
	ClearMap();
}

std::string CDBMgr::LoadTexture(INDEX _index, LPCTSTR pszFileName )
{
	// CxImage에 이미지를 로딩하는 함수
	std::string strKey = "";
	std::string strFilePath = "";

	CString		strName;
	CString		strPath;

	CxImage* pTexture = new CxImage;
	bool bResult = pTexture->Load(pszFileName, CXIMAGE_FORMAT_TGA);
	if(bResult == true)
	{
		//	Path에서 파일이름만 찾아옴
		strName = PathFindFileName(pszFileName);
		CT2CA pszConvertedName (strName);
		std::string _strName(pszConvertedName);
		strKey = _strName;

		strPath = pszFileName;
		CT2CA pszConvertedPath (strPath);
		std::string _strPath(pszConvertedPath);
		strFilePath = _strPath;

		// string 헤더 미포함으로 에러 났던 것을 헤더 포함으로 해결.
		m_MapTexture.insert(MAP_TEX_VALUETYPE(_index, pTexture));
		m_MapPath.insert(MAP_PATH_VALUETYPE(_index, strFilePath));
	}
	else
	{
		SAFE_DELETE(pTexture);
	}

	return strKey;
}

CBasicAsset CDBMgr::LoadBasicAsset( INDEX _index, LPCTSTR pszAssetPath, LPCTSTR pszAssetName, eAssetTarget _eTarget )
{
	//	파일을 찾아서 그 파일의 경로를 인자로 주고 있다.
	//	에셋의 경로, 이름, 저장할 타겟, 그리고 리스트박스의 인덱스까지.
	std::ifstream	ifLoad(pszAssetPath);
	std::string		strInput;

	CBasicAsset*	newBasicAsset = new CBasicAsset;

	if(ifLoad.good())
	{
		//20181008
		//	여기서 읽어오는 부분이 오류가 있었다.
		while(!ifLoad.eof())
		{
			CBaseInterface* newAsset = NULL;
			//	복수의 입력을 위한 InsertAsset과 SetFilePath의 배치

			//	로드 에셋 데이터
			//	오브젝트 매니저에서는 잘 돌아가는 코드가 여기서 힙 손상이 발생하는 이유?
			std::getline(ifLoad, strInput, '\n');

			if(!strcmp(strInput.c_str(), "[END]"))
			{
				newAsset = NULL;
			}

			if(!strcmp(strInput.c_str(), "[START]"))
			{
				std::getline(ifLoad, strInput, '\t');
				std::getline(ifLoad, strInput, '\n');
				strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());

				eAssetType LoadAssetType = (eAssetType)atoi(strInput.c_str());
				newAsset = MakeAdaptableBase(LoadAssetType);
				newAsset->SetAssetType(LoadAssetType);
			}

			if(newAsset != NULL)
			{
				newAsset->Load(ifLoad);
				newBasicAsset->InsertAsset(newAsset);
				newAsset = NULL;
			}
		}

		CString _csPath = pszAssetPath;
		CT2CA pszConvertedAnsiString (_csPath);
		std::string _stdPath(pszConvertedAnsiString);

		CString _csName = pszAssetName;
		CT2CA pszConvertedName (_csName);
		std::string _stdName(pszConvertedName);

		newBasicAsset->SetFilePath(_stdPath);
		newBasicAsset->SetFileName(_stdName);
		newBasicAsset->Initialize();

		if(newBasicAsset->GetAssetType() != ASSET_RADIOBUTTON || newBasicAsset->GetAssetType() != ASSET_TABBUTTON)
		{
			//	그룹버튼은 이부분 알아서 하는거로.
			//	현재는 밑의 함수 내부에서 라디오 버튼 따로 굴려주고있음.
			newBasicAsset->AssetBasePathSetting();
		}

		switch(_eTarget)
		{
		case ASSET_EDITOR_TARGET:
			m_MapAssetEditorAsset.insert(MAP_ASSET_VALUETYPE(_index, newBasicAsset));
			break;
		case SCENE_EDITOR_TARGET:
			m_vecSceneEditorAsset.insert(MAP_ASSET_VALUETYPE(_index, newBasicAsset));
			break;
		}

		ifLoad.close();
		CBasicAsset cResultAsset = *newBasicAsset;
		newBasicAsset = NULL;
		return cResultAsset;
	}

	SAFE_DELETE(newBasicAsset);
	ifLoad.close();
	return NULL;
}


bool CDBMgr::LoadUIFrame( INDEX _index, LPCTSTR pszFramePath, HWND _hWnd )
{
	//	리스트에서는 파일을 찾아서 그 경로를 넘겨주는 것으로 할 일을 다한다
	//	여기에서 그 패스의 파일을 찾아서 파일의 내용을 읽어와야한다

	std::ifstream	ifLoad(pszFramePath);
	std::string		strInput;

	CUIFrame* newFrame = new CUIFrame;
	CBasicAsset* newBasicAsset = NULL;

	bool bLoadComplete = true;
	POINT ptFramePos;
	ptFramePos.x = 0;
	ptFramePos.y = 0;

	if(ifLoad.good())
	{
		while(!ifLoad.eof())
		{
			std::getline(ifLoad, strInput, '\n');

			if(!strcmp(strInput.c_str(),"[REVISE_FRAME]"))
			{
				std::getline(ifLoad, strInput, ',');
				ptFramePos.x = atoi(strInput.c_str());
				std::getline(ifLoad, strInput, '\n');
				ptFramePos.y = atoi(strInput.c_str());
				std::getline(ifLoad, strInput, '\n');
			}

			if(!strcmp(strInput.c_str(), "[START]"))
			{
				newBasicAsset = new CBasicAsset;

				if(newBasicAsset->Load(ifLoad, ptFramePos.x, ptFramePos.y) != true)
				{
					SAFE_DELETE(newBasicAsset);
					ifLoad.close();
					bLoadComplete = false;
				}
				else
				{
					newBasicAsset->MakeOriginalFilePath();
					newBasicAsset->Run();
					newFrame->InsertBasicAsset(newBasicAsset);
				}
			}
		}

		// 패스 변경 : Release 
#ifdef RELEASE_PATH
		TCHAR szFramePath[MAX_PATH] = RESOURCE_DEFAULT_PATH;
		PathAppend(szFramePath, pszFramePath);
		std::string FramePath = CT2A( szFramePath );
#else
		std::string FramePath = CT2A( pszFramePath );
#endif
		newFrame->SetFrameFilePath(FramePath);
		m_MapUIFrame.insert(MAP_FRAME_VALUETYPE(_index, newFrame));
	}
	else
	{
		CString szWarning("다음 경로의 파일이 존재하지 않거나 깨진 파일입니다.\n");
		szWarning += pszFramePath;
		AfxMessageBox(szWarning);
		bLoadComplete = false;
	}

	ifLoad.close();
	return true;
}

bool CDBMgr::LoadUIScene( INDEX _index, LPCTSTR pszScenePath, HWND _hWnd )
{
	std::ifstream	ifLoad(pszScenePath);
	std::string		strInput;	

	CUIScene*	newScene = new CUIScene;

	//씬 파일 이름 저장
	CT2CA pszConvertedAnsiString (pszScenePath);
	std::string _Path(pszConvertedAnsiString);
	newScene->SetSceneFileName(_Path);

	CUIFrame*	newFrame = NULL;
	bool		bLoadFrame = true;

	while(!ifLoad.eof())
	{
		std::getline(ifLoad, strInput, '\n');

		if(!strcmp(strInput.c_str(), "[BACKGROUND]"))
		{
			std::getline(ifLoad, strInput, '\t');
			std::getline(ifLoad, strInput, '\n');
			strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
			strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
			newScene->SetBackgroundPath(strInput);
		}

		if(!strcmp(strInput.c_str(), "[BLUR IMAGE]"))
		{
			std::getline(ifLoad, strInput, '\t');
			std::getline(ifLoad, strInput, '\n');
			strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
			strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
			newScene->SetBlurImagePath(strInput);
		}

		if(!strcmp(strInput.c_str(), "[START]"))
		{
			newFrame = new CUIFrame;
			if(newFrame->Load(ifLoad) == false)
			{
				SAFE_DELETE(newFrame);
				SAFE_DELETE(newScene);
				ifLoad.close();
				bLoadFrame = false;
			}
			else
			{
				newFrame->SetMainFrmhWnd(_hWnd);
				newScene->InsertFrame(newFrame);
			}
		}
	}

	m_MapUIScene.insert(MAP_SCENE_VALUETYPE(_index, newScene));
	ifLoad.close();
	return true;
}


void CDBMgr::ClearMap()
{
	// 동적할당되어 있는 요소들을 전부 해제해준다.
	ClearTextureMap();
	ClearFrameMap();
	ClearSceneMap();
	MapClearFunc(m_vecSceneEditorAsset);
	MapClearFunc(m_MapAssetEditorAsset);

	if(m_MapPath.size() != 0)
	{
		m_MapPath.clear();
	}
}

CxImage* CDBMgr::GetTexture( INDEX _index )
{
	return m_MapTexture[_index];
}

CUIFrame* CDBMgr::GetUIFrame( INDEX _index )
{
	return m_MapUIFrame[_index];
}

CBasicAsset* CDBMgr::GetBasicAsset( INDEX _index, eAssetTarget _eTarget )
{
	return _eTarget == ASSET_EDITOR_TARGET ? m_MapAssetEditorAsset[_index] : m_vecSceneEditorAsset[_index];
}

void CDBMgr::ClearAssetMap(eAssetTarget _eTarget)
{
	ASSET_MAP* pTargetMap = _eTarget == ASSET_EDITOR_TARGET ? &m_MapAssetEditorAsset : &m_vecSceneEditorAsset;
	if(pTargetMap->size() != 0)
	{
		MAP_ASSET_ITER iter_begin = pTargetMap->begin();
		MAP_ASSET_ITER iter_end = pTargetMap->end();

		for(iter_begin; iter_begin != iter_end;)
		{
			SAFE_DELETE(iter_begin->second);
			iter_begin = pTargetMap->erase(iter_begin);
		}

		pTargetMap->clear();
	}
}

void CDBMgr::DeleteBasicAsset( INDEX _index, eAssetTarget _eTarget )
{
	ASSET_MAP* pTargetMap = _eTarget == ASSET_EDITOR_TARGET ? &m_MapAssetEditorAsset : &m_vecSceneEditorAsset;
	if(pTargetMap->size() != 0)
	{
		MAP_ASSET_ITER iter_find;
		iter_find = pTargetMap->find(_index);

		SAFE_DELETE(iter_find->second);
		pTargetMap->erase(iter_find);
	}
}

void CDBMgr::MapClearFunc( ASSET_MAP& _target )
{
	if(_target.size() != 0)
	{
		MAP_ASSET_ITER iter_begin = _target.begin();
		MAP_ASSET_ITER iter_end = _target.end();

		for(iter_begin; iter_begin != iter_end;)
		{
			SAFE_DELETE(iter_begin->second);
			iter_begin = _target.erase(iter_begin);
		}

		_target.clear();
	}
}

void CDBMgr::ClearTextureMap()
{
	if(m_MapTexture.size() != 0)
	{
		MAP_TEX_ITER iter_begin = m_MapTexture.begin();
		MAP_TEX_ITER iter_end = m_MapTexture.end();

		for(iter_begin; iter_begin != iter_end;)
		{
			SAFE_DELETE(iter_begin->second);
			iter_begin = m_MapTexture.erase(iter_begin);
		}

		m_MapTexture.clear();
	}
}

void CDBMgr::ClearFrameMap()
{
	if(m_MapUIFrame.size() != 0)
	{
		MAP_FRAME_ITER iter_begin = m_MapUIFrame.begin();
		MAP_FRAME_ITER iter_end = m_MapUIFrame.end();

		for(iter_begin; iter_begin != iter_end;)
		{
			iter_begin->second->Destroy();
			SAFE_DELETE(iter_begin->second);
			iter_begin = m_MapUIFrame.erase(iter_begin);
		}

		m_MapUIFrame.clear();
	}
}

void CDBMgr::ClearPathMap()
{
	if(m_MapPath.size() != 0)
	{
		m_MapPath.clear();
	}
}

void CDBMgr::ClearSceneMap()
{
	if(m_MapUIScene.size() != 0)
	{
		MAP_SCENE_ITER iter_begin = m_MapUIScene.begin();
		MAP_SCENE_ITER iter_end = m_MapUIScene.end();

		for(iter_begin; iter_begin != iter_end;)
		{
			iter_begin->second->Destroy();
			SAFE_DELETE(iter_begin->second);
			iter_begin = m_MapUIScene.erase(iter_begin);
		}

		m_MapUIScene.clear();
	}
}

CUIScene* CDBMgr::GetUIScene( INDEX _index )
{
	return m_MapUIScene[_index];
}

