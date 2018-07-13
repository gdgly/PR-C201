//-----------------------------------------------------------------------------
// Picture (Implementations) Version 1.00
//
// Routins 4 Showing Picture Files... (.BMP .DIB .EMF .GIF .ICO .JPG .WMF)
//
// Author: Dr. Yovav Gad, EMail: Sources@SuperMain.com ,Web: www.SuperMain.com
//=============================================================================
// 
// Full Story:
// ~~~~~~~~~~~
// There R Many Libraries To Handle Image Files, Anyway Most Of Them Do Not
// Include Source Files Or Just Very Complicated To Implement / Understand,
// 
// After Many Days Of Searching (And Not Finding) a Way To Load a JPG From a
// Resource And Show It On a *Dialog Based* Application, I Decided 2 Take Steps
//
// So I Created What I Call a Very *Simple* & Useful Class,
// It Can Easily Implemented By Adding It To a Project, And U Do Not Have To
// Be a Real JPEG Freak - And Invent All Header Reading From The Beginning
// (It Uses The IPicture Interface - Same Way As Internet Explorer Does)
//
// I Would Like To Thank Mr.Peter Hendrix For His Wonderful Work
// That I Found On: http://www.thecodeproject.com/bitmap/cpicture.asp
// Which I Was Inspired And Got The IPicture Interface Idea From...
//
// Guess U Will Find It Useful,
// Appreciate If U Can Mention My Name On Your Final Code,
// Please Feel Free To Send Me Any Improvements Or SaveAsJPG() Functions:
//
// (This Program Has No Bugs - Only Undocumented Solutions)
//
// Author: Dr. Yovav Gad, EMail: Sources@SuperMain.com ,Web: www.SuperMain.com
//
//--------------------------Example & Usage 4 Dummies--------------------------
//
//  U Need 2 Add "CPicture.CPP" and "CPicture.H" Into Your Project (From FileView)
//  So U Will Get Control Over The Functions In This Class,
//  Then U Can Create a Picture Object And Show It On a Device Context
//
//  CPicture m_Picture;  // Create a Picture Object (An Instance Of This Class)
//  #include "Picture.h" // Make Sure U Include This Where U Gonna Create The Object...
//
//  Load Picture Data Into The IPicture Interface (.BMP .DIB .EMF .GIF .ICO .JPG .WMF)
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	m_Picture.Load("Test.JPG"); // Load From a File - Just Load It (Show Later)
//	m_Picture.Load(IDR_TEST, "JPG"); // Load From a Resource - Just Load It (Show Later)
//  (U Must Include IDR_TEST In Your Resources Under a Custom Name, 4 Example - "JPG")
//  
//  When Using DC Object On a *Dialog Based* Application (CPaintDC dc(this);)
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	m_Picture.UpdateSizeOnDC(&dc); // Get Picture Dimentions In Pixels
//	m_Picture.Show(&dc, CPoint(0,0), CPoint(m_Picture.m_Width, m_Picture.m_Height), 0,0);
//	m_Picture.Show(&dc, CRect(0,0,100,100)); // Change Original Dimentions
//	m_Picture.ShowBitmapResource(&dc, IDB_TEST, CPoint(0,0)); // Show Bitmap Resource
//
//  OR When Using a Pointer On a "Regular" MFC Application (CDC* pDC)
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	m_Picture.UpdateSizeOnDC(pDC); // Get Picture Dimentions In Pixels
//	m_Picture.Show(pDC, CPoint(0,0), CPoint(m_Picture.m_Width, m_Picture.m_Height), 0,0);
//	m_Picture.Show(pDC, CRect(0,0,100,100)); // Change Original Dimentions
//	m_Picture.ShowBitmapResource(pDC, IDB_TEST, CPoint(0,0)); // Show Bitmap Resource
//
//  Show Picture Information
//  ~~~~~~~~~~~~~~~~~~~~~~~~
//	CString S;
//	S.Format("Size = %4d\nWidth = %4d\nHeight = %4d\nWeight = %4d\n",
//	m_Picture.m_Weight, m_Picture.m_Width, m_Picture.m_Height, m_Picture.m_Weight);
//	AfxMessageBox(S);
//
//----------------------------Cut The Bullshit Here----------------------------
#include "stdafx.h"
#include "Picture.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define HIMETRIC_INCH 2540
#define ERROR_TITLE "CPicture Error" // Error Title (Related To This Class)...

