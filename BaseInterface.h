#pragma once

struct SaveData
{
	std::string		m_szFilePath;
	std::string		m_szTexFilePath;
	std::string		m_szFolderPath;

	POINT			m_tStartPos;
	//	화면에서 Collision과 렌더를 하기 위한 멤버변수

	SaveData();
	~SaveData();
	void Initialize();
};

class CBaseInterface
{
protected:
	int		m_iAssetType;
	float	m_fScale_X;
	float	m_fScale_Y;
	POINT	m_tPosition;
	
	// 800*600 변경 이전 포지션 저장 [6/7/2019 Gabrielle]
	POINT	m_tOriginPos;

	RECT	m_tHitBox;
	RECT	m_tFileSize;
	
	int		m_iState;
	HWND	m_ParenthWnd;
	bool	m_bDisable;

public:
	virtual	void Initialize();
	virtual void Run() {}
	virtual void Render(HDC _DC, eAssetTarget _target) {}
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
	virtual	void FreeFocus(){}
	virtual POINT GetPosition()	{ return m_tPosition; }
	virtual void SetbDisable(bool _disable) { m_bDisable = _disable; }

	virtual void SetScale(float _fscaleX, float _fscaleY);
	void		SetAssetType(eAssetType _Att);
	
	eAssetType	GetAssetType();
	RECT*		GetHitBox();
	RECT		GetFileSize()	{ return m_tFileSize; }
	int			GetState()		{ return m_iState; }
	float		GetScaleX()		{ return m_fScale_X; }
	float		GetScaleY()		{ return m_fScale_Y; }

	// 800*600 [6/7/2019 Gabrielle]
	void SetOriginPos(POINT _ptPos);
	POINT GetOriginPos();

public:
	CBaseInterface(void);
	virtual ~CBaseInterface(void);
};

