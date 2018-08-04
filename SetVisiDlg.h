#if !defined(AFX_SETVISIDLG_H__17F9CDD4_2B31_4374_90A4_C4DEC14C1986__INCLUDED_)
#define AFX_SETVISIDLG_H__17F9CDD4_2B31_4374_90A4_C4DEC14C1986__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetVisiDlg.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CSetVisiDlg 

class CSetVisiDlg : public CDialog
{
// Konstruktion
public:
	CSetVisiDlg(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CSetVisiDlg)
	enum { IDD = IDD_SET_VISI };
	int		m_int_end;
	int		m_int_num_blocks;
	int		m_edit_start;
	int		m_int_tr_bl_fromto;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CSetVisiDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CSetVisiDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSetfocusEditStart();
	afx_msg void OnSetfocusEditEnd();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_SETVISIDLG_H__17F9CDD4_2B31_4374_90A4_C4DEC14C1986__INCLUDED_
