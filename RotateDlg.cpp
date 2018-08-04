// RotateDlg.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "T3ED.h"
#include "RotateDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CRotateDlg 


CRotateDlg::CRotateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRotateDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRotateDlg)
	m_edit_angle = 0.0;
	m_str_rtext = _T("");
	//}}AFX_DATA_INIT
}


void CRotateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRotateDlg)
	DDX_Text(pDX, IDC_EDIT_ANGLE, m_edit_angle);
	DDV_MinMaxDouble(pDX, m_edit_angle, -360., 360.);
	DDX_Text(pDX, IDC_RTEXT, m_str_rtext);
	DDV_MaxChars(pDX, m_str_rtext, 300);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRotateDlg, CDialog)
	//{{AFX_MSG_MAP(CRotateDlg)
		// HINWEIS: Der Klassen-Assistent fügt hier Zuordnungsmakros für Nachrichten ein
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CRotateDlg 
