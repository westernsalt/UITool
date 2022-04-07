#include "StdAfx.h"
#include "GroupButton.h"
#include "RadioBtnBase.h"
#include "TabBtnBase.h"


CGroupButton::CGroupButton(void)
{
	m_tPosition.x = 0;
	m_tPosition.y = 0;
	m_iBtnCount = 0;
	m_iBtnCol = 1;
	m_iBtnRow = 1;
	m_fScale_X = 1.0;
	m_fScale_Y = 1.0;
	m_iFocusIndex = NOTHING_IN_HERE;
	m_iTextBoxInterval = 0;
	m_iInterval = 0;
	m_GroupBtn = NULL;
	m_iState = STATE_NORMAL;
}


CGroupButton::~CGroupButton(void)
{
	if(m_iBtnCount != 0)
	{
		delete [] m_GroupBtn;
		m_GroupBtn = NULL;
	}
}

void CGroupButton::Initialize()
{
	for(int i = 0; i < m_iBtnCount; i++)
	{
		if(m_iAssetType == ASSET_RADIOBUTTON)
		{
			((CRadioBtnBase*)m_GroupBtn)[i].Initialize();
		}
		else if(m_iAssetType == ASSET_TABBUTTON)
		{
			((CTabBtnBase*)m_GroupBtn)[i].Initialize();
		}
	}

	SetHitBox();
}

void CGroupButton::Run()
{
	CBaseInterface::Run();
	CheckState();
	SetHitBox();
	AdjustGroupBtnPos();
	for(int i = 0; i < m_iBtnCount; i++)
	{
		if(m_iAssetType == ASSET_RADIOBUTTON)
		{
			((CRadioBtnBase*)m_GroupBtn)[i].Run();
		}
		else if(m_iAssetType == ASSET_TABBUTTON)
		{
			((CTabBtnBase*)m_GroupBtn)[i].Run();
		}
	}
}

void CGroupButton::Render( HDC _DC, eAssetTarget _target )
{
	for(int i = 0; i < m_iBtnCount; i++)
	{
		if(m_iAssetType == ASSET_RADIOBUTTON)
		{
			((CRadioBtnBase*)m_GroupBtn)[i].Run();
		}
		else if(m_iAssetType == ASSET_TABBUTTON)
		{
			((CTabBtnBase*)m_GroupBtn)[i].Run();
		}
	}

	AdjustGroupBtnPos();

	//	히트박스 체크
	//Rectangle(_DC, m_tHitBox.left, m_tHitBox.top, m_tHitBox.right, m_tHitBox.bottom);

	if(m_iFocusIndex != NOTHING_IN_HERE && _target == ASSET_EDITOR_TARGET)
	{
		//	현재 선택된 라디오버튼 테두리 지정
		HPEN CustomPen, OldPen;
		HBRUSH myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		HBRUSH oldBrush = (HBRUSH)SelectObject(_DC, myBrush);
		CustomPen = CreatePen(PS_SOLID, 5, RGB(255,0,0));
		OldPen = (HPEN)SelectObject(_DC,CustomPen);

		int iNum = 0;
		if(m_iAssetType == ASSET_RADIOBUTTON)
		{
			Rectangle(_DC, ((CRadioBtnBase*)m_GroupBtn)[m_iFocusIndex].GetHitBox()->left , ((CRadioBtnBase*)m_GroupBtn)[m_iFocusIndex].GetHitBox()->top 
				, ((CRadioBtnBase*)m_GroupBtn)[m_iFocusIndex].GetHitBox()->right , ((CRadioBtnBase*)m_GroupBtn)[m_iFocusIndex].GetHitBox()->bottom );
		} else if(m_iAssetType == ASSET_TABBUTTON)
		{
			Rectangle(_DC, ((CTabBtnBase*)m_GroupBtn)[m_iFocusIndex].GetHitBox()->left , ((CTabBtnBase*)m_GroupBtn)[m_iFocusIndex].GetHitBox()->top 
				, ((CTabBtnBase*)m_GroupBtn)[m_iFocusIndex].GetHitBox()->right , ((CTabBtnBase*)m_GroupBtn)[m_iFocusIndex].GetHitBox()->bottom );
		}

		SelectObject(_DC, OldPen);
		SelectObject(_DC, oldBrush);
		DeleteObject(myBrush);
		DeleteObject(CustomPen);
		//	출력 완료
	}

	for(int i = 0; i < m_iBtnCount; i++)
	{
		if(m_iAssetType == ASSET_RADIOBUTTON)
		{
			((CRadioBtnBase*)m_GroupBtn)[i].Render(_DC, _target);
		}
		else if(m_iAssetType == ASSET_TABBUTTON)
		{
			((CTabBtnBase*)m_GroupBtn)[i].Render(_DC, _target);
		}
	}	
}

