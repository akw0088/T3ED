// ExtraObjProps.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "T3ED.h"
#include "ExtraObjProps.h"
#include "T3EDDoc.h"
#include <math.h> 
//#include <string.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CExtraObjProps 


CExtraObjProps::CExtraObjProps(CWnd* pParent /*=NULL*/)
	: CDialog(CExtraObjProps::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExtraObjProps)
	m_int_Chunk = 0;
	m_int_Block = 0;
	m_int_NumPoly = 0;
	m_int_ObjNum = 0;
	m_long_Crosstype = 0;
	m_float_x = 0.0f;
	m_float_y = 0.0f;
	m_float_z = 0.0f;
	m_short_unknown1 = 0;
	m_short_unknown2 = 0;
	m_short_collide_effect = 0;
	m_long_unknown = 0;
	m_long_AnimMemory = 0;
	m_bool_ReCenter = FALSE;
	m_short_AnimDelay = 0;
	m_short_AnimM1 = 0;
	m_short_AnimM2 = 0;
	m_short_AnimM3 = 0;
	m_short_AnimM4 = 0;
	m_s_cb_UK3 = _T("");
	m_float_AnimX = 0.0f;
	m_float_AnimY = 0.0f;
	m_float_AnimZ = 0.0f;
	m_bool_Convert_Global = FALSE;
	m_bool_Convert_Poly = FALSE;
	//}}AFX_DATA_INIT
}


void CExtraObjProps::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExtraObjProps)
	DDX_Text(pDX, IDC_EDIT_CHUNK, m_int_Chunk);
	DDX_Text(pDX, IDC_EDIT_BLOCK, m_int_Block);
	DDX_Text(pDX, IDC_EDIT_NUMPOLY, m_int_NumPoly);
	DDX_Text(pDX, IDC_EDIT_OBJNUM, m_int_ObjNum);
	DDX_Text(pDX, IDC_EDIT_X, m_float_x);
	DDX_Text(pDX, IDC_EDIT_Y, m_float_y);
	DDX_Text(pDX, IDC_EDIT_Z, m_float_z);
	DDX_Text(pDX, IDC_EDIT_REFX_UK1, m_short_unknown1);
	DDX_Text(pDX, IDC_EDIT_REFX_UK2, m_short_unknown2);
	DDX_Text(pDX, IDC_EDIT_REFX_UK3, m_short_collide_effect);
	DDV_MinMaxInt(pDX, m_short_collide_effect, 0, 255);
	DDX_Text(pDX, IDC_EDIT_XOBJ_UK, m_long_unknown);
	DDX_Text(pDX, IDC_EDIT_XOBJ_ANIM_MEM, m_long_AnimMemory);
	DDX_Check(pDX, IDC_CHECK_RECENTER, m_bool_ReCenter);
	DDX_Text(pDX, IDC_EDIT_XOBJ_ANIMDELAY, m_short_AnimDelay);
	DDX_Text(pDX, IDC_EDIT_ANIM_M1, m_short_AnimM1);
	DDX_Text(pDX, IDC_EDIT_ANIM_M2, m_short_AnimM2);
	DDX_Text(pDX, IDC_EDIT_ANIM_M3, m_short_AnimM3);
	DDX_Text(pDX, IDC_EDIT_ANIM_M4, m_short_AnimM4);
	DDX_CBString(pDX, IDC_COMBO_XOBJ_UK3, m_s_cb_UK3);
	DDV_MaxChars(pDX, m_s_cb_UK3, 5);
	DDX_Text(pDX, IDC_EDIT_ANIM_X, m_float_AnimX);
	DDX_Text(pDX, IDC_EDIT_ANIM_Y, m_float_AnimY);
	DDX_Text(pDX, IDC_EDIT_ANIM_Z, m_float_AnimZ);
	DDX_Check(pDX, IDC_CHECK_CONVERT_GLOBAL, m_bool_Convert_Global);
	DDX_Check(pDX, IDC_CHECK_CONVERT_POLY, m_bool_Convert_Poly);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExtraObjProps, CDialog)
	//{{AFX_MSG_MAP(CExtraObjProps)
	ON_EN_CHANGE(IDC_EDIT_CHUNK, OnChangeEditChunk)
	ON_CBN_SELCHANGE(IDC_COMBO_CROSSTYPE, OnSelchangeComboCrosstype)
	ON_BN_CLICKED(IDC_CHECK_RECENTER, OnCheckRecenter)
	ON_CBN_SELCHANGE(IDC_COMBO_RECENTER, OnSelchangeComboRecenter)
	ON_CBN_SELCHANGE(IDC_COMBO_ANIML, OnSelchangeComboAniml)
	ON_BN_CLICKED(IDC_BUTTON_ANIM_EXPORT, OnButtonAnimExport)
	ON_BN_CLICKED(IDC_BUTTON_ANIM_IMPORT, OnButtonAnimImport)
	ON_CBN_KILLFOCUS(IDC_COMBO_XOBJ_UK3, OnKillfocusComboXobjUk3)
	ON_CBN_SELCHANGE(IDC_COMBO_XOBJ_UK3, OnSelchangeComboXobjUk3)
	ON_CBN_DROPDOWN(IDC_COMBO_XOBJ_UK3, OnDropdownComboXobjUk3)
	ON_CBN_EDITCHANGE(IDC_COMBO_XOBJ_UK3, OnEditchangeComboXobjUk3)
	ON_BN_CLICKED(IDC_CHECK_CONVERT_POLY, OnCheckConvertPoly)
	ON_BN_CLICKED(IDC_CHECK_CONVERT_GLOBAL, OnCheckConvertGlobal)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CExtraObjProps 

