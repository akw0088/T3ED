// T3EDFrame.h : interface of the CT3EDFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_T3EDFRAME_H__EA21D10C_8ED3_11D2_8486_000000000000__INCLUDED_)
#define AFX_T3EDFRAME_H__EA21D10C_8ED3_11D2_8486_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CT3EDFrame : public CFrameWnd
{
protected: // create from serialization only
	CT3EDFrame();
	DECLARE_DYNCREATE(CT3EDFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CT3EDFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CT3EDFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CT3EDFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_T3EDFRAME_H__EA21D10C_8ED3_11D2_8486_000000000000__INCLUDED_)
