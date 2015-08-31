// mfc6.h : main header file for the MFC6 application
//

#if !defined(AFX_MFC6_H__CA3282BA_80A7_493A_82A0_3BE56A6481B0__INCLUDED_)
#define AFX_MFC6_H__CA3282BA_80A7_493A_82A0_3BE56A6481B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMfc6App:
// See mfc6.cpp for the implementation of this class
//

class CMfc6App : public CWinApp
{
public:
	CMfc6App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMfc6App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMfc6App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFC6_H__CA3282BA_80A7_493A_82A0_3BE56A6481B0__INCLUDED_)
