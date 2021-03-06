#include "stdafx.h"
#include "38TReader.h"

	
int __stdcall WriteLog(CString szMsg1)
{
	//LPCTSTR szFile = "C:\\log.txt";
	//BYTE curTime[10];
	//FILE* hfile = fopen(szFile,_T("at+"));
	//if(hfile)
	//{
	//	fseek(hfile,0,SEEK_END);
	//	_fprintf_p(hfile,"%s: %s\n",szFile,szMsg1.GetBuffer(0));
	//	fclose(hfile);
	//}
	return 0;
}
;


CReader38T::CReader38T(void)
{
	m_hCard = 0;

	IO_REQ.dwProtocol = 0;
	IO_REQ.cbPciLength = 0;

	m_bShareConected = 0;
	m_bOpened = FALSE;

    m_hContext = 0;
   // m_strReaderName = CString("ACS CCID USB Reader 0");
   // m_strReaderName = CString("ACS CCID USB Reader 0");
   // m_strReaderName = CString("ACS ACR1281 1S Dual Reader SAM 0");
	                         //ACS ACR1281 1S Dual Reader SAM 0
	 m_strReaderName = CString("M&W R6-C042-SAM2Slot  0");
}

BOOL CReader38T::InitialReader()
{
	try
	{
		if(m_hContext == 0)
		{
			LONG retCode = ::SCardEstablishContext(SCARD_SCOPE_USER,NULL,NULL,&m_hContext);	
			if( retCode == SCARD_S_SUCCESS )
			{
				return true;
			}   ;
		}
        else
            return true;
	}
	catch(...)
	{
		AfxMessageBox(_T("您没有安装智能卡读写器ACS38T的驱动，或开启智能卡服务！"));
		return false;
	}
	return false;
}
CReader38T::~CReader38T(void)
{
}

void CReader38T::ClearBuffers()
{
	int index;
	//for( index = 0; index <= 262; index++ )
	{
		memset(SendBuff,0,262);
		memset(RecvBuff,0,262);
		//SendBuff[index] = 0x00;
		//RecvBuff[index] = 0x00;
	}
}

 int CReader38T::OnSpecialControl(CString command,CString& get) 
{
	USES_CONVERSION;
	byte ivCardType;
	int ret = 0;
	if(m_hCard==NULL)
	{
		ret = ConnectCard( &ivCardType);
		if(ret!= 0x9000)
		{
			return ret;
		}
	}
	if(m_hCard)
	{
		get = CString("");
		ClearBuffers();
		command.Replace(L" ",L"");
		command.Replace(L"	",L"");
		command.Replace(L"-",L"");
		command.Replace(L"\\",L"");
		command.Replace(L"/",L"");

		int comLen = command.GetLength()/2;
		for(int i=0;i<comLen;i++)
		{
			CString mid = command.Mid(i*2,2);
			int Hex =0;
			sscanf(W2A(mid.GetBuffer(0)),"%02x",&Hex);
			SendBuff[i] = Hex;
		}
		SendLen = comLen;
		RecvLen = 255;
		ByteRet = 0;
		retCode = CallCardControl();
		if(ByteRet>2)
		{
			for(int i=0;i<ByteRet;i++)
			{ 
				char temp[16];
				sprintf(temp,"%02x",RecvBuff[i]);
				get += CString(temp);
			}
		}
		else
		{
			char temp[16];
			sprintf(temp,"%04x",RecvBuff);
			get += CString(temp);
		}
	}
	return retCode;
}

 int CReader38T::OnStartBuzz() 
{
	ClearBuffers();
	SendBuff[0] = 0xE0;
	SendBuff[1] = 0x00;
	SendBuff[2] = 0x00;
	SendBuff[3] = 0x28;
	SendBuff[4] = 0x01;
	SendBuff[5] = 0xFF;
	SendLen = 6;
	RecvLen = 6;
	
	retCode = CallCardControl();
	return retCode;
}

