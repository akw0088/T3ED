// T3EDView.cpp : implementation of the CT3EDView class
//
// Modified by Hoo, 8/16/1999 (Total: 5 items)
// Hoo's D3D View functions added by Nappe1, (2/11/2001)

#include "stdafx.h"
#include "T3ED.h"
#include <math.h>
#include <afxdlgs.h> //Open File

#include "T3EDDoc.h"
#include "T3EDView.h"
#include "smooth.h"
#include "NewobjDlg.h"
#include "ClearAll.h"
#include "PolyFlag.h"
#include <windows.h>
#include "BlockProps.h"
#include "SoundLightProps.h"
#include "LodGenProgress.h"
#include "VirtualRoadAdjust.h"
#include "Vectors.h"
#include "RayTraceDlg.h"
#include "LodStartEnd.h"
#include "ExpandDlg.h"
#include "RotateDlg.h"
#include "BlockPaste.h"
#include "PolyProperties.h"
#include "ExtraObjProps.h"
#include "ExpandBlock.h"
#include "MoveTo.h"
#include "BlockProp.h"
#include "FrdFileDlg.h"
#include "GridListCtrl.h"
#include "InPlaceEdit.h"
#include "VRoadHeightsSpdFileDlg.h"
#include "FindPolygonDlg.h"
#include "SetVisiDlg.h"

#define SCROLLSZ 5000

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CT3EDView

IMPLEMENT_DYNCREATE(CT3EDView, CView)

BEGIN_MESSAGE_MAP(CT3EDView, CView)
	//{{AFX_MSG_MAP(CT3EDView)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_COMMAND(ID_VIEW_FULLRES, OnViewFullres)
	ON_COMMAND(ID_VIEW_HALFRES, OnViewHalfres)
	ON_COMMAND(ID_VIEW_LOWRES, OnViewLowres)
	ON_COMMAND(ID_VIEW_ZOOMIN, OnViewZoomin)
	ON_COMMAND(ID_VIEW_ZOOMOUT, OnViewZoomout)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FULLRES, OnUpdateViewFullres)
	ON_UPDATE_COMMAND_UI(ID_VIEW_HALFRES, OnUpdateViewHalfres)
	ON_UPDATE_COMMAND_UI(ID_VIEW_LOWRES, OnUpdateViewLowres)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMIN, OnUpdateViewZoomin)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMOUT, OnUpdateViewZoomout)
	ON_COMMAND(ID_VIEW_ROTATE, OnViewRotateMode)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ROTATE, OnUpdateViewRotateMode)
	ON_COMMAND(ID_VIEW_HEIGHT, OnViewTranslMode)
	ON_UPDATE_COMMAND_UI(ID_VIEW_HEIGHT, OnUpdateViewTranslMode)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
	ON_WM_KEYDOWN()
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWHIDEOBJECTS, OnUpdateViewShowhideobjects)
	ON_COMMAND(ID_VIEW_SHOWHIDEOBJECTS, OnViewShowhideobjects)
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_MODE_BLOCK, OnModeBlock)
	ON_UPDATE_COMMAND_UI(ID_MODE_BLOCK, OnUpdateModeBlock)
	ON_COMMAND(ID_MODE_EXTPOINT, OnModeExtpoint)
	ON_UPDATE_COMMAND_UI(ID_MODE_EXTPOINT, OnUpdateModeExtpoint)
	ON_COMMAND(ID_MODE_POINT, OnModePoint)
	ON_UPDATE_COMMAND_UI(ID_MODE_POINT, OnUpdateModePoint)
	ON_COMMAND(ID_MODE_OBJECT, OnModeObject)
	ON_UPDATE_COMMAND_UI(ID_MODE_OBJECT, OnUpdateModeObject)
	ON_COMMAND(ID_MODE_POLYGON, OnModePolygon)
	ON_UPDATE_COMMAND_UI(ID_MODE_POLYGON, OnUpdateModePolygon)
	ON_COMMAND(ID_VIEW_REFRESH, OnViewRefresh)
	ON_COMMAND(ID_VIEW_RECENTER, OnViewRecenter)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RECENTER, OnUpdateViewRecenter)
	ON_COMMAND(ID_VIEW_SHOWLANES, OnViewShowlanes)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWLANES, OnUpdateViewShowlanes)
	ON_COMMAND(ID_VIEW_SHOWTRACK, OnViewShowTrack)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWTRACK, OnUpdateViewShowTrack)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_MOVEXY, OnUpdateToolsMovexy)
	ON_COMMAND(ID_TOOLS_MOVEXY, OnToolsMovexy)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_MOVEZ, OnUpdateToolsMovez)
	ON_COMMAND(ID_TOOLS_MOVEZ, OnToolsMovez)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_MODE_SMOOTHING, OnUpdateModeSmoothing)
	ON_COMMAND(ID_MODE_SMOOTHING, OnModeSmoothing)
	ON_UPDATE_COMMAND_UI(ID_EDITMODES_EXTRASMOOTHING, OnUpdateEditmodesExtrasmoothing)
	ON_COMMAND(ID_EDITMODES_EXTRASMOOTHING, OnEditmodesExtrasmoothing)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_DELETE, OnUpdateToolsDelete)
	ON_COMMAND(ID_TOOLS_DELETE, OnToolsDelete)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_DUPLICATE, OnUpdateToolsDuplicate)
	ON_COMMAND(ID_TOOLS_DUPLICATE, OnToolsDuplicate)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_TEXTURE, OnUpdateToolsTexture)
	ON_COMMAND(ID_TOOLS_TEXTURE, OnToolsTexture)
	ON_COMMAND(ID_TOOLS_POLYFLAG, OnToolsPolyflag)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_POLYFLAG, OnUpdateToolsPolyflag)
	ON_COMMAND(ID_Tools_Neighbours, OnToolsTrackBlockProps)
	ON_COMMAND(ID_TOOLS_MERGE, OnToolsMerge)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_MERGE, OnUpdateToolsMerge)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_TOOLS_ADJUSTWIDTH, OnToolsAdjustwidth)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_ADJUSTWIDTH, OnUpdateToolsAdjustwidth)
	ON_COMMAND(ID_TOOLS_CLEARALL, OnToolsClearall)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_CLEARALL, OnUpdateToolsClearall)
	ON_COMMAND(ID_VIEW_VROAD, OnViewVroad)
	ON_UPDATE_COMMAND_UI(ID_VIEW_VROAD, OnUpdateViewVroad)
	ON_COMMAND(ID_EXPORT_VIS, OnExportVis)
	ON_COMMAND(ID_IMPORT_VIS, OnImportVis)
	ON_COMMAND(ID_COPY_Z, OnCopyZ)
	ON_COMMAND(ID_COPY_Zl, OnCOPYZl)
	ON_COMMAND(ID_Paste_Zh, OnPasteZh)
	ON_COMMAND(ID_PASTE_Zl, OnPASTEZl)
	ON_COMMAND(ID_EXPORT_TRKVERTICES, OnExportTrkvertices)
	ON_COMMAND(ID_TOOLS_PROPERTIES, OnToolsProperties)
	ON_COMMAND(ID_COPY_MODE, OnCopyMode)
	ON_COMMAND(ID_VIEW_EDITBLOCK, OnViewEditblock)
	ON_UPDATE_COMMAND_UI(ID_VIEW_EDITBLOCK, OnUpdateViewEditblock)
	ON_COMMAND(ID_VIEW_POLYFLAG, OnViewPolyflag)
	ON_UPDATE_COMMAND_UI(ID_VIEW_POLYFLAG, OnUpdateViewPolyflag)
	ON_COMMAND(ID_TOOLS_LODGEN, OnToolsLodgen)
	ON_COMMAND(ID_VIRTUALROADMASK, OnVirtualroadmask)
	ON_COMMAND(ID_VIRTUALROADEDGEMASK, OnVirtualroadedgemask)
	ON_UPDATE_COMMAND_UI(ID_VIRTUALROADMASK, OnUpdateVirtualroadmask)
	ON_UPDATE_COMMAND_UI(ID_VIRTUALROADEDGEMASK, OnUpdateVirtualroadedgemask)
	ON_COMMAND(ID_NONPASSABLEPOLYGONMASK, OnNonpassablepolygonmask)
	ON_UPDATE_COMMAND_UI(ID_NONPASSABLEPOLYGONMASK, OnUpdateNonpassablepolygonmask)
	ON_COMMAND(ID_MODE_VROADEDIT, OnModeVroadedit)
	ON_UPDATE_COMMAND_UI(ID_MODE_VROADEDIT, OnUpdateModeVroadedit)
	ON_COMMAND(ID_EXPORT_TEXTURE, OnExportTexture)
	ON_COMMAND(ID_IMPORT_TEXTURE, OnImportTexture)
	ON_COMMAND(ID_VIRROADBITMAP, OnVirroadbitmap)
	ON_UPDATE_COMMAND_UI(ID_VIRROADBITMAP, OnUpdateVirroadbitmap)
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_TOOLS_SHADOWRAYTRACER, OnToolsShadowraytracer)
	ON_COMMAND(ID_COPY_XY, OnCopyXY)
	ON_COMMAND(ID_PASTE_XY, OnPasteXY)
	ON_COMMAND(ID_EDIT_PASTE_NORMAL, OnEditPasteNormal)
	ON_COMMAND(ID_EDIT_PASTE_SHADER, OnEditPasteShader)
	ON_COMMAND(ID_EDIT_PASTE_AVERAGE, OnEditPasteAverage)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE_NORMAL, OnUpdateEditPasteNormal)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE_SHADER, OnUpdateEditPasteShader)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE_AVERAGE, OnUpdateEditPasteAverage)
	ON_UPDATE_COMMAND_UI(ID_COPY_Z, OnUpdateCopyZ)
	ON_UPDATE_COMMAND_UI(ID_Paste_Zh, OnUpdatePasteZh)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY_XY, OnUpdateEditCopyXy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE_XY, OnUpdateEditPasteXy)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_EXPAND, OnUpdateToolsExpand)
	ON_COMMAND(ID_TOOLS_EXPAND, OnToolsExpand)
	ON_UPDATE_COMMAND_UI(ID_EDITMODES_AUTOOBJMEMBER, OnUpdateEditmodesAutoobjmember)
	ON_COMMAND(ID_EDITMODES_AUTOOBJMEMBER, OnEditmodesAutoobjmember)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_ROTATE, OnUpdateToolsRotate)
	ON_COMMAND(ID_TOOLS_ROTATE, OnToolsRotate)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO_KEEP_VR, OnUpdateEditUndoKeepVr)
	ON_COMMAND(ID_EDIT_UNDO_KEEP_VR, OnEditUndoKeepVr)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_MOVETO, OnUpdateToolsMoveto)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_PROPERTIES, OnUpdateToolsProperties)
	ON_UPDATE_COMMAND_UI(ID_Tools_Neighbours, OnUpdateToolsNeighbours)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_SHADOWRAYTRACER, OnUpdateToolsShadowraytracer)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_LODGEN, OnUpdateToolsLodgen)
	ON_UPDATE_COMMAND_UI(ID_EXPORT_VIS, OnUpdateExportVis)
	ON_UPDATE_COMMAND_UI(ID_EXPORT_TRKVERTICES, OnUpdateExportTrkvertices)
	ON_UPDATE_COMMAND_UI(ID_EXPORT_TEXTURE, OnUpdateExportTexture)
	ON_UPDATE_COMMAND_UI(ID_IMPORT_VIS, OnUpdateImportVis)
	ON_UPDATE_COMMAND_UI(ID_IMPORT_TEXTURE, OnUpdateImportTexture)
	ON_COMMAND(ID_TOOLS_MOVETO, OnToolsMoveto)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE_LINE_XY, OnUpdateEditPasteLineXy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE_LINE_XYZ, OnUpdateEditPasteLineXyz)
	ON_COMMAND(ID_EDIT_PASTE_LINE_XYZ, OnEditPasteLineXyz)
	ON_COMMAND(ID_EDIT_PASTE_LINE_XY, OnEditPasteLineXy)
	ON_UPDATE_COMMAND_UI(ID_IMPORT_OBJECT, OnUpdateImportObject)
	ON_COMMAND(ID_IMPORT_OBJECT, OnImportObject)
	ON_UPDATE_COMMAND_UI(ID_EXPORT_OBJECT, OnUpdateExportObject)
	ON_COMMAND(ID_EXPORT_OBJECT, OnExportObject)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_INVERT, OnUpdateToolsInvert)
	ON_COMMAND(ID_TOOLS_INVERT, OnToolsInvert)
	ON_UPDATE_COMMAND_UI(ID_FILE_PROPERTIES, OnUpdateFileProperties)
	ON_COMMAND(ID_FILE_PROPERTIES, OnFileProperties)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_VROAD_HEIGHTS_SPDFILES, OnUpdateToolsVroadHeightsSpdfiles)
	ON_COMMAND(ID_TOOLS_VROAD_HEIGHTS_SPDFILES, OnToolsVroadHeightsSpdfiles)
	ON_COMMAND(ID_EDIT_FIND, OnEditFind)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND, OnUpdateEditFind)
	ON_UPDATE_COMMAND_UI(ID_EDIT_MERGE, OnUpdateEditMerge)
	ON_COMMAND(ID_EDIT_MERGE, OnEditMerge)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_SET_VIS, OnUpdateSetVis)
	ON_COMMAND(ID_SET_VIS, OnSetVis)
	ON_UPDATE_COMMAND_UI(ID_VIEW_VISIBILITY, OnUpdateViewVisibility)
	ON_COMMAND(ID_VIEW_VISIBILITY, OnViewVisibility)
	ON_UPDATE_COMMAND_UI(ID_VIS_FW_INCREASE, OnUpdateVisFwIncrease)
	ON_UPDATE_COMMAND_UI(ID_VIS_FW_DECREASE, OnUpdateVisFwDecrease)
	ON_UPDATE_COMMAND_UI(ID_VIS_BW_INCREASE, OnUpdateVisBwIncrease)
	ON_UPDATE_COMMAND_UI(ID_VIS_BW_DECREASE, OnUpdateVisBwDecrease)
	ON_COMMAND(ID_VIS_BW_INCREASE, OnVisBwIncrease)
	ON_COMMAND(ID_VIS_BW_DECREASE, OnVisBwDecrease)
	ON_COMMAND(ID_VIS_FW_INCREASE, OnVisFwIncrease)
	ON_COMMAND(ID_VIS_FW_DECREASE, OnVisFwDecrease)
	ON_COMMAND(ID_EDIT_COPY_XY, OnCopyXY)
	ON_COMMAND(ID_EDIT_PASTE_XY, OnPasteXY)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMIN3D, OnUpdateTrViewZoomin3d)
	ON_COMMAND(ID_VIEW_ZOOMIN3D, OnViewZoomin3d)
	ON_COMMAND(ID_VIEW_ZOOMOUT3D, OnViewZoomout3d)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMOUT3D, OnUpdateTrViewZoomout3d)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CT3EDView construction/destruction

CT3EDView::CT3EDView()
{
	darkbluePen=new CPen(PS_SOLID,1,RGB(0,0,127));
	bluePen=new CPen(PS_SOLID,1,RGB(121,112,202));
	//bluePen=new CPen(PS_SOLID,1,RGB(64,88,127));
	lightbluePen=new CPen(PS_SOLID,1,RGB(218,240,250));
	orangePen=new CPen(PS_SOLID,3,RGB(10,96,40));
	redPen=new CPen(PS_SOLID,1,RGB(255,0,0));
	darkgreenPen=new CPen(PS_SOLID,1,RGB(0,127,0));
	greenPen=new CPen(PS_SOLID,1,RGB(80,239,80));
	lightgreenPen=new CPen(PS_SOLID,1,RGB(201,250,201));
	//green2Pen=new CPen(PS_SOLID,1,RGB(32,192,32));
	//green2Pen=new CPen(PS_SOLID,1,RGB(129,40,166));
	green2Pen=new CPen(PS_SOLID,1,RGB(20,177,186));
	brownPen=new CPen(PS_SOLID,1,RGB(127,127,0));
	purplePen=new CPen(PS_SOLID,1,RGB(255,64,255));
	grayPen=new CPen (PS_SOLID,1,RGB(225,225,225));
	gray2Pen=new CPen (PS_SOLID,1,RGB(150,150,150));
	blackPen=new CPen (PS_SOLID,2,RGB(0,0,0));
	red2Pen=new CPen(PS_SOLID,2,RGB(127,0,0));
}

CT3EDView::~CT3EDView()
{
	delete orangePen;
	delete bluePen;
	delete lightbluePen;
	delete redPen;
	delete darkgreenPen;
	delete greenPen;
	delete lightgreenPen;
	delete green2Pen;
	delete brownPen;
	delete purplePen;
	delete grayPen;
	delete gray2Pen;
	delete blackPen;
	delete red2Pen;
}

BOOL CT3EDView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= WS_HSCROLL|WS_VSCROLL;
	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CT3EDView drawing


inline int round(double d)
{
 int lo = d>0 ? (int)d : (int)d-1, hi = lo+1;
 return (d-lo)<(hi-d) ? lo : hi;
}


inline int CT3EDView::FloatX(struct FLOATPT &p)
{ 
	return (int)((p.x*cosPhi-p.y*sinPhi)/scale)+dxoffs; 
}

inline int CT3EDView::FloatY(struct FLOATPT &p)
{ 
	return (int)(-(p.y*cosPhi*cosTheta+p.x*sinPhi*cosTheta+
										p.z*sinTheta)/scale)+dyoffs;
}

inline int CT3EDView::IntX(struct INTPT &p)
{ 
	return (int)((p.x*cosPhi-p.y*sinPhi)/(65536*scale))+dxoffs;
}

inline int CT3EDView::IntY(struct INTPT &p)
{ 
	return (int)(-(p.y*cosPhi*cosTheta+p.x*sinPhi*cosTheta+
						p.z*sinTheta)/(65536*scale))+dyoffs;
}

inline int CT3EDView::FloatFloatX(struct FLOATPT &p,struct FLOATPT &q)
{ 
	return (int)(((p.x+q.x)*cosPhi-(p.y+q.y)*sinPhi)/scale)+dxoffs; 
}

inline int CT3EDView::FloatFloatY(struct FLOATPT &p,struct FLOATPT &q)
{ 
	return (int)(-((p.y+q.y)*cosPhi*cosTheta+(p.x+q.x)*sinPhi*cosTheta+
									(p.z+q.z)*sinTheta)/scale)+dyoffs;
}

BOOL CT3EDView::MeetsClipRect(struct TRKBLOCK *t,LPRECT r)
{
	int x[4],y[4],zsize,i;

	for (i=0;i<=3;i++) { 
		x[i]=FloatX(t->ptBounding[i]);
		y[i]=FloatY(t->ptBounding[i]);
	}
	zsize=(int)(200*sinTheta/scale);
	if ((x[0]<r->left)&&(x[1]<r->left)&&(x[2]<r->left)&&(x[3]<r->left))
		return FALSE;
	if ((x[0]>r->right)&&(x[1]>r->right)&&(x[2]>r->right)&&(x[3]>r->right))
		return FALSE;
	if ((y[0]<r->top-zsize)&&(y[1]<r->top-zsize)&&(y[2]<r->top-zsize)&&(y[3]<r->top-zsize))
		return FALSE;
	if ((y[0]>r->bottom+zsize)&&(y[1]>r->bottom+zsize)&&(y[2]>r->bottom+zsize)&&(y[3]>r->bottom+zsize))
		return FALSE;
	return TRUE;
}

void CT3EDView::NewSel()
{
	if (refpoly!=NULL)
	{
		CSelData *sel=new CSelData;
		sel->multisel=multisel;
		sel->refblock=refblock;
		sel->refpolychunk=refpolychunk;
		sel->refpolyobj=refpolyobj;
		sel->refpolyno=refpolyno;
		sel->isxobj=isxobj;
		sel->refpoly=refpoly;
		sel->refvertices=refvertices;
		sel->refnode=refnode;
		multisel=sel;
	}
}

void CT3EDView::ShowSelectionAt(CDC *pDC,int xx,int yy)
{
	CT3EDDoc* pDoc = GetDocument();
	int j,k,l,num;
	int truedx,truedy;
	LPPOLYGONDATA p;
	struct FLOATPT *v;
	struct OBJPOLYBLOCK *o;
	struct XOBJDATA *x;
	struct INTPT pt;
	CSelData *sel;

	dxoffs=xx-(int)((refnode->x*cosPhi-refnode->y*sinPhi)/scale);
	dyoffs=yy+(int)((refnode->y*cosPhi*cosTheta+
	refnode->x*sinPhi*cosTheta+refnode->z*sinTheta)/scale);
	switch(selMode) {
		case ID_MODE_BLOCK:
			v=pDoc->trk[refblock].vert;
			p=pDoc->poly[refblock].poly[nDetail];
			num=pDoc->poly[refblock].sz[nDetail];
			for (j=0;j<num;j++,p++) {
				pDC->MoveTo(FloatX(v[p->vertex[0]]),FloatY(v[p->vertex[0]]));
				pDC->LineTo(FloatX(v[p->vertex[1]]),FloatY(v[p->vertex[1]]));
				pDC->LineTo(FloatX(v[p->vertex[2]]),FloatY(v[p->vertex[2]]));
				pDC->LineTo(FloatX(v[p->vertex[3]]),FloatY(v[p->vertex[3]]));
				pDC->LineTo(FloatX(v[p->vertex[0]]),FloatY(v[p->vertex[0]]));
			}
			break;
		case ID_MODE_EXTPOINT: // might add an extra circle ?
		case ID_MODE_POINT:
			for (sel=this;sel!=NULL;sel=sel->multisel) {
				j=FloatX(*(sel->refnode)); k=FloatY(*(sel->refnode));
				pDC->MoveTo(j-5,k);
				pDC->LineTo(j+5,k);
				pDC->MoveTo(j,k-5);
				pDC->LineTo(j,k+5);
			}
			break;
		case ID_MODE_POLYGON: 
			for (sel=this;sel!=NULL;sel=sel->multisel) {
				if (sel->refpoly==NULL) break;
				truedx=dxoffs; truedy=dyoffs;
				if (sel->isxobj) {
					dxoffs=FloatX(*(sel->refnode));
					dyoffs=FloatY(*(sel->refnode));
				}
				pDC->MoveTo(FloatX(sel->refvertices[sel->refpoly->vertex[0]]),
							FloatY(sel->refvertices[sel->refpoly->vertex[0]]));
				pDC->LineTo(FloatX(sel->refvertices[sel->refpoly->vertex[1]]),
							FloatY(sel->refvertices[sel->refpoly->vertex[1]]));
				pDC->LineTo(FloatX(sel->refvertices[sel->refpoly->vertex[2]]),
							FloatY(sel->refvertices[sel->refpoly->vertex[2]]));
				pDC->LineTo(FloatX(sel->refvertices[sel->refpoly->vertex[3]]),
							FloatY(sel->refvertices[sel->refpoly->vertex[3]]));
				pDC->LineTo(FloatX(sel->refvertices[sel->refpoly->vertex[0]]),
							FloatY(sel->refvertices[sel->refpoly->vertex[0]]));
				dxoffs=truedx; dyoffs=truedy;
			}
			break;
		case ID_MODE_OBJECT: 
		  for (sel=this;sel!=NULL;sel=sel->multisel) {
			if (sel->objno<0) break;
			//JimD: Showing Selection at VRoad Source
			if (sel->isxobj==IS_VROAD) {
				//draw refpoInts:
				pt.x=pDoc->col.vroad[sel->objno].refPt.x;
				pt.y=pDoc->col.vroad[sel->objno].refPt.y;
				pt.z=pDoc->col.vroad[sel->objno].refPt.z;
				pDC->MoveTo(IntX(pt)-5,IntY(pt)+5);
				pDC->LineTo(IntX(pt)+5,IntY(pt)-5);
				pDC->MoveTo(IntX(pt)-5,IntY(pt)-5);
				pDC->LineTo(IntX(pt)+5,IntY(pt)+5);
			} else
			//Nappe1: Showing Selection at Sound Source
			if (sel->isxobj==IS_SOUNDSRC) {
				j=(int)(1/scale);
				k=IntX(pDoc->trk[sel->refblock].soundsrc[sel->objno].refpoint);
				l=IntY(pDoc->trk[sel->refblock].soundsrc[sel->objno].refpoint);
				pDC->MoveTo(k,l);
				pDC->LineTo(k+j+2,l+j+2);
				pDC->MoveTo(k,l);
				pDC->LineTo(k+j+2,l-j-2);
				pDC->LineTo(k+j+2,l+j+2);

				
				pDC->MoveTo(k,l-j);
				pDC->LineTo(k,l+j);
				pDC->LineTo(k-j,l+j);	
				pDC->LineTo(k-j,l-j);
				pDC->LineTo(k,l-j);
			} else
			//ADDITION ENDS
			if (sel->isxobj==IS_LIGHTSRC) {
				//MessageBox("täällä!", "ALERT", MB_ICONERROR);
				j=(int)(1/scale);
				k=IntX(pDoc->trk[sel->refblock].lightsrc[sel->objno].refpoint);
				l=IntY(pDoc->trk[sel->refblock].lightsrc[sel->objno].refpoint);
				pDC->MoveTo(k-j,l-j);
				pDC->LineTo(k+j+1,l+j+1);
				pDC->MoveTo(k-j,l+j);
				pDC->LineTo(k+j+1,l-j-1);
				//MessageBox("täällä!", "ALERT", MB_ICONERROR);
			} else
			if (sel->isxobj) {
				truedx=dxoffs; truedy=dyoffs;
				x=pDoc->xobj[4*sel->refblock+sel->objchunk].obj+sel->objno;
				dxoffs=FloatX(x->ptRef); dyoffs=FloatY(x->ptRef);
				v=x->vert;
				p=x->polyData;
				for (k=0;k<x->nPolygons;k++,p++) {
					pDC->MoveTo(FloatX(v[p->vertex[0]]),FloatY(v[p->vertex[0]]));
					pDC->LineTo(FloatX(v[p->vertex[1]]),FloatY(v[p->vertex[1]]));
					pDC->LineTo(FloatX(v[p->vertex[2]]),FloatY(v[p->vertex[2]]));
					pDC->LineTo(FloatX(v[p->vertex[3]]),FloatY(v[p->vertex[3]]));
					pDC->LineTo(FloatX(v[p->vertex[0]]),FloatY(v[p->vertex[0]]));
				}
				dxoffs=truedx; dyoffs=truedy;
			} else {
				v=pDoc->trk[sel->refblock].vert;
				o=&(pDoc->poly[sel->refblock].obj[sel->objchunk]);
				p=o->poly[sel->objno];
				num=o->numpoly[sel->objno];
				for (l=0;l<num;l++,p++) {
					pDC->MoveTo(FloatX(v[p->vertex[0]]),FloatY(v[p->vertex[0]]));
					pDC->LineTo(FloatX(v[p->vertex[1]]),FloatY(v[p->vertex[1]]));
					pDC->LineTo(FloatX(v[p->vertex[2]]),FloatY(v[p->vertex[2]]));
					pDC->LineTo(FloatX(v[p->vertex[3]]),FloatY(v[p->vertex[3]]));
					pDC->LineTo(FloatX(v[p->vertex[0]]),FloatY(v[p->vertex[0]]));
					// Draw Normals:
					FLOATPT center, QuadN; 
					center.x = (v[p->vertex[0]].x + v[p->vertex[1]].x + v[p->vertex[2]].x + v[p->vertex[3]].x) / 4;
					center.y = (v[p->vertex[0]].y + v[p->vertex[1]].y + v[p->vertex[2]].y + v[p->vertex[3]].y) / 4;
					center.z = (v[p->vertex[0]].z + v[p->vertex[1]].z + v[p->vertex[2]].z + v[p->vertex[3]].z) / 4;
					
					QuadN = pDoc->QuadNormalVectorCalc(v[p->vertex[0]],v[p->vertex[1]],v[p->vertex[2]],v[p->vertex[3]]);
					QuadN.x = QuadN.x+center.x;
					QuadN.y = QuadN.y+center.y;
					QuadN.z = QuadN.z+center.z;

					pDC->MoveTo(FloatX(center),FloatY(center));
					pDC->LineTo(FloatX(QuadN),FloatY(QuadN));

				}
			}
		  }
			break;
	}
}

void CT3EDView::ShowHideCursor()
{
	CDC *dc;

	if (!isDragging) return;
	dc=GetDC();
	dc->SelectObject(redPen);
	dc->SetROP2(R2_NOTXORPEN);
	ShowSelectionAt(dc,dragX,dragY);
	ReleaseDC(dc);
}

