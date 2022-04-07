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
	void	SetGroupIntervalPos(POINT _pos);							//��ư ���� ���� ���� ����	
	POINT	GetGroupIntervalPos();										//��ư ���� ���� ���� ���� ��������
	void	SetTextPreview(CString strValue);							//�ؽ�Ʈ �̸�����
	void	SetText(BYTE _r, BYTE _g, BYTE _b, int _sort);				//�ؽ�Ʈ ���� ����

public:
	CTabBtnBase(void);
	virtual ~CTabBtnBase(void);
};