int CReader38T::OnStopBuzz() 
{
	ClearBuffers();
	SendBuff[0] = 0xE0;
	SendBuff[1] = 0x00;
	SendBuff[2] = 0x00;
	SendBuff[3] = 0x28;
	SendBuff[4] = 0x01;
	SendBuff[5] = 0x00;
	SendLen = 3;
	RecvLen = 6;
	
	retCode = CallCardControl();
	return retCode;
}	 ;
int CReader38T::InnerSetPoll(BYTE set ) 
{
	
	ClearBuffers();
	SendBuff[0] = 0x23;
	SendBuff[1] = 0x01;
	SendBuff[2] = set;
	SendLen = 3;
	RecvLen = 7;
	retCode = CallCardControl();
	return retCode; 
	
};
int  CReader38T::InnerReadPoll(BYTE &set ) 
{
	
	ClearBuffers();
	SendBuff[0] = 0x23;
	SendBuff[1] = 0x00;
	SendLen = 2;
	RecvLen = 6;
	retCode = CallCardControl();
	set = RecvBuff[5] ;
	return retCode; 
	
};

int CReader38T::ModifyPoll(SCARDCONTEXT hContext,BOOL type)
{
	if(m_bOpened==FALSE)
		return 0xff01;

	byte ivCardType;
	int ret = 0;
	if(m_hCard==NULL)
	{
		ret = ConnectCard( &ivCardType);
		if(ret!= 0x9000)
		{
			return ret;
		}
	}
	if(m_hCard)
	{
		if(type)
		{
			BYTE set;
			int ret = InnerReadPoll(set );
			if(SCARD_S_SUCCESS == ret)
			{
				ret = InnerSetPoll(0x80 | set ) ;
			}
		}
		else
		{
			BYTE set;
			int ret = InnerReadPoll(set );
			if(SCARD_S_SUCCESS == ret)
			{			
				ret = InnerSetPoll(0x7F & set ) ;
			}	
		}	
		if(ret == SCARD_S_SUCCESS)
			return 0x9000;
		else
		{
			return GetScardErrCode(ret)	;
		}
	}
	else
		return 0xff01	;
}   ;

void	CReader38T::SetOpenState(byte iReadGroupNum,BOOL open)
{
	char c[10];memset(c,0,10);
	itoa((BYTE)iReadGroupNum,c,10);
	if(m_strReaderName.Find(c[0],0)>0)
	{
		m_bOpened = open;
	}
};

int CReader38T::OnOpenContextless()
{
	if(m_bOpened==FALSE)
		return 0xff01;

	byte ivCardType;
	int ret = 0;
	if(m_hCard==NULL)
	{
		ret = ConnectCard( &ivCardType);
		if(ret!= 0x9000)
		{
			return ret;
		}
	}
	if(m_hCard)
	{
		ClearBuffers();

		SendBuff[0] = 0x23;
		SendBuff[1] = 0x01;
		SendBuff[2] = 0;
		SendBuff[2] |=	 0x01 ;//1
		SendBuff[2] |=	 0x08 ;//4
		SendBuff[2] |=	 0x10 ;//0.5 S
		SendBuff[2] |=	 0x80 ;//6
		SendLen = 3;
		RecvLen = 7;
			
		retCode = CallCardControl();
		char msg[64]={0};
		sprintf(msg,"%02x",retCode);
		WriteLog(CString(_T("OnOpenContextless: ")+CString(msg)));
		if(ret == SCARD_S_SUCCESS)
			return 0x9000;
		else
		{
			return GetScardErrCode(ret)	;
		}
	}
	else
		return 0xff01	;
}

int CReader38T::OnBuzz(unsigned short ms)
{
	if(m_bOpened==FALSE)
		return 0xff01;

	byte ivCardType;
	int ret = 0;
	if(m_hCard==NULL)
	{
		ret = ConnectCard( &ivCardType);
		if(ret!= 0x9000)
		{
			return ret;
		}
	}
	if(m_hCard)
	{
		int ret = OnStartBuzz();
		if(SCARD_S_SUCCESS == ret)
		{
			Sleep(ms);
		}
		ret = OnStopBuzz();
		if(ret == SCARD_S_SUCCESS)
			return 0x9000;
		else
		{
			return GetScardErrCode(ret)	;
		}
	}
	else
		return 0xff01	;
}
int CReader38T::SetPPS() 
{
//0xE0 0x00 0x00 0x24 0x02
	ClearBuffers();
	SendBuff[0] = 0xE0;
	SendBuff[1] = 0x00;
	SendBuff[2] = 0x00;
	SendBuff[3] = 0x24;
	SendBuff[4] = 0x02;
	SendBuff[5] = 0x02;
	SendBuff[6] = 0x02;
	SendLen = 7;
	RecvLen = 0xFF;

	retCode = CallCardControl();
	
	return retCode;
}

