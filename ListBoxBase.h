#pragma once
#include "baseinterface.h"

class CScrollBase;
class CAssetBase;
class CListItemBase;

class CListBoxBase :
	public CBaseInterface
{
#define SCROLL_AMOUNT		1

	typedef std::vector<CListItemBase*>				VEC_LISTITEM;
	typedef std::vector<CListItemBase*>::iterator	VEC_LISTITEM_ITER;

	CAssetBase*		m_pBackground;
	CScrollBase*	m_pScroll;
	CListItemBase*	m_pListItem;

	int				m_iListItemCount;
	RECT			m_rtListBoxArea;
	POINT			m_ptInterval;
	POINT			m_ptListItemStartPos;
	std::string		m_strFilePath;
	std::string		m_strScrollPath;
	std::string		m_strListItemPath;

private:
	//	리스트 내부 간격 및 배치 계산용 변수들
	POINT	m_ptListItemCount;
	int		m_iScrollAmount;
	int		m_iScrollArea;

private:
	//	직접적으로 외부에서 만들어서 주는것이 아니라 내부에서 만들어서 쓴다.
	VEC_LISTITEM	m_vecListItem;
	VEC_LISTITEM	m_vecDisplayList;


public:
	virtual	void Initialize();
	virtual void Run();
	virtual void Render(HDC _DC, eAssetTarget _target);
	virtual void Copy(CBaseInterface* _Copy);
	virtual	void Save(std::ofstream& _OfSave);
	virtual void Load(std::ifstream& _IfLoad);
	virtual void SetState(eAssetState _state);
	virtual void CheckState();
	virtual void SetHitBox();
	virtual void SetParenthWnd(HWND _hWnd);
	virtual void AddPosition(POINT _pos);
	virtual bool SubstractPosition(POINT _pos);
	virtual void MakeOriginialFilePath();
	virtual void ConvertFilePath();
	virtual void SetPosition(POINT _pt);

	//  [6/7/2019 Gabrielle]
	virtual void SetScale(float _fscaleX, float _fscaleY);

public:
	void	CalculateScrollScale();
	void	SettingListItem();
	void	ClearListItemVector();
	void	ClearDisplayList();
	void	SetDisplayList();

	void	SetScrollBase(CScrollBase* _base);
	void	SetBackground(CAssetBase* _base);
	void	SetListItemBase(CListItemBase* _base);

	void	SetFilePath(std::string _Path);
	void	SetScrollPath(std::string _Path);
	void	SetListItemPath(std::string _Path);

	void	SetListItemCount(int _iCount);
	void	SetListBoxInterval(int _col, int _row);
	void	SetListBoxArea(int _xpos, int _ypos, int _width, int _height);
	void	SetListBoxProperty(int _itemcount, std::string _bgPath, POINT _startPos, RECT _listboxarea, POINT _interval, POINT _ListitemPos);


	std::string	GetFilePath();
	std::string GetScrollPath();
	std::string GetListItemPath();

	int			GetListBoxItemCount();
	POINT		GetBackgroundPos();
	RECT		GetBackgroundSize();
	RECT		GetListBoxAreaSize();
	POINT		GetListBoxInterval();
	POINT		GetListItemStartPos();

public:
	CListBoxBase(void);
	virtual ~CListBoxBase(void);
};

