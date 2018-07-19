#include "stdafx.h"
#include "C201Interf.h"
#include "C201Fun.h"
#include "md5module.h"
#include "math.h"
extern HWND  hMainAppWnd ;



//YMC Level YMC Printing energy level setting. From 0 (Low) to 6 (High).
//Film Quantity Usable retransfer film quantity. The possible range is from 0 (None)
//to 10 (Full).
//Card Quantity The quantity of cards in the card tray.
//0 None 2 Any
//Ink Code Specifies the kind of the ink ribbon. Possible values are:
extern void WriteLog(CString log);

//
//btHardwareStatus Bit								2A 14 11
//Content
//0	Mechanism Ready		Printer is in standby status
//1	Mechanism Busy		Printer is ON
//2	Mechanism Error		Printer is stopped due to error
//3	Mechanism Print		Printer is printing
//4	Interface Ready		Printer is available for receiving new print data
//5	Interface Busy		Printer is not available for receiving new print data
//6,7	Reserved			Not used

//btCardPosition
//0	Pick up Unit	Card feeding system
//1	Flip Turn Unit	Flip turn unit
//2	Encoder Unit	Magnetic encoder unit
//3	Print Unit		Print unit
//4	Reserved		Not used
//5	Heat Roller Unit	Heat roller unit
//6,7	Reserved		Not used

//btPrinterStatus
//0	Yellow Printing			Printing with the yellow ribbon
//1	Magenta Printing		Printing with the magenta ribbon
//2	Cyan Printing			Printing with the cyan ribbon
//3	Resin Black Printing	Printing resin black
//4	Reserved				Not used
//5	Holographic Overlay		Transferring hologram
//6,7	Reserved			Not used

//btEncoderStatus
//0	ISO Track 1 Encoding	Encoding ISO track 1
//1	ISO Track 2 Encoding	Encoding ISO track 2
//2	ISO Track 3 Encoding	Encoding ISO track 3
//3	JIS II Encoding			Encoding JIS II track
//4	IC R/W Access Enable	Access to IC R/W, 1: Possible, 2: Not possible
//5,6	Reserved				Not used
//7	IC Type					IC type, 1: IC R/W 2, 2: IC R/W 1
//
//
//btMemoryStatus
//0	Memory Clear	Print data not received (Not used)
//1	Memory Ready	Print data transfer possible
//2	Memory Full		Print data reception complete (Not used)
//3	Memory Printing	Card being printed with the print data content
//4	Memory Finish	Card successfully printed with the print data content
//5	Memory Error	Error while printing card with the print data content
//6	Memory Enable	Priority for print data transfer
//7	Reserved		Not used
//

//btOptionUnitStatus
//0	Heat Roller Warm Up	Print data not received (Not used)
//1-7	Reserved				Not used

bool C201::ResetPrint(CString			PrinterName)
{
	USES_CONVERSION;	
	bool bSuccess = false;
	PR5600HANDLE			pr56handle		= INVALID_HANDLE_VALUE;
    PR5600RESULT			pr56Result		= PR56ERR_NO_ERROR;

    pr56handle = pr56XXOpenPrinter(0, 0, PrinterName, NULL, 0);
	if(pr56handle != INVALID_HANDLE_VALUE)
	{
		PR5600_STATUS_INFO2		info2			= {0};
		PR5600_PRINTER_SENSE	sense			= {0};
		DWORD					dwReturned		= 0;
		DWORD					dwSenseReturned	= 0;

		pr56Result = pr56XXGetPrinterStatusInformationEx(pr56handle, &info2, sizeof(info2), &dwReturned, &sense, sizeof(sense), &dwSenseReturned);
		if (pr56Result == PR56ERR_SUCCESS)
		{
			//if ((info2.btStandbyStatus & 2) == 2)
			{
				//BYTE	byExecCmd[4]	= {0x1b, 0x23, 0x30, 0x2e};  0x1b 0x23 0x30 0x2E	To restart card issuance.
//                                          0x1b 0x23   0x31 0x2E	
                //To stop card issuance with an error.
//                  The card at the waiting position will be disposed to the error eject exit by the error discharge motion.
//
                //Specify                   0x1b 0x3d  0x31  0x2e.

				BYTE	byExecCmd[4]	= {0x1b, 0x3d, 0x32, 0x2e};
  /*
  
			// Eject card command settings
			memset(byCommand, 0, sizeof(byCommand));
			byCommand[0] = 0x1b;
			byCommand[1] = 0x3d;
			byCommand[2] = 0x31;
			byCommand[3] = 0x2e;

			funcSetPosMessage(pPrintInfo->hWnd, MSG_UPDATE_STATUE, L"Request for card ejection ... pr56XXPrint()");
			result = ppr56XXPrint(pr56handle, byCommand, 4);
			if(result == PR56ERR_SUCCESS)
			{
				funcSetPosMessage(pPrintInfo->hWnd, MSG_UPDATE_STATUE, L"Function (pr56XXPrint) failed.");
			}
  */
				pr56Result = pr56XXPrint(pr56handle, byExecCmd, sizeof(byExecCmd));
				if(pr56Result == PR56ERR_SUCCESS)
				{
					bSuccess = true;
				}
			}
		}
		pr56Result = pr56XXClosePrinter(pr56handle);
	}
	return bSuccess;
}

C201::C201(void)
{
	m_hInstance = NULL;
	m_hCardDCForPrinting = NULL;
	m_hCardDCForParKSmartCard  = NULL;
	pr56handle = INVALID_HANDLE_VALUE;
	//PR5600HANDLE pr56handle =  NULL;		// Printer handle
  	pDEVMODE = NULL;			// DEVMODE structure

	m_bIsPrinting = false;
	m_bParkingSmartCard = false;
	m_bNeedPrinting = true;

	
}

C201::~C201(void)
{
	if(m_hInstance)
	{
		FreeLibrary(m_hInstance);
		m_hInstance = NULL;
	}
	m_Pic1.FreePictureData();
	if(m_hCardDCForPrinting)
	{
		DeleteDC(m_hCardDCForPrinting);
		m_hCardDCForPrinting = 0;
	};

	if(m_hCardDCForParKSmartCard)
	{
		DeleteDC(m_hCardDCForParKSmartCard);
		m_hCardDCForParKSmartCard = 0;
	};

	if(pDEVMODE)
	{
		delete pDEVMODE;
		pDEVMODE = NULL;
	}

	PR5600RESULT result  = pr56XXClosePrinter(pr56handle);
	if(result != PR56ERR_SUCCESS)
	{
		pr56handle = NULL;
	}	
}

//1604 

