// Vectors.h: vector class templates
/////////////////////////////////////

#if !defined(AFX_VECTORS_H__BEBAF00F_026D_11D4_8756_02608C9A770C__INCLUDED_)
#define AFX_VECTORS_H__BEBAF00F_026D_11D4_8756_02608C9A770C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <math.h>
#include <float.h>

#pragma pack(push)
#pragma pack(1)

#define FLT_GEOMEPS	1E-4f
#define DBL_GEOMEPS	1E-8

// *************************
// * Template declarations *
// *************************

/* RT - type, MTH - math threshold, GTH - geom threshold */

template <class RT, RT MTH, RT GTH> class CTVector2D;
template <class RT, RT MTH, RT GTH> class CTVector3D;
template <class RT, RT MTH, RT GTH> class CTVector4D;

typedef CTVector3D<float,  FLT_EPSILON, FLT_GEOMEPS> CFltVector3D, CFltVector;
typedef CTVector2D<float,  FLT_EPSILON, FLT_GEOMEPS> CFltVector2D, CFltTexCoords;
typedef CTVector4D<float,  FLT_EPSILON, FLT_GEOMEPS> CFltVector4D, CFltColor;

typedef CTVector3D<double, DBL_EPSILON, DBL_GEOMEPS> CDblVector3D, CDblVector;
typedef CTVector2D<double, DBL_EPSILON, DBL_GEOMEPS> CDblVector2D, CDblTexCoords;
typedef CTVector4D<double, DBL_EPSILON, DBL_GEOMEPS> CDblVector4D, CDblColor;

// CTVector2D
//////////////

template <class RT, RT MTH, RT GTH>
class CTVector2D
{
public:
	typedef RT AVector2DData[2];
	union {
		struct { RT X, Y; };
		AVector2DData Coords;
	};

	CTVector2D() {};
	CTVector2D(RT);
	CTVector2D(RT, RT);
	CTVector2D(const CTVector2D<RT,MTH,GTH>&);
	CTVector2D(const CTVector3D<RT,MTH,GTH>&);
	~CTVector2D() {};

	CTVector2D<RT,MTH,GTH>& Assign(RT, RT);

	CTVector2D<RT,MTH,GTH> operator + (const CTVector2D<RT,MTH,GTH>&) const;
	CTVector2D<RT,MTH,GTH> operator - (const CTVector2D<RT,MTH,GTH>&) const;
	CTVector2D<RT,MTH,GTH> operator * (RT) const;
	CTVector2D<RT,MTH,GTH> operator / (RT) const;

// Per-coordinate division:
	CTVector2D<RT,MTH,GTH> operator / (const CTVector2D<RT,MTH,GTH>&) const;

// Scalar multiplication:
	RT operator | (const CTVector2D<RT,MTH,GTH>&) const; 
// Per-coordinate miltiplication:
	CTVector2D<RT,MTH,GTH> operator & (const CTVector2D<RT,MTH,GTH>&) const;

	CTVector2D<RT,MTH,GTH>& operator = (const CTVector2D<RT,MTH,GTH>&);

	CTVector2D<RT,MTH,GTH>& operator += (const CTVector2D<RT,MTH,GTH>&);
	CTVector2D<RT,MTH,GTH>& operator -= (const CTVector2D<RT,MTH,GTH>&);
	CTVector2D<RT,MTH,GTH>& operator *= (RT);
	CTVector2D<RT,MTH,GTH>& operator /= (RT);

	CTVector2D<RT,MTH,GTH>& operator /= (const CTVector2D<RT,MTH,GTH>&); // per-coordinate div
	CTVector2D<RT,MTH,GTH>& operator &= (const CTVector2D<RT,MTH,GTH>&); // per-coordinate mul
	
	RT operator [] (int) const;

// Comparison with MTH precision:
	bool operator == (const CTVector2D<RT,MTH,GTH>&) const;
	bool operator != (const CTVector2D<RT,MTH,GTH>&) const;

// Comparison with GTH precision:
	bool SimilarTo (const CTVector2D<RT,MTH,GTH>&) const;
	
	RT SqrNorm(void) const;
	RT Norm(void) const;

// Get negative copy:
	CTVector2D<RT,MTH,GTH> operator - (void) const;
	CTVector2D<RT,MTH,GTH> Negative(void) const { return operator -(); }
// Negativate this:
	CTVector2D<RT,MTH,GTH>& Negativate(void);

// Get normalized copy:
	CTVector2D<RT,MTH,GTH> operator ! (void) const;
	CTVector2D<RT,MTH,GTH> Normalized(void) const { return operator !(); }
// Normalize this:
	CTVector2D<RT,MTH,GTH>& Normalize(void);

// Get clamped copy:
	CTVector2D<RT,MTH,GTH> Clamped(RT min = 0, RT max = 1) const;
// Clamp this:
	CTVector2D<RT,MTH,GTH>& Clamp(RT min = 0, RT max = 1);
};

// CTVector3D
//////////////

template <class RT, RT MTH, RT GTH>
class CTVector3D
{
public:
	typedef RT AVector3DData[3];
	union {
		struct { RT X, Y, Z; };
		struct { RT R, G, B; };
		AVector3DData Coords;
	};

