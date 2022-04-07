#include "StdAfx.h"
#include "ObjMgr.h"

#include "BasicAsset.h"
#include "BaseInterface.h"
#include "AssetBase.h"
#include "UIFrame.h"
#include "DBMgr.h"
#include "TextBox.h"
#include "BtnBase.h"
#include "CheckBoxBase.h"
#include "GroupButton.h"
#include "InputText.h"
#include "UIScene.h"

IMPLEMENT_SINGLETON(CObjMgr);

CObjMgr::CObjMgr(void)
{
	m_iFocusBasicAsset = NOTHING_IN_HERE;
	m_iFocusAssetBase = NOTHING_IN_HERE;
	m_iFocusUIFrame = NOTHING_IN_HERE;

	m_BackgroundBase = NULL;
	m_BlurImageBase = NULL;

	m_strAtt[ASSET_TEXTBOX]		= "TXT";
	m_strAtt[ASSET_PICTURE]		= "PIC";
	m_strAtt[ASSET_LISTBOX]		= "LST";
	m_strAtt[ASSET_COMBOBOX]	= "CMB";
	m_strAtt[ASSET_SLIDER]		= "SLD";
	m_strAtt[ASSET_BUTTON]		= "BTN";
	m_strAtt[ASSET_CHECKBOX]	= "CHK";
	m_strAtt[ASSET_RADIOBUTTON]	= "RAD";
	m_strAtt[ASSET_INPUTTEXT]	= "INP";
	m_strAtt[ASSET_SCROLL]		= "SCR";
	m_strAtt[ASSET_LISTITEM]	= "ITM";
	m_strAtt[ASSET_PROGRESSBAR]	= "PRG";
	m_strAtt[ASSET_IMAGEANIMATION]	= "IMG";
	m_strAtt[ASSET_TABBUTTON]	= "TAB";
	m_strAtt[ASSET_THIRDGRID]	= "GRI";


	m_strSaveEditAssetFileName = "";
	m_strSaveSceneFileName = "";
	m_strSaveFrameFileName = "";
	m_bShowID = false;
	m_iCurDlgBarIndex = -1;
	m_b800x600 = false;
}


CObjMgr::~CObjMgr(void)
{
	ClearMap();
	ClearFrameMap();
	ClearSceneMap();
}

void CObjMgr::Run(eAssetTarget _target)
{
	if(_target == ASSET_EDITOR_TARGET)
	{
		VEC_INTERFACE_ITER iter_begin;
		VEC_INTERFACE_ITER iter_end;

		iter_begin = m_vecBaseInterface.begin();
		iter_end = m_vecBaseInterface.end();

		for(iter_begin; iter_begin != iter_end; iter_begin++)
		{
			((CBaseInterface*)*iter_begin)->Run();
		}
	}
	else if(_target == SCENE_EDITOR_TARGET)
	{
		VEC_BASICASSET_ITER iter_begin;
		VEC_BASICASSET_ITER iter_end;

		iter_begin = m_vecBasicAsset.begin();
		iter_end = m_vecBasicAsset.end();

		for(iter_begin; iter_begin != iter_end; iter_begin++)
		{
			((CBasicAsset*)*iter_begin)->Run();
		}

		VEC_FRAME_ITER iter_frame_begin;
		VEC_FRAME_ITER iter_frame_end;

		iter_frame_begin = m_vecUIFrame.begin();
		iter_frame_end = m_vecUIFrame.end();

		for(iter_frame_begin; iter_frame_begin != iter_frame_end; iter_frame_begin++)
		{
			((CUIFrame*)*iter_frame_begin)->Run();
		}

		//std::cout << m_vecUIFrame.size() << std::endl;
	}
}

void CObjMgr::Render(eAssetTarget _target, HDC _DC)
{
	if(_target == ASSET_EDITOR_TARGET)
	{
		//	에셋 에디터에서만 그리게 하는 부분들을 처리해주기 위하여.
		VEC_INTERFACE_ITER iter_begin;
		VEC_INTERFACE_ITER iter_end;

		iter_begin = m_vecBaseInterface.begin();
		iter_end = m_vecBaseInterface.end();

		for(iter_begin; iter_begin != iter_end; iter_begin++)
		{
			((CBaseInterface*)*iter_begin)->Render(_DC, _target);
		}
	}
	else if(_target == SCENE_EDITOR_TARGET)
	{
		if(m_BackgroundBase != NULL)
		{
			m_BackgroundBase->Render(_DC, _target);
		}

		if(m_BlurImageBase != NULL)
			m_BlurImageBase->Render(_DC, _target);

		//에셋 인터페이스 렌더
		int nAssetCount = m_vecBasicAsset.size();
		for(int i = 0; i < nAssetCount; i++)
		{
			m_vecBasicAsset[i]->Render(_DC, _target);
			RECT * rtFocusHitBox = m_vecBasicAsset[i]->GetHitBox();	
			if(m_bShowID)
			{
				SetTextColor(_DC, RGB(255, 255, 255));
				DrawID(_DC, i, m_vecBasicAsset[i]->GetPosition(), *rtFocusHitBox, UIASSET_INDEX_RGB);
			}			

			//선택되어진 에셋에 빨간색 테두리 만들어줌.
			if(m_iFocusBasicAsset == i)
			{
				HPEN CustomPen, OldPen;
				HBRUSH myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
				HBRUSH oldBrush = (HBRUSH)SelectObject(_DC, myBrush);
				CustomPen = CreatePen(PS_SOLID, 2, RGB(255,0,0));
				OldPen = (HPEN)SelectObject(_DC,CustomPen);
				Rectangle(_DC, rtFocusHitBox->left , rtFocusHitBox->top, rtFocusHitBox->right, rtFocusHitBox->bottom);
				SelectObject(_DC, OldPen);
				SelectObject(_DC, oldBrush);
				DeleteObject(myBrush);
				DeleteObject(CustomPen);
			}			
		}

		//프레임 인터페이스 렌더
		int nFrameCount = m_vecUIFrame.size();
		for(int j = 0; j < nFrameCount; j++)
		{
			m_vecUIFrame[j]->Render(_DC, _target, m_bShowID);
			RECT rtFramHitBox = m_vecUIFrame[j]->GetHitBox();
			if(m_bShowID)
			{				
				POINT ptPos;
				ptPos.x = m_vecUIFrame[j]->GetPosition().x;
				ptPos.y = m_vecUIFrame[j]->GetPosition().y - DRAW_ID_HEIGHT;

				RECT ptBoundary;
				RECT_COPY(ptBoundary, rtFramHitBox);
				ptBoundary.left = ptPos.x - 5;
				ptBoundary.top = ptPos.y - 5;
				ptBoundary.right += DRAW_BOUNDARY_INDENT;
				ptBoundary.bottom += DRAW_BOUNDARY_INDENT;
				SetTextColor(_DC, RGB(255, 0, 0));
				DrawID(_DC, j, ptPos, ptBoundary, UIFRAME_INDEX_RGB);
			}

			//선택되어진 프레임에 빨간색 테두리 만들어줌.
			if( j == m_iFocusUIFrame)
			{
				HPEN CustomPen, OldPen;
				HBRUSH myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
				HBRUSH oldBrush = (HBRUSH)SelectObject(_DC, myBrush);
				CustomPen = CreatePen(PS_SOLID, 2, RGB(255,0,0));
				OldPen = (HPEN)SelectObject(_DC,CustomPen);
				Rectangle(_DC, m_vecUIFrame[j]->GetPosition().x , m_vecUIFrame[j]->GetPosition().y
					, m_vecUIFrame[j]->GetPosition().x + m_vecUIFrame[j]->GetOriginSize().x * m_vecUIFrame[j]->GetScaleX()
					, m_vecUIFrame[j]->GetPosition().y + m_vecUIFrame[j]->GetOriginSize().y * m_vecUIFrame[j]->GetScaleY());
// 				Rectangle(_DC, rtFramHitBox.left , rtFramHitBox.top
// 					, rtFramHitBox.right+5, rtFramHitBox.bottom +5);
				SelectObject(_DC, OldPen);
				SelectObject(_DC, oldBrush);
				DeleteObject(myBrush);
				DeleteObject(CustomPen);
			}			
		}
	}
}

