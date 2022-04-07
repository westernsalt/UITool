#include "StdAfx.h"
#include "ListBoxBase.h"


CListBoxBase::CListBoxBase(void)
{
	m_pScroll = new CScrollBase;
	m_pBackground = new CAssetBase;
	m_pListItem = new CListItemBase;
	m_strFilePath = "";
	m_strListItemPath = "";
	m_strScrollPath = "";
	m_tPosition.x = 0; 
	m_tPosition.y = 0;
	m_ptInterval.x = 0;
	m_ptInterval.y = 0;
	m_fScale_X = 1.0;
	m_fScale_Y = 1.0;
	m_iScrollArea = 0;
	m_iScrollAmount = SCROLL_AMOUNT;
	m_pBackground->SetAssetType(ASSET_PICTURE);
	m_pScroll->SetAssetType(ASSET_SCROLL);
	m_ptListItemStartPos.x = 0;
	m_ptListItemStartPos.y = 0;
}


CListBoxBase::~CListBoxBase(void)
{
	SAFE_DELETE(m_pScroll);
	SAFE_DELETE(m_pBackground);
	SAFE_DELETE(m_pListItem);

	ClearListItemVector();

	m_vecDisplayList.clear();
}

void CListBoxBase::SetFilePath( std::string _Path )
{
	m_strFilePath = _Path;
}

void CListBoxBase::SetScrollBase( CScrollBase* _base )
{
	if(m_pScroll == NULL)
	{
		m_pScroll = new CScrollBase;
	}

	m_pScroll->Copy(_base);
	SettingListItem();
	m_pScroll->SetScrollProperty();
}

void CListBoxBase::SetBackground( CAssetBase* _base )
{
	if(m_pBackground == NULL)
	{
		m_pBackground = new CAssetBase;
	}

	m_pBackground->Copy(_base);

	m_rtListBoxArea.left = m_tPosition.x;
	m_rtListBoxArea.top = m_tPosition.y;
	m_rtListBoxArea.right = m_rtListBoxArea.left + m_pBackground->GetFileSize().right;
	m_rtListBoxArea.bottom = m_rtListBoxArea.top + m_pBackground->GetFileSize().bottom;
}

void CListBoxBase::SetListItemBase( CListItemBase* _base )
{
	SAFE_DELETE(m_pListItem);
	m_pListItem = new CListItemBase;

	ClearListItemVector();

	m_pListItem->Copy(_base);

	SetListItemCount(m_iListItemCount);
	SetDisplayList();
}

void CListBoxBase::Initialize()
{
	m_pListItem->Initialize();
	if(m_pScroll != NULL)
		m_pScroll->Initialize();
	if(m_pBackground != NULL)
		m_pBackground->Initialize();

	int iCount = m_vecListItem.size();
	for(int i = 0; i < iCount; i++)
	{
		m_vecListItem[i]->Initialize();
	}
}

void CListBoxBase::Run()
{
	CBaseInterface::Run();
	SetHitBox();
	CheckState();
	CalculateScrollScale();

	if(m_pScroll != NULL)
		m_pScroll->Run();

	if(m_pBackground != NULL)
		m_pBackground->Run();

	//m_vecDisplayList를 런해주자.
	int iCount = m_vecDisplayList.size();
	for(int i = 0; i<iCount; i++)
	{
		m_vecDisplayList[i]->Run();
	}
}

