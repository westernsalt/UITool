#pragma once

#include "BaseInterface.h"

class CTextBox : public CBaseInterface
{
protected:
	//	Text 구현에 필요한 요소
	RECT	m_tTextBox;
	UINT	m_iSortFlag;
	int		m_iFontSize;
	bool	m_bDefaultShow;
	BYTE	m_byRed;
	BYTE	m_byGreen;
	BYTE	m_byBlue;

	BYTE	m_byDisableRed;
	BYTE	m_byDisableGreen;
	BYTE	m_byDisableBlue;

	HFONT	m_hFont;
	HFONT	m_hOldFont;

	HDC		m_hDC;

	POINT	m_ptTextIndent;

	//	텍스트 미리보기 지원
	CString	m_strPreview;

	bool	m_bFontBold;

	// 멀티라인 텍스트 행간 추가 [7/15/2019 Gabrielle]
	int		m_iVerticalIndent;
	bool	m_bVerticalIndent;
	std::vector<std::string> m_vecMultiLine;

private:
	void	ScaleToFontSize(float _fScale);
	float	FontSizeToScale();

public:
	void	Initialize();
	void	Render(HDC _DC, eAssetTarget _target);
	void	Run();
	void	Save(std::ofstream& _OfSave);
	void	Load(std::ifstream& _ifLoad);
	void	Copy(CBaseInterface* _Copy);

public:
	void	CalculateTextBox(HDC _DC);
	void	CalculateFileSize();
	void	SetHitBox();

public:
	//	텍스트 박스의 위치는 텍스쳐에 상대적인 좌표이다.
	//	현재 폰트세팅이 프로퍼티 함수 내부에 설정되어 있지 않다.
	void	SetTextProperty(RECT _rtBox, BYTE _red, BYTE _green, BYTE _blue, UINT _SortFlag);
	void	SetDisableTextColor(BYTE _red, BYTE _green, BYTE _blue);
	void	SetTextPreview(CString _string);
	void	SetTextIndent(POINT _indent);
	void	SetFontSize(int _size);
	void	SetDefaultShow(bool _show);
	void	SetFontBold(bool _bold);
	void	SetVerticalIndent(int _iIndent);
	RECT	GetTextBox();
	int		GetSortFlag();
	int		GetFontSize();
	BYTE	GetRGB(eRGB _flag);
	BYTE	GetDisableRGB(eRGB _flag);
	POINT	GetTextIndent() { return m_ptTextIndent; }
	bool	GetbDefaultShow(){ return m_bDefaultShow; }
	bool	GetFontBold()	{ return m_bFontBold; }
	int		GetVerticalIndent(){ return m_iVerticalIndent; }
	void	SetMultiTextLine(char* _strText, RECT _rtTextBox);

public:
	CTextBox(void);
	virtual ~CTextBox(void);
};

