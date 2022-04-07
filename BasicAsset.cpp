#include "StdAfx.h"
#include "BasicAsset.h"

#include "BaseInterface.h"
#include "AssetBase.h"
#include "GroupButton.h"
#include "SliderBase.h"

CBasicAsset::CBasicAsset(void)
{
	ZeroMemory(m_szFilePath, sizeof(char) * MAX_PATH);
	ZeroMemory(m_szFileName, sizeof(char) * MAX_PATH);
	ZeroMemory(&m_tStartPos, sizeof(POINT));
	ZeroMemory(&m_tPosition, sizeof(POINT));
	m_Asset = NULL;
	RECT_INITIALIZE(m_tHitBox);
	m_fScale_X = 1.0f;
	m_fScale_Y = 1.0f;
	m_eAssetType = ASSET_DEFAULT;
}

CBasicAsset::CBasicAsset( CBasicAsset* _asset )
{
	ZeroMemory(m_szFilePath, sizeof(char) * MAX_PATH);
	ZeroMemory(m_szFileName, sizeof(char) * MAX_PATH);
	ZeroMemory(&m_tStartPos, sizeof(POINT));
	ZeroMemory(&m_tPosition, sizeof(POINT));
	RECT_INITIALIZE(m_tHitBox);
	m_Asset = NULL;
	m_fScale_X = 1.0f;
	m_fScale_Y = 1.0f;

	strncpy_s(m_szFilePath, _asset->m_szFilePath, MAX_PATH);
	strncpy_s(m_szFileName, _asset->m_szFileName, MAX_PATH);
	m_eAssetType = _asset->m_eAssetType;

	CBaseInterface* newBase = NULL;
	newBase = MakeAdaptableBase(_asset->GetAssetType());
	newBase->SetAssetType(_asset->m_Asset->GetAssetType());
	m_eAssetType = newBase->GetAssetType();

	newBase->Copy(_asset->m_Asset);
	if(_asset->m_Asset->GetAssetType() == ASSET_RADIOBUTTON || _asset->m_Asset->GetAssetType() == ASSET_TABBUTTON)
	{
		((CGroupButton*)newBase)->SetFocusIndex(0);
	}
	m_Asset = newBase;

	MakeOriginalFilePath();

	m_tStartPos = _asset->m_tStartPos;
	m_tPosition = _asset->m_tPosition;
	m_fScale_X = _asset->m_fScale_X;
	m_fScale_Y = _asset->m_fScale_Y;
	RECT_COPY(m_tHitBox, _asset->m_tHitBox);
	RECT_COPY(m_tAssetSize, _asset->m_tAssetSize);

	// ���� �����ǿ������� ����� ������ ���̱� ����.
	m_ptOriginPos = m_tPosition;
}

CBasicAsset::~CBasicAsset(void)
{
}

void CBasicAsset::Initialize()
{
	m_eAssetType = m_Asset->GetAssetType();
	InitializeAssetBase();
	SetAssetSize();
	SetHitBox();
}

void CBasicAsset::Run()
{
	SetAssetSize();
	SetHitBox();
	m_Asset->Run();
}

void CBasicAsset::Render(HDC _DC, eAssetTarget _target)
{
	m_Asset->Render(_DC, _target);
	//Rectangle(_DC, m_tHitBox.left, m_tHitBox.top, m_tHitBox.right, m_tHitBox.bottom);
}

void CBasicAsset::SetFilePath(std::string _Path)
{
	strncpy_s(m_szFilePath, _Path.c_str(), _Path.length());
}

void CBasicAsset::InitializeAssetBase()
{
	m_Asset->SetPosition(m_tPosition);
	SetAssetSize();
}

