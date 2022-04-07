#include "StdAfx.h"
#include "UIScene.h"
#include "UIFrame.h"
#include "ObjMgr.h"


CUIScene::CUIScene(void)
{
	m_SceneFileName = "";
}

CUIScene::CUIScene( CUIScene* _Scene )
{
	m_BackgroundPath = _Scene->m_BackgroundPath;
	m_BlurImagePath = _Scene->m_BlurImagePath;

	int iCount = _Scene->m_vecFrame.size();
	for(int i = 0 ; i < iCount; i++)
	{
		CUIFrame* newFrame = new CUIFrame(_Scene->m_vecFrame[i]);
		m_vecFrame.push_back(newFrame);
	}

	std::cout << "Scene카피 성공" << std::endl;
}

CUIScene::~CUIScene(void)
{
}

void CUIScene::InsertFrame( CUIFrame* _frame )
{
	m_vecFrame.push_back(_frame);
}

void CUIScene::Copy(CUIScene* _Scene)
{
	int iCount = _Scene->m_vecFrame.size();
	for(int i = 0 ; i < iCount; i++)
	{
		CUIFrame* newFrame = new CUIFrame(_Scene->m_vecFrame[i]);
		newFrame->Run();
		m_vecFrame.push_back(newFrame);	
	}
}

void CUIScene::SetParenthWnd( HWND _hwnd )
{
	m_ParenthWnd = _hwnd;

	int iCount = m_vecFrame.size();
	for(int i = 0; i < iCount; i++)
	{
		m_vecFrame[i]->SetMainFrmhWnd(m_ParenthWnd);
	}
}

void CUIScene::Destroy()
{
	int iCount = m_vecFrame.size();
	for(int i = 0; i < iCount; i++)
	{
		SAFE_DELETE(m_vecFrame[i]);
	}

	m_vecFrame.clear();
}

void CUIScene::Initialize()
{
	int iCount = m_vecFrame.size();
	for(int i = 0; i < iCount; i++)
	{
		m_vecFrame[i]->Initialize();
	}
}

void CUIScene::InsertFrameToObjMgr()
{
	int iCount = m_vecFrame.size();
	for(int i = 0; i < iCount; i++)
	{
		CObjMgr::GetInstance()->InsertUIFrame(m_vecFrame[i]);
	}
}

void CUIScene::SetBackgroundPath( std::string _path )
{
	m_BackgroundPath = _path;
}

void CUIScene::SetBlurImagePath(std::string _path )
{
	m_BlurImagePath = _path;
}

void CUIScene::SetSceneFileName(std::string _path)
{
	std::string strFileName = _path;
	int nPos = strFileName.rfind("\\");
	strFileName.erase(0, nPos+1);

	m_SceneFileName = strFileName;
}

std::string CUIScene::GetBackgroundPath()
{
	return m_BackgroundPath;
}

std::string CUIScene::GetBlurImagePath()
{
	return m_BlurImagePath;
}

std::string CUIScene::GetSceneFileName()
{
	return m_SceneFileName;
}
