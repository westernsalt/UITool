#include "StdAfx.h"
#include "CheckBoxBase.h"


CCheckBoxBase::CCheckBoxBase(void)
{
	m_IconAssetBase = new CAssetBase;
	m_IconDisableAssetBase = new CAssetBase;

	m_iState = BTN_NORMAL;
	m_bChecked = false;
	m_bIconFront = true;
	m_iInterval = 0;
	m_strIconAssetPath = "";
	m_strIconDisableAssetPath = "";
	m_ptIconIndent.x = 0;
	m_ptIconIndent.y = 0;
}


CCheckBoxBase::~CCheckBoxBase(void)
{
	SAFE_DELETE(m_IconAssetBase);
	SAFE_DELETE(m_IconDisableAssetBase);
}

void CCheckBoxBase::SetCheckBoxProperty( POINT _normaltex, POINT _pressedtex, POINT _disabletex, POINT _highlighttex, POINT _checkmarktex, POINT _BtnSize, POINT _checkMarkSize, int _interval )
{
	SetButtonProperty(_normaltex, _pressedtex, _disabletex, _highlighttex, _BtnSize);
	m_ptCheckMarkTex.x = _checkmarktex.x;
	m_ptCheckMarkTex.y = _checkmarktex.y;
	m_ptCheckMarkSize.x = _checkMarkSize.x;
	m_ptCheckMarkSize.y = _checkMarkSize.y;
	m_ptCheckMarkPos.x = m_tPosition.x + (_BtnSize.x / 2) - (m_ptCheckMarkSize.x / 2);
	m_ptCheckMarkPos.y = m_tPosition.y + (_BtnSize.y / 2) - (m_ptCheckMarkSize.y / 2);
	m_iInterval = _interval;
}

void CCheckBoxBase::Initialize()
{
	CBtnBase::Initialize();
}

void CCheckBoxBase::Run()
{
	CBaseInterface::CheckState();
	CAssetBase::Run();
	CalculateTextBox();
	m_TextBox.Run();

	if(m_strIconAssetPath != "")
	{
		m_IconAssetBase->Run();
		m_IconDisableAssetBase->Run();
	}

	CalculateHitBox();
}

void CCheckBoxBase::Render( HDC _DC, eAssetTarget _target )
{
	std::string strDrawPath = "";

	//Rectangle(_DC, m_tHitBox.left, m_tHitBox.top, m_tHitBox.right, m_tHitBox.bottom);

	strDrawPath = m_AssetBaseSaveData.m_szFilePath;

	if(strDrawPath != "")
	{
		CA2T wt(strDrawPath.c_str());

		if( m_cTexture == NULL )
		{
			m_cTexture = new CxImage;			
			m_bResult = m_cTexture->Load(wt, CXIMAGE_FORMAT_TGA);
		}

		if(m_bResult == true)		
		{
			::SetStretchBltMode(_DC, COLORONCOLOR);
			CxImage CropImage = *m_cTexture;
			RECT rtCrop;

			switch(m_iState)
			{
			case STATE_NORMAL:
				rtCrop.left = m_ptNormalTex.x;
				rtCrop.top = m_ptNormalTex.y;
				break;
			case STATE_DISABLE:
				rtCrop.left = m_ptDisableTex.x;
				rtCrop.top = m_ptDisableTex.y;
				break;
			case STATE_HIGHLIGHT:
				rtCrop.left = m_ptHighlightTex.x;
				rtCrop.top = m_ptHighlightTex.y;
				break;
			case STATE_PRESSED:
				rtCrop.left = m_ptPressedTex.x;
				rtCrop.top = m_ptPressedTex.y;
				break;
			}

			rtCrop.right = rtCrop.left + m_tSize.x;
			rtCrop.bottom = rtCrop.top + m_tSize.y;

			CropImage.Crop(rtCrop);
			CropImage.Draw(_DC, m_tPosition.x, m_tPosition.y, m_tSize.x * m_fScale_X
				, m_tSize.y * m_fScale_Y);

			if(m_bChecked)
			{
				RECT rtMark;
				m_ptCheckMarkPos.x = m_tPosition.x + (m_tSize.x * m_fScale_X / 2) - (m_ptCheckMarkSize.x * m_fScale_X / 2);
				m_ptCheckMarkPos.y = m_tPosition.y + (m_tSize.y * m_fScale_Y / 2) - (m_ptCheckMarkSize.y * m_fScale_Y / 2);
				rtMark.left = m_ptCheckMarkTex.x;
				rtMark.top = m_ptCheckMarkTex.y;
				rtMark.right = rtMark.left + m_ptCheckMarkSize.x;
				rtMark.bottom = rtMark.top + m_ptCheckMarkSize.y;
				CxImage CropMarkImg = *m_cTexture;
				CropMarkImg.Crop(rtMark);
				CropMarkImg.Draw(_DC, m_ptCheckMarkPos.x, m_ptCheckMarkPos.y, m_ptCheckMarkSize.x * m_fScale_X, m_ptCheckMarkSize.y * m_fScale_Y);
			}
		}
	}


	if(m_strIconAssetPath != "")
	{
		if(m_iState != STATE_DISABLE)
		{
			m_IconAssetBase->Render(_DC, _target);
		}
		else
		{
			m_IconDisableAssetBase->Render(_DC, _target);
		}
	}

	m_TextBox.Render(_DC, _target);
}

