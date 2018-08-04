// T3EDFrame.cpp : implementation of the CT3EDFrame class
//

#include "stdafx.h"
#include "T3ED.h"

#include "T3EDFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CT3EDFrame

IMPLEMENT_DYNCREATE(CT3EDFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CT3EDFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CT3EDFrame)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	// Global help commands
	ON_COMMAND(ID_HELP_FINDER, CFrameWnd::OnHelpFinder)
	ON_COMMAND(ID_HELP, CFrameWnd::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, CFrameWnd::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, CFrameWnd::OnHelpFinder)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_BLOCKNUM,
	ID_INDICATOR_SCROLLMODE,
	ID_INDICATOR_CORDINATES,
	ID_INDICATOR_PASTEMODE
};

/////////////////////////////////////////////////////////////////////////////
// CT3EDFrame construction/destruction

CT3EDFrame::CT3EDFrame()
{
	// TODO: add member initialization code here
	
}

CT3EDFrame::~CT3EDFrame()
{
}

int CT3EDFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	
	
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.Create(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	// TODO: Remove this if you don't want tool tips or a resizeable toolbar
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	return 0;
}

BOOL CT3EDFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CT3EDFrame diagnostics

#ifdef _DEBUG
void CT3EDFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CT3EDFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CT3EDFrame message handlers


