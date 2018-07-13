//-----------------------------------------------------------------------------
// Picture (Prototypes) Version 1.00
//
// Routins 4 Showing Picture Files... (.BMP .DIB .EMF .GIF .ICO .JPG .WMF)
//
// Author: Dr. Yovav Gad, EMail: Sources@SuperMain.com ,Web: www.SuperMain.com
//=============================================================================

#if !defined(AFX_PICTURE_H__COPYFREE_BY_YOVAV_GAD__SOURCES_AT_SUPERMAIN_DOT_COM__INCLUDED_)
#define AFX_PICTURE_H__COPYFREE_BY_YOVAV_GAD__SOURCES_AT_SUPERMAIN_DOT_COM__INCLUDED_

#if _MSC_VER > 1000

#pragma once
#endif // _MSC_VER > 1000
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT
#include "Ole2.h"

class CPicture
{
public:
	CPicture();
	virtual ~CPicture();

	void	FreePictureData();
	bool	Draw(HDC hdc,int x,int y,int width,int high);;
	BOOL	Load(LPCTSTR pszPathName);

	BOOL	Load(byte* pszFile,int len);

	bool    DrawPhoto(HDC hdc, int x, int y, int w, int h, DWORD m_TransColour,char optype);
private:
	void    DrawTransparent(CDC *pToDC, int w, int h, CDC *pFromDC,DWORD m_TransColour,CBitmap* stretchbmp);
	bool	DrawPhotoImage(CDC* pDC, int x, int y, int w, int h, DWORD m_TransColour,char optype);
                     // the image size
    //COLORREF m_TransparentColour;       // the transparent colour
	IPicture* m_IPicture; // Same As LPPICTURE (typedef IPicture __RPC_FAR *LPPICTURE)

};

#endif // !defined(AFX_PICTURE_H__COPYFREE_BY_YOVAV_GAD__SOURCES_AT_SUPERMAIN_DOT_COM__INCLUDED_)
