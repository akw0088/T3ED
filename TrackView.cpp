// TrackView.cpp : implementation file
//
// Created by Hoo, 8/16/1999 (All new)
// Hoo's D3D View functions added by Nappe1, (2/11/2001)


#define INITGUID           // Must precede other defines and includes

#include "stdafx.h"
#include "T3ED.h"
#include "T3EDDoc.h"
#include "T3EDView.h"
#include "TrackView.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int BPP;                    // Bit depth of the current display mode
GUID DriverGUID[MAX_DRIVERS];     // GUIDs of available D3D drivers 
char DriverName[MAX_DRIVERS][50]; // Names of available D3D drivers 
int  NumDrivers;                  // Number of available D3D drivers 

DWORD BPPToDDBD(int bpp);
HRESULT WINAPI enumDeviceFunc(LPGUID lpGuid, LPSTR lpDeviceDescription, 
    LPSTR lpDeviceName, LPD3DDEVICEDESC lpHWDesc, 
    LPD3DDEVICEDESC lpHELDesc, LPVOID lpContext);

/////////////////////////////////////////////////////////////////////////////
// CTrackView

CTrackView::CTrackView()
{
    bInitialized = FALSE;
	lpData = NULL;
	// Get a window class name
	wndClassName = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_OWNDC);
}

CTrackView::~CTrackView()
{
	// Delete texture image pointers
	CleanUp();
	if (lpData != NULL)
	{
		delData();
	}
}

bool CTrackView::Init(CWnd* pWnd, CQFSView * qfs, TEXTUREBLOCK *texture, BOOL isHSMode)
{
	RECT rect;
	CRect grect; //Gripper
	CDC *dc;
	//DWORD dwStyle = WS_OVERLAPPED | WS_POPUP | WS_CAPTION | WS_THICKFRAME;
	//DWORD dwStyle = WS_OVERLAPPED | WS_POPUPWINDOW | WS_CAPTION | WS_THICKFRAME | WS_MAXIMIZEBOX;
	DWORD dwStyle = WS_OVERLAPPEDWINDOW | WS_POPUP | WS_CAPTION| WS_THICKFRAME;
	//DWORD dwStyle = WS_POPUP | WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME;
	// Create a window for track view
	DWORD dwExStyle = WS_EX_CLIENTEDGE | WS_EX_STATICEDGE | WS_EX_OVERLAPPEDWINDOW;

	rect.top = 190;
	rect.bottom = 640;
	rect.left = 190;
	rect.right = 640;
	if (!CreateEx( dwExStyle, wndClassName, "Track View", dwStyle, rect, pWnd, 0)) 
		return HandleError(TV_ER_WINDOW);
	
    GetClientRect(grect);
    grect.left = grect.right;
    grect.top  = grect.bottom;
    m_Grip.Create(WS_CHILD | SBS_SIZEBOX | SBS_SIZEBOXBOTTOMRIGHTALIGN |
        SBS_SIZEGRIP | WS_VISIBLE, grect, this, AFX_IDW_SIZE_BOX);

	dc = GetDC();
	BPP = dc->GetDeviceCaps(BITSPIXEL);
	ReleaseDC(dc);

	if (!EnumDrivers()) return false;
	if (!InitD3D()) return false;

	if (!MakeTexture(qfs)) return false;

	bHSMode = isHSMode;
	ptexture = texture;

	return true;
}

void CTrackView::ShowView(BOOL bShow)
{
	if (bShow) 
		ShowWindow(SW_SHOW);
	else
		ShowWindow(SW_HIDE);
}

bool CTrackView::EnumDrivers()
{
	LPDIRECTDRAW lpDD;
	LPDIRECT3D3 lpD3D;
	HRESULT rval;

	// Create a DirectDraw object and query for the Direct3D interface 
	// to use to enumerate the drivers.
     
	DirectDrawCreate(NULL, &lpDD, NULL);
	rval = lpDD->QueryInterface(IID_IDirect3D3, (void**) &lpD3D);
	if (rval != DD_OK)
	{
		lpDD->Release();
		return HandleError(TV_ER_INIT);
	}
    
	// Enumerate the drivers, setting CurrDriver to -1 to initialize the
	// driver selection code in enumDeviceFunc.
     
	CurrDriver = -1;
	NumDrivers = 0;
	lpD3D->EnumDevices(enumDeviceFunc, &CurrDriver);
    
	// Ensure at least one valid driver was found.
     
	if (NumDrivers == 0)
		return HandleError(TV_ER_ENUM);
	lpD3D->Release();
	lpDD->Release();
    
	return TRUE;
}

bool CTrackView::InitD3D()
{
	RECT rc;
	HRESULT rval;
	LPDIRECT3DRM lpD3DRMOLD;
	int i;
    // Create the D3DRM object and the D3DRM window object.
     
    lpD3DRMOLD = NULL;
    Direct3DRMCreate(&lpD3DRMOLD);
	rval = lpD3DRMOLD->QueryInterface(IID_IDirect3DRM3, (void**) &lpD3DRM);
	lpD3DRMOLD->Release();
	if (rval != DD_OK)
		return HandleError(TV_ER_INIT);
	
    // Create the master scene frame and camera frame.
     
    lpD3DRM->CreateFrame(NULL, &scene);
    lpD3DRM->CreateFrame(scene, &camera);
    camera->SetPosition( scene, D3DVAL(0.0), D3DVAL(0.0), D3DVAL(0.0));

    // Create a DirectDrawClipper object and associate the 
    // window with it.
     
    DirectDrawCreateClipper(0, &lpDDClipper, NULL);
    lpDDClipper->SetHWnd(0, m_hWnd);
    
    // Create the D3DRM device by using the selected D3D driver.
     
    GetClientRect(&rc);
    if (!CreateDevAndView(lpDDClipper, CurrDriver, rc.right, rc.bottom))
	{
	    lpD3DRM->Release();
		lpDDClipper->Release();
        return HandleError(TV_ER_INIT);
	}

	//Initialize Scene
	float light1[3] = {0.7f, 0.7f, 0.7f};
	float light2[3] = {0.3f, 0.4f, 0.4f}; 
    LPDIRECT3DRMLIGHT lpLight1 = NULL;
	LPDIRECT3DRMLIGHT lpLight2 = NULL;
    LPDIRECT3DRMFRAME3 lpLightframe = NULL;
	
	lpWorld_frame = NULL;
	for (i=0;i<MAXBL;i++) lp_builder[i] = NULL;
	/*lp_builder[0] = NULL;
	lp_builder[1] = NULL;
	lp_builder[2] = NULL;*/
	for (i=0;i<MAXEBL;i++) lp_Objbuilder[i] = NULL;
	for (i=0;i<MAXBL;i++) lpStreetLightframe[i] = NULL;
	/*lpStreetLightframe[0] = NULL;
	lpStreetLightframe[1] = NULL;
	lpStreetLightframe[2] = NULL;*/
    MakeFrames(scene, camera, &lpLightframe, &lpWorld_frame);
	MakeLights(scene, camera, lpLightframe, &lpLight1, light1, &lpLight2, light2);
    SetPositions(scene, camera, lpLightframe, lpWorld_frame);
    lpLight1->Release();
	lpLight2->Release();
	lpLightframe->Release();

	bInitialized = TRUE;  // Initialization completed

	return true;
}

