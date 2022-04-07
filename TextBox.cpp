#include "StdAfx.h"
#include "TextBox.h"


CTextBox::CTextBox(void)
{
	m_iAssetType = ASSET_TEXTBOX;
	CBaseInterface::Initialize();
	m_bDisable = false;
	m_iFontSize = DEFAULT_FONT_SIZE;
	Initialize();
	m_bDefaultShow = true;
	m_bFontBold = false;

	m_iVerticalIndent = 0;
	m_bVerticalIndent = false;

	m_hDC = NULL;
}


CTextBox::~CTextBox(void)
{
}

void CTextBox::Initialize()
{
	CBaseInterface::Initialize();
	RECT_INITIALIZE(m_tTextBox);
	RECT_INITIALIZE(m_tHitBox);
	m_iSortFlag = DT_CENTER | DT_VCENTER | DT_SINGLELINE;
	m_byRed = 0;
	m_byGreen = 0;
	m_byBlue = 0;
	m_tPosition.x = 0;
	m_tPosition.y = 0;
	m_strPreview = "";
	m_iAssetType = ASSET_TEXTBOX;
	m_bDisable = false;
	m_byDisableRed = 0;
	m_byDisableGreen = 0;
	m_byDisableBlue = 0;
	m_ptTextIndent.x = 0;
	m_ptTextIndent.y = 0;

	m_iVerticalIndent = 0;
	m_bVerticalIndent = false;
	m_hDC = NULL;
}

void CTextBox::Render( HDC _DC, eAssetTarget _target )
{
	m_hDC = _DC;
	int nFlag = m_bFontBold == true ? FW_SEMIBOLD : FALSE;
	m_hFont = CreateFont(m_iFontSize, 0, 0, 0, nFlag, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1, VARIABLE_PITCH | FF_ROMAN, L"Gulim");
	m_hOldFont = (HFONT)SelectObject(_DC, m_hFont);

	if(!m_bDisable)
		SetTextColor(_DC,RGB( m_byRed, m_byGreen, m_byBlue));
	else
		SetTextColor(_DC,RGB( m_byDisableRed, m_byDisableGreen, m_byDisableBlue));

	int nOldMode = SetBkMode(_DC,TRANSPARENT);

	//	실시간 텍스트영역 확장을 위한 추가.
	SetHitBox();

	//	텍스트 박스 영역 지정하기 위한 파트
	HPEN CustomPen, OldPen;
	HBRUSH myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH oldBrush = (HBRUSH)SelectObject(_DC, myBrush);
	CustomPen = CreatePen(PS_DOT, 1, BLACK_PEN);
	OldPen = (HPEN)SelectObject(_DC,CustomPen);

	Rectangle(_DC, m_tTextBox.left, m_tTextBox.top, m_tTextBox.right, m_tTextBox.bottom);

	SelectObject(_DC, OldPen);
	SelectObject(_DC, oldBrush);
	DeleteObject(myBrush);
	DeleteObject(CustomPen);
	//	텍스트 박스 출력 완료
	
	RECT tDrawText;
	RECT_COPY(tDrawText,m_tTextBox);
	tDrawText.left += m_ptTextIndent.x;
	tDrawText.top += m_ptTextIndent.y;

	float fCalcIndent = (m_iVerticalIndent + m_iFontSize) / 2.f;

	if(m_bVerticalIndent)
	{
		tDrawText.top = (m_tTextBox.top - fCalcIndent);
		tDrawText.bottom = (m_tTextBox.bottom - fCalcIndent);
		DrawText(_DC, DEFAULT_MULTILINE_TEXT2, -1, &tDrawText, m_iSortFlag);

		tDrawText.top = (m_tTextBox.top + fCalcIndent);
		tDrawText.bottom = (m_tTextBox.bottom + fCalcIndent);
		DrawText(_DC, DEFAULT_MULTILINE_TEXT1, -1, &tDrawText, m_iSortFlag);
	}
	else
	{
		DrawText(_DC, L"Default", -1, &tDrawText, m_iSortFlag);	
	}

	SetBkMode(_DC, nOldMode);
	SelectObject(_DC, m_hOldFont);
	DeleteObject(m_hFont);
}

