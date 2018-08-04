// BlockPaste.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "T3ED.h"
#include "BlockPaste.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CBlockPaste 


CBlockPaste::CBlockPaste(CWnd* pParent /*=NULL*/)
	: CDialog(CBlockPaste::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBlockPaste)
	m_add_replace_block = -1;
	m_bool_move = FALSE;
	m_bool_rotate = FALSE;
	m_bool_move_z = FALSE;
	m_bool_expand_lenght = FALSE;
	m_bool_expand_width = FALSE;
	m_bool_paste_heights = FALSE;
	m_bool_paste_spdfa = FALSE;
	m_bool_paste_vroad = FALSE;
	m_bool_paste_block = FALSE;
	m_bool_paste_spdra = FALSE;
	//}}AFX_DATA_INIT
}


void CBlockPaste::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBlockPaste)
	DDX_Radio(pDX, IDC_RADIO_ADD_REPLACE1, m_add_replace_block);
	DDX_Check(pDX, IDC_CHECK_MOVE, m_bool_move);
	DDX_Check(pDX, IDC_CHECK_ROTATE, m_bool_rotate);
	DDX_Check(pDX, IDC_CHECK_MOVEZ, m_bool_move_z);
	DDX_Check(pDX, IDC_CHECK_EXPAND_LENGHT, m_bool_expand_lenght);
	DDX_Check(pDX, IDC_CHECK_EXPAND_WIDTH, m_bool_expand_width);
	DDX_Check(pDX, IDC_CHECK_PASTEHEIGHTS, m_bool_paste_heights);
	DDX_Check(pDX, IDC_CHECK_PASTESPDFA, m_bool_paste_spdfa);
	DDX_Check(pDX, IDC_CHECK_PASTEVROAD, m_bool_paste_vroad);
	DDX_Check(pDX, IDC_CHECK_PASTEBLOCK, m_bool_paste_block);
	DDX_Check(pDX, IDC_CHECK_PASTESPDRA, m_bool_paste_spdra);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBlockPaste, CDialog)
	//{{AFX_MSG_MAP(CBlockPaste)
	ON_BN_CLICKED(IDC_RADIO_ADD_REPLACE1, OnRadioAddReplace1)
	ON_BN_CLICKED(IDC_RADIO_ADD_REPLACE2, OnRadioAddReplace2)
	ON_BN_DOUBLECLICKED(IDC_RADIO_ADD_REPLACE1, OnDoubleclickedRadioAddReplace1)
	ON_BN_DOUBLECLICKED(IDC_RADIO_ADD_REPLACE2, OnDoubleclickedRadioAddReplace2)
	ON_BN_CLICKED(IDC_CHECK_PASTEBLOCK, OnCheckPasteblock)
	ON_BN_CLICKED(IDC_RADIO_ADD_REPLACE3, OnRadioAddReplace3)
	ON_BN_DOUBLECLICKED(IDC_RADIO_ADD_REPLACE3, OnDoubleclickedRadioAddReplace3)
	ON_BN_CLICKED(IDC_RADIO_ADD_REPLACE4, OnRadioAddReplace4)
	ON_BN_DOUBLECLICKED(IDC_RADIO_ADD_REPLACE4, OnDoubleclickedRadioAddReplace4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CBlockPaste 

void CBlockPaste::OnRadioAddReplace1() 
{
	m_add_replace_block=0;
	UpdateView();
}

void CBlockPaste::OnRadioAddReplace2() 
{
	m_add_replace_block=1;
	UpdateView();
}

void CBlockPaste::OnRadioAddReplace3() 
{
	m_add_replace_block=2;
	UpdateView();	
}

void CBlockPaste::OnRadioAddReplace4() 
{
	m_add_replace_block=3;
	UpdateView();	
}

void CBlockPaste::UpdateView()
{
	CWnd *CheckPasteBlock=GetDlgItem(IDC_CHECK_PASTEBLOCK);
	CWnd *CheckMove=GetDlgItem(IDC_CHECK_MOVE);
	CWnd *CheckMoveZ=GetDlgItem(IDC_CHECK_MOVEZ);
	CWnd *CheckRotate=GetDlgItem(IDC_CHECK_ROTATE);
	CWnd *CheckLenght=GetDlgItem(IDC_CHECK_EXPAND_LENGHT);
	CWnd *CheckWidth=GetDlgItem(IDC_CHECK_EXPAND_WIDTH);
	CWnd *CheckPasteHeights=GetDlgItem(IDC_CHECK_PASTEHEIGHTS);
	CWnd *CheckPasteSpdfa=GetDlgItem(IDC_CHECK_PASTESPDFA);
	CWnd *CheckPasteSpdra=GetDlgItem(IDC_CHECK_PASTESPDRA);
	CWnd *CheckPasteVRoad=GetDlgItem(IDC_CHECK_PASTEVROAD);

	if (m_add_replace_block==2)
	{
		CheckPasteBlock->EnableWindow(TRUE);
		CheckPasteHeights->EnableWindow(TRUE);
		CheckPasteSpdfa->EnableWindow(TRUE);
		CheckPasteSpdra->EnableWindow(TRUE);
	}
	else
	{
		CheckPasteBlock->EnableWindow(FALSE);
		CheckPasteHeights->EnableWindow(FALSE);
		CheckPasteSpdfa->EnableWindow(FALSE);
		CheckPasteSpdra->EnableWindow(FALSE);
	}

	if ((m_add_replace_block==2)&&(m_bool_paste_block))
	{
		CheckPasteVRoad->EnableWindow(TRUE);
		CheckMove->EnableWindow(TRUE);
		CheckMoveZ->EnableWindow(TRUE);
		CheckRotate->EnableWindow(TRUE);
		CheckLenght->EnableWindow(TRUE);
		CheckWidth->EnableWindow(TRUE);
	}
	else
	{
		CheckPasteVRoad->EnableWindow(FALSE);
		CheckMove->EnableWindow(FALSE);
		CheckMoveZ->EnableWindow(FALSE);
		CheckRotate->EnableWindow(FALSE);
		CheckLenght->EnableWindow(FALSE);
		CheckWidth->EnableWindow(FALSE);
	}
}

BOOL CBlockPaste::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Zusätzliche Initialisierung hier einfügen
	UpdateView();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}

void CBlockPaste::OnDoubleclickedRadioAddReplace1() 
{
	m_add_replace_block=0;
	UpdateView();
}

void CBlockPaste::OnDoubleclickedRadioAddReplace2() 
{
	m_add_replace_block=1;
	UpdateView();
}

void CBlockPaste::OnDoubleclickedRadioAddReplace3() 
{
	m_add_replace_block=2;
	UpdateView();
}

void CBlockPaste::OnDoubleclickedRadioAddReplace4() 
{
	m_add_replace_block=3;
	UpdateView();	
}

void CBlockPaste::OnCheckPasteblock() 
{
	// TODO: Code für die Behandlungsroutine der Steuerelement-Benachrichtigung hier einfügen
	UpdateData(TRUE);
	UpdateView();
}





