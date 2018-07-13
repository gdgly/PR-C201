/*********************************************************************
*
*	< Application Interface >
*	  PRCDAPI.H
*
*         Official edition Ver1.01	2012.05.31
*
*         PR-C201 Card Printer Driver
*					Copyright(c) 2012 Nisca Corporation.
*
**********************************************************************/
#ifndef _PRDAPI_H
#define _PRDAPI_H

#define PR56ERR_SUCCESS		PR56ERR_NO_ERROR
#define MAX_PATH		260 
#define NSC_PCNAME_LENMAX	20
#define PR5600_PRODUCTID_LEN	16
#define MAX_SHARENAME_LENGTH	80
#define NSC_TAG_LEN		8
#define NSC_SERIALNO_LEN	8


typedef PVOID	PR5600HANDLE;

/********************************************************/
/*	Enumeration											*/
/********************************************************/

// PR5600RESULT列挙体
typedef enum {
	PR56ERR_NO_ERROR = 0,						// 0  No error or successful.
	PR56ERR_NOT_REGISTERED,						// 1  Not executable because application is not registered.
	PR56ERR_ALREADY_REGISTERED,					// 2  Request for registration of an already registered application.
	PR56ERR_INVALID_ARGUMENTS,					// 3  Invalid argument.
	PR56ERR_DRIVER_FAILURE,						// 4  Printer driver error.
	PR56ERR_PORT_FAILURE,						// 5  Port monitor IO error.
	PR56ERR_TOO_MANY_REGISTRATION,				// 6  Registration not possible because too many applications are registered.
	PR56ERR_ALREADY_USED,						// 7  Already used by another application.
	PR56ERR_INVALID_ID,							// 8  Invalid printer ID.
	PR56ERR_INVALID_HANDLE,						// 9  Invalid printer handle.
	PR56ERR_TIMEOUT,							// 10 Timeout has occurred.
	PR56ERR_PRINTER_NOT_OPEN,					// 11 Printer is not open.
	PR56ERR_PRINTER_NOT_EXIST,					// 12 The printer does not exist (it has been deleted). Or, it is powered off.
	PR56ERR_INSUFFICIENT_BUFFER,				// 13 Invalid buffer.
	PR56ERR_BUSY,								// 14 Request rejected because an exclusive processing is being executed.
	PR56ERR_IN_PROGRESS,						// 15 Request rejected because printer driver is waiting for response.
	PR56ERR_NO_DATA,							// 16 No data present.
	PR56ERR_COMM,								// 17 Command send/receive failure.
	PR56ERR_MEMORY,								// 18 Memory error in printer driver.
	PR56ERR_OFFLINE,							// 19 Printer is offline.
	PR56ERR_NAC_RETURNED,						// 20 NAC received as command response.
	PR56ERR_READWRITE_INCOMPLETE,				// 21 Direct read/write incomplete.
	PR56ERR_TAG_NOT_SIGNED,						// 22 Unregistered tag.
	PR56ERR_ICENC_NOT_CONNECT,					// 23 IC encoder disconnected.
	PR56ERR_ICENC_NOT_READY,					// 24 Card not in the IC encoder access position.
	PR56ERR_NOT_SUPPORTED_MODEL,				// 25 Internal error
	PR56ERR_SERVER_NOTFOUND,					// 26 Internal error
	PR56ERR_NOT_READY_SERVER,					// 27 Internal error
	PR56ERR_NOT_READY_SMON,						// 28 Internal error
	PR56ERR_NOT_RESERVED,						// 29 Internal error
	PR56ERR_NOT_SUPPORTED						// 30 Internal error
} PR5600RESULT;