	CTVector3D() {};
	CTVector3D(RT);
	CTVector3D(RT, RT, RT);
	CTVector3D(const CTVector3D<RT,MTH,GTH>&);
	CTVector3D(const CTVector2D<RT,MTH,GTH>&);
	~CTVector3D() {};

	CTVector3D<RT,MTH,GTH>& Assign(RT, RT, RT);

	CTVector3D<RT,MTH,GTH> operator + (const CTVector3D<RT,MTH,GTH>&) const;
	CTVector3D<RT,MTH,GTH> operator - (const CTVector3D<RT,MTH,GTH>&) const;
	CTVector3D<RT,MTH,GTH> operator * (RT) const;
	CTVector3D<RT,MTH,GTH> operator / (RT) const;

// Per-coordinate division:
	CTVector3D<RT,MTH,GTH> operator / (const CTVector3D<RT,MTH,GTH>&) const;

// Vector multiplication:
	CTVector3D<RT,MTH,GTH> operator * (const CTVector3D<RT,MTH,GTH>&) const;  
// Scalar multiplication:
	RT operator | (const CTVector3D<RT,MTH,GTH>&) const; 
// Per-coordinate miltiplication:
	CTVector3D<RT,MTH,GTH> operator & (const CTVector3D<RT,MTH,GTH>&) const;

	CTVector3D<RT,MTH,GTH>& operator = (const CTVector3D<RT,MTH,GTH>&);

	CTVector3D<RT,MTH,GTH>& operator += (const CTVector3D<RT,MTH,GTH>&);
	CTVector3D<RT,MTH,GTH>& operator -= (const CTVector3D<RT,MTH,GTH>&);
	CTVector3D<RT,MTH,GTH>& operator *= (RT);
	CTVector3D<RT,MTH,GTH>& operator /= (RT);

	CTVector3D<RT,MTH,GTH>& operator *= (const CTVector3D<RT,MTH,GTH>&); // vector mul
	CTVector3D<RT,MTH,GTH>& operator /= (const CTVector3D<RT,MTH,GTH>&); // per-coordinate div
	CTVector3D<RT,MTH,GTH>& operator &= (const CTVector3D<RT,MTH,GTH>&); // per-coordinate mul
	
	RT operator [] (int) const;

// Comparison with MTH precision:
	bool operator == (const CTVector3D<RT,MTH,GTH>&) const;
	bool operator != (const CTVector3D<RT,MTH,GTH>&) const;

// Comparison with GTH precision:
	bool SimilarTo (const CTVector3D<RT,MTH,GTH>&) const;
	
	RT SqrNorm(void) const;
	RT Norm(void) const;

// Get negative copy:
	CTVector3D<RT,MTH,GTH> operator - (void) const;
// Negativate this:
	CTVector3D<RT,MTH,GTH>& Negativate(void);

// Get normalized copy:
	CTVector3D<RT,MTH,GTH> operator ! (void) const;
// Normalize this:
	CTVector3D<RT,MTH,GTH>& Normalize(void);

// Get clamped copy:
	CTVector3D<RT,MTH,GTH> Clamped(RT min = 0, RT max = 1) const;
// Clamp this:
	CTVector3D<RT,MTH,GTH>& Clamp(RT min = 0, RT max = 1);
};

// CTVector4D 
//////////////

template <class RT, RT MTH, RT GTH>
class CTVector4D
/* 
 +-------------------------------------------------------------------------------------------------------+
 | Attention: to perform operations without using fourth component, just typecast an object to CTVector3D |
 +-------------------------------------------------------------------------------------------------------+ 
*/
{
public:
	typedef RT AVector4DData[4];
	union {
		struct { RT X, Y, Z, W; };
		struct { RT R, G, B, A; };
		AVector4DData Coords;
	};

	CTVector4D() {};
	CTVector4D(RT);
	CTVector4D(RT, RT, RT, RT = 1.0);
	CTVector4D(const CTVector4D<RT,MTH,GTH>&);
	CTVector4D(const CTVector3D<RT,MTH,GTH>&);
	CTVector4D(const CTVector2D<RT,MTH,GTH>&);
	~CTVector4D() {};

	CTVector4D<RT,MTH,GTH>& Assign(RT, RT, RT, RT);

	CTVector4D<RT,MTH,GTH> operator + (const CTVector4D<RT,MTH,GTH>&) const;
	CTVector4D<RT,MTH,GTH> operator - (const CTVector4D<RT,MTH,GTH>&) const;
	CTVector4D<RT,MTH,GTH> operator * (RT) const;
	CTVector4D<RT,MTH,GTH> operator / (RT) const;

// Per-coordinate division:
	CTVector4D<RT,MTH,GTH> operator / (const CTVector4D<RT,MTH,GTH>&) const;

// Scalar multiplication:
	RT operator | (const CTVector4D<RT,MTH,GTH>&) const; 
// Per-coordinate miltiplication:
	CTVector4D<RT,MTH,GTH> operator & (const CTVector4D<RT,MTH,GTH>&) const;

	CTVector4D<RT,MTH,GTH>& operator = (const CTVector4D<RT,MTH,GTH>&);

	CTVector4D<RT,MTH,GTH>& operator += (const CTVector4D<RT,MTH,GTH>&);
	CTVector4D<RT,MTH,GTH>& operator -= (const CTVector4D<RT,MTH,GTH>&);
	CTVector4D<RT,MTH,GTH>& operator *= (RT);
	CTVector4D<RT,MTH,GTH>& operator /= (RT);

	CTVector4D<RT,MTH,GTH>& operator /= (const CTVector4D<RT,MTH,GTH>&); // per-coordinate div
	CTVector4D<RT,MTH,GTH>& operator &= (const CTVector4D<RT,MTH,GTH>&); // per-coordinate mul
	
	RT operator [] (int) const;

// Comparison with MTH precision:
	bool operator == (const CTVector4D<RT,MTH,GTH>&) const;
	bool operator != (const CTVector4D<RT,MTH,GTH>&) const;

// Comparison with GTH precision:
	bool SimilarTo (const CTVector4D<RT,MTH,GTH>&) const;
	
	RT SqrNorm(void) const;
	RT Norm(void) const;

// Get negative copy:
	CTVector4D<RT,MTH,GTH> operator - (void) const;
// Negativate this:
	CTVector4D<RT,MTH,GTH>& Negativate(void);

// Get normalized copy:
	CTVector4D<RT,MTH,GTH> operator ! (void) const;
// Normalize this:
	CTVector4D<RT,MTH,GTH>& Normalize(void);

// Get clamped copy:
	CTVector4D<RT,MTH,GTH> Clamped(RT min = 0, RT max = 1) const;
// Clamp this:
	CTVector4D<RT,MTH,GTH>& Clamp(RT min = 0, RT max = 1);
};