bool CTrackView::CreateDevAndView(LPDIRECTDRAWCLIPPER lpDDClip, int driver, 
    int width, int height)
{
    HRESULT rval;

    // Create the D3DRM device from this window by using the specified 
    // D3D driver.
     
    lpD3DRM->CreateDeviceFromClipper(lpDDClip, &DriverGUID[driver], width, height, &dev);
    
    // Create the D3DRM viewport by using the camera frame. Set the 
    // background depth to a large number. The width and height
    // might have been slightly adjusted, so get them from the device.
     
    width = dev->GetWidth();
    height = dev->GetHeight();
    rval = lpD3DRM->CreateViewport(dev, camera, 0, 0, width, height, &view);
    if (rval != D3DRM_OK)
	{
        dev->Release();
        return false;
    }
    rval = view->SetBack(D3DVAL(10000.0));
    if (rval != D3DRM_OK)
	{
        dev->Release();
        view->Release();
        return false;
    }
    
    // Set the render quality, fill mode, lighting state,
    // and color shade info.
     
    if (!SetRenderState())
        return false;
    return true;
}

bool CTrackView::CreateSceneEx(XOBJBLOCK * x, CQFSView * qfs, int which)
{
	// Add the polygons of track block xobj - green polygons
	// x=pDoc->xobj[i];
	int i, j, k, m, n;
	DWORD * faces;
	D3DVECTOR * normals;
	D3DVECTOR norm1, norm2;
	LPDIRECT3DRMFACEARRAY * lpFaceArray;
	D3DVECTOR * v;
	LPPOLYGONDATA p;
	XOBJDATA * l;
	
	// Reset xobj polygons builder
	if (lp_Objbuilder[which] != NULL)
	{
		lpWorld_frame->DeleteVisual((LPDIRECT3DRMVISUAL) lp_Objbuilder[which]);
		lp_Objbuilder[which]->Release();
	}
	lpD3DRM->CreateMeshBuilder(&lp_Objbuilder[which]);

	lpFaceArray = new LPDIRECT3DRMFACEARRAY [x->nobj];
	if (!lpFaceArray) return HandleError(TV_ER_ALLOC);

	// Create xobj faces and add them to builder
	l=x->obj;
	for (j=0;j<x->nobj;j++,l++)
	{
		i = 0;
		m = 0;
		n = 0;
		faces = new DWORD [(l->nPolygons+1)*18];
		if (!faces) 
		{
			delete [] lpFaceArray;
			return HandleError(TV_ER_ALLOC);
		}
		normals = new D3DVECTOR [ l->nPolygons*2];
		if (!normals)
		{
			delete [] lpFaceArray;
			delete [] faces;
			return HandleError(TV_ER_ALLOC);
		}
		v = new D3DVECTOR [ l->nPolygons * 4 ];
		if (!v) 
		{
			delete [] lpFaceArray;
			delete [] faces;
			delete [] normals;
			return HandleError(TV_ER_ALLOC);
		}
		p = l->polyData;
		for (k=0;k<l->nPolygons;k++,p++)
		{
			faces[i] = 4;
			D3DRMVectorAdd(&(v[n]), (D3DVECTOR *) &(l->vert[p->vertex[0]]),
				(D3DVECTOR *) &(l->ptRef));
			faces[i+1] = n++;
			D3DRMVectorAdd(&(v[n]), (D3DVECTOR *) &(l->vert[p->vertex[1]]),
				(D3DVECTOR *) &(l->ptRef));
			faces[i+3] = n++;
			D3DRMVectorAdd(&(v[n]), (D3DVECTOR *) &(l->vert[p->vertex[2]]),
				(D3DVECTOR *) &(l->ptRef));
			faces[i+5] = n++;
			D3DRMVectorAdd(&(v[n]), (D3DVECTOR *) &(l->vert[p->vertex[3]]),
				(D3DVECTOR *) &(l->ptRef));
			faces[i+7] = n++;
			D3DRMVectorSubtract(&norm1, (D3DVECTOR *) &(v[faces[i+1]]),
				(D3DVECTOR *) &(v[faces[i+5]]));
			D3DRMVectorSubtract(&norm2, (D3DVECTOR *) &(v[faces[i+3]]),
				(D3DVECTOR *) &(v[faces[i+7]]));
			D3DRMVectorNormalize(D3DRMVectorCrossProduct(&(normals[m]), &norm1, &norm2));
			faces[i+2] = faces[i+4] = faces[i+6] = faces[i+8] = m;
			m++;
			i+=9;
			if (bHSMode && p->hs_textflags&0x8000)
			//if (bHSMode ? p->unknown1&0x8000
			//	: ptexture[p->texture].unknown2) // Does it has two faces?
			{
				faces[i] = 4;
				faces[i+1] = n-3; //p->vertex[1];
				faces[i+3] = n-4; //p->vertex[0];
				faces[i+5] = n-1; //p->vertex[3];
				faces[i+7] = n-2; //p->vertex[2];
				normals[m].x = -normals[m-1].x;
				normals[m].y = -normals[m-1].y;
				normals[m].z = -normals[m-1].z;
				faces[i+2] = faces[i+4] = faces[i+6] = faces[i+8] = m;
				m++;
				i+=9;
			}
		}
		faces[i] = 0;
		lp_Objbuilder[which]->AddFaces(n, v, m, normals, faces, &(lpFaceArray[j]));
		delete [] faces;
		delete [] normals;
		delete [] v;
	}
	MakeMesh(&lp_Objbuilder[which]);

	// Stick a texture for each faces
	int index;
	HRESULT rval;
	LPDIRECT3DRMTEXTURE3 lpTex = NULL;
	LPDIRECT3DRMTEXTURE lpTex1 = NULL;
	LPDIRECT3DRMWRAP lpWrap = NULL;
	LPDIRECT3DRMFACE lpFace = NULL;

	l=x->obj;
	for (j=0;j<x->nobj;j++,l++)
	{
		m=0;
		p = l->polyData;
		for (k=0;k<l->nPolygons;k++,p++)
		{
			rval = (lpFaceArray[j])->GetElement(m, &lpFace);
			if (rval != 0) AfxMessageBox("Oh Oh!");
			index = p->texture;
			if (qfs->col[index].islane) 
			{
				if (qfs->col[index].texture<14)
				{
					lpTex = GetStockTexture(qfs->col[index].texture);
					lpTex->QueryInterface(IID_IDirect3DRMTexture, (void**) &lpTex1);
					lpTex1->SetDecalTransparency(TRUE);
					rval= lpFace->SetTexture(lpTex1);
				}	
			}	
			else if ((qfs->pBitmaps!=NULL)&&(qfs->col[index].texture<qfs->nQfsEntries)&&
				(qfs->pBitmaps[qfs->col[index].texture]!=NULL))
			{
				lpTex = GetTexture(qfs->col[index].texture);
				lpTex->QueryInterface(IID_IDirect3DRMTexture, (void**) &lpTex1);
				lpTex1->SetDecalTransparency(TRUE);
				rval = lpFace->SetTexture(lpTex1);
			}
			MakeWrap(p->hs_textflags, lpFace, index, false);
			m++;
			lpFace->Release();
			if (bHSMode && p->hs_textflags&0x8000)
			//if (bHSMode ? p->unknown1&0x8000
			//	: ptexture[p->texture].unknown2) // Does it has two faces?
			{
				rval = (lpFaceArray[j])->GetElement(m, &lpFace);
				if (rval != 0) AfxMessageBox("Oh Oh!");
				rval = lpFace->SetTexture(lpTex1);
				MakeWrap(p->hs_textflags^0x10, lpFace, index, true);
				m++;
			}
			if (lpTex1 != NULL)
			{
				lpTex1->Release();
				lpTex1 = NULL;
			}
			lpFace->Release();
		}
	}

	// Now that the xobj has been created, add it to the world frame.
	lpWorld_frame->AddVisual((LPDIRECT3DRMVISUAL) lp_Objbuilder[which]);

	// Release the face array
	for (j=0;j<x->nobj;j++)
		lpFaceArray[j]->Release();
	delete [] lpFaceArray;
	return true;
}

