// T3EDDoc.h : interface of the CT3EDDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_T3EDDOC_H__EA21D10E_8ED3_11D2_8486_000000000000__INCLUDED_)
#define AFX_T3EDDOC_H__EA21D10E_8ED3_11D2_8486_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// #define dofree(x) if ((x)!=NULL) free(x)
#define dofree free
#define UNDO_LEVELS 16
#define MAX_BLOCKS 500

#include "QFSView.h"
//#include "Vectors.h"

// !!! for arrays : structures are aligned to their largest member
// !!! structure members are aligned on their own size (up to the /Zp parameter)
#include <iostream.h> //  Off Read
#include <fstream.h> //  Off Read

typedef struct FLOATPT
{
	float x,z,y;
} FLOATPT;

typedef struct INTPT
{
	long x,z,y;
} INTPT;

typedef struct NEIGHBORDATA  // info on neighbouring block numbers
{
	short blk,unknown;
} NEIGHBORDATA;

typedef struct POSITIONDATA  // enumerate polygons which lie at center
{
	short polygon;
	unsigned char nPolygons;
	char unknown;
	short extraNeighbor1,extraNeighbor2;
} POSITIONDATA;

typedef struct POLYVROADDATA  // vroad data associated with a polygon
{
	unsigned char vroadEntry;
	unsigned char flags;
	unsigned char unknown[6];
	unsigned char hs_minmax[4];
	unsigned char hs_orphan[4];
	unsigned char hs_unknown;
	bool virtualroadedge;
} POLYVROADDATA;

typedef struct VROADDATA  // vroad vectors
{
	short xNorm,zNorm,yNorm;
	short xForw,zForw,yForw;
} VROADDATA;

// WARNING: in the following 2 structures, don't rely on crossindex :
// it's not implemented in NFSHS's REFXOBJ, and due to a bug in T3ED
// refxobj.crossindex doesn't change properly if a polyobj's REFPOLYOBJ
// is deleted !

typedef struct REFXOBJ // description of a block's XOBJects.
{
	struct INTPT pt;
	short unknown1;
	short globalno;  // sequence number in all of the track's xobjs
	short unknown2;
	char crossindex; // position in first POLYOBJ chunk (0 if not in first chunk)
	char collideeffect; //0=Nothing, 1=Solid (like Trees), 2 Hit & fall (Signs), 3 Drivethrough with sound effekt
} REFXOBJ; // !!! does not list the animated XOBJs

/* LET'S JUST FORGET THEM -- UNUSED BY NFS3 ! 
typedef struct REFPOLYOBJ // description of first POLYOBJ chunk
{
	short entrysize;
	char type;       // 4 if XOBJ, 1 if described in POLYOBJ chunk
	char no;         // serial number among the block's objects (useless)
		// !!! not consecutive as the 4 POLYOBJ chunks are interlaced !
	struct INTPT pt;
	unsigned char crossindex; // only present if XOBJ : index in REFXOBJ table
	char unknown[3]; // always 0 in NFS3, not in NFSHS
} REFPOLYOBJ;
*/

typedef struct SOUNDSRC {
	struct INTPT refpoint;
	long type;
} SOUNDSRC;

typedef struct LIGHTSRC {
	struct INTPT refpoint;
	long type;
} LIGHTSRC;

typedef struct TRKBLOCK
{
	struct FLOATPT ptCentre;
	struct FLOATPT ptBounding[4];
	long nVertices; // total stored
	long nHiResVert,nLoResVert,nMedResVert; // #poly[...]+#polyobj
	long nVerticesDup,nObjectVert;
	struct FLOATPT *vert;  // the vertices
	unsigned long *unknVertices; //Shading Information as RGBA 8Bpc
	struct NEIGHBORDATA nbdData[0x12C];  // neighboring blocks
	long nStartPos,nPositions;
	long nPolygons,nVRoad,nXobj,nPolyobj,nSoundsrc,nLightsrc;
	struct POSITIONDATA *posData;  // positions along track
	struct POLYVROADDATA *polyData;  // polygon vroad references & flags
	struct VROADDATA *vroadData;   // vroad vectors
	struct REFXOBJ *xobj;
/* USELESS & BUGGY : let's just forget it
	struct REFPOLYOBJ **polyobj; // table of pointers on the blocks themselves
							     // malloc is linear and 20*nPolyobj, though.
*/
	struct SOUNDSRC *soundsrc;
	struct LIGHTSRC *lightsrc;
	struct FLOATPT hs_ptMin,hs_ptMax;
	long hs_neighbors[8];
} TRKBLOCK;

