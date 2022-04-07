#include "StdAfx.h"
#include "SliderBase.h"

#include "BtnBase.h"
#include "AssetBase.h"


CSliderBase::CSliderBase(void)
{
	m_pBackground = new CAssetBase;
	m_pSliderGauge = new CAssetBase;
	m_bBtnPressed = false;
	m_iGauge = 0;
	m_fGaugeScale = 1;
	m_strTexPath = "";
	m_fScale_X = 1.0;
	m_fScale_Y = 1.0;
	m_tPosition.x = 0;
	m_tPosition.y = 0;
	m_ptGaugeStartPos.x = 0;
	m_ptGaugeStartPos.y = 0;
	m_iAssetType = ASSET_SLIDER;
	RECT_INITIALIZE(m_tFileSize);
}


CSliderBase::~CSliderBase(void)
{
	SAFE_DELETE(m_pBackground);
	SAFE_DELETE(m_pSliderGauge);
}

void CSliderBase::Initialize()
{
	m_Button.Initialize();
	m_pSliderGauge->Initialize();
	m_pBackground->Initialize();
}

void CSliderBase::Run()
{
	CheckState();
	m_Button.Run();
	m_pSliderGauge->Run();
	m_pBackground->Run();
	SetBasePosition();
	SetHitBox();
}

void CSliderBase::Render( HDC _DC, eAssetTarget _target )
{
	m_pBackground->Render(_DC, _target);
	m_pSliderGauge->Render(_DC, _target);
	m_Button.Render(_DC, _target);
	//Rectangle(_DC, m_tHitBox.left, m_tHitBox.top, m_tHitBox.right, m_tHitBox.bottom);
	/*std::cout << m_tHitBox.left << ","<< m_tHitBox.top <<
	","<< m_tHitBox.right << ","<< m_tHitBox.bottom << std::endl;*/
}

void CSliderBase::Copy( CBaseInterface* _Copy )
{
	m_iAssetType = _Copy->GetAssetType();
	RECT_COPY(m_tFileSize, ((CSliderBase*)_Copy)->m_tFileSize);
	m_tPosition.x = ((CSliderBase*)_Copy)->GetPosition().x;
	m_tPosition.y = ((CSliderBase*)_Copy)->GetPosition().y;

	CBtnBase copybase = ((CSliderBase*)_Copy)->m_Button;
	m_Button.Copy(&copybase);
	m_pBackground->Copy(((CSliderBase*)_Copy)->m_pBackground);
	m_pSliderGauge->Copy(((CSliderBase*)_Copy)->m_pSliderGauge);
	m_ptGaugeStartPos = ((CSliderBase*)_Copy)->m_ptGaugeStartPos;

	//	하단은 후에 이니셜라이즈 작업으로 이동
	SetHitBox();
	CSliderBase::MakeOriginialFilePath();

	m_pSliderGauge->SetPosition(m_tPosition);
	//	게이지 스케일링
	//	1픽셀에 대한 게이지의 가로 크기 비율을 얻어낸다
	m_fGaugeScale = 1.0f / m_pSliderGauge->GetFileSize().right;
}

void CSliderBase::Save( std::ofstream& _OfSave )
{
	CBaseInterface::Save(_OfSave);
	_OfSave << "[SLIDER START]" << std::endl;

	POINT MinusPos;
	MinusPos.x = m_tPosition.x * -1;
	MinusPos.y = m_tPosition.y * -1;

	_OfSave << "GaugeStartPos" << "\t" << m_ptGaugeStartPos.x << "," << m_ptGaugeStartPos.y << std::endl;

	m_Button.SubstractPosition(m_tPosition);
	m_pSliderGauge->SubstractPosition(m_tPosition);
	m_Button.Save(_OfSave);
	m_pSliderGauge->Save(_OfSave);
	m_pBackground->Save(_OfSave);

	m_Button.SubstractPosition(MinusPos);
	m_pSliderGauge->SubstractPosition(MinusPos);

	_OfSave << "GaugeScale" << "\t" << m_fGaugeScale << std::endl;
	_OfSave << "[SLIDER END]" << std::endl;
}

