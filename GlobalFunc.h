#ifndef GLOBAL_FUNC_H_
#define GLOBAL_FUNC_H_

#include "BaseInterface.h"
#include "AssetBase.h"
#include "TextBox.h"
#include "BtnBase.h"
#include "CheckBoxBase.h"
#include "GroupButton.h"
#include "SliderBase.h"
#include "ScrollBase.h"
#include "ListItemBase.h"
#include "ListBoxBase.h"
#include "ComboBoxBase.h"
#include "InputText.h"
#include "ProgressBarBase.h"
#include "ImageAnimationBase.h"
#include "ThirdGridBase.h"

static HBITMAP MakeDIBSection(CDC& dc, int width, int height)
{
	BITMAPINFO bmi;
	LPVOID pBits;
	HBITMAP hBitmap;
	memset(&bmi.bmiHeader, 0, sizeof(BITMAPINFOHEADER));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biWidth = width;
	bmi.bmiHeader.biHeight = height;
	bmi.bmiHeader.biPlanes = 1;
	hBitmap = ::CreateDIBSection(dc.GetSafeHdc(), &bmi, DIB_RGB_COLORS, (void**)&pBits, NULL, 0);
	return hBitmap;
}

static void DrawID( HDC _DC, int _index, POINT _pos, RECT _GuideRect, COLORREF _BGColor )
{	
	HFONT hFont = CreateFont(12,0,0,0,FW_HEAVY,0,0,0,HANGEUL_CHARSET,3,2,1,VARIABLE_PITCH | FF_ROMAN, L"Gulim");
	HFONT hOldFont = (HFONT)SelectObject(_DC, hFont);

	RECT tTextBox;
	RECT_INITIALIZE(tTextBox);
	tTextBox.left = _pos.x;
	tTextBox.top = _pos.y - DRAW_ID_HEIGHT;
	tTextBox.right = _pos.x + DRAW_ID_HEIGHT * 2;
	tTextBox.bottom = _pos.y;

	//	��� ��� ����
	int nOldMode = SetBkMode(_DC,TRANSPARENT);

	//	���� �� ����
	HPEN CustomPen, OldPen;
	CustomPen = CreatePen(PS_SOLID, 2, _BGColor);
	OldPen = (HPEN)SelectObject(_DC,CustomPen);

	//	�ε��� ��� �簢�� ����
	//	�÷��� �´� �귯�� ����
	HBRUSH myBrush = (HBRUSH)CreateSolidBrush(_BGColor);
	HBRUSH oldBrush = (HBRUSH)SelectObject(_DC, myBrush);
	Rectangle(_DC, tTextBox.left, tTextBox.top, tTextBox.right, tTextBox.bottom);
	DeleteObject(myBrush);
	SelectObject(_DC, oldBrush);


	//	���� �簢�� ����
	//	������ ĥ���� �ʵ�, �� �׵θ��� ĥ�Ѵ�.
	//	���� �����ؾ��ϹǷ� NullBrush ���
	HBRUSH NullBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH OldNullBrush = (HBRUSH)SelectObject(_DC, NullBrush);
	Rectangle(_DC, _GuideRect.left, _GuideRect.top, _GuideRect.right, _GuideRect.bottom );
	DeleteObject(NullBrush);
	SelectObject(_DC, OldNullBrush);

	//	������ �ڿ� ��ȯ
	DeleteObject(CustomPen);
	SelectObject(_DC, OldPen);

	//	�ش� ���̰��� ���� �ε��� ���
	char szIndex[50] = {0,};
	itoa(_index, szIndex, 10);
	CString strIndex(szIndex);
	DrawText(_DC, strIndex, -1, &tTextBox, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	SelectObject(_DC, hOldFont);
	SetBkMode(_DC, nOldMode);
	DeleteObject(hFont);
}

static CBaseInterface* MakeAdaptableBase(eAssetType _type)
{
	CBaseInterface* newBase = NULL;

	switch(_type)
	{
	case ASSET_PICTURE:
		newBase = new CAssetBase;
		break;
	case ASSET_BUTTON:
		newBase = new CBtnBase;
		break;
	case ASSET_CHECKBOX:
		newBase = new CCheckBoxBase;
		break;
	case ASSET_TABBUTTON:
	case ASSET_RADIOBUTTON:
		newBase = new CGroupButton;
		break;
	case ASSET_SLIDER:
		newBase = new CSliderBase;
		break;
	case ASSET_SCROLL:
		newBase = new CScrollBase;
		break;
	case ASSET_LISTITEM:
		newBase = new CListItemBase;
		break;
	case ASSET_LISTBOX: 
		newBase = new CListBoxBase;
		break;
	case ASSET_COMBOBOX:
		newBase = new CComboBoxBase;
		break;
	case ASSET_INPUTTEXT:
		newBase = new CInputText;
		break;
	case ASSET_TEXTBOX:
		newBase = new CTextBox;
		break;
	case ASSET_PROGRESSBAR:
		newBase = new CProgressBarBase;
		break;
	case ASSET_IMAGEANIMATION:
		newBase = new CImageAnimationBase;
		break;
	case ASSET_THIRDGRID:
		newBase = new CThirdGridBase;
		break;
	}

	return newBase;
}

static void	SetEditWndText( CEdit* _edit, int _value )
{
	CString strResult;
	char szITOA[20];
	std::string szResult;

	_itoa_s(_value, szITOA, 10);
	szResult = szITOA;
	strResult = szResult.c_str();
	_edit->SetWindowText(strResult);
}

static void EditTextSetting( int _iNum, CEdit* _Edit )
{
	char mulString[100];
	TCHAR TString[100];

	_itoa_s(_iNum, mulString, 10);
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, mulString, -1, TString, 100);
	_Edit->SetWindowText(TString);
}

