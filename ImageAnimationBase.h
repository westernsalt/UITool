#pragma once

#include "AssetBase.h"
#include <MMSystem.h>
#pragma comment(lib, "winmm.lib")


class CImageAnimationBase : public CAssetBase
{
private:
	POINT m_ptTextureSize;		//선택한 텍스쳐 전체 사이즈
	POINT m_ptImageStartPos;	//이미지 시작위치
	POINT m_ptImageInterval;	//이미지 간의 간격
	POINT m_ptImageSize;		//이미지 한 장당 사이즈	
	POINT m_ptImageNum;			//이미지 가로, 세로 갯수
	POINT m_ptCurrentImageStartPos;	//현재 이미지 시작위치
	int m_iImageTotalNum;		//전체 이미지 갯수
	float m_fDelayTime;			//이미지간의 딜레이 타임
	int m_iLoop;				//애니메이션 루프 여부
	int m_iImageCount;			//현재 찍고 있는 이미지 장수
	int m_iColPos;				//현재 세로 줄 수 
	int m_iRowCount;			//현재 가로 이미지 위치
	float m_fStartTime;			//Delay시작 시간

public:
	void SetImageAnimationProperty(POINT textureSize, POINT imageStartPos, POINT imageInterval, POINT imageSize, POINT imageNum, 
								   int imageTotalNum, float delayTime, int loop);
	//void ChangeTexture();

public:
	POINT	GetImageSpace() { return m_ptImageInterval; }
	POINT	GetSingleImageSize() { return m_ptImageSize; }
	POINT	GetImageCount() { return m_ptImageNum; }
	POINT	GetTextureSize(){ return m_ptTextureSize; }
	POINT	GetImageStartPos() {return m_ptImageStartPos; }
	float	GetDelayTime()	{ return m_fDelayTime; }
	int		GetAniLoop()	{ return m_iLoop; }
	int		GetImageTotalCount() { return m_iImageTotalNum; }
	RECT	GetSingleImageSizeToRect();
	void ChangeTexture();

public:
	virtual void Initialize();
	virtual void Run();
	virtual void Render(HDC _DC, eAssetTarget _target);
	virtual void Save(std::ofstream& _OfSave);
	virtual void Load(std::ifstream& _IfLoad);
	virtual void Copy(CBaseInterface* _Copy);
	virtual void SetPosition(POINT _pt);


public:
	CImageAnimationBase(void);
	virtual ~CImageAnimationBase(void);
};