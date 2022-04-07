#include "StdAfx.h"
#include "ScrollBase.h"


CScrollBase::CScrollBase(void)
{
	m_iAssetType = ASSET_SCROLL;
	CBaseInterface::Initialize();
}


CScrollBase::~CScrollBase(void)
{
}

void CScrollBase::Initialize()
{
	m_Button.Initialize();
	m_pBackground->Initialize();
}

void CScrollBase::Run()
{ 
	CheckState();
	m_Button.Run();
	m_pBackground->Run();
	SetScrollProperty();
	SetHitBox();
}

void CScrollBase::Render( HDC _DC, eAssetTarget _target )
{
	//Rectangle(_DC, m_tHitBox.left, m_tHitBox.top, m_tHitBox.right, m_tHitBox.bottom);
	m_pBackground->Render(_DC, _target);
	m_Button.Render(_DC, _target);
	//Rectangle(_DC, m_Button.GetHitBox()->left,m_Button.GetHitBox()->top, m_Button.GetHitBox()->right, m_Button.GetHitBox()->bottom); 
	/*Rectangle(_DC, m_pBackground->GetHitBox()->left,m_pBackground->GetHitBox()->top,
	m_pBackground->GetHitBox()->right, m_pBackground->GetHitBox()->bottom);*/
}

void CScrollBase::Copy( CBaseInterface* _Copy )
{
	m_iAssetType = _Copy->GetAssetType();
	RECT_COPY(m_tFileSize, ((CScrollBase*)_Copy)->m_tFileSize);
	m_tPosition.x = ((CScrollBase*)_Copy)->GetPosition().x;
	m_tPosition.y = ((CScrollBase*)_Copy)->GetPosition().y;

	CBtnBase copybase = ((CScrollBase*)_Copy)->m_Button;
	m_Button.Copy(&copybase);
	m_pBackground->Copy(((CScrollBase*)_Copy)->m_pBackground);

	SetHitBox();
	CScrollBase::MakeOriginialFilePath();
}

void CScrollBase::Save( std::ofstream& _OfSave )
{
	CBaseInterface::Save(_OfSave);
	
	_OfSave << "[SCROLL START]" << std::endl;
	
	POINT MinusPos;
	MinusPos.x = m_tPosition.x * -1;
	MinusPos.y = m_tPosition.y * -1;

	m_Button.SubstractPosition(m_tPosition);

	m_Button.Save(_OfSave);
	m_pBackground->Save(_OfSave);

	m_Button.SubstractPosition(MinusPos);
	_OfSave << "[SCROLL END]" << std::endl;
}

void CScrollBase::Load( std::ifstream& _IfLoad )
{
	std::string strInput;
	CBaseInterface::Load(_IfLoad);

	//	StartRead
	std::getline(_IfLoad, strInput, '\n');

	m_Button.Load(_IfLoad);
	m_pBackground->Load(_IfLoad);

	POINT MinusPos;
	MinusPos.x = m_tPosition.x * -1;
	MinusPos.y = m_tPosition.y * -1;

	m_Button.SubstractPosition(MinusPos);
	std::getline(_IfLoad, strInput, '\n');
}

void CScrollBase::SetState( eAssetState _state )
{
	m_iState = _state;
	if(m_iState != STATE_PRESSED)
	{
		m_bBtnPressed = false;
	}
}

void CScrollBase::CheckState()
{
	bool	bResult;
	POINT	ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(m_ParenthWnd, &ptMouse);

	bResult = PtInRect(&m_tHitBox, ptMouse);
	if(bResult && (m_iState != STATE_PRESSED))
	{
		m_iState = STATE_HIGHLIGHT;
		m_bBtnPressed = false;
	}
	else if((bResult == false) && (m_iState != STATE_DISABLE))
	{
		m_iState = STATE_NORMAL;
		if(m_Button.GetState() == STATE_PRESSED)
		{
			m_Button.SetState(STATE_NORMAL);
			m_bBtnPressed = false;
		}
	}

	//	충돌 로직
	//	ObjMgr에서 충돌유무에 대한 판단을 한다
	//	해당 판단한 결과물을 가지고 View에서 액션을 취한다.
	m_Button.CheckState();
	if(m_iState ==STATE_PRESSED)
	{
		if(m_Button.GetState() == STATE_HIGHLIGHT)
		{
			m_bBtnPressed = true;
		}
		else
		{
			m_Button.SetState(STATE_NORMAL);
			m_bBtnPressed = false;
		}
	}
	else
	{
		m_bBtnPressed = false;
	}
}

