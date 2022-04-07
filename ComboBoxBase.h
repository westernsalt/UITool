#pragma once

#include "baseinterface.h"

class CTextBox;
class CBtnBase;
class CListBoxBase;

class CComboBoxBase :
	public CBaseInterface
{
	CBtnBase*		m_pNormalBtn;
	CBtnBase*		m_pOpenBtn;
	CListBoxBase*	m_pDropDownListBox;

private:
	bool	m_bDropDown;

private:
	std::string m_strFilePath;
	std::string m_strListBoxPath;

public:
	void SetFilePath(std::string _path);
	void SetComboBoxProperty(BYTE _red, BYTE _green, BYTE _blue, int _sort);
	void SetListBoxBase(CListBoxBase* _base);
	void SetNormalBtnProperty(POINT _normaltex, POINT _pressedtex,
		POINT _disabletex, POINT _highlighttex, POINT _BtnSize);
	void SetOpenBtnProperty(POINT _normaltex, POINT _pressedtex,
		POINT _disabletex, POINT _highlighttex,POINT _BtnSize);
	void SetTextPreview(CString _Preview);
	void SetDisableText(BYTE _red, BYTE _green, BYTE _blue);
	void SetbDisable(bool _disable);
	void SetTextIndent(POINT _indent);

	POINT GetTextIndent();

public:
	virtual void SetParenthWnd(HWND _hWnd);

public:
	CTextBox	GetTextBox();
	CBtnBase*	GetNormalBtn();
	CBtnBase*	GetOpenBtn();
	std::string	GetFilePath();
	std::string	GetListBoxPath();

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
	virtual bool SubstractPosition(POINT _pos);
	virtual void MakeOriginialFilePath();
	virtual void ConvertFilePath();
	virtual void SetPosition(POINT _pt);

	// 800*600 [6/7/2019 Gabrielle]
	virtual void SetScale(float _fscaleX, float _fscaleY);

public:
	CComboBoxBase(void);
	virtual ~CComboBoxBase(void);
};

