#pragma once
#include "baseinterface.h"
class CListItemBase :
	public CBaseInterface
{
	typedef std::vector<CBaseInterface*>					VEC_LISTITEM;
	typedef std::vector<CBaseInterface*>::iterator			VEC_LISTITEM_ITER;
	typedef std::vector<CBaseInterface*>::reverse_iterator	VEC_LISTITEM_REVERSE_ITER;

	VEC_LISTITEM	m_vecListItem;
	bool			m_bZeroPosLock;
	POINT			m_ptMin;
	POINT			m_ptMax;
	int				m_iPrevState;
	int				m_iFocusIndex;	
	int				m_iCurDlgBarIndex;

	bool			m_bSceneEditor;

	// 800*600 [6/7/2019 Gabrielle]
	POINT			m_ptOriginPos;

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
	virtual POINT GetPosition();

	// 800*600 [6/7/2019 Gabrielle]
	virtual void SetScale(float _fscaleX, float _fscaleY);
	POINT	GetOriginPos();

public:
	void		InsertListItem(CBaseInterface* _pitem);
	void		CalculateAssetSize();
	POINT		CalculateAssetMinPos();
	bool		DeleteFocusItem();
	void		SetFocusAssetPos(POINT _ptPos);

	void		ChangeListitemDepth(int nSelectIndex, int iChangePos);
	void		SetFocusItemIndex(int nIndex);		//포커스한 아이템 인데스 저장
	int			GetFocusItemIndex();				//포커스한 아이템 인데스 가져오기
	std::vector<CBaseInterface*> GetListItemList();	//리스트아이템 리스트 가져오기
	void		SetFocusToNone();
	void		SetZeroPosLock(bool _bLock);

public:
	CListItemBase(void);
	virtual ~CListItemBase(void);
};