int CReader38T::GetATS(char*Ats) 
{
	ClearBuffers();
	SendBuff[0] = 0xFF;
	SendBuff[1] = 0xCA;
	SendBuff[2] = 0x01;
	SendBuff[3] = 0x00;
	SendBuff[4] = 0x00;
	SendLen = 5;
	RecvLen = 16;

	retCode = CallCardControl();
	
	return retCode;
}

unsigned short CReader38T::ConnectCard(byte* ivCardType) 
{
	DWORD Protocol = 3;

	if(m_hCard)
	{
		retCode = SCardDisconnect(m_hCard, SCARD_UNPOWER_CARD);
	}
	m_hCard = 0;
    m_bShareConected = 0;
	//char *name = new char[m_strReaderName.GetLength()];
	//memset(name,0,m_strReaderName.GetLength());
	//strcpy(name,m_strReaderName.GetBuffer(0));
	//char name[256]={0};
	//memcpy(name,m_strReaderName.GetBuffer(0),m_strReaderName.GetLength());
	retCode = SCardConnect( m_hContext,
							m_strReaderName.GetBuffer(0),
							SCARD_SHARE_SHARED,
							Protocol,
							&m_hCard,
							&Protocol );

	if( retCode != SCARD_S_SUCCESS )
	{
		//memset(name,0,m_strReaderName.GetLength());
		//strcpy(name,m_strReaderName.GetBuffer(0));
		retCode = SCardConnect( m_hContext,
								m_strReaderName.GetBuffer(0),
								SCARD_SHARE_DIRECT,
								0,
								&m_hCard,
								&Protocol );

		if( retCode != SCARD_S_SUCCESS )
		{
			//delete name;
			//name = NULL;
			return GetScardErrCode(retCode)	;
		}
		else
        {
			m_bShareConected = 1;
            m_bOpened = true;
        }
		
	}
	else
    {
		m_bShareConected = 2;
        m_bOpened = true;
    }


	IO_REQ.dwProtocol = Protocol;
	IO_REQ.cbPciLength = sizeof( SCARD_IO_REQUEST );
	*ivCardType = Protocol;

	//delete name;
	//name = NULL;
	return 	 GetScardErrCode(retCode)		  ;;

}
unsigned short CReader38T::GetCardAtr1(SCARDCONTEXT hContext,char*srAtr)
{
	DWORD     dwState, dwProtocol;
	BYTE    bAtr[32]; //ATR
	DWORD     bAtrLen = 32;
	DWORD cch = SCARD_AUTOALLOCATE;
	memset(bAtr,0,32);
	CString read = m_strReaderName;

	BYTE bSend[1024];memset(bSend,0,1024);
	BYTE bRecv[1024];memset(bRecv,0,1024);
	DWORD dwSend = 10;
	DWORD dwRecv = 260;
	retCode = SCardStatus(m_hCard,read.GetBuffer(0),&cch,&dwState,&dwProtocol,(LPBYTE)&bRecv,&dwRecv);
	if(	dwRecv>2 && retCode == SCARD_S_SUCCESS)
	{
		if(srAtr>0)
			memcpy(srAtr,bRecv,dwRecv-2);
		return 0x9000;
	}
	else
		return 0xff0f;
}
;
unsigned short CReader38T::GetCardAtr2(SCARDCONTEXT hContext,char*srAtr)
{
	DWORD     dwState, dwProtocol;
	BYTE    bAtr[32]; //ATR
	DWORD     bAtrLen = 32;
	DWORD cch = SCARD_AUTOALLOCATE;
	memset(bAtr,0,32);
	CString read = m_strReaderName;

	BYTE bSend[1024];memset(bSend,0,1024);
	BYTE bRecv[1024];memset(bRecv,0,1024);
	DWORD dwSend = 10;
	DWORD dwRecv = 260;
	bSend[0] = 0xFF;		 //80ca000009
	bSend[1] = 0xCA;
	bSend[2] = 0x01;
	bSend[3] = 0x00;
	bSend[4] = 0x00;
	dwSend = 5;

	retCode = SCardTransmit(m_hCard,
	&IO_REQ,
	bSend,
	dwSend,
	NULL,
	bRecv,
	&dwRecv );
	if(dwRecv>=2 && retCode == SCARD_S_SUCCESS) 
	{
		if(srAtr>0)
		{
			memcpy(srAtr,  bRecv, dwRecv-2);
		}
		return 0x9000;
		//if((dwRecv-2)==12)
			//*cardtype   = 1;
	}
	else
 		return 0xff0f;
}
;
unsigned short CReader38T::GetCardSn(SCARDCONTEXT hContext,char*srABCardPhySn)
{
	DWORD     dwState, dwProtocol;
	BYTE    bAtr[32]; //ATR
	DWORD     bAtrLen = 32;
	DWORD cch = SCARD_AUTOALLOCATE;
	memset(bAtr,0,32);
	CString read = m_strReaderName;

	BYTE bSend[1024];memset(bSend,0,1024);
	BYTE bRecv[1024];memset(bRecv,0,1024);
	DWORD dwSend = 10;
	DWORD dwRecv = 260;
	bSend[0] = 0xFF;		 //80ca000009
	bSend[1] = 0xCA;
	bSend[2] = 0x00;
	bSend[3] = 0x00;
	bSend[4] = 0x00;
	dwSend = 5;

	retCode = SCardTransmit(m_hCard,
	&IO_REQ,
	bSend,
	dwSend,
	NULL,
	bRecv,
	&dwRecv );
	if(dwRecv>=2 && retCode == SCARD_S_SUCCESS) 
	{
		if(srABCardPhySn>0)
			memcpy(srABCardPhySn,bRecv,4);
		return 0x9000;
	}
	return 0xff0f;
}
;

