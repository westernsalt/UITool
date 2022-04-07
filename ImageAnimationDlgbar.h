#pragma once
#include "afxwin.h"


// CImageAnimationDlgbar ��ȭ �����Դϴ�.
class CImageAnimationBase;

class CImageAnimationDlgbar : public CDialogBar
{
	DECLARE_DYNAMIC(CImageAnimationDlgbar)

private:
	CEdit* m_pTexturePathEdit;	
	CEdit* m_pTextureWidthSizeEdit;
	CEdit* m_pTextureHeightSizeEdit;
	CEdit* m_pImageStartPosX;
	CEdit* m_pImageStartPosY;
	CEdit* m_pImageWidthEdit;
	CEdit* m_pImageHeightEdit;
	CEdit* m_pImageWidthSpaceEdit;
	CEdit* m_pImageHeightSpaceEdit;
	CEdit* m_pImageWidthNumEdit;
	CEdit* m_pImageHeightNumEdit;
	CEdit* m_pImageTotalNumEdit;
	CEdit* m_pDelayTimeEdit;	
	CButton* m_pAniLoop;

private:
	std::string	m_strTexturePath;

	POINT m_ptTextureSize;		//������ �ؽ��� ��ü ������
	POINT m_ptImageStartPos;	//�̹��� ���� ��ġ
	POINT m_ptImageInterval;	//�̹��� ���� ����
	POINT m_ptImageSize;		//�̹��� �� ��� ������	
	POINT m_ptImageNum;			//�̹��� ����, ���� ����
	int m_iImageTotalNum;		//��ü �̹��� ����
	float m_fDelayTime;			//�̹������� ������ Ÿ��	
	int m_iLoop;				//�ִϸ��̼� ���� ����

	bool	m_bChange;

private:
	CImageAnimationBase * m_ImageAnimationBase;

private:
	HWND	m_ParenthWnd;
	HWND	m_PicturehWnd;

public:
	void	Initialize();
	void	CreateAsset();
	void	RefreshDlgBar(CImageAnimationBase* _LoadBase, POINT& _NormalTex);
	void	SetImageAnimationProperty();
	void	SetMemberImageAnimationBase(CImageAnimationBase* _base) { m_ImageAnimationBase = _base; }
	void	SettingControl(void* _data, eImageAnimationControl _controlID);

	void	SetParenthWnd(HWND _hWnd){	m_ParenthWnd = _hWnd;	}
	void	SetPictureWnd(HWND _hWnd){	m_PicturehWnd = _hWnd;	}	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	CImageAnimationDlgbar(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CImageAnimationDlgbar();

public:
	afx_msg void OnEnChangeEditShowTexturePath();
	afx_msg void OnEnChangeAnimationAniTextureSizeWidth();
	afx_msg void OnEnChangeAnimationAniTextureSizeHeight();
	afx_msg void OnEnChangeAnimationImageSizeWidth();
	afx_msg void OnEnChangeAnimationImageSizeHeight();
	afx_msg void OnEnChangeAnimationImageSpaceWidth();
	afx_msg void OnEnChangeAnimationImageSpaceHeight();
	afx_msg void OnEnChangeAnimationImageNumWidth();
	afx_msg void OnEnChangeAnimationImageNumHeight();
	afx_msg void OnEnChangeAnimationImageTotalNum();
	afx_msg void OnEnChangeAnimationDelaytime();	
	afx_msg LRESULT OnSelchangeTexturelist(WPARAM wParam, LPARAM lParam);	//�ؽ��� �����쿡�� ����Ʈ Ŭ�������� �̺�Ʈ ó��
	afx_msg LRESULT OnChangeDragboxInfo(WPARAM wParam, LPARAM lParam);
	afx_msg void OnEnChangeAniImageStartPosX();
	afx_msg void OnEnChangeAniImageStartPosY();
	afx_msg void OnBnClickedAniLoop();
};