void CT3EDView::OnDraw(CDC* pDC)
{
	CT3EDDoc* pDoc = GetDocument();
	RECT rect;
	int i,j,k=0,l,m,num;
	int truedx,truedy;
	LPPOLYGONDATA p;
	struct FLOATPT *v;
	struct OBJPOLYBLOCK *o;
	struct XOBJDATA *x;
	struct LIGHTSRC *light;
	HPEN pen, oldPen;
	HBRUSH brush, brush2, oldBrush;

	ASSERT_VALID(pDoc);
	if (pDoc->bEmpty) return;

	GetClientRect(&rect);
	dxoffs=rect.right/2-offsetx
			  -(int)((refnode->x*cosPhi-refnode->y*sinPhi)/scale);
	dyoffs=rect.bottom/2-offsety+(int)((refnode->y*cosPhi*cosTheta+
				refnode->x*sinPhi*cosTheta+refnode->z*sinTheta)/scale);
	pDC->GetClipBox(&rect);

	// remove the cursor to avoid interference
	if (isDragging) {
		pDC->SetROP2(R2_WHITE);
		ShowSelectionAt(pDC,dragX,dragY);
		pDC->SetROP2(R2_COPYPEN);
	}

	if (DrawVRoadBitMap) {
///////////////////////////////////////////////////////////////////////////////////////////
// Draw passable and driveable road map (by COL XBID15 info)
///////////////////////////////////////////////////////////////////////////////////////////

	brush = CreateSolidBrush(RGB(0,196,0));
	brush2 = CreateSolidBrush(RGB(0,127,0));
	oldBrush = (HBRUSH)::SelectObject(pDC->m_hDC, brush);

	pDC->SelectStockObject(NULL_PEN);

	for (i=0;i<pDoc->nBlocks;i++) 
	{
		if (!MeetsClipRect(&(pDoc->trk[i]),&rect)) 
			continue;

		v=pDoc->trk[i].vert;
		p=pDoc->poly[i].poly[4];

		for (int n = 0; n < pDoc->trk[i].nPositions; n++)
		{
			int nodeIdx = i*8+n;

			POSITIONDATA& posd = pDoc->trk[i].posData[n];
			COLVROAD_VIT& vr = *(COLVROAD_VIT*)&pDoc->col.vroad[nodeIdx];

			p = pDoc->poly[i].poly[4] + posd.polygon;
			num = posd.nPolygons;

			int center = pDoc->GetCenterQuad(i, n);

			short rWidth, lWidth;
			if (!pDoc->bHSMode)
			{
				lWidth = vr.scales >> 4; 
				rWidth = vr.scales & 0x0F;
			}
			else
			{
				long *tmpptr = pDoc->col.hs_extra + nodeIdx * 7;
				lWidth = (short)tmpptr[3]; 
				rWidth = (short)tmpptr[4];
			}

			p = pDoc->poly[i].poly[4] + posd.polygon;
			for (j=0; j<num; j++, p++) 
			{
				POLYVROADDATA& pvr = pDoc->trk[i].polyData[k];

				unsigned long _bit = 1L << (center - j + 8);
				if ((vr.FullMask & _bit) == 0)
					continue;

				POINT points[4];
				points[0].x = FloatX(v[p->vertex[0]]); points[0].y = FloatY(v[p->vertex[0]]);
				points[1].x = FloatX(v[p->vertex[1]]); points[1].y = FloatY(v[p->vertex[1]]);
				points[2].x = FloatX(v[p->vertex[2]]); points[2].y = FloatY(v[p->vertex[2]]);
				points[3].x = FloatX(v[p->vertex[3]]); points[3].y = FloatY(v[p->vertex[3]]);

				if (j <= center)
				{
					if (center - j < lWidth)
						::SelectObject(pDC->m_hDC, brush);
					else
						::SelectObject(pDC->m_hDC, brush2);
				}
				else
				{
					if (j - center <= rWidth)
						::SelectObject(pDC->m_hDC, brush);
					else
						::SelectObject(pDC->m_hDC, brush2);
				}

				pDC->Polygon(points, 4);
			}
		}
	}

	pDC->SelectObject(oldBrush);
	DeleteObject(brush2);
	DeleteObject(brush);


///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

	/*if (bShowLanes) {
		pDC->SelectObject(brownPen);
		for (i=0;i<pDoc->nBlocks;i++) {
			if (!MeetsClipRect(&(pDoc->trk[i]),&rect)) continue;
			v=pDoc->trk[i].vert;
			for (k=1;k<7;k++) {
				if ((k!=6)&&(k!=nDetail+1)) continue;
				p=pDoc->poly[i].poly[k];
				num=pDoc->poly[i].sz[k];
				for (j=0;j<num;j++,p++) {
					pDC->MoveTo(FloatX(v[p->vertex[0]]),FloatY(v[p->vertex[0]]));
					pDC->LineTo(FloatX(v[p->vertex[1]]),FloatY(v[p->vertex[1]]));
					pDC->LineTo(FloatX(v[p->vertex[2]]),FloatY(v[p->vertex[2]]));
					pDC->LineTo(FloatX(v[p->vertex[3]]),FloatY(v[p->vertex[3]]));
					pDC->LineTo(FloatX(v[p->vertex[0]]),FloatY(v[p->vertex[0]]));
				}
			}
			// show light sources
			light=pDoc->trk[i].lightsrc;
			m=(int)(1/scale);
			for (j=0;j<pDoc->trk[i].nLightsrc;j++,light++) {
				k=IntX(light->refpoint);
				l=IntY(light->refpoint);
				pDC->MoveTo(k-m,l-m);
				pDC->LineTo(k+m+1,l+m+1);
				pDC->MoveTo(k-m,l+m);
				pDC->LineTo(k+m+1,l-m-1);
			}
		}
	}

	pDC->SelectStockObject(BLACK_PEN);
	for (i=0;i<pDoc->nBlocks;i++) {
		if (!MeetsClipRect(&(pDoc->trk[i]),&rect)) continue;
		v=pDoc->trk[i].vert;
		p=pDoc->poly[i].poly[nDetail];
		num=pDoc->poly[i].sz[nDetail];
		for (j=0;j<num;j++,p++) {
			pDC->MoveTo(FloatX(v[p->vertex[0]]),FloatY(v[p->vertex[0]]));
			pDC->LineTo(FloatX(v[p->vertex[1]]),FloatY(v[p->vertex[1]]));
			pDC->LineTo(FloatX(v[p->vertex[2]]),FloatY(v[p->vertex[2]]));
			pDC->LineTo(FloatX(v[p->vertex[3]]),FloatY(v[p->vertex[3]]));
			pDC->LineTo(FloatX(v[p->vertex[0]]),FloatY(v[p->vertex[0]]));
		}
	}*/


///////////////////////////////////////////////////////////////////////////////////////////
// Draw center line
///////////////////////////////////////////////////////////////////////////////////////////

	pen = CreatePen(PS_SOLID, 5, RGB(196,0,0));
	oldPen = (HPEN)::SelectObject(pDC->m_hDC, pen);

	COLVROAD_VIT& _ovr = *(COLVROAD_VIT*)&pDoc->col.vroad[pDoc->col.vroadHead.nrec-1];

	int _ox = IntX(_ovr.refPt);
	int _oy = IntY(_ovr.refPt);
	pDC->MoveTo(_ox, _oy);

	for (i=0;i<pDoc->col.vroadHead.nrec;i++) 
	{
		int x = IntX(pDoc->col.vroad[i].refPt);
		int y = IntY(pDoc->col.vroad[i].refPt);
		pDC->LineTo(x, y);
	}

	pDC->SelectObject(oldPen);
	DeleteObject(pen);


///////////////////////////////////////////////////////////////////////////////////////////
// Draw vroad borders
///////////////////////////////////////////////////////////////////////////////////////////

	pen = CreatePen(PS_SOLID, 5, RGB(196,0,196));
	oldPen = (HPEN)::SelectObject(pDC->m_hDC, pen);

	CFltVector RefPt, Right, res;

	RefPt = CFltVector(_ovr.refPt.x / 65536.0f, _ovr.refPt.z / 65536.0f, _ovr.refPt.y / 65536.0f);
	Right = CFltVector(_ovr.right.x / 128.0f, _ovr.right.z / 128.0f, _ovr.right.y / 128.0f);
	Right.Normalize();
	res = RefPt - Right * (_ovr.leftWall / 65536.0f);

	_ox = FloatX(*(FLOATPT*)&res);
	_oy = FloatY(*(FLOATPT*)&res);
	pDC->MoveTo(_ox, _oy);

	for (i=0;i<pDoc->col.vroadHead.nrec;i++) 
	{
		COLVROAD_VIT& vr = *(COLVROAD_VIT*)&pDoc->col.vroad[i];
		RefPt = CFltVector(vr.refPt.x / 65536.0f, vr.refPt.z / 65536.0f, vr.refPt.y / 65536.0f);
		Right = CFltVector(vr.right.x / 128.0f, vr.right.z / 128.0f, vr.right.y / 128.0f);
		Right.Normalize();
		res = RefPt - Right * (vr.leftWall / 65536.0f);

		int x = FloatX(*(FLOATPT*)&res);
		int y = FloatY(*(FLOATPT*)&res);
		pDC->LineTo(x, y);
	}

	RefPt = CFltVector(_ovr.refPt.x / 65536.0f, _ovr.refPt.z / 65536.0f, _ovr.refPt.y / 65536.0f);
	Right = CFltVector(_ovr.right.x / 128.0f, _ovr.right.z / 128.0f, _ovr.right.y / 128.0f);
	Right.Normalize();
	res = RefPt + Right * (_ovr.rightWall / 65536.0f);

	_ox = FloatX(*(FLOATPT*)&res);
	_oy = FloatY(*(FLOATPT*)&res);
	pDC->MoveTo(_ox, _oy);

	for (i=0;i<pDoc->col.vroadHead.nrec;i++) 
	{
		COLVROAD_VIT& vr = *(COLVROAD_VIT*)&pDoc->col.vroad[i];
		RefPt = CFltVector(vr.refPt.x / 65536.0f, vr.refPt.z / 65536.0f, vr.refPt.y / 65536.0f);
		Right = CFltVector(vr.right.x / 128.0f, vr.right.z / 128.0f, vr.right.y / 128.0f);
		Right.Normalize();
		res = RefPt + Right * (vr.rightWall / 65536.0f);

		int x = FloatX(*(FLOATPT*)&res);
		int y = FloatY(*(FLOATPT*)&res);
		pDC->LineTo(x, y);
	}

	pDC->SelectObject(oldPen);
	DeleteObject(pen);


///////////////////////////////////////////////////////////////////////////////////////////
// Draw lane centers
///////////////////////////////////////////////////////////////////////////////////////////

	pen = CreatePen(PS_SOLID, 2, RGB(0,196,196));
	oldPen = (HPEN)::SelectObject(pDC->m_hDC, pen);

	if (pDoc->bHSMode)
	{
		RefPt = CFltVector(_ovr.refPt.x / 65536.0f, _ovr.refPt.z / 65536.0f, _ovr.refPt.y / 65536.0f);
		Right = CFltVector(_ovr.right.x / 128.0f, _ovr.right.z / 128.0f, _ovr.right.y / 128.0f);
		float *delta = (float*)pDoc->col.hs_extra + (pDoc->col.vroadHead.nrec-1) * 7 + 0;
		float d;

		d = *delta * ((long*)(delta))[3];
		res = RefPt - Right * d;

		_ox = FloatX(*(FLOATPT*)&res);
		_oy = FloatY(*(FLOATPT*)&res);
		pDC->MoveTo(_ox, _oy);

		delta = (float*)pDoc->col.hs_extra + 0;
		for (i=0;i<pDoc->col.vroadHead.nrec;i++, delta += 7) 
		{
			COLVROAD_VIT& vr = *(COLVROAD_VIT*)&pDoc->col.vroad[i];

			RefPt = CFltVector(vr.refPt.x / 65536.0f, vr.refPt.z / 65536.0f, vr.refPt.y / 65536.0f);
			Right = CFltVector(vr.right.x / 128.0f, vr.right.z / 128.0f, vr.right.y / 128.0f);
			d = *delta * ((long*)(delta))[3];

			res = RefPt - Right * d;

			int x = FloatX(*(FLOATPT*)&res);
			int y = FloatY(*(FLOATPT*)&res);
			pDC->LineTo(x, y);
		}

		RefPt = CFltVector(_ovr.refPt.x / 65536.0f, _ovr.refPt.z / 65536.0f, _ovr.refPt.y / 65536.0f);
		Right = CFltVector(_ovr.right.x / 128.0f, _ovr.right.z / 128.0f, _ovr.right.y / 128.0f);
		delta = (float*)pDoc->col.hs_extra + (pDoc->col.vroadHead.nrec-1) * 7 + 1;
		d = *delta * ((long*)(delta))[3];

		res = RefPt + Right * d;

		_ox = FloatX(*(FLOATPT*)&res);
		_oy = FloatY(*(FLOATPT*)&res);
		pDC->MoveTo(_ox, _oy);

		delta = (float*)pDoc->col.hs_extra + 1;
		for (i=0;i<pDoc->col.vroadHead.nrec;i++, delta += 7) 
		{
			COLVROAD_VIT& vr = *(COLVROAD_VIT*)&pDoc->col.vroad[i];

			RefPt = CFltVector(vr.refPt.x / 65536.0f, vr.refPt.z / 65536.0f, vr.refPt.y / 65536.0f);
			Right = CFltVector(vr.right.x / 128.0f, vr.right.z / 128.0f, vr.right.y / 128.0f);
			d = *delta * ((long*)(delta))[3];

			res = RefPt + Right * d;

			int x = FloatX(*(FLOATPT*)&res);
			int y = FloatY(*(FLOATPT*)&res);
			pDC->LineTo(x, y);
		}
	}
	else
	{
		RefPt = CFltVector(_ovr.refPt.x / 65536.0f, _ovr.refPt.z / 65536.0f, _ovr.refPt.y / 65536.0f);
		Right = CFltVector(_ovr.right.x / 128.0f, _ovr.right.z / 128.0f, _ovr.right.y / 128.0f);

		short lWidth = _ovr.scales >> 4; 
		res = RefPt - Right * _ovr.LeftCenter / 2 * lWidth;

		_ox = FloatX(*(FLOATPT*)&res);
		_oy = FloatY(*(FLOATPT*)&res);
		pDC->MoveTo(_ox, _oy);

		for (i=0;i<pDoc->col.vroadHead.nrec;i++) 
		{
			COLVROAD_VIT& vr = *(COLVROAD_VIT*)&pDoc->col.vroad[i];

			RefPt = CFltVector(vr.refPt.x / 65536.0f, vr.refPt.z / 65536.0f, vr.refPt.y / 65536.0f);
			Right = CFltVector(vr.right.x / 128.0f, vr.right.z / 128.0f, vr.right.y / 128.0f);

			lWidth = vr.scales >> 4; 
			res = RefPt - Right * vr.LeftCenter / 2 * lWidth;

			int x = FloatX(*(FLOATPT*)&res);
			int y = FloatY(*(FLOATPT*)&res);
			pDC->LineTo(x, y);
		}

		RefPt = CFltVector(_ovr.refPt.x / 65536.0f, _ovr.refPt.z / 65536.0f, _ovr.refPt.y / 65536.0f);
		Right = CFltVector(_ovr.right.x / 128.0f, _ovr.right.z / 128.0f, _ovr.right.y / 128.0f);

		short rWidth = _ovr.scales & 0x0F;

		res = RefPt + Right * _ovr.RightCenter / 2 * rWidth;

		_ox = FloatX(*(FLOATPT*)&res);
		_oy = FloatY(*(FLOATPT*)&res);
		pDC->MoveTo(_ox, _oy);

		for (i=0;i<pDoc->col.vroadHead.nrec;i++) 
		{
			COLVROAD_VIT& vr = *(COLVROAD_VIT*)&pDoc->col.vroad[i];

			RefPt = CFltVector(vr.refPt.x / 65536.0f, vr.refPt.z / 65536.0f, vr.refPt.y / 65536.0f);
			Right = CFltVector(vr.right.x / 128.0f, vr.right.z / 128.0f, vr.right.y / 128.0f);

			rWidth = vr.scales & 0x0f; 
			res = RefPt + Right * vr.RightCenter / 2 * rWidth;

			int x = FloatX(*(FLOATPT*)&res);
			int y = FloatY(*(FLOATPT*)&res);
			pDC->LineTo(x, y);
		}
	}

	pDC->SelectObject(oldPen);
	DeleteObject(pen);
	}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

// HAVING A LOOK AT POLYGON FLAGS : impassability
/*	pDC->SelectObject(redPen);
	for (i=0;i<pDoc->nBlocks;i++) {
		if (!MeetsClipRect(&(pDoc->trk[i]),&rect)) continue;
		v=pDoc->trk[i].vert;
		p=pDoc->poly[i].poly[nDetail];
		num=pDoc->poly[i].sz[nDetail];
		for (j=0;j<num;j++,p++)
			if ((pDoc->trk[i].polyData[j].unknownFlags&0xF)%14==0) {
			 pDC->MoveTo(FloatX(v[p->vertex[0]]),FloatY(v[p->vertex[0]]));
			 pDC->LineTo(FloatX(v[p->vertex[2]]),FloatY(v[p->vertex[2]]));
			 pDC->MoveTo(FloatX(v[p->vertex[1]]),FloatY(v[p->vertex[1]]));
			 pDC->LineTo(FloatX(v[p->vertex[3]]),FloatY(v[p->vertex[3]]));
		}
	}
*/

// CHECK VROAD
/*	pDC->SelectObject(redPen);
	for (i=0;i<pDoc->nBlocks;i++) {
		if (!MeetsClipRect(&(pDoc->trk[i]),&rect)) continue;
		v=pDoc->trk[i].vert;
		p=pDoc->poly[i].poly[4];
		num=pDoc->poly[i].sz[4];
		for (j=0;j<num;j++,p++) {
			struct FLOATPT pt;
			pt.x=(v[p->vertex[0]].x+v[p->vertex[2]].x)/2;
			pt.z=(v[p->vertex[0]].z+v[p->vertex[2]].z)/2;
			pt.y=(v[p->vertex[0]].y+v[p->vertex[2]].y)/2;
			pDC->MoveTo(FloatX(pt),FloatY(pt));
			pt.x+=pDoc->trk[i].vroadData[j].xForw/6000.0;
			pt.z+=pDoc->trk[i].vroadData[j].zForw/6000.0;
			pt.y+=pDoc->trk[i].vroadData[j].yForw/6000.0;
			pDC->LineTo(FloatX(pt),FloatY(pt));
		}
	}
*/
	/*if (bShowObjects) {
		pDC->SelectObject(bluePen);
		for (i=0;i<pDoc->nBlocks;i++) {
			if (!MeetsClipRect(&(pDoc->trk[i]),&rect)) continue;
			v=pDoc->trk[i].vert;
			for (j=0;j<4;j++) {
				o=&(pDoc->poly[i].obj[j]);
				if (o->n1==0) continue;
				for (k=0;k<o->nobj;k++) {
					p=o->poly[k];
					num=o->numpoly[k];
					for (l=0;l<num;l++,p++) {
			pDC->MoveTo(FloatX(v[p->vertex[0]]),FloatY(v[p->vertex[0]]));
			pDC->LineTo(FloatX(v[p->vertex[1]]),FloatY(v[p->vertex[1]]));
			pDC->LineTo(FloatX(v[p->vertex[2]]),FloatY(v[p->vertex[2]]));
			pDC->LineTo(FloatX(v[p->vertex[3]]),FloatY(v[p->vertex[3]]));
			pDC->LineTo(FloatX(v[p->vertex[0]]),FloatY(v[p->vertex[0]]));
					}
				}
			}
		}
		pDC->SelectObject(greenPen);
		truedx=dxoffs; truedy=dyoffs;
		for (i=0;i<4*pDoc->nBlocks;i++) {
			if (!MeetsClipRect(&(pDoc->trk[i/4]),&rect)) { i=i|3; continue; }
			x=pDoc->xobj[i].obj;
			for (j=0;j<pDoc->xobj[i].nobj;j++,x++) {
				dxoffs=FloatX(x->ptRef); dyoffs=FloatY(x->ptRef);
				v=x->vert;
				p=x->polyData;
				for (k=0;k<x->nPolygons;k++,p++) {
			pDC->MoveTo(FloatX(v[p->vertex[0]]),FloatY(v[p->vertex[0]]));
			pDC->LineTo(FloatX(v[p->vertex[1]]),FloatY(v[p->vertex[1]]));
			pDC->LineTo(FloatX(v[p->vertex[2]]),FloatY(v[p->vertex[2]]));
			pDC->LineTo(FloatX(v[p->vertex[3]]),FloatY(v[p->vertex[3]]));
			pDC->LineTo(FloatX(v[p->vertex[0]]),FloatY(v[p->vertex[0]]));
				}
				dxoffs=truedx; dyoffs=truedy;
			}
		}
	}*/

/* a glimpse at the vroad...
	pDC->SelectObject(greenPen);
	for (i=0;i<pDoc->col.vroadHead.nrec;i++) {
		struct INTPT pt;
		pt.x=pDoc->col.vroad[i].refPt.x-pDoc->col.vroad[i].right.x*(pDoc->col.vroad[i].leftWall/128);
		pt.y=pDoc->col.vroad[i].refPt.y-pDoc->col.vroad[i].right.y*(pDoc->col.vroad[i].leftWall/128);
		pt.z=pDoc->col.vroad[i].refPt.z-pDoc->col.vroad[i].right.z*(pDoc->col.vroad[i].leftWall/128);
		pDC->MoveTo(IntX(pt),IntY(pt));
		pt.x=pDoc->col.vroad[i].refPt.x+pDoc->col.vroad[i].right.x*(pDoc->col.vroad[i].rightWall/128);
		pt.y=pDoc->col.vroad[i].refPt.y+pDoc->col.vroad[i].right.y*(pDoc->col.vroad[i].rightWall/128);
		pt.z=pDoc->col.vroad[i].refPt.z+pDoc->col.vroad[i].right.z*(pDoc->col.vroad[i].rightWall/128);
		pDC->LineTo(IntX(pt),IntY(pt));
	}
*/
/* or at the bounding rectangles...
	pDC->SelectObject(brownPen);
	for (i=0;i<pDoc->nBlocks;i++) {
		// pDoc->RecalcBoundingBox(i);
		pDC->MoveTo(FloatX(pDoc->trk[i].ptBounding[0]),FloatY(pDoc->trk[i].ptBounding[0]));
		pDC->LineTo(FloatX(pDoc->trk[i].ptBounding[1]),FloatY(pDoc->trk[i].ptBounding[1]));
		pDC->LineTo(FloatX(pDoc->trk[i].ptBounding[2]),FloatY(pDoc->trk[i].ptBounding[2]));
		pDC->LineTo(FloatX(pDoc->trk[i].ptBounding[3]),FloatY(pDoc->trk[i].ptBounding[3]));
		pDC->LineTo(FloatX(pDoc->trk[i].ptBounding[0]),FloatY(pDoc->trk[i].ptBounding[0]));
	}
	pDC->SelectObject(redPen);
	i=refblock;
	pDC->MoveTo(FloatX(pDoc->trk[i].ptBounding[0]),FloatY(pDoc->trk[i].ptBounding[0]));
	pDC->LineTo(FloatX(pDoc->trk[i].ptBounding[1]),FloatY(pDoc->trk[i].ptBounding[1]));
	pDC->LineTo(FloatX(pDoc->trk[i].ptBounding[2]),FloatY(pDoc->trk[i].ptBounding[2]));
	pDC->LineTo(FloatX(pDoc->trk[i].ptBounding[3]),FloatY(pDoc->trk[i].ptBounding[3]));
	pDC->LineTo(FloatX(pDoc->trk[i].ptBounding[0]),FloatY(pDoc->trk[i].ptBounding[0]));
*/
/* or at the first polyobj chunk refpositions
	pDC->SelectObject(brownPen);
	for (i=0;i<pDoc->nBlocks;i++)
		for (j=0;j<pDoc->trk[i].nPolyobj;j++) {
			pDC->MoveTo(IntX(pDoc->trk[i].polyobj[j]->pt)-5,IntY(pDoc->trk[i].polyobj[j]->pt));
			pDC->LineTo(IntX(pDoc->trk[i].polyobj[j]->pt)+5,IntY(pDoc->trk[i].polyobj[j]->pt));
			pDC->MoveTo(IntX(pDoc->trk[i].polyobj[j]->pt),IntY(pDoc->trk[i].polyobj[j]->pt)-5);
			pDC->LineTo(IntX(pDoc->trk[i].polyobj[j]->pt),IntY(pDoc->trk[i].polyobj[j]->pt)+5);
		}	
*/
	// selection highlighting
	/*GetClientRect(&rect);
	pDC->SelectObject(redPen);
	ShowSelectionAt(pDC,rect.right/2-offsetx,rect.bottom/2-offsety);

	// dragging cursor, using redPen in xor mode
	if (isDragging) {
		pDC->SetROP2(R2_NOTXORPEN);
		ShowSelectionAt(pDC,dragX,dragY);
	}*/















/*OAOAOAOAOAOOAOOAOAOAOAOOAOAOAOAOAOAOAOOAOAOAOAOOAOAOAOAOAOOAOAOAOOAOAOAOAOOAOAOAO */



/*
	CT3EDDoc* pDoc = GetDocument();
	RECT rect;*/
	
	struct POLYVROADDATA *t;
	/*int i,j,k,l,m,num, kk;
	int truedx,truedy;
	LPPOLYGONDATA p;
	struct FLOATPT *v;
	struct OBJPOLYBLOCK *o;
	struct XOBJDATA *x;
	struct LIGHTSRC *light;*/
	struct SOUNDSRC *sound;
	//static CBitmap *G_bitmap; // = NULL;*/
	bool RoadBoundaryFlagShowed;	
	
	
/*
	
	ASSERT_VALID(pDoc);
	if (pDoc->bEmpty) return; */

	
	GetClientRect(&rect);
	dxoffs=rect.right/2-offsetx
			  -(int)((refnode->x*cosPhi-refnode->y*sinPhi)/scale);
	dyoffs=rect.bottom/2-offsety+(int)((refnode->y*cosPhi*cosTheta+
				refnode->x*sinPhi*cosTheta+refnode->z*sinTheta)/scale);
	pDC->GetClipBox(&rect);
	
	// remove the cursor to avoid interference
	if (isDragging) {
		pDC->SetROP2(R2_WHITE);
		ShowSelectionAt(pDC,dragX,dragY);
		pDC->SetROP2(R2_COPYPEN);
	}

	
	if (bShowLanes) {
		pDC->SelectObject(brownPen);
		for (i=0;i<pDoc->nBlocks;i++) {
			if (!MeetsClipRect(&(pDoc->trk[i]),&rect)) continue;
			v=pDoc->trk[i].vert;
			for (k=1;k<7;k++) {
				if ((k!=6)&&(k!=nDetail+1)) continue;
				p=pDoc->poly[i].poly[k];
				num=pDoc->poly[i].sz[k];
				for (j=0;j<num;j++,p++) {
					pDC->MoveTo(FloatX(v[p->vertex[0]]),FloatY(v[p->vertex[0]]));
					pDC->LineTo(FloatX(v[p->vertex[1]]),FloatY(v[p->vertex[1]]));
					pDC->LineTo(FloatX(v[p->vertex[2]]),FloatY(v[p->vertex[2]]));
					pDC->LineTo(FloatX(v[p->vertex[3]]),FloatY(v[p->vertex[3]]));
					pDC->LineTo(FloatX(v[p->vertex[0]]),FloatY(v[p->vertex[0]]));
				}
			}
		}
	}
	
	/*CDC memDC; 
	if (G_bitmap == NULL)
	{
		G_bitmap = new CBitmap();
		if (G_bitmap->LoadBitmap("IDB_BACKIMAGE"))
		{
			MessageBox("Failed to load bitmap", "ALERT", MB_ICONERROR);
		}	 
	}
	memDC.SelectObject(&G_bitmap);
	memDC.CreateCompatibleDC(pDC); 
	
	//CPoint point(rect.left, rect.top);
	//CSize *cSize = new CSize(rect.right,rect.bottom);
	//point.Offset(100, 100);
	
	pDC->SelectStockObject(BLACK_PEN);
	if (pDC->BitBlt(rect.left,rect.top,150,150,&memDC,0,0,SRCCOPY)==false) 
		MessageBox("täällä taas!", "ALERT", MB_ICONERROR);
		
	MessageBox("täällä!", "ALERT", MB_ICONERROR);
	//pDC->DrawState(point, *cSize, *G_bitmap, DST_BITMAP, NULL); */

	
	
	 
	for (i=0;i<pDoc->nBlocks;i++) { 
		t=pDoc->trk[i].polyData+j;
		if (!MeetsClipRect(&(pDoc->trk[i]),&rect)) continue;
		v=pDoc->trk[i].vert;
		p=pDoc->poly[i].poly[nDetail];
		num=pDoc->poly[i].sz[nDetail];
		
		for (j=0;j<num;j++,p++) {
			
			if (BlkHighlight==true)
				if (i != refblock) 
					pDC->SelectObject(grayPen);
				else pDC->SelectObject(blackPen);
			else  
				pDC->SelectStockObject(BLACK_PEN);

			if (ShowBasedOnVisi==true)
				if (i != refblock) 
					if (pDoc->IsBlockVisibile(refblock,i))
						pDC->SelectStockObject(BLACK_PEN);
					else
						//pDC->SelectObject(grayPen);
						pDC->SelectObject(gray2Pen);
				else pDC->SelectStockObject(BLACK_PEN);
			
			pDC->MoveTo(FloatX(v[p->vertex[0]]),FloatY(v[p->vertex[0]]));
			pDC->LineTo(FloatX(v[p->vertex[1]]),FloatY(v[p->vertex[1]]));
			pDC->LineTo(FloatX(v[p->vertex[2]]),FloatY(v[p->vertex[2]]));
			
			pDC->LineTo(FloatX(v[p->vertex[3]]),FloatY(v[p->vertex[3]]));
			pDC->LineTo(FloatX(v[p->vertex[0]]),FloatY(v[p->vertex[0]]));
		
		}
		v=pDoc->trk[i].vert;
		p=pDoc->poly[i].poly[nDetail];
		num=pDoc->poly[i].sz[nDetail];
		//kk=0;
		for (j=0;j<num;j++,p++) {
			t=pDoc->trk[i].polyData+j;
			if (DrawVRB == true)  RoadBoundaryFlagShowed=(bool)(t->flags & 0x80); //Nappe1: adding more felxibility to Poly color coding
			if (DrawVRE == true)  RoadBoundaryFlagShowed=(bool)(t->virtualroadedge==true);
			
			//else RoadBoundaryFlagShowed=false;
			//if ((t->flags  & 0x80) && (DrawFlags==true)) {
			if ((DrawNonPass==true) && (DrawFlags==true))
				if ((t->flags&0x0f)%14 == 0)
			{
				pDC->SelectObject(red2Pen);
				pDC->MoveTo(FloatX(v[p->vertex[0]]),FloatY(v[p->vertex[0]]));
				pDC->LineTo(FloatX(v[p->vertex[1]]),FloatY(v[p->vertex[1]]));
				pDC->LineTo(FloatX(v[p->vertex[2]]),FloatY(v[p->vertex[2]]));
				/*if (i==177) 
					MessageBox("draw stop");*/
				pDC->LineTo(FloatX(v[p->vertex[3]]),FloatY(v[p->vertex[3]])); 
				pDC->LineTo(FloatX(v[p->vertex[0]]),FloatY(v[p->vertex[0]]));
			}
			
			if ((RoadBoundaryFlagShowed==true) && (DrawFlags==true)) {
				if (DrawNonPass==true) 
					pDC->SelectObject(red2Pen);
				if (BlkHighlight==true)
					if (i==refblock)
							pDC->SelectObject(orangePen);
					else
							pDC->SelectObject(grayPen);
				else	
					pDC->SelectObject(orangePen);

				/*if (i==177) 
					MessageBox("draw stop");*/
				pDC->MoveTo(FloatX(v[p->vertex[0]]),FloatY(v[p->vertex[0]])); 
				if (t->hs_orphan[0]!=0)
					pDC->SelectObject(red2Pen);
				else
					pDC->SelectObject(orangePen);
			
				pDC->LineTo(FloatX(v[p->vertex[1]]),FloatY(v[p->vertex[1]]));
				if (t->hs_orphan[3]!=0)
					pDC->SelectObject(red2Pen);
				else
					pDC->SelectObject(orangePen);
				pDC->LineTo(FloatX(v[p->vertex[2]]),FloatY(v[p->vertex[2]]));
				/*if (i==177) 
					MessageBox("draw stop");*/
				if (t->hs_orphan[2]!=0)
					pDC->SelectObject(red2Pen);
				else
					pDC->SelectObject(orangePen);
				pDC->LineTo(FloatX(v[p->vertex[3]]),FloatY(v[p->vertex[3]]));
				if (t->hs_orphan[1]!=0)
					pDC->SelectObject(red2Pen);
				else
					pDC->SelectObject(orangePen);
				pDC->LineTo(FloatX(v[p->vertex[0]]),FloatY(v[p->vertex[0]]));
			}
		}
	}
	//DRAW Axis:
	/*struct FLOATPT Axis1;
	Axis1.x=-1000;
	Axis1.y=0;
	Axis1.z=0;
	struct FLOATPT Axis2;
	Axis2.x=1000;
	Axis2.y=0;
	Axis2.z=0;
	

	pDC->SelectObject(red2Pen);
	pDC->MoveTo(FloatX(Axis1),FloatY(Axis1));
	pDC->LineTo(FloatX(Axis2),FloatY(Axis2));
	Axis1.x=0;
	Axis1.y=-1000;
	Axis1.z=0;

	Axis2.x=0;
	Axis2.y=1000;
	Axis2.z=0;
	pDC->SelectObject(orangePen);
	pDC->MoveTo(FloatX(Axis1),FloatY(Axis1));
	pDC->LineTo(FloatX(Axis2),FloatY(Axis2));
	Axis1.x=0;
	Axis1.y=0;
	Axis1.z=-1000;

	Axis2.x=0;
	Axis2.y=0;
	Axis2.z=1000;

	pDC->SelectObject(redPen);
	pDC->MoveTo(FloatX(Axis1),FloatY(Axis1));
	pDC->LineTo(FloatX(Axis2),FloatY(Axis2));*/


// HAVING A LOOK AT POLYGON FLAGS : impassability
/*
	pDC->SelectObject(redPen);
	for (i=0;i<pDoc->nBlocks;i++) {
		if (!MeetsClipRect(&(pDoc->trk[i]),&rect)) continue;
		v=pDoc->trk[i].vert;
		p=pDoc->poly[i].poly[nDetail];
		num=pDoc->poly[i].sz[nDetail];
		for (j=0;j<num;j++,p++)
			if ((pDoc->trk[i].polyData[j].unknownFlags&0xF)%14==0) {
			 pDC->MoveTo(FloatX(v[p->vertex[0]]),FloatY(v[p->vertex[0]]));
			 pDC->LineTo(FloatX(v[p->vertex[2]]),FloatY(v[p->vertex[2]]));
			 pDC->MoveTo(FloatX(v[p->vertex[1]]),FloatY(v[p->vertex[1]]));
			 pDC->LineTo(FloatX(v[p->vertex[3]]),FloatY(v[p->vertex[3]]));
		}
	}
*/

// CHECK VROAD
/*
	pDC->SelectObject(redPen);
	for (i=0;i<pDoc->nBlocks;i++) {
		if (!MeetsClipRect(&(pDoc->trk[i]),&rect)) continue;
		v=pDoc->trk[i].vert;
		p=pDoc->poly[i].poly[4];
		num=pDoc->poly[i].sz[4];
		for (j=0;j<num;j++,p++) {
			struct FLOATPT pt;
			pt.x=(v[p->vertex[0]].x+v[p->vertex[2]].x)/2;
			pt.z=(v[p->vertex[0]].z+v[p->vertex[2]].z)/2;
			pt.y=(v[p->vertex[0]].y+v[p->vertex[2]].y)/2;
			pDC->MoveTo(FloatX(pt),FloatY(pt));
			pt.x+=pDoc->trk[i].vroadData[j].xForw/6000.0;
			pt.z+=pDoc->trk[i].vroadData[j].zForw/6000.0;
			pt.y+=pDoc->trk[i].vroadData[j].yForw/6000.0;
			pDC->LineTo(FloatX(pt),FloatY(pt));
		}
	}
*/

	if (bShowObjects) {
		pDC->SelectObject(brownPen);
		for (i=0;i<pDoc->nBlocks;i++) {
			if (!MeetsClipRect(&(pDoc->trk[i]),&rect)) continue;
			if ((ShowBasedOnVisi==true)&&(!pDoc->IsBlockVisibile(refblock,i))) continue; // Don't draw if in "Show Based On Visibility" mode, if not visible
			// show light sources
			light=pDoc->trk[i].lightsrc;
			m=(int)(1/scale);
			pDC->SelectObject(brownPen);
			for (j=0;j<pDoc->trk[i].nLightsrc;j++,light++) {
				k=IntX(light->refpoint);
				l=IntY(light->refpoint);
				pDC->MoveTo(k-m,l-m);
				pDC->LineTo(k+m+1,l+m+1);
				pDC->MoveTo(k-m,l+m);
				pDC->LineTo(k+m+1,l-m-1);
			}
				// Nappe1: show sound sources
			pDC->SelectObject(purplePen);
			sound=pDoc->trk[i].soundsrc;
			m=(int)(1/scale);
			for (j=0;j<pDoc->trk[i].nSoundsrc;j++,sound++) {
				k=IntX(sound->refpoint);
				l=IntY(sound->refpoint);
				/*
				pDC->MoveTo(k-m-1,l-m-1);
				pDC->LineTo(k+m+1,l+m+1);
				pDC->MoveTo(k-m,l+m);
				pDC->LineTo(k+m+2,l-m-2);
			//lisaa
				pDC->MoveTo(k+m+2,l-m-2);
				pDC->LineTo(k-m-1,l-m-1); 
			//ruksi & boksi
				pDC->MoveTo(k-m,l-m);
				pDC->LineTo(k+m+1,l+m+1);
				pDC->LineTo(k-m,l+m);
				pDC->LineTo(k+m+1,l-m-1);
				pDC->LineTo(k-m,l-m); */
				
				pDC->MoveTo(k,l);
				pDC->LineTo(k+m+2,l+m+2);
				pDC->MoveTo(k,l);
				pDC->LineTo(k+m+2,l-m-2);
				pDC->LineTo(k+m+2,l+m+2);

				
				pDC->MoveTo(k,l-m);
				pDC->LineTo(k,l+m);
				pDC->LineTo(k-m,l+m);	
				pDC->LineTo(k-m,l-m);
				pDC->LineTo(k,l-m);
			}
		}
		//pDC->SelectObject(bluePen);
		for (i=0;i<pDoc->nBlocks;i++) {
			if (ShowBasedOnVisi==true)
				if (i==refblock)
					pDC->SelectObject(bluePen); // Object color for selected block
				else
					if (pDoc->IsBlockVisibile(refblock,i))
						pDC->SelectObject(darkbluePen); // Object color for visbile not not selected blocks
					else
						continue; //Don't draw objects on non visible blocks. Old: pDC->SelectObject(lightbluePen);
			else pDC->SelectObject(darkbluePen);

			if (!MeetsClipRect(&(pDoc->trk[i]),&rect)) continue;
			v=pDoc->trk[i].vert;
			for (j=0;j<4;j++) {
				o=&(pDoc->poly[i].obj[j]);
				if (o->n1==0) continue;
				for (k=0;k<o->nobj;k++) {
					p=o->poly[k];
					num=o->numpoly[k];
					for (l=0;l<num;l++,p++) {
			pDC->MoveTo(FloatX(v[p->vertex[0]]),FloatY(v[p->vertex[0]]));
			pDC->LineTo(FloatX(v[p->vertex[1]]),FloatY(v[p->vertex[1]]));
			pDC->LineTo(FloatX(v[p->vertex[2]]),FloatY(v[p->vertex[2]]));
			pDC->LineTo(FloatX(v[p->vertex[3]]),FloatY(v[p->vertex[3]]));
			pDC->LineTo(FloatX(v[p->vertex[0]]),FloatY(v[p->vertex[0]]));
					}
				}
			}
		}
		
		 // Draw XOBJ
		//pDC->SelectObject(greenPen);

		truedx=dxoffs; truedy=dyoffs;
		for (i=0;i<4*pDoc->nBlocks;i++) {
			if (ShowBasedOnVisi==true)
				if (int(i/4)==refblock)
					pDC->SelectObject(greenPen); // Color for extra objects on selected block
				else
					if (pDoc->IsBlockVisibile(refblock,int(i/4)))// IsBlockVisibile(int(i/4)))
						pDC->SelectObject(darkgreenPen); //Extra object color for visbile but not selected blocks
					else
						continue; //Don't draw objects on non visible blocks. Old: pDC->SelectObject(lightgreenPen);
			else pDC->SelectObject(darkgreenPen);

			if (!MeetsClipRect(&(pDoc->trk[i/4]),&rect)) { i=i|3; continue; }
			x=pDoc->xobj[i].obj;
			for (j=0;j<pDoc->xobj[i].nobj;j++,x++) {
				dxoffs=FloatX(x->ptRef); dyoffs=FloatY(x->ptRef);
				v=x->vert;
				p=x->polyData;
				for (k=0;k<x->nPolygons;k++,p++) {
			pDC->MoveTo(FloatX(v[p->vertex[0]]),FloatY(v[p->vertex[0]]));
			pDC->LineTo(FloatX(v[p->vertex[1]]),FloatY(v[p->vertex[1]]));
			pDC->LineTo(FloatX(v[p->vertex[2]]),FloatY(v[p->vertex[2]]));
			pDC->LineTo(FloatX(v[p->vertex[3]]),FloatY(v[p->vertex[3]]));
			pDC->LineTo(FloatX(v[p->vertex[0]]),FloatY(v[p->vertex[0]]));
				}
				dxoffs=truedx; dyoffs=truedy;
			}
		}
	}

	// JimD : Draw global xobj
	pDC->SelectObject(green2Pen);
	struct XOBJBLOCK *xb;
	xb=&(pDoc->xobj[4*pDoc->nBlocks]);
	if (xb->nobj>0)
	{
	  truedx=dxoffs; truedy=dyoffs;
	  x=xb->obj;
	  for (j=0;j<xb->nobj;j++,x++) 
	  {
		if (j>0) {
			v=x->vert;
		  }
		dxoffs=truedx; dyoffs=truedy;
		dxoffs=FloatX(x->ptRef); dyoffs=FloatY(x->ptRef);
		v=x->vert;
		p=x->polyData;
		for (k=0;k<x->nPolygons;k++,p++) 
		{
			pDC->MoveTo(FloatX(v[p->vertex[0]]),FloatY(v[p->vertex[0]]));
			pDC->LineTo(FloatX(v[p->vertex[1]]),FloatY(v[p->vertex[1]]));
			pDC->LineTo(FloatX(v[p->vertex[2]]),FloatY(v[p->vertex[2]]));
			pDC->LineTo(FloatX(v[p->vertex[3]]),FloatY(v[p->vertex[3]]));
			pDC->LineTo(FloatX(v[p->vertex[0]]),FloatY(v[p->vertex[0]]));
		}
	  }
	  dxoffs=truedx; dyoffs=truedy;
	}

// a glimpse at the vroad...
//Nappe1: added option to show virtual road (Denis  originally used this when 
//		  he was beta testing HS File Formats.)
//begin
	if (DrawVRoad==TRUE) 
	{
		//Get Original dx/dy-offs again
		/*dxoffs=rect.right/2-offsetx
			  -(int)((refnode->x*cosPhi-refnode->y*sinPhi)/scale);
		dyoffs=rect.bottom/2-offsety+(int)((refnode->y*cosPhi*cosTheta+
				refnode->x*sinPhi*cosTheta+refnode->z*sinTheta)/scale);*/
		for (i=0;i<pDoc->col.vroadHead.nrec;i++) {	
			struct INTPT pt, pt2; //, ptl;
			pDC->SelectObject(purplePen);
			pt.x=pDoc->col.vroad[i].refPt.x-pDoc->col.vroad[i].right.x*(pDoc->col.vroad[i].leftWall/128);
			pt.y=pDoc->col.vroad[i].refPt.y-pDoc->col.vroad[i].right.y*(pDoc->col.vroad[i].leftWall/128);
			pt.z=pDoc->col.vroad[i].refPt.z-pDoc->col.vroad[i].right.z*(pDoc->col.vroad[i].leftWall/128);
			pDC->MoveTo(IntX(pt),IntY(pt));
			pt2.x=pDoc->col.vroad[i].refPt.x+pDoc->col.vroad[i].right.x*(pDoc->col.vroad[i].rightWall/128);
			pt2.y=pDoc->col.vroad[i].refPt.y+pDoc->col.vroad[i].right.y*(pDoc->col.vroad[i].rightWall/128);
			pt2.z=pDoc->col.vroad[i].refPt.z+pDoc->col.vroad[i].right.z*(pDoc->col.vroad[i].rightWall/128);
			pDC->LineTo(IntX(pt2),IntY(pt2));
			/*dldata = (float) DL[i]/255.0f;
			

			fakefloatcoord.x=(pt.x) + ((pt2.x-pt.x)  * dldata);
			fakefloatcoord.y=(pt.y) + ((pt2.y-pt.y) * dldata);
			fakefloatcoord.z=(pt.z) + ((pt2.z-pt.z) * dldata);
			
			ptl.x = round(fakefloatcoord.x);
			ptl.y = round(fakefloatcoord.y);
			ptl.z = round(fakefloatcoord.z);
			
			pDC->SelectObject(red2Pen);
			pDC->MoveTo(IntX(ptl)-5,IntY(ptl)+5);
			pDC->LineTo(IntX(ptl)+5,IntY(ptl)-5);
			pDC->MoveTo(IntX(ptl)-5,IntY(ptl)-5);
			pDC->LineTo(IntX(ptl)+5,IntY(ptl)+5);*/

			//draw driving line poInts (right):
			/*pt2.x=pDoc->col.vroad[i].refPt.x-pDoc->col.vroad[i].right.x*(pDoc->col.vroad[i].leftWall/128);
			pt2.y=pDoc->col.vroad[i].refPt.y-pDoc->col.vroad[i].right.y*(pDoc->col.vroad[i].leftWall/128);
			pt2.z=pDoc->col.vroad[i].refPt.z-pDoc->col.vroad[i].right.z*(pDoc->col.vroad[i].leftWall/128);*/
			
			//draw refpoInts:
			pDC->SelectObject(brownPen);	
			pt.x=pDoc->col.vroad[i].refPt.x;
			pt.y=pDoc->col.vroad[i].refPt.y;
			pt.z=pDoc->col.vroad[i].refPt.z;
			pDC->MoveTo(IntX(pt)-5,IntY(pt)+5);
			pDC->LineTo(IntX(pt)+5,IntY(pt)-5);
			pDC->MoveTo(IntX(pt)-5,IntY(pt)-5);
			pDC->LineTo(IntX(pt)+5,IntY(pt)+5);
			

		}
	}
//end
// or at the bounding rectangles...
/*	pDC->SelectObject(brownPen);
	for (i=0;i<pDoc->nBlocks;i++) {
		// pDoc->RecalcBoundingBox(i);
		pDC->MoveTo(FloatX(pDoc->trk[i].ptBounding[0]),FloatY(pDoc->trk[i].ptBounding[0]));
		pDC->LineTo(FloatX(pDoc->trk[i].ptBounding[1]),FloatY(pDoc->trk[i].ptBounding[1]));
		pDC->LineTo(FloatX(pDoc->trk[i].ptBounding[2]),FloatY(pDoc->trk[i].ptBounding[2]));
		pDC->LineTo(FloatX(pDoc->trk[i].ptBounding[3]),FloatY(pDoc->trk[i].ptBounding[3]));
		pDC->LineTo(FloatX(pDoc->trk[i].ptBounding[0]),FloatY(pDoc->trk[i].ptBounding[0]));
	}
	pDC->SelectObject(redPen);
	i=refblock;
	pDC->MoveTo(FloatX(pDoc->trk[i].ptBounding[0]),FloatY(pDoc->trk[i].ptBounding[0]));
	pDC->LineTo(FloatX(pDoc->trk[i].ptBounding[1]),FloatY(pDoc->trk[i].ptBounding[1]));
	pDC->LineTo(FloatX(pDoc->trk[i].ptBounding[2]),FloatY(pDoc->trk[i].ptBounding[2]));
	pDC->LineTo(FloatX(pDoc->trk[i].ptBounding[3]),FloatY(pDoc->trk[i].ptBounding[3]));
	pDC->LineTo(FloatX(pDoc->trk[i].ptBounding[0]),FloatY(pDoc->trk[i].ptBounding[0]));
*/
// or at the first polyobj chunk refpositions
/*	pDC->SelectObject(brownPen);
	for (i=0;i<pDoc->nBlocks;i++)
		for (j=0;j<pDoc->trk[i].nPolyobj;j++) {
			pDC->MoveTo(IntX(pDoc->trk[i].polyobj[j]->pt)-5,IntY(pDoc->trk[i].polyobj[j]->pt));
			pDC->LineTo(IntX(pDoc->trk[i].polyobj[j]->pt)+5,IntY(pDoc->trk[i].polyobj[j]->pt));
			pDC->MoveTo(IntX(pDoc->trk[i].polyobj[j]->pt),IntY(pDoc->trk[i].polyobj[j]->pt)-5);
			pDC->LineTo(IntX(pDoc->trk[i].polyobj[j]->pt),IntY(pDoc->trk[i].polyobj[j]->pt)+5);
		}	
*/
	// selection highlighting
	GetClientRect(&rect);		
	pDC->SelectObject(redPen);
	ShowSelectionAt(pDC,rect.right/2-offsetx,rect.bottom/2-offsety);

	// dragging cursor, using redPen in xor mode
	if (isDragging) {
		pDC->SetROP2(R2_NOTXORPEN);
		ShowSelectionAt(pDC,dragX,dragY);
	}

	//HOO: Main Track View interface (1)
	if (bShowTrack)
	{
		if (!pDoc->qfsView.bIsLoadQfs)
		{
			// Is texture images loaded
			if (pDoc->bCreateScene)
			{
				AfxMessageBox("Track View requires texture.");
				pDoc->TrackView->DestroyWindow();
				pDoc->bCreateScene=FALSE;
				bShowTrack=FALSE;
				return;
			}
			
			if (pDoc->qfsView.LoadQFS() != 0)
			{
				pDoc->qfsView.bIsLoadQfs=FALSE;
			}
			else
			{
				pDoc->qfsView.bIsLoadQfs=TRUE;
				pDoc->qfsView.promptLoad=FALSE;
			}

			if (!pDoc->qfsView.bIsLoadQfs)
			{
				bShowTrack=FALSE;
				return;
			}
		}
		if (!pDoc->bCreateScene)
		{
			//Initialize Track View
			if (!pDoc->TrackView->Init(GetParent(), &(pDoc->qfsView), pDoc->texture, pDoc->bHSMode))
			{
				bShowTrack=FALSE;
				pDoc->bCreateScene=FALSE;
				return;
			}
			pDoc->bCreateScene=TRUE;
			pDoc->bUpdatedScene=FALSE;
			sceneRefBlock = -1;
			pDoc->TrackView->ShowView(bShowTrack);
		}
		if (!pDoc->TrackView->bInitialized)
		{
			bShowTrack=FALSE;
			pDoc->bCreateScene=FALSE;
			return;
		}
		if (!bUpdatedRes || !pDoc->bUpdatedScene || refblock != sceneRefBlock)
		{
			// Create and update 3 blocks or more
			bUpdatedRes = TRUE;
			pDoc->bUpdatedScene=TRUE;
			m=0;
			if (ShowBasedOnVisi==true) //Show track based on visibility is enabled
			{
				TrachViewDrawBlock(pDoc->nBlocks,m);m++; //Show global objects
				TrachViewDrawBlock(refblock,m);m++; //Show selected block
				i=1;
				do 
				{
					if (pDoc->IsBlockVisibile(refblock,RealBlockNum(refblock+i))) // Is this block visible ? Then draw in preview
					{
						TrachViewDrawBlock(RealBlockNum(refblock+i),m);m++; //Show selected block
					}
					if (pDoc->IsBlockVisibile(refblock,RealBlockNum(refblock-i))) // Is this block visible ? Then draw in preview
					{
						TrachViewDrawBlock(RealBlockNum(refblock-i),m);m++; //Show selected block
					}
					i++;
				} while ( (m<29)&&(i<pDoc->nBlocks/2) );

				while ( m < 30 ) 
				{
					pDoc->TrackView->CreateEmptyScene(m);
					m++;
				}
			}
			else
			{	//Standard view
				TrachViewDrawBlock(pDoc->nBlocks,m);m++; //Show global objects
				TrachViewDrawBlock(refblock,m);m++; //Show selected block
				TrachViewDrawBlock(RealBlockNum(refblock+1),m);m++; //Show blocks after selected
				TrachViewDrawBlock(RealBlockNum(refblock+2),m);m++;
				TrachViewDrawBlock(RealBlockNum(refblock-1),m);m++; //Show blocks before selected
				TrachViewDrawBlock(RealBlockNum(refblock-2),m);m++;
				while ( m < 30 ) 
				{
					pDoc->TrackView->CreateEmptyScene(m);
					m++;
				}
			}

			sceneRefBlock = refblock;			//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		}
		// Set camera position and orientation
		if ((isxobj==1) && selMode == ID_MODE_OBJECT && objno>=0)
		{
			x = pDoc->xobj[4*refblock+objchunk].obj+objno;
			pDoc->TrackView->UpdateScene(theta, phi, 
				(int) (x->ptRef.x + 0.5*x->vert[x->polyData->vertex[0]].x
				+ 0.5*x->vert[x->polyData->vertex[2]].x),
				(int) (x->ptRef.y + 0.5*x->vert[x->polyData->vertex[0]].y
				+ 0.5*x->vert[x->polyData->vertex[2]].y),
				(int) (x->ptRef.z + 0.5*x->vert[x->polyData->vertex[0]].z
				+ 0.5*x->vert[x->polyData->vertex[2]].z),
				300.00 * scale * scale3d + 5);
		}
		else
		{
			if (selMode == ID_MODE_POLYGON && refpoly!=NULL && isxobj)
			{
				pDoc->TrackView->UpdateScene(theta, phi,
					(int) (refnode->x + 0.5*refvertices[refpoly->vertex[0]].x
					+ 0.5*refvertices[refpoly->vertex[2]].x),
					(int) (refnode->y + 0.5*refvertices[refpoly->vertex[0]].y
					+ 0.5*refvertices[refpoly->vertex[2]].y),
					(int) (refnode->z + 0.5*refvertices[refpoly->vertex[0]].z
					+ 0.5*refvertices[refpoly->vertex[2]].z), 300.00 * scale  * scale3d + 5);
			}
			else
			{
				pDoc->TrackView->UpdateScene(theta, phi, (int) refnode->x, (int) refnode->y,
					(int) refnode->z, 300.00 * scale  * scale3d + 5);
			}
		}
	}
	//HOO: (1)
}

//HOO: (2)
void CT3EDView::TrackKeyDown(UINT tmsg, BOOL bmode)
{
	CT3EDDoc* pDoc;
	bool phichange = FALSE;
	bool thetachange = FALSE;
	if (!bmode)
	{
		pDoc = GetDocument();
		CleanCursorZone(selMode,FALSE);
		selMode=ID_MODE_BLOCK;
		isTruePoint=FALSE; refpoly=NULL; objno=-1;
		editMode=NULL;
		CleanCursorZone(selMode,TRUE);
	}
	if (tmsg & 2) //left
	{
		if (bmode)
		{
			phi-=5;
			phichange = TRUE;
		}
		else
		{
			refblock = (refblock==0 ? pDoc->nBlocks-1: refblock-1);
			refnode=&(pDoc->trk[refblock].ptCentre);
			UpdateRefBlock();
		}
	}
	if (tmsg & 1)
	{
		if (bmode)
		{
			phi+=5;
			phichange = TRUE;
		}
		else
		{
			refblock = (refblock==pDoc->nBlocks-1 ? 0 : refblock+1);
			refnode=&(pDoc->trk[refblock].ptCentre);
			UpdateRefBlock();
		}
	}
	if (tmsg & 4) //up
	{
		if (bmode)
		{
			theta-=5;
			thetachange = TRUE;
		}
	}
	if (tmsg & 8)
	{
		if (bmode)
		{
			theta+=5;
			thetachange = TRUE;
		}
	}
	if (thetachange)
	{
		if (theta<0) theta=0;
		if (theta>90) theta=90;
		cosTheta=(float)cos(theta*3.14159265359/180.0);
		sinTheta=(float)sin(theta*3.14159265359/180.0);
		SetScrollPos(SB_VERT,theta,TRUE);
	}
	if (phichange)
	{
		if (phi<-180) phi+=360;
		if (phi>180) phi-=360;
		cosPhi=(float)cos(phi*3.14159265359/180.0);
		sinPhi=-(float)sin(phi*3.14159265359/180.0);
		SetScrollPos(SB_HORZ,phi,TRUE);
	}
	if (thetachange || phichange || !bmode)
	{
		InvalidateRect(NULL,TRUE);
		return;
	}
	switch(tmsg)
	{
	case 16:OnViewZoomin(); break;
	case 32:OnViewZoomout(); break;
	}
}
//HOO: (2)

/////////////////////////////////////////////////////////////////////////////
// CT3EDView diagnostics

#ifdef _DEBUG
void CT3EDView::AssertValid() const
{
	CView::AssertValid();
}

void CT3EDView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CT3EDDoc* CT3EDView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CT3EDDoc)));
	return (CT3EDDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CT3EDView message handlers

void CT3EDView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	RECT rect;
	BOOL act=TRUE,scroll=FALSE;
	int oldx;

	if (bScrollRotate) {
		switch(nSBCode) {
			case SB_LEFT: phi=-180; break;
			case SB_LINELEFT: phi-=5; break;
			case SB_LINERIGHT: phi+=5; break;
			case SB_PAGELEFT: phi-=30; break;
			case SB_PAGERIGHT: phi+=30; break;
			case SB_RIGHT: phi=180; break;
			case SB_THUMBPOSITION: phi=nPos; break;
			case SB_THUMBTRACK: phi=nPos; act=FALSE; break;
			default: return;
		}
		if (phi<-180) phi+=360;
		if (phi>180) phi-=360;
		cosPhi=(float)cos(phi*3.14159265359/180.0);
		sinPhi=-(float)sin(phi*3.14159265359/180.0);
		SetScrollPos(SB_HORZ,phi,TRUE);
	} else {
		GetClientRect(&rect);
		oldx=offsetx;
		switch(nSBCode) {
			case SB_LEFT: offsetx=-SCROLLSZ; break;
			case SB_LINELEFT: offsetx-=40; scroll=TRUE; break;
			case SB_LINERIGHT: offsetx+=40; scroll=TRUE; break;
			case SB_PAGELEFT: offsetx-=rect.right/2; scroll=TRUE; break;
			case SB_PAGERIGHT: offsetx+=rect.right/2; scroll=TRUE; break;
			case SB_RIGHT: offsetx=SCROLLSZ; break;
			case SB_THUMBPOSITION: offsetx=nPos; break;
			case SB_THUMBTRACK: offsetx=nPos; act=FALSE; break;
			default: return;
		}
		if (offsetx<-SCROLLSZ) offsetx=-SCROLLSZ;
		if (offsetx>SCROLLSZ) offsetx=SCROLLSZ;
		SetScrollPos(SB_HORZ,offsetx,TRUE);
	}
	if (act) {
		if (scroll) { ScrollWindow(oldx-offsetx,0); UpdateWindow(); }
		else InvalidateRect(NULL,TRUE);
	}
}

