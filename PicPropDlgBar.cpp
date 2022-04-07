// PicPropDlgBar.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "UITool.h"
#include "PicPropDlgBar.h"
#include "afxdialogex.h"
#include "ObjMgr.h"
#include "DBMgr.h"


// CPicPropDlgBar 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPicPropDlgBar, CDialogBar)

CPicPropDlgBar::CPicPropDlgBar(CWnd* pParent /*=NULL*/)
{
	m_strTexturePath = "";
	m_ptPosition.x = 0;
	m_ptPosition.y = 0;
	m_ptScaleSize = m_ptPosition;
	m_ptSize = m_ptPosition;
	m_PictureAssetBase = NULL;
	m_bChange = true;
}

CPicPropDlgBar::~CPicPropDlgBar()
{

}

void CPicPropDlgBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	m_TexturePathEdit	= (CEdit*)GetDlgItem(IDC_EDIT_PICTURE_TEXTURE_PATH);
	m_PictureXPosEdit	= (CEdit*)GetDlgItem(IDC_EDIT_PIC_X_POS);
	m_PictureYPosEdit	= (CEdit*)GetDlgItem(IDC_EDIT_PIC_Y_POS);
	m_PictureWidthEdit	= (CEdit*)GetDlgItem(IDC_EDIT_PIC_WIDTH);
	m_PictureHeightEdit	= (CEdit*)GetDlgItem(IDC_EDIT_PIC_HEIGHT);
	m_PictureScaleWidthEdit = (CEdit*)GetDlgItem(IDC_EDIT_PIC_SCALE_WIDTH);
	m_PictureScaleHeightEdit= (CEdit*)GetDlgItem(IDC_EDIT_PIC_SCALE_HEIGHT);

	if(CObjMgr::GetInstance()->GetFocusAssetBase() == NULL)
	{
		m_PictureAssetBase = NULL;
	}
}


BEGIN_MESSAGE_MAP(CPicPropDlgBar, CDialogBar)
	ON_EN_CHANGE(IDC_EDIT_PICTURE_TEXTURE_PATH, &CPicPropDlgBar::OnEnChangeEditPictureTexturePath)
	ON_EN_CHANGE(IDC_EDIT_PIC_X_POS, &CPicPropDlgBar::OnEnChangeEditPicXPos)
	ON_EN_CHANGE(IDC_EDIT_PIC_Y_POS, &CPicPropDlgBar::OnEnChangeEditPicYPos)
	ON_EN_CHANGE(IDC_EDIT_PIC_WIDTH, &CPicPropDlgBar::OnEnChangeEditPicWidth)
	ON_EN_CHANGE(IDC_EDIT_PIC_HEIGHT, &CPicPropDlgBar::OnEnChangeEditPicHeight)
	ON_MESSAGE(WM_SELCHANGE_TEXTURELIST, &CPicPropDlgBar::OnSelchangeTexturelist)
	ON_MESSAGE(WM_CHANGE_DRAGBOX_INFO, &CPicPropDlgBar::OnChangeDragboxInfo)
	ON_EN_CHANGE(IDC_EDIT_PIC_SCALE_WIDTH, &CPicPropDlgBar::OnEnChangeEditPicScaleWidth)
	ON_EN_CHANGE(IDC_EDIT_PIC_SCALE_HEIGHT, &CPicPropDlgBar::OnEnChangeEditPicScaleHeight)
END_MESSAGE_MAP()


// CPicPropDlgBar 메시지 처리기입니다.


void CPicPropDlgBar::OnEnChangeEditPictureTexturePath()
{
	UpdateData(true);
	m_strTexturePath = GetEditTextStdString(m_TexturePathEdit);
}


void CPicPropDlgBar::OnEnChangeEditPicXPos()
{
	UpdateData(true);
	if(m_bChange)
	{
		m_ptPosition.x = GetEditText(m_PictureXPosEdit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptPosition.x, (LPARAM)DRAGBOX_LEFT);
	}
	SetPictureProperty();
}


