// T3ED.h : main header file for the T3ED application
//

#if !defined(AFX_T3ED_H__EA21D108_8ED3_11D2_8486_000000000000__INCLUDED_)
#define AFX_T3ED_H__EA21D108_8ED3_11D2_8486_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CT3EDApp:
// See T3ED.cpp for the implementation of this class
//

class CT3EDApp : public CWinApp
{
public:
	CT3EDApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CT3EDApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CT3EDApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//HOO: (1)
extern HCURSOR hcursorRotate;
extern HCURSOR hcursorMove;
//HOO: (1)


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_T3ED_H__EA21D108_8ED3_11D2_8486_000000000000__INCLUDED_)
