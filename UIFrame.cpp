#include "StdAfx.h"
#include "UIFrame.h"

#include "BasicAsset.h"
#include "AssetBase.h"
#include "ObjMgr.h"

CUIFrame::CUIFrame(void)
{
	m_tPosition.x = 0;
	m_tPosition.y = 0;
	m_fScaleX = 1.f;
	m_fScaleY = 1.f;
	m_iFocusAsset = NOTHING_IN_HERE;
}

CUIFrame::CUIFrame( CUIFrame* _Scene)
{
	int iCount = _Scene->m_vecAsset.size();
	for(int i = 0; i < iCount; i++)
	{
		CBasicAsset* newAsset = new CBasicAsset(_Scene->m_vecAsset[i]);
		newAsset->SetAssetType(_Scene->m_vecAsset[i]->GetAssetType());
		newAsset->SetParenthWnd(m_MainFrmhWnd);
		InsertBasicAsset(newAsset);
	}

	m_strFramePath = _Scene->m_strFramePath;
	m_tPosition = _Scene->m_tPosition;
	m_tFrameSize = _Scene->m_tFrameSize;
	RECT_COPY(m_tHitBox, _Scene->m_tHitBox);

	m_ptMin = _Scene->m_ptMin;
	m_ptMax = _Scene->m_ptMax;
	m_iFocusAsset = NOTHING_IN_HERE;

	m_fScaleX = 1.f;
	m_fScaleY = 1.f;

	CalculateAssetSize();

	m_tFrameOriginalSize = m_tFrameSize;

	iCount = m_vecAsset.size();
	for(int i = 0; i < iCount; i++)
	{
		m_vecAsset[i]->SetOriginPos(m_vecAsset[i]->GetPosition());
	}

	m_tFrameOriginalPos = m_tPosition;

	m_fScaleY = _Scene->m_fScaleY;
	m_fScaleX = _Scene->m_fScaleX;

	SetScale(m_fScaleX, m_fScaleY);
}

CUIFrame::~CUIFrame(void)
{
	Destroy();
}

void CUIFrame::InsertBasicAsset( CBasicAsset* _asset )
{
	m_vecAsset.push_back(_asset);
}

void CUIFrame::Initialize()
{
	CalculateAssetSize();
	m_tPosition = m_ptMin;
	SetHitBox();
}

void CUIFrame::Run()
{
	//CalculateAssetSize();
	SetHitBox();
	CheckState();

	int iCount = m_vecAsset.size();
	for(int i = 0; i < iCount; i++)
	{
		m_vecAsset[i]->Run();
	}
}

void CUIFrame::Render( HDC _hdc, eAssetTarget _target, bool _RenderIndex )
{
	int iCount = m_vecAsset.size();
	for(int i = 0; i < iCount; i++)
	{
		m_vecAsset[i]->Render(_hdc, _target);
		if(_RenderIndex)
		{
			SetTextColor(_hdc, RGB(255, 255, 255));
			DrawID(_hdc, i, m_vecAsset[i]->GetPosition(), *(m_vecAsset[i]->GetHitBox()),UIASSET_INDEX_RGB);
		}
	}
}

void CUIFrame::Destroy()
{
	int iCount = m_vecAsset.size();
	for(int i = 0; i < iCount; i++)
	{
		SAFE_DELETE(m_vecAsset[i]);
	}

	m_vecAsset.clear();
}

bool CUIFrame::CollisionCheck( POINT _pos )
{
	//	여기에서 콜리전이 확인되면 다같이 움직이는거다.
	bool bResult = false;

	if(PtInRect(&m_tHitBox, _pos))
	{
		bResult = true;
	}
	else
		bResult = false;

	return bResult;
}

void CUIFrame::SetMainFrmhWnd( HWND _hWnd )
{
	m_MainFrmhWnd = _hWnd;

	int iCount = m_vecAsset.size();
	for(int i = 0; i < iCount; i++)
	{
		m_vecAsset[i]->SetParenthWnd(m_MainFrmhWnd);
	}
}

