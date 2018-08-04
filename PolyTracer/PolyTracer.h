#ifndef __POLY_TRACER_H
#define __POLY_TRACER_H

#ifndef __VEC_H
#include "Vec.h"
#endif

namespace PolyTracer {
    struct IntersectionInfo {
        int    polyid;
        real   distance;
        Vec3   normal;
        Vec3   location;
    };

    class Tracer {
    public:
        virtual ~Tracer() {}
        virtual bool rayIntersection(const Vec3& rayBegin, const Vec3& rayDir,
                                     IntersectionInfo& ii) const = 0;
        virtual bool rayShadowIntersection(const Vec3& rayBegin, const Vec3& rayDir,
                                           real distance) const = 0;
    };

    Tracer* CreateTracer(int triangleCount, const Vec3* vertices);
}

#endif
