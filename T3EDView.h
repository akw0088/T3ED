// T3EDView.h : interface of the CT3EDView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_T3EDVIEW_H__EA21D110_8ED3_11D2_8486_000000000000__INCLUDED_)
#define AFX_T3EDVIEW_H__EA21D110_8ED3_11D2_8486_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define LOW_DETAIL 0
#define MED_DETAIL 2
#define HIGH_DETAIL 4

#define NOT_XOBJ 0
#define IS_XOBJ 1
#define IS_LIGHTSRC 2
#define IS_SOUNDSRC 3
#define IS_VROAD 4

class CSelData 
{
public:
	int refblock, sceneRefBlock;
	struct FLOATPT *refnode;
	struct FLOATPT myrefnode; // if it's an xobj !!!
	// for POLYGON mode
	LPPOLYGONDATA refpoly;
	struct FLOATPT *refvertices;
	long *refshadingvert;
	int refpolychunk; // 0..3 for objects/xobjs, else 0..6
	int refpolyobj;   // object #, -1 if track polygon
	int refpolyno;
	// for both POLYGON and OBJECT modes
	int isxobj; // 0 if not, 1 if xobj, 2 if lightsrc, 3 if soundsrc 
		// in polygon mode, must add refnode==center to coordinates
	// OBJECT mode only
	int objchunk,objno;
	// chaining data
	CSelData *multisel;
	// to detect multimove attempts
	struct FLOATPT origpos[4];
	BOOL origflag;
	// destructor
	virtual ~CSelData() { if (multisel) delete multisel; }
};
	

