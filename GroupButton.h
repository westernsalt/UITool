#pragma once

class CBtnBase;

class CGroupButton : public CBaseInterface
{
	CBtnBase*	m_GroupBtn;
	int			m_iFocusIndex;		//	현재 그룹버튼 내에서 포커스를 받은 버튼.
	int			m_iBtnCount;
	int			m_iMaxBtnCount;
	int			m_iBtnCol;
	int			m_iBtnRow;
	int			m_iInterval;
	int			m_iTextBoxInterval;

public:
	void		Create(eAssetType _type, int _col, int _row);

public:
	virtual	void Initialize();
	virtual void Run();
	virtual void Render(HDC _DC, eAssetTarget _target);
	virtual void Copy(CBaseInterface* _Copy);
	virtual	void Save(std::ofstream& _OfSave);
	virtual void Load(std::ifstream& _IfLoad);

	virtual void CheckState();
	virtual void SetHitBox();
	virtual void SetState(eAssetState _state);
	virtual void SetParenthWnd(HWND _hWnd);
	virtual bool SubstractPosition(POINT _pos);
	virtual void SetPosition(POINT _pt);
	virtual void SetbDisable(bool _disable);

	// 800*600 [6/7/2019 Gabrielle]
	virtual void SetScale(float _fscaleX, float _fscaleY);
	
	void	AdjustGroupBtnPos();
	void	SetParenthWndToGroup();
	void	MakeOriginialFilePath();
	void	ConvertFilePath();
	void	SetBtnInterval(int _iInterval);
	void	SetTextSize(int _iSize);

public:
	CBtnBase*	GetGroupBase();
	std::string	GetFilePath();
	int			GetFocusIndex(){ return m_iFocusIndex; }
	int			GetTextBoxInterval(){ return m_iTextBoxInterval; }
	int			GetBtnCol() { return m_iBtnCol; }
	int			GetBtnRow() { return m_iBtnRow; }
	int			GetGroupInterval() { return m_iInterval; }
	POINT		GetRecentBtnSize();
	int			GetBtnVerticalIndent();

public:
	void	SetTextPreview(CString _preview, eAssetType _type);
	void	SetFilePath(std::string _path);
	void	SetCol(int _col) { m_iBtnCol = _col; }
	void	SetRow(int _row) { m_iBtnRow = _row; }
	void	SetFocusIndex(int _index);
	void	SetIconAsset(CAssetBase* _base, eIconPosition _iconPosition, std::string _assetPath);
	void	SetIconPosition(eIconPosition _iconPos);
	void	SetIconIndent(POINT _indent);
	
	void	InsertGroupedBtn(CBtnBase* _btn, HWND _parenthWnd);
	void	SetGroupBtnProperty(POINT _normal, POINT _press, POINT _disable, POINT _highlight,
		POINT _checkmark, POINT _size, POINT _checkmarkSize, int _interval, eAssetType _type);
	void	SetText(BYTE _red, BYTE _green, BYTE _blue, int _array, eAssetType _type);
	void	SetDisableText(BYTE _red, BYTE _green, BYTE _blue);
	void	SetDefaultShow(bool _show);
	void	SetFontBold(bool _bold);
	void	SetTextBoxVerticalIndent(int _iIndent);

public:
	CGroupButton(void);
	~CGroupButton(void);
};