//-----------------------------------------------------------------------------
// Does:   Constructor - Create a New CPicture Object To Hold Picture Data
// ~~~~
//
//-----------------------------------------------------------------------------
CPicture::CPicture()
//=============================================================================
{
	m_IPicture = NULL;

}

//-----------------------------------------------------------------------------
// Does:   Destructor - Free Data And Information From The CPicture Object
// ~~~~
//
//-----------------------------------------------------------------------------
CPicture::~CPicture()
//=============================================================================
{
	if(m_IPicture != NULL) FreePictureData(); // Important - Avoid Leaks...
}


//-----------------------------------------------------------------------------
// Does:   Free The Allocated Memory That Holdes The IPicture Interface Data
// ~~~~    And Clear Picture Information
//
// Note:   This Might Also Be Useful If U Only Need To Show The Picture Once
// ~~~~~   Or If U Copy The Picture To The Device Context, So It Can Still
//         Remain On Screen - But IPicture Data Is Not Needed No More
//
//-----------------------------------------------------------------------------
void CPicture::FreePictureData()
//=============================================================================
{
	if(m_IPicture != NULL)
		{
		m_IPicture->Release();
		m_IPicture = NULL;
		}
}



bool CPicture::Draw(HDC hdc,int x,int y,int width,int high)
{
	if (m_IPicture != NULL)
	{
		long hmWidth;
		long hmHeight;
		m_IPicture->get_Width(&hmWidth);
		m_IPicture->get_Height(&hmHeight);


    return S_OK == m_IPicture->Render(hdc,
                      x,                  // Left
                      y,                   // Top
                      width, // Right
                      high, // Bottom
                      0,
                      hmHeight,
                      hmWidth,
                      -hmHeight,
                      NULL);
		
//		return Draw(pDC, Pos.x, Pos.y, nWidth, nHeight);
	}
	return false;
}
bool CPicture::DrawPhoto(HDC hdc, int x, int y, int w, int h, DWORD m_TransColour,char optype)
{
        //CBrush background(HS_DIAGCROSS, RGB(255, 0, 0));
        //CDC *pDC = m_picture.GetDC();
    CDC DC;
	CDC* pDC  = CDC::FromHandle(hdc); 
    //DC.CreateCompatibleDC(pDC);
    //int save = DC.SaveDC();
        //DC.SelectObject(background);
        //CBitmap bmp;
        //bmp.CreateCompatibleBitmap(pDC, ImageRect.Width(), ImageRect.Height());
        //DC.SelectObject(bmp);
        //DC.FillRect(CRect(0, 0, ImageRect.Width(), ImageRect.Height()), &background);
	DrawPhotoImage(pDC, x, y, w, h, 0xFF000000,optype);
    //pDC->BitBlt(0, 0, w, h, &DC, 0, 0, SRCCOPY);// pDC->BitBlt(0, 0, w, h, &DC, 0-x, 0-y, SRCCOPY);pDC->BitBlt(x, y, w, h, &DC, 0, 0, SRCCOPY);
    //DC.RestoreDC(save);
    //DC.DeleteDC();
	return TRUE;
    //ReleaseDC(pDC);
}
//void WriteBmp(CBitmap *PBitmap,HDC hdc)
//{
//	CString m_FileName("C:\\1.jpg");
//	BITMAPINFO *m_pBMI;
//	BYTE *m_pDIBData;
//	BITMAPFILEHEADER bfh;
//	BITMAPINFOHEADER bi;
//	BITMAP BM;
//	PBitmap->GetObject(sizeof(BM),&BM);
//	//CClientDC dc(this);
//	bi.biSize   = sizeof(BITMAPINFOHEADER);
//	bi.biWidth   = BM.bmWidth;
//	bi.biHeight    = BM.bmHeight;
//	bi.biPlanes    = 1;
//	bi.biBitCount   = 24;
//	bi.biCompression = 0;
//	bi.biSizeImage   = 0;
//	bi.biXPelsPerMeter = 0;
//	bi.biYPelsPerMeter = 0;
//	bi.biClrUsed   = 0;
//	bi.biClrImportant = 0;
//	    
//	bfh.bfType=0x4d42;
//	bfh.bfOffBits=sizeof(bfh)+sizeof(BITMAPINFOHEADER);
//	bfh.bfSize=bfh.bfOffBits+BM.bmWidth*BM.bmHeight*3;
//	bfh.bfReserved1=0;
//	bfh.bfReserved2=0;
//
//	m_pBMI=(BITMAPINFO*)new char[sizeof(BITMAPINFO)];
//		m_pDIBData=(BYTE*)new char[bfh.bfSize-bfh.bfOffBits];
//
//	memcpy(m_pBMI,&bi,sizeof(BITMAPINFOHEADER));
//	GetDIBits(hdc, (HBITMAP)PBitmap->GetSafeHandle(), 0l, (DWORD)bi.biHeight,
//	(LPVOID)m_pDIBData,(LPBITMAPINFO)m_pBMI, (DWORD)DIB_RGB_COLORS);
//	CFile file;
//	if(!file.Open(m_FileName,CFile::modeCreate|CFile::modeWrite))
//	{
//		AfxMessageBox("error!");
//		return;
//	}
//	file.Write(&bfh,sizeof(bfh));
//	file.Write(m_pBMI,sizeof(BITMAPINFOHEADER));
//	file.Write(m_pDIBData,BM.bmWidth*BM.bmHeight*3);
//	file.Close();
//	delete m_pBMI;
//	delete m_pDIBData;
//}
//