typedef struct _PR5600_STATUS_INFO2{
	BYTE	btHardwareStatus;					// プリンターのハードウェアの状態
	BYTE	btCardPosition;						// プリンター内のカードの位置
	BYTE	btPrinterStatus;					// プリンターの印刷動作の状態
	BYTE	btEncoderStatus;					// 処理状態
	BYTE	btMemoryStatus[2];					// メモリA,Bの使用状態
	BYTE	btDataStatus[2];					// メモリA,Bの印刷データの受信状態
	BYTE	btOptionUnitStatus;					// オプションユニットの特別な状態
	BYTE	btTotalPrintNumber[3];					// 総印刷枚数
	BYTE	btColorControlValue[3];					// 色合い調整値
	CHAR	acMagEncoderROMVersion[4];				// 磁気エンコーダのROMバージョン
	CHAR	acHeatRollerROMVersion[4];				// ヒートローラユニットのROMバージョン
	BYTE	btICTagData[NSC_TAG_LEN];				// 現在IC-R/Wにアクセスしているカードのタグ情報
	BYTE	btReserved[ 10 ];					// 未使用
	BYTE	btStandbyStatus;					// プリンターの待機状態 
	BYTE	btRibbonRemainder;					// インクリボンの残量
	BYTE	btFilmRemainder;					// 再転写フィルムの残量
	BYTE	btReserved2[ 16 ];					// 未使用
} PR5600_STATUS_INFO2, *PPR5600_STATUS_INFO2;


/********************************************************/
/*	Structure											*/
/********************************************************/

#pragma pack(1)

// PR5600_PRINTER_INFO structure
typedef struct _PR5600_PRINTER_INFO{
	DWORD	dwInternalID;						// Indicates the internal ID for identification used by the printer driver.
	DWORD	dwPrinterID;						// Indicates the ID (Printer ID) specified in the printer itself.
	WCHAR	wszName[MAX_PATH + 4];				// Printer name (UTF-16 character string) of the printer folder. It is a name the user can change.
	WCHAR   wszServerName[NSC_PCNAME_LENMAX];	// Indicates the print server's PC name (UTF-16 character string). It is a name the user cannot change
	WCHAR   wszShareName[MAX_SHARENAME_LENGTH]; // Indicates the print server's shared name (UTF-16 character string). It is a name the user cannot change.
	WCHAR	wszDriverName[MAX_PATH + 4];		// Indicates the printer driver name (UTF-16 character string). It is a name the user cannot change.
	CHAR	acProductId[PR5600_PRODUCTID_LEN];	// Indicates the printer product ID. It is a non-NULL terminated 16-byte ASCII character string.
	CHAR	szSerial[16];						// Indicates a 7-byte printer serial number plus 1-byte additional information. It is a NULL terminated character string.
	BOOL	bIcEncContact;						// A flag that indicates the printer is equipped with IC R/W 1.
	BOOL	bIcEncNoncontact;					// A flag that indicates the printer is equipped with IC R/W 2.
	BOOL	bOnline;							// A flag that indicates the printer is online.
} PR5600_PRINTER_INFO, *PPR5600_PRINTER_INFO;


// PR5600APPNOTIFY structure
typedef struct _PR5600APPNOTIFY{
	DWORD	Notify;								// 0: Send message when card reaches the designated access position.
	HWND	hWnd;								// Specify the window handle for receiving notifications.
	UINT	nMsg;								// Specify messages for which notifications are expected. Specify message numbers between 0x8000 and 0xA000 (WM_APP).
} PR5600APPNOTIFY, *PPR5600APPNOTIFY;


// PR5600_PRINTER_STATUS structur
typedef struct _PR5600_PRINTER_STATUS{
	union {
		DWORD	dwStatus;						// Indicates the status information and memory information structures as DWORD values.
			struct {
			BYTE	btStatus;					// Indicates the printer operational status. Refer to PR5600_STATUS_INFO structure member btHardwareStatus.
			BYTE	btMemA;						// Indicates the memory A operational status. Refer to PR5600_STATUS_INFO structure member btMemoryStatus.
			BYTE	btMemB;						// Indicates the memory B operational status. Refer to PR5600_STATUS_INFO structure member btMemoryStatus.
			BYTE	btReserved;					// Not used.
			} byte;
	};
	union {
		DWORD	dwStatus2;						// Indicates additional status information and additional memory information structures as DWORD values.
		struct {
			BYTE	btCard;						// Indicates the card location. Refer to PR5600_STATUS_INFO structure member btCardPosition.
			BYTE	btPrinter;					// Indicates the printing status. Refer to PR5600 STATUS_INFO structure member btPrinterStatus.
			BYTE	btEncoder;					// Indicates the encoding status. Refer to PR5600_STATUS_INFO structure member btEncoderStatus.
			BYTE	btReserved;					// Not used.
			} byte2;
	};
} PR5600_PRINTER_STATUS, *PPR5600_PRINTER_STATUS;


