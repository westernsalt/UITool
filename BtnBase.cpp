#include "StdAfx.h"
#include "BtnBase.h"


CBtnBase::CBtnBase(void)
{
	m_iState = BTN_NORMAL;
	m_iAssetType  = ASSET_BUTTON;
	m_bTextBoxSetApart = false;
	m_bIconFont = true;
	m_iIconIndent = 0;
}


CBtnBase::~CBtnBase(void)
{
}

void CBtnBase::Initialize()
{
	CAssetBase::Initialize();
}

void CBtnBase::Run()
{
	CBaseInterface::CheckState();
	CAssetBase::Run();
	SetTextBoxPosition();
	m_TextBox.Run();
	SetHitBox();
}

void CBtnBase::Render(HDC _DC, eAssetTarget _target)
{
	std::string strDrawPath = "";

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

			if(m_bTextBoxSetApart)
			{
				if(m_bIconFont)
				{
					CropImage.Draw(_DC, m_tPosition.x, m_tPosition.y,
						m_tSize.x * m_fScale_X, m_tSize.y * m_fScale_Y);
				}
				else
				{
					CropImage.Draw(_DC, m_TextBox.GetHitBox()->right + m_iIconIndent, m_tPosition.y,
						m_tSize.x * m_fScale_X, m_tSize.y * m_fScale_Y);
				}
			}
			else
			{
				CropImage.Draw(_DC, m_tPosition.x, m_tPosition.y,
					m_tSize.x * m_fScale_X, m_tSize.y * m_fScale_Y);
			}
		}
	}
	m_TextBox.Render(_DC, _target);
}

void CBtnBase::SetButtonProperty( POINT _normaltex, POINT _pressedtex, POINT _disabletex, POINT _highlighttex, POINT _btnsize )
{
	m_ptNormalTex.x = _normaltex.x;
	m_ptNormalTex.y = _normaltex.y;

	m_ptPressedTex.x = _pressedtex.x;
	m_ptPressedTex.y = _pressedtex.y;

	m_ptDisableTex.x = _disabletex.x;
	m_ptDisableTex.y = _disabletex.y;

	m_ptHighlightTex.x = _highlighttex.x;
	m_ptHighlightTex.y = _highlighttex.y;

	m_tSize.x = _btnsize.x * m_fScale_X;
	m_tSize.y = _btnsize.y * m_fScale_Y;

	m_iState = BTN_NORMAL;
	m_tFileSize.right = m_tSize.x * m_fScale_X;
	m_tFileSize.bottom = m_tSize.y * m_fScale_Y;
	
	m_tHitBox.left = m_tPosition.x;
	m_tHitBox.top = m_tPosition.y;
	m_tHitBox.right = m_tHitBox.left + m_tSize.x * m_fScale_X;
	m_tHitBox.bottom = m_tHitBox.top + m_tSize.y * m_fScale_Y;
}

void CBtnBase::Save( std::ofstream& _OfSave )
{
	CAssetBase::Save(_OfSave);

	_OfSave << "[BUTTON START]" << std::endl;
	_OfSave << "NormalTex" << "\t\t"	<< m_ptNormalTex.x << "," << m_ptNormalTex.y << std::endl;
	_OfSave << "PressedTex" << "\t\t"	<< m_ptPressedTex.x << "," << m_ptPressedTex.y << std::endl;
	_OfSave << "DisableTex" << "\t\t"	<< m_ptDisableTex.x << "," << m_ptDisableTex.y << std::endl;
	_OfSave << "HighlightTex" << "\t"	<< m_ptHighlightTex.x << "," << m_ptHighlightTex.y << std::endl;
	_OfSave << "Size" << "\t\t\t" << m_tSize.x << "," << m_tSize.y << std::endl;
	_OfSave << "TextBoxSetApart" << "\t" << m_bTextBoxSetApart << std::endl;
	_OfSave << "IconFront" << "\t\t" << m_bIconFont << std::endl;
	_OfSave << "TextBoxIndent" << "\t" << m_iIconIndent << std::endl;
	
	POINT ptBtnPosition = m_tPosition;
	m_TextBox.SubstractPosition(ptBtnPosition);
	m_TextBox.Save(_OfSave);
	m_TextBox.AddPosition(ptBtnPosition);

	_OfSave << "[BUTTON END]" << std::endl;
}

void CBtnBase::Load( std::ifstream& _IfLoad )
{
	CAssetBase::Load(_IfLoad);

	std::string strInput;
	//	StartRead
	std::getline(_IfLoad, strInput, '\n');
	//	Normal
	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, ',');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_ptNormalTex.x = atoi(strInput.c_str());

	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_ptNormalTex.y = atoi(strInput.c_str());

	//	Pressed
	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, ',');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_ptPressedTex.x = atoi(strInput.c_str());

	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_ptPressedTex.y = atoi(strInput.c_str());

	//	Disable
	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, ',');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_ptDisableTex.x = atoi(strInput.c_str());

	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_ptDisableTex.y = atoi(strInput.c_str());

	//	Highlight
	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, ',');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_ptHighlightTex.x = atoi(strInput.c_str());

	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_ptHighlightTex.y = atoi(strInput.c_str());

	//	Size
	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, ',');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_tSize.x = atoi(strInput.c_str());

	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_tSize.y = atoi(strInput.c_str());

	//	textboxsetapart
	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_bTextBoxSetApart = atoi(strInput.c_str());

	//	IconFront
	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_bIconFont = atoi(strInput.c_str());

	//	Indent
	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_iIconIndent = atoi(strInput.c_str());

	m_TextBox.Load(_IfLoad);

	std::getline(_IfLoad, strInput, '\n');
}


