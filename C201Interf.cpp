// DatacardMag.cpp : Defines the initialization routines for the DLL.
//



#include "stdafx.h"
#include "afxcoll.h"
#include  "WINSPOOL.H"
#include <tchar.h>
#include "C201Interf.h"
#include "C201Fun.h"

#include "resource.h"

static CMapPtrToPtr			g_PrintMap;
HWND  hMainAppWnd = NULL;
//#ifdef _DEBUG
//#define new DEBUG_NEW
//#undef THIS_FILE
//static char THIS_FILE[] = __FILE__;
//#endif


//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CCP_Print_DllApp

BEGIN_MESSAGE_MAP(CCP_Print_DllApp, CWinApp)
	//{{AFX_MSG_MAP(CCP_Print_DllApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCP_Print_DllApp construction

#if _MSC_VER >= 1300    // for VC 7.0
  // from ATL 7.0 sources
  #ifndef _delayimp_h
  extern "C" IMAGE_DOS_HEADER __ImageBase;
  #endif
#endif

HMODULE GetCurrentModule()
{
#if _MSC_VER < 1300    // earlier than .NET compiler (VC 6.0)

  // Here's a trick that will get you the handle of the module
  // you're running in without any a-priori knowledge:
  // http://www.dotnet247.com/247reference/msgs/13/65259.aspx

  MEMORY_BASIC_INFORMATION mbi;
  static int dummy;
  VirtualQuery( &dummy, &mbi, sizeof(mbi) );

  return reinterpret_cast<HMODULE>(mbi.AllocationBase);

#else    // VC 7.0

  // from ATL 7.0 sources

  return reinterpret_cast<HMODULE>(&__ImageBase);
#endif
}



typedef struct tagWNDINFO
{
    DWORD dwProcessId;
    HWND hWnd;
    HWND Dll_hwnd;
} WNDINFO, *LPWNDINFO;

BOOL CALLBACK YourEnumProc(HWND hWnd, LPARAM lParam)
{
    DWORD dwProcessId;
    GetWindowThreadProcessId(hWnd, &dwProcessId);
    LPWNDINFO pInfo = (LPWNDINFO)lParam;
    if( (dwProcessId == pInfo->dwProcessId) && (hWnd != pInfo->Dll_hwnd) )   //确认找到的句柄不是DLL窗口的句柄
    //如果找到的是DLL窗口的句柄则返回FALSE以便继续查找宿主的窗口句柄
    {
        pInfo->hWnd = hWnd;
        return FALSE;
    }
    return TRUE;
}
HWND GetProcessMainWnd(DWORD dwProcessId, HWND Dll_hwnd)//将DLL窗口的句柄传入，以便识别找到的句柄不是DLL窗口的句柄
{
    WNDINFO wi;
    wi.dwProcessId = dwProcessId;
    wi.hWnd = NULL;
    wi.Dll_hwnd = Dll_hwnd;
    EnumWindows(YourEnumProc,(LPARAM)&wi);
    HWND re_hwnd = wi.hWnd;
    while( GetParent(re_hwnd) != NULL )    //循环查找父窗口，以便保证返回的句柄是最顶层的窗口句柄
    {
        re_hwnd = GetParent( re_hwnd );
    }
    return re_hwnd;
}


CCP_Print_DllApp::CCP_Print_DllApp()
{
	g_PrintMap.InitHashTable(131);
}

int CCP_Print_DllApp::ExitInstance()
{
	C201* pPrinter = NULL;
	POSITION pos = g_PrintMap.GetStartPosition( ) ;
	while(pos)
	{
		HANDLE devNo;
		g_PrintMap.GetNextAssoc(pos,(void *&)devNo,(void *&)pPrinter);
		if(pPrinter)
		{
			delete pPrinter;
			pPrinter = NULL;
		}
	};
	g_PrintMap.RemoveAll();
	return CWinApp::ExitInstance();
}

CCP_Print_DllApp theApp;
/////////////////////////////////////////////////////////////////////////////
// The one and only CCP_Print_DllApp object
void WriteLog(CString log)
{
	return ;
	USES_CONVERSION;
	//COleDateTime timeNow;						
	//timeNow = COleDateTime::GetCurrentTime();
	SYSTEMTIME  sysTime;
	GetSystemTime(&sysTime);

	char ctime[64];memset(ctime, 0,64);

	sprintf(ctime,"%02dH-%02dM-%02dS-%03dMs",sysTime.wHour,sysTime.wMinute,sysTime.wSecond,sysTime.wMilliseconds); 

	CString  curTime = CString(ctime);

	memset(ctime, 0,64);
	sprintf(ctime,"JT_StartOneJob%dY-%dM-%dD",sysTime.wYear,sysTime.wMonth,sysTime.wDay); 

	CString  fileName =  CString(ctime);;

	//CString  curTime = timeNow.Format(CString("%Y-%m-%d %H:%M:%S"));
	//CString  fileName = timeNow.Format(CString("%Y-%m-%d"));
	fileName = CString(CString(_T("d:\\"))) + fileName + CString(CString(_T(".log")));
	
	FILE* hfile = fopen(W2A(fileName.GetBuffer(0)),W2A((L"a+")));
	if(hfile)
	{
		fseek(hfile,0,SEEK_END);
		fprintf(hfile,"%s: %s\n",curTime,log);
		fclose(hfile);
	}
};
	


