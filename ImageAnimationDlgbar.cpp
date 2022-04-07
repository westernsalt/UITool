// CImageAnimationDlgbar.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "UITool.h"
#include "ImageAnimationDlgbar.h"
#include "afxdialogex.h"
#include "DBMgr.h"
#include "ObjMgr.h"
#include "ImageAnimationBase.h"

// CImageAnimationDlgbar 대화 상자입니다.
IMPLEMENT_DYNAMIC(CImageAnimationDlgbar, CDialogBar)

	CImageAnimationDlgbar::CImageAnimationDlgbar(CWnd* pParent /*=NULL*/)	
{
	m_ImageAnimationBase = NULL;

	m_ptTextureSize.x = 0;
	m_ptTextureSize.y = 0;
	m_ptImageStartPos.x = 0;
	m_ptImageStartPos.y = 0;
	m_ptImageInterval.x = 0;
	m_ptImageInterval.y = 0;
	m_ptImageSize.x = 0;
	m_ptImageSize.y = 0;
	m_ptImageNum.x = 0;
	m_ptImageNum.y = 0;
	m_iImageTotalNum = 0;
	m_fDelayTime = 0.0f;
	m_iLoop = 0;
}

CImageAnimationDlgbar::~CImageAnimationDlgbar()
{
}

void CImageAnimationDlgbar::Initialize()
{
	UpdateData(true);

	m_strTexturePath = "";

	m_ptTextureSize.x = 0;
	m_ptTextureSize.y = 0;
	m_ptImageInterval.x = 0;
	m_ptImageInterval.y = 0;
	m_ptImageSize.x = 0;
	m_ptImageSize.y = 0;
	m_ptImageNum.x = 0;
	m_ptImageNum.y = 0;
	m_iImageTotalNum = 0;
	m_fDelayTime = 0.0f;

	m_bChange = false;

	m_pTexturePathEdit->SetWindowText(L"0");
	m_pTextureWidthSizeEdit->SetWindowText(L"0");
	m_pTextureHeightSizeEdit->SetWindowText(L"0");
	m_pImageStartPosX->SetWindowText(L"0");
	m_pImageStartPosY->SetWindowText(L"0");
	m_pImageWidthEdit->SetWindowText(L"0");
	m_pImageHeightEdit->SetWindowText(L"0");
	m_pImageWidthSpaceEdit->SetWindowText(L"0");
	m_pImageHeightSpaceEdit->SetWindowText(L"0");
	m_pImageWidthNumEdit->SetWindowText(L"0");
	m_pImageHeightNumEdit->SetWindowText(L"0");
	m_pImageTotalNumEdit->SetWindowText(L"0");
	m_pDelayTimeEdit->SetWindowText(L"0");
	m_pAniLoop->SetCheck(BST_UNCHECKED);

	m_bChange = true;
}

void CImageAnimationDlgbar::CreateAsset()
{	
	if(m_ImageAnimationBase == NULL)
	{
		m_ImageAnimationBase = new CImageAnimationBase;
		std::string strAtt =  "IMAGEANIMATION";
		POINT ptZeroPos;
		ptZeroPos.x = 0;
		ptZeroPos.y = 0;

		//	에셋의 포지션을 설정한다
		m_ImageAnimationBase->SetPosition(ptZeroPos);
		m_ImageAnimationBase->SetAssetType(ASSET_IMAGEANIMATION);
		m_ImageAnimationBase->SetFilePath(m_strTexturePath);
		m_ImageAnimationBase->SetParenthWnd(m_ParenthWnd);
		//	에셋의 멤버 변수들을 셋팅해준다
		((CImageAnimationBase*)m_ImageAnimationBase)->SetImageAnimationProperty(m_ptImageStartPos, m_ptTextureSize, m_ptImageInterval, 
																				m_ptImageSize, m_ptImageNum, m_iImageTotalNum, m_fDelayTime, m_iLoop);

		//	만들어진 에셋을 Map에 저장하는 과정
		CObjMgr::GetInstance()->InsertAssetBase(m_ImageAnimationBase);
		::SendMessage(m_ParenthWnd, WM_DRAW_ASSETBASE, NULL, NULL);	
	}
}