void CScrollBase::SetHitBox()
{
	int iSubCount = 0;

	iSubCount = m_Button.GetFileSize().right - m_pBackground->GetFileSize().right;

	m_tFileSize.left = 0;
	m_tFileSize.top = 0;
	m_tFileSize.right = m_pBackground->GetFileSize().right * m_pBackground->GetScaleX();
	m_tFileSize.bottom = m_pBackground->GetFileSize().bottom * m_pBackground->GetScaleY();

	if(iSubCount > 0)
	{
		m_tFileSize.right = m_pBackground->GetFileSize().right * m_pBackground->GetScaleX() + iSubCount;	
	}

	m_tHitBox.left = m_tPosition.x;
	m_tHitBox.top = m_tPosition.y;
	m_tHitBox.right = (LONG)(m_tHitBox.left + (m_tFileSize.right) * m_fScale_X);
	m_tHitBox.bottom = (LONG)(m_tHitBox.top + (m_tFileSize.bottom) * m_fScale_Y);
}

void CScrollBase::SetParenthWnd( HWND _hWnd )
{
	m_ParenthWnd = _hWnd;
	m_Button.SetParenthWnd(_hWnd);
	m_pBackground->SetParenthWnd(_hWnd);
}

bool CScrollBase::SubstractPosition( POINT _pos )
{
	bool bResult = false;
	if(m_bBtnPressed == false)
	{
		m_tPosition.x -= _pos.x;
		m_tPosition.y -= _pos.y;
		SetHitBox();
		Run();
		bResult = false;
	}
	else if(m_bBtnPressed)
	{
		RECT* BGHitbox = m_pBackground->GetHitBox();

		if(_pos.y > 0)
		{
			if(m_iGauge > 0)
			{
				if(m_iGauge - _pos.y < 0)
				{
					m_iGauge = 0;
				}
				else
					m_iGauge -= _pos.y;
			}
		}
		else
		{
			int iBtnHeight = m_Button.GetFileSize().bottom * m_Button.GetScaleY();

			if(m_iGauge <= (BGHitbox->bottom - BGHitbox->top) - iBtnHeight)
			{
				if(m_iGauge > (BGHitbox->bottom - BGHitbox->top) - iBtnHeight)
				{
					m_iGauge = (BGHitbox->bottom - BGHitbox->top) - iBtnHeight;
				}
				else
				{
					m_iGauge -= _pos.y;
				}
			}
		}
		bResult = true;
	}

	SetScrollProperty();
	return bResult;
}

void CScrollBase::AddPosition( POINT _pos )
{
	if(m_bBtnPressed == false)
	{
		m_tPosition.x += _pos.x;
		m_tPosition.y += _pos.y;
	}
	else if (m_bBtnPressed)
	{
		if(m_iGauge > 0)
		{
			m_iGauge += _pos.y;
		}
	}
	SetScrollProperty();
}

void CScrollBase::SetScrollProperty()
{
	//	다이알로그에서는 정보만 넘겨주고 슬라이더베이스 내부에서 애들을 세팅해주는 거로 한다.
	POINT ptBtnPos;
	POINT ptPosition;
	POINT ptBtnCenter;

	int iBgWidth = 0;
	int iBgHeight= 0;

	ptPosition.x = m_tPosition.x;
	ptPosition.y = m_tPosition.y;
	ptBtnPos.x = 0;
	ptBtnPos.y = 0;
	ptBtnCenter.x = 0;
	ptBtnCenter.y = 0;

	//	배경 위치세팅
	//m_pBackground->SetPosition(ptPosition);

	int iSubCount = 0;

	iSubCount = m_Button.GetFileSize().right - m_pBackground->GetFileSize().right;

	if(iSubCount > 0)
	{
		POINT ptPos;
		ptPos.x = m_tPosition.x + (float)(iSubCount / 2.f);
		ptPos.y = m_tPosition.y;
		m_pBackground->SetPosition(ptPos);
	}
	else
	{
		m_pBackground->SetPosition(ptPosition);
	}

	iBgWidth = m_pBackground->GetFileSize().right;
	iBgHeight = m_pBackground->GetFileSize().bottom;

	int iBtnWidth = m_Button.GetFileSize().right;
	int iBtnHeight = m_Button.GetFileSize().bottom;

	//	버튼 위치 지정
	ptBtnPos.y = m_tPosition.y + m_iGauge;

	if(iSubCount > 0)
	{
		ptBtnPos.x = m_tPosition.x;
	}
	else
	{
		ptBtnPos.x = LONG(m_tPosition.x + (iBgWidth / 2.f) - (iBtnWidth / 2.f));
	}

	m_Button.SetPosition(ptBtnPos);
}

void CScrollBase::MakeOriginialFilePath()
{
	m_Button.MakeOriginialFilePath();
	m_pBackground->MakeOriginialFilePath();
}

void CScrollBase::ConvertFilePath()
{
	m_Button.ConvertFilePath();
	m_pBackground->ConvertFilePath();
}

void CScrollBase::SetFilePath(std::string _Path)
{
	m_strTexPath = _Path;
	m_Button.SetFilePath(_Path);
	m_pBackground->SetFilePath(_Path);
}

void CScrollBase::SetPosition( POINT _pt )
{
	m_tPosition = _pt;
}

int CScrollBase::GetGauge()
{
	return m_iGauge;
}

void CScrollBase::SetScale( float _fscaleX, float _fscaleY )
{
	CSliderBase::SetScale(_fscaleX, _fscaleY);
}