unsigned short	C201::C201_StartOneJob(CString strPrintName)
{
	USES_CONVERSION;
	if(!IsMyPrinter(NULL))
		return	  0x9077;

	if(m_hInstance == NULL)
	{		
		if (!C201_LoadC201DLL(strPrintName))
		{
			return 0x9099;
		}
	}

	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

    m_bSCFailOutPuted = false;

    PR5600RESULT result = PR56ERR_NO_ERROR; // Direct communication function return value	
	PR5600_STATUS_INFO2		info2;
	PR5600_PRINTER_SENSE	sense;
	DWORD                   dwSenseReturned = 0;
	DWORD					dwReturned = 0;

	m_strPrinterName = CString(strPrintName);
	int nameLen  = strPrintName.GetLength();
	char sort = strPrintName.GetAt(nameLen-1);

	try
	{
		if(m_hCardDCForPrinting==0)
		{
			m_hCardDCForPrinting = CreateDC(NULL, m_strPrinterName, NULL, NULL);
			if(m_hCardDCForPrinting==0) 
			{
				return 0x9010;
			}

			pr56handle = pr56XXOpenPrinter(0, 0, m_strPrinterName.GetBuffer(0), NULL, 0);
			if(pr56handle == INVALID_HANDLE_VALUE)
			{
				byte reson = 0;
				DWORD	dwSize = 0;	
				DWORD	pdwNeeded = 0;
				DWORD	pdwReturned = 0;
				result = pr56XXEnumEnabledPrinters(NULL, dwSize, &pdwNeeded, &pdwReturned);
				if(result == PR56ERR_SUCCESS)
				{
					if(pdwNeeded > 0) // When there are available printers
					{
						reson = 1;	
					}
					else
						reson =2 ;
				}
				else
					reson = 3;

				PR5600RESULT result  = pr56XXClosePrinter(pr56handle);
				if(result != PR56ERR_SUCCESS)
				{
					pr56handle = NULL;
				}
				DeleteDC(m_hCardDCForPrinting);;
				m_hCardDCForPrinting = 0;
				if(reson==1)
					return 0x9012;// 写错打印机名;
				else if(reson==2)
					return 0x9013;//无可用的打印机，需要重启电脑
				else
					return 0x9014;//函数调用失败。重装驱动。
			}
			result = pr56XXGetPrinterStatusInformationEx(pr56handle, &info2, sizeof(info2), &dwReturned, &sense, sizeof(sense), &dwSenseReturned);
			if(result == PR56ERR_NO_ERROR)
			{
				byte s1 = info2.btHardwareStatus&0x10;
				byte s2 = info2.btHardwareStatus&0x04;
				if (s1==0x10 && s2!=0x04)
				{
					return 0x9000;
				}
				else
				{
					PR5600RESULT result  = pr56XXClosePrinter(pr56handle);
					if(result != PR56ERR_SUCCESS)
					{
						pr56handle = NULL;
					}
					DeleteDC(m_hCardDCForPrinting);;
					m_hCardDCForPrinting = 0;
					return 0x9015  ;
				};
			}
			else   
			{
				PR5600RESULT result  = pr56XXClosePrinter(pr56handle);
				if(result != PR56ERR_SUCCESS)
				{
					pr56handle = NULL;
				}
				DeleteDC(m_hCardDCForPrinting);;
				m_hCardDCForPrinting = 0;
				return 0x9016;
			}

		}

	}
	catch(...)
	{			
		return 0x9017;//!!!
	}	
	
	return 0x9000;
};

BOOL	C201::C201_SetPrintDuplexOrNot(int bDuplexMode)
{
	HANDLE      hPrinter = NULL;			// Printer handle

	BOOL suc  = false;

	DWORD	dwNeeded = 0;
	DWORD	dwReturned = 0;
	// Acquires the printer handle.
	if(OpenPrinter(m_strPrinterName.GetBuffer(0), &hPrinter, NULL) >= 0)
	{
		// Acquires the DEVMODE structure size.
		dwNeeded = DocumentProperties(hMainAppWnd, hPrinter, m_strPrinterName.GetBuffer(0), NULL, NULL, 0);
		if(dwNeeded >= 0)
		{
			// Allocates a memory area to acquire DEVMODE structure information.
			if(pDEVMODE)
			{
				delete pDEVMODE;
				pDEVMODE = NULL;
			}
			pDEVMODE = (LPDEVMODE)malloc(dwNeeded);
			if(pDEVMODE != NULL)
			{		
				// Acquires the DEVMODE structure.
				dwReturned = DocumentProperties(hMainAppWnd, hPrinter, m_strPrinterName.GetBuffer(0), pDEVMODE, NULL, DM_OUT_BUFFER);
				if (dwReturned >= 0)
				{
					// Simplex specification (Fixed)
					pDEVMODE->dmDuplex = bDuplexMode;
					
					// Updates the DEVMODE structure content.
					dwReturned = DocumentProperties(hMainAppWnd, hPrinter, m_strPrinterName.GetBuffer(0), pDEVMODE, pDEVMODE, DM_IN_BUFFER);
					if (dwReturned >= 0)
					{
						suc = true;
					}					
				}
				// Acquires the DEVMODE structure.
				//dwReturned = DocumentProperties(hMainAppWnd, hPrinter, m_strPrinterName.GetBuffer(0), pDEVMODE, NULL, DM_OUT_BUFFER);
				//if (dwReturned >= 0)
				//{
				//	// Simplex specification (Fixed)
				//	pDEVMODE->dmDuplex = bDuplexMode;
				//	
				//	// Updates the DEVMODE structure content.
				//	dwReturned = DocumentProperties(hMainAppWnd, hPrinter, m_strPrinterName.GetBuffer(0), pDEVMODE, pDEVMODE, DM_IN_BUFFER|DM_OUT_BUFFER);
				//	if (dwReturned >= 0)
				//	{
				//		suc = true;
				//	}					
				//}
			}
		}
	}

	// Closes the printer handle.
	if(hPrinter != NULL)
	{
		ClosePrinter(hPrinter);
		hPrinter = NULL;
	}
	return suc;
}

