// TextureProps.cpp : implementation file
//

#include "stdafx.h"
#include "T3ED.h"
#include "TextureProps.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextureProps dialog


CTextureProps::CTextureProps(CWnd* pParent /*=NULL*/)
	: CDialog(CTextureProps::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTextureProps)
	m_rotation = -1;
	m_tiling = -1;
	m_flipv = FALSE;
	m_fliph = FALSE;
	m_twosided = FALSE;
	m_animduration = 0;
	m_animlenght = 0;
	m_animated = FALSE;
	//}}AFX_DATA_INIT
}


void CTextureProps::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTextureProps)
	DDX_Control(pDX, IDC_ANIMLENGTH, m_AnimLenCtrl);
	DDX_Control(pDX, IDC_ANIMDURATION, m_AnimDurCtrl);
	DDX_Radio(pDX, IDC_ROTNONE, m_rotation);
	DDX_Radio(pDX, IDC_TILINGNONE, m_tiling);
	DDX_Check(pDX, IDC_FLIPV, m_flipv);
	DDX_Check(pDX, IDC_FLIPH, m_fliph);
	DDX_Check(pDX, IDC_TWOSIDED, m_twosided);
	DDX_Text(pDX, IDC_ANIMDURATION, m_animduration);
	DDV_MinMaxInt(pDX, m_animduration, 0, 31);
	DDX_Text(pDX, IDC_ANIMLENGTH, m_animlenght);
	DDV_MinMaxInt(pDX, m_animlenght, 0, 7);
	DDX_Check(pDX, IDC_ANIMTEX, m_animated);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTextureProps, CDialog)
	//{{AFX_MSG_MAP(CTextureProps)
	ON_BN_CLICKED(IDC_ANIMTEX, OnAnimtex)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextureProps message handlers

void CTextureProps::OnAnimtex() 
{
	// TODO: Add your control notification handler code here
	/*if propsDlg.m_animated=False
	{
		propsDlg.m_AnimDurCtrl.disabled=true;
		propsDlg.m_AnimLenCtrl.disabled=true;
	}
	else
	{
		propsDlg.m_AnimDurCtrl.disabled=false;
		propsDlg.m_AnimLenCtrl.disabled=false;
	}*/

}
