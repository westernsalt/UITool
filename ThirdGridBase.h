#pragma once

class CThirdGridBase : public CAssetBase
{
private:	
	POINT		m_ptFirstImageStartPos;		//첫번째 이미지 시작 위치
	POINT		m_ptSecondImageStartPos;	//두번째 이미지 시작 위치
	POINT		m_ptThirdImageStartPos;		//세번째 이미지 시작 위치
	POINT		m_ptImageSize;				//이미지 크기
	eArrayType	m_eScaleType;				//스케일 타입(가로, 세로)
	int			m_iPixel;					//스케일될 Pixel값

public:
	//3그리드 프로퍼티값 세팅
	void SetThirdGridProperty(POINT imageSize, POINT firstImageStartPos, POINT secondImageStartPos, POINT thirdImageStartPos, eArrayType eScaleType, int pixel);	

public:
	POINT	GetFirstImagePos() { return m_ptFirstImageStartPos; }	//첫번째 이미지 시작 위치 가져오기
	POINT	GetSecondImagePos() { return m_ptSecondImageStartPos; }	//두번째 이미지 시작 위치 가져오기
	POINT	GetThirdImagePos() { return m_ptThirdImageStartPos; }	//세번째 이미지 시작 위치 가져오기
	POINT	GetImageSize() { return m_ptImageSize; }				//이미지 크기 가져오기
	int		GetScaleType() { return (int)m_eScaleType; }			//스케일 타입 가져오기
	int		GetPixel() { return m_iPixel; }							//늘어날  Pixel값 가져오기
	RECT	GetImageSizeToRect();									//이미지 렉트값 가져오기

public:
	virtual void Initialize();
	virtual void Run();
	virtual void Render(HDC _DC, eAssetTarget _target);				//에셋 렌더
	virtual void Save(std::ofstream& _OfSave);						//에셋 정보 저장
	virtual void Load(std::ifstream& _IfLoad);						//에셋 불러오기
	virtual void Copy(CBaseInterface* _Copy);						//에셋 값 복사
	virtual void SetPosition(POINT _pt);							//에셋 위치 세팅

public:
	CThirdGridBase(void);
	virtual ~CThirdGridBase(void);
};