unsigned short CReader38T::GetCardPhySnAtr(SCARDCONTEXT hContext,byte*cardtype,char*srABCardPhySn,char*srAtr)
{
    *cardtype = 0;
	char myAtr1[1024];memset(myAtr1,0,1024);
	char myAtr2[1024];memset(myAtr2,0,1024);

	unsigned short ret1 = GetCardAtr1( hContext,myAtr1);
	unsigned short ret2 = GetCardAtr2( hContext,myAtr2);
	unsigned short ret3 = GetCardSn( hContext,srABCardPhySn);

	if(ret2==0x9000)
	{
		int l = strlen(myAtr2);
		memcpy(srAtr,myAtr2,l);
		if(l==12)
			*cardtype = 1;
		return 0x9000;
	}
	else if(ret1 == 0x9000)
	{
		memcpy(srAtr,myAtr1,strlen(myAtr1));
		return 0x9000;
	}
	else
		return GetScardErrCode(ret1)		  ;

   

};
 int CReader38T::CallCardControl()
{	
	retCode = SCardControl( m_hCard,
							IOCTL_SMARTCARD_ACR1251_ESCAPE_COMMAND,
							&SendBuff,
							SendLen,
							RecvBuff,
							RecvLen,
							&ByteRet );

	return retCode;
}


 BOOL CReader38T::ItIsMe(HANDLE devNo ,bool bSAM)
{
	char c[10];memset(c,0,10);
	itoa((BYTE)devNo,c,10);

	if(m_strReaderName.GetLength()>0)
	{
		if(m_strReaderName.Find(c[0],0)>0)
		{
			if(!bSAM)
			{
				if(m_strReaderName.Find(_T("PICC"),0)>0)
				//if(m_strReaderName.Find(_T("FW_Upgrade"),0)>0)
				{
					return TRUE;
				}
			}
			else
			{
				if(m_strReaderName.Find(_T("SAM"),0)>0)
				{
					return TRUE;
				}				
			}
		}
	}
	return FALSE;
 };