void CPicPropDlgBar::OnEnChangeEditPicYPos()
{
	UpdateData(true);
	if(m_bChange)
	{
		m_ptPosition.y = GetEditText(m_PictureYPosEdit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptPosition.y, (LPARAM)DRAGBOX_TOP);

	}
	SetPictureProperty();
}


void CPicPropDlgBar::OnEnChangeEditPicWidth()
{
	UpdateData(true);
	if(m_bChange)
	{
		m_ptSize.x = GetEditText(m_PictureWidthEdit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptSize.x, (LPARAM)DRAGBOX_WIDTH);
	}
	SetPictureProperty();
}


void CPicPropDlgBar::OnEnChangeEditPicHeight()
{
	UpdateData(true);
	if(m_bChange)
	{
		m_ptSize.y = GetEditText(m_PictureHeightEdit);
		::SendMessage(m_PicturehWnd, WM_UPDATE_DRAGBOX_INFO, (WPARAM)m_ptSize.y, (LPARAM)DRAGBOX_HEIGHT);
	}
	SetPictureProperty();
}

void CPicPropDlgBar::Initialize()
{
	UpdateData(true);
	m_strTexturePath = "";
	m_ptPosition.x = 0;
	m_ptPosition.y = 0;
	m_ptSize = m_ptPosition;
	m_ptScaleSize = m_ptPosition;

	m_bChange = false;

	m_TexturePathEdit->SetWindowText(L"");
	m_PictureXPosEdit->SetWindowText(L"0");
	m_PictureYPosEdit->SetWindowText(L"0");
	m_PictureWidthEdit->SetWindowText(L"0");
	m_PictureHeightEdit->SetWindowText(L"0");
	m_PictureScaleWidthEdit->SetWindowText(L"0");
	m_PictureScaleHeightEdit->SetWindowText(L"0");

	m_bChange = true;
}

void CPicPropDlgBar::RefreshDlgBar(CAssetBase* _LoadBase, POINT& _NormalTex)
{
	m_bChange = false;

	UpdateData(true);

	SetMemberBase(_LoadBase);

	int iNum = _LoadBase->GetStartPos().x;
	_NormalTex.x = iNum;
	m_ptPosition.x = iNum;
	EditTextSetting(iNum, m_PictureXPosEdit);

	iNum = _LoadBase->GetStartPos().y;
	_NormalTex.y = iNum;
	m_ptPosition.y = iNum;
	EditTextSetting(iNum, m_PictureYPosEdit);

	iNum = _LoadBase->GetFileSize().right;
	m_ptSize.x = iNum;
	EditTextSetting(iNum, m_PictureWidthEdit);

	iNum = _LoadBase->GetFileSize().bottom;
	m_ptSize.y = iNum;
	EditTextSetting(iNum, m_PictureHeightEdit);

	m_strTexturePath = _LoadBase->GetFilePath();
	CString TexPath(m_strTexturePath.c_str());
	m_TexturePathEdit->SetWindowText(TexPath);

	float fScaleX = _LoadBase->GetScaleX();
	float fScaleY = _LoadBase->GetScaleY();

	m_ptScaleSize.x = (float)m_ptSize.x * fScaleX;
	m_ptScaleSize.y = (float)m_ptSize.y * fScaleY;
	EditTextSetting(m_ptScaleSize.x, m_PictureScaleWidthEdit);
	EditTextSetting(m_ptScaleSize.y, m_PictureScaleHeightEdit);

	m_bChange = true;
}

void CPicPropDlgBar::SetParenthWnd(HWND _hWnd)
{
	m_ParenthWnd = _hWnd;
}

void CPicPropDlgBar::CreateAsset()
{
	if(m_PictureAssetBase == NULL)
	{
		m_PictureAssetBase = new CAssetBase;
		RECT rtSize;
		RECT_INITIALIZE(rtSize);
		rtSize.right = m_ptSize.x;
		rtSize.bottom = m_ptSize.y;
		
		POINT ptZero;
		ptZero.x = 0;
		ptZero.y = 0;

		m_PictureAssetBase->SetPosition(ptZero);
		//	에셋의 멤버 변수들을 셋팅해준다
		m_PictureAssetBase->SetAsset(m_strTexturePath, ASSET_PICTURE,
			rtSize, m_ptPosition, 1.0, 1.0);

		//	만들어진 에셋을 Map에 저장하는 과정
		CObjMgr::GetInstance()->InsertAssetBase(m_PictureAssetBase);
	}
}

