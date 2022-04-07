#pragma once

class CUIFrame;
class CAssetBase;

class CUIScene
{
	typedef std::vector<CUIFrame*>				VEC_FRAME;
	typedef VEC_FRAME::iterator					VEC_FRAME_ITER;
	typedef VEC_FRAME::reverse_iterator			VEC_FRAME_REVERSE_ITER;

	VEC_FRAME	m_vecFrame;

private:
	std::string m_BackgroundPath;
	std::string m_BlurImagePath;
	std::string m_SceneFileName;

private:
	HWND	m_ParenthWnd;

public:
	void	Initialize();
	void	Copy(CUIScene* _Scene);
	void	InsertFrame(CUIFrame* _frame);
	void	InsertFrameToObjMgr();
	void	SetParenthWnd(HWND _hwnd);
	void	SetBackgroundPath(std::string _path);
	void	SetBlurImagePath(std::string _path);
	void    SetSceneFileName(std::string _path);
	void	Destroy();
	
public:
	std::string GetBackgroundPath();
	std::string GetBlurImagePath();
	std::string GetSceneFileName();

public:
	CUIScene(void);
	CUIScene(CUIScene* _Scene);
	virtual ~CUIScene(void);
};

