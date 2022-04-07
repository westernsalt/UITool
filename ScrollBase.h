#pragma once

#include "baseinterface.h"
#include "BtnBase.h"

class CScrollBase :
	public CSliderBase
{
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
	virtual bool SubstractPosition(POINT _pos);
	virtual void AddPosition(POINT _pos);
	virtual void SetPosition(POINT _pt);
	virtual void SetScale(float _fscaleX, float _fscaleY);

public:
	void	SetScrollProperty();
	void	MakeOriginialFilePath();
	void	ConvertFilePath();
	void	SetFilePath(std::string _Path);
	int		GetGauge();
	bool	GetButtonClicked(){ return m_bBtnPressed; }

public:
	CScrollBase(void);
	virtual ~CScrollBase(void);
};

