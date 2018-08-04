// LodProgress.cpp : implementation file
//

#include "stdafx.h"
#include "T3ED.h"
#include "LodProgress.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// LodProgress dialog


LodProgress::LodProgress(CWnd* pParent /*=NULL*/)
	: CDialog(LodProgress::IDD, pParent)
{
	//{{AFX_DATA_INIT(LodProgress)
	m_progress = _T("");
	//}}AFX_DATA_INIT
}


void LodProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(LodProgress)
	DDX_Control(pDX, IDOK, m_okbutton);
	DDX_Text(pDX, IDC_NUMBLOCK, m_progress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(LodProgress, CDialog)
	//{{AFX_MSG_MAP(LodProgress)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// LodProgress message handlers