void CGroupButton::Copy( CBaseInterface* _Copy )
{
	CBaseInterface::Copy(_Copy);

	m_iBtnCount = ((CGroupButton*)_Copy)->m_iBtnCount;
	m_iBtnCol = ((CGroupButton*)_Copy)->m_iBtnCol;
	m_iBtnRow = ((CGroupButton*)_Copy)->m_iBtnRow;
	m_iMaxBtnCount = ((CGroupButton*)_Copy)->m_iMaxBtnCount;
	m_tPosition.x = ((CGroupButton*)_Copy)->m_tPosition.x;
	m_tPosition.y = ((CGroupButton*)_Copy)->m_tPosition.y;
	m_iInterval = ((CGroupButton*)_Copy)->m_iInterval;
	m_iTextBoxInterval = ((CGroupButton*)_Copy)->m_iTextBoxInterval;
	RECT_COPY(m_tFileSize, ((CGroupButton*)_Copy)->m_tFileSize);

	eAssetType BtnType = ((CGroupButton*)_Copy)->GetAssetType();

	if(BtnType == ASSET_RADIOBUTTON && m_GroupBtn == NULL)
	{
		m_GroupBtn = new CRadioBtnBase[m_iBtnCount];

		//	아니 이해가 안가네. 지역변수로서 소멸하는데 왜 오류가 뜨냐고 
		for(int i = 0; i < m_iBtnCount; i++)
		{
			//후일에 문제가 된다면 ((CRadioBtnBase*)m_GroupBtn) 이거를 잊지마라.
			//	그냥 쓰면 형변환이 되지 않기 때문에 제대로 기능하지 못한다.
			((CRadioBtnBase*)m_GroupBtn)[i].Copy(&(((CRadioBtnBase*)(((CGroupButton*)_Copy)->m_GroupBtn))[i]));
			((CRadioBtnBase*)m_GroupBtn)[i].MakeOriginialFilePath();
		}
		AdjustGroupBtnPos();
	} 
	else if(BtnType == ASSET_TABBUTTON && m_GroupBtn == NULL)
	{
		m_GroupBtn = new CTabBtnBase[m_iBtnCount];

		for(int i = 0; i < m_iBtnCount; i++)
		{
			//후일에 문제가 된다면 ((CRadioBtnBase*)m_GroupBtn) 이거를 잊지마라.
			//	그냥 쓰면 형변환이 되지 않기 때문에 제대로 기능하지 못한다.
			((CTabBtnBase*)m_GroupBtn)[i].Copy(&((CTabBtnBase*)(((CGroupButton*)_Copy)->m_GroupBtn))[i]);
			((CTabBtnBase*)m_GroupBtn)[i].MakeOriginialFilePath();
		}
		AdjustGroupBtnPos();
	}

	//	0으로 설정할 경우 리스트아이템에서는 제대로 돌아가지만 라디오다이알로그에서 문제가 발생한다.
	//	포커스 인덱스를 -1로 하면 제대로 돌아는 가지만 리스트아이템 다이얼로그에서 호출했을 경우
	//	체크상태를 읽지 못한다.
}

void CGroupButton::Save( std::ofstream& _OfSave )
{
	CBaseInterface::Save(_OfSave);
	_OfSave << "[GROUPBUTTON START]" << std::endl;
	_OfSave << "GroupInterval" << "\t" << m_iInterval << std::endl;
	_OfSave << "TextBoxInterval" << "\t" << m_iTextBoxInterval << std::endl;

	_OfSave << "BtnCount" << "\t\t" << m_iBtnCount << std::endl;
	_OfSave << "BtnCol" << "\t\t\t" << m_iBtnCol << std::endl;
	_OfSave << "BtnRow" << "\t\t\t" << m_iBtnRow << std::endl;
	_OfSave << "MaxBtnCount" << "\t\t" << m_iMaxBtnCount << std::endl;

	for(int i = 0; i < m_iBtnCount; i++)
	{
		if(m_iAssetType == ASSET_RADIOBUTTON)
		{
			((CRadioBtnBase*)m_GroupBtn)[i].Save(_OfSave);
		} else if(m_iAssetType == ASSET_TABBUTTON)
		{
			((CTabBtnBase*)m_GroupBtn)[i].Save(_OfSave);
		}
	}

	_OfSave << "[GROUDPBUTTON END]" << std::endl;
}