bool CTrackView::CreateScene(TRKBLOCK * b, POLYGONBLOCK * p, CQFSView * qfs, int which, int nDetail)
{
	int i,j,k,l,m,n;
	LPDIRECT3DRMLIGHT lpLight = NULL;

	//Lights
	//Assume a lightsrc color
	float RGB[3] = {0.6f, 0.6f, 0.5f};

	if (lpStreetLightframe[which] != NULL)
	{
		scene->DeleteChild(lpStreetLightframe[which]);
		lpStreetLightframe[which]->Release();
	}
	lpD3DRM->CreateFrame(scene, &(lpStreetLightframe[which]));
	for (i=0; i<b->nLightsrc; i++)
	{
		//Assume all lightsrc type = D3DRMLIGHT_POINT
		lpD3DRM->CreateLightRGB(D3DRMLIGHT_POINT, 
        D3DVAL(RGB[0]), D3DVAL(RGB[1]), D3DVAL(RGB[2]), &lpLight);
	    lpStreetLightframe[which]->AddLight(lpLight);
		lpStreetLightframe[which]->SetPosition(scene, D3DVAL(b->lightsrc[i].refpoint.x),
			D3DVAL(b->lightsrc[i].refpoint.z), D3DVAL(b->lightsrc[i].refpoint.y));
		lpLight->Release();
	}

	//Polygons
	//Add one track block's polygons to its builder
	int nfaces = 0;
	DWORD * faces;
	D3DVECTOR * normals;
	D3DVECTOR norm1, norm2;
	D3DVECTOR * v;
	LPDIRECT3DRMFACEARRAY lpFaceArray;
	
	if (lp_builder[which] != NULL)
	{
		lpWorld_frame->DeleteVisual((LPDIRECT3DRMVISUAL) lp_builder[which]);
		lp_builder[which]->Release();
	}
	lpD3DRM->CreateMeshBuilder(&(lp_builder[which]));
	//Count the number of faces first
	for (j=0;j<7;j++)
	{
		if (j != 6 && j != nDetail && j != nDetail+1) continue;
		nfaces += p->sz[j];
	}
	for (j=0;j<4;j++)
	{
		for (k=0;k<p->obj[j].nobj;k++)
		{
			nfaces +=p->obj[j].numpoly[k];
		}
	}
	i=0;
	m=0;
	n=0;
	faces = new DWORD [(nfaces+1)*18];
	if (!faces) return HandleError(TV_ER_ALLOC);
	v = new D3DVECTOR [nfaces*4];
	if (!v) 
	{
		delete [] faces;
		return HandleError(TV_ER_ALLOC);
	}
	normals = new D3DVECTOR [ nfaces*2 ];
	if (!normals) 
	{
		delete [] faces;
		delete [] normals;
		return HandleError(TV_ER_ALLOC);
	}
	for (j=0;j<7;j++) for (k=0;k<p->sz[j];k++)
	{
		if (j != 6 && j != nDetail && j != nDetail+1) continue;
		faces[i] = 4;
		memcpy(&(v[n]), &(b->vert[p->poly[j][k].vertex[0]]), sizeof(D3DVECTOR));
		faces[i+1] = n++;
		memcpy(&(v[n]), &(b->vert[p->poly[j][k].vertex[1]]), sizeof(D3DVECTOR));
		faces[i+3] = n++;
		memcpy(&(v[n]), &(b->vert[p->poly[j][k].vertex[2]]), sizeof(D3DVECTOR));
		faces[i+5] = n++;
		memcpy(&(v[n]), &(b->vert[p->poly[j][k].vertex[3]]), sizeof(D3DVECTOR));
		faces[i+7] = n++;
		D3DRMVectorSubtract(&norm1, (D3DVECTOR *) &(b->vert[p->poly[j][k].vertex[0]]),
			(D3DVECTOR *) &(b->vert[p->poly[j][k].vertex[2]]));
		D3DRMVectorSubtract(&norm2, (D3DVECTOR *) &(b->vert[p->poly[j][k].vertex[1]]),
			(D3DVECTOR *) &(b->vert[p->poly[j][k].vertex[3]]));
		D3DRMVectorNormalize(D3DRMVectorCrossProduct(&(normals[m]), &norm1, &norm2));
		faces[i+2] = faces[i+4] = faces[i+6] = faces[i+8] = m;
		m++;
		i+=9;
		if (bHSMode && p->poly[j][k].hs_textflags&0x8000)
		//if (bHSMode ? p->poly[j][k].unknown1&0x8000
		//	: ptexture[p->poly[j][k].texture].unknown2) // Does it has two faces?
		{
			faces[i] = 4;
			faces[i+1] = n-3; //p->poly[j][k].vertex[1];
			faces[i+3] = n-4; //p->poly[j][k].vertex[0];
			faces[i+5] = n-1; //p->poly[j][k].vertex[3];
			faces[i+7] = n-2; //p->poly[j][k].vertex[2];
			normals[m].x = -normals[m-1].x;
			normals[m].y = -normals[m-1].y;
			normals[m].z = -normals[m-1].z;
			faces[i+2] = faces[i+4] = faces[i+6] = faces[i+8] = m;
			m++;
			i+=9;
		}
	}
	for (j=0;j<4;j++) for (k=0;k<p->obj[j].nobj;k++)
	{
		for (l=0;l<p->obj[j].numpoly[k];l++)
		{
			faces[i] = 4;
			memcpy(&(v[n]), &(b->vert[p->obj[j].poly[k][l].vertex[0]]), sizeof(D3DVECTOR));
			faces[i+1] = n++;
			memcpy(&(v[n]), &(b->vert[p->obj[j].poly[k][l].vertex[1]]), sizeof(D3DVECTOR));
			faces[i+3] = n++;
			memcpy(&(v[n]), &(b->vert[p->obj[j].poly[k][l].vertex[2]]), sizeof(D3DVECTOR));
			faces[i+5] = n++;
			memcpy(&(v[n]), &(b->vert[p->obj[j].poly[k][l].vertex[3]]), sizeof(D3DVECTOR));
			faces[i+7] = n++;
			D3DRMVectorSubtract(&norm1, (D3DVECTOR *) &(b->vert[p->obj[j].poly[k][l].vertex[0]]),
				(D3DVECTOR *) &(b->vert[p->obj[j].poly[k][l].vertex[2]]));
			D3DRMVectorSubtract(&norm2, (D3DVECTOR *) &(b->vert[p->obj[j].poly[k][l].vertex[1]]),
				(D3DVECTOR *) &(b->vert[p->obj[j].poly[k][l].vertex[3]]));
			D3DRMVectorNormalize(D3DRMVectorCrossProduct(&(normals[m]), &norm1, &norm2));
			faces[i+2] = faces[i+4] = faces[i+6] = faces[i+8] = m;
			m++;
			i+=9;
			if (bHSMode && p->obj[j].poly[k][l].hs_textflags&0x8000)
			//if (bHSMode ? p->obj[j].poly[k][l].unknown1&0x8000
			//	: ptexture[p->obj[j].poly[k][l].texture].unknown2) // Does it has two faces?
			{
				faces[i] = 4;
				faces[i+1] = n-3; //p->obj[j].poly[k][l].vertex[1];
				faces[i+3] = n-4; //p->obj[j].poly[k][l].vertex[0];
				faces[i+5] = n-1; //p->obj[j].poly[k][l].vertex[3];
				faces[i+7] = n-2; //p->obj[j].poly[k][l].vertex[2];
				normals[m].x = -normals[m-1].x;
				normals[m].y = -normals[m-1].y;
				normals[m].z = -normals[m-1].z;
				faces[i+2] = faces[i+4] = faces[i+6] = faces[i+8] = m;
				m++;
				i+=9;
			}
		}
	}
	faces[i] = 0;
	lp_builder[which]->AddFaces(n, v, m, normals, faces, &lpFaceArray);
	MakeMesh(&lp_builder[which]);

	//Add it to the world frame
    lpWorld_frame->AddVisual((LPDIRECT3DRMVISUAL) lp_builder[which]);

	delete [] faces;
	delete [] normals;
	delete [] v;

	//Texture
	//Stick a texture to each faces
	int index;
	HRESULT rval;
	LPDIRECT3DRMTEXTURE3 lpTex = NULL;
	LPDIRECT3DRMTEXTURE lpTex1 = NULL;
	LPDIRECT3DRMWRAP lpWrap = NULL;
	LPDIRECT3DRMFACE lpFace = NULL;

	m=0;
	for (j=0;j<7;j++) for (k=0;k<p->sz[j];k++)
	{
		if (j != 6 && j != nDetail && j != nDetail+1) continue;
		rval = lpFaceArray->GetElement(m, &lpFace);
		index = p->poly[j][k].texture;
		if (qfs->col[index].islane) 
		{
			if (qfs->col[index].texture<14)
			{
				lpTex = GetStockTexture(qfs->col[index].texture);
				lpTex->QueryInterface(IID_IDirect3DRMTexture, (void**) &lpTex1);
				lpTex1->SetDecalTransparency(TRUE);
				rval = lpFace->SetTexture(lpTex1);
			}
		}
		else if ((qfs->pBitmaps!=NULL)&&(qfs->col[index].texture<qfs->nQfsEntries)&&
			(qfs->pBitmaps[qfs->col[index].texture]!=NULL))
		{
			lpTex = GetTexture(qfs->col[index].texture);
			lpTex->QueryInterface(IID_IDirect3DRMTexture, (void**) &lpTex1);
			lpTex1->SetDecalTransparency(TRUE);
			rval = lpFace->SetTexture(lpTex1);
		}
		MakeWrap(p->poly[j][k].hs_textflags, lpFace, index, false);
		m++;
		lpFace->Release();
		if (bHSMode && p->poly[j][k].hs_textflags&0x8000)
		//if (bHSMode ? p->poly[j][k].unknown1&0x8000
		//	: ptexture[p->poly[j][k].texture].unknown2) // Does it has two faces?
		{
			rval = lpFaceArray->GetElement(m, &lpFace);
			rval = lpFace->SetTexture(lpTex1);
			MakeWrap(p->poly[j][k].hs_textflags^0x10, lpFace, index, true);
			m++;
		}
		if (lpTex1 != NULL)
		{
			lpTex1->Release();
			lpTex1 = NULL;
		}
		lpFace->Release();
	}
	for (j=0;j<4;j++) for (k=0;k<p->obj[j].nobj;k++)
	{
		for (l=0;l<p->obj[j].numpoly[k];l++)
		{
			rval = lpFaceArray->GetElement(m, &lpFace);
			index = p->obj[j].poly[k][l].texture;
			if (qfs->col[index].islane) 
			{
			if (qfs->col[index].texture<14)
			{
				lpTex = GetStockTexture(qfs->col[index].texture);
				lpTex->QueryInterface(IID_IDirect3DRMTexture, (void**) &lpTex1);
				lpTex1->SetDecalTransparency(TRUE);
				rval = lpFace->SetTexture(lpTex1);
			}
		}
		else if ((qfs->pBitmaps!=NULL)&&(qfs->col[index].texture<qfs->nQfsEntries)&&
			(qfs->pBitmaps[qfs->col[index].texture]!=NULL))
		{
			lpTex = GetTexture(qfs->col[index].texture);
			lpTex->QueryInterface(IID_IDirect3DRMTexture, (void**) &lpTex1);
			lpTex1->SetDecalTransparency(TRUE);
			rval = lpFace->SetTexture(lpTex1);
		}
		MakeWrap(p->obj[j].poly[k][l].hs_textflags, lpFace, index, false);
		m++;
		lpFace->Release();
		if (bHSMode && p->obj[j].poly[k][l].hs_textflags&0x8000)
		//if (bHSMode ? p->obj[j].poly[k][l].unknown1&0x8000
		//	: ptexture[p->obj[j].poly[k][l].texture].unknown2) // Does it has two faces?
		{
			rval = lpFaceArray->GetElement(m, &lpFace);
			rval = lpFace->SetTexture(lpTex1);
			MakeWrap(p->obj[j].poly[k][l].hs_textflags^0x10, lpFace, index, true);
			m++;
		}
		if (lpTex1 != NULL)
		{
			lpTex1->Release();
			lpTex1 = NULL;
		}
		lpFace->Release();
		}
	}
	// Release face array
	lpFaceArray->Release();
	return true;
}				

