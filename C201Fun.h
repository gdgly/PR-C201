#pragma once
#include "stdafx.h"
#include "Picture.h"
#include "PRCDAPI.H"
#include "PRCDMODE.H"
#include "38TReader.h"

#define MSG_SEND_MESSAGE	WM_APP + 3;


// Information structure to pass to the thread
typedef struct {
	TCHAR			cDriverName[256];	// Printer driver name
	HWND			hWnd;				// Window handle
	BOOL			bEncISOTr1;			// Magnetic encoding operation_ISOTr1
	BOOL			bEncJISTr1;			// Magnetic encoding operation_JISTr1
	BOOL			bEncISOTr2;			// Magnetic encoding operation_ISOTr2
	BOOL			bEncISOTr3;			// Magnetic encoding operation_ISOTr3
	BOOL			bEncJISTr3;			// Magnetic encoding operation_JISTr3
	BOOL			bEncJIS2;			// Magnetic encoding operation_JIS2
	BOOL			bMotionContactIC;	// Optional normal operation type_IC R/W 1
	BOOL			bMotionNonCotactIC;	// Optional normal operation type_IC R/W 2
	BOOL			bMotionOptionbox;	// ptional normal operation type_External box
	BOOL			bMotionMg;			// Optional normal operation type_Magnetic encoder
	BYTE			bMotionMgTrack;		// Magnetic track to read in normal operation
	BOOL			bAheadContactIC;	// Optional pre-processing operation type_IC R/W 1
	BOOL			bAheadNonCotactIC;	// Optional pre-processing operation type_IC R/W 2
	BOOL			bAheadOptionbox;	// Optional pre-processing operation type_External box
	BOOL			bAheadMg;			// Optional pre-processing operation type_Magnetic encoder
	BYTE			bAheadMgTrack;		// Magnetic track to read in pre-processing operation
  } PrintInfo;


typedef PR5600RESULT	(CALLBACK* _pr56XXEnumEnabledPrinters)(PVOID, DWORD, PDWORD, PDWORD);
typedef VOID			(CALLBACK* _pr56XXTerminate)(VOID);
typedef PR5600HANDLE	(CALLBACK* _pr56XXOpenPrinter)(DWORD, DWORD, PCWSTR, HWND, UINT);
typedef PR5600RESULT	(CALLBACK* _pr56XXClosePrinter)(PR5600HANDLE);
typedef PR5600RESULT	(CALLBACK* _pr56XXRegisterMessage)(PR5600HANDLE, DWORD, PR5600APPNOTIFY*);
typedef PR5600RESULT	(CALLBACK* _pr56XXGetLogs)(DWORD, DWORD, PCWSTR, DWORD, PDWORD, PPR5600_LOG);
typedef PR5600RESULT	(CALLBACK* _pr56XXGetPrinterData)(PR5600HANDLE, PBYTE, DWORD, PDWORD);
typedef PR5600RESULT	(CALLBACK* _pr56XXPrint_SetICEncoder)(PR5600HANDLE, DWORD, BOOL);
typedef PR5600RESULT	(CALLBACK* _pr56XXPrint)(PR5600HANDLE, PBYTE, DWORD);
typedef PR5600RESULT (CALLBACK* _pr56XXGetPrinterStatusInformationEx)(PR5600HANDLE, PPR5600_STATUS_INFO2,
	DWORD, PDWORD, PPR5600_PRINTER_SENSE, DWORD, PDWORD);


class C201
{
public:
	C201(void);
	~C201(void);
public:
	int					m_CardType;
	BOOL				m_bNeedPrinter;

	// Function pointer
	_pr56XXEnumEnabledPrinters				pr56XXEnumEnabledPrinters;
	_pr56XXTerminate						pr56XXTerminate;
	_pr56XXOpenPrinter						pr56XXOpenPrinter;
	_pr56XXClosePrinter						pr56XXClosePrinter;
	_pr56XXRegisterMessage					pr56XXRegisterMessage;
	_pr56XXGetLogs							pr56XXGetLogs;
	_pr56XXGetPrinterData					pr56XXGetPrinterData;
	_pr56XXPrint_SetICEncoder				pr56XXPrint_SetICEncoder;
	_pr56XXPrint							pr56XXPrint;
	_pr56XXGetPrinterStatusInformationEx    pr56XXGetPrinterStatusInformationEx;
	HINSTANCE m_hInstance;
public:
	CPicture		m_Pic1;
	HDC				m_hCardDCForPrinting;
	HDC				m_hCardDCForParKSmartCard;
	CString			m_strPrinterName;

	BOOL			m_bIsPrinting;
	BOOL			m_bParkingSmartCard;
	BOOL			m_bNeedPrinting;
	PR5600HANDLE pr56handle ;		// Printer handle
	DEVMODE		*pDEVMODE ;			// DEVMODE structure

	unsigned short		m_iC201Result;
    
    BOOL                m_bSCFailOutPuted;
public:
	bool ResetPrint(CString			PrinterName);
	BOOL	C201_LoadC201DLL(CString strPrintName);		 
	unsigned short		C201_StartOneJob(CString strPrintName);
    bool	C201_SendEjectCardCommand();

	unsigned short		C201_ParkSmartCard(char isContactCard ,bool NeedPrinting);
	unsigned short		C201_UnParkSmartCard(bool isgood);	
	unsigned short		C201_PrintPretreatment ();
	unsigned short		C201_PrintText(
int X,							//��ӡλ�õĺ�����
int Y,							//��ӡλ�õ�������
int Width,
int Height,
CString Font,					//ʹ�õ���������
float Size,						//�ֺŴ�С
unsigned char Attribute,		//��������
int Color,					//������ɫ��ʹ�ñ�׼��Web��ɫ���룩
CString Text ,					//��ӡ���ı�
BOOL  TransBkColor,             //����͸��
int  wordSpace,                         //�ּ��
int  rowSpace
)	 ;

unsigned short   C201_PrintImage(
int X,		//��ӡλ�õĺ�����
int Y,		//��ӡλ�õ�������
int Weight,	//��ӡͼƬ�Ŀ��
int Height,	//��ӡͼƬ�ĸ߶�
CString Path,	//ͼƬ��ȫ·��
int rotate,
BOOL flag
)	;


unsigned short  C201_PrintImage(
int X,		//��ӡλ�õĺ�����
int Y,		//��ӡλ�õ�������
int Weight,	//��ӡͼƬ�Ŀ��
int Height,	//��ӡͼƬ�ĸ߶�
byte* picData,	//ͼƬ��ȫ·��
int picLen
);
unsigned short			C201_StartPrinting(bool haveNext);

unsigned short C201_EndJob() ;
unsigned short C201_GetPrinterSn( char* Sn); 

public:
	BOOL			m_bIsMyPrinter;
	CReader38T		m_38TReader;
	BOOL			IsMyPrinter(char*buf);
	bool			C201_WaitCardPrinting();
	bool			C201_WaitCardOutSlot(int time);
	bool			C201_SendMoveCardCommand();
	bool			C201_SendICSoltCommand(byte command);
	unsigned short			C201_CheckState(char*data);

	bool			C201_Moveout();


	BOOL	C201_SetPrintDuplexOrNot(int bDuplexMode);
	ApplicationTag applicationtag;			// ApplicationTag structure
};