void CUIFrame::Save( std::ofstream& _OfSave )
{
	ConvertFilePathToRelative();
	_OfSave << "FrameFilePath" << "\t" << m_strFramePath << std::endl;
	_OfSave << "FramePosition" << "\t" << m_tPosition.x << "," << m_tPosition.y << std::endl;
	_OfSave << "FrameSize" << "\t\t" << m_tFrameOriginalSize.x << "," << m_tFrameOriginalSize.y << std::endl;	//	 원래는 m_tFramsSize였음
	_OfSave << "FrameScale" << "\t\t" <<  m_fScaleX << "," << m_fScaleY << std::endl;
	MakeOriginalFilePathToAbsolute();
}

bool CUIFrame::Load( std::ifstream& _IfLoad )
{
	std::string strInput;

	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_strFramePath = strInput;

	// FramePosition
	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, ',');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_tPosition.x = atoi(strInput.c_str());

	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_tPosition.y = atoi(strInput.c_str());

	//	FrameSize
	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, ',');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_tFrameSize.x = atoi(strInput.c_str());

	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_tFrameSize.y = atoi(strInput.c_str());

	std::getline(_IfLoad, strInput, '\t');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());

	if(!strcmp(strInput.c_str(), "FrameScale"))
	{
		std::getline(_IfLoad, strInput, '\t');
		std::getline(_IfLoad, strInput, ',');
		strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
		strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
		float fScaleX = atof(strInput.c_str());

		std::getline(_IfLoad, strInput, '\n');
		strInput.erase(std::remove(strInput.begin(), strInput.end(), '\n'), strInput.end());
		strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
		float fScaleY = atof(strInput.c_str());

		m_fScaleX = fScaleX;
		m_fScaleY = fScaleY;
	}

	MakeOriginalFilePathToAbsolute();
	bool bLoadOKay = LoadFrame(m_strFramePath);
	if(bLoadOKay == false)
	{
		return false;
	}
	
	POINT ptMovingPos;
	ptMovingPos.x = m_tPosition.x * -1;
	ptMovingPos.y = m_tPosition.y * -1;

	SubstractPosition(ptMovingPos);

	m_tFrameOriginalSize = m_tFrameSize;

	int iCount = m_vecAsset.size();
	for(int i = 0; i < iCount; i++)
	{
		m_vecAsset[i]->SetOriginPos(m_vecAsset[i]->GetPosition());
	}

	m_tFrameOriginalPos = m_tPosition;

	return true;
}

void CUIFrame::ConvertFilePath()
{
	int iCount = m_vecAsset.size();
	for(int i = 0; i < iCount; i++)
	{
		//	여기서 상대 경로로 바꿔준다.
	}
}

void CUIFrame::MakeOriginalFilePath()
{
	int iCount = m_vecAsset.size();
	for(int i = 0; i < iCount; i++)
	{
		m_vecAsset[i]->AssetBasePathSetting();
	}
}

void CUIFrame::Copy( CUIFrame* _Frame )
{
	m_MainFrmhWnd = _Frame->m_MainFrmhWnd;
	m_tPosition = _Frame->m_tPosition;
	m_tFrameSize = _Frame->m_tFrameSize;
	RECT_COPY(m_tHitBox, _Frame->m_tHitBox);

	for(int i = 0; i < _Frame->m_vecAsset.size(); i++)
	{
		CBasicAsset* newAsset = new CBasicAsset(_Frame->m_vecAsset[i]);
		newAsset->SetAssetType(_Frame->m_vecAsset[i]->GetAssetType());
		newAsset->SetParenthWnd(m_MainFrmhWnd);
		InsertBasicAsset(newAsset);
	}

	m_strFramePath = _Frame->m_strFramePath;
	m_tPosition = _Frame->m_tPosition;
	m_tFrameSize = _Frame->m_tFrameSize;
	RECT_COPY(m_tHitBox, _Frame->m_tHitBox);

	m_ptMin = _Frame->m_ptMin;
	m_ptMax = _Frame->m_ptMax;

	m_fScaleX = 1.f;
	m_fScaleY = 1.f;
}

