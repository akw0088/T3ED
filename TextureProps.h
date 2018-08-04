#if !defined(AFX_TEXTUREPROPS_H__4FFF5081_F578_11D4_BA8C_444553540000__INCLUDED_)
#define AFX_TEXTUREPROPS_H__4FFF5081_F578_11D4_BA8C_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// TextureProps.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTextureProps dialog

class CTextureProps : public CDialog
{
// Construction
public:
	CTextureProps(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTextureProps)
	enum { IDD = IDD_TEXTUREPROPS };
	CEdit	m_AnimLenCtrl;
	CEdit	m_AnimDurCtrl;
	int		m_rotation;
	int		m_tiling;
	BOOL	m_flipv;
	BOOL	m_fliph;
	BOOL	m_twosided;
	short	m_animduration;
	short	m_animlenght;
	BOOL	m_animated;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTextureProps)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTextureProps)
	afx_msg void OnAnimtex();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTUREPROPS_H__4FFF5081_F578_11D4_BA8C_444553540000__INCLUDED_)
