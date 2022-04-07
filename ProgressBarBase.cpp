#include "StdAfx.h"
#include "ProgressBarBase.h"


CProgressBarBase::CProgressBarBase(void)
{
	Initialize();
	m_pBackground = new CAssetBase;
	m_pExtraGauge = new CAssetBase;
}

CProgressBarBase::~CProgressBarBase(void)
{
	SAFE_DELETE(m_pBackground);
	SAFE_DELETE(m_pExtraGauge);
	DestroyGaugeBar();
	m_vecGaugeBarScale.clear();
}

void CProgressBarBase::Initialize()
{
	CBaseInterface::Initialize();
	m_iGauge = 0;
	m_strTexPath = "";
	m_iAssetType = ASSET_PROGRESSBAR;
}

void CProgressBarBase::Run()
{
	CheckState();
	SetHitBox();
	SetGaugeScale();
	SetProgressbarProperty();
}

void CProgressBarBase::Render( HDC _DC, eAssetTarget _target )
{
	int iCurGauge = 0;
	m_pBackground->Render(_DC, _target);

	SetGaugeScale();

	//	전체렌더를 하고 있지만 igage에 따라서 다르게 렌더해야할것.
	m_iVecCount = m_vecGaugeBar.size();
	for(int i = 0; i<m_iVecCount; i++)
	{
		if((m_iGauge > i*100) && (m_iGauge < (i+1)*100))
		{
			float fScale = m_vecGaugeBarScale[i];
			m_vecGaugeBar[i]->SetScale(fScale * (m_iGauge - i*100) * m_fScale_X, 1.f * m_fScale_Y);
			m_vecGaugeBar[i]->Render(_DC, _target);
		}
		else if(m_iGauge > i * 100)
		{
			m_vecGaugeBar[i]->SetScale(m_vecGaugeBarScale[i] * 100, 1.f);
			m_vecGaugeBar[i]->Render(_DC, _target);
		}
	}

	if(m_pExtraGauge != NULL && m_iExtraGauge != 0)
	{
		iCurGauge = m_iGauge % 100;
		int iBgHeight = m_pBackground->GetFileSize().bottom;
		float fCurGauge = m_pBackground->GetFileSize().right * iCurGauge / 100.f;
		POINT ExtraGaugePos;
		ExtraGaugePos.x = float(m_tPosition.x + fCurGauge);
		ExtraGaugePos.y = m_tPosition.y + (iBgHeight / 2.f) - (m_pExtraGauge->GetFileSize().bottom / 2) + m_ptGaugeIndent.y;

		m_pExtraGauge->SetPosition(ExtraGaugePos);
	
		if(m_iExtraGauge + (m_iGauge % 100) < 100 && m_iExtraGauge < 100)
		{
			m_pExtraGauge->SetScale(m_fExtraGaugeScale * m_iExtraGauge, 1.f);
		}
		else
		{
			//	프로그래스바의 원점
			POINT RemainGaugePos;
			RemainGaugePos.x = m_tPosition.x;
			RemainGaugePos.y = m_tPosition.y + (iBgHeight / 2.f) - (m_pExtraGauge->GetFileSize().bottom / 2) + m_ptGaugeIndent.y;

			m_pExtraGauge->SetPosition(RemainGaugePos);

			if(m_iExtraGauge >= 100)
			{
				m_pExtraGauge->SetScale(m_fExtraGaugeScale * 100, 1.f);
			}
			else if(m_iExtraGauge != 0 && m_iExtraGauge < 100)
			{
				//	100퍼센트를 뚫은 게이지 값을 얻는다.
				int iRemainGauge = (m_iExtraGauge + m_iGauge)%100;
				CAssetBase RemainRenderBase;
				RemainRenderBase.Copy(m_pExtraGauge);
				RemainRenderBase.SetPosition(ExtraGaugePos);

				m_pExtraGauge->SetScale(m_fExtraGaugeScale * iRemainGauge, 1.f);
				RemainRenderBase.SetScale(m_fExtraGaugeScale * (m_iExtraGauge -iRemainGauge), 1.f);
				RemainRenderBase.Render(_DC, _target);
			}
		}
		
		m_pExtraGauge->Render(_DC, _target);
	}
}

