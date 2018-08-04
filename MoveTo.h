#if !defined(AFX_MOVETO_H__58096E15_7B3D_4EAC_9DD5_618DC888A7EE__INCLUDED_)
#define AFX_MOVETO_H__58096E15_7B3D_4EAC_9DD5_618DC888A7EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MoveTo.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CMoveTo 

class CMoveTo : public CDialog
{
// Konstruktion
public:
	BOOL block_mode;
	//BOOL no_smooth;
	CMoveTo(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CMoveTo)
	enum { IDD = IDD_MOVETO };
	float	m_dest_x;
	float	m_dest_y;
	float	m_dest_z;
	BOOL	m_chk_no_smooth;
	int		m_int_startblock;
	int		m_int_endblock;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CMoveTo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CMoveTo)
	virtual BOOL OnInitDialog();
	afx_msg void OnChecknosmooth();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_MOVETO_H__58096E15_7B3D_4EAC_9DD5_618DC888A7EE__INCLUDED_