void CSliderBase::Load( std::ifstream& _IfLoad )
{
	std::string strInput;
	CBaseInterface::Load(_IfLoad);

	//	StartRead
	std::getline(_IfLoad, strInput, '\n');

	//	GaugeStartPos Load
	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, ',');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_ptGaugeStartPos.x = atoi(strInput.c_str());

	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_ptGaugeStartPos.y = atoi(strInput.c_str());

	m_Button.Load(_IfLoad);
	m_pSliderGauge->Load(_IfLoad);
	m_pBackground->Load(_IfLoad);

	POINT MinusPos;
	MinusPos.x = m_tPosition.x * -1;
	MinusPos.y = m_tPosition.y * -1;

	m_Button.SubstractPosition(MinusPos);
	m_pSliderGauge->SubstractPosition(MinusPos);

	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_fGaugeScale = atof(strInput.c_str());
	std::getline(_IfLoad, strInput, '\n');
}

void CSliderBase::SetHitBox()
{
	m_tFileSize.left = 0;
	m_tFileSize.top = 0;
	m_tFileSize.right = m_pBackground->GetFileSize().right + m_Button.GetFileSize().right;
	m_tFileSize.bottom = m_Button.GetFileSize().bottom;//m_pBackground->GetFileSize().bottom;

	m_tHitBox.left = m_tPosition.x;
	m_tHitBox.top = m_tPosition.y;
	m_tHitBox.right = (LONG)(m_tHitBox.left + (m_tFileSize.right) * m_fScale_X);
	m_tHitBox.bottom = (LONG)(m_tHitBox.top + (m_tFileSize.bottom) * m_fScale_Y);
}

void CSliderBase::SetParenthWnd( HWND _hWnd )
{
	m_ParenthWnd = _hWnd;
	m_Button.SetParenthWnd(_hWnd);
	m_pBackground->SetParenthWnd(_hWnd);
	m_pSliderGauge->SetParenthWnd(_hWnd);
}

void CSliderBase::SetBtn(POINT _normaltex,POINT _pressedtex,POINT _disabletex,POINT _highlighttex,POINT _btnSize)
{
	m_Button.SetButtonProperty(_normaltex, _pressedtex, _disabletex, _highlighttex, _btnSize);
}

void CSliderBase::SetBackGround(SaveData _data, POINT _size)
{
	RECT rtSize;
	rtSize.left = 0;
	rtSize.top = 0;
	rtSize.right = _size.x;
	rtSize.bottom = _size.y;

	//m_pBackground->SetPosition(m_tPosition);
	m_pBackground->SetAsset(m_strTexPath, ASSET_PICTURE, rtSize, _data.m_tStartPos, 1.f, 1.f);
	RECT_COPY(m_tFileSize,m_pBackground->GetFileSize());
	m_tFileSize.right += m_Button.GetFileSize().right;
}

void CSliderBase::SetSliderGauge(SaveData _data, POINT _size, POINT _startpos)
{
	RECT rtSize;
	rtSize.left = 0;
	rtSize.top = 0;
	rtSize.right = _size.x;
	rtSize.bottom = _size.y;

	m_pSliderGauge->SetPosition(m_tPosition);
	m_pSliderGauge->SetAsset(m_strTexPath, ASSET_PICTURE, rtSize, _data.m_tStartPos, 1.f, 1.f);
	//	게이지 스케일링
	//	1픽셀에 대한 게이지의 가로 크기 비율을 얻어낸다
	m_fGaugeScale = 1.0f / rtSize.right;
	m_ptGaugeStartPos = _startpos;
}

