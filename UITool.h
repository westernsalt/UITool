
// UITool.h : UITool ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CUIToolApp:
// �� Ŭ������ ������ ���ؼ��� UITool.cpp�� �����Ͻʽÿ�.
//

class CUIToolApp : public CWinApp
{
public:
	CUIToolApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CUIToolApp theApp;