bool CTrackView::UpdateScene(int theta, int phi, int x, int z, int y, double scale)
{	
	// Set the camera orientation and view
    HRESULT rval;
	x *= 2;
	y *= 2;
	z *= 2;
	double dtheta = theta * 3.14159265359/180.0;
	double dphi = phi * 3.14159265359/180.0;
	double px = x + scale * sin(dtheta) * sin(dphi);
	double pz = z - scale * sin(dtheta) * cos(dphi);
	double py = y + scale * cos(dtheta);
	double vx = -sin(dtheta) * sin(dphi);
	double vz = sin(dtheta) * cos(dphi);
	double vy = -cos(dtheta);
	double ux = -cos(dtheta) * sin(dphi);
	double uz = cos(dtheta) * cos(dphi);
	double uy = sin(dtheta);

    camera->SetPosition(scene, D3DVAL(px), D3DVAL(py), D3DVAL(pz));
    camera->SetOrientation(scene, 
        D3DVAL(vx), D3DVAL(vy), D3DVAL(vz),
		D3DVAL(ux), D3DVAL(uy), D3DVAL(uz));

    // Clear the viewport.
    rval = view->Clear(D3DRMCLEAR_ALL);
    if (rval != D3DRM_OK)
        return false;
   
    // Render the scene to the viewport.
    rval = view->Render(scene);
    if (rval != D3DRM_OK)
        return false;
    
    // Update the window.
    rval = dev->Update();
    if (rval != D3DRM_OK)
        return false;

	InvalidateRect(NULL, false);
    return true;
}

