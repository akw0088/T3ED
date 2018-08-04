#if !defined(AFX_BLOCKPASTE_H__B188FD80_9D1B_4229_9AE6_A46216126DB8__INCLUDED_)
#define AFX_BLOCKPASTE_H__B188FD80_9D1B_4229_9AE6_A46216126DB8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BlockPaste.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CBlockPaste 

class CBlockPaste : public CDialog
{
// Konstruktion
public:
	CBlockPaste(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CBlockPaste)
	enum { IDD = IDD_BLOCKPASTE };
	int		m_add_replace_block;
	BOOL	m_bool_move;
	BOOL	m_bool_rotate;
	BOOL	m_bool_move_z;
	BOOL	m_bool_expand_lenght;
	BOOL	m_bool_expand_width;
	BOOL	m_bool_paste_heights;
	BOOL	m_bool_paste_spdfa;
	BOOL	m_bool_paste_vroad;
	BOOL	m_bool_paste_block;
	BOOL	m_bool_paste_spdra;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CBlockPaste)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CBlockPaste)
	afx_msg void OnRadioAddReplace1();
	afx_msg void OnRadioAddReplace2();
	virtual BOOL OnInitDialog();
	afx_msg void OnDoubleclickedRadioAddReplace1();
	afx_msg void OnDoubleclickedRadioAddReplace2();
	afx_msg void OnCheckPasteblock();
	afx_msg void OnRadioAddReplace3();
	afx_msg void OnDoubleclickedRadioAddReplace3();
	afx_msg void OnRadioAddReplace4();
	afx_msg void OnDoubleclickedRadioAddReplace4();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void UpdateView();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_BLOCKPASTE_H__B188FD80_9D1B_4229_9AE6_A46216126DB8__INCLUDED_
