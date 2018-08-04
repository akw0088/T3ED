
#include "PolyTracer.h"
#include <cassert>
#include <algorithm>
#include <vector>

#include <iostream>

#define BIG_VALUE (1.0e36)

namespace PolyTracer {
    inline real my_det(const Vec2& v0, const Vec2& v1, const Vec2& p) {
        return (v0[0]-p[0])*(v1[1]-p[1])-(v1[0]-p[0])*(v0[1]-p[1]);
    }
    
    struct Triangle {
        Vec3  normal;          // Jokaisella kolmion pisteellä normal*p=planeC
        real  planeC;
        int   bestProjection;  // 0=YZ, 1=ZX, 2=XY
        Vec2  projectedVertices[3];
        int   mailbox;
        
        Triangle() {}
        void set(const Vec3* v) {
            normal = (v[1]-v[0])%(v[2]-v[0]);
            planeC = normal * v[0];

            // Find projection axis
            real absnormal0 = fabs(normal[0]);
            real absnormal1 = fabs(normal[1]);
            real absnormal2 = fabs(normal[2]);

            if(absnormal0 > absnormal1 && absnormal0 > absnormal2) 
                bestProjection = 0;
            else if(absnormal1 > absnormal2) 
                bestProjection = 1;
            else 
                bestProjection = 2;
             
            // Project vertices
            int c0 = (bestProjection+1)%3;
            int c1 = (bestProjection+2)%3;
            for(int i=0;i<3;++i) {
                projectedVertices[i][0] = v[i][c0];
                projectedVertices[i][1] = v[i][c1];
            }

            // Orientate projected triangle
            if( my_det(projectedVertices[0], 
                       projectedVertices[1], 
                       projectedVertices[2]) < 0.0 )
                std::swap(projectedVertices[1], projectedVertices[2]);

            mailbox = -1;
        }
    };

#define POLY_INTERSECTION_TEST                                  \
    real dp = cur->normal*rayDir;                               \
    if(dp >= 0.0)                                               \
        continue;                                               \
                                                                \
    real t = cur->planeC - cur->normal*rayBegin;                \
                                                                \
    if(t >= 0.0 )                                               \
        continue;                                               \
                                                                \
    t /= dp;                                                    \
                                                                \
    if( t > distance )                                          \
        continue;                                               \
                                                                \
    real projectedPoint[2];                                     \
                                                                \
    if(cur->bestProjection==0) {                                \
        projectedPoint[0] = rayBegin[1] + rayDir[1]*t;          \
        projectedPoint[1] = rayBegin[2] + rayDir[2]*t;          \
    }                                                           \
    else if(cur->bestProjection==1) {                           \
        projectedPoint[0] = rayBegin[2] + rayDir[2]*t;          \
        projectedPoint[1] = rayBegin[0] + rayDir[0]*t;          \
    }                                                           \
    else {                                                      \
        projectedPoint[0] = rayBegin[0] + rayDir[0]*t;          \
        projectedPoint[1] = rayBegin[1] + rayDir[1]*t;          \
    }                                                           \
                                                                \
    Vec2* pvertex = cur->projectedVertices;                     \
                                                                \
    real a0 = pvertex[0][0]-projectedPoint[0];                  \
    real a1 = pvertex[0][1]-projectedPoint[1];                  \
    real b0 = pvertex[1][0]-projectedPoint[0];                  \
    real b1 = pvertex[1][1]-projectedPoint[1];                  \
                                                                \
    if( a0*b1 < b0*a1 )                                         \
        continue;                                               \
                                                                \
    real c0 = pvertex[2][0]-projectedPoint[0];                  \
    real c1 = pvertex[2][1]-projectedPoint[1];                  \
                                                                \
    if( b0*c1 < c0*b1 || c0*a1 < a0*c1 )                        \
        continue;
    


    struct TriangleRef {
        Triangle* triangle;
        Vec3      minc;
        Vec3      maxc;
        int       vertexCount;
        Vec3*     vertices;

