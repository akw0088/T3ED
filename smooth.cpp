// smooth.cpp : implementation file
//

#include "stdafx.h"
#include "T3ED.h"
#include "smooth.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSmoothingBox1 dialog


CSmoothingBox1::CSmoothingBox1(CWnd* pParent /*=NULL*/)
	: CDialog(CSmoothingBox1::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSmoothingBox1)
	m_extraSmooth = FALSE;
	//}}AFX_DATA_INIT
}

#define _countof(array) (sizeof(array)/sizeof(array[0]))

BOOL MyParseInt(LPCTSTR lpszText,int *result)
{
	TCHAR chFirst = lpszText[0];
	int l;

	while (*lpszText==' '||*lpszText=='\t') lpszText++;
	l = _tcstol(lpszText,(LPTSTR*)&lpszText,10);
	if ((l==0)&&(chFirst!='0'))
		return FALSE;   // could not convert
	while (*lpszText==' '||*lpszText=='\t') lpszText++;
	if (*lpszText!='\0') return FALSE;  // not terminated
	*result=l;
	return TRUE;
}

void DDX_CBInt(CDataExchange *pDX,int nIDC,int &value)
{
	HWND hWndCtrl=pDX->PrepareCtrl(nIDC);
	TCHAR szT[32];

	if (pDX->m_bSaveAndValidate) {
		::GetWindowText(hWndCtrl,szT,_countof(szT));
		if (!MyParseInt(szT,&value)) {
			AfxMessageBox(AFX_IDP_PARSE_INT);
			pDX->Fail();
		}
	} else {
		wsprintf(szT,"%d",value);
		if (::SendMessage(hWndCtrl,CB_SELECTSTRING,(WPARAM)-1,(LPARAM)(LPCTSTR)szT)==CB_ERR)
			::SetWindowText(hWndCtrl,szT);
	}
}

void CSmoothingBox1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSmoothingBox1)
	DDX_Check(pDX, IDC_CHECK1, m_extraSmooth);
	//}}AFX_DATA_MAP
	DDX_CBInt(pDX, IDC_EDIT1, m_sDist);
	DDV_MinMaxInt(pDX, m_sDist, 1, 500);
	DDX_CBInt(pDX, IDC_EDIT2, m_sWidth);
	DDV_MinMaxInt(pDX, m_sWidth, 1, 500);
	OnCheckExtraSmoothing();
}


BEGIN_MESSAGE_MAP(CSmoothingBox1, CDialog)
	//{{AFX_MSG_MAP(CSmoothingBox1)
	ON_BN_CLICKED(IDC_CHECK1, OnCheckExtraSmoothing)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSmoothingBox1 message handlers

void CSmoothingBox1::OnCheckExtraSmoothing() 
{
	if (IsDlgButtonChecked(IDC_CHECK1)) {
//		SendDlgItemMessage(IDC_EDIT1,WM_ENABLE,FALSE);
		GetDlgItem(IDC_EDIT1)->ModifyStyle(0,WS_DISABLED);
		GetDlgItem(IDC_SMOOTH1_STATIC1)->ShowWindow(FALSE);
		GetDlgItem(IDC_SMOOTH1_STATIC3)->ShowWindow(TRUE);
		GetDlgItem(IDC_SMOOTH1_STATIC2)->ShowWindow(FALSE);
		GetDlgItem(IDC_SMOOTH1_STATIC4)->ShowWindow(TRUE);
	} else {
//		SendDlgItemMessage(IDC_EDIT1,WM_ENABLE,TRUE);
		GetDlgItem(IDC_EDIT1)->ModifyStyle(WS_DISABLED,0);
		GetDlgItem(IDC_SMOOTH1_STATIC1)->ShowWindow(TRUE);
		GetDlgItem(IDC_SMOOTH1_STATIC3)->ShowWindow(FALSE);
		GetDlgItem(IDC_SMOOTH1_STATIC2)->ShowWindow(TRUE);
		GetDlgItem(IDC_SMOOTH1_STATIC4)->ShowWindow(FALSE);
	}
}
/////////////////////////////////////////////////////////////////////////////
// CSmoothingBox2 dialog


CSmoothingBox2::CSmoothingBox2(CWnd* pParent /*=NULL*/)
	: CDialog(CSmoothingBox2::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSmoothingBox2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSmoothingBox2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSmoothingBox2)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_CBInt(pDX, IDC_EDIT1, m_sDist);
	DDV_MinMaxInt(pDX, m_sDist, 0, 50);
	DDX_CBInt(pDX, IDC_EDIT2, m_sWidth);
	DDV_MinMaxInt(pDX, m_sWidth, 1, 500);
}


BEGIN_MESSAGE_MAP(CSmoothingBox2, CDialog)
	//{{AFX_MSG_MAP(CSmoothingBox2)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSmoothingBox2 message handlers
