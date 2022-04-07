#include "StdAfx.h"
#include "AssetBase.h"

SaveData::SaveData()
{
	Initialize();
}

SaveData::~SaveData()
{

}

void SaveData::Initialize()
{
	m_szFilePath.clear();
	m_szFolderPath.clear();
	ZeroMemory(&m_tStartPos, sizeof(POINT));
}

CAssetBase::CAssetBase(void)
{	
	Initialize();
	m_ptTextureSize.x = 0;
	m_ptTextureSize.y = 0;
	m_fFrameScaleX = 1.f;
	m_fFrameScaleY = 1.f;
}

CAssetBase::CAssetBase( CAssetBase* _asset )
{
	Initialize();
	Copy(_asset);
}


CAssetBase::~CAssetBase(void)
{
	SAFE_DELETE( m_cTexture );
}

void CAssetBase::SetAsset(std::string _path, eAssetType _Attribute, RECT _FileSize, POINT _StartPos,  float _fScaleX, float _fScaleY)
{
	//	세팅되는 픽쳐가 이전과 다른 경우.
	if(m_cTexture != NULL && (_FileSize.right != m_tFileSize.right
	|| _FileSize.bottom != m_tFileSize.bottom || _StartPos.x != m_AssetBaseSaveData.m_tStartPos.x
	|| _StartPos.y != m_AssetBaseSaveData.m_tStartPos.y))
	{
		SAFE_DELETE(m_cTexture);
	}

	//	속성값들을 매개변수로 초기화시켜주는 과정
	SetFilePath(_path);
	SetAssetType(ASSET_PICTURE);
	SetFileSize(_FileSize);
	SetScale(_fScaleX, _fScaleY);

	m_AssetBaseSaveData.m_tStartPos = _StartPos;

	if(_path != "")
	{
		SetTexFilePath(_path);
	}
}

void CAssetBase::SetHitBox()
{
	//	현재의 포지션을 기준점으로 텍스쳐 크기만큼의 영역을 HitBox로 설정한다
	m_tHitBox.left = m_tPosition.x;
	m_tHitBox.top = m_tPosition.y;
	m_tHitBox.right = (LONG)(m_tHitBox.left + m_tFileSize.right * m_fScale_X);
	m_tHitBox.bottom = (LONG)(m_tHitBox.top + m_tFileSize.bottom * m_fScale_Y);
}

void CAssetBase::Render(HDC _DC, eAssetTarget _target)
{
	std::string strDrawPath = "";

	strDrawPath = m_AssetBaseSaveData.m_szFilePath;

	if(strDrawPath != "")
	{
		CA2T wt(strDrawPath.c_str());
		//CxImage cTexture;
		if( m_cTexture == NULL && m_tFileSize.right != 0
			&& m_tFileSize.bottom != 0 )
		{
			m_cTexture = new CxImage;			
			m_bResult = m_cTexture->Load(wt, CXIMAGE_FORMAT_TGA);
			m_cTexture->Crop(m_AssetBaseSaveData.m_tStartPos.x, m_AssetBaseSaveData.m_tStartPos.y,
				m_AssetBaseSaveData.m_tStartPos.x + m_tFileSize.right, m_AssetBaseSaveData.m_tStartPos.y + m_tFileSize.bottom);
			m_ptTextureSize.x = m_cTexture->GetWidth();
			m_ptTextureSize.y = m_cTexture->GetHeight();

			if(m_bResult == false)
			{
				std::cout << "잘못된 경로에서 파일을 읽었습니다." << std::endl;
			}
		}
		
		if(m_bResult == true && m_tFileSize.right != 0
			&& m_tFileSize.bottom != 0)
		{
			::SetStretchBltMode(_DC, COLORONCOLOR);	
			m_cTexture->Draw(_DC, m_tPosition.x, m_tPosition.y, m_tFileSize.right * m_fScale_X * m_fFrameScaleX
				, m_tFileSize.bottom  * m_fScale_Y * m_fFrameScaleY);
		}
	}

	//Rectangle(_DC, m_tHitBox.left, m_tHitBox.top, m_tHitBox.right, m_tHitBox.bottom);
}

void CAssetBase::Run()
{
	SetHitBox();
}

