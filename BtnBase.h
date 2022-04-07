#pragma once

#include "AssetBase.h"
#include "TextBox.h"

class CBtnBase : public CAssetBase
{
protected:
	POINT		m_ptNormalTex;
	POINT		m_ptPressedTex;
	POINT		m_ptDisableTex;
	POINT		m_ptHighlightTex;

	POINT		m_tSize;
	CTextBox	m_TextBox;

	bool	m_bTextBoxSetApart;

private:
	//	아이콘과 텍스트박스를 분리시킨다
	bool	m_bIconFont;
	int		m_iIconIndent;

public:
	void	SetTextBoxPosition();
	void	SetbIconFont(bool _set);
	void	SetbTextBoxSetApart(bool _set);
	void	SetTextboxIndent(int _indent);


	// 행간 추가 [7/16/2019 Gabrielle]
	void	SetTextBoxVerticalIndent(int _iIndent);

public:
	bool	GetTextBoxSetApart();
	bool	GetIconFront();
	int		GetIconIndent();

public:
	virtual void Initialize();
	virtual void Run();
	virtual void Render(HDC _DC, eAssetTarget _target);
	virtual void Save(std::ofstream& _OfSave);
	virtual void Load(std::ifstream& _IfLoad);
	virtual void Copy(CBaseInterface* _Copy);
	virtual void SetPosition(POINT _pt);
	virtual void SetHitBox();

	// 해상도 작업 진행중 [6/7/2019 Gabrielle]
	virtual void SetScale(float _fscaleX, float _fscaleY);

public:
	void SetButtonProperty(POINT _normaltex, POINT _pressedtex,
		POINT _disabletex, POINT _highlighttex, POINT _BtnSize);
	void SetText(BYTE _r, BYTE _g, BYTE _b, int _sort);
	void SetDisableText(BYTE _r, BYTE _g, BYTE _b);
	void SetTextPreview(CString _string);
	void SetbDisable(bool _disable);
	void SetTextIndent(POINT _indent);
	void SetTextSize(int _iSize);
	void SetDefaultShow(bool _show);
	void SetFontBold(bool _bold);

	int			GetTextSize();
	bool		GetDefaultShow()	{ return m_TextBox.GetbDefaultShow(); }
	bool		GetFontBold()		{ return m_TextBox.GetFontBold(); }
	POINT		GetNormalPos()		{ return m_ptNormalTex; }
	POINT		GetPressedPos()		{ return m_ptPressedTex; }
	POINT		GetDisablePos()		{ return m_ptDisableTex; }
	POINT		GetHighlightPos()	{ return m_ptHighlightTex; }
	POINT		GetSize()			{ return m_tSize; }
	CTextBox	GetTextBox()		{ return m_TextBox; }

public:
	CBtnBase(void);
	virtual ~CBtnBase(void);
};