void CListBoxBase::Render( HDC _DC, eAssetTarget _target )
{
	SettingListItem();

	HPEN CustomPen, OldPen;
	HBRUSH myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH oldBrush = (HBRUSH)SelectObject(_DC, myBrush);
	CustomPen = CreatePen(PS_DOT, 1, BLACK_PEN);
	OldPen = (HPEN)SelectObject(_DC,CustomPen);

	Rectangle(_DC, m_rtListBoxArea.left, m_rtListBoxArea.top
		, m_rtListBoxArea.left + (m_rtListBoxArea.right - m_rtListBoxArea.left) * m_fScale_X
		, m_rtListBoxArea.top + (m_rtListBoxArea.bottom - m_rtListBoxArea.top) * m_fScale_Y);

	SelectObject(_DC, OldPen);
	SelectObject(_DC, oldBrush);
	DeleteObject(myBrush);
	DeleteObject(CustomPen);

	if(m_pBackground != NULL)
	{
		if(m_pBackground->GetFilePath() != "")
		{
			m_pBackground->Render(_DC, _target);
		}
	}

	if(m_pListItem != NULL)
	{
		int iVecCount = m_vecDisplayList.size();
		for(int i = 0; i < iVecCount; i++)
		{
			m_vecDisplayList[i]->Render(_DC, _target);
		}
	}

	if(m_pScroll != NULL)
		m_pScroll->Render(_DC, _target);

	int iListBoxWidth = m_rtListBoxArea.right - m_rtListBoxArea.left;
	int iListBoxHeight = m_rtListBoxArea.bottom - m_rtListBoxArea.top;
	TCHAR szText[256] = {0,};

	if(iListBoxWidth / (m_pListItem->GetFileSize().right + m_ptInterval.x / 2) == 0)
	{
		int iNum = (m_pListItem->GetFileSize().right + m_ptInterval.x / 2) - iListBoxWidth;

		HFONT hFont =CreateFont(15, 0, 0, 0, 1, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1, VARIABLE_PITCH | FF_ROMAN, L"Gulim");
		HFONT hOldFont = (HFONT)SelectObject(_DC, hFont);
		SetTextColor(_DC,RGB( 255, 0, 0));

		RECT tDrawText;
		tDrawText.left = 0;
		tDrawText.top = 0;
		tDrawText.right = 450;
		tDrawText.bottom = 50;

		swprintf(szText, L"리스트의 가로크기가 리스트아이템의 크기보다 %d 작습니다", iNum);
		DrawText(_DC, (LPCWSTR)szText, -1, &tDrawText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);	

		SelectObject(_DC, hOldFont);
		DeleteObject(hFont);
	}

	if(iListBoxHeight / (m_pListItem->GetFileSize().bottom + m_ptInterval.y / 2) == 0)
	{
		int iNum = (m_pListItem->GetFileSize().bottom + m_ptInterval.y / 2) - iListBoxHeight;

		HFONT hFont = CreateFont(15, 0, 0, 0, 1, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1, VARIABLE_PITCH | FF_ROMAN, L"Gulim");
		HFONT hOldFont = (HFONT)SelectObject(_DC, hFont);
		SetTextColor(_DC,RGB( 255, 0, 0));

		RECT tDrawText;
		tDrawText.left = 0;
		tDrawText.top = 50;
		tDrawText.right = 450;
		tDrawText.bottom = 100;

		swprintf(szText, L"리스트의 세로크기가 리스트아이템의 크기보다 %d 작습니다", iNum);
		DrawText(_DC, (LPCWSTR)szText, -1, &tDrawText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);	

		SelectObject(_DC, hOldFont);
		DeleteObject(hFont);
	}

	//Rectangle(_DC, m_tHitBox.left, m_tHitBox.top, m_tHitBox.right, m_tHitBox.bottom);
}

void CListBoxBase::Copy( CBaseInterface* _Copy )
{
	CBaseInterface::Copy(_Copy);

	m_strFilePath = ((CListBoxBase*)_Copy)->m_strFilePath;
	m_ptInterval = ((CListBoxBase*)_Copy)->m_ptInterval;
	m_strScrollPath = ((CListBoxBase*)_Copy)->m_strScrollPath;
	m_strListItemPath = ((CListBoxBase*)_Copy)->m_strListItemPath;
	m_ptListItemStartPos = ((CListBoxBase*)_Copy)->m_ptListItemStartPos;
	RECT_COPY(m_rtListBoxArea, ((CListBoxBase*)_Copy)->m_rtListBoxArea);

	if(m_strScrollPath != "")
	{
		if(m_pScroll == NULL)
		{
			m_pScroll = new CScrollBase;
		}

		m_pScroll->Copy(((CListBoxBase*)_Copy)->m_pScroll);
		MakeAbsolutePath(m_strScrollPath);
	}

	m_pListItem->Copy(((CListBoxBase*)_Copy)->m_pListItem);

	if(m_strFilePath != "")
	{
		if(m_pBackground == NULL)
		{
			m_pBackground = new CAssetBase;
		}

		m_pBackground->Copy(((CListBoxBase*)_Copy)->m_pBackground);
	}

	//	리스트 아이템 카운트를 생성한 뒤에 내부를 채워야하므로 해당 변수는 가장 마지막에 세팅해주도록 한다
	SetListItemCount(((CListBoxBase*)_Copy)->m_iListItemCount);

	MakeAbsolutePath(m_strListItemPath);

	if(m_strFilePath != "")
	{
		MakeAbsolutePath(m_strFilePath);
	}

	Run();
}

