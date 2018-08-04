#if !defined(AFX_EXPANDDLG_H__2F45FD91_2104_4D1B_B665_19B8D622B22A__INCLUDED_)
#define AFX_EXPANDDLG_H__2F45FD91_2104_4D1B_B665_19B8D622B22A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExpandDlg.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CExpandDlg 

class CExpandDlg : public CDialog
{
// Konstruktion
public:
	CExpandDlg(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CExpandDlg)
	enum { IDD = IDD_Expand };
	float	m_floatExpMulti_x;
	float	m_floatExpMulti_y;
	float	m_floatExpMulti_z;
	BOOL	m_chk_connected;
	int		m_radio_alignment;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CExpandDlg)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CExpandDlg)
	afx_msg void OnCheckConnect();
	virtual BOOL OnInitDialog();
	afx_msg void OnCancelMode();
	afx_msg void OnKillfocusEditx();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void UpdateCheckConnectView();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_EXPANDDLG_H__2F45FD91_2104_4D1B_B665_19B8D622B22A__INCLUDED_
