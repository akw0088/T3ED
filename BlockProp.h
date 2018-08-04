#if !defined(AFX_BLOCKPROP_H__745A631C_7CA9_4F92_BD36_EDF951122835__INCLUDED_)
#define AFX_BLOCKPROP_H__745A631C_7CA9_4F92_BD36_EDF951122835__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BlockProp.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CBlockProp 

class CBlockProp : public CDialog
{
// Konstruktion
public:
	CBlockProp(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CBlockProp)
	enum { IDD = IDD_BLOCK_PROPS };
	float	m_float_ptCentreX;
	float	m_float_ptCentreY;
	float	m_float_ptCentreZ;
	long	m_nVertices;
	long	m_nHiResVert;
	long	m_nLoResVert;
	long	m_nMedResVert;
	long	m_nLightsrc;
	long	m_nSoundsrc;
	//}}AFX_DATA


// �berschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktions�berschreibungen
	//{{AFX_VIRTUAL(CBlockProp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterst�tzung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CBlockProp)
		// HINWEIS: Der Klassen-Assistent f�gt hier Member-Funktionen ein
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ f�gt unmittelbar vor der vorhergehenden Zeile zus�tzliche Deklarationen ein.

#endif // AFX_BLOCKPROP_H__745A631C_7CA9_4F92_BD36_EDF951122835__INCLUDED_