void CObjMgr::SaveAsset( std::string FilePath )
{
	//	MFC 는 파일입출력을 실행할때 MBCS로 데이터를 저장한다.
	//	String을 저장하기 위해서는 별도의 방법이 필요하다.
	//	따라서 TEXT방식과 BINARY방식을 혼용하였다.
	//	이는 LOAD함수도 마찬가지이다.
	std::ofstream	ofSave(FilePath.c_str());

	int iVecCount = m_vecBaseInterface.size();
	POINT ptMinPos = CalculateBaseMinPos();

	for(int i = 0; i < iVecCount; i++)
	{
		if(m_vecBaseInterface[i]->GetAssetType() != ASSET_TEXTBOX)
		{
			m_vecBaseInterface[i]->ConvertFilePath();
		}
	}

	for(int i = 0; i < iVecCount; i++)
	{
		ofSave << "[START]" << std::endl;

		if(m_vecBaseInterface[i]->GetAssetType() == ASSET_LISTITEM)
		{
			CListItemBase* pListItemBase = ((CListItemBase*)m_vecBaseInterface[i]);
			pListItemBase->SetFocusItemIndex(NOTHING_IN_HERE);
			ptMinPos = pListItemBase->CalculateAssetMinPos();
		}

		m_vecBaseInterface[i]->SubstractPosition(ptMinPos);
		m_vecBaseInterface[i]->Save(ofSave);
		ofSave << "[END]\t" << std::endl;
	}

	ofSave.close();
}

void CObjMgr::SaveFrame( std::string FilePath )
{
	std::ofstream	ofSave(FilePath.c_str());
	VEC_BASICASSET_ITER iter_begin;
	VEC_BASICASSET_ITER iter_end;

	iter_begin = m_vecBasicAsset.begin();
	iter_end = m_vecBasicAsset.end();

	for(iter_begin; iter_begin != iter_end; iter_begin++)
	{
		((CBasicAsset*)(*iter_begin))->ConvertFilePath();
	}

	POINT ptMinPos = CalculateAssetMinPos();

	//  [1/17/2019 Gabrielle]
	// 여기서 좌표값을 저장해주고 프레임을 수정할때는 이 좌표값까지 불러오게 한다.
	ofSave << "[REVISE_FRAME]" << std::endl;
	ofSave << ptMinPos.x << "," << ptMinPos.y << std::endl;
	ofSave << "[REVISE_FRAME_END]" << std::endl;
	//ptMinPos

	iter_begin = m_vecBasicAsset.begin();

	////	마이너스 좌표가 left,top의 최저인 경우는 스크린좌표를 그대로 저장하게한다.
	//if(ptMinPos.x < 0 || ptMinPos.y < 0)
	//{
	//	ofSave << "[START]" << std::endl;
	//	((CBasicAsset*)(*iter_begin))->Save(ofSave);
	//	ofSave << "[END]\t" << std::endl;
	//	iter_begin++;
	//}

	for(iter_begin; iter_begin != iter_end; iter_begin++)
	{
		ofSave << "[START]" << std::endl;
		((CBasicAsset*)(*iter_begin))->Save(ofSave, ptMinPos.x, ptMinPos.y);
		ofSave << "[END]\t" << std::endl;
	}

	iter_begin = m_vecBasicAsset.begin();

	for(iter_begin; iter_begin != iter_end; iter_begin++)
	{
		((CBasicAsset*)(*iter_begin))->MakeOriginalFilePath();
	}

	ofSave.close();
}

void CObjMgr::LoadAsset( std::string FilePath, HWND _hWnd )
{	
	std::ifstream	ifLoad(FilePath.c_str());
	std::string		strInput;	

	CBaseInterface* newAsset = NULL;

	while(!ifLoad.eof())
	{
		//	복수의 입력을 위한 InsertAsset과 SetFilePath의 배치
		//	로드 에셋 데이터
		std::getline(ifLoad, strInput, '\n');
		if(!strcmp(strInput.c_str(), "[END]\t"))
		{
			return;
		}

		if(!strcmp(strInput.c_str(), "[START]"))
		{
			std::getline(ifLoad, strInput, '\t');
			std::getline(ifLoad, strInput, '\n');
			strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());

			eAssetType LoadAssetType = (eAssetType)atoi(strInput.c_str());
			newAsset = MakeAdaptableBase(LoadAssetType);
			newAsset->SetAssetType(LoadAssetType);
			::SendMessage(_hWnd, WM_CHANGE_ATTCOMBOBOX, (WPARAM)newAsset, NULL);
		}

		if(newAsset != NULL)
		{
			newAsset->Load(ifLoad);
			newAsset->SetParenthWnd(_hWnd);
			InsertAssetBase(newAsset);
			//	먼저 에셋 속성값으로 combobox를 바꿔준다음 하자.
			::SendMessage(_hWnd, WM_REFRESH_DLGBAR_INFO, (WPARAM)newAsset, NULL);
		}
	}

	ifLoad.close();
}

