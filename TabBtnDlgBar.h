#pragma once
// TabBtnDlgBar ��ȭ �����Դϴ�.

class CTabBtnDlgBar : public CDialogBar
{
	DECLARE_DYNAMIC(CTabBtnDlgBar)

private:
	CEdit*		m_TexturePathEdit;		//�ؽ��� ��� ������
	CEdit*		m_NormalLeft_Edit;		//Normal Left ��ǥ ������
	CEdit*		m_NormalTop_Edit;		//Normal Top ��ǥ ������
	CEdit*		m_HighLightLeft_Edit;	//HighLight Left ��ǥ ������
	CEdit*		m_HIghLightTop_Edit;	//HighLight Top ��ǥ ������
	CEdit*		m_PressedLeft_Edit;		//Pressed Left ��ǥ ������
	CEdit*		m_PressedTop_Edit;		//Pressed Top ��ǥ ������
	CEdit*		m_DisableLeft_Edit;		//Disable Left ��ǥ ������
	CEdit*		m_DisableTop_Edit;		//Disable Top ��ǥ ������
	CEdit*		m_BtnWidthEdit;			//��ư ���� ũ�� ������
	CEdit*		m_BtnHeightEdit;		//��ư ���� ũ�� ������
	CEdit*		m_RadioBtnColEdit;		//���� ���� ���� ������
	CEdit*		m_RadioBtnRowEdit;		//���� ���� ���� ������
	CEdit*		m_RadioBtnIntervalEdit;	//���°��� ���� ������
	CButton*	m_NormalRadioBtn;		//Normal �̹��� ����(���� ��ư) ������
	CButton*	m_HighlightRadioBtn;	//Highlight �̹��� ����(���� ��ư) ������
	CButton*	m_PressedRadioBtn;		//Pressed �̹��� ����(���� ��ư) ������
	CButton*	m_DisableRadioBtn;		//Disable �̹��� ����(���� ��ư)  ������
	CButton*	m_RadioSortLeftBtn;		//�ؽ�Ʈ ��ġ(Left) ������
	CButton*	m_RadioSortMidBtn;		//�ؽ�Ʈ ��ġ(Middle) ������
	CButton*	m_RadioSortRightBtn;	//�ؽ�Ʈ ��ġ(Right) ������
	CButton*	m_DisableCheckBox;		//Disable ����(������ư) ������

private:
	//	��ư �ؽ�Ʈ
	CEdit*		m_TextRGB_R_Edit;		//�ؽ�Ʈ R �� ������
	CEdit*		m_TextRGB_G_Edit;		//�ؽ�Ʈ G �� ������
	CEdit*		m_TextRGB_B_Edit;		//�ؽ�Ʈ B �� ������
	CEdit*		m_IntervalEdit;			//��ư�ȿ��� �ؽ�Ʈ ó�� ��ġ ���� �� ������
	CEdit*		m_DisableRGB_R_Edit;	//Disable RGB R ������
	CEdit*		m_DisableRGB_G_Edit;	//Disable RGB G ������
	CEdit*		m_DisableRGB_B_Edit;	//Disable RGB B ������
	CEdit*		m_TextPreviewEdit;		//�ؽ�Ʈ �̸� ���� ������

	// �ؽ�Ʈ �ణ [7/16/2019 Gabrielle]
	CEdit*		m_BtnTextVerticalIndentEdit;

private:
	CGroupButton* m_TabGroupBase;	//�׷��ư Ŭ���� ����

	std::string	m_strTexPath;		//�ؽ�Ʈ ��� ����	
	POINT		m_ptNormalTex;		//Normal���� �ؽ�Ʈ ��ġ
	POINT		m_ptHighlightTex;	//Highlight���� �ؽ�Ʈ ��ġ
	POINT		m_ptPressedTex;		//Pressed���� �ؽ�Ʈ ��ġ
	POINT		m_ptDisableTex;		//Disable���� �ؽ�Ʈ ��ġ	

	POINT		m_ptColRow;			//����Ž�� OR ���� Ÿ�� ��ư���� ����
	int			m_iArrayType;		//������ġ(����, ����)
	int			m_iRadioBtnInterval;//��ư���� ����

	bool		m_bChange;
	POINT		m_ptBtnSize;		//��ư ũ��
	int			m_iSelBtn;			//���� ���õǾ��ִ� ����(Normal, Highlight, Pressed, Disable)
	int			m_iTextBoxInterval;	//��ư�ȿ��� �ؽ�Ʈ ó�� ��ġ ���� ��

	// ��ư �ؽ�Ʈ
	int			m_iSortFlag;		//�ؽ�Ʈ ����
	BYTE		m_byRed;			//�ؽ�Ʈ ��(R)
	BYTE		m_byGreen;			//�ؽ�Ʈ ��(G)
	BYTE		m_byBlue;			//�ؽ�Ʈ ��(B)

	BYTE		m_byDisableRed;		//��ư Disable �����϶� �ؽ�Ʈ R ��
	BYTE		m_byDisableGreen;	//��ư Disable �����϶� �ؽ�Ʈ G ��
	BYTE		m_byDisableBlue;	//��ư Disable �����϶� �ؽ�Ʈ B ��

	CString		m_strPreview;		//�ؽ�Ʈ �̸����� ����

private:
	HWND	m_ParenthWnd;			//ParenthWnd
	HWND	m_PictureWnd;			//PictureWnd

public:
	POINT	m_ptCurPos;				//���� ��ġ

public:
	void	SetParenthWnd(HWND _hWnd) { m_ParenthWnd = _hWnd; }
	void	SetPictureWnd(HWND _hWnd) { m_PictureWnd = _hWnd; }	

	void	Initialize();													//Ŭ���� �ʱ�ȭ
	void	SetMemberTabGroupBase(CGroupButton* _base);						//�׷��ư ����
	void	SetTabBtnProperty();											//�ǹ�ư ���� ����
	void	CreateAsset();													//�ǹ�ư ���� ����
	void	CreateTabBtn();													//�׷��ư ����
	void	SettingControl( void* _data, eRadioBtnDlgBarControl _controlID );//���̾�α� ����
	void	RefreshDlgBar(CGroupButton* _LoadBase, POINT& _NormalTex);			//������ ���� �ʱ�ȭ

public:
	CTabBtnDlgBar(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CTabBtnDlgBar();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
