#pragma once

class CThirdGridBase : public CAssetBase
{
private:	
	POINT		m_ptFirstImageStartPos;		//ù��° �̹��� ���� ��ġ
	POINT		m_ptSecondImageStartPos;	//�ι�° �̹��� ���� ��ġ
	POINT		m_ptThirdImageStartPos;		//����° �̹��� ���� ��ġ
	POINT		m_ptImageSize;				//�̹��� ũ��
	eArrayType	m_eScaleType;				//������ Ÿ��(����, ����)
	int			m_iPixel;					//�����ϵ� Pixel��

public:
	//3�׸��� ������Ƽ�� ����
	void SetThirdGridProperty(POINT imageSize, POINT firstImageStartPos, POINT secondImageStartPos, POINT thirdImageStartPos, eArrayType eScaleType, int pixel);	

public:
	POINT	GetFirstImagePos() { return m_ptFirstImageStartPos; }	//ù��° �̹��� ���� ��ġ ��������
	POINT	GetSecondImagePos() { return m_ptSecondImageStartPos; }	//�ι�° �̹��� ���� ��ġ ��������
	POINT	GetThirdImagePos() { return m_ptThirdImageStartPos; }	//����° �̹��� ���� ��ġ ��������
	POINT	GetImageSize() { return m_ptImageSize; }				//�̹��� ũ�� ��������
	int		GetScaleType() { return (int)m_eScaleType; }			//������ Ÿ�� ��������
	int		GetPixel() { return m_iPixel; }							//�þ  Pixel�� ��������
	RECT	GetImageSizeToRect();									//�̹��� ��Ʈ�� ��������

public:
	virtual void Initialize();
	virtual void Run();
	virtual void Render(HDC _DC, eAssetTarget _target);				//���� ����
	virtual void Save(std::ofstream& _OfSave);						//���� ���� ����
	virtual void Load(std::ifstream& _IfLoad);						//���� �ҷ�����
	virtual void Copy(CBaseInterface* _Copy);						//���� �� ����
	virtual void SetPosition(POINT _pt);							//���� ��ġ ����

public:
	CThirdGridBase(void);
	virtual ~CThirdGridBase(void);
};