void CUIFrame::SubstractPosition( POINT _pos )
{
	POINT ptMovingPos;
	bool bBtnMove  = false;

	int iCount = m_vecAsset.size();

	if(m_iFocusAsset != NOTHING_IN_HERE)
	{
		ptMovingPos = m_vecAsset[m_iFocusAsset]->GetPosition();
		ptMovingPos.x -= _pos.x;
		ptMovingPos.y -= _pos.y;
		if(m_vecAsset[m_iFocusAsset]->SubsAssetBasePos(_pos, ptMovingPos))
		{
			return;
		}
	}

	for(int i = 0; i<iCount; i++)
	{
		if(i != m_iFocusAsset)
		{
			ptMovingPos = m_vecAsset[i]->GetPosition();
			ptMovingPos.x -= _pos.x;
			ptMovingPos.y -= _pos.y;
			bBtnMove = m_vecAsset[i]->SubsAssetBasePos(_pos, ptMovingPos);
		}
	}

	if(!bBtnMove)
	{
		m_tPosition.x -= _pos.x;
		m_tPosition.y -= _pos.y;
	}
}

void CUIFrame::SetPosition( POINT _pos )
{
	POINT MoveDist;

	CalculateAssetSize();

	MoveDist.x = m_ptMin.x - _pos.x;
	MoveDist.y = m_ptMin.y - _pos.y;

	int iCount = m_vecAsset.size();
	for(int i = 0; i<iCount; i++)
	{
		m_vecAsset[i]->SubsAssetBasePos(MoveDist, _pos);
	}

	m_tPosition = _pos;
}

void CUIFrame::SetState( eAssetState _state )
{
	m_iState = _state;
	CheckState();
}

void CUIFrame::CheckState()
{
	bool	bResult;
	POINT	ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(m_MainFrmhWnd, &ptMouse);

	bResult = PtInRect(&m_tHitBox, ptMouse);
	if(bResult && (m_iState != STATE_PRESSED))
	{
		m_iState = STATE_HIGHLIGHT;
	}
	else if((bResult == false) && (m_iState != STATE_DISABLE))
	{
		m_iState = STATE_NORMAL;
	}

	int iCount = m_vecAsset.size();
	for(int i = 0; i<iCount; i++)
	{
		m_vecAsset[i]->CheckState();
	}

	if(m_iState == STATE_PRESSED)
	{
		int iCount = m_vecAsset.size();
		for(int i = 0; i<iCount; i++)
		{
			if(m_vecAsset[i]->GetState() == STATE_HIGHLIGHT)
			{
				m_iFocusAsset = i;
				m_vecAsset[i]->SetState(STATE_PRESSED);
				m_vecAsset[i]->CheckState();
			}
		}
	}
	else
	{
		int iCount = m_vecAsset.size();
		for(int i = 0; i<iCount; i++)
		{
			if(m_vecAsset[i]->GetState() == STATE_PRESSED)
			{
				m_iFocusAsset = NOTHING_IN_HERE;
				m_vecAsset[i]->SetState(STATE_NORMAL);
			}
		}
	}
}

void CUIFrame::SetHitBox()
{
	m_tHitBox.left = m_tPosition.x;
	m_tHitBox.top = m_tPosition.y;
	m_tHitBox.right = m_tHitBox.left + m_tFrameOriginalSize.x * m_fScaleX;
	m_tHitBox.bottom = m_tHitBox.top + m_tFrameOriginalSize.y * m_fScaleY;
}