// Utility functions

template <class RT>
RT Clamped(RT value, RT min, RT max);

template <class RT>
void Clamp(RT& valueRef, RT min, RT max);

// ************************
// * Template definitions *
// ************************

template <class RT>
inline RT Clamped(RT value, RT min, RT max)
{
	ASSERT(max >= min);
	if (value < min)
		value = min;
	if (value > max)
		value = max;
	return value;
}

template <class RT>
inline void Clamp(RT& valueRef, RT min, RT max)
{
	ASSERT(max >= min);
	if (valueRef < min)
		valueRef = min;
	if (valueRef > max)
		valueRef = max;
}

// CTVector2D:
/////////////

template <class RT, RT MTH, RT GTH>
inline CTVector2D<RT,MTH,GTH>::CTVector2D(RT Fill)
{
	X = Y = Fill;
}

template <class RT, RT MTH, RT GTH>
inline CTVector2D<RT,MTH,GTH>::CTVector2D(RT VectorX, RT VectorY)
{
	Assign(VectorX, VectorY);
}

template <class RT, RT MTH, RT GTH>
inline CTVector2D<RT,MTH,GTH>& CTVector2D<RT,MTH,GTH>::Assign(RT VectorX, RT VectorY)
{
	X = VectorX;
	Y = VectorY;
	return *this;
}

template <class RT, RT MTH, RT GTH>
inline	CTVector2D<RT,MTH,GTH>::CTVector2D(const CTVector2D<RT,MTH,GTH>& Vector)
{
	X = Vector.X;
	Y = Vector.Y;
}

template <class RT, RT MTH, RT GTH>
inline CTVector2D<RT,MTH,GTH>::CTVector2D(const CTVector3D<RT,MTH,GTH>& Vector)
{
	X = Vector.X;
	Y = Vector.Y;
}

template <class RT, RT MTH, RT GTH>
inline CTVector2D<RT,MTH,GTH> CTVector2D<RT,MTH,GTH>::operator + (const CTVector2D<RT,MTH,GTH>& Vector) const
{
	return CTVector2D(X + Vector.X, Y + Vector.Y);
}

template <class RT, RT MTH, RT GTH>
inline CTVector2D<RT,MTH,GTH> CTVector2D<RT,MTH,GTH>::operator - (const CTVector2D<RT,MTH,GTH>& Vector) const
{
	return CTVector2D(X - Vector.X, Y - Vector.Y);
}

template <class RT, RT MTH, RT GTH>
inline CTVector2D<RT,MTH,GTH> CTVector2D<RT,MTH,GTH>::operator * (RT Multiplier) const
{
	return CTVector2D<RT,MTH,GTH>(X * Multiplier, Y * Multiplier);
}

template <class RT, RT MTH, RT GTH>
inline CTVector2D<RT,MTH,GTH> operator * (RT Multiplier, const CTVector2D<RT,MTH,GTH>& Vector)
{
	return Vector * Multiplier;
}

template <class RT, RT MTH, RT GTH>
inline CTVector2D<RT,MTH,GTH> CTVector2D<RT,MTH,GTH>::operator / (RT Divider) const
{
	return CTVector2D(X / Divider, Y / Divider);
}

template <class RT, RT MTH, RT GTH>
inline CTVector2D<RT,MTH,GTH> CTVector2D<RT,MTH,GTH>::operator / (const CTVector2D<RT,MTH,GTH>& Vector) const
// Per-coordinate division:
{
	return CTVector2D(X / Vector.X, Y / Vector.Y);
}