void CObjMgr::LoadFrame( std::string FilePath, HWND _hWnd )	//	여기에도 hWnd가 들어가야한다.
{
	std::ifstream	ifLoad(FilePath.c_str());
	std::string		strInput;	

	CUIFrame*		newFrame		= new CUIFrame;
	CBasicAsset*	newBasicAsset	= NULL;
	POINT ptFramePos;
	ptFramePos.x = 0;
	ptFramePos.y = 0;

	if(ifLoad.good())
	{
		while(!ifLoad.eof())
		{
			std::getline(ifLoad, strInput, '\n');

			if(!strcmp(strInput.c_str(),"[REVISE_FRAME]"))
			{
				std::getline(ifLoad, strInput, ',');
				ptFramePos.x = atoi(strInput.c_str());
				std::getline(ifLoad, strInput, '\n');
				ptFramePos.y = atoi(strInput.c_str());
			}

			if(!strcmp(strInput.c_str(), "[START]"))
			{
				newBasicAsset = new CBasicAsset;
				newBasicAsset->Load(ifLoad);
				newBasicAsset->MakeOriginalFilePath();
				newBasicAsset->Run();
				newFrame->InsertBasicAsset(newBasicAsset);
			}
		}
		std::string FramePath = FilePath;
		newFrame->SetFrameFilePath(FramePath);
		InsertUIFrame(newFrame);
	}

	ifLoad.close();
}

bool CObjMgr::SaveAssetByFileDlg()
{
	if(m_vecBaseInterface.empty())
	{
		AfxMessageBox(TEXT("저장할 데이터가 존재하지 않습니다."));
		return false;
	}

	// 해당 이름으로 에셋을 저장한다.
	CString strPathName;
	std::string strRai = "rai";
	CA2T wt(strRai.c_str());
	CA2T saveName(m_strSaveEditAssetFileName.c_str());

	//	 하단 함수의 두번째 인자에 wt가 들어갔었다.
	CFileDialog FileDlg(FALSE, NULL, saveName);

	TCHAR strInitPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, strInitPath);
	// 패스 변경 : Release 
#if defined _DEBUG || defined RELEASE_DEV_PATH
	_tcscat_s(strInitPath, L"\\Asset");
#else
		_tcscat_s(strInitPath, ASSET_FOLDER_PATH);
#endif
	FileDlg.m_ofn.lpstrInitialDir = strInitPath;

	int iResult = FileDlg.DoModal();
	if(iResult == IDOK)
	{
		bool bExist = false;
		strPathName = FileDlg.GetPathName();
		CT2CA pszConvertedAnsiString (strPathName);
		std::string _Path(pszConvertedAnsiString);
		CA2T wt(_Path.c_str());
		//	경로상에 이미 이 이름을 가진 파일이 있을 때에는 덮어써야한다.
		bExist = PathFileExists(wt);
		if(!bExist)
		{
			//	중복되는 이름을 가진 아이가 없으므로, 새로 만드는거니까 앞에 시그니쳐를 붙인다
			EditAssetFileName(_Path);
		}	
		SaveAsset(_Path);
		m_strSaveEditAssetFileName = "";
		return true;
	}
	else if(iResult == IDCANCEL)
	{
		return false;
	}
}

void CObjMgr::LoadAssetByFileDlg(HWND _hWnd)
{
	CString strPathName;
	std::string strRai = "rai";
	CA2T wt(strRai.c_str());
	CFileDialog FileDlg(TRUE, wt, NULL);

	TCHAR strInitPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, strInitPath);

	// 패스 변경 : Release 
#if defined _DEBUG || defined RELEASE_DEV_PATH
	_tcscat_s(strInitPath, L"\\Asset");
#else
	_tcscat_s(strInitPath, ASSET_FOLDER_PATH);
#endif
	FileDlg.m_ofn.lpstrInitialDir = strInitPath;

	int iResult = FileDlg.DoModal();
	CBaseInterface* newBase = NULL;

	if(IDOK == iResult)
	{
		//	현재 떠있는 Asset들을 클리어하는 함수.
		ClearTargetMap(ASSET_EDITOR_TARGET);

		strPathName = FileDlg.GetPathName();
		CT2CA pszConvertedAnsiString (strPathName);
		std::string _Path(pszConvertedAnsiString);
		LoadAsset(_Path, _hWnd);
	}
}

void CObjMgr::EditAssetFileName(std::string& _PathName)
{
	CString	PathFileName;
	CA2T	_ConvertPath(_PathName.c_str());
	PathFileName = PathFindFileName(_ConvertPath);
	PathRemoveFileSpec(_ConvertPath);
	PathAddBackslash(_ConvertPath);

	VEC_INTERFACE_ITER iter_begin = m_vecBaseInterface.begin();
	CBaseInterface* assetBase = (CBaseInterface*)(*iter_begin);

	//	switch() 속성에 따라서 이름을 수정해준다
	for(int i = 0; i<ASSET_END; ++i)
	{
		//	둘의 문자열이 같은 경우
		if(assetBase->GetAssetType() == i)
		{
			CT2CA pszConvertFileName(PathFileName);
			std::string ConvertString(pszConvertFileName);
			std::string strAppend = ConvertString + "_" + m_strAtt[i] + ".rai";
			CA2T	_ConvertAppend(strAppend.c_str());
			PathAppend(_ConvertPath, _ConvertAppend);
			break;
		}
	}

	CString szResult(_ConvertPath);
	CT2CA pszConvertedAnsiString (szResult);
	std::string strResult(pszConvertedAnsiString);
	_PathName = strResult;
}

//	Frame Save And Load
bool CObjMgr::SaveFrameByFileDlg()
{
	if(m_vecBasicAsset.empty())
	{
		AfxMessageBox(TEXT("저장할 데이터가 존재하지 않습니다."));
		return false;
	}

	CString strPathName;
	std::string strRai = "rfi";
	CA2T wt(strRai.c_str());
	CA2T saveName(m_strSaveFrameFileName.c_str());

	CFileDialog FileDlg(FALSE, wt, saveName);

	TCHAR strInitPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, strInitPath);
	// 패스 변경 : Release 
#if defined _DEBUG || defined RELEASE_DEV_PATH
	if(!m_b800x600)
		_tcscat_s(strInitPath, L"\\Frame");
	else
		_tcscat_s(strInitPath, L"\\Frame800x600");
#else
	if(!m_b800x600)
		_tcscat_s(strInitPath, FRAME_FOLDER_PATH);
	else
		_tcscat_s(strInitPath, FRAME_800x600_FOLDER_PATH);
#endif
	FileDlg.m_ofn.lpstrInitialDir = strInitPath;

	int iResult = FileDlg.DoModal();

	if(IDOK == iResult)
	{
		strPathName = FileDlg.GetPathName();
		CT2CA pszConvertedAnsiString (strPathName);
		std::string _Path(pszConvertedAnsiString);
		SaveFrame(_Path);
		m_strSaveFrameFileName = "";
		return true;
	}
	else if(IDCANCEL == iResult)
	{
		return false;
	}
}

