// VRoadHeightsSpdFileDlg.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "T3ED.h"
#include "T3EDDoc.h"
#include <math.h> 
#include "GridListCtrl.h"
#include "InPlaceEdit.h"
#include "VRoadHeightsSpdFileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CVRoadHeightsSpdFileDlg 


CVRoadHeightsSpdFileDlg::CVRoadHeightsSpdFileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVRoadHeightsSpdFileDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVRoadHeightsSpdFileDlg)
	m_Chk_Heights = FALSE;
	m_Chk_SpdFa = FALSE;
	m_Chk_SpdRa = FALSE;
	m_Chk_VRoad = FALSE;
	//}}AFX_DATA_INIT
	m_pImageList = 0;
	m_pListEdit = 0;
}


void CVRoadHeightsSpdFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVRoadHeightsSpdFileDlg)
	DDX_Control(pDX, IDC_LIST, m_GridListCtrl);
	DDX_Check(pDX, IDC_CHECK_HEIGHTS, m_Chk_Heights);
	DDX_Check(pDX, IDC_CHECK_SPDFA, m_Chk_SpdFa);
	DDX_Check(pDX, IDC_CHECK_SPDRA, m_Chk_SpdRa);
	DDX_Check(pDX, IDC_CHECK_VROAD, m_Chk_VRoad);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVRoadHeightsSpdFileDlg, CDialog)
	//{{AFX_MSG_MAP(CVRoadHeightsSpdFileDlg)
	ON_NOTIFY(LVN_BEGINLABELEDIT, IDC_LIST, OnBeginlabeleditList)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST, OnEndlabeleditList)
	ON_BN_CLICKED(IDC_CHECK_HEIGHTS, OnCheckHeights)
	ON_BN_CLICKED(IDC_CHECK_SPDFA, OnCheckSpdfa)
	ON_BN_CLICKED(IDC_CHECK_SPDRA, OnCheckSpdra)
	ON_BN_CLICKED(IDC_CHECK_VROAD, OnCheckVroad)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CVRoadHeightsSpdFileDlg 

BOOL CVRoadHeightsSpdFileDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	int i,iActualItem;
	TCHAR buffer[30];

	VERIFY(font.CreateFont(
       16,                        // nHeight
       0,                         // nWidth
       0,                         // nEscapement
       0,                         // nOrientation
       FW_NORMAL,                 // nWeight
       FALSE,					  // bItalic
       FALSE,                     // bUnderline
       0,                         // cStrikeOut
       ANSI_CHARSET,              // nCharSet
       OUT_DEFAULT_PRECIS,        // nOutPrecision
       CLIP_DEFAULT_PRECIS,       // nClipPrecision
       DEFAULT_QUALITY,           // nQuality
       DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
       _T("Arial"))
    );  
	
	GetDlgItem(IDC_LIST)->SetFont(&font);

	m_GridListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES);
	m_GridListCtrl.PrepareControl(0);

	LV_ITEM			lvitem;
	LV_COLUMN		lvcolumn;
	CRect rect;
	m_GridListCtrl.GetWindowRect( &rect );


	// Insert columns using the order field
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH | LVCF_ORDER;
	lvcolumn.fmt = LVCFMT_LEFT;
	lvcolumn.pszText = "Nr.";
	lvcolumn.iSubItem = 0;
	lvcolumn.iOrder = 0;
	lvcolumn.cx = 40;  
	m_GridListCtrl.InsertColumn(0, &lvcolumn);  

	lvcolumn.pszText = "VRoad X";
	lvcolumn.iSubItem = 1;
	lvcolumn.iOrder = 1;
	lvcolumn.cx = 100;  
	m_GridListCtrl.InsertColumn(1, &lvcolumn);  

	lvcolumn.pszText = "VRoad Y";
	lvcolumn.iSubItem = 2;
	lvcolumn.iOrder = 2;
	lvcolumn.cx = 100;  
	m_GridListCtrl.InsertColumn(2, &lvcolumn);  

	lvcolumn.pszText = "VRoad Z";
	lvcolumn.iSubItem = 3;
	lvcolumn.iOrder = 3;
	lvcolumn.cx = 100;  
	m_GridListCtrl.InsertColumn(3, &lvcolumn);  

	lvcolumn.pszText = "Heights.sim";
	lvcolumn.iSubItem = 4;
	lvcolumn.iOrder = 4;
	lvcolumn.cx = 100;  
	m_GridListCtrl.InsertColumn(4, &lvcolumn);

	lvcolumn.pszText = "spdFA Speed";
	lvcolumn.iSubItem = 5;
	lvcolumn.iOrder = 5;
	lvcolumn.cx = 100;  
	m_GridListCtrl.InsertColumn(5, &lvcolumn);

	lvcolumn.pszText = "spdFA prejection";
	lvcolumn.iSubItem = 6;
	lvcolumn.iOrder = 6;
	lvcolumn.cx = 100;  
	m_GridListCtrl.InsertColumn(6, &lvcolumn);

	lvcolumn.pszText = "spdFA float";
	lvcolumn.iSubItem = 7;
	lvcolumn.iOrder = 7;
	lvcolumn.cx = 100;  
	m_GridListCtrl.InsertColumn(7, &lvcolumn);

	lvcolumn.pszText = "spdRA Speed";
	lvcolumn.iSubItem = 8;
	lvcolumn.iOrder = 8;
	lvcolumn.cx = 100;  
	m_GridListCtrl.InsertColumn(8, &lvcolumn);

	lvcolumn.pszText = "spdRA prejection";
	lvcolumn.iSubItem = 9;
	lvcolumn.iOrder = 9;
	lvcolumn.cx = 100;  
	m_GridListCtrl.InsertColumn(9, &lvcolumn);

	lvcolumn.pszText = "spdRA float";
	lvcolumn.iSubItem = 10;
	lvcolumn.iOrder = 10;
	lvcolumn.cx = 100;  
	m_GridListCtrl.InsertColumn(10, &lvcolumn);

	lvitem.mask=LVIF_TEXT;
	for (i=0;i<m_nPositions;i++)
	{
		lvitem.mask = LVIF_TEXT; //Prepare first colum
		lvitem.iItem=i;
		lvitem.iSubItem=0;
		sprintf(buffer, "%d.", i);
		lvitem.pszText = buffer;
		iActualItem=m_GridListCtrl.InsertItem(&lvitem); //Finally add the column

		lvitem.mask = LVIF_TEXT; //Prepare second colum, add vroad x values
		lvitem.iItem=iActualItem;
		lvitem.iSubItem=1;
		sprintf(buffer, "%f", (float)m_colvrdata[i].refPt.x/65536);
		lvitem.pszText = buffer;
		m_GridListCtrl.SetItem(&lvitem);

		lvitem.mask = LVIF_TEXT; //Prepare 3rd colum, add vroad y values
		lvitem.iItem=iActualItem;
		lvitem.iSubItem=2;
		sprintf(buffer, "%f", (float)m_colvrdata[i].refPt.y/65536);
		lvitem.pszText = buffer;
		m_GridListCtrl.SetItem(&lvitem);

		lvitem.mask = LVIF_TEXT; //Prepare 4th colum, add vroad z values
		lvitem.iItem=iActualItem;
		lvitem.iSubItem=3;
		sprintf(buffer, "%f", (float)m_colvrdata[i].refPt.z/65536);
		lvitem.pszText = buffer;
		m_GridListCtrl.SetItem(&lvitem);

		lvitem.mask = LVIF_TEXT; //Prepare 5th colum, add heights.sim values
		lvitem.iItem=iActualItem;
		lvitem.iSubItem=4;
		sprintf(buffer, "%f", (float)m_heightsdata[i].heights);
		lvitem.pszText = buffer;
		m_GridListCtrl.SetItem(&lvitem);

		lvitem.mask = LVIF_TEXT; //Prepare 6th colum, add spdFA.bin speed values
		lvitem.iItem=iActualItem;
		lvitem.iSubItem=5;
		sprintf(buffer, "%d", (int)spdFAdata[i].Speedvalue);
		lvitem.pszText = buffer;
		m_GridListCtrl.SetItem(&lvitem);

		lvitem.mask = LVIF_TEXT; //Prepare 7th colum, add spdFA.bin best prejection values
		lvitem.iItem=iActualItem;
		lvitem.iSubItem=6;
		sprintf(buffer, "%d", (int)spdFAdata[i].Bestprejection);
		lvitem.pszText = buffer;
		m_GridListCtrl.SetItem(&lvitem);

		lvitem.mask = LVIF_TEXT; //Prepare 8th colum, add spdFA.bin turn values
		lvitem.iItem=iActualItem;
		lvitem.iSubItem=7;
		sprintf(buffer, "%f", (float)spdFAdata[i].turn);
		lvitem.pszText = buffer;
		m_GridListCtrl.SetItem(&lvitem);

		lvitem.mask = LVIF_TEXT; //Prepare 9th colum, add spdRA.bin speed values
		lvitem.iItem=iActualItem;
		lvitem.iSubItem=8;
		sprintf(buffer, "%d", (int)spdRAdata[i].Speedvalue);
		lvitem.pszText = buffer;
		m_GridListCtrl.SetItem(&lvitem);

		lvitem.mask = LVIF_TEXT; //Prepare 10th colum, add spdRA.bin best prejection values
		lvitem.iItem=iActualItem;
		lvitem.iSubItem=9;
		sprintf(buffer, "%d", (int)spdRAdata[i].Bestprejection);
		lvitem.pszText = buffer;
		m_GridListCtrl.SetItem(&lvitem);

		lvitem.mask = LVIF_TEXT; //Prepare 11th colum, add spdRA.bin turn values
		lvitem.iItem=iActualItem;
		lvitem.iSubItem=10;
		sprintf(buffer, "%f", (float)spdRAdata[i].turn);
		lvitem.pszText = buffer;
		m_GridListCtrl.SetItem(&lvitem);

	}
	
	OnCheckVroad();
	OnCheckHeights();
	OnCheckSpdfa();
	OnCheckSpdra();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}