template <class RT, RT MTH, RT GTH>
inline CTVector2D<RT,MTH,GTH> CTVector2D<RT,MTH,GTH>::operator & (const CTVector2D<RT,MTH,GTH>& Vector) const
// Per-coordinate miltiplication
{
	return CTVector2D(
		  X * Vector.X,
		  Y * Vector.Y);
}

template <class RT, RT MTH, RT GTH>
inline RT CTVector2D<RT,MTH,GTH>::operator | (const CTVector2D<RT,MTH,GTH>& Vector) const
// Scalar miltiplication
{
	return (X * Vector.X + Y * Vector.Y);
}

template <class RT, RT MTH, RT GTH>
inline CTVector2D<RT,MTH,GTH> CTVector2D<RT,MTH,GTH>::operator - (void) const
{
	return CTVector2D(-X, -Y);
}

template <class RT, RT MTH, RT GTH>
inline CTVector2D<RT,MTH,GTH> CTVector2D<RT,MTH,GTH>::operator ! (void) const
// Get normalized copy
{
	return CTVector2D(*this).Normalize();
}

template <class RT, RT MTH, RT GTH>
inline CTVector2D<RT,MTH,GTH>& CTVector2D<RT,MTH,GTH>::operator = (const CTVector2D<RT,MTH,GTH>& Vector)
{
	memcpy(&Coords, &Vector.Coords, sizeof(Coords));
	return *this;
}

template <class RT, RT MTH, RT GTH>
inline CTVector2D<RT,MTH,GTH>& CTVector2D<RT,MTH,GTH>::operator += (const CTVector2D<RT,MTH,GTH>& Vector) 
{
	X += Vector.X;
	Y += Vector.Y;
	return *this;
}

template <class RT, RT MTH, RT GTH>
inline CTVector2D<RT,MTH,GTH>& CTVector2D<RT,MTH,GTH>::operator -= (const CTVector2D<RT,MTH,GTH>& Vector)
{
	X -= Vector.X;
	Y -= Vector.Y;
	return *this;
}

template <class RT, RT MTH, RT GTH>
inline CTVector2D<RT,MTH,GTH>& CTVector2D<RT,MTH,GTH>::operator *= (RT Multiplier)
{
	X *= Multiplier;
	Y *= Multiplier;
	return *this;
}

template <class RT, RT MTH, RT GTH>
inline CTVector2D<RT,MTH,GTH>& CTVector2D<RT,MTH,GTH>::operator /= (RT Divider)
{
	X /= Divider;
	Y /= Divider;
	return *this;
}

template <class RT, RT MTH, RT GTH>
inline CTVector2D<RT,MTH,GTH>& CTVector2D<RT,MTH,GTH>::operator /= (const CTVector2D<RT,MTH,GTH>& Vector)
// Per-coordinate division
{
	X /= Vector.X;
	Y /= Vector.Y;
	return *this;
}

template <class RT, RT MTH, RT GTH>
inline CTVector2D<RT,MTH,GTH>& CTVector2D<RT,MTH,GTH>::operator &= (const CTVector2D<RT,MTH,GTH>& Vector)
// Per-coordinate miltiplication
{
	X *= Vector.X;
	Y *= Vector.Y;
	return *this;
}

template <class RT, RT MTH, RT GTH>
inline RT CTVector2D<RT,MTH,GTH>::operator [] (int Index) const
{
	return Coords[Index];
}

template <class RT, RT MTH, RT GTH>
inline bool CTVector2D<RT,MTH,GTH>::operator == (const CTVector2D<RT,MTH,GTH>& Vector) const
{
	return (*this - Vector).SqrNorm() < MTH*MTH;
}

template <class RT, RT MTH, RT GTH>
inline bool CTVector2D<RT,MTH,GTH>::operator != (const CTVector2D<RT,MTH,GTH>& Vector) const
{
	return (*this - Vector).SqrNorm() >= MTH*MTH;
}

template <class RT, RT MTH, RT GTH>
inline bool CTVector2D<RT,MTH,GTH>::SimilarTo(const CTVector2D<RT,MTH,GTH>& Vector) const
{
	return (*this - Vector).SqrNorm() < GTH*GTH;
}

template <class RT, RT MTH, RT GTH>
inline RT CTVector2D<RT,MTH,GTH>::SqrNorm(void) const
{
	return X * X + Y * Y;
}

template <class RT, RT MTH, RT GTH>
inline RT CTVector2D<RT,MTH,GTH>::Norm(void) const
{
	return (RT)sqrt(SqrNorm());
}

template <class RT, RT MTH, RT GTH>
inline CTVector2D<RT,MTH,GTH>& CTVector2D<RT,MTH,GTH>::Normalize(void)
{
	RT norm = Norm();
	if (norm > MTH)
		*this /= norm;
	else 
		X = Y = 0.0; 

	return *this;
}

template <class RT, RT MTH, RT GTH>
inline CTVector2D<RT,MTH,GTH>& CTVector2D<RT,MTH,GTH>::Negativate(void)
{
	X = -X;	Y = -Y;
	return *this;
}

template <class RT, RT MTH, RT GTH>
inline CTVector2D<RT,MTH,GTH> CTVector2D<RT,MTH,GTH>::Clamped(RT min, RT max) const
{
	CTVector2D<RT,MTH,GTH>(
			::Clamped<RT>(X, min, max),
			::Clamped<RT>(Y, min, max)
		);
}