void CGroupButton::Load( std::ifstream& _IfLoad )
{
	CBaseInterface::Load(_IfLoad);
	std::string strInput;
	//	StartRead
	std::getline(_IfLoad, strInput, '\n');
	//	Interval Load
	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\n'), strInput.end());
	m_iInterval = atoi(strInput.c_str());

	//	TextboxInterval Load
	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\n'), strInput.end());
	m_iTextBoxInterval = atoi(strInput.c_str());

	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\n'), strInput.end());
	m_iBtnCount = atoi(strInput.c_str());

	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\n'), strInput.end());
	m_iBtnCol = atoi(strInput.c_str());

	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\n'), strInput.end());
	m_iBtnRow = atoi(strInput.c_str());

	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\n'), strInput.end());
	m_iMaxBtnCount = atoi(strInput.c_str());

	if(m_iAssetType == ASSET_RADIOBUTTON)
	{
		m_GroupBtn = new CRadioBtnBase[m_iMaxBtnCount];
	}
	else if(m_iAssetType == ASSET_TABBUTTON)
	{
		m_GroupBtn = new CTabBtnBase[m_iMaxBtnCount];
	}

	for(int i = 0; i < m_iBtnCount; i++)
	{
		if(m_iAssetType == ASSET_RADIOBUTTON)
		{
			((CRadioBtnBase*)m_GroupBtn)[i].Load(_IfLoad);
		}
		else if(m_iAssetType == ASSET_TABBUTTON)
		{
			((CTabBtnBase*)m_GroupBtn)[i].Load(_IfLoad);
		}
	}

	RECT_COPY(m_tFileSize, m_tHitBox);
	std::getline(_IfLoad, strInput, '\n');
}

void CGroupButton::CheckState()
{
	CBaseInterface::CheckState();

	for(int i = 0; i < m_iBtnCount; i++)
	{
		//	상태를 체크한다. groupbtn에 있는 요소들은 콜리전을 먹지 않으므로,
		//	내부에서 이 아이가 충돌되었을 경우 포커스를 맞춰주는 형식을 취하기로 한다.
		if(m_iAssetType == ASSET_RADIOBUTTON)
		{
			if(m_iState == STATE_PRESSED)
			{
				((CRadioBtnBase*)m_GroupBtn)[i].CalculateTextBox();
				((CRadioBtnBase*)m_GroupBtn)[i].CalculateHitBox();
				((CRadioBtnBase*)m_GroupBtn)[i].CheckState();
				if(((CRadioBtnBase*)m_GroupBtn)[i].GetState() == STATE_HIGHLIGHT)
				{
					m_iFocusIndex = i;
					::SendMessage(m_ParenthWnd, WM_CHANGE_GROUPBUTTON_INDEX, (WPARAM)this, m_iFocusIndex);
					return ;
				}
			}
		}
		if(m_iAssetType == ASSET_TABBUTTON)
		{
			if(m_iState == STATE_PRESSED)
			{
				((CTabBtnBase*)m_GroupBtn)[i].CheckState();
				if(((CTabBtnBase*)m_GroupBtn)[i].GetState() == STATE_HIGHLIGHT)
				{
					m_iFocusIndex = i;
					::SendMessage(m_ParenthWnd, WM_CHANGE_GROUPBUTTON_INDEX, (WPARAM)this, m_iFocusIndex);
					return ;
				}
			}
		}
	}
}

