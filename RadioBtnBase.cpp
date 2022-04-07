#include "StdAfx.h"
#include "RadioBtnBase.h"


CRadioBtnBase::CRadioBtnBase(void)
{
}


CRadioBtnBase::~CRadioBtnBase(void)
{

}

void CRadioBtnBase::SetGroupIntervalPos( POINT _pos )
{
	m_ptGroupIntervalPos.x = _pos.x;
	m_ptGroupIntervalPos.y = _pos.y;
}

POINT CRadioBtnBase::GetGroupIntervalPos()
{
	return m_ptGroupIntervalPos;
}

void CRadioBtnBase::Render( HDC _DC, eAssetTarget _target )
{
	CCheckBoxBase::Render(_DC, _target);
	CBaseInterface::CheckState();
}

void CRadioBtnBase::SetChecked(bool _checked)
{
	m_bChecked = _checked;
}

void CRadioBtnBase::Save( std::ofstream& _OfSave )
{
	CCheckBoxBase::Save(_OfSave);
	_OfSave << "[RADIOBUTTON START]" << std::endl;
	_OfSave << "IntervalPos" << "\t\t" << m_ptGroupIntervalPos.x << "," << m_ptGroupIntervalPos.y << std::endl;
	_OfSave << "[RADIOBUTTON END]" << std::endl;
}

void CRadioBtnBase::Load( std::ifstream& _IfLoad )
{
	CCheckBoxBase::Load(_IfLoad);

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

void CRadioBtnBase::Copy( CBaseInterface* _Copy )
{
	CCheckBoxBase::Copy(_Copy);
	m_ptGroupIntervalPos.x = ((CRadioBtnBase*)_Copy)->m_ptGroupIntervalPos.x;
	m_ptGroupIntervalPos.y = ((CRadioBtnBase*)_Copy)->m_ptGroupIntervalPos.y;
}

void CRadioBtnBase::CheckState()
{
	CBaseInterface::CheckState();
}

void CRadioBtnBase::SetText( BYTE _r, BYTE _g, BYTE _b, int _sort )
{
	m_TextBox.SetTextProperty(m_tHitBox, _r, _g, _b, _sort);
	CalculateTextBox();
}