typedef struct POLYGONDATA
{
	short vertex[4];
	short texture;
	short hs_textflags; // HS texture properties
	unsigned char texanimdata; // used for animated textures
	//AnimInfo (Length : Period AS LSB 3:HSB 5)
	unsigned char unknown2;
} POLYGONDATA;

typedef struct POLYGONDATA *LPPOLYGONDATA;

typedef struct OBJPOLYBLOCK  // a POLYOBJ chunk
{
	long n1;        // total number of polygons
	long n2;        // total number of objects including XOBJs
	long nobj;      // not stored in .FRD : number of type 1 objects
	long *types;    // when 1, there is an associated object; else XOBJ
	long *numpoly;  // size of each object (only for type 1 objects)
	LPPOLYGONDATA *poly;	// the polygons themselves
} OBJPOLYBLOCK;

typedef struct POLYGONBLOCK
{
	long sz[7],szdup[7]; 
	// 7 blocks == low res / 0 / med. res / 0 / high res / 0 / ??central
	LPPOLYGONDATA poly[7];
    struct OBJPOLYBLOCK obj[4]; // the POLYOBJ chunks
		// if not present, then all objects in the chunk are XOBJs
		// the 1st chunk is described anyway in the TRKBLOCK
} POLYGONBLOCK;

/*typedef struct ANIMDATA
{
	struct INTPT pt;
	float costheta,sintheta;
} ANIMDATA; */

/*typedef struct ANIMDATA
{
	struct INTPT pt;
	unsigned char od[8];
	//long costheta, sintheta;
} ANIMDATA;*/

typedef struct ANIMDATA
{
	struct INTPT pt;
	short od1,od2,od3,od4;
	//long costheta, sintheta;
} ANIMDATA;

typedef struct XOBJDATA
{
	long crosstype; // type 4, or more rarely 3 (animated)
	long crossno;   // obj number from REFXOBJ table in TRKBLOCK
	long unknown;
// this section only for type 4 basic objects
	struct FLOATPT ptRef;
	long AnimMemory; // JimDiabolo : nAnimLength * 20 + 8
// this section only for type 3 animated objects
	short unknown3[9]; // 6 first are all alike; [6]==[8]=?; [7]=0
	// in HS, only 6 are used ; 6 = expected 4
	char type3,objno;  // type3==3; objno==index among all block's objects?
	short nAnimLength,AnimDelay; //JimDiabolo : The bigger the AnimDelay, that slower is the movement
	struct ANIMDATA *animData;
// common section
	long nVertices;
	struct FLOATPT *vert;  // the vertices
	long *unknVertices;
	long nPolygons;
	struct POLYGONDATA *polyData;  // polygon data
} XOBJDATA;

typedef struct XOBJBLOCK
{
	long nobj;
	struct XOBJDATA *obj;
} XOBJBLOCK;

#pragma pack(1)
typedef struct TEXTUREBLOCK // WARNING: packed but not byte-aligned !!!
{
	short width,height;
	long unknown1;
	float corners[8]; // 4x planar coordinates == tiling?
	long unknown2;
	char islane;      // 1 if not a real texture (lane), 0 usually
	short texture;    // index in QFS file
} TEXTUREBLOCK;
#pragma pack()

// the associated COL file

#define XBID_TEXTUREINFO 2
#define XBID_STRUCT3D 8
#define XBID_OBJECT 7
#define XBID_OBJECT2 18
#define XBID_VROAD 15

typedef struct XBHEAD {
	long size;
	short xbid;
	short nrec;
} XBHEAD;

typedef struct COLTEXTUREINFO {
	short texture;  // position in .QFS file
	short unknown1; // zero ?
	short unknown2; // texture offset ?
	short unknown3;
} COLTEXTUREINFO;

typedef struct COLVERTEX {
	struct FLOATPT pt; // relative coord
	long unknown;      // like the unknVertices structures in FRD
} COLVERTEX;

typedef struct COLPOLYGON {
	short texture;
	char v[4]; // vertices
} COLPOLYGON;

typedef struct COLSTRUCT3D {
	long size;
	short nVert,nPoly;
	struct COLVERTEX *vertex;
	struct COLPOLYGON *polygon;
} COLSTRUCT3D;

typedef struct COLOBJECT {
	short size;
	char type;     // 1 = basic object, 3 = animated ...
	char struct3D; // reference in previous block
// type 1
	struct INTPT ptRef;
// type 3
	short animLength;
	short unknown;
	struct ANIMDATA *animData; // same structure as in xobjs
} COLOBJECT;

