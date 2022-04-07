// ThirdGridDlgBar.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "UITool.h"
#include "ThirdGridDlgBar.h"
#include "afxdialogex.h"
#include "DBMgr.h"
#include "ObjMgr.h"
#include "ThirdGridBase.h"

// ThirdGridDlgBar 대화 상자입니다.
IMPLEMENT_DYNAMIC(CThirdGridDlgBar, CDialogBar)
CThirdGridDlgBar::CThirdGridDlgBar(CWnd* pParent /*=NULL*/)
{
	m_ThirdGridBase = NULL;
}

CThirdGridDlgBar::~CThirdGridDlgBar()
{
}

void CThirdGridDlgBar::Initialize()
{
	UpdateData(true);

	m_strTexturePath = "";
	m_TexturePathEdit->SetWindowText(L"0");
	m_FirstLeft_Edit->SetWindowText(L"0");
	m_FirstTop_Edit->SetWindowText(L"0");
	m_SecondLeft_Edit->SetWindowText(L"0");
	m_SecondTop_Edit->SetWindowText(L"0");
	m_ThirdLeft_Edit->SetWindowText(L"0");
	m_ThirdTop_Edit->SetWindowText(L"0");
	m_Image_Width_Edit->SetWindowText(L"0");
	m_ImageHeight_Edit->SetWindowText(L"0");
	m_ScaleImageSize_Edit->SetWindowText(L"0");

	m_FirstRadioBtn->SetCheck(1);
	m_SeconRadioBtn->SetCheck(0);
	m_ThirdRadioBtn->SetCheck(0);

	m_WidthScaleRadioBtn->SetCheck(1);
	m_HeightScaleRadioBtn->SetCheck(0);

	m_ptFirstImageTex.x = 0;
	m_ptFirstImageTex.y = 0;
	m_ptSecondImageTex.x = 0;
	m_ptSecondImageTex.y = 0;
	m_ptThirdImageTex.x = 0;
	m_ptThirdImageTex.y = 0;
	m_ptImageSize.x = 0;
	m_ptImageSize.y = 0;
	m_iPixelSize = 0;
	m_bChange = true;
	m_eImageType = FIRST_IMAGE;
	m_eScaleType = ARRAYTYPE_WIDTH;
}

void CThirdGridDlgBar::CreateAsset()
{
	if(m_ThirdGridBase == NULL)
	{
		m_ThirdGridBase = new CThirdGridBase;		
		POINT ptZeroPos;
		ptZeroPos.x = 0;
		ptZeroPos.y = 0;

		//	에셋의 포지션을 설정한다
		m_ThirdGridBase->SetPosition(ptZeroPos);
		m_ThirdGridBase->SetAssetType(ASSET_THIRDGRID);
		m_ThirdGridBase->SetFilePath(m_strTexturePath);
		m_ThirdGridBase->SetParenthWnd(m_ParenthWnd);
		//	에셋의 멤버 변수들을 셋팅해준다
		((CThirdGridBase*)m_ThirdGridBase)->SetThirdGridProperty(m_ptImageSize, m_ptFirstImageTex, m_ptSecondImageTex, 
																 m_ptThirdImageTex, m_eScaleType, m_iPixelSize);

		//	만들어진 에셋을 Map에 저장하는 과정
		CObjMgr::GetInstance()->InsertAssetBase(m_ThirdGridBase);
		::SendMessage(m_ParenthWnd, WM_DRAW_ASSETBASE, NULL, NULL);	
	}
}

void CThirdGridDlgBar::SetThirdGridProperty()
{
	if(m_strTexturePath != "")
	{
		CreateAsset();

		m_ThirdGridBase->SetFilePath(m_strTexturePath);
		m_ThirdGridBase->SetThirdGridProperty(m_ptImageSize, m_ptFirstImageTex, m_ptSecondImageTex, 
											  m_ptThirdImageTex, m_eScaleType, m_iPixelSize);
		m_ThirdGridBase->SetParenthWnd(m_ParenthWnd);
		::SendMessage(m_ParenthWnd, WM_DRAW_ASSETBASE, NULL, NULL);
	}	
}