bool CObjMgr::LoadFrameByFileDlg(HWND _hWnd)
{
	//ClearTargetMap(SCENE_EDITOR_TARGET);
	CString strPathName;
	std::string strRai = "rfi";
	CA2T wt(strRai.c_str());
	CFileDialog FileDlg(TRUE, wt, NULL);

	TCHAR strInitPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, strInitPath);
	// 패스 변경 : Release 
#if defined _DEBUG || defined RELEASE_DEV_PATH
	_tcscat_s(strInitPath, L"\\Frame");
#else
	_tcscat_s(strInitPath, FRAME_FOLDER_PATH);
#endif
	FileDlg.m_ofn.lpstrInitialDir = strInitPath;

	int iResult = FileDlg.DoModal();
	if(IDOK == iResult)
	{
		ClearTargetMap(SCENE_EDITOR_TARGET);
		ClearFrameMap();

		strPathName = FileDlg.GetPathName();
		CT2CA pszConvertedAnsiString (strPathName);
		std::string _Path(pszConvertedAnsiString);
		LoadFrame(_Path, _hWnd);
		return true;
	}
	else if(IDCANCEL == iResult)
	{
		return false;
	}
}

void CObjMgr::ClearTargetMap( eAssetTarget _eTarget )
{
	if(_eTarget == ASSET_EDITOR_TARGET)
	{
		int iVecCount = m_vecBaseInterface.size();
		// 타겟으로 하는 맵만 청소한다.
		if(!m_vecBaseInterface.empty())
		{
			for(int i = 0; i < iVecCount;i++)
			{
				SAFE_DELETE(m_vecBaseInterface[i]);
			}
		}

		m_iFocusAssetBase = NOTHING_IN_HERE;
		m_vecBaseInterface.clear();
	}
	else if(_eTarget == SCENE_EDITOR_TARGET)
	{
		if(!m_vecBasicAsset.empty())
		{
			int iVecCount = m_vecBasicAsset.size();
			// 타겟으로 하는 맵만 청소한다.
			if(!m_vecBasicAsset.empty())
			{
				for(int i = 0; i < iVecCount;i++)
				{
					SAFE_DELETE(m_vecBasicAsset[i]);
				}
			}
		}

		m_iFocusBasicAsset = NOTHING_IN_HERE;
		m_vecBasicAsset.clear();

		ClearFrameMap();
		ClearSceneMap();

		SAFE_DELETE(m_BackgroundBase);
		SAFE_DELETE(m_BlurImageBase);
	}
}


void CObjMgr::ClearMap()
{
	//	커스텀 에셋 맵 클리어
	ClearTargetMap(ASSET_EDITOR_TARGET);
	ClearTargetMap(SCENE_EDITOR_TARGET);
	ClearFrameMap();
	ClearSceneMap();
}

void CObjMgr::InsertAssetBase(CBaseInterface* _asset)
{
	if(_asset != NULL)
	{
		m_iFocusAssetBase = m_vecBaseInterface.size();
		m_vecBaseInterface.push_back(_asset);
	}
}

void CObjMgr::InsertBasicAsset(CBasicAsset* _asset)
{
	if(_asset != NULL)
	{
		m_iFocusBasicAsset = m_vecBasicAsset.size();
		m_vecBasicAsset.push_back(_asset);
	}
}

void CObjMgr::InsertUIFrame(CUIFrame* _frame)
{
	m_iFocusUIFrame = m_vecUIFrame.size();
	m_vecUIFrame.push_back(_frame);
}

void CObjMgr::InsertUIScene(CUIScene* _scene)
{
	m_vecUIScene.push_back(_scene);
}

bool CObjMgr::AssetBaseCollisionCheck(POINT _MousePos)
{
	if(m_vecBaseInterface.empty())
	{
		return false;
	}

	VEC_INTERFACE_REVERSE_ITER iter_begin	= m_vecBaseInterface.rbegin();
	VEC_INTERFACE_REVERSE_ITER iter_end		= m_vecBaseInterface.rend();

	int iCount = m_vecBaseInterface.size();

	for(iter_begin; iter_end != iter_begin; iter_begin++)
	{
		--iCount;
		bool bResult = false;
		bResult = PtInRect(((CBaseInterface*)(*iter_begin))->GetHitBox(), _MousePos);
		if(bResult)
		{
			if(m_iFocusAssetBase != iCount && m_iFocusAssetBase != -1)
			{
				m_vecBaseInterface[m_iFocusAssetBase]->FreeFocus();
			}
			m_iFocusAssetBase = iCount;
			return bResult;
		}
	}

	return false;
}

bool CObjMgr::BasicAssetCollisionCheck( POINT _pos )
{
	VEC_BASICASSET_REVERSE_ITER iter_begin = m_vecBasicAsset.rbegin();
	VEC_BASICASSET_REVERSE_ITER iter_end  = m_vecBasicAsset.rend();

	int iCount = m_vecBasicAsset.size();

	//	나중에 삽입된 것이 상단에 노출되므로, 상단에 노출된 것을 먼저 픽처리 하기 위해서
	//	이렇게 해두었다.
	for(iter_begin; iter_end != iter_begin; iter_begin++)
	{
		--iCount;
		bool bResult = false;
		bResult = PtInRect(((CBasicAsset*)(*iter_begin))->GetHitBox(), _pos);

		if(bResult)
		{
			if(m_iFocusBasicAsset != iCount && m_iFocusBasicAsset != -1)
			{
				m_vecBasicAsset[m_iFocusBasicAsset]->FreeFocus();
				SetFocusUIFrameNULL();
			}

			m_iFocusBasicAsset = iCount;
			return bResult;
		}
	}

	return false;
}

void CObjMgr::DeleteAssetBase( CBaseInterface* _asset )
{
	VEC_INTERFACE_ITER iter_begin	= m_vecBaseInterface.begin();
	VEC_INTERFACE_ITER iter_end		= m_vecBaseInterface.end();

	for(iter_begin; iter_begin != iter_end; iter_begin++)
	{
		if(((CBaseInterface*)(*iter_begin)) == _asset)
		{
			SAFE_DELETE((CBaseInterface*)(*iter_begin));
			m_vecBaseInterface.erase(iter_begin);

			//불러온 파일 이름 초기화
			SetSaveFileName("");
			break;
		}
	}
}