void CListBoxBase::Save( std::ofstream& _OfSave )
{
	CBaseInterface::Save(_OfSave);
	_OfSave << "[LISTBOX START]" << std::endl;
	_OfSave << "ListBoxArea" << "\t\t" << m_rtListBoxArea.left << ","<< m_rtListBoxArea.top << ","
		<< m_rtListBoxArea.right << ","<< m_rtListBoxArea.bottom << std::endl;

	if(m_strScrollPath != "")
		ConvertRelativePath(m_strScrollPath);

	if(m_strFilePath != "")
		ConvertRelativePath(m_strFilePath);

	ConvertRelativePath(m_strListItemPath);

	_OfSave << "FilePath" << "\t\t" << m_strFilePath << std::endl;
	_OfSave << "ScrollPath" << "\t\t" << m_strScrollPath << std::endl;
	_OfSave << "ListItemPath" << "\t" << m_strListItemPath << std::endl;
	_OfSave << "ListItemCount" << "\t" << m_iListItemCount <<  std::endl;
	_OfSave << "ListInterval" << "\t" << m_ptInterval.x << "," << m_ptInterval.y << std::endl;
	_OfSave << "ListStartPos" << "\t" << m_ptListItemStartPos.x << "," << m_ptListItemStartPos.y << std::endl;


	POINT MinusPos;
	MinusPos.x = m_tPosition.x * -1;
	MinusPos.y = m_tPosition.y * -1;

	if(m_pScroll != NULL)
		m_pScroll->SubstractPosition(m_tPosition);

	if(m_pBackground != NULL)
	{
		m_pBackground->SubstractPosition(m_tPosition);
	}

	m_pBackground->Save(_OfSave);

	if(m_pScroll == NULL)
	{
		m_pScroll = new CScrollBase;
		m_pScroll->Save(_OfSave);
		SAFE_DELETE(m_pScroll);
	}
	else
	{
		m_pScroll->Save(_OfSave);
		MakeAbsolutePath(m_strScrollPath);
	}

	m_pListItem->SetFocusToNone();
	m_pListItem->Save(_OfSave);

	if(m_pScroll != NULL)
		m_pScroll->SubstractPosition(MinusPos);

	if(m_pBackground != NULL)
	{
		m_pBackground->SubstractPosition(MinusPos);
	}

	MakeAbsolutePath(m_strListItemPath);

	if(m_strFilePath != "")
		MakeAbsolutePath(m_strFilePath);

	_OfSave << "[LISTBOX END]" << std::endl;
}

void CListBoxBase::Load( std::ifstream& _IfLoad )
{
	std::string strInput;

	CBaseInterface::Load(_IfLoad);
	//	StartRead
	std::getline(_IfLoad, strInput, '\n');
	// ListBoxArea Load
	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, ',');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_rtListBoxArea.left = atoi(strInput.c_str());

	std::getline(_IfLoad, strInput, ',');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_rtListBoxArea.top = atoi(strInput.c_str());

	std::getline(_IfLoad, strInput, ',');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_rtListBoxArea.right = atoi(strInput.c_str());

	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_rtListBoxArea.bottom = atoi(strInput.c_str());

	//	FilePath Path Load
	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	m_strFilePath = strInput;

	//	Scroll Path Load
	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	m_strScrollPath = strInput;

	//	ListItem Path Load
	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	m_strListItemPath = strInput;

	//	ListItemCount Load
	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	m_iListItemCount = atoi(strInput.c_str());

	//	ListInterval Load
	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, ',');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_ptInterval.x = atoi(strInput.c_str());

	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_ptInterval.y = atoi(strInput.c_str());

	//	ListItemStartPos Load
	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, ',');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_ptListItemStartPos.x = atoi(strInput.c_str());

	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_ptListItemStartPos.y = atoi(strInput.c_str());


	std::getline(_IfLoad, strInput, '\n');

	m_pBackground->Load(_IfLoad);
	m_pScroll->Load(_IfLoad);


	std::getline(_IfLoad, strInput, '\n');
	m_pListItem->Load(_IfLoad);

	SetListItemCount(m_iListItemCount);

	std::getline(_IfLoad, strInput, '\n');

	// 패스 변경 : Release 
