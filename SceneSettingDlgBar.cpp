// SceneSettingDlgBar.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "UITool.h"
#include "SceneSettingDlgBar.h"

#include "ObjMgr.h"
#include "BasicAsset.h"
#include "UIFrame.h"


// CSceneSettingDlgBar

IMPLEMENT_DYNAMIC(CSceneSettingDlgBar, CDialogBar)

	CSceneSettingDlgBar::CSceneSettingDlgBar()
{
	m_iSelectDepthListIndex = -1;
	m_eRefreshType = REFRESH_ASSET;
	m_nFocusIndex = -1;
}

CSceneSettingDlgBar::~CSceneSettingDlgBar()
{
	DestroyWindow();
}


BEGIN_MESSAGE_MAP(CSceneSettingDlgBar, CDialogBar)
	ON_WM_CREATE()
	ON_EN_CHANGE(IDC_EDIT_POS_X, &CSceneSettingDlgBar::OnEnChangeEditPosX)
	ON_EN_CHANGE(IDC_EDIT_POS_Y, &CSceneSettingDlgBar::OnEnChangeEditPosY)
	ON_EN_CHANGE(IDC_EDIT_POS_Z, &CSceneSettingDlgBar::OnEnChangeEditPosZ)
	ON_MESSAGE(WM_BASIC_ASSET_FOCUSED, &CSceneSettingDlgBar::OnBasicAssetFocused)
	ON_EN_SETFOCUS(IDC_EDIT_POS_X, &CSceneSettingDlgBar::OnEnSetfocusEditPosX)
	ON_EN_SETFOCUS(IDC_EDIT_POS_Y, &CSceneSettingDlgBar::OnEnSetfocusEditPosY)
	ON_EN_SETFOCUS(IDC_EDIT_POS_Z, &CSceneSettingDlgBar::OnEnSetfocusEditPosZ)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_DEPTH_TOP, &CSceneSettingDlgBar::OnUpdateCmdUI)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_DEPTH_UP, &CSceneSettingDlgBar::OnUpdateCmdUI)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_DEPTH_DOWN, &CSceneSettingDlgBar::OnUpdateCmdUI)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_DEPTH_BOTTOM, &CSceneSettingDlgBar::OnUpdateCmdUI)
	ON_MESSAGE(WM_UIFRAME_FOCUSED, &CSceneSettingDlgBar::OnUiframeFocused)
	ON_CBN_SELCHANGE(IDC_COMBO_SCENE_RESOLUTION, &CSceneSettingDlgBar::OnCbnSelchangeComboSceneResolution)
	ON_LBN_SELCHANGE(IDC_DEPTHSETTING_LIST, &CSceneSettingDlgBar::OnLbnSelchangeDepthsettingList)
	ON_BN_CLICKED(IDC_BUTTON_DEPTH_TOP, &CSceneSettingDlgBar::OnBnClickedButtonDepthTop)
	ON_BN_CLICKED(IDC_BUTTON_DEPTH_BOTTOM, &CSceneSettingDlgBar::OnBnClickedButtonDepthBottom)
	ON_BN_CLICKED(IDC_BUTTON_DEPTH_UP, &CSceneSettingDlgBar::OnBnClickedButtonDepthUp)
	ON_BN_CLICKED(IDC_BUTTON_DEPTH_DOWN, &CSceneSettingDlgBar::OnBnClickedButtonDepthDown)

	ON_MESSAGE(WM_ASSETLIST_DOUBLE_CLICK, &CSceneSettingDlgBar::OnAssetListDoubleClick)
	ON_MESSAGE(WM_FRAMELIST_DOUBLE_CLICK, &CSceneSettingDlgBar::OnFrameListDoubleClick)
	ON_MESSAGE(WM_SCENELIST_DOUBLE_CLICK, &CSceneSettingDlgBar::OnSceneListDoubleClick)
	ON_BN_CLICKED(IDC_CHECK_SCENE_ID_SHOW, &CSceneSettingDlgBar::OnBnClickedCheckSceneIdShow)
	ON_EN_CHANGE(IDC_EDIT_FRAME_SCALE_X, &CSceneSettingDlgBar::OnEnChangeEditFrameScaleX)
	ON_EN_CHANGE(IDC_EDIT_FRAME_SCALE_Y, &CSceneSettingDlgBar::OnEnChangeEditFrameScaleY)