void CBasicAsset::SetHitBox()
{
	//	��Ʈ �ڽ� ������ ����
	m_tHitBox.left = m_tPosition.x;
	m_tHitBox.top = m_tPosition.y;
	m_tHitBox.right = m_tPosition.x +  m_Asset->GetFileSize().right * m_Asset->GetScaleX();/*m_tAssetSize.right ;*///* m_Asset->GetScaleX();
	m_tHitBox.bottom = m_tPosition.y +  m_Asset->GetFileSize().bottom * m_Asset->GetScaleY();/*m_tAssetSize.bottom ;*///* m_Asset->GetScaleY();
}

void CBasicAsset::SetAssetSize()
{
	//	��Ʈ �ڽ�(����)�� ��������� ����
	m_tAssetSize.left = 0;
	m_tAssetSize.top = 0;
	m_tAssetSize.right = m_Asset->GetFileSize().right * m_Asset->GetScaleX();
	m_tAssetSize.bottom = m_Asset->GetFileSize().bottom * m_Asset->GetScaleY();
}

void CBasicAsset::SetFileName( std::string _Name )
{
	strncpy_s(m_szFileName, _Name.c_str(), _Name.length());
}

void CBasicAsset::CalculatePosition(POINT _pos)
{
	POINT MoveDist;
	MoveDist.x = m_tPosition.x - _pos.x;
	MoveDist.y = m_tPosition.y - _pos.y;

	SubsAssetBasePos(MoveDist, _pos);
}

bool CBasicAsset::SubsAssetBasePos(POINT _pos, POINT _movePos)
{
	//���⼭ ��ȯ���� �޾Ƽ� ���� ��ư�� �������ٸ� ������ �����ǰ��� ������� ��������.
	if(m_eAssetType == ASSET_SLIDER || m_eAssetType == ASSET_SCROLL ||
		m_eAssetType == ASSET_LISTBOX || m_eAssetType == ASSET_COMBOBOX)
	{
		bool bBtnClicked = false;
		bBtnClicked = m_Asset->SubstractPosition(_pos);
		//bBtnClicked = ((CSliderBase*)(m_Asset[i]))->SubstractPosition(_pos);
		if(!bBtnClicked)
		{
			m_tPosition = _movePos;
		}
		else
		{//	���⼭ ��ư�� Ŭ���Ǿ� �����̰� ������ �˷��ش�.
			return true;
		}
	}
	else
	{
		//	Ŀ���� ���µ��� ��ǥ�� �ش� BasicAsset�� ��ǥ�� �����϶� ���� ũ��� ���������Ѵ�
		m_Asset->SubstractPosition(_pos);
		m_tPosition = m_Asset->GetPosition();
	}

	return false;
}

void CBasicAsset::AddAssetBasePos( POINT _pos )
{
	//	Ŀ���� ���µ��� ��ǥ�� �ش� BasicAsset�� ��ǥ�� �����϶� ���� ũ��� ���������Ѵ�
	m_Asset->AddPosition(_pos);
}

void CBasicAsset::SetScale(float _fx, float _fy)
{
	m_fScale_X = _fx;
	m_fScale_Y = _fy;

	if(m_Asset->GetAssetType() != ASSET_PICTURE)
	{
		m_Asset->SetScale(_fx, _fy);
	}
	else
	{
		((CAssetBase*)m_Asset)->SetFrameScale(m_fScale_X, m_fScale_Y);
	}

	SetAssetSize();
}

void CBasicAsset::ConvertFilePath()
{
	TCHAR	szPathBuffer[MAX_PATH];
	TCHAR	szConvertPath[MAX_PATH];
	TCHAR	szCurDir[MAX_PATH];
	char	szResult[MAX_PATH];
	bool	bCheckResult;

	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, m_szFilePath, -1, szConvertPath, MAX_PATH);
	GetCurrentDirectory(MAX_PATH, szCurDir);

	// �н� ���� : Release 
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
		ZeroMemory(m_szFilePath, MAX_PATH);
		wcstombs(szResult, (wchar_t*)szPathBuffer, MAX_PATH);
		strcpy_s(m_szFilePath, szResult);
	}
	else
	{
		std::cout << "����� ���� ����" << std::endl;
	}

	if(m_Asset->GetAssetType() != ASSET_TEXTBOX)
	{
		m_Asset->ConvertFilePath();
	}
}

