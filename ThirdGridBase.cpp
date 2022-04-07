// ThirdGridBase.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "ThirdGridBase.h"

CThirdGridBase::CThirdGridBase(void)
{
	m_ptImageSize.x = 0;
	m_ptImageSize.y = 0;
	m_ptFirstImageStartPos.x = 0;
	m_ptFirstImageStartPos.y = 0;
	m_ptSecondImageStartPos.x = 0;
	m_ptSecondImageStartPos.y = 0;
	m_ptThirdImageStartPos.x = 0;
	m_ptThirdImageStartPos.y = 0;
	m_eScaleType = ARRAYTYPE_WIDTH;
	m_iPixel = 0;
	m_iAssetType = ASSET_THIRDGRID;
}

CThirdGridBase::~CThirdGridBase(void)
{
}

void CThirdGridBase::Initialize()
{

}

void CThirdGridBase::Run()
{
	CAssetBase::Run();
}

//에셋 렌더
void CThirdGridBase::Render(HDC _DC, eAssetTarget _target)
{
	std::string strDrawPath = m_AssetBaseSaveData.m_szFilePath;
	if(strDrawPath != "")
	{
		CA2T wt(strDrawPath.c_str());
		if( m_cTexture == NULL )
		{
			m_cTexture = new CxImage;			
			m_bResult = m_cTexture->Load(wt, CXIMAGE_FORMAT_TGA);
		}

		//첫번째 이미지 렌더
		CxImage CropFirstImage = *m_cTexture;
		RECT rtCrop;
		rtCrop.left = m_ptFirstImageStartPos.x;
		rtCrop.top = m_ptFirstImageStartPos.y;
		rtCrop.right = rtCrop.left + m_ptImageSize.x;
		rtCrop.bottom = rtCrop.top + m_ptImageSize.y;

		CropFirstImage.Crop(rtCrop);
		CropFirstImage.Draw(_DC, m_tPosition.x, m_tPosition.y, m_ptImageSize.x * m_fScale_X, m_ptImageSize.y * m_fScale_Y);

		int nWidthStartPos = 0;
		int nHeightStartPos = 0;
		int nWidthSize = 0;
		int nHeightSize = 0;
		if(m_eScaleType == ARRAYTYPE_WIDTH)
		{
			nWidthStartPos =  m_tPosition.x + m_ptImageSize.x * m_fScale_X;			
			nHeightStartPos = m_tPosition.y;
			nWidthSize = m_iPixel;			
		}
		else if(m_eScaleType == ARRAYTYPE_HEIGHT)
		{
			nWidthStartPos =  m_tPosition.x;
			nHeightStartPos = m_tPosition.y + m_ptImageSize.y * m_fScale_Y;
			nHeightSize = m_iPixel;
		}
		
		//두번째 이미지 렌더
		CxImage CropSecondImage = *m_cTexture;
		RECT rtSecondCrop;
		rtSecondCrop.left = m_ptSecondImageStartPos.x;
		rtSecondCrop.top = m_ptSecondImageStartPos.y;
		rtSecondCrop.right = rtSecondCrop.left + m_ptImageSize.x;
		rtSecondCrop.bottom = rtSecondCrop.top + m_ptImageSize.y;

		CropSecondImage.Crop(rtSecondCrop);
		CropSecondImage.Draw(_DC, nWidthStartPos, nHeightStartPos, m_ptImageSize.x * m_fScale_X + nWidthSize * m_fScale_X, m_ptImageSize.y * m_fScale_Y + nHeightSize * m_fScale_Y);

		if(m_eScaleType == ARRAYTYPE_WIDTH)
		{
			nWidthStartPos =  m_tPosition.x + m_ptImageSize.x * m_fScale_X + nWidthSize * m_fScale_X;
			nHeightStartPos = m_tPosition.y;
		}
		else if(m_eScaleType == ARRAYTYPE_HEIGHT)
		{
			nWidthStartPos =  m_tPosition.x;
			nHeightStartPos = m_tPosition.y + m_ptImageSize.y * m_fScale_Y + nHeightSize * m_fScale_Y;
		}

		//세번째 이미지 렌더
		CxImage CropThirdImage = *m_cTexture;
		RECT rtThridCrop;
		rtThridCrop.left = m_ptThirdImageStartPos.x;
		rtThridCrop.top = m_ptThirdImageStartPos.y;
		rtThridCrop.right = rtThridCrop.left + m_ptImageSize.x;
		rtThridCrop.bottom = rtThridCrop.top + m_ptImageSize.y;

		CropThirdImage.Crop(rtThridCrop);
		CropThirdImage.Draw(_DC, nWidthStartPos, nHeightStartPos, m_ptImageSize.x * m_fScale_X, m_ptImageSize.y * m_fScale_Y);
	}
}

