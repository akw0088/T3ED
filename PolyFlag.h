#if !defined(AFX_POLYFLAG_H__AD1BC721_B87C_11D2_903B_94DC5004CA99__INCLUDED_)
#define AFX_POLYFLAG_H__AD1BC721_B87C_11D2_903B_94DC5004CA99__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// PolyFlag.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPolyFlags dialog

class CPolyFlags : public CDialog
{
// Construction
public:
	BOOL bTypeOnly;
	CPolyFlags(CWnd* pParent = NULL);   // standard constructor
	BOOL bHSMode;

// Dialog Data
	//{{AFX_DATA(CPolyFlags)
	enum { IDD = IDD_POLYFLAGS };
	int		m_type;
	int		m_walldetect;
	int		m_xobjdetect;
	int		m_hsForwNeighbor;
	int		m_hsBackNeighbor;
	int		m_hsLeftNeighbor;
	int		m_hsRightNeighbor;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPolyFlags)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPolyFlags)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POLYFLAG_H__AD1BC721_B87C_11D2_903B_94DC5004CA99__INCLUDED_)