void CProgressBarBase::Copy( CBaseInterface* _Copy )
{
	CBaseInterface::Copy(_Copy);

	m_iGauge = ((CProgressBarBase*)_Copy)->m_iGauge;
	m_strTexPath = ((CProgressBarBase*)_Copy)->m_strTexPath;

	m_pBackground->Copy(((CProgressBarBase*)_Copy)->m_pBackground);
	m_iVecCount = ((CProgressBarBase*)_Copy)->m_iVecCount;

	for(int i = 0; i<m_iVecCount; i++)
	{
		CAssetBase* CopyBase = new CAssetBase;
		CopyBase->Copy(((CProgressBarBase*)_Copy)->m_vecGaugeBar[i]);
		m_vecGaugeBar.push_back(CopyBase);
	}

	for(int i = 0; i<m_iVecCount; i++)
	{
		m_vecGaugeBarScale.push_back(((CProgressBarBase*)_Copy)->m_vecGaugeBarScale[i]);
	}

	m_ptGaugeIndent = ((CProgressBarBase*)_Copy)->m_ptGaugeIndent;

	//	여기서 게이지마다의 스케일을 계산하면 되는데 이거 각 게이지마다로 작업바꿔야함
	SetGaugeScale();
	for(int i = 0; i<m_iVecCount; i++)
	{
		m_vecGaugeBar[i]->SetScale(m_vecGaugeBarScale[i], 1.f);
	}

	m_pExtraGauge->Copy(((CProgressBarBase*)_Copy)->m_pExtraGauge);
	m_fExtraGaugeScale = ((CProgressBarBase*)_Copy)->m_fExtraGaugeScale;
	m_iExtraGauge = ((CProgressBarBase*)_Copy)->m_iExtraGauge;
}

void CProgressBarBase::Save( std::ofstream& _OfSave )
{
	CBaseInterface::Save(_OfSave);
	_OfSave << "[PROGRESSBAR START]" << std::endl;
	_OfSave << "FilePath" << "\t\t" << m_strTexPath << std::endl;
	_OfSave << "GaugePercentage" << "\t" << m_iGauge << std::endl;
	_OfSave << "ExtraGaugePercentage" << "\t" << m_iExtraGauge << std::endl;
	_OfSave << "GaugeIndent" << "\t\t" << m_ptGaugeIndent.x << "," << m_ptGaugeIndent.y << std::endl;
	
	m_iVecCount = m_vecGaugeBar.size();
	_OfSave << "GaugeCount" << "\t\t" << m_iVecCount << std::endl;

	m_pBackground->Save(_OfSave);
	
	POINT MinusPos;
	MinusPos.x = m_tPosition.x * -1;
	MinusPos.y = m_tPosition.y * -1;

	for(int i = 0; i < m_iVecCount; i++)
	{
		m_vecGaugeBar[i]->SubstractPosition(m_tPosition);
		m_vecGaugeBar[i]->SetScale(m_vecGaugeBarScale[i], 1.f);
		m_vecGaugeBar[i]->Save(_OfSave);
		m_vecGaugeBar[i]->SubstractPosition(MinusPos);
		m_vecGaugeBar[i]->SetScale(m_vecGaugeBarScale[i] * m_iGauge, 1.f);
		_OfSave << "GaugeBarfScale" << "\t" << m_vecGaugeBarScale[i] << std::endl;
	}

	m_pExtraGauge->SubstractPosition(m_tPosition);
	m_pExtraGauge->SetScale(m_fExtraGaugeScale, 1.f);
	m_pExtraGauge->Save(_OfSave);
	m_pExtraGauge->SubstractPosition(MinusPos);
	m_pExtraGauge->SetScale(m_fExtraGaugeScale * m_iExtraGauge, 1.f);

	_OfSave << "[PROGRESSBAR END]" << std::endl;
}