#if defined _DEBUG || defined RELEASE_DEV_PATH
	MakeAbsolutePath(m_strListItemPath);
#else
	m_strListItemPath = ConvertDebugPathToReleasePath(m_strListItemPath);
#endif

	if(m_strScrollPath == "")
	{
		SAFE_DELETE(m_pScroll);
	}
	else
	{
		// 패스 변경 : Release 
#if defined _DEBUG || defined RELEASE_DEV_PATH
		MakeAbsolutePath(m_strScrollPath);
#else
		m_strScrollPath = ConvertDebugPathToReleasePath(m_strScrollPath);
#endif
	}

	if(m_strFilePath == "")
	{
		SAFE_DELETE(m_pBackground);
	}
	else
	{
		// 패스 변경 : Release 
#if defined _DEBUG || defined RELEASE_DEV_PATH
		MakeAbsolutePath(m_strFilePath);
#else
		m_strFilePath = ConvertDebugPathToReleasePath(m_strFilePath);
#endif
	}
}

void CListBoxBase::SetState( eAssetState _state )
{
	m_iState = _state;
	CheckState();
}

void CListBoxBase::CheckState()
{
	int iCount = m_vecListItem.size();

	CBaseInterface::CheckState();

	if(m_pScroll != NULL)
		m_pScroll->CheckState();
	for(int i = 0; i < iCount; i++)
	{
		m_vecListItem[i]->CheckState();
	}

	if(m_iState == STATE_PRESSED)
	{
		if(m_pScroll != NULL)
		{
			if(m_pScroll->GetState() == STATE_HIGHLIGHT)
			{
				//	내가 눌렸고, 스크롤이 하이라이트 상태인 경우 스크롤에 프래스 상태를 넣어준다
				//	프레스상태가 들어간 뒤에는 하이라이트나 노멀이 먹지 않기 때문에
				//	외부에서 그 상태를 풀어줘야한다.
				m_pScroll->SetState(STATE_PRESSED);
			}
		}

		for(int i = 0; i < iCount; i++)
		{
			if(m_vecListItem[i]->GetState() == STATE_HIGHLIGHT)
			{
				m_vecListItem[i]->SetState(STATE_PRESSED);
				m_vecListItem[i]->CheckState();
			}
		}
	}
	else
	{
		if(m_pScroll != NULL)
		{
			if(m_pScroll->GetState() == STATE_PRESSED)
			{
				//	리스트박스가 눌린상태가 아닌경우
				m_pScroll->SetState(STATE_NORMAL);
			}
		}

		for(int i = 0; i < iCount; i++)
		{
			if(m_vecListItem[i]->GetState() == STATE_PRESSED)
			{
				m_vecListItem[i]->SetState(STATE_NORMAL);
			}
		}
	}
}

void CListBoxBase::SetHitBox()
{
	if(m_pScroll != NULL)
		m_pScroll->SetHitBox();

	int iCount = m_vecListItem.size();
	for(int i= 0; i<iCount; i++)
	{
		m_vecListItem[i]->SetHitBox();
	}

	//	가장 우하단은 스크롤의 right, bottom
	m_tHitBox.left = m_tPosition.x;
	m_tHitBox.top = m_tPosition.y;
	m_tHitBox.right = m_tHitBox.left + (m_rtListBoxArea.right - m_rtListBoxArea.left) * m_fScale_X;
	m_tHitBox.bottom = m_tHitBox.top + (m_rtListBoxArea.bottom - m_rtListBoxArea.top) * m_fScale_Y;

	if(m_pScroll != NULL && m_strScrollPath != "")
	{
		m_tHitBox.right += m_pScroll->GetFileSize().right;
	}

	m_tFileSize.left = 0;
	m_tFileSize.top = 0;
	m_tFileSize.right = (m_tHitBox.right - m_tHitBox.left) * m_fScale_X;
	m_tFileSize.bottom = (m_tHitBox.bottom - m_tHitBox.top) * m_fScale_Y;

	SettingListItem();
}