void CCheckBoxBase::Save( std::ofstream& _OfSave )
{
	RECT tOriginSize;
	RECT_COPY(tOriginSize, m_tFileSize);
	RECT_COPY(m_tFileSize, m_tHitBox);
	ConvertRelativePath(m_strIconAssetPath);
	ConvertRelativePath(m_strIconDisableAssetPath);
	CBtnBase::Save(_OfSave);
	_OfSave << "[CHECKBOX START]" << std::endl;
	_OfSave << "CheckMarkPos" << "\t" << m_ptCheckMarkPos.x - m_tPosition.x << "," << m_ptCheckMarkPos.y - m_tPosition.y << std::endl;
	_OfSave << "CheckMarkTex" << "\t" << m_ptCheckMarkTex.x << "," << m_ptCheckMarkTex.y << std::endl;
	_OfSave << "CheckMarkSize" << "\t" << m_ptCheckMarkSize.x << "," << m_ptCheckMarkSize.y << std::endl;
	_OfSave << "Interval" << "\t\t" << m_iInterval << std::endl;
	_OfSave << "IconFront" << "\t\t" << m_bIconFront << std::endl;
	_OfSave << "IconPath" << "\t\t" << m_strIconAssetPath << std::endl;
	_OfSave << "DisableIconPath" << "\t" << m_strIconDisableAssetPath << std::endl;
	_OfSave << "IconIndent" << "\t\t" << m_ptIconIndent.x << "," << m_ptIconIndent.y << std::endl;

	POINT MinusPos;
	MinusPos.x = m_tPosition.x * -1;
	MinusPos.y = m_tPosition.y * -1;

	m_IconAssetBase->SubstractPosition(m_tPosition);
	m_IconDisableAssetBase->SubstractPosition(m_tPosition);

	m_IconAssetBase->ConvertFilePath();
	m_IconAssetBase->Save(_OfSave);
	m_IconDisableAssetBase->ConvertFilePath();
	m_IconDisableAssetBase->Save(_OfSave);

	m_IconAssetBase->SubstractPosition(MinusPos);
	m_IconDisableAssetBase->SubstractPosition(MinusPos);

	_OfSave << "[CHECKBOX END]" << std::endl;
	MakeAbsolutePath(m_strIconAssetPath);
	MakeAbsolutePath(m_strIconDisableAssetPath);
	RECT_COPY(m_tFileSize, tOriginSize);
}

