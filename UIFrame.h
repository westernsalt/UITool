#pragma once

class CBasicAsset;

class CUIFrame
{
	typedef	std::vector<CBasicAsset*>	VEC_ASSET;
	typedef VEC_ASSET::iterator			VEC_ASSET_ITER;
	typedef VEC_ASSET::reverse_iterator	VEC_ASSET_REVERSE_ITER;

private:
	std::string m_strFramePath;
	POINT	m_tPosition;
	POINT	m_tFrameSize;

	// 해상도 800*600 스케일 [6/4/2019 Gabrielle]
	float	m_fScaleX;
	float	m_fScaleY;
	POINT	m_tFrameOriginalSize;
	POINT	m_tFrameOriginalPos;

	RECT	m_tHitBox;

private:
	VEC_ASSET m_vecAsset;

private:
	HWND	m_MainFrmhWnd;
	int		m_iFocusAsset;
	int		m_iState;
	bool	m_bMove;
	POINT	m_ptMin;
	POINT	m_ptMax;

public:
	void SubstractPosition(POINT _pos);
	void SetPosition(POINT _pos);
	void SetState(eAssetState _state);
	void SetScale(float _fScaleX = 1, float _fScaleY = 1);
	void SetHitBox();
	void SetFrameFilePath(std::string _path);
	void CheckState();
	void InsertBasicAsset(CBasicAsset* _asset);
	bool CollisionCheck(POINT _pos);
	void ConvertFilePathToRelative();
	void CalculateAssetSize();
	std::string GetFrameFileName();

public:	 
	void Initialize();
	void Run();
	void Render(HDC _DC, eAssetTarget _target, bool _renderindex);
	void Save(std::ofstream& _OfSave);
	bool Load(std::ifstream& _IfLoad);
	bool LoadFrame(std::string _Path);
	void Copy(CUIFrame* _Frame);
	void Destroy();

public:	 
	void ConvertFilePath();
	void MakeOriginalFilePath();
	void MakeOriginalFilePathToAbsolute();
	void SetMainFrmhWnd(HWND _hWnd);

public:
	POINT GetPosition() { return m_tPosition; }
	POINT GetSize();
	RECT  GetHitBox() { return m_tHitBox; }
	float GetScaleX();
	float GetScaleY();

	// 해상도 800*600 스케일 [6/4/2019 Gabrielle]
	POINT GetOriginSize();

public:
	CUIFrame(void);
	CUIFrame(CUIFrame* _Frame);
	~CUIFrame(void);
};