void CGroupButton::SetHitBox()
{
	if(m_iAssetType == ASSET_RADIOBUTTON)
	{
		m_tHitBox.left = ((CRadioBtnBase*)m_GroupBtn)[0].GetHitBox()->left;
		m_tHitBox.top = ((CRadioBtnBase*)m_GroupBtn)[0].GetHitBox()->top;
		m_tHitBox.right = ((CRadioBtnBase*)m_GroupBtn)[m_iBtnCount - 1].GetHitBox()->right;
		m_tHitBox.bottom = ((CRadioBtnBase*)m_GroupBtn)[m_iBtnCount - 1].GetHitBox()->bottom;

		m_tFileSize.left = 0;
		m_tFileSize.top = 0;
		m_tFileSize.right = m_tHitBox.right - m_tHitBox.left;
		m_tFileSize.bottom = m_tHitBox.bottom - m_tHitBox.top;
	}
	else if(m_iAssetType == ASSET_TABBUTTON)
	{
		m_tHitBox.left = ((CTabBtnBase*)m_GroupBtn)[0].GetHitBox()->left;
		m_tHitBox.top = ((CTabBtnBase*)m_GroupBtn)[0].GetHitBox()->top;
		m_tHitBox.right = ((CTabBtnBase*)m_GroupBtn)[m_iBtnCount - 1].GetHitBox()->right;
		m_tHitBox.bottom = ((CTabBtnBase*)m_GroupBtn)[m_iBtnCount - 1].GetHitBox()->bottom;

		m_tFileSize.left = 0;
		m_tFileSize.top = 0;
		m_tFileSize.right = m_tHitBox.right - m_tHitBox.left;
		m_tFileSize.bottom = m_tHitBox.bottom - m_tHitBox.top;
	}
}

void CGroupButton::Create( eAssetType _type, int _col, int _row )
{
	switch(_type)
	{
	case ASSET_RADIOBUTTON:
		{
			m_GroupBtn = new CRadioBtnBase[_col * _row];
			m_iMaxBtnCount = (_col*_row);
			m_iAssetType = ASSET_RADIOBUTTON;
			m_iBtnCol = _col;
			m_iBtnRow = _row;
			Initialize();
		}
		break;
	case ASSET_TABBUTTON:
		{
			m_GroupBtn = new CTabBtnBase[_col * _row];
			m_iMaxBtnCount = (_col*_row);
			m_iAssetType = ASSET_TABBUTTON;
			m_iBtnCol = _col;
			m_iBtnRow = _row;
			Initialize();
		}
		break;
	}

	
}

void CGroupButton::InsertGroupedBtn( CBtnBase* _btn, HWND _hWnd )
{
	if(m_GroupBtn != NULL)
	{
		if(m_iAssetType == ASSET_RADIOBUTTON)
		{
			m_iFocusIndex = m_iBtnCount;
			(((CRadioBtnBase*)m_GroupBtn)[m_iBtnCount]).Copy(_btn);

			POINT ptIntervalPos;
			ptIntervalPos.x = _btn->GetPosition().x - m_tPosition.x;
			ptIntervalPos.y = _btn->GetPosition().y - m_tPosition.y;
			(((CRadioBtnBase*)m_GroupBtn)[m_iBtnCount]).SetGroupIntervalPos(ptIntervalPos);
			(((CRadioBtnBase*)m_GroupBtn)[m_iBtnCount]).MakeOriginialFilePath();
			(((CRadioBtnBase*)m_GroupBtn)[m_iBtnCount]).CalculateTextBox();
			(((CRadioBtnBase*)m_GroupBtn)[m_iBtnCount]).CalculateHitBox();
			(((CRadioBtnBase*)m_GroupBtn)[m_iBtnCount]).SetParenthWnd(_hWnd);
			++m_iBtnCount;
		}
		else if(m_iAssetType == ASSET_TABBUTTON)
		{
			m_iFocusIndex = m_iBtnCount;
			(((CTabBtnBase*)m_GroupBtn)[m_iBtnCount]).Copy(_btn);

			POINT ptIntervalPos;
			ptIntervalPos.x = _btn->GetPosition().x - m_tPosition.x;
			ptIntervalPos.y = _btn->GetPosition().y - m_tPosition.y;
			(((CTabBtnBase*)m_GroupBtn)[m_iBtnCount]).SetGroupIntervalPos(ptIntervalPos);
			(((CTabBtnBase*)m_GroupBtn)[m_iBtnCount]).MakeOriginialFilePath();
			(((CTabBtnBase*)m_GroupBtn)[m_iBtnCount]).SetParenthWnd(_hWnd);
			++m_iBtnCount;
		}
	}
}

void CGroupButton::SetFilePath( std::string _path )
{
	for(int i = 0; i < m_iBtnCount; i++)
	{
		if(m_iAssetType == ASSET_RADIOBUTTON)
		{
			((CRadioBtnBase*)m_GroupBtn)[i].SetFilePath(_path);
		}
		else if(m_iAssetType == ASSET_TABBUTTON)
		{
			((CTabBtnBase*)m_GroupBtn)[i].SetFilePath(_path);
		}
	}
}

