// SetVisiDlg.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "T3ED.h"
#include "SetVisiDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CSetVisiDlg 


CSetVisiDlg::CSetVisiDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetVisiDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetVisiDlg)
	m_int_end = 0;
	m_int_num_blocks = 0;
	m_edit_start = 0;
	m_int_tr_bl_fromto = -1;
	//}}AFX_DATA_INIT
}


void CSetVisiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetVisiDlg)
	DDX_Text(pDX, IDC_EDIT_END, m_int_end);
	DDV_MinMaxInt(pDX, m_int_end, 0, 300);
	DDX_Text(pDX, IDC_EDIT_NUMBLOCKS, m_int_num_blocks);
	DDV_MinMaxInt(pDX, m_int_num_blocks, 1, 150);
	DDX_Text(pDX, IDC_EDIT_START, m_edit_start);
	DDV_MinMaxInt(pDX, m_edit_start, 0, 300);
	DDX_Radio(pDX, IDC_TRBLFROMTO1, m_int_tr_bl_fromto);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetVisiDlg, CDialog)
	//{{AFX_MSG_MAP(CSetVisiDlg)
	ON_EN_SETFOCUS(IDC_EDIT_START, OnSetfocusEditStart)
	ON_EN_SETFOCUS(IDC_EDIT_END, OnSetfocusEditEnd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CSetVisiDlg 

BOOL CSetVisiDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Zusätzliche Initialisierung hier einfügen
	//CWnd *edstart=GetDlgItem(IDC_EDIT_START);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}

void CSetVisiDlg::OnSetfocusEditStart() 
{
	((CButton*)GetDlgItem(IDC_TRBLFROMTO1))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_TRBLFROMTO2))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_TRBLFROMTO3))->SetCheck(0);
}

void CSetVisiDlg::OnSetfocusEditEnd() 
{
	((CButton*)GetDlgItem(IDC_TRBLFROMTO1))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_TRBLFROMTO2))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_TRBLFROMTO3))->SetCheck(0);
}
