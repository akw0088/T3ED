#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <vector>
#include <algorithm>
#include <iostream>
#include <cassert>
#include <string.h>
#include "Gart.h"

#define CROSSP(dest,a,b) \
  dest[0]=a[1]*b[2]-a[2]*b[1]; \
  dest[1]=a[2]*b[0]-a[0]*b[2]; \
  dest[2]=a[0]*b[1]-a[1]*b[0];
#define DOTP(a,b) ((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define NORMALIZE(v) { \
  float scale = 1.0/sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]); \
  v[0] *= scale; \
  v[1] *= scale; \
  v[2] *= scale; \
}
#define COPY(dest,src) \
  dest[0]=src[0]; \
  dest[1]=src[1]; \
  dest[2]=src[2];
#define ADD(dest,a,b) \
  dest[0]=a[0]+b[0]; \
  dest[1]=a[1]+b[1]; \
  dest[2]=a[2]+b[2];
#define SUB(dest,a,b) \
  dest[0]=a[0]-b[0]; \
  dest[1]=a[1]-b[1]; \
  dest[2]=a[2]-b[2];
#define MAD(dest,a,b,t) \
  dest[0]=a[0]+b[0]*t; \
  dest[1]=a[1]+b[1]*t; \
  dest[2]=a[2]+b[2]*t;
#define CROSS_SUM(v0,v1,p) ((v0[0]-p[0])*(v1[1]-p[1])-(v1[0]-p[0])*(v0[1]-p[1]))
#define BIG_VALUE (1.0e36)
#define SMALL_VALUE (0.0)

#define BV_DIRECTIONS 3

const float bvDirs[BV_DIRECTIONS*3] = {
    1.0, 0.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 0.0, 1.0
};

struct BVolume {
    float minc[BV_DIRECTIONS];
    float maxc[BV_DIRECTIONS];
};

#define RAY_BEGIN (preBV+BV_DIRECTIONS*2)
#define BVDISTANCE  (preBV[BV_DIRECTIONS*3])
bool RayIntersection(const BVolume* volume, float* preBV) {
    float t1, t2;
    if (preBV[0] < 0.0) {
        t1 = volume->maxc[0]*preBV[0] + preBV[1];
        t2 = volume->minc[0]*preBV[0] + preBV[1];
    }
    else if (preBV[0] > 0.0) {
        t1 = volume->minc[0]*preBV[0] + preBV[1];
        t2 = volume->maxc[0]*preBV[0] + preBV[1];
    }
    else if (RAY_BEGIN[0] < volume->minc[0] || RAY_BEGIN[0] > volume->maxc[0])
        return(0);
    else {
        t1 = -BIG_VALUE;
        t2 = BIG_VALUE;
    }

    if (t2 <= SMALL_VALUE) return(0);	

    float c1, c2;
    if (preBV[2] < 0.0) {
        c1 = volume->maxc[1]*preBV[2] + preBV[3];
        c2 = volume->minc[1]*preBV[2] + preBV[3];
    }
    else if (preBV[2] > 0.0) {
        c1 = volume->minc[1]*preBV[2] + preBV[3];
        c2 = volume->maxc[1]*preBV[2] + preBV[3];
    }
    else if (RAY_BEGIN[1] < volume->minc[1] || RAY_BEGIN[1] > volume->maxc[1]) return(0);
    else {
        c1 = -BIG_VALUE;
        c2 = BIG_VALUE;
    }
    if (c1 > t1) t1 = c1;
    if (c2 < t2) t2 = c2;
    if (c2 < SMALL_VALUE || t2 <= t1) return(0);	

    if (preBV[4] < 0.0) {
        c1 = volume->maxc[2]*preBV[4] + preBV[5];
        c2 = volume->minc[2]*preBV[4] + preBV[5];
    }
    else if (preBV[4] > 0.0) {
        c1 = volume->minc[2]*preBV[4] + preBV[5];
        c2 = volume->maxc[2]*preBV[4] + preBV[5];
    }
    else if (RAY_BEGIN[2] < volume->minc[2] || RAY_BEGIN[2] > volume->maxc[2]) return(0);
    else {
        c1 = -BIG_VALUE;
        c2 = BIG_VALUE;
    }
    if (c1 > t1) t1 = c1;
    if (c2 < t2) t2 = c2;
    if (c2 < SMALL_VALUE || t2 <= t1) return(0);

    BVDISTANCE = t1;

    return(1);
}