void CGroupButton::SetGroupBtnProperty( POINT _normal, POINT _press, POINT _disable, POINT _highlight, POINT _checkmark, POINT _size, POINT _checkmarksize,int _interval, eAssetType _type )
{
	if(m_iFocusIndex != NOTHING_IN_HERE)
	{
		m_iTextBoxInterval = _interval;
		switch(_type)
		{
		case ASSET_RADIOBUTTON:
			{
				for(int i = 0; i < m_iBtnCount; i++)
				{
					((CRadioBtnBase*)m_GroupBtn)[i].SetCheckBoxProperty(_normal, _press, _disable, _highlight, _checkmark, _size, _checkmarksize, _interval);
				}
			}
			break;
		case ASSET_TABBUTTON:
			{
				for(int i = 0; i < m_iBtnCount; i++)
				{
					((CTabBtnBase*)m_GroupBtn)[i].SetButtonProperty(_normal, _press, _disable, _highlight, _size);
				}
			}
			break;
		}		
	}
}

void CGroupButton::SetText( BYTE _red, BYTE _green, BYTE _blue, int _array, eAssetType _type )
{
	if(m_iFocusIndex != NOTHING_IN_HERE)
	{

		switch(_type)
		{
		case ASSET_RADIOBUTTON:
			{
				((CRadioBtnBase*)m_GroupBtn)[m_iFocusIndex].SetText(_red, _green, _blue,_array);
			}
			break;

		case ASSET_TABBUTTON:
			{
				((CTabBtnBase*)m_GroupBtn)[m_iFocusIndex].SetText(_red, _green, _blue,_array);
				for(int i = 0; i < m_iBtnCount; i++)
				{
					((CTabBtnBase*)m_GroupBtn)[i].SetText(_red, _green, _blue, _array);
				}
			}
			break;
		}
	}
}

void CGroupButton::AdjustGroupBtnPos()
{
	//	인터벌과 col, row에 따른 position 재배치
	POINT ptAdjustPos;

	ptAdjustPos.x = 0;
	ptAdjustPos.y = 0;

	int iBtnWidth;
	int iBtnHeight;

	for(int i = 0; i<m_iBtnCount; i++)
	{
		//	인터벌 재조정하는 부분 
		if(m_iAssetType == ASSET_RADIOBUTTON)
		{	
			if(i == 0)
			{
				((CRadioBtnBase*)m_GroupBtn)[0].SetPosition(m_tPosition);
				continue;
			}

			if(m_iBtnCol > m_iBtnRow)
			{
				iBtnWidth = ((CRadioBtnBase*)m_GroupBtn)[i - 1].GetHitBox()->right;
				ptAdjustPos.x = m_iInterval * m_fScale_X + iBtnWidth;
				ptAdjustPos.y = m_tPosition.y;
			}
			else if(m_iBtnRow > m_iBtnCol)
			{
				iBtnHeight = ((CRadioBtnBase*)m_GroupBtn)[i - 1].GetHitBox()->bottom;
				ptAdjustPos.y = m_iInterval * m_fScale_Y + iBtnHeight;
				ptAdjustPos.x = m_tPosition.x;
			}

			((CRadioBtnBase*)m_GroupBtn)[i].SetPosition(ptAdjustPos);
			((CRadioBtnBase*)m_GroupBtn)[i].CalculateTextBox();
			((CRadioBtnBase*)m_GroupBtn)[i].CalculateHitBox();
		}
		else if(m_iAssetType == ASSET_TABBUTTON)
		{	
			if(i == 0)
			{
				((CTabBtnBase*)m_GroupBtn)[0].SetPosition(m_tPosition);
				continue;
			}

			if(m_iBtnCol > m_iBtnRow)
			{
				iBtnWidth = ((CTabBtnBase*)m_GroupBtn)[i - 1].GetSize().x * i * m_fScale_X + m_tPosition.x;
				ptAdjustPos.x = m_iInterval + iBtnWidth;
				ptAdjustPos.y = m_tPosition.y;
			}
			else if(m_iBtnRow > m_iBtnCol)
			{
				iBtnHeight = ((CTabBtnBase*)m_GroupBtn)[i - 1].GetSize().y * i * m_fScale_Y+ m_tPosition.y;
				ptAdjustPos.y = m_iInterval + iBtnHeight;
				ptAdjustPos.x = m_tPosition.x;
			}

			((CTabBtnBase*)m_GroupBtn)[i].SetPosition(ptAdjustPos);
			((CTabBtnBase*)m_GroupBtn)[i].SetHitBox();
			((CTabBtnBase*)m_GroupBtn)[i].SetTextBoxPosition();

		}
	}

	SetHitBox();
}