class CT3EDView : public CView, CSelData
{
protected: // create from serialization only
	CT3EDView();
	DECLARE_DYNCREATE(CT3EDView)

// Attributes
public:
//	int refblock;
	bool DrawVRoad;
	bool BlkHighlight;
	bool ShowBasedOnVisi;
	bool DrawFlags;
	bool DrawVRB;
	bool DrawVRE;
	bool DrawNonPass;
	bool DrawVRoadBitMap;
	int offsetx,offsety; // scrollbar positions
	int dxoffs,dyoffs;   // only used at draw-time
	int theta,phi;
	float cosTheta,sinTheta,cosPhi,sinPhi;
	float scale,scale3d;
//	struct FLOATPT *refnode;
	CStatusBar *statusbar;
	CT3EDDoc* GetDocument();
	BOOL bShowObjects,bShowLanes;
	//HOO: For showing or hiding track (1)
	BOOL bShowTrack;
	BOOL bUpdatedRes;
	//HOO: (1)
	int nDetail; // 0 for lowres, 2 for medres, 4 for hires
	BOOL isScrolling; // view during scrolling ops : nodes, no objects
	BOOL bScrollRotate; // is scrolling rotation mode activated ?
	BOOL extraSmooth;
	BOOL isEmpty;
	CPen *darkbluePen, *bluePen,*lightbluePen,*redPen,*darkgreenPen, *greenPen,*lightgreenPen, *green2Pen,*brownPen,*purplePen, *orangePen, *grayPen,*gray2Pen, *blackPen, *red2Pen;
	int selMode,editMode;
	BOOL isDragging,delayMouseMove;
	int dragX,dragY;
	// for POINT/EXTPOINT modes
	BOOL isTruePoint; // else don't allow moves
//	struct FLOATPT myrefnode; // if it's an xobj !!!
	// for POLYGON mode
//	LPPOLYGONDATA refpoly;
//	struct FLOATPT *refvertices;
//	int refpolychunk; // 0..3 for objects/xobjs, else 0..6
//	int refpolyobj;   // object #, -1 if track polygon
//	int refpolyno;
	// for both POLYGON and OBJECT modes
//	BOOL isxobj; // then add refnode==center to coordinates
	// OBJECT mode only
//	int objchunk,objno;
	// smoothing : EXTPOINT mode, and BLOCK mode
	int smooth2Start,smooth2Width;   // in float dist units
	int smooth1Start,smooth1Width;   // in 1/8 blocks

// Operations
public:
	int FloatX(struct FLOATPT &p);
	int FloatY(struct FLOATPT &p);
	int IntX(struct INTPT &p);
	int IntY(struct INTPT &p);
	int FloatFloatX(struct FLOATPT &p,struct FLOATPT &q);
	int FloatFloatY(struct FLOATPT &p,struct FLOATPT &q);
	BOOL MeetsClipRect(struct TRKBLOCK *t,LPRECT r);
	void UpdateRefBlock();
	void CleanCursorZone(int mode,BOOL newstate);
	BOOL IsVisibleNode(int block,struct FLOATPT *pt);
	BOOL IsPolyOfDetail(int detail);
	void ShowHideCursor();
	void ShowSelectionAt(CDC *pDC,int xx,int yy);
	//HOO: (2)
	void TrackKeyDown(UINT tmsg, BOOL bmode);
	//HOO: (2)

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CT3EDView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	void OnViewZoomout3d();
	void OnViewZoomin3d();
	BOOL FindPolyMatches(FINDPOLYDATA &fpoly, LPPOLYGONDATA &p);
	void OnToolsMergeObject();
	virtual ~CT3EDView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CT3EDView)
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnViewFullres();
	afx_msg void OnViewHalfres();
	afx_msg void OnViewLowres();
	afx_msg void OnViewZoomin();
	afx_msg void OnViewZoomout();
	afx_msg void OnUpdateViewFullres(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewHalfres(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewLowres(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewZoomin(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewZoomout(CCmdUI* pCmdUI);
	afx_msg void OnViewRotateMode();
	afx_msg void OnUpdateViewRotateMode(CCmdUI* pCmdUI);
	afx_msg void OnViewTranslMode();
	afx_msg void OnUpdateViewTranslMode(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnUpdateViewShowhideobjects(CCmdUI* pCmdUI);
	afx_msg void OnViewShowhideobjects();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnModeBlock();
	afx_msg void OnUpdateModeBlock(CCmdUI* pCmdUI);
	afx_msg void OnModeExtpoint();
	afx_msg void OnUpdateModeExtpoint(CCmdUI* pCmdUI);
	afx_msg void OnModePoint();
	afx_msg void OnUpdateModePoint(CCmdUI* pCmdUI);
	afx_msg void OnModeObject();
	afx_msg void OnUpdateModeObject(CCmdUI* pCmdUI);
	afx_msg void OnModePolygon();
	afx_msg void OnUpdateModePolygon(CCmdUI* pCmdUI);
	afx_msg void OnViewRefresh();
	afx_msg void OnViewRecenter();
	afx_msg void OnUpdateViewRecenter(CCmdUI* pCmdUI);
	afx_msg void OnViewShowlanes();
	afx_msg void OnUpdateViewShowlanes(CCmdUI* pCmdUI);
	afx_msg void OnViewShowTrack();
	afx_msg void OnUpdateViewShowTrack(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolsMovexy(CCmdUI* pCmdUI);
	afx_msg void OnToolsMovexy();
	afx_msg void OnUpdateToolsMovez(CCmdUI* pCmdUI);
	afx_msg void OnToolsMovez();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateModeSmoothing(CCmdUI* pCmdUI);
	afx_msg void OnModeSmoothing();
	afx_msg void OnUpdateEditmodesExtrasmoothing(CCmdUI* pCmdUI);
	afx_msg void OnEditmodesExtrasmoothing();
	afx_msg void OnUpdateToolsDelete(CCmdUI* pCmdUI);
	afx_msg void OnToolsDelete();
	afx_msg void OnUpdateToolsDuplicate(CCmdUI* pCmdUI);
	afx_msg void OnToolsDuplicate();
	afx_msg void OnUpdateToolsTexture(CCmdUI* pCmdUI);
	afx_msg void OnToolsTexture();
	afx_msg void OnToolsPolyflag();
	afx_msg void OnUpdateToolsPolyflag(CCmdUI* pCmdUI);
	afx_msg void OnToolsTrackBlockProps();
	afx_msg void OnToolsMerge();
	afx_msg void OnUpdateToolsMerge(CCmdUI* pCmdUI);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnToolsAdjustwidth();
	afx_msg void OnUpdateToolsAdjustwidth(CCmdUI* pCmdUI);
	afx_msg void OnToolsClearall();
	afx_msg void OnUpdateToolsClearall(CCmdUI* pCmdUI);
	afx_msg void OnViewVroad();
	afx_msg void OnUpdateViewVroad(CCmdUI* pCmdUI);
	afx_msg void OnExportVis();
	afx_msg void OnImportVis();
	afx_msg void OnCopyZ();
	afx_msg void OnCOPYZl();
	afx_msg void OnPasteZh();
	afx_msg void OnPASTEZl();
	afx_msg void OnExportTrkvertices();
	afx_msg void OnToolsProperties();
	afx_msg void OnCopyMode();
	afx_msg void OnViewEditblock();
	afx_msg void OnUpdateViewEditblock(CCmdUI* pCmdUI);
	afx_msg void OnViewPolyflag();
	afx_msg void OnUpdateViewPolyflag(CCmdUI* pCmdUI);
	afx_msg void OnToolsLodgen();
	afx_msg void OnVirtualroadmask();
	afx_msg void OnVirtualroadedgemask();
	afx_msg void OnUpdateVirtualroadmask(CCmdUI* pCmdUI);
	afx_msg void OnUpdateVirtualroadedgemask(CCmdUI* pCmdUI);
	afx_msg void OnNonpassablepolygonmask();
	afx_msg void OnUpdateNonpassablepolygonmask(CCmdUI* pCmdUI);
	afx_msg void OnModeVroadedit();
	afx_msg void OnUpdateModeVroadedit(CCmdUI* pCmdUI);
	afx_msg void OnExportTexture();
	afx_msg void OnImportTexture();
	afx_msg void OnVirroadbitmap();
	afx_msg void OnUpdateVirroadbitmap(CCmdUI* pCmdUI);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnToolsShadowraytracer();
	afx_msg void OnCopyXY();
	afx_msg void OnPasteXY();
	afx_msg void OnEditPasteNormal();
	afx_msg void OnEditPasteShader();
	afx_msg void OnEditPasteAverage();
	afx_msg void OnUpdateEditPasteNormal(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPasteShader(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPasteAverage(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCopyZ(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePasteZh(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCopyXy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPasteXy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolsExpand(CCmdUI* pCmdUI);
	afx_msg void OnToolsExpand();
	afx_msg void OnUpdateEditmodesAutoobjmember(CCmdUI* pCmdUI);
	afx_msg void OnEditmodesAutoobjmember();
	afx_msg void OnUpdateToolsRotate(CCmdUI* pCmdUI);
	afx_msg void OnToolsRotate();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditUndoKeepVr(CCmdUI* pCmdUI);
	afx_msg void OnEditUndoKeepVr();
	afx_msg void OnUpdateToolsMoveto(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolsProperties(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolsNeighbours(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolsShadowraytracer(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolsLodgen(CCmdUI* pCmdUI);
	afx_msg void OnUpdateExportVis(CCmdUI* pCmdUI);
	afx_msg void OnUpdateExportTrkvertices(CCmdUI* pCmdUI);
	afx_msg void OnUpdateExportTexture(CCmdUI* pCmdUI);
	afx_msg void OnUpdateImportVis(CCmdUI* pCmdUI);
	afx_msg void OnUpdateImportTexture(CCmdUI* pCmdUI);
	afx_msg void OnToolsMoveto();
	afx_msg void OnUpdateEditPasteLineXy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPasteLineXyz(CCmdUI* pCmdUI);
	afx_msg void OnEditPasteLineXyz();
	afx_msg void OnEditPasteLineXy();
	afx_msg void OnUpdateImportObject(CCmdUI* pCmdUI);
	afx_msg void OnImportObject();
	afx_msg void OnUpdateExportObject(CCmdUI* pCmdUI);
	afx_msg void OnExportObject();
	afx_msg void OnUpdateToolsInvert(CCmdUI* pCmdUI);
	afx_msg void OnToolsInvert();
	afx_msg void OnUpdateFileProperties(CCmdUI* pCmdUI);
	afx_msg void OnFileProperties();
	afx_msg void OnUpdateToolsVroadHeightsSpdfiles(CCmdUI* pCmdUI);
	afx_msg void OnToolsVroadHeightsSpdfiles();
	afx_msg void OnEditFind();
	afx_msg void OnUpdateEditFind(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditMerge(CCmdUI* pCmdUI);
	afx_msg void OnEditMerge();
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnUpdateSetVis(CCmdUI* pCmdUI);
	afx_msg void OnSetVis();
	afx_msg void OnUpdateViewVisibility(CCmdUI* pCmdUI);
	afx_msg void OnViewVisibility();
	afx_msg void OnUpdateVisFwIncrease(CCmdUI* pCmdUI);
	afx_msg void OnUpdateVisFwDecrease(CCmdUI* pCmdUI);
	afx_msg void OnUpdateVisBwIncrease(CCmdUI* pCmdUI);
	afx_msg void OnUpdateVisBwDecrease(CCmdUI* pCmdUI);
	afx_msg void OnVisBwIncrease();
	afx_msg void OnVisBwDecrease();
	afx_msg void OnVisFwIncrease();
	afx_msg void OnVisFwDecrease();
	afx_msg void OnUpdateTrViewZoomin3d(CCmdUI* pCmdUI);
	afx_msg void OnTrViewZoomin3d();
	afx_msg void OnTrViewZoomout3d();
	afx_msg void OnUpdateTrViewZoomout3d(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int RealBlockNum(int blk);
	void TrachViewDrawBlock(int blk, int vblock);
	void NewSel();
	BOOL FindPolyVRMatches(FINDPOLYDATA &fpoly, POLYVROADDATA *pvr, LPPOLYGONDATA &p);
	void FindPoly(FINDPOLYDATA &fpoly, int startbl, int endbl, BOOL FindTrackPoly, BOOL FindObjectPoly, BOOL FindXObjectPoly);
};

#ifndef _DEBUG  // debug version in T3EDView.cpp
inline CT3EDDoc* CT3EDView::GetDocument()
   { return (CT3EDDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_T3EDVIEW_H__EA21D110_8ED3_11D2_8486_000000000000__INCLUDED_)
