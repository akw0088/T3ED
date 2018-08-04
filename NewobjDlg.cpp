// NewobjDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3ED.h"
#include "T3EDDoc.h"
#include "NewobjDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewobjDlg dialog


CNewobjDlg::CNewobjDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewobjDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewobjDlg)
	m_chunk = -1;
	m_xobj = -1;
	m_texture = 0;
	//}}AFX_DATA_INIT
	m_chunk=0;
	m_xobj=0;
	m_flags=0;
}


void CNewobjDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewobjDlg)
	DDX_Radio(pDX, IDC_CHUNK0, m_chunk);
	DDX_Radio(pDX, IDC_RADIO1, m_xobj);
	DDX_Text(pDX, IDC_EDIT1, m_texture);
	//}}AFX_DATA_MAP
	DDV_MinMaxInt(pDX, m_texture, 0, qfs->nColEntries-1);
}


BEGIN_MESSAGE_MAP(CNewobjDlg, CDialog)
	//{{AFX_MSG_MAP(CNewobjDlg)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewobjDlg message handlers

void CNewobjDlg::OnBrowse() 
{
	BOOL isok;
	int res;

	res=GetDlgItemInt(IDC_EDIT1,&isok,TRUE);
	if (!isok||(res<0)||(res>=qfs->nColEntries)) res=0;
	qfs->m_nBmp=res;
	qfs->m_flags=m_flags;
	if (qfs->DoModal()!=IDOK) return;
	SetDlgItemInt(IDC_EDIT1,qfs->m_nBmp,TRUE);
	m_flags=qfs->m_flags;
}

BOOL CNewobjDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Zusätzliche Initialisierung hier einfügen
	CComboBox *pComboBox = (CComboBox*) GetDlgItem(IDC_COMBO_XOBJTYPE);
	pComboBox->AddString("2 - Simple");
	pComboBox->SetItemData(0,2);
	pComboBox->AddString("3 - Animated");
	pComboBox->SetItemData(1,3);
	pComboBox->AddString("4 - With collision");
	pComboBox->SetItemData(2,4);
	pComboBox->SetCurSel(2);
	GetDlgItem(IDC_COMBO_XOBJTYPE)->EnableWindow(FALSE); 
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}

void CNewobjDlg::OnRadio1() 
{
	// TODO: Code für die Behandlungsroutine der Steuerelement-Benachrichtigung hier einfügen
	GetDlgItem(IDC_COMBO_XOBJTYPE)->EnableWindow(FALSE); 	
}

void CNewobjDlg::OnRadio2() 
{
	// TODO: Code für die Behandlungsroutine der Steuerelement-Benachrichtigung hier einfügen
	GetDlgItem(IDC_COMBO_XOBJTYPE)->EnableWindow(TRUE); 		
}

void CNewobjDlg::OnOK() 
{
	int selcombo;
	// TODO: Zusätzliche Prüfung hier einfügen
	CComboBox *pComboBox = (CComboBox*) GetDlgItem(IDC_COMBO_XOBJTYPE);
	selcombo=pComboBox->GetCurSel();
	xobj_crosstype=pComboBox->GetItemData(selcombo);
	
	CDialog::OnOK();
}