END_MESSAGE_MAP()

// CSceneSettingDlgBar 메시지 처리기입니다.

int CSceneSettingDlgBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pPos_X_Edit = (CEdit*)GetDlgItem(IDC_EDIT_POS_X);
	m_pPos_Y_Edit = (CEdit*)GetDlgItem(IDC_EDIT_POS_Y);
	m_pPos_Z_Edit = (CEdit*)GetDlgItem(IDC_EDIT_POS_Z);

	// 800x600 해상도 설정 Edit [6/4/2019 Gabrielle]
	m_pFrameScaleXEdit = (CEdit*)GetDlgItem(IDC_EDIT_FRAME_SCALE_X);
	m_pFrameScaleYEdit = (CEdit*)GetDlgItem(IDC_EDIT_FRAME_SCALE_Y);

	m_pResolutionCombobox = (CComboBox*)GetDlgItem(IDC_COMBO_SCENE_RESOLUTION);
	m_pDepthSettingListBox = (CListBox*)GetDlgItem(IDC_DEPTHSETTING_LIST);

	m_bEditFocused = false;

	m_strPosX = "0";
	m_strPosY = "0";
	m_strPosZ = "1";

	m_strScaleX = "0";
	m_strScaleY = "0";

	m_strResolution[RESOLUTION_1024_768] = "1024 * 768";
	m_strResolution[RESOLUTION_800_600] = "800 * 600";

	m_fScaleRatio = 0.f;
	m_bScaleChange = false;
	return 0;
}


void CSceneSettingDlgBar::OnEnChangeEditPosX()
{
	if( m_bEditFocused == false )
		return;

	AdaptedChangedPosEdit();
}


void CSceneSettingDlgBar::OnEnChangeEditPosY()
{
	if( m_bEditFocused == false )
		return;

	AdaptedChangedPosEdit();
}


void CSceneSettingDlgBar::OnEnChangeEditPosZ()
{
	if( m_bEditFocused == false )
		return;

	m_pPos_Z_Edit = (CEdit*)GetDlgItem(IDC_EDIT_POS_Z);
}

void CSceneSettingDlgBar::AdaptedChangedPosEdit()
{
	POINT	ptMoveDist;

	if(m_bEditFocused)
	{
		m_pPos_X_Edit = (CEdit*)GetDlgItem(IDC_EDIT_POS_X);
		m_pPos_Y_Edit = (CEdit*)GetDlgItem(IDC_EDIT_POS_Y);

		m_pPos_X_Edit->GetWindowText(m_strPosX);
		m_pPos_Y_Edit->GetWindowText(m_strPosY);

		CT2CA pszConvertedX (m_strPosX);
		std::string _strConvertX(pszConvertedX);

		CT2CA pszConvertedY (m_strPosY);
		std::string _strConvertY(pszConvertedY);

		ptMoveDist.x = atoi(_strConvertX.c_str());
		ptMoveDist.y = atoi(_strConvertY.c_str());
	}

	//	선택된 에셋이 없다고 하면 포지션을 옮길 아이도 없으므로 하지 않는다.
	if(CObjMgr::GetInstance()->GetFocusBasicAsset() != NULL && m_bEditFocused)
	{
		CObjMgr::GetInstance()->GetFocusBasicAsset()->CalculatePosition(ptMoveDist);
	}

	if(CObjMgr::GetInstance()->GetFocusUIFrame() != NULL && m_bEditFocused)
	{
		POINT ptPosition;
		ptPosition = CObjMgr::GetInstance()->GetFocusUIFrame()->GetPosition();

		POINT ptMovingAmount;
		ptMovingAmount.x = 0;
		ptMovingAmount.y = 0;

		ptMovingAmount.x = ptPosition.x - ptMoveDist.x;
		ptMovingAmount.y = ptPosition.y - ptMoveDist.y;

		CObjMgr::GetInstance()->GetFocusUIFrame()->SubstractPosition(ptMovingAmount);
	}
}