void CThirdGridDlgBar::RefreshDlgBar( CThirdGridBase* _LoadBase, POINT& _NormalTex )
{
	m_bChange = false;

	SetMemberThirdGridBase(_LoadBase);

	m_ptFirstImageTex.x  = _LoadBase->GetFirstImagePos().x;
	EditTextSetting(m_ptFirstImageTex.x, m_FirstLeft_Edit);
	m_ptFirstImageTex.y = _LoadBase->GetFirstImagePos().y;
	EditTextSetting(m_ptFirstImageTex.y, m_FirstTop_Edit);
	_NormalTex = m_ptFirstImageTex;

	m_ptSecondImageTex.x  = _LoadBase->GetSecondImagePos().x;
	EditTextSetting(m_ptSecondImageTex.x, m_SecondLeft_Edit);
	m_ptSecondImageTex.y = _LoadBase->GetSecondImagePos().y;
	EditTextSetting(m_ptSecondImageTex.y, m_SecondTop_Edit);

	m_ptThirdImageTex.x  = _LoadBase->GetThirdImagePos().x;
	EditTextSetting(m_ptThirdImageTex.x, m_ThirdLeft_Edit);
	m_ptThirdImageTex.y = _LoadBase->GetThirdImagePos().y;
	EditTextSetting(m_ptThirdImageTex.y, m_ThirdTop_Edit);

	m_ptImageSize.x  = _LoadBase->GetImageSize().x;
	EditTextSetting(m_ptImageSize.x, m_Image_Width_Edit);
	m_ptImageSize.y = _LoadBase->GetImageSize().y;
	EditTextSetting(m_ptImageSize.y, m_ImageHeight_Edit);

	m_eScaleType = (eArrayType)_LoadBase->GetScaleType();
	if(m_eScaleType == ARRAYTYPE_WIDTH)
	{
		m_WidthScaleRadioBtn->SetCheck(1);
		m_HeightScaleRadioBtn->SetCheck(0);
	}
	else if(m_eScaleType == ARRAYTYPE_HEIGHT)
	{
		m_WidthScaleRadioBtn->SetCheck(0);
		m_HeightScaleRadioBtn->SetCheck(1);
	}
	m_iPixelSize = _LoadBase->GetPixel();
	EditTextSetting(m_iPixelSize, m_ScaleImageSize_Edit);

	m_bChange = true;
}

void CThirdGridDlgBar::SetMemberThirdGridBase(CThirdGridBase* _base)
{
	m_ThirdGridBase = _base;
}

void CThirdGridDlgBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);

	m_TexturePathEdit = (CEdit*)GetDlgItem(IDC_EDIT_THIRD_GRID_TEXTURE_PATH);
	m_FirstLeft_Edit = (CEdit*)GetDlgItem(IDC_EDIT_THIRD_GRID_FIRST_LEFT);
	m_FirstTop_Edit = (CEdit*)GetDlgItem(IDC_EDIT_THIRD_GRID_FIRST_TOP);
	m_SecondLeft_Edit = (CEdit*)GetDlgItem(IDC_EDIT_THIRD_GRID_SECOND_LEFT);
	m_SecondTop_Edit = (CEdit*)GetDlgItem(IDC_EDIT_THIRD_GRID_SECOND_TOP);
	m_ThirdLeft_Edit = (CEdit*)GetDlgItem(IDC_EDIT_THIRD_GRID_THIRD_LEFT);
	m_ThirdTop_Edit = (CEdit*)GetDlgItem(IDC_EDIT_THIRD_GRID_THIRD_TOP);
	m_Image_Width_Edit = (CEdit*)GetDlgItem(IDC_EDIT_THIRD_GRID_IMAGE_WIDTH);
	m_ImageHeight_Edit = (CEdit*)GetDlgItem(IDC_EDIT_THIRD_GRID_IMAGE_HEIGHT);
	m_ScaleImageSize_Edit = (CEdit*)GetDlgItem(IDC_EDIT_THIRD_GRID_SCALE_SIZE);

	m_FirstRadioBtn = (CButton*)GetDlgItem(IDC_RADIO_THIRD_GRID_FIRST_IMAGE);
	m_SeconRadioBtn = (CButton*)GetDlgItem(IDC_RADIO_THIRD_GRID_SECOND_IMAGE);
	m_ThirdRadioBtn = (CButton*)GetDlgItem(IDC_RADIO_THIRD_GRID_THIRD_IMAGE);

	m_WidthScaleRadioBtn = (CButton*)GetDlgItem(IDC_RADIO_THIRD_GRID_SCALE_TYPE_WIDTH);
	m_HeightScaleRadioBtn = (CButton*)GetDlgItem(IDC_RADIO_THIRD_GRID_SCALE_TYPE_HEIGHT);
}

