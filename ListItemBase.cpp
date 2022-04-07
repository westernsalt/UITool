#include "StdAfx.h"
#include "ListItemBase.h"
#include "ObjMgr.h"

CListItemBase::CListItemBase(void)
{
	m_iAssetType = ASSET_LISTITEM;
	m_tPosition.x = 0;
	m_tPosition.y = 0;
	m_fScale_X = 1.f;
	m_fScale_Y = 1.f;
	m_iFocusIndex = NOTHING_IN_HERE;
	m_iCurDlgBarIndex = CObjMgr::GetInstance()->GetCurDlgBarIndex();
	m_bZeroPosLock = false;
	m_bSceneEditor = false;
}

CListItemBase::~CListItemBase(void)
{
	int iVecCount = m_vecListItem.size();

	if(iVecCount != 0)
	{
		for(int i = 0; i < iVecCount;i++)
		{
			SAFE_DELETE(m_vecListItem[i]);
		}
		m_vecListItem.clear();
	}
}

void CListItemBase::Initialize()
{
	CBaseInterface::Initialize();
}

void CListItemBase::Run()
{
	CBaseInterface::Run();
	CalculateAssetSize();

	CheckState();
	SetHitBox();

	VEC_LISTITEM_ITER iter_begin = m_vecListItem.begin();
	VEC_LISTITEM_ITER iter_end = m_vecListItem.end();

	for(iter_begin; iter_begin != iter_end; iter_begin++)
	{
		((CBaseInterface*)*iter_begin)->Run();
	}
}

void CListItemBase::Render( HDC _DC, eAssetTarget _target )
{	
	int iVecCount = m_vecListItem.size();
	for(int i = 0; i < iVecCount; i++)
	{
		RECT* thitbox = m_vecListItem[i]->GetHitBox();
		m_vecListItem[i]->Render(_DC, _target);

		//선택한 에셋에 빨간색 테두리를 만들어준다.
		if(_target == ASSET_EDITOR_TARGET && m_iCurDlgBarIndex == DLGBAR_LISTITEM)
		{
			if(i == m_iFocusIndex && m_iState != STATE_PRESSED)
			{			
				HPEN CustomPen, OldPen;
				HBRUSH myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
				HBRUSH oldBrush = (HBRUSH)SelectObject(_DC, myBrush);
				CustomPen = CreatePen(PS_SOLID, 2, RGB(255,0,0));
				OldPen = (HPEN)SelectObject(_DC,CustomPen);
				Rectangle(_DC, thitbox->left , thitbox->top, thitbox->right, thitbox->bottom);
				SelectObject(_DC, OldPen);
				SelectObject(_DC, oldBrush);
				DeleteObject(myBrush);
				DeleteObject(CustomPen);
			}
			m_bSceneEditor = false;
		}
		else
		{
			m_bSceneEditor = true;
		}
	}
}

void CListItemBase::Copy( CBaseInterface* _Copy )
{
	CBaseInterface::Copy(_Copy);

	//	카피할 대상의 벡터를 받아와서 해당 벡터의 내용을 복사한다.
	VEC_LISTITEM_ITER iter_begin = ((CListItemBase*)_Copy)->m_vecListItem.begin();
	VEC_LISTITEM_ITER iter_end = ((CListItemBase*)_Copy)->m_vecListItem.end();

	for(iter_begin; iter_begin != iter_end; iter_begin++)
	{
		CBaseInterface* newBase = MakeAdaptableBase(((CBaseInterface*)*iter_begin)->GetAssetType());

		if(newBase != NULL)
		{
			newBase->Copy(((CBaseInterface*)*iter_begin));
			InsertListItem(newBase);
		}
	}

	CListItemBase::MakeOriginialFilePath();
}