void CTextBox::Run()
{
	SetHitBox();
}

void CTextBox::SetTextProperty( RECT _rtBox, BYTE _red, BYTE _green, BYTE _blue,UINT _SortFlag )
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

void CTextBox::CalculateTextBox(HDC _DC)
{
	if(m_strPreview != "")
	{
		int iTextboxWidth;
		SIZE Size;
		iTextboxWidth = m_tTextBox.right - m_tTextBox.left;
		GetTextExtentPoint32(_DC, m_strPreview, m_strPreview.GetLength(), &Size);
		if(Size.cx > iTextboxWidth)
		{
			m_tFileSize.right += (Size.cx - iTextboxWidth);
		}
		if(Size.cx < iTextboxWidth)
		{
			m_tFileSize.right -= (iTextboxWidth - Size.cx);
		}
	}
}

RECT CTextBox::GetTextBox()
{
	return m_tTextBox;
}

int CTextBox::GetSortFlag()
{
	return m_iSortFlag;
}

BYTE CTextBox::GetRGB( eRGB _flag )
{
	switch(_flag)
	{
	case RGB_R:
		return m_byRed;
	case RGB_G:
		return m_byGreen;
	case RGB_B:
		return m_byBlue;
	}
}

void CTextBox::SetHitBox()
{
	m_tHitBox.left = m_tPosition.x;
	m_tHitBox.top = m_tPosition.y;
	m_tHitBox.right = m_tHitBox.left + m_tFileSize.right * m_fScale_X;
	m_tHitBox.bottom = m_tHitBox.top + m_tFileSize.bottom * m_fScale_Y;

	RECT_COPY(m_tTextBox, m_tHitBox);
}

void CTextBox::CalculateFileSize()
{
	m_tFileSize.left = 0;
	m_tFileSize.top = 0;
	m_tFileSize.right = m_tTextBox.right - m_tTextBox.left;
	m_tFileSize.bottom = m_tTextBox.bottom - m_tTextBox.top;
}

void CTextBox::Save( std::ofstream& _OfSave )
{
	CBaseInterface::Save(_OfSave);
	_OfSave << "[TEXTBOX START]" << std::endl;
	_OfSave << "TextBox" << "\t\t\t" << m_tFileSize.left << "," << m_tFileSize.top << ","
		<< m_tFileSize.right << "," << m_tFileSize.bottom << std::endl;
	_OfSave << "TextScaling"<<"\t\t" << FontSizeToScale() << std::endl;
	_OfSave << "SortFlag" << "\t\t" << m_iSortFlag << std::endl;
	_OfSave << "RGBA" << "\t\t\t" << (int)m_byRed << "," << (int)m_byGreen << "," << (int)m_byBlue << std::endl;
	_OfSave << "DisableRGB" << "\t\t" << (int)m_byDisableRed << "," << (int)m_byDisableGreen << "," << (int)m_byDisableBlue << std::endl;
	_OfSave << "Indent" << "\t\t\t" << m_ptTextIndent.x << "," << m_ptTextIndent.y << std::endl;
	_OfSave << "DefaultShow" << "\t\t" << m_bDefaultShow << std::endl;

	//폰트 볼드 예외처리
	_OfSave << "[FONT_BOLD_START]" << std::endl;
	_OfSave << m_bFontBold << std::endl;

	//폰트 행간 예외처리
	_OfSave << "[LINE_SPACING]" << std::endl;
	_OfSave << m_iVerticalIndent << std::endl;

	_OfSave << "[TEXTBOX END]" << std::endl;	
}

