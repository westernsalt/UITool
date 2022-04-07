#pragma once
#include "afxwin.h"


// CImageAnimationDlgbar 대화 상자입니다.
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

	POINT m_ptTextureSize;		//선택한 텍스쳐 전체 사이즈
	POINT m_ptImageStartPos;	//이미지 시작 위치
	POINT m_ptImageInterval;	//이미지 간의 간격
	POINT m_ptImageSize;		//이미지 한 장당 사이즈	
	POINT m_ptImageNum;			//이미지 가로, 세로 갯수
	int m_iImageTotalNum;		//전체 이미지 갯수
	float m_fDelayTime;			//이미지간의 딜레이 타임	
	int m_iLoop;				//애니메이션 루프 여부

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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	CImageAnimationDlgbar(CWnd* pParent = NULL);   // 표준 생성자입니다.
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
	afx_msg LRESULT OnSelchangeTexturelist(WPARAM wParam, LPARAM lParam);	//텍스쳐 윈도우에서 리스트 클릭했을때 이벤트 처리
	afx_msg LRESULT OnChangeDragboxInfo(WPARAM wParam, LPARAM lParam);
	afx_msg void OnEnChangeAniImageStartPosX();
	afx_msg void OnEnChangeAniImageStartPosY();
	afx_msg void OnBnClickedAniLoop();
};
