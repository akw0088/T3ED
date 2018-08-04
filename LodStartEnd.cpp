// LodStartEnd.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "T3ED.h"
#include "LodStartEnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CLodStartEnd 


CLodStartEnd::CLodStartEnd(CWnd* pParent /*=NULL*/)
	: CDialog(CLodStartEnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLodStartEnd)
	m_iLodEnd = 0;
	m_iLodStart = 0;
	//}}AFX_DATA_INIT
}


void CLodStartEnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLodStartEnd)
	DDX_Text(pDX, IDC_EDITEND, m_iLodEnd);
	DDV_MinMaxInt(pDX, m_iLodEnd, 0, 255);
	DDX_Text(pDX, IDC_EDITSTART, m_iLodStart);
	DDV_MinMaxInt(pDX, m_iLodStart, 0, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLodStartEnd, CDialog)
	//{{AFX_MSG_MAP(CLodStartEnd)
		// HINWEIS: Der Klassen-Assistent fügt hier Zuordnungsmakros für Nachrichten ein
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CLodStartEnd 
