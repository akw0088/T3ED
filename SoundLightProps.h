#if !defined(AFX_SOUNDLIGHTPROPS_H__A6D0357F_1678_4335_A8AD_066DEF8C6FAC__INCLUDED_)
#define AFX_SOUNDLIGHTPROPS_H__A6D0357F_1678_4335_A8AD_066DEF8C6FAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SoundLightProps.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSoundLightProps dialog

class CSoundLightProps : public CDialog
{
// Construction
public:
	int maxblock;
	BOOL bSLAutoObjMem;
	CSoundLightProps(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSoundLightProps)
	enum { IDD = IDD_LSPROPS };
	CEdit	m_tyyppic;
	CEdit	m_tyyppihexc;
	long	m_tyyppi;
	CString	m_tyyppihex;
	CString	m_strText1;
	int		m_intBlock;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSoundLightProps)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSoundLightProps)
	afx_msg void OnChangeEdit1();
	virtual BOOL OnInitDialog();
	afx_msg void OnKillfocusEditBlock();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOUNDLIGHTPROPS_H__A6D0357F_1678_4335_A8AD_066DEF8C6FAC__INCLUDED_)