void CUIFrame::CalculateAssetSize()
{
	POINT	ptCmpMinPos, ptCmpMaxPos;
	POINT	ptFileSize;

	int iCount = m_vecAsset.size();
	//	각 에셋을 돌면서 최소 포지션과 최대포지션을 계산한다.
	//	해당 포지션을 계산한 이후에는 결과물을 이용하여 HitBox의 사이즈를 결정한다.
	for(int i = 0; i < iCount; i++)
	{
		if(i == 0)
		{
			ptCmpMinPos = m_vecAsset[i]->GetPosition();
			m_ptMin = m_vecAsset[i]->GetPosition();

			//	left, top을 좌표로 사용하고 있었기 때문에 Max 지점을 체크하기 위해서는 각 파일의 크기를
			//	left, top 좌표에 더해준 이후에 계산해야한다.
			if(m_vecAsset[i]->GetAssetType() == ASSET_CHECKBOX)
			{
				ptFileSize.x = (LONG)( (m_vecAsset[i]->GetHitBox()->right - m_vecAsset[i]->GetHitBox()->left)
					* m_vecAsset[i]->GetScaleX());
				ptFileSize.y = (LONG)((m_vecAsset[i]->GetHitBox()->bottom - m_vecAsset[i]->GetHitBox()->top)
					* m_vecAsset[i]->GetScaleY());
			}
			else
			{
				ptFileSize.x = (LONG)(m_vecAsset[i]->GetFileSize().right
					* m_vecAsset[i]->GetScaleX());
				ptFileSize.y = (LONG)(m_vecAsset[i]->GetFileSize().bottom
					* m_vecAsset[i]->GetScaleY());
			}

			ptCmpMaxPos.x = m_vecAsset[i]->GetPosition().x + ptFileSize.x;
			ptCmpMaxPos.y = m_vecAsset[i]->GetPosition().y + ptFileSize.y;
			m_ptMax.x = m_vecAsset[i]->GetPosition().x + ptFileSize.x;
			m_ptMax.y = m_vecAsset[i]->GetPosition().y + ptFileSize.y;
			continue;
		}

		ptFileSize.x = (LONG)(m_vecAsset[i]->GetFileSize().right * m_vecAsset[i]->GetScaleX());
		ptFileSize.y = (LONG)(m_vecAsset[i]->GetFileSize().bottom * m_vecAsset[i]->GetScaleY());
		ptCmpMaxPos.x = m_vecAsset[i]->GetPosition().x + ptFileSize.x;
		ptCmpMaxPos.y = m_vecAsset[i]->GetPosition().y + ptFileSize.y;
		ptCmpMinPos = m_vecAsset[i]->GetPosition();

		if(ptCmpMinPos.x < m_ptMin.x)
		{
			m_ptMin.x = ptCmpMinPos.x;
		}

		if(ptCmpMaxPos.x > m_ptMax.x)
		{
			m_ptMax.x = ptCmpMaxPos.x;
		}

		if(ptCmpMinPos.y < m_ptMin.y)
		{
			m_ptMin.y = ptCmpMinPos.y;
		}

		if(ptCmpMaxPos.y > m_ptMax.y)
		{
			m_ptMax.y = ptCmpMaxPos.y;
		}
	}

	m_tFrameSize.x = m_ptMax.x - m_ptMin.x;
	m_tFrameSize.y = m_ptMax.y - m_ptMin.y;

	m_tPosition = m_ptMin;
}

std::string CUIFrame::GetFrameFileName()
{
	std::string strFileName = m_strFramePath;
	int nPos = strFileName.rfind("\\");
	strFileName.erase(0, nPos+1);

	return strFileName;
}