void CBtnBase::Copy( CBaseInterface* _Copy )
{
	CAssetBase::Copy(_Copy);

	m_ptNormalTex = ((CBtnBase*)_Copy)->m_ptNormalTex;
	m_ptPressedTex = ((CBtnBase*)_Copy)->m_ptPressedTex;
	m_ptDisableTex = ((CBtnBase*)_Copy)->m_ptDisableTex;
	m_ptHighlightTex = ((CBtnBase*)_Copy)->m_ptHighlightTex;

	m_tSize = ((CBtnBase*)_Copy)->m_tSize;
	m_TextBox = ((CBtnBase*)_Copy)->m_TextBox;
	m_bIconFont = ((CBtnBase*)_Copy)->m_bIconFont;
	m_bTextBoxSetApart = ((CBtnBase*)_Copy)->m_bTextBoxSetApart;
	m_iIconIndent = ((CBtnBase*)_Copy)->m_iIconIndent;
}

void CBtnBase::SetText( BYTE _r, BYTE _g, BYTE _b, int _sort )
{
	m_TextBox.SetPosition(m_tPosition);
	m_TextBox.SetTextProperty(m_tHitBox, _r, _g, _b, _sort);
}

void CBtnBase::SetPosition( POINT _pt )
{
	m_tPosition = _pt;
}

void CBtnBase::SetTextPreview( CString _string )
{
	m_TextBox.SetTextPreview(_string);
}

void CBtnBase::SetDisableText( BYTE _r, BYTE _g, BYTE _b )
{
	m_TextBox.SetDisableTextColor(_r, _g, _b);
}

void CBtnBase::SetbDisable( bool _disable )
{
	m_bDisable = _disable;
	m_TextBox.SetbDisable(_disable);

	if(m_bDisable)
	{
		SetState(STATE_DISABLE);
	}
	else
	{
		SetState(STATE_NORMAL);
	}
}

void CBtnBase::SetTextIndent( POINT _indent )
{
	m_TextBox.SetTextIndent(_indent);
}

void CBtnBase::SetTextSize( int _iSize )
{
	m_TextBox.SetFontSize(_iSize);
}

int CBtnBase::GetTextSize()
{
	return m_TextBox.GetFontSize();
}

void CBtnBase::SetDefaultShow( bool _show )
{
	m_TextBox.SetDefaultShow(_show);
}

void CBtnBase::SetFontBold(bool _bold)
{
	m_TextBox.SetFontBold(_bold);
}

void CBtnBase::SetbIconFont( bool _set )
{
	m_bIconFont = _set;
}

void CBtnBase::SetbTextBoxSetApart( bool _set )
{
	m_bTextBoxSetApart = _set;
}

void CBtnBase::SetTextBoxPosition()
{
	if(m_bTextBoxSetApart == true)
	{
		if(m_bIconFont)
		{
			POINT TextBoxPosition;
			TextBoxPosition.x = m_tPosition.x + m_tSize.x + m_iIconIndent;
			TextBoxPosition.y = m_tPosition.y;

			m_TextBox.SetPosition(TextBoxPosition);
		}
		else
		{
			m_TextBox.SetPosition(m_tPosition);
		}
	}
	else
	{
		m_TextBox.SetPosition(m_tPosition);
	}
}

void CBtnBase::SetHitBox()
{
	if(m_bTextBoxSetApart == true)
	{
		m_tHitBox.left = m_tPosition.x;
		m_tHitBox.top = m_tPosition.y;
		m_tHitBox.right = m_tPosition.x + m_tSize.x * 2 + m_iIconIndent;
		m_tHitBox.bottom = m_tHitBox.top + m_tSize.y * m_fScale_Y;
	}
	else
	{
		m_tHitBox.left = m_tPosition.x;
		m_tHitBox.top = m_tPosition.y;
		m_tHitBox.right = m_tHitBox.left + m_tSize.x * m_fScale_X;
		m_tHitBox.bottom = m_tHitBox.top + m_tSize.y * m_fScale_Y;
	}

	m_tFileSize.right = m_tHitBox.right - m_tHitBox.left;
	m_tFileSize.bottom = m_tHitBox.bottom - m_tHitBox.top;
}

bool CBtnBase::GetTextBoxSetApart()
{
	return m_bTextBoxSetApart;
}

bool CBtnBase::GetIconFront()
{
	return m_bIconFont;
}

int CBtnBase::GetIconIndent()
{
	return m_iIconIndent;
}

void CBtnBase::SetTextboxIndent( int _indent )
{
	m_iIconIndent = _indent;
}

void CBtnBase::SetScale( float _fscaleX, float _fscaleY )
{
	m_fScale_X = _fscaleX;
	m_fScale_Y = _fscaleY;

	m_TextBox.SetScale(_fscaleX, _fscaleY);
}

void CBtnBase::SetTextBoxVerticalIndent( int _iIndent )
{
	m_TextBox.SetVerticalIndent(_iIndent);
}
