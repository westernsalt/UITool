#pragma once
#include "textbox.h"

class CAssetBase;

class CInputText :
	public CTextBox
{
	CAssetBase*		m_pTextBoxBG;
	std::string		m_pTexturePath;
	int				m_iInterval;
	bool			m_bFocus;
	bool			m_bPassword;
	bool			m_bFontBold;

public:
	void		SetFocus();
	void		InputText(std::string _text);
	void		SetbPassword(bool _bPassword);
	void		SetFontBold(bool _bBold);
	void		SetInterval(int _interval);
	void		SetFilePath(std::string _path) { m_pTexturePath = _path; }
	void		SetBackground(std::string _path, POINT _startpos, POINT _size);
	void		SetInputTextProperty(RECT _rtBox, BYTE _red, BYTE _green, BYTE _blue, UINT _SortFlag);

public:
	POINT		GetBackgroundStartPos();
	RECT		GetBackgroundSize();
	int			GetInterval();
	bool		GetbPassword();
	bool		GetFontBold();
	std::string	GetFilePath();
	
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
	virtual void FreeFocus();

	// 800*600 [6/7/2019 Gabrielle]
	virtual void SetScale(float _fscaleX, float _fscaleY);

public:
	CInputText(void);
	virtual ~CInputText(void);
};

