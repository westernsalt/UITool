#pragma once

#include "BaseInterface.h"

class CTextBox : public CBaseInterface
{
protected:
	//	Text ������ �ʿ��� ���
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

	//	�ؽ�Ʈ �̸����� ����
	CString	m_strPreview;

	bool	m_bFontBold;

	// ��Ƽ���� �ؽ�Ʈ �ణ �߰� [7/15/2019 Gabrielle]
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
	//	�ؽ�Ʈ �ڽ��� ��ġ�� �ؽ��Ŀ� ������� ��ǥ�̴�.
	//	���� ��Ʈ������ ������Ƽ �Լ� ���ο� �����Ǿ� ���� �ʴ�.
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