void WriteBMPFile(HBITMAP hBitMap, LPTSTR filename, HDC hDC) 
{ 
    BITMAP bmp;  
    PBITMAPINFO pbmi;  
    WORD cClrBits;  
    HANDLE hf; // file handle  
    BITMAPFILEHEADER hdr; // bitmap file-header  
    PBITMAPINFOHEADER pbih; // bitmap info-header  
    LPBYTE lpBits; // memory pointer  
    DWORD dwTotal; // total count of bytes  
    DWORD cb; // incremental count of bytes  
    BYTE *hp; // byte pointer  
    DWORD dwTmp;  
    // create the bitmapinfo header information 
    if (!GetObject( hBitMap, sizeof(BITMAP), (LPTSTR)&bmp)) 
    { 
        return; 
    } 
    // Convert the color format to a count of bits.  
    cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);  

    if (cClrBits == 1)  
		cClrBits = 1;  
    else if (cClrBits <= 4)  
		cClrBits = 4;  
    else if (cClrBits <= 8)  
		cClrBits = 8;  
    else if (cClrBits <= 16)  
		cClrBits = 16;  
    else if (cClrBits <= 24)  
		cClrBits = 24;  
    else 
		cClrBits = 32;  

    // Allocate memory for the BITMAPINFO structure. 
    if (cClrBits != 24)  
		pbmi = (PBITMAPINFO) LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (1 << cClrBits));  
    else  
		pbmi = (PBITMAPINFO) LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER));  
    // Initialize the fields in the BITMAPINFO structure.  
    pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);  
    pbmi->bmiHeader.biWidth = bmp.bmWidth;  
    pbmi->bmiHeader.biHeight = bmp.bmHeight;  
    pbmi->bmiHeader.biPlanes = bmp.bmPlanes;  
    pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;  

    if (cClrBits < 24)  
		pbmi->bmiHeader.biClrUsed = (1 <<cClrBits);  
    // If the bitmap is not compressed, set the BI_RGB flag.  
    pbmi->bmiHeader.biCompression = BI_RGB;  
    // Compute the number of bytes in the array of color  
    // indices and store the result in biSizeImage.  
    pbmi->bmiHeader.biSizeImage = (pbmi->bmiHeader.biWidth + 7) /8 * pbmi->bmiHeader.biHeight * cClrBits;  
    // Set biClrImportant to 0, indicating that all of the  
    // device colors are important.  
    pbmi->bmiHeader.biClrImportant = 0;  
    // now open file and save the data 
    pbih = (PBITMAPINFOHEADER) pbmi;  
    lpBits = (LPBYTE) GlobalAlloc(GMEM_FIXED, pbih->biSizeImage); 
    if (!lpBits)  
    { 
       return; 
    } 
    // Retrieve the color table (RGBQUAD array) and the bits  
    if (!GetDIBits(hDC, HBITMAP(hBitMap), 0, (WORD) pbih->biHeight, lpBits, pbmi, DIB_RGB_COLORS))  
    { 
        return; 
    } 
    // Create the .BMP file.  
    hf = CreateFile(filename, GENERIC_READ | GENERIC_WRITE, (DWORD) 0,  
                    NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,  
                    (HANDLE) NULL);  
    if (hf == INVALID_HANDLE_VALUE) 
    { 
        return;  
    } 
    hdr.bfType = 0x4d42; // 0x42 = "B" 0x4d = "M"  
    // Compute the size of the entire file.  
    hdr.bfSize = (DWORD) (sizeof(BITMAPFILEHEADER) + pbih->biSize + pbih->biClrUsed  
                * sizeof(RGBQUAD) + pbih->biSizeImage);  
    hdr.bfReserved1 = 0;  
    hdr.bfReserved2 = 0;  
    // Compute the offset to the array of color indices.  

    hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) +  
                    pbih->biSize + pbih->biClrUsed  * sizeof (RGBQUAD);  
    // Copy the BITMAPFILEHEADER into the .BMP file.  
    if (!WriteFile(hf, (LPVOID) &hdr, sizeof(BITMAPFILEHEADER), (LPDWORD) &dwTmp, NULL))  
    { 
        return;
    } 
    // Copy the BITMAPINFOHEADER and RGBQUAD array into the file.  
    if (!WriteFile(hf, (LPVOID) pbih, sizeof(BITMAPINFOHEADER) + pbih->biClrUsed * sizeof (RGBQUAD),    (LPDWORD) &dwTmp, ( NULL))) 
    { 
        return; 
    } 
    // Copy the array of color indices into the .BMP file.  
    dwTotal = cb = pbih->biSizeImage;  
    hp = lpBits;  
    if (!WriteFile(hf, (LPSTR) hp, (int) cb, (LPDWORD) &dwTmp,NULL)) 
    { 
        return; 
    } 
    // Close the .BMP file.  

    if (!CloseHandle(hf)) 
    { 
        return; 
    } 
    // Free memory.  
    GlobalFree((HGLOBAL)lpBits); 

}
bool CPicture::DrawPhotoImage(CDC* pDC, int x, int y, int w, int h, DWORD m_TransColour,char optype)
{
	//optype = 0;
	if(w<=1||h<=1)
		return false;
	CSize m_size;
    HBITMAP m_hImage; 
    if (m_IPicture)
    {
        m_IPicture->get_Handle((OLE_HANDLE *)&m_hImage);
        if (m_hImage)
        {
			BITMAP bmp;
			if (GetObject((HBITMAP)m_hImage, sizeof(BITMAP), &bmp))
			{
				m_size.cx = bmp.bmWidth;
				m_size.cy = bmp.bmHeight;
				//DeleteObject((HGDIOBJ)bmp);
			}
			else
				return false;
        }
		else
			return false;
    }
    if (!m_hImage)
        return false;


    // create memory DC
    CDC memDC;
    memDC.CreateCompatibleDC(pDC);
    int savedmemDC = memDC.SaveDC();

    CDC* pOutputDC = &memDC;
	
	float rateX;float rateY;
	if(optype==1)
	{
		w = m_size.cx;
		h = m_size.cy;
	}
	else if(optype==2)
	{
		rateX = (float (w))/m_size.cx;
		rateY = (float (h))/m_size.cy;
		if(rateX>rateY)
		{
			h = (int)(h*(rateX/rateY)+0.5);
		}
		else
		{
			w = (int)(w*(rateY/rateX)+0.5);
		}
		optype=0;
	}
	else if(optype==3)
	{
		rateX = (float (w))/m_size.cx;
		rateY = (float (h))/m_size.cy;
		if(rateX>rateY)
		{
			w = (int)(w*(rateY/rateX)+0.5);
		}
		else
		{
			h = (int)(h*(rateX/rateY)+0.5);
		}
		optype=0;
	}
    // Get the background image for transparent images
    CBitmap BackGroundBitmap;
    CBitmap stretchbmp;
    CBitmap Transparentbmp;

    CDC BackGroundDC;
    CDC StretchDC;
    CDC TransparentDC;

    // Create a DC and bitmap for the BackGround image
    BackGroundBitmap.CreateCompatibleBitmap(pDC, w, h);
    BackGroundDC.CreateCompatibleDC(pDC);
    int savedBackGroundDC = BackGroundDC.SaveDC();
    BackGroundDC.SelectObject(&BackGroundBitmap);
    //BackGroundDC.BitBlt(x, y, x + w, y + h, pDC, x, y, SRCCOPY);
	BackGroundDC.StretchBlt( 0, 0,  w,  h, pDC, x, y, w, h, SRCCOPY);


    int savedStretchDC = 0;
    int savedTransparentDC = 0;
    {   // place bitmap image into the memory DC
        memDC.SelectObject((HBITMAP)m_hImage);

        if (m_TransColour == CLR_DEFAULT)
            m_TransColour = memDC.GetPixel(0, 0);

        if (optype==0)
        {   // stretch the image
            StretchDC.CreateCompatibleDC(pDC);
            savedStretchDC = StretchDC.SaveDC();
            stretchbmp.CreateCompatibleBitmap(pDC, w, h);
            StretchDC.SelectObject(stretchbmp);
            StretchDC.SetStretchBltMode(COLORONCOLOR);
            StretchDC.StretchBlt(0, 0, w, h, &memDC, 0, 0, m_size.cx, m_size.cy, SRCCOPY);
            pOutputDC = &StretchDC;
        }

        //if (m_DrawFlags & PJAI_TRANSPARENT)
        {   // draw the image transparently
            TransparentDC.CreateCompatibleDC(pDC);
            savedTransparentDC = TransparentDC.SaveDC();
            Transparentbmp.CreateCompatibleBitmap(pDC, w, h);
            TransparentDC.SelectObject(&Transparentbmp);
            TransparentDC.StretchBlt(0, 0, w, h, &BackGroundDC, 0, 0,w, h, SRCCOPY);
				//BackGroundDC.StretchBlt( 0, 0,  w,  h, pDC, x, y, w, h, SRCCOPY);
            DrawTransparent(&TransparentDC, w, h, pOutputDC,m_TransColour,&Transparentbmp);
            pOutputDC = &TransparentDC;
        }

    }


    pDC->BitBlt(x, y, w, h, pOutputDC, 0, 0, SRCCOPY);

    // clean up after ourselves
    if (savedTransparentDC)
    {
        TransparentDC.RestoreDC(savedTransparentDC);
        TransparentDC.DeleteDC();
		TransparentDC.m_hDC = NULL;
    }

    if (savedStretchDC)
    {
        StretchDC.RestoreDC(savedStretchDC);
        StretchDC.DeleteDC();
		StretchDC.m_hDC = NULL;
    }

    memDC.RestoreDC(savedmemDC);
    memDC.DeleteDC();
	memDC.m_hDC = NULL;
    BackGroundDC.RestoreDC(savedBackGroundDC);
    BackGroundDC.DeleteDC();
	BackGroundDC.m_hDC = NULL;
    if (m_hImage )
    {
        DeleteObject((HGDIOBJ)m_hImage);
    }
	return TRUE;
}