void CImageAnimationDlgbar::RefreshDlgBar( CImageAnimationBase* _LoadBase, POINT& _NormalTex )
{
	m_bChange = false;

	SetMemberImageAnimationBase(_LoadBase);

	std::string Path;

	int iNum = _LoadBase->GetStartPos().x;
	_NormalTex.x = iNum;

	iNum = _LoadBase->GetStartPos().y;
	_NormalTex.y = iNum;

	iNum = _LoadBase->GetTextureSize().x;
	SettingControl(&iNum, IMAGEANIMATIONDLGBAR_TEXTURE_WIDTH_SIZE);

	iNum = _LoadBase->GetTextureSize().y;
	SettingControl(&iNum, IMAGEANIMATIONDLGBAR_TEXTURE_HEIGHT_SIZE);

	iNum = _LoadBase->GetImageStartPos().x;
	SettingControl(&iNum, IMAGEANIMATIONDLGBAR_IMAGE_START_POS_X);

	iNum = _LoadBase->GetImageStartPos().y;
	SettingControl(&iNum, IMAGEANIMATIONDLGBAR_IMAGE_START_POS_Y);

	iNum = _LoadBase->GetSingleImageSize().x;
	SettingControl(&iNum, IMAGEANIMATIONDLGBAR_IMAGE_WIDTH);

	iNum = _LoadBase->GetSingleImageSize().y;
	SettingControl(&iNum, IMAGEANIMATIONDLGBAR_IMAGE_HEIGHT);

	iNum = _LoadBase->GetImageSpace().x;
	SettingControl(&iNum, IMAGEANIMATIONDLGBAR_IMAGE_WIDTH_SPACE);

	iNum = _LoadBase->GetImageSpace().y;
	SettingControl(&iNum, IMAGEANIMATIONDLGBAR_IMAGE_HEIGHT_SPACE);

	iNum = _LoadBase->GetImageCount().x;
	SettingControl(&iNum, IMAGEANIMATIONDLGBAR_IMAGE_WIDTH_NUM);

	iNum = _LoadBase->GetImageCount().y;
	SettingControl(&iNum, IMAGEANIMATIONDLGBAR_IMAGE_HEIGHT_NUM);

	iNum = _LoadBase->GetImageTotalCount();
	SettingControl(&iNum, IMAGEANIMATIONDLGBAR_IMAGE_TOTAL_NUM);	

	float fNum = _LoadBase->GetDelayTime();
	SettingControl(&fNum, IMAGEANIMATIONDLGBAR_DELAY_TIME);

	iNum = _LoadBase->GetAniLoop();
	SettingControl(&iNum, IMAGEANIMATIONDLGBAR_ANI_LOOP);

	Path = _LoadBase->GetFilePath();
	SettingControl(&Path, IMAGEANIMATIONDLGBAR_TEXTUREPATH);

	//	셋팅컨트롤을 거치고 나서 모든 변수들이 초기화된다.
	//	그래서 이거를 맨 뒤로 밀어봤다.

	iNum = _LoadBase->GetSingleImageSize().x;
	SettingControl(&iNum, IMAGEANIMATIONDLGBAR_IMAGE_WIDTH);

	iNum = _LoadBase->GetSingleImageSize().y;
	SettingControl(&iNum, IMAGEANIMATIONDLGBAR_IMAGE_HEIGHT);

	m_bChange = true;
}

void CImageAnimationDlgbar::SetImageAnimationProperty()
{
	if(m_strTexturePath != "")
	{
		CreateAsset();

		m_ImageAnimationBase->SetFilePath(m_strTexturePath);
		m_ImageAnimationBase->SetImageAnimationProperty(m_ptTextureSize, m_ptImageStartPos, m_ptImageInterval, m_ptImageSize, m_ptImageNum,
			m_iImageTotalNum, m_fDelayTime, m_iLoop);
		m_ImageAnimationBase->SetParenthWnd(m_ParenthWnd);
		::SendMessage(m_ParenthWnd, WM_DRAW_ASSETBASE, NULL, NULL);
	}	
}