void CListBoxBase::SetParenthWnd( HWND _hWnd )
{
	m_ParenthWnd = _hWnd;

	if(m_pScroll != NULL)
		m_pScroll->SetParenthWnd(_hWnd);

	if(m_pBackground != NULL)
		m_pBackground->SetParenthWnd(_hWnd);
	m_pListItem->SetParenthWnd(_hWnd);

	int iCount = m_vecListItem.size();
	for(int i = 0; i < iCount; i++)
	{
		m_vecListItem[i]->SetParenthWnd(_hWnd);
	}
}

void CListBoxBase::AddPosition( POINT _pos )
{
	CBaseInterface::AddPosition(_pos);
}

bool CListBoxBase::SubstractPosition( POINT _pos )
{
	if(m_pScroll == NULL)
	{
		m_tPosition.x -= _pos.x;
		m_tPosition.y -= _pos.y;

		if(m_strFilePath != "")
			m_pBackground->SubstractPosition(_pos);

		SettingListItem();
		return false;
	}

	if(m_pScroll->GetState() != STATE_PRESSED && m_pScroll->GetButtonClicked() == false)
	{
		m_tPosition.x -= _pos.x;
		m_tPosition.y -= _pos.y;

		if(m_pBackground != NULL)
		{
			m_pBackground->SubstractPosition(_pos);
		}

		m_pScroll->SubstractPosition(_pos);
		SettingListItem();
	}
	else
	{
		m_pScroll->SubstractPosition(_pos);
		SettingListItem();
		return true;
	}

	return false;
}

void CListBoxBase::MakeOriginialFilePath()
{
	if(m_pScroll != NULL && m_strScrollPath != "")
		m_pScroll->MakeOriginialFilePath();

	if(m_pBackground != NULL && m_strFilePath != "")
		m_pBackground->MakeOriginialFilePath();

	if(m_pListItem != NULL)
	{
		int iVecCount = m_vecListItem.size();
		for(int i = 0; i<iVecCount; i++)
		{
			m_vecListItem[i]->MakeOriginialFilePath();
		}
	}
	m_pListItem->MakeOriginialFilePath();
}

void CListBoxBase::ConvertFilePath()
{
	if(m_pScroll != NULL && m_strScrollPath != "")
		m_pScroll->ConvertFilePath();

	if(m_pBackground != NULL && m_strFilePath != "")
		m_pBackground->ConvertFilePath();

	if(m_pListItem != NULL)
	{
		int iVecCount = m_vecListItem.size();
		for(int i = 0; i<iVecCount; i++)
		{
			m_vecListItem[i]->ConvertFilePath();
		}
	}

	m_pListItem->ConvertFilePath();
}