static void FloatEditTextSetting( float _fNum, CEdit* _Edit )
{
	CString strResult;
	char s1[10];               // ��ȯ�� ���ڿ��� ������ �迭	
	sprintf(s1, "%g", _fNum);
	std::string szResult = s1;
	strResult = szResult.c_str();

	_Edit->SetWindowText(strResult);
}

static std::string GetEditTextStdString(CEdit* _Edit)
{
	CString strWndText;
	_Edit->GetWindowText(strWndText);

	CT2CA pszConvertedText (strWndText);
	std::string _strConvertText(pszConvertedText);
	return _strConvertText;
}

static int GetEditText( CEdit* _Edit )
{
	return atoi(GetEditTextStdString(_Edit).c_str());
}

static std::string ConvertDebugPathToReleasePath(std::string _debugPath)
{
	CString	AssetFilePath(_debugPath.c_str());
	TCHAR szCurDir[MAX_PATH] = {0,};
	GetCurrentDirectory(MAX_PATH, szCurDir);
	TCHAR szReleasePath[MAX_PATH] = RESOURCE_DEFAULT_PATH;
	PathAppend(szCurDir, szReleasePath);
	PathAppend(szCurDir, AssetFilePath);
	std::string strReleasePath = CT2A( szCurDir );
	return strReleasePath;
}

static void ConvertRelativePath(std::string& _Path)
{
	TCHAR	szPathBuffer[MAX_PATH] = {0,};
	TCHAR	szConvertPath[MAX_PATH];
	TCHAR	szCurDir[MAX_PATH];
	char	szResult[MAX_PATH];
	bool	bCheckResult;

	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, _Path.c_str(), -1, szConvertPath, MAX_PATH);
	GetCurrentDirectory(MAX_PATH, szCurDir);

	// �н� ���� : Release 
#if defined _DEBUG || defined RELEASE_DEV_PATH
	bCheckResult = PathRelativePathTo(szPathBuffer, szCurDir, FILE_ATTRIBUTE_DIRECTORY,
		szConvertPath, FILE_ATTRIBUTE_NORMAL);
#else
	PathAppend(szCurDir, L"\\..\\DataSetup\\UI");
	bCheckResult = PathRelativePathTo(szPathBuffer, szCurDir, FILE_ATTRIBUTE_DIRECTORY,
		szConvertPath, FILE_ATTRIBUTE_NORMAL);
#endif

	if(bCheckResult == TRUE)
	{
		wcstombs(szResult, (wchar_t*)szPathBuffer, MAX_PATH);
		_Path = szResult;
	}
	else
	{
		std::cout << "����� ���� ����" << std::endl;
	}
}

static void MakeAbsolutePath(std::string& _Path)
{
	TCHAR szCurDir[MAX_PATH];
	TCHAR szAppendPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szCurDir);
	// �н� ���� : Release 
#ifdef RELEASE_PATH
	PathAppend(szCurDir, L"\\..\\DataSetup\\UI");
#endif

	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, _Path.c_str(), -1, szAppendPath, MAX_PATH);

	PathRemoveBlanks(szAppendPath);
	PathAppend(szCurDir, szAppendPath);

	char psz[MAX_PATH];
	wcstombs(psz, (wchar_t*)szCurDir, MAX_PATH);
	_Path = psz;
}

static CBaseInterface* LoadBaseInterface( std::string FilePath, HWND _hWnd )
{
	std::ifstream	ifLoad(FilePath.c_str());
	std::string		strInput;	

	CBaseInterface* newAsset = NULL;

	while(!ifLoad.eof())
	{
		//	������ �Է��� ���� InsertAsset�� SetFilePath�� ��ġ
		//	�ε� ���� ������
		std::getline(ifLoad, strInput, '\n');
		if(!strcmp(strInput.c_str(), "[END]\t"))
		{
			break;
		}

		if(!strcmp(strInput.c_str(), "[START]"))
		{
			std::getline(ifLoad, strInput, '\t');
			std::getline(ifLoad, strInput, '\n');
			strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());

			eAssetType LoadAssetType = (eAssetType)atoi(strInput.c_str());
			newAsset = MakeAdaptableBase(LoadAssetType);
			newAsset->SetAssetType(LoadAssetType);
		}

		if(newAsset != NULL)
		{
			newAsset->Load(ifLoad);
			newAsset->SetParenthWnd(_hWnd);
		}
	}

	//	newAsset�� ���� ��쿡�� ����� ������ ���� ���� ���̴�
	ifLoad.close();
	return newAsset;
}

#endif