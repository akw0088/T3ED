#if !defined(AFX_FRDFILEDLG_H__BBDAE85E_E61A_4DCD_A26F_8B458EC74945__INCLUDED_)
#define AFX_FRDFILEDLG_H__BBDAE85E_E61A_4DCD_A26F_8B458EC74945__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FrdFileDlg.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CFrdFileDlg 

class CFrdFileDlg : public CDialog
{
// Konstruktion
public:
	CFrdFileDlg(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CFrdFileDlg)
	enum { IDD = IDD_FILE_PROPERTIES };
	CString	m_str_File_Format;
	int		m_int_nBlocks;
	short	m_short_nVRoad;
	//}}AFX_DATA


// �berschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktions�berschreibungen
	//{{AFX_VIRTUAL(CFrdFileDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterst�tzung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CFrdFileDlg)
		// HINWEIS: Der Klassen-Assistent f�gt hier Member-Funktionen ein
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ f�gt unmittelbar vor der vorhergehenden Zeile zus�tzliche Deklarationen ein.

#endif // AFX_FRDFILEDLG_H__BBDAE85E_E61A_4DCD_A26F_8B458EC74945__INCLUDED_