void CPicPropDlgBar::SetPictureProperty()
{
	if(m_strTexturePath != "")
	{
		CreateAsset();

		RECT rtSize;
		RECT_INITIALIZE(rtSize);
		rtSize.right = m_ptSize.x;
		rtSize.bottom = m_ptSize.y;
		
		//	몇 사이즈로 변경하고자 할때 여기선 실 사이즈를 입력하지만
		//	내부적으로는 scale값을 입력하게 한다.
		if(m_ptScaleSize.x != 0 && m_ptScaleSize.y != 0)
		{
			float fScaleX = 0.f;
			float fScaleY = 0.f;

			fScaleX = (float)m_ptScaleSize.x / m_ptSize.x;
			fScaleY = (float)m_ptScaleSize.y / m_ptSize.y;

			m_PictureAssetBase->SetAsset(m_strTexturePath, ASSET_PICTURE,
				rtSize, m_ptPosition, fScaleX, fScaleY);
		}
		else
		{
			m_PictureAssetBase->SetAsset(m_strTexturePath, ASSET_PICTURE,
				rtSize, m_ptPosition, 1.0, 1.0);
		}
	}
}


afx_msg LRESULT CPicPropDlgBar::OnSelchangeTexturelist(WPARAM wParam, LPARAM lParam)
{
	UpdateData(true);

	m_strTexturePath = (CDBMgr::GetInstance()->GetPathMap()->find((int)wParam)->second);
	CA2T wt(m_strTexturePath.c_str());
	m_TexturePathEdit->SetWindowText(wt);

	if(m_PictureAssetBase != NULL)
	{
		m_PictureAssetBase->SetFilePath(m_strTexturePath);
	}

	return 0;
}


afx_msg LRESULT CPicPropDlgBar::OnChangeDragboxInfo(WPARAM wParam, LPARAM lParam)
{
	m_bChange = false;

	UpdateData(true);

	switch((int)lParam)
	{
	case DRAGBOX_LEFT:
		m_ptPosition.x = (int)wParam;
		SetEditWndText(m_PictureXPosEdit, m_ptPosition.x);
		break;
	case DRAGBOX_TOP:
		m_ptPosition.y = (int)wParam;
		SetEditWndText(m_PictureYPosEdit, m_ptPosition.y);
		break;
	case DRAGBOX_WIDTH:
		m_ptSize.x = (int)wParam;
		SetEditWndText(m_PictureWidthEdit, m_ptSize.x);
		break;
	case DRAGBOX_HEIGHT:
		m_ptSize.y = (int)wParam;
		SetEditWndText(m_PictureHeightEdit, m_ptSize.y);
		break;
	}

	m_bChange = true;

	return 0;
}

void CPicPropDlgBar::SetMemberBase( CAssetBase* _base )
{
	m_PictureAssetBase = _base;
}

void CPicPropDlgBar::OnEnChangeEditPicScaleWidth()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptScaleSize.x = GetEditText(m_PictureScaleWidthEdit);

		float fScaleX = 0.f;
		fScaleX = (float)m_ptScaleSize.x / m_ptSize.x;

		m_bChange = false;
		m_ptScaleSize.y = fScaleX * m_ptSize.y;
		EditTextSetting(m_ptScaleSize.y, m_PictureScaleHeightEdit);
		m_bChange = true;

		SetPictureProperty();
	}
}


void CPicPropDlgBar::OnEnChangeEditPicScaleHeight()
{
	if(m_bChange)
	{
		UpdateData(true);
		m_ptScaleSize.y = GetEditText(m_PictureScaleHeightEdit);

		float fScaleY = 0.f;
		fScaleY = (float)m_ptScaleSize.y / m_ptSize.y;

		m_bChange = false;
		m_ptScaleSize.x = fScaleY * m_ptSize.x;
		EditTextSetting(m_ptScaleSize.x, m_PictureScaleWidthEdit);
		m_bChange = true;
		SetPictureProperty();
	}
}