unsigned short	C201::C201_ParkSmartCard(char isContactCard ,bool NeedPrinting)
{
	USES_CONVERSION;
	if(!IsMyPrinter(NULL))
		return	  0x9077;

	if(!C201_SetPrintDuplexOrNot(DMDUP_SIMPLEX))
		return 0x9502;

	if(m_hCardDCForParKSmartCard==0)
	{
		m_hCardDCForParKSmartCard = CreateDC(NULL, m_strPrinterName, NULL, pDEVMODE);
		if(m_hCardDCForParKSmartCard==0) 
		{
			return 0x9010;
		}
	}


	PR5600RESULT result = PR56ERR_NO_ERROR;
	PR5600_STATUS_INFO2		info2;
	PR5600_PRINTER_SENSE	sense;
	DWORD                   dwSenseReturned = 0;
	DWORD					dwReturned = 0;

	OptionVar	optionvar;					// ICEncodeVar structure
	char		cErrorMessage[256] = {""};	// Error message
	unsigned short ret = 0x9000;
	DOCINFO		docInfo;					// DOCINFO structure

	docInfo.cbSize = sizeof(DOCINFO);
	docInfo.lpszDocName = L"ParkSmartCard";
	docInfo.lpszOutput = NULL;
	docInfo.lpszDatatype = NULL;
	docInfo.fwType = 0;

	byte HardwareStatus = 0;
	byte CardPosition	= 0;
	byte EncodeStation  = 0;
	byte StandbyStatus  = 0;  
	//WriteLog("P1");
	result = pr56XXGetPrinterStatusInformationEx(pr56handle, &info2, sizeof(info2), &dwReturned, &sense, sizeof(sense), &dwSenseReturned);
	if(result == PR56ERR_NO_ERROR)
	{
		HardwareStatus  = info2.btHardwareStatus;
		CardPosition    = info2.btCardPosition ;
		EncodeStation   = info2.btEncoderStatus;
		StandbyStatus   = info2.btStandbyStatus;
		if((info2.btHardwareStatus & 0x04)!=0)
			return 0x902E;
		if((info2.btEncoderStatus & 0x10)!=0)
		{
			//WriteLog("P2");
			return 0x9020;
		}
		if(	StandbyStatus==02)
		{
			//C201_SendMoveCardCommand();
			//WriteLog("P3");
			return 0x9021;
		}
	}
	else
		return 0x9023;

	if(StartDoc(m_hCardDCForParKSmartCard, &docInfo) > 0)
	{
		//WriteLog("P4");
		if(StartPage(m_hCardDCForParKSmartCard) > 0)
		{
			
			SYSTEMTIME  sysTime;
			GetSystemTime(&sysTime);
			DWORD rk = sysTime.wDay ^ sysTime.wDayOfWeek ^ sysTime.wHour ^ sysTime.wMilliseconds ^ sysTime.wMinute ^ sysTime.wMonth ^ sysTime.wSecond ^ sysTime.wYear;   

			srand(rk);
			for (int i = 0; i < 8; i++ )
			{
				int ran = (int)(rand()%10); 
				//char temp[16]={0};
				//sprintf(temp,"%02X",ran);
				applicationtag.abAppTag[i] = 0x30 + ran;
				//CitySnLs  += CString(temp);
			};
			applicationtag.hWnd = hMainAppWnd;//m_ComDlg->GetSafeHwnd();
			applicationtag.wMsg = 0;
		//WriteLog("P5");

			if(ExtEscape(m_hCardDCForParKSmartCard, APPLICATIONTAG, sizeof(applicationtag), (LPCSTR)&applicationtag, 0, NULL) > 0)
			{
				//WriteLog("P6");
				// Set value to PR5600APPNOTIFY structure
				// Registers the window handle and window messages to receive notifications from 
				// the printer driver when the target card reaches the access position.
				PR5600APPNOTIFY aPR5600APPNOTIFY[1];	// PR5600APPNOTIFY structure
				aPR5600APPNOTIFY[0].Notify = 0;
				aPR5600APPNOTIFY[0].hWnd = hMainAppWnd;//m_ComDlg->GetSafeHwnd();
				aPR5600APPNOTIFY[0].nMsg = MSG_SEND_MESSAGE;

				// Executes the direct communication function (pr56XXRegisterMessage) to register the message.
				//funcSetPosMessage(hWnd, MSG_UPDATE_STATUE, "Registration of message ... pr56XXRegisterMessage()");
				result = pr56XXRegisterMessage(pr56handle, 1, aPR5600APPNOTIFY);
				if(result == PR56ERR_SUCCESS)
				{
				// IC R/W 1
					//WriteLog("P7");
				
					optionvar.OptionType = isContactCard?0:1;
					optionvar.Motion = NeedPrinting?5:0;
					optionvar.MgTrack = 0x00;
					optionvar.Reserv = 0x00;
					optionvar.hWnd = hMainAppWnd;//m_ComDlg->GetSafeHwnd();
					optionvar.wMsg = MSG_SEND_MESSAGE;
					if(ExtEscape(m_hCardDCForParKSmartCard, OPTIONEXEC, sizeof(optionvar), (LPCSTR)&optionvar, 0, NULL) <= 0)
					{
						//WriteLog("P8");
						ret = 0x9024;
					}
				}
				else
				{
					//WriteLog("P9");
					ret = 0x9025;
				}
			}
			else
			{
				//WriteLog("P10");
				ret = 0x9026;
			}
			EndPage(m_hCardDCForParKSmartCard)	  ;
		}
		else
		{
			//WriteLog("P11");
			ret = 0x9027;
		}
		//WriteLog("P12");
		EndDoc(m_hCardDCForParKSmartCard) ;			
	}
	else
	{
		//WriteLog("P13");
		ret = 0x9028;
	}

	int time = 0;
	Sleep(100);
	while(1)
	{
		result = pr56XXGetPrinterStatusInformationEx(pr56handle, &info2, sizeof(info2), &dwReturned, &sense, sizeof(sense), &dwSenseReturned);
		if(result == PR56ERR_NO_ERROR)
		{
			//WriteLog("P14");
			bool b1 =	HardwareStatus  == info2.btHardwareStatus;
			bool b2 =	CardPosition    == info2.btCardPosition ;
			bool b3 =	EncodeStation   == info2.btEncoderStatus;
			bool b4 =	StandbyStatus   == info2.btStandbyStatus;

			if((info2.btHardwareStatus&0x04)!=0)
				return 0x902F;
			if(b3&&b4)
				Sleep(200);
			else
			{

				if ((info2.btStandbyStatus & 1) == 1)
				{
					HardwareStatus  = info2.btHardwareStatus;
					CardPosition    = info2.btCardPosition ;
					EncodeStation   = info2.btEncoderStatus;
					StandbyStatus   = info2.btStandbyStatus;
					Sleep(200);
					if(C201_SendMoveCardCommand())
					{
						int time = 0;
						while(1)
						{
							result = pr56XXGetPrinterStatusInformationEx(pr56handle, &info2, sizeof(info2), &dwReturned, &sense, sizeof(sense), &dwSenseReturned);
							 b1 =	HardwareStatus  == info2.btHardwareStatus;
							 b2 =	CardPosition    == info2.btCardPosition ;
							 b3 =	EncodeStation   == info2.btEncoderStatus;
							 b4 =	StandbyStatus   == info2.btStandbyStatus;

							if ((info2.btEncoderStatus&0x10) == 0x10)
								return 0x9000;
							else
								Sleep(200);
							if(time++>30)
								return 0x9029;
						}					 

					}
					else
						return 0x902A;
				}
				else
				{
					if ((info2.btEncoderStatus&0x10) == 0x10)
						return 0x9000;
					else
					{
						Sleep(200)   ;
						if(time++>30)
							return 0x902C;
					}
				}
			}
		}
		else
			return 0x902D;
	}
	return ret;
}

bool	C201::C201_SendMoveCardCommand()
{
	BYTE	byExecCmd[4]	= {0x1b, 0x23, 0x30, 0x2e};
	
	PR5600RESULT result  = pr56XXPrint(pr56handle, byExecCmd, 4);
	if(result == PR56ERR_SUCCESS)
	{
		return true;
	}
	else
		return false;
}


bool	C201::C201_SendEjectCardCommand()
{
	BYTE	byExecCmd[4]	= {0x1b, 0x3d, 0x30, 0x2e};
	
	PR5600RESULT result  = pr56XXPrint(pr56handle, byExecCmd, 4);
	if(result == PR56ERR_SUCCESS)
	{
		return true;
	}
	else
		return false;
}