afx_msg LRESULT CSceneSettingDlgBar::OnBasicAssetFocused(WPARAM wParam, LPARAM lParam)
{
	m_bEditFocused = false;
	RefreshPosEdit(REFRESH_ASSET);

	if(m_nFocusIndex != CObjMgr::GetInstance()->GetFocusBasicAssetIndex())
	{
		m_nFocusIndex = CObjMgr::GetInstance()->GetFocusBasicAssetIndex();
		SelectDepthListIndex(m_nFocusIndex);
	}

	return 0;
}

void CSceneSettingDlgBar::OnEnSetfocusEditPosX()
{
	m_bEditFocused = true;
}


void CSceneSettingDlgBar::OnEnSetfocusEditPosY()
{
	m_bEditFocused = true;
}


void CSceneSettingDlgBar::OnEnSetfocusEditPosZ()
{
	m_bEditFocused = true;
}

void CSceneSettingDlgBar::RefreshPosEdit(eRefreshType _type)
{
	// 현재 포커싱되어있는 프레임과 에셋 [6/4/2019 Gabrielle]
	CUIFrame* pFocusFrame = CObjMgr::GetInstance()->GetFocusUIFrame();
	CBasicAsset* pBasicAsset = CObjMgr::GetInstance()->GetFocusBasicAsset();

	if(CObjMgr::GetInstance()->GetTargetMapCount(SCENE_EDITOR_TARGET) == 0
		&& CObjMgr::GetInstance()->GetTargetMapCount(SCENE_EDITOR_FRAME_TARGET) == 0)
	{
		return;
	}
	CString strResult;
	char szITOA[20];

	m_pPos_X_Edit = (CEdit*)GetDlgItem(IDC_EDIT_POS_X);
	m_pPos_Y_Edit = (CEdit*)GetDlgItem(IDC_EDIT_POS_Y);

	// Edit 핸들 할당 [6/4/2019 Gabrielle]
	m_pFrameScaleXEdit = (CEdit*)GetDlgItem(IDC_EDIT_FRAME_SCALE_X);
	m_pFrameScaleYEdit = (CEdit*)GetDlgItem(IDC_EDIT_FRAME_SCALE_Y);

	POINT ptPos;
	POINT ptSize;

	switch(_type)
	{
	case REFRESH_FRAME:
		{
			if(pFocusFrame == NULL)
			{
				return;
			}

			ptPos = pFocusFrame->GetPosition();
			ptSize = pFocusFrame->GetOriginSize();
			ptSize.x *= pFocusFrame->GetScaleX();
			ptSize.y *= pFocusFrame->GetScaleY();
			m_fScaleRatio = ptSize.x / (float)ptSize.y;
		}
		break;
	case REFRESH_ASSET:
		{
			if(pBasicAsset == NULL)
			{
				return;
			}

			ptPos = pBasicAsset->GetPosition();
		}
		break;
	}

	_itoa_s(ptPos.x, szITOA, 10);
	std::string szResult = szITOA;
	strResult = szResult.c_str();
	m_pPos_X_Edit->SetWindowText(strResult);

	_itoa_s(ptPos.y, szITOA, 10);
	szResult = szITOA;
	strResult = szResult.c_str();
	m_pPos_Y_Edit->SetWindowText(strResult);


	// Scale에 대한 설정 [6/4/2019 Gabrielle]
	m_bScaleChange = true;
	_itoa_s(ptSize.x, szITOA, 10);
	szResult = szITOA;
	strResult = szResult.c_str();
	m_strScaleX = strResult;
	m_pFrameScaleXEdit->SetWindowText(strResult);

	_itoa_s(ptSize.y, szITOA, 10);
	szResult = szITOA;
	strResult = szResult.c_str();
	m_strScaleY = strResult;
	m_pFrameScaleYEdit->SetWindowText(strResult);
	m_bScaleChange = false;
}

void CSceneSettingDlgBar::UpdateEdit()
{
	m_pPos_X_Edit = (CEdit*)GetDlgItem(IDC_EDIT_POS_X);
	m_pPos_Y_Edit = (CEdit*)GetDlgItem(IDC_EDIT_POS_Y);
	m_pPos_Z_Edit = (CEdit*)GetDlgItem(IDC_EDIT_POS_Z);

	m_pResolutionCombobox = (CComboBox*)GetDlgItem(IDC_COMBO_SCENE_RESOLUTION);

	m_pPos_X_Edit->SetWindowText(m_strPosX);
	m_pPos_Y_Edit->SetWindowText(m_strPosY);
	m_pPos_Z_Edit->SetWindowText(m_strPosZ);

	for(int i = m_pResolutionCombobox->GetCount(); i >= 0; i--)
	{
		m_pResolutionCombobox->DeleteString(i);
	}

	for(int i = 0; i < RESOLUTION_END; i++)
	{
		m_pResolutionCombobox->AddString(m_strResolution[i]);
	}

	m_pResolutionCombobox->SetCurSel(0);
}