template <class RT, RT MTH, RT GTH>
inline CTVector2D<RT,MTH,GTH>& CTVector2D<RT,MTH,GTH>::Clamp(RT min, RT max)
{
	::Clamp<RT>(X, min, max);
	::Clamp<RT>(Y, min, max);
	return *this;
}


// CTVector3D:
//////////////

template <class RT, RT MTH, RT GTH>
inline CTVector3D<RT,MTH,GTH>::CTVector3D(RT Fill)
{
	X = Y = Z = Fill;
}

template <class RT, RT MTH, RT GTH>
inline CTVector3D<RT,MTH,GTH>::CTVector3D(RT VectorX, RT VectorY, RT VectorZ)
{
	Assign(VectorX, VectorY, VectorZ);
}

template <class RT, RT MTH, RT GTH>
inline CTVector3D<RT,MTH,GTH>& CTVector3D<RT,MTH,GTH>::Assign(RT VectorX, RT VectorY, RT VectorZ)
{
	X = VectorX;
	Y = VectorY;
	Z = VectorZ;
	return *this;
}

template <class RT, RT MTH, RT GTH>
inline	CTVector3D<RT,MTH,GTH>::CTVector3D(const CTVector3D<RT,MTH,GTH>& Vector)
{
	X = Vector.X;
	Y = Vector.Y;
	Z = Vector.Z;
}

template <class RT, RT MTH, RT GTH>
inline CTVector3D<RT,MTH,GTH>::CTVector3D(const CTVector2D<RT,MTH,GTH>& Vector)
{
	X = Vector.X;
	Y = Vector.Y;
	Z = 0.0;
}

template <class RT, RT MTH, RT GTH>
inline CTVector3D<RT,MTH,GTH> CTVector3D<RT,MTH,GTH>::operator + (const CTVector3D<RT,MTH,GTH>& Vector) const
{
	return CTVector3D(X + Vector.X, Y + Vector.Y, Z + Vector.Z);
}

template <class RT, RT MTH, RT GTH>
inline CTVector3D<RT,MTH,GTH> CTVector3D<RT,MTH,GTH>::operator - (const CTVector3D<RT,MTH,GTH>& Vector) const
{
	return CTVector3D(X - Vector.X, Y - Vector.Y, Z - Vector.Z);
}

template <class RT, RT MTH, RT GTH>
inline CTVector3D<RT,MTH,GTH> CTVector3D<RT,MTH,GTH>::operator * (RT Multiplier) const
{
	return CTVector3D(X * Multiplier, Y * Multiplier, Z * Multiplier);
}

template <class RT, RT MTH, RT GTH>
inline CTVector3D<RT,MTH,GTH> operator * (RT Multiplier, const CTVector3D<RT,MTH,GTH>& Vector)
{
	return Vector * Multiplier;
}

template <class RT, RT MTH, RT GTH>
inline CTVector3D<RT,MTH,GTH> CTVector3D<RT,MTH,GTH>::operator / (RT Divider) const
{
	return CTVector3D(X / Divider, Y / Divider, Z / Divider);
}

template <class RT, RT MTH, RT GTH>
inline CTVector3D<RT,MTH,GTH> CTVector3D<RT,MTH,GTH>::operator / (const CTVector3D<RT,MTH,GTH>& Vector) const
// Per-coordinate division:
{
	return CTVector3D(X / Vector.X, Y / Vector.Y, Z / Vector.Z);
}

template <class RT, RT MTH, RT GTH>
inline CTVector3D<RT,MTH,GTH> CTVector3D<RT,MTH,GTH>::operator * (const CTVector3D<RT,MTH,GTH>& Vector) const
// Vector miltiplication
{
	return CTVector3D(
		  (Y * Vector.Z) - (Z * Vector.Y), 
		  (Z * Vector.X) - (X * Vector.Z), 
		  (X * Vector.Y) - (Y * Vector.X));
}

template <class RT, RT MTH, RT GTH>
inline CTVector3D<RT,MTH,GTH> CTVector3D<RT,MTH,GTH>::operator & (const CTVector3D<RT,MTH,GTH>& Vector) const
// Per-coordinate miltiplication
{
	return CTVector3D(
		  X * Vector.X,
		  Y * Vector.Y,
		  Z * Vector.Z);
}

template <class RT, RT MTH, RT GTH>
inline RT CTVector3D<RT,MTH,GTH>::operator | (const CTVector3D<RT,MTH,GTH>& Vector) const
// Scalar miltiplication
{
	return (X * Vector.X + Y * Vector.Y + Z * Vector.Z);
}

template <class RT, RT MTH, RT GTH>
inline CTVector3D<RT,MTH,GTH> CTVector3D<RT,MTH,GTH>::operator - (void) const
{
	return CTVector3D(-X, -Y, -Z);
}

template <class RT, RT MTH, RT GTH>
inline CTVector3D<RT,MTH,GTH>& CTVector3D<RT,MTH,GTH>::operator = (const CTVector3D<RT,MTH,GTH>& Vector)
{
	memcpy(&Coords, &Vector.Coords, sizeof(Coords));
	return *this;
}