void CT3EDView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	RECT rect;
	BOOL act=TRUE,scroll=FALSE;
	int oldy;

	if (bScrollRotate) {
		switch(nSBCode) {
			case SB_TOP: theta=0; break;
			case SB_LINEUP: theta-=5; break;
			case SB_LINEDOWN: theta+=5; break;
			case SB_PAGEUP: theta-=15; break;
			case SB_PAGEDOWN: theta+=15; break;
			case SB_BOTTOM: theta=90; break;
			case SB_THUMBPOSITION: theta=nPos; break;
			case SB_THUMBTRACK: theta=nPos; act=FALSE; break;
			default: return;
		}
		if (theta<0) theta=0;
		if (theta>90) theta=90;
		cosTheta=(float)cos(theta*3.14159265359/180.0);
		sinTheta=(float)sin(theta*3.14159265359/180.0);
		SetScrollPos(SB_VERT,theta,TRUE);
	} else {
		GetClientRect(&rect);
		oldy=offsety;
		switch(nSBCode) {
			case SB_TOP: offsety=-SCROLLSZ; break;
			case SB_LINEUP: offsety-=40; scroll=TRUE; break;
			case SB_LINEDOWN: offsety+=40; scroll=TRUE; break;
			case SB_PAGEUP: offsety-=rect.bottom/2; scroll=TRUE; break;
			case SB_PAGEDOWN: offsety+=rect.bottom/2; scroll=TRUE; break;
			case SB_BOTTOM: offsety=SCROLLSZ; break;
			case SB_THUMBPOSITION: offsety=nPos; break;
			case SB_THUMBTRACK: offsety=nPos; act=FALSE; break;
			default: return;
		}
		if (offsety<-SCROLLSZ) offsety=-SCROLLSZ;
		if (offsety>SCROLLSZ) offsety=SCROLLSZ;
		SetScrollPos(SB_VERT,offsety,TRUE);
	}
	if (act) {
		if (scroll) { ScrollWindow(0,oldy-offsety); UpdateWindow(); }
		else InvalidateRect(NULL,TRUE);
	}
}

void CT3EDView::OnViewFullres() 
{
	if (nDetail==HIGH_DETAIL) return;
	if (multisel!=NULL) { delete multisel; multisel=NULL; }
	if ((refpoly!=NULL)&&IsPolyOfDetail(nDetail)) refpoly=NULL;
	nDetail=HIGH_DETAIL;
	if (isTruePoint&&!bShowLanes)
		if (!IsVisibleNode(refblock,refnode)) isTruePoint=FALSE;
	InvalidateRect(NULL,TRUE);
}

void CT3EDView::OnViewHalfres() 
{
	if (nDetail==MED_DETAIL) return;
	if (multisel!=NULL) { delete multisel; multisel=NULL; }
	if ((refpoly!=NULL)&&IsPolyOfDetail(nDetail)) refpoly=NULL;
	nDetail=MED_DETAIL;
	if (isTruePoint&&!bShowLanes)
		if (!IsVisibleNode(refblock,refnode)) isTruePoint=FALSE;
	InvalidateRect(NULL,TRUE);
}

void CT3EDView::OnViewLowres() 
{
	if (nDetail==LOW_DETAIL) return;
	if (multisel!=NULL) { delete multisel; multisel=NULL; }
	if ((refpoly!=NULL)&&IsPolyOfDetail(nDetail)) refpoly=NULL;
	nDetail=LOW_DETAIL;
	if (isTruePoint&&!bShowLanes)
		if (!IsVisibleNode(refblock,refnode)) isTruePoint=FALSE;
	InvalidateRect(NULL,TRUE);
}

void CT3EDView::OnViewZoomin() 
{
	if (scale<0.005) return;
	scale=scale/(float)1.5;
	offsetx=(int)1.5*offsetx;
	offsety=(int)1.5*offsety;
	if (offsetx<-SCROLLSZ) offsetx=-SCROLLSZ;
	if (offsety<-SCROLLSZ) offsety=-SCROLLSZ;
	if (offsetx>SCROLLSZ) offsetx=SCROLLSZ;
	if (offsety>SCROLLSZ) offsety=SCROLLSZ;
	InvalidateRect(NULL,TRUE);
	if (!bScrollRotate) {
		SetScrollPos(SB_HORZ,offsetx,TRUE);
		SetScrollPos(SB_VERT,offsety,TRUE);
	}
}

void CT3EDView::OnViewZoomout() 
{
	if (scale>5.0) return;
	scale=scale*(float)1.5;
	offsetx=(int)(offsetx/1.5);
	offsety=(int)(offsety/1.5);
	InvalidateRect(NULL,TRUE);
	if (!bScrollRotate) {
		SetScrollPos(SB_HORZ,offsetx,TRUE);
		SetScrollPos(SB_VERT,offsety,TRUE);
	}
}

void CT3EDView::OnViewZoomin3d()
{
	if (scale3d<0.005) return;
	scale3d=scale3d/(float)1.25;
	InvalidateRect(NULL,TRUE);
}

void CT3EDView::OnViewZoomout3d()
{
	if (scale3d>5.0) return;
	scale3d=scale3d*(float)1.25;
	InvalidateRect(NULL,TRUE);
}

void CT3EDView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();

	refblock=0;
	multisel=NULL;
	offsetx=0; offsety=0;
	theta=30; phi=0;
	cosTheta=(float)sqrt(3.0)/2; sinTheta=0.5; cosPhi=1.0; sinPhi=0.0;
	scale=1.0;scale3d=1.0;
	nDetail=HIGH_DETAIL;
	bShowObjects=TRUE; bShowLanes=TRUE;
	//HOO: Initialize (3)
	bShowTrack=FALSE;
	bUpdatedRes = TRUE;
	//HOO: (3)
	isScrolling=FALSE;
	bScrollRotate=TRUE;
	isEmpty=GetDocument()->bEmpty;
	selMode=ID_MODE_BLOCK;
	isTruePoint=FALSE; refpoly=NULL; objno=-1;
	isDragging=FALSE;
	editMode=NULL;
	smooth2Start=0; smooth2Width=25; // ~half a block
	smooth1Start=12; smooth1Width=20; // over 3 blocks
	extraSmooth=TRUE;

	SetScrollRange(SB_HORZ,-SCROLLSZ,SCROLLSZ,FALSE);
	SetScrollRange(SB_VERT,-SCROLLSZ,SCROLLSZ,FALSE);
	SetScrollPos(SB_HORZ,0,TRUE);
	SetScrollPos(SB_VERT,0,TRUE);
	statusbar=(CStatusBar *)GetParentFrame()->GetMessageBar();
	//statusbar->SetPaneInfo(4, statusbar->GetItemID(4), SBPS_STRETCH, NULL );
	statusbar->SetPaneInfo(4, statusbar->GetItemID(4), SBPS_NORMAL, 70 );
	if (isEmpty) {
		statusbar->SetPaneText(4,"Normal (z)",TRUE);
		statusbar->SetPaneText(1,"No Block",TRUE);
		statusbar->SetPaneText(2,"EMPTY",TRUE);
//		GetParentFrame()->GetMenu()->EnableMenuItem(2,MF_BYPOSITION|MF_GRAYED);
//		GetParentFrame()->GetMenu()->EnableMenuItem(3,MF_BYPOSITION|MF_GRAYED);
	} else {
		statusbar->SetPaneText(1,"Block 0",TRUE);
		statusbar->SetPaneText(2,"TRANSL",TRUE);
//		GetParentFrame()->GetMenu()->EnableMenuItem(2,MF_BYPOSITION|MF_ENABLED);
//		GetParentFrame()->GetMenu()->EnableMenuItem(3,MF_BYPOSITION|MF_ENABLED);
		refnode=&(GetDocument()->trk[refblock].ptCentre);
	}
}

void CT3EDView::OnUpdateViewFullres(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!isEmpty);
	pCmdUI->SetCheck((nDetail==HIGH_DETAIL)&&!isEmpty);
}

void CT3EDView::OnUpdateViewHalfres(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!isEmpty);
	pCmdUI->SetCheck((nDetail==MED_DETAIL)&&!isEmpty);
}

void CT3EDView::OnUpdateViewLowres(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!isEmpty);
	pCmdUI->SetCheck((nDetail==LOW_DETAIL)&&!isEmpty);
}

void CT3EDView::OnUpdateViewZoomin(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable((scale>=0.005)&&!isEmpty);
}

void CT3EDView::OnUpdateViewZoomout(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable((scale<=5.0)&&!isEmpty);	
}

void CT3EDView::UpdateRefBlock()
{
	CT3EDDoc *pDoc=GetDocument();
	struct TRKBLOCK *b;
	CSelData *sel;
	CString s;
	s.Format("Block %d",refblock);
	if (refblock==pDoc->nBlocks) s="Global";
	statusbar->SetPaneText(1,s,TRUE);

	if (selMode==ID_MODE_OBJECT) //JimD: Show objects middle
	{
		for (sel=this;sel!=NULL;sel=sel->multisel) 
			if ((sel->isxobj==0)||(sel->isxobj==1)) //Only available for objects
			{
			  if (pDoc->FindObjMinMax(refblock,isxobj,objchunk,objno)) //Find new min & max of object
			  {
				  s.Format("X:%f Y:%f Z:%f InMemZ:%f", pDoc->MemObjMiddle.x, pDoc->MemObjMiddle.y, pDoc->MemObjMiddle.z, pDoc->memory_Zh );
				  statusbar->SetPaneText(3,s,TRUE);
			  }
			}
			else
			{
				//Sound & Light sources
				b=&pDoc->trk[refblock];
				s="";
				if (isxobj==3) s.Format("X:%f Y:%f Z:%f InMemZ:%f", ((float)b->soundsrc[objno].refpoint.x)/65536, ((float)b->soundsrc[objno].refpoint.y)/65536, ((float)b->soundsrc[objno].refpoint.z)/65536, pDoc->memory_Zh );
				if (isxobj==2) s.Format("X:%f Y:%f Z:%f InMemZ:%f", ((float)b->lightsrc[objno].refpoint.x)/65536, ((float)b->lightsrc[objno].refpoint.y)/65536, ((float)b->lightsrc[objno].refpoint.z)/65536, pDoc->memory_Zh );
				statusbar->SetPaneText(3,s,TRUE);
			}
	}
}

void CT3EDView::OnViewRotateMode() 
{
	int x,y,i,num,n0,d,d0;
	struct TRKBLOCK *trk0,*trk;
	RECT r;

	bScrollRotate=TRUE;
	statusbar->SetPaneText(2,"ROTATE",TRUE);
	SetScrollRange(SB_HORZ,-180,180,FALSE);
	SetScrollRange(SB_VERT,0,90,FALSE);
	SetScrollPos(SB_HORZ,phi,TRUE);
	SetScrollPos(SB_VERT,theta,TRUE);
	
	// look for a suitable reference point around which to rotate
	GetClientRect(&r);
	if ((offsetx>r.right)||(offsetx<-r.right)||
			(offsety>r.bottom)||(offsety<-r.bottom)) {
		dxoffs=-offsetx
				-(int)((refnode->x*cosPhi-refnode->y*sinPhi)/scale);
		dyoffs=-offsety+(int)((refnode->y*cosPhi*cosTheta+
				refnode->x*sinPhi*cosTheta+refnode->z*sinTheta)/scale);
		trk0=GetDocument()->trk;
		num=GetDocument()->nBlocks;
		n0=-1; d0=1<<30;
		for (i=0,trk=trk0;i<num;i++,trk++) {
			x=FloatX(trk->ptCentre); y=FloatY(trk->ptCentre);
			if (x<0) x=-x; if (y<0) y=-y;
			if ((x<r.right/2)&&(y<r.bottom/2)) {
				d=(x<<16)/r.right+(y<<16)/r.bottom;
				if (d<d0) { d0=d; n0=i; }
			}
		}
		if (n0>=0) {
			trk=trk0+n0;
			x=FloatX(trk->ptCentre); y=FloatY(trk->ptCentre);
			refblock=n0;
			UpdateRefBlock();
			refnode=&(trk->ptCentre);
			isTruePoint=FALSE; refpoly=NULL; objno=-1;
		}
	}

	offsetx=0; offsety=0; // recenter so we don't get lost
	InvalidateRect(NULL,TRUE);
}

void CT3EDView::OnUpdateViewRotateMode(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!isEmpty);
	pCmdUI->SetCheck(bScrollRotate&&!isEmpty);
}

void CT3EDView::OnViewTranslMode() 
{
	bScrollRotate=FALSE;
	statusbar->SetPaneText(2,"TRANSL",TRUE);
	SetScrollRange(SB_HORZ,-SCROLLSZ,SCROLLSZ,FALSE);
	SetScrollRange(SB_VERT,-SCROLLSZ,SCROLLSZ,FALSE);
	SetScrollPos(SB_HORZ,offsetx,TRUE);
	SetScrollPos(SB_VERT,offsety,TRUE);
}

void CT3EDView::OnUpdateViewTranslMode(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!isEmpty);
	pCmdUI->SetCheck(!bScrollRotate&&!isEmpty);
}

void CT3EDView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	InvalidateRect(NULL,TRUE);
}

void CT3EDView::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!isEmpty);	
}

void CT3EDView::OnUpdateFileSaveAs(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!isEmpty);	
}

void CT3EDView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (isDragging) return;
	switch(nChar) {
		case VK_LEFT:  OnHScroll(SB_LINELEFT,0,NULL); break;
		case VK_RIGHT: OnHScroll(SB_LINERIGHT,0,NULL); break;
		case VK_UP:    OnVScroll(SB_LINEUP,0,NULL); break;
		case VK_DOWN:  OnVScroll(SB_LINEDOWN,0,NULL); break;
		default: CView::OnKeyDown(nChar, nRepCnt, nFlags);
	}
}

void CT3EDView::OnUpdateViewShowhideobjects(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!isEmpty);	
	pCmdUI->SetCheck(bShowObjects&&!isEmpty);
}

void CT3EDView::OnViewShowhideobjects() 
{
	bShowObjects=!bShowObjects;
	if ((multisel!=NULL)&&!bShowObjects) { delete multisel; multisel=NULL; }
	if (!bShowObjects&&(refpoly!=NULL))
		if (!(IsPolyOfDetail(nDetail)||IsPolyOfDetail(6))) refpoly=NULL;
	if (!bShowObjects) objno=-1;
	InvalidateRect(NULL,!bShowObjects);
}

void CT3EDView::OnViewShowlanes() 
{
	bShowLanes=!bShowLanes;
	if ((multisel!=NULL)&&!bShowLanes) { delete multisel; multisel=NULL; }
	if (isTruePoint&&!bShowLanes)
		if (!IsVisibleNode(refblock,refnode)) isTruePoint=FALSE;
	if ((refpoly!=NULL)&&!bShowLanes&&!isxobj&&(refpolyobj<0))
		if ((refpolychunk==6)||(refpolychunk&1)) refpoly=NULL;
	InvalidateRect(NULL,!bShowLanes);
}

void CT3EDView::OnUpdateViewShowlanes(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!isEmpty);
	pCmdUI->SetCheck(bShowLanes&&!isEmpty);
}

//HOO: Function to show track (4)
void CT3EDView::OnViewShowTrack()
{
	CT3EDDoc* pDoc = GetDocument();

	bShowTrack=!bShowTrack;
	if (pDoc->bCreateScene)
	{
		pDoc->TrackView->ShowView(bShowTrack);
	}
	InvalidateRect(NULL, TRUE);
}

void CT3EDView::OnUpdateViewShowTrack(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!isEmpty);
	pCmdUI->SetCheck(bShowTrack&&!isEmpty);
}
//HOO: (4)

void CT3EDView::OnUpdateTrViewZoomin3d(CCmdUI* pCmdUI) 
{
	// TODO: Code für die Befehlsbehandlungsroutine zum Aktualisieren der Benutzeroberfläche hier einfügen
	pCmdUI->Enable(!isEmpty&&bShowTrack);
}

void CT3EDView::OnUpdateTrViewZoomout3d(CCmdUI* pCmdUI) 
{
	// TODO: Code für die Befehlsbehandlungsroutine zum Aktualisieren der Benutzeroberfläche hier einfügen
	pCmdUI->Enable(!isEmpty&&bShowTrack);
}

void CT3EDView::OnTrViewZoomin3d() 
{
	// TODO: Code für Befehlsbehandlungsroutine hier einfügen
	
}

void CT3EDView::OnTrViewZoomout3d() 
{
	// TODO: Code für Befehlsbehandlungsroutine hier einfügen
	
}