void CObjMgr::DeleteBasicAsset( CBasicAsset* _asset )
{
	VEC_BASICASSET_ITER iter_begin	= m_vecBasicAsset.begin();
	VEC_BASICASSET_ITER iter_end	= m_vecBasicAsset.end();

	for(iter_begin; iter_begin != iter_end; iter_begin++)
	{
		if((CBasicAsset*)(*iter_begin) == _asset)
		{
			SAFE_DELETE((CBasicAsset*)(*iter_begin));
			m_vecBasicAsset.erase(iter_begin);

			SetSaveFrameFileName("");
			break;
		}
	}
}

int CObjMgr::GetTargetMapCount(eAssetTarget _Target)
{
	int iCount = 0;

	switch(_Target)
	{
	case ASSET_EDITOR_TARGET:
		iCount = m_vecBaseInterface.size();
		break;
	case SCENE_EDITOR_TARGET:
		iCount = m_vecBasicAsset.size();
		break;
	case SCENE_EDITOR_FRAME_TARGET:
		iCount = m_vecUIFrame.size();
		break;
	}
	
	return iCount;
}

void CObjMgr::ClearFrameMap()
{
	if(!m_vecUIFrame.empty())
	{
		int iVecCount = m_vecUIFrame.size();

		for(int i = 0; i<iVecCount; i++)
		{
			SAFE_DELETE(m_vecUIFrame[i]);
		}

		m_vecUIFrame.clear();

		//불러온 파일 이름 초기화
		SetSaveFrameFileName("");
	}

	m_iFocusUIFrame = NOTHING_IN_HERE;
}

CBaseInterface* CObjMgr::FindAssetBaseByIndex( INDEX _index )
{
	return m_vecBaseInterface[_index];
}

CBasicAsset* CObjMgr::FindBasicAssetByIndex( INDEX _index )
{
	return m_vecBasicAsset[_index];
}

CBasicAsset* CObjMgr::GetFocusBasicAsset()
{
	if(m_iFocusBasicAsset != NOTHING_IN_HERE)
		return m_vecBasicAsset[m_iFocusBasicAsset];
	else
		return NULL;
}

//포커스된 BasicAsset 인덱스 가져오기
int CObjMgr::GetFocusBasicAssetIndex()
{
	return m_iFocusBasicAsset;
}

CBaseInterface* CObjMgr::GetFocusAssetBase()
{
	if(m_iFocusAssetBase != NOTHING_IN_HERE)
		return m_vecBaseInterface[m_iFocusAssetBase];
	else
		return NULL;
}

void CObjMgr::SaveScene( std::string FilePath )
{
	//	UIFrame클래스를 돌면서 해당 클래스의 Save를 수행한다.
	//	여기서는 각 에셋들의 정보를 담기보다, 에셋들의 위치를 저장하게 할 예정이다.
	std::ofstream	ofSave(FilePath.c_str());

	if(m_BackgroundBase != NULL)
	{
		ofSave << "[BACKGROUND]" << std::endl;
		// 패스 변경 : Release 
		// [12/4/2018 Gabrielle]
		ConvertRelativePath(m_strBackgroundAssetPath);
		ofSave << "BackgroundPath" << "\t" << m_strBackgroundAssetPath << std::endl;
		MakeAbsolutePath(m_strBackgroundAssetPath);
	}

	if(m_BlurImageBase != NULL)
	{
		ofSave << "[BLUR IMAGE]" << std::endl;
		// 패스 변경 : Release 
		// [12/4/2018 Gabrielle]
		ConvertRelativePath(m_strBlurImageAssetPath);
		ofSave << "BlurImagePath" << "\t" << m_strBlurImageAssetPath << std::endl;
		MakeAbsolutePath(m_strBlurImageAssetPath);
	}

	VEC_FRAME_ITER iter_begin;
	VEC_FRAME_ITER iter_end;

	iter_begin = m_vecUIFrame.begin();
	iter_end = m_vecUIFrame.end();

	for(iter_begin; iter_begin != iter_end; iter_begin++)
	{
		((CUIFrame*)(*iter_begin))->ConvertFilePath();
	}

	iter_begin = m_vecUIFrame.begin();

	for(iter_begin; iter_begin != iter_end; iter_begin++)
	{
		ofSave << "[START]" << std::endl;
		(*iter_begin)->Save(ofSave);
		ofSave << "[END]\t" << std::endl;
	}

	iter_begin = m_vecUIFrame.begin();

	for(iter_begin; iter_begin != iter_end; iter_begin++)
	{
		((CUIFrame*)(*iter_begin))->MakeOriginalFilePath();
	}

	ofSave.close();
}

void CObjMgr::LoadScene( std::string FilePath, HWND _hWnd, HWND _ViewhWnd )
{
	std::ifstream	ifLoad(FilePath.c_str());
	std::string		strInput;	

	CUIScene*	newScene = new CUIScene;
	CUIFrame*	newFrame = NULL;

	while(!ifLoad.eof())
	{
		std::getline(ifLoad, strInput, '\n');

		if(!strcmp(strInput.c_str(), "[BACKGROUND]"))
		{
			std::getline(ifLoad, strInput, '\t');
			std::getline(ifLoad, strInput, '\n');
			strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
			strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
			m_strBackgroundAssetPath = strInput;
			LoadBackgroundAsset(m_strBackgroundAssetPath, _hWnd, _ViewhWnd);
		}

		if(!strcmp(strInput.c_str(), "[START]"))
		{
			newFrame = new CUIFrame;
			newFrame->Load(ifLoad);
		}

		if(!strcmp(strInput.c_str(), "[END]"))
		{
			newScene->InsertFrame(newFrame);
		}
		else
		{
			std::getline(ifLoad, strInput, '\t');
			std::getline(ifLoad, strInput, ',');
			strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());
			strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
			float fScaleX = atof(strInput.c_str());

			std::getline(ifLoad, strInput, '\n');
			strInput.erase(std::remove(strInput.begin(), strInput.end(), '\n'), strInput.end());
			strInput.erase(std::remove(strInput.begin(), strInput.end(), ','), strInput.end());
			float fScaleY = atof(strInput.c_str());

			newFrame->SetScale(fScaleX, fScaleY);
		}
	}

	InsertUIScene(newScene);
	ifLoad.close();
}

bool CObjMgr::SaveSceneByFileDlg()
{
	if(m_vecUIFrame.empty())
	{
		AfxMessageBox(TEXT("저장할 데이터가 존재하지 않습니다."));
		return false;
	}

	CString strPathName;
	std::string strRai = "rsi";
	CA2T wt(strRai.c_str());
	CA2T saveName(m_strSaveSceneFileName.c_str());

	CFileDialog FileDlg(FALSE, wt, saveName);

	TCHAR strInitPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, strInitPath);
	// 패스 변경 : Release 
#if defined _DEBUG || defined RELEASE_DEV_PATH
	if(!m_b800x600)
		_tcscat_s(strInitPath, L"\\Scene");
	else
		_tcscat_s(strInitPath, L"\\Scene800x600");