void CImageAnimationDlgbar::SettingControl( void* _data, eImageAnimationControl _controlID )
{
	UpdateData(true);

	m_bChange = false;
	UpdateData(true);

	switch(_controlID)
	{
	case IMAGEANIMATIONDLGBAR_TEXTUREPATH:
		{
			m_strTexturePath = *((std::string*)_data);
			CString TexPath(m_strTexturePath.c_str());
			m_pTexturePathEdit->SetWindowText(TexPath);
		}		
		break;
	case IMAGEANIMATIONDLGBAR_TEXTURE_WIDTH_SIZE:
		m_ptTextureSize.x = *((int*)_data);
		EditTextSetting(m_ptTextureSize.x, m_pTextureWidthSizeEdit);
		break;
	case IMAGEANIMATIONDLGBAR_TEXTURE_HEIGHT_SIZE: 
		m_ptTextureSize.y = *((int*)_data);
		EditTextSetting(m_ptTextureSize.y, m_pTextureHeightSizeEdit);
		break;
	case IMAGEANIMATIONDLGBAR_IMAGE_START_POS_X:
		m_ptImageStartPos.x = *((int*)_data);
		EditTextSetting(m_ptImageStartPos.x, m_pImageStartPosX);
		break;
	case IMAGEANIMATIONDLGBAR_IMAGE_START_POS_Y:
		m_ptImageStartPos.y = *((int*)_data);
		EditTextSetting(m_ptImageStartPos.y, m_pImageStartPosY);
		break;
	case IMAGEANIMATIONDLGBAR_IMAGE_WIDTH:
		m_ptImageSize.x = *((int*)_data);
		EditTextSetting(m_ptImageSize.x, m_pImageWidthEdit);
		break;
	case IMAGEANIMATIONDLGBAR_IMAGE_HEIGHT:
		m_ptImageSize.y = *((int*)_data);
		EditTextSetting(m_ptImageSize.y, m_pImageHeightEdit);
		break;
	case IMAGEANIMATIONDLGBAR_IMAGE_WIDTH_SPACE:
		m_ptImageInterval.x = *((int*)_data);
		EditTextSetting(m_ptImageInterval.x, m_pImageWidthSpaceEdit);
		break;
	case IMAGEANIMATIONDLGBAR_IMAGE_HEIGHT_SPACE:
		m_ptImageInterval.y = *((int*)_data);
		EditTextSetting(m_ptImageInterval.y, m_pImageHeightSpaceEdit);
		break;
	case IMAGEANIMATIONDLGBAR_IMAGE_WIDTH_NUM:
		m_ptImageNum.x = *((int*)_data);
		EditTextSetting(m_ptImageNum.x, m_pImageWidthNumEdit);
		break;
	case IMAGEANIMATIONDLGBAR_IMAGE_HEIGHT_NUM:
		m_ptImageNum.y = *((int*)_data);
		EditTextSetting(m_ptImageNum.y, m_pImageHeightNumEdit);
		break;
	case IMAGEANIMATIONDLGBAR_IMAGE_TOTAL_NUM:
		m_iImageTotalNum = *((int*)_data);
		EditTextSetting(m_iImageTotalNum, m_pImageTotalNumEdit);
		break; 
	case IMAGEANIMATIONDLGBAR_DELAY_TIME:
		m_fDelayTime = *((float*)_data);
		FloatEditTextSetting(m_fDelayTime, m_pDelayTimeEdit);
		break;
	case IMAGEANIMATIONDLGBAR_ANI_LOOP:
		m_iLoop = *((int*)_data);
		m_pAniLoop->SetCheck(m_iLoop);
		break;
	}

	m_bChange = true;
}

void CImageAnimationDlgbar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	m_pTexturePathEdit = (CEdit*)GetDlgItem(IDC_EDIT_ANI_TEXTURE_PATH);
	m_pTextureWidthSizeEdit = (CEdit*)GetDlgItem(IDC_ANI_TEXTURE_SIZE_WIDTH);
	m_pTextureHeightSizeEdit = (CEdit*)GetDlgItem(IDC_ANI_TEXTURE_SIZE_HEIGHT);
	m_pImageStartPosX = (CEdit*)GetDlgItem(IDC_ANI_IMAGE_START_POS_X);
	m_pImageStartPosY = (CEdit*)GetDlgItem(IDC_ANI_IMAGE_START_POS_Y);	
	m_pImageWidthEdit = (CEdit*)GetDlgItem(IDC_ANI_FRAME_TEXTURE_SIZE_WIDTH);
	m_pImageHeightEdit = (CEdit*)GetDlgItem(IDC_ANI_FRAME_TEXTURE_SIZE_HEIGHT);
	m_pImageWidthSpaceEdit = (CEdit*)GetDlgItem(IDC_ANI_IMAGE_SPACE_WIDTH);
	m_pImageHeightSpaceEdit = (CEdit*)GetDlgItem(IDC_ANI_IMAGE_SPACE_HEIGHT);
	m_pImageWidthNumEdit = (CEdit*)GetDlgItem(IDC_ANI_IMAGE_NUM_WIDTH);
	m_pImageHeightNumEdit = (CEdit*)GetDlgItem(IDC_ANI_IMAGE_NUM_HEIGHT);
	m_pImageTotalNumEdit = (CEdit*)GetDlgItem(IDC_ANI_IMAGE_TOTAL_NUM);
	m_pDelayTimeEdit = (CEdit*)GetDlgItem(IDC_ANI_DELAYTIME);
	m_pAniLoop = (CButton*)GetDlgItem(IDC_ANI_LOOP);

	if(CObjMgr::GetInstance()->GetFocusAssetBase() == NULL)
	{
		m_ImageAnimationBase = NULL;
	}
}


