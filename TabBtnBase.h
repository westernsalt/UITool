#pragma once
class CTabBtnBase : public CBtnBase
{
private:
	POINT	m_ptGroupIntervalPos;

public:
	virtual void	Render(HDC _DC, eAssetTarget _target);
	virtual void	Save(std::ofstream& _OfSave);
	virtual void	Load(std::ifstream& _IfLoad);
	virtual void	Copy(CBaseInterface* _Copy);
	virtual void	Run();

public:
	void	SetGroupIntervalPos(POINT _pos);							//버튼 안의 글자 간격 세팅	
	POINT	GetGroupIntervalPos();										//버튼 안의 글자 간격 정보 가져오기
	void	SetTextPreview(CString strValue);							//텍스트 미리보기
	void	SetText(BYTE _r, BYTE _g, BYTE _b, int _sort);				//텍스트 정보 세팅

public:
	CTabBtnBase(void);
	virtual ~CTabBtnBase(void);
};

