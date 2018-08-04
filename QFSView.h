#if !defined(AFX_QFSVIEW_H__BD580821_A660_11D2_BA8C_444553540000__INCLUDED_)
#define AFX_QFSVIEW_H__BD580821_A660_11D2_BA8C_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// QFSView.h : header file
//
// Modified by Hoo, 8/16/1999 (Total: 1 item / 2 message map functions added by MFC (Comment by Nappe1: "are they still there ??"))
/////////////////////////////////////////////////////////////////////////////
// CQFSView dialog
// Hoo's D3D View functions added by Nappe1, (2/11/2001)


class CQFSView : public CDialog
{
// Construction
public:
	CQFSView(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CQFSView)
	enum { IDD = IDD_TEXTURES };
	CButton	m_hsprops;
	CSpinButtonCtrl	m_spinCtrl;
	CStatic	m_bmpCtrl;
	CString	m_numBmp;
	int		m_nBmp;
	CString	m_strLabel;
	//}}AFX_DATA

	BOOL promptLoad, bIsLoadQfs;
	BOOL bHSMode;
	BOOL bHasHitFlags;
	short m_flags;
	unsigned char texanimdata; //for texture animation data...
	CString defPath,defName;
	HBITMAP *pBitmaps; // if NULL, not loaded
	HBITMAP pStockBitmaps[14];
	//HOO: These are images for lane texture (1)
	HBITMAP pStockTextures[11];
	//HOO: (1)
	struct TEXTUREBLOCK *col;
	int nQfsEntries,nColEntries;

	BOOL QFSReachOffset(CFile &qfs,int toreach,unsigned char *x,int &bufpos,int &bufshift);
	int LoadQFS();
	void Reset();
	void MakeStockBitmaps();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQFSView)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CQFSView)
	afx_msg void OnChangeEdit1();
	afx_msg void OnFileopen();
	afx_msg void OnHsprops();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CQFSLoadProgress dialog

class CQFSLoadProgress : public CDialog
{
// Construction
public:
	CQFSLoadProgress(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CQFSLoadProgress)
	enum { IDD = IDD_LOADING_QFS };
	CProgressCtrl	m_progress;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQFSLoadProgress)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CQFSLoadProgress)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QFSVIEW_H__BD580821_A660_11D2_BA8C_444553540000__INCLUDED_)