void CPicture::DrawTransparent(CDC *pToDC, int w, int h, CDC *pFromDC,DWORD m_TransColour,CBitmap* stretchbmp)
{									//背景内容			//照片内容
    // handle for the grayscale image
    HBITMAP Gray = NULL;

    CDC *pOutputDC = pFromDC;//照片内容

    CDC MonoDC;
    MonoDC.CreateCompatibleDC(pToDC);//背景内容

    //CDC DisabledDC;
    //DisabledDC.CreateCompatibleDC(pToDC);//背景内容
    //CBitmap DisabledBitmap;

    int savedToDC = pToDC->SaveDC();
    int savedFromDC = pFromDC->SaveDC();
    int savedMonoDC = MonoDC.SaveDC();
    //int savedDisabledDC = DisabledDC.SaveDC();

    pToDC->SetBkColor(RGB(255, 255, 255));
    pToDC->SetTextColor(RGB(0, 0, 0));
    pFromDC->SetBkColor(m_TransColour);
    

    CBitmap MonoDCbmp;
    MonoDCbmp.CreateBitmap(w, h, 1, 1, NULL);
    MonoDC.SelectObject(&MonoDCbmp);

    MonoDC.BitBlt(0, 0, w, h, pOutputDC, 0, 0, SRCCOPY);//照片内容考入MonoDC SRCCOPY Copies the source rectangle directly to the destination rectangle. 
	//WriteBMPFile((HBITMAP)MonoDCbmp.m_hObject, "C:\\Mono.bmp", MonoDC.GetSafeHdc()) ;

	WriteBMPFile((HBITMAP)stretchbmp->m_hObject, L"C:\\stretch0.bmp", pToDC->GetSafeHdc()) ;
    pToDC->BitBlt(0, 0, w, h, pOutputDC, 0, 0, SRCINVERT);//照片内容反相后考入pToDC ; Combines the colors of the source and destination rectangles by using the Boolean XOR operator.
	//WriteBMPFile((HBITMAP)stretchbmp->m_hObject, "C:\\stretch1.bmp", pToDC->GetSafeHdc()) ;

	pToDC->BitBlt(0, 0, w, h, &MonoDC, 0, 0, SRCAND);     //SRCAND Combines the colors of the source and destination rectangles by using the Boolean AND operator. 
	//WriteBMPFile((HBITMAP)stretchbmp->m_hObject, "C:\\stretch2.bmp", pToDC->GetSafeHdc()) ;
	
	pToDC->BitBlt(0, 0, w, h, pOutputDC, 0, 0, SRCINVERT);
	//WriteBMPFile((HBITMAP)stretchbmp->m_hObject, "C:\\stretch3.bmp", pToDC->GetSafeHdc()) ;
    //DisabledDC.RestoreDC(savedDisabledDC);
    //DisabledDC.DeleteDC();

    MonoDC.RestoreDC(savedMonoDC);
    MonoDC.DeleteDC();
	MonoDC.m_hDC = NULL;

    pFromDC->RestoreDC(savedFromDC);
    pToDC->RestoreDC(savedToDC);


}