void CProgressBarBase::Load( std::ifstream& _IfLoad )
{
	CBaseInterface::Load(_IfLoad);

	std::string strInput;
	//	StartRead
	std::getline(_IfLoad, strInput, '\n');
	//	FilePath Load
	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	m_strTexPath = strInput;

	//	GaugePercentage Load
	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	m_iGauge = atoi(strInput.c_str());

	//	ExtraGaugePercentage Load
	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	m_iExtraGauge = atoi(strInput.c_str());
	
	//	GaugeIndent Load
	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, ',');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_ptGaugeIndent.x = atoi(strInput.c_str());

	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_ptGaugeIndent.y = atoi(strInput.c_str());

	//	게이지 카운트
	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	m_iVecCount = atoi(strInput.c_str());

	m_pBackground->Load(_IfLoad);

	POINT MinusPos;
	MinusPos.x = m_tPosition.x * -1;
	MinusPos.y = m_tPosition.y * -1;

	for(int i = 0; i < m_iVecCount; i++)
	{
		CAssetBase* newBase = new CAssetBase;
		m_vecGaugeBar.push_back(newBase);
		m_vecGaugeBar[i]->Load(_IfLoad);
		m_vecGaugeBar[i]->SubstractPosition(MinusPos);
		std::getline(_IfLoad, strInput, '\t');
		std::getline(_IfLoad, strInput, '\n');
		strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
		m_vecGaugeBarScale.push_back(atof(strInput.c_str()));
	}

	m_pExtraGauge->Load(_IfLoad);
	m_pExtraGauge->SubstractPosition(MinusPos);

	std::getline(_IfLoad, strInput, '\n');
}

void CProgressBarBase::SetState( eAssetState _state )
{
	m_iState = _state;
	CheckState();
}

void CProgressBarBase::CheckState()
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

void CProgressBarBase::SetHitBox()
{
	m_tHitBox.left = m_tPosition.x;
	m_tHitBox.top = m_tPosition.y;
	m_tHitBox.right = m_tHitBox.left + m_pBackground->GetFileSize().right;
	m_tHitBox.bottom = m_tHitBox.top + m_pBackground->GetFileSize().bottom;
}

void CProgressBarBase::SetParenthWnd( HWND _hWnd )
{
	m_ParenthWnd = _hWnd;
}

bool CProgressBarBase::SubstractPosition( POINT _pos )
{
	m_tPosition.x -= _pos.x;
	m_tPosition.y -= _pos.y;

	m_pBackground->SubstractPosition(_pos);

	for(int i = 0; i < m_iVecCount; i++)
	{
		m_vecGaugeBar[i]->SubstractPosition(_pos);
	}

	return false;
}

void CProgressBarBase::AddPosition( POINT _pos )
{
	m_pBackground->AddPosition(_pos);

	for(int i = 0; i < m_iVecCount; i++)
	{
		m_vecGaugeBar[i]->AddPosition(_pos);
	}
}

void CProgressBarBase::SetPosition( POINT _pt )
{
	m_tPosition.x = _pt.x;
	m_tPosition.y = _pt.y;

	m_pBackground->SetPosition(_pt);

	POINT ptPos;
	ptPos.x = m_tPosition.x + m_ptGaugeIndent.x;
	ptPos.y = m_tPosition.y + m_ptGaugeIndent.y;

	for(int i = 0; i < m_iVecCount; i++)
	{
		m_vecGaugeBar[i]->SetPosition(ptPos);
	}
}

void CProgressBarBase::SetProgressBar( std::string _path, POINT _size, POINT _startPos )
{
	RECT ptSize;
	ptSize.left = 0;
	ptSize.top = 0;
	ptSize.right = _size.x;
	ptSize.bottom = _size.y;

	m_pBackground->SetPosition(m_tPosition);
	m_pBackground->SetAsset(_path, ASSET_PICTURE, ptSize, _startPos, 1.f, 1.f);

	RECT_COPY(m_tFileSize, ptSize);
}

void CProgressBarBase::SetGauge( std::string _path, POINT _size, POINT _startPos, POINT _GaugeIndent )
{
	//	다이알로그바에서 셋팅하는 함수는 이거다.
	//	이거 세팅을 해주면 내부적으로 insert를 하고 게이지를 넣도록 하자.
	//	Gaugebar 구성을 먼저 한다음 다이알로그를 게이지바에 맞춰야겠다.
	RECT ptSize;
	ptSize.left = 0;
	ptSize.top = 0;
	ptSize.right = _size.x;
	ptSize.bottom = _size.y;

	POINT ptPos;
	ptPos.x = m_tPosition.x;
	ptPos.y = m_tPosition.y + (m_pBackground->GetFileSize().bottom / 2.f) - (_size.y / 2.f);


	//	들어온 정보를 토대로 에셋베이스를 만들어서 벡터에 집어넣는다
	CAssetBase* newBase = new CAssetBase;
	newBase->SetPosition(ptPos);
	newBase->SetAsset(_path, ASSET_PICTURE, ptSize, _startPos, 1.f, 1.f);
	m_vecGaugeBar.push_back(newBase);
	m_ptGaugeIndent = _GaugeIndent;
	SetGaugeScale();
}