unsigned short		C201::C201_UnParkSmartCard(bool isgood)
{
	if(!IsMyPrinter(NULL))
		return	  0x9077;

	unsigned short	ret = 0x9000;		
	if(isgood)
	{
	    if(C201_SendICSoltCommand(1))
	    {
		    if(C201_WaitCardOutSlot(4000))
		    {
			    ret =  0x9000;
		    }
		    else
			    ret =  0x9031;
	    }
	    else
		    ret = 0x9032;
    }
	else
	{
 	    if(C201_SendICSoltCommand(1))
	    {
		    if(C201_WaitCardOutSlot(4000))
		    {
                if(C201_SendEjectCardCommand())
                {
		            if(C201_WaitCardOutSlot(4000))
		            {
                        m_bSCFailOutPuted = true;
			            ret = 0x9000;
                    }
                    else
                        ret = 0x9034;
                }
                else
                    ret = 0x9033;
		    }
		    else
			    ret = 0x9031;
	    }
	    else
		    ret = 0x9032;        
	}

	//if(!C201_SetPrintDuplexOrNot(DuplexMode))
	//	return 0x9053;

	return ret; 
}


bool		C201::C201_SendICSoltCommand(byte command)
{
    PR5600RESULT result = PR56ERR_NO_ERROR; // Direct communication function return value
	// End notification
	result = pr56XXPrint_SetICEncoder(pr56handle, command/*isgood?1:2*/, TRUE); // Specifies 2 in case of error termination.
	return result == PR56ERR_NO_ERROR;

}
//bool	C201::C201_WaitCardPrinting()
//{
//	PR5600_STATUS_INFO2		info2;
//	PR5600_PRINTER_SENSE	sense;
//	DWORD                   dwSenseReturned = 0;
//	DWORD					dwReturned = 0;
//	PR5600RESULT result = PR56ERR_NO_ERROR; // Direct communication function return value
//	int i=0;			 
//	BOOL  bNeedSendOut = false;
//		int time = 0;
//	result = pr56XXGetPrinterStatusInformationEx(pr56handle, &info2, sizeof(info2), &dwReturned, &sense, sizeof(sense), &dwSenseReturned);
//	if(info2.btCardPosition==8)
//	{
//		while(1)
//		{
//			Sleep(100);
//			result = pr56XXGetPrinterStatusInformationEx(pr56handle, &info2, sizeof(info2), &dwReturned, &sense, sizeof(sense), &dwSenseReturned);
//
///*			char temp[256]={0};
//			sprintf(temp,"WaitCard1 result:%04x,time: %04x",result,time);
//			WriteLog(CString(temp));*/	
//
//			if(info2.btStandbyStatus==2)
//				break;
//			
//			if(time++>30)
//				break;
//
//		}
//	}
//	else
//	{
//		char temp[256]={0};
//		sprintf(temp,"WaitCard2 result:%04x,info2.btCardPosition: %04x",result,info2.btCardPosition);
//		WriteLog(CString(temp));	
//	}
//		
//	bool meetErr = false;
//	if(info2.btStandbyStatus != 2)
//	{
//		Sleep(5000);	
//
//		int i=0;
//		while(1)
//		{
//			i++;
//			result = pr56XXGetPrinterStatusInformationEx(pr56handle, &info2, sizeof(info2), &dwReturned, &sense, sizeof(sense), &dwSenseReturned);
//
//			bool b1 = (info2.btMemoryStatus[0] & 0x08) ==0x08;
//			bool b2 = (info2.btMemoryStatus[1] & 0x08) ==0x08;
//			bool printing = b1||b2;
//			meetErr = (info2.btHardwareStatus&0x04!=0)||(info2.btMemoryStatus[0]&0x20!=0)||(info2.btMemoryStatus[1]&0x20!=0) ;
//			if(meetErr)
//				break;
//			if(printing && (info2.btStandbyStatus != 2))
//			{
//				Sleep(200);
//				char temp[256]= {0};
//				sprintf(temp,"WaitCard4 i:%02x, time:%02x, PrinterStatus:%02x, CardPosition:%02x, StandbyStatus:%02x, MA:%02x, MB:%02x\r\n",i,time,info2.btPrinterStatus,info2.btCardPosition,info2.btStandbyStatus,info2.btMemoryStatus[0],info2.btMemoryStatus[1]);
//				//TRACE(temp);
//				WriteLog(CString(temp));
//			}
//			else
//			{
//				 bNeedSendOut = info2.btStandbyStatus == 2;
//
//				 {
//					char temp[256]={0};
//					sprintf(temp,"WaitCard5 result:%04x,info2.btCardPosition: %04x",result,info2.btCardPosition);
//					WriteLog(CString(temp));	
//				 }
//				 break;
//			}
//		}
//	}
//	else
//	{
//		bNeedSendOut = true;
//
//		char temp[256]={0};
//		sprintf(temp,"WaitCard3 result:%04x,info2.btCardPosition: %04x",result,info2.btCardPosition);
//		WriteLog(CString(temp));	
//	}
//
//
//	if(bNeedSendOut)
//	{
//		char temp[256]={0};
//		sprintf(temp,"WaitCard6 result:%04x,info2.btCardPosition: %04x",result,info2.btCardPosition);
//		WriteLog(CString(temp));	
//
//		bool bmove =  C201_SendMoveCardCommand();
//		if(meetErr)
//			return false;
//		else
//			return bmove;
//	}	
//	else
//	{
//		char temp[256]={0};
//		sprintf(temp,"WaitCard7 result:%04x,info2.btCardPosition: %04x",result,info2.btCardPosition);
//		WriteLog(CString(temp));	
//	}
//	if(meetErr)
//		return false;
//	else
//		return true;
//}
bool	C201::C201_WaitCardPrinting()
{
	PR5600_STATUS_INFO2		info2;
	PR5600_PRINTER_SENSE	sense;
	DWORD                   dwSenseReturned = 0;
	DWORD					dwReturned = 0;
	PR5600RESULT result = PR56ERR_NO_ERROR; // Direct communication function return value
	int i=0;			 
	BOOL  bNeedSendOut = FALSE;

	bool meetErr = false;
	int time = 0;
	
	result = pr56XXGetPrinterStatusInformationEx(pr56handle, &info2, sizeof(info2), &dwReturned, &sense, sizeof(sense), &dwSenseReturned);
	while(info2.btCardPosition==0)
	{
		result = pr56XXGetPrinterStatusInformationEx(pr56handle, &info2, sizeof(info2), &dwReturned, &sense, sizeof(sense), &dwSenseReturned);
		Sleep(500);
		time++;

		meetErr = ((info2.btHardwareStatus&0x04)!=0)||(((info2.btMemoryStatus[0]&0x20)!=0)&&((info2.btMemoryStatus[1]&0x20)!=0)) ;
		if(meetErr)
			break;
        if(!m_bSCFailOutPuted)
        {
		    if(time>20)
			    break;
        }
        else
        {
		    if(time>4)
			    break;
        }
	}
	if(meetErr)
		return false;
	time = 0;
	result = pr56XXGetPrinterStatusInformationEx(pr56handle, &info2, sizeof(info2), &dwReturned, &sense, sizeof(sense), &dwSenseReturned);
	if(info2.btCardPosition==8)
	{
		while(1)
		{
			Sleep(500);
			result = pr56XXGetPrinterStatusInformationEx(pr56handle, &info2, sizeof(info2), &dwReturned, &sense, sizeof(sense), &dwSenseReturned);

			char temp[256]={0};
			sprintf(temp,"WaitCard1 result:%04x,time: %04x",result,time);
			TRACE(temp);
			WriteLog(CString(temp));	

			if(info2.btStandbyStatus==2||info2.btStandbyStatus==0)
			{
				result = pr56XXGetPrinterStatusInformationEx(pr56handle, &info2, sizeof(info2), &dwReturned, &sense, sizeof(sense), &dwSenseReturned);
				meetErr = ((info2.btHardwareStatus&0x04)!=0)||(((info2.btMemoryStatus[0]&0x20)!=0)&&((info2.btMemoryStatus[1]&0x20)!=0)) ;
			
				break;
			}
			if(time++>80)
				break;

		}
	}
	else
	{
		result = pr56XXGetPrinterStatusInformationEx(pr56handle, &info2, sizeof(info2), &dwReturned, &sense, sizeof(sense), &dwSenseReturned);
		meetErr = ((info2.btHardwareStatus&0x04)!=0)||(((info2.btMemoryStatus[0]&0x20)!=0)&&((info2.btMemoryStatus[1]&0x20)!=0)) ;

		char temp[256]={0};
		sprintf(temp,"WaitCard2 result:%04x,info2.btCardPosition: %04x",result,info2.btCardPosition);
		TRACE(temp);
		WriteLog(CString(temp));	
	}
	if(meetErr)
		return false;		
	if(info2.btStandbyStatus != 2)
	{
		//Sleep(5000);	

		int i=0;
		while(1)
		{
			i++;
			result = pr56XXGetPrinterStatusInformationEx(pr56handle, &info2, sizeof(info2), &dwReturned, &sense, sizeof(sense), &dwSenseReturned);

			bool b1 = (info2.btMemoryStatus[0] & 0x08) ==0x08;
			bool b2 = (info2.btMemoryStatus[1] & 0x08) ==0x08;
			bool printing = b1||b2;
			if(printing)
			{
				meetErr = ((info2.btHardwareStatus&0x04)!=0)||(((info2.btMemoryStatus[0]&0x20)!=0)&&((info2.btMemoryStatus[1]&0x20)!=0)) ;
				if(meetErr)
					break;
			}
			if(printing && (info2.btStandbyStatus != 2))
			{
				Sleep(200);
				char temp[256]= {0};
				sprintf(temp,"WaitCard4 i:%02x, time:%02x, PrinterStatus:%02x, CardPosition:%02x, StandbyStatus:%02x, MA:%02x, MB:%02x\r\n",i,time,info2.btPrinterStatus,info2.btCardPosition,info2.btStandbyStatus,info2.btMemoryStatus[0],info2.btMemoryStatus[1]);
				TRACE(temp);
				WriteLog(CString(temp));
			}
			else
			{
				//meetErr = (((info2.btMemoryStatus[0]&0x20)!=0)||((info2.btMemoryStatus[1]&0x20)!=0));
				//if((info2.btMemoryStatus[0])&0x04==0x04)
				//{
				//	if(info2.btMemoryStatus[0] &0x10 !=0x10)
				//		meetErr = true;

				//}

				//if((info2.btMemoryStatus[1])&0x04==0x04)
				//{
				//	if(info2.btMemoryStatus[1] &0x10 !=0x10)
				//		meetErr = true;

				//}
				if((info2.btMemoryStatus[0])==0x14 ||(info2.btMemoryStatus[1])==0x14)
					meetErr = false;
				else
					meetErr = true;

				 bNeedSendOut = info2.btStandbyStatus == 2;

				 {
					char temp[256]={0};
					sprintf(temp,"WaitCard5 result:%04x,info2.btCardPosition: %04x",result,info2.btCardPosition);

					WriteLog(CString(temp));	
					TRACE(temp);
				 }

				 break;
			}
		}
	}
	else
	{
		bNeedSendOut = true;
		result = pr56XXGetPrinterStatusInformationEx(pr56handle, &info2, sizeof(info2), &dwReturned, &sense, sizeof(sense), &dwSenseReturned);
		meetErr = ((info2.btHardwareStatus&0x04)!=0)||(((info2.btMemoryStatus[0]&0x20)!=0)&&((info2.btMemoryStatus[1]&0x20)!=0)) ;

		char temp[256]={0};
		sprintf(temp,"WaitCard3 result:%04x,info2.btCardPosition: %04x",result,info2.btCardPosition);
		WriteLog(CString(temp));	
	}


	if(bNeedSendOut)
	{
		char temp[256]={0};
		sprintf(temp,"WaitCard6 result:%04x,info2.btCardPosition: %04x",result,info2.btCardPosition);
		WriteLog(CString(temp));	

		bool bmove =  C201_SendMoveCardCommand();
		if(meetErr)
			return false;
		else
			return bmove;
	}	
	else
	{
		char temp[256]={0};
		sprintf(temp,"WaitCard7 result:%04x,info2.btCardPosition: %04x",result,info2.btCardPosition);
		WriteLog(CString(temp));	
	}
	if(meetErr)
		return false;
	else
		return true;
}