typedef struct COLVECTOR {
	signed char x,z,y,unknown;
} COLVECTOR;

typedef struct COLVECTOR_VIT {
	signed char x,z,y;
} COLVECTOR_VIT;


typedef struct COLVROAD {
	struct INTPT refPt;
	long unknown;  // flags ?
	struct COLVECTOR normal,forward,right;
	long leftWall,rightWall;
} COLVROAD;

typedef struct COLVROAD_VIT {
	struct INTPT refPt;
	union {
		struct { unsigned char RightMask, LeftMask; };
		unsigned short FullMask;
	};
	unsigned char wallKinds;
	char _zero;
	struct COLVECTOR_VIT normal;
	unsigned char scales;
	struct COLVECTOR_VIT forward;
	unsigned char LeftCenter;
	struct COLVECTOR_VIT right;
	unsigned char RightCenter;
	long leftWall,rightWall;
} COLVROAD_VIT;

typedef struct HS_VROADBLOCK { // HS's equivalent to a COLVROAD
    struct FLOATPT refPt;
    struct FLOATPT normal,forward,right;
    float leftWall,rightWall;
	union {
		struct {
			float centers[2];
			short xtraNei[2];
			long scales[2];
			short fullMap;
			short _unknown1;
			char wallKinds[2];
			char _unknown2[2];
		};
		struct {
			float unknown1[2];
			long unknown2[5];
		};
	};
} HS_VROADBLOCK;


typedef struct COLFILE {
	char collID[4]; // 'COLL'
	long version;   // 11
	long fileLength;
	long nBlocks;
	long xbTable[5];
	struct XBHEAD textureHead;
	struct COLTEXTUREINFO *texture;
	struct XBHEAD struct3DHead;
	struct COLSTRUCT3D *struct3D;
	struct XBHEAD objectHead;
	struct COLOBJECT *object;
	struct XBHEAD object2Head;
	struct COLOBJECT *object2;
	struct XBHEAD vroadHead;
	struct COLVROAD *vroad;
	//struct HS_VROADBLOCK *vroad;
	long *hs_extra; // for the extra HS data in COLVROAD
} COLFILE;

typedef struct UNDOINFO {
	struct TRKBLOCK *trk;
	struct POLYGONBLOCK *poly;
	struct XOBJBLOCK *xobj[4];
} UNDOINFO;

typedef struct CLIPBLOCK {
	int clipBlk;
	int clipIsobj;
	int clipChunk;
	int clipNo;
	BOOL bHSMode;
	BOOL clipspdFALoaded;
	BOOL clipspdRALoaded;
	BOOL clipHeightsLoaded;
	struct TRKBLOCK *trk;
	struct POLYGONBLOCK *poly;
	struct XOBJBLOCK *xobj[4];
} CLIPBLOCK;

typedef struct FINDPOLYDATA {
	int texture;
	BOOL	search_texture;
	short   hs_flags;
	BOOL	search_hs_flags;
	unsigned char texanimdata;
	BOOL	search_texanimdata;
	unsigned char	pvrflags;
	BOOL			search_pvrflags;
} FINDPOLYDATA;

typedef struct PDATA
	{
		int vertex[4];
		int texture;
		unsigned char texanimdata;
		short hs_textflags; //HS Texture flags
	} PDATA;

typedef struct NOBJDATA
	{
		int nVert;
		int nPoly;
		FLOATPT *Vertices;
		PDATA *Polys;
	} NOBJDATA;