void CSceneSettingDlgBar::OnUpdateCmdUI( CCmdUI* pCmdUI )
{
	pCmdUI->Enable(TRUE);
}

afx_msg LRESULT CSceneSettingDlgBar::OnUiframeFocused(WPARAM wParam, LPARAM lParam)
{
	m_bEditFocused = false;
	RefreshPosEdit(REFRESH_FRAME);	

	if(m_nFocusIndex != CObjMgr::GetInstance()->GetFocusUIFrameIndex())
	{
		m_nFocusIndex = CObjMgr::GetInstance()->GetFocusUIFrameIndex();
		SelectDepthListIndex(m_nFocusIndex);
	}

	return 0;
}

afx_msg LRESULT CSceneSettingDlgBar::OnAssetListDoubleClick(WPARAM wParam, LPARAM lParam)
{
	m_eRefreshType = REFRESH_ASSET;
	RefreshDepthSettingList(-1);
	return 0;
}

afx_msg LRESULT CSceneSettingDlgBar::OnFrameListDoubleClick(WPARAM wParam, LPARAM lParam)
{
	m_eRefreshType = REFRESH_FRAME;
	RefreshDepthSettingList(-1);
	return 0;
}

afx_msg LRESULT CSceneSettingDlgBar::OnSceneListDoubleClick(WPARAM wParam, LPARAM lParam)
{
	m_eRefreshType = REFRESH_FRAME;
	RefreshDepthSettingList(-1);
	return 0;
}

void CSceneSettingDlgBar::OnCbnSelchangeComboSceneResolution()
{
	//	해상도가 선택되었을 경우 취할 액션을 여기에 서술한다.
	m_pResolutionCombobox = (CComboBox*)GetDlgItem(IDC_COMBO_SCENE_RESOLUTION);

	int iCurSel = m_pResolutionCombobox->GetCurSel();

	// 해상도 설정 800*600 작업 [6/3/2019 Gabrielle]
	if(iCurSel == RESOLUTION_1024_768)
	{
		CObjMgr::GetInstance()->Setb800x600(false);
		::SendMessage(m_ParenthWnd, WM_CHANGE_RESOLUTION, WND_WIDTH, WND_HEIGHT);
	}
	else if(iCurSel == RESOLUTION_800_600)
	{
		CObjMgr::GetInstance()->Setb800x600(true);
		::SendMessage(m_ParenthWnd, WM_CHANGE_RESOLUTION, WND_SMALL_WIDTH, WND_SMALL_HEIGHT);
	}
}

//뎁스 세팅 리스트 갱신
void CSceneSettingDlgBar::RefreshDepthSettingList(int nIndex)
{
	m_pDepthSettingListBox = (CListBox*)GetDlgItem(IDC_DEPTHSETTING_LIST);	

	//리스트에 있는 항목을 비워준다.
	if(m_pDepthSettingListBox->GetCount() != 0)
	{
		for(int i = m_pDepthSettingListBox->GetCount(); i >= 0; i--)
		{
			m_pDepthSettingListBox->DeleteString(i);
		}
	}	

	//리스트 박스에 정보 보여주기
	if(m_eRefreshType == REFRESH_ASSET)
	{
		std::vector<CBasicAsset*> basicAssetList = CObjMgr::GetInstance()->GetBasicAssetList();
		if(basicAssetList.size() != 0)
		{
			for(int i=0; i<basicAssetList.size(); ++i)
			{
				CBasicAsset* pAsset = basicAssetList[i];
				if(pAsset != NULL)
				{
					CA2T wt(pAsset->GetFileName().c_str());
					m_pDepthSettingListBox->AddString(wt);
				}			
			}
		}
	}
	else if(m_eRefreshType == REFRESH_FRAME)
	{
		std::vector<CUIFrame*> frameList = CObjMgr::GetInstance()->GetUIFrameList();
		if(frameList.size() != 0)
		{
			for(int i=0; i<frameList.size(); ++i)
			{
				CUIFrame* pFrame = frameList[i];
				if(pFrame != NULL)
				{
					CA2T wt(pFrame->GetFrameFileName().c_str());
					m_pDepthSettingListBox->AddString(wt);
				}			
			}
		}
	}

	SelectDepthListIndex(nIndex);	
}