BEGIN_MESSAGE_MAP(CImageAnimationDlgbar, CDialogBar)
	ON_EN_CHANGE(IDC_EDIT_ANI_TEXTURE_PATH, &CImageAnimationDlgbar::OnEnChangeEditShowTexturePath)
	ON_EN_CHANGE(IDC_ANI_TEXTURE_SIZE_WIDTH, &CImageAnimationDlgbar::OnEnChangeAnimationAniTextureSizeWidth)
	ON_EN_CHANGE(IDC_ANI_TEXTURE_SIZE_HEIGHT, &CImageAnimationDlgbar::OnEnChangeAnimationAniTextureSizeHeight)
	ON_EN_CHANGE(IDC_ANI_IMAGE_START_POS_X, &CImageAnimationDlgbar::OnEnChangeAniImageStartPosX)
	ON_EN_CHANGE(IDC_ANI_IMAGE_START_POS_Y, &CImageAnimationDlgbar::OnEnChangeAniImageStartPosY)
	ON_EN_CHANGE(IDC_ANI_FRAME_TEXTURE_SIZE_WIDTH, &CImageAnimationDlgbar::OnEnChangeAnimationImageSizeWidth)
	ON_EN_CHANGE(IDC_ANI_FRAME_TEXTURE_SIZE_HEIGHT, &CImageAnimationDlgbar::OnEnChangeAnimationImageSizeHeight)
	ON_EN_CHANGE(IDC_ANI_IMAGE_SPACE_WIDTH, &CImageAnimationDlgbar::OnEnChangeAnimationImageSpaceWidth)
	ON_EN_CHANGE(IDC_ANI_IMAGE_SPACE_HEIGHT, &CImageAnimationDlgbar::OnEnChangeAnimationImageSpaceHeight)
	ON_EN_CHANGE(IDC_ANI_IMAGE_NUM_WIDTH, &CImageAnimationDlgbar::OnEnChangeAnimationImageNumWidth)
	ON_EN_CHANGE(IDC_ANI_IMAGE_NUM_HEIGHT, &CImageAnimationDlgbar::OnEnChangeAnimationImageNumHeight)
	ON_EN_CHANGE(IDC_ANI_IMAGE_TOTAL_NUM, &CImageAnimationDlgbar::OnEnChangeAnimationImageTotalNum)
	ON_EN_CHANGE(IDC_ANI_DELAYTIME, &CImageAnimationDlgbar::OnEnChangeAnimationDelaytime)	
	ON_MESSAGE(WM_SELCHANGE_TEXTURELIST, &CImageAnimationDlgbar::OnSelchangeTexturelist)
	ON_MESSAGE(WM_CHANGE_DRAGBOX_INFO, &CImageAnimationDlgbar::OnChangeDragboxInfo)	
	ON_BN_CLICKED(IDC_ANI_LOOP, &CImageAnimationDlgbar::OnBnClickedAniLoop)
END_MESSAGE_MAP()

void CImageAnimationDlgbar::OnEnChangeEditShowTexturePath()
{
	UpdateData(TRUE);
	SetImageAnimationProperty();
	m_ImageAnimationBase->ChangeTexture();
}

void CImageAnimationDlgbar::OnEnChangeAnimationAniTextureSizeWidth()
{
	UpdateData(TRUE);
}

void CImageAnimationDlgbar::OnEnChangeAnimationAniTextureSizeHeight()
{
	UpdateData(TRUE);
}

void CImageAnimationDlgbar::OnEnChangeAnimationImageSizeWidth()
{
	UpdateData(TRUE);
	m_ptImageSize.x =GetEditText(m_pImageWidthEdit);
	SetImageAnimationProperty();	
}

void CImageAnimationDlgbar::OnEnChangeAnimationImageSizeHeight()
{
	UpdateData(TRUE);
	m_ptImageSize.y =GetEditText(m_pImageHeightEdit);
	SetImageAnimationProperty();
}

void CImageAnimationDlgbar::OnEnChangeAnimationImageSpaceWidth()
{
	UpdateData(TRUE);
	m_ptImageInterval.x = GetEditText(m_pImageWidthSpaceEdit);
	SetImageAnimationProperty();
}

void CImageAnimationDlgbar::OnEnChangeAnimationImageSpaceHeight()
{
	UpdateData(TRUE);
	m_ptImageInterval.y = GetEditText(m_pImageHeightSpaceEdit);
	SetImageAnimationProperty();
}

