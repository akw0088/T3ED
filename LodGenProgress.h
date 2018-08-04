#if !defined(AFX_LODGENPROGRESS_H__D7EAF1FB_65C8_485C_A140_73EBF34DC41D__INCLUDED_)
#define AFX_LODGENPROGRESS_H__D7EAF1FB_65C8_485C_A140_73EBF34DC41D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LodGenProgress.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLodGenProgress dialog

class CLodGenProgress : public CDialog
{
// Construction
public:
	CLodGenProgress(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLodGenProgress)
	enum { IDD = IDD_LOD_GENERATION };
	CProgressCtrl	m_lodprogress;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLodGenProgress)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLodGenProgress)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LODGENPROGRESS_H__D7EAF1FB_65C8_485C_A140_73EBF34DC41D__INCLUDED_)
