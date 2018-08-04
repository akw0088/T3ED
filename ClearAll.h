#if !defined(AFX_CLEARALL_H__CB8546C1_364C_11D3_BA8C_444553540000__INCLUDED_)
#define AFX_CLEARALL_H__CB8546C1_364C_11D3_BA8C_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ClearAll.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CClearAll dialog

class CClearAll : public CDialog
{
// Construction
public:
	CClearAll(CWnd* pParent = NULL);   // standard constructor

	CQFSView *qfs;

// Dialog Data
	//{{AFX_DATA(CClearAll)
	enum { IDD = IDD_CLEARTRACK };
	BOOL	m_trackXY;
	BOOL	m_trackZ;
	BOOL	m_sceneryZ;
	BOOL	m_xobj;
	BOOL	m_polyobj;
	BOOL	m_roadTextures;
	BOOL	m_sceneryTextures;
	int		m_roadTexture;
	int		m_sceneryTexture;
	BOOL	m_fences;
	BOOL	m_globalobj;
	BOOL	m_trackBLK;
	int		m_TrknBlocks;
	BOOL	m_trackShade;
	BOOL	m_block_neighbours;
	//}}AFX_DATA
	short m_roadFlags;
	short m_sceneryFlags;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClearAll)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CClearAll)
	afx_msg void OnBrowse6();
	afx_msg void OnBrowse7();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLEARALL_H__CB8546C1_364C_11D3_BA8C_444553540000__INCLUDED_)
