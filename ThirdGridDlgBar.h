#pragma once
#include "afxwin.h"

// ThirdGridDlgBar ��ȭ �����Դϴ�.
class CThirdGridBase;
class CThirdGridDlgBar : public CDialogBar
{
	DECLARE_DYNAMIC(CThirdGridDlgBar)

public:
	CThirdGridDlgBar(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CThirdGridDlgBar();

private:
	CEdit *		m_TexturePathEdit;			//�ؽ��� ��� ������
	CEdit *		m_FirstLeft_Edit;			//ù��° �̹��� ���� ��ġ ������
	CEdit *		m_FirstTop_Edit;			//ù��° �̹��� ��� ��ġ ������
	CEdit *		m_SecondLeft_Edit;			//�ι�° �̹��� ���� ��ġ ������
	CEdit *		m_SecondTop_Edit;			//�ι�° �̹��� ��� ��ġ ������
	CEdit *		m_ThirdLeft_Edit;			//����° �̹��� ���� ��ġ ������
	CEdit *		m_ThirdTop_Edit;			//����° �̹��� ��� ��ġ ������
	CEdit *		m_Image_Width_Edit;			//�̹��� ���� ũ�� ������
	CEdit *		m_ImageHeight_Edit;			//�̹��� ���� ũ�� ������
	CEdit *		m_ScaleImageSize_Edit;		//��� �̹��� �þ �� ������

	CButton *	m_FirstRadioBtn;			//ù��° �̹��� ���� ���� ��ư
	CButton *	m_SeconRadioBtn;			//�ι�° �̹��� ���� ���� ��ư
	CButton *	m_ThirdRadioBtn;			//����° �̹��� ���� ���� ��ư
	CButton *	m_WidthScaleRadioBtn;		//���� ������ Ÿ�� ���� ��ư
	CButton *	m_HeightScaleRadioBtn;		//���� ������ Ÿ�� ���� ��ư

private:
	std::string			m_strTexturePath;	//�ؽ��� ��� ����	
	POINT				m_ptFirstImageTex;	//ù��° �̹��� ���� ��ġ ����
	POINT				m_ptSecondImageTex;	//�ι�° �̹��� ���� ��ġ ����
	POINT				m_ptThirdImageTex;	//����° �̹��� ���� ��ġ ����
	POINT				m_ptImageSize;		//�̹��� ũ�� ����(ù��°, �ι�°, ������ ũ�Ⱑ �����ؾ���.)
	int					m_iPixelSize;		//�ι�° �̹̰� �þ Pixel�� ����
	eThirdGridImageType	m_eImageType;		//�̹��� ���� Ÿ�� ����(ù��°, �ι�°, ����°)
	eArrayType			m_eScaleType;		//������ Ÿ�� ����
	bool				m_bChange;

private:
	CThirdGridBase *	m_ThirdGridBase;	//ThirdGridBase �� ����

private:
	HWND	m_ParenthWnd;					//�θ� ������
	HWND	m_PicturehWnd;					//�ؽ��� ������

public:
	void	Initialize();
	void	CreateAsset();													//ThirdGrid ���� ����
	void	SetThirdGridProperty();											//ThirdGrid �Ӽ� �� ����
	void	RefreshDlgBar(CThirdGridBase* _LoadBase, POINT& _NormalTex);	//���� �ҷ������� �����Ϳ� �Ӽ��� ����
	void	SetMemberThirdGridBase(CThirdGridBase* _base);					//ThirdGridBase ����

	void	SetParenthWnd(HWND _hWnd){	m_ParenthWnd = _hWnd;	}			//�θ������� ����
	void	SetPictureWnd(HWND _hWnd){	m_PicturehWnd = _hWnd;	}			//�ؽ��� ������ ����

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEditThirdGridTexturePath();
	afx_msg void OnEnChangeEditThirdGridFirstLeft();
	afx_msg void OnEnChangeEditThirdGridFirstTop();
	afx_msg void OnBnClickedRadioThirdGridFirstImage();
	afx_msg void OnEnChangeEditThirdGridSecondLeft();
	afx_msg void OnEnChangeEditThirdGridSecondTop();	
	afx_msg void OnBnClickedRadioThirdGridSecondImage();
	afx_msg void OnEnChangeEditThirdGridThirdLeft();
	afx_msg void OnEnChangeEditThirdGridThirdTop();
	afx_msg void OnBnClickedRadioThirdGridThirdImage();
	afx_msg void OnEnChangeEditThirdGridImageWidth();
	afx_msg void OnEnChangeEditThirdGridImageHeight();	
	afx_msg void OnBnClickedRadioThirdGridScaleTypeWidth();
	afx_msg void OnBnClickedRadioThirdGridScaleTypeHeight();
	afx_msg void OnEnChangeEditThirdGridScaleSize();
	afx_msg LRESULT OnSelchangeTexturelist(WPARAM wParam, LPARAM lParam);	//�ؽ��� �����쿡�� ����Ʈ Ŭ�������� �̺�Ʈ ó��
	afx_msg LRESULT OnChangeDragboxInfo(WPARAM wParam, LPARAM lParam);	
};
