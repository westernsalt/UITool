#include "StdAfx.h"
#include "InputText.h"

#include "AssetBase.h"
#include "ObjMgr.h"

CInputText::CInputText(void)
{
	m_pTextBoxBG = new CAssetBase;
	m_bFocus = false;
	m_bPassword = false;
	m_iInterval = 0;
	m_pTexturePath = "";
}


CInputText::~CInputText(void)
{
	SAFE_DELETE(m_pTextBoxBG);
}

void CInputText::Initialize()
{
	CTextBox::Initialize();
}

void CInputText::Run()
{
	CTextBox::Run();
}

void CInputText::Render( HDC _DC, eAssetTarget _target )
{
	if(m_bFocus)
	{
		::SendMessage(m_ParenthWnd, WM_REQUEST_STRING, (WPARAM)&m_strPreview, NULL);
	}

	int nBoldFlag = m_bFontBold == true ? FW_SEMIBOLD : FALSE;
	m_hFont =CreateFont(m_iFontSize, 0, 0, 0, nBoldFlag, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1, VARIABLE_PITCH | FF_ROMAN, L"Gulim");
	m_hOldFont = (HFONT)SelectObject(_DC, m_hFont);

	if(m_bDisable)
	{
		SetTextColor(_DC,RGB( m_byDisableRed, m_byDisableGreen, m_byDisableBlue));
	}
	else
		SetTextColor(_DC,RGB( m_byRed, m_byGreen, m_byBlue));

	int nOldMode = SetBkMode(_DC,TRANSPARENT);

	//	실시간 텍스트영역 확장을 위한 추가.
	SetHitBox();

	m_pTextBoxBG->Render(_DC, _target);

	if(m_strPreview != "" && m_bPassword == false)
	{
		DrawText(_DC, m_strPreview, -1, &m_tTextBox, m_iSortFlag);	
	}
	else if(m_bPassword)
	{
		CString strPassword;
		for(int i = 0; i<m_strPreview.GetLength(); i++)
		{
			strPassword.AppendChar('*');
		}
		DrawText(_DC, strPassword, -1, &m_tTextBox, m_iSortFlag);
	}

	SetBkMode(_DC, nOldMode);
	SelectObject(_DC, m_hOldFont);
	DeleteObject(m_hFont);
}

void CInputText::Copy( CBaseInterface* _Copy )
{
	CTextBox::Copy(_Copy);
	m_iInterval = ((CInputText*)_Copy)->m_iInterval;
	m_pTexturePath = ((CInputText*)_Copy)->m_pTexturePath;
	m_bPassword = ((CInputText*)_Copy)->m_bPassword;
	m_pTextBoxBG->Copy(((CInputText*)_Copy)->m_pTextBoxBG);
	m_bFontBold = ((CInputText*)_Copy)->m_bFontBold;
}

void CInputText::Save( std::ofstream& _OfSave )
{
	CTextBox::Save(_OfSave);
	_OfSave << "[INPUTTEXT START]" << std::endl;
	_OfSave << "PasswordType" << "\t" << m_bPassword << std::endl;
	_OfSave << "Interval" << "\t\t" << m_iInterval << std::endl;
	m_pTextBoxBG->Save(_OfSave);
	_OfSave << "[INPUTTEXT END]" << std::endl;
}

void CInputText::Load( std::ifstream& _IfLoad )
{
	CTextBox::Load(_IfLoad);

	m_bFontBold = CTextBox::GetFontBold();

	std::string strInput;
	
	//	StartRead
	std::getline(_IfLoad, strInput, '\n');

	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_bPassword = atoi(strInput.c_str());

	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_iInterval = atoi(strInput.c_str());

	m_pTextBoxBG->Load(_IfLoad);
	std::getline(_IfLoad, strInput, '\n');
}