#else
	if(!m_b800x600)
		_tcscat_s(strInitPath, SCENE_FOLDER_PATH);
	else
		_tcscat_s(strInitPath, SCENE_800x600_FOLDER_PATH);
#endif
	FileDlg.m_ofn.lpstrInitialDir = strInitPath;

	int iResult = FileDlg.DoModal();

	if(IDOK == iResult)
	{
		strPathName = FileDlg.GetPathName();
		CT2CA pszConvertedAnsiString (strPathName);
		std::string _Path(pszConvertedAnsiString);
		SaveScene(_Path);
		m_strSaveSceneFileName = "";
		return true;
	}
	else if(IDCANCEL == iResult)
	{
		return false;
	}
}

bool CObjMgr::LoadSceneByFileDlg( HWND _hWnd, HWND _ViewhWnd )
{
	ClearTargetMap(SCENE_EDITOR_TARGET);
	CString strPathName;
	std::string strRai = "rsi";
	CA2T wt(strRai.c_str());
	CFileDialog FileDlg(TRUE, wt, NULL);

	TCHAR strInitPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, strInitPath);
	// 패스 변경 : Release 
#if defined _DEBUG || defined RELEASE_DEV_PATH
	_tcscat_s(strInitPath, L"\\Scene");
#else
	_tcscat_s(strInitPath, SCENE_FOLDER_PATH);
#endif
	FileDlg.m_ofn.lpstrInitialDir = strInitPath;

	int iResult = FileDlg.DoModal();
	if(IDOK == iResult)
	{
		CObjMgr::GetInstance()->ClearTargetMap(SCENE_EDITOR_TARGET);
		CObjMgr::GetInstance()->ClearSceneMap();
		CObjMgr::GetInstance()->ClearFrameMap();
		ClearTargetMap(SCENE_EDITOR_TARGET);
		ClearFrameMap();

		strPathName = FileDlg.GetPathName();
		CT2CA pszConvertedAnsiString (strPathName);
		std::string _Path(pszConvertedAnsiString);
		LoadScene(_Path, _hWnd, _ViewhWnd);
		return true;
	}
	else if(IDCANCEL == iResult)
	{
		return false;
	}
}

CUIFrame* CObjMgr::FindUIFrameByIndex( INDEX _index )
{
	return m_vecUIFrame[_index];
}

CUIFrame* CObjMgr::GetFocusUIFrame()
{
	if(m_iFocusUIFrame != NOTHING_IN_HERE)
	{
		return m_vecUIFrame[m_iFocusUIFrame];
	}
	else
	{
		return NULL;
	}
}

//포커스된 프레임 인덱스 가져오기
int CObjMgr::GetFocusUIFrameIndex()
{
	return m_iFocusUIFrame;
}

std::vector<CUIFrame*> CObjMgr::GetUIFrameList()
{
	return m_vecUIFrame;
}

//포커스한 에셋 인덱스 저장
void CObjMgr::SetFocusAssetIndex(int nIndex)
{
	m_iFocusBasicAsset = nIndex;
}

//포커스한 프레임 인덱스 저장
void CObjMgr::SetFocusFrameIndex(int nIndex)
{
	m_iFocusUIFrame = nIndex;
}

std::vector<CBasicAsset*> CObjMgr::GetBasicAssetList()
{
	return m_vecBasicAsset;
}

bool CObjMgr::UIFrameCollisionCheck( POINT _pos )
{
	VEC_FRAME_REVERSE_ITER iter_begin = m_vecUIFrame.rbegin();
	VEC_FRAME_REVERSE_ITER iter_end = m_vecUIFrame.rend();

	int iCount = m_vecUIFrame.size();

	for(iter_begin; iter_end != iter_begin; iter_begin++)
	{
		--iCount;
		bool bResult = false;
		bResult = ((CUIFrame*)(*iter_begin))->CollisionCheck(_pos);

		if(bResult)
		{
			m_iFocusUIFrame = iCount;
			SetFocusBasicAssetNULL();
			return bResult;
		}
	}

	return false;
}

void CObjMgr::DeleteUIFrame( CUIFrame* _frame )
{
	VEC_FRAME_ITER iter_begin	= m_vecUIFrame.begin();
	VEC_FRAME_ITER iter_end		= m_vecUIFrame.end();

	for(iter_begin; iter_begin != iter_end; iter_begin++)
	{
		if(((CUIFrame*)(*iter_begin)) == _frame)
		{
			SAFE_DELETE((CUIFrame*)(*iter_begin));
			m_vecUIFrame.erase(iter_begin);
			break;
		}
	}

	//불러온 파일 이름 초기화
	if(m_vecUIFrame.size() == 0)		
		SetSaveFrameFileName("");
}

//씬에서 프레임 리스트 뎁스 변경
void CObjMgr::ChangeFrameListDepth(int nSelectIndex, int iChangePos)
{
	CUIFrame* pTemp = new CUIFrame(m_vecUIFrame[nSelectIndex]);
	if(pTemp != NULL)
	{
		m_vecUIFrame.erase(find(m_vecUIFrame.begin(), m_vecUIFrame.end(), m_vecUIFrame[nSelectIndex]));	
		m_vecUIFrame.insert(m_vecUIFrame.begin()+iChangePos, pTemp);
	}	
}


//씬에서 BasicAsset 리스트 뎁스 변경
void CObjMgr::ChangeBasicAssetListDepth(int nSelectIndex, int iChangePos)
{
	CBasicAsset* pTemp = new CBasicAsset(m_vecBasicAsset[nSelectIndex]);
	if(pTemp != NULL)
	{
		m_vecBasicAsset.erase(find(m_vecBasicAsset.begin(), m_vecBasicAsset.end(), m_vecBasicAsset[nSelectIndex]));	
		m_vecBasicAsset.insert(m_vecBasicAsset.begin()+iChangePos, pTemp);
	}	
}

void CObjMgr::ClearSceneMap()
{
	if(!m_vecUIScene.empty())
	{
		int iVecCount = m_vecUIScene.size();

		for(int i = 0; i<iVecCount; i++)
		{
			SAFE_DELETE(m_vecUIScene[i]);
		}

		m_vecUIScene.clear();
		SetSaveSceneFileName("");
	}
}