void CListBoxBase::SettingListItem()
{
	//	배경에셋을 집어넣을때 RECT멤버변수도 수정을 할수 있도록 한다.
	POINT tPos;
	static int iPrevGauge;

	//	배경이 존재하지 않고 영역만 설정되어있는 경우
	if(m_strFilePath == "")
	{
		tPos.x = m_tPosition.x + (m_rtListBoxArea.right - m_rtListBoxArea.left) * m_fScale_X;	//	가로 위치는 리스트박스 영역의 오른쪽과 같다
		tPos.y = m_tPosition.y;			//	높이는 현재 리스트박스의 top과 같다

		if(m_strScrollPath != "")
		{
			m_pScroll->SetPosition(tPos);
		}
	}

	//	리스트 박스 내부에 리스트아이템 포지션을 재설정해주기 위한 초기화
	tPos.x = m_tPosition.x + m_ptListItemStartPos.x;
	tPos.y = m_tPosition.y + m_ptListItemStartPos.y;

	int iWidth = m_rtListBoxArea.right - m_rtListBoxArea.left;
	int iHeight = m_rtListBoxArea.bottom - m_rtListBoxArea.top;

	m_rtListBoxArea.left = m_tPosition.x ;
	m_rtListBoxArea.top = m_tPosition.y;
	m_rtListBoxArea.right = m_tPosition.x + iWidth;
	m_rtListBoxArea.bottom = m_tPosition.y + iHeight;

	SetListBoxInterval(m_ptInterval.x, m_ptInterval.y);

	int iCount = 0;
	m_iScrollArea = 0;

	//	여기서는 리스트박스 내부시트의 상대적인 좌표를 정해주는 것
	//	스크롤링을 위해서는 실제 좌표는 Gauge와 연산이 이뤄진 좌표로 해야한다

	if(m_strListItemPath != "")
	{
		SetListItemCount(m_iListItemCount);

		while(true)
		{
			if(m_iListItemCount == 0)
			{
				break;
			}
			//	정해진 갯수마다 밑으로 밀어가며 포지션을 다 잡아준다
			//	이 리스트의 포지션을 잡아주고는 있지만 이것들이 디스플레이 되는 것은 아니다
			//	이 리스트에서 스크롤 영역과 일치하는 곳에 위치한 아이들만
			//	디스플레이 벡터로 옮겨서 해당 아이들만 디스플레이해줄 것이다.
			//	근데 이렇게 하면 절반만 보여주는게 안되는데.
			//	렌더를 할때 변수를 넘겨주도록 하자.
			//	커트라인처럼 넘겨줘서 해당 커트라인을 넘는 부분은 렌더하지 못하게 하자.

			for(int iCol = 0; iCol < m_ptListItemCount.x; iCol++)
			{
				if(iCount == m_iListItemCount)
				{
					break;
				}

				m_vecListItem[iCount]->SetPosition(tPos);
				tPos.x += m_pListItem->GetFileSize().right * m_pListItem->GetScaleX() + m_ptInterval.x;
				++iCount;
			}
			//	다음에 위치할 리스트아이템의 좌표를 계산해놓는다
			tPos.y += m_pListItem->GetFileSize().bottom * m_pListItem->GetScaleY() + m_ptInterval.y;
			tPos.x = m_tPosition.x + m_ptListItemStartPos.x;

			if(iCount >= (m_ptListItemCount.x * m_ptListItemCount.y))
			{
				//	리스트에 전부 나타나지 않는 영역을 계산한다.
				m_iScrollArea += m_pListItem->GetFileSize().bottom * m_pListItem->GetScaleY() + m_ptInterval.y;
			}

			if(iCount == m_iListItemCount || m_ptListItemCount.x == 0 || m_ptListItemCount.y == 0)
			{
				break;
			}
		}
	}

	if(m_iScrollArea != 0 && m_pScroll != NULL && m_strScrollPath != "")
	{
		int iBtnHeight;
		//	원래는 저 상수 들어간 부분에 m_iScrollAmount라는 스크롤할 민감도? 변수가 들어가야한다
		iBtnHeight = m_pScroll->GetFileSize().bottom - (m_iScrollArea / m_iScrollAmount);
		//이 부분에서 버튼이 몇의 스케일을 먹어야하는지를 계산하면 된다.

		int iScrollBtnSize = m_pScroll->GetSliderBtnBase()->GetSize().y;

		//if(iScrollBtnSize != 0)
			//float fScaleY = iBtnHeight / iScrollBtnSize;
	}

	// 게이지가 0일 때의 리스트박스에서의 위치를 전부 계산했다.
	//	그럼 이제 스크롤링된 거에 따라서 어떻게 위치가 변하는지를 계산해준 후
	//	해당 계산된 좌표의 리스트아이템 중 Area안에 들어온 애들만 display하고 run해주면 된다.

	POINT ptSubsNum;
	ptSubsNum.x = 0;

	if(m_pScroll != NULL && m_strScrollPath != "")
		ptSubsNum.y = m_pScroll->GetGauge() * m_iScrollAmount;
	else
	{
		ptSubsNum.y = 0;
	}

	if(m_pScroll != NULL && m_strScrollPath != "")
	{
		if(m_pScroll->GetGauge() != 0)
		{
			//	여기에서 벌벌벌벌 거리고 떠는 이유는?
			//	스크롤링된 것에 따라서 위치가 변해야하므로 그 과정을 계산했다.
			int	iVecCount = m_vecListItem.size();
			for(int i = 0; i < iVecCount; i++)
			{
				POINT ptPos = m_vecListItem[i]->GetPosition();
				ptPos.y -= ptSubsNum.y;
				m_vecListItem[i]->SetPosition(ptPos);
			}
		}
	}

	SetDisplayList();
}