void CAssetBase::SetTexFilePath(std::string _strPath )
{
	if(_strPath != "")
	{
		TCHAR Path[MAX_PATH];
		char szPath[MAX_PATH];
		
		//	확장자를 tex파일로 바꿔준다.
		CString tempPath(_strPath.c_str());
		PathRemoveExtension(tempPath.GetBuffer());
		PathAddExtension(tempPath.GetBuffer(), L".tex");

		CT2CA pszConvertedAnsiString (tempPath);
		std::string szRelativeTexPath(pszConvertedAnsiString);

		// *********시작*********
		// 상대경로 설정하는 로직

		TCHAR	szPathBuffer[MAX_PATH];
		TCHAR	szConvertPath[MAX_PATH];
		TCHAR	szCurDir[MAX_PATH];
		char	szResult[MAX_PATH];
		bool	bCheckResult;

		//	확장자가 바뀐 현재 텍스쳐 경로를 tchar 형식의 데이터에 담는다.
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szRelativeTexPath.c_str(), -1, szConvertPath, MAX_PATH);

		//	현재 UITool.exe가 속한 폴더를 가져온다
		GetCurrentDirectory(MAX_PATH, szCurDir);

		// 패스 변경 : Release 
		//	현재 경로에 각각에 필요한 폴더 경로를 덧붙인다
#if defined _DEBUG || defined RELEASE_DEV_PATH
		PathAppend(szCurDir, L"\\Texture");
		//PathAddExtension(szCurDir, L"\\Texture");
#else
		PathAppend(szCurDir, TGA_FOLDER_PATH);
		//PathAddExtension(szCurDir, TGA_FOLDER_PATH);
#endif

		// 현재 경로와 현재 디렉토리를 계산하여 상대경로로 만든다.
		bCheckResult = PathRelativePathTo(szPathBuffer, szCurDir, FILE_ATTRIBUTE_DIRECTORY,
			szConvertPath, FILE_ATTRIBUTE_NORMAL);
		if(bCheckResult == TRUE)
		{
			szRelativeTexPath.clear();
			wcstombs(szResult, (wchar_t*)szPathBuffer, MAX_PATH);
			szRelativeTexPath = szResult;
		}
		else
		{
			std::cout << "상대경로 설정 실패" << std::endl;
		}
		// *********끝*********

		// 지금은 경로가 정해지지 않았으므로 디폴트 경로를 지정해서 저장하게 한다.
		SetFolderPath(TEXTURE_FOLDER_PATH);

		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, m_AssetBaseSaveData.m_szFolderPath.c_str(), -1, Path, MAX_PATH);

		char szTexPath[MAX_PATH] = {0,};
		strcpy_s(szTexPath, szRelativeTexPath.c_str());
		CString AppendPath(szResult);
		PathAppend(Path, AppendPath.GetBuffer());

		m_AssetBaseSaveData.m_szTexFilePath.clear();
		wcstombs(szPath, (wchar_t*)Path, MAX_PATH);
		std::string FinalTexFilePath = szPath;
		m_AssetBaseSaveData.m_szTexFilePath = FinalTexFilePath;
		//strcpy_s(m_AssetBaseSaveData.m_szTexFilePath.c_str(), szPath);
	}
}

std::string CAssetBase::GetFilePath()
{
	std::string strPath = m_AssetBaseSaveData.m_szFilePath;
	return strPath;
}
//  [11/30/2018 Gabrielle]
//	루트 폴더 경로 지정하는거 바꿔줘야한다. 
//	지금은 Bin폴더에서 가져오게 했는데 이거에서 한단계 위로 올라가게 해야함.

void CAssetBase::ConvertFilePath()
{
	TCHAR	szPathBuffer[MAX_PATH];
	TCHAR	szConvertPath[MAX_PATH];
	TCHAR	szCurDir[MAX_PATH];
	char	szResult[MAX_PATH];
	bool	bCheckResult;

	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, m_AssetBaseSaveData.m_szFilePath.c_str(), -1, szConvertPath, MAX_PATH);
	GetCurrentDirectory(MAX_PATH, szCurDir);

	// 패스 변경 : Release 
#if defined _DEBUG || defined RELEASE_DEV_PATH
	bCheckResult = PathRelativePathTo(szPathBuffer, szCurDir, FILE_ATTRIBUTE_DIRECTORY,
		szConvertPath, FILE_ATTRIBUTE_NORMAL);
#else
	PathAppend(szCurDir, L"\\..\\DataSetup\\UI");
	bCheckResult = PathRelativePathTo(szPathBuffer, szCurDir, FILE_ATTRIBUTE_DIRECTORY,
		szConvertPath, FILE_ATTRIBUTE_NORMAL);
#endif
	
	
	if(bCheckResult == TRUE)
	{
		m_AssetBaseSaveData.m_szFilePath.clear();
		wcstombs(szResult, (wchar_t*)szPathBuffer, MAX_PATH);
		m_AssetBaseSaveData.m_szFilePath = szResult;
	}
	else
	{
		std::cout << "상대경로 설정 실패" << std::endl;
	}
}

void CAssetBase::MakeOriginialFilePath()
{
	TCHAR szCurDir[MAX_PATH];
	TCHAR szAppendPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szCurDir);

	// 패스 변경 : Release 