void CListItemBase::Save( std::ofstream& _OfSave )
{
	POINT ptCurPos = m_tPosition;
	POINT ptZeroPos;
	ptZeroPos.x = ptZeroPos.y = 0;

	SetPosition(ptZeroPos);

	CBaseInterface::Save(_OfSave);

	_OfSave << "[LISTITEM START]" << std::endl;

	int iVecCount = m_vecListItem.size();
	for(int i = 0; i<iVecCount; i++)
	{
		_OfSave << "[LISTITEM]" << std::endl;
		m_vecListItem[i]->Save(_OfSave);
		_OfSave << "[LISTITEM_END]" <<std::endl;
	}

	_OfSave << "[LISTITEM COMPLETE]" << std::endl;
	
	SetPosition(ptCurPos);
}

void CListItemBase::Load( std::ifstream& _IfLoad )
{
	CBaseInterface::Load(_IfLoad);

	std::string strInput;

	//	StartRead
	std::getline(_IfLoad, strInput, '\n');

	while(!_IfLoad.eof())
	{
		CBaseInterface* newAsset = NULL;
		//	복수의 입력을 위한 InsertAsset과 SetFilePath의 배치
		
		std::getline(_IfLoad, strInput, '\n');

		if(!strcmp(strInput.c_str(), "[LISTITEM_END]"))
		{
			std::getline(_IfLoad, strInput, '\n');
			newAsset = NULL;

			if(!strcmp(strInput.c_str(), "[LISTITEM_COMPLETE]"))
			{
				return;
			}
		}

		if(!strcmp(strInput.c_str(), "[LISTITEM]"))
		{
			std::getline(_IfLoad, strInput, '\t');
			std::getline(_IfLoad, strInput, '\n');
			strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());

			eAssetType LoadAssetType = (eAssetType)atoi(strInput.c_str());

			newAsset = MakeAdaptableBase(LoadAssetType);
			newAsset->SetAssetType(LoadAssetType);
		}

		if(newAsset != NULL)
		{
			POINT MinusPos = m_tPosition;
			MinusPos.x *= -1;
			MinusPos.y *= -1;

			newAsset->Load(_IfLoad);
			newAsset->SubstractPosition(MinusPos);
			InsertListItem(newAsset);
			
			// 800*600 [6/7/2019 Gabrielle]
			newAsset->SetOriginPos(newAsset->GetPosition());
			newAsset = NULL;
		}
	}

	std::getline(_IfLoad, strInput, '\n');
}

void CListItemBase::SetState( eAssetState _state )
{
	m_iState = _state;
	CheckState();
}

void CListItemBase::CheckState()
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
	}

	if(m_iFocusIndex != NOTHING_IN_HERE && m_iState != STATE_PRESSED)
	{
		//	만약 포커스가 잡혀있고 들어온 상태의 값이 프레스 상태가 아니라면
		//	포커스가 계속해서 프레스 상태를 유지할 필요가 없으므로 해당 포커스의 프레스 상태를 해지한다.
		//	하이라이트값은 계속해서 체크해야하기 때문에 하단에서 checkstate for문을 돌게한다.
		m_vecListItem[m_iFocusIndex]->SetState(STATE_NORMAL);
	}

	int iVecCount = m_vecListItem.size();
	

	for(int i = 0; i < iVecCount; i++)
	{
		m_vecListItem[i]->CheckState();
	}

	//	이미 다른 에셋이 pressed 상태가 되어있는데
	//	그 에셋의 위를 지나갈때 pressed를 넘겨주게 되기 때문에 생기는 문제.
	for(int i = 0; i < iVecCount; i++)
	{
		int iState = m_vecListItem[i]->GetState();
		if(m_iState == STATE_PRESSED && iState == STATE_HIGHLIGHT)
		{
			if(m_iFocusIndex != NOTHING_IN_HERE)
			{
				if(m_vecListItem[m_iFocusIndex]->GetState() == STATE_PRESSED)
				{
					break;
				}
			}
			m_iFocusIndex = i;
			m_vecListItem[i]->SetState(STATE_PRESSED);
			break;
		}
	}
}