inline void CalcBVolume(const GartPolygon* poly, BVolume* volume) {
    float** vertices = poly->vertices;
    for(int j=0;j<BV_DIRECTIONS;++j) 
        volume->minc[j] = volume->maxc[j] = DOTP(*vertices, (bvDirs+j*3));

    ++vertices;
    for(int i=1;i<poly->count;++i,++vertices) {
        for(int j=0;j<BV_DIRECTIONS;++j) {
            float temp = DOTP(*vertices, (bvDirs+j*3));
            if(temp < volume->minc[j])
                volume->minc[j] = temp;
            else if(temp > volume->maxc[j])
                volume->maxc[j] = temp;
        }
    }
}

void Merge(BVolume* volume1, const BVolume* volume2) {
    for(int i=0;i<BV_DIRECTIONS;++i) {
        if(volume2->minc[i] < volume1->minc[i])
            volume1->minc[i] = volume2->minc[i];
        if(volume2->maxc[i] > volume1->maxc[i])
            volume1->maxc[i] = volume2->maxc[i];
    }
}

inline float min(float a, float b) { return(a<b?a:b); }
inline float max(float a, float b) { return(a>b?a:b); }

void Merge(BVolume* volume, const BVolume* volume1, const BVolume* volume2) {
    for(int i=0;i<BV_DIRECTIONS;++i) {
        volume->minc[i] = min(volume1->minc[i],volume2->minc[i]);
        volume->maxc[i] = max(volume1->maxc[i],volume2->maxc[i]);
    }
}

inline float Area(const BVolume* volume) {
    float d0 = volume->maxc[0]-volume->minc[0];
    float d1 = volume->maxc[1]-volume->minc[1];
    float d2 = volume->maxc[2]-volume->minc[2];
    return(d0*(d1+d2)+d1*d2);
}

inline float MergedArea(const BVolume* volume1, const BVolume* volume2) {
    float d0 = max(volume1->maxc[0],volume2->maxc[0])-min(volume1->minc[0],volume2->minc[0]);
    float d1 = max(volume1->maxc[1],volume2->maxc[1])-min(volume1->minc[1],volume2->minc[1]);
    float d2 = max(volume1->maxc[2],volume2->maxc[2])-min(volume1->minc[2],volume2->minc[2]);
    return(d0*(d1+d2)+d1*d2);
}

bool IsInside(const BVolume* volume1, const BVolume* volume2) {
    for(int i=0;i<BV_DIRECTIONS;++i) {
        if(volume1->maxc[i] > volume2->maxc[i]) return(0);
        if(volume1->minc[i] < volume2->minc[i]) return(0);
    }
    return(1);
}

struct __Polygon {
    float  normal[3];  // Jokaisella polygonin pisteellä normal*p=planeC
    float  planeC;
    int     bestProjection; // 0=YZ, 1=ZX, 2=XY
    float* projectedVertices;
    int mailbox;
    int     vertexCount;
};

struct KdNode {
    KdNode* neg;
    union {
        KdNode* pos;
        __Polygon** polygonsBegin;
    };
    union {
        int     splittingCoord;
        __Polygon** polygonsEnd;
    };
    float  splittingValue;
};

int KdTreeDepth(KdNode* node) {
    if(node->neg==0) return(0);
    int depth1 = KdTreeDepth(node->neg);
    int depth2 = KdTreeDepth(node->pos);

    if(depth1>depth2) return(depth1+1);
    else              return(depth2+1);
}

struct KdNodeRef {
    KdNode* node;
    float b;
    float bv[3];

    KdNodeRef() {}
};

struct Polyref {
    __Polygon* poly;
    float     intervals[6];
    int        vertexCount;
    float*    vertices;
};