C201* SerchPrinterByHandle(HANDLE devNo)
{
	C201* pPrinter = NULL;
	if(devNo)
	{
		g_PrintMap.Lookup((void*)devNo,(void *&)pPrinter);
	}
	return pPrinter;
};


CP_Dll unsigned short JT_StartOneJob(char* PrintName,HANDLE* devNo)//BOOL CP_ConnectEqt(
{
	hMainAppWnd = GetProcessMainWnd( GetCurrentProcessId(),theApp.GetMainWnd()->GetSafeHwnd()  );
	if(hMainAppWnd==NULL)
		return 0x9501;

	C201* pPrinter = NULL;
	
	INT_PTR count = g_PrintMap.GetCount();

	POSITION pos = g_PrintMap.GetStartPosition( ) ;

	HANDLE devNoTemp;

	for (pos = g_PrintMap.GetStartPosition(); pos != NULL;)
	{
		g_PrintMap.GetNextAssoc(pos,devNoTemp,(void *&)pPrinter);
		if(pPrinter)
		{
			WriteLog(pPrinter->m_strPrinterName);
			if(pPrinter->m_strPrinterName ==  CString(PrintName))
			{
				WriteLog("Gy_Gzky_OpenPrinter get the one have same name");
				break;
			}
			else
				pPrinter = NULL;
		}
	};
	if(!pos)
		WriteLog("Gy_Gzky_OpenPrinter did not get the one have same name");

	BOOL first = FALSE;

	if(pPrinter==NULL)
	{
		pPrinter = new C201();
		first = TRUE;
		WriteLog("Gy_Gzky_OpenPrinter Create New CDCPrinter 0");
	}
	else if(pPrinter->m_hCardDCForPrinting == NULL )
	{
		g_PrintMap.RemoveKey(devNoTemp);
		delete pPrinter;

		pPrinter = new C201();
		first = TRUE;

		WriteLog("Gy_Gzky_OpenPrinter Delete old and Create New CDCPrinter");
	}

	if(pPrinter)
	{
		DWORD ret = pPrinter->C201_StartOneJob(PrintName);
		if(ret==0x9000)
		{
			if(first)
				g_PrintMap.SetAt(hMainAppWnd,pPrinter);
			*devNo = hMainAppWnd;//pPrinter->m_ComDlg;
			
			pPrinter->m_bIsPrinting = false;
			pPrinter->m_bParkingSmartCard = false;
			pPrinter->m_bNeedPrinting = true;

			WriteLog("Gy_Gzky_OpenPrinter save the just New CDCPrinter 0");
			return 0x9000;
		}
		else
		{
			if(first)
				delete pPrinter;
			WriteLog("Gy_Gzky_OpenPrinter Delete the just New CDCPrinter 2");
			return ret;

		}
	}
	else
	{
		return 0x9102;
	}
}
;

//CWinThread *m_print;
//
//DWORD  WINAPI thread_Print(LPVOID pParam)
//{
//	C201 *pPrinter = (C201*)pParam;
//	return 	  pPrinter->C201_ParkSmartCard(pPrinter->m_CardType,pPrinter->m_bNeedPrinter) ;
//
//}


CP_Dll int JT_ParkSmartCard(HANDLE devNo,char isContactCard ,bool NeedPrinting)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	C201* pPrinter = SerchPrinterByHandle(devNo);;
	if(pPrinter==NULL)
	{
		return 0x9103;
	}				 
		
	int ret = 0x9000;

	if(pPrinter->m_bParkingSmartCard)
		return 0x9000;
	//pPrinter->m_CardType = isContactCard;
	//pPrinter->m_bNeedPrinter = NeedPrinting;
	ret = pPrinter->C201_ParkSmartCard(isContactCard,NeedPrinting);
	//m_print  = AfxBeginThread((AFX_THREADPROC)thread_Print, (LPVOID)pPrinter,THREAD_PRIORITY_HIGHEST);
	//int time = 0;
	//while(!((CMessageDlg*)pPrinter->m_ComDlg)->m_bIcArrived)
	//{
	//	Sleep(1000);
	//	if(time++>10)
	//		break;
	//}

	if(ret==0x9000)
	{
		pPrinter->m_bNeedPrinting = true;
		pPrinter->m_bParkingSmartCard = true;
	}
	return ret;
}

					  
CP_Dll unsigned short JT_UnParkSmartCard(HANDLE devNo,bool isgood)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	C201* pPrinter = SerchPrinterByHandle(devNo);;
	if(pPrinter==NULL)
	{
		return 0x9104;
	}

	unsigned short ret = pPrinter->C201_UnParkSmartCard( isgood) ;
	if(ret == 0x9000)
		pPrinter->m_bParkingSmartCard = false;
	return ret;
};