void CBasicAsset::MakeOriginalFilePath()
{
	TCHAR szCurDir[MAX_PATH];
	TCHAR szAppendPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szCurDir);

	// �н� ���� : Release 
#ifdef RELEASE_PATH
	PathAppend(szCurDir, L"\\..\\DataSetup\\UI");
#endif

	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, m_szFilePath, -1, szAppendPath, MAX_PATH);

	PathRemoveBlanks(szAppendPath);
	PathAppend(szCurDir, szAppendPath);

	char psz[MAX_PATH];
	wcstombs(psz, (wchar_t*)szCurDir, MAX_PATH);

	strncpy_s(m_szFilePath, psz, MAX_PATH);

	if(m_Asset->GetAssetType() != ASSET_TEXTBOX)
	{
		m_Asset->MakeOriginialFilePath();
	}
}

void CBasicAsset::AssetBasePathSetting()
{
	if(m_Asset->GetAssetType() != ASSET_TEXTBOX)
	{
		m_Asset->MakeOriginialFilePath();
	}
}



void CBasicAsset::SetParenthWnd( HWND _hWnd )
{
	m_ParenthWnd = _hWnd;
	m_Asset->SetParenthWnd(_hWnd);
}

void CBasicAsset::Save( std::ofstream& _OfSave, int _SubsX, int _SubsY )
{
	_OfSave << "AssetPath\t\t" << m_szFilePath << std::endl;
	_OfSave << "AssetPosition\t" << m_tPosition.x - _SubsX << "," << m_tPosition.y - _SubsY << std::endl;
}

bool CBasicAsset::Load( std::ifstream& _IfLoad, int _SubX, int _SubY )
{
	//	������ ���� �ε��ϴ� �κп��� �ϴ� ��ž
	//	objmgr�� DbMgr�� ���ľ��Ѵ�.
	std::string strInput;

	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	strcpy_s(m_szFilePath, strInput.c_str());

	CString szFileName(strInput.c_str());
	PathStripPath(szFileName.GetBuffer());
	CT2CA pszConvertedAnsiString (szFileName);
	std::string _Name(pszConvertedAnsiString);
	strcpy_s(m_szFileName, _Name.c_str());

	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, ',');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_tPosition.x = atoi(strInput.c_str());

	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_tPosition.y = atoi(strInput.c_str());

	if(_SubX < 0)
	{
		m_tPosition.x += _SubX;
	}

	if(_SubY < 0)
	{
		m_tPosition.y += _SubY;
	}

	bool bResult = false;
	
	// �н� ���� : Release 
#ifdef RELEASE_PATH
	CString	AssetFilePath(m_szFilePath);
	TCHAR szCurDir[MAX_PATH] = {0,};
	GetCurrentDirectory(MAX_PATH, szCurDir);
	TCHAR szReleasePath[MAX_PATH] = RESOURCE_DEFAULT_PATH;
	PathAppend(szCurDir, szReleasePath);
	PathAppend(szCurDir, AssetFilePath);
	std::string strReleasePath = CT2A( szCurDir );
	bResult = LoadAsset(strReleasePath.c_str());
#else
	bResult = LoadAsset(m_szFilePath);
#endif

	// �ϳ��� ������ ������ �ϳ��� ���º��̽��� �����ٴ� ����.
	if(bResult == false)
	{
		return bResult;
	}

	m_Asset->SetPosition(m_tPosition);
	return bResult;
}

void CBasicAsset::InitPosition()
{
	m_tPosition = m_Asset->GetPosition();
}

void CBasicAsset::SubstractPosition( POINT _pos )
{
	m_tPosition.x -= _pos.x;
	m_tPosition.y -= _pos.y;
}

RECT* CBasicAsset::GetHitBox()
{
	SetHitBox();
	return &m_tHitBox;
}

