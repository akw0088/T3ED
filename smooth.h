#if !defined(AFX_SMOOTH_H__C29FD461_A427_11D2_BA8C_444553540000__INCLUDED_)
#define AFX_SMOOTH_H__C29FD461_A427_11D2_BA8C_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// smooth.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSmoothingBox1 dialog

class CSmoothingBox1 : public CDialog
{
// Construction
public:
	CSmoothingBox1(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSmoothingBox1)
	enum { IDD = IDD_SMOOTHING1 };
	BOOL	m_extraSmooth;
	//}}AFX_DATA

	int m_sWidth,m_sDist;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSmoothingBox1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSmoothingBox1)
	afx_msg void OnCheckExtraSmoothing();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CSmoothingBox2 dialog

class CSmoothingBox2 : public CDialog
{
// Construction
public:
	CSmoothingBox2(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSmoothingBox2)
	enum { IDD = IDD_SMOOTHING2 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	int m_sWidth,m_sDist;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSmoothingBox2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSmoothingBox2)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMOOTH_H__C29FD461_A427_11D2_BA8C_444553540000__INCLUDED_)