bool CObjMgr::LoadBackgroundAsset( std::string _Path, HWND _hWnd, HWND _ViewhWnd )
{
	if(m_BackgroundBase != NULL)
	{
		SAFE_DELETE(m_BackgroundBase);
	}

	m_BackgroundBase = new CAssetBase;

	std::cout << "배경 경로 : " << _Path << std::endl;

	// 패스 변경 : Release 
#ifdef RELEASE_PATH
	CString	AssetFilePath(_Path.c_str());
	TCHAR szCurDir[MAX_PATH] = {0,};
	GetCurrentDirectory(MAX_PATH, szCurDir);
	TCHAR szReleasePath[MAX_PATH] = RESOURCE_DEFAULT_PATH;
	PathAppend(szCurDir, szReleasePath);
	PathAppend(szCurDir, AssetFilePath);
	std::string strReleasePath = CT2A( szCurDir );
	std::ifstream	ifLoad(strReleasePath.c_str());
#else
	std::ifstream	ifLoad(_Path.c_str());
#endif

	std::string		strInput;

	if(ifLoad.bad())
	{
		std::cout << "정상적인 경로가 아닙니다." << std::endl;
	}

	while(!ifLoad.eof())
	{
		//	로드 에셋 데이터
		std::getline(ifLoad, strInput, '\n');
		if(!strcmp(strInput.c_str(), "[END]\t"))
		{
			return true;
		}

		if(!strcmp(strInput.c_str(), "[START]"))
		{
			//	AssetType읽어오는 부분.
			std::cout << "파일에서 읽어온 단위 : " << strInput << std::endl;
			std::getline(ifLoad, strInput, '\t');
			std::cout << "파일에서 읽어온 단위 : " << strInput << std::endl;
			std::getline(ifLoad, strInput, '\n');
			std::cout << "파일에서 읽어온 단위 : " << strInput << std::endl;

			strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());

			if(atoi(strInput.c_str()) != ASSET_PICTURE)
			{
				AfxMessageBox(TEXT("잘못된 에셋이 선택되었습니다.\n정확한 텍스쳐 에셋을 선택해주세요."));
				ifLoad.close();
				SAFE_DELETE(m_BackgroundBase);
				return false;
			}

			m_BackgroundBase->SetAssetType(ASSET_PICTURE);

			std::cout << "배경 파일 로딩 시작( BackgroundBase -> Load ) " << std::endl;
			m_BackgroundBase->Load(ifLoad);
			m_BackgroundBase->SetParenthWnd(_hWnd);
			std::cout << "배경 파일 로딩 종료( BackgroundBase -> Load ) " << std::endl;
			RECT rtClientSize;
			POINT ptPosition;
			GetClientRect(_ViewhWnd, &rtClientSize);
			ptPosition.x = rtClientSize.right / 2.f - m_BackgroundBase->GetFileSize().right / 2.f;
			ptPosition.y = rtClientSize.bottom / 2.f - m_BackgroundBase->GetFileSize().bottom / 2.f;
			m_BackgroundBase->SetPosition(ptPosition);

			// 패스 변경 : Release 
#ifdef RELEASE_PATH
			m_strBackgroundAssetPath = strReleasePath;
#else
			m_strBackgroundAssetPath = _Path;
#endif
		}
	}

	ifLoad.close();
	return true;
}

bool CObjMgr::LoadBlurImageAsset( std::string _Path, HWND _hWnd, HWND _ViewhWnd )
{
	if(m_BlurImageBase != NULL)
	{
		SAFE_DELETE(m_BlurImageBase);
	}

	m_BlurImageBase = new CAssetBase;

	// 패스 변경 : Release 
#ifdef RELEASE_PATH
	CString	AssetFilePath(_Path.c_str());
	TCHAR szCurDir[MAX_PATH] = {0,};
	GetCurrentDirectory(MAX_PATH, szCurDir);
	TCHAR szReleasePath[MAX_PATH] = RESOURCE_DEFAULT_PATH;
	PathAppend(szCurDir, szReleasePath);
	PathAppend(szCurDir, AssetFilePath);
	std::string strReleasePath = CT2A( szCurDir );
	std::ifstream	ifLoad(strReleasePath.c_str());
#else
	std::ifstream	ifLoad(_Path.c_str());
#endif

	std::string		strInput;

	while(!ifLoad.eof())
	{
		//	로드 에셋 데이터
		std::getline(ifLoad, strInput, '\n');
		if(!strcmp(strInput.c_str(), "[END]\t"))
		{
			return true;
		}

		if(!strcmp(strInput.c_str(), "[START]"))
		{
			std::getline(ifLoad, strInput, '\t');
			std::getline(ifLoad, strInput, '\n');
			strInput.erase(std::remove(strInput.begin(), strInput.end(), '\t'), strInput.end());

			if(atoi(strInput.c_str()) != ASSET_PICTURE)
			{
				AfxMessageBox(TEXT("잘못된 에셋이 선택되었습니다.\n정확한 텍스쳐 에셋을 선택해주세요."));
				ifLoad.close();
				SAFE_DELETE(m_BlurImageBase);
				return false;
			}

			m_BlurImageBase->SetAssetType(ASSET_PICTURE);
			m_BlurImageBase->Load(ifLoad);

			m_BlurImageBase->SetParenthWnd(_hWnd);

			RECT rtClientSize;
			POINT ptPosition;
			GetClientRect(_ViewhWnd, &rtClientSize);
			ptPosition.x = rtClientSize.right / 2.f - m_BlurImageBase->GetFileSize().right / 2.f;
			ptPosition.y = rtClientSize.bottom / 2.f - m_BlurImageBase->GetFileSize().bottom / 2.f;
			m_BlurImageBase->SetPosition(ptPosition);

			m_strBlurImageAssetPath = _Path;
		}
	}

	ifLoad.close();
	return true;
}

void CObjMgr::SetSaveFileName(std::string strValue)
{
	m_strSaveEditAssetFileName = strValue;
}

void CObjMgr::SetSaveSceneFileName(std::string strValue)
{
	m_strSaveSceneFileName = strValue;
}

void CObjMgr::SetSaveFrameFileName(std::string strValue)
{
	m_strSaveFrameFileName = strValue;
}

void CObjMgr::ShowObjectID()
{
	m_bShowID = true;
}

void CObjMgr::HideObjectID()
{
	m_bShowID = false;
}

void CObjMgr::SetCurDlgBarIndex(int nIndex)
{
	m_iCurDlgBarIndex = nIndex;
}

int CObjMgr::GetCurDlgBarIndex()
{
	return m_iCurDlgBarIndex;
}