/*
void DisplayImage(HDC hDC, LPCTSTR szImagePath)
{
HANDLE hFile=CreateFile(szImagePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); //从指定的路径szImagePath中读取文件句柄
DWORD dwFileSize=GetFileSize(hFile, NULL); //获得图片文件的大小，用来分配全局内存

HGLOBAL hImageMemory=GlobalAlloc(GMEM_MOVEABLE, dwFileSize); //给图片分配全局内存

void *pImageMemory=GlobalLock(hImageMemory); //锁定内存

DWORD dwReadedSize; //保存实际读取的文件大小

ReadFile(hFile, pImageMemory, dwFileSize, &dwReadedSize, NULL); //读取图片到全局内存当中

GlobalUnlock(hImageMemory); //解锁内存

CloseHandle(hFile); //关闭文件句柄

IStream *pIStream;//创建一个IStream接口指针，用来保存图片流

IPicture *pIPicture;//创建一个IPicture接口指针，表示图片对象

CreateStreamOnHGlobal(hImageMemory, false, &pIStream) //用全局内存初使化IStream接口指针

OleLoadPicture(pIStream, 0, false, IID_IPicture, (LPVOID*)&(pIPicture));//用OleLoadPicture获得IPicture接口指针

//得到IPicture COM接口对象后，你就可以进行获得图片信息、显示图片等操作

OLE_XSIZE_HIMETRIC hmWidth;

LE_YSIZE_HIMETRIC hmHeight;

pIPicture->get_Width(&hmWidth); //用接口方法获得图片的宽和高
pIPicture->get_Height(&hmHeight);
pIPicture->Render(hDC,0,0,100,100,0,hmHeight,hmWidth,-hmHeight,NULL); //在指定的DC上绘出图片
GlobalFree(hImageMemory); //释放全局内存
pIStream->Release(); //释放pIStream
pIPicture->Release(); //释放pIPicture
}
*/