void CTextBox::Load( std::ifstream& _ifLoad )
{
	std::string strInput;
	CBaseInterface::Load(_ifLoad);

	//	StartRead
	std::getline(_ifLoad, strInput, '\n');

	// TextBox Load
	std::getline(_ifLoad, strInput, '\t');
	std::getline(_ifLoad, strInput, ',');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_tTextBox.left = atoi(strInput.c_str());

	std::getline(_ifLoad, strInput, ',');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_tTextBox.top = atoi(strInput.c_str());

	std::getline(_ifLoad, strInput, ',');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_tTextBox.right = atoi(strInput.c_str());

	std::getline(_ifLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_tTextBox.bottom = atoi(strInput.c_str());

	//	FontSize
	std::getline(_ifLoad, strInput, '\t');
	std::getline(_ifLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	ScaleToFontSize(atof(strInput.c_str()));

	//	SortFlag
	std::getline(_ifLoad, strInput, '\t');
	std::getline(_ifLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_iSortFlag = atoi(strInput.c_str());

	//	RGBA
	std::getline(_ifLoad, strInput, '\t');
	std::getline(_ifLoad, strInput, ',');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_byRed = atoi(strInput.c_str());

	std::getline(_ifLoad, strInput, ',');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_byGreen = atoi(strInput.c_str());

	std::getline(_ifLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_byBlue = atoi(strInput.c_str());

	//	DisableRgba
	std::getline(_ifLoad, strInput, '\t');
	std::getline(_ifLoad, strInput, ',');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_byDisableRed = atoi(strInput.c_str());

	std::getline(_ifLoad, strInput, ',');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_byDisableGreen = atoi(strInput.c_str());

	std::getline(_ifLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_byDisableBlue = atoi(strInput.c_str());

	// TextIndent
	std::getline(_ifLoad, strInput, '\t');
	std::getline(_ifLoad, strInput, ',');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_ptTextIndent.x = atoi(strInput.c_str());

	std::getline(_ifLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_ptTextIndent.y = atoi(strInput.c_str());

	//	Default Show
	std::getline(_ifLoad, strInput, '\t');
	std::getline(_ifLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_bDefaultShow = atoi(strInput.c_str());

	std::getline(_ifLoad, strInput, '\n');

	if(!strcmp(strInput.c_str(), "[FONT_BOLD_START]"))
	{
		std::getline(_ifLoad, strInput, '\n');
		strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
		strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
		m_bFontBold = atoi(strInput.c_str());

		std::getline(_ifLoad, strInput, '\n');
	}

	if(!strcmp(strInput.c_str(), "[LINE_SPACING]"))
	{
		std::getline(_ifLoad, strInput, '\n');
		strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
		strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
		m_iVerticalIndent = atoi(strInput.c_str());

		m_bVerticalIndent = true;

		std::getline(_ifLoad, strInput, '\n');
	}
}

void CTextBox::Copy( CBaseInterface* _Copy )
{
	CBaseInterface::Copy(_Copy);

	RECT_COPY(m_tTextBox, ((CTextBox*)_Copy)->GetTextBox());
	m_iSortFlag = ((CTextBox*)_Copy)->GetSortFlag();
	m_byRed = ((CTextBox*)_Copy)->GetRGB(RGB_R);
	m_byGreen = ((CTextBox*)_Copy)->GetRGB(RGB_G);
	m_byBlue = ((CTextBox*)_Copy)->GetRGB(RGB_B);
	m_ptTextIndent = ((CTextBox*)_Copy)->GetTextIndent();
	m_iFontSize = ((CTextBox*)_Copy)->m_iFontSize;

	m_byDisableRed = ((CTextBox*)_Copy)->GetDisableRGB(RGB_R);
	m_byDisableGreen = ((CTextBox*)_Copy)->GetDisableRGB(RGB_G);
	m_byDisableBlue = ((CTextBox*)_Copy)->GetDisableRGB(RGB_B);
	m_bDefaultShow = ((CTextBox*)_Copy)->m_bDefaultShow;
	m_bFontBold = ((CTextBox*)_Copy)->m_bFontBold;
	m_bVerticalIndent = ((CTextBox*)_Copy)->m_bVerticalIndent;
	m_iVerticalIndent = ((CTextBox*)_Copy)->m_iVerticalIndent;
}

void CTextBox::SetTextPreview( CString _string )
{
	m_strPreview = _string;
}

void CTextBox::SetDisableTextColor( BYTE _red, BYTE _green, BYTE _blue )
{
	m_byDisableRed = _red;
	m_byDisableGreen = _green;
	m_byDisableBlue = _blue;
}

BYTE CTextBox::GetDisableRGB( eRGB _flag )
{
	switch(_flag)
	{
	case RGB_R:
		return m_byDisableRed;
	case RGB_G:
		return m_byDisableGreen;
	case RGB_B:
		return m_byDisableBlue;
	}
}

void CTextBox::SetTextIndent( POINT _indent )
{
	m_ptTextIndent = _indent;
}

void CTextBox::SetFontSize( int _size )
{
	m_iFontSize = _size;
}

float CTextBox::FontSizeToScale()
{
	float fFontScaling = 0.f;
	int iRemainder = m_iFontSize - DEFAULT_FONT_SIZE;
	if(iRemainder < 0)
	{
		fFontScaling = 1.f;
		return fFontScaling;
	}

	fFontScaling = 0.5f * iRemainder;
	fFontScaling += 1.f;
	return fFontScaling;
}

void CTextBox::ScaleToFontSize( float _fScale )
{
	float fScale = _fScale;
	fScale -= 1.f;
	fScale /= 0.5f;
	m_iFontSize = fScale + DEFAULT_FONT_SIZE;
}

int CTextBox::GetFontSize()
{
	return m_iFontSize;
}

void CTextBox::SetDefaultShow( bool _show )
{
	m_bDefaultShow = _show;
}

void CTextBox::SetFontBold(bool _bold)
{
	m_bFontBold = _bold;
}

void CTextBox::SetVerticalIndent( int _iIndent )
{
	m_iVerticalIndent = _iIndent;

	if(m_iVerticalIndent != 0)
	{
		m_bVerticalIndent = true;
	}
	else
	{
		m_bVerticalIndent = false;
	}
}

// 만들다가 실패한 함수 [7/16/2019 Gabrielle]
void CTextBox::SetMultiTextLine( char* _strText, RECT _rtTextBox )
{
	if(m_hDC == NULL)
	{
		return;
	}

	int nFlag = m_bFontBold == true ? FW_SEMIBOLD : FALSE;
	HFONT hFont = CreateFont(m_iFontSize, 0, 0, 0, nFlag, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1, VARIABLE_PITCH | FF_ROMAN, L"Gulim");
	HFONT hOldFont = (HFONT)SelectObject(m_hDC, hFont);

	// 한줄에 들어갈수 있는양을 재서 벡터에 한줄단위로 넣어준다. [7/16/2019 Gabrielle]
	int iTextSize = 0;
	int iMultiTextSize = 0;
	CSize tSize;

	char szBuffer[512] = {0,};
	char szMultiLineBuffer[512] = {0,};

	// 문자열 복사 [7/16/2019 Gabrielle]
	strcpy(szBuffer, _strText);
	iTextSize = strlen(szBuffer);

	int iCharacterLength = 0;
	int iMultiIndex = 0;

	int nFit = 0;
	int nDX[8] = {0,};

	for(int i = 0; i < iTextSize; i+= iCharacterLength)
	{
		int iWordLength = strlen(szMultiLineBuffer);

		BOOL ok = ::GetTextExtentPointA(m_hDC, szMultiLineBuffer, iWordLength, &tSize);
		// 텍스트의 사이즈가 텍스트박스를 넘는 경우 [7/16/2019 Gabrielle]
		if(tSize.cx >= (_rtTextBox.right - _rtTextBox.left) || i == iTextSize - 1)
		{
			std::string strNewLine(szMultiLineBuffer);

			m_vecMultiLine.push_back(strNewLine);
			ZeroMemory(szMultiLineBuffer, sizeof(szMultiLineBuffer));
			iMultiIndex = 0;
		}

		iCharacterLength = 0;
		
		while(szBuffer[iCharacterLength + i] != ' ')
		{
			szMultiLineBuffer[iMultiIndex + iCharacterLength] = szBuffer[i + iCharacterLength];
			++iCharacterLength;
		}

		szMultiLineBuffer[iMultiIndex + iCharacterLength] = szBuffer[i + iCharacterLength];
		szMultiLineBuffer[iMultiIndex + iCharacterLength + 1] = NULL;
		iMultiIndex += (iCharacterLength +1);
		++iCharacterLength;
	}

	SelectObject(m_hDC, m_hOldFont);
	DeleteObject(m_hFont);
}


/*

fScaling	/ FontSize
1		12
1.5		13
2		14
2.5		15
3		16
3.5		17
4		18
4.5		19
5		20
5.5		21
6		22
6.5		23
7		24
7.5		25

*/