void CListItemBase::SetHitBox()
{
	m_tHitBox.left = m_tPosition.x;
	m_tHitBox.top = m_tPosition.y;
	m_tHitBox.right = m_tHitBox.left + m_tFileSize.right * m_fScale_X;
	m_tHitBox.bottom = m_tHitBox.top + m_tFileSize.bottom * m_fScale_Y;

	VEC_LISTITEM_ITER iter_begin = m_vecListItem.begin();
	VEC_LISTITEM_ITER iter_end = m_vecListItem.end();

	for(iter_begin; iter_begin != iter_end; iter_begin++)
	{
		((CBaseInterface*)*iter_begin)->SetHitBox();
	}
}

void CListItemBase::SetParenthWnd( HWND _hWnd )
{
	m_ParenthWnd = _hWnd;

	VEC_LISTITEM_ITER iter_begin = m_vecListItem.begin();
	VEC_LISTITEM_ITER iter_end = m_vecListItem.end();

	for(iter_begin; iter_begin != iter_end; iter_begin++)
	{
		((CBaseInterface*)*iter_begin)->SetParenthWnd(_hWnd);
	}
}

void CListItemBase::AddPosition( POINT _pos )
{
	CBaseInterface::AddPosition(_pos);
}

bool CListItemBase::SubstractPosition( POINT _pos )
{
	//	포커스 된 애만 움직인다.
	//	이 함수는 listbox에서 호출되면 안된다
	//	또한 이 클래스는 rai로 분류되어도 scene에서 불러와지면 안된다.
	//	차라리 특정 확장자로 저장되는 편이 낫지만....

	if(m_bSceneEditor)
	{
		m_tPosition.x -= _pos.x;
		m_tPosition.y -= _pos.y;

		int ivecsize = m_vecListItem.size();
		for(int i = 0; i<ivecsize; i++)
		{
			m_vecListItem[i]->SubstractPosition(_pos);
		}
	}
	else
	{
		if(m_iFocusIndex != NOTHING_IN_HERE)
		{
			m_vecListItem[m_iFocusIndex]->SubstractPosition(_pos);
		}
		else
		{
			//	만약 포커스가 잡혀있지 않은 경우,
			//	이 상황에서는 초기화나 카피를 한 상태이므로 포지션 조정을 전부 시켜주도록한다
			int ivecsize = m_vecListItem.size();
			for(int i = 0; i<ivecsize; i++)
			{
				m_vecListItem[i]->SubstractPosition(_pos);
			}
		}
	}

	return false;
}

void CListItemBase::MakeOriginialFilePath()
{
	VEC_LISTITEM_ITER iter_begin = m_vecListItem.begin();
	VEC_LISTITEM_ITER iter_end = m_vecListItem.end();

	for(iter_begin; iter_begin != iter_end; iter_begin++)
	{
		((CBaseInterface*)*iter_begin)->MakeOriginialFilePath();
	}
}

void CListItemBase::ConvertFilePath()
{
	VEC_LISTITEM_ITER iter_begin = m_vecListItem.begin();
	VEC_LISTITEM_ITER iter_end = m_vecListItem.end();

	for(iter_begin; iter_begin != iter_end; iter_begin++)
	{
		((CBaseInterface*)*iter_begin)->ConvertFilePath();
	}
}