BOOL CExtraObjProps::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString sStr = "";
	
	// TODO: Zusätzliche Initialisierung hier einfügen
	CWnd *edblock=GetDlgItem(IDC_EDIT_BLOCK);
	CWnd *edchunk=GetDlgItem(IDC_EDIT_CHUNK);
	CComboBox *pCbCrosstype = (CComboBox*) GetDlgItem(IDC_COMBO_CROSSTYPE);
	pCbCrosstype->AddString("1 - <Global>");
	pCbCrosstype->SetItemData(0,1);
	pCbCrosstype->AddString("2 - Simple");
	pCbCrosstype->SetItemData(1,2);
	pCbCrosstype->AddString("3 - Animated");
	pCbCrosstype->SetItemData(2,3);
	pCbCrosstype->AddString("4 - Collision");
	pCbCrosstype->SetItemData(3,4);
	pCbCrosstype->SetCurSel(m_long_Crosstype-1);
	GetDlgItem(IDC_COMBO_CROSSTYPE)->EnableWindow(TRUE); 

	m_bool_ReCenter=FALSE;
	m_int_ReCenter=2;
	CComboBox *pCbReCenter = (CComboBox*) GetDlgItem(IDC_COMBO_RECENTER);
	pCbReCenter->AddString("X & Y = Middle | Z = Top");
	pCbReCenter->SetItemData(0,1);
	pCbReCenter->AddString("X & Y = Middle | Z = Middle");
	pCbReCenter->SetItemData(1,2);
	pCbReCenter->AddString("X & Y = Middle | Z = Bottom");
	pCbReCenter->SetItemData(2,3);
	pCbReCenter->AddString("Manual Change");
	pCbReCenter->SetItemData(3,4);
	pCbReCenter->SetCurSel(m_int_ReCenter-1);

	if (m_long_Crosstype==3)
	{	
		InitAnimCombobox();
	}

	if (!m_bExtraObjPropsAutoObjMem)
	{
		edblock->EnableWindow(TRUE);
		edchunk->EnableWindow(TRUE);
		//GetDlgItem(IDC_COMBO_CROSSTYPE)->EnableWindow(TRUE); 
	}
	if (m_bool_IsGlobal)
	{
		GetDlgItem(IDC_CHECK_CONVERT_GLOBAL)->SetWindowText("Convert to Extra Object : "); 
	}

	InitUnknown3Combobox();
	
	CExtraObjProps::UpdateView();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}

void CExtraObjProps::OnChangeEditChunk() 
{
	UpdateData(TRUE);
	if (m_int_Chunk<0) 	
		m_int_Chunk=0;

	if (m_int_Chunk>3) 	
		m_int_Chunk=3;
	UpdateData(FALSE);
}