bool	C201::C201_WaitCardOutSlot(int time)
{
	PR5600_STATUS_INFO2		info2;
	PR5600_PRINTER_SENSE	sense;
	DWORD                   dwSenseReturned = 0;
	DWORD					dwReturned = 0;
	PR5600RESULT result = PR56ERR_NO_ERROR; // Direct communication function return value
	int i=0;

	while(1)
	{
		result = pr56XXGetPrinterStatusInformationEx(pr56handle, &info2, sizeof(info2), &dwReturned, &sense, sizeof(sense), &dwSenseReturned);
		if ((info2.btCardPosition ) != 2 && ((info2.btEncoderStatus &0x10) == 0x00))
			return true;
		Sleep(200);
		i++;   
		if(time<i*200)
			return false;
	}		
}
;

unsigned short	C201::C201_CheckState(char*data)
{
	PR5600_STATUS_INFO2		info2;
	PR5600_PRINTER_SENSE	sense;
	DWORD                   dwSenseReturned = 0;
	DWORD					dwReturned = 0;
	PR5600RESULT result = PR56ERR_NO_ERROR; // Direct communication function return value
	int i=0;
	result = pr56XXGetPrinterStatusInformationEx(pr56handle, &info2, sizeof(info2), &dwReturned, &sense, sizeof(sense), &dwSenseReturned);
	CString getStr("");
	char temp[64]={0};
	if(result==PR56ERR_NO_ERROR)
	{
		sprintf(temp,"btHardwareStatus:%02x,",info2.btHardwareStatus);
		getStr += CString(temp);
		memset(temp,0,64);

		sprintf(temp,"btCardPosition:%02x,",info2.btCardPosition);
		getStr += CString(temp);
		memset(temp,0,64);

		sprintf(temp,"btPrinterStatus:%02x,",info2.btPrinterStatus);
		getStr += CString(temp);
		memset(temp,0,64);

		sprintf(temp,"btEncoderStatus:%02x,",info2.btEncoderStatus);
		getStr += CString(temp);
		memset(temp,0,64);

		sprintf(temp,"btMemoryStatus0:%02x,",info2.btMemoryStatus[0]);
		getStr += CString(temp);
		memset(temp,0,64);

		sprintf(temp,"btMemoryStatus1:%02x,",info2.btMemoryStatus[1]);
		getStr += CString(temp);
		memset(temp,0,64);

		sprintf(temp,"btStandbyStatus:%02x,",info2.btStandbyStatus);
		getStr += CString(temp);
		memset(temp,0,64);

		sprintf(temp,"btOptionUnitStatus:%02x,",info2.btOptionUnitStatus);
		getStr += CString(temp);
		memset(temp,0,64);

		sprintf(temp,"btDataStatus0:%02x,",info2.btDataStatus[0]);
		getStr += CString(temp);
		memset(temp,0,64);

		sprintf(temp,"btDataStatus1:%02x,",info2.btDataStatus[1]);
		getStr += CString(temp);

		memcpy(data,getStr.GetBuffer(0),getStr.GetLength());
		return 0x9000;
	}
	else
		return 0x90F1;
	
}


