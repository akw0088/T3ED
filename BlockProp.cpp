// BlockProp.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "T3ED.h"
#include "BlockProp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CBlockProp 


CBlockProp::CBlockProp(CWnd* pParent /*=NULL*/)
	: CDialog(CBlockProp::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBlockProp)
	m_float_ptCentreX = 0.0f;
	m_float_ptCentreY = 0.0f;
	m_float_ptCentreZ = 0.0f;
	m_nVertices = 0;
	m_nHiResVert = 0;
	m_nLoResVert = 0;
	m_nMedResVert = 0;
	m_nLightsrc = 0;
	m_nSoundsrc = 0;
	//}}AFX_DATA_INIT
}


void CBlockProp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBlockProp)
	DDX_Text(pDX, IDC_EDIT_X, m_float_ptCentreX);
	DDX_Text(pDX, IDC_EDIT_Y, m_float_ptCentreY);
	DDX_Text(pDX, IDC_EDIT_Z, m_float_ptCentreZ);
	DDX_Text(pDX, IDC_EDIT_NVERTICES, m_nVertices);
	DDX_Text(pDX, IDC_EDIT_NHIGHVERT, m_nHiResVert);
	DDX_Text(pDX, IDC_EDIT_NLOWVERT, m_nLoResVert);
	DDX_Text(pDX, IDC_EDIT_NMEDVERT, m_nMedResVert);
	DDX_Text(pDX, IDC_EDIT_NLIGHTSRC, m_nLightsrc);
	DDX_Text(pDX, IDC_EDIT_NSOUNDSRC, m_nSoundsrc);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBlockProp, CDialog)
	//{{AFX_MSG_MAP(CBlockProp)
		// HINWEIS: Der Klassen-Assistent fügt hier Zuordnungsmakros für Nachrichten ein
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CBlockProp 