bool CUIFrame::LoadFrame( std::string _Path )
{
	std::ifstream	ifLoad(_Path.c_str());
	std::string		strInput;	
	bool bFileOkay = false;

	CBasicAsset*	newBasicAsset	= NULL;

	if(!ifLoad)
	{
		CString szWarning("다음 경로의 파일이 존재하지 않거나 깨진 파일입니다.\n");
		szWarning += _Path.c_str();
		AfxMessageBox(szWarning);
		return false;
	}

	while(!ifLoad.eof())
	{
		std::getline(ifLoad, strInput, '\n');

		if(!strcmp(strInput.c_str(), "[START]"))
		{
			newBasicAsset = new CBasicAsset;
			bFileOkay = newBasicAsset->Load(ifLoad);
			if(bFileOkay)
			{
				newBasicAsset->MakeOriginalFilePath();
				newBasicAsset->Run();
				InsertBasicAsset(newBasicAsset);
			}
		}
	}

	ifLoad.close();
}

void CUIFrame::SetFrameFilePath( std::string _path )
{
	m_strFramePath = _path;
}

void CUIFrame::ConvertFilePathToRelative()
{
	TCHAR	szPathBuffer[MAX_PATH];
	TCHAR	szConvertPath[MAX_PATH];
	TCHAR	szCurDir[MAX_PATH];
	char	szResult[MAX_PATH];
	bool	bCheckResult;

	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, m_strFramePath.c_str(), -1, szConvertPath, MAX_PATH);
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
		m_strFramePath.clear();
		wcstombs(szResult, (wchar_t*)szPathBuffer, MAX_PATH);
		m_strFramePath = szResult;
	}
	else
	{
		std::cout << "상대경로 설정 실패" << std::endl;
	}
}

void CUIFrame::MakeOriginalFilePathToAbsolute()
{
	TCHAR szCurDir[MAX_PATH];
	TCHAR szAppendPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szCurDir);

	// 패스 변경 : Release 
#ifdef RELEASE_PATH
	PathAppend(szCurDir, L"\\..\\DataSetup\\UI");
#endif

	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, m_strFramePath.c_str(), -1, szAppendPath, MAX_PATH);

	PathRemoveBlanks(szAppendPath);
	PathAppend(szCurDir, szAppendPath);

	char psz[MAX_PATH];
	wcstombs(psz, (wchar_t*)szCurDir, MAX_PATH);

	m_strFramePath = psz;
}

POINT CUIFrame::GetSize()
{
	return m_tFrameSize;
}

void CUIFrame::SetScale( float _fScaleX /*= 1*/, float _fScaleY /*= 1*/ )
{
	// 외부에서 세팅하는 스케일값이 넘어오면 해당 스케일을 본인과 Asset에 적용시킨다 [6/5/2019 Gabrielle]
	m_fScaleX = _fScaleX;
	m_fScaleY = _fScaleY;

	int iCount = m_vecAsset.size();
	for(int i = 0 ; i<iCount; i++)
	{
		//	스케일링 적용
		m_vecAsset[i]->SetScale(m_fScaleX, m_fScaleY);

		// 스케일링에 따른 포지션 설정 [6/7/2019 Gabrielle] 상대좌표는 O, 절대좌표는 X
		POINT ptAssetPosition = m_vecAsset[i]->GetOriginPos();

		//  스케일링 이후에 에셋들의 좌표가 상대적으로는 맞지만 프레임 좌표와는 맞지 않는 상황
		ptAssetPosition.x -= m_tFrameOriginalPos.x;	//	애초에 원점 좌표에서 이동할 거리를 계산하게 하기 위함
		ptAssetPosition.y -= m_tFrameOriginalPos.y;

		POINT ptScalePosition = ptAssetPosition;
		ptScalePosition.x *= m_fScaleX;
		ptScalePosition.y *= m_fScaleY;

		ptScalePosition.x += m_tPosition.x;
		ptScalePosition.y += m_tPosition.y;

		m_vecAsset[i]->SetPosition(ptScalePosition);
	}
}

float CUIFrame::GetScaleX()
{
	return m_fScaleX;
}

float CUIFrame::GetScaleY()
{
	return m_fScaleY;
}

POINT CUIFrame::GetOriginSize()
{
	return m_tFrameOriginalSize;
}