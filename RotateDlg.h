#if !defined(AFX_ROTATEDLG_H__70876FF9_29D0_4A58_AD20_6197EC31FDFB__INCLUDED_)
#define AFX_ROTATEDLG_H__70876FF9_29D0_4A58_AD20_6197EC31FDFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RotateDlg.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CRotateDlg 

class CRotateDlg : public CDialog
{
// Konstruktion
public:
	CRotateDlg(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CRotateDlg)
	enum { IDD = IDD_ROTATE };
	double	m_edit_angle;
	CString	m_str_rtext;
	//}}AFX_DATA


// �berschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktions�berschreibungen
	//{{AFX_VIRTUAL(CRotateDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterst�tzung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CRotateDlg)
		// HINWEIS: Der Klassen-Assistent f�gt hier Member-Funktionen ein
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ f�gt unmittelbar vor der vorhergehenden Zeile zus�tzliche Deklarationen ein.

#endif // AFX_ROTATEDLG_H__70876FF9_29D0_4A58_AD20_6197EC31FDFB__INCLUDED_