void CExtraObjProps::OnSelchangeComboCrosstype() 
{
	// TODO: Code für die Behandlungsroutine der Steuerelement-Benachrichtigung hier einfügen
	int crosstype;
	CComboBox *pCbCrosstype = (CComboBox*) GetDlgItem(IDC_COMBO_CROSSTYPE);
	//selcombo=pCbCrosstype->GetCurSel();
	crosstype=pCbCrosstype->GetItemData(pCbCrosstype->GetCurSel());
	if (crosstype==1)
		pCbCrosstype->SetCurSel(m_long_Crosstype-1);
	else
		{
			m_long_Crosstype=crosstype;
			//UpdateView();
			if (crosstype==3)
			{
				m_short_unknown1=0;
				m_short_unknown2=0;
				m_short_collide_effect=0;
				m_long_AnimMemory=348;
				m_short_AnimDelay=50;
			}
			if (crosstype==2)
			{
				m_short_unknown1=0;
				m_short_unknown2=0;
				m_short_collide_effect=0;
			}
			if (crosstype==4)
			{
				m_short_unknown1=255;
				m_short_unknown2=255;
				m_short_collide_effect=1;
			}
			UpdateView();
			UpdateData(FALSE);
		}
	CheckSel();
}

void CExtraObjProps::CheckSel()
{
	/*if (m_long_Crosstype==4) //Only Type 4 Objects have a Behavior
		GetDlgItem(IDC_EDIT_BEHAVIOR)->EnableWindow(TRUE); 
	else
		GetDlgItem(IDC_EDIT_BEHAVIOR)->EnableWindow(FALSE); */
}

