#include "StdAfx.h"
#include "ComboBoxBase.h"

#include "TextBox.h"


CComboBoxBase::CComboBoxBase(void)
{
	m_pNormalBtn = new CBtnBase;
	m_pOpenBtn = new CBtnBase;
	m_pDropDownListBox = new CListBoxBase;

	m_pDropDownListBox->SetAssetType(ASSET_LISTBOX);
	m_pOpenBtn->SetAssetType(ASSET_BUTTON);
	m_pNormalBtn->SetAssetType(ASSET_BUTTON);

	m_bDropDown = false;

	m_tPosition.x = 0;
	m_tPosition.y = 0;

	m_iAssetType = ASSET_COMBOBOX;

	m_fScale_X = 1.f;
	m_fScale_Y = 1.f;
}


CComboBoxBase::~CComboBoxBase(void)
{
	SAFE_DELETE(m_pNormalBtn);
	SAFE_DELETE(m_pOpenBtn);
	SAFE_DELETE(m_pDropDownListBox);
}

void CComboBoxBase::SetFilePath( std::string _path )
{
	m_strFilePath = _path;
	m_pNormalBtn->SetFilePath(_path);
	m_pOpenBtn->SetFilePath(_path);
}

CTextBox CComboBoxBase::GetTextBox()
{
	return m_pNormalBtn->GetTextBox();
}

std::string CComboBoxBase::GetFilePath()
{
	return m_strFilePath;
}

CBtnBase* CComboBoxBase::GetNormalBtn()
{
	return m_pNormalBtn;
}

CBtnBase* CComboBoxBase::GetOpenBtn()
{
	return m_pOpenBtn;
}

std::string CComboBoxBase::GetListBoxPath()
{
	return m_strListBoxPath;
}

void CComboBoxBase::SetNormalBtnProperty( POINT _normaltex, POINT _pressedtex, POINT _disabletex, POINT _highlighttex, POINT _BtnSize )
{
	m_pNormalBtn->SetButtonProperty(_normaltex, _pressedtex, _disabletex, _highlighttex, _BtnSize);
}

void CComboBoxBase::SetOpenBtnProperty( POINT _normaltex, POINT _pressedtex, POINT _disabletex, POINT _highlighttex,POINT _BtnSize )
{
	m_pOpenBtn->SetButtonProperty(_normaltex, _pressedtex, _disabletex, _highlighttex, _BtnSize);
}

void CComboBoxBase::SetComboBoxProperty( BYTE _red, BYTE _green, BYTE _blue, int _sort )
{
	m_pNormalBtn->SetText(_red, _green, _blue, _sort);
	m_pOpenBtn->SetText(_red, _green, _blue, _sort);
}

void CComboBoxBase::SetParenthWnd( HWND _hWnd )
{
	m_ParenthWnd = _hWnd;
	m_pOpenBtn->SetParenthWnd(_hWnd);
	m_pNormalBtn->SetParenthWnd(_hWnd);
	m_pDropDownListBox->SetParenthWnd(_hWnd);
}

void CComboBoxBase::Initialize()
{
	m_pDropDownListBox->Initialize();
	m_pNormalBtn->Initialize();
	m_pOpenBtn->Initialize();
}

void CComboBoxBase::Run()
{
	m_pOpenBtn->Run();
	m_pDropDownListBox->Run();
	m_pNormalBtn->Run();

	CheckState();
	SetHitBox();
}

void CComboBoxBase::Render( HDC _DC, eAssetTarget _target )
{
	if(m_bDropDown)
	{
		m_pOpenBtn->Render(_DC, _target);
		m_pDropDownListBox->Render(_DC, _target);
	}
	else
	{
		m_pNormalBtn->Render(_DC, _target);
	}

	//Rectangle(_DC, m_tHitBox.left, m_tHitBox.top, m_tHitBox.right, m_tHitBox.bottom);
}