//에셋 정보 저장
void CThirdGridBase::Save(std::ofstream& _OfSave)
{
	CAssetBase::Save(_OfSave);
	_OfSave << "[THIRDGRID START]" << std::endl;
	_OfSave << "FirstTex" << "\t\t"	<< m_ptFirstImageStartPos.x << "," << m_ptFirstImageStartPos.y << std::endl;
	_OfSave << "SecondTex" << "\t\t"	<< m_ptSecondImageStartPos.x << "," << m_ptSecondImageStartPos.y << std::endl;	
	_OfSave << "ThirdTex" << "\t\t"	<< m_ptThirdImageStartPos.x << "," << m_ptThirdImageStartPos.y << std::endl;
	_OfSave << "ImageSize" << "\t\t"	<< m_ptImageSize.x << "," << m_ptImageSize.y << std::endl;
	_OfSave << "ScaleType" << "\t\t"	<< m_eScaleType << std::endl;
	_OfSave << "Pixel" << "\t"	<< m_iPixel << std::endl;
	_OfSave << "[THIRDGRID END]" << std::endl;
}

//에셋 불러오기
void CThirdGridBase::Load(std::ifstream& _IfLoad)
{
	CAssetBase::Load(_IfLoad);

	std::string strInput;
	//StartRead
	std::getline(_IfLoad, strInput, '\n');
	//FirstImage
	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, ',');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_ptFirstImageStartPos.x = atoi(strInput.c_str());

	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_ptFirstImageStartPos.y = atoi(strInput.c_str());

	//SecondImage
	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, ',');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_ptSecondImageStartPos.x = atoi(strInput.c_str());

	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_ptSecondImageStartPos.y = atoi(strInput.c_str());

	//ThirdImage
	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, ',');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_ptThirdImageStartPos.x = atoi(strInput.c_str());

	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_ptThirdImageStartPos.y = atoi(strInput.c_str());

	//ImageSize
	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, ',');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_ptImageSize.x = atoi(strInput.c_str());

	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_ptImageSize.y = atoi(strInput.c_str());

	//ScaleType
	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_eScaleType = (eArrayType)(atoi(strInput.c_str()));

	//Pixel
	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_iPixel = atoi(strInput.c_str());

	std::getline(_IfLoad, strInput, '\n');
}

//에셋 값 복사
void CThirdGridBase::Copy(CBaseInterface* _Copy)
{
	CAssetBase::Copy(_Copy);

	m_ptFirstImageStartPos = ((CThirdGridBase*)_Copy)->m_ptFirstImageStartPos;
	m_ptSecondImageStartPos = ((CThirdGridBase*)_Copy)->m_ptSecondImageStartPos;
	m_ptThirdImageStartPos = ((CThirdGridBase*)_Copy)->m_ptThirdImageStartPos;
	m_ptImageSize = ((CThirdGridBase*)_Copy)->m_ptImageSize;
	m_eScaleType = ((CThirdGridBase*)_Copy)->m_eScaleType;
	m_iPixel = ((CThirdGridBase*)_Copy)->m_iPixel;
}

//에셋 위치 세팅
void CThirdGridBase::SetPosition(POINT _pt)
{
	m_tPosition = _pt;
}

//3그리드 프로퍼티값 세팅
void CThirdGridBase::SetThirdGridProperty(POINT imageSize, POINT firstImageStartPos, POINT secondImageStartPos, POINT thirdImageStartPos, eArrayType eScaleType, int pixel)
{
	m_ptImageSize = imageSize;
	m_ptFirstImageStartPos = firstImageStartPos;
	m_ptSecondImageStartPos = secondImageStartPos;
	m_ptThirdImageStartPos = thirdImageStartPos;	
	m_eScaleType = eScaleType;
	m_iPixel = pixel;

	if(m_eScaleType == ARRAYTYPE_WIDTH)
	{
		m_tFileSize.right = imageSize.x *3 + m_iPixel;
		m_tFileSize.bottom = imageSize.y;
		m_tHitBox.right = imageSize.x *3 + m_iPixel;
		m_tHitBox.bottom = imageSize.y;
	}
	else if(m_eScaleType == ARRAYTYPE_HEIGHT)
	{
		m_tFileSize.right = imageSize.x;
		m_tFileSize.bottom = imageSize.y *3 + m_iPixel;
		m_tHitBox.right = imageSize.x;
		m_tHitBox.bottom = imageSize.y *3 + m_iPixel;
	}	
}

//이미지 렉트값 가져오기
RECT CThirdGridBase::GetImageSizeToRect()
{
	RECT rtSize;
	rtSize.left = 0;
	rtSize.top = 0;
	rtSize.right = m_ptImageSize.x;
	rtSize.bottom = m_ptImageSize.y;

	return rtSize;
}