void CT3EDView::OnLButtonDown(UINT nFlags, CPoint point) 
{	
	int i,j,k,l,m,n,x,y,i0,j0,k0,l0,dist,dist0,x0,y0;
	RECT rect;
	BOOL needErase=FALSE;
	CT3EDDoc *pDoc=GetDocument();
	struct FLOATPT *coreref,*pt,*pt0,mypt; //,tmppt;
	struct XOBJDATA *xx;
	LPPOLYGONDATA p,p0;
	struct OBJPOLYBLOCK *o;
	struct LIGHTSRC *light;
	struct SOUNDSRC *sound;
	int isx0;
	POINT ptnew;
	CString s;

	if (isEmpty) return;
	GetClientRect(&rect);
	dxoffs=rect.right/2-offsetx
			  -(int)((refnode->x*cosPhi-refnode->y*sinPhi)/scale);
	dyoffs=rect.bottom/2-offsety+(int)((refnode->y*cosPhi*cosTheta+
				refnode->x*sinPhi*cosTheta+refnode->z*sinTheta)/scale);

	rect.left=point.x-10; rect.right=point.x+10;
	rect.top=point.y-10; rect.bottom=point.y+10;

	CleanCursorZone(selMode,FALSE);
	switch(selMode) {
		case ID_MODE_BLOCK:
			i0=-1; dist0=1<<30;
			for (i=0;i<pDoc->nBlocks;i++) {
				x=FloatX(pDoc->trk[i].ptCentre);
				y=FloatY(pDoc->trk[i].ptCentre);
				dist=(x>point.x)?(x-point.x):(point.x-x);
				dist+=(y>point.y)?(y-point.y):(point.y-y);
				//if ((dist<dist0) { i0=i; x0=x; y0=y; dist0=dist; }

				if ((dist<dist0)&&((!ShowBasedOnVisi)|(ShowBasedOnVisi&&(pDoc->IsBlockVisibile(refblock,i)))))
					{ i0=i; x0=x; y0=y; dist0=dist; }
			}
			if (!MeetsClipRect(&(pDoc->trk[i0]),&rect)) return;
			if (refblock==i0) return;
			refblock=i0;
			refnode=&(pDoc->trk[i0].ptCentre);
			s.Format("X:%f Y:%f Z:%f InMemZ:%f", refnode->x, refnode->y, refnode->z, pDoc->memory_Zh );
			statusbar->SetPaneText(3,s,TRUE);
			break;

		case ID_MODE_POINT:
		case ID_MODE_EXTPOINT:
			coreref=NULL; pt0=NULL; dist0=10;
			for (i=0;i<pDoc->nBlocks;i++) {
				if (!MeetsClipRect(&(pDoc->trk[i]),&rect)) continue;
				if ((ShowBasedOnVisi)&&(!pDoc->IsBlockVisibile(refblock,i))) continue;
				pt=pDoc->trk[i].vert;
				n=pDoc->trk[i].nVertices;
				for (j=0;j<n;j++,pt++) {
					x=FloatX(*pt); y=FloatY(*pt);
					dist=((x>point.x)?(x-point.x):(point.x-x))
						+((y>point.y)?(y-point.y):(point.y-y));
					if (dist<dist0)
						if (IsVisibleNode(i,pt)) {
							i0=i; x0=x; y0=y; 
							dist0=dist; pt0=pt; 
							isx0=0;
							coreref=NULL;
						}
				}
			  if (bShowObjects||bShowLanes) {
				for (j=4*i;j<4*i+4;j++) { // explore xobjs
					xx=pDoc->xobj[j].obj;
					for (k=0;k<pDoc->xobj[j].nobj;k++,xx++) {
						pt=xx->vert;
						for (l=0;l<xx->nVertices;l++,pt++) {
							x=FloatFloatX(*pt,xx->ptRef); y=FloatFloatY(*pt,xx->ptRef);
							dist=((x>point.x)?(x-point.x):(point.x-x))
								+((y>point.y)?(y-point.y):(point.y-y));
							if (dist<dist0) {
								i0=i; x0=x; y0=y; 
								dist0=dist; pt0=pt;
								coreref=&(xx->ptRef);
								isx0=1;
							}
						}
					}
				}
				
			  }
			}
			if (pt0==NULL) return;
			if ((nFlags&(MK_CONTROL|MK_SHIFT))
				&& (editMode!=ID_TOOLS_MERGE) && (selMode==ID_MODE_POINT)) {
				CSelData *sel=new CSelData;
				sel->multisel=multisel;
				sel->refblock=refblock;
				if (refnode==&myrefnode) {
					sel->myrefnode.x=myrefnode.x;
					sel->myrefnode.y=myrefnode.y;
					sel->myrefnode.z=myrefnode.z;
					sel->refnode=&(sel->myrefnode);
				} 
				else sel->refnode=refnode;
				multisel=sel;
			} else {
				if (multisel!=NULL)
				{ delete multisel; multisel=NULL; }
			}
			memcpy(&mypt,refnode,12);
			if (!isx0) 
				{
				refnode=pt0;
				s.Format("X:%f Y:%f Z:%f InMemZ:%f", refnode->x, refnode->y, refnode->z, pDoc->memory_Zh );
				}
			else if (isx0==1) { // chose a xobj
				refnode=&myrefnode;
				myrefnode.x=coreref->x+pt0->x;
				myrefnode.z=coreref->z+pt0->z;
				myrefnode.y=coreref->y+pt0->y;
				s.Format("X:%f Y:%f Z:%f InMemZ:%f",myrefnode.x, myrefnode.y, myrefnode.z, pDoc->memory_Zh );
			}

			// merge vertices command
			if ((editMode==ID_TOOLS_MERGE)&&(nFlags&MK_CONTROL)) {
				pDoc->PrepareNewUndo(&mypt,refblock,offsetx,offsety);
				pDoc->MergePoint(refnode,&mypt);
				refnode=&myrefnode;
				memcpy(refnode,&mypt,12);
				InvalidateRect(NULL,TRUE);
				isTruePoint=TRUE;
				return;
			}
			refblock=i0;
			isTruePoint=TRUE;
			
				statusbar->SetPaneText(3,s,TRUE);
			break;

		case ID_MODE_POLYGON:
			i0=-1; dist0=1<<30; p0=NULL;
			for (i=0;i<pDoc->nBlocks;i++) {
				if (!MeetsClipRect(&(pDoc->trk[i]),&rect)) continue;
				if ((ShowBasedOnVisi)&&(!pDoc->IsBlockVisibile(refblock,i))) continue;
				pt=pDoc->trk[i].vert;
				for (k=0;k<7;k++) {
					if (!bShowLanes) if (k!=nDetail) continue;
					if (bShowLanes) if (((k&6)!=nDetail)&&(k!=6)) continue;
					p=pDoc->poly[i].poly[k];
					n=pDoc->poly[i].sz[k];
					for (j=0;j<n;j++,p++) {
						x=(FloatX(pt[p->vertex[0]])+FloatX(pt[p->vertex[1]])
						  +FloatX(pt[p->vertex[2]])+FloatX(pt[p->vertex[3]]))/4;
						y=(FloatY(pt[p->vertex[0]])+FloatY(pt[p->vertex[1]])
						  +FloatY(pt[p->vertex[2]])+FloatY(pt[p->vertex[3]]))/4;
						dist=((x>point.x)?(x-point.x):(point.x-x))
							+((y>point.y)?(y-point.y):(point.y-y));
						if (dist<dist0) {
							i0=i; x0=x; y0=y; 
							j0=k; k0=-1; l0=j;
							dist0=dist; p0=p; pt0=pt; isx0=0;
						}
					}
				}
			}
		if (bShowObjects) {
			for (i=0;i<=pDoc->nBlocks;i++) {
				if (!MeetsClipRect(&(pDoc->trk[i]),&rect)) continue;
				if ((ShowBasedOnVisi)&&(!pDoc->IsBlockVisibile(refblock,i))) continue;
				pt=pDoc->trk[i].vert;
				for (j=0;j<4;j++) {
					o=&(pDoc->poly[i].obj[j]);
					if (o->n1==0) continue;
					for (k=0;k<o->nobj;k++) {
						p=o->poly[k];
						n=o->numpoly[k];
						for (l=0;l<n;l++,p++) {
							x=(FloatX(pt[p->vertex[0]])+FloatX(pt[p->vertex[1]])
							  +FloatX(pt[p->vertex[2]])+FloatX(pt[p->vertex[3]]))/4;
							y=(FloatY(pt[p->vertex[0]])+FloatY(pt[p->vertex[1]])
							  +FloatY(pt[p->vertex[2]])+FloatY(pt[p->vertex[3]]))/4;
							dist=((x>point.x)?(x-point.x):(point.x-x))
								+((y>point.y)?(y-point.y):(point.y-y));
							if (dist<dist0) {
								i0=i; x0=x; y0=y; 
								j0=j; k0=k; l0=l;
								dist0=dist; p0=p; pt0=pt; isx0=0;
							}
						}
					}
				}
			}
			for (i=0;i<(4*pDoc->nBlocks);i++) {
				if (!MeetsClipRect(&(pDoc->trk[i/4]),&rect)) { i=i|3; continue; }
				if ((ShowBasedOnVisi)&&(!pDoc->IsBlockVisibile(refblock,int(i/4)))) continue;
				xx=pDoc->xobj[i].obj;
				for (j=0;j<pDoc->xobj[i].nobj;j++,xx++) {
					pt=xx->vert;
					p=xx->polyData;
					for (k=0;k<xx->nPolygons;k++,p++) {
						x=(FloatFloatX(pt[p->vertex[0]],xx->ptRef)+FloatFloatX(pt[p->vertex[1]],xx->ptRef)
						  +FloatFloatX(pt[p->vertex[2]],xx->ptRef)+FloatFloatX(pt[p->vertex[3]],xx->ptRef))/4;
						y=(FloatFloatY(pt[p->vertex[0]],xx->ptRef)+FloatFloatY(pt[p->vertex[1]],xx->ptRef)
						  +FloatFloatY(pt[p->vertex[2]],xx->ptRef)+FloatFloatY(pt[p->vertex[3]],xx->ptRef))/4;
						dist=((x>point.x)?(x-point.x):(point.x-x))
							+((y>point.y)?(y-point.y):(point.y-y));
						if (dist<dist0) {
							i0=i/4; x0=x; y0=y; 
							j0=i%4; k0=j; l0=k;
							dist0=dist; p0=p; pt0=pt; 
							isx0=1; coreref=&(xx->ptRef);
						}
					}
				}
			}
		}
			if (p0==NULL) return;
			if ((nFlags&(MK_CONTROL|MK_SHIFT)) && (refpoly!=NULL)) {
				CSelData *sel=new CSelData;
				sel->multisel=multisel;
				sel->refblock=refblock;
				sel->refpolychunk=refpolychunk;
				sel->refpolyobj=refpolyobj;
				sel->refpolyno=refpolyno;
				sel->isxobj=isxobj;
				sel->refpoly=refpoly;
				sel->refvertices=refvertices;
				sel->refnode=refnode;
				multisel=sel;
			} else {
				if (multisel!=NULL)
				{ delete multisel; multisel=NULL; }
			}
			refblock=i0;
			refpolychunk=j0;
			refpolyobj=k0;
			refpolyno=l0;
			isxobj=isx0;
			refpoly=p0;
			refvertices=pt0;
			if (isx0) pt=coreref;
			else pt=refvertices+refpoly->vertex[0];
			x0=FloatX(*pt); y0=FloatY(*pt);
			refnode=pt;
			break;

		case ID_MODE_OBJECT:
			if (!bShowObjects) return;
			i0=-1; dist0=1<<30;
			for (i=0;i<pDoc->nBlocks;i++) {
				if (!MeetsClipRect(&(pDoc->trk[i]),&rect)) continue;
				if ((ShowBasedOnVisi)&&(!pDoc->IsBlockVisibile(refblock,i))) continue;
				pt=pDoc->trk[i].vert;
				for (j=0;j<4;j++) {
					o=&(pDoc->poly[i].obj[j]);
					if (o->n1==0) continue;
					for (k=0;k<o->nobj;k++) {
						p=o->poly[k];
						n=o->numpoly[k];
						for (l=0;l<n;l++,p++) {
							x=(FloatX(pt[p->vertex[0]])+FloatX(pt[p->vertex[1]])
							  +FloatX(pt[p->vertex[2]])+FloatX(pt[p->vertex[3]]))/4;
							y=(FloatY(pt[p->vertex[0]])+FloatY(pt[p->vertex[1]])
							  +FloatY(pt[p->vertex[2]])+FloatY(pt[p->vertex[3]]))/4;
							dist=((x>point.x)?(x-point.x):(point.x-x))
								+((y>point.y)?(y-point.y):(point.y-y));
							if (dist<dist0) {
								i0=i; j0=j; k0=k;
								dist0=dist; isx0=0;
							}
						}
					}
				}
				
				for (k=0,light=pDoc->trk[i].lightsrc;k<pDoc->trk[i].nLightsrc;k++,light++)
				{
					//MessageBox("täällä!", "ALERT", MB_ICONERROR);
					x=IntX(light->refpoint);
					y=IntY(light->refpoint);
					dist=((x>point.x)?(x-point.x):(point.x-x))
						+((y>point.y)?(y-point.y):(point.y-y));
					if (dist<dist0) {
						i0=i; j0=0; k0=k;
						dist0=dist;
						isx0=2;
					//MessageBox("täällä!", "ALERT", MB_ICONERROR);
					}
				}
				
				//Nappe1: Selecting sound sources:
				for (k=0,sound=pDoc->trk[i].soundsrc;k<pDoc->trk[i].nSoundsrc;k++,sound++)
				{
					x=IntX(sound->refpoint);
					y=IntY(sound->refpoint);
					dist=((x>point.x)?(x-point.x):(point.x-x))
						+((y>point.y)?(y-point.y):(point.y-y));
					if (dist<dist0) {
						i0=i; j0=0; k0=k;
						dist0=dist;
						isx0=3;
					}
				}
				// Nappe1: ADDITION ENDS 

				//JimD: Selecting VRoad Points
				if (DrawVRoad==TRUE)
				for (k=0;k<pDoc->col.vroadHead.nrec;k++)
				{
					x=IntX(pDoc->col.vroad[k].refPt);
					y=IntY(pDoc->col.vroad[k].refPt);
					dist=((x>point.x)?(x-point.x):(point.x-x))
						+((y>point.y)?(y-point.y):(point.y-y));
					if (dist<dist0) 
					{
						i0=0;
						for (m=0;m<pDoc->nBlocks;m++)
							if ((k>=pDoc->trk[m].nStartPos)&&(k<=(pDoc->trk[m].nStartPos+pDoc->trk[m].nPositions)))
								i0=m;
						j0=0; k0=k;
						dist0=dist;
						isx0=4; //VRoad == Extra Object 4 !
					}
				}
			
			}
			for (i=0;i<(1+4*pDoc->nBlocks);i++) { //Extra & Global objects
				if ((i!=4*pDoc->nBlocks)&&(!MeetsClipRect(&(pDoc->trk[i/4]),&rect))) { i=i|3; continue; }
				if ((ShowBasedOnVisi)&&(!pDoc->IsBlockVisibile(refblock,int(i/4)))) continue;
				//if ((!(4*pDoc->nBlocks==i))&&(ShowBasedOnVisi)&&(!IsBlockVisibile(int(i/4)))) continue;
				xx=pDoc->xobj[i].obj;
				for (j=0;j<pDoc->xobj[i].nobj;j++,xx++) {
					pt=xx->vert;
					p=xx->polyData;
					for (k=0;k<xx->nPolygons;k++,p++) {
						x=(FloatFloatX(pt[p->vertex[0]],xx->ptRef)+FloatFloatX(pt[p->vertex[1]],xx->ptRef)
						  +FloatFloatX(pt[p->vertex[2]],xx->ptRef)+FloatFloatX(pt[p->vertex[3]],xx->ptRef))/4;
						y=(FloatFloatY(pt[p->vertex[0]],xx->ptRef)+FloatFloatY(pt[p->vertex[1]],xx->ptRef)
						  +FloatFloatY(pt[p->vertex[2]],xx->ptRef)+FloatFloatY(pt[p->vertex[3]],xx->ptRef))/4;
						dist=((x>point.x)?(x-point.x):(point.x-x))
							+((y>point.y)?(y-point.y):(point.y-y));
						if (dist<dist0) {
							i0=i/4; j0=i%4; k0=j;
							dist0=dist; isx0=1;
						}
					}
				}
			}
			if (i0<0) return;
			if ((nFlags&(MK_CONTROL|MK_SHIFT)) && (objno>=0)) {
				CSelData *sel=new CSelData;
				sel->multisel=multisel;
				sel->refblock=refblock;
				sel->isxobj=isxobj;
				sel->objchunk=objchunk;
				sel->objno=objno;
				if (refnode==&myrefnode) {
					sel->myrefnode.x=myrefnode.x;
					sel->myrefnode.y=myrefnode.y;
					sel->myrefnode.z=myrefnode.z;
					sel->refnode=&(sel->myrefnode);
				} 
				else sel->refnode=refnode;
				multisel=sel;
			} else {
				if (multisel!=NULL)
				{ delete multisel; multisel=NULL; }
			}
			refblock=i0;
			isxobj=isx0;
			objchunk=j0;
			objno=k0;
			//MessageBox("täällä!", "ALERT", MB_ICONERROR);
			if (isx0==4) {
				pt=&myrefnode;
				myrefnode.x=((float)pDoc->col.vroad[k0].refPt.x)/65536;
				myrefnode.y=((float)pDoc->col.vroad[k0].refPt.y)/65536;
				myrefnode.z=((float)pDoc->col.vroad[k0].refPt.z)/65536;
				
			}
			if (isx0==3) {
				pt=&myrefnode;
				myrefnode.x=((float)pDoc->trk[i0].soundsrc[k0].refpoint.x)/65536;
				myrefnode.y=((float)pDoc->trk[i0].soundsrc[k0].refpoint.y)/65536;
				myrefnode.z=((float)pDoc->trk[i0].soundsrc[k0].refpoint.z)/65536;
				
			}
			else if (isx0==2) {
				pt=&myrefnode;
				myrefnode.x=((float)pDoc->trk[i0].lightsrc[k0].refpoint.x)/65536;
				myrefnode.y=((float)pDoc->trk[i0].lightsrc[k0].refpoint.y)/65536;
				myrefnode.z=((float)pDoc->trk[i0].lightsrc[k0].refpoint.z)/65536;
			}
			// Nappe1: selecting Sound Source
			
			
			// ADDITION ENDS
			else if (isx0==1) 
				if (refblock==pDoc->nBlocks) //Global object ?
				{
					pDoc->FindObjMinMax(refblock,isxobj,objchunk,objno);
					pt=&pDoc->MemObjMiddle;
				}
				else
					pt=&(pDoc->xobj[4*i0+j0].obj[k0].ptRef);
			else if (isx0==0) pt=pDoc->trk[i0].vert	+pDoc->poly[i0].obj[j0].poly[k0]->vertex[0]; //JimD 
			x0=FloatX(*pt); y0=FloatY(*pt);
			refnode=pt;
			
			// merge vertices command
			if ((editMode==ID_TOOLS_MERGE)&&(nFlags&MK_CONTROL)) 
			{
				OnToolsMergeObject();
				return;
			}
			break;
	}

	GetClientRect(&rect);
	if (bScrollRotate) {
		offsetx=0; 
		offsety=0; 
		needErase=TRUE;
		ptnew.x=rect.right/2; ptnew.y=rect.bottom/2;
		ClientToScreen(&ptnew);
		if ((editMode==ID_TOOLS_MOVEXY)||(editMode==ID_TOOLS_MOVEZ))
			SetCursorPos(ptnew.x,ptnew.y);
	}
	else {	// x0,y0 == coordinates of new refnode
		offsetx=-(x0-rect.right/2); 
		offsety=-(y0-rect.bottom/2);
		if (offsetx<-rect.right/2+40)
			{ offsetx+=rect.right/2; needErase=TRUE; }
		if (offsetx>rect.right/2-40)
			{ offsetx-=rect.right/2; needErase=TRUE; }
		if (offsety<-rect.bottom/2+40)
			{ offsety+=rect.bottom/2; needErase=TRUE; }
		if (offsety>rect.bottom/2-40)
			{ offsety-=rect.bottom/2; needErase=TRUE; }
		SetScrollPos(SB_HORZ,offsetx,TRUE);
		SetScrollPos(SB_VERT,offsety,TRUE);
	}
	if (needErase) InvalidateRect(NULL,TRUE);
	else CleanCursorZone(selMode,TRUE);
	UpdateRefBlock();
	delayMouseMove=TRUE;
}

void CT3EDView::OnModeBlock() 
{
	if (isDragging) return; // safety
	CleanCursorZone(selMode,FALSE);
	selMode=ID_MODE_BLOCK;
	if (multisel!=NULL) { delete multisel; multisel=NULL; }
	isTruePoint=FALSE; refpoly=NULL; objno=-1;
	editMode=NULL;
	CleanCursorZone(selMode,TRUE);
}

void CT3EDView::OnUpdateModeBlock(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!isEmpty);
	pCmdUI->SetCheck((selMode==ID_MODE_BLOCK)&&!isEmpty);
}

void CT3EDView::OnModeExtpoint() 
{
	if (isDragging) return; // safety
	CleanCursorZone(selMode,FALSE);
	selMode=ID_MODE_EXTPOINT;
	if (multisel!=NULL) { delete multisel; multisel=NULL; }
	refpoly=NULL; objno=-1;
	editMode=NULL;
	CleanCursorZone(selMode,TRUE);
}

void CT3EDView::OnUpdateModeExtpoint(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!isEmpty);
	pCmdUI->SetCheck((selMode==ID_MODE_EXTPOINT)&&!isEmpty);
}

void CT3EDView::OnModePoint() 
{
	if (isDragging) return; // safety
	CleanCursorZone(selMode,FALSE);
	selMode=ID_MODE_POINT;
	if (multisel!=NULL) { delete multisel; multisel=NULL; }
	refpoly=NULL; objno=-1;
	editMode=NULL;
	CleanCursorZone(selMode,TRUE);
}

void CT3EDView::OnUpdateModePoint(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!isEmpty);
	pCmdUI->SetCheck((selMode==ID_MODE_POINT)&&!isEmpty);
}

void CT3EDView::OnModeObject() 
{
	if (isDragging) return; // safety
	CleanCursorZone(selMode,FALSE);
	selMode=ID_MODE_OBJECT;
	if (multisel!=NULL) { delete multisel; multisel=NULL; }
	isTruePoint=FALSE; refpoly=NULL;
	editMode=NULL;
	CleanCursorZone(selMode,TRUE);
}

void CT3EDView::OnUpdateModeObject(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!isEmpty&&bShowObjects);
	pCmdUI->SetCheck((selMode==ID_MODE_OBJECT)&&!isEmpty);
}

void CT3EDView::OnModePolygon() 
{
	if (isDragging) return; // safety
	CleanCursorZone(selMode,FALSE);
	selMode=ID_MODE_POLYGON;
	if (multisel!=NULL) { delete multisel; multisel=NULL; }
	isTruePoint=FALSE; objno=-1;
	editMode=NULL;
	CleanCursorZone(selMode,TRUE);
}

void CT3EDView::OnUpdateModePolygon(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!isEmpty);
	pCmdUI->SetCheck((selMode==ID_MODE_POLYGON)&&!isEmpty);
}

void CT3EDView::CleanCursorZone(int mode,BOOL newstate)
{
	RECT r;
	int x,y,zsize,i;
	struct FLOATPT *t;
	CSelData *sel;

	if (isEmpty) return;
	GetClientRect(&r);
	dxoffs=r.right/2-offsetx
			  -(int)((refnode->x*cosPhi-refnode->y*sinPhi)/scale);
	dyoffs=r.bottom/2-offsety+(int)((refnode->y*cosPhi*cosTheta+
				refnode->x*sinPhi*cosTheta+refnode->z*sinTheta)/scale);

	switch(mode) {
		case ID_MODE_BLOCK: 
		case ID_MODE_POLYGON: 
		case ID_MODE_OBJECT: 
			r.left=1<<30; r.right=-(1<<30);
			r.top=1<<30; r.bottom=-(1<<30);
			for (sel=this;sel!=NULL;sel=sel->multisel) {
				t=GetDocument()->trk[sel->refblock].ptBounding;
				zsize=(int)(200*sinTheta/scale);
				for (i=0;i<=3;i++) { 
					x=FloatX(t[i]);
					y=FloatY(t[i]);
					if (x<=r.left) r.left=x-1;
					if (x>=r.right) r.right=x+1;
					if (y-zsize<=r.top) r.top=y-zsize-1;
					if (y+zsize>=r.bottom) r.bottom=y+zsize+1;
				}
			}
			InvalidateRect(&r,FALSE); 
			break;
		case ID_MODE_EXTPOINT: // may later need to enlarge to circle
		case ID_MODE_POINT: 
			for (sel=this;sel!=NULL;sel=sel->multisel) {
				x=FloatX(*(sel->refnode));
				y=FloatY(*(sel->refnode));
				r.top=y-6; r.bottom=y+6;
				r.left=x-6; r.right=x+6;
				InvalidateRect(&r,!newstate);
			}
			break;
	}
}

void CT3EDView::OnViewRefresh() 
{
	InvalidateRect(NULL,TRUE);	
}

void CT3EDView::OnViewRecenter() 
{
	if ((offsetx==0)&&(offsety==0)) return;
	offsetx=0; offsety=0;
	InvalidateRect(NULL,TRUE);
}

void CT3EDView::OnUpdateViewRecenter(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!isEmpty);
}

BOOL CT3EDView::IsVisibleNode(int block,struct FLOATPT *pt)
{ 
// could be made much simpler using trk.nxxxxxVert !!!
	CT3EDDoc *pDoc=GetDocument();
	struct FLOATPT *v;
	struct OBJPOLYBLOCK *o;
	LPPOLYGONDATA p;
	int i,j,k,n;

	if (bShowLanes) return TRUE; // anything becomes visible
	if (pt==&myrefnode) return bShowObjects; // an xobj refpoint
	v=pDoc->trk[block].vert;
	p=pDoc->poly[block].poly[nDetail];
	n=pDoc->poly[block].sz[nDetail];
	for (i=0;i<n;i++,p++) {
		if (pt==v+p->vertex[0]) return TRUE;
		if (pt==v+p->vertex[1]) return TRUE;
		if (pt==v+p->vertex[2]) return TRUE;
		if (pt==v+p->vertex[3]) return TRUE;
	}
	if (bShowObjects) {
		for (j=0;j<4;j++) {
			o=&(pDoc->poly[block].obj[j]);
			if (o->n1==0) continue;
			for (k=0;k<o->nobj;k++) {
				p=o->poly[k];
				n=o->numpoly[k];
				for (i=0;i<n;i++,p++) {
					if (pt==v+p->vertex[0]) return TRUE;
					if (pt==v+p->vertex[1]) return TRUE;
					if (pt==v+p->vertex[2]) return TRUE;
					if (pt==v+p->vertex[3]) return TRUE;
				}
			}
		}
	}
	return FALSE;
}

BOOL CT3EDView::IsPolyOfDetail(int detail)
{
	if (refpoly==NULL) return FALSE;
	if (isxobj) return FALSE;
	if (refpolyobj>=0) return FALSE;
	return ((refpolychunk&6)==detail); // detail or detail+1
}

void CT3EDView::OnUpdateToolsMovexy(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!isEmpty);
	pCmdUI->SetCheck(editMode==ID_TOOLS_MOVEXY);
}

void CT3EDView::OnToolsMovexy() 
{
	if (isDragging) return; // safety
	if (editMode==ID_TOOLS_MOVEXY) editMode=NULL;
	else editMode=ID_TOOLS_MOVEXY;
}

void CT3EDView::OnUpdateToolsMovez(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!isEmpty);
	pCmdUI->SetCheck(editMode==ID_TOOLS_MOVEZ);
}

void CT3EDView::OnToolsMovez() 
{
	if (isDragging) return; // safety
	if (editMode==ID_TOOLS_MOVEZ) editMode=NULL;
	else editMode=ID_TOOLS_MOVEZ;
}

void CT3EDView::OnMouseMove(UINT nFlags, CPoint point) 
{
	RECT rect;

	if (!(nFlags&MK_LBUTTON)) return;
	if (delayMouseMove) { delayMouseMove=FALSE; return; }
	if ((editMode!=ID_TOOLS_MOVEXY)&&(editMode!=ID_TOOLS_MOVEZ)) return;
	if (isDragging) ShowHideCursor(); // hide cursor
	else {
		// check if dragging is allowed
		if (selMode==ID_MODE_BLOCK) 
			if ((extraSmooth?1:smooth1Start)+2*smooth1Width
				>=GetDocument()->col.vroadHead.nrec-20) return;
		if (selMode==ID_MODE_POINT)
			if (!isTruePoint) return;
		if (selMode==ID_MODE_OBJECT)
			if ((!bShowObjects)||(objno<0)) return;
		if (selMode==ID_MODE_POLYGON)
			if (refpoly==NULL) return;
		isDragging=TRUE;
		SetCapture();
	}
	GetClientRect(&rect);
	if ((point.x<0)||(point.y<0)||(point.x>rect.right)||(point.y>rect.bottom)) 
		{ ReleaseCapture(); isDragging=FALSE; return; }
	dragX=point.x; dragY=point.y;
	if (editMode==ID_TOOLS_MOVEZ) dragX=rect.right/2-offsetx;
	if (((editMode==ID_TOOLS_MOVEZ)&&(theta==0))||
		((editMode==ID_TOOLS_MOVEXY)&&(theta==90)))
		dragY=rect.bottom/2-offsety;
	ShowHideCursor();
}

void CT3EDView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	float dx,dy,dz,a,b;
	RECT rect;
	CT3EDDoc *pDoc=GetDocument();
	int newblk,i,orgno,newno,orgblk;
	struct FLOATPT pt,*newpt;
	CSelData *sel,*sel2;
	CString s;

	if (pDoc->MoveToPoint.DestPointSet==TRUE) isDragging=TRUE; //Manually entered move distance ?
	if (!isDragging) return;
	GetClientRect(&rect);
	ShowHideCursor();
	isDragging=FALSE;
	ReleaseCapture();
	dragX=point.x; dragY=point.y;
	if (editMode==ID_TOOLS_MOVEZ) dragX=rect.right/2-offsetx;
	if (((editMode==ID_TOOLS_MOVEZ)&&(theta==0))||
		((editMode==ID_TOOLS_MOVEXY)&&(theta==90)))
		dragY=rect.bottom/2-offsety;

	dx=0; dy=0; dz=0;
	if ((editMode==ID_TOOLS_MOVEZ)&&(theta!=0))
		dz=(rect.bottom/2-offsety-dragY)*scale/sinTheta;
	if (editMode==ID_TOOLS_MOVEXY) {
		a=(dragX-rect.right/2+offsetx)*scale;
		if (theta==90) b=0;
		else b=(rect.bottom/2-offsety-dragY)*scale/cosTheta;
		dx=a*cosPhi+b*sinPhi;
		dy=-a*sinPhi+b*cosPhi;
	}
	if (pDoc->MoveToPoint.DestPointSet==TRUE) //Distance manually set
	{
		dx=pDoc->MoveToPoint.DestPt.x;
		dy=pDoc->MoveToPoint.DestPt.y;
		dz=pDoc->MoveToPoint.DestPt.z;
		pDoc->MoveToPoint.DestPointSet=FALSE;
		if (pDoc->settings.MoveTo_no_smooth) nFlags=MK_CONTROL;
	}

	// proceed with move depending on what we want
	pDoc->PrepareNewUndo(refnode,refblock,offsetx,offsety);
	switch(selMode) {
		case ID_MODE_BLOCK: 
			if ((MK_CONTROL&&nFlags)==1) //Control pressed ?
				{ 
				pDoc->MoveBlockSimple(refblock,dx,dy,dz);
				//pDoc->MoveBlocks(refblock,dx,dy,dz,smooth1Start,smooth1Width,2);
				}
			else //No Control, then regular move
				pDoc->MoveBlocks(refblock,dx,dy,dz,smooth1Start,smooth1Width,extraSmooth);
			s.Format("X:%f Y:%f Z:%f InMemZ:%f", refnode->x, refnode->y, refnode->z, pDoc->memory_Zh );
			statusbar->SetPaneText(3,s,TRUE);
			break;
		case ID_MODE_POINT:
			for (sel=this;sel!=NULL;sel=sel->multisel)
				memcpy(sel->origpos,sel->refnode,12);
			for (sel=this;sel!=NULL;sel=sel->multisel)
				if (fabs(sel->refnode->x-sel->origpos->x)+
					fabs(sel->refnode->y-sel->origpos->y)+
					fabs(sel->refnode->z-sel->origpos->z) < 1E-3)
						pDoc->MovePointBy(sel->refnode,dx,dy,dz);
			break;
			pDoc->MovePointBy(refnode,dx,dy,dz);
			break;
		case ID_MODE_EXTPOINT:
			pDoc->ExtMovePointBy(refnode,dx,dy,dz,(float)smooth2Start,(float)smooth2Width);
			break;
		case ID_MODE_OBJECT:
			for (sel=this;sel!=NULL;sel=sel->multisel)
				sel->origflag=TRUE;
			for (sel=this;sel!=NULL;sel=sel->multisel) 
				//if ((sel->origflag)&&(sel->refblock!=pDoc->nBlocks))
			if (sel->origflag){
				orgblk=sel->refblock;
				orgno=sel->objno;
				newblk=pDoc->MoveObjectBy(orgblk,sel->isxobj,sel->objchunk,orgno,dx,dy,dz);
				for (sel2=this;sel2!=NULL;sel2=sel2->multisel)
					if ((sel2->refblock==orgblk)&&
						(sel2->isxobj==sel->isxobj)&&
						(sel2->objchunk==sel->objchunk)&&
						(sel2->objno==orgno))
							sel2->origflag=FALSE;
				if ((newblk!=orgblk)&&(sel->isxobj!=4)) {
					if (sel->isxobj==3) {
						newno=pDoc->trk[newblk].nSoundsrc-1;
					} else
					if (sel->isxobj==2) {
						newno=pDoc->trk[newblk].nLightsrc-1;
					} else
					if (sel->isxobj==1) {
						newno=pDoc->xobj[4*newblk+sel->objchunk].nobj-1;
						newpt=&(pDoc->xobj[4*newblk+sel->objchunk].obj[newno].ptRef);
					} else {
						newno=pDoc->poly[newblk].obj[sel->objchunk].nobj-1;
						newpt=pDoc->trk[newblk].vert+
							pDoc->poly[newblk].obj[sel->objchunk].poly[newno]->vertex[0];
					}
					for (sel2=this;sel2!=NULL;sel2=sel2->multisel)
						if ((sel2->refblock==orgblk)&&
							(sel2->isxobj==sel->isxobj)&&
							(sel2->objchunk==sel->objchunk)) 
						{
							if (sel2->objno>orgno) sel2->objno--;
							else if (sel2->objno==orgno) { 
								sel2->objno=newno;
								sel2->refblock=newblk;
								if (sel2->isxobj!=2) sel2->refnode=newpt;
							}
						}
				}
			}
			UpdateRefBlock();
			//UpdateStatusbar();
			break;
		case ID_MODE_POLYGON:
			for (sel=this;sel!=NULL;sel=sel->multisel) {
				for (i=0;i<4;i++) {
					memcpy(sel->origpos+i,
						sel->refvertices+sel->refpoly->vertex[i],12);
					if (sel->isxobj) { 
						sel->origpos[i].x+=sel->refnode->x;
						sel->origpos[i].y+=sel->refnode->y;
						sel->origpos[i].z+=sel->refnode->z;
					}
				}
			}
			for (sel=this;sel!=NULL;sel=sel->multisel) {
				for (i=0;i<4;i++) {
					memcpy(&pt,
						sel->refvertices+sel->refpoly->vertex[i],12);
					if (sel->isxobj) { 
						pt.x+=sel->refnode->x;
						pt.y+=sel->refnode->y;
						pt.z+=sel->refnode->z;
					}
					if (fabs(pt.x-sel->origpos[i].x)+
						fabs(pt.y-sel->origpos[i].y)+
						fabs(pt.z-sel->origpos[i].z) < 1E-3)
							pDoc->MovePointBy(&pt,dx,dy,dz);
				}
			}
			break;
	}
	InvalidateRect(NULL,TRUE);
	if ((selMode==ID_MODE_POLYGON)&&isxobj) return; // no scroll !
	offsetx=rect.right/2-dragX;
	offsety=rect.bottom/2-dragY;
	for (sel=this;sel!=NULL;sel=sel->multisel)
		if (sel->refnode==&sel->myrefnode) {
			sel->myrefnode.x+=dx; 
			sel->myrefnode.y+=dy; 
			sel->myrefnode.z+=dz;
		}
}


void CT3EDView::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable((!isEmpty)&&(GetDocument()->undoLevel>0));
}

void CT3EDView::OnEditUndo() 
{
	CT3EDDoc *pDoc=GetDocument();
	int lev=pDoc->undoLevel-1;

	if (multisel) delete multisel; multisel=NULL;
	refnode=&myrefnode;
	memcpy(refnode,&(pDoc->undoRefpt[lev]),12);
	refblock=pDoc->undoRefblk[lev];
	offsetx=pDoc->undoOfsx[lev];
	offsety=pDoc->undoOfsy[lev];
	pDoc->PerformUndo(false);
	isTruePoint=FALSE; refpoly=NULL; objno=-1;
	InvalidateRect(NULL,TRUE);
}

void CT3EDView::OnUpdateModeSmoothing(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!isEmpty&&(selMode==ID_MODE_BLOCK||selMode==ID_MODE_EXTPOINT));
}

void CT3EDView::OnModeSmoothing() 
{
	if (selMode==ID_MODE_BLOCK) {
		CSmoothingBox1 dlg;
		dlg.m_sDist=smooth1Start;
		dlg.m_sWidth=smooth1Width;
		dlg.m_extraSmooth=extraSmooth;
		if (dlg.DoModal()!=IDOK) return;
		smooth1Start=dlg.m_sDist;
		smooth1Width=dlg.m_sWidth;
		extraSmooth=dlg.m_extraSmooth;
	}
	if (selMode==ID_MODE_EXTPOINT) {
		CSmoothingBox2 dlg;
		dlg.m_sDist=smooth2Start;
		dlg.m_sWidth=smooth2Width;
		if (dlg.DoModal()!=IDOK) return;
		smooth2Start=dlg.m_sDist;
		smooth2Width=dlg.m_sWidth;
	}
}

void CT3EDView::OnUpdateEditmodesExtrasmoothing(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!isEmpty&&(selMode==ID_MODE_BLOCK));
	pCmdUI->SetCheck(!isEmpty&&extraSmooth);
}

void CT3EDView::OnEditmodesExtrasmoothing() 
{
	extraSmooth=!extraSmooth;	
}

void CT3EDView::OnUpdateToolsDelete(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!isEmpty&&
	   (((selMode==ID_MODE_OBJECT)&&(objno!=-1))||
		((selMode==ID_MODE_POLYGON)&&(refpoly!=NULL))||
		((selMode==ID_MODE_BLOCK)&&(refblock!=-1))));
}

void CT3EDView::OnToolsDelete() 
{
	CDC *dc;
	RECT rect;
	CT3EDDoc *pDoc=GetDocument();

	if (selMode==ID_MODE_BLOCK)
	{
		if (MessageBox("There is no undo for this operation. You really want to delete this block ?","T3ED",MB_YESNO)==IDNO) return;
		refblock=pDoc->DeleteBlock(refblock);
		/*pDoc->trk[refblock].hs_neighbors[0]=0; //Next block is the first of track
		pDoc->trk[refblock].hs_neighbors[1]=refblock-1; //Set previous block as neighbour
		pDoc->trk[0].hs_neighbors[0]=refblock; //Set some visible blocks
		for (i=1;i<6;i++)
			{
				pDoc->trk[refblock].nbdData[i*2-1].blk=i-1;
				pDoc->trk[refblock].nbdData[i*2].blk=refblock-i;
			}*/
		pDoc->RecalcBoundingBox(refblock);
		refnode=&(pDoc->trk[refblock].ptCentre); //Setview to Block Centre
		UpdateRefBlock();
		InvalidateRect(NULL,TRUE);

		return;
	}

	if ((selMode==ID_MODE_POLYGON)&&(!isxobj)&&(refpolyobj==-1)&&
		(refpolychunk<=4)&&!(refpolychunk&1))
		if (pDoc->poly[refblock].sz[refpolychunk]==1) {
			AfxMessageBox("Can't remove the last polygon of a block !");
			return;
		}

	dc=GetDC(); // delete object to avoid flicker
	dc->SelectObject(redPen);
	dc->SetROP2(R2_WHITE);
	GetClientRect(&rect);
	ShowSelectionAt(dc,rect.right/2-offsetx,rect.bottom/2-offsety);
	ReleaseDC(dc);

	// multisel doesn't work here
	if (multisel) { delete multisel; multisel=NULL; }

	myrefnode.x=refnode->x; 
	myrefnode.y=refnode->y; 
	myrefnode.z=refnode->z;
	refnode=&myrefnode;

	pDoc->PrepareNewUndo(refnode,refblock,offsetx,offsety);
	if (selMode==ID_MODE_OBJECT)
		pDoc->DelObject(refblock,isxobj,objchunk,objno);
	else if (selMode==ID_MODE_POLYGON) {
		if (!isxobj&&(refpolychunk==4)&&(refpolyobj==-1)) {
			pDoc->DelTrackPolygon(refblock,refpolyno);
			InvalidateRect(NULL,TRUE);
		} else pDoc->DelPolygon(refblock,isxobj,refpolychunk,refpolyobj,refpolyno);
	}
	objno=-1; refpoly=NULL;
	InvalidateRect(NULL,FALSE);
}

void CT3EDView::OnUpdateToolsDuplicate(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!isEmpty&&
		(((selMode==ID_MODE_OBJECT)&&(bShowObjects))||
		((selMode==ID_MODE_POLYGON)&&(refpoly!=NULL))));
}

void CT3EDView::OnToolsDuplicate() 
{
	CT3EDDoc *pDoc=GetDocument();
	CNewobjDlg newDlg;
	int texture,xtype;
	short flags;
	struct XOBJDATA *x;

	// multisel doesn't work here
	if (multisel) delete multisel; multisel=NULL;

	if (selMode==ID_MODE_OBJECT) { // object new/duplicate
		if (objno==-1) {
			newDlg.qfs=&(pDoc->qfsView);
			newDlg.m_flags=-32768;
			if (newDlg.DoModal()!=IDOK) return;
			isxobj=newDlg.m_xobj;
			objchunk=newDlg.m_chunk;
			texture=newDlg.m_texture;
			flags=newDlg.m_flags;
			xtype=newDlg.xobj_crosstype;
		}
		pDoc->PrepareNewUndo(refnode,refblock,offsetx,offsety);
		if (objno==-1) {
			pDoc->NewObject(refblock,isxobj,objchunk,texture,flags,xtype);
			offsetx=0; offsety=0;
		}
		else pDoc->DuplObject(refblock,isxobj,objchunk,objno);
		InvalidateRect(NULL,(objno==-1));
		if (isxobj==3) {
			objno=pDoc->trk[refblock].nSoundsrc-1;
			// refnode is still &myrefnode
		} else 
		if (isxobj==2) {
			objno=pDoc->trk[refblock].nLightsrc-1;
			// refnode is still &myrefnode
		} else
		if (isxobj==1) {
			objno=pDoc->xobj[4*refblock+objchunk].nobj-1;
			refnode=&(pDoc->xobj[4*refblock+objchunk].obj[objno].ptRef);
		} else {
			objno=pDoc->poly[refblock].obj[objchunk].nobj-1;
			refnode=pDoc->trk[refblock].vert+
				pDoc->poly[refblock].obj[objchunk].poly[objno]->vertex[0];
		}
	} else { // polygon duplicate
		pDoc->PrepareNewUndo(refnode,refblock,offsetx,offsety);
		if (!isxobj&&(refpolychunk==4)&&(refpolyobj==-1))
			pDoc->DuplTrackPolygon(refblock,refpolyno);
		else refpolyno=pDoc->DuplPolygon(refblock,isxobj,refpolychunk,refpolyobj,refpolyno);
		if (isxobj) {
			x=pDoc->xobj[4*refblock+refpolychunk].obj+refpolyobj;
			refpoly=x->polyData+refpolyno;
			refvertices=x->vert;
			refnode=&(x->ptRef);
		} else {
			if (refpolyobj==-1) refpoly=pDoc->poly[refblock].poly[refpolychunk]+refpolyno;
			else refpoly=pDoc->poly[refblock].obj[refpolychunk].poly[refpolyobj]+refpolyno;
			refvertices=pDoc->trk[refblock].vert;
			refnode=refvertices+refpoly->vertex[0];
		}
		InvalidateRect(NULL,TRUE);
	}
}