void CGroupButton::SetParenthWndToGroup()
{
	if(m_ParenthWnd != NULL)
	{
		for(int i = 0; i < m_iBtnCount; i++)
		{
			if(m_iAssetType == ASSET_RADIOBUTTON)
			{
				//	에셋에디터의 핸들을 집어넣는다.
				((CRadioBtnBase*)m_GroupBtn)[i].SetParenthWnd(m_ParenthWnd);
			}
			else if(m_iAssetType == ASSET_TABBUTTON)
			{
				//	에셋에디터의 핸들을 집어넣는다.
				((CTabBtnBase*)m_GroupBtn)[i].SetParenthWnd(m_ParenthWnd);
			}
		}
	}
}

POINT CGroupButton::GetRecentBtnSize()
{
	POINT ptSize;

	ptSize.x = 0;
	ptSize.y = 0;

	if(m_iAssetType == ASSET_RADIOBUTTON)
	{
		((CRadioBtnBase*)m_GroupBtn)[m_iBtnCount - 1].CalculateTextBox();
		((CRadioBtnBase*)m_GroupBtn)[m_iBtnCount - 1].CalculateHitBox();
		//	에셋에디터의 핸들을 집어넣는다.
		ptSize.x = ((CRadioBtnBase*)m_GroupBtn)[m_iBtnCount - 1].GetHitBox()->right - ((CRadioBtnBase*)m_GroupBtn)[m_iBtnCount - 1].GetHitBox()->left;
		ptSize.y = ((CRadioBtnBase*)m_GroupBtn)[m_iBtnCount - 1].GetHitBox()->bottom - ((CRadioBtnBase*)m_GroupBtn)[m_iBtnCount - 1].GetHitBox()->top;
	}
	else if(m_iAssetType == ASSET_TABBUTTON)
	{
		//	에셋에디터의 핸들을 집어넣는다.
		ptSize.x = ((CTabBtnBase*)m_GroupBtn)[m_iBtnCount - 1].GetHitBox()->right - ((CTabBtnBase*)m_GroupBtn)[m_iBtnCount - 1].GetHitBox()->left;
		ptSize.y = ((CTabBtnBase*)m_GroupBtn)[m_iBtnCount - 1].GetHitBox()->bottom - ((CTabBtnBase*)m_GroupBtn)[m_iBtnCount - 1].GetHitBox()->top;
	}

	return ptSize;
}

void CGroupButton::SetState(eAssetState _state)
{
	m_iState = _state;
	CheckState();

	if(m_iFocusIndex != NOTHING_IN_HERE)
	{
		if(m_iAssetType == ASSET_RADIOBUTTON)
		{
			((CRadioBtnBase*)m_GroupBtn)[m_iFocusIndex].SetState(_state);

			for(int i = 0; i < m_iBtnCount; i++)
			{
				//	에셋에디터의 핸들을 집어넣는다.
				if(((CRadioBtnBase*)m_GroupBtn)[m_iFocusIndex].GetChecked())
				{
					if(i != m_iFocusIndex && ((CRadioBtnBase*)m_GroupBtn)[i].GetChecked())
					{
						((CRadioBtnBase*)m_GroupBtn)[i].SetChecked(false);
					}
				}
			}
		}
		else if(m_iAssetType == ASSET_TABBUTTON)
		{
			((CTabBtnBase*)m_GroupBtn)[m_iFocusIndex].SetState(_state);

			//포커스가 STATE_PRESSED=>STATE_NORMAL 상태로 변경
			for(int i = 0; i < m_iBtnCount; i++)
			{
				if(i != m_iFocusIndex && ((CTabBtnBase*)m_GroupBtn)[i].GetState() == STATE_PRESSED)
				{
					((CTabBtnBase*)m_GroupBtn)[i].SetState(STATE_NORMAL);
				}
			}
		}
	}	
}