typedef	struct SETTING {
	int	m_add_replace_block;
	BOOL m_bool_add_replace_block_move;
	BOOL m_bool_add_replace_block_move_z;
	BOOL m_bool_add_replace_block_rotate;
	BOOL m_bool_add_replace_block_expand_lenght;
	BOOL m_bool_add_replace_block_expand_width;
	BOOL m_bool_add_replace_block_paste_block;
	BOOL m_bool_add_replace_block_paste_heights;
	BOOL m_bool_add_replace_block_paste_spdfa;
	BOOL m_bool_add_replace_block_paste_spdra;
	BOOL m_bool_add_replace_block_paste_vroad;
	float	expand_m_floatExpMulti_x;
	float	expand_m_floatExpMulti_y;
	float	expand_m_floatExpMulti_z;
	BOOL	expand_m_chk_connected;
	int		expand_m_radio_alignment;
	float	expand_bl_m_edit_height;
	float	expand_bl_m_edit_lenght;
	float	expand_bl_m_edit_width;
	double	rotate_m_edit_angle;
	float	ray_m_theta;
	float	ray_m_rho;
	int		ray_m_iEndBlock;
	int		ray_m_iStartBlock;
	BOOL	ray_m_bCheckTrack;
	BOOL	ray_m_bCheckObjects;
	float MoveTo_dest_x;
	float MoveTo_dest_y;
	float MoveTo_dest_z;
	BOOL  MoveTo_no_smooth;
	BOOL  ConnectBlocks;
	float ConnectBlocks_dist;
	int	AdjustRoad_AdjustType;
	int AdjustRoad_StartBlock;
	int AdjustRoad_EndBlock;
	BOOL	VRoadHeightsSpd_Chk_Heights;
	BOOL	VRoadHeightsSpd_Chk_SpdFa;
	BOOL	VRoadHeightsSpd_Chk_SpdRa;
	BOOL	VRoadHeightsSpd_Chk_VRoad;
	int		FindPoly_block_choice;
	BOOL	FindPoly_trackpolys;
	int		FindPoly_startbl;
	int		FindPoly_endbl;
	BOOL	FindPoly_extraobj;
	BOOL	FindPoly_polygonobj;
	int		FindPoly_texture;
	BOOL	FindPoly_chk_texture;
	short   FindPoly_m_flags;
	BOOL	FindPoly_chk_m_flags;
	unsigned char FindPoly_texanimdata; //for texture animation data...
	BOOL	FindPoly_chk_texanimdata;
	unsigned char	FindPoly_pvrflags;
	BOOL	FindPoly_chk_pvrflags;
	int		setvisi_int_num_blocks;
	int		setvisi_int_tr_bl_fromto;
	int		setvisi_int_end;
	int		setvisi_edit_start;
	} SETTING;

typedef struct MOVETO {
	BOOL DestPointSet;
	struct FLOATPT DestPt;
} MOVETO;

typedef struct HEIGHTSSIM {
	float heights; // store heights.sim data
} HEIGHTSSIM;

typedef struct SPDFILE { // store spdxA.bin data
	unsigned char Speedvalue;
	unsigned char Bestprejection;
	float turn; 
} SPDFILE;

//HOO: Header file to add a track view (1)
#include "TrackView.h"
//HOO: Place here because it requires some of the above structures (1)

class CT3EDDoc : public CDocument
{
protected: // create from serialization only
	CT3EDDoc();
	DECLARE_DYNCREATE(CT3EDDoc)

// Attributes
public: 
	
	float memory_Zh; //Nappe1: Height_h Copy/Paste
	float memory_Zl; //Nappe1: Height_l Copy/Paste
	float memory_x; //JimD: X position Copy/Paste
	float memory_y; //JimD: Y position Copy/Paste
	int ZCopyMode; //Nappe1: memory_Zh's paste mode.
	char AttachpointCount; //Nappe1 number of Attach Points.
	FLOATPT *AttachpointList[256]; //Nappe1: for attaching objects on the ground.
	FLOATPT MemObjMax, MemObjMin, MemObjMiddle;
	BOOL bEmpty;
	BOOL bHSMode;
	BOOL HeightsLoaded, spdFALoaded, spdRALoaded ;
	BOOL bAutoObjMem;
	char header[28]; /* file header */
	long nBlocks;
	struct TRKBLOCK *trk;
	struct POLYGONBLOCK *poly;
	struct XOBJBLOCK *xobj; // xobj[4*blk+chunk]; global=xobj[4*nblocks]
	long hs_morexobjlen;
	char *hs_morexobj;  // 4N & 4N+1 in HS format (xobj[4N] left empty)
	long nTextures;
	struct TEXTUREBLOCK *texture;
	struct COLFILE col;
	struct SETTING settings;
	struct MOVETO MoveToPoint;
	struct HEIGHTSSIM *hightssim;
	struct SPDFILE *spdFAbin, *spdRAbin;

