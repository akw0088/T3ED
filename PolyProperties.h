#if !defined(AFX_POLYPROPERTIES_H__BF3E434A_B5F2_44D3_89F9_038F1684B52E__INCLUDED_)
#define AFX_POLYPROPERTIES_H__BF3E434A_B5F2_44D3_89F9_038F1684B52E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PolyProperties.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CPolyProperties 

class CPolyProperties : public CDialog
{
// Konstruktion
public:
	BOOL m_bPolyPropsAutoObjMem;
	CPolyProperties(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CPolyProperties)
	enum { IDD = IDD_OBJPOLYPROPS };
	int		m_intBlock;
	int		m_intNumPoly;
	int		m_int_ObjNum;
	int		m_int_Chunk;
	BOOL	m_bool_convert;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CPolyProperties)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CPolyProperties)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditChunk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_POLYPROPERTIES_H__BF3E434A_B5F2_44D3_89F9_038F1684B52E__INCLUDED_