unsigned short    C201::C201_PrintPretreatment ()
{
	WriteLog("DC_FeedCard"); 
	
	WriteLog("		0");
	if(m_hCardDCForPrinting == NULL  ||m_strPrinterName.IsEmpty() )
	{
		return 0x9041;//m_sLastOprateRst = 0x9001;
	}

	DOCINFO			m_di;
	CString strDocName = CString(_T("Print ")) + m_strPrinterName ;
	m_di.lpszDocName = strDocName.GetBuffer(0); 
	m_di.lpszOutput  = NULL;
	m_di.lpszDatatype = NULL;
	m_di.fwType = 0;
	WriteLog("		StartDoc 0");
	if( StartDoc(m_hCardDCForPrinting,&m_di) >0)
	{
		WriteLog("		StartDoc 1 TRUE");
	
		if (StartPage(m_hCardDCForPrinting) > 0)
		{ 
			WriteLog("		StartPage 1 TRUE");	
		}
		else
		{
			EndDoc(m_hCardDCForPrinting) ;
			WriteLog("		StartPage 1 FALSE");
			return 0x9042;
			
		}
	}
	else
	{
		WriteLog("		StartDoc 1 FALSE");
		return 0x9043;
	}

	return 0x9000;
};




int CALLBACK _EnumFontFamExProc(	ENUMLOGFONTEX * lpelfe,
													NEWTEXTMETRICEX * /*lpntme*/,
													int /*nFontType*/,
													LPARAM lParam )
{ 
	USES_CONVERSION;
	LOGFONTA* pfd = (LOGFONTA*)lParam;

	if( strcpy((pfd->lfFaceName)  , W2A(lpelfe->elfLogFont.lfFaceName))==0)
	{
		pfd->lfCharSet			= lpelfe->elfLogFont.lfCharSet;
		pfd->lfClipPrecision	= lpelfe->elfLogFont.lfClipPrecision;
		pfd->lfEscapement		= lpelfe->elfLogFont.lfEscapement;
		pfd->lfOutPrecision		= lpelfe->elfLogFont.lfOutPrecision;
		pfd->lfPitchAndFamily	= lpelfe->elfLogFont.lfPitchAndFamily;
		pfd->lfQuality			= lpelfe->elfLogFont.lfQuality;
		return FALSE;	// Finished
	}
	else
		return TRUE;	// Continue
}