inline void UpdateIntervals(Polyref* polyref) {
    float* cur = polyref->vertices;
    for(int j=0;j<3;++j) {
        polyref->intervals[j*2] = cur[j];
        polyref->intervals[j*2+1] = cur[j];
    }
    cur += 3;
    for(int i=1;i<polyref->vertexCount;++i, cur+=3) {
        for(int j=0;j<3;++j) {
            if(cur[j] < polyref->intervals[j*2])
                polyref->intervals[j*2] = cur[j];
            else if(cur[j] > polyref->intervals[j*2+1])
                polyref->intervals[j*2+1] = cur[j];
        }
    }
}

struct SwitchPoint {
    float value;
    int    dir;

    bool operator<(const SwitchPoint& b) const {
        return(value < b.value || (value==b.value && dir>b.dir));
    }
};

float FindBestSplit(int axis, float* intervals, std::vector<Polyref*>& polyrefs,
                     float& _split) {
    int polygonCount = (int)polyrefs.size();
    SwitchPoint* spoints = new SwitchPoint[polygonCount*2];
    for(int i=0;i<polygonCount;++i) {
        spoints[i*2].value = polyrefs[i]->intervals[axis*2];
        spoints[i*2].dir   = 1;
        spoints[i*2+1].value = polyrefs[i]->intervals[axis*2+1];
        spoints[i*2+1].dir   = -1;
    }
    
    std::sort(spoints, spoints+polygonCount*2);

    ////
    float cost0, cost1;
    float a, b;
    {
        float l1, l2;
        if(axis==0) {
            a = intervals[0];
            b = intervals[1];
            l1 = intervals[3]-intervals[2];
            l2 = intervals[5]-intervals[4];
        }
        else if(axis==1) {
            a = intervals[2];
            b = intervals[3];
            l1 = intervals[1]-intervals[0];
            l2 = intervals[5]-intervals[4];
        }
        else {
            a = intervals[4];
            b = intervals[5];
            l1 = intervals[1]-intervals[0];
            l2 = intervals[3]-intervals[2];
        }
        cost0 = l1*l2;
        cost1 = l1+l2;
    }  
    float bestCost = BIG_VALUE;
    int    bestSplitId = -1;

    int negCount = 0;
    int posCount = polygonCount;

    for(i=0;i<2*polygonCount;++i) {
        if( spoints[i].dir == -1 )
            --posCount;

        float split = spoints[i].value;
        if( split > a && split < b) {
            float cost = cost0*(posCount+negCount)+cost1*( (b-split)*posCount + (split-a)*negCount );
            if( cost < bestCost ) {
                bestCost = cost;
                bestSplitId = i;
            }
        }

        if( spoints[i].dir == 1 )
            ++negCount;

        assert(posCount+negCount >= polygonCount);
    }
    assert(negCount==polygonCount);
    assert(posCount==0);

    _split = spoints[bestSplitId].value;
    delete[](spoints);
    return(bestCost);
}

float FindBestSplit(float* intervals, std::vector<Polyref*>& polyrefs,
                     float& split, int& axis) {
    axis = 0; 
    float cost = FindBestSplit(0, intervals, polyrefs, split);

    float split1;
    float cost1 = FindBestSplit(1, intervals, polyrefs, split1);

    if(cost1 < cost) {
        cost = cost1;
        split = split1;
        axis = 1;
    }

    cost1 = FindBestSplit(2, intervals, polyrefs, split1);

    if(cost1 < cost) {
        cost = cost1;
        split = split1;
        axis = 2;
    }

    return(cost);
}