        TriangleRef() : vertices(0) {}
        ~TriangleRef() {
            if(vertices) delete[](vertices);
        }
        void set(Triangle* _triangle, const Vec3* v) {
            triangle = _triangle;
            vertexCount = 3;
            vertices = new Vec3[3];
            vertices[0] = v[0];
            vertices[1] = v[1];
            vertices[2] = v[2];
            updateBoundingBox();
        }
        void updateBoundingBox(void) {
            minc = maxc = vertices[0];
            for(int i=1;i<vertexCount;++i)
                for(int j=0;j<3;++j) 
                    if(vertices[i][j] < minc[j])
                        minc[j] = vertices[i][j];
                    else if(vertices[i][j] > maxc[j])
                        maxc[j] = vertices[i][j];            
        }
        void split(int axis, real t, TriangleRef& negPoly, TriangleRef& posPoly) {
            posPoly.triangle = triangle;
            negPoly.triangle = triangle;

            negPoly.vertexCount = 0;
            posPoly.vertexCount = 0;

            Vec3 prev = vertices[vertexCount-1];
            for(int i=0;i<vertexCount;++i) {
                if((prev[axis] < t && vertices[i][axis] > t) ||
                   (prev[axis] > t && vertices[i][axis] < t)) {
                    ++negPoly.vertexCount;
                    ++posPoly.vertexCount;
                }
                if(vertices[i][axis] <= t)
                    ++negPoly.vertexCount;
                if(vertices[i][axis] >= t)
                    ++posPoly.vertexCount;
                prev = vertices[i];
            }

            assert(negPoly.vertexCount > 0);
            assert(posPoly.vertexCount > 0);
            
            Vec3* negVert = new Vec3[negPoly.vertexCount];
            negPoly.vertices = negVert;
            Vec3* posVert = new Vec3[posPoly.vertexCount];
            posPoly.vertices = posVert;

            for( i=0;i<vertexCount;++i) {
                if((prev[axis] < t && vertices[i][axis] > t) ||
                   (prev[axis] > t && vertices[i][axis] < t)) {
                    real s = (t-prev[axis])/(vertices[i][axis]-prev[axis]);
                    *negVert = prev+(vertices[i]-prev)*s;
                    (*negVert)[axis] = t; // varmuuden vuoksi
                    *posVert = *negVert;

                    ++negVert;
                    ++posVert;
                }
                if(vertices[i][axis] <= t) 
                    *(negVert++) = vertices[i];
                if(vertices[i][axis] >= t) 
                    *(posVert++) = vertices[i];                
                prev = vertices[i];
            }

            negPoly.updateBoundingBox();
            posPoly.updateBoundingBox();
        }
    };

    struct SwitchPoint {
        real value;
        int  dir;

        bool operator<(const SwitchPoint& b) const {
            return(value < b.value || (value==b.value && dir>b.dir));
        }
    };

