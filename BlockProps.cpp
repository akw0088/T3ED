// BlockProps.cpp : implementation file
//

#include "stdafx.h"
#include "T3ED.h"
#include "BlockProps.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBlockProps dialog


CBlockProps::CBlockProps(CWnd* pParent /*=NULL*/)
	: CDialog(CBlockProps::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBlockProps)
	m_Neigh1 = 0;
	m_Neigh2 = 0;
	m_Neigh3 = 0;
	m_Neigh4 = 0;
	m_Neigh5 = 0;
	m_Neigh6 = 0;
	m_Neigh7 = 0;
	m_Neigh8 = 0;
	m_dbstart = 0;
	m_dbstop = 0;
	m_clist1value = _T("");
	m_bool_merge = FALSE;
	m_float_Dist = 0.0f;
	//}}AFX_DATA_INIT
}


void CBlockProps::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBlockProps)
	DDX_Text(pDX, IDC_NEIGHBOUR_EDIT1, m_Neigh1);
	DDV_MinMaxLong(pDX, m_Neigh1, -2, 1000);
	DDX_Text(pDX, IDC_NEIGHBOUR_EDIT2, m_Neigh2);
	DDV_MinMaxLong(pDX, m_Neigh2, -2, 1000);
	DDX_Text(pDX, IDC_NEIGHBOUR_EDIT3, m_Neigh3);
	DDV_MinMaxLong(pDX, m_Neigh3, -2, 1000);
	DDX_Text(pDX, IDC_NEIGHBOUR_EDIT4, m_Neigh4);
	DDV_MinMaxLong(pDX, m_Neigh4, -2, 1000);
	DDX_Text(pDX, IDC_NEIGHBOUR_EDIT5, m_Neigh5);
	DDV_MinMaxLong(pDX, m_Neigh5, -2, 1000);
	DDX_Text(pDX, IDC_NEIGHBOUR_EDIT6, m_Neigh6);
	DDV_MinMaxLong(pDX, m_Neigh6, -2, 1000);
	DDX_Text(pDX, IDC_NEIGHBOUR_EDIT7, m_Neigh7);
	DDV_MinMaxLong(pDX, m_Neigh7, -2, 1000);
	DDX_Text(pDX, IDC_NEIGHBOUR_EDIT8, m_Neigh8);
	DDV_MinMaxLong(pDX, m_Neigh8, -2, 1000);
	DDX_Check(pDX, IDC_CHECK_MERGE, m_bool_merge);
	DDX_Text(pDX, IDC_EDIT_DIST, m_float_Dist);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBlockProps, CDialog)
	//{{AFX_MSG_MAP(CBlockProps)
	ON_BN_CLICKED(IDC_CHECK_MERGE, OnCheckMerge)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBlockProps message handlers

void CBlockProps::OnCheckMerge() 
{
	// TODO: Code für die Behandlungsroutine der Steuerelement-Benachrichtigung hier einfügen
	m_bool_merge=!m_bool_merge;
	UpdateView();
}

BOOL CBlockProps::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Zusätzliche Initialisierung hier einfügen
	UpdateView();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}

void CBlockProps::UpdateView()
{
	CWnd *edit_dist=GetDlgItem(IDC_EDIT_DIST);
	if (m_bool_merge)
		edit_dist->EnableWindow(TRUE);
	else
		edit_dist->EnableWindow(FALSE);
}