void CTrackView::MakeFrames(LPDIRECT3DRMFRAME3 lpScene, LPDIRECT3DRMFRAME3 lpCamera,
    LPDIRECT3DRMFRAME3 * lplpLightFrame1,
    LPDIRECT3DRMFRAME3 * lplpWorld_frame)
{
    lpD3DRM->CreateFrame(lpScene, lplpLightFrame1);
    lpD3DRM->CreateFrame(lpScene, lplpWorld_frame);
}

void CTrackView::MakeLights(LPDIRECT3DRMFRAME3 lpScene, LPDIRECT3DRMFRAME3 lpCamera, 
    LPDIRECT3DRMFRAME3 lpLightFrame1, 
    LPDIRECT3DRMLIGHT * lplpLight1, float * light,
	LPDIRECT3DRMLIGHT * lplpLight2, float * light2)
{
	
    lpD3DRM->CreateLightRGB(D3DRMLIGHT_DIRECTIONAL, 
        D3DVAL(light2[0]), D3DVAL(light2[1]), D3DVAL(light2[2]), lplpLight2);
                          
    lpLightFrame1->AddLight(*lplpLight2);

    lpD3DRM->CreateLightRGB(D3DRMLIGHT_AMBIENT, 
        D3DVAL(light[0]), D3DVAL(light[1]), D3DVAL(light[2]), lplpLight1);
    
    lpScene->AddLight(*lplpLight1);
}

void CTrackView::MakeMesh(LPDIRECT3DRMMESHBUILDER3 * lplp_builder)
{
    (*lplp_builder)->Scale(D3DVAL(2), D3DVAL(2), D3DVAL(2));
      
    // Set to white to avoid unexpected texture-blending results.
    (*lplp_builder)->SetColorRGB(D3DVAL(1), D3DVAL(1), D3DVAL(1));
}

void CTrackView::MakeWrap(short flags, LPDIRECT3DRMFACE lpFace, long nt, bool isback)
{
	int map[4], hold;
	if (!bHSMode)
	{
		map[0] = 0;
		map[1] = 1;
		map[2] = 2;
		map[3] = 3;
		/*if (!isback)
		{
			hold = map[0];
			map[0] = map[1];
			map[1] = hold;
			hold = map[2];
			map[2] = map[3];
			map[3] = hold;
		}*/
		lpFace->SetTextureCoordinates(map[0], ptexture[nt].corners[0], ptexture[nt].corners[1]);
		lpFace->SetTextureCoordinates(map[1], ptexture[nt].corners[2], ptexture[nt].corners[3]);
		lpFace->SetTextureCoordinates(map[2], ptexture[nt].corners[4], ptexture[nt].corners[5]);
		lpFace->SetTextureCoordinates(map[3], ptexture[nt].corners[6], ptexture[nt].corners[7]);
		return;
	}
	//(flags>>2)&3 indicates the multiple of 90° by which the 
	//texture should be rotated (0 for no rotation, 1 for 90°,
	//2 for 180°, 3 for 270°) ; a non-zero value of flags&0x10
	//indicates that the texture is horizontally flipped ; a
	//non-zero value of flags&0x20 indicates that the texture 
	//is vertically flipped. The value of (flags>>6)&7 indicates
	//the scaling factor : 0 is no scaling ; 1 means that the 
	//texture is tiled twice horizontally ; 2 that the texture 
	//is tile twice vertically ; 3 indicates 4x horizontal
	//tiling, 4 indicates 4x vertical tiling. Finally, a non-zero
	//value of flags&0x8000 indicates that the polygon is one-sided. 
	//ox, oy, and oz :: Origin of the wrap. 
	//dx, dy, and dz :: The z-axis of the wrap. 
	//ux, uy, and uz ::	The y-axis of the wrap. 
	//ou and ov :: Origin in the texture. 
	//su and sv :: Scale factor in the texture
	D3DVALUE oi[2], oo[2], ii[2], io[2];
	switch((flags>>2)&3)
	{
	case 0:
		map[0] = 0;
		map[1] = 1;
		map[2] = 2;
		map[3] = 3;
		break;
	case 1:
		map[0] = 3;
		map[1] = 0;
		map[2] = 1;
		map[3] = 2;
		break;
	case 2:
		map[0] = 2;
		map[1] = 3;
		map[2] = 0;
		map[3] = 1;
		break;
	case 3:
		map[0] = 1;
		map[1] = 2;
		map[2] = 3;
		map[3] = 0;
		break;
	}
	switch ((flags>>4)&3)
	{
	case 1:
		hold = map[0];
		map[0] = map[1];
		map[1] = hold;
		hold = map[2];
		map[2] = map[3];
		map[3] = hold;
		break;
	case 2:
		hold = map[0];
		map[0] = map[3];
		map[3] = hold;
		hold = map[2];
		map[2] = map[1];
		map[1] = hold;
		break;
	case 3:
		hold = map[1];
		map[1] = map[3];
		map[3] = hold;
		hold = map[2];
		map[2] = map[0];
		map[0] = hold;		
		break;
	}
	switch ((flags>>6)&7)
	
	//Nappe1 2/15/2001: fixed Texture tiling bug in track view. 
	//(Hoo's original code below as remarked. After that same but with a fix.)
	/*{
	case 0:
		oi[1] = D3DVAL(1.0f);
		ii[0] = D3DVAL(1.0f);
		ii[1] = D3DVAL(1.0f);
		io[0] = D3DVAL(1.0f);
		break;
	case 1:
		oi[1] = D3DVAL(1.0f);
		ii[0] = D3DVAL(0.5f);
		ii[1] = D3DVAL(1.0f);
		io[0] = D3DVAL(0.5f);
		break;
	case 2:
		oi[1] = D3DVAL(0.5f);
		ii[0] = D3DVAL(1.0f);
		ii[1] = D3DVAL(0.5f);
		io[0] = D3DVAL(1.0f);
		break;
	case 3:
		oi[1] = D3DVAL(1.0f);
		ii[0] = D3DVAL(0.25f);
		ii[1] = D3DVAL(1.0f);
		io[0] = D3DVAL(0.25f);
		break;
	case 4:
		oi[1] = D3DVAL(0.25f);
		ii[0] = D3DVAL(1.0f);
		ii[1] = D3DVAL(0.25f);
		io[0] = D3DVAL(1.0f);
		break;
	}*/
	{
	case 0:
		oi[1] = D3DVAL(1.0f);
		ii[0] = D3DVAL(1.0f);
		ii[1] = D3DVAL(1.0f);
		io[0] = D3DVAL(1.0f);
		break;
	case 1:
		oi[1] = D3DVAL(1.0f);
		ii[0] = D3DVAL(2.0f);
		ii[1] = D3DVAL(1.0f);
		io[0] = D3DVAL(2.0f);
		break;
	case 2:
		oi[1] = D3DVAL(2.0f);
		ii[0] = D3DVAL(1.0f);
		ii[1] = D3DVAL(2.0f);
		io[0] = D3DVAL(1.0f);
		break;
	case 3:
		oi[1] = D3DVAL(1.0f);
		ii[0] = D3DVAL(4.0f);
		ii[1] = D3DVAL(1.0f);
		io[0] = D3DVAL(4.0f);
		break;
	case 4:
		oi[1] = D3DVAL(4.0f);
		ii[0] = D3DVAL(1.0f);
		ii[1] = D3DVAL(4.0f);
		io[0] = D3DVAL(1.0f);
		break;
	}
	oi[0] = D3DVAL(0.0f);
	io[1] = D3DVAL(0.0f);
	oo[0] = D3DVAL(0.0f);
	oo[1] = D3DVAL(0.0f);
	lpFace->SetTextureCoordinates(map[0], oi[0], oi[1]);
	lpFace->SetTextureCoordinates(map[1], ii[0], ii[1]);
	lpFace->SetTextureCoordinates(map[2], io[0], io[1]);
	lpFace->SetTextureCoordinates(map[3], oo[0], oo[1]);

}