void CT3EDView::OnExportObject() 
{
	CT3EDDoc *pDoc=GetDocument();
	CString defPath,defName,filename,fname,sStr;
	CSelData *sel;
	struct NOBJDATA NewObjData;

	if (multisel) delete multisel; multisel=NULL;
	sel=this;

	if ((selMode==ID_MODE_OBJECT)&&(sel->objno>=0)) //Object selected ?
	{
		CFileDialog m_ldFile(FALSE,"off",defName,OFN_FILEMUSTEXIST|OFN_HIDEREADONLY,"Object File (*.off)|*.off|All Files (*.*)|*.*||");
		if (m_ldFile.DoModal() != IDOK) return; // Dialogfeld Öffnen zeigen und Ergebnis auffangen
		filename = m_ldFile.GetPathName();// Gewählten Pfad & Dateinamen ermitteln
		fname = m_ldFile.GetFileName();// Gewählten Dateinamen ermitteln
		if (!(GetFileAttributes(filename) == 0xFFFFFFFF))
			if (MessageBox("File already exists. Continue ?","T3ED",MB_OKCANCEL)==IDCANCEL) return;

		pDoc->FindObjMinMax(sel->refblock,sel->isxobj,sel->objchunk,sel->objno);
		pDoc->CreateObjectData(&NewObjData,sel->refblock,sel->isxobj,sel->objchunk,sel->objno,TRUE);
		if (NewObjData.nPoly>0)
			if (pDoc->SaveOffFile(&NewObjData,filename))
			{
				sStr.Format("Object successfully exported into '%s'.",fname);
				statusbar->SetPaneText(0,sStr,TRUE);
			}
			else
				MessageBox("Object export failed.","T3ED",MB_ICONWARNING);
		dofree(NewObjData.Vertices); //Give back memory
		dofree(NewObjData.Polys);
	}
}

void CT3EDView::OnImportObject() 
{
	CString defPath,defName,filename,fname;
	CFileDialog m_ldFile(TRUE,"off",defName,OFN_FILEMUSTEXIST|OFN_HIDEREADONLY,"Object File (*.off)|*.off|All Files (*.*)|*.*||");
	// Dialogfeld Öffnen zeigen und Ergebnis auffangen
	if (m_ldFile.DoModal() != IDOK) return;
	// Gewählten Dateinamen ermitteln
	filename = m_ldFile.GetPathName();
	fname = m_ldFile.GetFileName();

	CT3EDDoc *pDoc=GetDocument();
	CString sStr = "";
	CNewobjDlg newDlg;
	int texture, xtype, objno;
	short flags;
	newDlg.qfs=&(pDoc->qfsView);
	if (newDlg.DoModal()!=IDOK) return;
	
	isxobj=newDlg.m_xobj;
	objchunk=newDlg.m_chunk;
	texture=newDlg.m_texture;
	flags=newDlg.m_flags;
	xtype=newDlg.xobj_crosstype;

	pDoc->PrepareNewUndo(refnode,refblock,offsetx,offsety);
	pDoc->NewObject(refblock,isxobj,objchunk,texture,-32768,xtype); //Basis Object zum Importieren
	offsetx=0; offsety=0;
	if (isxobj==1) //Find number of new object
		objno=pDoc->xobj[4*refblock+objchunk].nobj-1;
	else
		objno=pDoc->poly[refblock].obj[objchunk].nobj-1;

	if (pDoc->ImportOffFile(filename,refblock, isxobj, objchunk, objno))
	{
		OnModeObject();
		sStr.Format("Object '%s' successfully imported.",fname);
		statusbar->SetPaneText(0,sStr,TRUE);
		/* if (isxobj==1) objno=pDoc->xobj[4*refblock+objchunk].nobj-1;
		else objno=pDoc->poly[refblock].obj[objchunk].nobj-1; */
		
		if (multisel) { delete multisel; multisel=NULL; }
		pDoc->FindObjMinMax(refblock,isxobj,objchunk,objno);
		refnode=&pDoc->MemObjMiddle; //Setview to new object center
		InvalidateRect(NULL,TRUE);
	}
	else
	{
		MessageBox("Import failed.");
		pDoc->DelObject(refblock, isxobj, objchunk, objno);
		objno=-1; refpoly=NULL;
		InvalidateRect(NULL,FALSE);
	}
	//OnModeObject();
}

void CT3EDView::OnToolsMoveto() 
{
	CT3EDDoc *pDoc=GetDocument();
	int i;
	CString sStr = "";
	CMoveTo MoveToDlg;
	MoveToDlg.m_dest_x=pDoc->settings.MoveTo_dest_x;
	MoveToDlg.m_dest_y=pDoc->settings.MoveTo_dest_y;
	MoveToDlg.m_dest_z=pDoc->settings.MoveTo_dest_z;
	MoveToDlg.m_chk_no_smooth=pDoc->settings.MoveTo_no_smooth;
	MoveToDlg.block_mode=(selMode==ID_MODE_BLOCK);
	MoveToDlg.m_int_startblock=refblock;
	MoveToDlg.m_int_endblock=refblock;


	if (!(MoveToDlg.DoModal () == IDOK)) return; //Cancel pressed
	{
		pDoc->settings.MoveTo_dest_x=MoveToDlg.m_dest_x;
		pDoc->settings.MoveTo_dest_y=MoveToDlg.m_dest_y;
		pDoc->settings.MoveTo_dest_z=MoveToDlg.m_dest_z;
		pDoc->settings.MoveTo_no_smooth=MoveToDlg.m_chk_no_smooth;
		pDoc->MoveToPoint.DestPt.x=MoveToDlg.m_dest_x;
		pDoc->MoveToPoint.DestPt.y=MoveToDlg.m_dest_y;
		pDoc->MoveToPoint.DestPt.z=MoveToDlg.m_dest_z;
		if ((MoveToDlg.m_int_startblock!=refblock)|(MoveToDlg.m_int_endblock!=refblock))
		{
			pDoc->PrepareNewUndo(refnode,refblock,offsetx,offsety);
			for (i=MoveToDlg.m_int_startblock;i<(MoveToDlg.m_int_endblock+1);i++)
			{
				if ((MoveToDlg.m_chk_no_smooth)&&(i<pDoc->nBlocks)) pDoc->MoveBlockSimple(i,MoveToDlg.m_dest_x,MoveToDlg.m_dest_y,MoveToDlg.m_dest_z);
			}
			sStr.Format("Block %d to %d moved.",MoveToDlg.m_int_startblock,MoveToDlg.m_int_endblock);
			statusbar->SetPaneText(0,sStr,TRUE);
			refnode=&(pDoc->trk[refblock].ptCentre);
			sStr.Format("X:%f Y:%f Z:%f InMemZ:%f", refnode->x, refnode->y, refnode->z, pDoc->memory_Zh );
			statusbar->SetPaneText(3,sStr,TRUE);
			UpdateRefBlock();
			InvalidateRect(NULL,TRUE);
		}
		else //Move one Block, or object, or point
		{
			pDoc->MoveToPoint.DestPointSet=TRUE;
			OnLButtonUp(0,NULL);
			pDoc->MoveToPoint.DestPointSet=FALSE;
		}

		if (selMode==ID_MODE_BLOCK) refnode=&(pDoc->trk[refblock].ptCentre); //Setview to Block Centre
		OnViewRecenter();
		//InvalidateRect(NULL,TRUE);
	}	
}

void CT3EDView::OnSetVis() 
{
	CT3EDDoc *pDoc=GetDocument();
	CSetVisiDlg SetVisi;
	int start, end;

	//Last settings
	SetVisi.m_int_num_blocks=pDoc->settings.setvisi_int_num_blocks;
	SetVisi.m_int_tr_bl_fromto=pDoc->settings.setvisi_int_tr_bl_fromto;
	SetVisi.m_int_end=pDoc->settings.setvisi_int_end;
	SetVisi.m_edit_start=pDoc->settings.setvisi_edit_start;

	if (!(SetVisi.DoModal () == IDOK)) return; //Cancel pressed
	   {
		if (SetVisi.m_edit_start<0) SetVisi.m_edit_start=0;
		if (SetVisi.m_int_end>=pDoc->nBlocks) SetVisi.m_int_end=pDoc->nBlocks-1;
		if (!(SetVisi.m_edit_start<SetVisi.m_int_end)) return;
		//Store settings
		pDoc->settings.setvisi_int_num_blocks=SetVisi.m_int_num_blocks;
		pDoc->settings.setvisi_int_tr_bl_fromto=SetVisi.m_int_tr_bl_fromto;
		pDoc->settings.setvisi_int_end=SetVisi.m_int_end;
		pDoc->settings.setvisi_edit_start=SetVisi.m_edit_start;
		if (SetVisi.m_int_tr_bl_fromto==0) {
			start=refblock;
			end=refblock;}
		if (SetVisi.m_int_tr_bl_fromto==1) {
			start=SetVisi.m_edit_start;
			end=SetVisi.m_int_end;}
		if (SetVisi.m_int_tr_bl_fromto==2) {
			start=0;
			end=pDoc->nBlocks-1;}
		pDoc->SetVisibility(start,end,SetVisi.m_int_num_blocks);
	   }
	   pDoc->bUpdatedScene=FALSE;  //Mark Trackpreview as invalid
	   InvalidateRect(NULL,TRUE);	//Update View
}


void CT3EDView::OnToolsExpand() 
{
	CT3EDDoc *pDoc=GetDocument();
	CSelData *sel;
	CExpandDlg Expand;
	ExpandBlock ExpandB;
	CString sStr;
	FLOATPT OldMax, OldMin;
	//BOOL er;

	//Use default settings
	Expand.m_floatExpMulti_x=pDoc->settings.expand_m_floatExpMulti_x;
	Expand.m_floatExpMulti_y=pDoc->settings.expand_m_floatExpMulti_y;
	Expand.m_floatExpMulti_z=pDoc->settings.expand_m_floatExpMulti_z;
	Expand.m_chk_connected=pDoc->settings.expand_m_chk_connected;
	Expand.m_radio_alignment=pDoc->settings.expand_m_radio_alignment;
	if (multisel) delete multisel; multisel=NULL;
	sel=this;

	if ((selMode==ID_MODE_OBJECT)&&(sel->objno>=0)) //Object selected ?
	  if (pDoc->FindObjMinMax(refblock,isxobj,objchunk,objno)) //Find Minimum & Maximum
	  {
	   if (!(Expand.DoModal () == IDOK)) return; //Cancel pressed
	   {
		   	//Store used settings
			pDoc->settings.expand_m_floatExpMulti_x=Expand.m_floatExpMulti_x;
			pDoc->settings.expand_m_floatExpMulti_y=Expand.m_floatExpMulti_y;
			pDoc->settings.expand_m_floatExpMulti_z=Expand.m_floatExpMulti_z;
			pDoc->settings.expand_m_chk_connected=Expand.m_chk_connected;
			pDoc->settings.expand_m_radio_alignment=Expand.m_radio_alignment;

		   OldMax=pDoc->MemObjMax;
		   OldMin=pDoc->MemObjMin;
		   //sStr.Format("Expand.m_radio_alignment = %i", Expand.m_radio_alignment);
		   //MessageBox(sStr, "Expand-Shrink", MB_OK);
		
		   pDoc->PrepareNewUndo(refnode,refblock,offsetx,offsety); //undo should be possible
		   pDoc->ExpandObject(refblock,isxobj,objchunk,objno,Expand.m_floatExpMulti_x,Expand.m_floatExpMulti_y,Expand.m_floatExpMulti_z); //Do the expand job
		   pDoc->FindObjMinMax(refblock,isxobj,objchunk,objno); //Find new min & max of object
		   if (Expand.m_radio_alignment==0) //Restore top position of object
			   pDoc->MoveObjectBy(refblock,isxobj,objchunk,objno,0,0,(OldMax.z-pDoc->MemObjMax.z));

		   if (Expand.m_radio_alignment==2) //Restore bottom position of object
			   pDoc->MoveObjectBy(refblock,isxobj,objchunk,objno,0,0,(OldMin.z-pDoc->MemObjMin.z));

		   InvalidateRect(NULL,TRUE);
	   }
	  }
	  if (selMode==ID_MODE_BLOCK) //Expand Block
	  {
		ExpandB.m_edit_height=pDoc->settings.expand_bl_m_edit_height;
		ExpandB.m_edit_lenght=pDoc->settings.expand_bl_m_edit_lenght;
		ExpandB.m_edit_width=pDoc->settings.expand_bl_m_edit_width;
	    if (!(ExpandB.DoModal () == IDOK)) return; //Cancel pressed
		pDoc->PrepareNewUndo(refnode,refblock,offsetx,offsety); //undo should be possible
		pDoc->ExpandBlock(refblock,ExpandB.m_edit_lenght, ExpandB.m_edit_width,ExpandB.m_edit_height); //Expand the Block
		
		pDoc->settings.expand_bl_m_edit_height=ExpandB.m_edit_height;
		pDoc->settings.expand_bl_m_edit_lenght=ExpandB.m_edit_lenght;
		pDoc->settings.expand_bl_m_edit_width=ExpandB.m_edit_width;

		pDoc->RecalcBoundingBox(refblock);
		refnode=&(pDoc->trk[refblock].ptCentre); //Setview to Block Centre
		InvalidateRect(NULL,TRUE);
	  }
}

void CT3EDView::OnToolsInvert() 
{
	CT3EDDoc *pDoc=GetDocument();
	CSelData *sel;
	LPPOLYGONDATA p;
	int vertdata[4], i;
	if (selMode==ID_MODE_POLYGON) //Quick Polygon rotation
	{
		pDoc->PrepareNewUndo(refnode,refblock,offsetx,offsety);
		for (sel=this;sel!=NULL;sel=sel->multisel) 
		{
		  if (sel->isxobj)
			  pDoc->PrepareModifyXobj(4*sel->refblock+sel->refpolychunk);
		  else 
			  pDoc->PrepareModifyPoly(sel->refblock);
		  p=sel->refpoly;
		  for (i=0;i<4;i++)
			  vertdata[i]=p->vertex[i];
		  p->vertex[0]=vertdata[1];
		  p->vertex[1]=vertdata[0];
		  p->vertex[2]=vertdata[3];
		  p->vertex[3]=vertdata[2];
		  InvalidateRect(NULL,TRUE);
		}
	}
}

void CT3EDView::OnToolsRotate() 
{
	CT3EDDoc *pDoc=GetDocument();
	CSelData *sel;
	CRotateDlg RotateDlg;
	CString sStr;
	LPPOLYGONDATA p;
	int i,vertexnr;
	//double pi = 3.1415926535;
	//double rangle, sinrangle;
	
	if (selMode==ID_MODE_POLYGON) //Quick Polygon rotation
	{
		pDoc->PrepareNewUndo(refnode,refblock,offsetx,offsety);
		for (sel=this;sel!=NULL;sel=sel->multisel) 
		{
		  if (sel->isxobj)
			pDoc->PrepareModifyXobj(4*sel->refblock+sel->refpolychunk);
		  else pDoc->PrepareModifyPoly(sel->refblock);
		  p=sel->refpoly;
		  vertexnr=p->vertex[0];
		  for (i=0;i<3;i++)
			  p->vertex[i]=p->vertex[i+1];
		  p->vertex[3]=vertexnr;
		  InvalidateRect(NULL,TRUE);
		}
	}
	if ((selMode==ID_MODE_OBJECT)&&(objno!=-1)) //Object selected ?
	{
		if (multisel) delete multisel; multisel=NULL;
		sel=this;
	   RotateDlg.m_edit_angle=pDoc->settings.rotate_m_edit_angle;
	   RotateDlg.m_str_rtext="Rotate object with an angle of : ";
	   if (!(RotateDlg.DoModal () == IDOK)) return; //Cancel pressed ?
	   {
		   pDoc->settings.rotate_m_edit_angle=RotateDlg.m_edit_angle;
		   //rangle=((RotateDlg.m_edit_angle/180) * pi);
		   //sinrangle=sin( rangle );
		   //sStr.Format("RotateDlg.m_edit_angle = %f",RotateDlg.m_edit_angle );
		   //MessageBox(sStr, "T3ED Rotate", MB_OK);	
		   pDoc->PrepareNewUndo(refnode,refblock,offsetx,offsety); //undo should be possible
		   //pDoc->ExpandObject(refblock,isxobj,objchunk,objno,Expand.m_floatExpMulti_x,Expand.m_floatExpMulti_y,Expand.m_floatExpMulti_z); //Do the expand job
		   pDoc->FindObjMinMax(refblock,isxobj,objchunk,objno); //Find new min & max of object
		   pDoc->RotateObject(refblock,isxobj,objchunk,objno,RotateDlg.m_edit_angle); //Rotate object
		   InvalidateRect(NULL,TRUE);
	   }
	}

	if ((selMode==ID_MODE_BLOCK)&&(refblock>-1)) //Block selected ?
	{
	   if (multisel) delete multisel; multisel=NULL;
	   sel=this;
	   RotateDlg.m_edit_angle=pDoc->settings.rotate_m_edit_angle;
	   RotateDlg.m_str_rtext="Rotate block with an angle of : ";
	   if (!(RotateDlg.DoModal () == IDOK)) return; //Cancel pressed ?
	   {
		   pDoc->settings.rotate_m_edit_angle=RotateDlg.m_edit_angle;
		   //rangle=((RotateDlg.m_edit_angle/180) * pi);
		   //sinrangle=sin( rangle );
		   //sStr.Format("RotateDlg.m_edit_angle = %f",RotateDlg.m_edit_angle );
		   //MessageBox(sStr, "T3ED Rotate", MB_OK);	
		   pDoc->PrepareNewUndo(refnode,refblock,offsetx,offsety); //undo should be possible
		   pDoc->RotateBlock(refblock,RotateDlg.m_edit_angle);
		   //pDoc->ExpandObject(refblock,isxobj,objchunk,objno,Expand.m_floatExpMulti_x,Expand.m_floatExpMulti_y,Expand.m_floatExpMulti_z); //Do the expand job
		   //pDoc->FindObjMinMax(refblock,isxobj,objchunk,objno); //Find new min & max of object
		   //pDoc->RotateObject(refblock,isxobj,objchunk,objno,RotateDlg.m_edit_angle); //Rotate object
		   InvalidateRect(NULL,TRUE);
	   }
	}
}

void CT3EDView::OnUpdateToolsTexture(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!isEmpty&&
	   (((selMode==ID_MODE_OBJECT)&&(objno!=-1))||
		((selMode==ID_MODE_POLYGON)&&(refpoly!=NULL))||(selMode==ID_MODE_BLOCK)));
}

void CT3EDView::OnToolsTexture() 
{
	CT3EDDoc *pDoc=GetDocument();
	CQFSView *qfs=&(pDoc->qfsView);
	CSelData *sel;
	CString sStr = "Test";
	LPPOLYGONDATA p;
	struct OBJPOLYBLOCK *o;
	struct XOBJDATA *x;
	int mintex=9999;
	int k=0, l=0;
	int offset=0;

	if (selMode==ID_MODE_POLYGON)
	{
	  //Shadingtmp unsigned long;
	  qfs->m_nBmp=refpoly->texture;
	  qfs->m_flags=refpoly->hs_textflags;
	 //
	  qfs->texanimdata=refpoly->texanimdata; //Nappe1: Should work for NFSHS ANIMData...
	  qfs->bHasHitFlags=FALSE;
	  sStr.Format("Select new texture. Currently number %u is used.",refpoly->texture);
	  qfs->m_strLabel=sStr;
	  if (qfs->DoModal()!=IDOK) return;
	  pDoc->PrepareNewUndo(refnode,refblock,offsetx,offsety);
	  for (sel=this;sel!=NULL;sel=sel->multisel) {
		if (sel->isxobj)
			pDoc->PrepareModifyXobj(4*sel->refblock+sel->refpolychunk);
		else pDoc->PrepareModifyPoly(sel->refblock);
		sel->refpoly->texture=qfs->m_nBmp;
		
		if (pDoc->bHSMode && qfs->bHasHitFlags)
			{
			sel->refpoly->hs_textflags=qfs->m_flags;
			sel->refpoly->texanimdata=qfs->texanimdata; //Nappe1: updating AnimData to the refpoly.
			}
	  }
	}
	if (selMode==ID_MODE_OBJECT) //JimD: Now it's possible to change all textures of an object
	{
		  for (sel=this;sel!=NULL;sel=sel->multisel) //look at all objects
			if ((sel->isxobj==0)||(sel->isxobj==1)) //Only available for objects
			  mintex=pDoc->MinTexObject(sel->refblock,sel->isxobj,sel->objchunk,sel->objno,mintex); // Get the lowest texture number from current object
		  qfs->m_nBmp=mintex; //Select lowest as default
		  sel=this;
		  if (isxobj==0) //set pointer to first polygon of object
			{
			  o=&pDoc->poly[sel->refblock].obj[sel->objchunk];
			  p=o->poly[sel->objno];
			}
		  if (isxobj==1) //set pointer to first polygon of extra object
			{
			  x=&pDoc->xobj[4*sel->refblock+sel->objchunk].obj[sel->objno];
			  p=x->polyData;
			}

		  qfs->m_flags=p->hs_textflags;
		  qfs->texanimdata=p->texanimdata; //Nappe1: Should work for NFSHS ANIMData...
		  qfs->bHasHitFlags=FALSE;

	      sStr.Format("Select new textures for object. Currently number %u is the lowest.",mintex);
	      qfs->m_strLabel=sStr;
	      if (qfs->DoModal()!=IDOK) return; // End if cancel was pressed
		  offset=qfs->m_nBmp-mintex; // recalc offset for changes
		  for (sel=this;sel!=NULL;sel=sel->multisel)
			if ((sel->isxobj==0)||(sel->isxobj==1)) //Only available for objects
			{
			  pDoc->PrepareNewUndo(sel->refnode,sel->refblock,offsetx,offsety); //undo should be possible
			  pDoc->ChangeTexObject(sel->refblock,sel->isxobj,sel->objchunk,sel->objno,offset,(pDoc->bHSMode && qfs->bHasHitFlags),qfs->m_flags,qfs->texanimdata); //change poly or extra object
			}
	}
	if (selMode==ID_MODE_BLOCK)
	{
		for (sel=this;sel!=NULL;sel=sel->multisel)
			mintex=pDoc->MinTexBlock(sel->refblock,mintex);
		qfs->m_nBmp=mintex; //Select lowest as default
	    sStr.Format("Select new textures for Trackblock polygons. Currently number %u is the lowest.",mintex);
	    qfs->m_strLabel=sStr;
		if (qfs->DoModal()!=IDOK) return; // End if cancel was pressed
		offset=qfs->m_nBmp-mintex; // recalc offset for changes
		for (sel=this;sel!=NULL;sel=sel->multisel)
		{
		  pDoc->PrepareNewUndo(sel->refnode,sel->refblock,offsetx,offsety); //undo should be possible
		  pDoc->ChangeTexBlock(sel->refblock,offset); //change textures of blockpolygons
		}
	}
	//HOO: (5)
	InvalidateRect(NULL,false);
	//HOO: (5)
}

//Nappe1: (DEV2.5 ADD ON) NFS HS NEIGHBOUR EDITING
void CT3EDView::OnToolsTrackBlockProps()
{
	CT3EDDoc *pDoc=GetDocument();
	CBlockProps propsdlg;
	int i;
	//CSelData *sel;
	//CString teksti = 'tekstiä';

	struct TRKBLOCK blk;
	//int nulld;
	//float nullf;

	blk=pDoc->trk[refblock];
	propsdlg.m_Neigh1=blk.hs_neighbors[0];
	propsdlg.m_Neigh2=blk.hs_neighbors[1];
	propsdlg.m_Neigh3=blk.hs_neighbors[2];
	propsdlg.m_Neigh4=blk.hs_neighbors[3];
	propsdlg.m_Neigh5=blk.hs_neighbors[4];
	propsdlg.m_Neigh6=blk.hs_neighbors[5];
	propsdlg.m_Neigh7=blk.hs_neighbors[6];
	propsdlg.m_Neigh8=blk.hs_neighbors[7];
	propsdlg.m_bool_merge=pDoc->settings.ConnectBlocks;
	propsdlg.m_float_Dist=pDoc->settings.ConnectBlocks_dist;
	//propsdlg.m_Neigh9=data.hs_neighbors[8];
	//propsdlg.m_clist1.AddString(teksti);

				

	if (propsdlg.DoModal()!=IDOK) return;
	pDoc->PrepareNewUndo(refnode,refblock,offsetx,offsety);
	pDoc->PrepareModifyTrk(refblock);
	
	pDoc->trk[refblock].hs_neighbors[0]=propsdlg.m_Neigh1;
	pDoc->trk[refblock].hs_neighbors[1]=propsdlg.m_Neigh2;
	pDoc->trk[refblock].hs_neighbors[2]=propsdlg.m_Neigh3;
	pDoc->trk[refblock].hs_neighbors[3]=propsdlg.m_Neigh4;
	pDoc->trk[refblock].hs_neighbors[4]=propsdlg.m_Neigh5;
	pDoc->trk[refblock].hs_neighbors[5]=propsdlg.m_Neigh6;
	pDoc->trk[refblock].hs_neighbors[6]=propsdlg.m_Neigh7;
	pDoc->trk[refblock].hs_neighbors[7]=propsdlg.m_Neigh8;
	pDoc->settings.ConnectBlocks=propsdlg.m_bool_merge;
	pDoc->settings.ConnectBlocks_dist=propsdlg.m_float_Dist;
	
	if (!propsdlg.m_bool_merge) return;
	pDoc->PrepareNewUndo(refnode,refblock,offsetx,offsety); //undo should be possible
	pDoc->PrepareModifyTrk(refblock);
	for (i=0;i<8;i++)
		if (pDoc->trk[refblock].hs_neighbors[i]>-1)
		{
			pDoc->PrepareModifyTrk(pDoc->trk[refblock].hs_neighbors[i]);
			pDoc->ConnectBlocks(refblock, pDoc->trk[refblock].hs_neighbors[i], propsdlg.m_float_Dist);			
			pDoc->RecalcBoundingBox(refblock);
			refnode=&(pDoc->trk[refblock].ptCentre); //Setview to Block Centre
			InvalidateRect(NULL,TRUE);
		}
}
//Nappe1: (DEV2.5 ADD ON) NFS HS NEIGHBOUR EDITING (END)

void CT3EDView::OnToolsVroadHeightsSpdfiles() 
{
	
	CVRoadHeightsSpdFileDlg VRHSFDlg;

	struct TRKBLOCK blk;
	CT3EDDoc *pDoc=GetDocument();
	
	blk=pDoc->trk[refblock];
	if ((blk.nPositions==0)||(!pDoc->spdFALoaded)||(!pDoc->spdRALoaded)||(!pDoc->HeightsLoaded)) return;

	//Copy vRoad Info & Data 
	VRHSFDlg.m_nPositions=blk.nPositions;
	VRHSFDlg.m_colvrdata=(struct COLVROAD *)malloc(blk.nPositions*sizeof(struct COLVROAD));
	memcpy(VRHSFDlg.m_colvrdata,&pDoc->col.vroad[blk.nStartPos],blk.nPositions*sizeof(struct COLVROAD));

	VRHSFDlg.m_heightsdata=(struct HEIGHTSSIM *)malloc(blk.nPositions*sizeof(struct HEIGHTSSIM));
	memcpy(VRHSFDlg.m_heightsdata,&pDoc->hightssim[blk.nStartPos],blk.nPositions*sizeof(struct HEIGHTSSIM));

	VRHSFDlg.spdFAdata=(struct SPDFILE *)malloc(blk.nPositions*sizeof(struct SPDFILE));
	memcpy(VRHSFDlg.spdFAdata,&pDoc->spdFAbin[blk.nStartPos],blk.nPositions*sizeof(struct SPDFILE));

	VRHSFDlg.spdRAdata=(struct SPDFILE *)malloc(blk.nPositions*sizeof(struct SPDFILE));
	memcpy(VRHSFDlg.spdRAdata,&pDoc->spdRAbin[blk.nStartPos],blk.nPositions*sizeof(struct SPDFILE));
	
	VRHSFDlg.m_Chk_Heights=pDoc->settings.VRoadHeightsSpd_Chk_Heights;
	VRHSFDlg.m_Chk_SpdFa=pDoc->settings.VRoadHeightsSpd_Chk_SpdFa;
	VRHSFDlg.m_Chk_SpdRa=pDoc->settings.VRoadHeightsSpd_Chk_SpdRa;
	VRHSFDlg.m_Chk_VRoad=pDoc->settings.VRoadHeightsSpd_Chk_VRoad;

	if (VRHSFDlg.DoModal()!=IDOK) return; //Show Dialog.

	pDoc->settings.VRoadHeightsSpd_Chk_Heights=VRHSFDlg.m_Chk_Heights;
	pDoc->settings.VRoadHeightsSpd_Chk_SpdFa=VRHSFDlg.m_Chk_SpdFa;
	pDoc->settings.VRoadHeightsSpd_Chk_SpdRa=VRHSFDlg.m_Chk_SpdRa;
	pDoc->settings.VRoadHeightsSpd_Chk_VRoad=VRHSFDlg.m_Chk_VRoad;

	pDoc->PrepareNewUndo(refnode,refblock,offsetx,offsety);
	//pDoc->PrepareModifyTrk(refblock);
	pDoc->PrepareModifyVRoadHeightsSpdFiles();
	//Copy back changed data	
	memcpy(&pDoc->col.vroad[blk.nStartPos],VRHSFDlg.m_colvrdata,blk.nPositions*sizeof(struct COLVROAD));
	memcpy(&pDoc->hightssim[blk.nStartPos],VRHSFDlg.m_heightsdata,blk.nPositions*sizeof(struct HEIGHTSSIM));
	memcpy(&pDoc->spdFAbin[blk.nStartPos],VRHSFDlg.spdFAdata,blk.nPositions*sizeof(struct SPDFILE));
	memcpy(&pDoc->spdRAbin[blk.nStartPos],VRHSFDlg.spdRAdata,blk.nPositions*sizeof(struct SPDFILE));

	dofree(VRHSFDlg.m_colvrdata);
	dofree(VRHSFDlg.m_heightsdata);
	dofree(VRHSFDlg.spdFAdata);
	dofree(VRHSFDlg.spdRAdata);

	InvalidateRect(NULL,TRUE);
}

void CT3EDView::OnToolsPolyflag() 
{
	CT3EDDoc *pDoc=GetDocument();
	CPolyFlags polydlg;
	struct POLYVROADDATA *data;
	//LPPOLYGONDATA p;
	int i;
	CSelData *sel;

	data=pDoc->trk[refblock].polyData+refpolyno;
	//polydlg.m_vredge=data->virtualroadedge;
	polydlg.m_walldetect=((data->flags&0x80)!=0);
	polydlg.m_xobjdetect=((data->flags&0x40)!=0);
	polydlg.m_type=(data->flags&0x1f);
	polydlg.bHSMode=pDoc->bHSMode;
	polydlg.bTypeOnly=FALSE;
	if ((pDoc->bHSMode)&&((data->flags&0x0f)%14!=0)) {
		polydlg.m_hsForwNeighbor=(data->hs_orphan[0]!=0);
		polydlg.m_hsBackNeighbor=(data->hs_orphan[2]!=0);
		polydlg.m_hsLeftNeighbor=(data->hs_orphan[1]!=0);
		polydlg.m_hsRightNeighbor=(data->hs_orphan[3]!=0);
	}
	if (multisel) { 
		polydlg.m_walldetect=2;
		polydlg.m_xobjdetect=2;
		polydlg.m_hsForwNeighbor=2;
		polydlg.m_hsBackNeighbor=2;
		//polydlg.m_vredge=2;
	}
	
	if (polydlg.DoModal()!=IDOK) return;

	pDoc->PrepareNewUndo(refnode,refblock,offsetx,offsety);

	for (sel=this;sel!=NULL;sel=sel->multisel) 
		if ((sel->refpolyobj==-1)&&(sel->refpolychunk==4))
	{
		pDoc->PrepareModifyTrk(sel->refblock);
		data=pDoc->trk[sel->refblock].polyData+sel->refpolyno;

		if ((pDoc->bHSMode)&&(polydlg.m_type%14!=0)) {
			if (polydlg.m_hsForwNeighbor<=1)
				data->hs_orphan[0]=polydlg.m_hsForwNeighbor?0xFF:0;
			if (polydlg.m_hsBackNeighbor<=1)
				data->hs_orphan[2]=polydlg.m_hsBackNeighbor?0xFF:0;
			if (polydlg.m_hsLeftNeighbor<=1)
				data->hs_orphan[1]=polydlg.m_hsLeftNeighbor?0xFF:0;
			if (polydlg.m_hsRightNeighbor<=1)
				data->hs_orphan[3]=polydlg.m_hsRightNeighbor?0xFF:0;

		}
		//data->virtualroadedge=polydlg.m_vredge;
		data->flags=(data->flags&0x70)+polydlg.m_type;
		if (polydlg.m_xobjdetect==1) data->flags|=0x40;
		if (polydlg.m_xobjdetect==0) data->flags&=0xbf;
		if (polydlg.m_walldetect==1) data->flags|=0x80;
		if (polydlg.m_walldetect==0) data->flags&=0x7f;
		if ((polydlg.m_type%14!=0)&&((data->flags&0x1f)%14==0))
			data->flags|=0x80; // made it passable : beware of neighbors

		int refnodeno = 0;
		int n = sel->refpolyno;
		for (i = 0; i < pDoc->trk[sel->refblock].nPositions; i++)
		{
			if (n < pDoc->trk[sel->refblock].posData[i].nPolygons)
				break;

			n -= pDoc->trk[sel->refblock].posData[i].nPolygons;
			refnodeno++;
		}

		pDoc->CorrectVirtualRoad(sel->refblock, refnodeno);
	}
	InvalidateRect(NULL, TRUE);
	RedrawWindow();
}

void CT3EDView::OnUpdateToolsPolyflag(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!isEmpty&&(selMode==ID_MODE_POLYGON)&&(refpoly!=NULL)
		&&(refpolyobj==-1)&&(refpolychunk==4));
}

void CT3EDView::OnToolsMerge() 
{
	if (isDragging) return; // safety
	if (editMode==ID_TOOLS_MERGE) editMode=NULL;
	else editMode=ID_TOOLS_MERGE;
	if (multisel) {
		CleanCursorZone(selMode,FALSE);
		delete multisel; multisel=NULL;
	}
}

void CT3EDView::OnUpdateToolsMerge(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!isEmpty&&((selMode==ID_MODE_POINT)|(selMode==ID_MODE_OBJECT)));
	pCmdUI->SetCheck(editMode==ID_TOOLS_MERGE);
}

void CT3EDView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CT3EDDoc *pDoc=GetDocument();

	if (editMode!=ID_TOOLS_MERGE) return;
	if (selMode!=ID_MODE_POINT) return;
	isTruePoint=FALSE;
	OnLButtonDown(0,point); // select point, recenter...
	if (!isTruePoint) return;
	isTruePoint=FALSE;
	myrefnode.x=refnode->x;
	myrefnode.z=refnode->z;
	myrefnode.y=refnode->y;
	refnode=&myrefnode;
	pDoc->PrepareNewUndo(refnode,refblock,offsetx,offsety);
	pDoc->SplitPoint(refnode);
	InvalidateRect(NULL,TRUE);
}

void CT3EDView::OnToolsAdjustwidth() 
{
	CT3EDDoc *pDoc=GetDocument();
	CVirtualRoadAdjust VRADlg;	
	
	//Get settings
	VRADlg.m_AdjustType = pDoc->settings.AdjustRoad_AdjustType;
	VRADlg.m_StartBlock = pDoc->settings.AdjustRoad_StartBlock;
	VRADlg.m_EndBlock = pDoc->settings.AdjustRoad_EndBlock;

	if (VRADlg.DoModal()!=IDOK) return;

	if (VRADlg.m_StartBlock<0) VRADlg.m_StartBlock=0;
	if (VRADlg.m_EndBlock > (pDoc->nBlocks-1)) VRADlg.m_EndBlock = GetDocument()->nBlocks-1;
	//Save settings
	pDoc->settings.AdjustRoad_AdjustType=VRADlg.m_AdjustType;
	pDoc->settings.AdjustRoad_StartBlock=VRADlg.m_StartBlock; 
	pDoc->settings.AdjustRoad_EndBlock=VRADlg.m_EndBlock;

	GetDocument()->PrepareNewUndo(refnode,refblock,offsetx,offsety);
	GetDocument()->AdjustRoadWidth(VRADlg.m_AdjustType, VRADlg.m_MapLaneEdges, VRADlg.m_StartBlock , VRADlg.m_EndBlock,VRADlg.m_float_roadwidth );
	InvalidateRect(NULL,FALSE);
}

void CT3EDView::OnUpdateToolsAdjustwidth(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!isEmpty);	
}