void CProgressBarBase::SetProgressbarProperty()
{
	POINT ptGaugePos;

	int iBgHeight= 0;
	ptGaugePos.x = 0;
	ptGaugePos.y = 0;

	iBgHeight = m_pBackground->GetFileSize().bottom;

	//	슬라이더 배경, 게이지 위치세팅
	m_pBackground->SetPosition(m_tPosition);

	ptGaugePos.x = m_tPosition.x + m_ptGaugeIndent.x;
	
	SetGaugeScale();

	for(int i = 0; i < m_iVecCount; i++)
	{
		ptGaugePos.y = m_tPosition.y + (iBgHeight / 2.f) - (m_vecGaugeBar[i]->GetFileSize().bottom / 2) + m_ptGaugeIndent.y;
		m_vecGaugeBar[i]->SetPosition(ptGaugePos);
		m_vecGaugeBar[i]->SetScale(m_iGauge * m_vecGaugeBarScale[i], 1.f);
	}
}

void CProgressBarBase::SetFilePath( std::string _Path )
{
	m_strTexPath = _Path;
	m_pBackground->SetFilePath(m_strTexPath);
	
	for(int i = 0; i < m_iVecCount; i++)
	{
		m_vecGaugeBar[i]->SetFilePath(m_strTexPath);
	}

	SetGaugeScale();
}

void CProgressBarBase::ConvertFilePath()
{
	ConvertRelativePath(m_strTexPath);
	for(int i = 0; i < m_iVecCount; i++)
	{
		m_vecGaugeBar[i]->ConvertFilePath();
	}
	m_pBackground->ConvertFilePath();
}

void CProgressBarBase::MakeOriginialFilePath()
{
	MakeAbsolutePath(m_strTexPath);
	for(int i = 0; i < m_iVecCount; i++)
	{
		m_vecGaugeBar[i]->MakeOriginialFilePath();
	}
	m_pBackground->MakeOriginialFilePath();
}

void CProgressBarBase::SetGaugeAmount( int _iNum )
{
	m_iGauge = _iNum;
	SetGaugeScale();
}

void CProgressBarBase::SetGaugeScale()
{
	m_vecGaugeBarScale.clear();

	int iBackgroundWidth = 0;
	int iGaugeWidth = 0;

	m_iVecCount = m_vecGaugeBar.size();
	for(int i = 0; i < m_iVecCount; i++)
	{
		iBackgroundWidth = m_pBackground->GetFileSize().right - m_ptGaugeIndent.x;
		iGaugeWidth = m_vecGaugeBar[i]->GetFileSize().right;
		m_vecGaugeBarScale.push_back(iBackgroundWidth / (float)iGaugeWidth / 100.f);
	}

	if(m_pExtraGauge != NULL)
	{
		iGaugeWidth = m_pExtraGauge->GetFileSize().right;
		m_fExtraGaugeScale = iBackgroundWidth / (float)iGaugeWidth / 100.f;
	}
}

void CProgressBarBase::DestroyGaugeBar()
{
	m_iVecCount = m_vecGaugeBar.size();
	for(int i = 0; i < m_iVecCount; i++)
	{
		SAFE_DELETE(m_vecGaugeBar[i]);
		m_vecGaugeBar[i] = NULL;
	}

	m_vecGaugeBar.clear();
}

void CProgressBarBase::DeleteGaugeBar( int _index )
{
	VEC_GAUGEBAR_ITER iter_begin = m_vecGaugeBar.begin();
	VEC_GAUGEBAR_ITER iter_end = m_vecGaugeBar.end();

	for(iter_begin; iter_begin != iter_end; iter_begin++)
	{
		if((*iter_begin) == m_vecGaugeBar[_index])
		{
			SAFE_DELETE(m_vecGaugeBar[_index]);
			iter_begin = m_vecGaugeBar.erase(iter_begin);
			break;
		}
	}

	VEC_GAUGEBAR_SCALE_ITER iter_scale_begin = m_vecGaugeBarScale.begin();
	VEC_GAUGEBAR_SCALE_ITER iter_scale_end = m_vecGaugeBarScale.end();

	for(iter_scale_begin; iter_scale_begin != iter_scale_end; iter_scale_begin++)
	{
		if((*iter_scale_begin) == m_vecGaugeBarScale[_index])
		{
			iter_scale_begin = m_vecGaugeBarScale.erase(iter_scale_begin);
			break;
		}
	}
}