void CListItemBase::CalculateAssetSize()
{
	POINT	ptCmpMinPos, ptCmpMaxPos;
	POINT	ptFileSize;

	VEC_LISTITEM_ITER	iter_begin	= m_vecListItem.begin();
	VEC_LISTITEM_ITER	iter_end	= m_vecListItem.end();

	//	각 에셋을 돌면서 최소 포지션과 최대포지션을 계산한다.
	//	해당 포지션을 계산한 이후에는 결과물을 이용하여 HitBox의 사이즈를 결정한다.
	for(iter_begin; iter_begin != iter_end; iter_begin++)
	{
		if(iter_begin == m_vecListItem.begin())
		{
			ptCmpMinPos = ((CBaseInterface*)*iter_begin)->GetPosition();
			m_ptMin = ((CBaseInterface*)*iter_begin)->GetPosition();

			//	left, top을 좌표로 사용하고 있었기 때문에 Max 지점을 체크하기 위해서는 각 파일의 크기를
			//	left, top 좌표에 더해준 이후에 계산해야한다.
			if(((CBaseInterface*)*iter_begin)->GetAssetType() == ASSET_CHECKBOX)
			{
				ptFileSize.x = (LONG)( (((CBaseInterface*)*iter_begin)->GetHitBox()->right - ((CBaseInterface*)*iter_begin)->GetHitBox()->left)
					* ((CBaseInterface*)*iter_begin)->GetScaleX());
				ptFileSize.y = (LONG)((((CBaseInterface*)*iter_begin)->GetHitBox()->bottom - ((CBaseInterface*)*iter_begin)->GetHitBox()->top)
					* ((CBaseInterface*)*iter_begin)->GetScaleY());
			}
			else
			{
				ptFileSize.x = (LONG)(((CBaseInterface*)*iter_begin)->GetFileSize().right
					* ((CBaseInterface*)*iter_begin)->GetScaleX());
				ptFileSize.y = (LONG)(((CBaseInterface*)*iter_begin)->GetFileSize().bottom
					* ((CBaseInterface*)*iter_begin)->GetScaleY());
			}

			ptCmpMaxPos.x = ((CBaseInterface*)*iter_begin)->GetPosition().x + ptFileSize.x;
			ptCmpMaxPos.y = ((CBaseInterface*)*iter_begin)->GetPosition().y + ptFileSize.y;
			m_ptMax.x = ((CBaseInterface*)*iter_begin)->GetPosition().x + ptFileSize.x;
			m_ptMax.y = ((CBaseInterface*)*iter_begin)->GetPosition().y + ptFileSize.y;
			continue;
		}

		ptFileSize.x = (LONG)(((CBaseInterface*)*iter_begin)->GetFileSize().right * ((CBaseInterface*)*iter_begin)->GetScaleX());
		ptFileSize.y = (LONG)(((CBaseInterface*)*iter_begin)->GetFileSize().bottom * ((CBaseInterface*)*iter_begin)->GetScaleY());
		ptCmpMaxPos.x = ((CBaseInterface*)*iter_begin)->GetPosition().x + ptFileSize.x;
		ptCmpMaxPos.y = ((CBaseInterface*)*iter_begin)->GetPosition().y + ptFileSize.y;
		ptCmpMinPos = ((CBaseInterface*)*iter_begin)->GetPosition();

		if(ptCmpMinPos.x < m_ptMin.x)
		{
			m_ptMin.x = ptCmpMinPos.x;
		}

		if(ptCmpMaxPos.x > m_ptMax.x)
		{
			m_ptMax.x = ptCmpMaxPos.x;
		}

		if(ptCmpMinPos.y < m_ptMin.y)
		{
			m_ptMin.y = ptCmpMinPos.y;
		}

		if(ptCmpMaxPos.y > m_ptMax.y)
		{
			m_ptMax.y = ptCmpMaxPos.y;
		}
	}

	m_tFileSize.left = 0;
	m_tFileSize.top = 0;
	m_tFileSize.right = m_ptMax.x - m_ptMin.x;
	m_tFileSize.bottom = m_ptMax.y - m_ptMin.y;

	m_tPosition = m_ptMin;
}

void CListItemBase::InsertListItem( CBaseInterface* _pitem )
{	
	m_iFocusIndex = m_vecListItem.size();
	m_vecListItem.push_back(_pitem);
}