template <class RT, RT MTH, RT GTH>
inline CTVector3D<RT,MTH,GTH>& CTVector3D<RT,MTH,GTH>::operator += (const CTVector3D<RT,MTH,GTH>& Vector) 
{
	X += Vector.X;
	Y += Vector.Y;
	Z += Vector.Z;
	return *this;
}

template <class RT, RT MTH, RT GTH>
inline CTVector3D<RT,MTH,GTH>& CTVector3D<RT,MTH,GTH>::operator -= (const CTVector3D<RT,MTH,GTH>& Vector)
{
	X -= Vector.X;
	Y -= Vector.Y;
	Z -= Vector.Z;
	return *this;
}

template <class RT, RT MTH, RT GTH>
inline CTVector3D<RT,MTH,GTH>& CTVector3D<RT,MTH,GTH>::operator *= (RT Multiplier)
{
	X *= Multiplier;
	Y *= Multiplier;
	Z *= Multiplier;
	return *this;
}

template <class RT, RT MTH, RT GTH>
inline CTVector3D<RT,MTH,GTH>& CTVector3D<RT,MTH,GTH>::operator /= (RT Divider)
{
	X /= Divider;
	Y /= Divider;
	Z /= Divider;
	return *this;
}

template <class RT, RT MTH, RT GTH>
inline CTVector3D<RT,MTH,GTH>& CTVector3D<RT,MTH,GTH>::operator *= (const CTVector3D<RT,MTH,GTH>& Vector)
// Vector miltiplication
{
	RT x = Y * Vector.Z - Z * Vector.Y;
	RT y = Z * Vector.X - X * Vector.Z;
	RT z = X * Vector.Y - Y * Vector.X;
	X = x; Y = y; Z = z;
	return *this;
}

template <class RT, RT MTH, RT GTH>
inline CTVector3D<RT,MTH,GTH>& CTVector3D<RT,MTH,GTH>::operator /= (const CTVector3D<RT,MTH,GTH>& Vector)
// Per-coordinate division
{
	X /= Vector.X;
	Y /= Vector.Y;
	Z /= Vector.Z;
	return *this;
}

template <class RT, RT MTH, RT GTH>
inline CTVector3D<RT,MTH,GTH>& CTVector3D<RT,MTH,GTH>::operator &= (const CTVector3D<RT,MTH,GTH>& Vector)
// Per-coordinate miltiplication
{
	X *= Vector.X;
	Y *= Vector.Y;
	Z *= Vector.Z;
	return *this;
}

template <class RT, RT MTH, RT GTH>
inline RT CTVector3D<RT,MTH,GTH>::operator [] (int Index) const
{
	return Coords[Index];
}

template <class RT, RT MTH, RT GTH>
inline bool CTVector3D<RT,MTH,GTH>::operator == (const CTVector3D<RT,MTH,GTH>& Vector) const
{
	return (*this - Vector).SqrNorm() < MTH*MTH;
}

template <class RT, RT MTH, RT GTH>
inline bool CTVector3D<RT,MTH,GTH>::operator != (const CTVector3D<RT,MTH,GTH>& Vector) const
{
	return (*this - Vector).SqrNorm() >= MTH*MTH;
}

template <class RT, RT MTH, RT GTH>
inline bool CTVector3D<RT,MTH,GTH>::SimilarTo(const CTVector3D<RT,MTH,GTH>& Vector) const
{
	return (*this - Vector).SqrNorm() < GTH*GTH;
}

template <class RT, RT MTH, RT GTH>
inline RT CTVector3D<RT,MTH,GTH>::SqrNorm(void) const
{
	return X * X + Y * Y + Z * Z;
}

template <class RT, RT MTH, RT GTH>
inline RT CTVector3D<RT,MTH,GTH>::Norm(void) const
{
	return (RT)sqrt(SqrNorm());
}

template <class RT, RT MTH, RT GTH>
inline CTVector3D<RT,MTH,GTH>& CTVector3D<RT,MTH,GTH>::Normalize(void)
{
	RT norm = Norm();
	if (norm > MTH)
		*this /= norm;
	else 
		X = Y = Z = 0.0; 

	return *this;
}

template <class RT, RT MTH, RT GTH>
inline CTVector3D<RT,MTH,GTH> CTVector3D<RT,MTH,GTH>::operator ! (void) const
// Get normalized copy
{
	return CTVector3D(*this).Normalize();
}

template <class RT, RT MTH, RT GTH>
inline CTVector3D<RT,MTH,GTH>& CTVector3D<RT,MTH,GTH>::Negativate(void)
{
	X = -X;	Y = -Y;	Z = -Z;
	return *this;
}

template <class RT, RT MTH, RT GTH>
inline CTVector3D<RT,MTH,GTH> CTVector3D<RT,MTH,GTH>::Clamped(RT min, RT max) const
{
	CTVector3D<RT,MTH,GTH>(
			::Clamped<RT>(X, min, max),
			::Clamped<RT>(Y, min, max),
			::Clamped<RT>(Z, min, max)
		);
}

