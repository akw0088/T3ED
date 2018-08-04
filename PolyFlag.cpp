// PolyFlag.cpp : implementation file
//

#include "stdafx.h"
#include "T3ED.h"
#include "PolyFlag.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPolyFlags dialog


CPolyFlags::CPolyFlags(CWnd* pParent /*=NULL*/)
	: CDialog(CPolyFlags::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPolyFlags)
	m_type = -1;
	m_walldetect = FALSE;
	m_xobjdetect = FALSE;
	m_hsForwNeighbor = FALSE;
	m_hsBackNeighbor = FALSE;
	m_hsLeftNeighbor = FALSE;
	m_hsRightNeighbor = FALSE;
	//}}AFX_DATA_INIT
}


void CPolyFlags::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPolyFlags)
	DDX_CBIndex(pDX, IDC_COMBO1, m_type);
	DDX_Check(pDX, IDC_CHECK1, m_walldetect);
	DDX_Check(pDX, IDC_CHECK2, m_xobjdetect);
	DDX_Check(pDX, IDC_CHECK3, m_hsForwNeighbor);
	DDX_Check(pDX, IDC_CHECK4, m_hsBackNeighbor);
	DDX_Check(pDX, IDC_CHECK5, m_hsLeftNeighbor);
	DDX_Check(pDX, IDC_CHECK6, m_hsRightNeighbor);
	//}}AFX_DATA_MAP
	if (!bHSMode) {
		GetDlgItem(IDC_CHECK3)->ModifyStyle(WS_VISIBLE,WS_DISABLED);
		GetDlgItem(IDC_CHECK4)->ModifyStyle(WS_VISIBLE,WS_DISABLED);
	}
}


BEGIN_MESSAGE_MAP(CPolyFlags, CDialog)
	//{{AFX_MSG_MAP(CPolyFlags)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPolyFlags message handlers

BOOL CPolyFlags::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (bTypeOnly)
	{
		GetDlgItem(IDC_CHECK1)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK2)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK3)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK4)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK5)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK6)->EnableWindow(FALSE);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}
