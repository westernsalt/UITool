#pragma once
#include "PictureView.h"

class CBasicAsset;
class CUIFrame;

class CSceneAssetPreview : public CWnd
{
	DECLARE_DYNCREATE(CSceneAssetPreview)

private:
	RECT			m_tClientRt;
	CBasicAsset *	CopyAsset;		//������ ���� ����
	CUIFrame *		CopyFrame;		//������ ������ ����

private:
	void DeleteAsset();	//������ ����, ������ �ʱ�ȭ

public:
	void SetAssetPreview(int nFocusIndex);		//������ ���� �����ֱ�
	void SetFramePreview(int nFocusIndex);		//������ ������ �����ֱ�

public:
	CSceneAssetPreview();
	virtual ~CSceneAssetPreview();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnTimer(UINT_PTR nIDEvent);				//������ ����, �������� �ǽð� �������ϱ����ؼ� ����
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
};


