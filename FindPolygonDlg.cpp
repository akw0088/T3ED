// FindPolygonDlg.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "T3ED.h"
#include "QFSView.h"
#include "PolyFlag.h"
#include "FindPolygonDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CFindPolygonDlg 


CFindPolygonDlg::CFindPolygonDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFindPolygonDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFindPolygonDlg)
	m_trackpolys = FALSE;
	m_extraobj = FALSE;
	m_polygonobj = FALSE;
	m_texture = 0;
	m_endbl = 0;
	m_startbl = 0;
	m_block_choice = -1;
	m_chk_texture = FALSE;
	m_chk_hstexture = FALSE;
	m_chk_animated = FALSE;
	m_chk_pvrflags = FALSE;
	//}}AFX_DATA_INIT
}


void CFindPolygonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFindPolygonDlg)
	DDX_Check(pDX, IDC_CHECK_TRACK, m_trackpolys);
	DDX_Check(pDX, IDC_CHECK_EXTRAOBJ, m_extraobj);
	DDX_Check(pDX, IDC_CHECK_POLYOBJ, m_polygonobj);
	DDX_Text(pDX, IDC_EDIT_TEXTURE, m_texture);
	DDX_Text(pDX, IDC_EDIT_ENDBL, m_endbl);
	DDV_MinMaxInt(pDX, m_endbl, 0, 300);
	DDX_Text(pDX, IDC_EDIT_STARTBL, m_startbl);
	DDV_MinMaxInt(pDX, m_startbl, 0, 300);
	DDX_Radio(pDX, IDC_BLOCK_RADIO1, m_block_choice);
	DDX_Check(pDX, IDC_CHECK_TEXTURE, m_chk_texture);
	DDX_Check(pDX, IDC_CHECK_HSTEXTURE, m_chk_hstexture);
	DDX_Check(pDX, IDC_CHECK_ANIMATED, m_chk_animated);
	DDX_Check(pDX, IDC_CHECK_PVRFLAGS, m_chk_pvrflags);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFindPolygonDlg, CDialog)
	//{{AFX_MSG_MAP(CFindPolygonDlg)
	ON_BN_CLICKED(IDC_BLOCK_RADIO1, OnBlockRadio1)
	ON_BN_CLICKED(IDC_BLOCK_RADIO2, OnBlockRadio2)
	ON_BN_DOUBLECLICKED(IDC_BLOCK_RADIO1, OnDoubleclickedBlockRadio1)
	ON_BN_DOUBLECLICKED(IDC_BLOCK_RADIO2, OnDoubleclickedBlockRadio2)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_TEXTURE, OnButtonSelectTexture)
	ON_EN_SETFOCUS(IDC_EDIT_ENDBL, OnSetfocusEditEndbl)
	ON_EN_SETFOCUS(IDC_EDIT_STARTBL, OnSetfocusEditStartbl)
	ON_EN_KILLFOCUS(IDC_EDIT_STARTBL, OnKillfocusEditStartbl)
	ON_EN_KILLFOCUS(IDC_EDIT_ENDBL, OnKillfocusEditEndbl)
	ON_BN_CLICKED(IDC_CHECK_HSTEXTURE, OnCheckHstexture)
	ON_BN_CLICKED(IDC_CHECK_ANIMATED, OnCheckAnimated)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_PVRFLAGS, OnButtonSelectPvrflags)
	ON_BN_CLICKED(IDC_CHECK_PVRFLAGS, OnCheckPvrflags)
	ON_BN_CLICKED(IDC_CHECK_POLYOBJ, OnCheckPolyobj)
	ON_BN_CLICKED(IDC_CHECK_EXTRAOBJ, OnCheckExtraobj)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CFindPolygonDlg 

void CFindPolygonDlg::OnBlockRadio1() 
{
	// TODO: Code für die Behandlungsroutine der Steuerelement-Benachrichtigung hier einfügen
	m_block_choice=0;
}

void CFindPolygonDlg::OnBlockRadio2() 
{
	// TODO: Code für die Behandlungsroutine der Steuerelement-Benachrichtigung hier einfügen
	m_block_choice=1;
}

void CFindPolygonDlg::OnDoubleclickedBlockRadio1() 
{
	OnBlockRadio1();
}

void CFindPolygonDlg::OnDoubleclickedBlockRadio2() 
{
	OnBlockRadio2();
}

void CFindPolygonDlg::OnButtonSelectTexture() 
{
	CString sStr = "Test";

	qfs->m_nBmp=m_texture;
	qfs->m_flags=hs_flags;
	qfs->texanimdata=texanimdata;
	qfs->bHSMode=bHSMode;
	qfs->bHasHitFlags=FALSE;
	sStr.Format("Select texture to search for");
	qfs->m_strLabel=sStr;
	if (qfs->DoModal()!=IDOK) return;

	m_texture=qfs->m_nBmp;
	hs_flags=qfs->m_flags;
	texanimdata=qfs->texanimdata;
	m_chk_texture=TRUE;
	if (qfs->bHasHitFlags) m_chk_hstexture=TRUE;
	if ((qfs->bHasHitFlags)&&(qfs->texanimdata>0)) m_chk_animated=TRUE;
	UpdateData(FALSE);
}

void CFindPolygonDlg::OnSetfocusEditEndbl() 
{
	m_block_choice=1;
	UpdateData(FALSE);
}

void CFindPolygonDlg::OnSetfocusEditStartbl() 
{
	m_block_choice=1;
	UpdateData(FALSE);
}

void CFindPolygonDlg::OnKillfocusEditStartbl() 
{
	UpdateData(TRUE);	
}

void CFindPolygonDlg::OnKillfocusEditEndbl() 
{
	UpdateData(TRUE);	
}

void CFindPolygonDlg::OnCheckHstexture() 
{
	if (bHSMode) //Only Possible for HS tracks
		UpdateData(TRUE);
	else
		UpdateData(FALSE);
}

void CFindPolygonDlg::OnCheckAnimated() 
{
	if (bHSMode) //Only Possible for HS tracks
		UpdateData(TRUE);
	else
		UpdateData(FALSE);	
}


void CFindPolygonDlg::OnButtonSelectPvrflags() 
{
	CPolyFlags polydlg;

	UpdateData(TRUE);
	polydlg.m_type=pvrflags;
	polydlg.bHSMode=bHSMode;
	polydlg.bTypeOnly=TRUE;
	
	if (polydlg.DoModal()!=IDOK) return;

	pvrflags=polydlg.m_type;
	m_chk_pvrflags=TRUE;
	m_extraobj=0;
	m_polygonobj=0;
	UpdateData(FALSE);
}

void CFindPolygonDlg::OnCheckPvrflags() 
{
	UpdateData(TRUE);
	if (m_chk_pvrflags)
	{
		m_extraobj=0;
		m_polygonobj=0;
		UpdateData(FALSE);
	}
}

void CFindPolygonDlg::OnCheckPolyobj() 
{
	OnCheckPvrflags();
}

void CFindPolygonDlg::OnCheckExtraobj() 
{
	OnCheckPvrflags();	
}
