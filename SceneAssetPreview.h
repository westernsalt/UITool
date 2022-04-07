#pragma once
#include "PictureView.h"

class CBasicAsset;
class CUIFrame;

class CSceneAssetPreview : public CWnd
{
	DECLARE_DYNCREATE(CSceneAssetPreview)

private:
	RECT			m_tClientRt;
	CBasicAsset *	CopyAsset;		//선택한 에셋 정보
	CUIFrame *		CopyFrame;		//선택한 프레임 정보

private:
	void DeleteAsset();	//복사한 에셋, 프레임 초기화

public:
	void SetAssetPreview(int nFocusIndex);		//선택한 에셋 보여주기
	void SetFramePreview(int nFocusIndex);		//선택한 프레임 보여주기

public:
	CSceneAssetPreview();
	virtual ~CSceneAssetPreview();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnTimer(UINT_PTR nIDEvent);				//선택한 에셋, 프레임을 실시간 렌더링하기위해서 세팅
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
};