template <class RT, RT MTH, RT GTH>
inline CTVector3D<RT,MTH,GTH>& CTVector3D<RT,MTH,GTH>::Clamp(RT min, RT max)
{
	::Clamp<RT>(X, min, max);
	::Clamp<RT>(Y, min, max);
	::Clamp<RT>(Z, min, max);
	return *this;
}


// CTVector4D:
//////////////

template <class RT, RT MTH, RT GTH>
inline CTVector4D<RT,MTH,GTH>::CTVector4D(RT Fill)
{
	X = Y = Z = W = Fill;
}

template <class RT, RT MTH, RT GTH>
inline CTVector4D<RT,MTH,GTH>::CTVector4D(RT VectorX, RT VectorY, RT VectorZ, RT VectorW)
{
	Assign(VectorX, VectorY, VectorZ, VectorW);
}

template <class RT, RT MTH, RT GTH>
inline CTVector4D<RT,MTH,GTH>& CTVector4D<RT,MTH,GTH>::Assign(RT VectorX, RT VectorY, RT VectorZ, RT VectorW)
{
	X = VectorX;
	Y = VectorY;
	Z = VectorZ;
	W = VectorW;
	return *this;
}

template <class RT, RT MTH, RT GTH>
inline	CTVector4D<RT,MTH,GTH>::CTVector4D(const CTVector4D<RT,MTH,GTH>& Vector)
{
	X = Vector.X;
	Y = Vector.Y;
	Z = Vector.Z;
	W = Vector.W;
}

template <class RT, RT MTH, RT GTH>
inline CTVector4D<RT,MTH,GTH>::CTVector4D(const CTVector3D<RT,MTH,GTH>& Vector)
{
	X = Vector.X;
	Y = Vector.Y;
	Z = Vector.Z;
	W = 1.0;
}

template <class RT, RT MTH, RT GTH>
inline CTVector4D<RT,MTH,GTH>::CTVector4D(const CTVector2D<RT,MTH,GTH>& Vector)
{
	X = Vector.X;
	Y = Vector.Y;
	Z = 0.0;
	W = 1.0;
}

template <class RT, RT MTH, RT GTH>
inline CTVector4D<RT,MTH,GTH> CTVector4D<RT,MTH,GTH>::operator + (const CTVector4D<RT,MTH,GTH>& Vector) const
{
	return CTVector4D(X + Vector.X, Y + Vector.Y, Z + Vector.Z, W + Vector.W);
}

template <class RT, RT MTH, RT GTH>
inline CTVector4D<RT,MTH,GTH> CTVector4D<RT,MTH,GTH>::operator - (const CTVector4D<RT,MTH,GTH>& Vector) const
{
	return CTVector4D(X - Vector.X, Y - Vector.Y, Z - Vector.Z, W - Vector.W);
}

template <class RT, RT MTH, RT GTH>
inline CTVector4D<RT,MTH,GTH> CTVector4D<RT,MTH,GTH>::operator * (RT Multiplier) const
{
	return CTVector4D(X * Multiplier, Y * Multiplier, Z * Multiplier, W * Multiplier);
}

template <class RT, RT MTH, RT GTH>
inline CTVector4D<RT,MTH,GTH> operator * (RT Multiplier, const CTVector4D<RT,MTH,GTH>& Vector)
{
	return Vector * Multiplier;
}

template <class RT, RT MTH, RT GTH>
inline CTVector4D<RT,MTH,GTH> CTVector4D<RT,MTH,GTH>::operator / (RT Divider) const
{
	return CTVector4D(X / Divider, Y / Divider, Z / Divider, W / Divider);
}

template <class RT, RT MTH, RT GTH>
inline CTVector4D<RT,MTH,GTH> CTVector4D<RT,MTH,GTH>::operator / (const CTVector4D<RT,MTH,GTH>& Vector) const
// Per-coordinate division:
{
	return CTVector4D(X / Vector.X, Y / Vector.Y, Z / Vector.Z, W / Vector.W);
}

template <class RT, RT MTH, RT GTH>
inline CTVector4D<RT,MTH,GTH> CTVector4D<RT,MTH,GTH>::operator & (const CTVector4D<RT,MTH,GTH>& Vector) const
// Per-coordinate miltiplication
{
	return CTVector4D(X * Vector.X, Y * Vector.Y, Z * Vector.Z, W * Vector.W);
}

template <class RT, RT MTH, RT GTH>
inline RT CTVector4D<RT,MTH,GTH>::operator | (const CTVector4D<RT,MTH,GTH>& Vector) const
// Scalar miltiplication
{
	return (X * Vector.X + Y * Vector.Y + Z * Vector.Z + W * Vector.W);
}

template <class RT, RT MTH, RT GTH>
inline CTVector4D<RT,MTH,GTH> CTVector4D<RT,MTH,GTH>::operator - (void) const
{
	return CTVector4D(-X, -Y, -Z, -W);
}

template <class RT, RT MTH, RT GTH>
inline CTVector4D<RT,MTH,GTH>& CTVector4D<RT,MTH,GTH>::operator = (const CTVector4D<RT,MTH,GTH>& Vector)
{
	memcpy(&Coords, &Vector.Coords, sizeof(Coords));
	return *this;
}

