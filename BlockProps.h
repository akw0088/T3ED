#if !defined(AFX_BLOCKPROPS_H__0F204D03_5CD5_4BE1_A940_8C366BD28A5C__INCLUDED_)
#define AFX_BLOCKPROPS_H__0F204D03_5CD5_4BE1_A940_8C366BD28A5C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BlockProps.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBlockProps dialog

class CBlockProps : public CDialog
{
// Construction
public:
	CBlockProps(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBlockProps)
	enum { IDD = IDD_TRKBLOCK_DIALOG };
	CListBox	m_clist1;
	long	m_Neigh1;
	long	m_Neigh2;
	long	m_Neigh3;
	long	m_Neigh4;
	long	m_Neigh5;
	long	m_Neigh6;
	long	m_Neigh7;
	long	m_Neigh8;
	short	m_dbstart;
	short	m_dbstop;
	CString	m_clist1value;
	BOOL	m_bool_merge;
	float	m_float_Dist;
	//}}AFX_DATA
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBlockProps)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBlockProps)
	afx_msg void OnCheckMerge();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void UpdateView();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BLOCKPROPS_H__0F204D03_5CD5_4BE1_A940_8C366BD28A5C__INCLUDED_)
