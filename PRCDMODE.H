/*********************************************************************
*
*	< Application Interface >
*	  PRCDMODE.H
*
*         Official edition	Ver1.00		2012.02.20
*
*         PR-C201 Series Card Printer Driver
*					Copyright(c) 2012 Nisca Corporation.
*
**********************************************************************/
#ifndef	_DMODE_H
#define	_DMODE_H


/********************************************************/
/*	define												*/
/********************************************************/
#define	ENCODEDATA				10000		// Specification of magnetic encoding
#define	OPTIONEXEC				10002		// Specification of operation for external box
#define	APPLICATIONTAG			10004		// Specification of application tag
#define	RESINBLACK				10006		// Specification of black printing
#define NSC_ESC_ENCODEDATA_MAX		200		// Magnetic encoder write size
#define _DOCDMODE_SIZE				352		// _DOCDMODE structure size
#define KEYDMODE_DMY_SIZE			4		// KEYDMODE structure size




/********************************************************/
/*	Structure											*/
/********************************************************/

#pragma pack(1)


/************************************************/
/*	ResinBlack structure						*/
/************************************************/
typedef struct {
	WORD			Resin;					// [1:Start, 0:End]
} ResinBlack;


/************************************************/
/*	EncodeVar structure							*/
/************************************************/
typedef struct {
	WORD			nTrack;					// Track [0:ISO Track1, 1:JIS Track1, 2:ISO Track2, 3:ISO Track3, 4:JIS Track3, 5:JIS2]
	WORD			nSize;					// Data size
	BYTE			Data[NSC_ESC_ENCODEDATA_MAX];	// Data
} EncodeVar;


/************************************************/
/*	OptionVar structure							*/
/************************************************/
typedef struct {
	BYTE			OptionType;				// Type				[0:IC R/W 1, 1:IC R/W 2, 2:External box, 3:Magnetic encoder (reader)]
	BYTE			Motion;					// Motion			[0:Normal printing, 5:Pre-processing]
	BYTE			MgTrack;				// Magnetic Track	[Bit0:ISO Track1, Bit1:JIS Track1, Bit2:ISO Track2, Bit3:ISO Track3, Bit4:JIS Track3, Bit5:JIS2, Bit6:Reserve, Bit7:Reserve]
	BYTE			Reserv;					// Reserved			0x00
	HWND			hWnd;					// Specify the window handle on the application side for receiving messages.
	WORD			wMsg;					// Specify the message number for which notification is expected.
} OptionVar;


/************************************************/
/*	ApplicationTag structure					*/
/************************************************/
typedef struct {
	BYTE			abAppTag[8];			// Specifies the tag number.
	HWND			hWnd;					// Specify the window handle on the application side for receiving messages.
	WORD			wMsg;					// Specify the message number for which notification is expected.
} ApplicationTag;


/************************************************/
/*	RESINSETTING structure						*/
/************************************************/
typedef struct
{
	BOOL			bNotResin;				// Non Binary conversion Area 	[0=Not use, 1=Use]
	SHORT			nNotResinLeft;			// Invalid area Left	[Unit: pixel]
	SHORT			nNotResinWidth;			// Invalid area Width	[Unit: pixel]
	SHORT			nNotResinTop;			// Invalid area Top		[Unit: pixel]
	SHORT			nNotResinHeight;		// Invalid area Height	[Unit: pixel]
} RESINSETTING, FAR * LPRESINSETTING;


/************************************************/
/*	OVERCOATSETTING structure					*/
/************************************************/
typedef struct
{
	BOOL			bNonCoating;			// Non transferable area 	[0=Not use, 1=Use]
	SHORT			nNonCoatingLeft;		// Non-transfer area Left	[Unit: pixel]
	SHORT			nNonCoatingTop;			// Non-transfer area Top	[Unit: pixel]
	SHORT			nNonCoatingWidth;		// Non-transfer area Width	[Unit: pixel]
	SHORT			nNonCoatingHeight;		// Non-transfer area Height	[Unit: pixel]
} OVERCOATSETTING, FAR * LPOVERCOATSETTING;


