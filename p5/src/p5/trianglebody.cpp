#include "p5/trianglebody.hpp"
#include <iostream>
#include <math.h>   

namespace _462 {

	TriangleBody::TriangleBody( Triangle* geom )
	{
		vertices[0] = geom->vertices[0].position;
		vertices[1] = geom->vertices[1].position;
		vertices[2] = geom->vertices[2].position;
		orientation = Quaternion::Identity();
		velocity = Vector3::Zero();
		velocity = Vector3::Zero();
		
	}

	Vector3 TriangleBody::step_position( real_t dt, real_t motion_damping )
	{
		return Vector3::Zero();
	}

	Vector3 TriangleBody::step_orientation( real_t dt, real_t motion_damping )
	{
		return Vector3::Zero();
	}

	void TriangleBody::apply_force( const Vector3& f, const Vector3& offset )
	{
		return;
	}
	
	Vector3 TriangleBody::normal_T(TriangleBody& body2){
		
		
		Vector3 V,W, A, B, C ,N, normalN;
		
		A = body2.vertices[0];
		B = body2.vertices[1];
		C = body2.vertices[2];
		
		V = B - A;
		W = C - A;
		
		N = cross(V,W);
		
		normalN = normalize(N);
		
		
		return normalN;
		
	}

	bool TriangleBody::checkBary(SphereBody& body1, TriangleBody& body2)
	{
		
		Vector3 AB, AC, A, B, C, N, p;
		real_t u,v;
		
		A = body2.vertices[0];
		B = body2.vertices[1];
		C = body2.vertices[2];
		
		AB = B - A;
		AC = C - A;
		
		N = cross(AB, AC);
		
		p = body1.position;
		
		// edge 1, result needed to compute v
		Vector3 AP =p - A;
		Vector3 ABxAP = cross(AB, AP);
		real_t v_num;
		if ((v_num = dot(N, ABxAP)) < 0){
			return false; // P is on the left side
		}
		// edge 2
		Vector3 BP = p - B;
		Vector3 BC = C - B;
		Vector3 BCxBP = cross(BC, BP);
		if (dot(N, BCxBP) < 0){
			return false; // P is on the left side
		}
		// edge 3, needed to compute u
		Vector3 CP = p - C;
		// we have computed AC already so we can avoid computing CA by
		// inverting the vectors in the cross product:
		// Cross(CA, CP) = cross(CP, AC);
		Vector3 CAxCP = cross(CP, AC);
		real_t u_num;
		if ((u_num = dot(N, CAxCP)) < 0){
			return false; }// P is on the left side;
		// compute barycentric coordinates
		real_t denom = dot(N, N); // ABxAC.N where N = ABxAC
		u = u_num / denom;
		v = v_num / denom;

		return true; // this ray hits the triangle
	}
	/*Vector3 bary, v1, v2, v0, f;
		real_t dot00, dot01, dot02, dot11, dot12, invDenom,u,v;

		// Compute vectors        
		v0 = body2.vertices[2] - body2.vertices[0];
		v1 = body2.vertices[1] - body2.vertices[0];
		v2 = body1.position - body2.vertices[0];

		// Compute dot products
		dot00 = dot(v0, v0);
		dot01 = dot(v0, v1);
		dot02 = dot(v0, v2);
		dot11 = dot(v1, v1);
		dot12 = dot(v1, v2);

		// Compute barycentric coordinates
		invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
		u = (dot11 * dot02 - dot01 * dot12) * invDenom;
		v = (dot00 * dot12 - dot01 * dot02) * invDenom;

		// Check if point is in triangle
		if( (u >= 0) && (v >= 0) && (u + v < 1)){
			return true;
		}

		return false;
		
	}*/
	
}