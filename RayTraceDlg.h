#if !defined(AFX_RAYTRACEDLG_H__E713F52B_0872_4F14_96C7_0283B32B38EF__INCLUDED_)
#define AFX_RAYTRACEDLG_H__E713F52B_0872_4F14_96C7_0283B32B38EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RayTraceDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRayTraceDlg dialog

class CRayTraceDlg : public CDialog
{
// Construction
public:
	CRayTraceDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRayTraceDlg)
	enum { IDD = IDD_RAYTRACE };
	float	m_theta;
	float	m_rho;
	int		m_iEndBlock;
	int		m_iStartBlock;
	BOOL	m_bCheckTrack;
	BOOL	m_bCheckObjects;
	CString	m_text_endbl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRayTraceDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRayTraceDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RAYTRACEDLG_H__E713F52B_0872_4F14_96C7_0283B32B38EF__INCLUDED_)