void CCheckBoxBase::Load( std::ifstream& _IfLoad )
{
	std::string strInput;

	CBtnBase::Load(_IfLoad);
	//	StartRead
	std::getline(_IfLoad, strInput, '\n');

	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, ',');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_ptCheckMarkPos.x = atoi(strInput.c_str());
	m_ptCheckMarkPos.x += m_tPosition.x;

	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_ptCheckMarkPos.y = atoi(strInput.c_str());
	m_ptCheckMarkPos.y += m_tPosition.y;

	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, ',');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_ptCheckMarkTex.x = atoi(strInput.c_str());

	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_ptCheckMarkTex.y = atoi(strInput.c_str());

	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, ',');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_ptCheckMarkSize.x = atoi(strInput.c_str());

	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_ptCheckMarkSize.y = atoi(strInput.c_str());

	// Interval
	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_iInterval = atoi(strInput.c_str());

	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_bIconFront = atoi(strInput.c_str());

	//	아이콘 패스
	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_strIconAssetPath = strInput;

	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_strIconDisableAssetPath = strInput;

	//	IconIndent
	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, ',');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_ptIconIndent.x = atoi(strInput.c_str());

	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_ptIconIndent.y = atoi(strInput.c_str());

	m_ptCheckMarkPos.x = m_tPosition.x + (m_tSize.x/*m_tFileSize.right*/ / 2) - (m_ptCheckMarkSize.x / 2);
	m_ptCheckMarkPos.y = m_tPosition.y + (m_tSize.y/*m_tFileSize.bottom*/ / 2) - (m_ptCheckMarkSize.y / 2);

	m_IconAssetBase->Load(_IfLoad);
	m_IconDisableAssetBase->Load(_IfLoad);

	POINT MinusPos;
	MinusPos.x = m_tPosition.x * -1;
	MinusPos.y = m_tPosition.y * -1;

	m_IconAssetBase->SubstractPosition(MinusPos);
	m_IconDisableAssetBase->SubstractPosition(MinusPos);

	if(m_strIconAssetPath != "")
		MakeAbsolutePath(m_strIconAssetPath);

	if(m_strIconDisableAssetPath != "")
		MakeAbsolutePath(m_strIconDisableAssetPath);

	CalculateTextBox();

	std::getline(_IfLoad, strInput, '\n');

	m_tFileSize.right = m_tSize.x;
	m_tFileSize.bottom = m_tSize.y;
}

void CCheckBoxBase::Copy( CBaseInterface* _Copy )
{
	CBtnBase::Copy(_Copy);
	m_ptCheckMarkTex = ((CCheckBoxBase*)_Copy)->m_ptCheckMarkTex;
	m_ptCheckMarkSize = ((CCheckBoxBase*)_Copy)->m_ptCheckMarkSize;
	m_ptCheckMarkPos = ((CCheckBoxBase*)_Copy)->m_ptCheckMarkPos;
	m_iInterval = ((CCheckBoxBase*)_Copy)->m_iInterval;
	m_bIconFront = ((CCheckBoxBase*)_Copy)->m_bIconFront;

	m_strIconAssetPath = ((CCheckBoxBase*)_Copy)->m_strIconAssetPath;
	m_strIconDisableAssetPath = ((CCheckBoxBase*)_Copy)->m_strIconDisableAssetPath;

	m_ptIconIndent = ((CCheckBoxBase*)_Copy)->m_ptIconIndent;

	m_IconAssetBase->Copy( ((CCheckBoxBase*)_Copy)->m_IconAssetBase );
	m_IconDisableAssetBase->Copy( ((CCheckBoxBase*)_Copy)->m_IconDisableAssetBase );

	CalculateTextBox();
}

void CCheckBoxBase::SetState( eAssetState _state )
{
	m_iPrevState = m_iState;
	m_iState = _state;

	if(m_iState == STATE_PRESSED && m_iPrevState!=STATE_PRESSED)
	{
		m_bChecked = !m_bChecked;
	}
}

void CCheckBoxBase::SetText( BYTE _r, BYTE _g, BYTE _b, int _sort )
{
	m_TextBox.SetTextProperty(m_tHitBox,_r, _g, _b, _sort);
	CalculateTextBox();
}