// PR5600_PRINTER_ERROR structure
typedef union _PR5600_PRINTER_ERROR{
	DWORD	dwError;							// Indicates the error information structure as DWORD values.
	struct {
		BYTE	btSenseKey;						// Indicates the sense key. Refer to PR5600_PRINTER_SENSE structure member variable btSenseKey.
		BYTE	btAdditionalSenseCode;			// Indicates additional information about the error. Refer to PR5600_PRINTER_SENSE structure member variables btAdditionalSenseCode and btAdditionalSenseCodeQualifier.
		BYTE	btAdditionalSenseCodeQualifier; // Indicates additional information about the error. Refer to PR5600_PRINTER_SENSE structure member variables btAdditionalSenseCode and btAdditionalSenseCodeQualifier.
		BYTE	btFieldReplaceableUnitCode;		// Indicates information of a faulty unit. Refer to PR5600_PRINTER_SENSE structure member variable btFieldReplaceableUnitCode.
	};
} PR5600_PRINTER_ERROR, *PPR5600_PRINTER_ERROR;


// PR5600_LOG structure
typedef struct _PR5600_LOG {
	CHAR					szUser[MAX_PATH + 4];		// Indicates the name of the user owner of the job.
	CHAR					szDocument[MAX_PATH + 4];	// Indicates the job's document name.
	CHAR					acTag[NSC_TAG_LEN];			// Indicates the tag number specified to the job.
	DWORD					dwPrinterID;				// Indicates the printer ID number.
	CHAR					acSerial[NSC_SERIALNO_LEN];	// Indicates a 7-byte printer serial number plus 1-byte additional information.
	SYSTEMTIME				timeBegin;					// Indicates the time the printing started (the time data was passed on from the spooler to the language monitor).
	SYSTEMTIME				timeEnd;					// Indicates the time the printing ended (the time the card was ejected).
	SYSTEMTIME				timeRecv;					// Indicates the time the language monitor received all job data from the spooler
	SYSTEMTIME				timeSend;					// Indicates the time the language monitor sent all job data to the printer.
	PR5600_PRINTER_STATUS	Status;						// The PR5600_PRINTER_STATUS structure storing the printer operational status and memory usage status.
	PR5600_PRINTER_ERROR	Error;						// The PR5600_PRINTER_ERROR structure storing the printer error information.
	DWORD					dwDataType;					// Indicates the type of the data in this log record. 0:image 1:text 2:Magnetic 3:IC R/W
	DWORD					dwErrorPage;				// Indicates the card number where an error occurred.
	CHAR					szError[256];				// Indicates the error message.
	DWORD					dwErrorCode;				// Indicates the error code.
	CHAR					szHandling[512];			// Indicates how to handle the error.
} PR5600_LOG, *PPR5600_LOG;


// PR5600_STATUS_INFO structure
typedef struct _PR5600_STATUS_INFO{
	BYTE	btHardwareStatus;							// Indicates the printer hardware status.
	BYTE	btCardPosition;								// Indicates the location of the card within the printer.
	BYTE	btPrinterStatus;							// Indicates the printer's printing status.
	BYTE	btEncoderStatus;							// Bits 0 to 3 indicate the operational status of the magnetic encoder unit while bits 4 and 7, the access status to the IC R/W.
	BYTE	btMemoryStatus[2];							// Indicates the usage status of memory A (btMemoryStatus[0]) and memory B (btMemoryStatus[1]).
	BYTE	btDataStatus[2];							// Indicates the print data reception status of memory A (btDataStatus[0]) and memory B (btDataStatus[1]).
	BYTE	btOptionUnitStatus;							// Indicates special statuses of optional units.
	BYTE	btTotalPrintNumber[3];						// Indicates the total of cards printed.
	BYTE	btColorControlValue[3];						// Indicates the saturation adjustment values. Indexes are: "0" for Yellow, "1" for Magenta, and "2" for Cyan.
	CHAR	acMagEncoderROMVersion[4];					// Indicates the magnetic encoder's ROM version.
	CHAR	acHeatRollerROMVersion[4];					// Indicates the heat roller unit's ROM version.
	BYTE	btICTagData[NSC_TAG_LEN];					// Indicates the tag information of the card at the access position of IC R/W.
	BYTE	btReserved;									// Not used
} PR5600_STATUS_INFO, *PPR5600_STATUS_INFO;



