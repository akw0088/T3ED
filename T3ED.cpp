// T3ED.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "T3ED.h"

#include "T3EDFrame.h"
#include "T3EDDoc.h"
#include "T3EDView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CT3EDApp

BEGIN_MESSAGE_MAP(CT3EDApp, CWinApp)
	//{{AFX_MSG_MAP(CT3EDApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CT3EDApp construction

CT3EDApp::CT3EDApp()
{		
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance

}

/////////////////////////////////////////////////////////////////////////////
// The one and only CT3EDApp object

CT3EDApp theApp;
//HOO: (1)
HCURSOR hcursorMove;
HCURSOR hcursorRotate;
//HOO: (1)
/////////////////////////////////////////////////////////////////////////////
// CT3EDApp initialization

BOOL CT3EDApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CT3EDDoc),
		RUNTIME_CLASS(CT3EDFrame),       // main SDI frame window
		RUNTIME_CLASS(CT3EDView));
	AddDocTemplate(pDocTemplate);

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();
	OnAppAbout(); // About...

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	afx_msg void OnCopyright2();
	afx_msg void OnCopyright3();
	afx_msg void OnStaticUrl();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
	char copyright[200],*c;
	int sum;
	CWnd *cstat;
	RECT r1,r2;
	
	cstat=GetDlgItem(IDC_COPYRIGHT);
	cstat->ModifyStyle(WS_DISABLED,WS_VISIBLE);
	GetWindowRect(&r1); 
	cstat->GetWindowRect(&r2);
	cstat->BringWindowToTop();
	sum=314159;
	if ((r2.left<r1.left)||(r2.right>r1.right)||(r2.right-r2.left<180)||
		(r2.top<r1.top)||(r2.bottom>r1.bottom)||(r2.bottom-r2.top<10))
		sum=0;
		
	GetDlgItemText(IDC_COPYRIGHT,copyright,199);
	for (c=copyright;*c!=0;c++)
		sum=19*sum^(sum>>3)+*c;
	if (sum!=1813495491) {
		MessageBox("This version of T3ED has been tampered with !!!!\n"
			"Please download the original version from\n"
			"http://www.math.polytechnique.fr/cmat/auroux/nfs/",
			"Piracy alert",MB_ICONERROR);
		PostQuitMessage(0);
	}
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_BN_CLICKED(IDC_COPYRIGHT2, OnCopyright2)
	ON_BN_CLICKED(IDC_COPYRIGHT3, OnCopyright3)
	ON_BN_CLICKED(IDC_STATIC_URL, OnStaticUrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CT3EDApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CT3EDApp commands

void CAboutDlg::OnCopyright2() 
{
	// TODO: Add your control notification handler code here
	
}

void CAboutDlg::OnCopyright3() 
{
	// TODO: Add your control notification handler code here
	
}

void CAboutDlg::OnStaticUrl() 
{
	// TODO: Add your control notification handler code here
	//MessageBox("keke!");
	ShellExecute(NULL, "Open", "http://rp-design.totalnfs.net", NULL, NULL, SW_NORMAL);
	
}