void CInputText::SetState( eAssetState _state )
{
	CTextBox::SetState(_state);

	if(_state == STATE_PRESSED)
	{
		if(m_bFocus == false)
		{
			::SendMessage(m_ParenthWnd, WM_INIT_STRING, NULL, NULL);
		}
		m_bFocus = true;
	}
}

void CInputText::CheckState()
{
	CBaseInterface::CheckState();
}

void CInputText::SetHitBox()
{
	CTextBox::SetHitBox();
	m_pTextBoxBG->SetHitBox();
	m_tTextBox.left += m_iInterval;
}

void CInputText::SetParenthWnd( HWND _hWnd )
{
	m_ParenthWnd = _hWnd;
	m_pTextBoxBG->SetParenthWnd(_hWnd);
}

void CInputText::AddPosition( POINT _pos )
{
	CBaseInterface::AddPosition(_pos);
	m_pTextBoxBG->AddPosition(_pos);
}

bool CInputText::SubstractPosition( POINT _pos )
{
	CBaseInterface::SubstractPosition(_pos);
	m_pTextBoxBG->SubstractPosition(_pos);
	return false;
}

void CInputText::MakeOriginialFilePath()
{
	m_pTextBoxBG->MakeOriginialFilePath();
}

void CInputText::ConvertFilePath()
{
	m_pTextBoxBG->ConvertFilePath();
}

void CInputText::SetPosition( POINT _pt )
{
	CBaseInterface::SetPosition(_pt);
	m_pTextBoxBG->SetPosition(_pt);
}

void CInputText::SetBackground(std::string _path, POINT _startpos, POINT _size )
{
	m_tFileSize.left = 0;
	m_tFileSize.top = 0;
	m_tFileSize.right = _size.x;
	m_tFileSize.bottom = _size.y;

	m_pTextBoxBG->SetAsset(_path, ASSET_PICTURE, m_tFileSize, _startpos, 1.f, 1.f);
}

void CInputText::FreeFocus()
{
	m_bFocus = false;
}

void CInputText::InputText( std::string _text )
{
	if(m_bFocus)
	{
		CString szInput(_text.c_str());
		m_strPreview = szInput;
	}
}

std::string CInputText::GetFilePath()
{
	return m_pTextBoxBG->GetFilePath();
}

POINT CInputText::GetBackgroundStartPos()
{
	return m_pTextBoxBG->GetStartPos();
}

RECT CInputText::GetBackgroundSize()
{
	return m_pTextBoxBG->GetFileSize();
}

int CInputText::GetInterval()
{
	return m_iInterval;
}

bool CInputText::GetbPassword()
{
	return m_bPassword;
}

bool CInputText::GetFontBold()
{
	return m_bFontBold;
}

void CInputText::SetInputTextProperty( RECT _rtBox, BYTE _red, BYTE _green, BYTE _blue, UINT _SortFlag )
{
	RECT_COPY(m_tTextBox, _rtBox);
	m_byRed = _red;
	m_byGreen = _green;
	m_byBlue = _blue;
	m_iSortFlag = _SortFlag;

	//	텍스트 박스의 크기를 알았으니 현재 텍스트 박스의 절대 크기를 측정한다
	CalculateFileSize();
	//	텍스트 박스의 크기를 알았으니 히트박스를 초기화해준다.
	SetHitBox();
}

void CInputText::SetFocus()
{
	m_bFocus = true;
}

void CInputText::SetbPassword( bool _bPassword )
{	
	m_bPassword = _bPassword;
}

void CInputText::SetFontBold(bool _bBold)
{
	CTextBox::SetFontBold(_bBold);
	m_bFontBold = _bBold;
}

void CInputText::SetInterval( int _interval )
{
	m_iInterval = _interval;
}

void CInputText::SetScale( float _fscaleX, float _fscaleY )
{
	CBaseInterface::SetScale(_fscaleX, _fscaleY);

	m_pTextBoxBG->SetScale(_fscaleX, _fscaleY);
}