KdNode* CreateNode(float* intervals, std::vector<Polyref*>& polyrefs,
                   int maxDepth, int maxFails) {
/*    std::cout << "CreateNode: ("
              << intervals[0] << " "
              << intervals[1] << " "
              << intervals[2] << " "
              << intervals[3] << " "
              << intervals[4] << " "
              << intervals[5] << ") polys: " << polyrefs.size() << std::endl;
*/
    //KdNode* ret = new KdNode;
	KdNode retNode;				//Fixed by Nappe1 (Leak Fix)
	KdNode* ret = &retNode;		//Fixed by Nappe1 (Leak Fix)

    if(polyrefs.empty()) {
        ret->neg = 0;
        ret->polygonsBegin = 0;
        return(ret);
    }

    float split;
    int    axis;

    float l0 = intervals[1]-intervals[0];
    float l1 = intervals[3]-intervals[2];
    float l2 = intervals[5]-intervals[4];
    float area = l0*(l1+l2)+l1*l2;
    float splittingCost = FindBestSplit(intervals, polyrefs, split, axis); /*+1.1*area;*/
    float leafCost = area*polyrefs.size();
/*
    std::cout << "  splittingCost = " << splittingCost << std::endl;
    std::cout << "  leafCost      = " << leafCost << std::endl;
    std::cout << "  area          = " << area << std::endl;        
*/
    if( leafCost*0.75 < splittingCost )
        --maxFails;

    if( polyrefs.size()<2 || maxDepth==0|| maxFails==0 ) {
        ret->neg = 0;
        ret->polygonsBegin = new __Polygon*[polyrefs.size()];
        ret->polygonsEnd = ret->polygonsBegin+polyrefs.size();
        for(unsigned int i=0;i<polyrefs.size();++i)
            ret->polygonsBegin[i] = polyrefs[i]->poly;
    }
    else {
        std::vector<Polyref*> negPolys;
        std::vector<Polyref*> posPolys;
    
        for(std::vector<Polyref*>::iterator it = polyrefs.begin();
            it!=polyrefs.end();++it) {
            Polyref* cur = *it;
            if(cur->intervals[axis*2+1]<=split)
                negPolys.push_back(cur);
            else if(cur->intervals[axis*2]>=split)
                posPolys.push_back(cur);
            else {
                int count = cur->vertexCount;
                float* vertices = cur->vertices;

                int negSize=0;
                int posSize=0;
                float prev[3];
                COPY(prev, (vertices+(count-1)*3));
                for(int i=0;i<count;++i) {
                    if((prev[axis]-split)*(vertices[axis]-split) < 0.0) {
                        ++negSize;
                        ++posSize;
                    }
                    if(vertices[axis] <= split)
                        ++negSize;
                    if(vertices[axis] >= split)
                        ++posSize;
                    COPY(prev, vertices);
                    vertices += 3;
                }

                float* negVert=new float[3*negSize];
                float* posVert=new float[3*posSize];

                Polyref* negPoly = new Polyref;
                negPoly->poly = cur->poly;
                negPoly->vertexCount = negSize;
                negPoly->vertices = negVert;

                Polyref* posPoly = new Polyref;
                posPoly->poly = cur->poly;
                posPoly->vertexCount = posSize;
                posPoly->vertices = posVert;

                vertices = cur->vertices;
                for(i=0;i<count;++i, vertices+=3) {
                    if((prev[axis]-split)*(vertices[axis]-split) < 0.0) {
                        float t = (split-prev[axis])/(vertices[axis]-prev[axis]);
                        negVert[0] = prev[0]+(vertices[0]-prev[0])*t;
                        negVert[1] = prev[1]+(vertices[1]-prev[1])*t;
                        negVert[2] = prev[2]+(vertices[2]-prev[2])*t;
                        negVert[axis] = split;
                        COPY(posVert, negVert);
                        posVert += 3;
                        negVert += 3;
                        --posSize;
                        --negSize;
                    }
                    if(vertices[axis] <= split) {
                        COPY(negVert, vertices);
                        negVert+=3;
                        --negSize;
                    }
                    if(vertices[axis] >= split) {
                        COPY(posVert, vertices);
                        posVert+=3;
                        --posSize;
                    }
                    COPY(prev, vertices);
                }
                UpdateIntervals(negPoly);
                UpdateIntervals(posPoly);

                negPolys.push_back(negPoly);
                posPolys.push_back(posPoly);               
            }
        }
    
        float newIntervals[6];
        for(int i=0;i<3;++i) 
            if(i == axis) {
                newIntervals[i*2] = intervals[i*2];
                newIntervals[i*2+1] = split;        
            }
            else {
                newIntervals[i*2] = intervals[i*2];
                newIntervals[i*2+1] = intervals[i*2+1];            
            }    
        ret->neg = CreateNode(newIntervals, negPolys, maxDepth-1,
                              maxFails);
        newIntervals[axis*2] = split;
        newIntervals[axis*2+1] = intervals[axis*2+1];
        ret->pos = CreateNode(newIntervals, posPolys, maxDepth-1,
                              maxFails);
        ret->splittingCoord = axis;
        ret->splittingValue = split;
    }
	return(ret);    
}