void CListBoxBase::SetListItemCount( int _iCount )
{
	if(m_pListItem != NULL)
	{
		//	리스트아이템의 개수가 정해졌으므로 생성해서 벡터에 넣어준다.
		if(m_vecListItem.size() != _iCount)
		{
			ClearListItemVector();

			for(int i = 0; i < _iCount; i++)
			{
				CListItemBase* newItemBase = new CListItemBase;
				newItemBase->Copy(m_pListItem);
				newItemBase->SetParenthWnd(m_ParenthWnd);
				m_vecListItem.push_back(newItemBase);
			}
		}
	}

	m_iListItemCount = _iCount;

	SetDisplayList();
}

void CListBoxBase::ClearListItemVector()
{
	int iCount = m_vecListItem.size();

	for(int i = 0; i < iCount; i++)
	{
		SAFE_DELETE(m_vecListItem[i]);
	}

	m_vecListItem.clear();
}

void CListBoxBase::SetListBoxArea( int _xpos, int _ypos, int _width, int _height )
{
	m_rtListBoxArea.left = _xpos;
	m_rtListBoxArea.top = _ypos;
	m_rtListBoxArea.right = m_rtListBoxArea.left + _width;
	m_rtListBoxArea.bottom = m_rtListBoxArea.top + _height;
}

void CListBoxBase::SetListBoxInterval( int _col, int _row )
{
	//	리스트박스 내부의 간격 설정
	m_ptInterval.x = _col;
	m_ptInterval.y = _row;

	int iListBoxWidth = m_rtListBoxArea.right - m_rtListBoxArea.left;
	int iListBoxHeight = m_rtListBoxArea.bottom - m_rtListBoxArea.top;

	if(m_strListItemPath != "")
	{
		//  [4/8/2019 Gabrielle]
		//	라키온 수식과 동기화
		//	리스트박스에 최대 몇개의 아이템이 들어갈 수 있는지를 계산
		m_ptListItemCount.x = iListBoxWidth / (m_pListItem->GetFileSize().right * m_pListItem->GetScaleX() + m_ptInterval.x / 2);
		m_ptListItemCount.y = iListBoxHeight / (m_pListItem->GetFileSize().bottom * m_pListItem->GetScaleY() + m_ptInterval.y / 2);

		if(m_ptListItemCount.x == 0)
		{
			m_ptListItemCount.x = 1;
		}

		if(m_ptListItemCount.y == 0)
		{
			m_ptListItemCount.y = 1;
		}
	}
	else
	{
		std::cout << "리스트아이템이 설정되어 있지 않아 리스트 내부 슬롯 계산이 이뤄지지 않았습니다" << std::endl;
	}
}

std::string CListBoxBase::GetFilePath()
{
	if(m_strFilePath == "")
	{
		std::cout << "[리스트박스] 파일 경로가 존재하지 않습니다." << std::endl;
	}

	return m_strFilePath;
}

std::string CListBoxBase::GetScrollPath()
{
	if(m_strScrollPath == "")
	{
		std::cout << "[리스트박스 스크롤] 파일 경로가 존재하지 않습니다." << std::endl;
	}
	return m_strScrollPath;
}

std::string CListBoxBase::GetListItemPath()
{
	if(m_strListItemPath == "")
	{
		std::cout << "[리스트박스 리스트아이템] 파일 경로가 존재하지 않습니다." << std::endl;
	}
	return m_strListItemPath;
}