// PR5600_PRINTER_SENSE structure
typedef struct _PR5600_PRINTER_SENSE{
	BYTE	btErrorCode;								// Stores a fixed value (11110000).
	BYTE	btSegmentNumber;							// Stores a fixed value (00000000).
	BYTE	btSenseKey;									// SenseKey
	BYTE	btInfomation[4];							// Indicates the difference between the byte length requested by the command and the transferred data length. This is a 4-byte big-endian value.
	BYTE	btAdditionalSenseLength;					// Indicates the number of bytes of the additional sense data. Fixed to "0B" (hex).
	BYTE	btCommandSpecificInformation[4];			// All bytes fixed to "0".
	BYTE	btAdditionalSenseCode;						// Additional information about the error.
	BYTE	btAdditionalSenseCodeQualifier;				// Additional information about the error.
	BYTE	btFieldReplaceableUnitCode;					// Information about the faulty unit.
	BYTE	btSenseKeySpecific[3];						// Fixed to "0".
	BYTE	btAdditionalSenseBytes;						// Fixed to "0".
	BYTE	btErrorClassCode;							// Error class code managed by the printer.
	BYTE	btAdditionalErrorCode;						// Detailed error code managed by the printer.
	BYTE	btErrorUnitCode;							// A code that indicates the unit managing "btErrorClassCode" and "btAdditionalErrorCode".
} PR5600_PRINTER_SENSE, *PPR5600_PRINTER_SENSE;


// PR5600_SYSTEM_INFO structure
typedef struct _PR5600_SYSTEM_INFO{
	BYTE	btConfiguration[2];							// Indicates the printer configuration information. Each bit indicates whether the option is connected (1) or not connected (0).
	BYTE	btRibbonSetting;							// Indicates the type of ribbon selected.
	BYTE	btRibbonPanel;								// Indicates the type of panel (type of data) that can be printed with the selected type of ribbon.
	BYTE	btReserved1;								// Not used.
	BYTE	btMemoryMode;								// Indicates the memory management method. Fixed to "02" (hex).
	BYTE	btPrinterSetting;							// Indicates settings related to the function for printing cards.
	BYTE	btReserved2;								// Not used.
	WORD	wdMaxHorizontalSizeImage;					// Indicates the size of the area where image data can be printed.
	WORD	wdMaxVirticalSizeImage;						// Indicates the size of the area where image data can be printed.
	WORD	wdMaxHorizontalSizeText;					// Indicates the size of the area where text data can be printed.
	WORD	wdMaxVirticalSizeText;						// Indicates the size of the area where text data can be printed.
	WORD	wdMaxHorizontalSizeOverlay;					// Indicates the size of the area where protection layer data can be printed.
	WORD	wdMaxVirticalSizeOverlay;					// Indicates the size of the area where protection layer data can be printed.
	WORD	wdStartHorizontalPositionImage;				// Indicates the starting position of the area where image data can be printed.
	WORD	wdStartVirticalPositionImage;				// Indicates the starting position of the area where image data can be printed.
	WORD	wdStartHorizontalPositionText;				// Indicates the starting position of the area where text data can be printed.
	WORD	wdStartVirticalPositionText;				// Indicates the starting position of the area where text data can be printed.
	WORD	wdStartHorizontalPositionOverlay;			// Indicates the starting position of the area where protection layer data can be printed.
	WORD	wdStartVirticalPositionOverlay;				// Indicates the starting position of the area where protection layer data can be printed.
	WORD	wdResolution;								// Indicates the printer resolution in [dpi].
	CHAR	acBaseRevision[4];							// Indicates the base program version.
	CHAR	acMainRevision[4];							// Indicates the main program version.
	CHAR	acSerialNumber[8];							// Indicates the printer serial number.
	CHAR	cPrinterID;									// Indicates the printer ID number.
} PR5600_SYSTEM_INFO, *PPR5600_SYSTEM_INFO;


// PRNINFO4ICREADY structure
typedef struct _PRNINFO4ICREADY {
	DWORD	dwInternalID;								// Indicates the internal ID for identification used by the printer driver.
	DWORD	dwPrinterID;								// Indicates the ID (Printer ID) specified in the printer itself.
	WCHAR	wszName[264];								// Printer name (UTF-16 character string) of the printer folder. It is a name the user can change.
	CHAR	acTag[NSC_TAG_LEN];							// Tag number set to each card to be printed by the application.
} PRNINFO4ICREADY, *PPRNINFO4ICREADY;


