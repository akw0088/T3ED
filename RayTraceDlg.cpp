// RayTraceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3ED.h"
#include "RayTraceDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRayTraceDlg dialog


CRayTraceDlg::CRayTraceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRayTraceDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRayTraceDlg)
	m_theta = 0.0f;
	m_rho = 0.0f;
	m_iEndBlock = 0;
	m_iStartBlock = 0;
	m_bCheckTrack = FALSE;
	m_bCheckObjects = FALSE;
	m_text_endbl = _T("");
	//}}AFX_DATA_INIT
}


void CRayTraceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRayTraceDlg)
	DDX_Text(pDX, IDC_EDIT1, m_theta);
	DDX_Text(pDX, IDC_EDIT2, m_rho);
	DDX_Text(pDX, IDC_EDIT4, m_iEndBlock);
	DDV_MinMaxInt(pDX, m_iEndBlock, 0, 255);
	DDX_Text(pDX, IDC_EDIT3, m_iStartBlock);
	DDV_MinMaxInt(pDX, m_iStartBlock, 0, 255);
	DDX_Check(pDX, IDC_CHECKTrack, m_bCheckTrack);
	DDX_Check(pDX, IDC_CHECKObjects, m_bCheckObjects);
	DDX_Text(pDX, IDC_TEXT_ENDBL, m_text_endbl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRayTraceDlg, CDialog)
	//{{AFX_MSG_MAP(CRayTraceDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRayTraceDlg message handlers