void CExtraObjProps::UpdateView()
{
		if (!m_bool_Convert_Poly)
		{
			GetDlgItem(IDC_TXT_REFX_UK1)->EnableWindow(TRUE); 
			GetDlgItem(IDC_TXT_REFX_UK2)->EnableWindow(TRUE); 
			GetDlgItem(IDC_TXT_REFX_UK3)->EnableWindow(TRUE); 
			GetDlgItem(IDC_GROUP_REFXOBJ)->EnableWindow(TRUE); 
			GetDlgItem(IDC_TXT_CROSSTYPE)->EnableWindow(TRUE); 
			GetDlgItem(IDC_COMBO_CROSSTYPE)->EnableWindow(TRUE); 
			GetDlgItem(IDC_CHECK_RECENTER)->EnableWindow(TRUE); 
			GetDlgItem(IDC_EDIT_XOBJ_UK)->EnableWindow(TRUE); 
			GetDlgItem(IDC_TXT_XOBJ_UK)->EnableWindow(TRUE); 
			GetDlgItem(IDC_EDIT_XOBJ_ANIM_MEM)->EnableWindow(TRUE); 
			GetDlgItem(IDC_TXT_XOBJ_ANIM_MEM)->EnableWindow(TRUE); 
			GetDlgItem(IDC_COMBO_XOBJ_UK3)->EnableWindow(TRUE); 
			GetDlgItem(IDC_TXT_XOBJ_UK3)->EnableWindow(TRUE); 
			GetDlgItem(IDC_CHECK_CONVERT_GLOBAL)->EnableWindow(TRUE); 
		}
		else
		{
			GetDlgItem(IDC_TXT_REFX_UK1)->EnableWindow(FALSE);
			GetDlgItem(IDC_TXT_REFX_UK2)->EnableWindow(FALSE);
			GetDlgItem(IDC_TXT_REFX_UK3)->EnableWindow(FALSE);
			GetDlgItem(IDC_GROUP_REFXOBJ)->EnableWindow(FALSE);
			GetDlgItem(IDC_TXT_CROSSTYPE)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO_CROSSTYPE)->EnableWindow(FALSE);
			GetDlgItem(IDC_CHECK_RECENTER)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_XOBJ_UK)->EnableWindow(FALSE);
			GetDlgItem(IDC_TXT_XOBJ_UK)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_XOBJ_ANIM_MEM)->EnableWindow(FALSE);
			GetDlgItem(IDC_TXT_XOBJ_ANIM_MEM)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO_XOBJ_UK3)->EnableWindow(FALSE);
			GetDlgItem(IDC_TXT_XOBJ_UK3)->EnableWindow(FALSE);
			GetDlgItem(IDC_CHECK_CONVERT_GLOBAL)->EnableWindow(FALSE); 
		}

		if (!m_bool_Convert_Global)
		{
			GetDlgItem(IDC_CHECK_CONVERT_POLY)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_CHECK_CONVERT_POLY)->EnableWindow(FALSE);
		}


		if ((m_long_Crosstype==3)&&(!m_bool_Convert_Poly))
		{
			GetDlgItem(IDC_TXT_XOBJ_ANIMDELAY)->EnableWindow(TRUE); 
			GetDlgItem(IDC_EDIT_XOBJ_ANIMDELAY)->EnableWindow(TRUE); 
			GetDlgItem(IDC_TXT_ANIML)->EnableWindow(TRUE); 
			GetDlgItem(IDC_COMBO_ANIML)->EnableWindow(TRUE); 
			GetDlgItem(IDC_GROUP_ANIMDATA)->EnableWindow(TRUE); 
			GetDlgItem(IDC_TXT_ANIM_X)->EnableWindow(TRUE); 
			GetDlgItem(IDC_EDIT_ANIM_X)->EnableWindow(TRUE); 
			GetDlgItem(IDC_TXT_ANIM_Y)->EnableWindow(TRUE); 
			GetDlgItem(IDC_EDIT_ANIM_Y)->EnableWindow(TRUE); 
			GetDlgItem(IDC_TXT_ANIM_Z)->EnableWindow(TRUE); 
			GetDlgItem(IDC_EDIT_ANIM_Z)->EnableWindow(TRUE);
			GetDlgItem(IDC_TXT_ANIM_M1)->EnableWindow(TRUE); 
			GetDlgItem(IDC_EDIT_ANIM_M1)->EnableWindow(TRUE);
			GetDlgItem(IDC_TXT_ANIM_M2)->EnableWindow(TRUE); 
			GetDlgItem(IDC_EDIT_ANIM_M2)->EnableWindow(TRUE);
			GetDlgItem(IDC_TXT_ANIM_M3)->EnableWindow(TRUE); 
			GetDlgItem(IDC_EDIT_ANIM_M3)->EnableWindow(TRUE);
			GetDlgItem(IDC_TXT_ANIM_M4)->EnableWindow(TRUE); 
			GetDlgItem(IDC_EDIT_ANIM_M4)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_ANIM_EXPORT)->EnableWindow(TRUE); 
			GetDlgItem(IDC_BUTTON_ANIM_IMPORT)->EnableWindow(TRUE); 
		}
		else
		{
			GetDlgItem(IDC_TXT_XOBJ_ANIMDELAY)->EnableWindow(FALSE); 
			GetDlgItem(IDC_EDIT_XOBJ_ANIMDELAY)->EnableWindow(FALSE); 
			GetDlgItem(IDC_TXT_ANIML)->EnableWindow(FALSE); 
			GetDlgItem(IDC_COMBO_ANIML)->EnableWindow(FALSE); 
			GetDlgItem(IDC_GROUP_ANIMDATA)->EnableWindow(FALSE); 
			GetDlgItem(IDC_TXT_ANIM_X)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_ANIM_X)->EnableWindow(FALSE);
			GetDlgItem(IDC_TXT_ANIM_Y)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_ANIM_Y)->EnableWindow(FALSE);
			GetDlgItem(IDC_TXT_ANIM_Z)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_ANIM_Z)->EnableWindow(FALSE);
			GetDlgItem(IDC_TXT_ANIM_M1)->EnableWindow(FALSE); 
			GetDlgItem(IDC_EDIT_ANIM_M1)->EnableWindow(FALSE);
			GetDlgItem(IDC_TXT_ANIM_M2)->EnableWindow(FALSE); 
			GetDlgItem(IDC_EDIT_ANIM_M2)->EnableWindow(FALSE);
			GetDlgItem(IDC_TXT_ANIM_M3)->EnableWindow(FALSE); 
			GetDlgItem(IDC_EDIT_ANIM_M3)->EnableWindow(FALSE);
			GetDlgItem(IDC_TXT_ANIM_M4)->EnableWindow(FALSE); 
			GetDlgItem(IDC_EDIT_ANIM_M4)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_ANIM_EXPORT)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_ANIM_IMPORT)->EnableWindow(FALSE);
		}

		if ((m_long_Crosstype==4)&&(!m_bool_Convert_Poly))
		{
			GetDlgItem(IDC_EDIT_REFX_UK1)->EnableWindow(TRUE); 
			GetDlgItem(IDC_EDIT_REFX_UK2)->EnableWindow(TRUE); 
			GetDlgItem(IDC_EDIT_REFX_UK3)->EnableWindow(TRUE); 
		}
		else
		{
			GetDlgItem(IDC_EDIT_REFX_UK1)->EnableWindow(FALSE); 
			GetDlgItem(IDC_EDIT_REFX_UK2)->EnableWindow(FALSE); 
			GetDlgItem(IDC_EDIT_REFX_UK3)->EnableWindow(FALSE); 
		}

		if ((m_bool_ReCenter)&&(!m_bool_Convert_Poly))
			GetDlgItem(IDC_COMBO_RECENTER)->EnableWindow(TRUE); 
		else
			GetDlgItem(IDC_COMBO_RECENTER)->EnableWindow(FALSE); 

		if ((m_bool_ReCenter)&&(m_int_ReCenter==4)&&(!m_bool_Convert_Poly))
		{
			GetDlgItem(IDC_EDIT_X)->EnableWindow(TRUE); 
			GetDlgItem(IDC_EDIT_Y)->EnableWindow(TRUE); 
			GetDlgItem(IDC_EDIT_Z)->EnableWindow(TRUE); 
		}
		else
		{
			GetDlgItem(IDC_EDIT_X)->EnableWindow(FALSE); 
			GetDlgItem(IDC_EDIT_Y)->EnableWindow(FALSE); 
			GetDlgItem(IDC_EDIT_Z)->EnableWindow(FALSE); 
		}

		if (m_bool_IsGlobal) //Global objects can't be converted to polygon objects
		{
			GetDlgItem(IDC_CHECK_CONVERT_POLY)->EnableWindow(FALSE);
		}


}