LPDIRECT3DRMTEXTURE3 CTrackView::GetStockTexture(long index)
{
	//Map lane textures to lane polygons
	
	if (index > 9)
	{
		return lpData[nData-1].lpTex;
	}
	return lpData[index+nData-11].lpTex;
}

LPDIRECT3DRMTEXTURE3 CTrackView::GetTexture(long index)
{
	//Get textures of real polygon faces
	return lpData[index].lpTex;
}

bool CTrackView::MakeTexture(CQFSView * qfs)
{
	//Make textures from qfs images
	DIBSECTION bmp;
	D3DRMIMAGE * d3dinfo;
	LPDATADATA2 lpvBits = lpData;
	HBITMAP hbmp;
	CString keke;
	struct MYHEAD
	{
		long pal[1024];
	} bmi;
	nData = qfs->nQfsEntries+11;
	lpData = lpvBits = new DATADATA2 [qfs->nQfsEntries+11];
	if (!lpData) return HandleError(TV_ER_ALLOC);

	HDC hdc = ::CreateCompatibleDC(NULL);
	if (!hdc) return HandleError(TV_ER_HDC);

	for (long j=0;j<qfs->nQfsEntries+11;j++)
	{
		lpvBits[j].lpTex = NULL;
		lpvBits[j].bits = new LPVOID [1024+sizeof(D3DRMIMAGE)];
		if (!lpvBits[j].bits) return HandleError(TV_ER_ALLOC);
		
		ZeroMemory(lpvBits[j].bits, 1024+sizeof(D3DRMIMAGE));
		if (j >= qfs->nQfsEntries)
			hbmp = qfs->pStockTextures[j-qfs->nQfsEntries];
		else
			hbmp = qfs->pBitmaps[j];
		
		d3dinfo = (D3DRMIMAGE *) lpvBits[j].bits;
		::GetObject(hbmp, (sizeof DIBSECTION), &bmp);
 		
		//Nappe1: having a look what happens when res got bigger than 128x128
		//It Loads okay...
		//keke.Format("%d x %d BPL: %d",bmp.dsBm.bmWidth, bmp.dsBm.bmHeight, bmp.dsBm.bmWidthBytes);
		//if (bmp.dsBm.bmWidth>128)
		//	MessageBox(keke);

		
		d3dinfo->width = bmp.dsBm.bmWidth;
		d3dinfo->height = bmp.dsBm.bmHeight;
		d3dinfo->depth = bmp.dsBm.bmBitsPixel;
		d3dinfo->bytes_per_line = bmp.dsBm.bmWidthBytes;
		d3dinfo->aspectx = 1;
		d3dinfo->aspecty = 1;
		d3dinfo->red_mask = 255;
		d3dinfo->green_mask = 255;
		d3dinfo->blue_mask = 255;
		d3dinfo->alpha_mask = 0;
		if (bmp.dsBmih.biBitCount > 8)
		{
			d3dinfo->red_mask = bmp.dsBitfields[0];
			d3dinfo->green_mask = bmp.dsBitfields[1];
			d3dinfo->blue_mask = bmp.dsBitfields[2];
			d3dinfo->alpha_mask = 0;
		}
		if (bmp.dsBmih.biBitCount < 9)
		{
			d3dinfo->rgb = FALSE;
			d3dinfo->buffer1 = bmp.dsBm.bmBits;
			memset(&bmi, 255, sizeof(MYHEAD));
			::SelectObject(hdc, hbmp);
			::GetDIBColorTable( hdc, 0, 256, (RGBQUAD *) bmi.pal);
		
			d3dinfo->palette_size = 1;
			d3dinfo->palette = (D3DRMPALETTEENTRY *) ((LPSTR) lpvBits[j].bits + (WORD) sizeof(D3DRMIMAGE));
			for (long i=0; i<256;i++)
			{
				if(((RGBQUAD*)&bmi.pal[i])->rgbReserved == 0)
				{
					d3dinfo->palette[i].red = ((RGBQUAD*)&bmi.pal[i])->rgbRed;
					d3dinfo->palette[i].green = ((RGBQUAD*)&bmi.pal[i])->rgbGreen;
					d3dinfo->palette[i].blue = ((RGBQUAD*)&bmi.pal[i])->rgbBlue;
					d3dinfo->palette[i].flags = 1;
					d3dinfo->palette_size++;
				}
			}
		}
		else
		{
			d3dinfo->rgb = TRUE;
			if (bmp.dsBmih.biBitCount == 16)
			{
				//0000 0011 1110 0000
				d3dinfo->buffer1 = bmp.dsBm.bmBits;
				d3dinfo->red_mask = 31;
				d3dinfo->green_mask = 992;
				d3dinfo->blue_mask = 31744;
				d3dinfo->alpha_mask = 32768;

			}
			
			if (bmp.dsBmih.biBitCount == 32)
			{
				//11111111 00000000
				d3dinfo->buffer1 = bmp.dsBm.bmBits;
				d3dinfo->alpha_mask = 4278190080;
				d3dinfo->blue_mask = 255; //16711680;
				d3dinfo->green_mask = 65280;
				d3dinfo->red_mask = 16711680; //255;

			}

		}
		HRESULT rval = lpD3DRM->CreateTexture(d3dinfo, &(lpvBits[j].lpTex));
		if (rval != D3DRM_OK) return HandleError(rval);
	}
	::DeleteDC(hdc);
	return true;
}

void CTrackView::SetPositions(LPDIRECT3DRMFRAME3 lpScene, 
    LPDIRECT3DRMFRAME3 lpCamera, LPDIRECT3DRMFRAME3 lpLightFrame1, 
    LPDIRECT3DRMFRAME3 lpWorld_frame)
{
	lpLightFrame1->SetPosition(lpScene, D3DVAL(45), D3DVAL(45), D3DVAL(45));
	
    lpCamera->SetPosition(lpScene, D3DVAL(0.0), D3DVAL(0.0), D3DVAL(0.0));
    lpCamera->SetOrientation(lpScene, 
        D3DVAL(0.0), D3DVAL(0.0), D3DVAL(1), 
        D3DVAL(0.0), D3DVAL(1), D3DVAL(0.0));
                                        
    lpWorld_frame->SetPosition(lpScene, 
        D3DVAL(0.0), D3DVAL(0.0), D3DVAL(0.0));
    lpWorld_frame->SetOrientation(lpScene, 
        D3DVAL(0.0), D3DVAL(0.0), D3DVAL(1),
        D3DVAL(0.0), D3DVAL(1), D3DVAL(0.0));
                                
    lpWorld_frame->SetRotation(lpScene, 
        D3DVAL(0.0), D3DVAL(0.0), D3DVAL(0.0), D3DVAL(0.0));
}

