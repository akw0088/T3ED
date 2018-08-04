#include <math.h>
//#include "H:\ohjelmointi\dx81sdk\include\d3dx8math.h"
#define EPSILON 0.000001

int intersect_triangle(double orig[3], double dir[3],
                   double vert0[3], double vert1[3], double vert2[3],
                   double *t, double *u, double *v)
{
   double edge1[3], edge2[3], tvec[3], pvec[3], qvec[3];
   double det,inv_det;


#define CROSS(dest,v1,v2) \
          dest[0]=v1[1]*v2[2]-v1[2]*v2[1]; \
          dest[1]=v1[2]*v2[0]-v1[0]*v2[2]; \
          dest[2]=v1[0]*v2[1]-v1[1]*v2[0];
#define DOT(v1,v2) (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])

#define SUB(dest,v1,v2) \
          dest[0]=v1[0]-v2[0]; \
          dest[1]=v1[1]-v2[1]; \
          dest[2]=v1[2]-v2[2]; 

   
   
   
  /* find vectors for two edges sharing vert0 */
   SUB(edge1, vert1, vert0);
   SUB(edge2, vert2, vert0);

   /* begin calculating determinant - also used to calculate U parameter */
   CROSS(pvec, dir, edge2);

   /* if determinant is near zero, ray lies in plane of triangle */
   det = DOT(edge1, pvec);

   /* calculate distance from vert0 to ray origin */
   SUB(tvec, orig, vert0);
   inv_det = 1.0 / det;
   
   CROSS(qvec, tvec, edge1);
      
   if (det > EPSILON)
   {
      *u = DOT(tvec, pvec);
      if (*u < 0.0 || *u > det)
	 return 0;
            
      /* calculate V parameter and test bounds */
      *v = DOT(dir, qvec);
      if (*v < 0.0 || *u + *v > det)
	 return 0;
      
   }
   /*else if(det < -EPSILON)
   {
      // calculate U parameter and test bounds 
      *u = DOT(tvec, pvec);
      if (*u > 0.0 || *u < det)
	 return 0;
      
      // calculate V parameter and test bounds 
      *v = DOT(dir, qvec) ;
      if (*v > 0.0 || *u + *v < det)
	 return 0;
   }*/
   else return 0;  /* ray is parallell to the plane of the triangle */

   *t = DOT(edge2, qvec) * inv_det;
   (*u) *= inv_det;
   (*v) *= inv_det;

   return 1;
}

////////////////////////////////////////////////////////////////////////
//                                                                    //
//            Line with Triangle Collision Detection                  //
//                                                                    //
////////////////////////////////////////////////////////////////////////
// With reference to gametutorials.com                                //
////////////////////////////////////////////////////////////////////////

 

/*bool TriangleToLineCollision(D3DXVECTOR3 v1, D3DXVECTOR3 v2, D3DXVECTOR3 v3,
                                            D3DXVECTOR p1, D3DXVECTOR3 p2 )
{
      // v1-v3 are our points representing our triangle
      // p1-p2 are the two points for our line
      //<Part-1->
      D3DXVECTOR3 vA = v2 - v1;
      D3DXVECTOR3 vB = v3 - v1; 
      D3DXVec3Normalize(&vA, &vA);
      D3DXVec3Normalize(&vB, &vB);
      D3DXVECTOR3 vNormal;
      D3DXVECTOR3( &vNormal, &vA, &vB );
      float A = vNormal.x;
      float B = vNormal.y;
      float C = vNormal.z;
      float x = v1.x; // Just choose any point from our triangle, as long as 
      float y = v1.y; // its on our plane.
      float z = v1.z;
      // Now we use the magic plane law/formula - e.g. Ax + By + Cz + D = 0;
      // Where ABC - Plane Normals
      //       xyz - point on the plane
      //         D - distance from origin
      float fDistanceFromOrigin = (-A*x - B*y - C*z );

 

      // Now we use our two points for our line, and make sure that
      // they cross our triangles plane.

      float distance1 = A*p1.x + B*p1.y + C*p1.z + fDistanceFromOrigin;
      float distance2 = A*p2.x + B*p2.y + C*p2.z + fDistanceFromOrigin;
      if( distance1 * distance2 >= 0 ) // As if one was on one side of the plane and the other
            return false;                // was on the other side, then one would have a -ve
                                       // value and the other a +ve, leading to a negative
                                       // result when multiplied together!
      //<Part-2->
      D3DXVECTOR3 vLineDirection = p1 - p2;
      D3DXVec3Normalize( &vLineDirection, &vLineDirection );
      float fNumerator   = -(A*p1.x + B*p1.y + C*p1.z + fDistanceFromOrigin);
      float fDenominator = D3DXVec3Dot( &vNormal, &vLineDirection );
      // Be careful of divide by zero's! Can be very bad for the computer!
      if( fDenominator == 0.0f )
            return false;
      float fDist = Numerator / Denominator;
      D3DXVECTOR vPoint;
      vPoint.x = p1.x + (vLineDirection.x * fDist);
      vPoint.y = p1.y + (vLineDirection.y * fDist);
      vPoint.z = p1.z + (vLineDirection.z * fDist);
      // Excellent thing to know, vPoint is the point in the middle of the triangle
      // where our line clides with the triangle face - but only if there's a collision
      // as its the point where it hits the plane.  The next part determines if its
      // inside the triangles.
      double Angle = 0.0f;
      D3DXVECTOR3 vTriangle[3] = { v1, v2, v3 };
      for( int i=0; i<3; i++ )
      {
            D3DXVECTOR3 vS = vTriangle[i]       - vPoint;
            D3DXVECTOR3 vT = vTriangle[(i+1)%3] - vPoint;
            float d = D3DXVec3Dot(&vS, &vT);
            float mag = D3DXVec3Length(&vS, &vS) * D3DXVec3Length(&vT, &vT);
            // sqrt( (vS.x*vS.x)+
            //       (vS.y*vS.y)+
            //       (vS.z*vS.z) )
            double tempAngle = acos( d / mag );
            if( _isnan(tempAngle) )
                  tempAngle = 0;
            Angle += tempAngle;

      }
      if( Angle > (2*PI) )
            return true;
      return false;

}*/