	//HOO: The track view object (2)
	CTrackView* TrackView;
	BOOL bUpdatedScene;
	BOOL bCreateScene;
	//HOO: Scene can be only created once after reseting (2)

// undo info
	struct UNDOINFO undo[UNDO_LEVELS][MAX_BLOCKS];
	struct COLVROAD *undoCol[UNDO_LEVELS];
	struct FLOATPT undoRefpt[UNDO_LEVELS];
	struct HEIGHTSSIM *undoHeights[UNDO_LEVELS];
	struct SPDFILE *undoSpdFa[UNDO_LEVELS], *undoSpdRa[UNDO_LEVELS];
	int undoRefblk[UNDO_LEVELS],undoOfsx[UNDO_LEVELS],undoOfsy[UNDO_LEVELS];
	int undoLevel;
	BOOL wasInitiallyModified;
	CQFSView qfsView;

// Operations
public:
	void PrepareNewUndo(struct FLOATPT *refpt,int block,int ofsx,int ofsy);
	void PrepareModifyTrk(int blk);
	void PrepareModifyPoly(int blk);
	void PrepareModifyXobj(int blk);
	void FreeTrkContents(struct TRKBLOCK *trk);
	void FreePolyContents(struct POLYGONBLOCK *poly);
	void FreeXobjContents(struct XOBJBLOCK *xobj);
	void DeleteUndo(int i);
	void PerformUndo(BOOL KeepVRoad);