std::string CBasicAsset::GetFilePath()
{
	std::string strPath = m_szFilePath;
	return strPath;
}

std::string CBasicAsset::GetFileName()
{
	std::string strPath = m_szFileName;
	return strPath;
}

eAssetType CBasicAsset::GetAssetType()
{
	return m_eAssetType;
}

void CBasicAsset::AddPosition( POINT _pos )
{
	m_tPosition.x += _pos.x;
	m_tPosition.y += _pos.y;
	m_Asset->SetPosition(m_tPosition);
}

void CBasicAsset::SetAssetType( eAssetType _type )
{
	m_eAssetType = _type;
}

void CBasicAsset::FreeFocus()
{
	m_Asset->FreeFocus();
}

void CBasicAsset::CheckState()
{
	bool	bResult;
	POINT	ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(m_ParenthWnd, &ptMouse);

	bResult = PtInRect(&m_tHitBox, ptMouse);
	if(bResult && (m_iState != STATE_PRESSED))
	{
		m_iState = STATE_HIGHLIGHT;
	}
	else if((bResult == false) && (m_iState != STATE_DISABLE))
	{
		m_iState = STATE_NORMAL;
	}

	m_Asset->CheckState();

	if(m_iState == STATE_PRESSED)
	{
		//if(m_Asset->GetState() == STATE_HIGHLIGHT)
		{
			m_Asset->SetState(STATE_PRESSED);
		}
	}
	else
	{
		if(m_Asset->GetState() == STATE_PRESSED)
		{
			m_Asset->SetState(STATE_NORMAL);
		}
	}
}

void CBasicAsset::SetState( eAssetState _state )
{
	m_iState = _state;
	CheckState();
}

bool CBasicAsset::LoadAsset(std::string _Path)
{
	std::ifstream	ifLoad(_Path.c_str());
	std::string		strInput;	

	CBaseInterface* newAsset = NULL;

	if(!ifLoad)
	{
		CString szWarning("���� ����� ������ �������� �ʰų� ���� �����Դϴ�.\n");
		szWarning += _Path.c_str();
		AfxMessageBox(szWarning);
		return false;
	}

	while(!ifLoad.eof())
	{
		//	������ �Է��� ���� InsertAsset�� SetFilePath�� ��ġ
		//	�ε� ���� ������
		std::getline(ifLoad, strInput, '\n');
		if(!strcmp(strInput.c_str(), "[END]\t"))
		{
			return true;
		}

		if(!strcmp(strInput.c_str(), "[START]"))
		{
			std::getline(ifLoad, strInput, '\t');
			std::getline(ifLoad, strInput, '\n');
			strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());

			eAssetType LoadAssetType = (eAssetType)atoi(strInput.c_str());

			newAsset = MakeAdaptableBase(LoadAssetType);
			newAsset->SetAssetType(LoadAssetType);
			m_eAssetType = LoadAssetType;
		}

		if(newAsset != NULL)
		{
			if(ifLoad.good())
			{
				newAsset->Load(ifLoad);
				newAsset->SetParenthWnd(m_ParenthWnd);
				m_Asset = newAsset;
			}
		}
	}

	ifLoad.close();
	return true;
}

void CBasicAsset::InsertAsset( CBaseInterface* _asset )
{
	if(m_Asset != NULL)
	{
		SAFE_DELETE(m_Asset);
	}
	m_Asset = _asset;
}

RECT CBasicAsset::GetFileSize()
{
	return m_tAssetSize;
}

POINT CBasicAsset::GetOriginPos()
{
	return m_ptOriginPos;
}

void CBasicAsset::SetPosition(POINT _ptPos)
{
	m_Asset->SetPosition(_ptPos);
	m_tPosition = m_Asset->GetPosition();
}

void CBasicAsset::SetOriginPos( POINT _ptPOs )
{
	m_ptOriginPos = _ptPOs;
}
