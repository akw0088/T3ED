#if !defined(AFX_FINDPOLYGONDLG_H__0DC696F7_5016_468C_8B57_EE7A2D70A032__INCLUDED_)
#define AFX_FINDPOLYGONDLG_H__0DC696F7_5016_468C_8B57_EE7A2D70A032__INCLUDED_

#include "QFSView.h"	// Hinzugefügt von der Klassenansicht
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FindPolygonDlg.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CFindPolygonDlg 

class CFindPolygonDlg : public CDialog
{
// Konstruktion
public:
	unsigned char pvrflags;
	BOOL bHSMode;
	unsigned char texanimdata;
	short hs_flags;
	CQFSView *qfs;
	CFindPolygonDlg(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CFindPolygonDlg)
	enum { IDD = IDD_FINDPOLYGON };
	BOOL	m_trackpolys;
	BOOL	m_extraobj;
	BOOL	m_polygonobj;
	int		m_texture;
	int		m_endbl;
	int		m_startbl;
	int		m_block_choice;
	BOOL	m_chk_texture;
	BOOL	m_chk_hstexture;
	BOOL	m_chk_animated;
	BOOL	m_chk_pvrflags;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CFindPolygonDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CFindPolygonDlg)
	afx_msg void OnBlockRadio1();
	afx_msg void OnBlockRadio2();
	afx_msg void OnDoubleclickedBlockRadio1();
	afx_msg void OnDoubleclickedBlockRadio2();
	afx_msg void OnButtonSelectTexture();
	afx_msg void OnSetfocusEditEndbl();
	afx_msg void OnSetfocusEditStartbl();
	afx_msg void OnKillfocusEditStartbl();
	afx_msg void OnKillfocusEditEndbl();
	afx_msg void OnCheckHstexture();
	afx_msg void OnCheckAnimated();
	afx_msg void OnButtonSelectPvrflags();
	afx_msg void OnCheckPvrflags();
	afx_msg void OnCheckPolyobj();
	afx_msg void OnCheckExtraobj();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_FINDPOLYGONDLG_H__0DC696F7_5016_468C_8B57_EE7A2D70A032__INCLUDED_
