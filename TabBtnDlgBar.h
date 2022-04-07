#pragma once
// TabBtnDlgBar 대화 상자입니다.

class CTabBtnDlgBar : public CDialogBar
{
	DECLARE_DYNAMIC(CTabBtnDlgBar)

private:
	CEdit*		m_TexturePathEdit;		//텍스쳐 경로 에디터
	CEdit*		m_NormalLeft_Edit;		//Normal Left 좌표 에디터
	CEdit*		m_NormalTop_Edit;		//Normal Top 좌표 에디터
	CEdit*		m_HighLightLeft_Edit;	//HighLight Left 좌표 에디터
	CEdit*		m_HIghLightTop_Edit;	//HighLight Top 좌표 에디터
	CEdit*		m_PressedLeft_Edit;		//Pressed Left 좌표 에디터
	CEdit*		m_PressedTop_Edit;		//Pressed Top 좌표 에디터
	CEdit*		m_DisableLeft_Edit;		//Disable Left 좌표 에디터
	CEdit*		m_DisableTop_Edit;		//Disable Top 좌표 에디터
	CEdit*		m_BtnWidthEdit;			//버튼 가로 크기 에디터
	CEdit*		m_BtnHeightEdit;		//버튼 세로 크기 에디터
	CEdit*		m_RadioBtnColEdit;		//가로 에셋 갯수 에디터
	CEdit*		m_RadioBtnRowEdit;		//세로 에셋 갯수 에디터
	CEdit*		m_RadioBtnIntervalEdit;	//에셋간의 간격 에디터
	CButton*	m_NormalRadioBtn;		//Normal 이미지 선택(라디오 버튼) 에디터
	CButton*	m_HighlightRadioBtn;	//Highlight 이미지 선택(라디오 버튼) 에디터
	CButton*	m_PressedRadioBtn;		//Pressed 이미지 선택(라디오 버튼) 에디터
	CButton*	m_DisableRadioBtn;		//Disable 이미지 선택(라디오 버튼)  에디터
	CButton*	m_RadioSortLeftBtn;		//텍스트 위치(Left) 에디터
	CButton*	m_RadioSortMidBtn;		//텍스트 위치(Middle) 에디터
	CButton*	m_RadioSortRightBtn;	//텍스트 위치(Right) 에디터
	CButton*	m_DisableCheckBox;		//Disable 선택(라디오버튼) 에디터

private:
	//	버튼 텍스트
	CEdit*		m_TextRGB_R_Edit;		//텍스트 R 값 에디터
	CEdit*		m_TextRGB_G_Edit;		//텍스트 G 값 에디터
	CEdit*		m_TextRGB_B_Edit;		//텍스트 B 값 에디터
	CEdit*		m_IntervalEdit;			//버튼안에서 텍스트 처음 위치 설정 값 에디터
	CEdit*		m_DisableRGB_R_Edit;	//Disable RGB R 에디터
	CEdit*		m_DisableRGB_G_Edit;	//Disable RGB G 에디터
	CEdit*		m_DisableRGB_B_Edit;	//Disable RGB B 에디터
	CEdit*		m_TextPreviewEdit;		//텍스트 미리 보기 에디터

	// 텍스트 행간 [7/16/2019 Gabrielle]
	CEdit*		m_BtnTextVerticalIndentEdit;

private:
	CGroupButton* m_TabGroupBase;	//그룹버튼 클래스 저장

	std::string	m_strTexPath;		//텍스트 경로 저장	
	POINT		m_ptNormalTex;		//Normal상태 텍스트 위치
	POINT		m_ptHighlightTex;	//Highlight상태 텍스트 위치
	POINT		m_ptPressedTex;		//Pressed상태 텍스트 위치
	POINT		m_ptDisableTex;		//Disable상태 텍스트 위치	

	POINT		m_ptColRow;			//가로탐입 OR 세로 타입 버튼간의 간격
	int			m_iArrayType;		//생성위치(가로, 세로)
	int			m_iRadioBtnInterval;//버튼간의 간격