void CSliderBase::SetSliderProperty()
{
	//	다이알로그에서는 정보만 넘겨주고 슬라이더베이스 내부에서 애들을 세팅해주는 거로 한다.
	POINT ptBtnPos;
	POINT ptGaugePos;

	int iBgWidth = 0;
	int iBgHeight= 0;
	ptBtnPos.x = 0;
	ptBtnPos.y = 0;
	ptGaugePos.x = 0;
	ptGaugePos.y = 0;

	iBgWidth = m_pBackground->GetFileSize().right;
	iBgHeight = m_pBackground->GetFileSize().bottom;

	//	슬라이더 배경, 게이지 위치세팅
	// 배경위치 수정 [5/29/2019 Gabrielle]
	POINT ptBackground;
	ptBackground.x = m_tPosition.x + m_Button.GetFileSize().right / 2.f;
	ptBackground.y = m_tPosition.y + m_Button.GetFileSize().bottom / 2.f - iBgHeight / 2.f;
	m_pBackground->SetPosition(ptBackground/*m_tPosition*/);

	ptGaugePos.y = /*m_tPosition*/ptBackground.y + (iBgHeight / 2.f) - (m_pSliderGauge->GetFileSize().bottom / 2) + m_ptGaugeStartPos.y;
	ptGaugePos.x = /*m_tPosition*/ptBackground.x + m_ptGaugeStartPos.x;

	// m_tposition.y + ibgheight / 2 - m_pSliderGauge.filesize.bottom / 2
	m_pSliderGauge->SetPosition(ptGaugePos);
	m_pSliderGauge->SetScale(m_iGauge * m_fGaugeScale, 1.f);

	int iBtnWidth = m_Button.GetFileSize().right;
	int iBtnHeight = m_Button.GetFileSize().bottom;

	ptBtnPos.y = m_tPosition.y + (iBgHeight / 2.f) - (iBtnHeight / 2.f);
	ptBtnPos.x = m_tPosition.x + m_iGauge - (iBtnWidth / 2.f);

	m_Button.SetPosition(m_tPosition/*ptBtnPos*/);
}

void CSliderBase::SetFilePath( std::string _Path )
{
	m_strTexPath = _Path;
	m_Button.SetFilePath(_Path);
	m_pBackground->SetFilePath(_Path);
	m_pSliderGauge->SetFilePath(_Path);
}

void CSliderBase::ConvertFilePath()
{
	m_Button.ConvertFilePath();
	m_pBackground->ConvertFilePath();
	m_pSliderGauge->ConvertFilePath();
}

void CSliderBase::MakeOriginialFilePath()
{
	m_Button.MakeOriginialFilePath();
	m_pBackground->MakeOriginialFilePath();
	m_pSliderGauge->MakeOriginialFilePath();
}

void CSliderBase::SetState( eAssetState _state )
{
	m_iState = _state;
}

void CSliderBase::CheckState()
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
	//	충돌 로직
	//	ObjMgr에서 충돌유무에 대한 판단을 한다
	//	해당 판단한 결과물을 가지고 View에서 액션을 취한다.
	m_Button.CheckState();
	if(m_iState == STATE_PRESSED)
	{
		if(m_Button.GetState() == STATE_HIGHLIGHT)
		{
			m_bBtnPressed = true;
		}
		else
		{
			m_bBtnPressed = false;
		}
	}
	else
	{
		m_bBtnPressed = false;
	}
}

bool CSliderBase::SubstractPosition( POINT _pos )
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

		if(_pos.x > 0)
		{	//	줄어들고 있는 경우.
			if(m_iGauge > 0)// && m_tPosition.x <= m_Button.GetPosition().x)
			{
				//	최소치를 넘길 수치가 들어올 경우 최소 치수로만 고정
				if(m_iGauge - _pos.x < 0)
				{
					m_iGauge = 0;
				}
				else
				{
					m_iGauge -= _pos.x;
				}
			}
		}
		else // 증가하고 있는 경우
		{
			//	최대치를 넘긴 수치가 들어온다면 최대치수로만 고정.
			if(m_iGauge <= BGHitbox->right - BGHitbox->left)
			{
				if(m_iGauge - _pos.x > BGHitbox->right - BGHitbox->left)
				{
					m_iGauge = BGHitbox->right - BGHitbox->left;
				}
				else
				{
					m_iGauge -= _pos.x;
				}
			}
		}
		bResult = true;
	}

	SetSliderProperty();
	return bResult;
}

void CSliderBase::AddPosition( POINT _pos )
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
			m_iGauge += _pos.x;
		}
	}
	SetSliderProperty();
}

void CSliderBase::SetBasePosition()
{
	SetSliderProperty();
}

void CSliderBase::SetPosition( POINT _pt )
{
	m_tPosition = _pt;
}

void CSliderBase::SetScale( float _fscaleX, float _fscaleY )
{
	CBaseInterface::SetScale(_fscaleX, _fscaleY);

	m_Button.SetScale(_fscaleX, _fscaleY);
	m_pBackground->SetScale(_fscaleX, _fscaleY);
	m_pSliderGauge->SetScale(_fscaleX, _fscaleY);
}