/************************************************/
/*	COLOR_SETTING_TAB structure					*/
/************************************************/
typedef struct
{
	BOOL			bResinBlackUse;			// Black [0=Not use, 1=Use]

	SHORT			nResinAreaLeft;			// Binary (BLACK) Conversion Area Left		[Unit: pixel]
	SHORT			nResinAreaWidth;		// Binary (BLACK) Conversion Area Width		[Unit: pixel]
	SHORT			nResinAreaTop;			// Binary (BLACK) Conversion Area Top		[Unit: pixel]
	SHORT			nResinAreaHeight;		// Binary (BLACK) Conversion Area Height	[Unit: pixel]

	BOOL			bResinText;				// Applicable black text data				[0=OFF 1=ON]
	BOOL			bResinMonoImg;			// Applicable black and white graphics		[0=OFF 1=ON]
	BOOL			bResinLine;				// Applicable black Line data				[0=OFF 1=ON]
	BOOL			bResinColorImg;			// Applicable black as part of color graphics [0=OFF 1=ON]

	RESINSETTING	ResinSetting[3];		// Non Binary conversion area Up to 3 places[Unit: pixel]

	SHORT			nBrightness;			// Brightness		[Å|50 - Å{50]
	SHORT			nContrast;				// Contrast			[Å|50 - Å{50]
	SHORT			nChroma;				// Contrast			[Å|50 - Å{50]

//Å@Since there is no protection layer in PR-C201, members "nProtectiveLayer" to "OverCoatSetting" below are reserved.
//Å@PR-C101 and PR5350 are members for protection layer setting since transfer to protection layer is possible in these models.
	SHORT			nProtectiveLayer;		// PR-C101/PR5350ÅFOverCoat setting		[0 - 8]
	SHORT			nCoatingLeft;			// PR-C101/PR5350ÅFOverCoat area Left	[Unit: pixel]
	SHORT			nCoatingTop;			// PR-C101/PR5350ÅFOverCoat area Top	[Unit: pixel]
	SHORT			nCoatingWidth;			// PR-C101/PR5350ÅFOverCoat area Width	[Unit: pixel]
	SHORT			nCoatingHeight;			// PR-C101/PR5350ÅFOverCoat area Height	[Unit: pixel]

	OVERCOATSETTING		OverCoatSetting[5];	// PR-C101/PR5350ÅFNon-overcoat area	[Unit: pixel]

	BOOL			bUVBmpFileUse;			// UV layer transfer	[0=Not use, 1=Use]
	SHORT			nUVBmpFileX;			// UV layer transfer pattern file (X axis)
	SHORT			nUVBmpFileY;			// UV layer transfer pattern file (Y axis)
	TCHAR			uUVBmpFile[MAX_PATH];	// UV layer transfer pattern file (1-bit Bitmap file name)

	BOOL			bHologram;				// Hologram				[0=Not use, 1=Use]

	BYTE			nMonoPrint;				// Black printing setting						[0=Color and Black, 1=Black (black and white)]
	BYTE			nMonoDefault;			// Compulsion BLACK Print						[0=Threshold, 1=Pattern Dithe, 2=Error Diffusio, 3=Custom]
	BYTE			nMonoText;				// Custom Setting Applicable black property data			[0=Threshold, 1=Pattern Dithe, 2=Error Diffusio, 3=Custom]
	BYTE			nMonoBinary;			// Custom Setting Applicable black and white graphics		[0=Threshold, 1=Pattern Dithe, 2=Error Diffusio, 3=Custom]
	BYTE			nMonoLine;				// Custom Setting Applicable black Line data 				[0=Threshold, 1=Pattern Dithe, 2=Error Diffusio, 3=Custom]
	BYTE			nMonoPicture;			// Custom Setting Applicable black as part of color graphics[0=Threshold, 1=Pattern Dithe, 2=Error Diffusio, 3=Custom]

	SHORT			nReserve7;				// Reserved7
	SHORT			nReserve8;				// Reserved8
	SHORT			nReserve9;				// Reserved9
	SHORT			nReserve10;				// Reserved10
	SHORT			nReserve11;				// Reserved11
	SHORT			nReserve12;				// Reserved12
	SHORT			nReserve13;				// Reserved13
	SHORT			nReserve14;				// Reserved14
	SHORT			nReserve15;				// Reserved15
	SHORT			nReserve16;				// Reserved16
	SHORT			nReserve17;				// Reserved17
	SHORT			nReserve18;				// Reserved18
	SHORT			nReserve19;				// Reserved19

} COLOR_SETTING_TAB, FAR * LPCOLOR_SETTING_TAB ;

