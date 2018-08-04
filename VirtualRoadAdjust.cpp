// VirtualRoadAdjust.cpp : implementation file
//

#include "stdafx.h"
#include "T3ED.h"
#include "VirtualRoadAdjust.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVirtualRoadAdjust dialog


CVirtualRoadAdjust::CVirtualRoadAdjust(CWnd* pParent /*=NULL*/)
	: CDialog(CVirtualRoadAdjust::IDD, pParent)
{
	

	//{{AFX_DATA_INIT(CVirtualRoadAdjust)
	m_AdjustType = -1;
	m_MapLaneEdges = FALSE;
	m_StartBlock = 0;
	m_EndBlock = 0;
	m_float_roadwidth = 0.0f;
	//}}AFX_DATA_INIT
}


void CVirtualRoadAdjust::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CDialog::OnFinalRelease();
}

void CVirtualRoadAdjust::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVirtualRoadAdjust)
	DDX_Radio(pDX, IDC_RADIO5, m_AdjustType);
	DDX_Check(pDX, IDC_CHECK1, m_MapLaneEdges);
	DDX_Text(pDX, IDC_EDIT1, m_StartBlock);
	DDV_MinMaxInt(pDX, m_StartBlock, 0, 300);
	DDX_Text(pDX, IDC_EDIT2, m_EndBlock);
	DDV_MinMaxInt(pDX, m_EndBlock, 0, 300);
	DDX_Text(pDX, IDC_EDIT3, m_float_roadwidth);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVirtualRoadAdjust, CDialog)
	//{{AFX_MSG_MAP(CVirtualRoadAdjust)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CVirtualRoadAdjust message handlers