POINT CListBoxBase::GetBackgroundPos()
{
	return m_pBackground->GetStartPos();
}

RECT CListBoxBase::GetBackgroundSize()
{
	return m_pBackground->GetFileSize();
}

POINT CListBoxBase::GetListBoxInterval()
{
	return m_ptInterval;
}

int CListBoxBase::GetListBoxItemCount()
{
	return m_iListItemCount;
}

void CListBoxBase::SetListBoxProperty( int _itemcount, std::string _bgPath, POINT _startpos, RECT _listboxarea, POINT _interval, POINT _ListitemPos )
{
	m_iListItemCount = _itemcount;
	RECT_COPY(m_rtListBoxArea, _listboxarea);
	m_ptInterval = _interval;
	m_strFilePath = _bgPath;
	m_pBackground->SetAsset(_bgPath, ASSET_PICTURE, _listboxarea, _startpos, 1.f, 1.f);
	m_ptListItemStartPos = _ListitemPos;
}

void CListBoxBase::SetScrollPath( std::string _Path )
{
	m_strScrollPath = _Path;
}

void CListBoxBase::SetListItemPath( std::string _Path )
{
	m_strListItemPath = _Path;
}

void CListBoxBase::ClearDisplayList()
{
	int iCount = m_vecDisplayList.size();
	for(int i = 0; i < iCount; i++)
	{
		SAFE_DELETE(m_vecDisplayList[i]);
	}

	m_vecDisplayList.clear();
}

void CListBoxBase::SetPosition( POINT _pt )
{
	m_tPosition.x = _pt.x;
	m_tPosition.y = _pt.y;

	if(m_pBackground != NULL)
		m_pBackground->SetPosition(_pt);

	SettingListItem();
}

void CListBoxBase::CalculateScrollScale()
{
	if(m_pScroll != NULL && m_strScrollPath != "")
	{
		int iHeight = (m_rtListBoxArea.bottom - m_rtListBoxArea.top) * m_fScale_Y;
		int iScrollHeight = m_pScroll->GetBackgroundBar()->GetFileSize().bottom * m_fScale_Y;

		if(iScrollHeight < iHeight || iScrollHeight > iHeight)
		{
			//	얼마의 길이가 부족한지를 계산하고 해당 부족한 길이를 나누어 배수를 구해 스케일링
			float fNum = (float)iHeight / iScrollHeight;
			m_pScroll->GetBackgroundBar()->SetScale(1.f, fNum * m_fScale_Y);
		}
	}
}

void CListBoxBase::SetDisplayList()
{
	int iVecCount = m_vecListItem.size();
	m_vecDisplayList.clear();

	for(int i = 0;i < iVecCount; i++)
	{
		//	움직일때의 미세한 차이로 디스플레이 리스트에 들어가지 않아
		//	깜박임 현상이 발생한다.
		if((m_vecListItem[i]->GetPosition().y >= m_tHitBox.top - 5)
			&& (m_vecListItem[i]->GetPosition().y + m_vecListItem[i]->GetFileSize().bottom * m_vecListItem[i]->GetScaleY() < m_tHitBox.bottom + 5))
		{
			m_vecDisplayList.push_back(m_vecListItem[i]);
		}
	}
}

POINT CListBoxBase::GetListItemStartPos()
{
	return m_ptListItemStartPos;
}

RECT CListBoxBase::GetListBoxAreaSize()
{
	return m_rtListBoxArea;
}

void CListBoxBase::SetScale( float _fscaleX, float _fscaleY )
{
	CBaseInterface::SetScale(_fscaleX, _fscaleY);

	if(m_pBackground != NULL)
		m_pBackground->SetScale(_fscaleX, _fscaleY);

	if(m_pScroll != NULL && m_strScrollPath != "")
	{
		m_pScroll->SetScale(_fscaleX, _fscaleY);
	}

	m_pListItem->SetScale(_fscaleX, _fscaleY);

	int iCount = m_vecListItem.size();
	for(int i = 0 ; i<iCount; i++)
	{
		m_vecListItem[i]->SetScale(_fscaleX, _fscaleY);
	}
}
