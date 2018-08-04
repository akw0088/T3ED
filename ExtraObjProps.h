#if !defined(AFX_EXTRAOBJPROPS_H__615F866B_134A_4821_A9D0_5B966DF40619__INCLUDED_)
#define AFX_EXTRAOBJPROPS_H__615F866B_134A_4821_A9D0_5B966DF40619__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExtraObjProps.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CExtraObjProps 

class CExtraObjProps : public CDialog
{
// Konstruktion
public:
	BOOL m_bool_IsGlobal;
	void InitUnknown3Combobox();
	short m_nAnimLength;
	struct ANIMDATA *m_AnimData;
	short *m_short_unknown3;
	int m_int_ReCenter;
	BOOL m_bExtraObjPropsAutoObjMem;
	CExtraObjProps(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CExtraObjProps)
	enum { IDD = IDD_EXTRAOBJPROPS };
	int		m_int_Chunk;
	int		m_int_Block;
	int		m_int_NumPoly;
	int		m_int_ObjNum;
	long	m_long_Crosstype;
	float	m_float_x;
	float	m_float_y;
	float	m_float_z;
	short	m_short_unknown1;
	short	m_short_unknown2;
	short	m_short_collide_effect;
	long	m_long_unknown;
	long	m_long_AnimMemory;
	BOOL	m_bool_ReCenter;
	short	m_short_AnimDelay;
	short	m_short_AnimM1;
	short	m_short_AnimM2;
	short	m_short_AnimM3;
	short	m_short_AnimM4;
	CString	m_s_cb_UK3;
	float	m_float_AnimX;
	float	m_float_AnimY;
	float	m_float_AnimZ;
	BOOL	m_bool_Convert_Global;
	BOOL	m_bool_Convert_Poly;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CExtraObjProps)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CExtraObjProps)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditChunk();
	afx_msg void OnSelchangeComboCrosstype();
	afx_msg void OnCheckRecenter();
	afx_msg void OnSelchangeComboRecenter();
	afx_msg void OnSelchangeComboAniml();
	afx_msg void OnButtonAnimExport();
	afx_msg void OnButtonAnimImport();
	afx_msg void OnKillfocusComboXobjUk3();
	afx_msg void OnSelchangeComboXobjUk3();
	afx_msg void OnDropdownComboXobjUk3();
	afx_msg void OnEditchangeComboXobjUk3();
	afx_msg void OnCheckConvertPoly();
	afx_msg void OnCheckConvertGlobal();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	double Runden(double x);
	int LastCbAnimSel;
	BOOL cb_uk3_edit;
	void StoreComboXobjUk3into_UK3();
	int cb_uk3_sel;
	void InitAnimCombobox();
	void UpdateView();
	void CheckSel();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_EXTRAOBJPROPS_H__615F866B_134A_4821_A9D0_5B966DF40619__INCLUDED_
