#if !defined(AFX_LODPROGRESS_H__3339B6C4_9E60_4317_9A51_C6B92E747B6F__INCLUDED_)
#define AFX_LODPROGRESS_H__3339B6C4_9E60_4317_9A51_C6B92E747B6F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LodProgress.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// LodProgress dialog

class LodProgress : public CDialog
{
// Construction
public:
	LodProgress(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(LodProgress)
	enum { IDD = IDD_DIALOG2 };
	CButton	m_okbutton;
	CString	m_progress;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(LodProgress)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(LodProgress)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LODPROGRESS_H__3339B6C4_9E60_4317_9A51_C6B92E747B6F__INCLUDED_)
