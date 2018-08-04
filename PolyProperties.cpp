// PolyProperties.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "T3ED.h"
#include "PolyProperties.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CPolyProperties 


CPolyProperties::CPolyProperties(CWnd* pParent /*=NULL*/)
	: CDialog(CPolyProperties::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPolyProperties)
	m_intBlock = 0;
	m_intNumPoly = 0;
	m_int_ObjNum = 0;
	m_int_Chunk = 0;
	m_bool_convert = FALSE;
	//}}AFX_DATA_INIT
}


void CPolyProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPolyProperties)
	DDX_Text(pDX, IDC_EDIT_BLOCK, m_intBlock);
	DDV_MinMaxInt(pDX, m_intBlock, 0, 65535);
	DDX_Text(pDX, IDC_EDIT_NUMPOLY, m_intNumPoly);
	DDX_Text(pDX, IDC_EDIT_OBJNUM, m_int_ObjNum);
	DDX_Text(pDX, IDC_EDIT_CHUNK, m_int_Chunk);
	DDX_Check(pDX, IDC_CHECK_CONVERT, m_bool_convert);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPolyProperties, CDialog)
	//{{AFX_MSG_MAP(CPolyProperties)
	ON_EN_CHANGE(IDC_EDIT_CHUNK, OnChangeEditChunk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CPolyProperties 

BOOL CPolyProperties::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Zusätzliche Initialisierung hier einfügen
	CWnd *edblock=GetDlgItem(IDC_EDIT_BLOCK);
	CWnd *edchunk=GetDlgItem(IDC_EDIT_CHUNK);
	if (!m_bPolyPropsAutoObjMem)
	{
		edblock->EnableWindow(TRUE);
		edchunk->EnableWindow(TRUE);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}

void CPolyProperties::OnChangeEditChunk() 
{
	// TODO: Fügen Sie hier Ihren Code für die Benachrichtigungsbehandlungsroutine des Steuerelements hinzu
	UpdateData(TRUE);
	if (m_int_Chunk<0) 	
		m_int_Chunk=0;

	if (m_int_Chunk>3) 	
		m_int_Chunk=3;
	UpdateData(FALSE);
}
