// LSPROPS.cpp : implementation file
//

#include "stdafx.h"
#include "T3ED.h"
#include "LSPROPS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// LightSoundProps dialog


LightSoundProps::LightSoundProps(CWnd* pParent /*=NULL*/)
	: CDialog(LightSoundProps::IDD, pParent)
{
	//{{AFX_DATA_INIT(LightSoundProps)
	m_type = 0;
	//}}AFX_DATA_INIT
}


void LightSoundProps::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(LightSoundProps)
	DDX_Text(pDX, IDC_EDIT1, m_type);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(LightSoundProps, CDialog)
	//{{AFX_MSG_MAP(LightSoundProps)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// LightSoundProps message handlers
