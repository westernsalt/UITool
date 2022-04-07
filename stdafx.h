
// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이 
// 들어 있는 포함 파일입니다.

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 일부 CString 생성자는 명시적으로 선언됩니다.

// MFC의 공통 부분과 무시 가능한 경고 메시지에 대한 숨기기를 해제합니다.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 핵심 및 표준 구성 요소입니다.
#include <afxext.h>         // MFC 확장입니다.





#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 공용 컨트롤에 대한 MFC 지원입니다.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows 공용 컨트롤에 대한 MFC 지원입니다.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC의 리본 및 컨트롤 막대 지원

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

//	Bool 변수 강제, 성능경고문구 표시하지 않게 해둠.
#pragma warning (disable:4800)
//	itoa warning 표시하지 않게 해둠
#pragma warning (disable:4996)
//	모든 경로에서 값을 반환하진 않습니다 경고 표시 안하게 해둠
#pragma warning (disable:4715)