    real FindBestSplit(int axis, const Vec3& minc, const Vec3& maxc, 
                       std::vector<TriangleRef*>& trirefs,
                       real& _split) {
        int polygonCount = (int)trirefs.size();
        SwitchPoint* spoints = new SwitchPoint[polygonCount*2];
        for(int i=0;i<polygonCount;++i) {
            spoints[i*2].value = trirefs[i]->minc[axis];
            spoints[i*2].dir   = 1;
            spoints[i*2+1].value = trirefs[i]->maxc[axis];
            spoints[i*2+1].dir   = -1;
        }
    
        std::sort(spoints, spoints+polygonCount*2);

        ////
        real cost0, cost1;
        real a, b;
        {
            real l1, l2;
            int c1 = (axis+1)%3;
            int c2 = (axis+2)%3;
            a = minc[axis];
            b = maxc[axis];
            l1 = maxc[c1]-minc[c1];
            l2 = maxc[c2]-minc[c2];
            cost0 = l1*l2;
            cost1 = l1+l2;
        }  
        real bestCost = BIG_VALUE;
        int  bestSplitId = -1;

        int negCount = 0;
        int posCount = polygonCount;

        for(int i=0;i<2*polygonCount;++i) {
            if( spoints[i].dir == -1 )
                --posCount;

            real split = spoints[i].value;
            if( split > a && split < b) {
                real cost = cost0*(posCount+negCount)+cost1*( (b-split)*posCount + (split-a)*negCount );
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

        if(bestSplitId == -1)
            return(BIG_VALUE);

        _split = spoints[bestSplitId].value;
        delete[](spoints);
        return(bestCost);
    }

    real FindBestSplit(const Vec3& minc, const Vec3& maxc, 
                       std::vector<TriangleRef*>& trirefs,
                       real& split, int& axis) {
        axis = 0; 
        real cost = FindBestSplit(0, minc, maxc, trirefs, split);

        real split1;
        real cost1 = FindBestSplit(1, minc, maxc, trirefs, split1);

        if(cost1 < cost) {
            cost = cost1;
            split = split1;
            axis = 1;
        }

        cost1 = FindBestSplit(2, minc, maxc, trirefs, split1);

        if(cost1 < cost) {
            cost = cost1;
            split = split1;
            axis = 2;
        }

        return(cost);
    }

    struct KdNode {
        KdNode* neg;
        union {
            KdNode* pos;
            Triangle** polygonsBegin;
        };
        union {
            int     splittingCoord;
            Triangle** polygonsEnd;
        };
        real splittingValue;

        KdNode(const Vec3& minc, const Vec3& maxc, 
               std::vector<TriangleRef*>& trirefs,
               int maxDepth, int maxFails) {
            if(trirefs.empty()) {
                neg = 0;
                polygonsBegin = 0;
                return;
            }

            if( trirefs.size()>0 && maxDepth>0 ) {
                real split;
                int  axis;

                Vec3 l  = maxc-minc;
                real area = l[0]*(l[1]+l[2])+l[1]*l[2];
                real splittingCost = FindBestSplit(minc, maxc, trirefs, split, axis); /*+1.1*area;*/
/*                std::cout << trirefs.size() << " " << splittingCost << std::endl;
                std::cout << "  => " << axis << " " << split << std::endl;
*/
                real leafCost = area*trirefs.size();
                /*
                  std::cout << "  splittingCost = " << splittingCost << std::endl;
                  std::cout << "  leafCost      = " << leafCost << std::endl;
                  std::cout << "  area          = " << area << std::endl;        
                */
                if( leafCost*0.75 < splittingCost )
                    --maxFails;

                if( maxFails>0 ) {
                    std::vector<TriangleRef*> negTris;
                    std::vector<TriangleRef*> posTris;

                    std::vector<TriangleRef*> newTris;
                    
                    for(std::vector<TriangleRef*>::iterator it = trirefs.begin();
                        it!=trirefs.end();++it) {
                        TriangleRef* cur = *it;
                        if(cur->maxc[axis]<=split)
                            negTris.push_back(cur);
                        else if(cur->minc[axis]>=split)
                            posTris.push_back(cur);
                        else {
                            TriangleRef* negPoly = new TriangleRef;
                            TriangleRef* posPoly = new TriangleRef;

                            newTris.push_back(negPoly);
                            newTris.push_back(posPoly);
                            
                            cur->split(axis, split, *negPoly, *posPoly);
                            
                            negTris.push_back(negPoly);
                            posTris.push_back(posPoly);               
                        }
                    }

                    Vec3 midc = maxc;
                    midc[axis] = split;
                    neg = new KdNode(minc, midc, negTris, maxDepth-1, maxFails);
                    midc = minc;
                    midc[axis] = split;
                    pos = new KdNode(midc, maxc, posTris, maxDepth-1, maxFails);

                    for(unsigned int i=0;i<newTris.size();++i)
                        delete(newTris[i]);
                    
                    splittingCoord = axis;
                    splittingValue = split;

                    return;
                }
            }
            neg = 0;
            polygonsBegin = new Triangle*[trirefs.size()];
            polygonsEnd = polygonsBegin+trirefs.size();
            for(unsigned int i=0;i<trirefs.size();++i)
                polygonsBegin[i] = trirefs[i]->triangle;
        }

        ~KdNode() {
            if(neg) {
                delete(neg);
                delete(pos);
            }
            else if(polygonsBegin)
                delete(polygonsBegin);
        }

        int depth(void) {
            if(neg==0) 
                return(0);
            return(std::max(neg->depth(), pos->depth())+1);
        }

        int size(void) {
            if(neg==0)
                return(1);
            else
                return(1+neg->size()+pos->size());
        }
    };

    struct KdNodeRef {
        KdNode* node;
        real b;
        Vec3 bv;
    };

    class TracerImpl : public Tracer {
        std::vector<Triangle> triangles;
        Vec3                  bbox_minc;
        Vec3                  bbox_maxc;
        KdNode*               root;
        KdNodeRef*            nodeStack;    
        mutable int           rayid;
    public:
        TracerImpl(int triangleCount, const Vec3* vertices) 
            : triangles(triangleCount) {
            TriangleRef* _trirefs  = new TriangleRef[triangleCount];
            std::vector<TriangleRef*> trirefs(triangleCount);
            
            bbox_maxc = bbox_minc = vertices[0];
            for(int i=0;i<triangleCount;++i) {
                triangles[i].set(vertices+3*i);
                _trirefs[i].set(&triangles[i], vertices+3*i); 
                trirefs[i] = &_trirefs[i];
                
                for(int j=0;j<3;++j) {
                    if(_trirefs[i].minc[j] < bbox_minc[j])
                        bbox_minc[j] = _trirefs[i].minc[j];
                    if(_trirefs[i].maxc[j] > bbox_maxc[j])
                        bbox_maxc[j] = _trirefs[i].maxc[j];
                }                    
            }                      

            int dmax = (int)(1.2*log(triangleCount)/log(2.0)+2.0);
            int failmax = (int)(1.0+0.2*dmax);
            root = new KdNode(bbox_minc, bbox_maxc, trirefs, dmax, failmax);
            delete[](_trirefs);

            int treeDepth = root->depth();
            nodeStack = new KdNodeRef[treeDepth];

            std::cout << "size: " << root->size() << " depth: " << treeDepth << std::endl;
        }
        ~TracerImpl() {
            delete(root);
            delete[](nodeStack);
        }
#define MAD(dest,a,b,t) \
  dest[0]=a[0]+b[0]*t; \
  dest[1]=a[1]+b[1]*t; \
  dest[2]=a[2]+b[2]*t;

        bool rayIntersection(const Vec3& _rayBegin, const Vec3& _rayDir,                       
                             IntersectionInfo& ii) const {
            Vec3 rayBegin = _rayBegin;
            Vec3 rayDir   = _rayDir;
            real      distance = BIG_VALUE;
            Triangle* bestPoly = 0;
            
            real a=0.0, b=BIG_VALUE;
            Vec3 rayDirInv;
            for(int i=0;i<3;++i) {
                real ta, tb;
                real scale = 1.0/rayDir[i];
                rayDirInv[i] = scale;
                if( rayDir[i] > 0.0 ) {
                    ta = (bbox_minc[i]-rayBegin[i])*scale;
                    tb = (bbox_maxc[i]-rayBegin[i])*scale;
                }
                else if( rayDir[i] < 0.0 ) {
                    ta = (bbox_maxc[i]-rayBegin[i])*scale;
                    tb = (bbox_minc[i]-rayBegin[i])*scale;
                }
                else {
                    if( rayBegin[i] <= bbox_minc[i] || rayBegin[i] >= bbox_maxc[i] ) {
                        ta = BIG_VALUE;
                        tb = -BIG_VALUE;
                    }
                    else {
                        ta = -BIG_VALUE;
                        tb = BIG_VALUE;
                    }
                }
                if( ta > a ) a = ta;
                if( tb < b ) b = tb;
            }

            if( a >= b ) return(0);

            ++rayid;

            KdNodeRef* curStack = nodeStack;
            KdNodeRef* nodeStackBottom = curStack;

            KdNode* curNode = root;

            // Vec3 av = rayBegin + rayDir*a;
            // Vec3 bv = rayBegin + rayDir*b;

            Vec3 av, bv;
            MAD(av, rayBegin, rayDir, a);
            MAD(bv, rayBegin, rayDir, b);

            while(1) {
                if(curNode->neg) {
                    int splittingCoord = curNode->splittingCoord;
                    real splittingValue = curNode->splittingValue;

                    if(av[splittingCoord] <= splittingValue) {
                        if(bv[splittingCoord] <= splittingValue) {
                            curNode = curNode->neg;
                            continue;
                        }
                        else {
                            curStack->node = curNode->pos;
                            curNode = curNode->neg;
                        }
                    }
                    else {
                        if(bv[splittingCoord] >= splittingValue) {
                            curNode = curNode->pos;
                            continue;
                        }
                        else {
                            curStack->node = curNode->neg;
                            curNode = curNode->pos;
                        }
                    }
                    curStack->b = b;
                    curStack->bv = bv;
                    ++curStack;
                    b = (splittingValue - rayBegin[splittingCoord]) * rayDirInv[splittingCoord];
                    // bv = rayBegin + rayDir*b;
                    MAD(bv, rayBegin, rayDir, b);
                }
                else {
                    if(curNode->polygonsBegin) 
                        for(Triangle** curp = curNode->polygonsBegin;
                            curp!=curNode->polygonsEnd;++curp) {
                            Triangle* cur = *curp;
                            if(cur->mailbox != rayid) {
                                cur->mailbox = rayid;
                
                                POLY_INTERSECTION_TEST;
  
                                distance = t;
                                bestPoly = cur;
                            }
                        }

                    if(distance <= b || curStack==nodeStackBottom) break;
                    --curStack;        
                    curNode = curStack->node;
                    a = b;
                    av = bv;
        
                    b = curStack->b;        
                    bv = curStack->bv;
                }
            }
                 
            if(bestPoly == 0)
                return(0);

            ii.polyid   = bestPoly-&triangles[0];
            ii.distance = distance;
            ii.normal   = bestPoly->normal;
            ii.location = rayBegin + rayDir*distance;
            return(1);
        }

        bool rayShadowIntersection(const Vec3& rayBegin, const Vec3& rayDir,
                                   real distance) const {
            real a=0.0, b=distance;
            Vec3 rayDirInv;
            for(int i=0;i<3;++i) {
                real ta, tb;
                real scale = 1.0/rayDir[i];
                rayDirInv[i] = scale;
                if( rayDir[i] > 0.0 ) {
                    ta = (bbox_minc[i]-rayBegin[i])*scale;
                    tb = (bbox_maxc[i]-rayBegin[i])*scale;
                }
                else if( rayDir[i] < 0.0 ) {
                    ta = (bbox_maxc[i]-rayBegin[i])*scale;
                    tb = (bbox_minc[i]-rayBegin[i])*scale;
                }
                else {
                    if( rayBegin[i] <= bbox_minc[i] || rayBegin[i] >= bbox_maxc[i] ) {
                        ta = BIG_VALUE;
                        tb = -BIG_VALUE;
                    }
                    else {
                        ta = -BIG_VALUE;
                        tb = BIG_VALUE;
                    }
                }
                if( ta > a ) a = ta;
                if( tb < b ) b = tb;
            }

            if( a >= b ) return(0);

            ++rayid;

            KdNodeRef* curStack = nodeStack;
            KdNodeRef* nodeStackBottom = curStack;

            KdNode* curNode = root;

            // Vec3 av = rayBegin + rayDir*a;
            // Vec3 bv = rayBegin + rayDir*b;

            Vec3 av, bv;
            MAD(av, rayBegin, rayDir, a);
            MAD(bv, rayBegin, rayDir, b);

            while(1) {
                if(curNode->neg) {
                    int splittingCoord = curNode->splittingCoord;
                    real splittingValue = curNode->splittingValue;
        
                    if(av[splittingCoord] <= splittingValue) {
                        if(bv[splittingCoord] <= splittingValue) {
                            curNode = curNode->neg;
                            continue;
                        }
                        else {
                            curStack->node = curNode->pos;
                            curNode = curNode->neg;
                        }
                    }
                    else {
                        if(bv[splittingCoord] >= splittingValue) {
                            curNode = curNode->pos;
                            continue;
                        }
                        else {
                            curStack->node = curNode->neg;
                            curNode = curNode->pos;
                        }
                    }
                    curStack->b = b;
                    curStack->bv = bv;
                    ++curStack;
                    b = (splittingValue - rayBegin[splittingCoord]) * rayDirInv[splittingCoord];
                    // bv = rayBegin + rayDir*b;
                    MAD(bv, rayBegin, rayDir, b);
                }
                else {
                    if(curNode->polygonsBegin) 
                        for(Triangle** curp = curNode->polygonsBegin;
                            curp!=curNode->polygonsEnd;++curp) {
                            Triangle* cur = *curp;
                            if(cur->mailbox != rayid) {
                                cur->mailbox = rayid;
                
                                POLY_INTERSECTION_TEST;
  
                                return(1);
                            }
                        }

                    if(curStack==nodeStackBottom) break;
                    --curStack;        
                    curNode = curStack->node;
                    a = b;
                    av = bv;
        
                    b = curStack->b;        
                    bv = curStack->bv;
                }
            }

            return(0);
        }
    };

    Tracer* CreateTracer(int triangleCount, const Vec3* vertices) {
        return(new TracerImpl(triangleCount, vertices));
    }
}