template <class RT, RT MTH, RT GTH>
inline CTVector4D<RT,MTH,GTH>& CTVector4D<RT,MTH,GTH>::operator += (const CTVector4D<RT,MTH,GTH>& Vector) 
{
	X += Vector.X;
	Y += Vector.Y;
	Z += Vector.Z;
	W += Vector.W;
	return *this;
}

template <class RT, RT MTH, RT GTH>
inline CTVector4D<RT,MTH,GTH>& CTVector4D<RT,MTH,GTH>::operator -= (const CTVector4D<RT,MTH,GTH>& Vector)
{
	X -= Vector.X;
	Y -= Vector.Y;
	Z -= Vector.Z;
	W -= Vector.W;
	return *this;
}

template <class RT, RT MTH, RT GTH>
inline CTVector4D<RT,MTH,GTH>& CTVector4D<RT,MTH,GTH>::operator *= (RT Multiplier)
{
	X *= Multiplier;
	Y *= Multiplier;
	Z *= Multiplier;
	W *= Multiplier;
	return *this;
}

template <class RT, RT MTH, RT GTH>
inline CTVector4D<RT,MTH,GTH>& CTVector4D<RT,MTH,GTH>::operator /= (RT Divider)
{
	X /= Divider;
	Y /= Divider;
	Z /= Divider;
	W /= Divider;
	return *this;
}

template <class RT, RT MTH, RT GTH>
inline CTVector4D<RT,MTH,GTH>& CTVector4D<RT,MTH,GTH>::operator /= (const CTVector4D<RT,MTH,GTH>& Vector)
// Per-coordinate division
{
	X /= Vector.X;
	Y /= Vector.Y;
	Z /= Vector.Z;
	W /= Vector.W;
	return *this;
}

template <class RT, RT MTH, RT GTH>
inline CTVector4D<RT,MTH,GTH>& CTVector4D<RT,MTH,GTH>::operator &= (const CTVector4D<RT,MTH,GTH>& Vector)
// Per-coordinate miltiplication
{
	X *= Vector.X;
	Y *= Vector.Y;
	Z *= Vector.Z;
	W *= Vector.W;
	return *this;
}

template <class RT, RT MTH, RT GTH>
inline RT CTVector4D<RT,MTH,GTH>::operator [] (int Index) const
{
	return Coords[Index];
}

template <class RT, RT MTH, RT GTH>
inline bool CTVector4D<RT,MTH,GTH>::operator == (const CTVector4D<RT,MTH,GTH>& Vector) const
{
	return (*this - Vector).SqrNorm() < MTH*MTH;
}

template <class RT, RT MTH, RT GTH>
inline bool CTVector4D<RT,MTH,GTH>::operator != (const CTVector4D<RT,MTH,GTH>& Vector) const
{
	return (*this - Vector).SqrNorm() >= MTH*MTH;
}

template <class RT, RT MTH, RT GTH>
inline bool CTVector4D<RT,MTH,GTH>::SimilarTo(const CTVector4D<RT,MTH,GTH>& Vector) const
{
	return (*this - Vector).SqrNorm() < GTH*GTH;
}

template <class RT, RT MTH, RT GTH>
inline RT CTVector4D<RT,MTH,GTH>::SqrNorm(void) const
{
	return X*X + Y*Y + Z*Z + W*W;
}

template <class RT, RT MTH, RT GTH>
inline RT CTVector4D<RT,MTH,GTH>::Norm(void) const
{
	return (RT)sqrt(SqrNorm());
}

template <class RT, RT MTH, RT GTH>
inline CTVector4D<RT,MTH,GTH>& CTVector4D<RT,MTH,GTH>::Normalize(void)
{
	RT norm = Norm();
	if (norm > MTH)
		*this /= norm;
	else 
		X = Y = Z = W = 0.0; 

	return *this;
}

template <class RT, RT MTH, RT GTH>
inline CTVector4D<RT,MTH,GTH> CTVector4D<RT,MTH,GTH>::operator ! (void) const
// Get normalized copy
{
	return CTVector4D(*this).Normalize();
}

template <class RT, RT MTH, RT GTH>
inline CTVector4D<RT,MTH,GTH>& CTVector4D<RT,MTH,GTH>::Negativate(void)
{
	X = -X;	Y = -Y;	Z = -Z; W = -W;
	return *this;
}

template <class RT, RT MTH, RT GTH>
inline CTVector4D<RT,MTH,GTH> CTVector4D<RT,MTH,GTH>::Clamped(RT min, RT max) const
{
	CTVector4D<RT,MTH,GTH>(
			::Clamped<RT>(X, min, max),
			::Clamped<RT>(Y, min, max),
			::Clamped<RT>(Z, min, max),
			::Clamped<RT>(W, min, max)
		);
}

template <class RT, RT MTH, RT GTH>
inline CTVector4D<RT,MTH,GTH>& CTVector4D<RT,MTH,GTH>::Clamp(RT min, RT max)
{
	::Clamp<RT>(X, min, max);
	::Clamp<RT>(Y, min, max);
	::Clamp<RT>(Z, min, max);
	::Clamp<RT>(W, min, max);
	return *this;
}

#pragma pack(pop)
#endif // !defined(AFX_VECTORS_H__BEBAF00F_026D_11D4_8756_02608C9A770C__INCLUDED_)