void CProgressBarBase::SelectGaugeBarUp( int _index )
{
	VEC_GAUGEBAR_ITER iter_begin = m_vecGaugeBar.begin();
	VEC_GAUGEBAR_ITER iter_end = m_vecGaugeBar.end();

	for(iter_begin; iter_begin != iter_end; iter_begin++)
	{
		if((*iter_begin) == m_vecGaugeBar[_index])
		{
			break;
		}
	}

	CAssetBase* insertItem = m_vecGaugeBar[_index];
	m_vecGaugeBar.insert(--iter_begin, insertItem);

	
	VEC_GAUGEBAR_REVITER iter_reverse_begin = m_vecGaugeBar.rbegin();
	VEC_GAUGEBAR_REVITER iter_reverse_end = m_vecGaugeBar.rend();

	for(iter_reverse_begin; iter_reverse_begin != iter_reverse_end; iter_reverse_begin++)
	{
		if((*iter_reverse_begin) == m_vecGaugeBar[_index])
		{
			VEC_GAUGEBAR_ITER originIter = iter_reverse_begin.base();
			m_vecGaugeBar.erase(originIter);
			break;
		}
	}
}

void CProgressBarBase::SelectGaugeBarDown( int _index )
{
	VEC_GAUGEBAR_ITER iter_begin = m_vecGaugeBar.begin();
	VEC_GAUGEBAR_ITER iter_end = m_vecGaugeBar.end();

	for(iter_begin; iter_begin != iter_end; iter_begin++)
	{
		if((*iter_begin) == m_vecGaugeBar[_index])
		{
			break;
		}
	}

	m_vecGaugeBar.insert(++iter_begin, m_vecGaugeBar[_index]);

	iter_begin = m_vecGaugeBar.begin();
	iter_end = m_vecGaugeBar.end();

	for(iter_begin; iter_begin != iter_end; iter_begin++)
	{
		if((*iter_begin) == m_vecGaugeBar[_index])
		{
			iter_begin = m_vecGaugeBar.erase(iter_begin);
			break;
		}
	}
}

void CProgressBarBase::SetIndent( POINT _indent )
{
	m_ptGaugeIndent = _indent;
}

void CProgressBarBase::SetExtraGauge( std::string _path, POINT _size, POINT _startPos )
{
	RECT ptSize;
	ptSize.left = 0;
	ptSize.top = 0;
	ptSize.right = _size.x;
	ptSize.bottom = _size.y;

	POINT ptPos;
	ptPos.x = m_tPosition.x;
	ptPos.y = m_tPosition.y + (m_pBackground->GetFileSize().bottom / 2.f) - (_size.y / 2.f);

	if(m_pExtraGauge != NULL)
	{
		SAFE_DELETE(m_pExtraGauge);
	}

	//	들어온 정보를 토대로 에셋베이스를 만들어서 벡터에 집어넣는다
	CAssetBase* newBase = new CAssetBase;
	newBase->SetPosition(ptPos);
	newBase->SetAsset(_path, ASSET_PICTURE, ptSize, _startPos, 1.f, 1.f);
	m_pExtraGauge = newBase;
	SetGaugeScale();
}

void CProgressBarBase::SetExtraGaugeAmount( int _iNum )
{
	m_iExtraGauge = _iNum;
	SetGaugeScale();
}

void CProgressBarBase::SetScale( float _fscaleX, float _fscaleY )
{
	CBaseInterface::SetScale(_fscaleX, _fscaleY);

	m_pBackground->SetScale(_fscaleX, _fscaleY);

	int iCount = m_vecGaugeBar.size();
	for(int i = 0; i < iCount; i++)
	{
		m_vecGaugeBar[i]->SetScale(_fscaleX, _fscaleY);
	}
}