std::string CGroupButton::GetFilePath()
{
	if(m_iBtnCount != 0)
	{
		return m_GroupBtn[0].GetFilePath();
	}
}

void CGroupButton::MakeOriginialFilePath()
{
	if(m_iBtnCount != 0)
	{
		for(int i = 0; i < m_iBtnCount; i++)
		{
			if(m_iAssetType == ASSET_RADIOBUTTON)
			{
				((CRadioBtnBase*)m_GroupBtn)[i].MakeOriginialFilePath();
			}
			else if(m_iAssetType == ASSET_TABBUTTON)
			{
				((CTabBtnBase*)m_GroupBtn)[i].MakeOriginialFilePath();
			}
		}
	}
}

CBtnBase* CGroupButton::GetGroupBase()
{
	return m_GroupBtn;
}

void CGroupButton::SetFocusIndex( int _index )
{
	m_iFocusIndex = _index;
	
	if(m_iAssetType == ASSET_RADIOBUTTON)
	{
		((CRadioBtnBase*)m_GroupBtn)[m_iFocusIndex].SetChecked(true);
		
		for(int i = 0; i < m_iBtnCount; i++)
		{
			if(((CRadioBtnBase*)m_GroupBtn)[m_iFocusIndex].GetChecked())
			{
				if(i != m_iFocusIndex && ((CRadioBtnBase*)m_GroupBtn)[i].GetChecked())
				{
					((CRadioBtnBase*)m_GroupBtn)[i].SetChecked(false);
				}
			}
		}
	}
}

void CGroupButton::ConvertFilePath()
{
	if(m_iBtnCount != 0)
	{
		for(int i = 0; i < m_iBtnCount; i++)
		{
			if(m_iAssetType == ASSET_RADIOBUTTON)
			{
				((CRadioBtnBase*)m_GroupBtn)[i].ConvertFilePath();
			}
			else if(m_iAssetType == ASSET_TABBUTTON)
			{
				((CTabBtnBase*)m_GroupBtn)[i].ConvertFilePath();
			}
		}
	}
}

void CGroupButton::SetParenthWnd( HWND _hWnd )
{
	m_ParenthWnd = _hWnd;
	SetParenthWndToGroup();
}

void CGroupButton::SetBtnInterval( int _iInterval )
{
	m_iInterval = _iInterval;
}

void CGroupButton::SetTextPreview( CString _preview, eAssetType _type )
{
	if(m_iFocusIndex != NOTHING_IN_HERE)
	{
		switch(_type)
		{
		case ASSET_RADIOBUTTON:
			{
				((CRadioBtnBase*)m_GroupBtn)[m_iFocusIndex].GetpTextBox()->SetTextPreview(_preview);
			}
			break;

		case ASSET_TABBUTTON:
			{
				((CTabBtnBase*)m_GroupBtn)[m_iFocusIndex].SetTextPreview(_preview);
			}
			break;
		}
	}
}

bool CGroupButton::SubstractPosition( POINT _pos )
{
	m_tPosition.x -= _pos.x;
	m_tPosition.y -= _pos.y;

	if(m_iBtnCount != 0)
	{
		for(int i = 0; i < m_iBtnCount; i++)
		{
			if(m_iAssetType == ASSET_RADIOBUTTON)
			{
				((CRadioBtnBase*)m_GroupBtn)[i].SubstractPosition(_pos);
			}
			else if(m_iAssetType == ASSET_RADIOBUTTON)
			{
				((CTabBtnBase*)m_GroupBtn)[i].SubstractPosition(_pos);
			}
		}
	}

	SetHitBox();
	Run();

	return false;
}

void CGroupButton::SetPosition( POINT _pt )
{
	m_tPosition = _pt;
}

void CGroupButton::SetDisableText( BYTE _red, BYTE _green, BYTE _blue )
{
	if(m_iBtnCount != 0)
	{
		for(int i = 0; i < m_iBtnCount; i++)
		{
			if(m_iAssetType == ASSET_RADIOBUTTON)
			{
				((CRadioBtnBase*)m_GroupBtn)[i].SetDisableText(_red, _green, _blue);
			}
			else if(m_iAssetType == ASSET_TABBUTTON)
			{
				((CTabBtnBase*)m_GroupBtn)[i].SetDisableText(_red, _green, _blue);
			}
		}
	}
}