void CVRoadHeightsSpdFileDlg::OnBeginlabeleditList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	CString str = pDispInfo->item.pszText;
	int item = pDispInfo->item.iItem;
	int subitem = pDispInfo->item.iSubItem;
	// Construct and create the custom multiline edit control.
	// We could just as well have used a combobox, checkbox, 
	// rich text control, etc.
	m_pListEdit = new CInPlaceEdit( item, subitem, str );
	// Start with a small rectangle.  We'll change it later.
	CRect  rect( 0,0,1,1 );
	DWORD dwStyle = ES_LEFT;
	dwStyle |= WS_BORDER|WS_CHILD|WS_VISIBLE|ES_MULTILINE|ES_AUTOVSCROLL;
	m_pListEdit->Create( dwStyle, rect, &m_GridListCtrl, 103 );
	// Have the Grid position and size the custom edit control
	m_GridListCtrl.PositionControl( m_pListEdit, item, subitem );
	// Have the edit box size itself to its content.
	m_pListEdit->CalculateSize();
	// Return TRUE so that the list control will hnadle NOT edit label itself. 
	*pResult = 1;
}

void CVRoadHeightsSpdFileDlg::OnEndlabeleditList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	int item = pDispInfo->item.iItem;
	int subitem = pDispInfo->item.iSubItem;
	// This is coming from the grid list control notification.
	if( m_pListEdit )
		{
		CString str;
		if( pDispInfo->item.pszText )
			m_GridListCtrl.SetItemText( item, subitem, pDispInfo->item.pszText );
		delete m_pListEdit;
		m_pListEdit = 0;
		}
	*pResult = 0;
}

