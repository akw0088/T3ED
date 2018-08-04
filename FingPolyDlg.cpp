// FingPolyDlg.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "T3ED.h"
#include "FingPolyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CFingPolyDlg 


CFingPolyDlg::CFingPolyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFingPolyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFingPolyDlg)
		// HINWEIS: Der Klassen-Assistent fügt hier Elementinitialisierung ein
	//}}AFX_DATA_INIT
}


void CFingPolyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFingPolyDlg)
		// HINWEIS: Der Klassen-Assistent fügt hier DDX- und DDV-Aufrufe ein
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFingPolyDlg, CDialog)
	//{{AFX_MSG_MAP(CFingPolyDlg)
	ON_BN_CLICKED(IDC_BLOCKSEL_RADIO1, OnBlockselRadio1)
	ON_BN_CLICKED(IDC_BLOCKSEL_RADIO2, OnBlockselRadio2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CFingPolyDlg 

void CFingPolyDlg::OnBlockselRadio1() 
{
	// TODO: Code für die Behandlungsroutine der Steuerelement-Benachrichtigung hier einfügen
	
}

void CFingPolyDlg::OnBlockselRadio2() 
{
	// TODO: Code für die Behandlungsroutine der Steuerelement-Benachrichtigung hier einfügen
	
}
