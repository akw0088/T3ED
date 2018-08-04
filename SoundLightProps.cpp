// SoundLightProps.cpp : implementation file
//

#include "stdafx.h"
#include "T3ED.h"
#include "SoundLightProps.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSoundLightProps dialog


CSoundLightProps::CSoundLightProps(CWnd* pParent /*=NULL*/)
	: CDialog(CSoundLightProps::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSoundLightProps)
	m_tyyppi = 0;
	m_tyyppihex = _T("");
	m_strText1 = _T("");
	m_intBlock = 0;
	//}}AFX_DATA_INIT
}


void CSoundLightProps::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSoundLightProps)
	DDX_Control(pDX, IDC_EDIT1, m_tyyppic);
	DDX_Control(pDX, IDC_EDIT2, m_tyyppihexc);
	DDX_Text(pDX, IDC_EDIT1, m_tyyppi);
	DDX_Text(pDX, IDC_EDIT2, m_tyyppihex);
	DDV_MaxChars(pDX, m_tyyppihex, 8);
	DDX_Text(pDX, IDC_TEXT1, m_strText1);
	DDV_MaxChars(pDX, m_strText1, 200);
	DDX_Text(pDX, IDC_EDIT_BLOCK, m_intBlock);
	DDV_MinMaxInt(pDX, m_intBlock, 0, 65535);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSoundLightProps, CDialog)
	//{{AFX_MSG_MAP(CSoundLightProps)
	ON_EN_CHANGE(IDC_EDIT1, OnChangeEdit1)
	ON_EN_KILLFOCUS(IDC_EDIT_BLOCK, OnKillfocusEditBlock)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSoundLightProps message handlers

void CSoundLightProps::OnChangeEdit1() 
{
	CString keke;
	//m_tyyppic.SetModify(TRUE);
	keke.Format("%X", m_tyyppi);
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	m_tyyppihex=keke;
	m_tyyppihexc.SetModify(TRUE);
}

BOOL CSoundLightProps::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CWnd *edblock=GetDlgItem(IDC_EDIT_BLOCK);
	if (!bSLAutoObjMem)
		edblock->EnableWindow(TRUE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}

void CSoundLightProps::OnKillfocusEditBlock() 
{
	UpdateData(TRUE);
	if (m_intBlock>maxblock)
		m_intBlock=maxblock;
	if (m_intBlock<0)
		m_intBlock=0;
	UpdateData(FALSE);	
}

void CSoundLightProps::OnOK() 
{
	// TODO: Zusätzliche Prüfung hier einfügen
	if (m_intBlock>maxblock)
		m_intBlock=maxblock;
	if (m_intBlock<0)
		m_intBlock=0;
	CDialog::OnOK();
}
