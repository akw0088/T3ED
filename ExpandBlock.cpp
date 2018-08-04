// ExpandBlock.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "T3ED.h"
#include "ExpandBlock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld ExpandBlock 


ExpandBlock::ExpandBlock(CWnd* pParent /*=NULL*/)
	: CDialog(ExpandBlock::IDD, pParent)
{
	//{{AFX_DATA_INIT(ExpandBlock)
	m_edit_height = 0.0f;
	m_edit_lenght = 0.0f;
	m_edit_width = 0.0f;
	//}}AFX_DATA_INIT
}


void ExpandBlock::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ExpandBlock)
	DDX_Text(pDX, IDC_EDITHEIGHT, m_edit_height);
	DDX_Text(pDX, IDC_EDITLENGTH, m_edit_lenght);
	DDX_Text(pDX, IDC_EDITWIDTH, m_edit_width);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ExpandBlock, CDialog)
	//{{AFX_MSG_MAP(ExpandBlock)
		// HINWEIS: Der Klassen-Assistent fügt hier Zuordnungsmakros für Nachrichten ein
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten ExpandBlock 