void CT3EDView::OnToolsClearall() 
{
	CT3EDDoc *pDoc=GetDocument();
	CClearAll clearDlg;
	int i,j;
	struct XOBJBLOCK *x;

	clearDlg.qfs=&(pDoc->qfsView);
	if (clearDlg.DoModal()!=IDOK) return;

	pDoc->PrepareNewUndo(refnode,refblock,offsetx,offsety);
	
	//ADDON By Nappe1: Cutting amount of track blocks:
	if (clearDlg.m_trackBLK) {
		if ((clearDlg.m_TrknBlocks >19) && (clearDlg.m_TrknBlocks < pDoc->nBlocks)) 
		{
			pDoc->CutAmountOfBlocks(clearDlg.m_TrknBlocks);
			//pDoc->nBlocks=clearDlg.m_TrknBlocks;
			//pDoc->col.vroadHead.nrec=8*(short)pDoc->nBlocks;
			//pDoc->col.vroadHead.nrec=pDoc->trk[pDoc->nBlocks-1].nStartPos+pDoc->trk[pDoc->nBlocks-1].nPositions; //Should be the same as above, but this must be right !
		}
		else MessageBox ("Track has to have at least 19 blocks and cannot have more than in the original one.");
	}
	//Nappe1: ADDITION ENDS.


	// one never knows what happens here
	myrefnode.x=refnode->x;
	myrefnode.z=refnode->z;
	myrefnode.y=refnode->y;
	refnode=&myrefnode;
	isTruePoint=FALSE;
	refpoly=NULL; objno=-1;
	//pDoc->col.nBlocks=80;
	//pDoc->nBlocks=10;
	
	//ADDON By Nappe1: setting up the shading vertices to FF FF FF FF as BGRA
	if (clearDlg.m_trackShade) {
		//Functions by Vitaly Kootin.	
		pDoc->SetRoadVertexShading(0, pDoc->nBlocks-1, 0xFFFFFFFF);
		pDoc->SetXObjVertexShading(0, pDoc->nBlocks-1, 0xFFFFFFFF);
	}
	//Nappe1: ADDITION ENDS.

	if (clearDlg.m_xobj)
		for (i=0;i<pDoc->nBlocks;i++)
			pDoc->DelAllObjects(i,TRUE);

	if (clearDlg.m_polyobj)
		for (i=0;i<pDoc->nBlocks;i++)
			pDoc->DelAllObjects(i,FALSE);

	if (clearDlg.m_fences) {
		for (i=0;i<pDoc->nBlocks;i++)
			pDoc->DelLanePolygons(i);
	}

	if (clearDlg.m_trackXY||clearDlg.m_trackZ) {
		pDoc->MoveBlocks(-1,0,0,0,clearDlg.m_trackXY,clearDlg.m_trackZ,-1);
		refnode=&(pDoc->trk[refblock].ptCentre);
		offsetx=0; offsety=0;
	}

	if (clearDlg.m_sceneryZ)
		pDoc->ClearSceneryZ();

	if (clearDlg.m_roadTextures)
		pDoc->SetAllTextures(clearDlg.m_roadTexture,TRUE,clearDlg.m_roadFlags);

	if (clearDlg.m_sceneryTextures)
		pDoc->SetAllTextures(clearDlg.m_sceneryTexture,FALSE,clearDlg.m_sceneryFlags);

	if (clearDlg.m_globalobj) {
		// can't keep consistent undo, so delete it all
		for (i=0;i<pDoc->undoLevel;i++) pDoc->DeleteUndo(i);
		pDoc->undoLevel=0;
		// global xobj
		x=&(pDoc->xobj[4*pDoc->nBlocks]);
		pDoc->FreeXobjContents(x);
		x->obj=NULL;
		x->nobj=0;
		if (pDoc->hs_morexobjlen>0) {
			pDoc->hs_morexobj=(char *)realloc(pDoc->hs_morexobj,8);
			pDoc->hs_morexobjlen=8; // two 0L's
			memset(pDoc->hs_morexobj,0,8);
		}
		// COL objects
		if (pDoc->col.struct3D!=NULL) 
			for (i=0;i<pDoc->col.struct3DHead.nrec;i++) {
				dofree(pDoc->col.struct3D[i].vertex);
				dofree(pDoc->col.struct3D[i].polygon);
			}
		dofree(pDoc->col.struct3D);
		pDoc->col.struct3D=NULL;
		if (pDoc->col.object!=NULL)
			for (i=0;i<pDoc->col.objectHead.nrec;i++)
				dofree(pDoc->col.object[i].animData);
		dofree(pDoc->col.object);
		pDoc->col.object=NULL;
		if (pDoc->col.object2!=NULL)
			for (i=0;i<pDoc->col.object2Head.nrec;i++)
				dofree(pDoc->col.object2[i].animData);
		dofree(pDoc->col.object2);
		pDoc->col.object2=NULL;
		pDoc->col.nBlocks=2;
		pDoc->col.fileLength=24+pDoc->col.textureHead.size+pDoc->col.vroadHead.size;
		pDoc->col.xbTable[0]=8;
		pDoc->col.xbTable[1]=8+pDoc->col.textureHead.size;
	}
	if (clearDlg.m_block_neighbours)
	{
		for (i=0;i<pDoc->nBlocks;i++)
		{
			pDoc->trk[i].hs_neighbors[0]=i+1; //Next block
			if (pDoc->trk[i].hs_neighbors[0]>=pDoc->nBlocks)
				pDoc->trk[i].hs_neighbors[0]=0;
			pDoc->trk[i].hs_neighbors[1]=i - 1; //Previous block
			if (pDoc->trk[i].hs_neighbors[1]<0)
				pDoc->trk[i].hs_neighbors[1]=pDoc->nBlocks - 1;
			for (j=2;j<8;j++)
				pDoc->trk[i].hs_neighbors[j]=-1;

		}	
		MessageBox ("Default Block Neighbours");
	}
	
	InvalidateRect(NULL);
}

void CT3EDView::OnUpdateToolsClearall(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!isEmpty);	
}

void CT3EDView::OnViewVroad() 
{
	DrawVRoad=!DrawVRoad;

}

void CT3EDView::OnUpdateViewVroad(CCmdUI* pCmdUI) 
{

	pCmdUI->Enable(!isEmpty);
	pCmdUI->SetCheck(DrawVRoad&&!isEmpty);
	InvalidateRect(NULL,TRUE);
	// TODO: Add your command update UI handler code here
	
}

//Nappe1: (DEV2.5 ADD ON) TRACK VISIBILITY TABLE EXPORT/IMPORT


void CT3EDView::OnExportVis() 
{
	bool list_END;
	// TODO: Add your command handler code here
	//MessageBox("alussa");
	CT3EDDoc *pDoc=GetDocument();
	//MessageBox("dokumentti saatu");


FILE* fout = fopen("c:/visibility.txt", "w");
		//MessageBox("Tiedosto auki.");
		fprintf(fout, "Version: %d.%d\n", 1,2);
		fprintf(fout, "Blocks: %d\n", pDoc->nBlocks);
		for(int i=0;i<pDoc->nBlocks ;i++)
		{
			fprintf(fout, "Block: %d\n", i);
			/*fprintf(fout, "%f\n", pDoc->trk[i].ptBounding[0].x);
			fprintf(fout, "%f\n", pDoc->trk[i].ptBounding[0].y);
			fprintf(fout, "%f\n", pDoc->trk[i].ptBounding[0].z);
			
			fprintf(fout, "%f\n", pDoc->trk[i].ptBounding[1].x);
			fprintf(fout, "%f\n", pDoc->trk[i].ptBounding[1].y);
			fprintf(fout, "%f\n", pDoc->trk[i].ptBounding[1].z);
			
			fprintf(fout, "%f\n", pDoc->trk[i].ptBounding[2].x);
			fprintf(fout, "%f\n", pDoc->trk[i].ptBounding[2].y);
			fprintf(fout, "%f\n", pDoc->trk[i].ptBounding[2].z);

			fprintf(fout, "%f\n", pDoc->trk[i].ptBounding[3].x);
			fprintf(fout, "%f\n", pDoc->trk[i].ptBounding[3].y);
			fprintf(fout, "%f\n", pDoc->trk[i].ptBounding[3].z); */
			list_END=false;
			for(int u=0;u<300 ;u++)
				{
				if (pDoc->trk[i].nbdData[u].blk!=-1)
					{
					if (list_END==false)
						fprintf(fout, "%d\n", pDoc->trk[i].nbdData[u].blk);
					}
				else 
					{
					if (list_END==false)
						fprintf(fout, "%d\n\n", pDoc->trk[i].nbdData[u].blk);
					list_END=true;
					}

				}
		}
		fclose(fout);
		MessageBox("Table Exported.");	
}

void CT3EDView::OnImportVis() 
{
	int nulld, nulld2;
	//float nullf;
	bool version_OK, list_END;
	short value;
	//char dump;
	CString s;

	CT3EDDoc *pDoc=GetDocument();
	// TODO: Add your command handler code here
	
	//pDoc->PrepareNewUndo(refnode,refblock,offsetx,offsety);
	//pDoc->PrepareModifyTrk(refblock);
	
	FILE* fin = fopen("c:/visibility.txt", "r");
	//fscanf(fin, "%d\n%c\n%f\n%s", &(arg->a), &(arg->b), &(arg->c), arg->d);
	//fclose(fin);
	if (fin==NULL)
		{
		version_OK=false;
		MessageBox("File open Error.");
		}
	else
		version_OK=true;

	/*for (loop=1;loop==9;loop++)
		fscanf(fin, "%c", &dump);*/
	if (version_OK==true)
		{
		fscanf(fin, "Version: %d.%d\n", &nulld, &nulld2);
		//s.Format("nulld: %d, nulld2: %d",nulld, nulld2);
		//MessageBox(s);

		if (nulld2!=2)
			{
			MessageBox("Wrong version of export file. Import abortted");
			version_OK=false;
			fclose(fin);
			}
		}
	if (version_OK==true)
		{
		fscanf(fin, "Blocks: %d\n", &nulld);
		if (nulld!=pDoc->nBlocks)
			{
			MessageBox("Export file has different amount of blocks than track.\nNothing imported for safety reasons.");
			version_OK=false;
			fclose(fin);
			}
		}

	if (version_OK==false) return;
	s="";
	for(int ii=0;ii<pDoc->nBlocks ;ii++)
		{
		s="";
		fscanf(fin, "Block: %d\n", &nulld);
		/*fscanf(fin, "%f\n", &(pDoc->trk[ii].ptBounding[0].x));
		fscanf(fin, "%f\n", &(pDoc->trk[ii].ptBounding[0].y));
		fscanf(fin, "%f\n", &(pDoc->trk[ii].ptBounding[0].z));
		
		fscanf(fin, "%f\n", &(pDoc->trk[ii].ptBounding[1].x));
		fscanf(fin, "%f\n", &(pDoc->trk[ii].ptBounding[1].y));
		fscanf(fin, "%f\n", &(pDoc->trk[ii].ptBounding[1].z));
		
		fscanf(fin, "%f\n", &(pDoc->trk[ii].ptBounding[2].x));
		fscanf(fin, "%f\n", &(pDoc->trk[ii].ptBounding[2].y));
		fscanf(fin, "%f\n", &(pDoc->trk[ii].ptBounding[2].z));

		fscanf(fin, "%f\n", &(pDoc->trk[ii].ptBounding[3].x));
		fscanf(fin, "%f\n", &(pDoc->trk[ii].ptBounding[3].y));
		fscanf(fin, "%f\n", &(pDoc->trk[ii].ptBounding[3].z)); */
			
		list_END=false;	
		for(int uu=0;uu<300 ;uu++)
			{
			if (list_END==false)
				fscanf(fin, "%hd\n", &value);
		
			if (value==-1)
				{
				if (list_END==false)
					{
					list_END=true;
					fscanf(fin, "\n");
					}
				}
			pDoc->trk[ii].nbdData[uu].blk=value;
			/*if (ii<5)
				{
				s.Format("Value: %d, Entry: %d\n",value, uu);
				MessageBox(s);
				} */
			}
		}
	fclose(fin);
	MessageBox("Table Imported.");
	pDoc->bUpdatedScene=FALSE;  //Mark Trackpreview as invalid
	InvalidateRect(NULL,TRUE);	//Update View
}
//Nappe1: (DEV2.5 ADD ON) TRACK VISIBILITY TABLE EXPORT/IMPORT (END)

void CT3EDView::OnEditCopy() 
{
	// Copy to Clipboard
	CT3EDDoc *pDoc=GetDocument();
	CSelData *sel;
	CString sStr = "";
	int size;
	
	if (selMode==ID_MODE_POINT) //Copy point position to clipboard
	{
		pDoc->CopyXYZToClipboard(refnode);
	}

	if (selMode==ID_MODE_OBJECT) //Copy objects to clipboard
	{
		if (multisel) delete multisel; multisel=NULL;
		sel=this;
		if ((sel!=NULL)&&(isxobj!=4)&&(refblock!=pDoc->nBlocks))
		{
			//MessageBox("Object to Clipboard.");
			size=pDoc->CopyBlockToClipboard(refblock,isxobj,objchunk,objno);
			if (size!=-1)
				{
				switch (isxobj) {
					case 0: 
						sStr.Format("Object %d from block %d (chunk %d) copied.(%d bytes)",objno, refblock, objchunk, size);
						break;
					case 1:
						sStr.Format("Extra object %d from block %d (chunk %d) copied.(%d bytes)",objno, refblock, objchunk, size);
						break;
					case 2:
						sStr.Format("Light object %d from block %d (chunk %d) copied.(%d bytes)",objno, refblock, objchunk, size);
						break;
					case 3:
						sStr.Format("Sound object %d from block %d (chunk %d) copied.(%d bytes)",objno, refblock, objchunk, size);
						break;
				}
			}
			else
			{
				sStr.Format("Copy failed.");
			}
			statusbar->SetPaneText(0,sStr,TRUE);
		}
	}

	if (selMode==ID_MODE_BLOCK ) //Copy block to clipboard
	{
		if (multisel) delete multisel; multisel=NULL;
		sel=this;
		if (sel!=NULL)
		{
			size=pDoc->CopyBlockToClipboard(refblock,-1,-1,-1);
			sStr.Format("Block %d copied to clipboard.(%d bytes)",refblock, size);
			statusbar->SetPaneText(0,sStr,TRUE);
		}
	}
}

void CT3EDView::OnEditCut() 
{
	CT3EDDoc *pDoc=GetDocument();
	CSelData *sel;
	int size;
	CString sStr = "";

	if (selMode==ID_MODE_BLOCK ) //Copy block to clipboard
	{
		if (multisel) delete multisel; multisel=NULL;
		sel=this;
		if (sel!=NULL)
		{
			size=pDoc->CopyBlockToClipboard(refblock,-1,-1,-1);
			sStr.Format("Cut Block %d to clipboard.(%d bytes,no undo)",refblock, size);
			statusbar->SetPaneText(0,sStr,TRUE);

			refblock=pDoc->DeleteBlock(refblock);
			pDoc->RecalcBoundingBox(refblock);
			refnode=&(pDoc->trk[refblock].ptCentre); //Setview to Block Centre
			UpdateRefBlock();
			InvalidateRect(NULL,TRUE);
		}
	}	
}

void CT3EDView::OnEditPaste() 	// Paste from Clipboard
{
	CT3EDDoc *pDoc=GetDocument();
	struct CLIPBLOCK *u, CbInfo;
	struct FLOATPT RefOld;
	struct INTPT ipt1,ipt2;
	struct COLVROAD *vrdata;
	//struct COLVROAD *cvr;
	float dx=0,dy=0,dz=0, mLenght=1, mWidth=1;
	int oldundoLevel, i;
	short neighbourdata[8], visdata[300];
	double AngleOld,AngleNew, len1, len2;
	double pi = 3.14159265358979323846264338;
	CBlockPaste BlockPasteDlg;
	CString sStr = "";
	BOOL er=FALSE;

	if (selMode==ID_MODE_POINT) //Paste x,y,z to a point
	{
		pDoc->PrepareNewUndo(refnode,refblock,offsetx,offsety);
		pDoc->PrepareModifyTrk(refblock);

		pDoc->PasteXYZfromClipboard(refnode);
		
		InvalidateRect(NULL,TRUE);
	}

	if ((selMode==ID_MODE_OBJECT)&&(refblock!=pDoc->nBlocks)) //Paste an object, if refblock isn't set to global
	{
		er=pDoc->BlockClipboardInfo(CbInfo); //Get Info about what is in Clipboard
		if ((CbInfo.clipNo!=-1)&&(CbInfo.clipIsobj!=4)) { //Only if a Object is found and selected

			if (multisel) delete multisel; multisel=NULL;
			pDoc->PrepareNewUndo(refnode,refblock,offsetx,offsety);
			pDoc->PrepareModifyTrk(refblock);
			pDoc->PasteBlockfromClipboard(-1,refblock,0,0,0,0,0); //Paste Object out of Block stored in Clipboard
		
			u=&pDoc->Clipblk;

			if (u->clipNo!=-1){
				switch (u->clipIsobj) {
					case 0: 
						sStr.Format("Pasted object to block %d.", refblock);
						break;
					case 1:
						sStr.Format("Pasted extra object to block %d.", refblock);
						break;
					case 2:
						sStr.Format("Pasted light object to block %d.", refblock);
						break;
					case 3:
						sStr.Format("Pasted sound object to block %d.", refblock);
						break;
				}
			statusbar->SetPaneText(0,sStr,TRUE);
			}

		//Set view to new object just pasted.
		isxobj=CbInfo.clipIsobj; 
		objchunk=CbInfo.clipChunk;
		if (isxobj==3) {
			objno=pDoc->trk[refblock].nSoundsrc-1;
			// refnode is still &myrefnode
		} else 
		if (isxobj==2) {
			objno=pDoc->trk[refblock].nLightsrc-1;
			// refnode is still &myrefnode
		} else
		if (isxobj==1) {
			objno=pDoc->xobj[4*refblock+objchunk].nobj-1;
			refnode=&(pDoc->xobj[4*refblock+objchunk].obj[objno].ptRef);
		} else {
			objno=pDoc->poly[refblock].obj[objchunk].nobj-1;
			refnode=pDoc->trk[refblock].vert+
			pDoc->poly[refblock].obj[objchunk].poly[objno]->vertex[0];
			}
		}
		InvalidateRect(NULL,TRUE);
	}

	if (selMode==ID_MODE_BLOCK) //Paste an block
	{
		er=pDoc->BlockClipboardInfo(CbInfo); //Get Info about what is in Clipboard
		if ((CbInfo.clipNo==-1)&&(er==TRUE)) //Found a block with no selected object
		{
			BlockPasteDlg.m_add_replace_block=pDoc->settings.m_add_replace_block; // Default setting
			BlockPasteDlg.m_bool_paste_block=pDoc->settings.m_bool_add_replace_block_paste_block;
			BlockPasteDlg.m_bool_move=pDoc->settings.m_bool_add_replace_block_move;
			BlockPasteDlg.m_bool_move_z=pDoc->settings.m_bool_add_replace_block_move_z;
			BlockPasteDlg.m_bool_rotate=pDoc->settings.m_bool_add_replace_block_rotate;
			BlockPasteDlg.m_bool_expand_lenght=pDoc->settings.m_bool_add_replace_block_expand_lenght;
			BlockPasteDlg.m_bool_expand_width=pDoc->settings.m_bool_add_replace_block_expand_width;
			BlockPasteDlg.m_bool_paste_heights=pDoc->settings.m_bool_add_replace_block_paste_heights;
			BlockPasteDlg.m_bool_paste_spdfa=pDoc->settings.m_bool_add_replace_block_paste_spdfa;
			BlockPasteDlg.m_bool_paste_spdra=pDoc->settings.m_bool_add_replace_block_paste_spdra;
			BlockPasteDlg.m_bool_paste_vroad=pDoc->settings.m_bool_add_replace_block_paste_vroad;		

			if (!(BlockPasteDlg.DoModal () == IDOK)) return; //Cancel pressed ?
			{
				pDoc->settings.m_add_replace_block=BlockPasteDlg.m_add_replace_block;
				pDoc->settings.m_bool_add_replace_block_paste_block=BlockPasteDlg.m_bool_paste_block;
				pDoc->settings.m_bool_add_replace_block_move=BlockPasteDlg.m_bool_move;
				pDoc->settings.m_bool_add_replace_block_move_z=BlockPasteDlg.m_bool_move_z;
				pDoc->settings.m_bool_add_replace_block_rotate=BlockPasteDlg.m_bool_rotate;
				pDoc->settings.m_bool_add_replace_block_expand_lenght=BlockPasteDlg.m_bool_expand_lenght;
				pDoc->settings.m_bool_add_replace_block_expand_width=BlockPasteDlg.m_bool_expand_width;
				pDoc->settings.m_bool_add_replace_block_paste_heights=BlockPasteDlg.m_bool_paste_heights;
				pDoc->settings.m_bool_add_replace_block_paste_spdfa=BlockPasteDlg.m_bool_paste_spdfa;
				pDoc->settings.m_bool_add_replace_block_paste_spdra=BlockPasteDlg.m_bool_paste_spdra;
				pDoc->settings.m_bool_add_replace_block_paste_vroad=BlockPasteDlg.m_bool_paste_vroad;
				if (BlockPasteDlg.m_add_replace_block==0) // Add block as new last block
				{
					pDoc->PasteBlockfromClipboard(0,pDoc->nBlocks,1,1,1,1,1); //Paste Block stored in Clipboard at the end
					refblock=pDoc->nBlocks-1;
					pDoc->trk[refblock].hs_neighbors[0]=0; //Next block is the first of track
					pDoc->trk[refblock].hs_neighbors[1]=refblock-1; //Set previous block as neighbour
					pDoc->trk[0].hs_neighbors[0]=refblock; //Set some visible blocks
					for (i=1;i<6;i++)
						{
							pDoc->trk[refblock].nbdData[i*2-1].blk=i-1;
							pDoc->trk[refblock].nbdData[i*2].blk=refblock-i;
						}
					pDoc->RecalcBoundingBox(refblock);
					refnode=&(pDoc->trk[refblock].ptCentre); //Setview to Block Centre
					UpdateRefBlock();
					InvalidateRect(NULL,TRUE);
				}
				if (BlockPasteDlg.m_add_replace_block==1) // Insert block before actual block
				{
					pDoc->PasteBlockfromClipboard(0,pDoc->nBlocks,1,1,1,1,1); //Paste Block stored in Clipboard at the end
					pDoc->MoveLastBlockTo(refblock);

					pDoc->RecalcBoundingBox(refblock);
					refnode=&(pDoc->trk[refblock].ptCentre); //Setview to Block Centre
					UpdateRefBlock();
					InvalidateRect(NULL,TRUE);

				}
				if (BlockPasteDlg.m_add_replace_block==2) //Replace selected block
				{
					RefOld=pDoc->trk[refblock].ptCentre; //Old block middle
					ipt1=pDoc->col.vroad[pDoc->trk[refblock].nStartPos].refPt; //First vroad point
					ipt2=pDoc->col.vroad[pDoc->trk[refblock].nStartPos+pDoc->trk[refblock].nPositions-1].refPt; //Last vroad point
					AngleOld = (180 * atan2((ipt1.y - ipt2.y),(ipt1.x - ipt2.x)))/pi;// Actual Angle of old vroad;
					len1=pDoc->VRoadLenght(refblock);
					vrdata=(struct COLVROAD *)malloc(36*pDoc->trk[refblock].nPositions); //Backup Vroad
					memcpy(vrdata, &pDoc->col.vroad[pDoc->trk[refblock].nStartPos], 36*pDoc->trk[refblock].nPositions);
					for (i=0;i<8;i++) //Backup Block Neighbours
						neighbourdata[i]=(short)pDoc->trk[refblock].hs_neighbors[i];
					for (i=0;i<300;i++) //Backup Visibility
						visdata[i]=pDoc->trk[refblock].nbdData[i].blk;

					pDoc->PrepareNewUndo(refnode,refblock,offsetx,offsety); //New undo point
					pDoc->PasteBlockfromClipboard(2,refblock, BlockPasteDlg.m_bool_paste_block, BlockPasteDlg.m_bool_paste_heights, BlockPasteDlg.m_bool_paste_spdfa, BlockPasteDlg.m_bool_paste_spdra, BlockPasteDlg.m_bool_paste_vroad); //Paste Block stored in Clipboard
					//refnode=&(pDoc->trk[refblock].ptCentre);

					oldundoLevel=pDoc->undoLevel; //One undo for pasted block is enough
					pDoc->undoLevel=0;
					if ((pDoc->settings.m_bool_add_replace_block_move)|(pDoc->settings.m_bool_add_replace_block_move_z)) //Move new block in position
					{
						//pDoc->PrepareNewUndo(refnode,refblock,offsetx,offsety);
						if (pDoc->settings.m_bool_add_replace_block_move)
						{
							dx=RefOld.x - pDoc->trk[refblock].ptCentre.x;
							dy=RefOld.y - pDoc->trk[refblock].ptCentre.y;
						}
						if (pDoc->settings.m_bool_add_replace_block_move_z)
							dz=RefOld.z - pDoc->trk[refblock].ptCentre.z;
						pDoc->MoveBlockSimple(refblock,dx,dy,dz);
					}
					if (pDoc->settings.m_bool_add_replace_block_rotate)
					{
						//pDoc->PrepareNewUndo(refnode,refblock,offsetx,offsety);
						ipt1=pDoc->col.vroad[pDoc->trk[refblock].nStartPos].refPt; //First vroad point
						ipt2=pDoc->col.vroad[pDoc->trk[refblock].nStartPos+pDoc->trk[refblock].nPositions-1].refPt; //Last vroad point
						AngleNew = (180 * atan2((ipt1.y - ipt2.y),(ipt1.x - ipt2.x)))/pi;// Actual Angle of old vroad;
						pDoc->RotateBlock(refblock,(AngleNew-AngleOld));
					}
					if (pDoc->settings.m_bool_add_replace_block_expand_lenght)
					{
						len2=pDoc->VRoadLenght(refblock);
						mLenght=(float)(len1/len2);
					}
					if ((pDoc->settings.m_bool_add_replace_block_expand_lenght)||pDoc->settings.m_bool_add_replace_block_expand_width)
						pDoc->ExpandBlock(refblock,mLenght, mWidth, 1);
/*					if (pDoc->settings.m_bool_add_replace_block_oldvroad)
					{
						for(i=0;i<pDoc->trk[refblock].nPositions;++i)
							pDoc->col.vroad[pDoc->trk[refblock].nStartPos + i].refPt=vrdata[i].refPt;
						//pDoc->MoveBlocks(refblock,0,0,0,0,0,3); //Only Remap vertices
					}*/
					for (i=0;i<8;i++) //Write back Block Neighbours
						pDoc->trk[refblock].hs_neighbors[i]=neighbourdata[i];
					for (i=0;i<300;i++) //Write back  Visibility
						pDoc->trk[refblock].nbdData[i].blk=visdata[i];
					dofree(vrdata);
					pDoc->undoLevel=oldundoLevel;
					pDoc->RecalcBoundingBox(refblock);
					refnode=&(pDoc->trk[refblock].ptCentre); //Setview to Block Centre
					InvalidateRect(NULL,TRUE);
				}
				if (BlockPasteDlg.m_add_replace_block==3) //Replace selected block
				{
					pDoc->PrepareNewUndo(refnode,refblock,offsetx,offsety); //New undo point
					pDoc->PasteBlockfromClipboard(3,refblock, BlockPasteDlg.m_bool_paste_block, BlockPasteDlg.m_bool_paste_heights, BlockPasteDlg.m_bool_paste_spdfa, BlockPasteDlg.m_bool_paste_spdra, BlockPasteDlg.m_bool_paste_vroad); //Paste Block stored in Clipboard

					pDoc->RecalcBoundingBox(refblock);
					refnode=&(pDoc->trk[refblock].ptCentre); //Setview to Block Centre
					InvalidateRect(NULL,TRUE);

				}
			}
		}
	}
}

void CT3EDView::OnEditFind() 
{
	CT3EDDoc *pDoc=GetDocument(); 
	FINDPOLYDATA PolyProps;
	CFindPolygonDlg FindPolyDlg;

	FindPolyDlg.m_block_choice=pDoc->settings.FindPoly_block_choice;
	FindPolyDlg.m_trackpolys=pDoc->settings.FindPoly_trackpolys;
	FindPolyDlg.m_startbl=pDoc->settings.FindPoly_startbl;
	FindPolyDlg.m_endbl=pDoc->settings.FindPoly_endbl;
	FindPolyDlg.m_extraobj=pDoc->settings.FindPoly_extraobj;
	FindPolyDlg.m_polygonobj=pDoc->settings.FindPoly_polygonobj;
	FindPolyDlg.m_texture=pDoc->settings.FindPoly_texture;
	FindPolyDlg.m_chk_texture=pDoc->settings.FindPoly_chk_texture;
	FindPolyDlg.hs_flags=pDoc->settings.FindPoly_m_flags;
	FindPolyDlg.m_chk_hstexture=pDoc->settings.FindPoly_chk_m_flags;
	FindPolyDlg.texanimdata=pDoc->settings.FindPoly_texanimdata;
	FindPolyDlg.m_chk_animated=pDoc->settings.FindPoly_chk_texanimdata;
	FindPolyDlg.pvrflags=pDoc->settings.FindPoly_pvrflags;
	FindPolyDlg.m_chk_pvrflags=pDoc->settings.FindPoly_chk_pvrflags;


	FindPolyDlg.bHSMode=pDoc->bHSMode;
	
	FindPolyDlg.qfs=&(pDoc->qfsView);
	

	if (!(FindPolyDlg.DoModal () == IDOK)) return; //Cancel pressed

	pDoc->settings.FindPoly_block_choice=FindPolyDlg.m_block_choice;
	pDoc->settings.FindPoly_trackpolys=FindPolyDlg.m_trackpolys;
	if (FindPolyDlg.m_endbl>=pDoc->nBlocks) FindPolyDlg.m_endbl=pDoc->nBlocks-1;
	pDoc->settings.FindPoly_endbl=FindPolyDlg.m_endbl;  
	if (FindPolyDlg.m_startbl>FindPolyDlg.m_endbl) FindPolyDlg.m_startbl=FindPolyDlg.m_endbl;
	pDoc->settings.FindPoly_startbl=FindPolyDlg.m_startbl;
	pDoc->settings.FindPoly_extraobj=FindPolyDlg.m_extraobj;
	pDoc->settings.FindPoly_polygonobj=FindPolyDlg.m_polygonobj;
	pDoc->settings.FindPoly_texture=FindPolyDlg.m_texture;  
	pDoc->settings.FindPoly_chk_texture=FindPolyDlg.m_chk_texture;
	pDoc->settings.FindPoly_m_flags=FindPolyDlg.hs_flags;
	pDoc->settings.FindPoly_chk_m_flags=FindPolyDlg.m_chk_hstexture;
	pDoc->settings.FindPoly_texanimdata=FindPolyDlg.texanimdata;
	pDoc->settings.FindPoly_chk_texanimdata=FindPolyDlg.m_chk_animated;
	pDoc->settings.FindPoly_pvrflags=FindPolyDlg.pvrflags;
	pDoc->settings.FindPoly_chk_pvrflags=FindPolyDlg.m_chk_pvrflags;

	//PolyProps == Information about what to find
	PolyProps.texture=FindPolyDlg.m_texture;
	PolyProps.search_texture=FindPolyDlg.m_chk_texture;
	PolyProps.hs_flags=FindPolyDlg.hs_flags;
	PolyProps.search_hs_flags=FindPolyDlg.m_chk_hstexture;
	PolyProps.texanimdata=FindPolyDlg.texanimdata;
	PolyProps.search_texanimdata=FindPolyDlg.m_chk_animated;
	PolyProps.pvrflags=FindPolyDlg.pvrflags;
	PolyProps.search_pvrflags=FindPolyDlg.m_chk_pvrflags;
	
	if (FindPolyDlg.m_block_choice==0)
		FindPoly(PolyProps,refblock,refblock,FindPolyDlg.m_trackpolys,FindPolyDlg.m_polygonobj,FindPolyDlg.m_extraobj);
	else
		FindPoly(PolyProps,FindPolyDlg.m_startbl,FindPolyDlg.m_endbl,FindPolyDlg.m_trackpolys,FindPolyDlg.m_polygonobj,FindPolyDlg.m_extraobj);

	//FindPoly(PolyProps,9,12,FALSE,FALSE,TRUE);
	InvalidateRect(NULL,TRUE);
	
	CDC *dc;
	if (!isDragging) return;
	dc=GetDC();
	dc->SelectObject(redPen);
	dc->SetROP2(R2_NOTXORPEN);
	ShowSelectionAt(dc,dragX,dragY);
	ReleaseDC(dc);
}

void CT3EDView::OnEditMerge() 
{
	CT3EDDoc *pDoc=GetDocument();
	int er;
	
	if (refblock<pDoc->nBlocks)
	{
		er=pDoc->MergeBlocks(refblock, refblock+1);
		if (er>-1)
		{
			refblock=er;
			pDoc->RecalcBoundingBox(refblock);
			refnode=&(pDoc->trk[refblock].ptCentre); //Setview to Block Centre
			InvalidateRect(NULL,TRUE);
		}
	}
}

BOOL CT3EDView::FindPolyMatches(FINDPOLYDATA &fpoly, LPPOLYGONDATA &p)
{
	BOOL Ok=TRUE;

	if ((fpoly.search_texture)&&(fpoly.texture!=p->texture)) Ok=FALSE;
	if ((fpoly.search_hs_flags)&&(fpoly.hs_flags!=p->hs_textflags)) Ok=FALSE;
	if ((fpoly.search_texanimdata)&&(p->texanimdata==0)) Ok=FALSE;

	return Ok;
}

BOOL CT3EDView::FindPolyVRMatches(FINDPOLYDATA &fpoly, POLYVROADDATA *pvr, LPPOLYGONDATA &p)
{
	BOOL Ok=TRUE;

	if ((fpoly.search_pvrflags)&&((pvr->flags&0x1f)!=fpoly.pvrflags)) Ok=FALSE;
	if ((fpoly.search_pvrflags)&&(p->texture>2047)) Ok=FALSE;

	return Ok;
}

void CT3EDView::FindPoly(FINDPOLYDATA &fpoly, int startbl, int endbl, BOOL FindTrackPoly, BOOL FindObjectPoly, BOOL FindXObjectPoly)
{
	int i,j,k,l,n,i0,j0;
	struct FLOATPT *pt;
	LPPOLYGONDATA p;
	struct OBJPOLYBLOCK *o;
	struct XOBJDATA *xx;
	struct POLYVROADDATA *pvrdata;
	CT3EDDoc *pDoc=GetDocument(); 

	if (multisel!=NULL)
		{ delete multisel; multisel=NULL; }
	refpoly=NULL;

	i0=-1; //p0=NULL,isx0=0;
	if (FindTrackPoly)
	for (i=startbl;i<endbl+1;i++) 
	{
		//if (!MeetsClipRect(&(pDoc->trk[i]),&rect)) continue;
		pt=pDoc->trk[i].vert;
		for (k=0;k<7;k++) {
			if (!bShowLanes) if (k!=nDetail) continue;
			if (bShowLanes) if (((k&6)!=nDetail)&&(k!=6)) continue;
			if ((fpoly.search_pvrflags==TRUE)&&(k>4)) continue; //Flags are only on lower chunks
			if ((fpoly.search_pvrflags==TRUE)&&(nDetail!=HIGH_DETAIL)) continue; //Flags search only on high detail
			p=pDoc->poly[i].poly[k];
			n=pDoc->poly[i].sz[k];
			for (j=0;j<n;j++,p++) 
			{
				pvrdata=pDoc->trk[i].polyData+j;
				if  ((FindPolyMatches(fpoly, p))&&FindPolyVRMatches(fpoly, pvrdata, p))
				{
						NewSel();
						refblock=i;		//refblock=i0;
						refpolychunk=k;	//refpolychunk=j0;
						refpolyobj=-1;	//refpolyobj=k0;
						refpolyno=j;	//refpolyno=l0;
						isxobj=0;
						refpoly=p;		//refpoly=p0;
						refvertices=pt;//refvertices=pt0;
						/*if (isx0) pt=coreref;
						else pt=refvertices+refpoly->vertex[0];
						x0=FloatX(*pt); y0=FloatY(*pt);*/
						refnode=pt;
				}
			}
		}
	}
	if (FindObjectPoly)
	for (i=startbl;i<endbl+1;i++) //Polygon objects
	{
		//if (!MeetsClipRect(&(pDoc->trk[i]),&rect)) continue;
		pt=pDoc->trk[i].vert;
		for (j=0;j<4;j++) {
			o=&(pDoc->poly[i].obj[j]);
			if (o->n1==0) continue;
			for (k=0;k<o->nobj;k++) {
				p=o->poly[k];
				n=o->numpoly[k];
				for (l=0;l<n;l++,p++) 
				{
					if (FindPolyMatches(fpoly, p))
					{
						NewSel();
						refblock=i;		//refblock=i0;
						refpolychunk=j;	//refpolychunk=j0;
						refpolyobj=k;	//refpolyobj=k0;
						refpolyno=l;	//refpolyno=l0;
						isxobj=0;
						refpoly=p;		//refpoly=p0;
						refvertices=pt;//refvertices=pt0;
					}
				}
			}
		}
	}
	if (FindXObjectPoly)
		for (i=4*startbl;i<4*(endbl+1);i++) {
			//if (!MeetsClipRect(&(pDoc->trk[i/4]),&rect)) { i=i|3; continue; }
			xx=pDoc->xobj[i].obj;
			for (j=0;j<pDoc->xobj[i].nobj;j++,xx++) {
				pt=xx->vert;
				p=xx->polyData;
				for (k=0;k<xx->nPolygons;k++,p++)
				{
					if (FindPolyMatches(fpoly, p))
					{
						NewSel();
						refblock=i/4;
						refpolychunk=j0=i%4;
						refpolyobj=j;
						refpolyno=k;
						isxobj=1;
						refpoly=p;
						refvertices=pt;
						refnode=&(xx->ptRef);
					}
				}
			}
		}
}