LONG CReader38T::CloseReader()
{
	if(m_hCard == 0)
	{
		m_bOpened = FALSE;
		m_bShareConected = 0;
		return 0x9000;
	}

	LONG  ret = SCardDisconnect( m_hCard, SCARD_UNPOWER_CARD );
	m_hCard = 0;
	m_bOpened = FALSE;
	m_bShareConected = 0;
	return 0x9000;

	return  GetScardErrCode(ret);
};
  unsigned short  CReader38T:: SendApduHex(char* apduData,int apduLen,char* respData,int* respLen )
 {
	if(m_bOpened==FALSE)
		return 0xff01;
	DWORD	dwRet = 0;
	if(m_bShareConected == 1 || m_hCard == NULL)
	{
		byte ivCardType = 0;
		dwRet = ConnectCard(&ivCardType);

		if(dwRet!=0x9000)
			return dwRet;
	}
	if(m_bShareConected == 1)
		return 0xFF0F;
	if(m_hCard == NULL)
		return 0xff01;

 	BYTE	icRead[1024]={0},icWrite[1024]={0};
	DWORD	res  = 0;
	DWORD	dwRecv = 0;


	dwRecv = 1024;//sizeof(icWrite);

	dwRet = SCardTransmit(m_hCard, &IO_REQ, (LPCBYTE)apduData, apduLen,
						NULL, icWrite, &dwRecv);
	if (SCARD_S_SUCCESS != dwRet)
	{
		char res1[64]={0};
		sprintf(res1,"SCardTransmit Return:%d,dwRecv%d",dwRet,dwRecv);
		WriteLog(CString(res1));
		return GetScardErrCode(dwRet)	;
	}
	else if(icWrite[dwRecv-2] == 0x61)
	{
		icRead[0] = 0x00;
		icRead[1] = 0xC0;
		icRead[2] = 0x00;
		icRead[3] = 0x00;
		icRead[4] = icWrite[dwRecv-1];
		dwRecv = sizeof(icWrite);

		dwRet = SCardTransmit(m_hCard, &IO_REQ, icRead, 5,
							NULL, icWrite, &dwRecv);
		if (SCARD_S_SUCCESS != dwRet)
		{
			return  GetScardErrCode(dwRet);
		}
	}
	*respLen =	 dwRecv - 2;
	memcpy(respData, icWrite, dwRecv - 2);
	if(dwRecv<1024|| dwRecv>2)
	{
		icRead[0]=icWrite[dwRecv-1];
		icRead[1]=icWrite[dwRecv-2];
		memcpy((DWORD*)&res,&icRead[0],2);
	}
	
	return res;
 }
 
  unsigned short   CReader38T::SendApduChar(char* apduCommd,char* respData)
 {

     USES_CONVERSION;

	 respData[0] = 0;
 	if(m_bOpened==FALSE)
		return 0xff01;

	DWORD dwRet = 0;
 	if(m_bShareConected == 1 || m_hCard == NULL)
	{
		byte	ivCardType = 0;
		dwRet = ConnectCard(&ivCardType);
		if(dwRet!=0x9000)
			return dwRet;
	}
	if(m_bShareConected == 1)
		return 0xFF0F;
	if(m_hCard == NULL)
		return 0xff01;

    char apduData[1024]={0};

    CString cmd = CString(apduCommd);
    cmd.Replace(L" ",L"");
    cmd.Replace(L"/",L"");
    cmd.Replace(L"\\",L"");

    strcpy(apduData, W2A(cmd.GetBuffer(0)));

	int apduLen	= strlen(apduData);

	char hexApdu[1024*4];
	memset(hexApdu,0,1024*4);
	
	for(int i =0;i<	apduLen;)
	{
		char str[2] ={0};
		int high = 0;
		str[0] =  apduData[i++];
		sscanf(str,"%x",&high);
		int low = 0;
		str[0] =  apduData[i++];
		sscanf(str,"%x",&low);
		hexApdu[i/2-1] = high*16 + low;
	};

	char hexrespData[1024*4]={0};	int respLen = 0	 ;
	unsigned short ret = SendApduHex(hexApdu,apduLen/2,hexrespData,&respLen ) ;

	char str[16] ;
	for(int i = 0;i<respLen;i++)
	{
		memset(str,0,4);
		BYTE data = hexrespData[i];
		sprintf(str,"%02x" ,data);
		strcat(respData,str);
	}
	return ret;
 }



  unsigned short  CReader38T::GetScardErrCode(int code)
{
	switch( code )
	{
	// Smartcard Reader interface errors
	case SCARD_E_CANCELLED:
		return 0xFF80;
		//return ("The action was canceled by an SCardCancel request.\n");
		break;
	case SCARD_E_CANT_DISPOSE:
		return 0xFF81;
		//return ("The system could not dispose of the media in the requested manner.\n");
		break;
	case SCARD_E_CARD_UNSUPPORTED:
		return 0xFF82;
		//return ("The smart card does not meet minimal requirements for support.\n");
		break;
	case SCARD_E_DUPLICATE_READER:
		return 0xFF83;
		//return ("The reader driver didn't produce a unique reader name.\n");
		break;
	case SCARD_E_INSUFFICIENT_BUFFER:	//0xff0c 参数范围错
		return 0xFF84;
		//return ("The data buffer for returned data is too small for the returned data.\n");
		break;
	case SCARD_E_INVALID_ATR:
		return 0xFF85;
		//return ("An ATR string obtained from the registry is not a valid ATR string.\n");
		break;
	case SCARD_E_INVALID_HANDLE:
		return 0xFF86;
		//return ("The supplied handle was invalid.\n");
		break;
	case SCARD_E_INVALID_PARAMETER://0xffoc	  参数范围错
		return 0xFF87;
		//return ("One or more of the supplied parameters could not be properly interpreted.\n");
		break;
	case SCARD_E_INVALID_TARGET:
		return 0xFF88;
		//return ("Registry startup information is missing or invalid.\n");
		break;
	case SCARD_E_INVALID_VALUE:		//0xff08   非法读卡器指令
		return 0xFF89;
		//return ("One or more of the supplied parameter values could not be properly interpreted.\n");
		break;
	case SCARD_E_NOT_READY:	//0xff15 卡无响应
		return 0xFF8A;
		//return ("The reader or card is not ready to accept commands.\n");
		break;
	case SCARD_E_NOT_TRANSACTED:
		return 0xFF8B;
		//return ("An attempt was made to end a non-existent transaction.\n");
		break;
	case SCARD_E_NO_MEMORY:
		return 0xFF8C;
		//return ("Not enough memory available to complete this command.\n");
		break;
	case SCARD_E_NO_SERVICE:
		return 0xFF8D;
		//return ("The smart card resource manager is not running.\n");
		break;
	case SCARD_E_NO_SMARTCARD:	//0xff0f 卡不存在
		return 0xFF8E;
		//return ("The operation requires a smart card, but no smart card is currently in the device.\n");
		break;
	case SCARD_E_PCI_TOO_SMALL:
		return 0xFF8F;
		//return ("The PCI receive buffer was too small.\n");
		break;
	case SCARD_E_PROTO_MISMATCH:	 //0xff0b 通讯错误
		return 0xFF90;
		//return ("The requested protocols are incompatible with the protocol currently in use with the card.\n");
		break;
	case SCARD_E_READER_UNAVAILABLE:   //0xff0d 非法读卡器
		return 0xFF91;
		//return ("The specified reader is not currently available for use.\n");
		break;
	case SCARD_E_READER_UNSUPPORTED:  //0xff0d 非法读卡器
		return 0xFF92;
		//return ("The reader driver does not meet minimal requirements for support.\n");
		break;
	case SCARD_E_SERVICE_STOPPED:
		return 0xFF93;
		//return ("The smart card resource manager has shut down.\n");
		break;
	case SCARD_E_SHARING_VIOLATION:
		return 0xFF94;
		//return ("The smart card cannot be accessed because of other outstanding connections.\n");
		break;
	case SCARD_E_SYSTEM_CANCELLED:
		return 0xFF95;
		//return ("The action was canceled by the system, presumably to log off or shut down.\n");
		break;
	case SCARD_E_TIMEOUT:	  //0xff09 读卡器超时
		return 0xFF96;
		//return ("The user-specified timeout value has expired.\n");
		break;
	case SCARD_E_UNKNOWN_CARD:
		return 0xFF97;
		//return ("The specified smart card name is not recognized.\n");
		break;
	case SCARD_E_UNKNOWN_READER:
		return 0xFF98;
		//return ("The specified reader name is not recognized.\n");
		break;
	case SCARD_F_COMM_ERROR:
		return 0xFF99;
		//return ("An internal communications error has been detected.\n");
		break;
	case SCARD_F_INTERNAL_ERROR:
		return 0xFF9A;
		//return ("An internal consistency check failed.\n");
		break;
	case SCARD_F_UNKNOWN_ERROR:
		return 0xFF9B;
		//return ("An internal error has been detected, but the source is unknown.\n");
		break;
	case SCARD_F_WAITED_TOO_LONG:
		return 0xFF9C;
		//return ("An internal consistency timer has expired.\n");
		break;
	case SCARD_W_REMOVED_CARD:
		return 0xFF9D;
		//return ("The smart card has been removed and no further communication is possible.\n");
		break;
	case SCARD_W_RESET_CARD:
		return 0xFF9E;
		//return ("The smart card has been reset, so any shared state information is invalid.\n");
		break;
	case SCARD_W_UNPOWERED_CARD:  //0xff12 卡未上电
		return 0xFF9F;
		//return ("Power has been removed from the smart card and no further communication is possible.\n");
		break;
	case SCARD_W_UNRESPONSIVE_CARD:	  //0xff15 卡无响应
		return 0xFFA0;
		//return ("The smart card is not responding to a reset.\n");
		break;
	case SCARD_W_UNSUPPORTED_CARD:
		return 0xFFA1;
		//return ("The reader cannot communicate with the card due to ATR string configuration conflicts.\n");
		break;
	case 0:
		return 0x9000;
	}
	return -1;
}