CP_Dll unsigned short JT_PrintPretreatment (HANDLE devNo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	C201* pPrinter = SerchPrinterByHandle(devNo);;
	if(pPrinter==NULL)
	{
		return 0x9105;
	}
	if(!pPrinter->m_bNeedPrinting)
		return 0x9003;
	return  pPrinter->C201_PrintPretreatment ( );
};
CP_Dll unsigned short  WINAPI  JT_PrintText(
HANDLE devNo,
float  X,							//打印位置的横坐标
float  Y,							//打印位置的纵坐标
float  Width,
float  Height,
char* Font,					//使用的字体名称
float Size,						//字号大小
unsigned char Attribute,		//文字属性
int Color,					//文字颜色（使用标准的Web颜色代码）
char* Text ,					//打印的文本
int  wordSpace,                         //字间距
int  rowSpace                        //行间距
)	 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	USES_CONVERSION;

	X = X/25.4f*600.0f;
	Y = Y/25.4f*600.0f;
	Width = Width/25.4f*600.0f;;
	Height = Height/25.4f*600.0f;
	wordSpace = wordSpace/25.4f*600.0f;
	rowSpace = rowSpace/25.4f*600.0f;

	C201* pPrinter = SerchPrinterByHandle(devNo);;
	if(pPrinter==NULL)
	{
		return 0x9106;
	}
	if(!pPrinter->m_bNeedPrinting)
		return 0x9107;

	BOOL  TransBkColor = true;
	return  pPrinter->C201_PrintText( X, Y, Width, Height, A2W(Font), Size, Attribute, Color,
			A2W(Text),	TransBkColor, wordSpace, rowSpace);
}


CP_Dll unsigned short JT_PrintImage(
HANDLE devNo,
float X,		//打印位置的横坐标
float Y,		//打印位置的纵坐标
float Width,	//打印图片的宽度
float Height,	//打印图片的高度
char* Path,	//图片的全路径
int rotate,
BOOL flag
)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	USES_CONVERSION;

	//X = X/25.4f*600.0f;
	//Y = Y/25.4f*600.0f;
	//Width = Width/25.4f*600.0f;;
	//Height = Height/25.4f*600.0f;

	C201* pPrinter = SerchPrinterByHandle(devNo);;
	if(pPrinter==NULL)
	{
		return 0x9108;
	}

	if(!pPrinter->m_bNeedPrinting)
		return 0x9109;
	return  pPrinter->C201_PrintImage(X, Y,	 Width, Height, A2W(Path), rotate, flag) ;	
}

CP_Dll unsigned short JT_StartPrinting(HANDLE devNo,bool haveNext)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	C201* pPrinter = SerchPrinterByHandle(devNo);;
	if(pPrinter==NULL)
	{
		return 0x910A;
	}
	if(!pPrinter->m_bNeedPrinting)
		return 0x910B;
	int ret=   pPrinter->C201_StartPrinting(haveNext) ;
		
	return ret;
};


CP_Dll unsigned short JT_EndJob(HANDLE devNo)  
{
	C201* pPrinter = SerchPrinterByHandle(devNo);;
	if(pPrinter==NULL)
	{
		return 0x910C;
	}
	return  pPrinter->C201_EndJob( )  ;
}


CP_Dll unsigned short JT_CheckState(HANDLE devNo,char* data)  
{
	C201* pPrinter = SerchPrinterByHandle(devNo);;
	if(pPrinter==NULL)
	{
		return 0x910D;
	}
	return pPrinter->C201_CheckState(data);
}



CP_Dll unsigned short JT_GetPrinterSn(HANDLE devNo,char* Sn)  
{
	C201* pPrinter = SerchPrinterByHandle(devNo);;
	if(pPrinter==NULL)
	{
		return 0x910D;
	}
	return pPrinter->C201_GetPrinterSn(Sn);
}


CP_Dll unsigned short JT_MoveCardOut(HANDLE devNo, BOOL flag)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	C201* pPrinter = SerchPrinterByHandle(devNo);;
	if(pPrinter==NULL)
	{
		return 0x9104;
	}
	if (!flag)
	{
		unsigned short ret = pPrinter->C201_UnParkSmartCard(false) ;		
		if(ret == 0x9000)
			pPrinter->m_bParkingSmartCard = false;
		return ret;
	}	
	unsigned short ret = pPrinter->C201_UnParkSmartCard(true) ;
	if(ret == 0x9000)
		pPrinter->m_bParkingSmartCard = false;
	ret = pPrinter->C201_PrintPretreatment ( );
	ret = pPrinter->C201_StartPrinting(false);
	return ret;
}