void DrawMultLineText(HDC hdc, CRect rect, int nRowDis, UINT nFromat, CString strText)
{
	WCHAR* pText = strText.GetBuffer(strText.GetLength());
	int nCount = strText.GetLength();
	CRect rtChar;
	CSize size;
	GetTextExtentPoint32(hdc, pText + 0, 1, &size);
	int nRowHeight = size.cy + nRowDis;
	rtChar.top = rect.top;
	rtChar.left = rect.left;
	rtChar.bottom = rtChar.top + nRowDis + size.cy;
	rtChar.right = rtChar.left + size.cx;
	CString strChar;
	for (int nCharIndex = 0; nCharIndex < nCount; nCharIndex++)
	{
		if (rtChar.right > rect.right)
		{
			rtChar.top = rtChar.bottom;
			rtChar.bottom += nRowHeight;
			GetTextExtentPoint32(hdc, pText + nCharIndex, 1, &size);
			rtChar.left = rect.left;
			rtChar.right = rtChar.left + size.cx;
			if (rtChar.bottom > rect.bottom)
				break;
		}
		strChar = pText[nCharIndex];
		DrawText(hdc, strChar, strChar.GetLength(), rtChar, nFromat);
		GetTextExtentPoint32(hdc, pText + nCharIndex + 1, 1, &size);
		rtChar.left = rtChar.right;
		rtChar.right += size.cx;
	}
}



 unsigned short  C201::C201_PrintText(int X, int Y,	 int Width, int Height, CString Font, float Size, 
	 unsigned char Attribute, int Color, CString Text , BOOL  TransBkColor, int  wordSpace,  int  rowSpace)
{
	USES_CONVERSION;
	if(m_hCardDCForPrinting == NULL  ||m_strPrinterName.IsEmpty() )
	{
		return m_iC201Result =  0x9051;
	}	

	//字体设置
	//LOGFONT lf;
	//memset(&lf, 0, sizeof(LOGFONT));				// zero out structure

	//strcpy(W2A(lf.lfFaceName),W2A(Font.GetBuffer(0)));

	//int dens = GetDeviceCaps(m_hCardDCForPrinting,LOGPIXELSY);
	//lf.lfHeight = -(int)
	//	 (
	//		abs(int((Size*dens/72.0f)+0.5f))
	//			   	
	//	 ) ;//Size;								// request a 12-pixel-height font


	//if(((Attribute)&0x80)==0x80)
	//	lf.lfWeight = 800;
	//else
	//	lf.lfWeight = 400;	
	//lf.lfCharSet = wordSpace;//DEFAULT_CHARSET;//134;
	//lf.lfOutPrecision	= 3;
	//lf.lfClipPrecision	= 2;
	//lf.lfQuality		= 1;//PROOF_QUALITY 来防止Windows缩放点阵字体
	//lf.lfPitchAndFamily	= DEFAULT_PITCH ;//VARIABLE_PITCH | TMPF_TRUETYPE | FF_MODERN ;//DEFAULT_PITCH ;//0;
	//lf.lfItalic = (((Attribute)&0x40)==0x40);
	//lf.lfUnderline = (((Attribute)&0x20)==0x20);
	//lf.lfStrikeOut = (((Attribute)&0x10)==0x10);

	//LOGFONT lf1;
	//memset(&lf1, 0, sizeof(LOGFONT));				// zero out structure
	//_tcsncpy(lf1.lfFaceName,Font, lstrlen(Font));
	//lf1.lfCharSet = DEFAULT_CHARSET;
	//EnumFontFamiliesEx(	m_hCardDCForPrinting, &lf1, (FONTENUMPROC)_EnumFontFamExProc, (LPARAM)(LPVOID)&lf, 0 );

	//HFONT newFont = CreateFontIndirect(&lf);
	//VERIFY(newFont);  // create the font

	//字号大小
	int dens = GetDeviceCaps(m_hCardDCForPrinting,LOGPIXELSY);
	int lfHeight = -(int)(abs(int((Size*dens/72.0f)+0.5f))) ;//Size;		
	//加粗
	int lfWeight = 0;
	if(((Attribute)&0x80)==0x80)
	{
		lfWeight = 700;
	}else{
		lfWeight = 400;
	}
		//	lf.lfWeight = 800;
		//else
		//	lf.lfWeight = 400;	
	HFONT newFont = CreateFont(lfHeight, 0, 0, 0, lfWeight, (((Attribute)&0x40)==0x40), (((Attribute)&0x20)==0x20),	(((Attribute)&0x10)==0x10), 
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, Font);

	HFONT hOldFont = (HFONT)SelectObject(m_hCardDCForPrinting, newFont);

	BYTE B = (Color&0xFF0000)/256/256;
	BYTE G = (Color&0xFF00)/256; 
	BYTE R = (Color&0xFF); 
	DWORD c = RGB(B,G,R); 
	//SetTextColor(m_hCardDCForPrinting, c);
	SetTextCharacterExtra(m_hCardDCForPrinting, (short)wordSpace);
	SetTextColor(m_hCardDCForPrinting, c);
	if(TransBkColor)
		SetBkMode(m_hCardDCForPrinting,TRANSPARENT);
	else
		SetBkMode(m_hCardDCForPrinting, OPAQUE);
	//BOOL opt = TextOut(m_hCardDCForPrinting, X, Y, Text, Text.GetLength());
	RECT rct;
	SetRect(&rct, X, Y, X + Width, Y + Height);
	DrawMultLineText(m_hCardDCForPrinting, rct, rowSpace, DT_EDITCONTROL | DT_WORDBREAK, Text);
	SelectObject(m_hCardDCForPrinting, hOldFont);   
	DeleteObject(newFont);
	return 0x9000;
}

 int downloadUrl(char* url1, char* picName)
 {
	 USES_CONVERSION;
	 CString strName(picName);
	 string url = url1;
	 //	string url = "http://pic33.photophoto.cn/20141118/0005018344622029_b.jpg";
	 size_t len = url.length();//获取字符串长度
	 int nmlen = MultiByteToWideChar(CP_ACP, 0, url.c_str(), len + 1, NULL, 0);//如果函数运行成功，并且cchWideChar为零，
	 //返回值是接收到待转换字符串的缓冲区所需求的宽字符数大小。
	 wchar_t* buffer = new wchar_t[nmlen];
	 MultiByteToWideChar(CP_ACP, 0, url.c_str(), len + 1, buffer, nmlen);
	 HRESULT hr = URLDownloadToFile(NULL, buffer, strName, 0, NULL);
	 if (hr == S_OK)
	 {
		 return 0;
	 }
	 return -1;
 }

 // 判断文件是否存在
 BOOL IsFileExist(const CString& csFile)
 {
	 DWORD dwAttrib = GetFileAttributes(csFile);
	 return INVALID_FILE_ATTRIBUTES != dwAttrib && 0 == (dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
 }



unsigned short  C201::C201_PrintImage(
int X,		//打印位置的横坐标
int Y,		//打印位置的纵坐标
int Weight,	//打印图片的宽度
int Height,	//打印图片的高度
CString Path,	//图片的全路径
int rotate,
BOOL flag
)
{
	USES_CONVERSION;	
	
	if(m_hCardDCForPrinting == NULL  ||m_strPrinterName.IsEmpty() )
	{
		return m_iC201Result =  0x9061;
	}	
	
	m_Pic1.FreePictureData(); 

	if (!IsFileExist(Path))
	{
		int downloadRet = downloadUrl(W2A(Path), "download1.jpg");
		if (downloadRet == -1)
		{
			return 0x9062;
		}
		Path = L"download1.jpg";
	}

	unsigned long ret = 0;
	CRect rcShow = CRect(X, Y, X + Weight, Y + Height);	
	Graphics graphics(m_hCardDCForPrinting);
	graphics.SetPageUnit(UnitMillimeter );//毫米
	graphics.SetPageScale(1.0f);

	Image image(Path);

	//显示区域比率
	float wShear = rcShow.Width() * 1.0f / image.GetWidth();
	float hShear = rcShow.Height() * 1.0f / image.GetHeight();
	float shear = image.GetWidth() * 1.0f / image.GetHeight();//获取图片比率，以至缩放时不变形

	float showH = 0;
	float showW = 0;

	if (wShear >= 1 && hShear >= 1)
	{
		//放大图片
		if (wShear >= hShear)//宽度比大于高度比
		{
			//图片高度为基准
			showH = image.GetWidth();
			showW = showH * shear * 1.0f;
		}
		else{
			//图片宽度为基准
			showW = image.GetHeight();
			showH = showW * 1.0f / shear;
		}
	}else
	{
		if (hShear <= wShear)
		{
			//显示区域高度为基准
			showH = rcShow.Height();
			showW = showH * shear * 1.0f;
		}
		else{
			//显示区域宽度为基准
			showW = rcShow.Width();
			showH = showW * 1.0f / shear;
		}
	}

	ImageAttributes imgAttributes;
	if (flag)
	{
		//设置透明色范围  
		graphics.TranslateTransform(image.GetWidth() + 10, 0);		
		imgAttributes.SetColorKey(Color(255, 230, 230, 230), Color(255, 255, 255, 255), ColorAdjustTypeBitmap);
	}

	//旋转图像
	Matrix matric(1, 0, 0, 1, 0, 0);
	ret = matric.RotateAt(rotate, PointF(showW / 2 + rcShow.left, showH + rcShow.top));
	ret = graphics.SetTransform(&matric);
	ret = graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);

	ret = graphics.DrawImage(&image, Gdiplus::RectF(rcShow.left, rcShow.top, showW, showH), 0, 0, image.GetWidth(), image.GetHeight(), Gdiplus::UnitPixel, &imgAttributes);
	
	return 0x9000;
}

unsigned short  C201::C201_PrintImage(
int X,		//打印位置的横坐标
int Y,		//打印位置的纵坐标
int Weight,	//打印图片的宽度
int Height,	//打印图片的高度
byte* picData,	//图片的全路径
int picLen
)
{
	if(m_hCardDCForPrinting == NULL  )
	{
		return m_iC201Result =  0x9061;
	}	
	
	m_Pic1.FreePictureData(); 
	if(m_Pic1.Load(picData,picLen))
	{
		if( m_Pic1.Draw(m_hCardDCForPrinting,X,Y,Weight,Height))
			return m_iC201Result =   0x9000;
		else
			return m_iC201Result =   0x9162;
	}
	else
		return m_iC201Result =   0x9163;
}