void CT3EDView::OnCopyXY() 
{
	// TODO: Code für Befehlsbehandlungsroutine hier einfügen
	CT3EDDoc *pDoc=GetDocument(); 
	pDoc->memory_x = refnode->x;
	pDoc->memory_y = refnode->y;
	InvalidateRect(NULL,TRUE);
	//MessageBox("Strg H.");
}

void CT3EDView::OnPasteXY() 
{
	// Paste XY
	CT3EDDoc *pDoc=GetDocument(); 
	CSelData *sel;
	float MoveAmountX,MoveAmountY;

	pDoc->PrepareNewUndo(refnode,refblock,offsetx,offsety);
	pDoc->PrepareModifyTrk(refblock);

	if (selMode==ID_MODE_POINT) // Only for Point mode
	{
		if (multisel) // Paste XY for more than one point
		{
			{
				for (sel=this;sel!=NULL;sel=sel->multisel)
				{
					MoveAmountX=-1*(sel->refnode->x-pDoc->memory_x);
					MoveAmountY=-1*(sel->refnode->y-pDoc->memory_y);
					pDoc->MovePointBy(sel->refnode,MoveAmountX,MoveAmountY,0);
				}
			}
		}
		else // Paste XY for one point
		{
			MoveAmountX=-1*(refnode->x-pDoc->memory_x);
			MoveAmountY=-1*(refnode->y-pDoc->memory_y);
			pDoc->MovePointBy(refnode,MoveAmountX,MoveAmountY,0);
		}
	}
	InvalidateRect(NULL,TRUE);
}

void CT3EDView::OnCopyZ() 
{
	// TODO: Add your command handler code here
	CT3EDDoc *pDoc=GetDocument(); 
	pDoc->memory_Zh = refnode->z;
	InvalidateRect(NULL,TRUE);
	
}


void CT3EDView::OnCOPYZl() 
{
	// TODO: Add your command handler code here
	CT3EDDoc *pDoc=GetDocument(); 
	pDoc->memory_Zl = refnode->z;
	InvalidateRect(NULL,TRUE);
}

void CT3EDView::OnPasteZh() 
{
	// TODO: Add your command handler code here
	CT3EDDoc *pDoc=GetDocument(); 
	CSelData *sel;
	float mx,my,mz;
	double MoveAmount, shadestep;
	double pieni, suuri; 
	int PasteMode, counter, numentrys, number;
	CString s;
	PasteMode=pDoc->ZCopyMode;
	struct POLYVROADDATA *data;
	struct FLOATPT p1,p2;
	
	data=pDoc->trk[refblock].polyData+refpolyno;
	
	
	pDoc->PrepareNewUndo(refnode,refblock,offsetx,offsety);
	pDoc->PrepareModifyTrk(refblock);
	
	
	
	if (selMode==ID_MODE_POLYGON)
	{	
		if (multisel)
		{
			if (PasteMode==2)
			{
				
				for (sel=this;sel!=NULL;sel=sel->multisel)
				{
					short TP;
					TP=sel->refpoly->vertex[1];
					sel->refpoly->vertex[1]=refpoly->vertex[0];
					sel->refpoly->vertex[0]=TP;

					TP=sel->refpoly->vertex[3];
					sel->refpoly->vertex[3]=refpoly->vertex[2];
					sel->refpoly->vertex[2]=TP;
				}
				
			}
			else
			{
				for (sel=this;sel!=NULL;sel=sel->multisel)
				{
					data=pDoc->trk[sel->refblock].polyData+sel->refpolyno;
					data->virtualroadedge=!data->virtualroadedge;
				}
			}
		}
		else 
		{
			if (PasteMode==2)
			{
				short TP;
				TP=sel->refpoly->vertex[1];
				sel->refpoly->vertex[1]=refpoly->vertex[0];
				sel->refpoly->vertex[0]=TP;

				TP=sel->refpoly->vertex[3];
				sel->refpoly->vertex[3]=refpoly->vertex[2];
				sel->refpoly->vertex[2]=TP;
			}
			else
			{
				data->virtualroadedge=!data->virtualroadedge;
			}
		}
	}
	if (selMode==ID_MODE_POINT)
	{
		if (multisel)
		{
			//PasteMode==2, Multiselect ZShade.
			if (PasteMode==2) 
			{
				suuri=-99999;
				pieni=99999;
				counter=0;
				for (sel=this;sel!=NULL;sel=sel->multisel)
				{
					//look for smalest and bigest
					if (counter<1)
						pieni=sel->refnode->z;
					suuri=sel->refnode->z;
					/*if (sel->refnode->z< small)
						small=sel->refnode->z;
					if (sel->refnode->z> big)
						big=sel->refnode->z;*/
					counter++;
				}
				counter--; //one too much, but why??
				shadestep=(suuri-pieni)/counter;
				numentrys=counter;
				counter=-1;
				number=numentrys+1;
				s.Format("Shading %d points.", number);
				MessageBox(s);
				for (sel=this;sel!=NULL;sel=sel->multisel)
				{
					counter++;
					if (counter<numentrys && counter>0) {
						//s.Format("Counter: %d, NumEntrys:%d",counter, numentrys);
						//MessageBox(s);
						MoveAmount=-1*(sel->refnode->z-((counter*shadestep)+pieni));
						pDoc->MovePointBy(sel->refnode, 0, 0, (float)MoveAmount);	
					}
				}
			}
			else if (PasteMode==3) //MultiSelect Average.
			{
				float MAverage = 0;
				counter=0;
				for (sel=this;sel!=NULL;sel=sel->multisel)
				{
					counter++;
					MAverage=MAverage+sel->refnode->z;
				}
				MAverage=MAverage/counter;
				for (sel=this;sel!=NULL;sel=sel->multisel)
				{
					MoveAmount=-1*(sel->refnode->z-MAverage);
					pDoc->MovePointBy(sel->refnode, 0, 0, (float)MoveAmount);	
				}

			}
			//Line up x,y,(z) mode
			else if ((PasteMode==4)|(PasteMode==5)) {
				
				//get start & end point values
				sel=this;
				p1.x=sel->refnode->x;
				p1.y=sel->refnode->y;
				p1.z=sel->refnode->z;
				counter=0;
				for (sel=this;sel!=NULL;sel=sel->multisel)
				{
					p2.x=sel->refnode->x;
					p2.y=sel->refnode->y;
					p2.z=sel->refnode->z;
					counter++;
				}
				number=counter-1;
				//Line up points.
				counter=0;
				if (number>1)
				{
					for (sel=this;sel!=NULL;sel=sel->multisel)
					{
						mx=(p1.x + counter*((p2.x - p1.x)/number)) - sel->refnode->x;
						my=(p1.y + counter*((p2.y - p1.y)/number)) - sel->refnode->y;
						mz=(p1.z + counter*((p2.z - p1.z)/number)) - sel->refnode->z;
						if (PasteMode==4) // only x & y
							pDoc->MovePointBy(sel->refnode, mx, my, 0);
						else // x, y & z
							pDoc->MovePointBy(sel->refnode, mx, my, mz);
						counter++;
					}
					s.Format("Line up x,y,(z) %d points.", counter);
					MessageBox(s);
				}
			}
			
			else
			{
				for (sel=this;sel!=NULL;sel=sel->multisel)
				{
					MoveAmount=-1*(sel->refnode->z - pDoc->memory_Zh);
					pDoc->MovePointBy(sel->refnode, 0, 0, (float)MoveAmount);	
				}
			}
		}
		else
		{
			MoveAmount=-1*(refnode->z-pDoc->memory_Zh);
			pDoc->MovePointBy(refnode, 0,0,(float)MoveAmount);
		}
	}
	//refnode->z = pDoc->memory_Zh;
	InvalidateRect(NULL,TRUE);
	//turbo.Format("refpoly number: %d", refpoly);
	//MessageBox(turbo);

}


void CT3EDView::OnExportTrkvertices() 
{
	CT3EDDoc *pDoc=GetDocument();

FILE* fout = fopen("Vertices.txt", "w");
		//MessageBox("Tiedosto auki.");
		fprintf(fout, "%f\n", 2.0);
		fprintf(fout, "%d\n", pDoc->nBlocks);
		for(int i=0;i<pDoc->nBlocks ;i++)
		{
			fprintf(fout, "%d\n", pDoc->trk[i].nVertices);
			
			for(int u=0;u<pDoc->trk[i].nVertices ;u++)
			{
				fprintf(fout, "X: %f\n", pDoc->trk[i].vert[u].x);
				fprintf(fout, "Y: %f\n", pDoc->trk[i].vert[u].y);
				fprintf(fout, "Z: %f\n", pDoc->trk[i].vert[u].z);
				fprintf(fout, "C: %X\n", pDoc->trk[i].unknVertices[u]);
			}
			
		}
		fclose(fout);
		MessageBox("Vertices Exportted.");	
}


/*void CT3EDView::OnExportQuads() 
{
	// TODO: Add your command handler code here
	//MessageBox("alussa");
	CT3EDDoc *pDoc=GetDocument();
	//MessageBox("dokumentti saatu");

FILE* fout = fopen("Mesh.txt", "w");
		//MessageBox("Tiedosto auki.");
		fprintf(fout, "%f\n", 3.0);
		fprintf(fout, "%d\n", pDoc->nBlocks);
		for(int i=0;i<pDoc->nBlocks ;i++)
		{
			fprintf(fout, "%d\n", pDoc->trk[i].nPositions);
			
			/for(int u=0;u<pDoc->trk[i].nPositions-1;u++)
			{
				//fprintf(fout, "X: %f\n", pDoc->poly[i]->poly[6]->vertex[0]
				fprintf(fout, "Y: %f\n", pDoc->trk[i].vert[u].y);
				fprintf(fout, "Z: %f\n", pDoc->trk[i].vert[u].z);
				fprintf(fout, "C: %X\n", pDoc->trk[i].unknVertices[u]);
			} 
		
		}
		fclose(fout);
		MessageBox("Vertices Exportted.");	
	
}*/

void CT3EDView::OnFileProperties() 
{
	CFrdFileDlg FrdFileDlg;
	CT3EDDoc *pDoc=GetDocument();

	FrdFileDlg.m_int_nBlocks=pDoc->nBlocks;
	FrdFileDlg.m_short_nVRoad=pDoc->col.vroadHead.nrec;
	if (pDoc->bHSMode)
		FrdFileDlg.m_str_File_Format="NFS 4";
	else
		FrdFileDlg.m_str_File_Format="NFS 3";

	FrdFileDlg.DoModal();
}

void CT3EDView::OnToolsProperties() 
{
	//PrepareModifyTrk(blk);
	//PrepareModifyPoly(blk);
	float dist,dist0;
	int i, num, oblock,oisxobj,oobjchunk,oobjno;
	CSelData *sel;
	CString keke="";
	CString text="";
	struct TRKBLOCK blk;
	CT3EDDoc *pDoc=GetDocument();
	struct NOBJDATA NewObjData;
	struct XOBJDATA *x, *ox;
	//struct INTPT *pt;
	struct REFXOBJ *rx;
	//struct FLOATPT *v;
	//float dx, dy, dz;

	if ((selMode==ID_MODE_OBJECT)&&(isxobj==4)) // VRoad Refpoint
	{
		CVRoadHeightsSpdFileDlg VRHSFDlg; // Create dialog
		num=0;
		for (sel=this;sel!=NULL;sel=sel->multisel) //look at all objects
			if (sel->isxobj==4)
			{
				num++;
				if (num==1)
				{
					VRHSFDlg.m_colvrdata=(struct COLVROAD *)malloc(sizeof(struct COLVROAD)); //Get first memory
					VRHSFDlg.m_heightsdata=(struct HEIGHTSSIM *)malloc(sizeof(struct HEIGHTSSIM));
					VRHSFDlg.spdFAdata=(struct SPDFILE *)malloc(sizeof(struct SPDFILE));
					VRHSFDlg.spdRAdata=(struct SPDFILE *)malloc(sizeof(struct SPDFILE));
				}
				else
				{
					VRHSFDlg.m_colvrdata=(struct COLVROAD *)realloc(VRHSFDlg.m_colvrdata,num * sizeof(struct COLVROAD)); //Get more memory
					VRHSFDlg.m_heightsdata=(struct HEIGHTSSIM *)realloc(VRHSFDlg.m_heightsdata,num * sizeof(struct HEIGHTSSIM));
					VRHSFDlg.spdFAdata=(struct SPDFILE *)realloc(VRHSFDlg.spdFAdata,num * sizeof(struct SPDFILE));
					VRHSFDlg.spdRAdata=(struct SPDFILE *)realloc(VRHSFDlg.spdRAdata,num * sizeof(struct SPDFILE));
				}
				memcpy(&VRHSFDlg.m_colvrdata[num-1],&pDoc->col.vroad[sel->objno],sizeof(struct COLVROAD));
				memcpy(&VRHSFDlg.m_heightsdata[num-1],&pDoc->hightssim[sel->objno],sizeof(struct HEIGHTSSIM));
				memcpy(&VRHSFDlg.spdFAdata[num-1],&pDoc->spdFAbin[sel->objno],sizeof(struct SPDFILE));
				memcpy(&VRHSFDlg.spdRAdata[num-1],&pDoc->spdRAbin[sel->objno],sizeof(struct SPDFILE));
			}

		VRHSFDlg.m_nPositions=num;
		VRHSFDlg.m_Chk_Heights=pDoc->settings.VRoadHeightsSpd_Chk_Heights;
		VRHSFDlg.m_Chk_SpdFa=pDoc->settings.VRoadHeightsSpd_Chk_SpdFa;
		VRHSFDlg.m_Chk_SpdRa=pDoc->settings.VRoadHeightsSpd_Chk_SpdRa;
		VRHSFDlg.m_Chk_VRoad=pDoc->settings.VRoadHeightsSpd_Chk_VRoad;

		if (VRHSFDlg.DoModal()==IDOK)
		{
			pDoc->settings.VRoadHeightsSpd_Chk_Heights=VRHSFDlg.m_Chk_Heights;
			pDoc->settings.VRoadHeightsSpd_Chk_SpdFa=VRHSFDlg.m_Chk_SpdFa;
			pDoc->settings.VRoadHeightsSpd_Chk_SpdRa=VRHSFDlg.m_Chk_SpdRa;
			pDoc->settings.VRoadHeightsSpd_Chk_VRoad=VRHSFDlg.m_Chk_VRoad;
			num=0;
			pDoc->PrepareNewUndo(refnode,refblock,offsetx,offsety); //Undo needed
			pDoc->PrepareModifyVRoadHeightsSpdFiles();
			for (sel=this;sel!=NULL;sel=sel->multisel) //look at all objects
				if (sel->isxobj==4)
				{
					num++; //Copy back changed data
					memcpy(&pDoc->col.vroad[sel->objno],&VRHSFDlg.m_colvrdata[num-1],sizeof(struct COLVROAD));
					memcpy(&pDoc->hightssim[sel->objno],&VRHSFDlg.m_heightsdata[num-1],sizeof(struct HEIGHTSSIM));
					memcpy(&pDoc->spdFAbin[sel->objno],&VRHSFDlg.spdFAdata[num-1],sizeof(struct SPDFILE));
					memcpy(&pDoc->spdRAbin[sel->objno],&VRHSFDlg.spdRAdata[num-1],sizeof(struct SPDFILE));
				}
			InvalidateRect(NULL,TRUE);
		}
		//Give back used memory
		dofree(VRHSFDlg.m_colvrdata);
		dofree(VRHSFDlg.m_heightsdata);
		dofree(VRHSFDlg.spdFAdata);
		dofree(VRHSFDlg.spdRAdata);

		return;
	}

	if (multisel) delete multisel; multisel=NULL;
	sel=this;

	blk=pDoc->trk[refblock];
	if (selMode==ID_MODE_BLOCK)
	{
		blk=pDoc->trk[refblock];
		CBlockProp BlockProp;
		BlockProp.m_float_ptCentreX=blk.ptCentre.x;
		BlockProp.m_float_ptCentreY=blk.ptCentre.y;
		BlockProp.m_float_ptCentreZ=blk.ptCentre.z;
		BlockProp.m_nVertices=blk.nVertices;
		BlockProp.m_nHiResVert=blk.nHiResVert;
		BlockProp.m_nMedResVert=blk.nMedResVert;
		BlockProp.m_nLoResVert=blk.nLoResVert;
		BlockProp.m_nLightsrc=blk.nLightsrc;
		BlockProp.m_nSoundsrc=blk.nSoundsrc;
		
		if (BlockProp.DoModal()!=IDOK) return;

	}


	if ((selMode==ID_MODE_OBJECT)&&(isxobj==0)) // Poly Object
	{
		CPolyProperties PProp;
		//struct OBJPOLYBLOCK *o;
		
		PProp.m_intBlock=refblock;
		PProp.m_int_Chunk=sel->objchunk;
		PProp.m_int_ObjNum=sel->objno;
		num=pDoc->poly[sel->refblock].obj[sel->objchunk].numpoly[sel->objno];
		PProp.m_intNumPoly=num;
		PProp.m_bPolyPropsAutoObjMem=pDoc->bAutoObjMem;

		if (PProp.DoModal()!=IDOK) return;

		if (PProp.m_bool_convert) //Convert polygon object to extra object
		{
			pDoc->PrepareNewUndo(refnode,refblock,offsetx,offsety); //undo should be possible
			pDoc->CreateObjectData(&NewObjData,refblock,isxobj,objchunk,objno,FALSE); //Get Object Data.
			pDoc->FindObjMinMax(refblock,isxobj,objchunk,objno); //Calculate middle of old object
			pDoc->DelObject(refblock,isxobj,objchunk,objno); //delete old object.
			refblock=PProp.m_intBlock; //Set new block
			objchunk=PProp.m_int_Chunk; //Set new chunk
			isxobj=1; //New object will be an extra object
			pDoc->NewObject(refblock,isxobj,objchunk,0,-32768,4); //Base object for import
			objno=pDoc->xobj[4*refblock+objchunk].nobj-1; //Get objno of new created object
			x=&pDoc->xobj[4*refblock+objchunk].obj[objno]; //Set pointer x to new xobj
			pDoc->MoveObjectBy(refblock,isxobj,objchunk,objno,(pDoc->MemObjMiddle.x - x->ptRef.x),(pDoc->MemObjMiddle.y - x->ptRef.y),(pDoc->MemObjMiddle.z - x->ptRef.z));//Move new object center to old center position
			pDoc->ImportObject(&NewObjData,refblock,isxobj,objchunk,objno,TRUE); // Import Object from strored data
			pDoc->DelPolygon(refblock,isxobj,objchunk,objno,0); //Delete first given polygon
			dofree(NewObjData.Vertices); //Give back memory
			dofree(NewObjData.Polys);
			//objno=pDoc->xobj[4*refblock+objchunk].nobj-1; //Get objno of new created object
			pDoc->FindObjMinMax(refblock,isxobj,objchunk,objno);
			refnode=&pDoc->MemObjMiddle; //Setview to new object center
			InvalidateRect(NULL,TRUE);
			return; //No more work to do.
		}

		if ((PProp.m_intBlock!=refblock)|(PProp.m_int_Chunk!=objchunk))
		{
			pDoc->PrepareNewUndo(refnode,refblock,offsetx,offsety); //undo should be possible
			if (PProp.m_intBlock!=refblock) //Move PolyObject to new block if changed
			{
				pDoc->ChangeObjBlock(refblock, isxobj, objchunk, objno,PProp.m_intBlock);
				refblock=PProp.m_intBlock;
				objno=pDoc->poly[refblock].obj[objchunk].nobj-1;
			}
			if (PProp.m_int_Chunk!=objchunk) //Move PolyObject to new block if changed
			{
				pDoc->CreateObjectData(&NewObjData,refblock,isxobj,objchunk,objno,FALSE); //Get Object Data.
				pDoc->DelObject(refblock,isxobj,objchunk,objno); //delete old object.
				objchunk=PProp.m_int_Chunk; //Set new chunk
				pDoc->NewObject(refblock,isxobj,objchunk,0,-32768,4); //Base object for import
				objno=pDoc->poly[refblock].obj[objchunk].nobj-1; //Get num of new created object
				pDoc->ImportObject(&NewObjData,sel->refblock,sel->isxobj,sel->objchunk,sel->objno,TRUE); // Import Object from strored data in new chunk
				pDoc->DelPolygon(refblock,isxobj,objchunk,objno,0); //Delete first given polygon
				dofree(NewObjData.Vertices); //Give back memory
				dofree(NewObjData.Polys);
			}
			pDoc->FindObjMinMax(refblock,isxobj,objchunk,objno);
			refnode=&pDoc->MemObjMiddle; //Setview to object center
			InvalidateRect(NULL,TRUE);
		}
	}

	if ((selMode==ID_MODE_OBJECT)&&(isxobj==1)) // Properties for Extra / Global Object
	{
		CExtraObjProps EObjProp;
		x=&pDoc->xobj[4*sel->refblock+sel->objchunk].obj[sel->objno];
		
		EObjProp.m_int_Block=refblock;
		EObjProp.m_bool_IsGlobal=FALSE;
		if (pDoc->nBlocks==refblock) EObjProp.m_bool_IsGlobal=TRUE;
		
		EObjProp.m_int_Chunk=sel->objchunk;
		EObjProp.m_int_ObjNum=sel->objno;
		//num=pDoc->xobj[sel->refblock*4+sel->objchunk].obj[sel->objno].nPolygons;
		EObjProp.m_int_NumPoly=pDoc->xobj[sel->refblock*4+sel->objchunk].obj[sel->objno].nPolygons;
		EObjProp.m_bExtraObjPropsAutoObjMem=pDoc->bAutoObjMem;
		EObjProp.m_long_Crosstype=x->crosstype;
		EObjProp.m_long_unknown=x->unknown;
		EObjProp.m_long_AnimMemory=x->AnimMemory;
		EObjProp.m_short_AnimDelay=x->AnimDelay;
		EObjProp.m_float_x=x->ptRef.x;
		EObjProp.m_float_y=x->ptRef.y;
		EObjProp.m_float_z=x->ptRef.z;		

		if (x->crosstype==4) 
		{
			rx=blk.xobj + x->crossno;
			EObjProp.m_short_unknown1=rx->unknown1;
			EObjProp.m_short_unknown2=rx->unknown2;
			EObjProp.m_short_collide_effect=rx->collideeffect;
		}
		else // Create a dummy rx
		{
			rx=(struct REFXOBJ *)malloc(sizeof(struct REFXOBJ));
			memset(rx,0,sizeof(struct REFXOBJ));
		}
		if ((x->crosstype==3)&&(x->nAnimLength>0)) //Animated
		{
			EObjProp.m_nAnimLength=x->nAnimLength;
			EObjProp.m_AnimData=(struct ANIMDATA *)malloc(x->nAnimLength * sizeof(struct ANIMDATA));
			memcpy(EObjProp.m_AnimData,x->animData,x->nAnimLength * sizeof(struct ANIMDATA));
		}
		EObjProp.m_short_unknown3=(short *) malloc(9 * sizeof(short)); //The unknown3 values
		memcpy(EObjProp.m_short_unknown3,x->unknown3,9 * sizeof(short));
		/*
		1 Static Global
		2 Static
		3 Animated
		4 Static, with collision
		5
		6 Complex-behavior (can be knocked over)
		*/

		if (EObjProp.DoModal()!=IDOK) return;

		//Do changes to X object ?
		if ((EObjProp.m_bool_ReCenter==TRUE)|(EObjProp.m_long_Crosstype!=x->crosstype)|(EObjProp.m_int_Block!=refblock)|(EObjProp.m_int_Chunk!=objchunk)|(EObjProp.m_short_unknown1!=rx->unknown1)|(EObjProp.m_short_unknown2!=rx->unknown2)|(EObjProp.m_short_collide_effect!=rx->collideeffect)|(EObjProp.m_long_unknown!=x->unknown)|(EObjProp.m_long_AnimMemory!=x->AnimMemory)|(EObjProp.m_short_AnimDelay!=x->AnimDelay))
			pDoc->PrepareNewUndo(refnode,EObjProp.m_int_Block,offsetx,offsety); //Create a new undo point if needed

		if (x->crosstype!=4) dofree(rx); //Give back memory for dummy rx

		x->unknown=EObjProp.m_long_unknown;
		x->AnimMemory=EObjProp.m_long_AnimMemory;
		x->AnimDelay=EObjProp.m_short_AnimDelay;
		x->nAnimLength=EObjProp.m_nAnimLength;
		if (x->crosstype==3) //Use new Data on Animated ojbects
		{
			dofree(x->animData);
			x->animData=EObjProp.m_AnimData;
		}
		memcpy(x->unknown3,EObjProp.m_short_unknown3,9 * sizeof(short));
		dofree(EObjProp.m_short_unknown3);

		if (EObjProp.m_bool_Convert_Poly) //Convert extra object to polygon object
		{
			pDoc->PrepareNewUndo(refnode,refblock,offsetx,offsety); //undo should be possible
			pDoc->CreateObjectData(&NewObjData,refblock,isxobj,objchunk,objno,FALSE); //Get Object Data.
			pDoc->FindObjMinMax(refblock,isxobj,objchunk,objno); //Calculate middle of old object
			pDoc->DelObject(refblock,isxobj,objchunk,objno); //delete old object.
			refblock=EObjProp.m_int_Block; //Set new block
			objchunk=EObjProp.m_int_Chunk; //Set new chunk
			isxobj=0; //New object will be an extra object
			pDoc->NewObject(refblock,isxobj,objchunk,0,-32768,4); //Base object for import
			objno=pDoc->poly[refblock].obj[objchunk].nobj-1;
			pDoc->ImportObject(&NewObjData,refblock,isxobj,objchunk,objno,TRUE); // Import Object from strored data
			pDoc->DelPolygon(refblock,isxobj,objchunk,objno,0); //Delete first given polygon
			dofree(NewObjData.Vertices); //Give back memory
			dofree(NewObjData.Polys);
			refnode=pDoc->trk[refblock].vert+
				pDoc->poly[refblock].obj[objchunk].poly[objno]->vertex[0];
			return; //No more work to do.
		}

		if ((EObjProp.m_bool_Convert_Global==TRUE)&&!(refblock==pDoc->nBlocks)&&(EObjProp.m_long_Crosstype!=3)) //Prepare Extra Object to global conversion
		{
			if (x->crosstype==4) EObjProp.m_long_Crosstype=2; //Set crosstype to 2 if extra object with collision should be converted into a global object
			EObjProp.m_int_Chunk=0; //Set chunk to 0 if extra object should be converted into a global object
		}

		if (EObjProp.m_long_Crosstype!=x->crosstype)
		{
			oblock=refblock; //Store reference obj data
			oisxobj=isxobj;
			oobjchunk=objchunk;
			oobjno=objno;
			refblock=EObjProp.m_int_Block; // New obj block
			objchunk=EObjProp.m_int_Chunk; // New obj chunk
			pDoc->CreateObjectData(&NewObjData,oblock,oisxobj,oobjchunk,oobjno,FALSE); //Store polygon and texture data of object
			pDoc->NewObject(refblock,isxobj,objchunk,0,-32768,EObjProp.m_long_Crosstype); //Base object for import
			objno=pDoc->xobj[4*refblock+objchunk].nobj-1; //Get objno of new created object
			x=&pDoc->xobj[4*refblock+objchunk].obj[objno]; //Set pointer x to new xobj
			ox=&pDoc->xobj[4*oblock+oobjchunk].obj[oobjno]; //ox is the old xobj
			pDoc->MoveObjectBy(refblock,isxobj,objchunk,objno,(ox->ptRef.x - x->ptRef.x),(ox->ptRef.y - x->ptRef.y),(ox->ptRef.z - x->ptRef.z));//Move new object center to old center position
			pDoc->ImportObject(&NewObjData,refblock,isxobj,objchunk,objno,TRUE); // Import Object from strored data
			pDoc->DelPolygon(refblock,isxobj,objchunk,objno,0); //Delete first given polygon
			dofree(NewObjData.Vertices); //Give back memory
			dofree(NewObjData.Polys);
			if (x->crosstype==3)
			{
				x->AnimMemory=EObjProp.m_long_AnimMemory;
				x->AnimDelay=EObjProp.m_short_AnimDelay;
			}
			pDoc->DelObject(oblock,oisxobj,oobjchunk,oobjno); //delete old object.
			objno=pDoc->xobj[4*refblock+objchunk].nobj-1; //Maybe objno has changed, but it's still the last object in chunk
			pDoc->FindObjMinMax(refblock,isxobj,objchunk,objno);
			refnode=&pDoc->MemObjMiddle; //Setview to new object center
			InvalidateRect(NULL,TRUE);
		}

		if ((EObjProp.m_int_Block!=refblock)|(EObjProp.m_int_Chunk!=objchunk))
		{
			pDoc->CopyObjectTo(refblock, isxobj, objchunk, objno, EObjProp.m_int_Block,EObjProp.m_int_Chunk);
			pDoc->DelObject(refblock, isxobj, objchunk, objno);
			refblock=EObjProp.m_int_Block;
			objchunk=EObjProp.m_int_Chunk;
			objno=pDoc->xobj[4*refblock+objchunk].nobj-1;
			pDoc->FindObjMinMax(refblock,isxobj,objchunk,objno);
			refnode=&pDoc->MemObjMiddle; //Setview to new object center
			InvalidateRect(NULL,TRUE);
		}

		if (EObjProp.m_bool_Convert_Global==TRUE) //Convert Global <-> Extra Objects
		{
			if (refblock==pDoc->nBlocks) //Convert global to extra object
			{
				x=&pDoc->xobj[4*refblock+objchunk].obj[objno];
				pDoc->FindObjMinMax(refblock,isxobj,objchunk,objno); //Calculate Center of object
				blk=pDoc->trk[0];
				dist0=(float)(sqrt((pDoc->MemObjMiddle.x - blk.ptCentre.x)*(pDoc->MemObjMiddle.x - blk.ptCentre.x)+
								   (pDoc->MemObjMiddle.y - blk.ptCentre.y)*(pDoc->MemObjMiddle.y - blk.ptCentre.y)+
	  							   (pDoc->MemObjMiddle.z - blk.ptCentre.z)*(pDoc->MemObjMiddle.z - blk.ptCentre.z)));
				  num=0;
				for (i=0;i<pDoc->nBlocks;i++) {
					blk=pDoc->trk[i];
					dist= (float)(sqrt((pDoc->MemObjMiddle.x - blk.ptCentre.x)*(pDoc->MemObjMiddle.x - blk.ptCentre.x)+
								       (pDoc->MemObjMiddle.y - blk.ptCentre.y)*(pDoc->MemObjMiddle.y - blk.ptCentre.y)+
	  							       (pDoc->MemObjMiddle.z - blk.ptCentre.z)*(pDoc->MemObjMiddle.z - blk.ptCentre.z)));
					if (dist<dist0) { num=i; dist0=dist; }
				}

				if (x->crosstype==1)
				{
					x->crosstype=2; //No Crosstype 1 on extra objects...
					EObjProp.m_bool_ReCenter=TRUE; //Recenter the new XOBJ
				}
				pDoc->ChangeObjBlock(refblock,isxobj,objchunk,objno,num); //Create XOBJ on block 0
				refblock=num;
				objno=pDoc->xobj[4*refblock+objchunk].nobj-1;
				pDoc->FindObjMinMax(refblock,isxobj,objchunk,objno);
				refnode=&pDoc->MemObjMiddle; //Setview to new object center
				InvalidateRect(NULL,TRUE);
			}
			else //Convert extra to global object
			{
				pDoc->ChangeObjBlock(refblock,isxobj,objchunk,objno,pDoc->nBlocks); //Create XOBJ on last block +1
				refblock=pDoc->nBlocks;
				objchunk=0; //Global has only chunk 0
				objno=pDoc->xobj[4*refblock+objchunk].nobj-1;
				x=&pDoc->xobj[4*refblock+objchunk].obj[objno];
				if (x->crosstype==2) //No Crosstype 2 on global objects..
				{
					pDoc->ChangeXobjRefpoint(refblock, isxobj, objchunk, objno, 0, 0, 0);
					x->crosstype=1;
					/*v=x->vert;
					for (k=0;k<x->nVertices;k++)
					{
						v[k].x-=dx;
						v[k].y-=dy;
						v[k].z-=dz;
					}*/

				}
				pDoc->FindObjMinMax(refblock,isxobj,objchunk,objno);
				refnode=&pDoc->MemObjMiddle; //Setview to new object center
				InvalidateRect(NULL,TRUE);
			}

		}

		blk=pDoc->trk[refblock];
		x=&pDoc->xobj[4*refblock+objchunk].obj[objno]; //Set pointer again if object has changed
		if (x->crosstype==4)
		{
			pDoc->PrepareModifyXobj(4*refblock+objchunk);
			rx=blk.xobj + x->crossno;
			if (EObjProp.m_short_unknown1!=rx->unknown1) (rx->unknown1=EObjProp.m_short_unknown1);
			if (EObjProp.m_short_unknown2!=rx->unknown2) (rx->unknown2=EObjProp.m_short_unknown2);
			if (EObjProp.m_short_collide_effect!=rx->collideeffect) (rx->collideeffect=(char) EObjProp.m_short_collide_effect);
		}
		if (EObjProp.m_bool_ReCenter==TRUE) //ReCenter Object
		{
			pDoc->FindObjMinMax(refblock,isxobj,objchunk,objno);
			if (EObjProp.m_int_ReCenter==1) pDoc->ChangeXobjRefpoint(refblock, isxobj, objchunk, objno, pDoc->MemObjMiddle.x,pDoc->MemObjMiddle.y,pDoc->MemObjMax.z);
			if (EObjProp.m_int_ReCenter==2) pDoc->ChangeXobjRefpoint(refblock, isxobj, objchunk, objno, pDoc->MemObjMiddle.x,pDoc->MemObjMiddle.y,pDoc->MemObjMiddle.z);
			if (EObjProp.m_int_ReCenter==3) pDoc->ChangeXobjRefpoint(refblock, isxobj, objchunk, objno, pDoc->MemObjMiddle.x,pDoc->MemObjMiddle.y,pDoc->MemObjMin.z);
			if (EObjProp.m_int_ReCenter==4) pDoc->ChangeXobjRefpoint(refblock, isxobj, objchunk, objno, EObjProp.m_float_x, EObjProp.m_float_y, EObjProp.m_float_z);
			refnode=&pDoc->MemObjMiddle; //Setview to new object center
			InvalidateRect(NULL,TRUE);
		}

	}

	if ((selMode==ID_MODE_OBJECT)&&((isxobj==2)||(isxobj==3))) // Light/special effect or sound object
	{
		CSoundLightProps ts;
	
		if (isxobj==3) 
		{	
			ts.m_tyyppi=blk.soundsrc[objno].type;
			ts.m_strText1="Set sound source data here. Lower Field shows value in Hexadecimal system.";
		}

		if (isxobj==2) 
		{
			ts.m_tyyppi=blk.lightsrc[objno].type;
			ts.m_strText1="Set light/special effect source data here. Lower Field shows value in Hexadecimal system.";
		}

		keke.Format("%X",ts.m_tyyppi); //Show HEX values
		ts.m_tyyppihex=keke;
		ts.m_intBlock=refblock;
		ts.bSLAutoObjMem=pDoc->bAutoObjMem;
		ts.maxblock=(pDoc->nBlocks-1);

		if (ts.DoModal()!=IDOK) return;
	
		if (isxobj==3) 	
			pDoc->trk[refblock].soundsrc[objno].type=ts.m_tyyppi;
		if (isxobj==2) 
			pDoc->trk[refblock].lightsrc[objno].type=ts.m_tyyppi;
		if (ts.m_intBlock!=refblock)
			{
			pDoc->PrepareNewUndo(refnode,refblock,offsetx,offsety); //undo should be possible
			pDoc->ChangeObjBlock(refblock, isxobj, objchunk, objno,ts.m_intBlock);
			sel=NULL;
			refblock=ts.m_intBlock;
			refnode=&(pDoc->trk[refblock].ptCentre); //Setview to Block Centre
			}
	}
	
	UpdateRefBlock(); //Maybe block has chaaged, show it.
	InvalidateRect(NULL,TRUE);
}

void CT3EDView::OnCopyMode() 
{
	// TODO: Add your command handler code here
	CT3EDDoc *pDoc=GetDocument();
	if (pDoc->ZCopyMode==1)
	{
		pDoc->ZCopyMode=2;
		statusbar->SetPaneText(4,"Shader (z)", TRUE);
	}
	else if (pDoc->ZCopyMode==2)
	{
		pDoc->ZCopyMode=3;
		statusbar->SetPaneText(4,"Average (z)", TRUE);
	}
	else if (pDoc->ZCopyMode==3)
	{
		pDoc->ZCopyMode=4;
		statusbar->SetPaneText(4,"Line (x,y)", TRUE);
	}
	else if (pDoc->ZCopyMode==4)
	{
		pDoc->ZCopyMode=5;
		statusbar->SetPaneText(4,"Line (x,y,z)", TRUE);
	}

	else
	{
		pDoc->ZCopyMode=1;
		statusbar->SetPaneText(4,"Normal (z)", TRUE);
	}

}

