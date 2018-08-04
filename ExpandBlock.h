#if !defined(AFX_EXPANDBLOCK_H__E70E8912_2FDF_4D02_809E_E518478CC00E__INCLUDED_)
#define AFX_EXPANDBLOCK_H__E70E8912_2FDF_4D02_809E_E518478CC00E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExpandBlock.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld ExpandBlock 

class ExpandBlock : public CDialog
{
// Konstruktion
public:
	ExpandBlock(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(ExpandBlock)
	enum { IDD = IDD_EXPANDBLOCK };
	float	m_edit_height;
	float	m_edit_lenght;
	float	m_edit_width;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(ExpandBlock)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(ExpandBlock)
		// HINWEIS: Der Klassen-Assistent fügt hier Member-Funktionen ein
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_EXPANDBLOCK_H__E70E8912_2FDF_4D02_809E_E518478CC00E__INCLUDED_
