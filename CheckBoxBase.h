#pragma once
#include "btnbase.h"
class CCheckBoxBase :
	public CBtnBase
{
protected:
	POINT	m_ptCheckMarkTex;
	POINT	m_ptCheckMarkSize;
	POINT	m_ptCheckMarkPos;
	POINT	m_ptIconIndent;
	int		m_iPrevState;
	int		m_iInterval;
	bool	m_bIconFront;
	bool	m_bChecked;
	bool	m_bBtnFrontPos;

	CAssetBase* m_IconAssetBase;
	CAssetBase* m_IconDisableAssetBase;

	std::string m_strIconAssetPath;
	std::string m_strIconDisableAssetPath;

public:
	virtual void Initialize();
	virtual void Run();
	virtual void Render(HDC _DC, eAssetTarget _target);
	virtual void Save(std::ofstream& _OfSave);
	virtual void Load(std::ifstream& _IfLoad);
	virtual void Copy(CBaseInterface* _Copy);
	virtual void SetState(eAssetState _state);
	virtual void SetPosition(POINT _pt);
	virtual bool SubstractPosition(POINT _pos);

	// 800*600 [6/7/2019 Gabrielle]
	virtual void SetScale(float _fscaleX, float _fscaleY);

public:
	void SetCheckBoxProperty(POINT _normaltex, POINT _pressedtex,
		POINT _disabletex, POINT _highlighttex, POINT _checkmarktex, POINT _BtnSize, POINT _CheckMarksize ,int _interval);
	void SetText(BYTE _r, BYTE _g, BYTE _b, int _sort);
	void SetIconAsset(CAssetBase* _base, eIconPosition _iconPos );
	void SetDisableIconAsset(CAssetBase* _base, eIconPosition _iconPos);
	void SetIconAssetPath(std::string _path);
	void SetIconDisableAssetPath(std::string _path);
	void SetIconIndent(POINT _indent);
	void SetIconPosition(eIconPosition _iconPos);
	void CalculateTextBox();
	void CalculateHitBox();
	void SetButtonPosition(eIconPosition btnPos);

	CTextBox* GetpTextBox(){ return &m_TextBox; }
	POINT GetCheckMarkPos() { return m_ptCheckMarkTex; }
	POINT GetCheckMarkSize() { return m_ptCheckMarkSize; }
	int	  GetInterval(){ return m_iInterval; }

	bool GetbIconFront() { return m_bIconFront; }
	POINT GetIconIndent() { return m_ptIconIndent; }
	std::string GetIconAssetPath() {  return m_strIconAssetPath; }
	std::string GetIconDisablePath() { return m_strIconDisableAssetPath; }
	bool GetBtnPositon()	{ return m_bBtnFrontPos; }

public:
	CCheckBoxBase(void);
	virtual ~CCheckBoxBase(void);
};