BOOL CReader38T::IsMyPrinter(char* buff)
{
	CString m_strPassWord = CString("");

	srand( (unsigned)time( NULL ) );

	CString CitySnLs("");
	CString liuShui("");

	int i;
	for ( i = 0; i < 8; i++ )
	{
		int ran = (double)rand() / (RAND_MAX + 1) * 255; 
		char temp[16]={0};
		sprintf(temp,"%02X",ran);
		CitySnLs  += CString(temp);
	};
	CString mf1_UK1 =			TriDes_EN("EC7414B9BDE84c2dA65C439AB3776FA4",L"4Guanyuan_Huzhenglin_GuanZheng13");
	CString Tag = TriDes_EN(  CitySnLs,mf1_UK1);

	CString data = CitySnLs + Tag.Mid(0,8);

    BOOL bIs =  false;
    if(InitialReader())
    {
        byte cardType = 0;
	    if(ConnectCard(&cardType)==0x9000)
        if(m_bShareConected==2)
        {
	        bIs = OnGetPassWord(data,buff); 
        };
    }	  
	CloseReader();
	return bIs;

}

 
BOOL CReader38T::OnGetPassWord(CString data,char* buff)
{
	USES_CONVERSION;
	CString rand("");
	CString apdu("");
	char getRes[256]={0};

	CString strPassWord("");

	unsigned short  ret = SendApduChar(("00 a4  00 00 02 df  02"),getRes);
	if(ret!=0x9000)
	{
		char temp[64] ={0};
		sprintf(temp,"Msg2:%02X",ret);
		AfxMessageBox(CString(temp));
        return false;
	}
    else
    {
        CString flag = CString(getRes).Mid(36,4);
        if(flag.CompareNoCase(L"C60A")!=0)
            return false;
//139 4523 5557 
    }
    if(buff)
    {
        CString jmyz =  CString(getRes).Mid(30, 10);
        strcpy(buff, W2A(jmyz.GetBuffer(0)));
    };
	ret = 0;
	{
		CString apdu = CString("0084000004");

		ret = SendApduChar(W2A(apdu.GetBuffer(0)),getRes);
		if(ret!=0x9000)
			return false;

		rand = CString(getRes);
		if(rand.GetLength()==8)
			rand += CString("00000000");
		else
			return false;
	}
	memset(getRes,0,256);
	CString PSAM_MK_DF02 =		Des_EN("B1540A31E5C246df9C283923BF300FB9",L"2Guanyuan_Huzhenglin_GuanZheng13");
	CString EnRnd = TriDes_EN(rand,PSAM_MK_DF02);
	apdu = CString("0082000008")+EnRnd;
	ret = SendApduChar(W2A(apdu.GetBuffer(0)),getRes);
	//if(ret!=0x9000)
	//{
	//	CString   m_strPassWord("");
	//	memset(getRes,0,256);

	//	Gy_SendApduHex(devNo,2,"00 a4  00 00 02 df  01",getRes);

	//	CString com1 = CString("80 fc  01 00 0D ") + data + CString(" 01");
	//	memset(getRes,0,256);
	//	Gy_SendApduHex(devNo,1,(com1.GetBuffer(0)),getRes);
	//	m_strPassWord += CString(getRes).Mid(0,12);
	//}
	//else
	if(ret==0x9000)
	{
		CString com1 = CString("80 FC 01 01 0D") + data +  CString("01");
		memset(getRes,0,256);
		ret = SendApduChar(W2A(com1.GetBuffer(0)),getRes);
		strPassWord = CString(getRes);			
	}
	if(strPassWord.GetLength()==12)
	{
		CString mf1_UK2 =			TriDes_EN("8ECAB6D8D2CA404eA2984CE4D73CB28E",L"5Guanyuan_Huzhenglin_GuanZheng13");
		CString data2 = data.Mid(4,14) + CString("01");
		CString cmp =   TriDes_EN(data2,mf1_UK2);
		cmp = cmp.Mid(0,12);
		return cmp.CollateNoCase(strPassWord)==0;
	}
	else
		return false;
}