//뎁스 옮기는 버튼 눌렀을 경우 리스트 포커스가 날라가서 강제로 포커스해줌.
void CSceneSettingDlgBar::SelectDepthListIndex(int nSelectIndex)
{
	if(nSelectIndex != -1)
	{
		m_pDepthSettingListBox = (CListBox*)GetDlgItem(IDC_DEPTHSETTING_LIST);
		m_iSelectDepthListIndex = nSelectIndex;
		m_pDepthSettingListBox->SetCurSel(nSelectIndex);		
	}
}

//뎁스세팅 리스트
void CSceneSettingDlgBar::OnLbnSelchangeDepthsettingList()
{
	m_pDepthSettingListBox = (CListBox*)GetDlgItem(IDC_DEPTHSETTING_LIST);
	m_iSelectDepthListIndex = m_pDepthSettingListBox->GetCurSel();	

	//뎁스 세팅 리스트에서 클릭시 왼쪽 하단에 위치값 적용
	if(m_eRefreshType == REFRESH_ASSET)
	{
		CObjMgr::GetInstance()->SetFocusAssetIndex(m_iSelectDepthListIndex);
		RefreshPosEdit(REFRESH_ASSET);
	}
	else if(m_eRefreshType == REFRESH_FRAME)
	{
		CObjMgr::GetInstance()->SetFocusFrameIndex(m_iSelectDepthListIndex);
		RefreshPosEdit(REFRESH_FRAME);
	}
}

//뎁스세팅(최상위)
void CSceneSettingDlgBar::OnBnClickedButtonDepthTop()
{
	if(m_eRefreshType == REFRESH_ASSET)
		CObjMgr::GetInstance()->ChangeBasicAssetListDepth(m_iSelectDepthListIndex, 0);
	else if(m_eRefreshType == REFRESH_FRAME)
		CObjMgr::GetInstance()->ChangeFrameListDepth(m_iSelectDepthListIndex, 0);

	RefreshDepthSettingList(0);
}

//뎁스세팅(최하위)
void CSceneSettingDlgBar::OnBnClickedButtonDepthBottom()
{
	m_pDepthSettingListBox = (CListBox*)GetDlgItem(IDC_DEPTHSETTING_LIST);
	if(m_eRefreshType == REFRESH_ASSET)
		CObjMgr::GetInstance()->ChangeBasicAssetListDepth(m_iSelectDepthListIndex, m_pDepthSettingListBox->GetCount()-1);
	else if(m_eRefreshType == REFRESH_FRAME)
		CObjMgr::GetInstance()->ChangeFrameListDepth(m_iSelectDepthListIndex, m_pDepthSettingListBox->GetCount()-1);

	RefreshDepthSettingList(m_pDepthSettingListBox->GetCount()-1);
}

//뎁스세팅(한칸위)
void CSceneSettingDlgBar::OnBnClickedButtonDepthUp()
{
	int nChangePos = m_iSelectDepthListIndex-1;
	if(nChangePos <= 0)
		nChangePos = 0;

	if(m_eRefreshType == REFRESH_ASSET)
		CObjMgr::GetInstance()->ChangeBasicAssetListDepth(m_iSelectDepthListIndex, nChangePos);
	else if(m_eRefreshType == REFRESH_FRAME)
		CObjMgr::GetInstance()->ChangeFrameListDepth(m_iSelectDepthListIndex, nChangePos);

	RefreshDepthSettingList(nChangePos);
}