bool CTrackView::SetRenderState()
{
    HRESULT rval;
    
    // Set the render quality (light toggle, fill mode, shade mode).
     
    rval = dev->SetQuality(D3DRMRENDER_FLAT);
    if (rval != D3DRM_OK) {
        return false;
    }
    
    // If you want to change the dithering mode, use SetDither here. 
	dev->SetTextureQuality(D3DRMTEXTURE_LINEAR);
	
	return true;
}

void CTrackView::CleanUp()
{
    if (bInitialized)
	{
		int i;
		bInitialized = FALSE;
		if (scene)	scene->Release();
		if (camera) camera->Release();
		if (view) view->Release();
		if (dev) dev->Release();
		if (lpD3DRM) lpD3DRM->Release();
		if (lpDDClipper) lpDDClipper->Release();
		if (lpWorld_frame) lpWorld_frame->Release();
		for (i=0;i<MAXBL;i++)
		{
			if (lp_builder[i]) lp_builder[i]->Release();
			if (lpStreetLightframe[i]) lpStreetLightframe[i]->Release();
		}
		/*if (lp_builder[0]) lp_builder[0]->Release();
		if (lp_builder[1]) lp_builder[1]->Release();
		if (lp_builder[2]) lp_builder[2]->Release();*/
		for (i=0;i<MAXEBL;i++)
		{
			if (lp_Objbuilder[i]) lp_Objbuilder[i]->Release();
		}
		/*
		if (lpStreetLightframe[0]) lpStreetLightframe[0]->Release();
		if (lpStreetLightframe[1]) lpStreetLightframe[1]->Release();
		if (lpStreetLightframe[2]) lpStreetLightframe[2]->Release();*/
		delData();
	}
}

void CTrackView::delData()
{
	//Delete the texture image pointers
	for (long j = 0; j<nData; j++)
	{
		if (lpData[j].bits != NULL) 
			delete [] lpData[j].bits;
		if (lpData[j].lpTex != NULL)
			lpData[j].lpTex->Release();
	}
	delete [] lpData;
	lpData = NULL;
}

