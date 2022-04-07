#pragma once

#include "AssetBase.h"
#include <MMSystem.h>
#pragma comment(lib, "winmm.lib")


class CImageAnimationBase : public CAssetBase
{
private:
	POINT m_ptTextureSize;		//������ �ؽ��� ��ü ������
	POINT m_ptImageStartPos;	//�̹��� ������ġ
	POINT m_ptImageInterval;	//�̹��� ���� ����
	POINT m_ptImageSize;		//�̹��� �� ��� ������	
	POINT m_ptImageNum;			//�̹��� ����, ���� ����
	POINT m_ptCurrentImageStartPos;	//���� �̹��� ������ġ
	int m_iImageTotalNum;		//��ü �̹��� ����
	float m_fDelayTime;			//�̹������� ������ Ÿ��
	int m_iLoop;				//�ִϸ��̼� ���� ����
	int m_iImageCount;			//���� ��� �ִ� �̹��� ���
	int m_iColPos;				//���� ���� �� �� 
	int m_iRowCount;			//���� ���� �̹��� ��ġ
	float m_fStartTime;			//Delay���� �ð�

public:
	void SetImageAnimationProperty(POINT textureSize, POINT imageStartPos, POINT imageInterval, POINT imageSize, POINT imageNum, 
								   int imageTotalNum, float delayTime, int loop);
	//void ChangeTexture();

public:
	POINT	GetImageSpace() { return m_ptImageInterval; }
	POINT	GetSingleImageSize() { return m_ptImageSize; }
	POINT	GetImageCount() { return m_ptImageNum; }
	POINT	GetTextureSize(){ return m_ptTextureSize; }
	POINT	GetImageStartPos() {return m_ptImageStartPos; }
	float	GetDelayTime()	{ return m_fDelayTime; }
	int		GetAniLoop()	{ return m_iLoop; }
	int		GetImageTotalCount() { return m_iImageTotalNum; }
	RECT	GetSingleImageSizeToRect();
	void ChangeTexture();

public:
	virtual void Initialize();
	virtual void Run();
	virtual void Render(HDC _DC, eAssetTarget _target);
	virtual void Save(std::ofstream& _OfSave);
	virtual void Load(std::ifstream& _IfLoad);
	virtual void Copy(CBaseInterface* _Copy);
	virtual void SetPosition(POINT _pt);


public:
	CImageAnimationBase(void);
	virtual ~CImageAnimationBase(void);
};