bool CListItemBase::DeleteFocusItem()
{	
	if(m_iFocusIndex != NOTHING_IN_HERE)
	{
		VEC_LISTITEM_ITER iter_begin = m_vecListItem.begin();
		VEC_LISTITEM_ITER iter_end = m_vecListItem.end();

		for(iter_begin; iter_begin != iter_end; iter_begin++)
		{
			if(((CBaseInterface*)*iter_begin) == m_vecListItem[m_iFocusIndex])
			{
				SAFE_DELETE(m_vecListItem[m_iFocusIndex]);
				m_vecListItem.erase(iter_begin);
				m_iFocusIndex = -1;
				return false;
			}
		}
	}

	return true;
}

void CListItemBase::SetPosition( POINT _pt )
{
	POINT ptMoveDist;
	ptMoveDist.x = m_tPosition.x - _pt.x;
	ptMoveDist.y = m_tPosition.y - _pt.y;

	int iVecCount = m_vecListItem.size();

	if(iVecCount != 0)
	{
		for(int i = 0; i < iVecCount;i++)
		{
			m_vecListItem[i]->SubstractPosition(ptMoveDist);
		}
	}

	m_tPosition = _pt;
}

void CListItemBase::SetFocusAssetPos(POINT _ptPos)
{
	if(m_iFocusIndex != NOTHING_IN_HERE)
	{
		m_vecListItem[m_iFocusIndex]->SetPosition(_ptPos);
	}
}

POINT CListItemBase::GetPosition()
{
	if(m_iFocusIndex != NOTHING_IN_HERE)
	{
		return m_vecListItem[m_iFocusIndex]->GetPosition();
	}

	return m_tPosition;
}

void CListItemBase::ChangeListitemDepth(int nSelectIndex, int iChangePos)
{
	CBaseInterface* pTemp = MakeAdaptableBase(m_vecListItem[nSelectIndex]->GetAssetType());
	pTemp->Copy(m_vecListItem[nSelectIndex]);
	if(pTemp != NULL)
	{
		m_vecListItem.erase(find(m_vecListItem.begin(), m_vecListItem.end(), m_vecListItem[nSelectIndex]));	
		m_vecListItem.insert(m_vecListItem.begin()+iChangePos, pTemp);
	}
	m_iFocusIndex = iChangePos;
}

//포커스한 아이템 인데스 저장
void CListItemBase::SetFocusItemIndex(int nIndex)
{
	m_iFocusIndex = nIndex;
}

//포커스한 아이템 인데스 가져오기
int CListItemBase::GetFocusItemIndex()
{
	return m_iFocusIndex;
}

//리스트아이템 리스트 가져오기
std::vector<CBaseInterface*> CListItemBase::GetListItemList()
{
	return m_vecListItem;
}