bool CTrackView::HandleError(HRESULT rval)
{
	//handle failure error
	switch(rval)
	{
	case TV_ER_WINDOW:
		AfxMessageBox("Error: Unable to Create TrackView Window");
		break;
	case TV_ER_ENUM:
		AfxMessageBox("Error: Graphics Driver not found");
		break;
	case TV_ER_INIT:
		AfxMessageBox("Error: Direct3D Related Problem");
		break;
	case TV_ER_ALLOC:
		AfxMessageBox("Error: Insufficient Memory");
		break;
	case TV_ER_HDC:
		AfxMessageBox("Error: Unable to Create Compatible Device Context");
		break;
	case TV_ER_COLORTABLE:
		AfxMessageBox("Error: Unable to Obtain Texture Color Table");
		break;
	case D3DRMERR_BADOBJECT:
		AfxMessageBox("D3DRMERR_BADOBJECT");
		break;
	case DDERR_INVALIDOBJECT:
		AfxMessageBox("DDERR_INVALIDOBJECT");
		break;
	case DDERR_INVALIDPARAMS:
		AfxMessageBox("DDERR_INVALIDPARAMS");
		break;
	case D3DRMERR_BADVALUE:
		AfxMessageBox("D3DRMERR_BADVALUE");
		break;
	default:
		AfxMessageBox("Unknown Error");
		break;
	}
	CleanUp();
	if (lpData != NULL)
	{
		delData();
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////
// D3D Callback Functions

DWORD BPPToDDBD(int bpp)
{
    switch(bpp) {
        case 1:
            return DDBD_1;
        case 2:
            return DDBD_2;
        case 4:
            return DDBD_4;
        case 8:
            return DDBD_8;
        case 16:
            return DDBD_16;
        case 24:
            return DDBD_24;
        case 32:
            return DDBD_32;
        default:
            return 0;
    }
}

HRESULT
WINAPI enumDeviceFunc(LPGUID lpGuid, LPSTR lpDeviceDescription, 
    LPSTR lpDeviceName, LPD3DDEVICEDESC lpHWDesc, 
    LPD3DDEVICEDESC lpHELDesc, LPVOID lpContext)
{
    static BOOL hardware = FALSE; // Current start driver is hardware 
    static BOOL mono = FALSE;     // Current start driver is mono light 
    LPD3DDEVICEDESC lpDesc;
    int *lpStartDriver = (int *)lpContext;
   
    // Decide which device description should be consulted.
     
    lpDesc = lpHWDesc->dcmColorModel ? lpHWDesc : lpHELDesc;
    
    // If this driver cannot render in the current display bit-depth, 
    // skip it and continue with the enumeration.
     
    if (!(lpDesc->dwDeviceRenderBitDepth & BPPToDDBD(BPP)))
        return D3DENUMRET_OK;
    
    // Record this driver's name and GUID.
     
    memcpy(&DriverGUID[NumDrivers], lpGuid, sizeof(GUID));
    lstrcpy(&DriverName[NumDrivers][0], lpDeviceName);
   
    // Choose hardware over software, RGB lights over mono lights.
     
    if (*lpStartDriver == -1) {
        
        // This is the first valid driver.
         
        *lpStartDriver = NumDrivers;
        hardware = lpDesc == lpHWDesc ? TRUE : FALSE;
        mono = lpDesc->dcmColorModel & D3DCOLOR_MONO ? TRUE : FALSE;
    } else if (lpDesc == lpHWDesc && !hardware) {
        
        // This driver is hardware and the start driver is not.
         
        *lpStartDriver = NumDrivers;
        hardware = lpDesc == lpHWDesc ? TRUE : FALSE;
        mono = lpDesc->dcmColorModel & D3DCOLOR_MONO ? TRUE : FALSE;
    } else if ((lpDesc == lpHWDesc && hardware ) || 
               (lpDesc == lpHELDesc && !hardware)) {
        if (lpDesc->dcmColorModel == D3DCOLOR_MONO && !mono) {
            
            // This driver and the start driver are the same type, and 
            // this driver is mono whereas the start driver is not.
             
            *lpStartDriver = NumDrivers;
            hardware = lpDesc == lpHWDesc ? TRUE : FALSE;
            mono = lpDesc->dcmColorModel & D3DCOLOR_MONO ? TRUE : FALSE;
        }
    }
    NumDrivers++;
    if (NumDrivers == MAX_DRIVERS)
        return (D3DENUMRET_CANCEL);
    return (D3DENUMRET_OK);
}

BEGIN_MESSAGE_MAP(CTrackView, CWnd)
	//{{AFX_MSG_MAP(CTrackView)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTrackView message handlers

void CTrackView::OnPaint() 
{
	RECT r;
	LPDIRECT3DRMWINDEVICE lpD3DRMWinDev;
	PAINTSTRUCT ps;
	
	if (bInitialized && dev)
	{
		// Create a Windows-specific Direct3D Retained Mode window device to handle this
		// message.
         
		if (GetUpdateRect(&r, FALSE))
		{
			BeginPaint(&ps); // device context for painting
			dev->QueryInterface(IID_IDirect3DRMWinDevice, (void **) &lpD3DRMWinDev);
			lpD3DRMWinDev->HandlePaint(ps.hdc);
			lpD3DRMWinDev->Release();
			EndPaint(&ps);
		}
	}	
	// Do not call CWnd::OnPaint() for painting messages
}

BOOL CTrackView::PreCreateWindow(CREATESTRUCT& cs) 
{
	//cs.dwExStyle |= WS_EX_TOPMOST;

	return CWnd::PreCreateWindow(cs);
}

void CTrackView::OnSize(UINT nType, int cx, int cy) 
{
	CWnd *pGripper = GetDlgItem(AFX_IDW_SIZE_BOX);
	if (pGripper)
    {
        CRect rectGripper, rectClient;
        pGripper->GetWindowRect(&rectGripper);
        GetClientRect(rectClient);
        rectClient.left = rectClient.right - rectGripper.Width();
        rectClient.top  = rectClient.bottom - rectGripper.Height();
        pGripper->MoveWindow(&rectClient);
    }

	RECT rect;
	CWnd::OnSize(nType, cx, cy);


	if (bInitialized && dev && view) 
	{
		GetClientRect(&rect);
		if (view->Configure(rect.left, rect.top, cx, cy) == D3DRMERR_BADVALUE)
		{
			view->Release();
			dev->Release();
			lpD3DRM->CreateDeviceFromClipper(lpDDClipper, &DriverGUID[CurrDriver], cx, cy, &dev);
		    cx = dev->GetWidth();
			cy = dev->GetHeight();
			dev->SetTextureQuality(D3DRMTEXTURE_LINEAR); //Nappe1: added to fix Filtering bug when scaling window.
			lpD3DRM->CreateViewport(dev, camera, 0, 0, cx, cy, &view);
			view->SetBack(D3DVAL(1000.0));
		}
		// Clear the viewport.
		view->Clear(D3DRMCLEAR_ALL);
		// Render the scene to the viewport.
		view->Render(scene);
		// Update the window.
		dev->Update();
		//InvalidateRect(NULL, false); /* could be the source of crash... */
	}
}

void CTrackView::OnDestroy() 
{
	CWnd::OnDestroy();
	
	CleanUp();
	
}

void CTrackView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CFrameWnd * pFrame = GetParentFrame();
	CView * pViewBase = pFrame->GetActiveView();
	ASSERT (pViewBase->IsKindOf(RUNTIME_CLASS( CT3EDView )));
	CT3EDView * pView = (CT3EDView *) pViewBase;

	switch(nChar) {
	case VK_LEFT:
		pView->TrackKeyDown(2, TRUE);
		break;
	case VK_RIGHT:
		pView->TrackKeyDown(1, TRUE);
		break;
	case VK_UP:
		pView->TrackKeyDown(4, TRUE);
		break;
	case VK_DOWN:
		pView->TrackKeyDown(8, TRUE);
		break;
	case VK_ADD:
	case VK_NEXT:
		pView->TrackKeyDown(16, TRUE);
		break;
	case VK_SUBTRACT:
	case VK_PRIOR:
		pView->TrackKeyDown(32, TRUE);
		break;
	default:
		CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
		break;
	}
}

void CTrackView::OnMouseMove(UINT nFlags, CPoint point) 
{
	POINT newPos;
	CFrameWnd * pFrame = GetParentFrame();
	CView * pViewBase = pFrame->GetActiveView();
	ASSERT (pViewBase->IsKindOf(RUNTIME_CLASS( CT3EDView )));
	CT3EDView * pView = (CT3EDView *) pViewBase;
	UINT tFlags = 0;
	bool isMove = FALSE;

	if (nFlags & MK_LBUTTON)
	{
		GetCursorPos(&newPos);
		if (newPos.x - mousePos.x > 5)
		{
			//mouse right
			tFlags = 1;
			mousePos.x += 5;
			isMove = TRUE;
		}
		else if (newPos.x - mousePos.x < -5)
		{
			//mouse left
			tFlags = 2;
			mousePos.x -= 5;
			isMove = TRUE;
		}
		if (newPos.y - mousePos.y > 5)
		{
			//mouse down
			tFlags |= 8;
			mousePos.y += 5;
			isMove = TRUE;
		}
		else if (newPos.y - mousePos.y < -5)
		{
			//mouse up
			tFlags |= 4;
			mousePos.y -= 5;
			isMove = TRUE;
		}
		if (isMove) pView->TrackKeyDown(tFlags, TRUE);
	}
	else if (nFlags & MK_RBUTTON)
	{
		GetCursorPos(&newPos);
		if (newPos.x - mousePos.x > 20)
		{
			//mouse right
			tFlags = 1;
			mousePos.x += 20;
			pView->TrackKeyDown(tFlags, FALSE);
		}
		else if (newPos.x - mousePos.x < -20)
		{
			//mouse left
			tFlags = 2;
			mousePos.x -= 20;
			pView->TrackKeyDown(tFlags, FALSE);
		}
	}
	CWnd::OnMouseMove(nFlags, point);
}

BOOL CTrackView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (nHitTest == HTCLIENT && message == WM_LBUTTONDOWN)
	{
		SetCursor(hcursorRotate);
		GetCursorPos(&mousePos);
	}
	else if (nHitTest == HTCLIENT && message == WM_RBUTTONDOWN)
	{
		SetCursor(hcursorMove);
		GetCursorPos(&mousePos);
	}
	else if (message == WM_LBUTTONUP || message == WM_RBUTTONUP)
	{
		GetCursorPos(&mousePos);
		return CWnd::OnSetCursor(pWnd, nHitTest, message);
	}
	return 1;
}

BOOL CTrackView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	return TRUE; //CWnd::OnEraseBkgnd(pDC);
}

void CTrackView::CreateEmptyScene(int which)
{
	int i;

	i=0;
	if (which<MAXBL)
	{
		if (lpStreetLightframe[which] != NULL)
		{
			scene->DeleteChild(lpStreetLightframe[which]);
			lpStreetLightframe[which]->Release();
		}
		lpD3DRM->CreateFrame(scene, &(lpStreetLightframe[which]));

		if (lp_builder[which] != NULL)
		{
			lpWorld_frame->DeleteVisual((LPDIRECT3DRMVISUAL) lp_builder[which]);
			lp_builder[which]->Release();
		}
		lpD3DRM->CreateMeshBuilder(&(lp_builder[which]));
	}
	
	for (i=0;i<4;i++)
		if ((which*4 + i)<MAXEBL)
		{
			if (lp_Objbuilder[which*4 + i] != NULL) 	// Reset xobj polygons builder
			{
				lpWorld_frame->DeleteVisual((LPDIRECT3DRMVISUAL) lp_Objbuilder[which*4 + i]);
				lp_Objbuilder[which*4 + i ]->Release();
			}
			lpD3DRM->CreateMeshBuilder(&lp_Objbuilder[which*4 + i]);
		}

}
