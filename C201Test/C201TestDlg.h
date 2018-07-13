// C201TestDlg.h : 头文件
//

#pragma once

typedef unsigned short  (CALLBACK *   JT_pfOpenPrinter)(char* PrintName,HANDLE* devNo);
typedef unsigned short  (CALLBACK *   JT_pfCheckState)(HANDLE devNo,char* data);

typedef unsigned short  (CALLBACK *   JT_pfEndJob)(HANDLE devNo);
typedef unsigned short  (CALLBACK *   JT_pfGetPrinterSn)(HANDLE devNo,char*PrinterSn);

typedef unsigned short  (CALLBACK *   JT_pfUnParkSmartCard)(HANDLE devNo,bool isgood);
typedef unsigned short  (CALLBACK *   JT_pfParkSmartCard)(HANDLE devNo,char isContactCard ,bool NeedPrint);
typedef unsigned short  (CALLBACK *   JT_pfPrintPretreatment )(HANDLE devNo);
typedef unsigned short  (CALLBACK *   JT_pfPrintImage)(
HANDLE devNo,
float X,		//打印位置的横坐标
float Y,		//打印位置的纵坐标
float Width,	//打印图片的宽度
float Height,	//打印图片的高度
char* Path,	//图片的全路径
int rotate, 
BOOL flag
)	;
 
typedef  unsigned short (CALLBACK *  JT_pfPrintText)(
HANDLE devNo,
float X,							//打印位置的横坐标
float Y,							//打印位置的纵坐标
float Width,
float Height,
char* Font,					//使用的字体名称
float Size,						//字号大小
unsigned char Attribute,		//文字属性
int Color,					//文字颜色（使用标准的Web颜色代码）
char* Text ,					//打印的文本
int  wordSpace,                         //字间距
int  rowSpace                        //行间距
)	;


typedef unsigned short  (CALLBACK *   JT_pfStartPrinting)(HANDLE devNo,bool haveNext);

typedef unsigned short  (CALLBACK *   JT_pMoveCardOut)(HANDLE devNo, BOOL flag);



// C201TestDlg 对话框
class C201TestDlg : public CDialog
{
// 构造
public:
	C201TestDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_C201TEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	
	
	bool LoadDll();
	
HINSTANCE	m_hPrinterDll ;
	JT_pfOpenPrinter		JT_StartOneJob ;
	JT_pfCheckState			JT_CheckState;
	JT_pfEndJob				JT_EndJob ;

	JT_pfParkSmartCard		JT_ParkSmartCard ;
	
	JT_pfUnParkSmartCard	JT_UnParkSmartCard ;
	JT_pfGetPrinterSn		JT_GetPrinterSn;
	
	JT_pfPrintPretreatment	JT_PrintPretreatment ;
	JT_pfPrintText			JT_PrintText ;
	JT_pfPrintImage			JT_PrintImage ;
	JT_pfStartPrinting		JT_StartPrinting ;

	JT_pMoveCardOut JT_MoveCardOut;

public:
	afx_msg void OnBnClickedOk();
	};