void CGroupButton::SetbDisable( bool _disable )
{
	if(m_iBtnCount != 0)
	{
		for(int i = 0; i < m_iBtnCount; i++)
		{
			if(m_iAssetType == ASSET_RADIOBUTTON)
			{
				((CRadioBtnBase*)m_GroupBtn)[i].SetbDisable(_disable);
			}
			else if(m_iAssetType == ASSET_TABBUTTON)
			{
				((CTabBtnBase*)m_GroupBtn)[i].SetbDisable(_disable);
			}
		}
	}
}

void CGroupButton::SetTextSize( int _iSize )
{
	if(m_iBtnCount != 0)
	{
		for(int i = 0; i < m_iBtnCount; i++)
		{
			if(m_iAssetType == ASSET_RADIOBUTTON)
			{
				((CRadioBtnBase*)m_GroupBtn)[i].SetTextSize(_iSize);
			}
		}
	}
}

void CGroupButton::SetIconAsset( CAssetBase* _base, eIconPosition _iconPosition, std::string _assetPath )
{
	if(m_iBtnCount != 0)
	{
		if(m_iAssetType == ASSET_RADIOBUTTON)
		{
			if(m_iFocusIndex != NOTHING_IN_HERE)
			{
				((CRadioBtnBase*)m_GroupBtn)[m_iFocusIndex].SetIconAsset(_base, _iconPosition);
				((CRadioBtnBase*)m_GroupBtn)[m_iFocusIndex].SetIconAssetPath(_assetPath);
			}
		}
	}
}

void CGroupButton::SetIconPosition( eIconPosition _iconPos )
{
	if(m_iBtnCount != 0)
	{
		if(m_iAssetType == ASSET_RADIOBUTTON)
		{
			if(m_iFocusIndex != NOTHING_IN_HERE)
			{
				((CRadioBtnBase*)m_GroupBtn)[m_iFocusIndex].SetIconPosition(_iconPos);
			}
		}
	}
}

void CGroupButton::SetIconIndent( POINT _indent )
{
	if(m_iBtnCount != 0)
	{
		if(m_iAssetType == ASSET_RADIOBUTTON)
		{
			if(m_iFocusIndex != NOTHING_IN_HERE)
			{
				((CRadioBtnBase*)m_GroupBtn)[m_iFocusIndex].SetIconIndent(_indent);
			}
		}
	}
}

void CGroupButton::SetDefaultShow( bool _show )
{
	if(m_iBtnCount != 0)
	{
		if(m_iAssetType == ASSET_RADIOBUTTON)
		{
			if(m_iFocusIndex != NOTHING_IN_HERE)
			{
				((CRadioBtnBase*)m_GroupBtn)[m_iFocusIndex].SetDefaultShow(_show);
			}
		}
	}
}

void CGroupButton::SetFontBold(bool _bold)
{
	if(m_iBtnCount != 0)
	{
		if(m_iAssetType == ASSET_RADIOBUTTON)
		{
			if(m_iFocusIndex != NOTHING_IN_HERE)
			{
				((CRadioBtnBase*)m_GroupBtn)[m_iFocusIndex].SetFontBold(_bold);
			}
		}
	}
}

void CGroupButton::SetScale( float _fscaleX, float _fscaleY )
{
	CBaseInterface::SetScale(_fscaleX, _fscaleY);

	for(int i = 0; i < m_iBtnCount; i++)
	{
		if(m_iAssetType == ASSET_RADIOBUTTON)
		{
			((CRadioBtnBase*)m_GroupBtn)[i].SetScale(_fscaleX, _fscaleY);
		}
		else if(m_iAssetType == ASSET_TABBUTTON)
		{
			((CTabBtnBase*)m_GroupBtn)[i].SetScale(_fscaleX, _fscaleY);
		}
	}
}

void CGroupButton::SetTextBoxVerticalIndent( int _iIndent )
{
	for(int i = 0; i < m_iBtnCount; i++)
	{
		if(m_iAssetType == ASSET_TABBUTTON)
		{
			((CTabBtnBase*)m_GroupBtn)[i].SetTextBoxVerticalIndent(_iIndent);
		}
	}
}

int CGroupButton::GetBtnVerticalIndent()
{
	if(m_iAssetType == ASSET_TABBUTTON)
	{
		return ((CTabBtnBase*)m_GroupBtn)[0].GetTextBox().GetVerticalIndent();
	}
}