void CExtraObjProps::OnCheckRecenter() 
{
	// TODO: Code für die Behandlungsroutine der Steuerelement-Benachrichtigung hier einfügen
	m_bool_ReCenter=!m_bool_ReCenter;
	UpdateView();
}

void CExtraObjProps::OnSelchangeComboRecenter() 
{
	CComboBox *pCbReCenter = (CComboBox*) GetDlgItem(IDC_COMBO_RECENTER);
	m_int_ReCenter=pCbReCenter->GetItemData(pCbReCenter->GetCurSel());
	UpdateView();
}


void CExtraObjProps::OnSelchangeComboAniml() 
{
	//struct ANIMDATA *m_AnimData;
	double pi = 3.14159265358979323846264338, t;
	int cursel;

	if ((LastCbAnimSel>-1)&&(LastCbAnimSel<m_nAnimLength)) //Write back changed values ?
	{
		UpdateData(TRUE);
		t=m_float_AnimX*65536;
		m_AnimData[LastCbAnimSel].pt.x=(long)Runden(m_float_AnimX*65536);
		m_AnimData[LastCbAnimSel].pt.y=(long)Runden(m_float_AnimY*65536);
		m_AnimData[LastCbAnimSel].pt.z=(long)Runden(m_float_AnimZ*65536);
		m_AnimData[LastCbAnimSel].od1=m_short_AnimM1;
		m_AnimData[LastCbAnimSel].od2=m_short_AnimM2;
		m_AnimData[LastCbAnimSel].od3=m_short_AnimM3;
		m_AnimData[LastCbAnimSel].od4=m_short_AnimM4;
		UpdateData(FALSE);
	}
	//Show new selection
	CComboBox *pCbAnimL = (CComboBox*) GetDlgItem(IDC_COMBO_ANIML);
	cursel=pCbAnimL->GetItemData(pCbAnimL->GetCurSel());
	UpdateData(TRUE);
	m_float_AnimX=(float)m_AnimData[cursel].pt.x/65536;
	m_float_AnimY=(float)m_AnimData[cursel].pt.y/65536;
	m_float_AnimZ=(float)m_AnimData[cursel].pt.z/65536;
	m_short_AnimM1=m_AnimData[cursel].od1;
	m_short_AnimM2=m_AnimData[cursel].od2;
	m_short_AnimM3=m_AnimData[cursel].od3;
	m_short_AnimM4=m_AnimData[cursel].od4;
	UpdateData(FALSE);
	LastCbAnimSel=cursel;
}