void CComboBoxBase::Copy( CBaseInterface* _Copy )
{
	CBaseInterface::Copy(_Copy);

	m_bDropDown = ((CComboBoxBase*)_Copy)->m_bDropDown;

	m_strFilePath = ((CComboBoxBase*)_Copy)->m_strFilePath;
	m_strListBoxPath = ((CComboBoxBase*)_Copy)->m_strListBoxPath;

	m_pNormalBtn->Copy(((CComboBoxBase*)_Copy)->m_pNormalBtn);
	m_pOpenBtn->Copy(((CComboBoxBase*)_Copy)->m_pOpenBtn);
	m_pDropDownListBox->Copy(((CComboBoxBase*)_Copy)->m_pDropDownListBox);
}

void CComboBoxBase::Save( std::ofstream& _OfSave )
{
	CBaseInterface::Save(_OfSave);
	_OfSave << "[COMBOBOX START]" << std::endl;
	ConvertRelativePath(m_strFilePath);
	ConvertRelativePath(m_strListBoxPath);
	
	_OfSave << "FilePath" << "\t\t" << m_strFilePath << std::endl;
	_OfSave << "ListBoxPath" << "\t\t" << m_strListBoxPath << std::endl;

	MakeAbsolutePath(m_strFilePath);
	MakeAbsolutePath(m_strListBoxPath);

	m_pNormalBtn->Save(_OfSave);
	m_pOpenBtn->Save(_OfSave);
	m_pDropDownListBox->Save(_OfSave);
	_OfSave << "[COMBOBOX START]" << std::endl;
}

void CComboBoxBase::Load( std::ifstream& _IfLoad )
{
	CBaseInterface::Load(_IfLoad);

	std::string strInput;
	//	StartRead
	std::getline(_IfLoad, strInput, '\n');
	//	FilePath Load
	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	m_strFilePath = strInput;

	//	ListBox Path Load
	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	m_strListBoxPath = strInput;

	m_pNormalBtn->Load(_IfLoad);
	m_pOpenBtn->Load(_IfLoad);
	m_pDropDownListBox->Load(_IfLoad);

	MakeAbsolutePath(m_strFilePath);
	MakeAbsolutePath(m_strListBoxPath);
	std::getline(_IfLoad, strInput, '\n');
}

void CComboBoxBase::SetState( eAssetState _state )
{
	//	콤보박스 드롭다운이 간헐적으로 되지 않는 현상;
	if(_state == STATE_PRESSED && m_iState != _state)
	{
		if(m_bDropDown == false)
		{
			m_bDropDown = true;
		}
		else
		{
			if(m_pOpenBtn->GetState() == STATE_HIGHLIGHT)
			{
				m_bDropDown = false;
			}
		}
	}

	m_iState = _state;

	m_pOpenBtn->SetState(_state);
	m_pNormalBtn->SetState(_state);

	CheckState();
}

void CComboBoxBase::CheckState()
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
		if(m_pDropDownListBox->GetState() == STATE_PRESSED)
		{
			m_pDropDownListBox->SetState(STATE_NORMAL);
		}
	}

	if(m_bDropDown)
	{
		m_pOpenBtn->CheckState();
		m_pDropDownListBox->CheckState();
	}
	else
	{
		m_pNormalBtn->CheckState();
	}

	if(m_iState == STATE_PRESSED)
	{
		if(m_pDropDownListBox->GetState() == STATE_HIGHLIGHT)
		{
			m_pDropDownListBox->SetState(STATE_PRESSED);
		}
	}
	else
	{
		m_pDropDownListBox->SetState(STATE_NORMAL);
		m_pDropDownListBox->CheckState();
		/*if(m_bDropDown)
		{
			m_pOpenBtn->SetState(STATE_NORMAL);
			m_pDropDownListBox->SetState(STATE_NORMAL);
		}
		else
		{
			m_pNormalBtn->SetState(STATE_NORMAL);
		}*/
	}
}