BEGIN_MESSAGE_MAP(CThirdGridDlgBar, CDialogBar)
	ON_EN_CHANGE(IDC_EDIT_THIRD_GRID_TEXTURE_PATH, &CThirdGridDlgBar::OnEnChangeEditThirdGridTexturePath)
	ON_EN_CHANGE(IDC_EDIT_THIRD_GRID_FIRST_LEFT, &CThirdGridDlgBar::OnEnChangeEditThirdGridFirstLeft)
	ON_EN_CHANGE(IDC_EDIT_THIRD_GRID_FIRST_TOP, &CThirdGridDlgBar::OnEnChangeEditThirdGridFirstTop)
	ON_BN_CLICKED(IDC_RADIO_THIRD_GRID_FIRST_IMAGE, &CThirdGridDlgBar::OnBnClickedRadioThirdGridFirstImage)
	ON_EN_CHANGE(IDC_EDIT_THIRD_GRID_SECOND_LEFT, &CThirdGridDlgBar::OnEnChangeEditThirdGridSecondLeft)
	ON_EN_CHANGE(IDC_EDIT_THIRD_GRID_SECOND_TOP, &CThirdGridDlgBar::OnEnChangeEditThirdGridSecondTop)	
	ON_BN_CLICKED(IDC_RADIO_THIRD_GRID_SECOND_IMAGE, &CThirdGridDlgBar::OnBnClickedRadioThirdGridSecondImage)
	ON_EN_CHANGE(IDC_EDIT_THIRD_GRID_THIRD_LEFT, &CThirdGridDlgBar::OnEnChangeEditThirdGridThirdLeft)
	ON_EN_CHANGE(IDC_EDIT_THIRD_GRID_THIRD_TOP, &CThirdGridDlgBar::OnEnChangeEditThirdGridThirdTop)
	ON_BN_CLICKED(IDC_RADIO_THIRD_GRID_THIRD_IMAGE, &CThirdGridDlgBar::OnBnClickedRadioThirdGridThirdImage)
	ON_EN_CHANGE(IDC_EDIT_THIRD_GRID_IMAGE_WIDTH, &CThirdGridDlgBar::OnEnChangeEditThirdGridImageWidth)
	ON_EN_CHANGE(IDC_EDIT_THIRD_GRID_IMAGE_HEIGHT, &CThirdGridDlgBar::OnEnChangeEditThirdGridImageHeight)	
	ON_BN_CLICKED(IDC_RADIO_THIRD_GRID_SCALE_TYPE_WIDTH, &CThirdGridDlgBar::OnBnClickedRadioThirdGridScaleTypeWidth)
	ON_BN_CLICKED(IDC_RADIO_THIRD_GRID_SCALE_TYPE_HEIGHT, &CThirdGridDlgBar::OnBnClickedRadioThirdGridScaleTypeHeight)
	ON_EN_CHANGE(IDC_EDIT_THIRD_GRID_SCALE_SIZE, &CThirdGridDlgBar::OnEnChangeEditThirdGridScaleSize)
	ON_MESSAGE(WM_CHANGE_DRAGBOX_INFO, &CThirdGridDlgBar::OnChangeDragboxInfo)
	ON_MESSAGE(WM_SELCHANGE_TEXTURELIST, &CThirdGridDlgBar::OnSelchangeTexturelist)
	