	bool		m_bChange;
	POINT		m_ptBtnSize;		//버튼 크기
	int			m_iSelBtn;			//현재 선택되어있는 상태(Normal, Highlight, Pressed, Disable)
	int			m_iTextBoxInterval;	//버튼안에서 텍스트 처음 위치 설정 값

	// 버튼 텍스트
	int			m_iSortFlag;		//텍스트 정렬
	BYTE		m_byRed;			//텍스트 색(R)
	BYTE		m_byGreen;			//텍스트 색(G)
	BYTE		m_byBlue;			//텍스트 색(B)

	BYTE		m_byDisableRed;		//버튼 Disable 상태일때 텍스트 R 값
	BYTE		m_byDisableGreen;	//버튼 Disable 상태일때 텍스트 G 값
	BYTE		m_byDisableBlue;	//버튼 Disable 상태일때 텍스트 B 값

	CString		m_strPreview;		//텍스트 미리보기 저장

private:
	HWND	m_ParenthWnd;			//ParenthWnd
	HWND	m_PictureWnd;			//PictureWnd

public:
	POINT	m_ptCurPos;				//에셋 위치

public:
	void	SetParenthWnd(HWND _hWnd) { m_ParenthWnd = _hWnd; }
	void	SetPictureWnd(HWND _hWnd) { m_PictureWnd = _hWnd; }	

	void	Initialize();													//클래스 초기화
	void	SetMemberTabGroupBase(CGroupButton* _base);						//그룹버튼 세팅
	void	SetTabBtnProperty();											//탭버튼 정보 저장
	void	CreateAsset();													//탭버튼 에셋 생성
	void	CreateTabBtn();													//그룹버튼 세팅
	void	SettingControl( void* _data, eRadioBtnDlgBarControl _controlID );//다이얼로그 세팅
	void	RefreshDlgBar(CGroupButton* _LoadBase, POINT& _NormalTex);			//에디터 정보 초기화

public:
	CTabBtnDlgBar(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTabBtnDlgBar();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEditTbNmLeft();
	afx_msg void OnEnChangeEditTbNmTop();
	afx_msg void OnBnClickedRadioTbNmBtn();
	afx_msg void OnEnChangeEditTbHlLeft();
	afx_msg void OnEnChangeEditTbHlTop();
	afx_msg void OnBnClickedRadioTbHlBtn();
	afx_msg void OnEnChangeEditTbPrLeft();
	afx_msg void OnEnChangeEditTbPrTop();	
	afx_msg void OnBnClickedRadioTbPrBtn();	
	afx_msg void OnEnChangeEditTbDaLeft();
	afx_msg void OnEnChangeEditTbDaTop();
	afx_msg void OnBnClickedRadioTbDaBtn();
	afx_msg void OnEnChangeEditTbCol();
	afx_msg void OnEnChangeEditTbRow();
	afx_msg void OnEnChangeEditTbInterval();
	afx_msg void OnBnClickedTbCheckDisablestate();
	afx_msg void OnBnClickedRadioTbLeftSort();
	afx_msg void OnBnClickedRadioTbCenterSort();
	afx_msg void OnBnClickedRadioTbRightSort();
	afx_msg void OnEnChangeEditTbInputbox();
	afx_msg void OnEnChangeEditTbTextboxinterval();
	afx_msg void OnEnChangeEditTbRgbR();
	afx_msg void OnEnChangeEditTbRgbG();
	afx_msg void OnEnChangeEditTbRgbB();
	afx_msg void OnEnChangeEditTbDaRgbR();
	afx_msg void OnEnChangeEditTbDaRgbG();
	afx_msg void OnEnChangeEditTbDaRgbB();
	afx_msg void OnEnChangeEditTbShowTexturePath();

	afx_msg LRESULT OnChangeDragboxInfo(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSelchangeTexturelist(WPARAM wParam, LPARAM lParam);
	afx_msg void OnEnChangeEditTbBtnWidth();
	afx_msg void OnEnChangeEditTbBtnHeight();
	afx_msg void OnEnChangeEditTabbtnVerticalIndent();
};