POINT CListItemBase::CalculateAssetMinPos()
{
	POINT	ptMinPos, ptMaxPos;
	POINT	ptCmpMinPos, ptCmpMaxPos;
	POINT	ptFileSize;

	int iCount = m_vecListItem.size();
	ptMinPos.x = 0;
	ptMinPos.y = 0;

	ptMaxPos = ptCmpMinPos = ptCmpMaxPos = ptFileSize = ptMinPos;

	//	각 에셋을 돌면서 최소 포지션과 최대포지션을 계산한다.
	//	해당 포지션을 계산한 이후에는 결과물을 이용하여 HitBox의 사이즈를 결정한다.
	for(int i = 0; i < iCount; i++)
	{
		if(i == 0)
		{
			ptCmpMinPos = m_vecListItem[i]->GetPosition();
			ptMinPos = m_vecListItem[i]->GetPosition();

			//	left, top을 좌표로 사용하고 있었기 때문에 Max 지점을 체크하기 위해서는 각 파일의 크기를
			//	left, top 좌표에 더해준 이후에 계산해야한다.
			if(m_vecListItem[i]->GetAssetType() == ASSET_CHECKBOX)
			{
				ptFileSize.x = (LONG)( (m_vecListItem[i]->GetHitBox()->right - m_vecListItem[i]->GetHitBox()->left)
					* m_vecListItem[i]->GetScaleX());
				ptFileSize.y = (LONG)((m_vecListItem[i]->GetHitBox()->bottom - m_vecListItem[i]->GetHitBox()->top)
					* m_vecListItem[i]->GetScaleY());
			}
			else
			{
				ptFileSize.x = (LONG)(m_vecListItem[i]->GetFileSize().right
					* m_vecListItem[i]->GetScaleX());
				ptFileSize.y = (LONG)(m_vecListItem[i]->GetFileSize().bottom
					* m_vecListItem[i]->GetScaleY());
			}

			ptCmpMaxPos.x = m_vecListItem[i]->GetPosition().x + ptFileSize.x;
			ptCmpMaxPos.y = m_vecListItem[i]->GetPosition().y + ptFileSize.y;
			ptMaxPos.x = m_vecListItem[i]->GetPosition().x + ptFileSize.x;
			ptMaxPos.y = m_vecListItem[i]->GetPosition().y + ptFileSize.y;
			continue;
		}

		ptFileSize.x = (LONG)(m_vecListItem[i]->GetFileSize().right * m_vecListItem[i]->GetScaleX());
		ptFileSize.y = (LONG)(m_vecListItem[i]->GetFileSize().bottom * m_vecListItem[i]->GetScaleY());
		ptCmpMaxPos.x = m_vecListItem[i]->GetPosition().x + ptFileSize.x;
		ptCmpMaxPos.y = m_vecListItem[i]->GetPosition().y + ptFileSize.y;
		ptCmpMinPos = m_vecListItem[i]->GetPosition();

		if(ptCmpMinPos.x < ptMinPos.x)
		{
			ptMinPos.x = ptCmpMinPos.x;
		}

		if(ptCmpMaxPos.x > ptMaxPos.x)
		{
			ptMaxPos.x = ptCmpMaxPos.x;
		}

		if(ptCmpMinPos.y < ptMinPos.y)
		{
			ptMinPos.y = ptCmpMinPos.y;
		}

		if(ptCmpMaxPos.y > ptMaxPos.y)
		{
			ptMaxPos.y = ptCmpMaxPos.y;
		}
	}

	return ptMinPos;
}

void CListItemBase::SetFocusToNone()
{
	m_iFocusIndex = NOTHING_IN_HERE;
	m_ptMin = CalculateAssetMinPos();
	m_tPosition.x =0;
	m_tPosition.y =0;
}

void CListItemBase::SetZeroPosLock( bool _bLock )
{
	m_bZeroPosLock = _bLock;
}

void CListItemBase::SetScale( float _fscaleX, float _fscaleY )
{
	CBaseInterface::SetScale(_fscaleX, _fscaleY);

	int iCount = m_vecListItem.size();
	for(int i = 0 ; i<iCount; i++)
	{
		m_vecListItem[i]->SetScale(_fscaleX, _fscaleY);

		// 스케일링에 따른 포지션 설정 [6/7/2019 Gabrielle] 상대좌표는 O, 절대좌표는 X
		POINT ptAssetPosition = m_vecListItem[i]->GetOriginPos();

		//  스케일링 이후에 에셋들의 좌표가 상대적으로는 맞지만 프레임 좌표와는 맞지 않는 상황
		POINT ptScalePosition = ptAssetPosition;
		ptScalePosition.x *= m_fScale_X;
		ptScalePosition.y *= m_fScale_Y;

		ptScalePosition.x += m_tPosition.x;
		ptScalePosition.y += m_tPosition.y;

		m_vecListItem[i]->SetPosition(ptScalePosition);
	}
}

POINT CListItemBase::GetOriginPos()
{
	return m_ptOriginPos;
}