END_MESSAGE_MAP()


// ThirdGridDlgBar 메시지 처리기입니다.
void CThirdGridDlgBar::OnEnChangeEditThirdGridTexturePath()
{
	UpdateData(TRUE);
}

void CThirdGridDlgBar::OnEnChangeEditThirdGridFirstLeft()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptFirstImageTex.x = GetEditText(m_FirstLeft_Edit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptFirstImageTex.x, (LPARAM)DRAGBOX_LEFT);
	}
	SetThirdGridProperty();
}

void CThirdGridDlgBar::OnEnChangeEditThirdGridFirstTop()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptFirstImageTex.y = GetEditText(m_FirstTop_Edit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptFirstImageTex.y, (LPARAM)DRAGBOX_TOP);
	}
	SetThirdGridProperty();
}

//첫번째 이미지 라디오 버튼
void CThirdGridDlgBar::OnBnClickedRadioThirdGridFirstImage()
{
	UpdateData(TRUE);
	m_eImageType = FIRST_IMAGE;
	::SendMessage(m_PicturehWnd, WM_CHANGE_RADIOBTN, (WPARAM)m_ptFirstImageTex.x, (LPARAM)m_ptFirstImageTex.y);
}

void CThirdGridDlgBar::OnEnChangeEditThirdGridSecondLeft()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptSecondImageTex.x = GetEditText(m_SecondLeft_Edit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptSecondImageTex.x, (LPARAM)DRAGBOX_LEFT);
	}
	SetThirdGridProperty();
}

void CThirdGridDlgBar::OnEnChangeEditThirdGridSecondTop()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptSecondImageTex.y = GetEditText(m_SecondTop_Edit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptSecondImageTex.y, (LPARAM)DRAGBOX_TOP);
	}
	SetThirdGridProperty();
}

//두번째 이미지 라디오 버튼
void CThirdGridDlgBar::OnBnClickedRadioThirdGridSecondImage()
{
	UpdateData(TRUE);
	m_eImageType = SECOND_IMAGE;
	::SendMessage(m_PicturehWnd, WM_CHANGE_RADIOBTN, (WPARAM)m_ptSecondImageTex.x, (LPARAM)m_ptSecondImageTex.y);
}

void CThirdGridDlgBar::OnEnChangeEditThirdGridThirdLeft()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptThirdImageTex.x = GetEditText(m_ThirdLeft_Edit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptThirdImageTex.x, (LPARAM)DRAGBOX_LEFT);
	}
	SetThirdGridProperty();
}

void CThirdGridDlgBar::OnEnChangeEditThirdGridThirdTop()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptThirdImageTex.y = GetEditText(m_ThirdTop_Edit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptThirdImageTex.y, (LPARAM)DRAGBOX_TOP);
	}
	SetThirdGridProperty();
}

//세번째 이미지 라디오 버튼
void CThirdGridDlgBar::OnBnClickedRadioThirdGridThirdImage()
{
	UpdateData(TRUE);
	m_eImageType = THIRD_IMAGE;
	::SendMessage(m_PicturehWnd, WM_CHANGE_RADIOBTN, (WPARAM)m_ptThirdImageTex.x, (LPARAM)m_ptThirdImageTex.y);
}

void CThirdGridDlgBar::OnEnChangeEditThirdGridImageWidth()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptImageSize.x = GetEditText(m_Image_Width_Edit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptImageSize.x, (LPARAM)DRAGBOX_LEFT);
	}
	SetThirdGridProperty();
}

void CThirdGridDlgBar::OnEnChangeEditThirdGridImageHeight()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptImageSize.y = GetEditText(m_ImageHeight_Edit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptImageSize.y, (LPARAM)DRAGBOX_TOP);
	}
	SetThirdGridProperty();
}

