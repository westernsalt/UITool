
// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������ 
// ��� �ִ� ���� �����Դϴ�.

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // �Ϻ� CString �����ڴ� ��������� ����˴ϴ�.

// MFC�� ���� �κа� ���� ������ ��� �޽����� ���� ����⸦ �����մϴ�.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC �ٽ� �� ǥ�� ���� ����Դϴ�.
#include <afxext.h>         // MFC Ȯ���Դϴ�.





#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC�� ���� �� ��Ʈ�� ���� ����

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <hash_map>
#include <vector>
#include <string>
#include <algorithm>

#include "locale.h"
#include <fstream>

//#include <cstdlib>
//#include "atlstr.h"

#include "Macro.h"
#include "Extern.h"
#include "Define.h"
#include "Include\ximage.h"

#include "GlobalFunc.h"

//	Bool ���� ����, ���ɰ���� ǥ������ �ʰ� �ص�.
#pragma warning (disable:4800)
//	itoa warning ǥ������ �ʰ� �ص�
#pragma warning (disable:4996)
//	��� ��ο��� ���� ��ȯ���� �ʽ��ϴ� ��� ǥ�� ���ϰ� �ص�
#pragma warning (disable:4715)
