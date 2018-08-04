// ExpandDlg.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "T3ED.h"
#include "ExpandDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CExpandDlg 


CExpandDlg::CExpandDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExpandDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExpandDlg)
	m_floatExpMulti_x = 0.0f;
	m_floatExpMulti_y = 0.0f;
	m_floatExpMulti_z = 0.0f;
	m_chk_connected = FALSE;
	m_radio_alignment = -1;
	//}}AFX_DATA_INIT
}


void CExpandDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExpandDlg)
	DDX_Text(pDX, IDC_EDITX, m_floatExpMulti_x);
	DDV_MinMaxFloat(pDX, m_floatExpMulti_x, 1.e-003f, 1000.f);
	DDX_Text(pDX, IDC_EDITY, m_floatExpMulti_y);
	DDV_MinMaxFloat(pDX, m_floatExpMulti_y, 1.e-003f, 1000.f);
	DDX_Text(pDX, IDC_EDITZ, m_floatExpMulti_z);
	DDV_MinMaxFloat(pDX, m_floatExpMulti_z, 1.e-003f, 1000.f);
	DDX_Check(pDX, IDC_CHECK_CONNECT, m_chk_connected);
	DDX_Radio(pDX, IDC_RADIOTOP, m_radio_alignment);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExpandDlg, CDialog)
	//{{AFX_MSG_MAP(CExpandDlg)
	ON_BN_CLICKED(IDC_CHECK_CONNECT, OnCheckConnect)
	ON_WM_CANCELMODE()
	ON_EN_KILLFOCUS(IDC_EDITX, OnKillfocusEditx)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CExpandDlg 

void CExpandDlg::OnCheckConnect() 
{
	//CWnd *edy=GetDlgItem(IDC_EDITY);
	//CWnd *edz=GetDlgItem(IDC_EDITZ);
	if (m_chk_connected)
	{
		m_chk_connected=FALSE;
		//edy->EnableWindow(TRUE);
		//edz->EnableWindow(TRUE);
	}
	else
	{
		m_chk_connected=TRUE;
		m_floatExpMulti_y=m_floatExpMulti_x;
		m_floatExpMulti_z=m_floatExpMulti_x;
		//edy->EnableWindow(FALSE);
		//edz->EnableWindow(FALSE);
		//UpdateData(FALSE);
	}
	UpdateCheckConnectView();
}

void CExpandDlg::UpdateCheckConnectView()
{
	CWnd *edy=GetDlgItem(IDC_EDITY);
	CWnd *edz=GetDlgItem(IDC_EDITZ);
	if (m_chk_connected)
	{
		edy->EnableWindow(FALSE);
		edz->EnableWindow(FALSE);
		UpdateData(FALSE);
	}
	else
	{
		edy->EnableWindow(TRUE);
		edz->EnableWindow(TRUE);
	}
}


BOOL CExpandDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Zusätzliche Initialisierung hier einfügen


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}

void CExpandDlg::OnCancelMode() 
{
	CDialog::OnCancelMode();
	
	// TODO: Code für die Behandlungsroutine für Nachrichten hier einfügen
	
}

void CExpandDlg::OnKillfocusEditx() 
{
	// TODO: Code für die Behandlungsroutine der Steuerelement-Benachrichtigung hier einfügen
	if (m_chk_connected)
	{
	UpdateData(TRUE);
	m_floatExpMulti_y=m_floatExpMulti_x;
	m_floatExpMulti_z=m_floatExpMulti_x;
	UpdateData(FALSE);
	}
	else
	{
		UpdateData(TRUE);
	}

}


void CExpandDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Code für die Behandlungsroutine für Nachrichten hier einfügen
	UpdateCheckConnectView();
}