unsigned short    C201::C201_StartPrinting(bool haveNext)
{
	if(haveNext)
	{
		if(EndPage(m_hCardDCForPrinting)>0)
		{
			if (StartPage(m_hCardDCForPrinting) > 0)
			{ 
				WriteLog("		StartPage 1 TRUE");	
				return 0x9000;
			}
			else
			{
				WriteLog("		StartPage 1 FALSE");
				EndDoc(m_hCardDCForPrinting) ;
				return 0x9071;
			}
		}
		else
		{
			WriteLog("		StartPage 1 FALSE");
			EndDoc(m_hCardDCForPrinting) ;
			return 0x9072;
		}
	}
	else
	{
		if(EndPage(m_hCardDCForPrinting)>0)
		{;
			if(EndDoc(m_hCardDCForPrinting)>0)
			{
				return 0x9000;
			}
		}
		else
		{
			WriteLog("		StartPage 1 FALSE");
			EndDoc(m_hCardDCForPrinting) ;
			return 0x9073;
		}
	}
	return 0x9074;
};


unsigned short C201::C201_EndJob()
{
	if(!IsMyPrinter(NULL))
		return	  0x9077;

	if(C201_WaitCardPrinting())
	{	   
		GdiplusShutdown(m_gdiplusToken);
		return 0x9000;
	}
	else{
		GdiplusShutdown(m_gdiplusToken);
		return 0x9081;
	}
};

//
//BOOL C201::IsMyPrinter()
//{
//    char Sn[64]={0};
//    C201_GetPrinterSn(Sn);
//    CString strSn(Sn);
//    if(strSn.CompareNoCase("A2500446")==0 || strSn.CompareNoCase("A2700775")==0 || strSn.CompareNoCase("A2700778")==0 ||
//       strSn.CompareNoCase("D3300070")==0 || strSn.CompareNoCase("D3300080")==0 || strSn.CompareNoCase("D3900591")==0 || strSn.CompareNoCase("D4600690")==0 )
//    {
//        return true;
//    }
//    else 
//        return false;
//
//}

BOOL C201::IsMyPrinter(char* buff)
{
	return true;
    if(!m_38TReader.IsMyPrinter(buff))
	    m_bIsMyPrinter = false;
    else
        m_bIsMyPrinter =  true; 
        
    return m_bIsMyPrinter;
}





unsigned short C201::C201_GetPrinterSn( char* Sn) 
{
	BOOL found =false;

	DWORD	dwSize = 0;	
	DWORD	pdwNeeded = 0;
	DWORD	pdwReturned = 0;
	TCHAR	cErrorMessage[256] = {L""};				// Error message
	PR5600_PRINTER_INFO *pr5600PrinterInfo = NULL;	// Array of PR5600_PRINTER_INFO structures
	PR5600RESULT result = PR56ERR_NO_ERROR;

	result = pr56XXEnumEnabledPrinters(NULL, dwSize, &pdwNeeded, &pdwReturned);

	if(result == PR56ERR_SUCCESS)
	{
		if(pdwNeeded > 0) // When there are available printers
		{	
			int iCount = pdwNeeded / (sizeof(PR5600_PRINTER_INFO));
			
			dwSize = sizeof(PR5600_PRINTER_INFO)*iCount;
			pr5600PrinterInfo = (PR5600_PRINTER_INFO*)malloc(dwSize);
			pdwNeeded = 0;
			pdwReturned = 0;

			result = pr56XXEnumEnabledPrinters(pr5600PrinterInfo, dwSize, &pdwNeeded, &pdwReturned);

			if(result == PR56ERR_SUCCESS)
			{
//				pr56XXTerminate();

				for(int i = 0; i < (int)pdwReturned; i++)
				{
					PR5600_PRINTER_INFO *pr5600Info = (PR5600_PRINTER_INFO *)((BYTE *)pr5600PrinterInfo + sizeof(PR5600_PRINTER_INFO)*i);
					CString strName = pr5600Info->wszName;
					if(m_strPrinterName.CompareNoCase(strName)==0)
					{
						//Sn = CString(pr5600Info->szSerial);
						memcpy(Sn,pr5600Info->szSerial,strlen(pr5600Info->szSerial));
						found = true;
						break;
					}
					pr5600Info = NULL;
				}
			}


			if(pr5600PrinterInfo != NULL)
			{
				free(pr5600PrinterInfo);
				pr5600PrinterInfo = NULL;
			}
		}
	}

	return found?0x9000:0x9120;
}

 
bool	C201::C201_Moveout()
{
	BYTE	byExecCmd[4]	= {0x1b, 0x3d, 0x30, 0x2e};

	PR5600RESULT result  = pr56XXPrint(pr56handle, byExecCmd, 4);
	if(result == PR56ERR_SUCCESS)
	{
		return true;
	}
	else
		return false;
}


BOOL	C201::C201_LoadC201DLL(CString strPrintName)
{
	CString dllName;
	if (strPrintName.Find(L"PR-C201", 0) > 0)
	{
		dllName = L"PRCDAPI.DLL";
	}else if (strPrintName.Find(L"Avansia", 0) > 0)
	{
		dllName = L"EVLDAPI.DLL";
	}else{
		return FALSE;
	}
	m_hInstance = LoadLibrary(dllName);

	if(m_hInstance == NULL)
	{		
		return FALSE;
	}

	// Acquisition of DLL address
	pr56XXEnumEnabledPrinters = (_pr56XXEnumEnabledPrinters)GetProcAddress(m_hInstance, "pr56XXEnumEnabledPrinters");
	pr56XXTerminate = (_pr56XXTerminate)GetProcAddress(m_hInstance, "pr56XXTerminate");
	pr56XXOpenPrinter = (_pr56XXOpenPrinter)GetProcAddress(m_hInstance, "pr56XXOpenPrinter");
	pr56XXClosePrinter = (_pr56XXClosePrinter)GetProcAddress(m_hInstance, "pr56XXClosePrinter");
	pr56XXRegisterMessage = (_pr56XXRegisterMessage)GetProcAddress(m_hInstance, "pr56XXRegisterMessage");
	pr56XXGetLogs = (_pr56XXGetLogs)GetProcAddress(m_hInstance, "pr56XXGetLogs");
	pr56XXGetPrinterData = (_pr56XXGetPrinterData)GetProcAddress(m_hInstance, "pr56XXGetPrinterData");
	pr56XXPrint_SetICEncoder = (_pr56XXPrint_SetICEncoder)GetProcAddress(m_hInstance, "pr56XXPrint_SetICEncoder");
	pr56XXPrint = (_pr56XXPrint)GetProcAddress(m_hInstance, "pr56XXPrint");
	pr56XXGetPrinterStatusInformationEx = (_pr56XXGetPrinterStatusInformationEx)GetProcAddress(m_hInstance, "pr56XXGetPrinterStatusInformationEx");

	if(pr56XXEnumEnabledPrinters == NULL || pr56XXTerminate == NULL || pr56XXOpenPrinter == NULL 
		|| pr56XXClosePrinter == NULL || pr56XXRegisterMessage == NULL ||  pr56XXGetLogs == NULL 
		|| pr56XXGetPrinterData == NULL || pr56XXPrint_SetICEncoder == NULL || pr56XXPrint == NULL
		|| pr56XXGetPrinterStatusInformationEx == NULL)
	{		
		return FALSE;
	}
	return TRUE;
}