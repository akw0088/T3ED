// LodGenProgress.cpp : implementation file
//

#include "stdafx.h"
#include "T3ED.h"
#include "LodGenProgress.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLodGenProgress dialog


CLodGenProgress::CLodGenProgress(CWnd* pParent /*=NULL*/)
	: CDialog(CLodGenProgress::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLodGenProgress)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CLodGenProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLodGenProgress)
	DDX_Control(pDX, IDC_PROGRESS2, m_lodprogress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLodGenProgress, CDialog)
	//{{AFX_MSG_MAP(CLodGenProgress)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLodGenProgress message handlers
