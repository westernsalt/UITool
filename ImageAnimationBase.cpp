#include "StdAfx.h"
#include "ImageAnimationBase.h"

CImageAnimationBase::CImageAnimationBase(void)
{
	m_ptTextureSize.x = 0;
	m_ptTextureSize.y = 0;
	m_ptImageStartPos.x = 0;
	m_ptImageStartPos.y = 0;
	m_ptImageInterval.x = 0;
	m_ptImageInterval.y = 0;
	m_ptImageSize.x = 0;
	m_ptImageSize.y = 0;
	m_ptCurrentImageStartPos.x = 0;
	m_ptCurrentImageStartPos.y = 0;
	m_ptImageNum.x = 0;
	m_ptImageNum.y = 0;
	m_iImageTotalNum = 0;
	m_fDelayTime = 0.0f;	

	m_iRowCount = 1;
	m_iImageCount = 1;
	m_iColPos = 0;
	m_iAssetType = ASSET_IMAGEANIMATION;
	m_fStartTime = (float)timeGetTime() * 0.001f;   //초단위로 바꿔줌
}

CImageAnimationBase::~CImageAnimationBase(void)
{
}

void CImageAnimationBase::Initialize()
{
	CAssetBase::Initialize();
}

void CImageAnimationBase::SetImageAnimationProperty(POINT textureSize, POINT imageStartPos, POINT imageInterval, POINT imageSize, POINT imageNum, int imageTotalNum, float delayTime, int loop)
{
	m_tFileSize.right = imageSize.x;
	m_tFileSize.bottom = imageSize.y;

	m_ptTextureSize = textureSize;
	m_ptImageStartPos = imageStartPos;
	m_ptCurrentImageStartPos = imageStartPos;
	m_ptImageInterval = imageInterval;
	m_ptImageSize = imageSize;
	m_ptImageNum = imageNum;
	m_iImageTotalNum = imageTotalNum;
	m_fDelayTime = delayTime;
	m_iLoop = loop;
}

void CImageAnimationBase::Run()
{
	CAssetBase::Run();
}

void CImageAnimationBase::Render(HDC _DC, eAssetTarget _target)
{	
	std::string strDrawPath = "";
	strDrawPath = m_AssetBaseSaveData.m_szFilePath;

	if(strDrawPath != "")
	{
		CA2T wt(strDrawPath.c_str());
		if(m_cTexture == NULL && m_tFileSize.right != 0 && m_tFileSize.bottom != 0)
		{
			m_cTexture = new CxImage;
			m_bResult = m_cTexture->Load(wt, CXIMAGE_FORMAT_TGA);
			//m_cTexture->Crop(m_AssetBaseSaveData.m_tStartPos.x, m_AssetBaseSaveData.m_tStartPos.y,
			//	m_AssetBaseSaveData.m_tStartPos.x + m_tFileSize.right, m_AssetBaseSaveData.m_tStartPos.y + m_tFileSize.bottom);
			m_ptTextureSize.x = m_cTexture->GetWidth();
			m_ptTextureSize.y = m_cTexture->GetHeight();
		}

		//이미지 돌아가면서 찍어주자.
		if(m_cTexture != NULL)
		{
			//처음 이미지위치 설정
			CxImage temp = *m_cTexture;
			temp.Crop(m_ptCurrentImageStartPos.x, m_ptCurrentImageStartPos.y,
				m_ptCurrentImageStartPos.x + m_tFileSize.right, m_ptCurrentImageStartPos.y + m_tFileSize.bottom);

			if(m_bResult == true && m_tFileSize.right != 0 && m_tFileSize.bottom != 0)
			{
				::SetStretchBltMode(_DC, COLORONCOLOR);
				temp.Draw(_DC, m_tPosition.x, m_tPosition.y, m_tFileSize.right * m_fScale_X, m_tFileSize.bottom  * m_fScale_Y);
			}

			//두번째부터 시간체크해서 위치값 설정
			float NowTime = (float)timeGetTime() * 0.001f;
			if( NowTime-m_fStartTime >=  m_fDelayTime)
			{
				++m_iRowCount;
				++m_iImageCount;
				m_fStartTime = (float)timeGetTime() * 0.001f;

				if(m_iImageTotalNum >= m_iImageCount)
				{
					m_ptCurrentImageStartPos.x = m_ptCurrentImageStartPos.x + m_ptImageSize.x + m_ptImageInterval.x;
					if(m_ptImageNum.x < m_iRowCount)
					{						
						++m_iColPos;
						m_iRowCount = 1;
						m_ptCurrentImageStartPos.x = m_ptImageStartPos.x;
						m_ptCurrentImageStartPos.y = (m_ptImageSize.y * m_iColPos)+ m_ptImageStartPos.y + (m_ptImageInterval.y* m_iColPos);
					}
				}
				else
				{
					if(m_iLoop == 1)
					{
						m_iRowCount = 1;
						m_iImageCount = 1;
						m_iColPos = 0;
						m_ptCurrentImageStartPos = m_ptImageStartPos;
						m_fStartTime = (float)timeGetTime() * 0.001f;
					}
				}
			}
		}
	}
}

