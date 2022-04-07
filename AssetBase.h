#pragma once
//저장할것과 안할것의 구분을 위해

#include "BaseInterface.h"
#include "TextBox.h"

class CAssetBase : public CBaseInterface
{
protected:
	SaveData	m_AssetBaseSaveData;
	CxImage*	m_cTexture;
	bool		m_bResult;
	eAssetState	m_eState;
	POINT		m_ptTextureSize;
	float		m_fFrameScaleX;
	float		m_fFrameScaleY;

public:
	void		SetAsset(std::string _path, eAssetType _Attribute, RECT _FileSize,
							POINT _StartPos,  float _fScaleX, float _fScaleY);

	void		SetFolderPath(std::string _strPath);

	void		SetHitBox();
	void		SetFileSize(RECT _FileSize);
	void		SetFilePath(std::string _path);
	void		SetTexFilePath(std::string _strPath);
	void		SetPosition(POINT _pos) { m_tPosition = _pos; }	

	void		ConvertFilePath();
	void		MakeOriginialFilePath();

	std::string	GetFilePath();
	RECT		GetFileSize()	{ return m_tFileSize; }
	POINT		GetStartPos()	{ return m_AssetBaseSaveData.m_tStartPos; }
	POINT		GetPosition()	{ return m_tPosition; }

public:
	virtual		void Initialize();
	virtual		void Run();
	virtual		void Render(HDC _DC, eAssetTarget _target);
	virtual		void Save(std::ofstream& _OfSave);
	virtual		void Load(std::ifstream& _IfLoad);
	virtual		void CheckState();
	virtual		void Copy(CBaseInterface* _copy);
	virtual		void SetFrameScale(float _fscaleX, float _fscaleY);

	void		CalculatePosition(POINT _pos);
	void		MultiplyPosition(float _fx, float _fy);

	SaveData	GetSaveData() {return m_AssetBaseSaveData; }
	void		SetSaveData( SaveData _Data ) { m_AssetBaseSaveData = _Data; }

public:
	CAssetBase(void);
	CAssetBase(CAssetBase* _asset);
	virtual ~CAssetBase(void);
};

