#pragma once
#include "baseinterface.h"

#include "BtnBase.h"

class CSliderBase :
	public CBaseInterface
{
	CAssetBase*		m_pSliderGauge;

protected:
	CBtnBase		m_Button;
	CAssetBase*		m_pBackground;

	float			m_fGaugeScale;
	POINT			m_ptGaugeStartPos;
	int				m_iGauge;
	bool			m_bBtnPressed;
	std::string		m_strTexPath;

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

	// 800*600 [6/7/2019 Gabrielle]
	virtual void SetScale(float _fscaleX, float _fscaleY);

public:
	//	����������� �Լ���
	void SetBtn(POINT _normaltex,POINT _pressedtex,POINT _disabletex,POINT _highlighttex,POINT _btnSize);
	void SetBackGround(SaveData _data, POINT _size);
	void SetSliderGauge(SaveData _data, POINT _size, POINT _startPos);
	void SetSliderProperty();
	void SetFilePath(std::string _Path);
	void SetBasePosition();

public:
	//	Getter �Լ�
	//	���̾�α׹� ���������� ���� �����������
	std::string GetFilePath(){ return m_strTexPath; }
	CBtnBase*	GetSliderBtnBase() { return &m_Button; }
	CAssetBase*	GetGaugeBar(){ return m_pSliderGauge; }
	CAssetBase*	GetBackgroundBar(){ return m_pBackground; }
	POINT		GetGaugeStartPos(){ return m_ptGaugeStartPos; }

public:
	void	CalculateSliderBtnPos();
	void	ConvertFilePath();
	void	MakeOriginialFilePath();

public:
	CSliderBase(void);
	virtual ~CSliderBase(void);
};

