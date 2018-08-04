#if !defined(AFX_NEWOBJDLG_H__06E1FC81_A896_11D2_903B_DA901B29DE7D__INCLUDED_)
#define AFX_NEWOBJDLG_H__06E1FC81_A896_11D2_903B_DA901B29DE7D__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// NewobjDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewobjDlg dialog

class CNewobjDlg : public CDialog
{
// Construction
public:
	int xobj_crosstype;
	CNewobjDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNewobjDlg)
	enum { IDD = IDD_NEWOBJECT };
	int		m_chunk;
	int		m_xobj;
	int		m_texture;
	//}}AFX_DATA
	short m_flags;

	CQFSView *qfs;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewobjDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewobjDlg)
	afx_msg void OnBrowse();
	virtual BOOL OnInitDialog();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWOBJDLG_H__06E1FC81_A896_11D2_903B_DA901B29DE7D__INCLUDED_)