POINT CObjMgr::CalculateAssetMinPos()
{
	POINT	ptMinPos, ptMaxPos;
	POINT	ptCmpMinPos, ptCmpMaxPos;
	POINT	ptFileSize;

	int iCount = m_vecBasicAsset.size();

	//	각 에셋을 돌면서 최소 포지션과 최대포지션을 계산한다.
	//	해당 포지션을 계산한 이후에는 결과물을 이용하여 HitBox의 사이즈를 결정한다.
	for(int i = 0; i < iCount; i++)
	{
		if(i == 0)
		{
			ptCmpMinPos = m_vecBasicAsset[i]->GetPosition();
			ptMinPos = m_vecBasicAsset[i]->GetPosition();

			//	left, top을 좌표로 사용하고 있었기 때문에 Max 지점을 체크하기 위해서는 각 파일의 크기를
			//	left, top 좌표에 더해준 이후에 계산해야한다.
			if(m_vecBasicAsset[i]->GetAssetType() == ASSET_CHECKBOX)
			{
				ptFileSize.x = (LONG)( (m_vecBasicAsset[i]->GetHitBox()->right - m_vecBasicAsset[i]->GetHitBox()->left)
					* m_vecBasicAsset[i]->GetScaleX());
				ptFileSize.y = (LONG)((m_vecBasicAsset[i]->GetHitBox()->bottom - m_vecBasicAsset[i]->GetHitBox()->top)
					* m_vecBasicAsset[i]->GetScaleY());
			}
			else
			{
				ptFileSize.x = (LONG)(m_vecBasicAsset[i]->GetFileSize().right
					* m_vecBasicAsset[i]->GetScaleX());
				ptFileSize.y = (LONG)(m_vecBasicAsset[i]->GetFileSize().bottom
					* m_vecBasicAsset[i]->GetScaleY());
			}

			ptCmpMaxPos.x = m_vecBasicAsset[i]->GetPosition().x + ptFileSize.x;
			ptCmpMaxPos.y = m_vecBasicAsset[i]->GetPosition().y + ptFileSize.y;
			ptMaxPos.x = m_vecBasicAsset[i]->GetPosition().x + ptFileSize.x;
			ptMaxPos.y = m_vecBasicAsset[i]->GetPosition().y + ptFileSize.y;
			continue;
		}

		ptFileSize.x = (LONG)(m_vecBasicAsset[i]->GetFileSize().right * m_vecBasicAsset[i]->GetScaleX());
		ptFileSize.y = (LONG)(m_vecBasicAsset[i]->GetFileSize().bottom * m_vecBasicAsset[i]->GetScaleY());
		ptCmpMaxPos.x = m_vecBasicAsset[i]->GetPosition().x + ptFileSize.x;
		ptCmpMaxPos.y = m_vecBasicAsset[i]->GetPosition().y + ptFileSize.y;
		ptCmpMinPos = m_vecBasicAsset[i]->GetPosition();

		if(ptCmpMinPos.x < ptMinPos.x)
		{
			ptMinPos.x = ptCmpMinPos.x;
		}

		if(ptCmpMaxPos.x > ptMaxPos.x)
		{
			ptMaxPos.x = ptCmpMaxPos.x;
		}

		if(ptCmpMinPos.y < ptMinPos.y)
		{
			ptMinPos.y = ptCmpMinPos.y;
		}

		if(ptCmpMaxPos.y > ptMaxPos.y)
		{
			ptMaxPos.y = ptCmpMaxPos.y;
		}
	}

	return ptMinPos;
}

POINT CObjMgr::CalculateBaseMinPos()
{
	POINT	ptMinPos, ptMaxPos;
	POINT	ptCmpMinPos, ptCmpMaxPos;
	POINT	ptFileSize;

	int iCount = m_vecBaseInterface.size();
	ptMinPos.x = 0;
	ptMinPos.y = 0;

	ptMaxPos = ptCmpMinPos = ptCmpMaxPos = ptFileSize = ptMinPos;

	//	각 에셋을 돌면서 최소 포지션과 최대포지션을 계산한다.
	//	해당 포지션을 계산한 이후에는 결과물을 이용하여 HitBox의 사이즈를 결정한다.
	for(int i = 0; i < iCount; i++)
	{
		if(i == 0)
		{
			ptCmpMinPos = m_vecBaseInterface[i]->GetPosition();
			ptMinPos = m_vecBaseInterface[i]->GetPosition();

			//	left, top을 좌표로 사용하고 있었기 때문에 Max 지점을 체크하기 위해서는 각 파일의 크기를
			//	left, top 좌표에 더해준 이후에 계산해야한다.
			if(m_vecBaseInterface[i]->GetAssetType() == ASSET_CHECKBOX)
			{
				ptFileSize.x = (LONG)( (m_vecBaseInterface[i]->GetHitBox()->right - m_vecBaseInterface[i]->GetHitBox()->left)
					* m_vecBaseInterface[i]->GetScaleX());
				ptFileSize.y = (LONG)((m_vecBaseInterface[i]->GetHitBox()->bottom - m_vecBaseInterface[i]->GetHitBox()->top)
					* m_vecBaseInterface[i]->GetScaleY());
			}
			else
			{
				ptFileSize.x = (LONG)(m_vecBaseInterface[i]->GetFileSize().right
					* m_vecBaseInterface[i]->GetScaleX());
				ptFileSize.y = (LONG)(m_vecBaseInterface[i]->GetFileSize().bottom
					* m_vecBaseInterface[i]->GetScaleY());
			}

			ptCmpMaxPos.x = m_vecBaseInterface[i]->GetPosition().x + ptFileSize.x;
			ptCmpMaxPos.y = m_vecBaseInterface[i]->GetPosition().y + ptFileSize.y;
			ptMaxPos.x = m_vecBaseInterface[i]->GetPosition().x + ptFileSize.x;
			ptMaxPos.y = m_vecBaseInterface[i]->GetPosition().y + ptFileSize.y;
			continue;
		}

		ptFileSize.x = (LONG)(m_vecBaseInterface[i]->GetFileSize().right * m_vecBaseInterface[i]->GetScaleX());
		ptFileSize.y = (LONG)(m_vecBaseInterface[i]->GetFileSize().bottom * m_vecBaseInterface[i]->GetScaleY());
		ptCmpMaxPos.x = m_vecBaseInterface[i]->GetPosition().x + ptFileSize.x;
		ptCmpMaxPos.y = m_vecBaseInterface[i]->GetPosition().y + ptFileSize.y;
		ptCmpMinPos = m_vecBaseInterface[i]->GetPosition();

		if(ptCmpMinPos.x < ptMinPos.x)
		{
			ptMinPos.x = ptCmpMinPos.x;
		}

		if(ptCmpMaxPos.x > ptMaxPos.x)
		{
			ptMaxPos.x = ptCmpMaxPos.x;
		}

		if(ptCmpMinPos.y < ptMinPos.y)
		{
			ptMinPos.y = ptCmpMinPos.y;
		}

		if(ptCmpMaxPos.y > ptMaxPos.y)
		{
			ptMaxPos.y = ptCmpMaxPos.y;
		}
	}

	return ptMinPos;
}

void CObjMgr::Setb800x600( bool _bOn )
{
	m_b800x600 = _bOn;
}
