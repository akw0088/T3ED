#if !defined(AFX_LODSTARTEND_H__EDBEE79B_30AB_4723_82CB_E923F7DAE79D__INCLUDED_)
#define AFX_LODSTARTEND_H__EDBEE79B_30AB_4723_82CB_E923F7DAE79D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LodStartEnd.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CLodStartEnd 

class CLodStartEnd : public CDialog
{
// Konstruktion
public:
	CLodStartEnd(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CLodStartEnd)
	enum { IDD = IDD_LOD_STARTEND };
	int		m_iLodEnd;
	int		m_iLodStart;
	//}}AFX_DATA


// �berschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktions�berschreibungen
	//{{AFX_VIRTUAL(CLodStartEnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterst�tzung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CLodStartEnd)
		// HINWEIS: Der Klassen-Assistent f�gt hier Member-Funktionen ein
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ f�gt unmittelbar vor der vorhergehenden Zeile zus�tzliche Deklarationen ein.

#endif // AFX_LODSTARTEND_H__EDBEE79B_30AB_4723_82CB_E923F7DAE79D__INCLUDED_
