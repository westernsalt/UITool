#pragma once
#include "baseinterface.h"
class CProgressBarBase :
	public CBaseInterface
{
	CAssetBase*		m_pBackground;
	CAssetBase*		m_pExtraGauge;

protected:
	POINT		m_ptGaugeIndent;
	int			m_iGauge;
	int			m_iExtraGauge;
	float		m_fExtraGaugeScale;
	std::string	m_strTexPath;

protected:
	typedef std::vector<CAssetBase*>		VEC_GAUGEBAR;
	typedef VEC_GAUGEBAR::iterator			VEC_GAUGEBAR_ITER;
	typedef VEC_GAUGEBAR::reverse_iterator	VEC_GAUGEBAR_REVITER;

	typedef std::vector<float>				VEC_GAUGEBAR_SCALE;
	typedef VEC_GAUGEBAR_SCALE::iterator	VEC_GAUGEBAR_SCALE_ITER;

	VEC_GAUGEBAR		m_vecGaugeBar;
	VEC_GAUGEBAR_SCALE	m_vecGaugeBarScale;

	//	현재 마지막으로 표시되고 있는 게이지가 뭔지.
	//int	m_iCurGauge;
	int		m_iVecCount;

public:
	void	InsertGaugeBar(CAssetBase* _base);
	void	DeleteGaugeBar(int _index);
	void	SelectGaugeBarUp(int _index);
	void	SelectGaugeBarDown(int _index);

private:
	HWND		m_ParenthWnd;

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

	// 800*600 작업 [6/7/2019 Gabrielle]
	virtual void SetScale(float _fscaleX, float _fscaleY);

private:
	void SetGaugeScale();

public:
	//	멤버변수셋팅 함수들
	void SetProgressBar(std::string _path, POINT _size, POINT _startPos);
	void SetGauge(std::string _path, POINT _size, POINT _startPos, POINT _GaugeIndent);
	void SetExtraGauge(std::string _path, POINT _size, POINT _startPos);
	void SetProgressbarProperty();
	void SetIndent(POINT _indent);
	void SetFilePath(std::string _Path);
	void SetGaugeAmount(int _iNum);
	void SetExtraGaugeAmount(int _iNum);
	void DestroyGaugeBar();

public:
	//	Getter 함수
	//	다이얼로그바 정보갱신을 위한 데이터참고용
	int			GetGaugePercentage() { return m_iGauge; }
	int			GetExtraGaugePercentage(){ return m_iExtraGauge; }
	std::string GetFilePath(){ return m_strTexPath; }
	CAssetBase*	GetBackgroundBar(){ return m_pBackground; }
	CAssetBase* GetExtraGaugeBar(){ return m_pExtraGauge; }
	POINT		GetGaugeIndent(){ return m_ptGaugeIndent; }
	VEC_GAUGEBAR* GetGaugeBarVector() { return &m_vecGaugeBar; }

public:
	void	ConvertFilePath();
	void	MakeOriginialFilePath();

public:
	CProgressBarBase(void);
	virtual ~CProgressBarBase(void);
};