struct __Geometry {
    __Polygon* polygonsBegin;
    __Polygon* polygonsEnd;
    float     intervals[6];
    KdNode*    root;
    int        rayid;
    KdNodeRef* nodeStack;
};

#define V(i) (poly->vertices[(i)])



Geometry* BuildGeometry(int polygonCount, GartPolygon* polygons) {
    __Geometry* geometry = new __Geometry;

    geometry->polygonsBegin = new __Polygon[polygonCount];
    geometry->polygonsEnd   = geometry->polygonsBegin+polygonCount;
    geometry->rayid         = 0;
        
    GartPolygon* poly = polygons;
    for(__Polygon* cur = geometry->polygonsBegin;cur!=geometry->polygonsEnd;++cur,++poly) {
        float edge1[3], edge2[3];
        
        SUB(edge1, V(poly->count-1), V(0));
        SUB(edge2, V(1), V(0));
        CROSSP(cur->normal, edge2, edge1);
        NORMALIZE(cur->normal);
        cur->planeC = DOTP(cur->normal, V(0));
        
        float absnormal0 = fabs(cur->normal[0]);
        float absnormal1 = fabs(cur->normal[1]);
        float absnormal2 = fabs(cur->normal[2]);
        
        cur->projectedVertices = new float[6];
        
        float* curv = cur->projectedVertices;        
        if(absnormal0 > absnormal1 && absnormal0 > absnormal2) {
            cur->bestProjection = 0;
            
            for(int i=0;i<3;++i) {
                *(curv++) = V(i)[1];
                *(curv++) = V(i)[2];
            }
        }
        else if(absnormal1 > absnormal2) {
            cur->bestProjection = 1;
            
            for(int i=0;i<3;++i) {
                *(curv++) = V(i)[2];
                *(curv++) = V(i)[0];
            }
        }
        else {
            cur->bestProjection = 2;
            
            for(int i=0;i<3;++i) {
                *(curv++) = V(i)[0];
                *(curv++) = V(i)[1];
            }
        }
        
        // Orientate projected polygons
        float area=0.0;
        curv = cur->projectedVertices+2;
        for(int i=poly->count-2;i>0;--i,curv+=2)
            area += CROSS_SUM(curv, (curv+2), cur->projectedVertices);
        if( area < 0.0 ) {
            float* a = cur->projectedVertices+2;
            float* b = cur->projectedVertices+2*(poly->count-1);
            while(a<b) {
                float temp = a[0];
                a[0] = b[0];
                b[0] = temp;
                temp = a[1];
                a[1] = b[1];
                b[1] = temp;
                a += 2;
                b -= 2;
            }
        }
        cur->mailbox = -1;
    }
    
    //////
    for(int j=0;j<3;++j) {
        geometry->intervals[j*2] = BIG_VALUE;
        geometry->intervals[j*2+1] = -BIG_VALUE;    
    }
    Polyref* _polyrefs = new Polyref[polygonCount];
    std::vector<Polyref*> polyrefs(polygonCount);
    for(int i=0;i<polygonCount;++i) {
        polyrefs[i] = &_polyrefs[i];
        _polyrefs[i].poly = geometry->polygonsBegin+i;
        _polyrefs[i].vertexCount = polygons[i].count;
        _polyrefs[i].vertices = new float[3*polygons[i].count];
        for(int j=0;j<polygons[i].count;++j) {
            _polyrefs[i].vertices[3*j] = polygons[i].vertices[j][0];
            _polyrefs[i].vertices[3*j+1] = polygons[i].vertices[j][1];
            _polyrefs[i].vertices[3*j+2] = polygons[i].vertices[j][2];
        }
        UpdateIntervals(polyrefs[i]);
        for(j=0;j<3;++j) {        
            if(_polyrefs[i].intervals[j*2] < geometry->intervals[j*2])
                geometry->intervals[j*2] = _polyrefs[i].intervals[j*2];
            if(_polyrefs[i].intervals[j*2+1] > geometry->intervals[j*2+1])
                geometry->intervals[j*2+1] = _polyrefs[i].intervals[j*2+1];            
        }
    }
    
    int dmax = (int)(1.2*log(polygonCount)/log(2.0)+2.0);
    int failmax = (int)(2+0.2*dmax);
    geometry->root = CreateNode(geometry->intervals, polyrefs,
                                dmax, failmax);
    //for(i=0;i<polygonCount;++i)		//Addon By: Nappe1 (Leak Fix)
	//	delete[](_polyrefs[i].vertices); //Addon By: Nappe1 (Leak Fix)
	delete[](_polyrefs);
    
    int treeDepth = KdTreeDepth(geometry->root);
    std::cout << "TreeDepth: " << treeDepth << std::endl;
    
    geometry->nodeStack = new KdNodeRef[treeDepth];

    return((Geometry*)geometry);    
}

