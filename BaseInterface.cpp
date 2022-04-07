#include "StdAfx.h"
#include "BaseInterface.h"

#include "GlobalFunc.h"


CBaseInterface::CBaseInterface(void)
{

}


CBaseInterface::~CBaseInterface(void)
{

}

void CBaseInterface::SetAssetType( eAssetType _Att )
{
	m_iAssetType = _Att;
}

eAssetType CBaseInterface::GetAssetType()
{
	return (eAssetType)m_iAssetType;
}

RECT* CBaseInterface::GetHitBox()
{
	return &m_tHitBox;
}

void CBaseInterface::SetPosition( POINT _pt )
{
	m_tPosition.x = _pt.x;
	m_tPosition.y = _pt.y;
}

void CBaseInterface::Initialize()
{
	m_iAssetType = NOTHING_IN_HERE;
	ZeroMemory(&m_tPosition, sizeof(POINT));
	RECT_INITIALIZE(m_tHitBox);
	RECT_INITIALIZE(m_tFileSize);
	m_fScale_X = 1.0;
	m_fScale_Y = 1.0;
}

void CBaseInterface::SetScale( float _fscaleX, float _fscaleY )
{
	m_fScale_X = _fscaleX;
	m_fScale_Y = _fscaleY;
}

void CBaseInterface::Save( std::ofstream& _OfSave )
{
	_OfSave << "Attribute" << "\t\t" << m_iAssetType << std::endl;
	_OfSave << "Position" << "\t\t" << m_tPosition.x << "," << m_tPosition.y << std::endl;
	_OfSave << "HitBox" << "\t\t\t" << m_tHitBox.left << "," << m_tHitBox.top << "," << m_tHitBox.right << "," << m_tHitBox.bottom << std::endl;
	_OfSave << "FileSize" << "\t\t" << m_tFileSize.left << "," << m_tFileSize.top << "," << m_tFileSize.right << "," << m_tFileSize.bottom << std::endl;
	_OfSave << "Scale" << "\t\t\t" << m_fScale_X << "," << m_fScale_Y << std::endl;
}

void CBaseInterface::Load( std::ifstream& _IfLoad )
{
	std::string strInput;

	//	Position Load
	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, ',');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_tPosition.x = atoi(strInput.c_str());

	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_tPosition.y = atoi(strInput.c_str());

	// HitBox Load
	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, ',');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_tHitBox.left = atoi(strInput.c_str());

	std::getline(_IfLoad, strInput, ',');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_tHitBox.top = atoi(strInput.c_str());

	std::getline(_IfLoad, strInput, ',');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_tHitBox.right = atoi(strInput.c_str());

	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_tHitBox.bottom = atoi(strInput.c_str());

	// FileSize Load
	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, ',');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_tFileSize.left = atoi(strInput.c_str());

	std::getline(_IfLoad, strInput, ',');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_tFileSize.top = atoi(strInput.c_str());

	std::getline(_IfLoad, strInput, ',');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_tFileSize.right = atoi(strInput.c_str());

	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_tFileSize.bottom = atoi(strInput.c_str());

	//	Scale Load
	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, ',');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_fScale_X = ((float)atof(strInput.c_str()));

	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_fScale_Y = ((float)atof(strInput.c_str()));

	m_tOriginPos = m_tPosition;
}

void CBaseInterface::Copy( CBaseInterface* _Copy )
{
	m_iAssetType = _Copy->GetAssetType();
	m_fScale_X = _Copy->GetScaleX();
	m_fScale_Y = _Copy->GetScaleY();
	m_tPosition = _Copy->GetPosition();
	m_tOriginPos = _Copy->GetOriginPos();

	RECT* tHitBox = _Copy->GetHitBox();
	RECT_COPY(m_tHitBox, (*tHitBox));
	RECT_COPY(m_tFileSize, _Copy->GetFileSize());
}

void CBaseInterface::CheckState()
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
}

void CBaseInterface::SetState( eAssetState _state )
{
	m_iState = _state;
}

void CBaseInterface::SetParenthWnd( HWND _hWnd )
{
	m_ParenthWnd = _hWnd;
}

void CBaseInterface::AddPosition( POINT _pos )
{
	m_tPosition.x += _pos.x;
	m_tPosition.y += _pos.y;
}

bool CBaseInterface::SubstractPosition( POINT _pos )
{
	m_tPosition.x -= _pos.x;
	m_tPosition.y -= _pos.y;
	SetHitBox();
	Run();
	//	특수한 경우에만 true를 돌려주도록한다.
	return false;
}

void CBaseInterface::SetHitBox()
{
	m_tHitBox.left = m_tPosition.x;
	m_tHitBox.top = m_tPosition.y;
	m_tHitBox.right = (LONG)(m_tHitBox.left + (m_tFileSize.right) * m_fScale_X);
	m_tHitBox.bottom = (LONG)(m_tHitBox.top + m_tFileSize.bottom * m_fScale_Y);
}

void CBaseInterface::MakeOriginialFilePath()
{

}

void CBaseInterface::ConvertFilePath()
{

}

void CBaseInterface::SetOriginPos( POINT _ptPos )
{
	m_tOriginPos = _ptPos;
}

POINT CBaseInterface::GetOriginPos()
{
	return m_tOriginPos;
}
