#if !defined(AFX_FINGPOLYDLG_H__9995A8A2_D4A3_45E0_AE2A_3D31E99178C5__INCLUDED_)
#define AFX_FINGPOLYDLG_H__9995A8A2_D4A3_45E0_AE2A_3D31E99178C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FingPolyDlg.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CFingPolyDlg 

lass CFingPolyDlg : public CDialog
{
// Konstruktion
public:
	CFingPolyDlg(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CFingPolyDlg)
	enum { IDD = IDD_FINDPOLYGON };
		// HINWEIS: Der Klassen-Assistent fügt hier Datenelemente ein
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CFingPolyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CFingPolyDlg)
	afx_msg void OnBlockselRadio1();
	afx_msg void OnBlockselRadio2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_FINGPOLYDLG_H__9995A8A2_D4A3_45E0_AE2A_3D31E99178C5__INCLUDED_
