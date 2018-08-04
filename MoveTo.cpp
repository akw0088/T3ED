// MoveTo.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "T3ED.h"
#include "MoveTo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CMoveTo 


CMoveTo::CMoveTo(CWnd* pParent /*=NULL*/)
	: CDialog(CMoveTo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMoveTo)
	m_dest_x = 0.0f;
	m_dest_y = 0.0f;
	m_dest_z = 0.0f;
	m_chk_no_smooth = FALSE;
	m_int_startblock = 0;
	m_int_endblock = 0;
	//}}AFX_DATA_INIT
}


void CMoveTo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMoveTo)
	DDX_Text(pDX, IDC_EDITX, m_dest_x);
	DDX_Text(pDX, IDC_EDITY, m_dest_y);
	DDX_Text(pDX, IDC_EDITZ, m_dest_z);
	DDX_Check(pDX, IDC_CHECKNOSMOOTH, m_chk_no_smooth);
	DDX_Text(pDX, IDC_EDIT_STARTBLOCK, m_int_startblock);
	DDV_MinMaxInt(pDX, m_int_startblock, 0, 500);
	DDX_Text(pDX, IDC_EDIT_ENDBLOCK, m_int_endblock);
	DDV_MinMaxInt(pDX, m_int_endblock, 0, 500);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMoveTo, CDialog)
	//{{AFX_MSG_MAP(CMoveTo)
	ON_BN_CLICKED(IDC_CHECKNOSMOOTH, OnChecknosmooth)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CMoveTo 

BOOL CMoveTo::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CWnd *check_smooth=GetDlgItem(IDC_CHECKNOSMOOTH);
	check_smooth->EnableWindow(block_mode);
	if (block_mode) 
	{
		GetDlgItem(IDC_TXT_STARTBLOCK)->ShowWindow(SW_SHOW); 
		GetDlgItem(IDC_EDIT_STARTBLOCK)->ShowWindow(SW_SHOW); 
		GetDlgItem(IDC_TXT_ENDBLOCK)->ShowWindow(SW_SHOW); 
		GetDlgItem(IDC_EDIT_ENDBLOCK)->ShowWindow(SW_SHOW); 
	}
	//if (!no_smooth) UpdateData(FALSE);

	// TODO: Zusätzliche Initialisierung hier einfügen
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}

void CMoveTo::OnChecknosmooth() 
{
	// TODO: Code für die Behandlungsroutine der Steuerelement-Benachrichtigung hier einfügen
	//no_smooth=!no_smooth;
}
