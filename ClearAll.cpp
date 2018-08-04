// ClearAll.cpp : implementation file
//

#include "stdafx.h"
#include "T3ED.h"
#include "T3EDDoc.h"
#include "ClearAll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClearAll dialog


CClearAll::CClearAll(CWnd* pParent /*=NULL*/)
	: CDialog(CClearAll::IDD, pParent)
{
	//{{AFX_DATA_INIT(CClearAll)
	m_trackXY = FALSE;
	m_trackZ = FALSE;
	m_sceneryZ = FALSE;
	m_xobj = FALSE;
	m_polyobj = FALSE;
	m_roadTextures = FALSE;
	m_sceneryTextures = FALSE;
	m_roadTexture = 0;
	m_sceneryTexture = 0;
	m_fences = FALSE;
	m_globalobj = FALSE;
	m_trackBLK = FALSE;
	m_TrknBlocks = 0;
	m_trackShade = FALSE;
	m_block_neighbours = FALSE;
	//}}AFX_DATA_INIT
	m_roadFlags=0;
	m_sceneryFlags=0;
}


void CClearAll::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CClearAll)
	DDX_Check(pDX, IDC_CHECK1, m_trackXY);
	DDX_Check(pDX, IDC_CHECK2, m_trackZ);
	DDX_Check(pDX, IDC_CHECK3, m_sceneryZ);
	DDX_Check(pDX, IDC_CHECK5, m_xobj);
	DDX_Check(pDX, IDC_CHECK4, m_polyobj);
	DDX_Check(pDX, IDC_CHECK6, m_roadTextures);
	DDX_Check(pDX, IDC_CHECK7, m_sceneryTextures);
	DDX_Text(pDX, IDC_EDIT6, m_roadTexture);
	DDX_Text(pDX, IDC_EDIT7, m_sceneryTexture);
	DDX_Check(pDX, IDC_CHECK8, m_fences);
	DDX_Check(pDX, IDC_CHECK9, m_globalobj);
	DDX_Check(pDX, IDC_CHECK10, m_trackBLK);
	DDX_Text(pDX, IDC_EDIT8, m_TrknBlocks);
	DDX_Check(pDX, IDC_CHECK11, m_trackShade);
	DDX_Check(pDX, IDC_CHECK13, m_block_neighbours);
	//}}AFX_DATA_MAP
	DDV_MinMaxInt(pDX, m_roadTexture, 0, qfs->nColEntries-1);
	DDV_MinMaxInt(pDX, m_sceneryTexture, 0, qfs->nColEntries-1);
}


BEGIN_MESSAGE_MAP(CClearAll, CDialog)
	//{{AFX_MSG_MAP(CClearAll)
	ON_BN_CLICKED(IDC_BROWSE6, OnBrowse6)
	ON_BN_CLICKED(IDC_BROWSE7, OnBrowse7)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClearAll message handlers

void CClearAll::OnBrowse6() 
{
	BOOL isok;
	int res;

	res=GetDlgItemInt(IDC_EDIT6,&isok,TRUE);
	if (!isok||(res<0)||(res>=qfs->nColEntries)) res=0;
	qfs->m_nBmp=res;
	qfs->m_flags=m_roadFlags;
	if (qfs->DoModal()!=IDOK) return;
	m_roadFlags=qfs->m_flags;
	SetDlgItemInt(IDC_EDIT6,qfs->m_nBmp,TRUE);
}

void CClearAll::OnBrowse7() 
{
	BOOL isok;
	int res;

	res=GetDlgItemInt(IDC_EDIT7,&isok,TRUE);
	if (!isok||(res<0)||(res>=qfs->nColEntries)) res=0;
	qfs->m_nBmp=res;
	qfs->m_flags=m_sceneryFlags;
	if (qfs->DoModal()!=IDOK) return;
	m_sceneryFlags=qfs->m_flags;
	SetDlgItemInt(IDC_EDIT7,qfs->m_nBmp,TRUE);
}