void DeleteKdNode(KdNode* node) {
	if(node->neg) {
		DeleteKdNode(node->pos);    
		DeleteKdNode(node->neg);
	}
	else {
		delete(node->polygonsBegin);
	}
	delete(node);
}

void DestroyGeometry(Geometry* _geometry) {
    __Geometry* geometry = (__Geometry*)_geometry;

    
    for(__Polygon* cur = geometry->polygonsBegin;cur!=geometry->polygonsEnd;++cur) 
        delete[](cur->projectedVertices);
    delete[](geometry->polygonsBegin);
    delete[](geometry->nodeStack); //Addon by: Nappe1 (Fixing leak.)
	DeleteKdNode(geometry->root);
	delete(geometry);

}

bool RayIntersection(Geometry* _geometry,
                     const float* rayBegin, const float* rayDir,
                     GartIntersectionInfo* info) {
    __Geometry* geometry = (__Geometry*)_geometry;

    float     bestDistance = BIG_VALUE;
    __Polygon* bestPoly = 0;

    float a=BIG_VALUE, b=-BIG_VALUE;
    float rayDirInv[3];
    for(int i=0;i<3;++i) {
        float ta, tb;
        float scale = 1.0/rayDir[i];
        rayDirInv[i] = scale;
        if( rayDir[i] > 0.0 ) {
            ta = (geometry->intervals[i*2]-rayBegin[i])*scale;
            tb = (geometry->intervals[i*2+1]-rayBegin[i])*scale;
        }
        else if( rayDir[i] < 0.0 ) {
            ta = (geometry->intervals[i*2+1]-rayBegin[i])*scale;
            tb = (geometry->intervals[i*2]-rayBegin[i])*scale;
        }
        else {
            ta = 0.0; // ??????????????????????????????
            tb = 0.0;
        }
        if( ta < a ) a = ta;
        if( tb > b ) b = tb;
    }
    
    if( a >= b ) return(0);
    
    int rayid    = geometry->rayid++;
    
    KdNodeRef* nodeStack = geometry->nodeStack;
    KdNodeRef* nodeStackBottom = nodeStack;
    
    KdNode* curNode = geometry->root;
    
    float av[3];
    float bv[3];
    
    av[0] = rayBegin[0]+rayDir[0]*a;
    av[1] = rayBegin[1]+rayDir[1]*a;
    av[2] = rayBegin[2]+rayDir[2]*a;
    
    bv[0] = rayBegin[0]+rayDir[0]*b;
    bv[1] = rayBegin[1]+rayDir[1]*b;
    bv[2] = rayBegin[2]+rayDir[2]*b;
    
    while(1) {
        if(curNode->neg) {
            int splittingCoord = curNode->splittingCoord;
            float splittingValue = curNode->splittingValue;
            
            if(av[splittingCoord] <= splittingValue) {
                if(bv[splittingCoord] <= splittingValue) {
                    curNode = curNode->neg;
                    continue;
                }
                else {
                    nodeStack->node = curNode->pos;
                    curNode = curNode->neg;
                }
            }
            else {
                if(bv[splittingCoord] >= splittingValue) {
                    curNode = curNode->pos;
                    continue;
                }
                else {
                    nodeStack->node = curNode->neg;
                    curNode = curNode->pos;
                }
            }
            nodeStack->b = b;
            COPY(nodeStack->bv, bv);
            ++nodeStack;
            b = (splittingValue - rayBegin[splittingCoord]) * rayDirInv[splittingCoord];
    
            MAD(bv,rayBegin,rayDir,b);
        }
        else {
            if(curNode->polygonsBegin) 
                for(__Polygon** curp = curNode->polygonsBegin;curp!=curNode->polygonsEnd;++curp) {
                    __Polygon* cur = *curp;
                    if(cur->mailbox != rayid) {
                        cur->mailbox = rayid;
                    
                        float dp = DOTP(cur->normal,rayDir);
                        if(dp >= 0.0) 
                            continue;
                        
                        float t = cur->planeC-DOTP(cur->normal,rayBegin);
                        
                        if(t >= 0.0 )
                            continue;
                        
                        t /= dp;
                        
                        if( t > bestDistance )
                            continue;
                        
                        float projectedPoint[2];
                        
                        if(cur->bestProjection==0) {
                            projectedPoint[0] = rayBegin[1] + rayDir[1]*t;
                            projectedPoint[1] = rayBegin[2] + rayDir[2]*t;
                        }
                        else if(cur->bestProjection==1) {
                            projectedPoint[0] = rayBegin[2] + rayDir[2]*t;
                            projectedPoint[1] = rayBegin[0] + rayDir[0]*t;
                        }
                        else {
                            projectedPoint[0] = rayBegin[0] + rayDir[0]*t;
                            projectedPoint[1] = rayBegin[1] + rayDir[1]*t;
                        }            
                        
                        float* pvertex = cur->projectedVertices;
                        
                        float a0 = pvertex[0]-projectedPoint[0];
                        float a1 = pvertex[1]-projectedPoint[1];
                        float b0 = pvertex[2]-projectedPoint[0];
                        float b1 = pvertex[3]-projectedPoint[1];
                        
                        if( a0*b1 < b0*a1 )
                            continue;
                        
                        float c0 = pvertex[4]-projectedPoint[0];
                        float c1 = pvertex[5]-projectedPoint[1];
                        
                        if( b0*c1 < c0*b1 || c0*a1 < a0*c1 )
                            continue;
      
                        bestDistance = t;
                        bestPoly = cur;
                    }
                }
    
            if(bestDistance <= b || nodeStack==nodeStackBottom) break;
            --nodeStack;        
            curNode = nodeStack->node;
            a = b;
            COPY(av, bv);
            
            b = nodeStack->b;        
            COPY(bv, nodeStack->bv);
        }
    }

    if(bestPoly == 0)
        return(0);

    info->polyid = bestPoly-geometry->polygonsBegin;
    info->distance = bestDistance;
    COPY(info->normal, bestPoly->normal);
    MAD(info->location, rayBegin, rayDir, bestDistance);
    return(1);    
}