void CThirdGridDlgBar::OnBnClickedRadioThirdGridScaleTypeWidth()
{
	UpdateData(TRUE);
	m_eScaleType = ARRAYTYPE_WIDTH;
	SetThirdGridProperty();
}

void CThirdGridDlgBar::OnBnClickedRadioThirdGridScaleTypeHeight()
{
	UpdateData(TRUE);
	m_eScaleType = ARRAYTYPE_HEIGHT;
	SetThirdGridProperty();
}

void CThirdGridDlgBar::OnEnChangeEditThirdGridScaleSize()
{
	UpdateData(TRUE);
	m_iPixelSize =  GetEditText(m_ScaleImageSize_Edit);
	SetThirdGridProperty();
}

//텍스쳐 윈도우에서 리스트 클릭했을때 이벤트 처리
afx_msg LRESULT CThirdGridDlgBar::OnSelchangeTexturelist(WPARAM wParam, LPARAM lParam)
{
	UpdateData(TRUE);

	m_strTexturePath = (CDBMgr::GetInstance()->GetPathMap()->find((int)wParam)->second);
	
	CA2T wt(m_strTexturePath.c_str());
	m_TexturePathEdit->SetWindowText(wt);

	if(m_ThirdGridBase != NULL)
	{
		m_ThirdGridBase->SetFilePath(m_strTexturePath);
		::SendMessage(m_ParenthWnd, WM_DRAW_ASSETBASE, NULL, NULL);
	}
	return 0;
}

afx_msg LRESULT CThirdGridDlgBar::OnChangeDragboxInfo(WPARAM wParam, LPARAM lParam)
{
	//	드래그 박스 인포메이션이 변경되었을 경우.
	//	크리티컬 세션처럼 쓴다.
	m_bChange = false;
	UpdateData(true);

	//첫번째, 두번째, 세번째 이미지 세팅
	switch(m_eImageType)
	{
	case FIRST_IMAGE:
		switch((int)lParam)
		{
		case DRAGBOX_LEFT:
			m_ptFirstImageTex.x = (int)wParam;
			SetEditWndText(m_FirstLeft_Edit, m_ptFirstImageTex.x);
			break;
		case DRAGBOX_TOP:
			m_ptFirstImageTex.y = (int)wParam;
			SetEditWndText(m_FirstTop_Edit, m_ptFirstImageTex.y);
			break;
		}
		break;
	case SECOND_IMAGE:
		switch((int)lParam)
		{
		case DRAGBOX_LEFT:
			m_ptSecondImageTex.x = (int)wParam;
			SetEditWndText(m_SecondLeft_Edit, m_ptSecondImageTex.x);
			break;
		case DRAGBOX_TOP:
			m_ptSecondImageTex.y = (int)wParam;
			SetEditWndText(m_SecondTop_Edit, m_ptSecondImageTex.y);
			break;
		}
		break;
	case THIRD_IMAGE:
		switch((int)lParam)
		{
		case DRAGBOX_LEFT:
			m_ptThirdImageTex.x = (int)wParam;
			SetEditWndText(m_ThirdLeft_Edit, m_ptThirdImageTex.x);
			break;
		case DRAGBOX_TOP:
			m_ptThirdImageTex.y = (int)wParam;
			SetEditWndText(m_ThirdTop_Edit, m_ptThirdImageTex.y);
			break;
		}
		break;
	}

	//이미지 크기 세팅
	switch((int)lParam)
	{
	case DRAGBOX_WIDTH:
		m_ptImageSize.x = (int)wParam;
		SetEditWndText(m_Image_Width_Edit, m_ptImageSize.x);
		break;
	case DRAGBOX_HEIGHT:
		m_ptImageSize.y = (int)wParam;
		SetEditWndText(m_ImageHeight_Edit, m_ptImageSize.y);
		break;
	}

	m_bChange = true;
	return 0;
}



