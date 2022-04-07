#pragma once
class CRadioBtnBase : public CCheckBoxBase
{
	POINT	m_ptGroupIntervalPos;

public:
	virtual void	Render(HDC _DC, eAssetTarget _target);
	virtual void	Save(std::ofstream& _OfSave);
	virtual void	Load(std::ifstream& _IfLoad);
	virtual void	Copy(CBaseInterface* _Copy);
	virtual void	CheckState();

public:
	void	SetGroupIntervalPos(POINT _pos);
	POINT	GetGroupIntervalPos();
	void	SetChecked(bool _checked);
	bool	GetChecked() { return m_bChecked; }
	void	SetText(BYTE _r, BYTE _g, BYTE _b, int _sort);

public:
	CRadioBtnBase(void);
	~CRadioBtnBase(void);
};