void CImageAnimationBase::Save(std::ofstream& _OfSave)
{
	CAssetBase::Save(_OfSave);
	_OfSave << "[ANIMATION START]" << std::endl;
	_OfSave << "TextureSize" << "\t\t"	<< m_ptTextureSize.x << "," << m_ptTextureSize.y << std::endl;
	_OfSave << "ImageStartPos" << "\t\t"	<< m_ptImageStartPos.x << "," << m_ptImageStartPos.y << std::endl;	
	_OfSave << "ImageInterval" << "\t\t"	<< m_ptImageInterval.x << "," << m_ptImageInterval.y << std::endl;
	_OfSave << "ImageSize" << "\t\t"	<< m_ptImageSize.x << "," << m_ptImageSize.y << std::endl;
	_OfSave << "ImageNum" << "\t"	<< m_ptImageNum.x << "," << m_ptImageNum.y << std::endl;
	_OfSave << "ImageTotalNum" << "\t\t\t" << m_iImageTotalNum << std::endl;
	_OfSave << "AniLoop" << "\t\t\t" << m_iLoop << std::endl;
	_OfSave << "DelayTime" << "\t\t\t" << m_fDelayTime << std::endl;
	_OfSave << "[ANIMATION END]" << std::endl;
}

void CImageAnimationBase::Load(std::ifstream& _IfLoad)
{
	CAssetBase::Load(_IfLoad);

	std::string strInput;
	//	StartRead
	std::getline(_IfLoad, strInput, '\n');
	//	TextureSize
	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, ',');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_ptTextureSize.x = atoi(strInput.c_str());

	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_ptTextureSize.y = atoi(strInput.c_str());

	//	ImageStartPos
	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, ',');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_ptImageStartPos.x = atoi(strInput.c_str());

	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_ptImageStartPos.y = atoi(strInput.c_str());

	//	ImageInterval
	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, ',');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_ptImageInterval.x = atoi(strInput.c_str());

	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_ptImageInterval.y = atoi(strInput.c_str());

	//	ImageSize
	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, ',');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_ptImageSize.x = atoi(strInput.c_str());

	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_ptImageSize.y = atoi(strInput.c_str());

	//	ImageNum
	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, ',');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_ptImageNum.x = atoi(strInput.c_str());

	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_ptImageNum.y = atoi(strInput.c_str());

	//	ImageTotalNum
	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_iImageTotalNum = atoi(strInput.c_str());

	//	AniLoop
	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_iLoop = atoi(strInput.c_str());

	//DelayTime
	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_fDelayTime = (float)(atof(strInput.c_str()));

	std::getline(_IfLoad, strInput, '\n');
}


void CImageAnimationBase::Copy(CBaseInterface* _Copy)
{
	CAssetBase::Copy(_Copy);

	m_ptTextureSize = ((CImageAnimationBase*)_Copy)->m_ptTextureSize;
	m_ptImageStartPos = ((CImageAnimationBase*)_Copy)->m_ptImageStartPos;
	m_ptImageInterval = ((CImageAnimationBase*)_Copy)->m_ptImageInterval;
	m_ptImageSize = ((CImageAnimationBase*)_Copy)->m_ptImageSize;
	m_ptImageNum = ((CImageAnimationBase*)_Copy)->m_ptImageNum;
	m_iImageTotalNum = ((CImageAnimationBase*)_Copy)->m_iImageTotalNum;
	m_fDelayTime = ((CImageAnimationBase*)_Copy)->m_fDelayTime;
	m_iLoop = ((CImageAnimationBase*)_Copy)->m_iLoop;
	m_ptCurrentImageStartPos = ((CImageAnimationBase*)_Copy)->m_ptImageStartPos;
}

void CImageAnimationBase::SetPosition(POINT _pt)
{
	m_tPosition = _pt;
}

RECT CImageAnimationBase::GetSingleImageSizeToRect()
{
	RECT rtSize;
	rtSize.left = 0;
	rtSize.top = 0;
	rtSize.right = m_ptImageSize.x;
	rtSize.bottom = m_ptImageSize.y;

	return rtSize;
}

void CImageAnimationBase::ChangeTexture()
{
	if(this != NULL && m_cTexture != NULL)
		SAFE_DELETE(m_cTexture);
}
