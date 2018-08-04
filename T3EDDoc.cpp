// T3EDDoc.cpp : implementation of the CT3EDDoc class
//
// Modified by Hoo, 8/16/1999 (Total: 7 items)
// Hoo's D3D View functions added by Nappe1, (2/11/2001)

#include "stdafx.h"
#include "T3ED.h"
#include <math.h>
#include "jgtTracer.h"
#include "Vectors.h"
#include <assert.h>
//#include <afx.h>

//#include "Gart/Gart.h"
//#include "PolyTracer/PolyTracer.h"

#include "T3EDDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CT3EDDoc

IMPLEMENT_DYNCREATE(CT3EDDoc, CDocument)

BEGIN_MESSAGE_MAP(CT3EDDoc, CDocument)
	//{{AFX_MSG_MAP(CT3EDDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CT3EDDoc construction/destruction

CT3EDDoc::CT3EDDoc()
{
	ASSERT(sizeof(struct FLOATPT)==12);
	ASSERT(sizeof(struct INTPT)==12);
	ASSERT(sizeof(struct NEIGHBORDATA)==4);
	ASSERT(sizeof(struct POSITIONDATA)==8);
//	ASSERT(sizeof(struct POLYVROADDATA)==8); no longer true !
	ASSERT(sizeof(struct VROADDATA)==12);
	ASSERT(sizeof(struct REFXOBJ)==20);
//	ASSERT(sizeof(struct REFPOLYOBJ)==20);
	ASSERT(sizeof(struct SOUNDSRC)==16);
	ASSERT(sizeof(struct LIGHTSRC)==16);
	ASSERT(sizeof(struct POLYGONDATA)==14);
	ASSERT(sizeof(struct ANIMDATA)==20);
	ASSERT(sizeof(struct XBHEAD)==8);
	ASSERT(sizeof(struct COLTEXTUREINFO)==8);
	ASSERT(sizeof(struct COLVERTEX)==16);
	ASSERT(sizeof(struct COLPOLYGON)==6);
	ASSERT(sizeof(struct COLVECTOR)==4);
	ASSERT(sizeof(struct COLVROAD)==36);
	bEmpty=TRUE;
	HeightsLoaded=FALSE;
	spdFALoaded=FALSE;
	spdRALoaded=FALSE;
	undoLevel=0;
	
	//HOO: Initialize (1)
	bUpdatedScene=FALSE;
	bCreateScene=FALSE;
	TrackView = new CTrackView;
	//HOO: (1)
}

CT3EDDoc::~CT3EDDoc()
{
	// clean the embedded QFS stock bitmaps
	for (int i=0;i<14;i++)
		if (qfsView.pStockBitmaps[i]!=NULL)
			::DeleteObject(qfsView.pStockBitmaps[i]);

	//HOO: Clean up (2)
	TrackView->DestroyWindow();
	delete TrackView;
	//HOO: (2)
}

BOOL CT3EDDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	bEmpty=TRUE;
	undoLevel=0;
	SendInitialUpdate(); // reset the view
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CT3EDDoc serialization

BOOL CT3EDDoc::LoadHSFRD(CArchive& ar,long nPos)
{
	int i,j,k,l,m,remn;
	struct TRKBLOCK *b;
	struct POLYGONBLOCK *p;
	struct XOBJDATA *x;
	struct OBJPOLYBLOCK *o;
	struct COLVROAD *vr;
	struct HS_VROADBLOCK vroadblk;
	struct VROADDATA *v;
	LPPOLYGONDATA pp;
	short *vno;
	unsigned char ptrspace[44]; // some useless data from HS FRDs
	struct FLOATPT v1,v2,norm;
	unsigned char *belong;
	float len;
	struct POLYGONDATA tmppoly;

	// emulate the COL file
	memcpy(col.collID,"COLL",4);
	col.version=11;
	col.fileLength=36*nPos+48;
	col.nBlocks=2;
	col.xbTable[0]=8; col.xbTable[1]=24;
	// fake a texture table with only one entry to please NFS3/T3ED
	col.textureHead.size=16;
	col.textureHead.xbid=XBID_TEXTUREINFO;
	col.textureHead.nrec=1;
	col.texture=(struct COLTEXTUREINFO *) malloc(sizeof(struct COLTEXTUREINFO));
	if (col.texture==NULL) return FALSE;
	memset(col.texture,0,8);
	// vroad XB
	col.vroadHead.size=8+36*nPos;
	col.vroadHead.xbid=XBID_VROAD;
	col.vroadHead.nrec=(short)nPos;
	col.vroad=(struct COLVROAD *)malloc(nBlocks*8*sizeof(struct COLVROAD));
	if (col.vroad==NULL) return FALSE;
	col.hs_extra=(long *)malloc(7*nPos*sizeof(long));
	if (col.hs_extra==NULL) return FALSE;
	for (i=0,vr=col.vroad;i<nPos;i++,vr++) {
		if (ar.Read(&vroadblk,84)!=84) return FALSE;
		vr->refPt.x=(long)(vroadblk.refPt.x*65536);
		vr->refPt.z=(long)(vroadblk.refPt.z*65536);
		vr->refPt.y=(long)(vroadblk.refPt.y*65536);
		// a wild guess
		vr->unknown=(long)((vroadblk.unknown2[3]&0xFFFF)+       // unknownLanes[2]
			               ((vroadblk.unknown2[4]&0xF)<<16)+    // wallKinds[0]
						   ((vroadblk.unknown2[4]&0xF00)<<20)); // wallKinds[1]
		if (vroadblk.normal.x>=1.0) vr->normal.x=127;
		else vr->normal.x=(signed char)(vroadblk.normal.x*128);
		if (vroadblk.normal.z>=1.0) vr->normal.z=127;
		else vr->normal.z=(signed char)(vroadblk.normal.z*128);
		if (vroadblk.normal.y>=1.0) vr->normal.y=127;
		else vr->normal.y=(signed char)(vroadblk.normal.y*128);
		if (vroadblk.forward.x>=1.0) vr->forward.x=127;
		else vr->forward.x=(signed char)(vroadblk.forward.x*128);
		if (vroadblk.forward.z>=1.0) vr->forward.z=127;
		else vr->forward.z=(signed char)(vroadblk.forward.z*128);
		if (vroadblk.forward.y>=1.0) vr->forward.y=127;
		else vr->forward.y=(signed char)(vroadblk.forward.y*128);
		if (vroadblk.right.x>=1.0) vr->right.x=127;
		else vr->right.x=(signed char)(vroadblk.right.x*128);
		if (vroadblk.right.z>=1.0) vr->right.z=127;
		else vr->right.z=(signed char)(vroadblk.right.z*128);
		if (vroadblk.right.y>=1.0) vr->right.y=127;
		else vr->right.y=(signed char)(vroadblk.right.y*128);
		vr->leftWall=(long)(vroadblk.leftWall*65536);
		vr->rightWall=(long)(vroadblk.rightWall*65536);
		memcpy(col.hs_extra+7*i,&(vroadblk.unknown1[0]),28);
	}

	// TRKBLOCKs
	for (i=0;i<nBlocks;i++) {
		b=&(trk[i]);
		p=&(poly[i]);
		// 7 track polygon numbers
		if (ar.Read(p->sz,28)!=28) return FALSE;
		memcpy(p->szdup,p->sz,28);
		// 4 object polygon numbers
		for (j=0;j<4;j++) if (ar.Read(&(p->obj[j].n1),4)!=4) return FALSE;
		// pointer space
		if (ar.Read(ptrspace,44)!=44) return FALSE;
		// 6 nVertices
		if (ar.Read(&(b->nVertices),24)!=24) return FALSE;
		if ((b->nVertices<0)/*||(b->nVertices>1000)*/) return FALSE;
		// pointer space
		if (ar.Read(ptrspace,8)!=8) return FALSE;
		// ptCentre, ptBounding == 60 bytes
		if (ar.Read(b,60)!=60) return FALSE;
		// nbdData
		if (ar.Read(b->nbdData,4*0x12C)!=4*0x12C) return FALSE;
		// xobj numbers
		for (j=4*i;j<4*i+4;j++) {
			if (ar.Read(&(xobj[j].nobj),4)!=4) return FALSE;
			if (ar.Read(ptrspace,4)!=4) return FALSE;
		}
		// nVRoad is not the same as in NFS3, will change later
		if (ar.Read(&(b->nVRoad),4)!=4) return FALSE;
		// 2 unknown specific FLOATPTs
		if (ar.Read(&(b->hs_ptMin),24)!=24) return FALSE;
		if (ar.Read(ptrspace,4)!=4) return FALSE;
		// nPositions
		if (ar.Read(&(b->nPositions),4)!=4) return FALSE;
		if (i==0) b->nStartPos=0;
		else b->nStartPos=trk[i-1].nStartPos+trk[i-1].nPositions;
		b->nPolygons=p->sz[4];
		// nXobj etc...
		if (ar.Read(&(b->nXobj),4)!=4) return FALSE;
		if (ar.Read(ptrspace,4)!=4) return FALSE;
		if (ar.Read(&(b->nPolyobj),4)!=4) return FALSE;
		if (ar.Read(ptrspace,4)!=4) return FALSE;
		if (ar.Read(&(b->nSoundsrc),4)!=4) return FALSE;
		if (ar.Read(ptrspace,4)!=4) return FALSE;
		if (ar.Read(&(b->nLightsrc),4)!=4) return FALSE;
		if (ar.Read(ptrspace,4)!=4) return FALSE;
		// neighbor data
		if (ar.Read(b->hs_neighbors,32)!=32) return FALSE;
	}
	
	// TRKBLOCKDATA
	for (i=0;i<nBlocks;i++) {
		b=&(trk[i]);
		p=&(poly[i]);
		// vertices
		b->vert=(struct FLOATPT *)malloc(b->nVertices*sizeof(struct FLOATPT));
		if (b->vert==NULL) return FALSE;
		if ((long)ar.Read(b->vert,12*b->nVertices)!=12*b->nVertices) return FALSE;
		b->unknVertices=(unsigned long *)malloc(b->nVertices*sizeof(unsigned long));
		if (b->unknVertices==NULL) return FALSE;
		if ((long)ar.Read(b->unknVertices,4*b->nVertices)!=4*b->nVertices) return FALSE;
		// polyData is a bit tricky
		b->polyData=(struct POLYVROADDATA *)malloc(b->nPolygons*sizeof(struct POLYVROADDATA));
		if (b->polyData==NULL) return FALSE;
		memset(b->polyData,0,b->nPolygons*sizeof(struct POLYVROADDATA));
		b->vroadData=(struct VROADDATA *)malloc(b->nPolygons*sizeof(struct VROADDATA));
		if (b->vroadData==NULL) return FALSE;
		for (j=0;j<b->nPolygons;j++) {
			b->polyData[j].vroadEntry=j;
			b->polyData[j].flags=0xE; // not passable
		}
		for (j=0;j<b->nVRoad;j++) {
			if (ar.Read(ptrspace,10)!=10) return FALSE;
			k=0;
			if (ar.Read(&k,2)!=2) return FALSE;
			memcpy(b->polyData[k].hs_minmax,ptrspace,8);
			b->polyData[k].flags=ptrspace[8];
			b->polyData[k].hs_unknown=ptrspace[9];
			if ((ptrspace[8]&15)==14) return FALSE;
			if (ar.Read(b->vroadData+k,12)!=12) return FALSE;
		}
		b->nVRoad=b->nPolygons;

		// the 4 misc. tables
		if (b->nXobj>0) {
			b->xobj=(struct REFXOBJ *)malloc(b->nXobj*sizeof(struct REFXOBJ));
			if (b->xobj==NULL) return FALSE;
			if ((long)ar.Read(b->xobj,20*b->nXobj)!=20*b->nXobj) return FALSE;
			// crossindex is f***ed up, but we don't really care
		}
		if (b->nPolyobj>0) {
			char *buffer=(char *)malloc(b->nPolyobj*20);
			if ((long)ar.Read(buffer,20*b->nPolyobj)!=20*b->nPolyobj) return FALSE;
			free(buffer);
		}
		b->nPolyobj=0;
		if (b->nSoundsrc>0) {
			b->soundsrc=(struct SOUNDSRC *)malloc(b->nSoundsrc*sizeof(struct SOUNDSRC));
			if (b->soundsrc==NULL) return FALSE;
			if ((long)ar.Read(b->soundsrc,16*b->nSoundsrc)!=16*b->nSoundsrc) return FALSE;
		}
		if (b->nLightsrc>0) {
			b->lightsrc=(struct LIGHTSRC *)malloc(b->nLightsrc*sizeof(struct LIGHTSRC));
			if (b->lightsrc==NULL) return FALSE;
			if ((long)ar.Read(b->lightsrc,16*b->nLightsrc)!=16*b->nLightsrc) return FALSE;
		}

		// track polygons
		for (j=0;j<7;j++) if (p->sz[j]!=0) {
			p->poly[j]=(LPPOLYGONDATA)malloc(p->sz[j]*sizeof(struct POLYGONDATA));
			if (p->poly[j]==NULL) return FALSE;
			for (k=0;k<p->sz[j];k++) {
				if (ar.Read(&tmppoly,13)!=13) return FALSE;
				for (m=0;m<4;m++) p->poly[j][k].vertex[m]=tmppoly.vertex[m^1];
				memcpy(&(p->poly[j][k].texture),&(tmppoly.texture),5);
				p->poly[j][k].unknown2=0xF9; //Nappe1: fixed for correct animation reading... (originally 0xF9)
			}
		}

		// make up some fake posData
		b->posData=(struct POSITIONDATA *)malloc(b->nPositions*sizeof(struct POSITIONDATA));
		if (b->posData==NULL) return FALSE;
		k=0; pp=p->poly[4];
		for (j=0;j<b->nPositions;j++) {
			b->posData[j].polygon=k;
			b->posData[j].unknown=0;
			b->posData[j].extraNeighbor1=-1;
			b->posData[j].extraNeighbor2=-1;
			do {
				l=0;
				do { if ((b->polyData[k].flags&0x0f)%14) l++; k++; pp++; }
				while ((k<b->nPolygons)&&(pp->vertex[0]==(pp-1)->vertex[1])&&(pp->vertex[3]==(pp-1)->vertex[2]));
				if (((j==b->nPositions-1)&&(k<b->nPolygons))||(k>b->nPolygons)) {
//					MessageBeep(MB_ICONEXCLAMATION);
//					afxDump << "failed " << i << " : " << k << " instead of " << b->nPolygons << "\n";
//					for (l=0;l<b->nPositions;l++) afxDump << b->posData[l].polygon << " ";
//					afxDump << "\n";
					k=b->nPolygons;
				}
			} while ((l==0)&&(k<b->nPolygons));
			b->posData[j].nPolygons=k-b->posData[j].polygon;
		}
		
		// still vroadData is missing for unpassable polygons
		for (j=0;j<b->nPolygons;j++) 
			if (b->polyData[j].flags==0xE) {
				v=b->vroadData+j;
				vno=p->poly[4][j].vertex;
				v1.x=b->vert[vno[1]].x-b->vert[vno[3]].x;
				v1.z=b->vert[vno[1]].z-b->vert[vno[3]].z;
				v1.y=b->vert[vno[1]].y-b->vert[vno[3]].y;
				v2.x=b->vert[vno[2]].x-b->vert[vno[0]].x;
				v2.z=b->vert[vno[2]].z-b->vert[vno[0]].z;
				v2.y=b->vert[vno[2]].y-b->vert[vno[0]].y;
				norm.x=-v1.y*v2.z+v1.z*v2.y;
				norm.y=-v1.z*v2.x+v1.x*v2.z;
				norm.z=-v1.x*v2.y+v1.y*v2.x;
				len=(float)sqrt(norm.x*norm.x+norm.y*norm.y+norm.z*norm.z);
				v->xNorm=(short)(norm.x*32767/len);
				v->zNorm=(short)(norm.z*32767/len);
				v->yNorm=(short)(norm.y*32767/len);
				v1.x=(float)col.vroad[b->nStartPos].forward.x; 
				v1.z=(float)col.vroad[b->nStartPos].forward.z; 
				v1.y=(float)col.vroad[b->nStartPos].forward.y; 
				len=(float)sqrt(v1.x*v1.x+v1.y*v1.y+v1.z*v1.z);
				v->xForw=(short)(v1.x*32767/len);
				v->zForw=(short)(v1.z*32767/len);
				v->yForw=(short)(v1.y*32767/len);
			}

		// POLYGONBLOCK OBJECTS
		o=p->obj;
		belong=(unsigned char*)malloc(b->nObjectVert);
		if (belong==NULL) return FALSE;
		for (j=0;j<4;j++,o++) if (o->n1>0) {
			memset(belong,0xFF,b->nObjectVert);
			pp=(LPPOLYGONDATA)malloc(14*o->n1);
			if (pp==NULL) return FALSE;
			for (k=0;k<o->n1;k++) {
				if (ar.Read(&tmppoly,13)!=13) return FALSE;
				for (m=0;m<4;m++) pp[k].vertex[m]=tmppoly.vertex[m^1];
				memcpy(&(pp[k].texture),&(tmppoly.texture),5);
				pp[k].unknown2=0xFF; // will temporarily store object's #
				//Nappe1: Destroys AnimData??! ah... it sets it to 0xF9 later... fixing There...

			}
			remn=o->n1; o->nobj=0;
			while (remn>0) {  // there are still polygons to be connected
				k=0; while (pp[k].unknown2!=0xFF) k++;
				pp[k].unknown2=(unsigned char)o->nobj;
				remn--;
				for (l=0;l<4;l++) belong[pp[k].vertex[l]]=(unsigned char)o->nobj;
				do {
					m=0;
					for (k=0;k<o->n1;k++) if (pp[k].unknown2==0xFF) {
						for (l=0;l<4;l++)
							if (belong[pp[k].vertex[l]]==(unsigned char)o->nobj) break;
						if (l<4) {
							remn--; m++;
							pp[k].unknown2=(unsigned char)o->nobj;
							for (l=0;l<4;l++)
								belong[pp[k].vertex[l]]=(unsigned char)o->nobj;
						}
					}
				} while (m>0); // we've been adding more polygons
				o->nobj++;
			}
			o->n2=o->nobj+xobj[4*i+j].nobj;
			o->types=(long *)malloc(o->n2*sizeof(long));
			if (o->types==NULL) return FALSE;
			o->numpoly=(long *)malloc(o->nobj*sizeof(long));
			if (o->numpoly==NULL) return FALSE;
			o->poly=(LPPOLYGONDATA *)malloc(4*o->nobj);
			if (o->poly==NULL) return FALSE;
			memset(o->poly,0,4*o->nobj);
			for (l=0;l<o->nobj;l++) {
				remn=0; 
				for (k=0;k<o->n1;k++) if (pp[k].unknown2==l) remn++;
				o->numpoly[l]=remn;
				o->poly[l]=(LPPOLYGONDATA)malloc(remn*sizeof(struct POLYGONDATA));
				if (o->poly[l]==NULL) return FALSE;
				remn=0;
				for (k=0;k<o->n1;k++) if (pp[k].unknown2==l) {
					memcpy(o->poly[l]+remn,pp+k,sizeof(struct POLYGONDATA));
					o->poly[l][remn].unknown2=0xF9; //Nappe1: fixed: Loads AnimData right. Didn't work??!
					remn++;
				}
			}
			free(pp);
			// there used to be something with REFPOLYOBJs if chunk 0
			for (k=0;k<o->nobj;k++)
				o->types[k]=1;
			for (k=o->nobj;k<o->n2;k++)
				o->types[k]=4; // to be replaced by 3/... later
		}
		free(belong);

	// XOBJs
		for (j=4*i;j<4*i+4;j++) if (xobj[j].nobj>0) {
			xobj[j].obj=(struct XOBJDATA *)malloc(xobj[j].nobj*sizeof(struct XOBJDATA));
			if (xobj[j].obj==NULL) return FALSE;
			memset(xobj[j].obj,0,xobj[j].nobj*sizeof(struct XOBJDATA));
			for (k=0;k<xobj[j].nobj;k++) {
				x=&(xobj[j].obj[k]);
				// 3 headers == 12 bytes 
				if (ar.Read(x,12)!=12) return FALSE;
				if ((x->crosstype==4)||(x->crosstype==2)) // basic objects
					{ if (ar.Read(&(x->ptRef),12)!=12) return FALSE; }
				else if (x->crosstype==3) { // animated objects
					// unkn3 instead of ptRef
					if (ar.Read(x->unknown3,12)!=12) return FALSE;
				}
				else return FALSE; // unknown object type
				if (p->obj[j&3].nobj!=0)
					p->obj[j&3].types[p->obj[j&3].nobj+k]=x->crosstype;
				// common part : vertices & polygons
				if (ar.Read(&(x->AnimMemory),4)!=4) return FALSE;
				if (ar.Read(ptrspace,4)!=4) return FALSE;
				if (ar.Read(&(x->nVertices),4)!=4) return FALSE;
				if (ar.Read(ptrspace,8)!=8) return FALSE;
				x->vert=(struct FLOATPT *)malloc(12*x->nVertices);
				if (x->vert==NULL) return FALSE;
				x->unknVertices=(long *)malloc(4*x->nVertices);
				if (x->unknVertices==NULL) return FALSE;
				if (ar.Read(&(x->nPolygons),4)!=4) return FALSE;
				if (ar.Read(ptrspace,4)!=4) return FALSE;
				x->polyData=(struct POLYGONDATA *)malloc(x->nPolygons*14);
				if (x->polyData==NULL) return FALSE;
			}
			// now the xobjdata
			for (k=0;k<xobj[j].nobj;k++) {
				x=&(xobj[j].obj[k]);
				if (x->crosstype==3) { // animated-specific header
					if (ar.Read(x->unknown3+6,2)!=2) return FALSE; 
					// if (x->unknown3[6]!=4) return FALSE;  // fails
					// type3, objno, animLength, unknown4
					if (ar.Read(&(x->type3),6)!=6) return FALSE;
					if (x->type3!=3) return FALSE;
					x->animData=(struct ANIMDATA *)malloc(20*x->nAnimLength);
					if (x->animData==NULL) return FALSE;
					if ((long)ar.Read(x->animData,20*x->nAnimLength)!=20*x->nAnimLength) return FALSE;
					// make a ref point from first anim position
					x->ptRef.x=(float)(x->animData->pt.x/65536.0);
					x->ptRef.z=(float)(x->animData->pt.z/65536.0);
					x->ptRef.y=(float)(x->animData->pt.y/65536.0);
				}
// appears in REFPOLYOBJ & REFXOBJ but not in XOBJs !
/*				if (x->crosstype==6) { // object with byte data
					x->hs_type6=(char *)malloc(x->unknown2);
					if ((long)ar.Read(x->hs_type6,x->unknown2)!=x->unknown2) return FALSE;
				}
*/				if ((long)ar.Read(x->vert,12*x->nVertices)!=12*x->nVertices) return FALSE;
				if ((long)ar.Read(x->unknVertices,4*x->nVertices)!=4*x->nVertices) return FALSE;
				for (l=0;l<x->nPolygons;l++) {
					if (ar.Read(&tmppoly,13)!=13) return FALSE;
					for (m=0;m<4;m++) x->polyData[l].vertex[m]=tmppoly.vertex[m^1];
					memcpy(&(x->polyData[l].texture),&(tmppoly.texture),5);
					x->polyData[l].unknown2=0xF9; //Nappe1: Fixed AnimData load. Didn't work?? 
					// what on earth for these Unknown2 definitions are used for internal checkings?
					//Now it doesn't crash, but doesn't Imply Necromancers definitions... wierd...
				}
			}
		}
	}

	j=4*nBlocks; //Global Objects
	if (ar.Read(&xobj[j],4)!=4) return FALSE;
	if (xobj[j].nobj>0) {
			xobj[j].obj=(struct XOBJDATA *)malloc(xobj[j].nobj*sizeof(struct XOBJDATA));
			if (xobj[j].obj==NULL) return FALSE;
			memset(xobj[j].obj,0,xobj[j].nobj*sizeof(struct XOBJDATA));
			for (k=0;k<xobj[j].nobj;k++) {
				x=&(xobj[j].obj[k]);
				// 3 headers == 12 bytes 
				if (ar.Read(x,12)!=12) return FALSE;
				if ((x->crosstype==4)||(x->crosstype==2)||(x->crosstype==1)) // basic objects + crosstype 1 
					{ if (ar.Read(&(x->ptRef),12)!=12) return FALSE; }
				else if (x->crosstype==3) { // animated objects
					// unkn3 instead of ptRef
					if (ar.Read(x->unknown3,12)!=12) return FALSE;
				}
				else return FALSE; // unknown object type
				/*if (p->obj[j&3].nobj!=0)
					p->obj[j&3].types[p->obj[j&3].nobj+k]=x->crosstype;*/
				// common part : vertices & polygons
				if (ar.Read(&(x->AnimMemory),4)!=4) return FALSE;
				if (ar.Read(ptrspace,4)!=4) return FALSE;
				if (ar.Read(&(x->nVertices),4)!=4) return FALSE;
				if (ar.Read(ptrspace,8)!=8) return FALSE;
				x->vert=(struct FLOATPT *)malloc(12*x->nVertices);
				if (x->vert==NULL) return FALSE;
				x->unknVertices=(long *)malloc(4*x->nVertices);
				if (x->unknVertices==NULL) return FALSE;
				if (ar.Read(&(x->nPolygons),4)!=4) return FALSE;
				if (ar.Read(ptrspace,4)!=4) return FALSE;
				x->polyData=(struct POLYGONDATA *)malloc(x->nPolygons*14);
				if (x->polyData==NULL) return FALSE;
			}
			// now the xobjdata
			for (k=0;k<xobj[j].nobj;k++) {
				x=&(xobj[j].obj[k]);
				if (x->crosstype==3) { // animated-specific header
					if (ar.Read(x->unknown3+6,2)!=2) return FALSE; 
					// if (x->unknown3[6]!=4) return FALSE;  // fails
					// type3, objno, animLength, unknown4
					if (ar.Read(&(x->type3),6)!=6) return FALSE;
					if (x->type3!=3) return FALSE;
					x->animData=(struct ANIMDATA *)malloc(20*x->nAnimLength);
					if (x->animData==NULL) return FALSE;
					if ((long)ar.Read(x->animData,20*x->nAnimLength)!=20*x->nAnimLength) return FALSE;
					// make a ref point from first anim position
					x->ptRef.x=(float)(x->animData->pt.x/65536.0);
					x->ptRef.z=(float)(x->animData->pt.z/65536.0);
					x->ptRef.y=(float)(x->animData->pt.y/65536.0);
				}
// appears in REFPOLYOBJ & REFXOBJ but not in XOBJs !
/*				if (x->crosstype==6) { // object with byte data
					x->hs_type6=(char *)malloc(x->unknown2);
					if ((long)ar.Read(x->hs_type6,x->unknown2)!=x->unknown2) return FALSE;
				}
*/				if ((long)ar.Read(x->vert,12*x->nVertices)!=12*x->nVertices) return FALSE;
				if ((long)ar.Read(x->unknVertices,4*x->nVertices)!=4*x->nVertices) return FALSE;
				for (l=0;l<x->nPolygons;l++) {
					if (ar.Read(&tmppoly,13)!=13) return FALSE;
					for (m=0;m<4;m++) x->polyData[l].vertex[m]=tmppoly.vertex[m^1];
					memcpy(&(x->polyData[l].texture),&(tmppoly.texture),5);
					x->polyData[l].unknown2=0xF9; //Nappe1: Fixed AnimData load. Didn't work?? 
					// what on earth for these Unknown2 definitions are used for internal checkings?
					//Now it doesn't crash, but doesn't Imply Necromancers definitions... wierd...
				}
			}
		}


	// remainder of the FRD file
	hs_morexobj=(char *)malloc(100000); // no more than 100K, I hope !
	hs_morexobjlen=ar.Read(hs_morexobj,100000);
	if (hs_morexobjlen==100000) return FALSE;
	hs_morexobj=(char *)realloc(hs_morexobj,hs_morexobjlen);

	// TEXTUREBLOCKs
	m=0;
	for (i=0;i<nBlocks;i++) {
		for (j=0;j<7;j++) 
			for (k=0,pp=poly[i].poly[j];k<poly[i].sz[j];k++,pp++)
				if (pp->texture>m) m=pp->texture;
		for (j=0;j<4;j++)
			for (k=0;k<poly[i].obj[j].nobj;k++)
				for (l=0,pp=poly[i].obj[j].poly[k];l<poly[i].obj[j].numpoly[k];l++,pp++)
					if (pp->texture>m) m=pp->texture;
	}
	for (i=0;i<4*nBlocks;i++)
		for (j=0;j<xobj[i].nobj;j++)
			for (k=0,pp=xobj[i].obj[j].polyData;k<xobj[i].obj[j].nPolygons;k++,pp++)
				if (pp->texture>m) m=pp->texture;
	//HOO: This is changed because some pStockBitmap can not be seen (3)
	nTextures=m += 15;
	//HOO: (3)
	texture=(struct TEXTUREBLOCK*)malloc(m*sizeof(struct TEXTUREBLOCK));
	if (texture==NULL) return FALSE;
	memset(texture,0,m*sizeof(struct TEXTUREBLOCK));
	for (i=0;i<m;i++) {
		texture[i].width=16; texture[i].height=16; // WHY ?????
		texture[i].corners[2]=1.0; // (1,0)
		texture[i].corners[4]=1.0; // (1,1)
		texture[i].corners[5]=1.0;
		texture[i].corners[7]=1.0; // (0,1)
		texture[i].texture=i&0x7FF;  // ANYWAY WE CAN'T FIND IT !
		texture[i].islane=i>>11;
	}

	CorrectVirtualRoad();
	return TRUE;
}

BOOL CT3EDDoc::LoadFRD(CArchive& ar)
{
	int i,j,k,l;
	struct TRKBLOCK *b;
	struct POLYGONBLOCK *p;
	struct XOBJDATA *x;
	struct OBJPOLYBLOCK *o;

	if (ar.Read(header,28)!=28) return FALSE; // header & numblocks
	if (ar.Read(&nBlocks,4)<4) return FALSE;
	nBlocks++;
	if ((nBlocks<1)||(nBlocks>500)) return FALSE; // 1st sanity check

	trk=(struct TRKBLOCK *)malloc((1+nBlocks)*sizeof(struct TRKBLOCK)); //+1 = Dummy trackblock for global objects
	if (trk==NULL) return FALSE;
	memset(trk,0,nBlocks*sizeof(struct TRKBLOCK));
	poly=(struct POLYGONBLOCK *)malloc(nBlocks*sizeof(struct POLYGONBLOCK));
	if (poly==NULL) return FALSE;
	memset(poly,0,nBlocks*sizeof(struct POLYGONBLOCK));
	xobj=(struct XOBJBLOCK *)malloc((4*nBlocks+4)*sizeof(struct XOBJBLOCK)); //+4 because global objects are added at the end
	if (xobj==NULL) return FALSE;
	memset(xobj,0,(4*nBlocks+4)*sizeof(struct XOBJBLOCK));

	if (ar.Read(&l,4)<4) return FALSE; // choose between NFS3 & NFSHS
	if ((l<0)||(l>5000)) bHSMode=FALSE;
	else if (((l+7)/8)==nBlocks) bHSMode=TRUE;
	//if (nBlocks==nBlocks) bHSMode=TRUE;
	else return FALSE; // unknown file type

	LoadSettings(); //Default settings for Dialogs.

	if (bHSMode) return LoadHSFRD(ar,l);
    
	memcpy(trk,&l,4);
    if (ar.Read(((char *)trk)+4,80)!=80) return FALSE;
	// TRKBLOCKs
	for (i=0;i<nBlocks;i++) {
		b=&(trk[i]);
		// ptCentre, ptBounding, 6 nVertices == 84 bytes
		if (i!=0) { if (ar.Read(b,84)!=84) return FALSE; }
		if ((b->nVertices<0)/*||(b->nVertices>1000)*/) return FALSE;
		b->vert=(struct FLOATPT *)malloc(b->nVertices*sizeof(struct FLOATPT));
		if (b->vert==NULL) return FALSE;
		if ((long)ar.Read(b->vert,12*b->nVertices)!=12*b->nVertices) return FALSE;
		b->unknVertices=(unsigned long *)malloc(b->nVertices*sizeof(unsigned long));
		if (b->unknVertices==NULL) return FALSE;
		if ((long)ar.Read(b->unknVertices,4*b->nVertices)!=4*b->nVertices) return FALSE;
		if (ar.Read(b->nbdData,4*0x12C)!=4*0x12C) return FALSE;

		// nStartPos & various blk sizes == 32 bytes
		if (ar.Read(&(b->nStartPos),32)!=32) return FALSE;
		if (i>0) if (b->nStartPos!=trk[i-1].nStartPos+trk[i-1].nPositions) return FALSE;
		b->posData=(struct POSITIONDATA *)malloc(b->nPositions*sizeof(struct POSITIONDATA));
		if (b->posData==NULL) return FALSE;
		if ((long)ar.Read(b->posData,8*b->nPositions)!=8*b->nPositions) return FALSE;
		b->polyData=(struct POLYVROADDATA *)malloc(b->nPolygons*sizeof(struct POLYVROADDATA));
		if (b->polyData==NULL) return FALSE;
		memset(b->polyData,0,b->nPolygons*sizeof(struct POLYVROADDATA));
		for (j=0;j<b->nPolygons;j++)
			if (ar.Read(b->polyData+j,8)!=8) return FALSE;
		b->vroadData=(struct VROADDATA *)malloc(b->nVRoad*sizeof(struct VROADDATA));
		if (b->vroadData==NULL) return FALSE;
		if ((long)ar.Read(b->vroadData,12*b->nVRoad)!=12*b->nVRoad) return FALSE;
		if (b->nXobj>0) {
			b->xobj=(struct REFXOBJ *)malloc(b->nXobj*sizeof(struct REFXOBJ));
			if (b->xobj==NULL) return FALSE;
			if ((long)ar.Read(b->xobj,20*b->nXobj)!=20*b->nXobj) return FALSE;
		}
		if (b->nPolyobj>0) {
			char *buffer=(char *)malloc(b->nPolyobj*20);
			if ((long)ar.Read(buffer,20*b->nPolyobj)!=20*b->nPolyobj) return FALSE;
			free(buffer);
		}
		b->nPolyobj=0;
		if (b->nSoundsrc>0) {
			b->soundsrc=(struct SOUNDSRC *)malloc(b->nSoundsrc*sizeof(struct SOUNDSRC));
			if (b->soundsrc==NULL) return FALSE;
			if ((long)ar.Read(b->soundsrc,16*b->nSoundsrc)!=16*b->nSoundsrc) return FALSE;
		}
		if (b->nLightsrc>0) {
			b->lightsrc=(struct LIGHTSRC *)malloc(b->nLightsrc*sizeof(struct LIGHTSRC));
			if (b->lightsrc==NULL) return FALSE;
			if ((long)ar.Read(b->lightsrc,16*b->nLightsrc)!=16*b->nLightsrc) return FALSE;
		}
	}

	// POLYGONBLOCKs
	for (i=0;i<nBlocks;i++) {
		p=&(poly[i]);
		for (j=0;j<7;j++) {
			if (ar.Read(&(p->sz[j]),4)!=4) return FALSE;
			if (p->sz[j]!=0) {
				if (ar.Read(&(p->szdup[j]),4)!=4) return FALSE;
				if (p->szdup[j]!=p->sz[j]) return FALSE;
				p->poly[j]=(LPPOLYGONDATA)malloc(p->sz[j]*sizeof(struct POLYGONDATA));
				if (p->poly[j]==NULL) return FALSE;
				if ((long)ar.Read(p->poly[j],14*p->sz[j])!=14*p->sz[j]) return FALSE;
			}
		}
		if (p->sz[4]!=trk[i].nPolygons) return FALSE; // sanity check
		for (j=0;j<4;j++) {
			o=&(p->obj[j]);
			if (ar.Read(&(o->n1),4)!=4) return FALSE;
			if (o->n1>0) {
				if (ar.Read(&(o->n2),4)!=4) return FALSE;
				o->types=(long *)malloc(o->n2*sizeof(long));
				if (o->types==NULL) return FALSE;
				o->numpoly=(long *)malloc(o->n2*sizeof(long));
				if (o->numpoly==NULL) return FALSE;
				o->poly=(LPPOLYGONDATA *)malloc(o->n2*sizeof(LPPOLYGONDATA));
				if (o->poly==NULL) return FALSE;
				o->nobj=0; l=0;
/*				if (j==0) // sanity check with TRKBLOCK
					if (trk[i].nPolyobj!=o->n2) return FALSE; */
				for(k=0;k<o->n2;k++) {
					if (ar.Read(o->types+k,4)!=4) return FALSE;
/*					if (j==0) // sanity check with TRKBLOCK
						if (o->types[k]!=trk[i].polyobj[k]->type) return FALSE; */
					if (o->types[k]==1) {
						if (ar.Read(o->numpoly+o->nobj,4)!=4) return FALSE;
						o->poly[o->nobj]=(LPPOLYGONDATA)malloc(o->numpoly[o->nobj]*sizeof(struct POLYGONDATA));
						if (o->poly[o->nobj]==NULL) return FALSE;
						if ((long)ar.Read(o->poly[o->nobj],14*o->numpoly[o->nobj])!=14*o->numpoly[o->nobj]) return FALSE;
						l+=o->numpoly[o->nobj];
						o->nobj++;
					}
				}
				if (l!=o->n1) return FALSE; // n1 == total nb polygons
			}
		}
	}

	// XOBJBLOCKs
	for (i=0;i<=4*nBlocks;i++) {
		if (ar.Read(&(xobj[i].nobj),4)!=4) return FALSE;
		if (xobj[i].nobj>0) {
			xobj[i].obj=(struct XOBJDATA *)malloc(xobj[i].nobj*sizeof(struct XOBJDATA));
			if (xobj[i].obj==NULL) return FALSE;
			memset(xobj[i].obj,0,xobj[i].nobj*sizeof(struct XOBJDATA));
		}
		for (j=0;j<xobj[i].nobj;j++) {
			x=&(xobj[i].obj[j]);
			// 3 headers == 12 bytes 
			if (ar.Read(x,12)!=12) return FALSE;
			if (x->crosstype==4) { // basic objects
				if (ar.Read(&(x->ptRef),12)!=12) return FALSE;
				if (ar.Read(&(x->AnimMemory),4)!=4) return FALSE;
			}
			else if (x->crosstype==3) { // animated objects
				// unkn3, type3, objno, nAnimLength, unkn4 == 24 bytes
				if (ar.Read(x->unknown3,24)!=24) return FALSE;
				if (x->type3!=3) return FALSE;
				x->animData=(struct ANIMDATA *)malloc(20*x->nAnimLength);
				if (x->animData==NULL) return FALSE;
				if ((long)ar.Read(x->animData,20*x->nAnimLength)!=20*x->nAnimLength) return FALSE;
				// make a ref point from first anim position
				x->ptRef.x=(float)(x->animData->pt.x/65536.0);
				x->ptRef.z=(float)(x->animData->pt.z/65536.0);
				x->ptRef.y=(float)(x->animData->pt.y/65536.0);
			}
			else return FALSE; // unknown object type
			// common part : vertices & polygons
			if (ar.Read(&(x->nVertices),4)!=4) return FALSE;
			x->vert=(struct FLOATPT *)malloc(12*x->nVertices);
			if (x->vert==NULL) return FALSE;
			if ((long)ar.Read(x->vert,12*x->nVertices)!=12*x->nVertices) return FALSE;
			x->unknVertices=(long *)malloc(4*x->nVertices);
			if (x->unknVertices==NULL) return FALSE;
			if ((long)ar.Read(x->unknVertices,4*x->nVertices)!=4*x->nVertices) return FALSE;
			if (ar.Read(&(x->nPolygons),4)!=4) return FALSE;
			x->polyData=(struct POLYGONDATA *)malloc(x->nPolygons*14);
			if (x->polyData==NULL) return FALSE;
			if ((long)ar.Read(x->polyData,14*x->nPolygons)!=14*x->nPolygons) return FALSE;
		}
	}
// sanity check with TRKBLOCK fails for TR00/TR04 with animated objects
/*	for (i=0;i<nBlocks;i++)
		if ((xobj[4*i].nobj+xobj[4*i+1].nobj+xobj[4*i+2].nobj+
				xobj[4*i+3].nobj)!=trk[i].nXobj) return FALSE;  */

	// TEXTUREBLOCKs
	if (ar.Read(&nTextures,4)!=4) return FALSE;
	texture=(struct TEXTUREBLOCK *)malloc(nTextures*sizeof(struct TEXTUREBLOCK));
	for (i=0;i<nTextures;i++)
		if (ar.Read(&(texture[i]),47)!=47) return FALSE;

	if (ar.Read(&i,4)!=0) return FALSE; // we ought to be at EOF now

	if (bHSMode==TRUE) 
		CorrectVirtualRoad();

	return TRUE;
}

void CT3EDDoc::LoadSettings()
{
	ZCopyMode=1; //Nappe1: setting up default CopyMode...
	bAutoObjMem=TRUE; //By default change object membership automatic

	//Default settings after loading
	settings.m_add_replace_block=0; 
	settings.m_bool_add_replace_block_paste_block=TRUE;
	settings.m_bool_add_replace_block_move=TRUE;
	settings.m_bool_add_replace_block_move_z=TRUE;
	settings.m_bool_add_replace_block_rotate=TRUE;
	settings.m_bool_add_replace_block_expand_lenght=TRUE;
	settings.m_bool_add_replace_block_expand_width=FALSE;
	settings.m_bool_add_replace_block_paste_heights=TRUE;
	settings.m_bool_add_replace_block_paste_spdfa=TRUE;
	settings.m_bool_add_replace_block_paste_spdra=TRUE;
	settings.m_bool_add_replace_block_paste_vroad=TRUE;
	settings.expand_m_floatExpMulti_x=1;
	settings.expand_m_floatExpMulti_y=1;
	settings.expand_m_floatExpMulti_z=1;
	settings.expand_m_chk_connected=TRUE;
	settings.expand_m_radio_alignment=2;
	settings.expand_bl_m_edit_height=1;
	settings.expand_bl_m_edit_lenght=1;
	settings.expand_bl_m_edit_width=1;
	settings.rotate_m_edit_angle=0;
	settings.ray_m_theta=0;
	settings.ray_m_rho=(float)0.85;
	settings.ray_m_iEndBlock=nBlocks;
	settings.ray_m_iStartBlock=0;
	settings.ray_m_bCheckTrack=TRUE;
	settings.ray_m_bCheckObjects=TRUE;
	settings.MoveTo_dest_x=0;
	settings.MoveTo_dest_y=0;
	settings.MoveTo_dest_z=0;
	settings.MoveTo_no_smooth=FALSE;
	settings.ConnectBlocks=FALSE;
	settings.ConnectBlocks_dist=(float)0.2;
	settings.AdjustRoad_AdjustType=0;
	settings.AdjustRoad_StartBlock=0;
	settings.AdjustRoad_EndBlock=nBlocks-1;
	settings.VRoadHeightsSpd_Chk_Heights=TRUE;
	settings.VRoadHeightsSpd_Chk_SpdFa=TRUE;
	settings.VRoadHeightsSpd_Chk_SpdRa=TRUE;
	settings.VRoadHeightsSpd_Chk_VRoad=TRUE;
	settings.FindPoly_block_choice=0;
	settings.FindPoly_trackpolys=TRUE;
	settings.FindPoly_startbl=0;
	settings.FindPoly_endbl=nBlocks-1;
	settings.FindPoly_extraobj=TRUE;;
	settings.FindPoly_polygonobj=TRUE;;
	settings.FindPoly_texture=0;
	settings.FindPoly_chk_texture=TRUE;
	settings.FindPoly_m_flags=0;
	settings.FindPoly_chk_m_flags=FALSE;
	settings.FindPoly_texanimdata=0;
	settings.FindPoly_chk_texanimdata=FALSE;
	settings.FindPoly_pvrflags=0;
	settings.FindPoly_chk_pvrflags=FALSE;
	settings.setvisi_int_num_blocks=15;
	settings.setvisi_int_tr_bl_fromto=0;
	settings.setvisi_int_end=nBlocks-1;;
	settings.setvisi_edit_start=0;
}

BOOL CT3EDDoc::LoadSpeedFile(CFile &spdfileh, struct SPDFILE *spdpoint)
{
	int i;
	unsigned char Byte[1];
	float value;
	
	for (i=0;i<col.vroadHead.nrec;i++)
	{
		if (spdfileh.Read(Byte,1)!=1) return FALSE;
		spdpoint[i].Speedvalue=Byte[0];
		i++;
		spdpoint[i].Speedvalue=Byte[0]; //store it a second time....
	}
	//i=spdpoint[col.vroadHead.nrec].Speedvalue;// Last value Ok, Del when working !

	for (i=0;i<col.vroadHead.nrec;i++)
	{
		if (spdfileh.Read(Byte,1)!=1) return FALSE;
		spdpoint[i].Bestprejection=Byte[0];
	}
	//i=spdpoint[col.vroadHead.nrec-1].Bestprejection;// Last value Ok, Del when working !

	for (i=0;i<col.vroadHead.nrec;i++)
	{
		if (spdfileh.Read(&value,4)!=4) return FALSE;
		spdpoint[i].turn=value;
	}
	//value=spdpoint[col.vroadHead.nrec].turn;// Last value Ok, Del when working !
	return TRUE;
}

void CT3EDDoc::SaveSpeedFile(CFile &spdfileh, SPDFILE *spdpoint)
{
	int i;
	
	for (i=0;i<col.vroadHead.nrec;i++)
	{
		spdfileh.Write(&spdpoint[i].Speedvalue,1); //write entry;
		i++; //skip the double entry
	}

	for (i=0;i<col.vroadHead.nrec;i++)
	{
		spdfileh.Write(&spdpoint[i].Bestprejection,1); //write entry;
	}

	for (i=0;i<col.vroadHead.nrec;i++)
	{
		spdfileh.Write(&spdpoint[i].turn,4); //write entry;
	}

	spdfileh.Close();
}

BOOL CT3EDDoc::LoadHeightssim(CFile &heightsfile)
{
	unsigned int size;

	size=col.vroadHead.nrec*sizeof(struct HEIGHTSSIM);
	//hightssim
	hightssim=(struct HEIGHTSSIM *)malloc(size);
	memset(hightssim,0,size);
	if (heightsfile.Read(hightssim,size)!=size) return FALSE;

	return TRUE;
}

void CT3EDDoc::SaveHeightssim(CFile &heightsfile)
{
	unsigned int size;

	size=col.vroadHead.nrec*sizeof(struct HEIGHTSSIM);
	heightsfile.Write(hightssim,size);

	heightsfile.Close();
}

BOOL CT3EDDoc::LoadCOL(CFile& coll)
{
	struct COLSTRUCT3D *s;
	struct COLOBJECT *o;
	int i,delta,dummy;

	col.hs_extra=NULL;
	if (coll.Read(&col,16)!=16) return FALSE;
	if ((col.collID[0]!='C')||(col.collID[1]!='O')||
		(col.collID[2]!='L')||(col.collID[3]!='L')) return FALSE;
	if (col.version!=11) return FALSE;
	if ((col.nBlocks!=2)&&(col.nBlocks!=4)&&(col.nBlocks!=5)) return FALSE;
	if ((long)coll.Read(col.xbTable,4*col.nBlocks)!=4*col.nBlocks) return FALSE;

	// texture XB
	if (coll.Read(&col.textureHead,8)!=8) return FALSE;
	if (col.textureHead.xbid!=XBID_TEXTUREINFO) return FALSE;
	if (col.textureHead.size!=8+8*col.textureHead.nrec) return FALSE;
	col.texture=(struct COLTEXTUREINFO *)
			malloc(col.textureHead.nrec*sizeof(struct COLTEXTUREINFO));
	if (col.texture==NULL) return FALSE;
	if ((long)coll.Read(col.texture,8*col.textureHead.nrec)!=8*col.textureHead.nrec) return FALSE;

	// struct3D XB
	if (col.nBlocks>=4) {
		if (coll.Read(&col.struct3DHead,8)!=8) return FALSE;
		if (col.struct3DHead.xbid!=XBID_STRUCT3D) return FALSE;
		s=col.struct3D=(struct COLSTRUCT3D *)
				malloc(col.struct3DHead.nrec*sizeof(struct COLSTRUCT3D));
		if (s==NULL) return FALSE;
		memset(s,0,col.struct3DHead.nrec*sizeof(struct COLSTRUCT3D));
		for (i=0;i<col.struct3DHead.nrec;i++,s++) {
			if (coll.Read(s,8)!=8) return FALSE;
			delta=(8+16*s->nVert+6*s->nPoly)%4;
			delta=(4-delta)%4;
			if (s->size!=8+16*s->nVert+6*s->nPoly+delta) return FALSE;
			s->vertex=(struct COLVERTEX *)malloc(16*s->nVert);
			if (s->vertex==NULL) return FALSE;
			if ((long)coll.Read(s->vertex,16*s->nVert)!=16*s->nVert) return FALSE;
			s->polygon=(struct COLPOLYGON *)malloc(6*s->nPoly);
			if (s->polygon==NULL) return FALSE;
			if ((long)coll.Read(s->polygon,6*s->nPoly)!=6*s->nPoly) return FALSE;
			if (delta>0) if ((int)coll.Read(&dummy,delta)!=delta) return FALSE;
		}

	// object XB
		if (coll.Read(&col.objectHead,8)!=8) return FALSE;
		if ((col.objectHead.xbid!=XBID_OBJECT)&&(col.objectHead.xbid!=XBID_OBJECT2)) return FALSE;
		o=col.object=(struct COLOBJECT *)
				malloc(col.objectHead.nrec*sizeof(struct COLOBJECT));
		if (o==NULL) return FALSE;
		memset(o,0,col.objectHead.nrec*sizeof(struct COLOBJECT));
		for (i=0;i<col.objectHead.nrec;i++,o++) {
			if (coll.Read(o,4)!=4) return FALSE;
			if (o->type==1) {
				if (o->size!=16) return FALSE;
				if (coll.Read(&(o->ptRef),12)!=12) return FALSE;
			} else if (o->type==3) {
				if (coll.Read(&(o->animLength),4)!=4) return FALSE;
				if (o->size!=8+20*o->animLength) return FALSE;
				o->animData=(struct ANIMDATA *)malloc(20*o->animLength);
				if (o->animData==NULL) return FALSE;
				if ((long)coll.Read(o->animData,20*o->animLength)!=20*o->animLength) return FALSE;
				o->ptRef.x=o->animData->pt.x;
				o->ptRef.z=o->animData->pt.z;
				o->ptRef.y=o->animData->pt.y;
			} else return FALSE; // unknown object type
		}
	}

	// object2 XB
	if (col.nBlocks==5) {
		if (coll.Read(&col.object2Head,8)!=8) return FALSE;
		if ((col.object2Head.xbid!=XBID_OBJECT)&&(col.object2Head.xbid!=XBID_OBJECT2)) return FALSE;
		o=col.object2=(struct COLOBJECT *)
				malloc(col.object2Head.nrec*sizeof(struct COLOBJECT));
		if (o==NULL) return FALSE;
		memset(o,0,col.object2Head.nrec*sizeof(struct COLOBJECT));
		for (i=0;i<col.object2Head.nrec;i++,o++) {
			if (coll.Read(o,4)!=4) return FALSE;
			if (o->type==1) {
				if (o->size!=16) return FALSE;
				if (coll.Read(&(o->ptRef),12)!=12) return FALSE;
			} else if (o->type==3) {
				if (coll.Read(&(o->animLength),4)!=4) return FALSE;
				if (o->size!=8+20*o->animLength) return FALSE;
				o->animData=(struct ANIMDATA *)malloc(20*o->animLength);
				if (o->animData==NULL) return FALSE;
				if ((long)coll.Read(o->animData,20*o->animLength)!=20*o->animLength) return FALSE;
				o->ptRef.x=o->animData->pt.x;
				o->ptRef.z=o->animData->pt.z;
				o->ptRef.y=o->animData->pt.y;
			} else return FALSE; // unknown object type
		}
	}

	// vroad XB
	if (coll.Read(&col.vroadHead,8)!=8) return FALSE;
	if (col.vroadHead.xbid!=XBID_VROAD) return FALSE;
	if (col.vroadHead.size!=8+36*col.vroadHead.nrec) return FALSE;
	ASSERT(col.vroadHead.nrec==trk[nBlocks-1].nStartPos+trk[nBlocks-1].nPositions);
	//col.vroad=(struct COLVROAD *)malloc(col.vroadHead.nrec*sizeof(struct COLVROAD));
	col.vroad=(struct COLVROAD *)malloc(nBlocks*8*sizeof(struct COLVROAD));
	if (col.vroad==NULL) return FALSE;
	if ((long)coll.Read(col.vroad,36*col.vroadHead.nrec)!=36*col.vroadHead.nrec) return FALSE;

	if (coll.Read(&i,4)!=0) return FALSE; // we ought to be at EOF now
	return TRUE;
}

void CT3EDDoc::SaveHSFRD(CArchive& ar)
{
	int i,j,k,l,m;
	struct TRKBLOCK *b;
	struct POLYGONBLOCK *p;
	struct XOBJDATA *x;
	struct HS_VROADBLOCK vroadblk;
	char ptrspace[44];
	struct POLYGONDATA tmppoly;

	HS_RecalcMinMaxXY(); // recompute some POLYVROADDATA

	ar.Write(header,28);
	nBlocks--;
	ar.Write(&nBlocks,4);
	nBlocks++;
	l=col.vroadHead.nrec;
	ar.Write(&l,4);

	for (i=0;i<l;i++) { // COL VROAD
		vroadblk.refPt.x=(float)(col.vroad[i].refPt.x/65536.0);
		vroadblk.refPt.z=(float)(col.vroad[i].refPt.z/65536.0);
		vroadblk.refPt.y=(float)(col.vroad[i].refPt.y/65536.0);
		vroadblk.normal.x=(float)(col.vroad[i].normal.x/128.0);
		vroadblk.normal.z=(float)(col.vroad[i].normal.z/128.0);
		vroadblk.normal.y=(float)(col.vroad[i].normal.y/128.0);
		vroadblk.forward.x=(float)(col.vroad[i].forward.x/128.0);
		vroadblk.forward.z=(float)(col.vroad[i].forward.z/128.0);
		vroadblk.forward.y=(float)(col.vroad[i].forward.y/128.0);
		vroadblk.right.x=(float)(col.vroad[i].right.x/128.0);
		vroadblk.right.z=(float)(col.vroad[i].right.z/128.0);
		vroadblk.right.y=(float)(col.vroad[i].right.y/128.0);
		vroadblk.leftWall=(float)(col.vroad[i].leftWall/65536.0);
		vroadblk.rightWall=(float)(col.vroad[i].rightWall/65536.0);
		memcpy(vroadblk.unknown1,col.hs_extra+7*i,28);
		ar.Write(&vroadblk,84);
	}

	memset(ptrspace,0,44);
	for (i=0;i<nBlocks;i++) { // TRKBLOCK heads
		b=&(trk[i]);
		p=&(poly[i]);
		ar.Write(p->sz,28); // track polygon numbers
		for (j=0;j<4;j++) ar.Write(&(p->obj[j].n1),4); // polyobjs
		ar.Write(ptrspace,44);
		ar.Write(&(b->nVertices),24); // vertices
		ar.Write(ptrspace,8);
		ar.Write(b,60); // ptCentre, ptBounding
		ar.Write(b->nbdData,4*0x12C);
		for (j=4*i;j<4*i+4;j++) {
			ar.Write(&(xobj[j].nobj),4);
			ar.Write(ptrspace,4);
		}
		// nVRoad has to be re-computed... (flags!=14)
		ASSERT(b->nPolygons==p->sz[4]);
		l=0;
		for (j=0;j<b->nPolygons;j++)
			if ((b->polyData[j].flags&15)!=14) l++;
		ar.Write(&l,4);
		ar.Write(&(b->hs_ptMin),24);
		ar.Write(ptrspace,4);
		ar.Write(&(b->nPositions),4);
		ar.Write(&(b->nXobj),4);
		ar.Write(ptrspace,12); // erases nPolyobj
		ar.Write(&(b->nSoundsrc),4);
		ar.Write(ptrspace,4);
		ar.Write(&(b->nLightsrc),4);
		ar.Write(ptrspace,4);
		ar.Write(b->hs_neighbors,32);
	}

	for (i=0;i<nBlocks;i++) { // TRKBLOCK data
		b=&(trk[i]);
		p=&(poly[i]);
		ar.Write(b->vert,12*b->nVertices);
		ar.Write(b->unknVertices,4*b->nVertices);
		for (j=0;j<b->nPolygons;j++)
			if ((b->polyData[j].flags&15)!=14) {
				ar.Write(b->polyData[j].hs_minmax,8);
				ar.Write(&(b->polyData[j].flags),1);
				ar.Write(&(b->polyData[j].hs_unknown),1);
				ar.Write(&j,2);
				ar.Write(b->vroadData+b->polyData[j].vroadEntry,12);
			}
		if (b->nXobj>0) ar.Write(b->xobj,20*b->nXobj);
		if (b->nSoundsrc) ar.Write(b->soundsrc,16*b->nSoundsrc);
		if (b->nLightsrc) ar.Write(b->lightsrc,16*b->nLightsrc);

		// polygons
		for (j=0;j<7;j++) for (k=0;k<p->sz[j];k++) {
			for (m=0;m<4;m++) tmppoly.vertex[m^1]=p->poly[j][k].vertex[m];
			memcpy(&(tmppoly.texture),&(p->poly[j][k].texture),5);
			ar.Write(&tmppoly,13);
		}
		for (j=0;j<4;j++) for (k=0;k<p->obj[j].nobj;k++)
			for (l=0;l<p->obj[j].numpoly[k];l++) {
				for (m=0;m<4;m++) tmppoly.vertex[m^1]=p->obj[j].poly[k][l].vertex[m];
				memcpy(&(tmppoly.texture),&(p->obj[j].poly[k][l].texture),5);
				ar.Write(&tmppoly,13);
			}

		// xobj
		for (j=4*i;j<4*i+4;j++) {
			for (k=0;k<xobj[j].nobj;k++) {
				x=&(xobj[j].obj[k]);
				ar.Write(x,12); // 3 headers
				if ((x->crosstype==4)||(x->crosstype==2))
					ar.Write(&(x->ptRef),12);
				else if (x->crosstype==3)
					ar.Write(x->unknown3,12);
				ar.Write(&(x->AnimMemory),4);
				ar.Write(ptrspace,4);
				ar.Write(&(x->nVertices),4);
				ar.Write(ptrspace,8);
				ar.Write(&(x->nPolygons),4);
				ar.Write(ptrspace,4);
			}
			for (k=0;k<xobj[j].nobj;k++) {
				x=&(xobj[j].obj[k]);
				if (x->crosstype==3) { // animated-specific
					ar.Write(x->unknown3+6,2);
					ar.Write(&(x->type3),6);
					ar.Write(x->animData,20*x->nAnimLength);
				}
				ar.Write(x->vert,12*x->nVertices);
				ar.Write(x->unknVertices,4*x->nVertices);
				for (l=0;l<x->nPolygons;l++) {
					for (m=0;m<4;m++) tmppoly.vertex[m^1]=x->polyData[l].vertex[m];
					memcpy(&(tmppoly.texture),&(x->polyData[l].texture),5);
					ar.Write(&tmppoly,13);
				}
			}
		}
	}

	j=4*nBlocks; //Global Objects
	ar.Write(&xobj[j],4);
	for (k=0;k<xobj[j].nobj;k++) 
	{
		x=&(xobj[j].obj[k]);
		ar.Write(x,12); // 3 headers
		if ((x->crosstype==4)||(x->crosstype==2)||(x->crosstype==1))
			ar.Write(&(x->ptRef),12);
		else if (x->crosstype==3)
			ar.Write(x->unknown3,12);
		ar.Write(&(x->AnimMemory),4);
		ar.Write(ptrspace,4);
		ar.Write(&(x->nVertices),4);
		ar.Write(ptrspace,8);
		ar.Write(&(x->nPolygons),4);
		ar.Write(ptrspace,4);
	}
	for (k=0;k<xobj[j].nobj;k++) 
	{
		x=&(xobj[j].obj[k]);
		if (x->crosstype==3) { // animated-specific
			ar.Write(x->unknown3+6,2);
			ar.Write(&(x->type3),6);
			ar.Write(x->animData,20*x->nAnimLength);
		}
		ar.Write(x->vert,12*x->nVertices);
		ar.Write(x->unknVertices,4*x->nVertices);
		for (l=0;l<x->nPolygons;l++) {
			for (m=0;m<4;m++) tmppoly.vertex[m^1]=x->polyData[l].vertex[m];
			memcpy(&(tmppoly.texture),&(x->polyData[l].texture),5);
			ar.Write(&tmppoly,13);
		}
	}
	if (hs_morexobjlen>0) ar.Write(hs_morexobj,hs_morexobjlen);
}

void CT3EDDoc::SaveFRD(CArchive& ar)
{
	int i,j,k,l;
	struct TRKBLOCK *b;
	struct POLYGONBLOCK *p;
	struct XOBJDATA *x;
	struct OBJPOLYBLOCK *o;

	ar.Write(header,28); // header & numblocks
	nBlocks--;
	ar.Write(&nBlocks,4);
	nBlocks++;

	// TRKBLOCKs
	for (i=0;i<nBlocks;i++) {
		b=&(trk[i]);
		// ptCentre, ptBounding, nVertices and 5 nUnknowns == 84 bytes
		ar.Write(b,84);
		ar.Write(b->vert,12*b->nVertices);
		ar.Write(b->unknVertices,4*b->nVertices);
		ar.Write(b->nbdData,4*0x12C);
		// nStartPos & various blk sizes == 32 bytes
		ar.Write(&(b->nStartPos),32);
		ar.Write(b->posData,8*b->nPositions);
		for (j=0;j<b->nPolygons;j++)
			ar.Write(b->polyData+j,8);
		ar.Write(b->vroadData,12*b->nVRoad);
		if (b->nXobj>0) ar.Write(b->xobj,20*b->nXobj);
//		if (b->nPolyobj>0) ar.Write(b->polyobj[0],20*b->nPolyobj);
		if (b->nSoundsrc>0) ar.Write(b->soundsrc,16*b->nSoundsrc);
		if (b->nLightsrc>0) ar.Write(b->lightsrc,16*b->nLightsrc);
	}

	// POLYGONBLOCKs
	for (i=0;i<nBlocks;i++) {
		p=&(poly[i]);
		for (j=0;j<7;j++) {
			ar.Write(&(p->sz[j]),4);
			if (p->sz[j]!=0) {
				ar.Write(&(p->szdup[j]),4);
				ar.Write(p->poly[j],14*p->sz[j]);
			}
		}
		for (j=0;j<4;j++) {
			o=&(p->obj[j]);
			ar.Write(&(o->n1),4);
			if (o->n1>0) {
				ar.Write(&(o->n2),4);
				l=0;
				for(k=0;k<o->n2;k++) {
					ar.Write(o->types+k,4);
					if (o->types[k]==1) {
						ar.Write(o->numpoly+l,4);
						ar.Write(o->poly[l],14*o->numpoly[l]);
						l++;
					}
				}
			}
		}
	}

	// XOBJBLOCKs
	for (i=0;i<=4*nBlocks;i++) {
		ar.Write(&(xobj[i].nobj),4);
		for (j=0;j<xobj[i].nobj;j++) {
			x=&(xobj[i].obj[j]);
			// 3 headers == 12 bytes 
			ar.Write(x,12);
			if (x->crosstype==4) { // basic objects
				ar.Write(&(x->ptRef),12);
				ar.Write(&(x->AnimMemory),4);
			} else if (x->crosstype==3) { // animated objects
				// unkn3, type3, objno, nAnimLength, unkn4 == 24 bytes
				ar.Write(x->unknown3,24);
				ar.Write(x->animData,20*x->nAnimLength);
			}
			ar.Write(&(x->nVertices),4);
			ar.Write(x->vert,12*x->nVertices);
			ar.Write(x->unknVertices,4*x->nVertices);
			ar.Write(&(x->nPolygons),4);
			ar.Write(x->polyData,14*x->nPolygons);
		}
	}

	// TEXTUREBLOCKs
	ar.Write(&nTextures,4);
	for (i=0;i<nTextures;i++)
		ar.Write(&(texture[i]),47);
}

void CT3EDDoc::SaveCOL(CFile& coll)
{
	struct COLSTRUCT3D *s;
	struct COLOBJECT *o;
	int i,delta,dummy;

	coll.Write(&col,16);
	coll.Write(col.xbTable,4*col.nBlocks);

	// texture XB
	coll.Write(&col.textureHead,8);
	coll.Write(col.texture,8*col.textureHead.nrec);

	// struct3D XB
	if (col.nBlocks>=4) {
		coll.Write(&col.struct3DHead,8);
		s=col.struct3D;
		for (i=0;i<col.struct3DHead.nrec;i++,s++) {
			coll.Write(s,8);
			coll.Write(s->vertex,16*s->nVert);
			coll.Write(s->polygon,6*s->nPoly);
			delta=(8+16*s->nVert+6*s->nPoly)%4;
			delta=(4-delta)%4; dummy=0; // padding
			if (delta>0) coll.Write(&dummy,delta);
		}

	// object XB
		coll.Write(&col.objectHead,8);
		o=col.object;
		for (i=0;i<col.objectHead.nrec;i++,o++) {
			coll.Write(o,4);
			if (o->type==1) coll.Write(&(o->ptRef),12);
			else if (o->type==3) {
				coll.Write(&(o->animLength),4);
				coll.Write(o->animData,20*o->animLength);
			}
		}
	}

	// object2 XB
	if (col.nBlocks==5) {
		coll.Write(&col.object2Head,8);
		o=col.object2;
		for (i=0;i<col.object2Head.nrec;i++,o++) {
			coll.Write(o,4);
			if (o->type==1) coll.Write(&(o->ptRef),12);
			else if (o->type==3) {
				coll.Write(&(o->animLength),4);
				coll.Write(o->animData,20*o->animLength);
			}
		}
	}

	// vroad XB
	coll.Write(&col.vroadHead,8);
	coll.Write(col.vroad,36*col.vroadHead.nrec);
	coll.Close();
}

void CT3EDDoc::Serialize(CArchive& ar)
{
	CFile coll,heightsfile, spdFAFile, spdRAFile;
	CString fileName, heightsfileName, spdFAName, spdRAName, EText;
	int i;
	EText="An error occured while loading :\n\n";
	
	fileName=ar.GetFile()->GetFilePath(); //Name & path of col file (NFS3)
	if (!fileName.Right(4).CompareNoCase(".FRD"))
		fileName=fileName.Left(fileName.GetLength()-4);
	fileName+=".COL";

	heightsfileName=ar.GetFile()->GetFilePath(); //Name & path of heights.sim
	if (!heightsfileName.Right(4).CompareNoCase(".FRD"))
		heightsfileName=heightsfileName.Left(heightsfileName.GetLength()-ar.GetFile()->GetFileName().GetLength());
	heightsfileName+="heights.sim";

	spdFAName=ar.GetFile()->GetFilePath(); //Name & path of spdFA.bin
	if (!spdFAName.Right(4).CompareNoCase(".FRD"))
		spdFAName=spdFAName.Left(spdFAName.GetLength()-ar.GetFile()->GetFileName().GetLength());
	spdFAName+="spdFA.bin";
	
	spdRAName=ar.GetFile()->GetFilePath(); //Name & path of spdRA.bin
	if (!spdRAName.Right(4).CompareNoCase(".FRD"))
		spdRAName=spdRAName.Left(spdRAName.GetLength()-ar.GetFile()->GetFileName().GetLength());
	spdRAName+="spdRA.bin";
	
	if (ar.IsStoring())
	{
		ASSERT(!bEmpty);
		if (bHSMode) SaveHSFRD(ar);
		else {
			SaveFRD(ar);
			if (!coll.Open(fileName,CFile::modeCreate|CFile::modeWrite|CFile::shareDenyWrite))
				AfxThrowFileException(CFileException::accessDenied,-1,fileName);
			SaveCOL(coll);
		}

		if (HeightsLoaded) //Was the heights.sim loaded
		{
			if (!heightsfile.Open(heightsfileName,CFile::modeCreate|CFile::modeWrite|CFile::shareDenyWrite))
				AfxThrowFileException(CFileException::accessDenied,-1,heightsfileName);
			SaveHeightssim(heightsfile);
		}
		if (spdFALoaded) //Was the spdFA.bin loaded ?
		{
			if (!spdFAFile.Open(spdFAName,CFile::modeCreate|CFile::modeWrite|CFile::shareDenyWrite))
				AfxThrowFileException(CFileException::accessDenied,-1,spdFAName);
			SaveSpeedFile(spdFAFile, spdFAbin);
		}
		if (spdRALoaded) //Was the spdRA.bin loaded ?
		{
			if (!spdRAFile.Open(spdRAName,CFile::modeCreate|CFile::modeWrite|CFile::shareDenyWrite))
				AfxThrowFileException(CFileException::accessDenied,-1,spdRAName);
			SaveSpeedFile(spdRAFile, spdRAbin);
		}
		for (i=0;i<undoLevel;i++) DeleteUndo(i);
		undoLevel=0;
	}
	else
	{
		if (!bEmpty) DeleteContents();
		bEmpty=FALSE;
		trk=NULL; poly=NULL; xobj=NULL; hs_morexobj=NULL;
		memset(&col,0,sizeof(struct COLFILE));
		texture=NULL;
		if (LoadFRD(ar)==FALSE) // failed loading FRD file
			AfxThrowArchiveException(CArchiveException::badIndex,NULL);
		if (!bHSMode) { // original NFS3 mode
			if (!coll.Open(fileName,CFile::modeRead|CFile::shareDenyWrite))
				AfxThrowFileException(CFileException::fileNotFound,-1,
					CString("The auxiliary file ")+fileName);
			if (LoadCOL(coll)==FALSE) // failed loading COL file
				AfxThrowArchiveException(CArchiveException::badIndex,NULL);
		}
		//Load height.sim if present
		if (!heightsfile.Open(heightsfileName,CFile::modeRead|CFile::shareDenyWrite))
		{
			HeightsLoaded=FALSE;
			EText=EText + "heights.sim\n";
		}
		else
		HeightsLoaded=LoadHeightssim(heightsfile);
		
		//Load the spdFa.bin speed file
		if (!spdFAFile.Open(spdFAName,CFile::modeRead|CFile::shareDenyWrite))
			spdFALoaded=FALSE;
		else
		{
			spdFAbin=(struct SPDFILE *)malloc(8*nBlocks*sizeof(struct SPDFILE));
			memset(spdFAbin,0,8*nBlocks*sizeof(struct SPDFILE));
			spdFALoaded=LoadSpeedFile(spdFAFile, spdFAbin);
			//float value;
			//value=spdFAbin[col.vroadHead.nrec-1].turn;// Last value Ok, Del when working !
		}
		if (!spdFALoaded) EText=EText + "spdFA.bin\n";

		//Load the spdRa.bin speed file
		if (!spdRAFile.Open(spdRAName,CFile::modeRead|CFile::shareDenyWrite))
			spdRALoaded=FALSE;
		else
		{
			spdRAbin=(struct SPDFILE *)malloc(8*nBlocks*sizeof(struct SPDFILE));
			memset(spdRAbin,0,8*nBlocks*sizeof(struct SPDFILE));
			spdRALoaded=LoadSpeedFile(spdRAFile, spdRAbin);
			//float value;
			//value=spdRAbin[col.vroadHead.nrec-1].turn;// Last value Ok, Del when working !
		}
		if (!spdRALoaded) EText=EText + "spdRA.bin\n";

		if (((!HeightsLoaded)||(!spdFALoaded)||(!spdRALoaded))&&bHSMode)
			AfxMessageBox(EText);
		/*if ((!HeightsLoaded)||(!spdFALoaded)||(!spdRALoaded))
			MessageBox(EText,"T3ED : Error",MB_ICONWARNING|MB_OK);*/
		
		// prepare basic info for QFS viewer
		ASSERT(qfsView.pBitmaps==NULL);
		//HOO: Do not need to load qfs if loaded by Track View (4)
		if (!qfsView.bIsLoadQfs) qfsView.promptLoad=TRUE;
		//HOO: (4)
		fileName=ar.GetFile()->GetFilePath();
		i=fileName.ReverseFind('\\');
		if (i<0) i=0;
		qfsView.defPath=fileName.Left(i);
		fileName=ar.GetFile()->GetFileName();
		if (!fileName.Right(4).CompareNoCase(".FRD"))
			 qfsView.defName=fileName.Left(fileName.GetLength()-4)+"0.QFS";
		else qfsView.defName.Empty();
		qfsView.nColEntries=nTextures;
		qfsView.col=texture;
		qfsView.bHSMode=bHSMode;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CT3EDDoc diagnostics

#ifdef _DEBUG
void CT3EDDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CT3EDDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CT3EDDoc commands

void CT3EDDoc::FreeTrkContents(struct TRKBLOCK *trk)
{
	if (trk==NULL) return;
	dofree(trk->vert);
	dofree(trk->unknVertices);
	dofree(trk->posData);
	dofree(trk->polyData);
	dofree(trk->vroadData);
	dofree(trk->xobj);
//	if (trk->polyobj!=NULL) dofree(trk->polyobj[0]); // because malloc is linear 20*nCross2
//	dofree(trk->polyobj);
	dofree(trk->soundsrc);
	dofree(trk->lightsrc);
}

void CT3EDDoc::FreePolyContents(struct POLYGONBLOCK *poly)
{
	int j,k;

	if (poly==NULL) return;
	for (j=0;j<7;j++) dofree(poly->poly[j]);
	for (j=0;j<4;j++) {
		dofree(poly->obj[j].types);
		dofree(poly->obj[j].numpoly);
		if (poly->obj[j].poly!=NULL)
			for (k=0;k<poly->obj[j].nobj;k++) 
				dofree(poly->obj[j].poly[k]);
		dofree(poly->obj[j].poly);
	}
}

void CT3EDDoc::FreeXobjContents(struct XOBJBLOCK *xobj)
{
	int j;

	if (xobj==NULL) return;
	if (xobj->obj!=NULL) for (j=0;j<xobj->nobj;j++) {
		dofree(xobj->obj[j].vert);
		dofree(xobj->obj[j].unknVertices);
		dofree(xobj->obj[j].polyData);
		if (xobj->obj[j].crosstype==3)
			dofree(xobj->obj[j].animData);
	}
	dofree(xobj->obj);
}

void CT3EDDoc::DeleteUndo(int i)
{
	int j,k;

	for (j=0;j<MAX_BLOCKS;j++) {
		FreeTrkContents(undo[i][j].trk);
		dofree(undo[i][j].trk);
		FreePolyContents(undo[i][j].poly);
		dofree(undo[i][j].poly);
		for (k=0;k<4;k++) {
			FreeXobjContents(undo[i][j].xobj[k]);
			dofree(undo[i][j].xobj[k]);
		}
	}
	dofree(undoCol[i]);
	dofree(undoHeights[i]);
	dofree(undoSpdFa[i]);
	dofree(undoSpdRa[i]);
}

void CT3EDDoc::DeleteContents() 
{
	int i;

	if (bEmpty) return;
	bEmpty=TRUE;			// free all allocated memory
	if (trk!=NULL) for (i=0;i<nBlocks;i++) 
		FreeTrkContents(trk+i);
	dofree(trk);
	if (poly!=NULL) for (i=0;i<nBlocks;i++) 
		FreePolyContents(poly+i);
	dofree(poly);
	if (xobj!=NULL) for (i=0;i<=4*nBlocks;i++)
		FreeXobjContents(xobj+i);
	dofree(xobj);
	dofree(texture);
	dofree(hs_morexobj);
	dofree(col.hs_extra);
	dofree(col.texture);
	if (col.struct3D!=NULL) 
		for (i=0;i<col.struct3DHead.nrec;i++) {
			dofree(col.struct3D[i].vertex);
			dofree(col.struct3D[i].polygon);
		}
	dofree(col.struct3D);
	if (col.object!=NULL)
		for (i=0;i<col.objectHead.nrec;i++)
			dofree(col.object[i].animData);
	dofree(col.object);
	if (col.object2!=NULL)
		for (i=0;i<col.object2Head.nrec;i++)
			dofree(col.object2[i].animData);
	dofree(col.object2);
	dofree(col.vroad);
	for (i=0;i<undoLevel;i++) DeleteUndo(i);
	undoLevel=0;
	qfsView.Reset();

	//HOO: Clean up the track view too (5)
	TrackView->DestroyWindow();
	bCreateScene=FALSE;
	//HOO: (5)
}

void CT3EDDoc::PrepareNewUndo(struct FLOATPT *refpt,int block,int ofsx,int ofsy)
{
	int i;

	if (undoLevel==0) wasInitiallyModified=IsModified();
	if (undoLevel==UNDO_LEVELS) {
		DeleteUndo(0);
		wasInitiallyModified=TRUE;
		for (i=0;i<UNDO_LEVELS-1;i++) {
			memcpy(undo[i],undo[i+1],MAX_BLOCKS*sizeof(struct UNDOINFO));
			memcpy(undoRefpt+i,undoRefpt+i+1,12);
			undoRefblk[i]=undoRefblk[i+1];
			undoOfsx[i]=undoOfsx[i+1];
			undoOfsy[i]=undoOfsy[i+1];
			undoCol[i]=undoCol[i+1];
			undoHeights[i]=undoHeights[i+1];
			undoSpdFa[i]=undoSpdFa[i+1];
			undoSpdRa[i]=undoSpdRa[i+1];
		}
	} else undoLevel++;
	memset(undo[undoLevel-1],0,MAX_BLOCKS*sizeof(struct UNDOINFO));
	memcpy(&(undoRefpt[undoLevel-1]),refpt,12);
	undoRefblk[undoLevel-1]=block;
	undoOfsx[undoLevel-1]=ofsx;
	undoOfsy[undoLevel-1]=ofsy;
	undoCol[undoLevel-1]=NULL;
	undoHeights[undoLevel-1]=NULL;
	undoSpdFa[undoLevel-1]=NULL;
	undoSpdRa[undoLevel-1]=NULL;
	SetModifiedFlag(TRUE);

	//HOO: Notified Track View to update scene (6)
	bUpdatedScene=FALSE;
	//HOO: (6)
}

void CT3EDDoc::PrepareModifyTrk(int blk)
{
	struct UNDOINFO *u;
	struct TRKBLOCK *t;

	if (undoLevel==0) return;
	if (blk==nBlocks) return;
	u=&(undo[undoLevel-1][blk]);
	if (u->trk!=NULL) return;
	u->trk=(struct TRKBLOCK *)malloc(sizeof(struct TRKBLOCK));
	t=&(trk[blk]);
	memcpy(u->trk,t,sizeof(struct TRKBLOCK));
	u->trk->vert=NULL;
	u->trk->unknVertices=NULL;
	if (t->nVertices!=0) {
		u->trk->vert=(struct FLOATPT *)malloc(12*t->nVertices);
		memcpy(u->trk->vert,t->vert,12*t->nVertices);
		u->trk->unknVertices=(unsigned long *)malloc(4*t->nVertices);
		memcpy(u->trk->unknVertices,t->unknVertices,4*t->nVertices);
	}
	u->trk->posData=NULL;
	if (t->nPositions!=0) {
		u->trk->posData=(struct POSITIONDATA *)malloc(8*t->nPositions);
		memcpy(u->trk->posData,t->posData,8*t->nPositions);
	}
	u->trk->polyData=NULL;
	if (t->nPolygons!=0) {
		u->trk->polyData=(struct POLYVROADDATA *)malloc(sizeof(struct POLYVROADDATA)*t->nPolygons);
		memcpy(u->trk->polyData,t->polyData,sizeof(struct POLYVROADDATA)*t->nPolygons);
	}
	u->trk->vroadData=NULL;
	if (t->nVRoad!=0) {
		u->trk->vroadData=(struct VROADDATA *)malloc(12*t->nVRoad);
		memcpy(u->trk->vroadData,t->vroadData,12*t->nVRoad);
	}
	u->trk->xobj=NULL;
	if (t->nXobj!=0) {
		u->trk->xobj=(struct REFXOBJ *)malloc(20*t->nXobj);
		memcpy(u->trk->xobj,t->xobj,20*t->nXobj);
	}
	u->trk->soundsrc=NULL;
	if (t->nSoundsrc!=0) {
		u->trk->soundsrc=(struct SOUNDSRC *)malloc(16*t->nSoundsrc);
		memcpy(u->trk->soundsrc,t->soundsrc,16*t->nSoundsrc);
	}
	u->trk->lightsrc=NULL;
	if (t->nLightsrc!=0) {
		u->trk->lightsrc=(struct LIGHTSRC *)malloc(16*t->nLightsrc);
		memcpy(u->trk->lightsrc,t->lightsrc,16*t->nLightsrc);
	}
}

void CT3EDDoc::PrepareModifyPoly(int blk)
{
	struct UNDOINFO *u;
	struct POLYGONBLOCK *p;
	struct OBJPOLYBLOCK *o1,*o2;
	int j,k;

	if (undoLevel==0) return;
	if (blk==nBlocks) return;
	u=&(undo[undoLevel-1][blk]);
	if (u->poly!=NULL) return;
	u->poly=(struct POLYGONBLOCK *)malloc(sizeof(struct POLYGONBLOCK));
	p=&(poly[blk]);
	memcpy(u->poly,p,sizeof(struct POLYGONBLOCK));
	for (j=0;j<7;j++) {
		u->poly->poly[j]=NULL;
		if (p->sz[j]!=0) {
			u->poly->poly[j]=(LPPOLYGONDATA)malloc(14*p->sz[j]);
			memcpy(u->poly->poly[j],p->poly[j],14*p->sz[j]);
		}
	}
	for (j=0;j<4;j++) {
		o1=&(p->obj[j]); 
		o2=&(u->poly->obj[j]);
		o2->types=NULL;
		if (o1->n2!=0) {
			o2->types=(long *)malloc(4*o1->n2);
			memcpy(o2->types,o1->types,4*o1->n2);
		}
		o2->numpoly=NULL;
		o2->poly=NULL;
		if (o1->nobj!=0) {
			o2->numpoly=(long *)malloc(4*o1->nobj);
			memcpy(o2->numpoly,o1->numpoly,4*o1->nobj);
			o2->poly=(LPPOLYGONDATA *)malloc(4*o1->nobj);
			for (k=0;k<o1->nobj;k++) {
				o2->poly[k]=(LPPOLYGONDATA)malloc(14*o1->numpoly[k]);
				memcpy(o2->poly[k],o1->poly[k],14*o1->numpoly[k]);
			}
		}
	}
}

void CT3EDDoc::PrepareModifyXobj(int blk)
{
	struct UNDOINFO *u;
	int j;
	struct XOBJDATA *x1,*x2;

	if (undoLevel==0) return;
	//if (blk==nBlocks) return; //Undo for Global objects !
	u=&(undo[undoLevel-1][blk/4]);
	if (u->xobj[blk%4]!=NULL) return;
	u->xobj[blk%4]=(struct XOBJBLOCK *)malloc(sizeof(struct XOBJBLOCK));
	u->xobj[blk%4]->nobj=xobj[blk].nobj;
	u->xobj[blk%4]->obj=NULL;
	if (xobj[blk].nobj==0) return;
	x1=xobj[blk].obj;
	x2=u->xobj[blk%4]->obj=(struct XOBJDATA *)
			malloc(xobj[blk].nobj*sizeof(struct XOBJDATA));
	memcpy(x2,x1,xobj[blk].nobj*sizeof(struct XOBJDATA));
	for (j=0;j<xobj[blk].nobj;j++,x1++,x2++) {
		if ((x1->crosstype==3)&&(x1->animData!=NULL)) {
			x2->animData=(struct ANIMDATA *)malloc(20*x1->nAnimLength);
			memcpy(x2->animData,x1->animData,20*x1->nAnimLength);
		}
		x2->vert=(struct FLOATPT *)malloc(12*x1->nVertices);
		memcpy(x2->vert,x1->vert,12*x1->nVertices);
		x2->unknVertices=(long *)malloc(4*x1->nVertices);
		memcpy(x2->unknVertices,x1->unknVertices,4*x1->nVertices);
		x2->polyData=(LPPOLYGONDATA)malloc(14*x1->nPolygons);
		memcpy(x2->polyData,x1->polyData,14*x1->nPolygons);
	}
}

void CT3EDDoc::PerformUndo(BOOL KeepVRoad)
{
	int i,j,n;
	BOOL recalcXobj=FALSE;

	undoLevel--;
	for (i=0;i<nBlocks;i++) {
		if (undo[undoLevel][i].trk!=NULL) {
			FreeTrkContents(&(trk[i]));
			memcpy(&(trk[i]),undo[undoLevel][i].trk,sizeof(struct TRKBLOCK));
			dofree(undo[undoLevel][i].trk);
		}
		if (undo[undoLevel][i].poly!=NULL) {
			FreePolyContents(&(poly[i]));
			memcpy(&(poly[i]),undo[undoLevel][i].poly,sizeof(struct POLYGONBLOCK));
			dofree(undo[undoLevel][i].poly);
		}
		for (j=0;j<4;j++) if (undo[undoLevel][i].xobj[j]!=NULL) {
			recalcXobj=TRUE;
			FreeXobjContents(&(xobj[4*i+j]));
			memcpy(&(xobj[4*i+j]),undo[undoLevel][i].xobj[j],sizeof(struct XOBJBLOCK));
			dofree(undo[undoLevel][i].xobj[j]);
		}
	}
	if (recalcXobj) {
		n=0;
		for (i=0;i<nBlocks;i++)
			for (j=0;j<trk[i].nXobj;j++)
				trk[i].xobj[j].globalno=n++;
	}

	if ((undoCol[undoLevel]!=NULL)&&(!KeepVRoad)) {
		dofree(col.vroad);
		col.vroad=undoCol[undoLevel];
	}

	if ((undoHeights[undoLevel]!=NULL)&&(HeightsLoaded))
		{
			dofree(hightssim);
			hightssim=undoHeights[undoLevel];
			undoHeights[undoLevel]=NULL;
		}
	if ((undoSpdFa[undoLevel]!=NULL)&&(spdFALoaded))
		{
			dofree(spdFAbin);
			spdFAbin=undoSpdFa[undoLevel];
			undoSpdFa[undoLevel]=NULL;
		}
	if ((undoSpdRa[undoLevel]!=NULL)&&(spdRALoaded))
		{
			dofree(spdRAbin);
			spdRAbin=undoSpdRa[undoLevel];
			undoSpdRa[undoLevel]=NULL;
		}

	if (undoLevel==0) SetModifiedFlag(wasInitiallyModified);

	//HOO: Scene changed (7)
	bUpdatedScene = FALSE;
	//HOO: Update track view (7)
}

//////////////////////////////////////////////////////////////////////
// Editing functions

BOOL CT3EDDoc::CanContainPoint(struct TRKBLOCK *t,struct FLOATPT *pt,float margin)
{
	if ((t->ptBounding[0].x<pt->x-margin)&&(t->ptBounding[1].x<pt->x-margin)&&
		(t->ptBounding[2].x<pt->x-margin)&&(t->ptBounding[3].x<pt->x-margin))
			return FALSE;
	if ((t->ptBounding[0].x>pt->x+margin)&&(t->ptBounding[1].x>pt->x+margin)&&
		(t->ptBounding[2].x>pt->x+margin)&&(t->ptBounding[3].x>pt->x+margin))
			return FALSE;
	if ((t->ptBounding[0].y<pt->y-margin)&&(t->ptBounding[1].y<pt->y-margin)&&
		(t->ptBounding[2].y<pt->y-margin)&&(t->ptBounding[3].y<pt->y-margin))
			return FALSE;
	if ((t->ptBounding[0].y>pt->y+margin)&&(t->ptBounding[1].y>pt->y+margin)&&
		(t->ptBounding[2].y>pt->y+margin)&&(t->ptBounding[3].y>pt->y+margin))
			return FALSE;
	return TRUE;
}

void CT3EDDoc::RecalcBoundingBox(int i)
{
	float xmin,xmax,ymin,ymax;
	int j,k,l,n;
	struct FLOATPT *v,w;
	struct XOBJDATA *x;

	if (i==nBlocks) return;
	PrepareModifyTrk(i);
	xmin=xmax=trk[i].ptCentre.x;
	ymin=ymax=trk[i].ptCentre.y;
	n=trk[i].nHiResVert;
	v=trk[i].vert;
	for (j=0;j<n;j++,v++) {
		if (v->x<xmin) xmin=v->x;
		if (v->x>xmax) xmax=v->x;
		if (v->y<ymin) ymin=v->y;
		if (v->y>ymax) ymax=v->y;
	}
	for (j=0;j<trk[i].nLightsrc;j++) {
		w.x=((float)trk[i].lightsrc[j].refpoint.x)/65536;
		w.y=((float)trk[i].lightsrc[j].refpoint.y)/65536;
		if (w.x<xmin) xmin=w.x;
		if (w.x>xmax) xmax=w.x;
		if (w.y<ymin) ymin=w.y;
		if (w.y>ymax) ymax=w.y;
	}
	for (l=4*i;l<4*i+4;l++)
		for (k=0;k<xobj[l].nobj;k++) {
			x=&(xobj[l].obj[k]);
			n=x->nVertices;
			v=x->vert;
			for (j=0;j<n;j++,v++) {
				if (v->x+x->ptRef.x<xmin) xmin=v->x+x->ptRef.x;
				if (v->x+x->ptRef.x>xmax) xmax=v->x+x->ptRef.x;
				if (v->y+x->ptRef.y<ymin) ymin=v->y+x->ptRef.y;
				if (v->y+x->ptRef.y>ymax) ymax=v->y+x->ptRef.y;
			}
		}
	trk[i].ptBounding[0].x=xmin-1; trk[i].ptBounding[0].y=ymin-1;
	trk[i].ptBounding[1].x=xmax+1; trk[i].ptBounding[1].y=ymin-1;
	trk[i].ptBounding[2].x=xmax+1; trk[i].ptBounding[2].y=ymax+1;
	trk[i].ptBounding[3].x=xmin-1; trk[i].ptBounding[3].y=ymax+1;
	for (j=0;j<4;j++) trk[i].ptBounding[j].z=trk[i].ptCentre.z;
}

void CT3EDDoc::RecalcPolyVroad(int blk,int no,struct FLOATPT *optdir)
{
	BOOL isUnique=TRUE;
	int i;
	struct TRKBLOCK *t=&(trk[blk]);
	struct POLYVROADDATA *p=&(t->polyData[no]);
	struct VROADDATA *v;
	struct FLOATPT v1,v2,norm;
	float len,prod;
	short *vno;
	
	PrepareModifyTrk(blk);
	for (i=0;i<t->nPolygons;i++)
		if ((i!=no)&&(t->polyData[i].vroadEntry==p->vroadEntry)) 
			isUnique=FALSE;
	if (!isUnique) {
		t->vroadData=(struct VROADDATA *)
			realloc(t->vroadData,(t->nVRoad+1)*sizeof(struct VROADDATA));
		memcpy(t->vroadData+t->nVRoad,t->vroadData+p->vroadEntry,
			sizeof(struct VROADDATA));
		p->vroadEntry=(unsigned char)t->nVRoad++;
	}
	v=t->vroadData+p->vroadEntry;
	vno=poly[blk].poly[4][no].vertex;
	v1.x=t->vert[vno[1]].x-t->vert[vno[3]].x;
	v1.z=t->vert[vno[1]].z-t->vert[vno[3]].z;
	v1.y=t->vert[vno[1]].y-t->vert[vno[3]].y;
	v2.x=t->vert[vno[2]].x-t->vert[vno[0]].x;
	v2.z=t->vert[vno[2]].z-t->vert[vno[0]].z;
	v2.y=t->vert[vno[2]].y-t->vert[vno[0]].y;
	norm.x=-v1.y*v2.z+v1.z*v2.y;
	norm.y=-v1.z*v2.x+v1.x*v2.z;
	norm.z=-v1.x*v2.y+v1.y*v2.x;
	len=(float)sqrt(norm.x*norm.x+norm.y*norm.y+norm.z*norm.z);
	v->xNorm=(short)(norm.x*32767/len);
	v->zNorm=(short)(norm.z*32767/len);
	v->yNorm=(short)(norm.y*32767/len);
	if (optdir!=NULL) {
		v1.x=optdir->x; v1.z=optdir->z; v1.y=optdir->y;
	} else {
		prod=v->xForw*norm.x+v->yForw*norm.y+v->zForw*norm.z;
		v1.x=v->xForw-prod*norm.x/(len*len);
		v1.z=v->zForw-prod*norm.z/(len*len);
		v1.y=v->yForw-prod*norm.y/(len*len);
	}
	len=(float)sqrt(v1.x*v1.x+v1.y*v1.y+v1.z*v1.z);
	v->xForw=(short)(v1.x*32767/len);
	v->zForw=(short)(v1.z*32767/len);
	v->yForw=(short)(v1.y*32767/len);
}

void CT3EDDoc::MovePointBy(struct FLOATPT *refpt,float dx,float dy,float dz)
{
	int i,j,k,l,n;
	struct FLOATPT *v;
	BOOL isModified;
	LPPOLYGONDATA p;
	struct XOBJDATA *x;
	struct FLOATPT pt;

	pt.x=refpt->x; pt.y=refpt->y; pt.z=refpt->z;
	for (i=0;i<nBlocks;i++) {
		if (!CanContainPoint(trk+i,&pt,0.0)) continue;
		isModified=FALSE;
		v=trk[i].vert;
		n=trk[i].nVertices;
		for (j=0;j<n;j++,v++)
/*			if ((v->x==pt.x)&&(v->z==pt.z)&&(v->y==pt.y)) { */
			if ((fabs(v->x-pt.x)<1E-3)&&(fabs(v->z-pt.z)<1E-3)&&(fabs(v->y-pt.y)<1E-3))
			{
				PrepareModifyTrk(i);
				isModified=TRUE;
				v->x+=dx; v->z+=dz; v->y+=dy;
				p=poly[i].poly[4];
				for (k=0;k<trk[i].nPolygons;k++,p++)
					if ((j==p->vertex[0])||(j==p->vertex[1])||
						(j==p->vertex[2])||(j==p->vertex[3]))
					RecalcPolyVroad(i,k);
			}
		for (j=4*i;j<4*i+4;j++) {
			x=xobj[j].obj;
			for (k=0;k<xobj[j].nobj;k++,x++) {
				v=x->vert;
				for (l=0;l<x->nVertices;l++,v++)
/*					if ((v->x+x->ptRef.x==pt.x)&&
						(v->z+x->ptRef.z==pt.z)&&
						(v->y+x->ptRef.y==pt.y)) */
					if ((fabs(v->x+x->ptRef.x-pt.x)<1E-3)&&(fabs(v->z+x->ptRef.z-pt.z)<1E-3)&&(fabs(v->y+x->ptRef.y-pt.y)<1E-3))
					{
						PrepareModifyXobj(j);
						isModified=TRUE;
						v->x+=dx; v->z+=dz; v->y+=dy;
					}
			}
		}
		if (isModified) {
			if ((dx!=0)||(dy!=0)) RecalcBoundingBox(i);
		}
	}
}

void CT3EDDoc::ExtMovePointBy(struct FLOATPT *refpt,float dx,float dy,float dz,float sDist,float sWidth)
{
	int i,j,k,l,n;
	struct FLOATPT *v;
	BOOL isModified;
	float dist;
	struct XOBJDATA *x;
	struct FLOATPT pt;
	struct LIGHTSRC *light;
	struct SOUNDSRC *sound;

	pt.x=refpt->x; pt.y=refpt->y; pt.z=refpt->z;
	for (i=0;i<nBlocks;i++) {
		if (!CanContainPoint(trk+i,&pt,sDist+sWidth)) continue;
		isModified=FALSE;
		v=trk[i].vert;
		n=trk[i].nVertices;
		for (j=0;j<n;j++,v++) {
			dist=(float)sqrt((v->x-pt.x)*(v->x-pt.x)+(v->y-pt.y)*(v->y-pt.y)+(v->z-pt.z)*(v->z-pt.z));
			if (dist<sDist+sWidth)
			{
				PrepareModifyTrk(i);
				isModified=TRUE;
				if (dist<sDist) dist=1.0;
				else dist=(float)(0.5+0.5*cos(3.1415926*(dist-sDist)/sWidth));
				v->x+=dx*dist; v->z+=dz*dist; v->y+=dy*dist;
			}
		}
		if (isModified)
			for (k=0;k<trk[i].nPolygons;k++)
				RecalcPolyVroad(i,k);
		// lights etc...
		for (j=0,light=trk[i].lightsrc;j<trk[i].nLightsrc;j++,light++) {
			dist=(float)sqrt(
				(light->refpoint.x/65536-pt.x)*(light->refpoint.x/65536-pt.x)+
				(light->refpoint.y/65536-pt.y)*(light->refpoint.y/65536-pt.y)+
				(light->refpoint.z/65536-pt.z)*(light->refpoint.z/65536-pt.z));
			if (dist<sDist+sWidth)
			{
				PrepareModifyTrk(i); isModified=TRUE;
				if (dist<sDist) dist=1.0;
				else dist=(float)(0.5+0.5*cos(3.1415926*(dist-sDist)/sWidth));
				light->refpoint.x+=(long)(65536*dx*dist); 
				light->refpoint.z+=(long)(65536*dz*dist); 
				light->refpoint.y+=(long)(65536*dy*dist);
			}
		}
		for (j=0,sound=trk[i].soundsrc;j<trk[i].nSoundsrc;j++,sound++) {
			dist=(float)sqrt(
				(sound->refpoint.x/65536-pt.x)*(sound->refpoint.x/65536-pt.x)+
				(sound->refpoint.y/65536-pt.y)*(sound->refpoint.y/65536-pt.y)+
				(sound->refpoint.z/65536-pt.z)*(sound->refpoint.z/65536-pt.z));
			if (dist<sDist+sWidth)
			{
				PrepareModifyTrk(i); isModified=TRUE;
				if (dist<sDist) dist=1.0;
				else dist=(float)(0.5+0.5*cos(3.1415926*(dist-sDist)/sWidth));
				sound->refpoint.x+=(long)(65536*dx*dist); 
				sound->refpoint.z+=(long)(65536*dz*dist); 
				sound->refpoint.y+=(long)(65536*dy*dist);
			}
		}
		// xobj
		for (j=4*i;j<4*i+4;j++) {
			x=xobj[j].obj;
			for (k=0;k<xobj[j].nobj;k++,x++) {
				v=x->vert;
				for (l=0;l<x->nVertices;l++,v++) {
					dist=(float)sqrt((v->x+x->ptRef.x-pt.x)*(v->x+x->ptRef.x-pt.x)+
							  (v->y+x->ptRef.y-pt.y)*(v->y+x->ptRef.y-pt.y)+
							  (v->z+x->ptRef.z-pt.z)*(v->z+x->ptRef.z-pt.z));
					if (dist<sDist+sWidth) {
						PrepareModifyXobj(j);
						isModified=TRUE;
						if (dist<sDist) dist=1.0;
						else dist=(float)(0.5+0.5*cos(3.1415926*(dist-sDist)/sWidth));
						v->x+=dx*dist; v->z+=dz*dist; v->y+=dy*dist;
					}
				}
			}
		}
		if (isModified) {
			if ((dx!=0)||(dy!=0)) RecalcBoundingBox(i);
		}
	}
}

void CT3EDDoc::ChangeXobjRefpoint(int blk, int isxobj, int chunk, int no, float ptx, float pty, float ptz)
{
	struct XOBJDATA *x;
	struct FLOATPT *v;
	float dx, dy, dz;
	int k;

	if (isxobj!=1) return; //Only Extra objects have a Refpoint
	//PrepareModifyXobj(4*blk+chunk); Done in MoveObject Function
	x=&(xobj[4*blk+chunk].obj[no]);
	dx=ptx - x->ptRef.x;dy=pty - x->ptRef.y;dz=ptz - x->ptRef.z;
	MoveObjectBy(blk, isxobj, chunk, no, dx, dy, dz); //Move the whole XObject
	v=x->vert;
	for (k=0;k<x->nVertices;k++)
	{
		v[k].x-=dx;
		v[k].y-=dy;
		v[k].z-=dz;
	}
}

int CT3EDDoc::MoveObjectBy(int blk,int isxobj,int chunk,int no,float dx,float dy,float dz)
{
	struct XOBJDATA *x;
	struct INTPT *pt;
	LPPOLYGONDATA p;
	struct TRKBLOCK *b;
	int i,j,k,l,c,num;
	short vno;
	float dist,dist0;
	struct FLOATPT *ref,tmppt;

	if (blk>nBlocks) return 0;
	PrepareModifyTrk(blk);
	b=&(trk[blk]);
	//JimD: Moving VRoad Points
	if (isxobj==4) {
		PrepareModifyVRoadHeightsSpdFiles();
		col.vroad[no].refPt.x+=(long)(dx*65536);
		col.vroad[no].refPt.z+=(long)(dz*65536);
		col.vroad[no].refPt.y+=(long)(dy*65536);
		ref=&tmppt;
		tmppt.x=((float)col.vroad[no].refPt.x)/65536;
		tmppt.z=((float)col.vroad[no].refPt.z)/65536;
		tmppt.y=((float)col.vroad[no].refPt.y)/65536;
	}
	else if (isxobj==3) {
		b->soundsrc[no].refpoint.x+=(long)(dx*65536);
		b->soundsrc[no].refpoint.z+=(long)(dz*65536);
		b->soundsrc[no].refpoint.y+=(long)(dy*65536);
		ref=&tmppt;
		tmppt.x=((float)b->soundsrc[no].refpoint.x)/65536;
		tmppt.z=((float)b->soundsrc[no].refpoint.z)/65536;
		tmppt.y=((float)b->soundsrc[no].refpoint.y)/65536;
	}
	else if (isxobj==2) {
		b->lightsrc[no].refpoint.x+=(long)(dx*65536);
		b->lightsrc[no].refpoint.z+=(long)(dz*65536);
		b->lightsrc[no].refpoint.y+=(long)(dy*65536);
		ref=&tmppt;
		tmppt.x=((float)b->lightsrc[no].refpoint.x)/65536;
		tmppt.z=((float)b->lightsrc[no].refpoint.z)/65536;
		tmppt.y=((float)b->lightsrc[no].refpoint.y)/65536;
	}
	else if (isxobj==1) {
		PrepareModifyXobj(4*blk+chunk);
		x=&(xobj[4*blk+chunk].obj[no]);
		if (x->crosstype==4) { // normal case
			pt=&(b->xobj[x->crossno].pt);
			pt->x+=(long)(dx*65536);
			pt->z+=(long)(dz*65536);
			pt->y+=(long)(dy*65536);
		}
		if (x->crosstype==3) { // animated xobj
			for (i=0;i<x->nAnimLength;i++) {
				x->animData[i].pt.x+=(long)(dx*65536);
				x->animData[i].pt.z+=(long)(dz*65536);
				x->animData[i].pt.y+=(long)(dy*65536);
			}
		}
		x->ptRef.x+=dx; x->ptRef.z+=dz; x->ptRef.y+=dy;
		ref=&(x->ptRef);
	} 
	else {
		num=poly[blk].obj[chunk].numpoly[no];
		p=poly[blk].obj[chunk].poly[no];
		ref=b->vert+p[0].vertex[0];
		for (i=0;i<num;i++) for (j=0;j<4;j++) {
			vno=p[i].vertex[j];
			c=0;
			for (k=0;k<i;k++) for (l=0;l<4;l++)
				if (p[k].vertex[l]==vno) c++;
			for (l=0;l<j;l++) if (p[i].vertex[l]==vno) c++;
			if (c==0) {
				b->vert[vno].x+=dx;
				b->vert[vno].y+=dy;
				b->vert[vno].z+=dz;
			}
		}
	}

	// see if the block should be changed
	if (((dx!=0)||(dy!=0))&&(bAutoObjMem)&&(isxobj!=4)) {
		RecalcBoundingBox(blk);
		dist0=(ref->x-b->ptCentre.x)*(ref->x-b->ptCentre.x)+
			  (ref->z-b->ptCentre.z)*(ref->z-b->ptCentre.z)+
			  (ref->y-b->ptCentre.y)*(ref->y-b->ptCentre.y);
		dist0=dist0/10; num=blk;
		for (i=0;i<nBlocks;i++) {
			b=&(trk[i]);
			dist=(ref->x-b->ptCentre.x)*(ref->x-b->ptCentre.x)+
				 (ref->z-b->ptCentre.z)*(ref->z-b->ptCentre.z)+
				 (ref->y-b->ptCentre.y)*(ref->y-b->ptCentre.y);
			if (dist<dist0) { num=i; dist0=dist; }
		}
		if (num!=blk) ChangeObjBlock(blk,isxobj,chunk,no,num);
		return num;
	}
	return blk;
}

void CT3EDDoc::ChangeObjBlock(int blk1,int isxobj,int chunk,int no,int blk2)
{
	struct XOBJDATA *x;
	struct XOBJBLOCK *xb1,*xb2;
	int i,j,k,l,n,ii,jj,nn,oldno,oldpos,shiftby,chunk2;
	short vno,vno2;
	struct OBJPOLYBLOCK *o,*oo;
	struct TRKBLOCK *t1,*t2;
	LPPOLYGONDATA p,pp;

	if (isxobj==4) return; //VRoad isn't a block member
	if (blk1==blk2) return;
	PrepareModifyTrk(blk1);
	PrepareModifyTrk(blk2);
	PrepareModifyPoly(blk1);
	PrepareModifyPoly(blk2);
	//Nappe1: Addition to moving soundsource
	if (isxobj==3) {
		t1=&(trk[blk1]);
		t2=&(trk[blk2]);
		t1->nSoundsrc--;
		t2->nSoundsrc++;
		t2->soundsrc=(struct SOUNDSRC *)realloc(t2->soundsrc,t2->nSoundsrc*sizeof(struct SOUNDSRC));
		memcpy(t2->soundsrc+t2->nSoundsrc-1,t1->soundsrc+no,sizeof(struct SOUNDSRC));
		for (i=no;i<t1->nSoundsrc;i++)
			memcpy(t1->soundsrc+i,t1->soundsrc+i+1,sizeof(struct SOUNDSRC));
	}
	//Nappe1: addition ends.
	else
	if (isxobj==2) {
		t1=&(trk[blk1]);
		t2=&(trk[blk2]);
		t1->nLightsrc--;
		t2->nLightsrc++;
		t2->lightsrc=(struct LIGHTSRC *)realloc(t2->lightsrc,t2->nLightsrc*sizeof(struct LIGHTSRC));
		memcpy(t2->lightsrc+t2->nLightsrc-1,t1->lightsrc+no,sizeof(struct LIGHTSRC));
		for (i=no;i<t1->nLightsrc;i++)
			memcpy(t1->lightsrc+i,t1->lightsrc+i+1,sizeof(struct LIGHTSRC));
	}
	else if (isxobj==1) {
		chunk2=chunk; if (blk2==nBlocks) chunk2=0; //Global object to chunk 0
		for (i=0;i<4;i++) PrepareModifyXobj(4*blk1+i);
		PrepareModifyXobj(4*blk2+chunk2);
		// update xobj blocks
		xb2=&(xobj[4*blk2+chunk2]); 
		xb1=&(xobj[4*blk1+chunk]);
		xb2->nobj++;
		xb2->obj=(struct XOBJDATA *)realloc(xb2->obj,xb2->nobj*sizeof(struct XOBJDATA));
		x=&(xb2->obj[xb2->nobj-1]);
		memcpy(x,&(xb1->obj[no]),sizeof(struct XOBJDATA));
		xb1->nobj--;
		for (i=no;i<xb1->nobj;i++)
			memcpy(&(xb1->obj[i]),&(xb1->obj[i+1]),sizeof(struct XOBJDATA));
		oldno=x->crossno;
		if (x->crosstype==4)
			for (i=4*blk1;i<4*blk1+4;i++)
				for (j=0;j<xobj[i].nobj;j++)
					if (xobj[i].obj[j].crossno>oldno)
						xobj[i].obj[j].crossno--;
		x->crossno=trk[blk2].nXobj;
		// update polyobj blocks
		o=&(poly[blk1].obj[chunk]);  
		if ((o->n1>0)&&(o->types!=NULL)) {  //TS o->types!=NULL can haben on global objects (UK)
			oldpos=-1; j=-1;
			while (j<no) if (o->types[++oldpos]!=1) j++;
			o->n2--;
			for (i=oldpos;i<o->n2;i++) o->types[i]=o->types[i+1];
		}
		o=&(poly[blk2].obj[chunk2]);
		if (o->n1>0) {
			o->n2++;
			o->types=(long *)realloc(o->types,4*o->n2);
			o->types[o->n2-1]=x->crosstype;
		}
		// update trk blocks
		t1=&(trk[blk1]);
		t2=&(trk[blk2]);
		if (x->crosstype==4) {
			t2->nXobj++;
			t2->xobj=(struct REFXOBJ *)realloc(t2->xobj,t2->nXobj*sizeof(struct REFXOBJ));
			memcpy(&(t2->xobj[t2->nXobj-1]),t1->xobj+oldno,sizeof(struct REFXOBJ));
			if (chunk2==0) t2->xobj[t2->nXobj-1].crossindex=0;
			t1->nXobj--;
			for (i=oldno;i<t1->nXobj;i++) {
				memcpy(&(t1->xobj[i]),&(t1->xobj[i+1]),sizeof(struct REFXOBJ));
				if ((chunk==0)&&(t1->xobj[i].crossindex!=0)) 
					t1->xobj[i].crossindex--;
			}
		}
		// update global xobj sequence numbers
		n=0;
		for (i=0;i<nBlocks;i++)
			for (j=0;j<trk[i].nXobj;j++)
				trk[i].xobj[j].globalno=n++;
	} else {
		// update vertices
		t1=&(trk[blk1]);
		t2=&(trk[blk2]);
		o=&(poly[blk1].obj[chunk]);
		n=o->numpoly[no];
		p=o->poly[no];
		shiftby=0;
		for (i=0;i<n;i++) for (j=0;j<4;j++) {
			vno=p[i].vertex[j];
			if (vno<0) continue;
			shiftby++;
			vno2=(short)t2->nObjectVert;
			// update dest block
			t2->nVertices++; t2->nVerticesDup++;
			t2->nObjectVert++;
			t2->nLoResVert++; t2->nHiResVert++; t2->nMedResVert++;
			t2->vert=(struct FLOATPT *)realloc(t2->vert,t2->nVertices*12);
			t2->unknVertices=(unsigned long *)realloc(t2->unknVertices,t2->nVertices*4);
			memmove(t2->vert+vno2+1,t2->vert+vno2,12*(t2->nVertices-vno2-1));
			memmove(t2->unknVertices+vno2+1,t2->unknVertices+vno2,4*(t2->nVertices-vno2-1));
			t2->unknVertices[vno2]=t1->unknVertices[vno];
			memcpy(t2->vert+vno2,t1->vert+vno,12);
			for (k=i;k<n;k++) for (l=0;l<4;l++)
				if (p[k].vertex[l]==vno) {
					p[k].vertex[l]=-vno2-1;
				}
			// update src block
			oo=poly[blk1].obj;
			for (ii=0;ii<4;ii++,oo++) 
				for (jj=0;jj<oo->nobj;jj++) {
					nn=oo->numpoly[jj];
					pp=oo->poly[jj];
					for (k=0;k<nn;k++,pp++) for (l=0;l<4;l++) {
						ASSERT(pp->vertex[l]!=vno);
						if (pp->vertex[l]>vno) pp->vertex[l]--;
					}
				}
			t1->nVertices--; t1->nVerticesDup--;
			t1->nObjectVert--;
			t1->nLoResVert--; t1->nHiResVert--; t1->nMedResVert--;
			memmove(t1->vert+vno,t1->vert+vno+1,12*(t1->nVertices-vno));
			memmove(t1->unknVertices+vno,t1->unknVertices+vno+1,4*(t1->nVertices-vno));
		}
		for (i=0;i<n;i++) for (j=0;j<4;j++)
			p[i].vertex[j]=-1-p[i].vertex[j];
		// p and n must be preserved for below
		for (i=0;i<=6;i++) {
			nn=poly[blk1].sz[i];
			pp=poly[blk1].poly[i];
			for (j=0;j<nn;j++,pp++) for (k=0;k<4;k++)
				pp->vertex[k]-=shiftby;
			nn=poly[blk2].sz[i];
			pp=poly[blk2].poly[i];
			for (j=0;j<nn;j++,pp++) for (k=0;k<4;k++)
				pp->vertex[k]+=shiftby;
		}
		// update objpolyblock
		o->n1-=n;
		o->n2--;
		o->nobj--;
		for (i=no;i<o->nobj;i++) {
			o->numpoly[i]=o->numpoly[i+1];
			o->poly[i]=o->poly[i+1];
		}
		oldpos=-1; j=-1;
		while (j<no) if (o->types[++oldpos]==1) j++;
		for (i=oldpos;i<o->n2;i++) o->types[i]=o->types[i+1];
		o=&(poly[blk2].obj[chunk]);
		if (o->n1==0) { // re-create objpolyblock
			o->n2=xobj[4*blk2+chunk].nobj;
			o->nobj=0;
			o->types=(long *)realloc(o->types,4*o->n2);
			for (i=0;i<o->n2;i++) 
				o->types[i]=xobj[4*blk2+chunk].obj[i].crosstype;
		}
		o->n1+=n;
		o->n2++;
		o->nobj++;
		o->types=(long *)realloc(o->types,4*o->n2);
		o->types[o->n2-1]=1;
		o->numpoly=(long *)realloc(o->numpoly,4*o->nobj);
		o->numpoly[o->nobj-1]=n;
		o->poly=(LPPOLYGONDATA *)realloc(o->poly,4*o->nobj);
		o->poly[o->nobj-1]=p;
	}
	RecalcBoundingBox(blk1);
	RecalcBoundingBox(blk2);
}

inline float CT3EDDoc::Distance(struct FLOATPT &a,struct INTPT &b) 
{
	return ((65536*a.x-b.x)*(65536*a.x-b.x)
		   +(65536*a.y-b.y)*(65536*a.y-b.y));
}

int CT3EDDoc::GlobalLocalCoord(struct FLOATPT &org,struct FLOATPT &res,struct COLVROAD *c,int start,int minblk,int maxblk)
{
	int i,n;
	float dist,test;
	struct INTPT *p1,*p2;

	n=col.vroadHead.nrec;
	i=start;
	dist=Distance(org,c[i].refPt);
	test=Distance(org,c[(i+1)%n].refPt);
	if (test<dist) { // move right
		do {
			i=(i+1)%n; dist=test;
			if (i==maxblk) break;
			test=Distance(org,c[(i+1)%n].refPt);
		} while (test<dist);
	} else { // move left ?
		test=Distance(org,c[(i+n-1)%n].refPt);
		while (test<dist) {
			i=(i+n-1)%n; dist=test;
			if (i==minblk) break;
			test=Distance(org,c[(i+n-1)%n].refPt);
		}
	}
	if (Distance(org,c[(i+1)%n].refPt)>Distance(org,c[(i+n-1)%n].refPt))
		i=(i+n-1)%n;
	p1=&(c[i].refPt); p2=&(c[(i+1)%n].refPt);
	dist=(float)(p2->x-p1->x)*(float)(p2->x-p1->x)+(float)(p2->y-p1->y)*(float)(p2->y-p1->y);
	test=(65536*org.x-p1->x)*(p2->x-p1->x)+(65536*org.y-p1->y)*(p2->y-p1->y);
	res.x=test/dist;
	test=(65536*org.x-p1->x)*(p2->y-p1->y)-(65536*org.y-p1->y)*(p2->x-p1->x);
	res.y=test/((float)sqrt(dist));
	res.z=65536*org.z-p1->z-res.x*(p2->z-p1->z);
	return i;
}

void CT3EDDoc::LocalGlobalCoord(struct FLOATPT &org,struct FLOATPT &res,struct COLVROAD *c,int i)
{
	int n;
	float dist;
	struct INTPT *p1,*p2;

	n=col.vroadHead.nrec;
	p1=&(c[i].refPt); p2=&(c[(i+1)%n].refPt);
	dist=(float)sqrt((float)(p2->x-p1->x)*(float)(p2->x-p1->x)+(float)(p2->y-p1->y)*(float)(p2->y-p1->y));
	res.x=(p1->x+org.x*(p2->x-p1->x)+org.y*(p2->y-p1->y)/dist)/65536;
	res.y=(p1->y+org.x*(p2->y-p1->y)-org.y*(p2->x-p1->x)/dist)/65536;
	res.z=(org.z+p1->z+org.x*(p2->z-p1->z))/65536;
}

void CT3EDDoc::UpdateColVroadVecs(int i)
{
	
	struct INTPT *p1,*p2;
	float distxy,dist;

	
	p1=&(col.vroad[i].refPt);
	p2=&(col.vroad[i?(i-1):(col.vroadHead.nrec-1)].refPt);
	distxy=(float)(p1->x-p2->x)*(float)(p1->x-p2->x)+
		   (float)(p1->y-p2->y)*(float)(p1->y-p2->y);
	dist=(float)sqrt(distxy+(float)(p1->z-p2->z)*(float)(p1->z-p2->z));
	distxy=(float)sqrt(distxy);
	col.vroad[i].forward.x=(signed char)((p1->x-p2->x)*127/dist);
	col.vroad[i].forward.y=(signed char)((p1->y-p2->y)*127/dist);
	col.vroad[i].forward.z=(signed char)((p1->z-p2->z)*127/dist);
	col.vroad[i].right.x=(signed char)((p1->y-p2->y)*127/distxy);
	col.vroad[i].right.y=(signed char)(-(p1->x-p2->x)*127/distxy);
	col.vroad[i].right.z=0;
	col.vroad[i].normal.x=(signed char)(-((p1->z-p2->z)/dist)*(p1->x-p2->x)*127/distxy);
	col.vroad[i].normal.y=(signed char)(-((p1->z-p2->z)/dist)*(p1->y-p2->y)*127/distxy);
	col.vroad[i].normal.z=(signed char)(distxy*127/dist);
}

void CT3EDDoc::MoveVertice(FLOATPT &ver, float dx, float dy, float dz)
{
	ver.x+=dx;
	ver.y+=dy;
	ver.z+=dz;
}

//Expand a Vertice from block centre
void CT3EDDoc::ExpandVertice(int blk, FLOATPT &ver, float xmulti, float ymulti, float zmulti)
{
	struct TRKBLOCK *t;
	float xdist, ydist, zdist;
	t=&(trk[blk]);
	xdist=(ver.x - t->ptCentre.x)*xmulti;
	ydist=(ver.y - t->ptCentre.y)*ymulti;
	zdist=(ver.z - t->ptCentre.z)*zmulti;

	ver.x=t->ptCentre.x + xdist;
	ver.y=t->ptCentre.y + ydist;
	ver.z=t->ptCentre.z + zdist;
}

void CT3EDDoc::ExpandINTPT(int blk, INTPT &pt, float xmulti, float ymulti, float zmulti)
{
	FLOATPT ver;

	ver.x=((float)pt.x/65536);
	ver.y=((float)pt.y/65536);
	ver.z=((float)pt.z/65536);

	ExpandVertice(blk,ver, xmulti, ymulti, zmulti);

	pt.x=(long)(ver.x*65536);
	pt.y=(long)(ver.y*65536);
	pt.z=(long)(ver.z*65536);

}

void CT3EDDoc::RotateVertice(int blk,FLOATPT &ver, double angledegree)
{
	double pi = 3.14159265358979323846264338;
	double rangle, AcAngle, NewAngle, Length;
	struct TRKBLOCK *t;
	t=&(trk[blk]);

	rangle=((angledegree/180) * pi);
	AcAngle = atan2((ver.y - t->ptCentre.y),(ver.x - t->ptCentre.x));// Actual Angle of point;
	NewAngle = (AcAngle - rangle); // Rotate clockwise
	Length=sqrt((ver.y - t->ptCentre.y)*(ver.y - t->ptCentre.y) + (ver.x - t->ptCentre.x)*(ver.x - t->ptCentre.x)); //Length to the middle of object
	ver.x=(float)(t->ptCentre.x + (cos(NewAngle))*Length);// New X-Position
	ver.y=(float)(t->ptCentre.y + (sin(NewAngle))*Length);// New Y-Position
}

void CT3EDDoc::RotateINTPT(int blk, INTPT &pt, double angledegree)
{
	FLOATPT ver;

	ver.x=((float)pt.x/65536);
	ver.y=((float)pt.y/65536);
	ver.z=((float)pt.z/65536);

	RotateVertice(blk,ver,angledegree);

	pt.x=(long)(ver.x*65536);
	pt.y=(long)(ver.y*65536);
	pt.z=(long)(ver.z*65536);

}

void CT3EDDoc::MoveINTPT(INTPT &pt, float dx, float dy, float dz)
{
	pt.x+=(long)(dx*65536);
	pt.y+=(long)(dy*65536);
	pt.z+=(long)(dz*65536);
}

void CT3EDDoc::ExpandBlock(int blk, float lenght, float width, float height)
{
	int n,i,k,oldundo,start,end;
	double AngleOld,AngleNew;
	double pi = 3.14159265358979323846264338;
	struct INTPT ipt1,ipt2;
	struct TRKBLOCK *t;
	struct XOBJDATA *x;
	struct LIGHTSRC *light;
	struct SOUNDSRC *sound;
	struct COLVROAD *cvr;
	t=&(trk[blk]);

	if (undoLevel>0)
	{
		//New undo for vroad
		n=col.vroadHead.nrec;
		undoCol[undoLevel-1]=(struct COLVROAD *)malloc(36*n);
		memcpy(undoCol[undoLevel-1],col.vroad,36*n);

		PrepareModifyTrk(blk);
		PrepareModifyPoly(blk);
		for (i=0;i<4;i++) PrepareModifyXobj(4*blk+i);
	}
	oldundo=undoLevel;
	undoLevel=0; //All undoinformation already stored

	ipt1=col.vroad[trk[blk].nStartPos].refPt; //First vroad point
	ipt2=col.vroad[trk[blk].nStartPos+trk[blk].nPositions-1].refPt; //Last vroad point
	AngleOld = (180 * atan2((ipt1.y - ipt2.y),(ipt1.x - ipt2.x)))/pi;// Actual Angle of old vroad;
	AngleNew = AngleOld - 90;
	RotateBlock(blk,AngleNew); //Rotate block that y=length & x=width

	//Expand the vroad
	start=trk[blk].nStartPos;
	end=trk[blk].nStartPos+trk[blk].nPositions;
	for (i=start;i<end;i++)
	{
		cvr=&col.vroad[i];
		ExpandINTPT(blk,cvr->refPt, width, lenght, height);
		//RotateINTPT(blk,cvr->refPt,angledegree);
	}
	n=col.vroadHead.nrec;
	for (i=0;i<n;i++) UpdateColVroadVecs(i); //Update the vectors

	//Expand Track Vertices
	for (i=0;i<t->nVertices;i++)
		ExpandVertice(blk,t->vert[i],width, lenght, height);
	for (i=0;i<4;i++)
		ExpandVertice(blk,t->ptBounding[i],width, lenght, height);
	ExpandVertice(blk,t->hs_ptMax,width, lenght, height);
	ExpandVertice(blk,t->hs_ptMin,width, lenght, height);

	//Expand the Extra Objects
	for (i=0;i<4;i++) //Chunk 0 to 3
	{
		if (xobj[4*blk+i].nobj>0) //Extra Objects there ?
		{
			PrepareModifyXobj(4*blk+i);	
			for (k=0;k<xobj[4*blk+i].nobj;k++) //Loop for all
			{
				x=&(xobj[4*blk+i].obj[k]);

				ExpandVertice(blk,x->ptRef,width, lenght, height); //rotate the refpoint

				MemObjMiddle.x=x->ptRef.x;
				MemObjMiddle.y=x->ptRef.y;
				MemObjMiddle.z=x->ptRef.z;
				ExpandObject(blk,1,i,k,width, lenght, height);
			}
		}
	}

	//Expand light sources, type 2 objects
	light=trk[blk].lightsrc;
	for (i=0;i<trk[blk].nLightsrc;i++,light++)
		{
		ExpandINTPT(blk,light->refpoint, width, lenght, height);
		}

	//Rotate sound sources, type 3 objects
	sound=trk[blk].soundsrc;
	for (i=0;i<trk[blk].nSoundsrc;i++,sound++)
		{
		ExpandINTPT(blk,sound->refpoint, width, lenght, height);
		}
	
	AngleNew = 90 - AngleOld; //Move back in old direction
	RotateBlock(blk,AngleNew);
	undoLevel=oldundo; // Re-enable undo levels
}

void CT3EDDoc::RotateBlock(int blk, double angledegree)
{
	int i,j,k,n,start,end;
	struct FLOATPT tmppt;
	struct INTPT *p1,*p2;
	struct XOBJDATA *x;
	struct TRKBLOCK *t;
	struct LIGHTSRC *light;
	struct SOUNDSRC *sound;
	struct COLVROAD *cvr;
	//FLOATPT v;
	t=&(trk[blk]);

	n=col.vroadHead.nrec;
	if (undoLevel>0)
	{
		//New undo for vroad
		undoCol[undoLevel-1]=(struct COLVROAD *)malloc(36*n);
		memcpy(undoCol[undoLevel-1],col.vroad,36*n);
	}

	//Rotate the vroad
	start=trk[blk].nStartPos;
	end=trk[blk].nStartPos+trk[blk].nPositions;
	for (i=start;i<end;i++)
	{
		cvr=&col.vroad[i];
		RotateINTPT(blk,cvr->refPt,angledegree);
	}
	for (i=0;i<n;i++) UpdateColVroadVecs(i); //Update the vectors

	PrepareModifyTrk(blk);
	//Rotate the track vertices around the center
	for (i=0;i<t->nVertices;i++)
		RotateVertice(blk,t->vert[i],angledegree);
	for (i=0;i<4;i++)
		RotateVertice(blk,t->ptBounding[i],angledegree);
	RotateVertice(blk,t->hs_ptMax,angledegree);
	RotateVertice(blk,t->hs_ptMin,angledegree);

	//Rotate the Extra Objects
	for (i=0;i<4;i++) //Chunk 0 to 3
	{
		if (xobj[4*blk+i].nobj>0) //Extra Objects there ?
		{
			PrepareModifyXobj(4*blk+i);	
			for (k=0;k<xobj[4*blk+i].nobj;k++) //Loop for all
			{
				x=&(xobj[4*blk+i].obj[k]);

				RotateVertice(blk,x->ptRef,angledegree); //rotate the refpoint

				MemObjMiddle.x=x->ptRef.x;
				MemObjMiddle.y=x->ptRef.y;
				MemObjMiddle.z=x->ptRef.z;
				RotateObject(blk,1,i,k,angledegree);
			}
		}
	}
	//Rotate light sources, type 2 objects
	light=trk[blk].lightsrc;
	for (i=0;i<trk[blk].nLightsrc;i++,light++)
		{
		RotateINTPT(blk,light->refpoint, angledegree);
		}

	//Rotate sound sources, type 3 objects
	sound=trk[blk].soundsrc;
	for (i=0;i<trk[blk].nSoundsrc;i++,sound++)
		{
		RotateINTPT(blk,sound->refpoint, angledegree);
		}

	//Correct PolyVroad
	p1=&(col.vroad[trk[blk].nStartPos].refPt);
	p2=&(col.vroad[trk[blk].nStartPos+trk[blk].nPositions-1].refPt);
	tmppt.x=(float)(p2->x-p1->x);
	tmppt.z=(float)(p2->z-p1->z);
	tmppt.y=(float)(p2->y-p1->y);
	for (j=0;j<trk[i].nPolygons;j++) RecalcPolyVroad(i,j,&tmppt);

}

void CT3EDDoc::MoveBlockSimple(int blk, float dx, float dy, float dz)
{
	int start,end,i,k,n;
	struct TRKBLOCK *t;
	//struct LIGHTSRC *light;
	//struct SOUNDSRC *sound;
	//struct FLOATPT *v;
	struct COLVROAD *cvr;
	t=&(trk[blk]);

	if (undoLevel>0)
	{
		//New undo for vroad
		n=col.vroadHead.nrec;
		undoCol[undoLevel-1]=(struct COLVROAD *)malloc(36*n);
		memcpy(undoCol[undoLevel-1],col.vroad,36*n);
	}

	start=trk[blk].nStartPos;
	end=trk[blk].nStartPos+trk[blk].nPositions;
	for (i=start;i<end;i++)
	{
		cvr=&col.vroad[i];
		MoveINTPT(cvr->refPt,dx,dy,dz);
	}

	PrepareModifyTrk(blk);
	//Move the track vertices
	MoveVertice(t->ptCentre,dx,dy,dz);
	for (i=0;i<t->nVertices;i++)
		MoveVertice(t->vert[i],dx,dy,dz);
	for (i=0;i<4;i++)
		MoveVertice(t->ptBounding[i],dx,dy,dz);
	MoveVertice(t->hs_ptMax,dx,dy,dz);
	MoveVertice(t->hs_ptMin,dx,dy,dz);

	//Move the Extra Objects
	for (i=0;i<4;i++) //Chunk 0 to 3
	{
		if (xobj[4*blk+i].nobj>0) //Extra Objects there ?
			for (k=0;k<xobj[4*blk+i].nobj;k++) //Loop for all
				MoveObjectBy(blk,1,i,k,dx,dy,dz);
	}
	//Move light sources, type 2 objects
	for (i=0;i<trk[blk].nLightsrc;i++)
		MoveObjectBy(blk,2,0,i,dx,dy,dz);
	//Move sound sources, type 3 objects
	for (i=0;i<trk[blk].nSoundsrc;i++)
		MoveObjectBy(blk,3,0,i,dx,dy,dz);
}

// if blk<0, then clear all track ; if sDist!=0 set XY circle, if sWidth!=0 set Z=0
void CT3EDDoc::MoveBlocks(int blk,float dx,float dy,float dz,int sDist,int sWidth,int extraSmooth)
{
	int start=0,minblk,maxblk,i,j,k,n,ref,min2,max2;
	struct INTPT *p1,*p2;
	float coeff,t;
	struct FLOATPT tmppt,tpt,*v;
	struct FLOATPT interp[5];
	struct LIGHTSRC *light;
	struct SOUNDSRC *sound;
	struct TRKBLOCK *trkbl;
	trkbl=&(trk[blk]);

	n=col.vroadHead.nrec;
	undoCol[undoLevel-1]=(struct COLVROAD *)malloc(36*n);
	memcpy(undoCol[undoLevel-1],col.vroad,36*n);

	if (extraSmooth==3) {
		// Only remap vertices to block
		for (i=0;i<n;i++) UpdateColVroadVecs(i);
		minblk=trkbl->nStartPos;
		maxblk=minblk + trkbl->nPositions;
	}
	if (blk==-1) {
		// new V0.06 clear-all function
		for (i=0;i<n;i++) {
			if (sDist!=0) {
				col.vroad[i].refPt.x=(int)(65536*n*cos(2*i*3.14159265359/n));
				col.vroad[i].refPt.y=(int)(65536*n*sin(2*i*3.14159265359/n));
			}
			if (sWidth!=0) col.vroad[i].refPt.z=0;
		}
		for (i=0;i<n;i++) UpdateColVroadVecs(i);
		minblk=0; maxblk=n-1;
	}

	if (extraSmooth==2) {
		minblk=trkbl->nStartPos;
		maxblk=start + trkbl->nPositions;

		for (i=minblk;i<maxblk;i++)
		{
			//col.vroad[i].refPt.x=(int)(65536*n*cos(2*i*3.14159265359/n));
			col.vroad[i].refPt.x=(int)(col.vroad[i].refPt.x + dx);
			col.vroad[i].refPt.y=(int)(col.vroad[i].refPt.y + dy);
			col.vroad[i].refPt.z=(int)(col.vroad[i].refPt.z + dz);
			//UpdateColVroadVecs(i);
		}
		for (i=minblk;i<maxblk;i++) UpdateColVroadVecs(i);
		RecalcBoundingBox(blk);
	}
	if (extraSmooth==1) {
		// new V0.02 deformation technique
		i=trk[blk].nStartPos+trk[blk].nPositions/2+sWidth;
		maxblk=(i+8)%n;
		i=i%n;
		interp[4].x=(float)col.vroad[i].refPt.x;
		interp[4].y=(float)col.vroad[i].refPt.y;
		interp[4].z=(float)col.vroad[i].refPt.z;
		interp[3].x=sWidth*(float)(col.vroad[i].refPt.x-col.vroad[(i+n-1)%n].refPt.x);
		interp[3].y=sWidth*(float)(col.vroad[i].refPt.y-col.vroad[(i+n-1)%n].refPt.y);
		interp[3].z=sWidth*(float)(col.vroad[i].refPt.z-col.vroad[(i+n-1)%n].refPt.z);
		i-=sWidth;
		if (i<0) i+=n;
		interp[2].x=(float)col.vroad[i].refPt.x+65536*dx;
		interp[2].y=(float)col.vroad[i].refPt.y+65536*dy;
		interp[2].z=(float)col.vroad[i].refPt.z+65536*dz;
		i-=sWidth;
		if (i<0) i+=n;
		minblk=(i+n-8)%n;
		interp[0].x=(float)col.vroad[i].refPt.x;
		interp[0].y=(float)col.vroad[i].refPt.y;
		interp[0].z=(float)col.vroad[i].refPt.z;
		interp[1].x=sWidth*(float)(col.vroad[(i+1)%n].refPt.x-col.vroad[i].refPt.x);
		interp[1].y=sWidth*(float)(col.vroad[(i+1)%n].refPt.y-col.vroad[i].refPt.y);
		interp[1].z=sWidth*(float)(col.vroad[(i+1)%n].refPt.z-col.vroad[i].refPt.z);
	
		for (j=-sWidth;j<=sWidth;j++) {
			t=((float)j)/sWidth;
			p1=&(col.vroad[i].refPt);
			if ((dx!=0)||(dy!=0)) {
				p1->x=(long)(interp[0].x*(1-t)*(t-1)*t*(2*t+3)/4+
							 interp[1].x*(1-t)*(t-1)*t*(t+1)/4+
							 interp[2].x*(1-t)*(1+t)*(1-t)*(1+t)+
							 interp[3].x*(t+1)*(t+1)*t*(t-1)/4+
							 interp[4].x*(t+1)*(t+1)*t*(3-2*t)/4);
				p1->y=(long)(interp[0].y*(1-t)*(t-1)*t*(2*t+3)/4+
							 interp[1].y*(1-t)*(t-1)*t*(t+1)/4+
							 interp[2].y*(1-t)*(1+t)*(1-t)*(1+t)+
							 interp[3].y*(t+1)*(t+1)*t*(t-1)/4+
							 interp[4].y*(t+1)*(t+1)*t*(3-2*t)/4);
			}
			if (dz!=0)
				p1->z=(long)(interp[0].z*(1-t)*(t-1)*t*(2*t+3)/4+
							 interp[1].z*(1-t)*(t-1)*t*(t+1)/4+
							 interp[2].z*(1-t)*(1+t)*(1-t)*(1+t)+
							 interp[3].z*(t+1)*(t+1)*t*(t-1)/4+
							 interp[4].z*(t+1)*(t+1)*t*(3-2*t)/4);
			UpdateColVroadVecs(i);
			if (++i==n) i=0;
		}
	} 
	if (extraSmooth==0) 
	{
		// V0.01 deformation technique
		i=trk[blk].nStartPos+(trk[blk].nPositions-sDist)/2-sWidth;
		while (i<0) i+=n;
		minblk=i;
		// construct the new vroad
		for (j=sWidth;j>=0;j--) {
			coeff=(float)(0.5+0.5*cos(3.1415926*j/sWidth));
			p1=&(col.vroad[i].refPt);
			p1->x+=(long)(dx*coeff*65536);
			p1->y+=(long)(dy*coeff*65536);
			p1->z+=(long)(dz*coeff*65536);
			UpdateColVroadVecs(i);
			if (++i==n) i=0;
		}
		for (j=1;j<sDist;j++) {
			p1=&(col.vroad[i].refPt);
			p1->x+=(long)(dx*65536);
			p1->y+=(long)(dy*65536);
			p1->z+=(long)(dz*65536);
			if (++i==n) i=0;
		}
		for (j=1;j<=sWidth;j++) {
			coeff=(float)(0.5+0.5*cos(3.1415926*j/sWidth));
			p1=&(col.vroad[i].refPt);
			p1->x+=(long)(dx*coeff*65536);
			p1->y+=(long)(dy*coeff*65536);
			p1->z+=(long)(dz*coeff*65536);
			UpdateColVroadVecs(i);
			if (++i==n) i=0;
		}
		maxblk=i;
	}

	// remap the vertices onto the vroad
	i=0;
	while (trk[i].nStartPos+trk[i].nPositions<=minblk) i++;
	while (TRUE) {
		PrepareModifyTrk(i);
		v=trk[i].vert;
		start=trk[i].nStartPos+trk[i].nPositions/2;
		min2=(trk[i].nStartPos+n-8)%n;
		max2=(trk[i].nStartPos+trk[i].nPositions+7)%n;
		ref=GlobalLocalCoord(trk[i].ptCentre,tpt,undoCol[undoLevel-1],start,min2,max2);
		LocalGlobalCoord(tpt,trk[i].ptCentre,col.vroad,ref);
		for (j=0;j<trk[i].nVertices;j++,v++) {
			if (fabs(v->x)>1E7) continue;
			if (fabs(v->y)>1E7) continue;
			ref=GlobalLocalCoord(*v,tpt,undoCol[undoLevel-1],start,min2,max2);
			LocalGlobalCoord(tpt,*v,col.vroad,ref);
		}
		for (j=0,light=trk[i].lightsrc;j<trk[i].nLightsrc;j++,light++) {
			tmppt.x=(float)(light->refpoint.x/65536);
			tmppt.y=(float)(light->refpoint.y/65536);
			tmppt.z=(float)(light->refpoint.z/65536);
			ref=GlobalLocalCoord(tmppt,tpt,undoCol[undoLevel-1],start,min2,max2);
			LocalGlobalCoord(tpt,tmppt,col.vroad,ref);
			light->refpoint.x=(long)(65536*tmppt.x);
			light->refpoint.y=(long)(65536*tmppt.y);
			light->refpoint.z=(long)(65536*tmppt.z);
		}
		for (j=0,sound=trk[i].soundsrc;j<trk[i].nSoundsrc;j++,sound++) {
			tmppt.x=(float)(sound->refpoint.x/65536);
			tmppt.y=(float)(sound->refpoint.y/65536);
			tmppt.z=(float)(sound->refpoint.z/65536);
			ref=GlobalLocalCoord(tmppt,tpt,undoCol[undoLevel-1],start,min2,max2);
			LocalGlobalCoord(tpt,tmppt,col.vroad,ref);
			sound->refpoint.x=(long)(65536*tmppt.x);
			sound->refpoint.y=(long)(65536*tmppt.y);
			sound->refpoint.z=(long)(65536*tmppt.z);
		}
		p1=&(col.vroad[trk[i].nStartPos].refPt);
		p2=&(col.vroad[trk[i].nStartPos+trk[i].nPositions-1].refPt);
		tmppt.x=(float)(p2->x-p1->x);
		tmppt.z=(float)(p2->z-p1->z);
		tmppt.y=(float)(p2->y-p1->y);
		for (j=0;j<trk[i].nPolygons;j++)
			RecalcPolyVroad(i,j,&tmppt);
		for (j=4*i;j<4*i+4;j++) if (xobj[j].nobj) {
			PrepareModifyXobj(j);
			for (k=0;k<xobj[j].nobj;k++) {
				ref=GlobalLocalCoord(xobj[j].obj[k].ptRef,tpt,undoCol[undoLevel-1],start,min2,max2);
				LocalGlobalCoord(tpt,xobj[j].obj[k].ptRef,col.vroad,ref);
			}
		}
		for (j=0;j<trk[i].nXobj;j++) {
			tmppt.x=(float)(trk[i].xobj[j].pt.x/65536.0);
			tmppt.z=(float)(trk[i].xobj[j].pt.z/65536.0);
			tmppt.y=(float)(trk[i].xobj[j].pt.y/65536.0);
			ref=GlobalLocalCoord(tmppt,tpt,undoCol[undoLevel-1],start,min2,max2);
			LocalGlobalCoord(tpt,tmppt,col.vroad,ref);
			trk[i].xobj[j].pt.x=(long)(tmppt.x*65536);
			trk[i].xobj[j].pt.z=(long)(tmppt.z*65536);
			trk[i].xobj[j].pt.y=(long)(tmppt.y*65536);
		}
		RecalcBoundingBox(i);
		if ((trk[i].nStartPos<=maxblk)&&
			(trk[i].nStartPos+trk[i].nPositions>maxblk)) break;
		i++;
		if (i==nBlocks) i=0;
	}
}

void CT3EDDoc::DelObject(int blk,int isxobj,int chunk,int no)
{
	struct XOBJBLOCK *xb;
	int i,j,k,l,n,ii,jj,nn,oldno,oldpos,shiftby;
	short vno;
	struct OBJPOLYBLOCK *o,*oo;
	struct TRKBLOCK *t;
	LPPOLYGONDATA p,pp;

	PrepareModifyTrk(blk);
	PrepareModifyPoly(blk);
	//Nappe1: Deleting Sound Source
	if (isxobj==3) {
		t=trk+blk;
		t->nSoundsrc--;
		for (i=no;i<t->nSoundsrc;i++)
			memcpy(t->soundsrc+i,t->soundsrc+i+1,sizeof(struct SOUNDSRC));
	} else
	// Nappe1: ADDITION ENDS.
	if (isxobj==2) {
		t=trk+blk;
		t->nLightsrc--;
		for (i=no;i<t->nLightsrc;i++)
			memcpy(t->lightsrc+i,t->lightsrc+i+1,sizeof(struct LIGHTSRC));
	} else
	if (isxobj==1) {
		for (i=0;i<4;i++) PrepareModifyXobj(4*blk+i);
		// update xobj blocks
		xb=&(xobj[4*blk+chunk]);
		oldno=xb->obj[no].crossno;
		nn=xb->obj[no].crosstype;
		if (nn==3) dofree(xb->obj[no].animData);
		dofree(xb->obj[no].vert);
		dofree(xb->obj[no].unknVertices);
		dofree(xb->obj[no].polyData);
		xb->nobj--;
		for (i=no;i<xb->nobj;i++)
			memcpy(&(xb->obj[i]),&(xb->obj[i+1]),sizeof(struct XOBJDATA));
		if (nn==4)
			for (i=4*blk;i<4*blk+4;i++)
				for (j=0;j<xobj[i].nobj;j++)
					if (xobj[i].obj[j].crossno>oldno)
						xobj[i].obj[j].crossno--;
		// update polyobj blocks
		o=&(poly[blk].obj[chunk]);
		if (o->n1>0) {
			oldpos=-1; j=-1;
			while (j<no) if (o->types[++oldpos]!=1) j++;
			o->n2--;
			for (i=oldpos;i<o->n2;i++) o->types[i]=o->types[i+1];
		}
		// update trk blocks
		t=&(trk[blk]);
		if (nn==4) {
			t->nXobj--;
			for (i=oldno;i<t->nXobj;i++) {
				memcpy(&(t->xobj[i]),&(t->xobj[i+1]),sizeof(struct REFXOBJ));
				if ((chunk==0)&&(t->xobj[i].crossindex!=0)) 
					t->xobj[i].crossindex--;
			}
		}
		// update global xobj sequence numbers
		n=0;
		for (i=0;i<nBlocks;i++)
			for (j=0;j<trk[i].nXobj;j++)
				trk[i].xobj[j].globalno=n++;
	} else {
		// update vertices
		t=&(trk[blk]);
		o=&(poly[blk].obj[chunk]);
		n=o->numpoly[no];
		p=o->poly[no];
		shiftby=0;
		for (i=0;i<n;i++) for (j=0;j<4;j++) {
			vno=p[i].vertex[j];
			if (vno<0) continue;
			shiftby++;
			for (k=i;k<n;k++) for (l=0;l<4;l++)
				if (p[k].vertex[l]==vno) p[k].vertex[l]=-1;
			oo=poly[blk].obj;
			for (ii=0;ii<4;ii++,oo++) 
				for (jj=0;jj<oo->nobj;jj++) {
					nn=oo->numpoly[jj];
					pp=oo->poly[jj];
					for (k=0;k<nn;k++,pp++) for (l=0;l<4;l++) {
						ASSERT(pp->vertex[l]!=vno);
						if (pp->vertex[l]>vno) pp->vertex[l]--;
					}
				}
			t->nVertices--; t->nVerticesDup--;
			t->nObjectVert--;
			t->nLoResVert--; t->nHiResVert--; t->nMedResVert--;
			memmove(t->vert+vno,t->vert+vno+1,12*(t->nVertices-vno));
			memmove(t->unknVertices+vno,t->unknVertices+vno+1,4*(t->nVertices-vno));
		}
		for (i=0;i<=6;i++) {
			nn=poly[blk].sz[i];
			pp=poly[blk].poly[i];
			for (j=0;j<nn;j++,pp++) for (k=0;k<4;k++)
				pp->vertex[k]-=shiftby;
		}
		// update objpolyblock
		o->n1-=n;
		o->n2--;
		o->nobj--;
		dofree(o->poly[no]);
		for (i=no;i<o->nobj;i++) {
			o->numpoly[i]=o->numpoly[i+1];
			o->poly[i]=o->poly[i+1];
		}
		oldpos=-1; j=-1;
		while (j<no) if (o->types[++oldpos]==1) j++;
		for (i=oldpos;i<o->n2;i++) o->types[i]=o->types[i+1];
	}
	RecalcBoundingBox(blk);
}

void CT3EDDoc::DuplObject(int blk,int isxobj,int chunk,int no)
{
	struct XOBJDATA *x;
	struct XOBJBLOCK *xb;
	int i,j,k,l,n,nn,oldno,shiftby;
	short vno,vno2;
	struct OBJPOLYBLOCK *o;
	struct TRKBLOCK *t;
	LPPOLYGONDATA p,pp;

	PrepareModifyTrk(blk);
	PrepareModifyPoly(blk);
	//Nappe1: Copying Sound sources
	if (isxobj==3) {
		t=trk+blk;
		t->nSoundsrc++;
		t->soundsrc=(struct SOUNDSRC *)realloc(t->soundsrc,t->nSoundsrc*sizeof(struct SOUNDSRC));
		//memcpy(t->lightsrc+t->nLightsrc-1,t->lightsrc+no,sizeof(struct LIGHTSRC));
		memcpy(t->soundsrc+t->nSoundsrc-1,t->soundsrc+no,sizeof(struct SOUNDSRC));
		//InvalidateRect(NULL, false);
	} else
	
	// ADDITION ENDS.
	if (isxobj==2) {
		t=trk+blk;
		t->nLightsrc++;
		t->lightsrc=(struct LIGHTSRC *)realloc(t->lightsrc,t->nLightsrc*sizeof(struct LIGHTSRC));
		memcpy(t->lightsrc+t->nLightsrc-1,t->lightsrc+no,sizeof(struct LIGHTSRC));
	} else
	if (isxobj==1) {
		PrepareModifyXobj(4*blk+chunk);
		// update xobj blocks
		xb=&(xobj[4*blk+chunk]);
		xb->nobj++;
		xb->obj=(struct XOBJDATA *)realloc(xb->obj,xb->nobj*sizeof(struct XOBJDATA));
		x=&(xb->obj[xb->nobj-1]);
		memcpy(x,&(xb->obj[no]),sizeof(struct XOBJDATA));
		x->vert=(struct FLOATPT *)malloc(12*x->nVertices);
		memcpy(x->vert,xb->obj[no].vert,12*x->nVertices);
		x->unknVertices=(long *)malloc(4*x->nVertices);
		memcpy(x->unknVertices,xb->obj[no].unknVertices,4*x->nVertices);
		x->polyData=(LPPOLYGONDATA)malloc(14*x->nPolygons);
		memcpy(x->polyData,xb->obj[no].polyData,14*x->nPolygons);
		if (x->crosstype==3) {
			x->animData=(struct ANIMDATA *)malloc(x->nAnimLength*sizeof(struct ANIMDATA));
			memcpy(x->animData,xb->obj[no].animData,x->nAnimLength*sizeof(struct ANIMDATA));
		}
		oldno=x->crossno;
		x->crossno=trk[blk].nXobj;
		// update polyobj blocks
		o=&(poly[blk].obj[chunk]);
		if (o->n1>0) {
			o->n2++;
			o->types=(long *)realloc(o->types,4*o->n2);
			o->types[o->n2-1]=x->crosstype;
		}
		// update trk blocks
		t=&(trk[blk]);
		if (x->crosstype==4) {
			t->nXobj++;
			t->xobj=(struct REFXOBJ *)realloc(t->xobj,t->nXobj*sizeof(struct REFXOBJ));
			memcpy(&(t->xobj[t->nXobj-1]),t->xobj+oldno,sizeof(struct REFXOBJ));
			if (chunk==0) t->xobj[t->nXobj-1].crossindex=(char)t->nPolyobj;
		}
		// update global xobj sequence numbers
		n=0;
		for (i=0;i<nBlocks;i++)
			for (j=0;j<trk[i].nXobj;j++)
				trk[i].xobj[j].globalno=n++;
	} else {
		// update vertices
		t=&(trk[blk]);
		o=&(poly[blk].obj[chunk]);
		n=o->numpoly[no];
		p=(LPPOLYGONDATA)malloc(n*sizeof(struct POLYGONDATA));
		memcpy(p,o->poly[no],n*sizeof(struct POLYGONDATA));
		shiftby=0;
		for (i=0;i<n;i++) for (j=0;j<4;j++) {
			vno=p[i].vertex[j];
			if (vno<0) continue;
			shiftby++;
			vno2=(short)t->nObjectVert;
			t->nVertices++; t->nVerticesDup++;
			t->nObjectVert++;
			t->nLoResVert++; t->nHiResVert++; t->nMedResVert++;
			t->vert=(struct FLOATPT *)realloc(t->vert,t->nVertices*12);
			t->unknVertices=(unsigned long *)realloc(t->unknVertices,t->nVertices*4);
			memmove(t->vert+vno2+1,t->vert+vno2,12*(t->nVertices-vno2-1));
			memmove(t->unknVertices+vno2+1,t->unknVertices+vno2,4*(t->nVertices-vno2-1));
			t->unknVertices[vno2]=t->unknVertices[vno];
			memcpy(t->vert+vno2,t->vert+vno,12);
			for (k=i;k<n;k++) for (l=0;l<4;l++)
				if (p[k].vertex[l]==vno) {
					p[k].vertex[l]=-vno2-1;
				}
		}
		for (i=0;i<n;i++) for (j=0;j<4;j++)
			p[i].vertex[j]=-1-p[i].vertex[j];
		// p and n must be preserved for below
		for (i=0;i<=6;i++) {
			nn=poly[blk].sz[i];
			pp=poly[blk].poly[i];
			for (j=0;j<nn;j++,pp++) for (k=0;k<4;k++)
				pp->vertex[k]+=shiftby;
		}
		// update objpolyblock
		o->n1+=n;
		o->n2++;
		o->nobj++;
		o->types=(long *)realloc(o->types,4*o->n2);
		o->types[o->n2-1]=1;
		o->numpoly=(long *)realloc(o->numpoly,4*o->nobj);
		o->numpoly[o->nobj-1]=n;
		o->poly=(LPPOLYGONDATA *)realloc(o->poly,4*o->nobj);
		o->poly[o->nobj-1]=p;
	}
	RecalcBoundingBox(blk);
}

void CT3EDDoc::CopyObjectTo(int srcblk, int srcisxobj, int srcchunk, int srcno, int dstblk, int dstchunk)
{
	struct XOBJDATA *dstx,*srcx;
	struct XOBJBLOCK *dstxb,*srcxb;
	int i,j,n,oldno;
	//short vno,vno2;
	struct OBJPOLYBLOCK *o;
	struct TRKBLOCK *dstt, *srct;
	//LPPOLYGONDATA p,pp;

	PrepareModifyTrk(dstblk);
	PrepareModifyPoly(dstblk);

	if (srcisxobj==1) 
	{
		PrepareModifyXobj(4*dstblk+dstchunk);
		// update xobj blocks
		srcxb=&(xobj[4*srcblk+srcchunk]);
		dstxb=&(xobj[4*dstblk+dstchunk]);
		dstxb->nobj++;
		dstxb->obj=(struct XOBJDATA *)realloc(dstxb->obj,dstxb->nobj*sizeof(struct XOBJDATA));
		srcx=&(srcxb->obj[srcno]);
		dstx=&(dstxb->obj[dstxb->nobj-1]);
		memcpy(dstx,&(srcxb->obj[srcno]),sizeof(struct XOBJDATA));
		dstx->vert=(struct FLOATPT *)malloc(12*dstx->nVertices);
		memcpy(dstx->vert,srcxb->obj[srcno].vert,12*dstx->nVertices);
		dstx->unknVertices=(long *)malloc(4*dstx->nVertices);
		memcpy(dstx->unknVertices,srcxb->obj[srcno].unknVertices,4*dstx->nVertices);
		dstx->polyData=(LPPOLYGONDATA)malloc(14*dstx->nPolygons);
		memcpy(dstx->polyData,srcxb->obj[srcno].polyData,14*dstx->nPolygons);
		if (dstx->crosstype==3) {
			dstx->animData=(struct ANIMDATA *)malloc(dstx->nAnimLength*sizeof(struct ANIMDATA));
			memcpy(dstx->animData,srcxb->obj[srcno].animData,dstx->nAnimLength*sizeof(struct ANIMDATA));
		}
		oldno=srcx->crossno;
		dstx->crossno=trk[dstblk].nXobj;
		// update polyobj blocks
		o=&(poly[dstblk].obj[dstchunk]);
		if (o->n1>0) {
			o->n2++;
			o->types=(long *)realloc(o->types,4*o->n2);
			o->types[o->n2-1]=dstx->crosstype;
		}
		// update trk blocks
		srct=&(trk[srcblk]);
		dstt=&(trk[dstblk]);
		if (dstx->crosstype==4) {
			dstt->nXobj++;
			dstt->xobj=(struct REFXOBJ *)realloc(dstt->xobj,dstt->nXobj*sizeof(struct REFXOBJ));
			memcpy(&(dstt->xobj[dstt->nXobj-1]),srct->xobj+oldno,sizeof(struct REFXOBJ));
			if (dstchunk==0) dstt->xobj[dstt->nXobj-1].crossindex=(char)dstt->nPolyobj;
		}
		// update global xobj sequence numbers
		n=0;
		for (i=0;i<nBlocks;i++)
			for (j=0;j<trk[i].nXobj;j++)
				trk[i].xobj[j].globalno=n++;
	}
}

void CT3EDDoc::NewObject(int blk,int isxobj,int chunk,int texture,short flags,int xobjcrosstype)
{
	struct XOBJDATA *x;
	struct XOBJBLOCK *xb;
	int i,j,k,n,nn;
	struct OBJPOLYBLOCK *o;
	struct TRKBLOCK *t;
	struct REFXOBJ *rx;
	LPPOLYGONDATA p,pp;
	short data1[17], data2[17];
	char  cdata1[] = "-822 2385 5501 8406 10988 13148 14802 15888 16363 16209 15432 14062 12152 9775 7022 3999 822";
	char  cdata2[] = "16363 16209 15432 14062 12152 9775 7022 3999 822 -2385 -5501 -8406 -10988 -13148 -14802 -15888 -16363";
	char  cdata3[] = "5389 465 1 0 5405 465 4 0 0"; //Unknown3 Data


	PrepareModifyTrk(blk);
	PrepareModifyPoly(blk);
	if ((isxobj)&&((xobjcrosstype==2)|(xobjcrosstype==3)|(xobjcrosstype==4)))
	{
		PrepareModifyXobj(4*blk+chunk);
		// update xobj blocks
		xb=&(xobj[4*blk+chunk]);
		xb->nobj++;
		xb->obj=(struct XOBJDATA *)realloc(xb->obj,xb->nobj*sizeof(struct XOBJDATA));
		x=&(xb->obj[xb->nobj-1]);
		memset(x,0,sizeof(struct XOBJDATA));
		x->crosstype=xobjcrosstype;
		//if (xobjcrosstype==4) x->crossno=trk[blk].nXobj;
		x->crossno=trk[blk].nXobj;
		memcpy(&(x->ptRef),&(trk[blk].ptCentre),12);
		x->nVertices=4;
		x->vert=(struct FLOATPT *)malloc(12*4);
		x->unknVertices=(long *)malloc(4*4);
		for (i=0;i<4;i++) {
			x->vert[i].z=(float)((i<2)?2:-2);
			x->vert[i].x=(float)((i%3)?2:-2);
			x->vert[i].y=0;
			x->unknVertices[i]=-1;
		}
		x->nPolygons=1;
		x->polyData=(LPPOLYGONDATA)malloc(14);
		for (i=0;i<4;i++) x->polyData->vertex[i]=i;
		x->polyData->texture=texture;
		x->polyData->hs_textflags=0;
		if (bHSMode) x->polyData->hs_textflags=flags;
		x->polyData->unknown2=0xF9;
		//x->polyData->flags=0x30; // 2-sided
		x->polyData->texanimdata=(char)flags;
		if (xobjcrosstype==3) //should be animated !
		{
			x->AnimMemory=348;
			x->AnimDelay=50;
			x->type3=3;
			x->nAnimLength=17; //Seems to be always 17
			x->animData=(struct ANIMDATA *)malloc(sizeof(ANIMDATA) * x->nAnimLength); //Get memory for Anim Data
			memset(x->animData,0,sizeof(ANIMDATA) * x->nAnimLength); //Fill will 0
			//Some values from snowys rotating donut...
			sscanf(cdata1, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",&data1[0], &data1[1], &data1[2], &data1[3], &data1[4], &data1[5], &data1[6], &data1[7], &data1[8], &data1[9], &data1[10], &data1[11], &data1[12], &data1[13], &data1[14], &data1[15], &data1[16]);
			sscanf(cdata2, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",&data2[0], &data2[1], &data2[2], &data2[3], &data2[4], &data2[5], &data2[6], &data2[7], &data2[8], &data2[9], &data2[10], &data2[11], &data2[12], &data2[13], &data2[14], &data2[15], &data2[16]);
			//sscanf(cdata3, "%d %d %d %d %d %d %d %d %d" ,&x->unknown3[0] ,&x->unknown3[1] ,&x->unknown3[2] ,&x->unknown3[3] ,&x->unknown3[4] ,&x->unknown3[5] ,&x->unknown3[6] ,&x->unknown3[7] ,&x->unknown3[8]);
			for (i=0;i<x->nAnimLength;i++) {
				x->animData[i].pt.x=(long)(x->ptRef.x * 65536); //No movement
				x->animData[i].pt.y=(long)(x->ptRef.y * 65536);
				x->animData[i].pt.z=(long)(x->ptRef.z * 65536);
				x->animData[i].od2=data1[i]; //Use data from snowys donut
				x->animData[i].od4=data2[i];
			}
		}
		// update polyobj blocks
		o=&(poly[blk].obj[chunk]);
		if (o->n1>0) {
			o->n2++;
			o->types=(long *)realloc(o->types,4*o->n2);
			o->types[o->n2-1]=x->crosstype;
		}
		// update trk blocks
		t=&(trk[blk]);
		if (xobjcrosstype==4)
		{
			t->nXobj++;
			t->xobj=(struct REFXOBJ *)realloc(t->xobj,t->nXobj*sizeof(struct REFXOBJ));
			rx=t->xobj+t->nXobj-1;
			memset(rx,0,sizeof(struct REFXOBJ));
			rx->pt.x=(int)(65536*t->ptCentre.x);
			rx->pt.z=(int)(65536*t->ptCentre.z);
			rx->pt.y=(int)(65536*t->ptCentre.y);
			rx->unknown1=255;
			rx->unknown2=255;
			rx->collideeffect=1; //Solid like Walls & Trees
		}
		// update global xobj sequence numbers
		n=0;
		for (i=0;i<nBlocks;i++)
			for (j=0;j<trk[i].nXobj;j++)
				trk[i].xobj[j].globalno=n++;
	}
	else {
		// update vertices
		t=&(trk[blk]);
		o=&(poly[blk].obj[chunk]);
		p=(LPPOLYGONDATA)malloc(sizeof(struct POLYGONDATA));
		for (i=0;i<4;i++) p->vertex[i]=t->nObjectVert+i;
		p->texture=texture;
		p->hs_textflags=0;
		if (bHSMode) p->hs_textflags=flags;
		p->unknown2=0xF9;
		//p->flags=0x30; // 2-sided
		p->texanimdata=(char)flags;
		t->nVertices+=4; t->nVerticesDup+=4;
		t->nObjectVert+=4;
		t->nLoResVert+=4; t->nHiResVert+=4; t->nMedResVert+=4;
		t->vert=(struct FLOATPT *)realloc(t->vert,t->nVertices*12);
		t->unknVertices=(unsigned long *)realloc(t->unknVertices,t->nVertices*4);
		memmove(t->vert+t->nObjectVert,t->vert+t->nObjectVert-4,12*(t->nVertices-t->nObjectVert));
		memmove(t->unknVertices+t->nObjectVert,t->unknVertices+t->nObjectVert-4,4*(t->nVertices-t->nObjectVert));
		for (i=0;i<4;i++) {
			t->vert[t->nObjectVert-4+i].z=t->ptCentre.z+((i<2)?2:-2);
			t->vert[t->nObjectVert-4+i].x=t->ptCentre.x+((i%3)?2:-2);
			t->vert[t->nObjectVert-4+i].y=t->ptCentre.y;
			t->unknVertices[t->nObjectVert-4+i]=-1;
		}
		for (i=0;i<=6;i++) {
			nn=poly[blk].sz[i];
			pp=poly[blk].poly[i];
			for (j=0;j<nn;j++,pp++) for (k=0;k<4;k++)
				pp->vertex[k]+=4;
		}
		// update objpolyblock
		if (o->n1==0) { // re-create objpolyblock
			o->n2=xobj[4*blk+chunk].nobj;
			o->nobj=0;
			o->types=(long *)realloc(o->types,4*o->n2);
			for (i=0;i<o->n2;i++) 
				o->types[i]=xobj[4*blk+chunk].obj[i].crosstype;
		}
		o->n1++;
		o->n2++;
		o->nobj++;
		o->types=(long *)realloc(o->types,4*o->n2);
		o->types[o->n2-1]=1;
		o->numpoly=(long *)realloc(o->numpoly,4*o->nobj);
		o->numpoly[o->nobj-1]=1;
		o->poly=(LPPOLYGONDATA *)realloc(o->poly,4*o->nobj);
		o->poly[o->nobj-1]=p;
	}
	RecalcBoundingBox(blk);
}

void CT3EDDoc::DelPolygon(int blk,int isxobj,int chunk,int obj,int no)
{
	struct XOBJDATA *x;
	int i,j,k,l,m,stopat;
	short vno;
	LPPOLYGONDATA p;
	struct POLYGONBLOCK *pp;
	struct OBJPOLYBLOCK *o;
	struct TRKBLOCK *t;
	struct FLOATPT backup;
	BOOL used;

	// XOBJ case
	if (isxobj) {
		PrepareModifyXobj(4*blk+chunk);
		x=&(xobj[4*blk+chunk].obj[obj]);
		ASSERT(no<x->nPolygons);
		if (x->nPolygons==1) {
			DelObject(blk,isxobj,chunk,obj);
			return;
		}
		for (i=0;i<4;i++) {
			vno=x->polyData[no].vertex[i];
			if (vno<0) continue;
			p=x->polyData; used=FALSE;
			for (j=0;j<x->nPolygons;j++,p++) if (j!=no)
				if ((p->vertex[0]==vno)||(p->vertex[1]==vno)||
					(p->vertex[2]==vno)||(p->vertex[3]==vno))
					used=TRUE;
			if (used) continue;
			p=x->polyData;
			for (j=0;j<x->nPolygons;j++,p++) for (k=0;k<4;k++) {
				if (p->vertex[k]==vno) p->vertex[k]=-1;
				if (p->vertex[k]>vno) p->vertex[k]--;
			}
			x->nVertices--;
			j=x->nVertices-vno; if (j==0) continue;
			memmove(x->vert+vno,x->vert+vno+1,12*j);
			memmove(x->unknVertices+vno,x->unknVertices+vno+1,4*j);
		}
		x->nPolygons--;
		memmove(x->polyData+no,x->polyData+no+1,(x->nPolygons-no)*sizeof(struct POLYGONDATA));
		return;
	}

	pp=&(poly[blk]);
	t=&(trk[blk]);
	PrepareModifyTrk(blk);
	PrepareModifyPoly(blk);
	// TRACK POLYGON case
	if (obj==-1) {
		ASSERT(no<pp->sz[chunk]);
		for (i=0;i<4;i++) {
			vno=pp->poly[chunk][no].vertex[i];
			if (vno>=t->nVertices) continue;
			// don't delete vertex if referenced elsewhere
			used=FALSE;
			for (k=0;k<=6;k++) {
				p=pp->poly[k];
				for (j=0;j<pp->sz[k];j++,p++) if ((k!=chunk)||(j!=no))
					if ((p->vertex[0]==vno)||(p->vertex[1]==vno)||
						(p->vertex[2]==vno)||(p->vertex[3]==vno))
						used=TRUE;
			}
			if (used) continue;
			stopat=t->nVertices; // will move to stopat-1
			if (chunk<=1) {
				p=pp->poly[2];
				for (j=0;j<pp->sz[2];j++,p++)
					if ((p->vertex[0]==vno)||(p->vertex[1]==vno)||
						(p->vertex[2]==vno)||(p->vertex[3]==vno))
						stopat=t->nLoResVert; // make it a medres
				p=pp->poly[3];
				for (j=0;j<pp->sz[3];j++,p++)
					if ((p->vertex[0]==vno)||(p->vertex[1]==vno)||
						(p->vertex[2]==vno)||(p->vertex[3]==vno))
						stopat=t->nLoResVert; // make it a medres
			}
			if ((chunk<=3)&&(stopat==t->nVertices)) {
				p=pp->poly[4];
				for (j=0;j<pp->sz[4];j++,p++)
					if ((p->vertex[0]==vno)||(p->vertex[1]==vno)||
						(p->vertex[2]==vno)||(p->vertex[3]==vno))
						stopat=t->nMedResVert; // make it a hires
				p=pp->poly[5];
				for (j=0;j<pp->sz[5];j++,p++)
					if ((p->vertex[0]==vno)||(p->vertex[1]==vno)||
						(p->vertex[2]==vno)||(p->vertex[3]==vno))
						stopat=t->nMedResVert; // make it a hires
			}
			for (l=0;l<=6;l++) {
				p=pp->poly[l];
				for (j=0;j<pp->sz[l];j++,p++) for (k=0;k<4;k++) {
					if (p->vertex[k]==vno) p->vertex[k]=stopat-1;
					else if ((p->vertex[k]>vno)&&(p->vertex[k]<stopat)) 
						p->vertex[k]--;
				}
			}
			if (t->nVertices==stopat) { t->nVertices--; t->nVerticesDup--; }
			if ((vno<t->nLoResVert)&&(t->nLoResVert<=stopat)) t->nLoResVert--;
			if ((vno<t->nMedResVert)&&(t->nMedResVert<=stopat)) t->nMedResVert--;
			if ((vno<t->nHiResVert)&&(t->nHiResVert<=stopat))  t->nHiResVert--;
			j=stopat-1-vno; if (j==0) continue;
			memcpy(&backup,t->vert+vno,12); l=t->unknVertices[vno];
			memmove(t->vert+vno,t->vert+vno+1,12*j);
			memmove(t->unknVertices+vno,t->unknVertices+vno+1,4*j);
			memcpy(t->vert+stopat-1,&backup,12);
			t->unknVertices[stopat-1]=l;
		}
		pp->sz[chunk]--; pp->szdup[chunk]--;
		memmove(pp->poly[chunk]+no,pp->poly[chunk]+no+1,(pp->sz[chunk]-no)*sizeof(struct POLYGONDATA));
		if (chunk==4) { // update TRKBLOCK too
			t->nPolygons--;
			for (i=0;i<t->nPositions;i++)
				if (t->posData[i].polygon>no) t->posData[i].polygon--;
			for (i=0;i<t->nPositions-1;i++)
				t->posData[i].nPolygons=t->posData[i+1].polygon-t->posData[i].polygon;
			t->posData[t->nPositions-1].nPolygons=t->nPolygons-t->posData[t->nPositions-1].polygon;
			j=t->polyData[no].vroadEntry;
			memmove(t->polyData+no,t->polyData+no+1,(t->nPolygons-no)*sizeof(struct POLYVROADDATA));
			used=FALSE;
			for (i=0;i<t->nPolygons;i++)
				if (t->polyData[i].vroadEntry==j) used=TRUE;
			if (!used) {
				for (i=0;i<t->nPolygons;i++)
					if (t->polyData[i].vroadEntry>j)
						t->polyData[i].vroadEntry--;
				t->nVRoad--;
				memmove(t->vroadData+j,t->vroadData+j+1,(t->nVRoad-j)*sizeof(struct VROADDATA));
			}
		}
		if (pp->sz[chunk]==0) {
			ASSERT((chunk!=0)&&(chunk!=2)&&(chunk!=4));
			free(pp->poly[chunk]);
			pp->poly[chunk]=NULL;
		}
		return;
	}

	// POLYOBJ case
	o=&(pp->obj[chunk]);
	ASSERT(no<o->numpoly[obj]);
	if (o->numpoly[obj]==1) {
		DelObject(blk,isxobj,chunk,obj);
		return;
	}
	stopat=0; // == shiftby
	for (i=0;i<4;i++) {
		vno=o->poly[obj][no].vertex[i];
		if (vno<0) continue;
		p=o->poly[obj]; used=FALSE;
		for (j=0;j<o->numpoly[obj];j++,p++) if (j!=no)
			if ((p->vertex[0]==vno)||(p->vertex[1]==vno)||
				(p->vertex[2]==vno)||(p->vertex[3]==vno))
				used=TRUE; // objects are disjoint by assumption
		if (used) continue;
		p=o->poly[obj]+no;
		for (k=0;k<4;k++) if (p->vertex[k]==vno) p->vertex[k]=-1;
		for (l=0;l<4;l++) for (m=0;m<pp->obj[l].nobj;m++) {
			p=pp->obj[l].poly[m];
			for (j=0;j<pp->obj[l].numpoly[m];j++,p++) for (k=0;k<4;k++) {
				ASSERT(p->vertex[k]!=vno);
				if (p->vertex[k]>vno) p->vertex[k]--;
			}
		}
		stopat++; t->nObjectVert--; 
		t->nLoResVert--; t->nMedResVert--; t->nHiResVert--;
		t->nVertices--; t->nVerticesDup--;
		memmove(t->vert+vno,t->vert+vno+1,12*(t->nVertices-vno));
		memmove(t->unknVertices+vno,t->unknVertices+vno+1,4*(t->nVertices-vno));
	}
	for (i=0;i<=6;i++) for (j=0;j<pp->sz[i];j++)
		for (k=0;k<4;k++) pp->poly[i][j].vertex[k]-=stopat;
	o->numpoly[obj]--;
	o->n1--;
	memmove(o->poly[obj]+no,o->poly[obj]+no+1,(o->numpoly[obj]-no)*sizeof(struct POLYGONDATA));
}

void CT3EDDoc::SetCollisionHandling(struct FLOATPT *pt)
{
	int i,j,k;
	struct FLOATPT *v;
	LPPOLYGONDATA p;
	struct POLYVROADDATA *data;

	for (i=0;i<nBlocks;i++) {
		if (!CanContainPoint(trk+i,pt,0.0)) continue;
		v=trk[i].vert+trk[i].nObjectVert;
		for (j=trk[i].nObjectVert;j<trk[i].nHiResVert;j++,v++)
			if ((fabs(v->x-pt->x)<1E-3)&&(fabs(v->z-pt->z)<1E-3)&&(fabs(v->y-pt->y)<1E-3))
			{
				PrepareModifyTrk(i);
				p=poly[i].poly[4];
				data=trk[i].polyData;
				for (k=0;k<trk[i].nPolygons;k++,p++,data++)
					if ((data->flags&0x0f)%14!=0)
						if ((j==p->vertex[0])||(j==p->vertex[1])||
							(j==p->vertex[2])||(j==p->vertex[3]))
								data->flags|=0x80; // collision detect
			}
	}
}

BOOL CT3EDDoc::DoesPointExist(struct FLOATPT *pt)
{
	int i,j,k,l,n;
	struct FLOATPT *v;
	struct XOBJDATA *x;

	for (i=0;i<nBlocks;i++) {
		if (!CanContainPoint(trk+i,pt,0.0)) continue;
		v=trk[i].vert;
		n=trk[i].nVertices;
		for (j=0;j<n;j++,v++)
			if ((fabs(v->x-pt->x)<1E-3)&&(fabs(v->z-pt->z)<1E-3)&&
				(fabs(v->y-pt->y)<1E-3)) return TRUE;
		for (j=4*i;j<4*i+4;j++) {
			x=xobj[j].obj;
			for (k=0;k<xobj[j].nobj;k++,x++) {
				v=x->vert;
				for (l=0;l<x->nVertices;l++,v++)
					if ((fabs(v->x+x->ptRef.x-pt->x)<1E-3)&&
						(fabs(v->z+x->ptRef.z-pt->z)<1E-3)&&
						(fabs(v->y+x->ptRef.y-pt->y)<1E-3))
						return TRUE;
			}
		}
	}
	return FALSE;
}

int CT3EDDoc::DuplPolygon(int blk,int isxobj,int chunk,int obj,int no)
{
	struct XOBJDATA *x;
	int i,j,k,l;
	short vno,vno2;
	LPPOLYGONDATA p;
	struct POLYGONBLOCK *pp;
	struct OBJPOLYBLOCK *o;
	struct TRKBLOCK *t;
	struct FLOATPT backup;

	// XOBJ case
	if (isxobj) {
		PrepareModifyXobj(4*blk+chunk);
		x=&(xobj[4*blk+chunk].obj[obj]);
		ASSERT(no<x->nPolygons);
		x->vert=(struct FLOATPT *)realloc(x->vert,12*(x->nVertices+4));
		x->unknVertices=(long *)realloc(x->unknVertices,4*(x->nVertices+4));
		x->polyData=(struct POLYGONDATA *)realloc(x->polyData,14*(x->nPolygons+1));
		p=x->polyData+x->nPolygons;
		memcpy(p,x->polyData+no,14);
		x->nPolygons++;
		for (i=0;i<4;i++) {
			vno=p->vertex[i];
			memcpy(&backup,x->vert+vno,12);
			backup.x+=0.5;
			while (DoesPointExist(&backup)) backup.x+=(float)2E-3;
			p->vertex[i]=(short)x->nVertices;
			memcpy(x->vert+x->nVertices,&backup,12);
			x->unknVertices[x->nVertices]=x->unknVertices[vno];
			x->nVertices++;
		}
		return x->nPolygons-1;
	}

	pp=&(poly[blk]);
	t=&(trk[blk]);
	PrepareModifyTrk(blk);
	PrepareModifyPoly(blk);
	// TRACK POLYGON case
	if (obj==-1) {
		ASSERT(no<pp->sz[chunk]);
		vno2=(short)t->nVertices;
		t->vert=(struct FLOATPT *)realloc(t->vert,12*(vno2+4));
		t->unknVertices=(unsigned long *)realloc(t->unknVertices,4*(vno2+4));
		if (chunk<6) { // create new vertices at nLoResVert
			vno2=(short)t->nLoResVert;
			memmove(t->vert+vno2+4,t->vert+vno2,12*(t->nVertices-vno2));
			memmove(t->unknVertices+vno2+4,t->unknVertices+vno2,4*(t->nVertices-vno2));
			for (l=0;l<=6;l++) { // l=2 would be enough
				p=pp->poly[l];
				for (j=0;j<pp->sz[l];j++,p++) 
					for (k=0;k<4;k++)
						if (p->vertex[k]>=vno2) p->vertex[k]+=4;
			}
			t->nLoResVert+=4;
			t->nMedResVert+=4;
			t->nHiResVert+=4;
		}
		t->nVertices+=4; t->nVerticesDup+=4;
		pp->poly[chunk]=(struct POLYGONDATA *)realloc(pp->poly[chunk],14*(pp->sz[chunk]+1));
		p=pp->poly[chunk]+no+1;
		memmove(p,pp->poly[chunk]+no,14*(pp->sz[chunk]-no));
		pp->sz[chunk]++; pp->szdup[chunk]++;
		for (i=0;i<4;i++) {
			vno=p->vertex[i];
			memcpy(&backup,t->vert+vno,12);
			backup.x+=0.5;
			while (DoesPointExist(&backup)) backup.x+=(float)2E-3;
			p->vertex[i]=vno2;
			memcpy(t->vert+vno2,&backup,12);
			t->unknVertices[vno2]=t->unknVertices[vno];
			vno2++;
		}
		if ((chunk>=2)&&(chunk<6)) { // multiresolution
			pp->poly[chunk-2]=(struct POLYGONDATA *)realloc(pp->poly[chunk-2],14*(pp->sz[chunk-2]+1));
			memcpy(pp->poly[chunk-2]+pp->sz[chunk-2],p,14);
			pp->sz[chunk-2]++; pp->szdup[chunk-2]++;
		}
		if ((chunk>=4)&&(chunk<6)) { // multiresolution
			pp->poly[chunk-4]=(struct POLYGONDATA *)realloc(pp->poly[chunk-4],14*(pp->sz[chunk-4]+1));
			memcpy(pp->poly[chunk-4]+pp->sz[chunk-4],p,14);
			pp->sz[chunk-4]++; pp->szdup[chunk-4]++;
		}
		if (chunk==4) { // update TRKBLOCK too
			t->nPolygons++;
			t->polyData=(struct POLYVROADDATA *)realloc(t->polyData,sizeof(struct POLYVROADDATA)*t->nPolygons);
			memmove(t->polyData+no+1,t->polyData+no,sizeof(struct POLYVROADDATA)*(t->nPolygons-1-no));
			for (i=0;i<t->nPositions;i++)
				if (t->posData[i].polygon>no) t->posData[i].polygon++;
			for (i=0;i<t->nPositions-1;i++)
				t->posData[i].nPolygons=t->posData[i+1].polygon-t->posData[i].polygon;
			t->posData[t->nPositions-1].nPolygons=t->nPolygons-t->posData[t->nPositions-1].polygon;
		}
		return (no+1);
	}

	// POLYOBJ case
	o=&(pp->obj[chunk]);
	ASSERT(no<o->numpoly[obj]);
	vno2=(short)t->nVertices;
	t->vert=(struct FLOATPT *)realloc(t->vert,12*(vno2+4));
	t->unknVertices=(unsigned long *)realloc(t->unknVertices,4*(vno2+4));
	vno2=(short)t->nObjectVert;
	memmove(t->vert+vno2+4,t->vert+vno2,12*(t->nVertices-vno2));
	memmove(t->unknVertices+vno2+4,t->unknVertices+vno2,4*(t->nVertices-vno2));
	for (l=0;l<=6;l++) {
		p=pp->poly[l];
		for (j=0;j<pp->sz[l];j++,p++) 
			for (k=0;k<4;k++)
				p->vertex[k]+=4;
	}
	t->nObjectVert+=4;
	t->nLoResVert+=4; t->nMedResVert+=4; t->nHiResVert+=4;
	t->nVertices+=4; t->nVerticesDup+=4;
	o->poly[obj]=(struct POLYGONDATA *)realloc(o->poly[obj],14*(o->numpoly[obj]+1));
	p=o->poly[obj]+o->numpoly[obj];
	memcpy(p,o->poly[obj]+no,14);
	o->numpoly[obj]++; 
	o->n1++;
	for (i=0;i<4;i++) {
		vno=p->vertex[i];
		memcpy(&backup,t->vert+vno,12);
		backup.x+=0.5;
		while (DoesPointExist(&backup)) backup.x+=(float)2E-3;
		p->vertex[i]=vno2;
		memcpy(t->vert+vno2,&backup,12);
		t->unknVertices[vno2]=t->unknVertices[vno];
		vno2++;
	}
	return o->numpoly[obj]-1;
}

void CT3EDDoc::ConnectBlocks(int blk1, int blk2, float mdist)
{
	int i,j,k,l,n1,n2,match,v1,v2;
	struct TRKBLOCK *t1, *t2;
	float dist, fdist;
	LPPOLYGONDATA p1,p2;

	t1=&(trk[blk1]);
	t2=&(trk[blk2]);
	n1=poly[blk1].sz[4];
	n2=poly[blk2].sz[4];
	p1=poly[blk1].poly[4];
	for (i=0;i<poly[blk1].sz[4];i++,p1++) //Loop for all highres polys of block1
	{
		for (j=0;j<4;j++) //Loop for all 4 points of polygon
		{
			v1=p1->vertex[j];
			match=-1;
			p2=poly[blk2].poly[4];
			fdist=100;
			for (k=0;k<poly[blk2].sz[4];k++,p2++) //Loop for all highres polys of block2
			{
				for (l=0;l<4;l++)
				{
					v2=p2->vertex[l];
					dist=(float)sqrt((pow((t1->vert[v1].x - t2->vert[v2].x),2) + pow((t1->vert[v1].y - t2->vert[v2].y),2) + pow((t1->vert[v1].z - t2->vert[v2].z),2)));
					if ((dist<fdist)&&(dist>0)&&dist<mdist) 
						{ match=v2; fdist=dist; }
				}
			}
			if (match>-1) //Near enuogh ?
			{
				t1->vert[v1].x=t2->vert[match].x;
				t1->vert[v1].y=t2->vert[match].y;
				t1->vert[v1].z=t2->vert[match].z;
			}
		}
	}
}

void CT3EDDoc::MergePoint(struct FLOATPT *pt1,struct FLOATPT *pt2)
{
	struct FLOATPT src,tgt,*v;
	LPPOLYGONDATA p,pp;
	int i,j,k,l,m,jj,kk,ll,mm;
	struct TRKBLOCK *t;
	struct OBJPOLYBLOCK *o;
	struct XOBJDATA *x;
	short vno,vold;
	BOOL isModified,doit;

	memcpy(&src,pt1,12);
	memcpy(&tgt,pt2,12);
	for (i=0,t=trk;i<nBlocks;i++,t++) {
		if (!CanContainPoint(t,&src,0.0)) continue;
		isModified=FALSE;
		// track vertices
		v=t->vert+t->nObjectVert;
		vno=-1;
		for (j=t->nObjectVert;j<t->nVertices;) {
			if (j==t->nHiResVert) vno=-1; // restart for road lanes
			doit=((fabs(v->x-src.x)<1E-2)&&(fabs(v->z-src.z)<1E-2)&&
				  (fabs(v->y-src.y)<1E-2));
			if ((!doit)&&((fabs(v->x-tgt.x)>1E-2)||
				(fabs(v->z-tgt.z)>1E-2)||(fabs(v->y-tgt.y)>1E-2)))
					{ j++; v++; continue; }
			PrepareModifyTrk(i);
			isModified=TRUE;
			v->x=tgt.x; v->z=tgt.z; v->y=tgt.y;
			p=poly[i].poly[4];  // first recalc vroad
			for (k=0;k<t->nPolygons;k++,p++)
				if ((j==p->vertex[0])||(j==p->vertex[1])||
					(j==p->vertex[2])||(j==p->vertex[3]))
					RecalcPolyVroad(i,k);
			if (vno==-1) {
				vno=(short)j;
				j++; v++;
			} else {
				PrepareModifyPoly(i);
				for (l=0;l<=6;l++) {
					p=poly[i].poly[l];
					for (k=0;k<poly[i].sz[l];k++,p++)
						for (m=0;m<4;m++)
							if (p->vertex[m]==j) p->vertex[m]=vno;
							else if (p->vertex[m]>j) p->vertex[m]--;
				}
				memmove(t->vert+j,t->vert+j+1,12*(t->nVertices-j-1));
				memmove(t->unknVertices+j,t->unknVertices+j+1,4*(t->nVertices-j-1));
				t->nVertices--; t->nVerticesDup--;
				if (j<t->nLoResVert) t->nLoResVert--;
				if (j<t->nMedResVert) t->nMedResVert--;
				if (j<t->nHiResVert) t->nHiResVert--;
				// don't increase j & v
			}
		}
		// polygon object vertices
		o=poly[i].obj;
		for (j=0;j<4;j++,o++) for (k=0;k<o->nobj;k++) {
			vno=-1;
			p=o->poly[k];
			for (l=0;l<o->numpoly[k];l++,p++) for (m=0;m<4;m++) {
				vold=p->vertex[m]; v=t->vert+vold;
				if (vold==vno) continue; // already done !
				doit=((fabs(v->x-src.x)<1E-2)&&(fabs(v->z-src.z)<1E-2)&&
					  (fabs(v->y-src.y)<1E-2));
				if ((!doit)&&((fabs(v->x-tgt.x)>1E-2)||
					(fabs(v->z-tgt.z)>1E-2)||(fabs(v->y-tgt.y)>1E-2)))
						continue;
				PrepareModifyTrk(i);
				isModified=TRUE;
				v->x=tgt.x; v->z=tgt.z; v->y=tgt.y;
				if (vno==-1) { vno=vold; continue; }
				PrepareModifyPoly(i);
				for (ll=0;ll<=6;ll++) {
					pp=poly[i].poly[ll];
					for (kk=0;kk<poly[i].sz[ll];kk++,pp++)
						for (mm=0;mm<4;mm++)
							pp->vertex[mm]--;
				}
				for (ll=0;ll<4;ll++) 
					for (jj=0;jj<poly[i].obj[ll].nobj;jj++) {
						pp=poly[i].obj[ll].poly[jj];
						for (kk=0;kk<poly[i].obj[ll].numpoly[jj];kk++,pp++)
							for (mm=0;mm<4;mm++) {
								if (pp->vertex[mm]==vold) pp->vertex[mm]=vno;
								if (pp->vertex[mm]>vold) pp->vertex[mm]--;
							}
					}
				memmove(t->vert+vold,t->vert+vold+1,12*(t->nVertices-vold-1));
				memmove(t->unknVertices+vold,t->unknVertices+vold+1,4*(t->nVertices-vold-1));
				t->nVertices--; t->nVerticesDup--;
				t->nLoResVert--; t->nMedResVert--; t->nHiResVert--;
				t->nObjectVert--;
			}
		}
		// xobj vertices
		for (jj=4*i;jj<4*i+4;jj++) {
			x=xobj[jj].obj;
			for (kk=0;kk<xobj[jj].nobj;kk++,x++) {
				v=x->vert;
				vno=-1;
				for (j=0;j<x->nVertices;) {
					doit=((fabs(v->x+x->ptRef.x-src.x)<1E-2)&&
						  (fabs(v->z+x->ptRef.z-src.z)<1E-2)&&
						  (fabs(v->y+x->ptRef.y-src.y)<1E-2));
					if ((!doit)&&((fabs(v->x+x->ptRef.x-tgt.x)>1E-2)||
						(fabs(v->z+x->ptRef.z-tgt.z)>1E-2)||
						(fabs(v->y+x->ptRef.y-tgt.y)>1E-2)))
						{ j++; v++; continue; }
					PrepareModifyXobj(jj);
					isModified=TRUE;
					v->x=tgt.x-x->ptRef.x; 
					v->z=tgt.z-x->ptRef.z; 
					v->y=tgt.y-x->ptRef.y;
					if (vno==-1) {
						vno=(short)j;
						j++; v++;
					} else {
						p=x->polyData;
						for (k=0;k<x->nPolygons;k++,p++)
							for (m=0;m<4;m++)
								if (p->vertex[m]==j) p->vertex[m]=vno;
								else if (p->vertex[m]>j) p->vertex[m]--;
						memmove(x->vert+j,x->vert+j+1,12*(x->nVertices-j-1));
						memmove(x->unknVertices+j,x->unknVertices+j+1,4*(x->nVertices-j-1));
						x->nVertices--;
						// don't increase j & v
					}
				}
			}
		}
		if (isModified) RecalcBoundingBox(i);
	}
}

void CT3EDDoc::SplitPoint(struct FLOATPT *refpt)
{
	struct FLOATPT pt,diag,*v;
	float diaglen;
	int i,j,k,l,m,n,vno2;
	int ii,jj,k0,l0,m0,shiftby;
	struct TRKBLOCK *t;
	LPPOLYGONDATA p;
	struct OBJPOLYBLOCK *o;
	struct XOBJDATA *x;
	BOOL isModified,flag;

	memcpy(&pt,refpt,12);
	for (i=0,t=trk;i<nBlocks;i++,t++) {
		if (!CanContainPoint(t,&pt,0.0)) continue;
		isModified=FALSE;
		// track vertices
		for (j=t->nObjectVert;j<t->nVertices;j++) {
			v=t->vert+j;
			if ((fabs(v->x-pt.x)>1E-3)||(fabs(v->y-pt.y)>1E-3)||
				(fabs(v->z-pt.z)>1E-3)) continue;
			PrepareModifyPoly(i);
			PrepareModifyTrk(i);
			isModified=TRUE;
			n=0;
			for (k=0;k<=6;k++) {
				p=poly[i].poly[k];
				for (l=0;l<poly[i].sz[k];l++,p++) for (m=0;m<4;m++)
					if (p->vertex[m]==j) 
						{ p->vertex[m]=-1; k0=k; l0=l; m0=m; n++; }
			}
			if (n==0) continue; // unused vertex !
			p=poly[i].poly[k0];
			p[l0].vertex[m0]=j; n--; // re-use j
			ii=poly[i].poly[k0][l0].vertex[m0^2];
			shiftby=k0;
			// find in lower res
			if ((k0<=5)&&(k0>=2)) {
				p=poly[i].poly[k0-2];
				for (l=0;l<poly[i].sz[k0-2];l++,p++)
					if (p->vertex[m0]==-1) 
						{ p->vertex[m0]=j; n--; shiftby=k0-2; }
			}
			if ((k0<=5)&&(k0>=4)) {
				p=poly[i].poly[k0-4];
				for (l=0;l<poly[i].sz[k0-4];l++,p++)
					if (p->vertex[m0]==-1) 
						{ p->vertex[m0]=j; n--; shiftby=k0-4; }
			}
			// neighbor too ? => left neighbor (k0,l0,m0 is last)
			if ((k0==4)&&(l0>0)&&(m0%3==0)) {
				p=poly[i].poly[4]+l0-1;
				if ((p->vertex[m0^1]==-1)&&(p->vertex[m0^2]==p[1].vertex[m0^3])) {
					p->vertex[m0^1]=j; n--;
					ii=poly[i].poly[k0][l0].vertex[m0^3];
					p=poly[i].poly[2];
					for (l=0;l<poly[i].sz[2];l++,p++)
						if (p->vertex[m0^1]==-1) 
							{ p->vertex[m0^1]=j; n--; if (shiftby>2) shiftby=2; }
					p=poly[i].poly[0];
					for (l=0;l<poly[i].sz[0];l++,p++)
						if (p->vertex[m0^1]==-1) 
							{ p->vertex[m0^1]=j; n--; shiftby=0; }
				}
			}
					
			diag.x=t->vert[ii].x-pt.x;
			diag.z=t->vert[ii].z-pt.z;
			diag.y=t->vert[ii].y-pt.y;
			diaglen=2*(float)sqrt(diag.x*diag.x+diag.z*diag.z+diag.y*diag.y+0.01);
			diag.x=v->x+diag.x/diaglen;
			diag.z=v->z+diag.z/diaglen;
			diag.y=v->y+diag.y/diaglen;
			while (DoesPointExist(&diag)) diag.x+=(float)0.1;
			memcpy(v,&diag,12);
			if (n==0) continue;
			// create a new vertex
			vno2=(t->nVertices++); t->nVerticesDup++;
			if (shiftby<6) vno2=(t->nHiResVert++);
			if (shiftby<4) vno2=(t->nMedResVert++);
			if (shiftby<2) vno2=(t->nLoResVert++);
			ASSERT(vno2>j);
			t->vert=(struct FLOATPT *)realloc(t->vert,12*t->nVertices);
			t->unknVertices=(unsigned long *)realloc(t->unknVertices,4*t->nVertices);
			memmove(t->vert+vno2+1,t->vert+vno2,12*(t->nVertices-vno2-1));
			memmove(t->unknVertices+vno2+1,t->unknVertices+vno2,4*(t->nVertices-vno2-1));
			memcpy(t->vert+vno2,&pt,12);
			t->unknVertices[vno2]=t->unknVertices[j];
			for (k=0;k<=6;k++) {
				p=poly[i].poly[k];
				for (l=0;l<poly[i].sz[k];l++,p++) {
					flag=FALSE;
					for (m=0;m<4;m++)
						if (p->vertex[m]==-1) p->vertex[m]=vno2;
						else if (p->vertex[m]==j) flag=TRUE;
						else if (p->vertex[m]>=vno2) p->vertex[m]++;
					if ((k==4)&&flag) RecalcPolyVroad(i,l);
				}
			}
		}
		// polyobj vertices
		o=poly[i].obj;
		shiftby=0;
		for (j=0;j<4;j++,o++) for (k=0;k<o->nobj;k++) {
			p=o->poly[k];
			for (l=0;l<o->numpoly[k];l++) for (m=0;m<4;m++) {
				vno2=p[l].vertex[m]; v=t->vert+vno2;
				if ((fabs(v->x-pt.x)>1E-3)||(fabs(v->y-pt.y)>1E-3)||
					(fabs(v->z-pt.z)>1E-3)) continue;
				PrepareModifyPoly(i);
				PrepareModifyTrk(i);
				isModified=TRUE;
				n=0;
				for (ii=0;ii<o->numpoly[k];ii++) for (jj=0;jj<4;jj++)
					if (p[ii].vertex[jj]==vno2) {
						if (n>0) p[ii].vertex[jj]=(short)t->nObjectVert;
						n++;
					}
				ii=p[l].vertex[m^2];
				diag.x=t->vert[ii].x-pt.x;
				diag.z=t->vert[ii].z-pt.z;
				diag.y=t->vert[ii].y-pt.y;
				diaglen=2*(float)sqrt(diag.x*diag.x+diag.z*diag.z+diag.y*diag.y+0.01);
				diag.x=v->x+diag.x/diaglen;
				diag.z=v->z+diag.z/diaglen;
				diag.y=v->y+diag.y/diaglen;
				while (DoesPointExist(&diag)) diag.x+=(float)0.1;
				memcpy(v,&diag,12);
				if (n==1) continue;
				t->nVertices++; t->nVerticesDup++;
			    t->nHiResVert++; t->nMedResVert++; t->nLoResVert++;
				t->nObjectVert++;
				t->vert=(struct FLOATPT *)realloc(t->vert,12*t->nVertices);
				t->unknVertices=(unsigned long *)realloc(t->unknVertices,4*t->nVertices);
				memmove(t->vert+t->nObjectVert,t->vert+t->nObjectVert-1,12*(t->nVertices-t->nObjectVert));
				memmove(t->unknVertices+t->nObjectVert,t->unknVertices+t->nObjectVert-1,4*(t->nVertices-t->nObjectVert));
				memcpy(t->vert+t->nObjectVert-1,&pt,12);
				t->unknVertices[t->nObjectVert-1]=t->unknVertices[vno2];
				shiftby++;
			}
		}
		for (k=0;k<=6;k++) {
			p=poly[i].poly[k];
			for (l=0;l<poly[i].sz[k];l++,p++)
				for (m=0;m<4;m++) p->vertex[m]+=shiftby;
		}
		// xobj vertices
		for (j=4*i;j<4*i+4;j++) for (k=0,x=xobj[j].obj;k<xobj[j].nobj;k++,x++) {
			p=x->polyData;
			for (l=0;l<x->nPolygons;l++) for (m=0;m<4;m++) {
				vno2=p[l].vertex[m]; v=x->vert+vno2;
				if ((fabs(v->x+x->ptRef.x-pt.x)>1E-3)||
					(fabs(v->y+x->ptRef.y-pt.y)>1E-3)||
					(fabs(v->z+x->ptRef.z-pt.z)>1E-3)) continue;
				PrepareModifyXobj(j);
				isModified=TRUE;
				n=0;
				for (ii=0;ii<x->nPolygons;ii++) for (jj=0;jj<4;jj++)
					if (p[ii].vertex[jj]==vno2) {
						if (n>0) p[ii].vertex[jj]=(short)x->nVertices;
						n++;
					}
				ii=p[l].vertex[m^2];
				diag.x=x->vert[ii].x-v->x;
				diag.z=x->vert[ii].z-v->z;
				diag.y=x->vert[ii].y-v->y;
				diaglen=2*(float)sqrt(diag.x*diag.x+diag.z*diag.z+diag.y*diag.y+0.01);
				diag.x=x->ptRef.x+v->x+diag.x/diaglen;
				diag.z=x->ptRef.z+v->z+diag.z/diaglen;
				diag.y=x->ptRef.y+v->y+diag.y/diaglen;
				while (DoesPointExist(&diag)) diag.x+=(float)0.1;
				v->x=diag.x-x->ptRef.x;
				v->z=diag.z-x->ptRef.z;
				v->y=diag.y-x->ptRef.y;
				if (n==1) continue;
				x->nVertices++;
				x->vert=(struct FLOATPT *)realloc(x->vert,12*x->nVertices);
				x->unknVertices=(long *)realloc(x->unknVertices,4*x->nVertices);
				v=x->vert+(x->nVertices-1);
				v->x=pt.x-x->ptRef.x;
				v->z=pt.z-x->ptRef.z;
				v->y=pt.y-x->ptRef.y;
				x->unknVertices[x->nVertices-1]=x->unknVertices[vno2];
			}
		}

		if (isModified) RecalcBoundingBox(i);
	}
}

void CT3EDDoc::AdjustRoadWidth(int AdjustType, bool LaneEdges, int Startblk, int Endblk, float size)
{
	int i,j,k,np, nodecheck, NodeOnProgress, comparevalue;
	double mind,maxd,d;
	//float dee, dee2;
	//struct TRKBLOCK *t=trk;
	struct TRKBLOCK *t;
	LPPOLYGONDATA p;
	float *LaneEdgeLeft;
	float *LaneEdgeRight;
	struct COLVROAD *v;
	struct FLOATPT *pt1,*pt2,pt, middlept;

	bool doit, doneonce;
	doit=true;
	doneonce=false;
	//checkedpoly=-1;
	if (undoCol[undoLevel-1]==NULL) {
		undoCol[undoLevel-1]=(struct COLVROAD *)malloc(36*col.vroadHead.nrec);
		memcpy(undoCol[undoLevel-1],col.vroad,36*col.vroadHead.nrec);
	}
	//for (j=t->nStartPos;j<t->nStartPos+t->nPositions;j++)
	
		
	for (i=Startblk;i<Endblk+1;i++,t++)
	{
		t=&(trk[i]);
		nodecheck=-1;
			//for (i=0;i<nBlocks;i++,t++)
		for (j=t->nStartPos;j<t->nStartPos+t->nPositions;j++) 
		{
			v=col.vroad+j;
			/*
			float *delta = (float*)pDoc->col.hs_extra + (pDoc->col.vroadHead.nrec-1) * 7 + 0;
			float d;

			d = *delta * ((long*)(delta))[3];
			res = RefPt - Right * d;

			*/
			
			LaneEdgeLeft = (float*)col.hs_extra+(j)*7;
			LaneEdgeRight = (float*)col.hs_extra+(j)*7+sizeof(float);
			mind=0;
			maxd=0;
			p=poly[i].poly[4];
			np=poly[i].sz[4];
			doneonce=false;
			NodeOnProgress=(j-t->nStartPos); //lasketaan node number.
			//generating array about amount of polys per strip.
			/*k=0; startpoly=0;
			while (k<8)
			{
				nPolysPerStrip[k]=0;
				k++;
			}
			k=0;
			while (k<np)
			{
				nPolysPerStrip[PolysNode(i,k)]++;
				k++;
			}
			k=0;
			for (pikkulaskuri=0;pikkulaskuri<j-t->nStartPos;pikkulaskuri++)
				startpoly=startpoly+nPolysPerStrip[pikkulaskuri]; */
	
			for (k=0;k<np;k++,p++)
			{	
				//mind=1.2;
				//maxd=1.3
				//if (AdjustType>0) 
				//{  
					if (NodeOnProgress==PolysNode(i,k))
					  {
						if (AdjustType==0) 
							comparevalue=((t->polyData[k].flags&0x0f)%14);
						if (AdjustType==1) 
							comparevalue=(t->polyData[k].flags  & 0x80);
						if (AdjustType==2)
							comparevalue=(t->polyData[k].virtualroadedge);
						if (AdjustType==3)
							{
								comparevalue=255;
								mind=-size;
								maxd=size;
							}
						if (comparevalue==1) //0x80 = virtual road boundary, 0xE = Non-passable.
						{
							if (doneonce==false)
							{
								doneonce=true;
								pt1=t->vert+p->vertex[0];
								pt2=t->vert+p->vertex[3];
							}
							else 
							{
								pt1=t->vert+p->vertex[1];
								pt2=t->vert+p->vertex[2];
							}
							middlept.x=((pt2->x-pt1->x)/2)+pt1->x;
							middlept.y=((pt2->y-pt1->y)/2)+pt1->y;
							middlept.z=((pt2->z-pt1->z)/2)+pt1->z;
							pt=middlept;
						
							d=(pt.x*65536-v->refPt.x)*v->right.x+
							(pt.y*65536-v->refPt.y)*v->right.y+
							(pt.z*65536-v->refPt.z)*v->right.z;
							
		
							d=d/128;
							if (d<mind) mind=d;
							if (d>maxd) maxd=d;
						}
					
					}
				
				//}
				//else 
				//{
				/*	if ((t->polyData[k].flags  & 0x80)) // && (k>checkedpoly)) //doit=true;
					 //for (k2=k;k2<np;k2++,p2++) 
					{
						//vertex[0] & vertex[3] for start check, vertex[1] & vertex[2] for end check.
					
						//for (l=0;l<4;l++) {
						if (doneonce==false && doit==true)
						{
							doneonce=true;
							pt1=t->vert+p->vertex[0];
							pt2=t->vert+p->vertex[3];
							
							middlept.x=((pt2->x-pt1->x)/2)+pt1->x;
							middlept.y=((pt2->y-pt1->y)/2)+pt1->y;
							middlept.z=((pt2->z-pt1->z)/2)+pt1->z;
							pt=middlept;
							
							d=(pt.x*65536-v->refPt.x)*v->right.x+
							(pt.y*65536-v->refPt.y)*v->right.y+
							(pt.z*65536-v->refPt.z)*v->right.z;

							mind=d;
						}
						else if (doneonce==true &&doit==true)
						{
							doneonce=false;
							doit=false;
							pt1=t->vert+p->vertex[1];
							pt2=t->vert+p->vertex[2];
							//checkedpoly=k;

							middlept.x=((pt2->x-pt1->x)/2)+pt1->x;
							middlept.y=((pt2->y-pt1->y)/2)+pt1->y;
							middlept.z=((pt2->z-pt1->z)/2)+pt1->z;
							pt=middlept;
								
							d=(pt.x*65536-v->refPt.x)*v->right.x+
							(pt.y*65536-v->refPt.y)*v->right.y+
							(pt.z*65536-v->refPt.z)*v->right.z;

							maxd=d;
						}
					
						if (doit==false) 
						{
							v->leftWall=-(long)mind;
							v->rightWall=(long)maxd;
							doit=true;
						}	
				
				
				
				
				/*pt1=t->vert+p->vertex[0];
					pt2=t->vert+p->vertex[3];
	
					middlept.x=((pt2->x-pt1->x)/2)+pt1->x;
					middlept.y=((pt2->y-pt1->y)/2)+pt1->y;
					middlept.z=((pt2->z-pt1->z)/2)+pt1->z;
					pt=middlept;
								
					d=(pt.x*65536-v->refPt.x)*v->right.x+
					(pt.y*65536-v->refPt.y)*v->right.y+
					(pt.z*65536-v->refPt.z)*v->right.z;

					d=d/128;
					//if (d<mind) mind=d;
					maxd=d;
				}*/
			}
			if (LaneEdges == false) {
				v->leftWall=-(long)mind;
				v->rightWall=(long)maxd;
			}
			else {
				
				//dee=LaneEdgeLeft;
				//dee2=LaneEdgeRight;
				
				//->LaneEdgeLeft=-(float)mind;///(float)mind;
				//->LaneEdgeRight=(float)maxd;
				//*LaneEdgeLeft * ((long*)(LaneEdgeLeft))[3]= -(float)mind;
				//*LaneEdgeRight * ((long*)(LaneEdgeRight))[3]=(float)maxd;


			}


		}
			
	}
	
}	
		
void CT3EDDoc::HS_RecalcMinMaxXY()
{
	int i,j,k;
	struct TRKBLOCK *t=trk;
	float x,y;
	unsigned char v,w;
	LPPOLYGONDATA p;

	for (i=0;i<nBlocks;i++,t++) {
		t->hs_ptMin.z=t->hs_ptMax.z=t->ptCentre.z;
		t->hs_ptMin.x=t->hs_ptMax.x=t->ptCentre.x;
		t->hs_ptMin.y=t->hs_ptMax.y=t->ptCentre.y;
		p=poly[i].poly[4];
		for (j=0;j<t->nPolygons;j++,p++)
			if ((t->polyData[j].flags&15)!=14)
				for (k=0;k<4;k++) {
					x=t->vert[p->vertex[k]].x;
					y=t->vert[p->vertex[k]].y;
					if (t->hs_ptMin.x>x) t->hs_ptMin.x=x;
					if (t->hs_ptMax.x<x) t->hs_ptMax.x=x;
					if (t->hs_ptMin.y>y) t->hs_ptMin.y=y;
					if (t->hs_ptMax.y<y) t->hs_ptMax.y=y;
				}
		t->hs_ptMin.x-=(float)0.1; t->hs_ptMin.y-=(float)0.1;
		t->hs_ptMax.x+=(float)0.1; t->hs_ptMax.y+=(float)0.1;
		p=poly[i].poly[4];
		for (j=0;j<t->nPolygons;j++,p++)
			if ((t->polyData[j].flags&15)!=14) {
				t->polyData[j].hs_minmax[0]=255;
				t->polyData[j].hs_minmax[1]=0;
				t->polyData[j].hs_minmax[2]=255;
				t->polyData[j].hs_minmax[3]=0;
				for (k=0;k<4;k++) {
					v=(unsigned char)(255*(t->vert[p->vertex[k]].x-t->hs_ptMin.x)/(t->hs_ptMax.x-t->hs_ptMin.x));
					w=(unsigned char)(255*(t->vert[p->vertex[k]].y-t->hs_ptMin.y)/(t->hs_ptMax.y-t->hs_ptMin.y));
					// min y
					if (w<t->polyData[j].hs_minmax[0]) t->polyData[j].hs_minmax[0]=w;
					// max y
					if (w>t->polyData[j].hs_minmax[1]) t->polyData[j].hs_minmax[1]=w;
					// min x
					if (v<t->polyData[j].hs_minmax[2]) t->polyData[j].hs_minmax[2]=v;
					// max x
					if (v>t->polyData[j].hs_minmax[3]) t->polyData[j].hs_minmax[3]=v;
				}
			}
	}
}

void CT3EDDoc::SetAllTextures(int texture,BOOL drivable,short flags)
{
	int i,j,k,l,np;
	struct TRKBLOCK *t=trk;
	LPPOLYGONDATA p;
	struct COLVROAD *v;
	struct FLOATPT pt;
	FLOAT mind,dist,ll;
	BOOL isin;

	for (i=0;i<nBlocks;i++,t++) {
		PrepareModifyPoly(i);
		for (j=0;j<=4;j+=2) {
			np=poly[i].sz[j];
			p=poly[i].poly[j];
			for (k=0;k<np;k++,p++) {
				// is it passable ?
				if (j==4) isin=((t->polyData[k].flags&0x0f)%14!=0);
				else {
					mind=(float)1E20;
					pt.x=65536*(t->vert[p->vertex[0]].x+t->vert[p->vertex[2]].x)/2;
					pt.z=65536*(t->vert[p->vertex[0]].z+t->vert[p->vertex[2]].z)/2;
					pt.y=65536*(t->vert[p->vertex[0]].y+t->vert[p->vertex[2]].y)/2;
					for (l=t->nStartPos;l<t->nStartPos+t->nPositions;l++) {
						v=col.vroad+l;
						dist=(pt.x-v->refPt.x)*(pt.x-v->refPt.x)
							+(pt.y-v->refPt.y)*(pt.y-v->refPt.y)
							+(pt.z-v->refPt.z)*(pt.z-v->refPt.z);
						if (dist<mind) { 
							mind=dist;
							ll=((pt.x-v->refPt.x)*v->right.x+
								(pt.y-v->refPt.y)*v->right.y+
								(pt.z-v->refPt.z)*v->right.z)/128;
							isin=((ll>-v->leftWall)&&(ll<v->rightWall));
						}
					}
				}
				if (isin==drivable) {
					p->texture=texture;
					if (bHSMode) p->hs_textflags=flags;
				}
			}
		}
	}
}

void CT3EDDoc::DelAllObjects(int blk,int isxobj)
{
	struct XOBJBLOCK *xb;
	int i,j,k,n;
	struct OBJPOLYBLOCK *o;
	struct TRKBLOCK *t;
	LPPOLYGONDATA p;

	PrepareModifyTrk(blk);
	PrepareModifyPoly(blk);
	if (isxobj) {
		for (i=0;i<4;i++) {
			PrepareModifyXobj(4*blk+i);
			xb=&(xobj[4*blk+i]);
			for (j=0;j<xb->nobj;j++) {
				if (xb->obj[j].crosstype==3) 
					dofree(xb->obj[j].animData);
				dofree(xb->obj[j].vert);
				dofree(xb->obj[j].unknVertices);
				dofree(xb->obj[j].polyData);
			}
			xb->nobj=0;
			o=&(poly[blk].obj[i]);
			if (o->n1>0) {
				o->n2=o->nobj;
				for (j=0;j<o->n2;j++) o->types[j]=1;
			}
		}
		trk[blk].nXobj=0;
		// update global xobj sequence numbers
		n=0;
		for (i=0;i<nBlocks;i++)
			for (j=0;j<trk[i].nXobj;j++)
				trk[i].xobj[j].globalno=n++;
	} else {
		t=&(trk[blk]);
		for (i=0;i<7;i++)
			for (j=0,p=poly[blk].poly[i];j<poly[blk].sz[i];j++,p++)
				for (k=0;k<4;k++) 
					p->vertex[k]-=(short)t->nObjectVert;
		t->nVertices-=t->nObjectVert; t->nVerticesDup-=t->nObjectVert;
		t->nLoResVert-=t->nObjectVert; t->nHiResVert-=t->nObjectVert; 
		t->nMedResVert-=t->nObjectVert;
		memmove(t->vert,t->vert+t->nObjectVert,12*t->nVertices);
		memmove(t->unknVertices,t->unknVertices+t->nObjectVert,4*t->nVertices);
		t->nObjectVert=0;
		dofree(t->lightsrc); t->lightsrc=NULL;
		t->nLightsrc=0;
		for (i=0,o=poly[blk].obj;i<4;i++,o++) {
			for (j=0;j<o->nobj;j++) dofree(o->poly[j]);
			o->n1=0; // block is marked unused ; no memory leak here
			o->nobj=0;
		}
	}
	RecalcBoundingBox(blk);
}

void CT3EDDoc::DelLanePolygons(int blk)
{
	int i,j,k,l,chunk,no;
	short vno;
	LPPOLYGONDATA p;
	struct POLYGONBLOCK *pp;
	struct TRKBLOCK *t;
	BOOL used;

	pp=&(poly[blk]);
	t=&(trk[blk]);
	PrepareModifyTrk(blk);
	PrepareModifyPoly(blk);
	
	for (chunk=0;chunk<=6;chunk++) {
		if ((chunk==0)||(chunk==2)||(chunk==4)) continue;
		for (no=0;no<pp->sz[chunk];no++) {
			for (i=0;i<4;i++) {
				vno=pp->poly[chunk][no].vertex[i];
				if (vno<0) continue;
				// don't delete vertex if referenced elsewhere
				used=FALSE;
				for (k=0;k<=4;k+=2) {
					p=pp->poly[k];
					for (j=0;j<pp->sz[k];j++,p++) if ((k!=chunk)||(j!=no))
						if ((p->vertex[0]==vno)||(p->vertex[1]==vno)||
							(p->vertex[2]==vno)||(p->vertex[3]==vno))
							used=TRUE;
				}
				if (used) continue;
				for (l=0;l<=6;l++) {
					p=pp->poly[l];
					for (j=0;j<pp->sz[l];j++,p++) for (k=0;k<4;k++) {
						if (p->vertex[k]==vno) p->vertex[k]=-1;
						else if (p->vertex[k]>vno) p->vertex[k]--;
					}
				}
				t->nVertices--; t->nVerticesDup--;
				if (vno<t->nLoResVert) t->nLoResVert--;
				if (vno<t->nMedResVert) t->nMedResVert--;
				if (vno<t->nHiResVert)  t->nHiResVert--;
				memmove(t->vert+vno,t->vert+vno+1,12*(t->nVertices-vno));
				memmove(t->unknVertices+vno,t->unknVertices+vno+1,4*(t->nVertices-vno));
			}
		}
		pp->sz[chunk]=0; pp->szdup[chunk]=0;
		dofree(pp->poly[chunk]);
		pp->poly[chunk]=NULL;
	}
	// soundsrc's come with fences
	dofree(t->soundsrc); t->soundsrc=NULL;
	t->nSoundsrc=0;
}

void CT3EDDoc::ClearSceneryZ()
{
	int i,start,min2,max2,ref,j,n;
	struct FLOATPT *v,tpt,tmppt;
	struct INTPT *p1,*p2;

	n=col.vroadHead.nrec;
	for (i=0;i<nBlocks;i++) {
		PrepareModifyTrk(i);
		v=trk[i].vert+trk[i].nObjectVert;
		start=trk[i].nStartPos+trk[i].nPositions/2;
		min2=(trk[i].nStartPos+n-8)%n;
		max2=(trk[i].nStartPos+trk[i].nPositions+7)%n;
		for (j=trk[i].nObjectVert;j<trk[i].nVertices;j++,v++) {
			if (fabs(v->x)>1E7) continue;
			if (fabs(v->y)>1E7) continue;
			ref=GlobalLocalCoord(*v,tpt,col.vroad,start,min2,max2);
			tpt.z=0;
			LocalGlobalCoord(tpt,tmppt,col.vroad,ref);
			v->z=tmppt.z;
		}
		p1=&(col.vroad[trk[i].nStartPos].refPt);
		p2=&(col.vroad[trk[i].nStartPos+trk[i].nPositions-1].refPt);
		tmppt.x=(float)(p2->x-p1->x);
		tmppt.z=(float)(p2->z-p1->z);
		tmppt.y=(float)(p2->y-p1->y);
		for (j=0;j<trk[i].nPolygons;j++) RecalcPolyVroad(i,j,&tmppt);
	}
}

void CT3EDDoc::DelTrackPolygon(int blk,int no)
{
	LPPOLYGONDATA p;
	struct POLYGONBLOCK *pp;
	struct TRKBLOCK *t;
	short *v;
	BOOL left,right;
	int i,j,k;
	struct FLOATPT a,b;

	pp=&(poly[blk]);
	t=&(trk[blk]);
	p=pp->poly[4]+no-1;
	left=right=FALSE;
	if (no>0) left=(p[0].vertex[1]==p[1].vertex[0])&&(p[0].vertex[2]==p[1].vertex[3]);
	if (no<pp->sz[4]-1) right=(p[1].vertex[1]==p[2].vertex[0])&&(p[1].vertex[2]==p[2].vertex[3]);
	if (!left&&!right) {
		if (AfxMessageBox("You are about to delete an isolated track polygon.\n"
			              "If this is the last polygon of a road node,\n"
						  "the resulting track will not work properly.\n"
						  "Are you sure you want to proceed ?",
			MB_ICONWARNING|MB_OKCANCEL)!=IDOK) return;
	}
	PrepareModifyTrk(blk);
	PrepareModifyPoly(blk);
	for (i=0;i<4;i++) SetCollisionHandling(t->vert+p[1].vertex[i]);
	if (bHSMode&&left)
		if ((t->polyData[no-1].flags&0x0f)%14) {
			t->polyData[no-1].hs_orphan[3]=0xFF;
			if (right) if ((t->polyData[no+1].flags&0x0f)%14!=0)
				t->polyData[no-1].hs_orphan[3]=0;
		}
	if (bHSMode&&right)
		if ((t->polyData[no+1].flags&0x0f)%14) {
			t->polyData[no+1].hs_orphan[1]=0xFF;
			if (left) if ((t->polyData[no-1].flags&0x0f)%14!=0)
				t->polyData[no+1].hs_orphan[1]=0;
		}

	if (left&&right) {
		// do some magic to reconnect the side polygons
		for (i=0;i<7;i++) for (j=0,p=pp->poly[i];j<pp->sz[i];j++,p++)
			for (k=0;k<4;k++)
				if (p->vertex[k]>=t->nHiResVert) p->vertex[k]+=2;
		p=pp->poly[4]+no-1;
		t->nVertices+=2; t->nVerticesDup+=2;
		t->nHiResVert+=2;
		t->vert=(struct FLOATPT *)realloc(t->vert,12*t->nVertices);
		t->unknVertices=(unsigned long *)realloc(t->unknVertices,4*t->nVertices);
		memmove(t->vert+t->nHiResVert,t->vert+t->nHiResVert-2,12*(t->nVertices-t->nHiResVert));
		memmove(t->unknVertices+t->nHiResVert,t->unknVertices+t->nHiResVert-2,4*(t->nVertices-t->nHiResVert));
		v=p[1].vertex;
		t->unknVertices[t->nHiResVert-2]=t->unknVertices[v[0]];
		t->unknVertices[t->nHiResVert-1]=t->unknVertices[v[2]];
		a.x=(t->vert[v[0]].x+t->vert[v[1]].x)/2;
		a.z=(t->vert[v[0]].z+t->vert[v[1]].z)/2;
		a.y=(t->vert[v[0]].y+t->vert[v[1]].y)/2;
		b.x=(t->vert[v[2]].x+t->vert[v[3]].x)/2;
		b.z=(t->vert[v[2]].z+t->vert[v[3]].z)/2;
		b.y=(t->vert[v[2]].y+t->vert[v[3]].y)/2;
		t->vert[t->nHiResVert-2].x=(19*a.x+b.x)/20;
		t->vert[t->nHiResVert-2].z=(19*a.z+b.z)/20;
		t->vert[t->nHiResVert-2].y=(19*a.y+b.y)/20;
		t->vert[t->nHiResVert-1].x=(19*b.x+a.x)/20;
		t->vert[t->nHiResVert-1].z=(19*b.z+a.z)/20;
		t->vert[t->nHiResVert-1].y=(19*b.y+a.y)/20;
		p[0].vertex[1]=t->nHiResVert-2;
		p[0].vertex[2]=t->nHiResVert-1;
		p[2].vertex[0]=t->nHiResVert-2;
		p[2].vertex[3]=t->nHiResVert-1;
		RecalcPolyVroad(blk,no-1);
		RecalcPolyVroad(blk,no+1);
	}

	DelPolygon(blk,FALSE,4,-1,no); // go through the usual DelPolygon procedure
 }


int CT3EDDoc::GetCenterQuad(int blockIdx, int nodeIdx)
{
	ASSERT(blockIdx < nBlocks);
	ASSERT(nodeIdx < trk[blockIdx].nPositions);
	int absNodeIdx = blockIdx * 8 + nodeIdx;

	POSITIONDATA& posd = trk[blockIdx].posData[nodeIdx];
	COLVROAD_VIT& vr = *(COLVROAD_VIT*)&col.vroad[absNodeIdx];

	LPPOLYGONDATA p = poly[blockIdx].poly[4] + posd.polygon;
	FLOATPT *v = trk[blockIdx].vert;

	int num = posd.nPolygons;
	CFltVector right(vr.right.x / 128.0f, vr.right.z / 128.0f, vr.right.y / 128.0f);

	int center = num;
	for (int j = 0; j < num; j++, p++) 
	{
		CFltVector delta = (*(CFltVector*)(&v[p->vertex[0]]) + *(CFltVector*)(&v[p->vertex[1]])) * 0.5 - CFltVector(vr.refPt.x / 65536.0f, vr.refPt.z / 65536.0f, vr.refPt.y / 65536.0f);
		float smul = delta | right;
		if (smul > 0)
		{
			center = j - 1;
			break;
		}
	}

	return center;
}

void CT3EDDoc::CorrectVirtualRoad(int blockIdx, int nodeIdx)
{
	ASSERT(blockIdx < nBlocks);
	ASSERT(nodeIdx < trk[blockIdx].nPositions);
	int absNodeIdx = blockIdx * 8 + nodeIdx;

	POSITIONDATA& posd = trk[blockIdx].posData[nodeIdx];
	COLVROAD_VIT& vr = *(COLVROAD_VIT*)&col.vroad[absNodeIdx];

	LPPOLYGONDATA p = poly[blockIdx].poly[4] + posd.polygon;
	FLOATPT *v = trk[blockIdx].vert;

	int num = posd.nPolygons;
	int center = GetCenterQuad(blockIdx, nodeIdx);

	vr.FullMask = 0;
	for (int j = 0; j < num; j++, p++) 
	{
		POLYVROADDATA& pvr = trk[blockIdx].polyData[posd.polygon + j];
		char ptype = pvr.flags & 0x0f;
		unsigned long _bit = 0x8000L >> (j+7 - center);
		if (ptype != 0 && ptype != 14)
			vr.FullMask |= _bit;
	}
}

void CT3EDDoc::CorrectVirtualRoad()
{
	for (int i = 0; i < nBlocks; i++)
		for (int j = 0; j < trk[i].nPositions; j++)
			CorrectVirtualRoad(i, j);
}

int CT3EDDoc::DuplTrackPolygon(int blk,int no)
{
	int i,j,k,l;
	LPPOLYGONDATA p;
	struct POLYGONBLOCK *pp;
	struct TRKBLOCK *t;
	struct FLOATPT a,b;
	short *v;

	pp=&(poly[blk]);
	t=&(trk[blk]);
	PrepareModifyTrk(blk);
	PrepareModifyPoly(blk);

	t->vert=(struct FLOATPT *)realloc(t->vert,12*(t->nVertices+2));
	t->unknVertices=(unsigned long *)realloc(t->unknVertices,4*(t->nVertices+2));
	// create 2 new vertices at nHiResVert
	memmove(t->vert+t->nHiResVert+2,t->vert+t->nHiResVert,12*(t->nVertices-t->nHiResVert));
	memmove(t->unknVertices+t->nHiResVert+2,t->unknVertices+t->nHiResVert,4*(t->nVertices-t->nHiResVert));
	for (l=0;l<=6;l++) {
		p=pp->poly[l];
		for (j=0;j<pp->sz[l];j++,p++) 
			for (k=0;k<4;k++)
				if (p->vertex[k]>=t->nHiResVert) p->vertex[k]+=2;
	}
	t->nHiResVert+=2;
	t->nVertices+=2; t->nVerticesDup+=2;
	pp->poly[4]=(struct POLYGONDATA *)realloc(pp->poly[4],14*(pp->sz[4]+1));
	p=pp->poly[4]+no;
	memmove(p+1,p,14*(pp->sz[4]-no));
	pp->sz[4]++; pp->szdup[4]++;

	v=p->vertex;
	t->unknVertices[t->nHiResVert-2]=t->unknVertices[v[0]];
	t->unknVertices[t->nHiResVert-1]=t->unknVertices[v[2]];
	a.x=(t->vert[v[0]].x+t->vert[v[1]].x)/2;
	a.z=(t->vert[v[0]].z+t->vert[v[1]].z)/2;
	a.y=(t->vert[v[0]].y+t->vert[v[1]].y)/2;
	b.x=(t->vert[v[2]].x+t->vert[v[3]].x)/2;
	b.z=(t->vert[v[2]].z+t->vert[v[3]].z)/2;
	b.y=(t->vert[v[2]].y+t->vert[v[3]].y)/2;
	t->vert[t->nHiResVert-2].x=(19*a.x+b.x)/20;
	t->vert[t->nHiResVert-2].z=(19*a.z+b.z)/20;
	t->vert[t->nHiResVert-2].y=(19*a.y+b.y)/20;
	t->vert[t->nHiResVert-1].x=(19*b.x+a.x)/20;
	t->vert[t->nHiResVert-1].z=(19*b.z+a.z)/20;
	t->vert[t->nHiResVert-1].y=(19*b.y+a.y)/20;
	p[0].vertex[1]=t->nHiResVert-2;
	p[0].vertex[2]=t->nHiResVert-1;
	p[1].vertex[0]=t->nHiResVert-2;
	p[1].vertex[3]=t->nHiResVert-1;
	
	// update TRKBLOCK too
	t->nPolygons++;
	t->polyData=(struct POLYVROADDATA *)realloc(t->polyData,sizeof(struct POLYVROADDATA)*t->nPolygons);
	memmove(t->polyData+no+1,t->polyData+no,sizeof(struct POLYVROADDATA)*(t->nPolygons-1-no));
	for (i=0;i<t->nPositions;i++)
		if (t->posData[i].polygon>no) t->posData[i].polygon++;
	for (i=0;i<t->nPositions-1;i++)
		t->posData[i].nPolygons=t->posData[i+1].polygon-t->posData[i].polygon;
	t->posData[t->nPositions-1].nPolygons=t->nPolygons-t->posData[t->nPositions-1].polygon;
	RecalcPolyVroad(blk,no);
	RecalcPolyVroad(blk,no+1);
	if (bHSMode&&((t->polyData[no].flags&0x0f)%14!=0)) {
		t->polyData[no].hs_orphan[3]=0;
		t->polyData[no+1].hs_orphan[1]=0;
	}
	return no;
}

void CT3EDDoc::SetRoadVertexShading(int nStartBlk, int nEndBlk, unsigned long dwColorValue)
{
	ASSERT(nStartBlk >= 0 && nEndBlk < nBlocks && nStartBlk <= nEndBlk);
	for (int nBlk = nStartBlk; nBlk <= nEndBlk; nBlk++)
	{
		for (int nVert = 0; nVert < trk[nBlk].nVertices; nVert++)
		{
			trk[nBlk].unknVertices[nVert] = dwColorValue;
		}
	}
}

void CT3EDDoc::SetXObjVertexShading(int nStartBlk, int nEndBlk, unsigned long dwColorValue)
{
	ASSERT(nStartBlk >= 0 && nEndBlk < nBlocks && nStartBlk <= nEndBlk);
	for (int nBlk = nStartBlk; nBlk <= nEndBlk; nBlk++)
	{
		for (int nChunk = 0; nChunk < 4; nChunk++)
		{
			XOBJBLOCK& xobjBlk = xobj[nBlk*4+nChunk];
			for (int nObj = 0; nObj < xobjBlk.nobj; nObj++)
			{
				for (int nVert = 0; nVert < xobjBlk.obj[nObj].nVertices; nVert++)
				{
					xobjBlk.obj[nObj].unknVertices[nVert] = dwColorValue;
				}
			}
		}
	}
}

unsigned long CT3EDDoc::RGBTool(CString inmode, unsigned long dwColorValue, CString outmode)
{
	unsigned char BlueValue;
	unsigned char RedValue;
	unsigned char GreenValue;
	unsigned char AlphaValue; 
	unsigned long RGBAValue, BGRAValue;
	unsigned long temp,temp2,temp3;

	if (inmode=="BRGA") 
	{
		BlueValue=	static_cast<unsigned char>((dwColorValue&0xFF000000) >> 24);
		GreenValue=	static_cast<unsigned char>((dwColorValue&0x00FF0000) >> 16);
		RedValue=	static_cast<unsigned char>((dwColorValue&0x0000FF00) >> 8);
		AlphaValue=	static_cast<unsigned char>((dwColorValue&0x000000FF));
	
		temp=(dwColorValue>>16)&0x0000FF00;  //Blue...
		temp2=(dwColorValue<<16)&0xFF000000;  //Red...
		temp3=(dwColorValue&0x00FF00FF);  //Green and Alpha...
		RGBAValue=temp+temp2+temp3;
		BGRAValue=dwColorValue;
	}
	else if (inmode=="RGBA")
	{
		BlueValue=	static_cast<unsigned char>((dwColorValue&0xFF000000) >> 24);
		GreenValue=	static_cast<unsigned char>((dwColorValue&0x00FF0000) >> 16);
		RedValue=	static_cast<unsigned char>((dwColorValue&0x0000FF00) >> 8);
		AlphaValue=	static_cast<unsigned char>((dwColorValue&0x000000FF));
	
		temp=(dwColorValue<<16)&0xFF000000;  //Blue...
		temp2=(dwColorValue>>16)&0x0000FF00;  //Red...
		temp3=(dwColorValue&0x00FF00FF);  //Green and Alpha...
		BGRAValue=temp+temp2+temp3;
		RGBAValue=dwColorValue;
	}

	if (outmode=="R") return RedValue;
	if (outmode=="G") return GreenValue;
	if (outmode=="B") return BlueValue;
	if (outmode=="A") return AlphaValue;
	if (outmode=="RGBA") return RGBAValue;
	if (outmode=="BGRA") return BGRAValue;
	else return 0;
}


float CT3EDDoc::VectorAngle(struct FLOATPT Vect1, struct FLOATPT Vect2, struct FLOATPT Startpt)
{
	double Angle, Vect1Len, Vect2Len;
	//Normalization
	Vect1.x=Vect1.x-Startpt.x;
	Vect1.z=Vect1.z-Startpt.z;
	Vect1.y=Vect1.y-Startpt.y;
	
	Vect2.x=Vect2.x-Startpt.x;
	Vect2.z=Vect2.z-Startpt.z;
	Vect2.y=Vect2.y-Startpt.y;
	//Calculating Angle:
	Vect1Len=sqrt(pow(Vect1.x,2)+pow(Vect1.z,2)+pow(Vect1.y,2));
	Vect2Len=sqrt(pow(Vect2.x,2)+pow(Vect2.z,2)+pow(Vect2.y,2));
	Angle=((Vect1.x*Vect2.x)+(Vect1.z*Vect2.z)+(Vect1.y*Vect2.y))/(Vect1Len*Vect2Len);
	Angle=acos(Angle);
	Angle=(Angle*180)/3.14592653;

	return static_cast<float>(Angle);
}


int CT3EDDoc::PolysNode(int blk, int polynum)
{
	int j,num, node, strip;
	float Distance;
	LPPOLYGONDATA p, prepoly;
	//LPPOLYGONDATA p;
	struct FLOATPT Dist,DbugDist, *v;
	//short *v;
	CString keke;
	bool NewStrip;

	//pp=&(poly[blk]);
	//t=&(trk[blk]);
	//PrepareModifyTrk(blk);
	//PrepareModifyPoly(blk);
	node=0;
	v=trk[blk].vert;
	p=poly[blk].poly[4];
	num=poly[blk].sz[4];
	strip=1;
	//PreSaveVert=-1;
	//PreVert=-1;
	
	NewStrip=false;

	Dist.x=0;
	Dist.y=0;
	Dist.z=0;
	
	//lastpoly=0;
	DbugDist.x=1.0;
	DbugDist.y=1;
	DbugDist.z=1;

	prepoly=p;
	p++;
	for (j=1;j<num;j++,p++) 
	{
		Dist.x=(float)fabs(v[p->vertex[3]].x - v[prepoly->vertex[2]].x);
		Dist.y=(float)fabs(v[p->vertex[3]].y - v[prepoly->vertex[2]].y);
		Dist.z=(float)fabs(v[p->vertex[3]].z - v[prepoly->vertex[2]].z);
		Distance=(float)sqrt(pow(Dist.x,2)+pow(Dist.y,2)+pow(Dist.z,2));
		if (Distance >0.1) 
				NewStrip=true;
		
		prepoly=p; //updating next previous polygon.

		if (NewStrip==true)
		{
			//if (strip==node)
			//	node=j;
			//else 
			//{
				strip++;
				//lastpoly=0;
				NewStrip=false;
			//}
		}
		if (j==polynum) node=strip-1;
		
	}
//	if (lastpoly==0) lastpoly=num
return node;		
}

CString CT3EDDoc::MedDetailFixer(int blk, int AngleLimit)
{
	int j,k,num, num2, strip, temp, check, check2, check3;
	int Detail1, Detail2;
	float Distance;
	LPPOLYGONDATA p, p2, p2end;
	//LPPOLYGONDATA p;
	struct FLOATPT Dist, *v,*vv;
	//short *v;
	CString keke;
	long TempVert[4], nMedPolys, Error, NumPolyPerStrip[8], HStrips[1024];
	//bool MedVert[9][127];
	bool NewStrip, GenerateMesh[8], SpecAct=false;
	

	//pp=&(poly[blk]);
	//t=&(trk[blk]);
	//PrepareModifyTrk(blk);
	//PrepareModifyPoly(blk);
	//k=1;
	if ( AngleLimit==1)
	{
		Detail1=4;
		Detail2=2;
	}
	else if (AngleLimit==2)
	{
		Detail1=2;
		Detail2=0;
	}
	else return ("ERROR!!");

	
	for (j=0;j<1024;j++)
	{
		HStrips[j]=-1;
	}

	
	
	v=trk[blk].vert;
	vv=trk[blk].vert;
	
	p2=poly[blk].poly[Detail2];
	num2=poly[blk].sz[Detail2];

	p=poly[blk].poly[Detail1];
	num=poly[blk].sz[Detail1];
	
	strip=1;
	//PreSaveVert=-1;
	//PreVert=-1;
	//i=2;
	Error=0;
	NewStrip=false;

	//Dist.x=1;
	//Dist.y=1;
	//Dist.z=1;
	
	/*DbugDist.x=1.0;
	DbugDist.y=1;
	DbugDist.z=1;*/

	TempVert[3]=p->vertex[3];
	TempVert[2]=p->vertex[2];
	TempVert[1]=p->vertex[1];
	TempVert[0]=p->vertex[0];
	HStrips[0]=strip;
	p++;
	for (j=1;j<num;j++,p++) 
	{
		Dist.x=(float)fabs(v[p->vertex[3]].x - v[TempVert[2]].x);
		Dist.y=(float)fabs(v[p->vertex[3]].y - v[TempVert[2]].y);
		Dist.z=(float)fabs(v[p->vertex[3]].z - v[TempVert[2]].z);
		Distance=(float)sqrt(pow(Dist.x,2)+pow(Dist.y,2)+pow(Dist.z,2));
		if (Distance >0.1) 
				NewStrip=true;
		
		/*if (j==num-1)
		{
			TempVert[7][0]=i+1;
			TempVert[8][0]=i+1;
		}*/		
		
		TempVert[3]=p->vertex[3];
		TempVert[2]=p->vertex[2];
		TempVert[1]=p->vertex[1];
		TempVert[0]=p->vertex[0];

		if (NewStrip==false)
		{
			keke="OK!";
			/*TempVert[strip][i]=p->vertex[3];
			TempVert[strip][i+1]=p->vertex[2];
			TempVert[strip+1][i]=p->vertex[1];
			TempVert[strip+1][i+1]=p->vertex[0];*/
		}
			//TempVert[7][0]=i+1;
			//TempVert[8][0]=i+1;
		else 
		{
			/*if ((strip==0) && (j==1))
			{
				DbugDist.x=Dist.x;
				DbugDist.y=Dist.y;
				//Dist.z=-1000;
			}*/

			NewStrip=false;
			/*if (strip==0)
				TempVert[strip][0]=i;
			else
				TempVert[strip][0]=i+1;*/
			strip++;
			if (strip>8)
			{
				strip=7;
				if (Error==0)
					Error=j;
			}
			/*i=1;
			TempVert[strip][i]=p->vertex[3];
			TempVert[strip][i+1]=p->vertex[2];
			TempVert[strip+1][i]=p->vertex[1];
			TempVert[strip+1][i+1]=p->vertex[0];*/

		}
		HStrips[j]=strip;
		

		/*if (j==num-1)
		{
			TempVert[7][0]=i+1;
			TempVert[8][0]=i+1;
		}*/	
	
	/*
		//pDC->MoveTo(FloatX(v[p->vertex[0]]),FloatY(v[p->vertex[0]]));
		//pDC->LineTo(FloatX(v[p->vertex[1]]),FloatY(v[p->vertex[1]]));
		//pDC->LineTo(FloatX(v[p->vertex[2]]),FloatY(v[p->vertex[2]]));
		//pDC->LineTo(FloatX(v[p->vertex[3]]),FloatY(v[p->vertex[3]]));
		//pDC->LineTo(FloatX(v[p->vertex[0]]),FloatY(v[p->vertex[0]])); */
	
	}

	//Medium Meshin polycountin selvitys:
	for (j=0;j<8;j++)
	{
		NumPolyPerStrip[j]=0;
		GenerateMesh[j]=false;
		//SpecialMode[j]=false;
	}

	for (j=0;j<1024;j++)
	{
		if (HStrips[j]!=-1)
			NumPolyPerStrip[HStrips[j]-1]++;
	}
	
	
	nMedPolys=0;
	if (NumPolyPerStrip[0]==NumPolyPerStrip[1])
	{
		nMedPolys=NumPolyPerStrip[0];
		GenerateMesh[0]=true;
		GenerateMesh[1]=true;
	}
	else nMedPolys=NumPolyPerStrip[0]+NumPolyPerStrip[1];
		


	
	if (NumPolyPerStrip[2]==NumPolyPerStrip[3])
	{
		nMedPolys=nMedPolys+NumPolyPerStrip[2];
		GenerateMesh[2]=true;
		GenerateMesh[3]=true;
	}
	else  nMedPolys=nMedPolys+NumPolyPerStrip[2]+NumPolyPerStrip[3];
		


	
	if (NumPolyPerStrip[4]==NumPolyPerStrip[5])
	{
		nMedPolys=nMedPolys+NumPolyPerStrip[4];
		GenerateMesh[4]=true;
		GenerateMesh[5]=true;
	}
	else nMedPolys=nMedPolys+NumPolyPerStrip[4]+NumPolyPerStrip[5];




	if (NumPolyPerStrip[6]==NumPolyPerStrip[7])
	{
		nMedPolys=nMedPolys+NumPolyPerStrip[6];
		GenerateMesh[6]=true;
		GenerateMesh[7]=true;
	}
	else nMedPolys=nMedPolys+NumPolyPerStrip[6]+NumPolyPerStrip[7];
		


	

	// MediumMeshin polygonim‰‰r‰n tasaus:
	if (nMedPolys == 0 )
		nMedPolys++;
	
	while (num2> nMedPolys)
	{
		DelPolygon(blk,0,Detail2,-1,num2-1);
		num2=poly[blk].sz[Detail2];
	}
	
	
	while (num2< nMedPolys)
	{
		temp=DuplPolygon(blk,0,Detail2,-1,0);
		num2=poly[blk].sz[Detail2];	
	}



	
	
	//Medium Meshin generointi:
	p2=poly[blk].poly[Detail2];
	num2=poly[blk].sz[Detail2];
	
	p2end=poly[blk].poly[Detail2];
	p=poly[blk].poly[Detail1];
	num=poly[blk].sz[Detail1];

	v=trk[blk].vert;
	check=0;
	check2=0;
	check3=0;
	//vv=trk[blk].vert;
	for (j=0;j<num;j++,p++) 
	{
		
		
		if (GenerateMesh[HStrips[j]-1]==true)
		{ 
			//tarkistetaan ett‰ molemmissa stripeiss‰ on sama m‰‰r‰ polyja
			//if (NumPolyPerStrip[HStrips[j]-1]==NumPolyPerStrip[HStrips[j]])
			if (HStrips[j]==1 || HStrips[j]==3 || HStrips[j]==5 || HStrips[j]==7)  
			{
				check++;
				p2->texture=p->texture;
				p2->texanimdata=p->texanimdata;
				p2->hs_textflags=p->hs_textflags;

				p2->vertex[2] = p->vertex[2];
				p2->vertex[3] = p->vertex[3];
				p2++;
				//if (SpecialMode[HStrips[j]-1]==false) p2++;
			}
			
			else if (HStrips[j]==2 || HStrips[j]==4 || HStrips[j]==6 || HStrips[j]==8)
			{
				check2++;
				p2end->vertex[0] = p->vertex[0];
			
				p2end->vertex[1] = p->vertex[1];
				p2end++;
				//if (SpecialMode[HStrips[j]-1]==false) p2++;
			}

			
		}
		else
		{
				check3++;
				p2->texture=p->texture;
				p2->texanimdata=p->texanimdata;
				p2->hs_textflags=p->hs_textflags;

				p2->vertex[2] = p->vertex[2];
				p2->vertex[3] = p->vertex[3];

				p2->vertex[0] = p->vertex[0];
				p2->vertex[1] = p->vertex[1];
				p2++;
				p2end++;
		}


		
		
	}
	//Low Mesh:
	
	/*
	nLowPolys=0;
	if (GenerateMesh[0]==true && GenerateMesh[1]==true && GenerateMesh[2]==true && GenerateMesh[3]==true)
		nLowPolys=NumPolyPerStrip[0];
	


	if (GenerateMesh[4]==true && GenerateMesh[5]==true && GenerateMesh[6]==true && GenerateMesh[7]==true)
		nLowPolys=nLowPolys+NumPolyPerStrip[4];

	*/
	
	
	
	/*
	p2=poly[blk].poly[2];
	num2=poly[blk].sz[2];

	p=poly[blk].poly[4];
	
	num=poly[blk].sz[4];
	
	
	for (j=0;j<num;j++,p++) //num;j++,p++) 
	{
		
		if (HStrips[j]==2 || HStrips[j]==4 || HStrips[j]==6 || HStrips[j]==8)  
		{
			
			//p2->texture=p->texture;
			//MergePoint(&v[p2->vertex[0]], &v[p->vertex[0]]);
			//tarkistetaan ett‰ molemmissa stripeiss‰ on sama m‰‰r‰ polyja
			//if (NumPolyPerStrip[HStrips[j]-1]==NumPolyPerStrip[HStrips[j]-2])
			if (GenerateMesh[HStrips[j]-1]==true)
			{
				check2++;
				p2->vertex[0] = p->vertex[0];
			
				p2->vertex[1] = p->vertex[1];
				p2++;
				//if (SpecialMode[HStrips[j]-1]==false) p2++;
			}
			

				//NumPolyPerStrip[HStrips[j]-2] kertoo montako polya t‰ss‰ stripiss‰ pit‰‰ olla.
				//NumPolyPerStrip[HStrips[j]-1] kertoo montako siin‰ on nyt.
				//HStrips[polygonnumber(0 to num-1)] kertoo polygonin sijainnin Strip tasolla. (1-8)
				//NumPolyPerStrip[stripnumber (0 to 7)] kertoo polygonien m‰‰r‰n stripiss‰.

				//1. on selvitett‰v‰ polygonin naapuruus.
				//2. naapuruus taulukon avulla connectoitava oikea m‰‰r‰ polygoneja oikeisiin
				// naapureihin.
			
		}
		
	}
	
	//tarkistetaan tarvittava lis‰detailin m‰‰r‰...
	ExtraMesh=0;
	j=0;
	while (j<8)
	{
		if (GenerateMesh[j]=false) 
			ExtraMesh=ExtraMesh+NumPolyPerStrip[j];
		j++;
	}
	
	LastMedDet=num2-1;
	while (num2< nMedPolys+ExtraMesh)
	{
		temp=DuplPolygon(blk,0,2,-1,LastMedDet);
		num2=poly[blk].sz[2];	
	}
	

	
	
	p2=poly[blk].poly[2]; //ei saa nollata koska polygon stack menee sekasin
	num2=poly[blk].sz[2];
	j=0;
	while (j<LastMedDet+1)
	{
		p2++;
		j++;
	}
	
	//check3=check2;//-1;
	p=poly[blk].poly[4];
	num=poly[blk].sz[4];

	v=trk[blk].vert;
	//vv=trk[blk].vert;
	for (j=0;j<num;j++,p++) 
	{
		  
		 
			//tarkistetaan ett‰ molemmissa stripeiss‰ on sama m‰‰r‰ polyja
			//if (NumPolyPerStrip[HStrips[j]-1]==NumPolyPerStrip[HStrips[j]])
			if (GenerateMesh[HStrips[j]-1]==false)
			{
				check3++;
				p2->texture=p->texture;
				p2->flags=p->flags;
				
				p2->vertex[2] = p->vertex[2];
				p2->vertex[3] = p->vertex[3];

				p2->vertex[0] = p->vertex[0];
				p2->vertex[1] = p->vertex[1];
				p2++; 
			}
			
		
		
		
		
	} */

	
	
	
	
	//TempVert[][] created
	
	//Creating MedVert bool array for new Vertex values...
	
	k=180-AngleLimit;
	AngleLimit=k;
	//nMedVertexes=0;
	//MStrips
	
	/*for (j=0;j<9;j++)
	{
		//if ((j==0) || (j==2) || (j==4) || (j==6) || (j==8))
		//{
			MedVert[j][1]=true;
			MedVert[j][TempVert[j][0]]=true;
			nMedVertexes=nMedVertexes+2;
		//}

		for (i=2;i<TempVert[j][0]-1;i++)
		{
			//nMedPolys++;
			Angle=VectorAngle(v[TempVert[j][i-1]], v[TempVert[j][i+1]], v[TempVert[j][i]]);
			if (Angle<AngleLimit)
			{
				MedVert[j][i]=true;
				nMedVertexes++;
			}
		}
		//if (j!=8)
		//{
			nMedVertSlice[j]=nMedVertexes;
			nMedVertexes=0;
		//}
	}
	nMedVertexes=0;
	nMedPolys=0;
	for (k=0;k<9;k++)
	{
	nMedVertexes=nMedVertexes+nMedVertSlice[k];
	if (k!=8)
		nMedPolys=nMedPolys+(nMedVertSlice[k]-1);
	}
	//nMedVertexes=nMedVertSlice[0]+nMedVertSlice[2]+nMedVertSlice[4]+nMedVertSlice[6]+nMedVertSlice[8];
	//nMedPolys=(nMedVertSlice[0])+(nMedVertSlice[2]-1)+(nMedVertSlice[4]-1)+(nMedVertSlice[6]-1);
	//Vertex values ready for Med Detail...
	
	//checking amount of needed polygons to full fill the block.
	//v=trk[blk].vert;
	p2=poly[blk].poly[2];
	num2=poly[blk].sz[2];

	MoreLess=num2-nMedPolys;
	if (MoreLess!=0)
	{
		if (MoreLess<0)
		{
		}
		else
		{
		}
	}
	
	/*keke.Format(" Error:%d, strip: %d, Num: %d, nMedPolys: %d, nMedVertexes: %d, MoreLess: %d \n nVert/strip: %d, %d, %d, %d, %d, %d, %d, %d, %d\nVectorAngle: %f", 
		Error, strip, num, nMedPolys, nMedVertexes, MoreLess, TempVert[0][0], TempVert[1][0], TempVert[2][0], 
		TempVert[3][0], TempVert[4][0], TempVert[5][0], TempVert[6][0], TempVert[7][0], 
		TempVert[8][0], VectorAngle(v[TempVert[0][1]], v[TempVert[0][3]], v[TempVert[0][2]]));*/
	
	keke.Format(" Error:%d, strip: %d, Num: %d, J: %d,Chk: %d, %d + %d = %d NumPolyPerStrip[0-7]: %d, %d, %d, %d, %d, %d, %d, %d, nMedPolys: %d, Num: %d, GenMesh: %d, %d, %d, %d, %d, %d, %d, %d", 
		Error, strip, num, j,check, check2, check3, (check2+check3), NumPolyPerStrip[0], NumPolyPerStrip[1], NumPolyPerStrip[2], NumPolyPerStrip[3], NumPolyPerStrip[4], NumPolyPerStrip[5], 
		NumPolyPerStrip[6], NumPolyPerStrip[7], nMedPolys, num2, GenerateMesh[0],GenerateMesh[1],GenerateMesh[2],GenerateMesh[3],GenerateMesh[4],GenerateMesh[5],GenerateMesh[6],GenerateMesh[7]);
	
	return keke;
}


CString CT3EDDoc::VertArrayFixer(int blk)
{
	int k,koo,l,m, num;
	long VertCount, LowDetail, MediumDetail, HighDetail, counter;
	short previousnode;
	bool dippa;
	LPPOLYGONDATA p;
	//LPPOLYGONDATA p;
	struct TRKBLOCK *t;
	struct FLOATPT *v;
	//short *v;
	CString keke;
	
	t=&trk[blk];
	//t->nLoResVert=t->nHiResVert; 
	//t->nMedResVert=t->nHiResVert;
	v=trk[blk].vert;
	short Newoneslow[2048]; 
	short Newonesmed[2048];
	short Newoneshigh[2048];
	
	for (k=0;k<2048;k++)
	{
		Newoneslow[k]=-1;
		Newonesmed[k]=-1;
		Newoneshigh[k]=-1;
	}
	
	v=trk[blk].vert;
	//vv=trk[blk].vert;
	
	p=poly[blk].poly[0];
	num=poly[blk].sz[0];
	
	counter=0;
	koo=0;
	for (k=0;k<num;k++,p++)
	{
		for (l=0;l<4;l++)
		{
			//counter=koo+1;
			dippa=true;
			for (m=0;m<koo+1;m++) 
			{
				
				if (Newoneslow[m]==p->vertex[l])
				{
					dippa=false;
					m=koo+1;
				}
			}
			if ((dippa==true) && (p->vertex[l]>t->nObjectVert-1))
			{
				Newoneslow[koo]=p->vertex[l];
				//Newonesmed[koo]=p->vertex[l];
				//Newoneshigh[koo]=p->vertex[l];
				koo++;
			}
		}
	}	
	
	
	
	p=poly[blk].poly[1];
	num=poly[blk].sz[1];
	for (k=0;k<num;k++,p++)
	{
		for (l=0;l<4;l++)
		{
			//counter=koo+1;
			dippa=true;
			for (m=0;m<koo+1;m++) 
			{
				
				if (Newoneslow[m]==p->vertex[l])
				{
					dippa=false;
					//m=koo+1;
				}
			}
			if ((dippa==true) && (p->vertex[l]>t->nObjectVert-1))
			{
				Newoneslow[koo]=p->vertex[l];
				//Newonesmed[koo]=p->vertex[l];
				//Newoneshigh[koo]=p->vertex[l];
				koo++;
			}
		}
	}
	LowDetail=koo-1;
	
	koo=0;
	p=poly[blk].poly[2];
	num=poly[blk].sz[2];
	for (k=0;k<num;k++,p++)
	{
		for (l=0;l<4;l++)
		{
			//counter=koo+1;
			dippa=true;
			for (m=0;m<500;m++) 
			{
				
				if ((Newonesmed[m]==p->vertex[l]) || (Newoneslow[m]==p->vertex[l]))
				{
					dippa=false;
					//m=koo+1;
				}
			}
			if ((dippa==true) && (p->vertex[l]>t->nObjectVert-1))
			{

				Newonesmed[koo]=p->vertex[l];
				//Newoneshigh[koo]=p->vertex[l];
				koo++;
			}
		}
	}
	
	p=poly[blk].poly[3];
	num=poly[blk].sz[3];
	for (k=0;k<num;k++,p++)
	{
		for (l=0;l<4;l++)
		{
			//counter=koo+1;
			dippa=true;
			for (m=0;m<500;m++) 
			{
				
				if ((Newonesmed[m]==p->vertex[l]) || (Newoneslow[m]==p->vertex[l]))
				{
					dippa=false;
					//m=koo+1;
				}
			}
			if ((dippa==true) && (p->vertex[l]>t->nObjectVert-1))
			{

				Newonesmed[koo]=p->vertex[l];
				//Newoneshigh[koo]=p->vertex[l];
				koo++;
			}
		}
	}
	MediumDetail=koo-1;
	koo=0;
	p=poly[blk].poly[4];
	num=poly[blk].sz[4];
	for (k=0;k<num;k++,p++)
	{
		for (l=0;l<4;l++)
		{
			//counter=koo+1;
			dippa=true;
			for (m=0;m<500;m++) 
			{
				
				if ((Newonesmed[m]==p->vertex[l]) || (Newoneslow[m]==p->vertex[l]) || (Newoneshigh[m]==p->vertex[l]))
				{
					dippa=false;
					//m=koo+1;
				}
			}
			if ((dippa==true) && (p->vertex[l]>t->nObjectVert-1))
			{

				
				Newoneshigh[koo]=p->vertex[l];
				koo++;
			}
		}
	}
	
	p=poly[blk].poly[5];
	num=poly[blk].sz[5];
	for (k=0;k<num;k++,p++)
	{
		for (l=0;l<4;l++)
		{
			//counter=koo+1;
			dippa=true;
			for (m=0;m<500;m++) 
			{
				
				if ((Newonesmed[m]==p->vertex[l]) || (Newoneslow[m]==p->vertex[l]) || (Newoneshigh[m]==p->vertex[l]))
				{
					dippa=false;
					//m=koo+1;
				}
			}
			if ((dippa==true) && (p->vertex[l]>t->nObjectVert-1))
			{

				
				Newoneshigh[koo]=p->vertex[l];
				koo++;
			}
		}
	}
	
	//check amount of vertices in different detail levels.
	//construct new Vertex Stream with koo amount of vertices. (total.)
	k=0;
	while (Newoneshigh[k]!=-1)
		k++;
	HighDetail=k;
	
	k=0;
	while (Newoneslow[k]!=-1)
		k++;
	LowDetail=k;

	k=0;
	while (Newonesmed[k]!=-1)
		k++;
	MediumDetail=k;

	VertCount=LowDetail+MediumDetail+HighDetail;

	FLOATPT NewVerStr[2048];
	//make another array that has available nodes sorted:
	short SortedVert[2][2048];
	short TempVert[2048];

	for (k=0;k<2048;k++)
	{
		NewVerStr[k].x=0;
		NewVerStr[k].y=0;
		NewVerStr[k].z=0;
		for (l=0;l<2;l++)
			SortedVert[l][k]=999;
		TempVert[k]=0;
	}
	previousnode=-2;

	for (k=0;k<LowDetail;k++)
	{
		//NewVerStr[k]=v[Newoneslow[k]];
		SortedVert[0][k]=Newoneslow[k];
		/*for (m=0;m<LowDetail;m++)
			if ((Newoneslow[m]<SortedVert[1][k]) && (Newoneslow[m]>previousnode))
				SortedVert[1][k]=Newoneslow[m];
		previousnode=SortedVert[1][k]; */
	}
	previousnode=-2;
	
	for (l=0;l<MediumDetail;l++, k++)
	{
		//NewVerStr[k]=v[Newonesmed[l]];
		SortedVert[0][k]=Newonesmed[l];
		/*for (m=0;m<MediumDetail;m++)
			if ((Newonesmed[m]<SortedVert[1][k]) && (Newonesmed[m]>previousnode))
				SortedVert[1][k]=Newonesmed[m];
		previousnode=SortedVert[1][k];*/
	}
	previousnode=-2;

	for (l=0;l<HighDetail;l++, k++)
	{
		//NewVerStr[k]=v[Newoneshigh[l]];
		SortedVert[0][k]=Newoneshigh[l];
		/*for (m=0;m<HighDetail;m++)
			if ((Newoneshigh[m]<SortedVert[1][k]) && (Newoneshigh[m]>previousnode))
				SortedVert[1][k]=Newoneshigh[m];
		previousnode=SortedVert[1][k];*/
	}

	k=0; previousnode=-2;
	for (l=0;l<VertCount;l++)
	{
		for (m=0;m<VertCount;m++)
			if ((SortedVert[0][m]<SortedVert[1][l]) && (SortedVert[0][m]>previousnode))
				SortedVert[1][l]=SortedVert[0][m];
		previousnode=SortedVert[1][l];
	}
	
	v=trk[blk].vert;
	//correcting Vertex Stream:
	for(m=0;m<VertCount;m++)
	{
		NewVerStr[m]=v[SortedVert[0][m]];
	}

	for(m=0;m<VertCount;m++)
	{
		v[SortedVert[1][m]]=NewVerStr[m];
	}
	//correcting Vertex Indexes on Meshes:
	for(counter=0;counter<6;counter++)
	{
		p=poly[blk].poly[counter];
		num=poly[blk].sz[counter];
		
		//counter=0;
		//koo=0;
		for (k=0;k<num;k++,p++)
		{
			for (l=0;l<4;l++)
			{
				for (m=0;m<VertCount;m++)
				{
					if (p->vertex[l]==SortedVert[0][m])
					{
						p->vertex[l]=SortedVert[1][m];
						m=VertCount;
					}
				}
			}
		}
	}
	
	

	//correcting Vertex Stream Offsets:
	//N‰ytt‰is toimivan?
	
	m=0;
	short LowStopSet, MedStopSet, HighStopSet; 
	while (m<LowDetail)
	{
		LowStopSet=SortedVert[1][m];
		m++;
	}
	LowStopSet++;
	m=0;
	while (m<LowDetail+MediumDetail)
	{
		MedStopSet=SortedVert[1][m];
		m++;
	}
	MedStopSet++;
	m=0;
	
	while (m<VertCount)
	{
		HighStopSet=SortedVert[1][m];
		m++;
	}
	HighStopSet++;

	t->nHiResVert=HighStopSet;
	t->nLoResVert=LowStopSet;    //t->nHiResVert; //LowDetail;
	t->nMedResVert=MedStopSet; //t->nHiResVert;   //MediumDetail
	//t->nHiResVert=HighDetail;		


	keke.Format("Low: %d / %d,  Med: %d / %d, High: %d, VertCount: %d / %d OK!", LowDetail, t->nLoResVert, MediumDetail, t->nMedResVert,HighDetail, VertCount, t->nHiResVert);
	return(keke);
}	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	/*
	
	IndexStart=t->nObjectVert;
	IndexEnd=t->nHiResVert-1;
	VertCount=IndexEnd-IndexStart;
	

	FLOATPT VertCheck[2047]; 
	FLOATPT NewVert[2047];
	long VertNew[2047];
	struct FLOATPT VertNew2[VertCount];
	struct FLOATPT VertNew4[VertCount]; 

	for (i=0;i<2047;i++)
	{
		VertCheck[i].x=-1; 
		VertNew[i]=-1;
	}

	

	//pp=&(poly[blk]);
	//t=&(trk[blk]);
	//PrepareModifyTrk(blk);
	//PrepareModifyPoly(blk);
	//k=1;
	
	v=trk[blk].vert;
	//vv=trk[blk].vert;
	
	p2=poly[blk].poly[0];
	num2=poly[blk].sz[0];
	
	
	//Low Detail:
	v=trk[blk].vert;
	p=poly[blk].poly[0];
	num=poly[blk].sz[0]; 

	for (k=0;k<num;k++,p++)
	{
		for (k2=0;k2<4;k2++)
		{
			VertCheck[(p->vertex[k2]-IndexStart)]=v[p->vertex[k2]];
		}
	}
	
	l=0;
	for (k=0;k<VertCount;k++)
	{
		if (VertCheck[k].x!=-1)
		{
			VertNew[l]=k;
			l++;
		}
	}
	NewLowEnd=l+IndexStart;
	
	v=trk[blk].vert;
	p=poly[blk].poly[2];
	num=poly[blk].sz[2]; 

	for (k=0;k<num;k++,p++)
	{
		for (k2=0;k2<4;k2++)
		{
			if (VertCheck[(p->vertex[k2]-IndexStart)].x==-1)
				VertCheck[(p->vertex[k2]-IndexStart)]=v[p->vertex[k2]];
		}
	}
	
	for (k=0;k<VertCount;k++)
	{
		if (VertCheck[k].x!=-1)
		{
			//Check if VertNew Array already has K value.
			for (k2=0;k2<l;k2++)
				if (VertNew[k2]==k)
					process=false;
				else 
					process=true;
			if (process==true)
			{
				VertNew[l]=k;
				l++;
			}
		}
	}
	NewMedEnd=l+IndexStart;
	
	v=trk[blk].vert;
	p=poly[blk].poly[4];
	num=poly[blk].sz[4]; 

	for (k=0;k<num;k++,p++)
	{
		for (k2=0;k2<4;k2++)
		{
			if (VertCheck[(p->vertex[k2]-IndexStart)].x==-1)
				VertCheck[(p->vertex[k2]-IndexStart)]=v[p->vertex[k2]];
		}
	}
	
	for (k=0;k<VertCount;k++)
	{
		if (VertCheck[k].x!=-1)
		{
			//Check if VertNew Array already has K value.
			for (k2=0;k2<l;k2++)
				if (VertNew[k2]==k)
					process=false;
				else 
					process=true;
			if (process==true)
			{
				VertNew[l]=k;
				l++;
			}
		}
	}
	
	v=trk[blk].vert;
	
	//fixing Vertex Stream:
	for (k=0;k<VertCount;k++)
		v[k]=VertCheck[VertNew[l]];
	//fixing Off-Sets:
	t->nLoResVert=NewLowEnd;
	t->nMedResVert=NewMedEnd;
	
	//fixing pointers on mesh:
	//Low:
	p=poly[blk].poly[0];
	num=poly[blk].sz[0];
	for (k=0;k<num;k++,p++)
	{
		for (k2=0;k2<4;k2++)
		{
			for (l=0;l<VertCount;l++)
				if (VertNew[l]==p->vertex[k2])
					p->vertex[k2]=l;
		}
	}
	//Med
	p=poly[blk].poly[2];
	num=poly[blk].sz[2];
	for (k=0;k<num;k++,p++)
	{
		for (k2=0;k2<4;k2++)
		{
			for (l=0;l<VertCount;l++)
				if (VertNew[l]==p->vertex[k2])
					p->vertex[k2]=l;
		}
	}
	//High:
	p=poly[blk].poly[0];
	num=poly[blk].sz[0];
	for (k=0;k<num;k++,p++)
	{
		for (k2=0;k2<4;k2++)
		{
			for (l=0;l<VertCount;l++)
				if (VertNew[l]==p->vertex[k2])
					p->vertex[k2]=l;
		}
	}
	*/

FLOATPT CT3EDDoc::SumVector(FLOATPT Vect1, FLOATPT Vect2)
{
	struct FLOATPT SumVect;
	SumVect.x=Vect1.x+Vect2.x;
	SumVect.y=Vect1.y+Vect2.y;
	SumVect.z=Vect1.z+Vect2.z;
	SumVect=VectorNormalize(SumVect);

	return SumVect;
}

FLOATPT CT3EDDoc::VectorNormalize (FLOATPT nc)
{
	float length;
	length = (float) sqrt((nc.x * nc.x) + (nc.y * nc.y) + (nc.z * nc.z));
	nc.x = nc.x/length;
	nc.y = nc.y/length;
    nc.z = nc.z/length;	
	return nc;
}
FLOATPT CT3EDDoc::QuadNormalVectorCalc(FLOATPT a, FLOATPT b, FLOATPT c, FLOATPT d)
{
	struct FLOATPT n1, n2, nc;
	//float length;
	n1=NormalVectorCalc(a,b,c);
	n2=NormalVectorCalc(a,c,d);
	nc=SumVector(n1, n2);
	


	return nc;
}

FLOATPT CT3EDDoc::NormalVectorCalc(FLOATPT a, FLOATPT b, FLOATPT c)
{
	FLOATPT v1, v2, out, normal;
	//float length;
	
	v1.x = c.x - a.x;
	v1.y = c.y - a.y;
    v1.z = c.z - a.z;


    v2.x = b.x - a.x;
    v2.y = b.y - a.y;
    v2.z = b.z - a.z;

	out.x = (v1.y * v2.z) - (v1.z * v2.y);
	out.y = (v1.z * v2.x) - (v1.x * v2.z);
    out.z = (v1.x * v2.y) - (v1.y * v2.x);

	normal=VectorNormalize(out);
	/*length = (float) sqrt((out.x * out.x) + (out.y * out.y) + (out.z * out.z));
	normal.x = out.x/length;
	normal.y = out.y/length;
    normal.z = out.z/length;*/


	return normal;

}

FLOATPT CT3EDDoc::VertexNormal(int blk, int VertexIndex)
{
	struct FLOATPT a,b,c,d;
	struct FLOATPT normal;
	LPPOLYGONDATA p;
	
	//LPPOLYGONDATA p;
	struct TRKBLOCK *t;
	struct FLOATPT *v;
	//unsigned long *sv; 
	//short *v;
	CString keke;
	//GartPolygon *messi;
	
	normal.x=0;
	normal.y=0;
	normal.z=0;

	t=&trk[blk];
	v=trk[blk].vert;
	
	p=poly[blk].poly[4];
	int num=poly[blk].sz[4];
	for (int j=0;j<num;j++,p++) {
		for (int k=0;k<4;k++) {
			if (p->vertex[k]==VertexIndex) {
				a=v[p->vertex[0]];
				b=v[p->vertex[1]];
				c=v[p->vertex[2]];
				d=v[p->vertex[3]];
				normal=SumVector(normal,QuadNormalVectorCalc(a,b,c,d));
				
			}
		}
	}


return normal;
}

CString CT3EDDoc::BlockShadingFixer(int blk, float theta, float rho)
{
	int i,j, j2,k,num,num2, blk2;
	int result;
	double uu, vv, tt;
	double corner1[3], corner2[3], corner3[3];
	double Pos[3], RayDir[3], StartPos[3];

	//long nGPolygons;
	LPPOLYGONDATA p, p2;
	
	//LPPOLYGONDATA p;
	struct TRKBLOCK *t;
	struct FLOATPT *v, *v2;
	unsigned long *sv; 
	//short *v;
	CString keke;
	//GartPolygon *messi;
	

	t=&trk[blk];
	v=trk[blk].vert;
	sv=trk[blk].unknVertices;

	//meshin luonti:
	//GartPolygon messi[40000];
	//kerrotaan polygonissa olevien solmupisteiden m‰‰r‰: (aina 3)
	//messi[0].count=3;
	//pointteri osoitetaulukkoon (alkioita edell‰ mainittu solmipisteiden m‰‰r‰) jossa solmupisteet floatteina:
	//messi[0].vertices=...
	
	//malloc example:
	// a1 = (double *)malloc(m*m*sizeof(double));
	
	double PI=3.14159265359;
	double PitchAngle = (double) theta; //0.00;//4.712; //0.44; //Theta
	double RhoAngle = (double) rho; //0.165; //0.13; //RHO
	//to get real Yaw from Rho, it has to be multiplied with 4.(*WRONG*)
	//Rho is given as fixed point. (1.0 is 2*pi in radian scale.)
	double YawAngle=(2*PI)*RhoAngle;

	//to get real Pitch from Theta, it needs +pi
	//PitchAngle=PitchAngle+pi;

	//double r;
	struct FLOATPT VNormal;
	VNormal.x=0;
	VNormal.y=0;
	VNormal.z=(float)0.05;

	
	//RayDir[0] = 0.0;
	//RayDir[1] = 0.0;
	//RayDir[2] = 1.0;
	/*
	x=rsin(phi)cos(theta), y=rsin(phi)sin(theta), z=rcos(phi), r=sqrt(x*x+y*y+z*z)...
	*/
	
	
	p2=poly[blk].poly[4];
	num2=poly[blk].sz[4];
	for (j2=0;j2<num2;j2++,p2++) {
		for (k=0;k<4;k++) {
			
			//bool tested = new bool[t->nVertices];
			/*for (int temp=0; temp>trk[blk].nVertices; temp++)
				tested[temp]=false;*/
			
			assert (p2->vertex[k]>=0 && p2->vertex[k]<t->nVertices);
			
			if ((p2->vertex[k]<0) || (p2->vertex[k]>t->nVertices))
			{
				keke.Format("FAILED! K:%d, J2:%d, BLK:%d",k,j2,blk);
				return (keke);
			}

			VNormal=VertexNormal(blk,p2->vertex[k]);
			StartPos[0]=(double)v[p2->vertex[k]].x+(VNormal.x/5);
			StartPos[1]=(double)v[p2->vertex[k]].y+(VNormal.y/5);
			StartPos[2]=(double)v[p2->vertex[k]].z+(VNormal.z/5);
			
			
			RayDir[0] = sin(YawAngle)*cos(PitchAngle);	
			RayDir[1] = sin(YawAngle)*sin(PitchAngle); 
			RayDir[2] =	cos(YawAngle);					
			
			
			

			//used for known light point
			/*EndPos[0]=372;
			EndPos[1]=-790;
			EndPos[2]=10;
			
			RayDir[0]=(EndPos[0]-StartPos[0]);
			RayDir[1]=(EndPos[1]-StartPos[1]);
			RayDir[2]=(EndPos[2]-StartPos[2]);
			r=sqrt((RayDir[0]*RayDir[0])+(RayDir[1]*RayDir[1])+(RayDir[2]*RayDir[2]));
			
			RayDir[0]=RayDir[0]/r;
			RayDir[1]=RayDir[1]/r;
			RayDir[2]=RayDir[2]/r;*/

			result = 0;
			i=0;
			

			while ((t->nbdData[i].blk != -1) && (i<300))
			{
				
				assert(i >= 0 && i < 300);

				
				blk2 = t->nbdData[i].blk;
				

				assert(blk2 >= 0 && blk2 < 300);
				
				p=poly[blk2].poly[4];
				num=poly[blk2].sz[4];
				v2=trk[blk2].vert;
				//Track Polygons
				//--------------
				
				for (j=0;j<num;j++,p++) {
					if (blk==0 && j2==0 && k==1)
					{
						keke.Format("keke");
					}

					//p->texture	
					if (result != 1) {
						assert(p->vertex[0] >= 0 && p->vertex[0] < trk[blk2].nVertices); 
						corner1[0] = (double)v2[p->vertex[0]].x;
						corner1[2] = (double)v2[p->vertex[0]].z;
						corner1[1] = (double)v2[p->vertex[0]].y;
						
						assert(p->vertex[1] >= 0 && p->vertex[1] < trk[blk2].nVertices);
						corner2[0] = (double)v2[p->vertex[3]].x;
						corner2[2] = (double)v2[p->vertex[3]].z;
						corner2[1] = (double)v2[p->vertex[3]].y;
						
						assert(p->vertex[2] >= 0 && p->vertex[2] < trk[blk2].nVertices);
						corner3[0] = (double)v2[p->vertex[2]].x;
						corner3[2] = (double)v2[p->vertex[2]].z;
						corner3[1] = (double)v2[p->vertex[2]].y;
						//run first Ray test, if false, then next quad.
						Pos[0]=StartPos[0];
						Pos[1]=StartPos[1];
						Pos[2]=StartPos[2];
						result = intersect_triangle(Pos, RayDir, corner1, corner2, corner3, &tt, &uu, &vv);
					}
					else
					{ 
						j=num;
						i=300;
					}

					if (result != 1) {
						corner1[0] = (double)v2[p->vertex[0]].x;
						corner1[2] = (double)v2[p->vertex[0]].z;
						corner1[1] = (double)v2[p->vertex[0]].y;
						
						corner2[0] = (double)v2[p->vertex[2]].x;
						corner2[2] = (double)v2[p->vertex[2]].z;
						corner2[1] = (double)v2[p->vertex[2]].y;
						
						corner3[0] = (double)v2[p->vertex[1]].x;
						corner3[2] = (double)v2[p->vertex[1]].z;
						corner3[1] = (double)v2[p->vertex[1]].y;
						//check the second half of the quad
						
						Pos[0]=StartPos[0];
						Pos[1]=StartPos[1];
						Pos[2]=StartPos[2];
						result = intersect_triangle(Pos, RayDir, corner1, corner2, corner3, &tt, &uu, &vv);
					}
					else
					{ 
						j=num;
						i=300;
					}
					
					
					if (result == 1) 
					{
					keke.Format("COLLISION! (BLK/POLY/Node) %d/%d/%d Collided with (BLK/Poly) %d/%d", blk, j2, k, blk2, j);
					//return (keke);
					}

						
				}
				//TrackPolygons End
				//lane Polygons
				//-------------
				//if (result!=1) {
					num=poly[blk2].sz[5];
					p=poly[blk2].poly[5];
					for (j=0;j<num;j++,p++) {
						if (blk==0 && j2==0 && k==1)
						{
							keke.Format("keke");
						}

						//p->texture	
						if (result != 1) {
							assert(p->vertex[0] >= 0 && p->vertex[0] < trk[blk2].nVertices); 
							corner1[0] = (double)v2[p->vertex[0]].x;
							corner1[2] = (double)v2[p->vertex[0]].z;
							corner1[1] = (double)v2[p->vertex[0]].y;
							
							assert(p->vertex[1] >= 0 && p->vertex[1] < trk[blk2].nVertices);
							corner2[0] = (double)v2[p->vertex[3]].x;
							corner2[2] = (double)v2[p->vertex[3]].z;
							corner2[1] = (double)v2[p->vertex[3]].y;
							
							assert(p->vertex[2] >= 0 && p->vertex[2] < trk[blk2].nVertices);
							corner3[0] = (double)v2[p->vertex[2]].x;
							corner3[2] = (double)v2[p->vertex[2]].z;
							corner3[1] = (double)v2[p->vertex[2]].y;
							//run first Ray test, if false, then next quad.
							Pos[0]=StartPos[0];
							Pos[1]=StartPos[1];
							Pos[2]=StartPos[2];
							result = intersect_triangle(Pos, RayDir, corner1, corner2, corner3, &tt, &uu, &vv);
						}
						else
						{ 
							//j=num;
							i=300;
						}

						if (result != 1) {
							corner1[0] = (double)v2[p->vertex[0]].x;
							corner1[2] = (double)v2[p->vertex[0]].z;
							corner1[1] = (double)v2[p->vertex[0]].y;
						
							corner2[0] = (double)v2[p->vertex[2]].x;
							corner2[2] = (double)v2[p->vertex[2]].z;
							corner2[1] = (double)v2[p->vertex[2]].y;
							
							corner3[0] = (double)v2[p->vertex[1]].x;
							corner3[2] = (double)v2[p->vertex[1]].z;
							corner3[1] = (double)v2[p->vertex[1]].y;
							//check the second half of the quad
							
							Pos[0]=StartPos[0];
							Pos[1]=StartPos[1];
							Pos[2]=StartPos[2];
							result = intersect_triangle(Pos, RayDir, corner1, corner2, corner3, &tt, &uu, &vv);
						}
						else
						{ 
							//j=num;
							i=300;
						}
					
					
						if (result == 1) 
						{
						keke.Format("COLLISION! (BLK/POLY/Node) %d/%d/%d Collided with (BLK/Poly) %d/%d", blk, j2, k, blk2, j);
						//return (keke);
						}

						
					}
				//}
				//Lane Polygons End
				//Blue Objects
				//------------
				//if (result!=1) {
					//for (k = 0; k < 4; k++)
					//{
						//for (j = 0; j < poly[nBlock].obj[k].nobj; j++)
						//{
							//for (i = 0; i < poly[nBlock].obj[k].numpoly[j]; i++)
							//{
					//poly[nBlock].obj[k].poly[j][i];
					for (long chunkcounter = 0; chunkcounter<4; chunkcounter++)
					{
						long numobj=poly[blk2].obj[chunkcounter].nobj;
						for (long objcounter=0; objcounter<numobj;objcounter++)
						{
							num=poly[blk2].obj[chunkcounter].numpoly[objcounter];
		
							p=poly[blk2].obj[chunkcounter].poly[objcounter];
							for (j=0;j<num;j++,p++) {
								if (blk==0 && j2==0 && k==1)
								{
									keke.Format("keke");
								}

								//p->texture	
								if (result != 1) {
									assert(p->vertex[0] >= 0 && p->vertex[0] < trk[blk2].nVertices); 
									corner1[0] = (double)v2[p->vertex[2]].x;
									corner1[2] = (double)v2[p->vertex[2]].z;
									corner1[1] = (double)v2[p->vertex[2]].y;
									
									assert(p->vertex[1] >= 0 && p->vertex[1] < trk[blk2].nVertices);
									corner2[0] = (double)v2[p->vertex[3]].x;
									corner2[2] = (double)v2[p->vertex[3]].z;
									corner2[1] = (double)v2[p->vertex[3]].y;
								
									assert(p->vertex[2] >= 0 && p->vertex[2] < trk[blk2].nVertices);
									corner3[0] = (double)v2[p->vertex[0]].x;
									corner3[2] = (double)v2[p->vertex[0]].z;
									corner3[1] = (double)v2[p->vertex[0]].y;
									//run first Ray test, if false, then next quad.
									Pos[0]=StartPos[0];
									Pos[1]=StartPos[1];
									Pos[2]=StartPos[2];
									result = intersect_triangle(Pos, RayDir, corner1, corner2, corner3, &tt, &uu, &vv);
								}
								else
								{ 
									//j=num;
									i=300;
								}

								if (result != 1) {
									corner1[0] = (double)v2[p->vertex[1]].x;
									corner1[2] = (double)v2[p->vertex[1]].z;
									corner1[1] = (double)v2[p->vertex[1]].y;
							
									corner2[0] = (double)v2[p->vertex[2]].x;
									corner2[2] = (double)v2[p->vertex[2]].z;
									corner2[1] = (double)v2[p->vertex[2]].y;
								
									corner3[0] = (double)v2[p->vertex[0]].x;
									corner3[2] = (double)v2[p->vertex[0]].z;
									corner3[1] = (double)v2[p->vertex[0]].y;
									//check the second half of the quad
								
									Pos[0]=StartPos[0];
									Pos[1]=StartPos[1];
									Pos[2]=StartPos[2];
									result = intersect_triangle(Pos, RayDir, corner1, corner2, corner3, &tt, &uu, &vv);
								}
								else
								{ 
									//j=num;
									i=300;
								}
								//Make same with another side:
								if (result != 1) {
									assert(p->vertex[0] >= 0 && p->vertex[0] < trk[blk2].nVertices); 
									corner1[0] = (double)v2[p->vertex[0]].x;
									corner1[2] = (double)v2[p->vertex[0]].z;
									corner1[1] = (double)v2[p->vertex[0]].y;
									
									assert(p->vertex[1] >= 0 && p->vertex[1] < trk[blk2].nVertices);
									corner2[0] = (double)v2[p->vertex[3]].x;
									corner2[2] = (double)v2[p->vertex[3]].z;
									corner2[1] = (double)v2[p->vertex[3]].y;
								
									assert(p->vertex[2] >= 0 && p->vertex[2] < trk[blk2].nVertices);
									corner3[0] = (double)v2[p->vertex[2]].x;
									corner3[2] = (double)v2[p->vertex[2]].z;
									corner3[1] = (double)v2[p->vertex[2]].y;
									//run first Ray test, if false, then next quad.
									Pos[0]=StartPos[0];
									Pos[1]=StartPos[1];
									Pos[2]=StartPos[2];
									result = intersect_triangle(Pos, RayDir, corner1, corner2, corner3, &tt, &uu, &vv);
								}
								else
								{ 
									//j=num;
									i=300;
								}

								if (result != 1) {
									corner1[0] = (double)v2[p->vertex[0]].x;
									corner1[2] = (double)v2[p->vertex[0]].z;
									corner1[1] = (double)v2[p->vertex[0]].y;
							
									corner2[0] = (double)v2[p->vertex[2]].x;
									corner2[2] = (double)v2[p->vertex[2]].z;
									corner2[1] = (double)v2[p->vertex[2]].y;
								
									corner3[0] = (double)v2[p->vertex[1]].x;
									corner3[2] = (double)v2[p->vertex[1]].z;
									corner3[1] = (double)v2[p->vertex[1]].y;
									//check the second half of the quad
								
									Pos[0]=StartPos[0];
									Pos[1]=StartPos[1];
									Pos[2]=StartPos[2];
									result = intersect_triangle(Pos, RayDir, corner1, corner2, corner3, &tt, &uu, &vv);
								}
								else
								{ 
									//j=num;
									i=300;
								}
					
								if (result == 1) 
								{
								keke.Format("COLLISION! (BLK/POLY/Node) %d/%d/%d Collided with (BLK/Poly) %d/%d", blk, j2, k, blk2, j);
								//return (keke);
								}

							}
						}
					}

				//}
				//Blue Polygons End.
				//Xtra Object Start
				//if (result=!1) {
					for (int XObjCounter = 0; XObjCounter < xobj[blk2].nobj; XObjCounter++)
					{
						FLOATPT* vert_array = xobj[blk2].obj[XObjCounter].vert;
						if (xobj[blk2].obj[XObjCounter].crosstype != 6)
						{
							for (int XObjPoly = 0; XObjPoly < xobj[blk2].obj[XObjCounter].nPolygons; XObjPoly++)
							{
							   
								POLYGONDATA& quad = xobj[blk2].obj[XObjCounter].polyData[XObjPoly];
								FLOATPT& ref_point = xobj[blk2].obj[XObjCounter].ptRef;
								
								
								if (result != 1) {
									
									if (xobj[blk2].obj[XObjCounter].crosstype!=1) 
									{

										corner1[0] = (double)vert_array[quad.vertex[0]].x+ref_point.x;
										corner1[2] = (double)vert_array[quad.vertex[0]].z+ref_point.z;
										corner1[1] = (double)vert_array[quad.vertex[0]].y+ref_point.y;
										
											
										corner2[0] = (double)vert_array[quad.vertex[3]].x+ref_point.x;
										corner2[2] = (double)vert_array[quad.vertex[3]].z+ref_point.z;
										corner2[1] = (double)vert_array[quad.vertex[3]].y+ref_point.y;
								
									
										corner3[0] = (double)vert_array[quad.vertex[2]].x+ref_point.x;
										corner3[2] = (double)vert_array[quad.vertex[2]].z+ref_point.z;
										corner3[1] = (double)vert_array[quad.vertex[2]].y+ref_point.y;
									}
									else
									{
										corner1[0] = (double)vert_array[quad.vertex[0]].x;
										corner1[2] = (double)vert_array[quad.vertex[0]].z;
										corner1[1] = (double)vert_array[quad.vertex[0]].y;
							
									
										corner2[0] = (double)vert_array[quad.vertex[3]].x;
										corner2[2] = (double)vert_array[quad.vertex[3]].z;
										corner2[1] = (double)vert_array[quad.vertex[3]].y;
								
									
										corner3[0] = (double)vert_array[quad.vertex[2]].x;
										corner3[2] = (double)vert_array[quad.vertex[2]].z;
										corner3[1] = (double)vert_array[quad.vertex[2]].y;
									}
									//run first Ray test, if false, then next quad.
									Pos[0]=StartPos[0];
									Pos[1]=StartPos[1];
									Pos[2]=StartPos[2];
									result = intersect_triangle(Pos, RayDir, corner1, corner2, corner3, &tt, &uu, &vv);
								}
								else
								{ 
								//j=num;
									i=300;
								}

								if (result != 1) {
									
									if (xobj[blk2].obj[XObjCounter].crosstype!=1) 
									{

										corner1[0] = (double)vert_array[quad.vertex[0]].x+ref_point.x;
										corner1[2] = (double)vert_array[quad.vertex[0]].z+ref_point.z;
										corner1[1] = (double)vert_array[quad.vertex[0]].y+ref_point.y;
										
											
										corner2[0] = (double)vert_array[quad.vertex[2]].x+ref_point.x;
										corner2[2] = (double)vert_array[quad.vertex[2]].z+ref_point.z;
										corner2[1] = (double)vert_array[quad.vertex[2]].y+ref_point.y;
								
									
										corner3[0] = (double)vert_array[quad.vertex[1]].x+ref_point.x;
										corner3[2] = (double)vert_array[quad.vertex[1]].z+ref_point.z;
										corner3[1] = (double)vert_array[quad.vertex[1]].y+ref_point.y;
									}
									else
									{
										corner1[0] = (double)vert_array[quad.vertex[0]].x;
										corner1[2] = (double)vert_array[quad.vertex[0]].z;
										corner1[1] = (double)vert_array[quad.vertex[0]].y;
							
									
										corner2[0] = (double)vert_array[quad.vertex[2]].x;
										corner2[2] = (double)vert_array[quad.vertex[2]].z;
										corner2[1] = (double)vert_array[quad.vertex[2]].y;
								
									
										corner3[0] = (double)vert_array[quad.vertex[1]].x;
										corner3[2] = (double)vert_array[quad.vertex[1]].z;
										corner3[1] = (double)vert_array[quad.vertex[1]].y;
									}
									//run first Ray test, if false, then next quad.
									Pos[0]=StartPos[0];
									Pos[1]=StartPos[1];
									Pos[2]=StartPos[2];
									result = intersect_triangle(Pos, RayDir, corner1, corner2, corner3, &tt, &uu, &vv);
								}
								else
								{ 
									//j=num;
									i=300;
								}
								//usually Dualsided, so another look with reversed vertex order:
								if (result != 1) {
									
									if (xobj[blk2].obj[XObjCounter].crosstype!=1) 
									{

										corner1[0] = (double)vert_array[quad.vertex[2]].x+ref_point.x;
										corner1[2] = (double)vert_array[quad.vertex[2]].z+ref_point.z;
										corner1[1] = (double)vert_array[quad.vertex[2]].y+ref_point.y;
										
											
										corner2[0] = (double)vert_array[quad.vertex[3]].x+ref_point.x;
										corner2[2] = (double)vert_array[quad.vertex[3]].z+ref_point.z;
										corner2[1] = (double)vert_array[quad.vertex[3]].y+ref_point.y;
								
									
										corner3[0] = (double)vert_array[quad.vertex[0]].x+ref_point.x;
										corner3[2] = (double)vert_array[quad.vertex[0]].z+ref_point.z;
										corner3[1] = (double)vert_array[quad.vertex[0]].y+ref_point.y;
									}
									else
									{
										corner1[0] = (double)vert_array[quad.vertex[2]].x;
										corner1[2] = (double)vert_array[quad.vertex[2]].z;
										corner1[1] = (double)vert_array[quad.vertex[2]].y;
							
									
										corner2[0] = (double)vert_array[quad.vertex[3]].x;
										corner2[2] = (double)vert_array[quad.vertex[3]].z;
										corner2[1] = (double)vert_array[quad.vertex[3]].y;
								
									
										corner3[0] = (double)vert_array[quad.vertex[0]].x;
										corner3[2] = (double)vert_array[quad.vertex[0]].z;
										corner3[1] = (double)vert_array[quad.vertex[0]].y;
									}
									//run first Ray test, if false, then next quad.
									Pos[0]=StartPos[0];
									Pos[1]=StartPos[1];
									Pos[2]=StartPos[2];
									result = intersect_triangle(Pos, RayDir, corner1, corner2, corner3, &tt, &uu, &vv);
								}
								else
								{ 
								//j=num;
									i=300;
								}

								if (result != 1) {
									
									if (xobj[blk2].obj[XObjCounter].crosstype!=1) 
									{

										corner1[0] = (double)vert_array[quad.vertex[1]].x+ref_point.x;
										corner1[2] = (double)vert_array[quad.vertex[1]].z+ref_point.z;
										corner1[1] = (double)vert_array[quad.vertex[1]].y+ref_point.y;
										
											
										corner2[0] = (double)vert_array[quad.vertex[2]].x+ref_point.x;
										corner2[2] = (double)vert_array[quad.vertex[2]].z+ref_point.z;
										corner2[1] = (double)vert_array[quad.vertex[2]].y+ref_point.y;
								
									
										corner3[0] = (double)vert_array[quad.vertex[0]].x+ref_point.x;
										corner3[2] = (double)vert_array[quad.vertex[0]].z+ref_point.z;
										corner3[1] = (double)vert_array[quad.vertex[0]].y+ref_point.y;
									}
									else
									{
										corner1[0] = (double)vert_array[quad.vertex[1]].x;
										corner1[2] = (double)vert_array[quad.vertex[1]].z;
										corner1[1] = (double)vert_array[quad.vertex[1]].y;
							
									
										corner2[0] = (double)vert_array[quad.vertex[2]].x;
										corner2[2] = (double)vert_array[quad.vertex[2]].z;
										corner2[1] = (double)vert_array[quad.vertex[2]].y;
								
									
										corner3[0] = (double)vert_array[quad.vertex[0]].x;
										corner3[2] = (double)vert_array[quad.vertex[0]].z;
										corner3[1] = (double)vert_array[quad.vertex[0]].y;
									}
									//run first Ray test, if false, then next quad.
									Pos[0]=StartPos[0];
									Pos[1]=StartPos[1];
									Pos[2]=StartPos[2];
									result = intersect_triangle(Pos, RayDir, corner1, corner2, corner3, &tt, &uu, &vv);
								}
								else
								{ 
									//j=num;
									i=300;
								}

							}
						}
					}
				//}
				//Xtra Object End.
				i++;
				
			}
			if (result == 1) 
					sv[p2->vertex[k]]=0xFF666666; //0xFF777777;
				else
					sv[p2->vertex[k]]=0xFFFFFFFF;
			//result=0;
			//delete tested[];

		}
	}
	
	
	return keke;
}

CString CT3EDDoc::ObjectShadingFixer(int blk, float theta, float rho)
{
	int i,j, j2,k,num,num2, blk2;
	int result;
	double uu, vv, tt;
	double corner1[3], corner2[3], corner3[3];
	double Pos[3], RayDir[3], StartPos[3];

	//long nGPolygons;
	LPPOLYGONDATA p, p2;
	
	//LPPOLYGONDATA p;
	struct TRKBLOCK *t;
	struct FLOATPT *v, *v2;
	unsigned long *sv; 
	//short *v;
	CString keke;
	//GartPolygon *messi;
	

	t=&trk[blk];
	v=trk[blk].vert;
	sv=trk[blk].unknVertices;

	//meshin luonti:
	//GartPolygon messi[40000];
	//kerrotaan polygonissa olevien solmupisteiden m‰‰r‰: (aina 3)
	//messi[0].count=3;
	//pointteri osoitetaulukkoon (alkioita edell‰ mainittu solmipisteiden m‰‰r‰) jossa solmupisteet floatteina:
	//messi[0].vertices=...
	
	//malloc example:
	// a1 = (double *)malloc(m*m*sizeof(double));
	
	double PI=3.14159265359;
	double PitchAngle = (double) theta; //0.00;//4.712; //0.44; //Theta
	double RhoAngle = (double) rho; //0.165; //0.13; //RHO
	//to get real Yaw from Rho, it has to be multiplied with 4.(*WRONG*)
	//Rho is given as fixed point. (1.0 is 2*pi in radian scale.)
	double YawAngle=(2*PI)*RhoAngle;

	//to get real Pitch from Theta, it needs +pi
	//PitchAngle=PitchAngle+pi;

	//double r;
	struct FLOATPT VNormal;
	VNormal.x=0;
	VNormal.y=0;
	VNormal.z=(float)0.05;

	
	//RayDir[0] = 0.0;
	//RayDir[1] = 0.0;
	//RayDir[2] = 1.0;
	/*
	x=rsin(phi)cos(theta), y=rsin(phi)sin(theta), z=rcos(phi), r=sqrt(x*x+y*y+z*z)...
	*/
	
	
	//p2=poly[blk].poly[4];
	//num2=poly[blk].sz[4];
	
	for (long chkchunkcounter = 0; chkchunkcounter<4; chkchunkcounter++)
					{
						long chknumobj=poly[blk].obj[chkchunkcounter].nobj;
						for (long chkobjcounter=0; chkobjcounter<chknumobj;chkobjcounter++)
						{
							num2=poly[blk].obj[chkchunkcounter].numpoly[chkobjcounter];
							p2=poly[blk].obj[chkchunkcounter].poly[chkobjcounter];
	
	for (j2=0;j2<num2;j2++,p2++) {
		for (k=0;k<4;k++) {
			
			//bool tested = new bool[t->nVertices];
			/*for (int temp=0; temp>trk[blk].nVertices; temp++)
				tested[temp]=false;*/
			
			assert (p2->vertex[k]>=0 && p2->vertex[k]<t->nVertices);
			
			if ((p2->vertex[k]<0) || (p2->vertex[k]>t->nVertices))
			{
				keke.Format("FAILED! K:%d, J2:%d, BLK:%d",k,j2,blk);
				return (keke);
			}

			VNormal=VertexNormal(blk,p2->vertex[k]);
			StartPos[0]=(double)v[p2->vertex[k]].x+(VNormal.x/5);
			StartPos[1]=(double)v[p2->vertex[k]].y+(VNormal.y/5);
			StartPos[2]=(double)v[p2->vertex[k]].z+(VNormal.z/5);
			
			
			RayDir[0] = sin(YawAngle)*cos(PitchAngle);	
			RayDir[1] = sin(YawAngle)*sin(PitchAngle); 
			RayDir[2] =	cos(YawAngle);					
			
			
			

			//used for known light point
			/*EndPos[0]=372;
			EndPos[1]=-790;
			EndPos[2]=10;
			
			RayDir[0]=(EndPos[0]-StartPos[0]);
			RayDir[1]=(EndPos[1]-StartPos[1]);
			RayDir[2]=(EndPos[2]-StartPos[2]);
			r=sqrt((RayDir[0]*RayDir[0])+(RayDir[1]*RayDir[1])+(RayDir[2]*RayDir[2]));
			
			RayDir[0]=RayDir[0]/r;
			RayDir[1]=RayDir[1]/r;
			RayDir[2]=RayDir[2]/r;*/

			result = 0;
			i=0;
			

			while ((t->nbdData[i].blk != -1) && (i<300))
			{
				
				assert(i >= 0 && i < 300);

				
				blk2 = t->nbdData[i].blk;
				

				assert(blk2 >= 0 && blk2 < 300);
				
				p=poly[blk2].poly[4];
				num=poly[blk2].sz[4];
				v2=trk[blk2].vert;
				//Track Polygons
				//--------------
				
				for (j=0;j<num;j++,p++) {
					if (blk==0 && j2==0 && k==1)
					{
						keke.Format("keke");
					}

					//p->texture	
					if (result != 1) {
						assert(p->vertex[0] >= 0 && p->vertex[0] < trk[blk2].nVertices); 
						corner1[0] = (double)v2[p->vertex[0]].x;
						corner1[2] = (double)v2[p->vertex[0]].z;
						corner1[1] = (double)v2[p->vertex[0]].y;
						
						assert(p->vertex[1] >= 0 && p->vertex[1] < trk[blk2].nVertices);
						corner2[0] = (double)v2[p->vertex[3]].x;
						corner2[2] = (double)v2[p->vertex[3]].z;
						corner2[1] = (double)v2[p->vertex[3]].y;
						
						assert(p->vertex[2] >= 0 && p->vertex[2] < trk[blk2].nVertices);
						corner3[0] = (double)v2[p->vertex[2]].x;
						corner3[2] = (double)v2[p->vertex[2]].z;
						corner3[1] = (double)v2[p->vertex[2]].y;
						//run first Ray test, if false, then next quad.
						Pos[0]=StartPos[0];
						Pos[1]=StartPos[1];
						Pos[2]=StartPos[2];
						result = intersect_triangle(Pos, RayDir, corner1, corner2, corner3, &tt, &uu, &vv);
					}
					else
					{ 
						j=num;
						i=300;
					}

					if (result != 1) {
						corner1[0] = (double)v2[p->vertex[0]].x;
						corner1[2] = (double)v2[p->vertex[0]].z;
						corner1[1] = (double)v2[p->vertex[0]].y;
						
						corner2[0] = (double)v2[p->vertex[2]].x;
						corner2[2] = (double)v2[p->vertex[2]].z;
						corner2[1] = (double)v2[p->vertex[2]].y;
						
						corner3[0] = (double)v2[p->vertex[1]].x;
						corner3[2] = (double)v2[p->vertex[1]].z;
						corner3[1] = (double)v2[p->vertex[1]].y;
						//check the second half of the quad
						
						Pos[0]=StartPos[0];
						Pos[1]=StartPos[1];
						Pos[2]=StartPos[2];
						result = intersect_triangle(Pos, RayDir, corner1, corner2, corner3, &tt, &uu, &vv);
					}
					else
					{ 
						j=num;
						i=300;
					}
					
					
					if (result == 1) 
					{
					keke.Format("COLLISION! (BLK/POLY/Node) %d/%d/%d Collided with (BLK/Poly) %d/%d", blk, j2, k, blk2, j);
					//return (keke);
					}

						
				}
				//TrackPolygons End
				//lane Polygons
				//-------------
				//if (result!=1) {
					num=poly[blk2].sz[5];
					p=poly[blk2].poly[5];
					for (j=0;j<num;j++,p++) {
						if (blk==0 && j2==0 && k==1)
						{
							keke.Format("keke");
						}

						//p->texture	
						if (result != 1) {
							assert(p->vertex[0] >= 0 && p->vertex[0] < trk[blk2].nVertices); 
							corner1[0] = (double)v2[p->vertex[0]].x;
							corner1[2] = (double)v2[p->vertex[0]].z;
							corner1[1] = (double)v2[p->vertex[0]].y;
							
							assert(p->vertex[1] >= 0 && p->vertex[1] < trk[blk2].nVertices);
							corner2[0] = (double)v2[p->vertex[3]].x;
							corner2[2] = (double)v2[p->vertex[3]].z;
							corner2[1] = (double)v2[p->vertex[3]].y;
							
							assert(p->vertex[2] >= 0 && p->vertex[2] < trk[blk2].nVertices);
							corner3[0] = (double)v2[p->vertex[2]].x;
							corner3[2] = (double)v2[p->vertex[2]].z;
							corner3[1] = (double)v2[p->vertex[2]].y;
							//run first Ray test, if false, then next quad.
							Pos[0]=StartPos[0];
							Pos[1]=StartPos[1];
							Pos[2]=StartPos[2];
							result = intersect_triangle(Pos, RayDir, corner1, corner2, corner3, &tt, &uu, &vv);
						}
						else
						{ 
							//j=num;
							i=300;
						}

						if (result != 1) {
							corner1[0] = (double)v2[p->vertex[0]].x;
							corner1[2] = (double)v2[p->vertex[0]].z;
							corner1[1] = (double)v2[p->vertex[0]].y;
						
							corner2[0] = (double)v2[p->vertex[2]].x;
							corner2[2] = (double)v2[p->vertex[2]].z;
							corner2[1] = (double)v2[p->vertex[2]].y;
							
							corner3[0] = (double)v2[p->vertex[1]].x;
							corner3[2] = (double)v2[p->vertex[1]].z;
							corner3[1] = (double)v2[p->vertex[1]].y;
							//check the second half of the quad
							
							Pos[0]=StartPos[0];
							Pos[1]=StartPos[1];
							Pos[2]=StartPos[2];
							result = intersect_triangle(Pos, RayDir, corner1, corner2, corner3, &tt, &uu, &vv);
						}
						else
						{ 
							//j=num;
							i=300;
						}
					
					
						if (result == 1) 
						{
						keke.Format("COLLISION! (BLK/POLY/Node) %d/%d/%d Collided with (BLK/Poly) %d/%d", blk, j2, k, blk2, j);
						//return (keke);
						}

						
					}
				//}
				//Lane Polygons End
				//Blue Objects
				//------------
				//if (result!=1) {
					//for (k = 0; k < 4; k++)
					//{
						//for (j = 0; j < poly[nBlock].obj[k].nobj; j++)
						//{
							//for (i = 0; i < poly[nBlock].obj[k].numpoly[j]; i++)
							//{
					//poly[nBlock].obj[k].poly[j][i];
					for (long chunkcounter = 0; chunkcounter<4; chunkcounter++)
					{
						long numobj=poly[blk2].obj[chunkcounter].nobj;
						for (long objcounter=0; objcounter<numobj;objcounter++)
						{
							num=poly[blk2].obj[chunkcounter].numpoly[objcounter];
		
							p=poly[blk2].obj[chunkcounter].poly[objcounter];
							for (j=0;j<num;j++,p++) {
								if (blk==0 && j2==0 && k==1)
								{
									keke.Format("keke");
								}

								//p->texture	
								if (result != 1) {
									assert(p->vertex[0] >= 0 && p->vertex[0] < trk[blk2].nVertices); 
									corner1[0] = (double)v2[p->vertex[0]].x;
									corner1[2] = (double)v2[p->vertex[0]].z;
									corner1[1] = (double)v2[p->vertex[0]].y;
									
									assert(p->vertex[1] >= 0 && p->vertex[1] < trk[blk2].nVertices);
									corner2[0] = (double)v2[p->vertex[3]].x;
									corner2[2] = (double)v2[p->vertex[3]].z;
									corner2[1] = (double)v2[p->vertex[3]].y;
								
									assert(p->vertex[2] >= 0 && p->vertex[2] < trk[blk2].nVertices);
									corner3[0] = (double)v2[p->vertex[2]].x;
									corner3[2] = (double)v2[p->vertex[2]].z;
									corner3[1] = (double)v2[p->vertex[2]].y;
									//run first Ray test, if false, then next quad.
									Pos[0]=StartPos[0];
									Pos[1]=StartPos[1];
									Pos[2]=StartPos[2];
									result = intersect_triangle(Pos, RayDir, corner1, corner2, corner3, &tt, &uu, &vv);
								}
								else
								{ 
									//j=num;
									i=300;
								}

								if (result != 1) {
									corner1[0] = (double)v2[p->vertex[0]].x;
									corner1[2] = (double)v2[p->vertex[0]].z;
									corner1[1] = (double)v2[p->vertex[0]].y;
							
									corner2[0] = (double)v2[p->vertex[2]].x;
									corner2[2] = (double)v2[p->vertex[2]].z;
									corner2[1] = (double)v2[p->vertex[2]].y;
								
									corner3[0] = (double)v2[p->vertex[1]].x;
									corner3[2] = (double)v2[p->vertex[1]].z;
									corner3[1] = (double)v2[p->vertex[1]].y;
									//check the second half of the quad
								
									Pos[0]=StartPos[0];
									Pos[1]=StartPos[1];
									Pos[2]=StartPos[2];
									result = intersect_triangle(Pos, RayDir, corner1, corner2, corner3, &tt, &uu, &vv);
								}
								else
								{ 
									//j=num;
									i=300;
								}
								//Make same with another side: (FIX: not needed...)
								/*if (result != 1) {
									assert(p->vertex[0] >= 0 && p->vertex[0] < trk[blk2].nVertices); 
									corner1[0] = (double)v2[p->vertex[0]].x;
									corner1[2] = (double)v2[p->vertex[0]].z;
									corner1[1] = (double)v2[p->vertex[0]].y;
									
									assert(p->vertex[1] >= 0 && p->vertex[1] < trk[blk2].nVertices);
									corner2[0] = (double)v2[p->vertex[3]].x;
									corner2[2] = (double)v2[p->vertex[3]].z;
									corner2[1] = (double)v2[p->vertex[3]].y;
								
									assert(p->vertex[2] >= 0 && p->vertex[2] < trk[blk2].nVertices);
									corner3[0] = (double)v2[p->vertex[2]].x;
									corner3[2] = (double)v2[p->vertex[2]].z;
									corner3[1] = (double)v2[p->vertex[2]].y;
									//run first Ray test, if false, then next quad.
									Pos[0]=StartPos[0];
									Pos[1]=StartPos[1];
									Pos[2]=StartPos[2];
									result = intersect_triangle(Pos, RayDir, corner1, corner2, corner3, &tt, &uu, &vv);
								}
								else
								{ 
									//j=num;
									i=300;
								}

								if (result != 1) {
									corner1[0] = (double)v2[p->vertex[0]].x;
									corner1[2] = (double)v2[p->vertex[0]].z;
									corner1[1] = (double)v2[p->vertex[0]].y;
							
									corner2[0] = (double)v2[p->vertex[2]].x;
									corner2[2] = (double)v2[p->vertex[2]].z;
									corner2[1] = (double)v2[p->vertex[2]].y;
								
									corner3[0] = (double)v2[p->vertex[1]].x;
									corner3[2] = (double)v2[p->vertex[1]].z;
									corner3[1] = (double)v2[p->vertex[1]].y;
									//check the second half of the quad
								
									Pos[0]=StartPos[0];
									Pos[1]=StartPos[1];
									Pos[2]=StartPos[2];
									result = intersect_triangle(Pos, RayDir, corner1, corner2, corner3, &tt, &uu, &vv);
								}
								else
								{ 
									//j=num;
									i=300;
								}*/
					
								if (result == 1) 
								{
								keke.Format("COLLISION! (BLK/POLY/Node) %d/%d/%d Collided with (BLK/Poly) %d/%d", blk, j2, k, blk2, j);
								//return (keke);
								}

							}
						}
					}

				//}
				//Blue Polygons End.
				//Xtra Object Start
				//if (result=!1) {
					for (int XObjCounter = 0; XObjCounter < xobj[blk2].nobj; XObjCounter++)
					{
						FLOATPT* vert_array = xobj[blk2].obj[XObjCounter].vert;
						if (xobj[blk2].obj[XObjCounter].crosstype != 6)
						{
							for (int XObjPoly = 0; XObjPoly < xobj[blk2].obj[XObjCounter].nPolygons; XObjPoly++)
							{
							   
								POLYGONDATA& quad = xobj[blk2].obj[XObjCounter].polyData[XObjPoly];
								FLOATPT& ref_point = xobj[blk2].obj[XObjCounter].ptRef;
								
								
								if (result != 1) {
									
									if (xobj[blk2].obj[XObjCounter].crosstype!=1) 
									{

										corner1[0] = (double)vert_array[quad.vertex[0]].x+ref_point.x;
										corner1[2] = (double)vert_array[quad.vertex[0]].z+ref_point.z;
										corner1[1] = (double)vert_array[quad.vertex[0]].y+ref_point.y;
										
											
										corner2[0] = (double)vert_array[quad.vertex[3]].x+ref_point.x;
										corner2[2] = (double)vert_array[quad.vertex[3]].z+ref_point.z;
										corner2[1] = (double)vert_array[quad.vertex[3]].y+ref_point.y;
								
									
										corner3[0] = (double)vert_array[quad.vertex[2]].x+ref_point.x;
										corner3[2] = (double)vert_array[quad.vertex[2]].z+ref_point.z;
										corner3[1] = (double)vert_array[quad.vertex[2]].y+ref_point.y;
									}
									else
									{
										corner1[0] = (double)vert_array[quad.vertex[0]].x;
										corner1[2] = (double)vert_array[quad.vertex[0]].z;
										corner1[1] = (double)vert_array[quad.vertex[0]].y;
							
									
										corner2[0] = (double)vert_array[quad.vertex[3]].x;
										corner2[2] = (double)vert_array[quad.vertex[3]].z;
										corner2[1] = (double)vert_array[quad.vertex[3]].y;
								
									
										corner3[0] = (double)vert_array[quad.vertex[2]].x;
										corner3[2] = (double)vert_array[quad.vertex[2]].z;
										corner3[1] = (double)vert_array[quad.vertex[2]].y;
									}
									//run first Ray test, if false, then next quad.
									Pos[0]=StartPos[0];
									Pos[1]=StartPos[1];
									Pos[2]=StartPos[2];
									result = intersect_triangle(Pos, RayDir, corner1, corner2, corner3, &tt, &uu, &vv);
								}
								else
								{ 
								//j=num;
									i=300;
								}

								if (result != 1) {
									
									if (xobj[blk2].obj[XObjCounter].crosstype!=1) 
									{

										corner1[0] = (double)vert_array[quad.vertex[0]].x+ref_point.x;
										corner1[2] = (double)vert_array[quad.vertex[0]].z+ref_point.z;
										corner1[1] = (double)vert_array[quad.vertex[0]].y+ref_point.y;
										
											
										corner2[0] = (double)vert_array[quad.vertex[2]].x+ref_point.x;
										corner2[2] = (double)vert_array[quad.vertex[2]].z+ref_point.z;
										corner2[1] = (double)vert_array[quad.vertex[2]].y+ref_point.y;
								
									
										corner3[0] = (double)vert_array[quad.vertex[1]].x+ref_point.x;
										corner3[2] = (double)vert_array[quad.vertex[1]].z+ref_point.z;
										corner3[1] = (double)vert_array[quad.vertex[1]].y+ref_point.y;
									}
									else
									{
										corner1[0] = (double)vert_array[quad.vertex[0]].x;
										corner1[2] = (double)vert_array[quad.vertex[0]].z;
										corner1[1] = (double)vert_array[quad.vertex[0]].y;
							
									
										corner2[0] = (double)vert_array[quad.vertex[2]].x;
										corner2[2] = (double)vert_array[quad.vertex[2]].z;
										corner2[1] = (double)vert_array[quad.vertex[2]].y;
								
									
										corner3[0] = (double)vert_array[quad.vertex[1]].x;
										corner3[2] = (double)vert_array[quad.vertex[1]].z;
										corner3[1] = (double)vert_array[quad.vertex[1]].y;
									}
									//run first Ray test, if false, then next quad.
									Pos[0]=StartPos[0];
									Pos[1]=StartPos[1];
									Pos[2]=StartPos[2];
									result = intersect_triangle(Pos, RayDir, corner1, corner2, corner3, &tt, &uu, &vv);
								}
								else
								{ 
									//j=num;
									i=300;
								}
								//usually Dualsided, so another look with reversed vertex order:
								/*if (result != 1) {
									
									if (xobj[blk2].obj[XObjCounter].crosstype!=1) 
									{

										corner1[0] = (double)vert_array[quad.vertex[2]].x+ref_point.x;
										corner1[2] = (double)vert_array[quad.vertex[2]].z+ref_point.z;
										corner1[1] = (double)vert_array[quad.vertex[2]].y+ref_point.y;
										
											
										corner2[0] = (double)vert_array[quad.vertex[3]].x+ref_point.x;
										corner2[2] = (double)vert_array[quad.vertex[3]].z+ref_point.z;
										corner2[1] = (double)vert_array[quad.vertex[3]].y+ref_point.y;
								
									
										corner3[0] = (double)vert_array[quad.vertex[0]].x+ref_point.x;
										corner3[2] = (double)vert_array[quad.vertex[0]].z+ref_point.z;
										corner3[1] = (double)vert_array[quad.vertex[0]].y+ref_point.y;
									}
									else
									{
										corner1[0] = (double)vert_array[quad.vertex[2]].x;
										corner1[2] = (double)vert_array[quad.vertex[2]].z;
										corner1[1] = (double)vert_array[quad.vertex[2]].y;
							
									
										corner2[0] = (double)vert_array[quad.vertex[3]].x;
										corner2[2] = (double)vert_array[quad.vertex[3]].z;
										corner2[1] = (double)vert_array[quad.vertex[3]].y;
								
									
										corner3[0] = (double)vert_array[quad.vertex[0]].x;
										corner3[2] = (double)vert_array[quad.vertex[0]].z;
										corner3[1] = (double)vert_array[quad.vertex[0]].y;
									}
									//run first Ray test, if false, then next quad.
									Pos[0]=StartPos[0];
									Pos[1]=StartPos[1];
									Pos[2]=StartPos[2];
									result = intersect_triangle(Pos, RayDir, corner1, corner2, corner3, &tt, &uu, &vv);
								}
								else
								{ 
								//j=num;
									i=300;
								}

								if (result != 1) {
									
									if (xobj[blk2].obj[XObjCounter].crosstype!=1) 
									{

										corner1[0] = (double)vert_array[quad.vertex[1]].x+ref_point.x;
										corner1[2] = (double)vert_array[quad.vertex[1]].z+ref_point.z;
										corner1[1] = (double)vert_array[quad.vertex[1]].y+ref_point.y;
										
											
										corner2[0] = (double)vert_array[quad.vertex[2]].x+ref_point.x;
										corner2[2] = (double)vert_array[quad.vertex[2]].z+ref_point.z;
										corner2[1] = (double)vert_array[quad.vertex[2]].y+ref_point.y;
								
									
										corner3[0] = (double)vert_array[quad.vertex[0]].x+ref_point.x;
										corner3[2] = (double)vert_array[quad.vertex[0]].z+ref_point.z;
										corner3[1] = (double)vert_array[quad.vertex[0]].y+ref_point.y;
									}
									else
									{
										corner1[0] = (double)vert_array[quad.vertex[1]].x;
										corner1[2] = (double)vert_array[quad.vertex[1]].z;
										corner1[1] = (double)vert_array[quad.vertex[1]].y;
							
									
										corner2[0] = (double)vert_array[quad.vertex[2]].x;
										corner2[2] = (double)vert_array[quad.vertex[2]].z;
										corner2[1] = (double)vert_array[quad.vertex[2]].y;
								
									
										corner3[0] = (double)vert_array[quad.vertex[0]].x;
										corner3[2] = (double)vert_array[quad.vertex[0]].z;
										corner3[1] = (double)vert_array[quad.vertex[0]].y;
									}
									//run first Ray test, if false, then next quad.
									Pos[0]=StartPos[0];
									Pos[1]=StartPos[1];
									Pos[2]=StartPos[2];
									result = intersect_triangle(Pos, RayDir, corner1, corner2, corner3, &tt, &uu, &vv);
								}
								else
								{ 
									//j=num;
									i=300;
								}*/

							}
						}
					}
				//}
				//Xtra Object End.
				i++;
				
			}
			if (result == 1) 
					sv[p2->vertex[k]]=0xFF666666; //0xFF777777;
				else
					sv[p2->vertex[k]]=0xFFFFFFFF;
			//result=0;
			//delete tested[];

		}
		}
		}
	}
	
	
	return keke;
}

int CT3EDDoc::MinTexBlock(int blk, int MinTextur) // Find the minimum texture number of a block
{
	int j,k,np;
	//int MinTextur=9999;
	LPPOLYGONDATA p;
	for (j=0;j<=4;j+=2) {
		np=poly[blk].sz[j];
		p=poly[blk].poly[j];
		for (k=0;k<np;k++,p++)
		{
			if (p->texture<MinTextur) MinTextur=p->texture;
		}
	}
	//sStr.Format("MinTexBlock : blk=%u,MinTextur=%u",blk,MinTextur);
	//AfxMessageBox (sStr);
	return MinTextur;
}

void CT3EDDoc::ChangeTexBlock(int blk, int offset) // Modifies all textures of a block by the offset
{
	int j,k,np;
	int MinTextur=9999;
	LPPOLYGONDATA p;

	//PrepareModifyTrk(blk);
	PrepareModifyPoly(blk);

	for (j=0;j<=4;j+=2) {
		np=poly[blk].sz[j];
		p=poly[blk].poly[j];
		for (k=0;k<np;k++,p++)
		{
				p->texture=p->texture+offset;
		}
	}
	//sStr.Format("MinTexObject : blk=%u,MinTextur=%u",blk,MinTextur);
	//AfxMessageBox (sStr);
	RecalcBoundingBox(blk);
}

int CT3EDDoc::MinTexObject(int blk, int isxobj, int chunk, int no, int MinTextur)
{
	struct XOBJDATA *x;
	struct OBJPOLYBLOCK *o;
	struct TRKBLOCK *t;
	int n=0, l=0;
	//int MinTextur=9999;
	CString sStr = "Test";
	LPPOLYGONDATA p;
	
	if (isxobj==0)
	{
		t=&(trk[blk]);
		o=&(poly[blk].obj[chunk]);
		n=o->numpoly[no];
		p=o->poly[no];
		for (l=0;l<n;l++,p++) 
		{
			//p=o->poly[l];
			if (p->texture<MinTextur) MinTextur=p->texture;
			//sStr.Format("Textur Nr. %u/%u = %u", l+1,n,p->texture);
			//AfxMessageBox (sStr);
			//fprintf(fout, "%d\n", p->texture);
		}

		//sStr.Format("MinTexObject : blk=%u, isxobj=%u, chunk=%u, no=%u, n(polys)=%u \tMinTextur=%u",blk, isxobj, chunk, no, n, MinTextur);
		//AfxMessageBox (sStr);
	}
	if (isxobj==1)
	{
		x=&(xobj[4*blk+chunk].obj[no]);
		p=x->polyData;
		n=x->nPolygons;
		for (l=0;l<x->nPolygons;l++,p++) if (p->texture<MinTextur) MinTextur=p->texture;
	}

	return MinTextur;
}

void CT3EDDoc::ChangeTexObject(int blk, int isxobj, int chunk, int no, int offset, BOOL HSandHHFlags, short flags, unsigned char texanimdata)
//change all textures of poly or extra objects by a given offset
{
	struct XOBJDATA *x;
	struct OBJPOLYBLOCK *o;
	struct TRKBLOCK *t;
	int n=0, l=0;
	int MinTextur=9999;
	LPPOLYGONDATA p;
	
	if (isxobj==0) //change all textures poly object
	{
		PrepareModifyPoly(blk);

		t=&(trk[blk]);
		o=&(poly[blk].obj[chunk]);
		n=o->numpoly[no];
		p=o->poly[no];
		for (l=0;l<n;l++,p++)
		{
			p->texture+=offset;
			if (HSandHHFlags)
			{
				p->hs_textflags=flags;
				p->texanimdata=texanimdata; //Nappe1: updating AnimData to the refpoly.
			}

		}
	}

	if (isxobj==1) //change all textures of extra object
	{
		PrepareModifyXobj(4*blk+chunk);

		x=&(xobj[4*blk+chunk].obj[no]);
		p=x->polyData;
		n=x->nPolygons;
		for (l=0;l<x->nPolygons;l++,p++)
		{
			p->texture+=offset;
			if (HSandHHFlags)
			{
				p->hs_textflags=flags;
				p->texanimdata=texanimdata; //Nappe1: updating AnimData to the refpoly.
			}

		}
	}
	RecalcBoundingBox(blk);
}

BOOL CT3EDDoc::FindObjMinMax(int blk, int isxobj, int chunk, int no)
// Findes values for virtual minimum, maximun and middle points of an object
{
	CString sStr;
	struct XOBJDATA *x;
	LPPOLYGONDATA p;
	struct TRKBLOCK *b;
	struct FLOATPT *v, xop;
	int i,j,k,l,c,num;
	short vno;
	struct FLOATPT *ref;
	bool ret;

	MemObjMax.x=-999999;MemObjMax.y=-999999;MemObjMax.z=-999999;
	MemObjMin.x=999999;MemObjMin.y=999999;MemObjMin.z=999999;
	ret=FALSE;
	//PrepareModifyTrk(blk);
	b=&(trk[blk]);

	//Sound sources.
	if (isxobj==3) {
		MemObjMax.x=((float)b->soundsrc[no].refpoint.x)/65536;
		MemObjMax.y=((float)b->soundsrc[no].refpoint.y)/65536;
		MemObjMax.z=((float)b->soundsrc[no].refpoint.z)/65536;

		MemObjMin.x=((float)b->soundsrc[no].refpoint.x)/65536;
		MemObjMin.y=((float)b->soundsrc[no].refpoint.y)/65536;
		MemObjMin.z=((float)b->soundsrc[no].refpoint.z)/65536;
		ret=TRUE;
	}

	//Light sources.
	if (isxobj==2) {
		MemObjMax.x=((float)b->lightsrc[no].refpoint.x)/65536;
		MemObjMax.y=((float)b->lightsrc[no].refpoint.y)/65536;
		MemObjMax.z=((float)b->lightsrc[no].refpoint.z)/65536;

		MemObjMin.x=((float)b->lightsrc[no].refpoint.x)/65536;
		MemObjMin.y=((float)b->lightsrc[no].refpoint.y)/65536;
		MemObjMin.z=((float)b->lightsrc[no].refpoint.z)/65536;
		ret=TRUE;
	}
	// Extra Objects
	if (isxobj==1) {
		//PrepareModifyXobj(4*blk+chunk);
		x=&(xobj[4*blk+chunk].obj[no]);
		p=x->polyData;
		v=x->vert;
		//x->ptRef.x  x->ptRef.z  x->ptRef.y+=dy;
		for (k=0;k<x->nPolygons;k++,p++) 
			for (l=0;l<4;l++) {
				xop.x=v[p->vertex[l]].x+x->ptRef.x;
				xop.y=v[p->vertex[l]].y+x->ptRef.y;
				xop.z=v[p->vertex[l]].z+x->ptRef.z;
			if (MemObjMax.x<(v[p->vertex[l]].x+x->ptRef.x))	MemObjMax.x=(v[p->vertex[l]].x+x->ptRef.x);
			if (MemObjMax.y<(v[p->vertex[l]].y+x->ptRef.y))	MemObjMax.y=(v[p->vertex[l]].y+x->ptRef.y);
			if (MemObjMax.z<(v[p->vertex[l]].z+x->ptRef.z))	MemObjMax.z=(v[p->vertex[l]].z+x->ptRef.z);
			if (MemObjMin.x>(v[p->vertex[l]].x+x->ptRef.x))	MemObjMin.x=(v[p->vertex[l]].x+x->ptRef.x);
			if (MemObjMin.y>(v[p->vertex[l]].y+x->ptRef.y))	MemObjMin.y=(v[p->vertex[l]].y+x->ptRef.y);
			if (MemObjMin.z>(v[p->vertex[l]].z+x->ptRef.z))	MemObjMin.z=(v[p->vertex[l]].z+x->ptRef.z);
		}
			ret=TRUE;
	} 
	// Polygon Objects
	if (isxobj==0) {
		num=poly[blk].obj[chunk].numpoly[no];
		p=poly[blk].obj[chunk].poly[no];
		ref=b->vert+p[0].vertex[0];
		for (i=0;i<num;i++) // Loop for all polygons from object
			for (j=0;j<4;j++) // Loop for all four points
				{
				vno=p[i].vertex[j];
				c=0;
				for (k=0;k<i;k++) for (l=0;l<4;l++)
					if (p[k].vertex[l]==vno) c++;
				for (l=0;l<j;l++) if (p[i].vertex[l]==vno) c++;
				if (c==0) {
					if (MemObjMax.x<b->vert[vno].x)	MemObjMax.x=b->vert[vno].x;
					if (MemObjMax.y<b->vert[vno].y)	MemObjMax.y=b->vert[vno].y;
					if (MemObjMax.z<b->vert[vno].z)	MemObjMax.z=b->vert[vno].z;
					if (MemObjMin.x>b->vert[vno].x)	MemObjMin.x=b->vert[vno].x;
					if (MemObjMin.y>b->vert[vno].y)	MemObjMin.y=b->vert[vno].y;
					if (MemObjMin.z>b->vert[vno].z)	MemObjMin.z=b->vert[vno].z;
				}
			}
			ret=TRUE;
		}
	if (ret) {
		MemObjMiddle.x=(((MemObjMax.x-MemObjMin.x)/2)+MemObjMin.x);
		MemObjMiddle.y=(((MemObjMax.y-MemObjMin.y)/2)+MemObjMin.y);
		MemObjMiddle.z=(((MemObjMax.z-MemObjMin.z)/2)+MemObjMin.z);
	}
	//MemObjMiddle;
	sStr.Format("MemObjMiddle.x=%f MemObjMiddle.y=%f MemObjMiddle.z=%f", MemObjMiddle.x,MemObjMiddle.y,MemObjMiddle.z);
	//AfxMessageBox (sStr);
	return ret;
}

BOOL CT3EDDoc::ExpandObject(int blk, int isxobj, int chunk, int no, float xmulti, float ymulti, float zmulti)
{
	CString sStr;
	struct XOBJDATA *x;
	LPPOLYGONDATA p;
	struct TRKBLOCK *b;
	struct FLOATPT *v,xop;
	int i,j,k,l,c,num;
	short vno;
	struct FLOATPT *ref;
	bool ret=FALSE;

	PrepareModifyTrk(blk);
	b=&(trk[blk]);

	// Extra Objects expand / shrink
	if (isxobj==1) {
		PrepareModifyXobj(4*blk+chunk);
		x=&(xobj[4*blk+chunk].obj[no]);
		p=x->polyData;
		v=x->vert;
		//x->ptref.x, x->ptRef.z, x->ptRef.y Referrence Points won't be changed
		//Loop for all vertices of the extra object
		for (k=0;k<x->nVertices;k++,p++)  {
				//calculate vertex position
				xop.x=(v[k].x+x->ptRef.x);
				xop.y=(v[k].y+x->ptRef.y);
				xop.z=(v[k].z+x->ptRef.z);
				//calculate vertex new position
				xop.x=(MemObjMiddle.x+((xop.x-MemObjMiddle.x)*xmulti));
				xop.y=(MemObjMiddle.y+((xop.y-MemObjMiddle.y)*ymulti));
				xop.z=(MemObjMiddle.z+((xop.z-MemObjMiddle.z)*zmulti));
				//calculate new position relative to ptRef;
				v[k].x=(xop.x-x->ptRef.x);
				v[k].y=(xop.y-x->ptRef.y);
				v[k].z=(xop.z-x->ptRef.z);
		}
			ret=TRUE;
	} 

	// Polygon Objects expand / shrink
	if (isxobj==0) {
		num=poly[blk].obj[chunk].numpoly[no];
		p=poly[blk].obj[chunk].poly[no];
		ref=b->vert+p[0].vertex[0];
		for (i=0;i<num;i++) // Loop for all polygons from object
			for (j=0;j<4;j++) // Loop for all four points
				{
				vno=p[i].vertex[j];
				c=0;
				for (k=0;k<i;k++) for (l=0;l<4;l++)
					if (p[k].vertex[l]==vno) c++;
				for (l=0;l<j;l++) if (p[i].vertex[l]==vno) c++;
				if (c==0) {
					//sStr.Format("(MemObjMiddle.x-(b->vert[vno].x))*xmulti)=%f", ((MemObjMiddle.x-(b->vert[vno].x))*xmulti));
					//AfxMessageBox (sStr);
					b->vert[vno].x=(MemObjMiddle.x+((b->vert[vno].x-MemObjMiddle.x)*xmulti));
					b->vert[vno].y=(MemObjMiddle.y+((b->vert[vno].y-MemObjMiddle.y)*ymulti));
					b->vert[vno].z=(MemObjMiddle.z+((b->vert[vno].z-MemObjMiddle.z)*zmulti));
				}
			}
			ret=TRUE;
		}
	return ret;
}

void CT3EDDoc::RotateObject(int blk, int isxobj, int chunk, int no, double angledegree)
{
	CString sStr;
	struct XOBJDATA *x;
	LPPOLYGONDATA p;
	struct TRKBLOCK *b;
	struct FLOATPT *v,xop;
	int i,j,k,l,c,num;
	short vno;
	struct FLOATPT *ref;
	//bool ret=FALSE;

	double pi = 3.14159265358979323846264338;
	double rangle, AcAngle, NewAngle, Length; 

	PrepareModifyTrk(blk);
	b=&(trk[blk]);

	rangle=((angledegree/180) * pi);

	// Rotate Extra Objects
	if (isxobj==1) {
		PrepareModifyXobj(4*blk+chunk);
		x=&(xobj[4*blk+chunk].obj[no]);
		p=x->polyData;
		v=x->vert;
		//x->ptRef.x, x->ptRef.z, x->ptRef.y Referrence Points won't be changed
		//Loop for all vertices of the extra object
		for (k=0;k<x->nVertices;k++,p++)  {
				//calculate vertex position
				xop.x=(v[k].x+x->ptRef.x);
				xop.y=(v[k].y+x->ptRef.y);
				xop.z=(v[k].z+x->ptRef.z);
				//calculate vertex new position
				AcAngle = atan2((xop.y - MemObjMiddle.y),(xop.x - MemObjMiddle.x));// Actual Angle of point;
				NewAngle = (AcAngle - rangle); // Rotate clockwise
				Length=sqrt((xop.y - MemObjMiddle.y)*(xop.y - MemObjMiddle.y) + (xop.x - MemObjMiddle.x)*(xop.x - MemObjMiddle.x)); //Length to the middle of object
				xop.x=(float)(MemObjMiddle.x + (cos(NewAngle))*Length);// New X-Position
				xop.y=(float)(MemObjMiddle.y + (sin(NewAngle))*Length);// New Y-Position

				//xop.x=(MemObjMiddle.x+((xop.x-MemObjMiddle.x)*xmulti));
				//xop.y=(MemObjMiddle.y+((xop.y-MemObjMiddle.y)*ymulti));
				//xop.z=(MemObjMiddle.z+((xop.z-MemObjMiddle.z)*zmulti));
				//calculate new position relative to ptRef;
				v[k].x=(xop.x-x->ptRef.x);
				v[k].y=(xop.y-x->ptRef.y);
				v[k].z=(xop.z-x->ptRef.z);
		}
	} 
	
	// Rotate Polygon Objects
	if (isxobj==0) {
		num=poly[blk].obj[chunk].numpoly[no];
		p=poly[blk].obj[chunk].poly[no];
		ref=b->vert+p[0].vertex[0];
		for (i=0;i<num;i++) // Loop for all polygons from object
			for (j=0;j<4;j++) // Loop for all four points
				{
				vno=p[i].vertex[j];
				c=0;
				for (k=0;k<i;k++) for (l=0;l<4;l++)
					if (p[k].vertex[l]==vno) c++;
				for (l=0;l<j;l++) if (p[i].vertex[l]==vno) c++;
				if (c==0) {
					AcAngle = atan2((b->vert[vno].y - MemObjMiddle.y),(b->vert[vno].x - MemObjMiddle.x));// Actual Angle of point;
					NewAngle = (AcAngle - rangle); // Rotate clockwise
					Length=sqrt((b->vert[vno].y - MemObjMiddle.y)*(b->vert[vno].y - MemObjMiddle.y) + (b->vert[vno].x - MemObjMiddle.x)*(b->vert[vno].x - MemObjMiddle.x)); //Length to the middle of object
					b->vert[vno].x=(float)(MemObjMiddle.x + (cos(NewAngle))*Length);// New X-Position
					b->vert[vno].y=(float)(MemObjMiddle.y + (sin(NewAngle))*Length);// New Y-Position
				}
			}
			//ret=TRUE;
		}
}

BOOL CT3EDDoc::CopyTextToClipboard(CString strText)
{
    BOOL bRet = FALSE; 
	if (OpenClipboard(NULL))
	{
		HGLOBAL clipbuffer;
		char * buffer;
		EmptyClipboard();
		clipbuffer = GlobalAlloc(GMEM_DDESHARE, strText.GetLength()+1);
		buffer = (char*)GlobalLock(clipbuffer);
		strcpy(buffer, LPCSTR(strText));
		GlobalUnlock(clipbuffer);
		SetClipboardData(CF_TEXT,clipbuffer);
		CloseClipboard();
	}
    return bRet; 
}

BOOL CT3EDDoc::CopyXYZToClipboard(FLOATPT *refpt)
{
		BOOL bRet = FALSE; 

		//New T3ED_FLOATPT format for clipboard
		UINT Clipboard_FLOATPT = RegisterClipboardFormat("T3ED_FLOATPT");

		if(OpenClipboard(NULL))
		{
			//allocate some global memory

			HGLOBAL clipbuffer;
			EmptyClipboard();
			clipbuffer = GlobalAlloc(GMEM_DDESHARE, sizeof(FLOATPT));
			FLOATPT * buffer = (FLOATPT*)GlobalLock(clipbuffer);

			//put the data into that memory

			*buffer = *refpt;

			//Put it on the clipboard

			GlobalUnlock(clipbuffer);
			SetClipboardData(Clipboard_FLOATPT,clipbuffer);
			CloseClipboard();
			bRet=TRUE;
		}
		return bRet; 
}

BOOL CT3EDDoc::PasteXYZfromClipboard(FLOATPT *refpt)
{
	UINT Clipboard_FLOATPT = RegisterClipboardFormat("T3ED_FLOATPT");
	
	BOOL bRet = FALSE; 
	FLOATPT data;
	float MoveAmountX,MoveAmountY,MoveAmountZ;
	if (OpenClipboard(NULL)) 
	{
		//get the buffer

		HANDLE hData = GetClipboardData(Clipboard_FLOATPT);
		if (hData!=0){
			FLOATPT * buffer = (FLOATPT *)GlobalLock( hData );

			//make a local copy
			data = *buffer;

			GlobalUnlock( hData );
			CloseClipboard();

			//Do the move of point(s) even if extra objects
			MoveAmountX=-1*(refpt->x-data.x);
			MoveAmountY=-1*(refpt->y-data.y);
			MoveAmountZ=-1*(refpt->z-data.z);
			MovePointBy(refpt,MoveAmountX,MoveAmountY,MoveAmountZ);
			bRet=TRUE;
		}
	}
	return bRet; 
}

BOOL CT3EDDoc::BlockClipboardInfo(CLIPBLOCK &cb)
{
	BOOL bRet = FALSE;
	UINT Clipboard_TrackBlock = RegisterClipboardFormat("T3ED_TRACKBLOCK");

	cb.clipBlk=-1;
	cb.clipIsobj=-1;
	cb.clipChunk=-1;
	cb.clipNo=-1;

	if (OpenClipboard(NULL)) 
	{
		//get the buffer

		HANDLE hData = GetClipboardData(Clipboard_TrackBlock);
		if (hData!=0){
			unsigned char * buffer = (unsigned char*)GlobalLock( hData );
			
			//offset=0;
			//Getting CLIPBLOCK Data
			//cb=(struct CLIPBLOCK *)malloc(sizeof(struct CLIPBLOCK));
			memcpy(&cb,buffer,sizeof(struct CLIPBLOCK));
			bRet = TRUE;
		}
	}
	return bRet;
}

void CT3EDDoc::PasteBlockfromClipboard(int mode, int blk, BOOL paste_block, BOOL paste_heights, BOOL paste_spdfa, BOOL paste_spdra, BOOL paste_vroad) //Paste Block or one object of Block from Clipboard
{
	struct CLIPBLOCK *cb;
	struct POLYGONBLOCK *p;
	struct OBJPOLYBLOCK *o1,*o2;
	struct XOBJDATA *x2;
	struct COLVROAD *vrdata;
	struct HEIGHTSSIM *heightsdata;
	struct SPDFILE *spdFAdata, *spdRAdata;
	LPPOLYGONDATA p1,p2;

	long vBlock;
	int blkxobj, chunk, er, AObjMem;
	int i,j,k,n,n1,n2, offset;
	float dx, dy, dz;
	bool FreeVBlock = FALSE;
	//char *retBuffer;

	UINT Clipboard_TrackBlock = RegisterClipboardFormat("T3ED_TRACKBLOCK");

	if (OpenClipboard(NULL)) 
	{
		//get the buffer

		HANDLE hData = GetClipboardData(Clipboard_TrackBlock);
		if (hData!=0){  //Only if matching data is found on Clipboard
			unsigned char * buffer = (unsigned char*)GlobalLock( hData );
			
			offset=0;
			//Getting CLIPBLOCK Data
			cb=(struct CLIPBLOCK *)malloc(sizeof(struct CLIPBLOCK));
			memcpy(cb,buffer+offset,sizeof(struct CLIPBLOCK));
			offset=offset+sizeof(struct CLIPBLOCK);

			vBlock=nBlocks + 1; //Create space for a new virtual Block trk
			trk=(struct TRKBLOCK *)realloc(trk, (1+vBlock)*sizeof(struct TRKBLOCK));
			poly=(struct POLYGONBLOCK *)realloc(poly, vBlock*sizeof(struct POLYGONBLOCK));
			xobj=(struct XOBJBLOCK *)realloc(xobj,(4*vBlock+4)*sizeof(struct XOBJBLOCK));
			xobj[4*vBlock]=xobj[4*nBlocks]; //Copy global objects at new end
			xobj[4*nBlocks].nobj=0;
			xobj[4*nBlocks].obj=NULL;
			for (i=1;i<4;i++) //Global object are now in 4*vBlock, other 3 chunks should be empty
			{
				xobj[4*vBlock+i].nobj=0;
				xobj[4*vBlock+i].obj=NULL;
			}

			//Getting TRKBLOCK Data
			cb->trk=&(trk[nBlocks]);
			memcpy(cb->trk,buffer+offset,sizeof(struct TRKBLOCK));
			offset=offset+sizeof(struct TRKBLOCK);
			
			//Getting Vertice Data
			cb->trk->vert=NULL;
			cb->trk->unknVertices=NULL;
			if (cb->trk->nVertices!=0){
				cb->trk->vert=(struct FLOATPT *)malloc(12*cb->trk->nVertices);
				memcpy(cb->trk->vert,buffer+offset,12*cb->trk->nVertices);
				offset=offset+12*cb->trk->nVertices;

				cb->trk->unknVertices=(unsigned long *)malloc(4*cb->trk->nVertices);
				memcpy(cb->trk->unknVertices,buffer+offset,4*cb->trk->nVertices);
				offset=offset+4*cb->trk->nVertices;
			}

			//Getting POSITIONDATA
			cb->trk->posData=NULL;
			if (cb->trk->nPositions!=0){
				cb->trk->posData=(struct POSITIONDATA *)malloc(8*cb->trk->nPositions);
				memcpy(cb->trk->posData,buffer+offset,8*cb->trk->nPositions);
				offset=offset+8*cb->trk->nPositions;
			}
			//Getting POLYVROADDATA
			cb->trk->polyData=NULL;
			if (cb->trk->nPolygons!=0) {
				cb->trk->polyData=(struct POLYVROADDATA *)malloc(sizeof(struct POLYVROADDATA)*cb->trk->nPolygons);
				memcpy(cb->trk->polyData,buffer+offset,sizeof(struct POLYVROADDATA)*cb->trk->nPolygons);
				offset=offset+sizeof(struct POLYVROADDATA)*cb->trk->nPolygons;
			}
			//Getting VROADDATA
			cb->trk->vroadData=NULL;
			if (cb->trk->nVRoad!=0) {
				cb->trk->vroadData=(struct VROADDATA *)malloc(12*cb->trk->nVRoad);
				memcpy(cb->trk->vroadData,buffer+offset,12*cb->trk->nVRoad);
				offset=offset+12*cb->trk->nVRoad;
			}
			//Getting REFXOBJ
			cb->trk->xobj=NULL;
			if (cb->trk->nXobj!=0) {
				cb->trk->xobj=(struct REFXOBJ *)malloc(20*cb->trk->nXobj);
				memcpy(cb->trk->xobj,buffer+offset,20*cb->trk->nXobj);
				offset=offset+20*cb->trk->nXobj;
			}
			//Getting SOUNDSRC
			cb->trk->soundsrc=NULL;
			if (cb->trk->nSoundsrc!=0) {
				cb->trk->soundsrc=(struct SOUNDSRC *)malloc(16*cb->trk->nSoundsrc);
				memcpy(cb->trk->soundsrc,buffer+offset,16*cb->trk->nSoundsrc);
				offset=offset+16*cb->trk->nSoundsrc;
			}
			//Getting LIGHTSRC
			if (cb->trk->nLightsrc!=0) {
				cb->trk->lightsrc=(struct LIGHTSRC *)malloc(16*cb->trk->nLightsrc);
				memcpy(cb->trk->lightsrc,buffer+offset,16*cb->trk->nLightsrc);
				offset=offset+16*cb->trk->nLightsrc;
			}

			//Getting POLYGONBLOCK
			cb->poly=&(poly[nBlocks]);
			memcpy(cb->poly,buffer+offset,sizeof(struct POLYGONBLOCK));
			offset=offset+sizeof(struct POLYGONBLOCK);
			p=cb->poly;
			//Getting LPPOLYGONDATA
			for (j=0;j<7;j++) {
				cb->poly->poly[j]=NULL;
				if (cb->poly->sz[j]!=0) {
					cb->poly->poly[j]=(LPPOLYGONDATA)malloc(14*cb->poly->sz[j]);
					memcpy(cb->poly->poly[j],buffer+offset,14*cb->poly->sz[j]);
					offset=offset+14*cb->poly->sz[j];
				}
			}

			for (j=0;j<4;j++) {
				o1=&(cb->poly->obj[j]); 
				o2=&(cb->poly->obj[j]);
				o2->types=NULL;
				if (o1->n2!=0) {
					o2->types=(long *)malloc(4*o1->n2);
					memcpy(o2->types,buffer+offset,4*o1->n2);
					offset=offset+4*o1->n2;
				}

				o2->numpoly=NULL;
				o2->poly=NULL;
				if (o1->nobj!=0) {
					o2->numpoly=(long *)malloc(4*o1->nobj);
					memcpy(o2->numpoly,buffer+offset,4*o1->nobj);
					offset=offset+4*o1->nobj;

					o2->poly=(LPPOLYGONDATA *)malloc(4*o1->nobj);
					for (k=0;k<o1->nobj;k++) {
						o2->poly[k]=(LPPOLYGONDATA)malloc(14*o1->numpoly[k]);
						memcpy(o2->poly[k],buffer+offset,14*o1->numpoly[k]);
						offset=offset+14*o1->numpoly[k];
					}
				}
			}

			//Getting XOBJBLOCK
			//cb->xobj=xobj[0];
			for (chunk=0;chunk<4;chunk++) 
				{
				blkxobj=chunk;

				cb->xobj[blkxobj]=(struct XOBJBLOCK *)malloc(sizeof(struct XOBJBLOCK));
				memcpy(cb->xobj[blkxobj],buffer+offset,sizeof(struct XOBJBLOCK));
				offset=offset+sizeof(struct XOBJBLOCK);
				cb->xobj[blkxobj]->obj=NULL;

				if (cb->xobj[blkxobj]->nobj!=0){
					//x1=xobj[blkxobj].obj;
					x2=cb->xobj[blkxobj]->obj=(struct XOBJDATA *)malloc(cb->xobj[blkxobj]->nobj*sizeof(struct XOBJDATA));
					memcpy(x2,buffer+offset,cb->xobj[blkxobj]->nobj*sizeof(struct XOBJDATA));
					offset=offset+cb->xobj[blkxobj]->nobj*sizeof(struct XOBJDATA);

					for (j=0;j<cb->xobj[blkxobj]->nobj;j++,x2++) { 
						if ((x2->crosstype==3)&&(x2->nAnimLength!=0)) { //x2->animData!=NULL ??? oder besser x2->nAnimLength ?
							x2->animData=(struct ANIMDATA *)malloc(20*x2->nAnimLength);
							memcpy(x2->animData,buffer+offset,20*x2->nAnimLength);
							offset=offset+20*x2->nAnimLength;
						}
						x2->vert=(struct FLOATPT *)malloc(12*x2->nVertices);
						memcpy(x2->vert,buffer+offset,12*x2->nVertices);
						offset=offset+12*x2->nVertices;

						x2->unknVertices=(long *)malloc(4*x2->nVertices);
						memcpy(x2->unknVertices,buffer+offset,4*x2->nVertices);
						offset=offset+4*x2->nVertices;

						x2->polyData=(LPPOLYGONDATA)malloc(14*x2->nPolygons);
						memcpy(x2->polyData,buffer+offset,14*x2->nPolygons);
						offset=offset+14*x2->nPolygons;
					}
				}
			}
			for (j=0;j<4;j++) {
				memcpy(&(xobj[4*(vBlock-1)+j]),cb->xobj[j],sizeof(struct XOBJBLOCK));
			}
			//Getting the vrdata
			vrdata=(struct COLVROAD *)malloc(36*cb->trk->nPositions);
			memcpy(vrdata,buffer+offset,36*cb->trk->nPositions);
			offset=offset+36*cb->trk->nPositions;

			//Getting the hightssim data
			heightsdata=(struct HEIGHTSSIM *)malloc(cb->trk->nPositions*sizeof(struct HEIGHTSSIM));
			for (i=0;i<cb->trk->nPositions;i++) //Setting default values
				heightsdata[i].heights=(float)7.6;
			if (cb->clipHeightsLoaded)
			{
				memcpy(heightsdata,buffer+offset,cb->trk->nPositions*sizeof(struct HEIGHTSSIM));
				offset=offset+cb->trk->nPositions*sizeof(struct HEIGHTSSIM);
			}

			//Getting the spdFAdata data
			spdFAdata=(struct SPDFILE *)malloc(cb->trk->nPositions*sizeof(struct SPDFILE));
			for (i=0;i<cb->trk->nPositions;i++) //Setting default values
			{
				spdFAdata[i].Bestprejection=(char) 60;
				spdFAdata[i].Speedvalue=(char) 80;
				spdFAdata[i].turn=0;
			}
			if (cb->clipspdFALoaded)
			{
				memcpy(spdFAdata,buffer+offset,cb->trk->nPositions*sizeof(struct SPDFILE));
				offset=offset+cb->trk->nPositions*sizeof(struct SPDFILE);
			}

			//Getting the spdRAdata data
			spdRAdata=(struct SPDFILE *)malloc(cb->trk->nPositions*sizeof(struct SPDFILE));
			for (i=0;i<cb->trk->nPositions;i++) //Setting default values
			{
				spdRAdata[i].Bestprejection=(char) 60;
				spdRAdata[i].Speedvalue=(char) 80;
				spdRAdata[i].turn=0;
			}
			if (cb->clipspdRALoaded)
			{
				memcpy(spdRAdata,buffer+offset,cb->trk->nPositions*sizeof(struct SPDFILE));
				offset=offset+cb->trk->nPositions*sizeof(struct SPDFILE);
			}

		// Paste whole block or only one object ?
		if (cb->clipNo!=-1) //Copy Object to block
			{
				FindObjMinMax((vBlock-1),cb->clipIsobj,cb->clipChunk,cb->clipNo); //Getting middle of selected object
				dx=trk[blk].ptCentre.x-MemObjMiddle.x; //Calculate distance
				dy=trk[blk].ptCentre.y-MemObjMiddle.y;
				dz=trk[blk].ptCentre.z-MemObjMiddle.z;
				AObjMem=bAutoObjMem;
				bAutoObjMem=TRUE; //Automatic object memership
				er=MoveObjectBy((vBlock-1),cb->clipIsobj,cb->clipChunk,cb->clipNo,dx,dy,dz); //Move the object to destination block					
				bAutoObjMem=AObjMem; //Set back old setting
				FreeVBlock = TRUE; //The rest of the data isn't needed anymore
				RecalcBoundingBox(blk);
			}
		else //add a block at the end
			if (mode==0) //at block at end of track
			{
				for (j=0;j<undoLevel;j++) DeleteUndo(j);
				undoLevel=0;
				nBlocks++;
				n=0;
				for (i=0;i<nBlocks;i++)
					for (j=0;j<trk[i].nXobj;j++)
						trk[i].xobj[j].globalno=n++;
				//pDoc->col.vroadHead.nrec=8*pDoc->nBlocks;
				cb->trk->nStartPos=trk[nBlocks-2].nStartPos+trk[nBlocks-2].nPositions;
				col.vroadHead.nrec=(short)(col.vroadHead.nrec+cb->trk->nPositions);
				//col.vroad=(struct COLVROAD *)realloc(col.vroad, 36*col.vroadHead.nrec);
				col.vroad=(struct COLVROAD *)realloc(col.vroad, nBlocks*8*sizeof(struct COLVROAD));
				memcpy(&col.vroad[cb->trk->nStartPos],vrdata,36*cb->trk->nPositions);

				if (HeightsLoaded){
					hightssim=(struct HEIGHTSSIM *)realloc(hightssim, col.vroadHead.nrec* sizeof(struct HEIGHTSSIM)); //More space for hights.sim data
					memcpy(&hightssim[cb->trk->nStartPos],heightsdata,cb->trk->nPositions * sizeof(struct HEIGHTSSIM)); //Copy heightsdata in place
				}
				if (spdFALoaded){
				spdFAbin=(struct SPDFILE *)realloc(spdFAbin, col.vroadHead.nrec* sizeof(struct SPDFILE)); //More space for spdFAbin data
				memcpy(&spdFAbin[cb->trk->nStartPos],spdFAdata,cb->trk->nPositions * sizeof(struct SPDFILE)); //Copy spdFAdata in place
				}

				if (spdRALoaded){
				spdRAbin=(struct SPDFILE *)realloc(spdRAbin, col.vroadHead.nrec* sizeof(struct SPDFILE)); //More space for spdRAbin data
				memcpy(&spdRAbin[cb->trk->nStartPos],spdRAdata,cb->trk->nPositions * sizeof(struct SPDFILE)); //Copy spdFAdata in place
				}
				
				// can't keep consistent undo, so delete it all
				for (i=0;i<undoLevel;i++) DeleteUndo(i);
				undoLevel=0;
			}
			if (mode==2) //Replace a block
			{
				//Undo for replaced block
				/*n=col.vroadHead.nrec;
				undoCol[undoLevel-1]=(struct COLVROAD *)malloc(36*n);
				memcpy(undoCol[undoLevel-1],col.vroad,36*n);*/

				PrepareModifyTrk(blk);
				PrepareModifyPoly(blk);
				PrepareModifyVRoadHeightsSpdFiles();
				for (i=0;i<4;i++) PrepareModifyXobj(4*blk+i);
				

				if (paste_block)
				{
					n=trk[blk].nStartPos; //store vroad start position
					FreeTrkContents(&(trk[blk]));
					memcpy(&(trk[blk]),&(trk[nBlocks]),sizeof(struct TRKBLOCK));
					trk[blk].nStartPos=n; //copy back vroad start position

					FreePolyContents(&(poly[blk]));
					memcpy(&(poly[blk]),&(poly[nBlocks]),sizeof(struct POLYGONBLOCK));

					for (j=0;j<4;j++)
					{
						FreeXobjContents(&(xobj[4*blk+j]));
						memcpy(&(xobj[4*blk+j]),cb->xobj[j],sizeof(struct XOBJBLOCK));
					}
					if (paste_vroad)
						memcpy(&col.vroad[trk[blk].nStartPos],vrdata,36*cb->trk->nPositions);

				}
				if ((paste_heights)&&(HeightsLoaded))
					memcpy(&hightssim[trk[blk].nStartPos],heightsdata,cb->trk->nPositions * sizeof(struct HEIGHTSSIM)); //Copy heightsdata in place
				if ((paste_spdfa)&&(spdFALoaded))
					memcpy(&spdFAbin[trk[blk].nStartPos],spdFAdata,cb->trk->nPositions * sizeof(struct SPDFILE)); //Copy heightsdata in place
				if ((paste_spdra)&&(spdRALoaded))
					memcpy(&spdRAbin[trk[blk].nStartPos],spdRAdata,cb->trk->nPositions * sizeof(struct SPDFILE)); //Copy heightsdata in place

				xobj[4*nBlocks]=xobj[4*vBlock]; //Copy back global objects
				trk=(struct TRKBLOCK *)realloc(trk, (1+ nBlocks)*sizeof(struct TRKBLOCK)); //+1 = Dummy trackblock for global objects
				poly=(struct POLYGONBLOCK *)realloc(poly, nBlocks*sizeof(struct POLYGONBLOCK));
				xobj=(struct XOBJBLOCK *)realloc(xobj,(4*nBlocks+1)*sizeof(struct XOBJBLOCK));
		
			}
			if (mode==3) //Replace texture only
			{
				PrepareModifyPoly(blk);

				//Replace track polygon texture
				for (k=0;k<7;k++) 
				{
					p1=poly[nBlocks].poly[k];
					n1=poly[nBlocks].sz[k];
					p2=poly[blk].poly[k];
					n2=poly[blk].sz[k];
					n=n1;if (n2<n1) n=n2;
					for (j=0;j<n;j++,p1++,p2++)
					{
						p2->texture=p1->texture;
						p2->hs_textflags=p1->hs_textflags;
						p2->texanimdata=p1->texanimdata;
						p2->unknown2=p1->unknown2;
					}
				}
				FreeVBlock = TRUE; //The rest of the data isn't needed anymore
			}
		}

		//Give back used memory
		dofree(vrdata);
		dofree(heightsdata);
		dofree(spdFAdata);
		dofree(spdRAdata);
		if (FreeVBlock){
			vBlock--;
			FreeTrkContents(&(trk[vBlock]));
			FreePolyContents(&(poly[vBlock]));
			for (j=0;j<4;j++) {
				FreeXobjContents(&(xobj[4*vBlock+j]));
			}
			xobj[4*nBlocks]=xobj[4*(nBlocks+1)]; //Copy back global objects
			dofree(cb);
			trk=(struct TRKBLOCK *)realloc(trk, (1+nBlocks)*sizeof(struct TRKBLOCK));
			poly=(struct POLYGONBLOCK *)realloc(poly, nBlocks*sizeof(struct POLYGONBLOCK));
			xobj=(struct XOBJBLOCK *)realloc(xobj,(4*nBlocks+1)*sizeof(struct XOBJBLOCK));
		}
	}
}

int CT3EDDoc::CopyBlockToClipboard(int blk, int isxobj, int chunk, int no)
{
	struct CLIPBLOCK *u;
	struct TRKBLOCK *t;
	struct POLYGONBLOCK *p;
	struct OBJPOLYBLOCK *o1;
	struct XOBJDATA *x1;
	//struct COLVROAD *vrdata;
	int blkxobj;
	int j,k,size;
	//char *retBuffer;

	u=&Clipblk;
	u->clipBlk=blk;
	u->clipIsobj=isxobj;
	u->clipChunk=chunk;
	u->clipNo=no;
	u->bHSMode=bHSMode;
	u->clipHeightsLoaded=HeightsLoaded;
	u->clipspdFALoaded=spdFALoaded;
	u->clipspdRALoaded=spdRALoaded;

	//Copy to Clipstring
	size=sizeof(struct CLIPBLOCK);
	unsigned char *retBuffer = (unsigned char *)malloc(size);
	memcpy(retBuffer, &Clipblk, sizeof(CLIPBLOCK));

	// from PrepareModifyTrk
	//u=&Clipblk;

	//u->trk=(struct TRKBLOCK *)malloc(sizeof(struct TRKBLOCK));
	t=&(trk[blk]);
	//memcpy(u->trk,t,sizeof(struct TRKBLOCK));
	//add to Clipstring
	retBuffer=(unsigned char*)realloc(retBuffer, size+sizeof(struct TRKBLOCK));
	memcpy(retBuffer+size,t,sizeof(struct TRKBLOCK));
	size=size+sizeof(struct TRKBLOCK);
	//end add to Clipstring

	//u->trk->vert=NULL;
	//u->trk->unknVertices=NULL;
	if (t->nVertices!=0) {
		//u->trk->vert=(struct FLOATPT *)malloc(12*t->nVertices);
		//memcpy(u->trk->vert,t->vert,12*t->nVertices);
		//add to Clipstring
		retBuffer=(unsigned char*)realloc(retBuffer, size+12*t->nVertices);
		memcpy(retBuffer+size,t->vert,12*t->nVertices);
		size=size+12*t->nVertices;
		//end add to Clipstring

		//u->trk->unknVertices=(unsigned long *)malloc(4*t->nVertices);
		//memcpy(u->trk->unknVertices,t->unknVertices,4*t->nVertices);
		//add to Clipstring
		retBuffer=(unsigned char*)realloc(retBuffer, size+4*t->nVertices);
		memcpy(retBuffer+size,t->unknVertices,4*t->nVertices);
		size=size+4*t->nVertices;
		//end add to Clipstring
	}
	//u->trk->posData=NULL;
	if (t->nPositions!=0) {
		//u->trk->posData=(struct POSITIONDATA *)malloc(8*t->nPositions);
		//memcpy(u->trk->posData,t->posData,8*t->nPositions);
		//add to Clipstring
		retBuffer=(unsigned char*)realloc(retBuffer, size+8*t->nPositions);
		memcpy(retBuffer+size,t->posData,8*t->nPositions);
		size=size+8*t->nPositions;
		//end add to Clipstring
	}
	//u->trk->polyData=NULL;
	if (t->nPolygons!=0) {
		//u->trk->polyData=(struct POLYVROADDATA *)malloc(sizeof(struct POLYVROADDATA)*t->nPolygons);
		//memcpy(u->trk->polyData,t->polyData,sizeof(struct POLYVROADDATA)*t->nPolygons);
		//add to Clipstring
		retBuffer=(unsigned char*)realloc(retBuffer, size+sizeof(struct POLYVROADDATA)*t->nPolygons);
		memcpy(retBuffer+size,t->polyData,sizeof(struct POLYVROADDATA)*t->nPolygons);
		size=size+sizeof(struct POLYVROADDATA)*t->nPolygons;
		//end add to Clipstring
	}
	//u->trk->vroadData=NULL;
	if (t->nVRoad!=0) {
		//u->trk->vroadData=(struct VROADDATA *)malloc(12*t->nVRoad);
		//memcpy(u->trk->vroadData,t->vroadData,12*t->nVRoad);
		//add to Clipstring
		retBuffer=(unsigned char*)realloc(retBuffer, size+12*t->nVRoad);
		memcpy(retBuffer+size,t->vroadData,12*t->nVRoad);
		size=size+12*t->nVRoad;
		//end add to Clipstring
	}
	//u->trk->xobj=NULL;
	if (t->nXobj!=0) {
		//u->trk->xobj=(struct REFXOBJ *)malloc(20*t->nXobj);
		//memcpy(u->trk->xobj,t->xobj,20*t->nXobj);
		//add to Clipstring
		retBuffer=(unsigned char*)realloc(retBuffer, size+20*t->nXobj);
		memcpy(retBuffer+size,t->xobj,20*t->nXobj);
		size=size+20*t->nXobj;
		//end add to Clipstring
	}
	//u->trk->soundsrc=NULL;
	if (t->nSoundsrc!=0) {
		//u->trk->soundsrc=(struct SOUNDSRC *)malloc(16*t->nSoundsrc);
		//memcpy(u->trk->soundsrc,t->soundsrc,16*t->nSoundsrc);
		//add to Clipstring
		retBuffer=(unsigned char*)realloc(retBuffer, size+16*t->nSoundsrc);
		memcpy(retBuffer+size,t->soundsrc,16*t->nSoundsrc);
		size=size+16*t->nSoundsrc;
		//end add to Clipstring
	}
	//u->trk->lightsrc=NULL;
	if (t->nLightsrc!=0) {
		//u->trk->lightsrc=(struct LIGHTSRC *)malloc(16*t->nLightsrc);
		//memcpy(u->trk->lightsrc,t->lightsrc,16*t->nLightsrc);
		//add to Clipstring
		retBuffer=(unsigned char*)realloc(retBuffer, size+16*t->nLightsrc);
		memcpy(retBuffer+size,t->lightsrc,16*t->nLightsrc);
		size=size+16*t->nLightsrc;
		//end add to Clipstring
	}

	//from PrepareModifyPoly
	//if (u->poly!=NULL) return;
	//u->poly=(struct POLYGONBLOCK *)malloc(sizeof(struct POLYGONBLOCK));
	p=&(poly[blk]);
	//memcpy(u->poly,p,sizeof(struct POLYGONBLOCK));
	//add to Clipstring
	retBuffer=(unsigned char*)realloc(retBuffer, size+sizeof(struct POLYGONBLOCK));
	memcpy(retBuffer+size,p=&(poly[blk]),sizeof(struct POLYGONBLOCK));
	size=size+sizeof(struct POLYGONBLOCK);
	//end add to Clipstring
	for (j=0;j<7;j++) {
		//u->poly->poly[j]=NULL;
		if (p->sz[j]!=0) {
			//u->poly->poly[j]=(LPPOLYGONDATA)malloc(14*p->sz[j]);
			//memcpy(u->poly->poly[j],p->poly[j],14*p->sz[j]);
			//add to Clipstring
			retBuffer=(unsigned char*)realloc(retBuffer, size+14*p->sz[j]);
			memcpy(retBuffer+size,p->poly[j],14*p->sz[j]);
			size=size+14*p->sz[j];
			//end add to Clipstring
		}
	}
	for (j=0;j<4;j++) {
		o1=&(p->obj[j]); 
		//o2=&(u->poly->obj[j]);
		//o2->types=NULL;
		if (o1->n2!=0) {
			//o2->types=(long *)malloc(4*o1->n2);
			//memcpy(o2->types,o1->types,4*o1->n2);
			//add to Clipstring
			retBuffer=(unsigned char*)realloc(retBuffer, size+4*o1->n2);
			memcpy(retBuffer+size,o1->types,4*o1->n2);
			size=size+4*o1->n2;
			//end add to Clipstring
		}
		//o2->numpoly=NULL;
		//o2->poly=NULL;
		if (o1->nobj!=0) {
			//o2->numpoly=(long *)malloc(4*o1->nobj);
			//memcpy(o2->numpoly,o1->numpoly,4*o1->nobj);
			//add to Clipstring
			retBuffer=(unsigned char*)realloc(retBuffer, size+4*o1->nobj);
			memcpy(retBuffer+size,o1->numpoly,4*o1->nobj);
			size=size+4*o1->nobj;
			//end add to Clipstring
			//o2->poly=(LPPOLYGONDATA *)malloc(4*o1->nobj);
			for (k=0;k<o1->nobj;k++) {
				//o2->poly[k]=(LPPOLYGONDATA)malloc(14*o1->numpoly[k]);
				//memcpy(o2->poly[k],o1->poly[k],14*o1->numpoly[k]);
				//add to Clipstring
				retBuffer=(unsigned char*)realloc(retBuffer, size+14*o1->numpoly[k]);
				memcpy(retBuffer+size,o1->poly[k],14*o1->numpoly[k]);
				size=size+14*o1->numpoly[k];
				//end add to Clipstring
			}
		}
	}

	for (chunk=0;chunk<4;chunk++) 
	{
		blkxobj=(4*blk+chunk);
		//add to Clipstring
		retBuffer=(unsigned char*)realloc(retBuffer, size+sizeof(struct XOBJBLOCK));
		//memcpy(retBuffer+size,u->xobj[blkxobj],sizeof(struct XOBJBLOCK));
		memcpy(retBuffer+size,&xobj[blkxobj],sizeof(struct XOBJBLOCK));
		size=size+sizeof(struct XOBJBLOCK);
		//end add to Clipstring
		if (xobj[blkxobj].nobj!=0){
		  x1=xobj[blkxobj].obj;
		  //add to Clipstring
		  retBuffer=(unsigned char*)realloc(retBuffer, size+xobj[blkxobj].nobj*sizeof(struct XOBJDATA));
		  memcpy(retBuffer+size,x1,xobj[blkxobj].nobj*sizeof(struct XOBJDATA));
		  size=size+xobj[blkxobj].nobj*sizeof(struct XOBJDATA);
		  //end add to Clipstring
		for (j=0;j<xobj[blkxobj].nobj;j++,x1++) {
			if ((x1->crosstype==3)&&(x1->nAnimLength!=0)) {
				//add to Clipstring
				retBuffer=(unsigned char*)realloc(retBuffer, size+20*x1->nAnimLength);
				memcpy(retBuffer+size,x1->animData,20*x1->nAnimLength);
				size=size+20*x1->nAnimLength;
				//end add to Clipstring
			}
			//add to Clipstring
			retBuffer=(unsigned char*)realloc(retBuffer, size+12*x1->nVertices);
			memcpy(retBuffer+size,x1->vert,12*x1->nVertices);
			size=size+12*x1->nVertices;
			//end add to Clipstring

			//add to Clipstring
			retBuffer=(unsigned char*)realloc(retBuffer, size+4*x1->nVertices);
			memcpy(retBuffer+size,x1->unknVertices,4*x1->nVertices);
			size=size+4*x1->nVertices;
			//end add to Clipstring

			//add to Clipstring
			retBuffer=(unsigned char*)realloc(retBuffer, size+14*x1->nPolygons);
			memcpy(retBuffer+size,x1->polyData,14*x1->nPolygons);
			size=size+14*x1->nPolygons;
			//end add to Clipstring
		}
		}
	}
	//Copy col.vroad data to clipstring
	t=&(trk[blk]);
	retBuffer=(unsigned char*)realloc(retBuffer, size+36*t->nPositions);
	memcpy(retBuffer+size,&col.vroad[t->nStartPos],36*t->nPositions);
	size=size+36*t->nPositions;

	//Copy hightssim data to clipstring
	if (HeightsLoaded) 
	{
		retBuffer=(unsigned char*)realloc(retBuffer, size + t->nPositions * sizeof(struct HEIGHTSSIM));
		memcpy(retBuffer+size,&hightssim[t->nStartPos],t->nPositions * sizeof(struct HEIGHTSSIM));
		size=size + t->nPositions * sizeof(struct HEIGHTSSIM);
	}

	//Copy spdFAbin data to clipstring
	if (spdFALoaded) 
	{
		retBuffer=(unsigned char*)realloc(retBuffer, size + t->nPositions * sizeof(struct SPDFILE));
		memcpy(retBuffer+size,&spdFAbin[t->nStartPos],t->nPositions * sizeof(struct SPDFILE));
		size=size + t->nPositions * sizeof(struct SPDFILE);
	}
	
	//Copy spdRAbin data to clipstring
	if (spdRALoaded) 
	{
		retBuffer=(unsigned char*)realloc(retBuffer, size + t->nPositions * sizeof(struct SPDFILE));
		memcpy(retBuffer+size,&spdRAbin[t->nStartPos],t->nPositions * sizeof(struct SPDFILE));
		size=size + t->nPositions * sizeof(struct SPDFILE);
	}


	//New T3ED_TRACKBLOCK format for clipboard
	UINT Clipboard_TrackBlock=RegisterClipboardFormat("T3ED_TRACKBLOCK");

	if(OpenClipboard(NULL))
		{
			//allocate some global memory
			HGLOBAL clipbuffer;
			EmptyClipboard();
			clipbuffer = GlobalAlloc(GMEM_DDESHARE, size);
			memcpy(clipbuffer,retBuffer,size); //Copy data from retBuffer to clipbuffer
			GlobalUnlock(clipbuffer);
			SetClipboardData(Clipboard_TrackBlock,clipbuffer);
			CloseClipboard();
		}
	else
		size=-1;

	return size;
	//CString sStr;
	//sStr.Format("Copy Object to Clipboard (size = %u)",size);
	//AfxMessageBox (sStr);
	//AfxMessageBox("Copy Object to Clipboard");
}



float CT3EDDoc::VRoadLenght(int blk)
{
	int start,end;
	FLOATPT fp1, fp2;
	struct COLVROAD *cvr;
	float lenght, dx, dy;
	struct INTPT ipt1, ipt2;

	start=trk[blk].nStartPos;
	end=trk[blk].nStartPos + trk[blk].nPositions -1;

	cvr=&col.vroad[start];
	ipt1=cvr->refPt;
	fp1.x=((float)ipt1.x/65536);
	fp1.y=((float)ipt1.y/65536);
	fp1.z=((float)ipt1.z/65536);

	cvr=&col.vroad[end];
	ipt2=cvr->refPt;
	fp2.x=((float)ipt2.x/65536);
	fp2.y=((float)ipt2.y/65536);
	fp2.z=((float)ipt2.z/65536);

	dx=(fp1.x) - (fp2.x);
	dy=(fp1.y) - (fp2.y);

	lenght=(float)(sqrt((dy*dy) + (dx*dx)));
	
	return lenght;

}

BOOL CT3EDDoc::SaveOffFile(NOBJDATA *NewObjData, CString FileName)
{
	BOOL Ok=TRUE;
	char* buff=new char[255];

	CString s,s2;
	//int np, point;
	int i;
	
	FILE* fout = fopen(FileName, "w");
	if (fout==NULL)
	{
		s.Format("Unable to open '%s'.", FileName);
		AfxMessageBox(s);	
		Ok=FALSE;
	}
	else
	{
		fprintf(fout, "OFF\n"); //Write Header
		fprintf(fout, "%d %d %d\n", NewObjData->nVert, NewObjData->nPoly, NewObjData->nPoly*4); //Write Info
		for (i=0;i<NewObjData->nVert;i++) //Write vertice data
			fprintf(fout, "%f %f %f\n", NewObjData->Vertices[i].x, NewObjData->Vertices[i].y, NewObjData->Vertices[i].z);
		for (i=0;i<NewObjData->nPoly;i++) //Write Polygon data
			fprintf(fout, "4 %d %d %d %d\n", NewObjData->Polys[i].vertex[0], NewObjData->Polys[i].vertex[1], NewObjData->Polys[i].vertex[2], NewObjData->Polys[i].vertex[3]);
		fclose(fout);
	}
	return Ok;
}

BOOL CT3EDDoc::ImportOffFile(CString FileName, int blk, int isxobj, int chunk, int objno)
{

	BOOL Ok=TRUE;
	char* buff=new char[255];

	CString s,s2;
	int np, point;
	int i,j,p1,p2,p3,p4,p5,p6,p7,p8;
	float x,y,z;
	struct NOBJDATA NewObjData;

	NewObjData.nVert=0;
	NewObjData.nPoly=0;


	FILE* fin = fopen(FileName, "r");
	if (fin==NULL)
	{
		s.Format("Unable to open '%s'.", FileName);
		AfxMessageBox(s);	
		Ok=FALSE;
	}
	else
	{
		fgets(buff,255,fin); 
		sscanf(buff,"%s",s);
		if (s=="OFF") // Ist it a off file ?
		{
			fgets(buff,255,fin); 
			sscanf(buff, "%d %d %d\n",&NewObjData.nVert,&NewObjData.nPoly, &point); //get number of vertices & polygons
			NewObjData.Vertices=(struct FLOATPT *)malloc(NewObjData.nVert * sizeof(FLOATPT));
			for (i=0;i<NewObjData.nVert;i++) // Read all vertices
			{
				fgets(buff,255,fin); 
				sscanf(buff, "%f %f %f",&x, &y, &z);
				NewObjData.Vertices[i].x=x;
				NewObjData.Vertices[i].y=y;
				NewObjData.Vertices[i].z=z;
			}
			NewObjData.Polys=(struct PDATA *)malloc(NewObjData.nPoly * sizeof(PDATA)); //Get some memory for polygon data
			for (i=0;i<NewObjData.nPoly;i++)
			{
				fgets(buff,255,fin); 
				sscanf(buff, "%d %d %d %d %d %d %d %d %d",&np,&p1,&p2,&p3,&p4,&p5,&p6,&p7,&p8); //Read number of polys, and the vertice indexes
				if ((np>2)&&(np<9)) //only Polygons with vertices betwenn 3 & 8 are supported by T3ED
				{
					if ((np==3)||(np==4))
					{
						NewObjData.Polys[i].vertex[0]=p1;	
						NewObjData.Polys[i].vertex[1]=p2;	
						NewObjData.Polys[i].vertex[2]=p3;	
						NewObjData.Polys[i].vertex[3]=p4;
					}
					if (np==3) NewObjData.Polys[i].vertex[3]=p1;
					if (np>4) // Polygone wwit more than 4 vertices have to be splited
					{
						NewObjData.nPoly++; //Increase number of polys
						NewObjData.Polys=(struct PDATA *)realloc(NewObjData.Polys, NewObjData.nPoly * sizeof(PDATA));
						if (np==5)
						{
							NewObjData.Polys[i].vertex[0]=p1;	//First poly
							NewObjData.Polys[i].vertex[1]=p2;	
							NewObjData.Polys[i].vertex[2]=p3;	
							NewObjData.Polys[i].vertex[3]=p4;
							i++;
							NewObjData.Polys[i].vertex[0]=p1;	//2nd poly	
							NewObjData.Polys[i].vertex[1]=p4;	
							NewObjData.Polys[i].vertex[2]=p5;	
							NewObjData.Polys[i].vertex[3]=p1;
						}
						if (np==6)
						{
							NewObjData.Polys[i].vertex[0]=p1;	//First poly
							NewObjData.Polys[i].vertex[1]=p2;	
							NewObjData.Polys[i].vertex[2]=p3;	
							NewObjData.Polys[i].vertex[3]=p4;
							i++;
							NewObjData.Polys[i].vertex[0]=p1;	//2nd poly	
							NewObjData.Polys[i].vertex[1]=p4;	
							NewObjData.Polys[i].vertex[2]=p5;	
							NewObjData.Polys[i].vertex[3]=p6;
						}
						if (np>6) // Polygone wwit more than 6 vertices have to be splited into 3 !
						{
							NewObjData.nPoly++; //Increase number of polys
							NewObjData.Polys=(struct PDATA *)realloc(NewObjData.Polys, NewObjData.nPoly * sizeof(PDATA));

							if (np==7)
							{
								NewObjData.Polys[i].vertex[0]=p1;	//First poly
								NewObjData.Polys[i].vertex[1]=p2;	
								NewObjData.Polys[i].vertex[2]=p3;	
								NewObjData.Polys[i].vertex[3]=p4;
								i++;
								NewObjData.Polys[i].vertex[0]=p1;	//2nd poly	
								NewObjData.Polys[i].vertex[1]=p4;	
								NewObjData.Polys[i].vertex[2]=p5;	
								NewObjData.Polys[i].vertex[3]=p6;
								i++;
								NewObjData.Polys[i].vertex[0]=p1;	//3nd poly	
								NewObjData.Polys[i].vertex[1]=p6;	
								NewObjData.Polys[i].vertex[2]=p7;	
								NewObjData.Polys[i].vertex[3]=p1;
							}
							if (np==8)
							{
								NewObjData.Polys[i].vertex[0]=p1;	//First poly
								NewObjData.Polys[i].vertex[1]=p2;	
								NewObjData.Polys[i].vertex[2]=p3;	
								NewObjData.Polys[i].vertex[3]=p4;
								i++;
								NewObjData.Polys[i].vertex[0]=p1;	//2nd poly	
								NewObjData.Polys[i].vertex[1]=p4;	
								NewObjData.Polys[i].vertex[2]=p5;	
								NewObjData.Polys[i].vertex[3]=p6;
								i++;
								NewObjData.Polys[i].vertex[0]=p1;	//3nd poly	
								NewObjData.Polys[i].vertex[1]=p6;	
								NewObjData.Polys[i].vertex[2]=p7;	
								NewObjData.Polys[i].vertex[3]=p8;
							}
						}
					}	
				}
				else
				{
					s.Format("Can't import polygons with more than 8 vertices.");
					AfxMessageBox(s);	
					Ok=FALSE;
					i=NewObjData.nPoly;
				}
			}
		}
		else
			Ok=FALSE;

		fclose(fin);
	}
	for (i=0;i<NewObjData.nPoly;i++) //No texture data given
		NewObjData.Polys[i].texture=-1;

	for (i=0;i<NewObjData.nPoly;i++) //Check all vertex indexes
		for (j=0;j<4;j++)
		{
			if (NewObjData.Polys[i].vertex[j]>=NewObjData.nVert) Ok=FALSE;
		}

	if (Ok) Ok=ImportObject(&NewObjData, blk, isxobj, chunk, objno, FALSE);
	dofree(NewObjData.Vertices); //Give back memory
	dofree(NewObjData.Polys);
	return Ok;
}

BOOL CT3EDDoc::ImportObject(struct NOBJDATA *NewObjData, int blk, int isxobj, int chunk, int objno, bool addmode)
{
	BOOL Ok=TRUE;
	int i,j,num,k,l,c=0,start=0;
	FLOATPT *ref, *v1;
	struct XOBJDATA *xo;
	struct TRKBLOCK *b;
	LPPOLYGONDATA p;
	short vno;

	if ((addmode)&&(isxobj==1))
	{
		xo=&(xobj[4*blk+chunk].obj[objno]);
		start=xo->nPolygons;
		c=1; //Create one more Poly in addmode
	}
	if ((addmode)&&(isxobj==0))
	{
		start=poly[blk].obj[chunk].numpoly[objno];
		c=1;//Create one more Poly in addmode
	}

	//Create all Polygons
	i=1; //First polygon was already given
	for (i=1;i<NewObjData->nPoly + c;i++)
		DuplPolygon(blk, isxobj, chunk, objno, (i-1));


	if (isxobj==1) 
	{
		PrepareModifyXobj(4*blk+chunk);
		xo=&(xobj[4*blk+chunk].obj[objno]);
		p=xo->polyData;
		v1=xo->vert;
		for (i=0;i<start;i++) //Goto start polynumber
			p++;
		for (i=start;i<xo->nPolygons;i++,p++) //Loop for all XOBJ polygons
		{
			if (NewObjData->Polys[i - start].texture>-1) // Set texture information if present
			{
				p->texture=NewObjData->Polys[i - start].texture;
				p->texanimdata=NewObjData->Polys[i - start].texanimdata;
				if (bHSMode) p->hs_textflags=NewObjData->Polys[i - start].hs_textflags;
			}
			for (j=0;j<4;j++) // Bring the 4 points in position
			{
				vno=p->vertex[j];
				v1[vno].x=NewObjData->Vertices[NewObjData->Polys[i - start].vertex[j]].x;
				v1[vno].y=NewObjData->Vertices[NewObjData->Polys[i - start].vertex[j]].y;
				v1[vno].z=NewObjData->Vertices[NewObjData->Polys[i - start].vertex[j]].z;
				if (addmode) //Exact Points xyz were given
				{
					v1[vno].x-=xo->ptRef.x;
					v1[vno].y-=xo->ptRef.y;
					v1[vno].z-=xo->ptRef.z;
				}
			}
		}

	}
	// Move Polygons to posistions the object has.
	if (isxobj==0) 
	{
		PrepareModifyTrk(blk);
		PrepareModifyPoly(blk);
		b=&(trk[blk]);
		num=poly[blk].obj[chunk].numpoly[objno];
		p=poly[blk].obj[chunk].poly[objno];
		ref=b->vert+p[0].vertex[0];
		for (i=0;i<num;i++) // Loop for all polygons from object ---for (i=0;i<num;i++)
			if (!(i<start))
			{
				if (NewObjData->Polys[i-start].texture>-1)
				{
					p[i].texture=NewObjData->Polys[i-start].texture;
					p[i].texanimdata=NewObjData->Polys[i-start].texanimdata;
					if (bHSMode) p[i].hs_textflags=NewObjData->Polys[i-start].hs_textflags;
				}
				for (j=0;j<4;j++) // Loop for all four points
				{
					vno=p[i].vertex[j];
					c=0;
					for (k=0;k<i;k++) for (l=0;l<4;l++)
						if (p[k].vertex[l]==vno) c++;
					for (l=0;l<j;l++) if (p[i].vertex[l]==vno) c++;
						if (c==0) 
						{
							b->vert[vno].x=NewObjData->Vertices[NewObjData->Polys[i-start].vertex[j]].x;
							b->vert[vno].y=NewObjData->Vertices[NewObjData->Polys[i-start].vertex[j]].y;
							b->vert[vno].z=NewObjData->Vertices[NewObjData->Polys[i-start].vertex[j]].z;
							if (!addmode) //Create Imported object in block middle
							{
							b->vert[vno].x+=b->ptCentre.x;
							b->vert[vno].y+=b->ptCentre.y;
							b->vert[vno].z+=b->ptCentre.z;
							}
						}
				}
			}
	}
	DeleteDoubleVerticesFromObject(blk, isxobj, chunk, objno); 	// Delete too many created vertices from Object
	return Ok;
}

void CT3EDDoc::CreateObjectData(NOBJDATA *NewObjData, int blk, int isxobj, int chunk, int objno, bool relative)
{
	int i,j,num;
	FLOATPT *ref, *v1;
	struct XOBJDATA *xo;
	struct TRKBLOCK *b;
	LPPOLYGONDATA p;
	short vno, *vlist;

	if (isxobj==0) 
	{
		b=&(trk[blk]);
		num=poly[blk].obj[chunk].numpoly[objno];
		p=poly[blk].obj[chunk].poly[objno];
		ref=b->vert+p[0].vertex[0];
		vlist=(short *)malloc(b->nVertices * sizeof(short));
		NewObjData->nPoly=num;
		NewObjData->nVert=0;
		NewObjData->Polys=(struct PDATA *)malloc(NewObjData->nPoly * sizeof(PDATA)); //Get some memory for polygon data
		for (i=0;i<b->nVertices;i++)
			vlist[i]=-1;
		for (i=0;i<num;i++) // Loop for all polygons from object
		{
			NewObjData->Polys[i].texture=p[i].texture;
			NewObjData->Polys[i].texanimdata=p[i].texanimdata;
			if (bHSMode) NewObjData->Polys[i].hs_textflags=p[i].hs_textflags;

			for (j=0;j<4;j++) // Loop for all four points
			{
				vno=p[i].vertex[j];
				if (vlist[vno]==-1)
				{
					vlist[vno]=NewObjData->nVert;
					NewObjData->nVert++;
					if (NewObjData->nVert==1) //First vertice
						NewObjData->Vertices=(struct FLOATPT *)malloc(NewObjData->nVert * sizeof(FLOATPT));
					else //additional Vertice
						NewObjData->Vertices=(struct FLOATPT *)realloc(NewObjData->Vertices, NewObjData->nVert * sizeof(FLOATPT));

					NewObjData->Vertices[vlist[vno]].x=b->vert[vno].x; //Store position of point
					NewObjData->Vertices[vlist[vno]].y=b->vert[vno].y;
					NewObjData->Vertices[vlist[vno]].z=b->vert[vno].z;
				}
				NewObjData->Polys[i].vertex[j]=vlist[vno];
			}
		}
		if (relative) //Make vertice positions relative to middle
			for (i=0;i<NewObjData->nVert;i++)
			{
				NewObjData->Vertices[i].x-=MemObjMiddle.x;
				NewObjData->Vertices[i].y-=MemObjMiddle.y;
				NewObjData->Vertices[i].z-=MemObjMiddle.z;
			}
	}

	if (isxobj==1) 
	{
		xo=&(xobj[4*blk+chunk].obj[objno]);
		NewObjData->nVert=xo->nVertices;
		NewObjData->nPoly=xo->nPolygons;
		NewObjData->Vertices=(struct FLOATPT *)malloc(NewObjData->nVert * sizeof(FLOATPT));
		NewObjData->Polys=(struct PDATA *)malloc(NewObjData->nPoly * sizeof(PDATA)); //Get some memory for polygon data

		v1=xo->vert;
		for (i=0;i<xo->nVertices;i++)
		{
			NewObjData->Vertices[i].x=v1[i].x;
			NewObjData->Vertices[i].y=v1[i].y;
			NewObjData->Vertices[i].z=v1[i].z;
			if (!relative)
			{
				NewObjData->Vertices[i].x+=xo->ptRef.x;
				NewObjData->Vertices[i].y+=xo->ptRef.y;
				NewObjData->Vertices[i].z+=xo->ptRef.z;
			}
		}
		p=xo->polyData;
		v1=xo->vert;
		for (i=0;i<xo->nPolygons;i++,p++) //Loop for all XOBJ polygons
		{
			NewObjData->Polys[i].texture=p->texture;
			NewObjData->Polys[i].texanimdata=p->texanimdata;
			if (bHSMode) NewObjData->Polys[i].hs_textflags=p->hs_textflags;
			for (j=0;j<4;j++)
				NewObjData->Polys[i].vertex[j]=p->vertex[j];
		}
	}
}

void CT3EDDoc::DeleteDoubleVerticesFromObject(int blk, int isxobj, int chunk, int objno)
{
	int i,j,num,k,l, vno1, vno2, nMerge=0;
	FLOATPT *v1, *v2, mpt1;
	struct XOBJDATA *xo;
	struct TRKBLOCK *b;
	LPPOLYGONDATA p,pp;
	
	// Delete double vertices from Polygon Object
	if (isxobj==0)
	{
		b=&(trk[blk]);
		num=poly[blk].obj[chunk].numpoly[objno];
		p=poly[blk].obj[chunk].poly[objno];
		for (i=0;i<num;i++) // Loop for all polygons from object
			for (j=0;j<4;j++) // Loop for all four points
				{
					vno1=p[i].vertex[j];
					v1=&b->vert[vno1];
					for (k=i+1;k<num;k++) // Loop for all polygons from object
						for (l=0;l<4;l++) // Loop for all four points
						{
							vno2=p[k].vertex[l];
							if (vno1!=vno2) //Not the same vertices ?
							{
								v2=&b->vert[vno2];
								if ((b->vert[vno1].x==b->vert[vno2].x)&&(b->vert[vno1].y==b->vert[vno2].y)&&(b->vert[vno1].z==b->vert[vno2].z))
									MergePoint(v1,v2); //Two points with same x,y,z 
							}

						}
			}
	}
	else // Delete double vertices from XObject
	{
		xo=&(xobj[4*blk+chunk].obj[objno]);
		p=xo->polyData;
		v1=xo->vert;
		for (i=0;i<xo->nPolygons;i++,p++) //Loop for all XOBJ polygons
			for (j=0;j<4;j++)
			{
				vno1=p->vertex[j]; //Get vertex number
				pp=xo->polyData;
				for (k=0;k<xo->nPolygons;k++,pp++) //Loop for all XOBJ polygons
					for (l=0;l<4;l++)
					{
						vno2=pp->vertex[l];
						if (vno1!=vno2)
						{
							if ((v1[vno1].x==v1[vno2].x)&&(v1[vno1].y==v1[vno2].y)&&(v1[vno1].z==v1[vno2].z))
							{
									mpt1.x=v1[vno1].x + xo->ptRef.x;
									mpt1.y=v1[vno1].y + xo->ptRef.y;
									mpt1.z=v1[vno1].z + xo->ptRef.z;
									MergePoint(&mpt1,&mpt1); //Two points with same x,y,z 
							}
						}
					}
			}
	}
}



void CT3EDDoc::CutAmountOfBlocks(int NewnBlocks)
{
	int i,j,n;

	for (i=0;i<undoLevel;i++) 
		DeleteUndo(i);
	undoLevel=0;

	for (i=NewnBlocks;i<nBlocks;i++)
	{
		FreeTrkContents(&(trk[i]));
		FreePolyContents(&(poly[i]));
		/*for (j=0;j<4;j++) 
			FreeXobjContents(&(xobj[4*i+j]));*/ //Causes crashes, I don't know why....better waste memory
	}
	xobj[4*NewnBlocks]=xobj[4*nBlocks]; //Copy global objects at new end
	for (i=1;i<4;i++) //Global object are now in 4*vBlock, other 3 chunks should be empty
		{
			xobj[4*NewnBlocks+i].nobj=0;
			xobj[4*NewnBlocks+i].obj=NULL;
		}


	nBlocks=NewnBlocks;
	col.vroadHead.nrec=(short) (trk[nBlocks-1].nStartPos + trk[nBlocks-1].nPositions); 
	
	trk=(struct TRKBLOCK *)realloc(trk, (1+nBlocks)*sizeof(struct TRKBLOCK));
	poly=(struct POLYGONBLOCK *)realloc(poly, nBlocks*sizeof(struct POLYGONBLOCK));
	xobj=(struct XOBJBLOCK *)realloc(xobj, (4*nBlocks+4)*sizeof(struct XOBJBLOCK));
	
	if (HeightsLoaded)
		hightssim=(struct HEIGHTSSIM *)realloc(hightssim, col.vroadHead.nrec*sizeof(struct HEIGHTSSIM));
	if (spdFALoaded	)
		spdFAbin=(struct SPDFILE *)realloc(spdFAbin, col.vroadHead.nrec*sizeof(struct SPDFILE));
	if (spdRALoaded	)
		spdRAbin=(struct SPDFILE *)realloc(spdRAbin, col.vroadHead.nrec*sizeof(struct SPDFILE));

	//recalcXobj
	n=0;
	for (i=0;i<nBlocks;i++)
		for (j=0;j<trk[i].nXobj;j++)
			trk[i].xobj[j].globalno=n++;

}

void CT3EDDoc::PrepareModifyVRoadHeightsSpdFiles()
{
	int n;

	if (undoLevel>0)
	{
		n=col.vroadHead.nrec;
		if (undoCol[undoLevel-1]==NULL)
		{
			//New undo for vroad
			undoCol[undoLevel-1]=(struct COLVROAD *)malloc(36*n);
			memcpy(undoCol[undoLevel-1],col.vroad,36*n);
		}
		if ((undoHeights[undoLevel-1]==NULL)&&HeightsLoaded)
		{
			//New undo for Heightssim
			undoHeights[undoLevel-1]=(struct HEIGHTSSIM *)malloc(n * sizeof(struct HEIGHTSSIM));
			memcpy(undoHeights[undoLevel-1],hightssim,n * sizeof(struct HEIGHTSSIM));
		}
		if ((undoSpdFa[undoLevel-1]==NULL)&&spdFALoaded)
		{
			//New undo for Heightssim
			undoSpdFa[undoLevel-1]=(struct SPDFILE *)malloc(n * sizeof(struct SPDFILE));
			memcpy(undoSpdFa[undoLevel-1],spdFAbin,n * sizeof(struct SPDFILE));
		}
		if ((undoSpdRa[undoLevel-1]==NULL)&&spdRALoaded)
		{
			//New undo for Heightssim
			undoSpdRa[undoLevel-1]=(struct SPDFILE *)malloc(n * sizeof(struct SPDFILE));
			memcpy(undoSpdRa[undoLevel-1],spdRAbin,n * sizeof(struct SPDFILE));
		}
	}
}

int CT3EDDoc::MergeBlocks(int blk1, int blk2)
{
	int i,j,er;
	long l;
	struct TRKBLOCK *t1,*t2,*tback;
	struct POLYGONBLOCK *p1,*p2,*pback;
	LPPOLYGONDATA pp;
	struct OBJPOLYBLOCK *o;
	//struct COLVROAD *vroadback;
	//struct XOBJDATA *x;
	//struct LIGHTSRC *light;
	//struct SOUNDSRC *sound;
	//struct FLOATPT *v;
	//struct COLVROAD *cvr;
	t1=&(trk[blk1]);
	t2=&(trk[blk2]);

	
	if ((blk1>-1)&&(blk1<nBlocks)&&(blk2>-1)&&(blk2<nBlocks)&&(blk1!=blk2))
	{
		tback=(struct TRKBLOCK *)malloc(sizeof(struct TRKBLOCK));
		memcpy(tback,&trk[blk1],sizeof(struct TRKBLOCK));

		if (t2->nVertices!=0) //get all vertices together
		{
			t1->nVertices+=t2->nVertices; //count together nVertices of both blocks
			t1->vert=(struct FLOATPT *)realloc(t1->vert, 12*t1->nVertices); //Get more space
			memcpy(&t1->vert[tback->nVertices], t2->vert, 12*t2->nVertices);
			assert(t1->vert[tback->nVertices].x==t2->vert[0].x);
			//memcpy((t1->vert+ 12*tback->nVertices), t2->vert, 12*t2->nVertices);
			t1->unknVertices=(unsigned long *)realloc(t1->unknVertices, 4*t1->nVertices);
			memcpy(&t1->unknVertices[tback->nVertices],t2->unknVertices,4*t2->nVertices);
		}
		if (t2->nPositions!=0) //maybe requires more doing...
		{
			t1->nPositions+=t2->nPositions; //count together 
			t1->posData=(struct POSITIONDATA *)realloc(t1->posData, 8*t1->nPositions);
			memcpy(&t1->posData[tback->nPositions],t2->posData,8*t2->nPositions);
			for (l=0;l<t2->nPositions;l++)
			{
				t1->posData[tback->nPositions+l].polygon=t1->posData[tback->nPositions+l].polygon + (short) tback->nPolygons;
			}
			//Just use only the first 8 vroad points
			//t2->nStartPos+=t2->nPositions;
			//t2->nPositions=0; //all vroad points belong to block1 now

		}
		if (t2->nPolygons!=0)
		{
			t1->nPolygons+=t2->nPolygons; //count together 
			t1->polyData=(struct POLYVROADDATA *)realloc(t1->polyData, sizeof(struct POLYVROADDATA)*t1->nPolygons);
			memcpy(&t1->polyData[tback->nPolygons],t2->polyData,sizeof(struct POLYVROADDATA)*t2->nPolygons);
		}
		if (t2->nVRoad!=0) 
		{
			t1->nVRoad+=t2->nVRoad; //count together
			t1->vroadData=(struct VROADDATA *)realloc(t1->vroadData, 12*t1->nVRoad);
			memcpy(&t1->vroadData[tback->nVRoad],t2->vroadData,12*t2->nVRoad);
		}

		p1=&(poly[blk1]);
		p2=&(poly[blk2]);
		pback=(struct POLYGONBLOCK *)malloc(sizeof(struct POLYGONBLOCK));
		memcpy(pback,&(poly[blk1]),sizeof(struct POLYGONBLOCK));
		for (j=0;j<7;j++) //loop for all chunks
		{
			if (p2->sz[j]!=0) //Chunk j has Polygons ?
			{
				p1->sz[j]+=p2->sz[j]; //add number of polys
				p1->poly[j]=(LPPOLYGONDATA)realloc(p1->poly[j],14*p1->sz[j]); //more space to store polydata
				pp=poly[blk1].poly[j]; //Choose first poly of block 1
				for (i=0;i<pback->sz[j];i++,pp++) //Loop to last poly
				{
					er=pp->texture; //Just for debugging
				}
				memcpy(pp, p2->poly[j], 14*p2->sz[j]); //Copy polydata of 2nd Block
				pp=poly[blk1].poly[j]; //Choose first poly of block 1
				for (i=0;i<p1->sz[j];i++,pp++)
				{
					if (i>=pback->sz[j]) //Copied poly of 2nd bock ?
					{
					pp->vertex[0]+=(short)tback->nVertices; //new vertice number, increased by nVertice of first block
					pp->vertex[1]+=(short)tback->nVertices;
					pp->vertex[2]+=(short)tback->nVertices;
					pp->vertex[3]+=(short)tback->nVertices;
					}
				}

			}
		}

		//Move objects
		for (j=0;j<4;j++) //loop for all chunks
		{
			o=&poly[blk2].obj[j]; //Polygon objects
			while (o->nobj>0)
			{
				ChangeObjBlock(blk2,0,j,0,blk1); //Move Object from blk2 to blk1
			}

			while (xobj[4*blk2+j].nobj>0)
			{
				ChangeObjBlock(blk2,1,j,0,blk1); //Move X Object from blk2 to blk1
			}
		}
		while (trk[blk2].nLightsrc>0)
		{
			ChangeObjBlock(blk2,2,j,0,blk1); //Move light sources, type 2 objects from blk2 to blk1
		}
		while (trk[blk2].nSoundsrc>0)
		{
			ChangeObjBlock(blk2,3,j,0,blk1); //Move sound sources, type 3 objects from blk2 to blk1
		}

		/*
		if(trk[blk1].nPositions==trk[blk2].nPositions) //Merge Vroad data of both blocks
		{
			vroadback=(struct COLVROAD *)malloc(36*(trk[blk1].nPositions+trk[blk2].nPositions));
			memcpy(vroadback,&col.vroad[t1->nStartPos],36*(trk[blk1].nPositions+trk[blk2].nPositions));
			for (i=0;i<trk[blk1].nPositions;i++)
			{
				memcpy(&col.vroad[t1->nStartPos+i],&vroadback[i*2],36);
			}

		dofree(vroadback);
		}*/

		//t1->nPolygons+=t2->nPolygons; //count together polygons of both blocks
		//t1->polyData=(struct POLYVROADDATA *)realloc(t1->polyData, t1->nPolygons*sizeof(struct POLYVROADDATA)); //Get more space for polygondata
		//memcpy(t1->polyData[tback->nPolygons],t2->polyData,t2->nPolygons*sizeof(struct POLYVROADDATA));
		//t1->nVertices
		dofree(tback);
		dofree(pback);
		DeleteBlock(blk2);

	}
	else
	{
		blk1=-1;
	}
	return blk1;
}

int CT3EDDoc::DeleteBlock(int blk)
{
	int i,j;
	short nrecback;
	struct TRKBLOCK *t1, *t2;

	t1=&(trk[blk]);
	t2=&(trk[blk+1]);
	nrecback=col.vroadHead.nrec;
	memcpy(&col.vroad[t1->nStartPos],&col.vroad[t2->nStartPos],36*(col.vroadHead.nrec-t2->nStartPos));
	col.vroadHead.nrec=(short)(col.vroadHead.nrec-t2->nPositions);
	//col.vroad=(struct COLVROAD *)realloc(col.vroad, 36*col.vroadHead.nrec);
	col.vroad=(struct COLVROAD *)realloc(col.vroad, nBlocks*8*sizeof(struct COLVROAD));
	for (i=blk+1;i<nBlocks;i++)
	{
		trk[i].nStartPos-=t1->nPositions;
	}

	FreeTrkContents(&(trk[blk]));
	FreePolyContents(&(poly[blk]));
	for (i=blk;i<nBlocks-1;i++)
	{
		memcpy(&(trk[i]),&(trk[i+1]),sizeof(struct TRKBLOCK));
		memcpy(&(poly[i]),&(poly[i+1]),sizeof(struct POLYGONBLOCK));
	}

	for (j=0;j<4;j++)
	{
		FreeXobjContents(&(xobj[4*blk+j]));
	}
	for (i=blk;i<nBlocks;i++)
	{
		for (j=0;j<4;j++)
		{
			memcpy(&(xobj[4*i+j]),&(xobj[4*(i+1)+j]),sizeof(struct XOBJBLOCK));
		}
	}

	if (HeightsLoaded)
		memcpy(&hightssim[trk[blk].nStartPos],&hightssim[trk[blk+1].nStartPos],(nrecback-trk[blk+1].nStartPos) * sizeof(struct HEIGHTSSIM)); //Copy heightsdata in place
	if (spdFALoaded)
		memcpy(&spdFAbin[trk[blk].nStartPos],&spdFAbin[trk[blk+1].nStartPos],(nrecback-trk[blk+1].nStartPos) * sizeof(struct SPDFILE)); //Copy heightsdata in place
	if (spdRALoaded)
		memcpy(&spdRAbin[trk[blk].nStartPos],&spdRAbin[trk[blk+1].nStartPos],(nrecback-trk[blk+1].nStartPos) * sizeof(struct SPDFILE)); //Copy heightsdata in place

	//xobj[4*nBlocks]=xobj[4*vBlock]; //Copy back global objects
	nBlocks--;
	trk=(struct TRKBLOCK *)realloc(trk, (1+ nBlocks)*sizeof(struct TRKBLOCK)); //+1 = Dummy trackblock for global objects
	poly=(struct POLYGONBLOCK *)realloc(poly, nBlocks*sizeof(struct POLYGONBLOCK));
	xobj=(struct XOBJBLOCK *)realloc(xobj,(4*nBlocks+1)*sizeof(struct XOBJBLOCK));

	//Check all Blocks
	for (i=0;i<nBlocks;i++)
	{
		for (j=0;j<8;j++)	//Correct HS neighbours
		{
			if (trk[i].hs_neighbors[j]>blk)	trk[i].hs_neighbors[j]--;
			if ((i==blk)&&(trk[i].hs_neighbors[j]==blk)) trk[i].hs_neighbors[j]--;
		}
		for (j=0;j<300;j++) //Correct Visibility Data
		{
			if (trk[i].nbdData[j].blk>blk)	trk[i].nbdData[j].blk--;
			if ((i==blk)&&(trk[i].nbdData[j].blk==blk)) trk[i].nbdData[j].blk--;
		}
	}

	
	if (blk==nBlocks) blk=0;
	return blk;
}

void CT3EDDoc::MoveLastBlockTo(int dblk)
{
	struct TRKBLOCK tback;
	struct POLYGONBLOCK pback;
	struct XOBJBLOCK xback[4];
	struct COLVROAD cvrback[8];
	struct HEIGHTSSIM hback[8];
	struct SPDFILE sFback[8],sRback[8];

	int i,u,pos=0;


	// Backup Last Block Data
	memcpy(&tback,&(trk[nBlocks-1]),sizeof(struct TRKBLOCK));
	memcpy(&pback,&(poly[nBlocks-1]),sizeof(struct POLYGONBLOCK));
	memcpy(&xback,&(xobj[4*(nBlocks-1)]),4*sizeof(struct XOBJBLOCK));
	memcpy(&cvrback,&col.vroad[tback.nStartPos],tback.nPositions*sizeof(struct COLVROAD));
	if (HeightsLoaded) memcpy(&hback,&hightssim[tback.nStartPos],tback.nPositions * sizeof(struct HEIGHTSSIM)); 
	if (spdFALoaded) memcpy(&sFback,&spdFAbin[tback.nStartPos],tback.nPositions * sizeof(struct SPDFILE));
	if (spdRALoaded) memcpy(&sRback,&spdRAbin[tback.nStartPos],tback.nPositions * sizeof(struct SPDFILE));

	for (i=nBlocks-1;i>dblk;i--) //Move vroad, heightsdata and spdXXdata Data one block up
	{
		memcpy(&col.vroad[trk[i].nStartPos],&col.vroad[trk[i-1].nStartPos],trk[i-1].nPositions*sizeof(struct COLVROAD));
		if (HeightsLoaded) memcpy(&hightssim[trk[i].nStartPos],&hightssim[trk[i-1].nStartPos],trk[i-1].nPositions*sizeof(struct HEIGHTSSIM));
		if (spdFALoaded)  memcpy(&spdFAbin[trk[i].nStartPos],&spdFAbin[trk[i-1].nStartPos],trk[i-1].nPositions * sizeof(struct SPDFILE));
		if (spdRALoaded)  memcpy(&spdRAbin[trk[i].nStartPos],&spdRAbin[trk[i-1].nStartPos],trk[i-1].nPositions * sizeof(struct SPDFILE));
	}

	for (i=nBlocks-1;i>dblk;i--) //Move Data one block up
	{
		memcpy(&(trk[i]),&(trk[i-1]),sizeof(struct TRKBLOCK));
		memcpy(&(poly[i]),&(poly[i-1]),sizeof(struct POLYGONBLOCK));
		memcpy(&(xobj[4*i]),&(xobj[4*(i-1)]),4*sizeof(struct XOBJBLOCK));
		//memcpy(&col.vroad[trk[i].nStartPos],&col.vroad[trk[i-1].nStartPos],trk[i-1].nPositions*sizeof(struct COLVROAD));
	}

	//insert backupped data
	memcpy(&(trk[dblk]),&tback,sizeof(struct TRKBLOCK));
	memcpy(&(poly[dblk]),&pback,sizeof(struct POLYGONBLOCK));
	memcpy(&(xobj[4*dblk]),&xback,4*sizeof(struct XOBJBLOCK));
	for (i=0;i<nBlocks;i++)//recalc vroad nStartPos first
	{
		trk[i].nStartPos=pos;
		pos+=trk[i].nPositions;
	}
	memcpy(&col.vroad[trk[dblk].nStartPos],&cvrback,trk[dblk].nPositions*sizeof(struct COLVROAD));
	if (HeightsLoaded) memcpy(&hightssim[trk[dblk].nStartPos],&hback,trk[dblk].nPositions * sizeof(struct HEIGHTSSIM)); //Copy heightsdata in place
	if (spdFALoaded)  memcpy(&spdFAbin[trk[dblk].nStartPos],&sFback,trk[dblk].nPositions * sizeof(struct SPDFILE));
	if (spdRALoaded)  memcpy(&spdRAbin[trk[dblk].nStartPos],&sRback,trk[dblk].nPositions * sizeof(struct SPDFILE));
	//Adjust visibility
	for(i=0;i<nBlocks ;i++)
	{
		for(u=0;u<300 ;u++)
		{
		if (trk[i].nbdData[u].blk!=-1)
			{
			if (trk[i].nbdData[u].blk>=dblk) trk[i].nbdData[u].blk++;
			}
		}
	}

}

void CT3EDDoc::SetVisibility(int startbl, int endbl, int distance)
{

	int block,visbl,i,n;

	if ((startbl<0)|(startbl>endbl)|(endbl>(nBlocks-1))) return;

	for (block=startbl; block<(endbl+1); block++)
	{
		n=1;
		for(i=0;i<300 ;i++) trk[block].nbdData[i].blk=-1;
		trk[block].nbdData[0].blk=block;
		for (i=1;i<(distance-1);i++)
		{
			visbl=block+i;
			if (visbl>(nBlocks-1)) visbl-=nBlocks;
			trk[block].nbdData[n].blk=visbl;
			n++;

			visbl=block-i;
			if (visbl<0) visbl+=nBlocks;
			trk[block].nbdData[n].blk=visbl;
			n++;
		}

	}

}

bool CT3EDDoc::IsBlockVisibile(int refbl, int block)
{

	bool er;
	int u;
	er=FALSE;
	refbl=RealBlockNum(refbl);
	block=RealBlockNum(block);

	for(u=0;u<300 ;u++)
	{
		if (trk[refbl].nbdData[u].blk==block) 
		{
			er=TRUE;
			u=300;
		}
	}
	return er;
}

void CT3EDDoc::InDecreaseVis(int block, int direction, int InDe)
{
  int  i, blk, vbl, u ,vis[300];

  //Clear Array
  for(u=0;u<300 ;u++)
	  vis[u]=-1;
  
  //Fill Array
   for(u=0;u<300 ;u++)
   {
	 vbl=trk[block].nbdData[u].blk;
	 if (vbl!=-1)
		 vis[vbl]=vbl;
   }

  if (InDe==1) //Increase Visibility
  {
	i=0;u=-1;
	//Serch for first not visible
	do
	{
		blk=RealBlockNum(block+i*direction);
		if (vis[blk]!=-1)
			i++;
		else
		{
			u=blk;
			i=nBlocks;
		}
	} while (i<nBlocks/2);
	if (u!=-1) vis[u]=u; //Make block visible
  }

  if (InDe==-1) //Decrease Visibility
  {
	i=nBlocks/2;u=-1;
	//Serch for farest away visible
	do
	{
		blk=RealBlockNum(block+i*direction);
		if (vis[blk]==-1)
			i--;
		else
		{
			u=blk;
			i=0;
		}
	} while (i>0);
	if (u!=-1) vis[u]=-1; //Make block invisible
  }

  //Write back Data
  u=1;
  for(i=0;i<300 ;i++) trk[block].nbdData[i].blk=-1;
  trk[block].nbdData[0].blk=block;
  for (i=1;i<(nBlocks/2);i++)
	{
		vbl=RealBlockNum(block+i);
		if (vis[vbl]!=-1)
		{
			trk[block].nbdData[u].blk=vis[vbl];
			u++;
		}

		vbl=RealBlockNum(block-i);
		if (vis[vbl]!=-1)
		{
			trk[block].nbdData[u].blk=vis[vbl];
			u++;
		}
	}
}

int CT3EDDoc::RealBlockNum(int blk)
{
	if (blk>=nBlocks) blk=blk-nBlocks;
	if (blk<0) blk=blk+nBlocks;

	return blk;
}