void CVRoadHeightsSpdFileDlg::OnOK() 
{
	int i,iw;
	//CString Str;
	float fw;

	for (i=0;i<m_nPositions;i++) //Read the changed values and write them back
	{
		//Read Values from GridList
		if (sscanf(m_GridListCtrl.GetItemText(i,1), "%f",&fw)==1)
			m_colvrdata[i].refPt.x=(int) Runden(65536 * fw); //VRoad.x is valid
		if (sscanf(m_GridListCtrl.GetItemText(i,2), "%f",&fw)==1)
			m_colvrdata[i].refPt.y=(int) Runden(65536 * fw); //VRoad.Y is valid
		if (sscanf(m_GridListCtrl.GetItemText(i,3), "%f",&fw)==1)
			m_colvrdata[i].refPt.z=(int) Runden(65536 * fw); //VRoad.Z is valid
		if (sscanf(m_GridListCtrl.GetItemText(i,4), "%f",&fw)==1)
			m_heightsdata[i].heights=fw; //heights is valid
		if (sscanf(m_GridListCtrl.GetItemText(i,5), "%d",&iw)==1)
			spdFAdata[i].Speedvalue=iw; //Speedvalueis valid
		if (sscanf(m_GridListCtrl.GetItemText(i,6), "%d",&iw)==1)
			spdFAdata[i].Bestprejection=iw; //Speedvalueis valid
		if (sscanf(m_GridListCtrl.GetItemText(i,7), "%f",&fw)==1)
			spdFAdata[i].turn=fw; //heights is valid
		if (sscanf(m_GridListCtrl.GetItemText(i,8), "%d",&iw)==1)
			spdRAdata[i].Speedvalue=iw; //Speedvalueis valid
		if (sscanf(m_GridListCtrl.GetItemText(i,9), "%d",&iw)==1)
			spdRAdata[i].Bestprejection=iw; //Speedvalueis valid
		if (sscanf(m_GridListCtrl.GetItemText(i,10), "%f",&fw)==1)
			spdRAdata[i].turn=fw; //heights is valid
	}
	
	CDialog::OnOK();
}

double CVRoadHeightsSpdFileDlg::Runden(double x)
{
	return (x >= 0) ? floor(x+0.5) : ceil(x-0.5);
}

void CVRoadHeightsSpdFileDlg::OnCheckHeights() 
{
	UpdateData(TRUE);
	if (m_Chk_Heights)
		m_GridListCtrl.SetColumnWidth(4,100);
	else
		m_GridListCtrl.SetColumnWidth(4,0);
	UpdateData(FALSE);	
}

void CVRoadHeightsSpdFileDlg::OnCheckSpdfa() 
{
	UpdateData(TRUE);
	if (m_Chk_SpdFa)
	{
		m_GridListCtrl.SetColumnWidth(5,100);
		m_GridListCtrl.SetColumnWidth(6,100);
		m_GridListCtrl.SetColumnWidth(7,100);
	}
	else
	{
		m_GridListCtrl.SetColumnWidth(5,0);
		m_GridListCtrl.SetColumnWidth(6,0);
		m_GridListCtrl.SetColumnWidth(7,0);
	}
	UpdateData(FALSE);	
}

void CVRoadHeightsSpdFileDlg::OnCheckSpdra() 
{
	UpdateData(TRUE);
	if (m_Chk_SpdRa)
	{
		m_GridListCtrl.SetColumnWidth(8,100);
		m_GridListCtrl.SetColumnWidth(9,100);
		m_GridListCtrl.SetColumnWidth(10,100);
	}
	else
	{
		m_GridListCtrl.SetColumnWidth(8,0);
		m_GridListCtrl.SetColumnWidth(9,0);
		m_GridListCtrl.SetColumnWidth(10,0);
	}
	UpdateData(FALSE);	
}

void CVRoadHeightsSpdFileDlg::OnCheckVroad() 
{
	UpdateData(TRUE);
	if (m_Chk_VRoad)
	{
		m_GridListCtrl.SetColumnWidth(1,100);
		m_GridListCtrl.SetColumnWidth(2,100);
		m_GridListCtrl.SetColumnWidth(3,100);
	}
	else
	{
		m_GridListCtrl.SetColumnWidth(1,0);
		m_GridListCtrl.SetColumnWidth(2,0);
		m_GridListCtrl.SetColumnWidth(3,0);
	}
	UpdateData(FALSE);
}
