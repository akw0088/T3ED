// FrdFileDlg.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "T3ED.h"
#include "FrdFileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CFrdFileDlg 


CFrdFileDlg::CFrdFileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFrdFileDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFrdFileDlg)
	m_str_File_Format = _T("");
	m_int_nBlocks = 0;
	m_short_nVRoad = 0;
	//}}AFX_DATA_INIT
}


void CFrdFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFrdFileDlg)
	DDX_Text(pDX, IDC_EDIT_FILE_FORMAT, m_str_File_Format);
	DDX_Text(pDX, IDC_EDIT_NBLOCKS, m_int_nBlocks);
	DDX_Text(pDX, IDC_EDIT_NVROAD, m_short_nVRoad);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFrdFileDlg, CDialog)
	//{{AFX_MSG_MAP(CFrdFileDlg)
		// HINWEIS: Der Klassen-Assistent fügt hier Zuordnungsmakros für Nachrichten ein
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CFrdFileDlg 
