#ifndef __VEC_H
#define __VEC_H

/*
  Määrittelee vektoriluokan Vec<class ring, int dim>,
  missä ring on vektorin kerroinrengas ja dim dimensio.

  Konstruktorit:
    Vec()      -> (?,?,...,?)
    Vec(x)     -> (x,x,...,x)
    Vec(x,y)   -> (x,y)        ( vain, jos dim=2 )
    Vec(x,y,z) -> (x,y,z)      ( vain, jos dim=3 )
    Vec(x,y,z) -> (x,y,z,w)    ( vain, jos dim=4 )

  Metodit:
    Vec += Vec
    Vec -= Vec
    Vec *= ring
    Vec ^= Vec       ( komponentittainen kertolasku )    
    - Vec
    ring lensq()     ( vektorin pituuden neliö )
    ring [int]       ( vektorin komponentti )
    set(x,y)         ( vain, jos dim=2 )
    set(x,y,z)       ( vain, jos dim=3 )

  Globaalit funktiot:
    Vec * Vec        ( pistetulo )
    Vec + Vec
    Vec - Vec
    Vec ^ Vec        ( komponentittainen kertolasku )
    ring * Vec
    Vec * ring
    ostream << Vec   ( vektorin tulostus )

  Funktiot, jotka on määritelty, kun ring=float tai ring=double:
    ring len(Vec)        ( vektorin pituus )
    normalize(Vec)       ( tekee vektorista yksikkövektorin )
    Vec normalized(Vec)  ( palauttaa vektorin suuntaisen yksikkövektorin )

  Funktiot, jotka on määritelty, kun dim=3:
    Vec % Vec         ( vektorien ristitulo )
    det(Vec,Vec,Vec)  ( vektorikolmitulo, eli kolmen vektorin muodostaman
                        matriisin determinantti )
*/

#include <cassert>
#include <iosfwd>
#include "Common.h"

template<class ring, int dim> 
class Vec {
    ring c[dim];
 public:
    Vec() {}
    Vec(ring x, ring y) {
        assert(dim==2);
        c[0] = x;
        c[1] = y;
    }
    Vec(ring x, ring y, ring z) {
        assert(dim==3);
        c[0] = x;
        c[1] = y;
        c[2] = z;
    }
    Vec(ring x, ring y, ring z, ring w) {
        assert(dim==4);
        c[0] = x;
        c[1] = y;
        c[2] = z;
        c[3] = w;
    }
    Vec &operator+=(const Vec &v) {
        for(int i=0;i<dim;++i)
            c[i] += v.c[i];
        return(*this); 
    }
    Vec &operator-=(const Vec &v) {
        for(int i=0;i<dim;++i)
            c[i] -= v.c[i];
        return(*this); 
    }
    Vec &operator*=(ring s) {
        for(int i=0;i<dim;++i)
            c[i] *= s;
        return(*this); 
    }
    Vec &operator^=(const Vec &v) {
        for(int i=0;i<dim;++i)
            c[i] *= v.c[i];
        return(*this); 
    }  
    const Vec operator-() const {
        Vec ret;
        for(int i=0;i<dim;++i)
            ret.c[i] = -c[i];
        return(ret);
    }
    ring lensq(void) const {
        ring sum = (ring)0;
        for(int i=0;i<dim;++i)
            sum += c[i]*c[i];
        return(sum);
    }
    const ring &operator[](int i) const { 
        assert(i >= 0 && i < dim);
        return( c[i] ); 
    }
    ring &operator[](int i) { 
        assert(i >= 0 && i < dim);
        return( c[i] ); 
    }
    void set(ring x, ring y) {
        assert(dim==2);
        c[0] = x;
        c[1] = y;
    }
    void set(ring x, ring y, ring z) {
        assert(dim==3);
        c[0] = x;
        c[1] = y;
        c[2] = z;
    }
};

template<class ring, int dim> 
inline ring operator*(const Vec<ring,dim> &a, const Vec<ring,dim> &b) {
    ring sum = (ring)0;
    for(int i=0;i<dim;++i)
        sum += a[i]*b[i];
    return(sum);
}

template<class ring> 
inline const Vec<ring,3> 
operator%(const Vec<ring,3> &a, const Vec<ring,3> &b) { 
    return( Vec<ring,3>(a[1]*b[2]-a[2]*b[1],
                        a[2]*b[0]-a[0]*b[2],
                        a[0]*b[1]-a[1]*b[0]) ); 
} 

template<class ring> 
inline ring 
det(const Vec<ring,3> &a, const Vec<ring,3> &b, const Vec<ring,3> &c) { 
    return( a[0]*(b[1]*c[2]-b[2]*c[1])+
            a[1]*(b[2]*c[0]-b[0]*c[2])+
            a[2]*(b[0]*c[1]-b[1]*c[0]) ); 
} 

template<class ring, int dim> 
inline std::ostream& operator<<(std::ostream& s, const Vec<ring,dim> &v) {
    s << "(";
    for(int i=0;i<dim;++i) {
        if(i>0)
            s << " ";
        s << v[i];
    }
    s << ")";
    return(s);
}

template<class ring, int dim> 
inline const Vec<ring,dim> 
operator+(const Vec<ring,dim> &a, const Vec<ring,dim> &b) { 
    return( Vec<ring,dim>(a) += b ); 
} 

template<class ring, int dim> 
inline const Vec<ring,dim> 
operator-(const Vec<ring,dim> &a, const Vec<ring,dim> &b) { 
    return( Vec<ring,dim>(a) -= b ); 
}

template<class ring, int dim> 
inline const Vec<ring,dim> 
operator^(const Vec<ring,dim> &a, const Vec<ring,dim> &b) { 
    return( Vec<ring,dim>(a) ^= b ); 
}

template<class ring, int dim> 
inline const Vec<ring,dim> operator*(const Vec<ring,dim> &a, ring b) { 
    return( Vec<ring,dim>(a) *= b ); 
}

template<class ring, int dim> 
inline const Vec<ring,dim> operator*(ring a, const Vec<ring,dim> &b) { 
    return( Vec<ring,dim>(b) *= a ); 
}

template<int dim> 
inline const Vec<float,dim> operator*(const Vec<float,dim> &a, double b) { 
    return( Vec<float,dim>(a) *= (float)b ); 
}

template<int dim> 
inline const Vec<float,dim> operator*(double a, const Vec<float,dim> &b) { 
    return( Vec<float,dim>(b) *= (float)a ); 
}

template<int dim>
inline float len(const Vec<float, dim> &v) { 
    return((float)sqrt(v.lensq())); 
} 

template<int dim>
inline double len(const Vec<double, dim> &v) { 
    return((double)sqrt(v.lensq())); 
} 

template<int dim>
inline void normalize(Vec<float, dim> &v) { 
    v *= (real)1.0/len(v); 
}

template<int dim>
inline void normalize(Vec<double, dim> &v) { 
    v *= 1.0/len(v); 
}

template<int dim>
inline const Vec<float, dim> normalized(const Vec<float, dim> &v) { 
    return( Vec<float, dim>(v) *= (real)1.0/len(v)); 
} 

template<int dim>
inline const Vec<double, dim> normalized(const Vec<double, dim> &v) { 
    return( Vec<double, dim>(v) *= 1.0/len(v)); 
} 

typedef Vec<real,2> Vec2;
typedef Vec<real,3> Vec3;
typedef Vec<real,4> Vec4;
typedef Vec4 RGBA;

#endif