#ifdef RELEASE_PATH
	PathAppend(szCurDir, L"\\..\\DataSetup\\UI");
#endif

	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, m_AssetBaseSaveData.m_szFilePath.c_str(), -1, szAppendPath, MAX_PATH);

	PathRemoveBlanks(szAppendPath);
	PathAppend(szCurDir, szAppendPath);

	char psz[MAX_PATH];
	wcstombs(psz, (wchar_t*)szCurDir, MAX_PATH);

	m_AssetBaseSaveData.m_szFilePath = psz;
}

void CAssetBase::SetFolderPath( std::string _strPath )
{
	m_AssetBaseSaveData.m_szFolderPath = _strPath;
}

void CAssetBase::Initialize()
{
	CBaseInterface::Initialize();
	m_AssetBaseSaveData.Initialize();
	m_bResult = false;	
	m_cTexture = NULL;	
	m_fScale_Y = 1.0;
	m_fScale_X = 1.0;
}

void CAssetBase::SetFilePath(std::string _path)
{
	if(m_cTexture != NULL)
	{
		SAFE_DELETE(m_cTexture);
	}
	m_AssetBaseSaveData.m_szFilePath = _path;
	SetTexFilePath(_path);
}

void CAssetBase::SetFileSize( RECT _FileSize )
{
	RECT_COPY(m_tFileSize, _FileSize);
}

void CAssetBase::Save( std::ofstream& _OfSave )
{
	CBaseInterface::Save(_OfSave);
	_OfSave << "[PICTURE START]" << std::endl;
	_OfSave << "FilePath" << "\t\t" << m_AssetBaseSaveData.m_szFilePath << std::endl;
	_OfSave << "TexFilePath" << "\t\t" << m_AssetBaseSaveData.m_szTexFilePath << std::endl;
	_OfSave << "FolderPath" << "\t\t" << m_AssetBaseSaveData.m_szFolderPath << std::endl;
	_OfSave << "StartPos" << "\t\t" << m_AssetBaseSaveData.m_tStartPos.x << "," << m_AssetBaseSaveData.m_tStartPos.y << std::endl;
	_OfSave << "[PICTURE END]" << std::endl;
}

void CAssetBase::Load( std::ifstream& _IfLoad )
{
	CBaseInterface::Load(_IfLoad);

	std::string strInput;
	//	StartRead
	std::getline(_IfLoad, strInput, '\n');
	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_AssetBaseSaveData.m_szFilePath = strInput;

	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_AssetBaseSaveData.m_szTexFilePath = strInput;

	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_AssetBaseSaveData.m_szFolderPath = strInput;

	//	StartPos Load
	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, ',');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_AssetBaseSaveData.m_tStartPos.x = atoi(strInput.c_str());

	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_AssetBaseSaveData.m_tStartPos.y = atoi(strInput.c_str());
	std::getline(_IfLoad, strInput, '\n');

	// 패스 변경 : Release 
#ifdef RELEASE_PATH
	CString	AssetFilePath(m_AssetBaseSaveData.m_szFilePath.c_str());
	TCHAR szCurDir[MAX_PATH] = {0,};
	GetCurrentDirectory(MAX_PATH, szCurDir);
	TCHAR szReleasePath[MAX_PATH] = RESOURCE_DEFAULT_PATH;
	PathAppend(szCurDir, szReleasePath);
	PathAppend(szCurDir, AssetFilePath);
	std::string strReleasePath = CT2A( szCurDir );
	m_AssetBaseSaveData.m_szFilePath = strReleasePath;
#endif
}

void CAssetBase::Copy( CBaseInterface* _copy )
{
	SAFE_DELETE(m_cTexture);

	CBaseInterface::Copy(_copy);

	m_AssetBaseSaveData.m_szFilePath =  ((CAssetBase*)_copy)->m_AssetBaseSaveData.m_szFilePath;
	m_AssetBaseSaveData.m_szTexFilePath = ((CAssetBase*)_copy)->m_AssetBaseSaveData.m_szTexFilePath;
	m_AssetBaseSaveData.m_szFolderPath = ((CAssetBase*)_copy)->m_AssetBaseSaveData.m_szFolderPath;
	m_AssetBaseSaveData.m_tStartPos = ((CAssetBase*)_copy)->m_AssetBaseSaveData.m_tStartPos;
}

void CAssetBase::CheckState()
{
	CBaseInterface::CheckState();
}

void CAssetBase::MultiplyPosition( float _fx, float _fy )
{
	m_tPosition.x = (LONG)(m_tPosition.x * _fx);
	m_tPosition.y = (LONG)(m_tPosition.y * _fy);
}

void CAssetBase::SetFrameScale( float _fscaleX, float _fscaleY )
{
	m_fFrameScaleX = _fscaleX;
	m_fFrameScaleY = _fscaleY;
}
