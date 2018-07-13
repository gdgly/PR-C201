#pragma once
#define INVALID_SW1SW2 -450
#define IOCTL_SMARTCARD_ACR128_ESCAPE_COMMAND SCARD_CTL_CODE(2079)
#define IOCTL_SMARTCARD_ACR1251_ESCAPE_COMMAND SCARD_CTL_CODE(3500)
#define BLACK RGB(0, 0, 0)
#define RED RGB(255, 0, 0)
#define GREEN RGB(0, 0x99, 0)

#include "WinSCard.h"

class CReader
{
public:
	CReader(void);
	~CReader(void);
 public:
	DWORD		SendLen, RecvLen, ByteRet;;
	CString     m_strControlRet;
	BYTE		SendBuff[1024], RecvBuff[1024];
	int			retCode;
	CString		m_strReaderName;
	SCARDHANDLE	m_hCard	   ;
    SCARD_IO_REQUEST		IO_REQ;
	BYTE		m_bShareConected;
	BOOL		m_bOpened;


public:
	void ClearBuffers()	;
	int OnStartBuzz() ;
	int OnStopBuzz();
	int OnSpecialControl(SCARDCONTEXT hContext,CString command,CString &get) ;
	int OnBuzz(SCARDCONTEXT hContext,unsigned short ms)   ;
	int SetPPS()	 ;
	int GetATS(char*Ats) ;
	int InnerReadPoll(BYTE &set ) ;
	int InnerSetPoll(BYTE set ) ;
	
	int ModifyPoll(SCARDCONTEXT hContext,BOOL type)   ;

	unsigned short ConnectCard(SCARDCONTEXT hContext,byte* ivCardType) ;
	unsigned short GetCardAtr1(SCARDCONTEXT hContext,char*srAtr);
	unsigned short GetCardAtr2(SCARDCONTEXT hContext,char*srAtr);
	unsigned short GetCardSn(SCARDCONTEXT hContext,char*srABCardPhySn);
	unsigned short GetCardPhySnAtr(SCARDCONTEXT hContext,byte*cardtype,char*srABCardPhySn,char*srAtr) ;
	
	int	 CallCardControl();
	BOOL ItIsMe(HANDLE devNo ,bool bSAM);
	LONG	CloseReader(HANDLE devNo);
	int		OnOpenContextless(SCARDCONTEXT hContext);
	void    SetOpenState(byte iReadGroupNum,BOOL open);
	unsigned short SendApduHex(SCARDCONTEXT hContext,char* apduData,int apduLen,char* respData,int* respLen );
	unsigned short SendApduChar(SCARDCONTEXT hContext,char* apduData,char* respData);
	unsigned short  GetScardErrCode(int code) ;
};