void CCheckBoxBase::CalculateTextBox()
{
	POINT tTextBoxPos;
	tTextBoxPos.x = m_tPosition.x + m_tSize.x/*m_tFileSize.right*/ * m_fScale_X + m_iInterval;

	if(m_strIconAssetPath != "")
	{
		POINT ptIconPos;
		ptIconPos.x = 0;
		ptIconPos.y = 0;

		if(m_bIconFront)
		{
			ptIconPos.x = m_tPosition.x + m_tSize.x/*m_tFileSize.right*/ + m_ptIconIndent.x;
			ptIconPos.y = m_ptIconIndent.y + m_tPosition.y;
			m_IconAssetBase->SetPosition(ptIconPos);
			m_IconDisableAssetBase->SetPosition(ptIconPos);
			tTextBoxPos.x += m_IconAssetBase->GetFileSize().right + m_iInterval + m_ptIconIndent.x;
		}
		else
		{
			ptIconPos.x = tTextBoxPos.x + m_tSize.x/*m_tFileSize.right*/ + m_ptIconIndent.x;
			ptIconPos.y = m_tPosition.y + m_ptIconIndent.y;
			m_IconAssetBase->SetPosition(ptIconPos);
			m_IconDisableAssetBase->SetPosition(ptIconPos);
		}
	}

	tTextBoxPos.y = m_tPosition.y;
	m_TextBox.SetPosition(tTextBoxPos);
	m_TextBox.SetHitBox();
}

void CCheckBoxBase::CalculateHitBox()
{
	//	텍스트박스의 기본 히트박스는 체크박스와 높낮이가 같다.
	//	따라서 오른쪽만 늘려주면 되지만 혹시모를 계산미스를 방지하기 위해서
	//	최우측하단 점을 히트박스의 right, bottom으로 지정한다
	m_tHitBox.left = m_tPosition.x;
	m_tHitBox.top = m_tPosition.y;
	m_tHitBox.right = m_tPosition.x + m_tSize.x * m_fScale_X * 2;
	m_tHitBox.bottom = m_tPosition.y + m_tSize.y * m_fScale_Y;
}

void CCheckBoxBase::SetPosition( POINT _pt )
{
	m_tPosition = _pt;
	CalculateTextBox();
}

void CCheckBoxBase::SetIconAsset( CAssetBase* _base, eIconPosition _iconPos )
{
	m_IconAssetBase->Copy(_base);
	SetIconPosition(_iconPos);
}

void CCheckBoxBase::SetIconAssetPath( std::string _path )
{
	m_strIconAssetPath = _path;
}

bool CCheckBoxBase::SubstractPosition( POINT _pos )
{
	CBtnBase::SubstractPosition(_pos);
	if(m_strIconAssetPath != "")
	{
		m_IconAssetBase->SubstractPosition(_pos);
		m_IconDisableAssetBase->SubstractPosition(_pos);
	}
	return false;
}

void CCheckBoxBase::SetIconIndent( POINT _indent )
{
	m_ptIconIndent = _indent;
}

void CCheckBoxBase::SetIconDisableAssetPath( std::string _path )
{
	m_strIconDisableAssetPath = _path;
}

void CCheckBoxBase::SetDisableIconAsset( CAssetBase* _base, eIconPosition _iconPos )
{
	m_IconDisableAssetBase->Copy(_base);

	SetIconPosition(_iconPos);
}

void CCheckBoxBase::SetIconPosition( eIconPosition _iconPos )
{
	if(ICON_BACK == _iconPos)
	{
		m_bIconFront = false;
	}
	else if(ICON_FRONT == _iconPos)
	{
		m_bIconFront = true;
	}

	CalculateTextBox();
}

void CCheckBoxBase::SetScale( float _fscaleX, float _fscaleY )
{
	CBtnBase::SetScale(_fscaleX, _fscaleY);
}
