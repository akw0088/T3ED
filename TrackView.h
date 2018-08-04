#if !defined(AFX_TRACKVIEW_H__74ABEDA4_4CC1_11D3_A355_00207811A639__INCLUDED_)
#define AFX_TRACKVIEW_H__74ABEDA4_4CC1_11D3_A355_00207811A639__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// TrackView.h : header file
//
// Created by Hoo, 8/16/1999 (All new)
// Hoo's D3D View functions added by Nappe1, (2/11/2001)

// Nappe1: Following line might be adjusted before rebuild:
#include "C:\Programme\DirectX 9.0 SDK\Include\d3drmwin.h"
#define MAX_DRIVERS 10           // Maximum D3D drivers expected 
#define TV_ER_WINDOW		0
#define TV_ER_ENUM			1
#define TV_ER_INIT			2
#define TV_ER_ALLOC			3
#define TV_ER_HDC			4
#define TV_ER_COLORTABLE	5
#define MAXBL	30
#define MAXEBL	120

/////////////////////////////////////////////////////////////////////////////
// CTrackView window
typedef struct DATADATA2
{
	LPVOID bits;
	LPDIRECT3DRMTEXTURE3 lpTex;
} * LPDATADATA2;

class CTrackView : public CWnd
{
// Construction
public:
	CTrackView();

	bool Init(CWnd* pWnd, CQFSView * qfs, TEXTUREBLOCK *texture, BOOL isHSMode);
	void ShowView(BOOL bShow);
	bool EnumDrivers();
	bool InitD3D();
	bool CreateDevAndView(LPDIRECTDRAWCLIPPER lpDDClip, int driver, int width, int height);
	bool CreateScene(TRKBLOCK * b, POLYGONBLOCK * p, CQFSView * qfs, int which, int nDetail);
	bool CreateSceneEx(XOBJBLOCK * x, CQFSView * qfs, int which);
	bool UpdateScene(int theta, int phi, int x, int z, int y, double scale);
	void MakeFrames(LPDIRECT3DRMFRAME3 lpScene, LPDIRECT3DRMFRAME3 lpCamera,
		LPDIRECT3DRMFRAME3 * lplpLightFrame1, LPDIRECT3DRMFRAME3 * lplpWorld_frame);
	void MakeLights(LPDIRECT3DRMFRAME3 lpScene, LPDIRECT3DRMFRAME3 lpCamera, 
		LPDIRECT3DRMFRAME3 lpLightFrame1, LPDIRECT3DRMLIGHT * lplpLight1, float * light,
		LPDIRECT3DRMLIGHT * lplpLight2, float * light2);
	void MakeMesh(LPDIRECT3DRMMESHBUILDER3 * lplp_builder);
	void MakeWrap(short flags, LPDIRECT3DRMFACE lpFace, long nt, bool isback);
	LPDIRECT3DRMTEXTURE3 GetStockTexture(long index);
	LPDIRECT3DRMTEXTURE3 GetTexture(long index);
	bool MakeTexture(CQFSView * qfs);
	void SetPositions(LPDIRECT3DRMFRAME3 lpScene, 
		LPDIRECT3DRMFRAME3 lpCamera, LPDIRECT3DRMFRAME3 lpLightFrame1, 
		LPDIRECT3DRMFRAME3 lpWorld_frame);
	bool SetRenderState();
	void CleanUp();
	void delData();
	bool HandleError(HRESULT rval);
	CScrollBar m_Grip;

// Attributes
protected:
	LPDIRECT3DRM3 lpD3DRM;          // Direct3DRM object 
	LPDIRECTDRAWCLIPPER lpDDClipper;// DirectDrawClipper object 

    LPDIRECT3DRMDEVICE3 dev;    // Direct3DRM device 
    LPDIRECT3DRMVIEWPORT2 view; // Direct3DRM viewport through which 
                                // the scene is viewed
    LPDIRECT3DRMFRAME3 scene;   // Master frame in which others are 
                                // placed 
    LPDIRECT3DRMFRAME3 camera;  // Frame describing the user's POV 

	LPDIRECT3DRMFRAME3 lpWorld_frame;			// The world frame, child of scene
	LPDIRECT3DRMMESHBUILDER3 lp_builder[MAXBL];		// 3 Track block polygons builder
	LPDIRECT3DRMMESHBUILDER3 lp_Objbuilder[MAXEBL];	// 12 Track block objects builder
	LPDIRECT3DRMFRAME3 lpStreetLightframe[MAXBL];	// 3 Track block Light sources

	long nData;					// Number of texture images
	LPDATADATA2 lpData;			// Texture image pointers
	TEXTUREBLOCK * ptexture;	// For NFS 3 Texture flags
	BOOL bHSMode;				// Is NFS HS mode?

    int  CurrDriver;            // Number of D3D driver currently
                                // being used 
	const char * wndClassName;	// Windows class name

	POINT mousePos;

// Operations
public:
    BOOL bInitialized;          // All D3DRM objects are initialized 

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrackView)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	void CreateEmptyScene(int which);
	virtual ~CTrackView();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTrackView)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRACKVIEW_H__74ABEDA4_4CC1_11D3_A355_00207811A639__INCLUDED_)


