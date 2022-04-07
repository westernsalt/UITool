#pragma once
#include "afxwin.h"

// ThirdGridDlgBar 대화 상자입니다.
class CThirdGridBase;
class CThirdGridDlgBar : public CDialogBar
{
	DECLARE_DYNAMIC(CThirdGridDlgBar)

public:
	CThirdGridDlgBar(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CThirdGridDlgBar();

private:
	CEdit *		m_TexturePathEdit;			//텍스쳐 경로 에디터
	CEdit *		m_FirstLeft_Edit;			//첫번째 이미지 왼쪽 위치 에디터
	CEdit *		m_FirstTop_Edit;			//첫번째 이미지 상단 위치 에디터
	CEdit *		m_SecondLeft_Edit;			//두번째 이미지 왼쪽 위치 에디터
	CEdit *		m_SecondTop_Edit;			//두번째 이미지 상단 위치 에디터
	CEdit *		m_ThirdLeft_Edit;			//세번째 이미지 왼쪽 위치 에디터
	CEdit *		m_ThirdTop_Edit;			//세번째 이미지 상단 위치 에디터
	CEdit *		m_Image_Width_Edit;			//이미지 가로 크기 에디터
	CEdit *		m_ImageHeight_Edit;			//이미지 세로 크기 에디터
	CEdit *		m_ScaleImageSize_Edit;		//가운데 이미지 늘어날 값 에디터

	CButton *	m_FirstRadioBtn;			//첫번째 이미지 지정 라디오 버튼
	CButton *	m_SeconRadioBtn;			//두번째 이미지 지정 라디오 버튼
	CButton *	m_ThirdRadioBtn;			//세번째 이미지 지정 라디오 버튼
	CButton *	m_WidthScaleRadioBtn;		//가로 스케일 타입 라디오 버튼
	CButton *	m_HeightScaleRadioBtn;		//세로 스케일 타입 라디오 버튼

private:
	std::string			m_strTexturePath;	//텍스쳐 경로 저장	
	POINT				m_ptFirstImageTex;	//첫번째 이미지 시작 위치 저장
	POINT				m_ptSecondImageTex;	//두번째 이미지 시작 위치 저장
	POINT				m_ptThirdImageTex;	//세번째 이미지 시작 위치 저장
	POINT				m_ptImageSize;		//이미지 크기 저장(첫번째, 두번째, 세번제 크기가 동일해야함.)
	int					m_iPixelSize;		//두번째 이미가 늘어날 Pixel값 저장
	eThirdGridImageType	m_eImageType;		//이미지 선택 타입 저장(첫번째, 두번째, 세번째)
	eArrayType			m_eScaleType;		//스케일 타입 저장
	bool				m_bChange;

private:
	CThirdGridBase *	m_ThirdGridBase;	//ThirdGridBase 값 저장

private:
	HWND	m_ParenthWnd;					//부모 윈도우
	HWND	m_PicturehWnd;					//텍스쳐 윈도우

public:
	void	Initialize();
	void	CreateAsset();													//ThirdGrid 에셋 생성
	void	SetThirdGridProperty();											//ThirdGrid 속성 값 세팅
	void	RefreshDlgBar(CThirdGridBase* _LoadBase, POINT& _NormalTex);	//에셋 불러왔을때 에디터에 속성값 세팅
	void	SetMemberThirdGridBase(CThirdGridBase* _base);					//ThirdGridBase 세팅

	void	SetParenthWnd(HWND _hWnd){	m_ParenthWnd = _hWnd;	}			//부모윈도우 세팅
	void	SetPictureWnd(HWND _hWnd){	m_PicturehWnd = _hWnd;	}			//텍스쳐 윈도우 세팅

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
	afx_msg LRESULT OnSelchangeTexturelist(WPARAM wParam, LPARAM lParam);	//텍스쳐 윈도우에서 리스트 클릭했을때 이벤트 처리
	afx_msg LRESULT OnChangeDragboxInfo(WPARAM wParam, LPARAM lParam);	
};
