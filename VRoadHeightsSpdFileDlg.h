#if !defined(AFX_VROADHEIGHTSSPDFILEDLG_H__FB95D050_471F_4810_A1DB_E604502DF410__INCLUDED_)
#define AFX_VROADHEIGHTSSPDFILEDLG_H__FB95D050_471F_4810_A1DB_E604502DF410__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VRoadHeightsSpdFileDlg.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CVRoadHeightsSpdFileDlg 

class CVRoadHeightsSpdFileDlg : public CDialog
{
// Konstruktion
public:
	struct HEIGHTSSIM *m_heightsdata;
	struct SPDFILE *spdFAdata, *spdRAdata;
	struct COLVROAD *m_colvrdata;
	//struct VROADDATA *m_vrdata;
	//struct COLVROAD *m_vrdata;
	long m_nPositions;
	CVRoadHeightsSpdFileDlg(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CVRoadHeightsSpdFileDlg)
	enum { IDD = IDD_VROAD_HEIGHTS_SPDFILES };
	CGridListCtrl	m_GridListCtrl;
	BOOL	m_Chk_Heights;
	BOOL	m_Chk_SpdFa;
	BOOL	m_Chk_SpdRa;
	BOOL	m_Chk_VRoad;
	//}}AFX_DATA
	CImageList *m_pImageList;
	CInPlaceEdit *m_pListEdit;


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CVRoadHeightsSpdFileDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CVRoadHeightsSpdFileDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBeginlabeleditList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeleditList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnCheckHeights();
	afx_msg void OnCheckSpdfa();
	afx_msg void OnCheckSpdra();
	afx_msg void OnCheckVroad();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	double Runden(double x);
	CFont font;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_VROADHEIGHTSSPDFILEDLG_H__FB95D050_471F_4810_A1DB_E604502DF410__INCLUDED_