void CComboBoxBase::SetHitBox()
{
	m_tHitBox.left = m_tPosition.x;
	m_tHitBox.top = m_tPosition.y;
	m_tHitBox.right = m_tHitBox.left + m_pNormalBtn->GetFileSize().right;
	m_tHitBox.bottom  = m_tHitBox.top + m_pNormalBtn->GetFileSize().bottom;

	if(m_bDropDown)
	{
		//	드롭다운 상태일 경우에는 드롭다운 리스트의 사이즈를 기존의 버튼 사이즈에 더해주어야한다.
		m_tHitBox.right = m_tHitBox.left + m_pDropDownListBox->GetFileSize().right;
		m_tHitBox.bottom = m_tHitBox.top + m_pDropDownListBox->GetFileSize().bottom + m_pNormalBtn->GetFileSize().bottom;;
	}

	m_tFileSize.left = 0;
	m_tFileSize.top = 0;
	m_tFileSize.right = m_tHitBox.right - m_tHitBox.left;
	m_tFileSize.bottom = m_tHitBox.bottom - m_tHitBox.top;
}

bool CComboBoxBase::SubstractPosition( POINT _pos )
{
	if(m_pDropDownListBox->SubstractPosition(_pos) == false)
	{
		m_tPosition.x -= _pos.x;
		m_tPosition.y -= _pos.y;

		m_pNormalBtn->SubstractPosition(_pos);
		m_pOpenBtn->SubstractPosition(_pos);
	}
	else
	{
		return true;
	}

	return false;
}

void CComboBoxBase::MakeOriginialFilePath()
{
	m_pNormalBtn->MakeOriginialFilePath();
	m_pOpenBtn->MakeOriginialFilePath();
	m_pDropDownListBox->MakeOriginialFilePath();
}

void CComboBoxBase::ConvertFilePath()
{
	m_pNormalBtn->ConvertFilePath();
	m_pOpenBtn->ConvertFilePath();
	m_pDropDownListBox->ConvertFilePath();
}

void CComboBoxBase::SetPosition( POINT _pt )
{
	m_tPosition = _pt;
	m_pNormalBtn->SetPosition(_pt);
	m_pOpenBtn->SetPosition(_pt);

	POINT tPos;
	tPos.x = _pt.x;
	tPos.y = _pt.y + m_pNormalBtn->GetSize().y;
	m_pDropDownListBox->SetPosition(tPos);
}

void CComboBoxBase::SetTextPreview( CString _Preview )
{
	m_pNormalBtn->SetTextPreview(_Preview);
}

void CComboBoxBase::SetListBoxBase( CListBoxBase* _base )
{
	m_pDropDownListBox->Copy(_base);

	POINT tPos;
	tPos.x = m_tPosition.x;
	tPos.y = m_tPosition.y + m_pNormalBtn->GetSize().y;
	m_pDropDownListBox->SetPosition(tPos);
}

void CComboBoxBase::SetDisableText( BYTE _red, BYTE _green, BYTE _blue )
{
	m_pNormalBtn->SetDisableText(_red, _green, _blue);
	m_pOpenBtn->SetDisableText(_red, _green, _blue);
}

void CComboBoxBase::SetbDisable( bool _disable )
{
	m_pNormalBtn->SetbDisable(_disable);
	m_pOpenBtn->SetbDisable(_disable);
}

void CComboBoxBase::SetTextIndent( POINT _indent )
{
	m_pOpenBtn->SetTextIndent(_indent);
	m_pNormalBtn->SetTextIndent(_indent);
}

POINT CComboBoxBase::GetTextIndent()
{
	return m_pNormalBtn->GetTextBox().GetTextIndent();
}

void CComboBoxBase::SetScale( float _fscaleX, float _fscaleY )
{
	CBaseInterface::SetScale(_fscaleX, _fscaleY);

	m_pNormalBtn->SetScale(_fscaleX, _fscaleY);
	m_pOpenBtn->SetScale(_fscaleX, _fscaleY);
	m_pDropDownListBox->SetScale(_fscaleX, _fscaleY);
}
