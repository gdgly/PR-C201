// C201TestDlg.h : ͷ�ļ�
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
float X,		//��ӡλ�õĺ�����
float Y,		//��ӡλ�õ�������
float Width,	//��ӡͼƬ�Ŀ��
float Height,	//��ӡͼƬ�ĸ߶�
char* Path,	//ͼƬ��ȫ·��
int rotate, 
BOOL flag
)	;
 
typedef  unsigned short (CALLBACK *  JT_pfPrintText)(
HANDLE devNo,
float X,							//��ӡλ�õĺ�����
float Y,							//��ӡλ�õ�������
float Width,
float Height,
char* Font,					//ʹ�õ���������
float Size,						//�ֺŴ�С
unsigned char Attribute,		//��������
int Color,					//������ɫ��ʹ�ñ�׼��Web��ɫ���룩
char* Text ,					//��ӡ���ı�
int  wordSpace,                         //�ּ��
int  rowSpace                        //�м��
)	;


typedef unsigned short  (CALLBACK *   JT_pfStartPrinting)(HANDLE devNo,bool haveNext);

typedef unsigned short  (CALLBACK *   JT_pMoveCardOut)(HANDLE devNo, BOOL flag);



// C201TestDlg �Ի���
class C201TestDlg : public CDialog
{
// ����
public:
	C201TestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_C201TEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