BOOL CPicture::Load(byte* pszFile,int len)
{
	HRESULT hr = S_FALSE;

	HGLOBAL hImageMemory=GlobalAlloc(GMEM_MOVEABLE, len); //给图片分配全局内存
	if(hImageMemory)
	{
		IStream *pIStream = NULL;//创建一个IStream接口指针，用来保存图片流
		void *pImageMemory=GlobalLock(hImageMemory); //锁定内存

		if(pImageMemory)
		{
			memcpy(pImageMemory,pszFile,len);
			GlobalUnlock(hImageMemory); //解锁内存
			CreateStreamOnHGlobal(hImageMemory, false, &pIStream); //用全局内存初使化IStream接口指针
			if(pIStream)
				hr = OleLoadPicture(pIStream, 0, false, IID_IPicture, (LPVOID*)&(m_IPicture));//用OleLoadPicture获得IPicture接口指针
		}
		else
			GlobalUnlock(hImageMemory); //解锁内存

		GlobalFree(hImageMemory); //释放全局内存
		if(pIStream)
			pIStream->Release(); //释放pIStream
	}

	return (SUCCEEDED(hr) && m_IPicture);
}




BOOL CPicture::Load(LPCTSTR pszPathName)
{
	CFile file;
	if (!file.Open(pszPathName, CFile::modeRead|CFile::shareDenyWrite))
		return FALSE;
	CArchive ar(&file, CArchive::load | CArchive::bNoFlushOnDelete);
	CArchiveStream arcstream(&ar);

	HRESULT hr = OleLoadPicture(((IStream*)&arcstream), 0, FALSE,
		IID_IPicture, (void**)&m_IPicture);


	file.Close();
	return (SUCCEEDED(hr) && m_IPicture);
}

