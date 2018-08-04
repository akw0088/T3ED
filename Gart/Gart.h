#ifndef __GART_H
#define __GART_H

typedef void Geometry;

struct GartPolygon {
    int count;
    float** vertices;
};

struct GartIntersectionInfo {
    int    polyid;
    float distance;
    float normal[3];
    float location[3];
};

Geometry* BuildGeometry(int polygonCount, GartPolygon* polygons);
void DestroyGeometry(Geometry* _geometry);
bool RayIntersection(Geometry* _geometry,
                     const float* rayBegin, const float* rayDir,
                     GartIntersectionInfo* info);
bool RayShadowIntersection(Geometry* _geometry,
                           const float* rayBegin, const float* rayDir, float bestDistance);

#endif
