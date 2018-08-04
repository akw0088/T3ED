#if !defined(AFX_LIGHTSOUNDPROPS_H__D1860DEC_AAAD_422E_9F07_E315E48DD75D__INCLUDED_)
#define AFX_LIGHTSOUNDPROPS_H__D1860DEC_AAAD_422E_9F07_E315E48DD75D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LightSoundProps.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// LightSoundProps dialog

class LightSoundProps : public CDialog
{
// Construction
public:
	LightSoundProps(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(LightSoundProps)
	enum { IDD = IDD_LIGHTSOUNDSRCPROPS };
	long	m_soundlighttype;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(LightSoundProps)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(LightSoundProps)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LIGHTSOUNDPROPS_H__D1860DEC_AAAD_422E_9F07_E315E48DD75D__INCLUDED_)
