
#if !defined(AFX_DATACARDMAG_H__D25E1B38_0FF4_4C9F_ADAE_A370299FB967__INCLUDED_)
#define AFX_DATACARDMAG_H__D25E1B38_0FF4_4C9F_ADAE_A370299FB967__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#define Mydll

#ifdef Mydll
#define CP_Dll extern "C" _declspec(dllexport)
#else
#define CP_Dll extern "C" _declspec(dllimport)
#endif


/////////////////////////////////////////////////////////////////////////////
// CCP_Print_DllApp
// See DatacardMag.cpp for the implementation of this class
//


#ifdef Mydll

class CCP_Print_DllApp : public CWinApp
{
public:
	CCP_Print_DllApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCP_Print_DllApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCP_Print_DllApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:

	virtual int ExitInstance();

};

#endif 
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATACARDMAG_H__D25E1B38_0FF4_4C9F_ADAE_A370299FB967__INCLUDED_)


CP_Dll unsigned short JT_StartOneJob(char* PrintName, HANDLE* devNo);

CP_Dll int JT_ParkSmartCard(HANDLE devNo, char isContactCard, bool NeedPrinting);

CP_Dll unsigned short JT_UnParkSmartCard(HANDLE devNo,bool isgood);

CP_Dll unsigned short JT_PrintPretreatment (HANDLE devNo);

CP_Dll unsigned short  WINAPI  JT_PrintText(HANDLE devNo, float  X,	 float  Y, float  Width, float  Height,
	char* Font,	float Size,	 unsigned char Attribute, int Color, char* Text , int  wordSpace, int  rowSpace);

CP_Dll unsigned short JT_PrintImage(HANDLE devNo, float X, float Y, float Width, float Height,
	char* Path, int rotate, BOOL flag);

CP_Dll unsigned short JT_StartPrinting(HANDLE devNo,bool haveNext);

CP_Dll unsigned short JT_EndJob(HANDLE devNo);

CP_Dll unsigned short JT_CheckState(HANDLE devNo, char* data);

CP_Dll unsigned short JT_GetPrinterSn(HANDLE devNo, char* Sn);

CP_Dll unsigned short JT_MoveCardOut(HANDLE devNo, BOOL flag);