void CExtraObjProps::OnButtonAnimExport() 
{

		CString defPath,defName,filename,fname,sStr;
		int i;
		//float fw;

		CFileDialog m_ldFile(FALSE,"txt",defName,OFN_FILEMUSTEXIST|OFN_HIDEREADONLY,"Text file (*.txt)|*.txt|All Files (*.*)|*.*||");
		if (m_ldFile.DoModal() != IDOK) return; // Dialogfeld Öffnen zeigen und Ergebnis auffangen
		filename = m_ldFile.GetPathName();// Gewählten Pfad & Dateinamen ermitteln
		fname = m_ldFile.GetFileName();// Gewählten Dateinamen ermitteln
		if (!(GetFileAttributes(filename) == 0xFFFFFFFF))
			if (MessageBox("File already exists. Continue ?","T3ED",MB_OKCANCEL)==IDCANCEL) return;

		FILE* fout = fopen(filename, "w");
		if (fout==NULL)
		{
			sStr.Format("Unable to open '%s'.", filename);
			AfxMessageBox(sStr);	
			//Ok=FALSE;
		}
		else
		{
			fprintf(fout, "T3ED_Animation\n"); //Write Header
			fprintf(fout, "Animation_length: %d\n",m_nAnimLength);
			for (i=0;i<m_nAnimLength;i++)
			{
				fprintf(fout, "%f %f %f %d %d %d %d\n",(float) m_AnimData[i].pt.x/65536,(float) m_AnimData[i].pt.y/65536,(float) m_AnimData[i].pt.z/65536, m_AnimData[i].od1, m_AnimData[i].od2, m_AnimData[i].od3, m_AnimData[i].od4);
			}
			fclose(fout);
		}
}

void CExtraObjProps::OnButtonAnimImport() 
{
	short newAnimLength;
	struct ANIMDATA *newAnimData;
	char* buff=new char[255];
	float x,y,z;
	short d1,d2,d3,d4;
	int i;
	BOOL Ok=TRUE;
	CString FileName,FName,s;
	CString defPath="",defName="";

	//FileName="C:\\Temp\\Redrock\\RadarFein.txt";
	CFileDialog m_ldFile(TRUE,"txt",defName,OFN_FILEMUSTEXIST|OFN_HIDEREADONLY,"Text File (*.txt)|*.txt|All Files (*.*)|*.*||");
	// Dialogfeld Öffnen zeigen und Ergebnis auffangen
	if (m_ldFile.DoModal() != IDOK) return;
	// Gewählten Dateinamen ermitteln
	FileName = m_ldFile.GetPathName();
	FName = m_ldFile.GetFileName();

	FILE* fin = fopen(FileName, "r");
	if (fin==NULL)
	{
		s.Format("Unable to open '%s'.", FName);
		AfxMessageBox(s);	
		Ok=FALSE;
	}
	else
	{
		fgets(buff,255,fin); 
		sscanf(buff,"%s",s);
		if (s=="T3ED_Animation") // Ist it a T3ED Animation file ?
		{
			fgets(buff,255,fin); 
			newAnimLength=0;
			sscanf(buff, "Animation_length: %d\n",&newAnimLength); //get number of animation steps

			if (newAnimLength>0)
			{
				newAnimData=(struct ANIMDATA *)malloc(newAnimLength * sizeof(struct ANIMDATA));
				//Fill with zero data 
				memset(newAnimData,0,newAnimLength * sizeof(struct ANIMDATA));
				for (i=0;i<newAnimLength;i++)
				{
					fgets(buff,255,fin); 
					x=m_float_x;y=m_float_y;z=m_float_z;d1=0;d2=0;d3=0;d4=0;
					sscanf(buff, "%f %f %f %d %d %d %d",&x, &y, &z, &d1, &d2, &d3, &d4);
					newAnimData[i].pt.x=(int)(x*65536);
					newAnimData[i].pt.y=(int)(y*65536);
					newAnimData[i].pt.z=(int)(z*65536);
					newAnimData[i].od1=d1;
					newAnimData[i].od2=d2;
					newAnimData[i].od3=d3;
					newAnimData[i].od4=d4;
				}
				Ok=TRUE;
			}
			else
				Ok=FALSE;
		}
		else
			Ok=FALSE;

		fclose(fin);
	}


	if (Ok==TRUE) //Everything Ok ? Then use the new animation
	{
		free (m_AnimData);
		m_AnimData=newAnimData;
		m_nAnimLength=newAnimLength;
		UpdateData(TRUE);
		m_long_AnimMemory=newAnimLength*20 + 8;
		UpdateData(FALSE);
		InitAnimCombobox();
	}
	else //Something is wrong with the animation file
	{
		s.Format("Error reading '%s'.", FName);
		AfxMessageBox(s);	
		Ok=FALSE;
	}
}

