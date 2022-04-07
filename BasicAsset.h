#pragma once

#include "BaseInterface.h"

#define TEXTBOX_COUNT		10

class CBasicAsset
{
	//	rai파일을 복수의 형태로 가진다.
protected:
	char			m_szFilePath[MAX_PATH];
	char			m_szFileName[MAX_PATH];
	CBaseInterface*	m_Asset;
	
	eAssetType	m_eAssetType;
	POINT		m_tStartPos;
	POINT		m_tPosition;

	float		m_fScale_X;
	float		m_fScale_Y;

	int			m_iState;

	RECT		m_tHitBox;
	RECT		m_tAssetSize;

	// 해상도 지원 문제 [6/7/2019 Gabrielle]
	POINT		m_ptOriginPos;

public:
	HWND		m_ParenthWnd;

	POINT		GetOriginPos();
	void		SetPosition(POINT _ptPos);
	void		SetOriginPos(POINT _ptPOs);

public:
	void	Initialize();
	void	Run();
	void	Render(HDC _DC, eAssetTarget _target);
	void	Save(std::ofstream& _OfSave, int _SubsX = 0, int _SubsY = 0);
	bool	Load(std::ifstream& _IfLoad, int _SubX = 0, int _SubY = 0);
	bool	LoadAsset(std::string _Path);
	void	CheckState();

	void	InsertAsset(CBaseInterface* _asset);
	void	InitializeAssetBase();
	bool	SubsAssetBasePos(POINT _pos, POINT _movePos);
	void	AddAssetBasePos(POINT _pos);
	void	SetState(eAssetState _state);
	void	SetAssetType(eAssetType _type);
	void	SetParenthWnd(HWND _hWnd);
	void	FreeFocus();

public:
	void		AddPosition(POINT _pos);
	void		SubstractPosition(POINT _pos);

	int				GetState()			{ return m_iState; }
	POINT			GetPosition()		{ return m_tPosition; }
	float			GetScaleX()			{ return m_fScale_X; }
	float			GetScaleY()			{ return m_fScale_Y; }
	RECT			GetFileSize();
	RECT*			GetHitBox();
	std::string		GetFilePath();
	std::string		GetFileName();
	eAssetType		GetAssetType();
	CBaseInterface*	GetAssetBase()	{	return m_Asset;	}

	void	CalculatePosition(POINT _pos);
	void	InitPosition();
	void	SetScale(float _fx, float _fy);
	void	SetHitBox();
	void	SetAssetSize();
	void	SetFilePath(std::string _Path);
	void	SetFileName(std::string _Name);
	void	ConvertFilePath();
	void	MakeOriginalFilePath();
	void	AssetBasePathSetting();

public:
	CBasicAsset(void);
	CBasicAsset(CBasicAsset* _asset);

	//	후에 이 클래스를 상속받는 아이가 있을 경우에는 반드시, 
	//	반드시 소멸자를 virtual로 선언해야한다
	~CBasicAsset(void);
};

