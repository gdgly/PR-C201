// C201TestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "C201Test.h"
#include "C201TestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//bool isContactCard ,bool fromRight
#define IsContactLessCard 0
#define IsContactCard 1




// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// C201TestDlg �Ի���




C201TestDlg::C201TestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(C201TestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void C201TestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(C201TestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &C201TestDlg::OnBnClickedOk)
	//ON_BN_CLICKED(IDOK2, &C201TestDlg::OnBnClickedOk2)
	//ON_BN_CLICKED(IDC_BUTTON1, &C201TestDlg::OnBnClickedButton1)
	//ON_BN_CLICKED(IDC_BUTTON2, &C201TestDlg::OnBnClickedButton2)
	//ON_BN_CLICKED(IDC_BUTTON3, &C201TestDlg::OnBnClickedButton3)
	//ON_BN_CLICKED(IDC_BUTTON4, &C201TestDlg::OnBnClickedButton4)
	//ON_BN_CLICKED(IDC_BUTTON5, &C201TestDlg::OnBnClickedButton5)
	//ON_BN_CLICKED(IDC_BUTTON6, &C201TestDlg::OnBnClickedButton6)
	//ON_BN_CLICKED(IDC_BUTTON7, &C201TestDlg::OnBnClickedButton7)
END_MESSAGE_MAP()


// C201TestDlg ��Ϣ�������

BOOL C201TestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	LoadDll();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void C201TestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void C201TestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR C201TestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

bool C201TestDlg::LoadDll()
{
	USES_CONVERSION;
	m_hPrinterDll = LoadLibraryW(CString("C201_Print_Dll.dll"));
	if(m_hPrinterDll)
	{				
		JT_CheckState = (JT_pfCheckState)		GetProcAddress(m_hPrinterDll, "JT_CheckState") ;;
		JT_StartOneJob = (JT_pfOpenPrinter)		GetProcAddress(m_hPrinterDll, "JT_StartOneJob") ;;
		JT_EndJob = (JT_pfEndJob)		GetProcAddress(m_hPrinterDll, "JT_EndJob") ;;
		JT_GetPrinterSn = (JT_pfGetPrinterSn)		GetProcAddress(m_hPrinterDll, "JT_GetPrinterSn") ;;

		JT_ParkSmartCard = (JT_pfParkSmartCard)		GetProcAddress(m_hPrinterDll, "JT_ParkSmartCard") ;;
		
		JT_UnParkSmartCard = (JT_pfUnParkSmartCard)		GetProcAddress(m_hPrinterDll, "JT_UnParkSmartCard") ;;

		JT_PrintPretreatment = (JT_pfPrintPretreatment)		GetProcAddress(m_hPrinterDll, "JT_PrintPretreatment") ;;
		JT_PrintText = (JT_pfPrintText)		GetProcAddress(m_hPrinterDll, "JT_PrintText") ;;
		JT_PrintImage = (JT_pfPrintImage)		GetProcAddress(m_hPrinterDll, "JT_PrintImage") ;;
		JT_StartPrinting = (JT_pfStartPrinting)		GetProcAddress(m_hPrinterDll, "JT_StartPrinting") ;

		JT_MoveCardOut = (JT_pMoveCardOut)		GetProcAddress(m_hPrinterDll, "JT_MoveCardOut") ;
		return true;
	}
	else
	{

		AfxMessageBox(CString("װ�� C201_Print_Dll.dll ʧ�ܣ�"));
		return FALSE;
	};
}

void C201TestDlg::OnBnClickedOk()
{
	USES_CONVERSION;
	char data[256];memset(data,0,256);

	char Status[1024]={0};
	
	BOOL m_bPrintSucc = FALSE;

	BOOL suc = TRUE;
	HANDLE devNo = NULL;

	CString m_strPrintName(L"Nisca PR-C201");
 
	unsigned short  ret0 = JT_StartOneJob(W2A(m_strPrintName.GetBuffer(0)),&devNo);

   
    char Sn[1024]={0};
    unsigned short  ret00 = JT_GetPrinterSn( devNo, Sn)  ;

	//AfxMessageBox(Sn);

	//if (ret0 != 0x9000)
	//	return ;

	bool bNeedPrint = true;
    bool bIsContactCard = true;

    unsigned short  ret1 = JT_ParkSmartCard(devNo,bIsContactCard,bNeedPrint);

    bool bWritedCardSucc = true/* = YourWritCardFunction() */;

	unsigned short  ret2 = JT_UnParkSmartCard(devNo,bWritedCardSucc);//Once you call JT_ParkSmartCard ,you must call JT_UnParkSmartCard next
//	ret1 = JT_MoveCardOut(devNo, false);
  
	if(bWritedCardSucc && bNeedPrint)
	{
		CString FontName("����");
		unsigned char txtpro = 0;					//,bool isContactCard ,bool fromRight,bool printAfterIC
		CString m_strName(_T("���Բ�������"));

		unsigned short  ret3 = JT_PrintPretreatment(devNo);
		if(0x9000==ret3)
		{
			//��ɫ��0xff0000;��ɫ��0x0000ff;��ɫ��0xffffff;��ɫɫ��0x000000;
			unsigned short  ret4 = JT_PrintText(devNo, 5, 0, 30, 5, W2A(FontName.GetBuffer(0)), 10, txtpro, 0xff0000, W2A(m_strName), 0, 0); 
// 			if(0x9000!=ret4)
// 			{
// 			suc = FALSE ; ; 
// 			}

			ret4 = JT_PrintText(devNo, 5, 6, 30, 5, "����", 10, txtpro, 0xff0000, "��    ���־��ط���", 0, 0);

 			unsigned short  ret5 = JT_PrintImage(devNo, 22, 10, 30, 30, "C:\\Users\\Administrator\\Desktop\\ceshi\\b.jpg", 0, 0);
			ret4 = JT_PrintImage(devNo, 22, 10, 30, 30, "C:\\Users\\Administrator\\Desktop\\ceshi\\���Կ���ͼƬ.png", 0, TRUE);
// 			if(0x9000!=ret5)
// 			{
// 			suc = FALSE ; ; 
// 			}
		}
		unsigned short  ret6 = JT_StartPrinting( devNo, 0);//once JT_PrintPretreatment be called ,JT_StartPrinting must be called next ,even JT_PrintPretreatment return fail
		if(0x9000!=ret6)//if you just print on one side ,you should call "JT_StartPrinting( devNo,1);" and don't call next steps;
		{
			suc = FALSE ; ; 
		}

		//unsigned short  ret7 = JT_PrintPretreatment(devNo);
		//if(0x9000!=ret7)
		//{
		//	unsigned short  ret8 = JT_PrintText(devNo,0, 0, 100, 20, W2A(FontName.GetBuffer(0)), 10, txtpro, 0, W2A(m_strName.GetBuffer(0)), 5, 10); 
		//	if(0x9000!=ret8)
		//	{
		//		suc = FALSE ; ; 
		//	}

		//	unsigned short  ret9 = JT_PrintImage(devNo,100, 100,664, 1036, "D:\\3.jpg", 180); ; 
		//	if(0x9000!=ret9)
		//	{
		//		suc = FALSE ; ; 
		//	}
		//}
		//unsigned short  ret10 = JT_StartPrinting( devNo,0);//once JT_PrintPretreatment be called ,JT_StartPrinting must be call next ,even JT_PrintPretreatment return fail
		//if(0x9000!=ret10)
		//{
		//	suc = FALSE ; ; 
		//}
	}
	unsigned short  ret11 = JT_EndJob(devNo);//once JT_StartOneJob be called ,JT_EndJob must be called next ,even JT_StartOneJob return fail
}