#pragma pack()




#ifdef APIF_EXPORTS
#define DLLFUNC __declspec(dllexport) WINAPI
#else
#define DLLFUNC __declspec(dllimport) WINAPI
#endif

#ifdef __cplusplus
extern "C" {
#endif


/********************************************************/
/*	Direct communication function						*/
/********************************************************/

PR5600RESULT DLLFUNC
pr56XXEnumPrinters(
	PVOID	pInfo,
	DWORD	dwSize,
	PDWORD	pdwNeeded,
	PDWORD	pdwReturned
);

PR5600RESULT DLLFUNC
pr56XXEnumEnabledPrinters(
	PVOID	pInfo,
	DWORD	dwSize,
	PDWORD	pdwNeeded,
	PDWORD	pdwReturned
);

VOID DLLFUNC
pr56XXTerminate(
	VOID
);

PR5600HANDLE DLLFUNC
pr56XXOpenPrinter(
	DWORD	dwInternalID,
	DWORD	dwPrinterID,
	PCWSTR	wszPrinterName,
	HWND	hWnd,
	UINT	Msg
);

PR5600RESULT DLLFUNC
pr56XXClosePrinter(
	PR5600HANDLE	hPrinter
);

PR5600RESULT DLLFUNC
pr56XXRegisterMessage(
	PR5600HANDLE		hPrinter,
	DWORD			dwcount,
	PR5600APPNOTIFY		*aNotifies
);

PR5600RESULT DLLFUNC
pr56XXGetLastError(
	PR5600HANDLE	hPrinter
);

PR5600RESULT DLLFUNC
pr56XXGetLogs(
	DWORD			dwInternalID,
	DWORD			dwPrinterID,
	PCWSTR			wszPrinterName,
	DWORD			dwLogs,
	PDWORD			pdwReturned,
	PPR5600_LOG		pLog
	);

PR5600RESULT DLLFUNC
pr56XXGetPrinterSystemInformation(
	PR5600HANDLE			hPrinter,
	PPR5600_SYSTEM_INFO		pInfo
	);

PR5600HANDLE DLLFUNC
pr56XXGetSysInfoOnly(
	PCWSTR				wszPrinterName,
	PPR5600_SYSTEM_INFO		pInfo
);

PR5600RESULT DLLFUNC
pr56XXGetPrinterStatusInformation(
	PR5600HANDLE			hPrinter,
	PPR5600_STATUS_INFO		pInfo,
	PPR5600_PRINTER_SENSE		pSense
);

PR5600RESULT DLLFUNC
pr56XXGetPrinterData(
	PR5600HANDLE	hPrinter,
	PBYTE		pBuffer,
	DWORD		dwSize,
	PDWORD		pdwReceived
);

DWORD DLLFUNC
pr56XXGetBusyTimeout(
	PR5600HANDLE	hPrinter
);

DWORD DLLFUNC
pr56XXSetBusyTimeout(
	PR5600HANDLE	hPrinter,
	DWORD		dwTimeout
);

PR5600RESULT DLLFUNC
pr56XXPrint(
	PR5600HANDLE	hPrinter,
	PBYTE		pBuffer,
	DWORD		dwSize
);

PR5600RESULT DLLFUNC
pr56XXPrint_SetICEncoder(
	PR5600HANDLE	hPrinter,
	DWORD		dwAction,
	BOOL		bContact
);

PR5600RESULT DLLFUNC
pr56XXGetPrinterStatusInformationEx(
	PR5600HANDLE			hPrinter,
	PPR5600_STATUS_INFO2	pInfo,
	DWORD                   dwInfoSize,
	PDWORD					pdwReceivedInfoSize,		// #318
	PPR5600_PRINTER_SENSE	pSense,
	DWORD                   dwSenseSize,
	PDWORD					pdwReceivedSenseSize		// #318
);

PR5600RESULT DLLFUNC
pr56XXPrint(
	PR5600HANDLE	hPrinter,
	PBYTE		pBuffer,
	DWORD		dwSize
);

#ifdef __cplusplus
}
#endif


#endif