	BOOL CanContainPoint(struct TRKBLOCK *t,struct FLOATPT *pt,float margin);
	void RecalcBoundingBox(int i);
	void RecalcPolyVroad(int blk,int no,struct FLOATPT *optdir=NULL);
	void MovePointBy(struct FLOATPT *refpt,float dx,float dy,float dz);
	void ExtMovePointBy(struct FLOATPT *refpt,float dx,float dy,float dz,float sDist,float sWidth);
	int MoveObjectBy(int blk,int isxobj,int chunk,int no,float dx,float dy,float dz);
	void ChangeObjBlock(int blk1,int isxobj,int chunk,int no,int blk2);
	float Distance(struct FLOATPT &a,struct INTPT &b);
	int GlobalLocalCoord(struct FLOATPT &org,struct FLOATPT &res,struct COLVROAD *c,int start,int minblk,int maxblk);
	void LocalGlobalCoord(struct FLOATPT &org,struct FLOATPT &res,struct COLVROAD *c,int i);
	void MoveBlocks(int blk,float dx,float dy,float dz,int sDist,int sWidth,BOOL extraSmooth);
	void UpdateColVroadVecs(int i);
	void DelObject(int blk,int isxobj,int chunk,int no);
	void DuplObject(int blk,int isxobj,int chunk,int no);
	void NewObject(int blk,int isxobj,int chunk,int texture,short flags,int xobjcrosstype);
	void DelPolygon(int blk,int isxobj,int chunk,int obj,int no);
	void SetCollisionHandling(struct FLOATPT *pt);
	BOOL DoesPointExist(struct FLOATPT *pt);
	int DuplPolygon(int blk,int isxobj,int chunk,int obj,int no);
	void MergePoint(struct FLOATPT *pt1,struct FLOATPT *pt2);
	void SplitPoint(struct FLOATPT *refpt);
	void AdjustRoadWidth(int AdjustType, bool LaneEdges, int Startblk, int Endblk,float size);
	void HS_RecalcMinMaxXY();
	void SetAllTextures(int texture,BOOL drivable,short flags);
	void DelAllObjects(int blk,int isxobj);
	void DelLanePolygons(int blk);
	void ClearSceneryZ();
	void DelTrackPolygon(int blk,int no);
	int DuplTrackPolygon(int blk,int no);
	void SetRoadVertexShading(int nStartBlk, int nEndBlk, unsigned long dwColorValue);
	void SetXObjVertexShading(int nStartBlk, int nEndBlk, unsigned long dwColorValue);
	unsigned long RGBTool(CString inmode, unsigned long dwColorValue, CString outmode); //Nappe1: name says it all. 
	float VectorAngle(struct FLOATPT Vect1, struct FLOATPT Vect2, struct FLOATPT Startpt);
	int PolysNode(int blk, int polynum);
	CString MedDetailFixer(int blk, int AngleLimit);
	CString VertArrayFixer(int blk);
	FLOATPT SumVector(FLOATPT vect1, FLOATPT vect2);
	FLOATPT VectorNormalize (FLOATPT nc);
	FLOATPT QuadNormalVectorCalc(FLOATPT a, FLOATPT b, FLOATPT c, FLOATPT d);
	FLOATPT NormalVectorCalc(FLOATPT a, FLOATPT b, FLOATPT c);
	FLOATPT VertexNormal(int blk, int VertexIndex);
	int GetCenterQuad(int blockIdx, int nodeIdx);
	void CorrectVirtualRoad(int blockIdx, int nodeIdx);
	void CorrectVirtualRoad();
	CString BlockShadingFixer(int blk, float theta, float rho);
	CString ObjectShadingFixer(int blk, float theta, float rho);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CT3EDDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void DeleteContents();
	//}}AFX_VIRTUAL

// Implementation
public:
	bool IsBlockVisibile(int refbl, int block);
	int RealBlockNum(int blk);
	void InDecreaseVis(int block,int direction,int InDe);
	void SetVisibility(int startbl, int endbl, int distance);
	void MoveLastBlockTo(int dblk);
	int DeleteBlock(int blk);
	int MergeBlocks(int blk1, int blk2);
	void PrepareModifyVRoadHeightsSpdFiles();
	void CutAmountOfBlocks(int NewnBlocks);
	void SaveSpeedFile(CFile &spdfileh, struct SPDFILE *spdpoint);
	BOOL LoadSpeedFile(CFile &spdfileh, struct SPDFILE *spdpoint);
	void SaveHeightssim(CFile &heightsfile);
	BOOL LoadHeightssim(CFile& heightsfile);
	void ChangeXobjRefpoint(int blk,int isxobj,int chunk,int no,float ptx,float pty,float ptz);
	void CopyObjectTo(int srcblk,int srcisxobj,int srcchunk,int srcno,int dstblk,int dstchunk);
	BOOL SaveOffFile(struct NOBJDATA *NewObjData, CString FileName);
	void CreateObjectData(struct NOBJDATA *NewObjData, int blk, int isxobj, int chunk, int objno, bool relative);
	BOOL ImportObject(struct NOBJDATA *NewObjData, int blk, int isxobj, int chunk, int objno, bool addmode);
	void DeleteDoubleVerticesFromObject(int blk, int isxobj, int chunk, int objno);
	BOOL ImportOffFile(CString FileName, int blk, int isxobj, int chunk, int no);
	void ConnectBlocks(int blk1, int blk2, float mdist);
	float VRoadLenght(int blk);
	void ExpandVertice(int blk,FLOATPT &ver, float xmulti, float ymulti, float zmulti);
	void ExpandBlock(int blk, float xmulti, float ymulti, float zmulti);
	void RotateINTPT(int blk,INTPT &pt, double angledegree);
	void RotateVertice(int blk, FLOATPT &ver, double angledegree);
	void RotateBlock(int blk, double angledegree);
	void MoveINTPT(INTPT &pt, float dx, float dy, float dz);
	void MoveVertice(FLOATPT &ver, float dx, float dy, float dz);
	void LoadSettings();
	void MoveBlockSimple(int blk,float dx,float dy,float dz);
	BOOL BlockClipboardInfo(struct CLIPBLOCK &cb);
	struct CLIPBLOCK Clipblk;
	//struct NOBJDATA NewObjData;
	void PasteBlockfromClipboard(int mode, int blk, BOOL paste_block, BOOL paste_heights, BOOL paste_spdfa, BOOL paste_spdra, BOOL paste_vroad);
	int CopyBlockToClipboard(int blk,int isxobj,int chunk,int no);
	BOOL PasteXYZfromClipboard(struct FLOATPT *refpt);
	BOOL CopyXYZToClipboard(struct FLOATPT *refpt);
	BOOL CopyTextToClipboard(CString strText);
	void RotateObject(int blk, int isxobj, int chunk, int no, double angledegree);
	BOOL ExpandObject(int blk, int isxobj, int chunk, int no, float xmulti, float ymulti, float zmulti);
	BOOL FindObjMinMax(int blk, int isxobj, int chunk, int no);
	void ChangeTexBlock(int blk,int offset);
	int MinTexBlock(int blk,int MinTextur);
	void ChangeTexObject(int blk, int isxobj, int chunk, int no, int offset,BOOL HSandHHFlags, short flags, unsigned char texanimdata);
	int MinTexObject(int blk,int isxobj,int chunk,int no,int MinTextur);
	virtual ~CT3EDDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	BOOL LoadFRD(CArchive& ar);
	BOOL LoadCOL(CFile& coll);
	void SaveFRD(CArchive& ar);
	void SaveCOL(CFile& coll);
	BOOL LoadHSFRD(CArchive& ar,long nPos);
	void CT3EDDoc::SaveHSFRD(CArchive& ar);

// Generated message map functions
protected:
	//{{AFX_MSG(CT3EDDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void ExpandINTPT(int blk, INTPT &pt, float xmulti, float ymulti, float zmulti);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_T3EDDOC_H__EA21D10E_8ED3_11D2_8486_000000000000__INCLUDED_)