void CImageAnimationDlgbar::OnEnChangeAnimationImageNumWidth()
{
	UpdateData(TRUE);
	m_ptImageNum.x = GetEditText(m_pImageWidthNumEdit);
	SetImageAnimationProperty();
}

void CImageAnimationDlgbar::OnEnChangeAnimationImageNumHeight()
{
	UpdateData(TRUE);
	m_ptImageNum.y = GetEditText(m_pImageHeightNumEdit);
	SetImageAnimationProperty();
}

void CImageAnimationDlgbar::OnEnChangeAnimationImageTotalNum()
{
	UpdateData(TRUE);
	m_iImageTotalNum = GetEditText(m_pImageTotalNumEdit);
	SetImageAnimationProperty();
}

void CImageAnimationDlgbar::OnEnChangeAnimationDelaytime()
{
	UpdateData(TRUE);
	CString strWndText;
	m_pDelayTimeEdit->GetWindowText(strWndText);

	CT2CA pszConvertedText (strWndText);
	std::string _strConvertText(pszConvertedText);

	m_fDelayTime = atof(_strConvertText.c_str());
	SetImageAnimationProperty();
}

void CImageAnimationDlgbar::OnEnChangeAniImageStartPosX()
{
	UpdateData(TRUE);
	m_ptImageStartPos.x = GetEditText(m_pImageStartPosX);
	SetImageAnimationProperty();
}

void CImageAnimationDlgbar::OnEnChangeAniImageStartPosY()
{
	UpdateData(TRUE);
	m_ptImageStartPos.y = GetEditText(m_pImageStartPosY);
	SetImageAnimationProperty();
}

void CImageAnimationDlgbar::OnBnClickedAniLoop()
{
	UpdateData(TRUE);
	m_pAniLoop = (CButton*)GetDlgItem(IDC_ANI_LOOP);
	if(m_pAniLoop->GetCheck() == BST_CHECKED)
		m_pAniLoop->SetCheck(BST_CHECKED);
	else
		m_pAniLoop->SetCheck(BST_UNCHECKED);

	m_iLoop = m_pAniLoop->GetCheck();
	
	SetImageAnimationProperty();
}

//텍스쳐 윈도우에서 리스트 클릭했을때 이벤트 처리
afx_msg LRESULT CImageAnimationDlgbar::OnSelchangeTexturelist(WPARAM wParam, LPARAM lParam)
{
	UpdateData(TRUE);

	m_strTexturePath = (CDBMgr::GetInstance()->GetPathMap()->find((int)wParam)->second);

	//텍스쳐 사이즈 업데이트
	CxImage* imageInfo = (CDBMgr::GetInstance()->GetTexMap()->find((int)wParam)->second);
	if(imageInfo != NULL)
	{
		int width = imageInfo->GetWidth();
		int height = imageInfo->GetHeight();
		m_ptTextureSize.x = width;
		m_ptTextureSize.y = height;

		CString strResult;
		char szITOA[20];
		_itoa_s(width, szITOA, 10);
		std::string szResult = szITOA;
		strResult = szResult.c_str();
		m_pTextureWidthSizeEdit->SetWindowText(strResult);

		_itoa_s(height, szITOA, 10);
		szResult = szITOA;
		strResult = szResult.c_str();
		m_pTextureHeightSizeEdit->SetWindowText(strResult);
	}	

	CA2T wt(m_strTexturePath.c_str());
	m_pTexturePathEdit->SetWindowText(wt);

	if(m_ImageAnimationBase != NULL)
	{
		m_ImageAnimationBase->SetFilePath(m_strTexturePath);
		::SendMessage(m_ParenthWnd, WM_DRAW_ASSETBASE, NULL, NULL);
	}
	return 0;
}

afx_msg LRESULT CImageAnimationDlgbar::OnChangeDragboxInfo(WPARAM wParam, LPARAM lParam)
{
	//	드래그 박스 인포메이션이 변경되었을 경우.
	//	크리티컬 세션처럼 쓴다.
	m_bChange = false;

	UpdateData(true);

	switch((int)lParam)
	{
	case DRAGBOX_WIDTH:
		m_ptImageSize.x = (int)wParam;
		SetEditWndText(m_pImageWidthEdit, m_ptImageSize.x);
		break;
	case DRAGBOX_HEIGHT:
		m_ptImageSize.y = (int)wParam;
		SetEditWndText(m_pImageHeightEdit, m_ptImageSize.y);
		break;
	}

	m_bChange = true;
	return 0;
}