void CExtraObjProps::InitAnimCombobox()
{
	CString sStr = "";
	int i;

	LastCbAnimSel=-1;
	CComboBox *pCbAnimL = (CComboBox*) GetDlgItem(IDC_COMBO_ANIML);
	pCbAnimL->ResetContent();
	for (i=0;i<m_nAnimLength;i++)
	{
		sStr.Format("Num %d", i);
		pCbAnimL->AddString(sStr);
		pCbAnimL->SetItemData(i,i);
	}
	pCbAnimL->SetCurSel(0);
	OnSelchangeComboAniml();
}

void CExtraObjProps::InitUnknown3Combobox()
{
	CString sStr = "";
	int i;
	CComboBox *pCbUK3 = (CComboBox*) GetDlgItem(IDC_COMBO_XOBJ_UK3);
	pCbUK3->ResetContent();
	for (i=0;i<9;i++)
	{
		sStr.Format("%d",m_short_unknown3[i]);
		pCbUK3->AddString(sStr);
		pCbUK3->SetItemData(i,m_short_unknown3[i]);
	}
	pCbUK3->SetCurSel(0);
	cb_uk3_sel=0;
	cb_uk3_edit=FALSE;
}

void CExtraObjProps::OnKillfocusComboXobjUk3() 
{
	StoreComboXobjUk3into_UK3();
}

void CExtraObjProps::OnSelchangeComboXobjUk3() 
{
	CComboBox *pCbUK3 = (CComboBox*) GetDlgItem(IDC_COMBO_XOBJ_UK3);
	cb_uk3_sel=pCbUK3->GetCurSel();
}

void CExtraObjProps::OnDropdownComboXobjUk3() 
{
	StoreComboXobjUk3into_UK3();
}

void CExtraObjProps::StoreComboXobjUk3into_UK3()
{
	short inp;
	CString sStr = "";

	if (!cb_uk3_edit) return;
	CComboBox *pCbUK3 = (CComboBox*) GetDlgItem(IDC_COMBO_XOBJ_UK3);
	UpdateData(TRUE);
	inp=-32768;
	sscanf(m_s_cb_UK3, "%d", &inp);
	//UpdateData(FALSE);
	//m_s_cb_UK3="";
	//i=pCbUK3->GetCurSel();
	if (inp!=-32768)
	{
		m_short_unknown3[cb_uk3_sel]=inp;
		sStr.Format("%d",inp);
		pCbUK3->DeleteString(cb_uk3_sel);
		pCbUK3->InsertString(cb_uk3_sel,sStr);
	}
	cb_uk3_edit=FALSE;
}


void CExtraObjProps::OnEditchangeComboXobjUk3() 
{
	// TODO: Code für die Behandlungsroutine der Steuerelement-Benachrichtigung hier einfügen
	cb_uk3_edit=TRUE;
}

double CExtraObjProps::Runden(double x)
{
  return (x >= 0) ? floor(x+0.5) : ceil(x-0.5);
}

void CExtraObjProps::OnCheckConvertPoly() 
{
	m_bool_Convert_Poly=!m_bool_Convert_Poly;
	if (m_bool_Convert_Poly) 
		m_bool_Convert_Global=FALSE;
	UpdateView();	
}

void CExtraObjProps::OnCheckConvertGlobal() 
{
	m_bool_Convert_Global=!m_bool_Convert_Global;
	if (m_bool_Convert_Global) 
		m_bool_Convert_Poly=FALSE;
	UpdateView();	
	
}