bool RayShadowIntersection(Geometry* _geometry,
                           const float* rayBegin, const float* rayDir,
                           float bestDistance) {
    __Geometry* geometry = (__Geometry*)_geometry;

    float preBV[BV_DIRECTIONS*3+1];
    
    for(int i=0;i<BV_DIRECTIONS;++i) {
        float c0 = DOTP(rayBegin, (bvDirs+i*3));
        float c1 = DOTP(rayDir, (bvDirs+i*3));
        if(c1==0.0) {
            preBV[2*i] = 0.0;
            preBV[2*i+1] = 0.0;
        }
        else {
            preBV[2*i] = 1.0/c1;
            preBV[2*i+1] = -c0/c1;
        }
        RAY_BEGIN[i] = c0;    
    }
    
    float a=0.0, b=BIG_VALUE;
    float rayDirInv[3];
    for(i=0;i<3;++i) {
        float ta, tb;
        float scale = 1.0/rayDir[i];
        rayDirInv[i] = scale;
        if( rayDir[i] > 0.0 ) {
            ta = (geometry->intervals[i*2]-rayBegin[i])*scale;
            tb = (geometry->intervals[i*2+1]-rayBegin[i])*scale;
        }
        else if( rayDir[i] < 0.0 ) {
            ta = (geometry->intervals[i*2+1]-rayBegin[i])*scale;
            tb = (geometry->intervals[i*2]-rayBegin[i])*scale;
        }
        else {
            ta = 0.0; // ??????????????????????????????
            tb = 0.0;
        }
        if( ta > a ) a = ta;
        if( tb < b ) b = tb;
    }
    
    if( a >= b ) return(0);
    
    int rayid    = geometry->rayid++;
    
    KdNodeRef* nodeStack = geometry->nodeStack;
    KdNodeRef* nodeStackBottom = nodeStack;
    
    KdNode* curNode = geometry->root;
    
    float av[3];
    float bv[3];
    
    av[0] = rayBegin[0]+rayDir[0]*a;
    av[1] = rayBegin[1]+rayDir[1]*a;
    av[2] = rayBegin[2]+rayDir[2]*a;
    
    bv[0] = rayBegin[0]+rayDir[0]*b;
    bv[1] = rayBegin[1]+rayDir[1]*b;
    bv[2] = rayBegin[2]+rayDir[2]*b;
    
    while(1) {
        if(curNode->neg) {
            int splittingCoord = curNode->splittingCoord;
            float splittingValue = curNode->splittingValue;
            
            if(av[splittingCoord] <= splittingValue) {
                if(bv[splittingCoord] <= splittingValue) {
                    curNode = curNode->neg;
                    continue;
                }
                else {
                    nodeStack->node = curNode->pos;
                    curNode = curNode->neg;
                }
            }
            else {
                if(bv[splittingCoord] >= splittingValue) {
                    curNode = curNode->pos;
                    continue;
                }
                else {
                    nodeStack->node = curNode->neg;
                    curNode = curNode->pos;
                }
            }
            nodeStack->b = b;
            COPY(nodeStack->bv, bv);
            ++nodeStack;
            b = (splittingValue - rayBegin[splittingCoord]) * rayDirInv[splittingCoord];
    
            bv[0] = rayBegin[0]+rayDir[0]*b;        
            bv[1] = rayBegin[1]+rayDir[1]*b;
            bv[2] = rayBegin[2]+rayDir[2]*b;                    
        }
        else {
            if(curNode->polygonsBegin) 
                for(__Polygon** curp = curNode->polygonsBegin;curp!=curNode->polygonsEnd;++curp) {
                    __Polygon* cur = *curp;
                    if(cur->mailbox != rayid) {
                        cur->mailbox = rayid;
                    
                        float dp = DOTP(cur->normal,rayDir);
                        if(dp >= 0.0) 
                            continue;
                        
                        float t = cur->planeC-DOTP(cur->normal,rayBegin);
                        
                        if(t >= 0.0 )
                            continue;
                        
                        t /= dp;
                        
                        if( t > bestDistance )
                            continue;
                        
                        float projectedPoint[2];
                        
                        if(cur->bestProjection==0) {
                            projectedPoint[0] = rayBegin[1] + rayDir[1]*t;
                            projectedPoint[1] = rayBegin[2] + rayDir[2]*t;
                        }
                        else if(cur->bestProjection==1) {
                            projectedPoint[0] = rayBegin[2] + rayDir[2]*t;
                            projectedPoint[1] = rayBegin[0] + rayDir[0]*t;
                        }
                        else {
                            projectedPoint[0] = rayBegin[0] + rayDir[0]*t;
                            projectedPoint[1] = rayBegin[1] + rayDir[1]*t;
                        }            
                        
                        float* pvertex = cur->projectedVertices;
                        
                        float a0 = pvertex[0]-projectedPoint[0];
                        float a1 = pvertex[1]-projectedPoint[1];
                        float b0 = pvertex[2]-projectedPoint[0];
                        float b1 = pvertex[3]-projectedPoint[1];
                        
                        if( a0*b1 < b0*a1 )
                            continue;
                        
                        float c0 = pvertex[4]-projectedPoint[0];
                        float c1 = pvertex[5]-projectedPoint[1];
                        
                        if( b0*c1 < c0*b1 || c0*a1 < a0*c1 )
                            continue;
    
                        return(1);
                    }
                }
    
            if(nodeStack==nodeStackBottom) break;
            --nodeStack;        
            curNode = nodeStack->node;
            a = b;
            COPY(av, bv);
            
            b = nodeStack->b;        
            COPY(bv, nodeStack->bv);
        }
    }

    return(0);                               
}                    
