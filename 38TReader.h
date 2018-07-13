#pragma once
#include "WinSCard.h"
#include "md5module.h"

#define INVALID_SW1SW2 -450
#define IOCTL_SMARTCARD_ACR128_ESCAPE_COMMAND SCARD_CTL_CODE(2079)
#define IOCTL_SMARTCARD_ACR1251_ESCAPE_COMMAND SCARD_CTL_CODE(3500)
#define BLACK RGB(0, 0, 0)
#define RED RGB(255, 0, 0)
#define GREEN RGB(0, 0x99, 0)


class CReader38T
{
public:
	CReader38T(void);
	~CReader38T(void);
 public:
	DWORD		SendLen, RecvLen, ByteRet;;
	CString     m_strControlRet;
	BYTE		SendBuff[1024], RecvBuff[1024];
	int			retCode;
	CString		m_strReaderName;

    SCARDCONTEXT	m_hContext ;
	SCARDHANDLE	m_hCard	   ;
    SCARD_IO_REQUEST		IO_REQ;
	BYTE		m_bShareConected;
	BOOL		m_bOpened;
public:
    BOOL IsMyPrinter(char* buff=0);
    BOOL InitialReader() ;
    BOOL OnGetPassWord(CString data,char* buff=0);


public:
	void ClearBuffers()	;
	int OnStartBuzz() ;
	int OnStopBuzz();
	int OnSpecialControl(CString command,CString &get) ;
	int OnBuzz(unsigned short ms)   ;
	int SetPPS()	 ;
	int GetATS(char*Ats) ;
	int InnerReadPoll(BYTE &set ) ;
	int InnerSetPoll(BYTE set ) ;
	
	int ModifyPoll(SCARDCONTEXT hContext,BOOL type)   ;

	unsigned short ConnectCard(byte* ivCardType) ;
	unsigned short GetCardAtr1(SCARDCONTEXT hContext,char*srAtr);
	unsigned short GetCardAtr2(SCARDCONTEXT hContext,char*srAtr);
	unsigned short GetCardSn(SCARDCONTEXT hContext,char*srABCardPhySn);
	unsigned short GetCardPhySnAtr(SCARDCONTEXT hContext,byte*cardtype,char*srABCardPhySn,char*srAtr) ;
	
	int	 CallCardControl();
	BOOL ItIsMe(HANDLE devNo ,bool bSAM);
	LONG	CloseReader();
	int		OnOpenContextless();
	void    SetOpenState(byte iReadGroupNum,BOOL open);
	unsigned short SendApduHex(char* apduData,int apduLen,char* respData,int* respLen );
	unsigned short SendApduChar(char* apduData,char* respData);
	unsigned short  GetScardErrCode(int code) ;

};
