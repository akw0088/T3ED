#if !defined(AFX_VIRTUALROADADJUST_H__8C200E94_931C_4173_829D_C2A7DFBBEB2C__INCLUDED_)
#define AFX_VIRTUALROADADJUST_H__8C200E94_931C_4173_829D_C2A7DFBBEB2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VirtualRoadAdjust.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CVirtualRoadAdjust dialog

class CVirtualRoadAdjust : public CDialog
{
// Construction
public:
	CVirtualRoadAdjust(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CVirtualRoadAdjust)
	enum { IDD = IDD_ADJUSTVR };
	int		m_AdjustType;
	BOOL	m_MapLaneEdges;
	int		m_StartBlock;
	int		m_EndBlock;
	float	m_float_roadwidth;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVirtualRoadAdjust)
	public:
	virtual void OnFinalRelease();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CVirtualRoadAdjust)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIRTUALROADADJUST_H__8C200E94_931C_4173_829D_C2A7DFBBEB2C__INCLUDED_)