void CT3EDView::OnViewEditblock() 
{
	BlkHighlight=!BlkHighlight;
}

void CT3EDView::OnUpdateViewEditblock(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!isEmpty);
	pCmdUI->SetCheck(BlkHighlight&&!isEmpty);
	InvalidateRect(NULL,TRUE);
	
}

void CT3EDView::OnViewPolyflag() 
{
	// TODO: Add your command handler code here
	DrawFlags=!DrawFlags;
}

void CT3EDView::OnUpdateViewPolyflag(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!isEmpty);
	pCmdUI->SetCheck(DrawFlags&&!isEmpty);
	InvalidateRect(NULL,TRUE);	
}

void CT3EDView::OnToolsLodgen() 
{
	// TODO: Add your command handler code here
	CT3EDDoc *pDoc=GetDocument();
	int dippa;
	int startbl, endbl, maxbl;

	CLodStartEnd LodStartEnd;
	maxbl=pDoc->nBlocks;
	LodStartEnd.m_iLodStart=0;
	LodStartEnd.m_iLodEnd=maxbl;
	if (!(LodStartEnd.DoModal () == IDOK)) return; //Cancel pressed
	
	startbl = LodStartEnd.m_iLodStart;
	endbl = LodStartEnd.m_iLodEnd;
	if (endbl>maxbl)
		endbl=maxbl;
	if (startbl>endbl)
		startbl=endbl;

	CWaitCursor WCursur; //Show wait cursor

	CString keke;
	CLodGenProgress *ProgresWin;
	//ProgresWin.EnableWindow();
	//ProgresWin.m_okbutton.EnableWindow();
	ProgresWin = new CLodGenProgress;
	//qfsloadbox->m_progress.SetRange(0,nQfsEntries);
	ProgresWin->Create(IDD_LOD_GENERATION, NULL);
	ProgresWin->ShowWindow(TRUE);
	ProgresWin->UpdateData(FALSE);
	ProgresWin->m_lodprogress.SetRange(startbl,endbl);

	for (dippa=startbl;dippa<endbl;dippa++)
	{
		ProgresWin->m_lodprogress.SetPos(dippa);
		
		keke.Format("Genererating meshes... Block %d (%d of %d Blocks) Med" ,dippa, dippa-startbl, endbl-startbl);
		ProgresWin->SetWindowText(keke);
		keke=pDoc->MedDetailFixer(dippa, 1);
		
		keke.Format("Genererating meshes... Block %d (%d of %d Blocks) Low" ,dippa,dippa-startbl, endbl-startbl);
		ProgresWin->SetWindowText(keke);
		keke=pDoc->MedDetailFixer(dippa, 2);
		
		keke.Format("Genererating meshes... Block %d (%d of %d Blocks) Sort" ,dippa,dippa-startbl, endbl-startbl);
		ProgresWin->SetWindowText(keke);
		keke=pDoc->VertArrayFixer(dippa);
		//keke.Format("Please wait while genererating meshes... (%d of %d Blocks)",dippa, pDoc->nBlocks);
		//ProgresWin.m_progress=keke;
	}
	MessageBox("LOD genereration finished !");

	delete ProgresWin;
}

void CT3EDView::OnVirtualroadmask() 
{
	
	// TODO: Add your command handler code here
	DrawVRE=DrawVRB;	
	DrawVRB=!DrawVRB;
	
}

void CT3EDView::OnVirtualroadedgemask() 
{
	DrawVRE=DrawVRB;	
	DrawVRB=!DrawVRB;
	// TODO: Add your command handler code here
	
}

void CT3EDView::OnUpdateVirtualroadmask(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!isEmpty);
	pCmdUI->SetCheck(DrawVRB&&!isEmpty);
	InvalidateRect(NULL,TRUE);	
	
}

void CT3EDView::OnUpdateVirtualroadedgemask(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!isEmpty);
	pCmdUI->SetCheck(DrawVRE&&!isEmpty);
	InvalidateRect(NULL,TRUE);	
}

void CT3EDView::OnNonpassablepolygonmask() 
{
	// TODO: Add your command handler code here
	DrawNonPass=!DrawNonPass;
	
}

void CT3EDView::OnUpdateNonpassablepolygonmask(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!isEmpty);
	pCmdUI->SetCheck(DrawNonPass&&!isEmpty);
	InvalidateRect(NULL,TRUE);		
}

void CT3EDView::OnModeVroadedit() 
{
	DrawVRoadBitMap=FALSE;
	DrawVRoad=TRUE;
	if (isDragging) return; // safety
	CleanCursorZone(selMode,FALSE);
	selMode=ID_MODE_OBJECT;
	if (multisel!=NULL) { delete multisel; multisel=NULL; }
	isTruePoint=FALSE; refpoly=NULL;
	editMode=NULL;
	CleanCursorZone(selMode,TRUE);
	InvalidateRect(NULL,TRUE);
}

void CT3EDView::OnUpdateModeVroadedit(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!isEmpty);
	pCmdUI->SetCheck(editMode==ID_MODE_VROADEDIT);	

}

void CT3EDView::OnExportTexture() 
{
	// TODO: Add your command handler code here
		//MessageBox("alussa");
	CT3EDDoc *pDoc=GetDocument();
	int i,j,k,l,num;
	int truedx,truedy;
	LPPOLYGONDATA p;
	struct FLOATPT *v;
	struct OBJPOLYBLOCK *o;
	struct XOBJDATA *x;
	//struct LIGHTSRC *light;
	//struct SOUNDSRC *sound;
	FILE* fout;
	//MessageBox("dokumentti saatu");

	fout = fopen("c:/Textures.txt", "w");
		//MessageBox("Tiedosto auki.");
		fprintf(fout, "%f\n", 3.0);
		//fprintf(fout, "%d\n", pDoc->nBlocks);
		
		fprintf(fout, "LOW\n");
		for (i=0;i<pDoc->nBlocks;i++) {
			v=pDoc->trk[i].vert;
			p=pDoc->poly[i].poly[0];
			num=pDoc->poly[i].sz[0];
			for (j=0;j<num;j++,p++) {
				//p->texture
				fprintf(fout, "%d\n", p->texture);
				/*
				pDC->MoveTo(FloatX(v[p->vertex[0]]),FloatY(v[p->vertex[0]]));
				pDC->LineTo(FloatX(v[p->vertex[1]]),FloatY(v[p->vertex[1]]));
				pDC->LineTo(FloatX(v[p->vertex[2]]),FloatY(v[p->vertex[2]]));
				pDC->LineTo(FloatX(v[p->vertex[3]]),FloatY(v[p->vertex[3]]));
				pDC->LineTo(FloatX(v[p->vertex[0]]),FloatY(v[p->vertex[0]])); */
			}
		}
		fprintf(fout, "MEDIUM\n");
		for (i=0;i<pDoc->nBlocks;i++) {
			v=pDoc->trk[i].vert;
			p=pDoc->poly[i].poly[2];
			num=pDoc->poly[i].sz[2];
			for (j=0;j<num;j++,p++) {
				fprintf(fout, "%d\n", p->texture);
				//p->texture
				/*
				pDC->MoveTo(FloatX(v[p->vertex[0]]),FloatY(v[p->vertex[0]]));
				pDC->LineTo(FloatX(v[p->vertex[1]]),FloatY(v[p->vertex[1]]));
				pDC->LineTo(FloatX(v[p->vertex[2]]),FloatY(v[p->vertex[2]]));
				pDC->LineTo(FloatX(v[p->vertex[3]]),FloatY(v[p->vertex[3]]));
				pDC->LineTo(FloatX(v[p->vertex[0]]),FloatY(v[p->vertex[0]])); */
			}
		}
		fprintf(fout, "HIGH\n");
		for (i=0;i<pDoc->nBlocks;i++) {
			v=pDoc->trk[i].vert;
			p=pDoc->poly[i].poly[4];
			num=pDoc->poly[i].sz[4];
			for (j=0;j<num;j++,p++) {
				fprintf(fout, "%d\n", p->texture);
				//p->texture
				/*
				pDC->MoveTo(FloatX(v[p->vertex[0]]),FloatY(v[p->vertex[0]]));
				pDC->LineTo(FloatX(v[p->vertex[1]]),FloatY(v[p->vertex[1]]));
				pDC->LineTo(FloatX(v[p->vertex[2]]),FloatY(v[p->vertex[2]]));
				pDC->LineTo(FloatX(v[p->vertex[3]]),FloatY(v[p->vertex[3]]));
				pDC->LineTo(FloatX(v[p->vertex[0]]),FloatY(v[p->vertex[0]])); */
			}
		}
		
		fprintf(fout, "LANES\n");
		for (i=0;i<pDoc->nBlocks;i++) {
			v=pDoc->trk[i].vert;
			for (k=1;k<6;k=k+2) {
				p=pDoc->poly[i].poly[k];
				num=pDoc->poly[i].sz[k];
				for (j=0;j<num;j++,p++) {
					fprintf(fout, "%d\n", p->texture);
					/*pDC->MoveTo(FloatX(v[p->vertex[0]]),FloatY(v[p->vertex[0]]));
					pDC->LineTo(FloatX(v[p->vertex[1]]),FloatY(v[p->vertex[1]]));
					pDC->LineTo(FloatX(v[p->vertex[2]]),FloatY(v[p->vertex[2]]));
					pDC->LineTo(FloatX(v[p->vertex[3]]),FloatY(v[p->vertex[3]]));
					pDC->LineTo(FloatX(v[p->vertex[0]]),FloatY(v[p->vertex[0]]));*/
				}
			}
		}
		fprintf(fout, "OBJS\n");
		for (i=0;i<pDoc->nBlocks;i++) {
			v=pDoc->trk[i].vert;
			for (j=0;j<4;j++) {
				o=&(pDoc->poly[i].obj[j]);
				if (o->n1==0) continue;
				for (k=0;k<o->nobj;k++) {
					p=o->poly[k];
					num=o->numpoly[k];
					for (l=0;l<num;l++,p++) {
						fprintf(fout, "%d\n", p->texture);
						/*pDC->MoveTo(FloatX(v[p->vertex[0]]),FloatY(v[p->vertex[0]]));
						pDC->LineTo(FloatX(v[p->vertex[1]]),FloatY(v[p->vertex[1]]));
						pDC->LineTo(FloatX(v[p->vertex[2]]),FloatY(v[p->vertex[2]]));
						pDC->LineTo(FloatX(v[p->vertex[3]]),FloatY(v[p->vertex[3]]));
						pDC->LineTo(FloatX(v[p->vertex[0]]),FloatY(v[p->vertex[0]]));*/
					}
				}
			}
		}
		fprintf(fout, "XOBJS\n");
		truedx=dxoffs; truedy=dyoffs;
		for (i=0;i<4*pDoc->nBlocks;i++) {
			
			x=pDoc->xobj[i].obj;
			for (j=0;j<pDoc->xobj[i].nobj;j++,x++) {
				dxoffs=FloatX(x->ptRef); dyoffs=FloatY(x->ptRef);
				v=x->vert;
				p=x->polyData;
				for (k=0;k<x->nPolygons;k++,p++) {
					fprintf(fout, "%d\n", p->texture);
					/*pDC->MoveTo(FloatX(v[p->vertex[0]]),FloatY(v[p->vertex[0]]));
					pDC->LineTo(FloatX(v[p->vertex[1]]),FloatY(v[p->vertex[1]]));
					pDC->LineTo(FloatX(v[p->vertex[2]]),FloatY(v[p->vertex[2]]));
					pDC->LineTo(FloatX(v[p->vertex[3]]),FloatY(v[p->vertex[3]]));
					pDC->LineTo(FloatX(v[p->vertex[0]]),FloatY(v[p->vertex[0]]));*/
				}
				dxoffs=truedx; dyoffs=truedy;
			}
		}

//***************************************************************************
	fclose(fout);
	MessageBox("Texture Data Exportted.");	
	
}

void CT3EDView::OnImportTexture() 
{
	// TODO: Add your command handler code here

CT3EDDoc *pDoc=GetDocument();
	int i,j,k,l,num;
	int truedx,truedy;
	LPPOLYGONDATA p;
	struct FLOATPT *v;
	struct OBJPOLYBLOCK *o;
	struct XOBJDATA *x;
	//struct LIGHTSRC *light;
	//struct SOUNDSRC *sound;
	FILE* fout;
	char input[10];
	//MessageBox("dokumentti saatu");

	fout = fopen("c:/Textures.txt", "r");
		//MessageBox("Tiedosto auki.");
		fscanf(fout, "%s\n", &input);
		//fprintf(fout, "%d\n", pDoc->nBlocks);
		fscanf(fout, "%s\n", &input);
		
		for (i=0;i<pDoc->nBlocks;i++) {
			v=pDoc->trk[i].vert;
			p=pDoc->poly[i].poly[0];
			num=pDoc->poly[i].sz[0];
			for (j=0;j<num;j++,p++) {
				//p->texture
				fscanf(fout, "%hd\n", &p->texture);
				/*
				pDC->MoveTo(FloatX(v[p->vertex[0]]),FloatY(v[p->vertex[0]]));
				pDC->LineTo(FloatX(v[p->vertex[1]]),FloatY(v[p->vertex[1]]));
				pDC->LineTo(FloatX(v[p->vertex[2]]),FloatY(v[p->vertex[2]]));
				pDC->LineTo(FloatX(v[p->vertex[3]]),FloatY(v[p->vertex[3]]));
				pDC->LineTo(FloatX(v[p->vertex[0]]),FloatY(v[p->vertex[0]])); */
			}
		}
		fscanf(fout, "%s\n", &input);
		for (i=0;i<pDoc->nBlocks;i++) {
			v=pDoc->trk[i].vert;
			p=pDoc->poly[i].poly[2];
			num=pDoc->poly[i].sz[2];
			for (j=0;j<num;j++,p++) {
				fscanf(fout, "%hd\n", &p->texture);
				//p->texture
				/*
				pDC->MoveTo(FloatX(v[p->vertex[0]]),FloatY(v[p->vertex[0]]));
				pDC->LineTo(FloatX(v[p->vertex[1]]),FloatY(v[p->vertex[1]]));
				pDC->LineTo(FloatX(v[p->vertex[2]]),FloatY(v[p->vertex[2]]));
				pDC->LineTo(FloatX(v[p->vertex[3]]),FloatY(v[p->vertex[3]]));
				pDC->LineTo(FloatX(v[p->vertex[0]]),FloatY(v[p->vertex[0]])); */
			}
		}
		fscanf(fout, "%s\n", &input);
		for (i=0;i<pDoc->nBlocks;i++) {
			v=pDoc->trk[i].vert;
			p=pDoc->poly[i].poly[4];
			num=pDoc->poly[i].sz[4];
			for (j=0;j<num;j++,p++) {
				fscanf(fout, "%hd\n", &p->texture);
				//p->texture
				/*
				pDC->MoveTo(FloatX(v[p->vertex[0]]),FloatY(v[p->vertex[0]]));
				pDC->LineTo(FloatX(v[p->vertex[1]]),FloatY(v[p->vertex[1]]));
				pDC->LineTo(FloatX(v[p->vertex[2]]),FloatY(v[p->vertex[2]]));
				pDC->LineTo(FloatX(v[p->vertex[3]]),FloatY(v[p->vertex[3]]));
				pDC->LineTo(FloatX(v[p->vertex[0]]),FloatY(v[p->vertex[0]])); */
			}
		}
		
		fscanf(fout, "%s\n", &input);
		for (i=0;i<pDoc->nBlocks;i++) {
			v=pDoc->trk[i].vert;
			for (k=1;k<6;k=k+2) {
				p=pDoc->poly[i].poly[k];
				num=pDoc->poly[i].sz[k];
				for (j=0;j<num;j++,p++) {
					fscanf(fout, "%hd\n", &p->texture);
					/*pDC->MoveTo(FloatX(v[p->vertex[0]]),FloatY(v[p->vertex[0]]));
					pDC->LineTo(FloatX(v[p->vertex[1]]),FloatY(v[p->vertex[1]]));
					pDC->LineTo(FloatX(v[p->vertex[2]]),FloatY(v[p->vertex[2]]));
					pDC->LineTo(FloatX(v[p->vertex[3]]),FloatY(v[p->vertex[3]]));
					pDC->LineTo(FloatX(v[p->vertex[0]]),FloatY(v[p->vertex[0]]));*/
				}
			}
		}
		fscanf(fout, "%s\n", &input);
		for (i=0;i<pDoc->nBlocks;i++) {
			v=pDoc->trk[i].vert;
			for (j=0;j<4;j++) {
				o=&(pDoc->poly[i].obj[j]);
				if (o->n1==0) continue;
				for (k=0;k<o->nobj;k++) {
					p=o->poly[k];
					num=o->numpoly[k];
					for (l=0;l<num;l++,p++) {
						fscanf(fout, "%hd\n", &p->texture);
						/*pDC->MoveTo(FloatX(v[p->vertex[0]]),FloatY(v[p->vertex[0]]));
						pDC->LineTo(FloatX(v[p->vertex[1]]),FloatY(v[p->vertex[1]]));
						pDC->LineTo(FloatX(v[p->vertex[2]]),FloatY(v[p->vertex[2]]));
						pDC->LineTo(FloatX(v[p->vertex[3]]),FloatY(v[p->vertex[3]]));
						pDC->LineTo(FloatX(v[p->vertex[0]]),FloatY(v[p->vertex[0]]));*/
					}
				}
			}
		}
		fscanf(fout, "%s\n", &input);
		truedx=dxoffs; truedy=dyoffs;
		for (i=0;i<4*pDoc->nBlocks;i++) {
			
			x=pDoc->xobj[i].obj;
			for (j=0;j<pDoc->xobj[i].nobj;j++,x++) {
				dxoffs=FloatX(x->ptRef); dyoffs=FloatY(x->ptRef);
				v=x->vert;
				p=x->polyData;
				for (k=0;k<x->nPolygons;k++,p++) {
					fscanf(fout, "%hd\n", &p->texture);
					/*pDC->MoveTo(FloatX(v[p->vertex[0]]),FloatY(v[p->vertex[0]]));
					pDC->LineTo(FloatX(v[p->vertex[1]]),FloatY(v[p->vertex[1]]));
					pDC->LineTo(FloatX(v[p->vertex[2]]),FloatY(v[p->vertex[2]]));
					pDC->LineTo(FloatX(v[p->vertex[3]]),FloatY(v[p->vertex[3]]));
					pDC->LineTo(FloatX(v[p->vertex[0]]),FloatY(v[p->vertex[0]]));*/
				}
				dxoffs=truedx; dyoffs=truedy;
			}
		}

//***************************************************************************
	fclose(fout);
	MessageBox("Texture Data Imported Successfully.");	
}

void CT3EDView::OnVirroadbitmap() 
{
	// TODO: Add your command handler code here
	DrawVRoadBitMap=!DrawVRoadBitMap;
}

void CT3EDView::OnUpdateVirroadbitmap(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!isEmpty);
	pCmdUI->SetCheck(DrawVRoadBitMap&&!isEmpty);
	InvalidateRect(NULL,TRUE);
}

BOOL CT3EDView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	//Zoom In/Out using Mouse Wheel 
	if (nFlags!=MK_CONTROL)
	{
		if (zDelta>0)
			OnViewZoomin();
		if (zDelta<0)
			OnViewZoomout();
	}
	else
	{
		if (zDelta>0)
			OnViewZoomin3d();
		if (zDelta<0)
			OnViewZoomout3d();
	}

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CT3EDView::OnToolsShadowraytracer() 
{
	// TODO: Add your command handler code here
	CT3EDDoc *pDoc=GetDocument(); 
	CString keke, keke2;
	CString sStr = "Test";
	int dippa;
	float theta, rho;
	int startbl=0, endbl=0, maxbl=0;
	CLodGenProgress *ProgresWin;
		
	CRayTraceDlg RayTraceDlg;

	//Default settings for dialog
	RayTraceDlg.m_theta=pDoc->settings.ray_m_theta;
	RayTraceDlg.m_rho=pDoc->settings.ray_m_rho;
	RayTraceDlg.m_iStartBlock=pDoc->settings.ray_m_iStartBlock;
	if (pDoc->settings.ray_m_iEndBlock>=pDoc->nBlocks)
		pDoc->settings.ray_m_iEndBlock=pDoc->nBlocks;
	sStr.Format("End block (%u = whole track) : ",pDoc->nBlocks);
	RayTraceDlg.m_text_endbl=sStr;
	RayTraceDlg.m_iEndBlock=pDoc->settings.ray_m_iEndBlock;
	RayTraceDlg.m_bCheckTrack=pDoc->settings.ray_m_bCheckTrack;
	RayTraceDlg.m_bCheckObjects=pDoc->settings.ray_m_bCheckObjects;

	if (RayTraceDlg.DoModal()!=IDOK) return; // Cancel pressed
	if (!(RayTraceDlg.m_bCheckTrack|RayTraceDlg.m_bCheckObjects)) return; //Nothing selected
	theta = RayTraceDlg.m_theta;
	rho = RayTraceDlg.m_rho;
	startbl = RayTraceDlg.m_iStartBlock;
	endbl = RayTraceDlg.m_iEndBlock;
	if (endbl>pDoc->nBlocks)
		endbl=pDoc->nBlocks;
	if (startbl>endbl)
		startbl=endbl;
	//Save settings for next time
	pDoc->settings.ray_m_theta=theta;
	pDoc->settings.ray_m_rho=rho;
	pDoc->settings.ray_m_iEndBlock=endbl;
	pDoc->settings.ray_m_iStartBlock=startbl;
	pDoc->settings.ray_m_bCheckTrack=RayTraceDlg.m_bCheckTrack;
	pDoc->settings.ray_m_bCheckObjects=RayTraceDlg.m_bCheckObjects;


	ProgresWin = new CLodGenProgress;

	ProgresWin->Create(IDD_LOD_GENERATION, NULL);
	ProgresWin->ShowWindow(TRUE);
	ProgresWin->UpdateData(FALSE);
	ProgresWin->m_lodprogress.SetRange(startbl,endbl);
	
	CWaitCursor WCursur; //Show wait cursor

	for (dippa=startbl;dippa<endbl;dippa++) //Shadow raytracing for selected Blocks
	{
		ProgresWin->m_lodprogress.SetPos(dippa);
		if (RayTraceDlg.m_bCheckTrack)
		{
		keke2.Format("RayTracing Shadows (Block  %d)... (%d of %d Blocks)",dippa, dippa-startbl, endbl-startbl);
		ProgresWin->SetWindowText(keke2);
		keke=pDoc->BlockShadingFixer(dippa, theta, rho);
		}
		if (RayTraceDlg.m_bCheckObjects)
		{
		keke2.Format("RayTracing Shadows (Object %d)... (%d of %d Blocks)",dippa, dippa-startbl, endbl-startbl);
		ProgresWin->SetWindowText(keke2);
		keke=pDoc->ObjectShadingFixer(dippa, theta, rho);
		}
		refblock=dippa;
		InvalidateRect(NULL, TRUE);
	}

	MessageBox("RayTracing Shadows finished !");
	delete ProgresWin;
}

void CT3EDView::OnPASTEZl(void)
{
// MessageBox("Q!");
}


void CT3EDView::OnEditPasteNormal() 
{
	// TODO: Code für Befehlsbehandlungsroutine hier einfügen
	CT3EDDoc *pDoc=GetDocument();
	pDoc->ZCopyMode=1;
	statusbar->SetPaneText(4,"Normal (z)", TRUE);
}

void CT3EDView::OnEditPasteShader() 
{
	// TODO: Code für Befehlsbehandlungsroutine hier einfügen
	CT3EDDoc *pDoc=GetDocument();
	pDoc->ZCopyMode=2;
	statusbar->SetPaneText(4,"Shader (z)", TRUE);
}

void CT3EDView::OnEditPasteAverage() 
{
	// TODO: Code für Befehlsbehandlungsroutine hier einfügen
	CT3EDDoc *pDoc=GetDocument();
	pDoc->ZCopyMode=3;
	statusbar->SetPaneText(4,"Average (z)", TRUE);
}

void CT3EDView::OnEditPasteLineXy() 
{
	CT3EDDoc *pDoc=GetDocument();
	pDoc->ZCopyMode=4;
	statusbar->SetPaneText(4,"Line (x,y)", TRUE);
}

void CT3EDView::OnEditPasteLineXyz() 
{
	CT3EDDoc *pDoc=GetDocument();
	pDoc->ZCopyMode=5;
	statusbar->SetPaneText(4,"Line (x,y,z)", TRUE);	
}

void CT3EDView::OnUpdateEditPasteNormal(CCmdUI* pCmdUI) 
{
	CT3EDDoc *pDoc=GetDocument();
	pCmdUI->Enable(!isEmpty);
	pCmdUI->SetCheck((pDoc->ZCopyMode==1)&&!isEmpty);
}

void CT3EDView::OnUpdateEditPasteShader(CCmdUI* pCmdUI) 
{
	CT3EDDoc *pDoc=GetDocument();
	pCmdUI->Enable(!isEmpty);
	pCmdUI->SetCheck((pDoc->ZCopyMode==2)&&!isEmpty);
}

void CT3EDView::OnUpdateEditPasteAverage(CCmdUI* pCmdUI) 
{
	CT3EDDoc *pDoc=GetDocument();
	pCmdUI->Enable(!isEmpty);
	pCmdUI->SetCheck((pDoc->ZCopyMode==3)&&!isEmpty);
}

void CT3EDView::OnUpdateEditPasteLineXy(CCmdUI* pCmdUI) 
{
	CT3EDDoc *pDoc=GetDocument();
	pCmdUI->Enable(!isEmpty);
	pCmdUI->SetCheck((pDoc->ZCopyMode==4)&&!isEmpty);	
}

void CT3EDView::OnUpdateEditPasteLineXyz(CCmdUI* pCmdUI) 
{
	CT3EDDoc *pDoc=GetDocument();
	pCmdUI->Enable(!isEmpty);
	pCmdUI->SetCheck((pDoc->ZCopyMode==5)&&!isEmpty);	
}

void CT3EDView::OnUpdateCopyZ(CCmdUI* pCmdUI) 
{
	// TODO: Code für die Befehlsbehandlungsroutine zum Aktualisieren der Benutzeroberfläche hier einfügen
	//CT3EDDoc *pDoc=GetDocument();
	pCmdUI->Enable((selMode==ID_MODE_POINT)&&!isEmpty);
	//pCmdUI->SetCheck((pDoc->ZCopyMode==1)&&!isEmpty);
}

void CT3EDView::OnUpdatePasteZh(CCmdUI* pCmdUI) 
{
	// TODO: Code für die Befehlsbehandlungsroutine zum Aktualisieren der Benutzeroberfläche hier einfügen
	pCmdUI->Enable((selMode==ID_MODE_POINT)&&!isEmpty);
}

void CT3EDView::OnUpdateEditCopyXy(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable((selMode==ID_MODE_POINT)&&!isEmpty);
}

void CT3EDView::OnUpdateEditPasteXy(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable((selMode==ID_MODE_POINT)&&!isEmpty);
}

void CT3EDView::OnUpdateToolsExpand(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable((((selMode==ID_MODE_OBJECT)|(selMode==ID_MODE_BLOCK))&&!isEmpty));
}

void CT3EDView::OnUpdateEditmodesAutoobjmember(CCmdUI* pCmdUI) 
{
	CT3EDDoc *pDoc=GetDocument();
	pCmdUI->Enable(!isEmpty);
	pCmdUI->SetCheck(pDoc->bAutoObjMem);	
}

void CT3EDView::OnEditmodesAutoobjmember() 
{
	CT3EDDoc *pDoc=GetDocument();
	pDoc->bAutoObjMem=(!pDoc->bAutoObjMem);	
}

void CT3EDView::OnUpdateToolsRotate(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable((((selMode==ID_MODE_OBJECT)|(selMode==ID_MODE_BLOCK)|(selMode==ID_MODE_POLYGON))&&!isEmpty));
}

void CT3EDView::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!isEmpty);
}

void CT3EDView::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!isEmpty);
}


void CT3EDView::OnUpdateEditUndoKeepVr(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable((!isEmpty)&&(GetDocument()->undoLevel>0));
}

void CT3EDView::OnEditUndoKeepVr()  //Undo without VRoad Data
{
	CT3EDDoc *pDoc=GetDocument();
	struct COLVROAD *vrdata;
	int lev=pDoc->undoLevel-1;

	vrdata=(struct COLVROAD *)malloc(36*pDoc->col.vroadHead.nrec); //Backup VRoad data
	memcpy(vrdata, pDoc->undoCol[lev],36*pDoc->col.vroadHead.nrec);

	if (multisel) delete multisel; multisel=NULL;
	refnode=&myrefnode;
	memcpy(refnode,&(pDoc->undoRefpt[lev]),12);
	refblock=pDoc->undoRefblk[lev];
	offsetx=pDoc->undoOfsx[lev];
	offsety=pDoc->undoOfsy[lev];
	pDoc->PerformUndo(true);
	isTruePoint=FALSE; refpoly=NULL; objno=-1;
	InvalidateRect(NULL,TRUE);

	pDoc->PrepareNewUndo(refnode,refblock,offsetx,offsety); //New undo for VRoad
	if (pDoc->undoCol[pDoc->undoLevel-1]==NULL) {
		pDoc->undoCol[pDoc->undoLevel-1]=(struct COLVROAD *)malloc(36*pDoc->col.vroadHead.nrec);
		memcpy(pDoc->undoCol[pDoc->undoLevel-1],vrdata,36*pDoc->col.vroadHead.nrec);
	}
	dofree(vrdata);
}

void CT3EDView::OnUpdateToolsMoveto(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!isEmpty);	
}

void CT3EDView::OnUpdateToolsProperties(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!isEmpty);	
}

void CT3EDView::OnUpdateToolsNeighbours(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!isEmpty);		
}

void CT3EDView::OnUpdateToolsShadowraytracer(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!isEmpty);	
}

void CT3EDView::OnUpdateToolsLodgen(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!isEmpty);	
}

void CT3EDView::OnUpdateExportVis(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!isEmpty);	
}

void CT3EDView::OnUpdateExportTrkvertices(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!isEmpty);	
}

void CT3EDView::OnUpdateExportTexture(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!isEmpty);	
}

void CT3EDView::OnUpdateImportVis(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!isEmpty);	
}

void CT3EDView::OnUpdateImportTexture(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!isEmpty);	
}

void CT3EDView::OnUpdateImportObject(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!isEmpty);	
}

void CT3EDView::OnUpdateExportObject(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(((selMode==ID_MODE_OBJECT)&&!isEmpty));	
}

void CT3EDView::OnToolsMergeObject()
{
	CT3EDDoc *pDoc=GetDocument();
	CSelData *sel;
	int orefblock,oisxobj,oobjchunk,oobjno=-1;
	struct NOBJDATA NewObjData;
	BOOL changed=FALSE;
	CString sStr = "";

	for (sel=this;sel!=NULL;sel=sel->multisel) //look at all objects
	{
		if ((sel->isxobj<2)&&(isxobj<2)) //Only available for objects
		{
			if ((!((sel->refblock==refblock)&&(sel->isxobj==isxobj)&&(sel->objchunk==objchunk)&&(sel->objno==objno)))&&(sel->isxobj<2)&&(isxobj<2)) //Two different (extra -) objects ? Because sound / light objects causes crashes
			{
				pDoc->PrepareNewUndo(refnode,refblock,offsetx,offsety); //New undo point
				//pDoc->PrepareNewUndo(sel->refnode,sel->refblock,offsetx,offsety);
				orefblock=sel->refblock;oisxobj=sel->isxobj;oobjchunk=sel->objchunk;oobjno=sel->objno;
				if ((sel->refblock==refblock)&&(sel->isxobj==isxobj)&&(sel->objchunk==objchunk)&&(objno<sel->objno))
					oobjno--; //decrease objno if deleted object is stored before
				pDoc->CreateObjectData(&NewObjData,refblock,isxobj,objchunk,objno,FALSE);
				pDoc->ImportObject(&NewObjData,sel->refblock,sel->isxobj,sel->objchunk,sel->objno,TRUE);
				pDoc->DelObject(refblock,isxobj,objchunk,objno);
				changed=TRUE;
			}
		}
	}
	//changed=TRUE;
	if (changed==TRUE)
	{
		offsetx=0; offsety=0;
		if (multisel) { delete multisel; multisel=NULL; }
		refblock=orefblock;isxobj=oisxobj;objchunk=oobjchunk;objno=oobjno;
		pDoc->FindObjMinMax(refblock,isxobj,objchunk,objno);
		refnode=&pDoc->MemObjMiddle; //Setview to new object center
		InvalidateRect(NULL,TRUE);
		sStr.Format("Merged one object with %d polygons.",NewObjData.nPoly);
		statusbar->SetPaneText(0,sStr,TRUE);
		dofree(NewObjData.Vertices); //Give back memory
		dofree(NewObjData.Polys);
	}
	changed=TRUE; //Nur für Haltepunkt, kann weg.
}

void CT3EDView::OnUpdateToolsInvert(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(((selMode==ID_MODE_POLYGON)&&!isEmpty));	
}

void CT3EDView::OnUpdateFileProperties(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!isEmpty);	
}

void CT3EDView::OnUpdateToolsVroadHeightsSpdfiles(CCmdUI* pCmdUI) 
{
	CT3EDDoc *pDoc=GetDocument();
	pCmdUI->Enable((!isEmpty)&&(pDoc->spdFALoaded)&&(pDoc->spdRALoaded)&&(pDoc->HeightsLoaded));		
}

void CT3EDView::OnUpdateEditFind(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!isEmpty&&
		(selMode==ID_MODE_POLYGON));	
}


void CT3EDView::OnUpdateEditMerge(CCmdUI* pCmdUI) 
{
	/*pCmdUI->Enable(!isEmpty&&
		(selMode==ID_MODE_BLOCK));*/
	pCmdUI->Enable(FALSE);
}


void CT3EDView::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(((selMode==ID_MODE_BLOCK)&&!isEmpty));		
}



void CT3EDView::OnUpdateSetVis(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!isEmpty);		
}


void CT3EDView::OnUpdateViewVisibility(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!isEmpty);
	pCmdUI->SetCheck(ShowBasedOnVisi&&!isEmpty);
	InvalidateRect(NULL,TRUE);	
}

void CT3EDView::OnViewVisibility() 
{
	CT3EDDoc* pDoc = GetDocument();
	ShowBasedOnVisi=!ShowBasedOnVisi;
	pDoc->bUpdatedScene=0; //Trackview needs to be refreshed
}

void CT3EDView::OnUpdateVisFwIncrease(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(ShowBasedOnVisi);
}

void CT3EDView::OnUpdateVisFwDecrease(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(ShowBasedOnVisi);
}

void CT3EDView::OnUpdateVisBwIncrease(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(ShowBasedOnVisi);	
}

void CT3EDView::OnUpdateVisBwDecrease(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(ShowBasedOnVisi);	
}

void CT3EDView::TrachViewDrawBlock(int blk, int vblock)
{
	CT3EDDoc* pDoc = GetDocument();

	if (blk>pDoc->nBlocks) blk=blk-pDoc->nBlocks;
	if (blk<0) blk=blk+pDoc->nBlocks;

	//Draw Track and Polygon objects
	if  (pDoc->nBlocks!=blk) pDoc->TrackView->CreateScene(&(pDoc->trk[blk]),&(pDoc->poly[blk]), &(pDoc->qfsView), vblock, nDetail);
	//Draw Extra objects
	pDoc->TrackView->CreateSceneEx(&(pDoc->xobj[blk*4]), &(pDoc->qfsView), vblock*4);
	pDoc->TrackView->CreateSceneEx(&(pDoc->xobj[1+blk*4]), &(pDoc->qfsView), vblock*4+1);
	pDoc->TrackView->CreateSceneEx(&(pDoc->xobj[2+blk*4]), &(pDoc->qfsView), vblock*4+2);
	pDoc->TrackView->CreateSceneEx(&(pDoc->xobj[3+blk*4]), &(pDoc->qfsView), vblock*4+3);
}

int CT3EDView::RealBlockNum(int blk)
{
	CT3EDDoc* pDoc = GetDocument();
	if (blk>=pDoc->nBlocks) blk=blk-pDoc->nBlocks;
	if (blk<0) blk=blk+pDoc->nBlocks;

	return blk;
}

void CT3EDView::OnVisFwIncrease() 
{
	CT3EDDoc* pDoc = GetDocument();
	pDoc->InDecreaseVis(refblock,1,1); //Add a visible block
	pDoc->bUpdatedScene=FALSE;  //Mark Trackpreview as invalid
	InvalidateRect(NULL,TRUE);	//Update View
}

void CT3EDView::OnVisFwDecrease() 
{
	CT3EDDoc* pDoc = GetDocument();
	pDoc->InDecreaseVis(refblock,1,-1);
	pDoc->bUpdatedScene=FALSE;
	InvalidateRect(NULL,TRUE);	
}

void CT3EDView::OnVisBwIncrease() 
{
	CT3EDDoc* pDoc = GetDocument();
	pDoc->InDecreaseVis(refblock,-1,1);
	pDoc->bUpdatedScene=FALSE;
	InvalidateRect(NULL,TRUE);	
}

void CT3EDView::OnVisBwDecrease() 
{
	CT3EDDoc* pDoc = GetDocument();
	pDoc->InDecreaseVis(refblock,-1,-1);
	pDoc->bUpdatedScene=FALSE;
	InvalidateRect(NULL,TRUE);	
}

