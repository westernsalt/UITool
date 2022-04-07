#include "StdAfx.h"
#include "TabBtnBase.h"

CTabBtnBase::CTabBtnBase(void)
{
	m_iAssetType = ASSET_TABBUTTON;
	m_TextBox.SetDefaultShow(false);
}

CTabBtnBase::~CTabBtnBase(void)
{

}

void CTabBtnBase::Render(HDC _DC, eAssetTarget _target)
{
	CBtnBase::Render(_DC, _target);	
	//Rectangle(_DC, m_tHitBox.left, m_tHitBox.top, m_tHitBox.right, m_tHitBox.bottom);

	//Rectangle(_DC, m_TextBox.GetTextBox().left, m_TextBox.GetTextBox().top, m_TextBox.GetTextBox().right, m_TextBox.GetTextBox().bottom);
}

void CTabBtnBase::Save(std::ofstream& _OfSave)
{
	CBtnBase::Save(_OfSave);
	_OfSave << "[TABBUTTON START]" << std::endl;
	_OfSave << "IntervalPos" << "\t\t" << m_ptGroupIntervalPos.x << "," << m_ptGroupIntervalPos.y << std::endl;
	_OfSave << "[TABBUTTON END]" << std::endl;
}

void CTabBtnBase::Load(std::ifstream& _IfLoad)
{
	CBtnBase::Load(_IfLoad);

	std::string strInput;

	//	StartRead
	std::getline(_IfLoad, strInput, '\n');

	std::getline(_IfLoad, strInput, '\t');
	std::getline(_IfLoad, strInput, ',');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_ptGroupIntervalPos.x = atoi(strInput.c_str());

	std::getline(_IfLoad, strInput, '\n');
	strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
	strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
	m_ptGroupIntervalPos.y = atoi(strInput.c_str());
	std::getline(_IfLoad, strInput, '\n');
}

void CTabBtnBase::Copy(CBaseInterface* _Copy)
{
	CBtnBase::Copy(_Copy);
	m_ptGroupIntervalPos.x = ((CTabBtnBase*)_Copy)->m_ptGroupIntervalPos.x;
	m_ptGroupIntervalPos.y = ((CTabBtnBase*)_Copy)->m_ptGroupIntervalPos.y;
}

//버튼 안의 글자 간격 세팅
void CTabBtnBase::SetGroupIntervalPos(POINT _pos)
{
	m_ptGroupIntervalPos.x = _pos.x;
	m_ptGroupIntervalPos.y = _pos.y;
}

//버튼 안의 글자 간격 정보 가져오기
POINT CTabBtnBase::GetGroupIntervalPos()
{
	return m_ptGroupIntervalPos;
}

//텍스트 미리보기
void CTabBtnBase::SetTextPreview(CString strValue)
{
	m_TextBox.SetTextPreview(strValue);
}

//텍스트 정보 세팅
void CTabBtnBase::SetText(BYTE _r, BYTE _g, BYTE _b, int _sort)
{
	m_TextBox.SetTextProperty(m_tHitBox, _r, _g, _b, _sort);
}

void CTabBtnBase::Run()
{
	CBaseInterface::CheckState();
	CAssetBase::Run();
	SetHitBox();
	m_TextBox.SetPosition(m_tPosition);
}