//뎁스세팅(한칸아래)
void CSceneSettingDlgBar::OnBnClickedButtonDepthDown()
{
	m_pDepthSettingListBox = (CListBox*)GetDlgItem(IDC_DEPTHSETTING_LIST);
	int nChangePos = m_iSelectDepthListIndex+1;
	if(nChangePos >= m_pDepthSettingListBox->GetCount()-1)
		nChangePos = m_pDepthSettingListBox->GetCount()-1;

	if(m_eRefreshType == REFRESH_ASSET)
		CObjMgr::GetInstance()->ChangeBasicAssetListDepth(m_iSelectDepthListIndex, nChangePos);
	else if(m_eRefreshType == REFRESH_FRAME)
		CObjMgr::GetInstance()->ChangeFrameListDepth(m_iSelectDepthListIndex, nChangePos);

	RefreshDepthSettingList(nChangePos);
}


void CSceneSettingDlgBar::OnBnClickedCheckSceneIdShow()
{
	CButton* IDShowCheckBox = (CButton*)GetDlgItem(IDC_CHECK_SCENE_ID_SHOW);

	if(IDShowCheckBox->GetCheck() == BST_CHECKED)
	{
		CObjMgr::GetInstance()->ShowObjectID();
	}
	else
	{
		CObjMgr::GetInstance()->HideObjectID();
	}
}

// 프레임의 크기를 설정하고자 할때. [6/4/2019 Gabrielle]
void CSceneSettingDlgBar::OnEnChangeEditFrameScaleX()
{
	CUIFrame* pFocusFrame = CObjMgr::GetInstance()->GetFocusUIFrame();

	if(pFocusFrame == NULL || m_bScaleChange)
	{
		return;
	}

	m_pFrameScaleXEdit = (CEdit*)GetDlgItem(IDC_EDIT_FRAME_SCALE_X);
	m_pFrameScaleYEdit = (CEdit*)GetDlgItem(IDC_EDIT_FRAME_SCALE_Y);

	m_pFrameScaleXEdit->GetWindowText(m_strScaleX);
	CT2CA pszConvertedX (m_strScaleX);
	std::string _strConvertX(pszConvertedX);

	float fScaleXSize = atof(_strConvertX.c_str());
	float fScaleYSize = fScaleXSize / m_fScaleRatio;

	char szScaleY[20] = {0,};
	sprintf(szScaleY, "%g", fScaleYSize);
	m_strScaleY = szScaleY;

	m_bScaleChange = true;
	m_pFrameScaleYEdit->SetWindowText(m_strScaleY);
	pFocusFrame->SetScale(atoi(_strConvertX.c_str()) / (float)pFocusFrame->GetOriginSize().x
		, atoi(szScaleY) / (float)pFocusFrame->GetOriginSize().y);
	//pFocusFrame->SetScale(fScaleXSize / pFocusFrame->GetOriginSize().x, fScaleYSize / pFocusFrame->GetOriginSize().y);
	m_bScaleChange = false;


}

void CSceneSettingDlgBar::OnEnChangeEditFrameScaleY()
{
	CUIFrame* pFocusFrame = CObjMgr::GetInstance()->GetFocusUIFrame();

	if(pFocusFrame == NULL || m_bScaleChange)
	{
		return;
	}

	m_pFrameScaleXEdit = (CEdit*)GetDlgItem(IDC_EDIT_FRAME_SCALE_X);
	m_pFrameScaleYEdit = (CEdit*)GetDlgItem(IDC_EDIT_FRAME_SCALE_Y);

	m_pFrameScaleYEdit->GetWindowText(m_strScaleY);
	CT2CA pszConvertedY (m_strScaleY);
	std::string _strConvertY(pszConvertedY);

	float fScaleYSize = atof(_strConvertY.c_str());
	float fScaleXSize = fScaleYSize * m_fScaleRatio;

	char szScaleX[20] = {0,};
	sprintf(szScaleX, "%g", fScaleXSize);
	m_strScaleX = szScaleX;

	m_bScaleChange = true;
	m_pFrameScaleXEdit->SetWindowText(m_strScaleX);

	pFocusFrame->SetScale(atoi(szScaleX) / (float)pFocusFrame->GetOriginSize().x
		, atoi(_strConvertY.c_str()) / (float)pFocusFrame->GetOriginSize().y);

	//pFocusFrame->SetScale(fScaleXSize / pFocusFrame->GetOriginSize().x, fScaleYSize / pFocusFrame->GetOriginSize().y);
	m_bScaleChange = false;
}