/************************************************/
/*	_EXTDMODE structure							*/
/************************************************/
typedef struct
{
	SHORT			nTransBlankLeft;		// X coordinate origin of the margin for calculating the physical card size
	SHORT			nTransBlankTop;			// Y coordinate origin of the margin for calculating the physical card size
	SHORT			nTransBlankWidth;		// Total margin width
	SHORT			nTransBlankHeight;		// Total margin height

	BOOL			bRotateFront;			// 180-degree rotation of the front side[0=Disable, 1=Enable]
	BOOL			bRotateBack;			// 180-degree rotation of the back side	[0=Disable, 1=Enable]
	BOOL			nReserve1;				// Reserve1
	BOOL			bResinDisableFront;		// [0=Setting from application disabled, and setting item Black displayed on the Printing Preferences screen, 
											//	1=Setting from application enabled, and setting item Black hidden from the Printing Preferences screen]
	BOOL			bResinDisableBack;		// [0=Setting from application disabled, and setting item Black displayed on the Printing Preferences screen,
											//	1=Setting from application enabled, and setting item Black hidden from the Printing Preferences screen]

	USHORT			nUnit;					// Unit	[0=Pixel, 1=Inch, 2=mm]

	SHORT			nReserve2;				// Reserve2
	SHORT			nReserve3;				// Reserve3
	SHORT			nReserve4;				// Reserve4
	SHORT			nReserve5;				// Reserve5
	SHORT			nReserve6;				// Reserve6
	SHORT			nReserve7;				// Reserve7
	SHORT			nReserve8;				// Reserve8


	COLOR_SETTING_TAB	FrontSideColor;		// Member of Graphics tab
	COLOR_SETTING_TAB	BackSideColor;		// Member of Graphics (Back) tab

} _EXTDMODE;


/************************************************/
/*	COLOR_SETTING_TAB2 structure				*/
/************************************************/
typedef struct
{
	DWORD			bColorGray;				// [0=RGB-CMY, 1=RGB-K, 2=RGB-CMYK]

	BOOL			bGammaUse;				// Gamma				[0=Not use, 1=Use]
	SHORT			nGamma;					// Gamma table value	[0=0.4, 1=0.6, 2=0.8, 3=1.0, 4=1.2, 5=1.4, 6=1.6, 7=1.8, 8=2.0, 9=2.2]
	BOOL			bGammaFileUse;			// Using file			[0=Not use, 1=Use]
	TCHAR			uGammaFile[MAX_PATH];	// Gamma file

	BOOL			bUVBmpFile1;			// UV layer transfer pattern file (No gradation)	[0=Not use, 1=Use]
	BOOL			bUVBmpFile2;			// UV layer transfer pattern file (With gradation)	[0=Not use, 1=Use]
	SHORT			nUVBmpFile2X;			// UV layer transfer pattern file (With gradation) X axis
	SHORT			nUVBmpFile2Y;			// UV layer transfer pattern file (With gradation) Y axis
	TCHAR			uUVBmpFile2[MAX_PATH];	// UV layer transfer pattern file (With gradation) File path(24-bit bitmap file name)

	SHORT			nProtectiveLayer;		// No non-transfer areas/5 place	[10=None, 11=5 places]
	OVERCOATSETTING		OverCoatSetting[5];	// Non transfer area 1-5			[Unit: pixel]

	INT			nReserve1[256];				// Reserve1
	INT			nReserve2[256];				// Reserve2
	INT			nReserve3[256];				// Reserve3
	INT			nReserve4[256];				// Reserve4

} COLOR_SETTING_TAB2, FAR * LPCOLOR_SETTING_TAB2 ;


/************************************************/
/*	_EXTDMODE2 structure						*/
/************************************************/
typedef struct
{
	COLOR_SETTING_TAB2	FrontSideColor;		// Member of Graphics tab	
	COLOR_SETTING_TAB2	BackSideColor;		// Member of Graphics (Back) tab

	INT			nReserve1[256];				// Reserve1
	INT			nReserve2[256];				// Reserve2
} _EXTDMODE2;

#pragma pack()


#endif
