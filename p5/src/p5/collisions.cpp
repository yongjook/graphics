#include "p5/collisions.hpp"


namespace _462 {

	bool collides( SphereBody& body1, SphereBody& body2, real_t collision_damping )
	{
		Vector3 v1,v2, vd1, vd2, center1, center2;
		Vector3 u1, u2,dir1t2;
		real_t m1,m2,r1,r2;
		
		r1 = body1.radius;
		r2 = body2.radius;
		
		v1 = body1.velocity;
		v2 = body2.velocity;
		m1 = body1.mass;
		m2 = body2.mass;

		center1 = body1.position;
		center2 = body2.position;
		/*
		printf("printing total radius\n");
		std::cout<<r1+r2<<std::endl;	
		printf("printing center distance\n");
		std::cout<<squared_distance(center2,center1)<<std::endl;	
		*/
		
		
		
		if(distance(center2,center1) < (r1+r2)){
			vd1 = v1 - v2;
			vd2 = Vector3::Zero(); // v2' = (0,0,0)
			real_t norm = length(center2 - center1);
			dir1t2 = (center2 - center1) * 1.0 / norm;
			//printf("printing distance\n");
			//std::cout<<dir1t2<<std::endl;
			
			
			vd2 = dir1t2 * 2 * m1  / ( m1 + m2) * dot(vd1 ,dir1t2);
			
			u2 = v2 + vd2;
			u1 = (m1*v1 + m2*v2 - m2*u2) / m1;
			
			body1.velocity = u1;
			body2.velocity = u2;
			return true;
		}
		// TODO detect collision. If there is one, update velocity

		return false;
	}

	bool collides( SphereBody& body1, TriangleBody& body2, real_t collision_damping )
	{
		// TODO detect collision. If there is one, update velocity
		Vector3  p1,p2,a,u, pointInTriangle, pointNotInTriangle,n, normalN;
		real_t d, correctedD;
		float correctedD1, correctedD2, correctedD3;
		Vector3 bary, A,B,C;
		
		A = body2.vertices[0];
		B = body2.vertices[1];
		C = body2.vertices[2]; 
		
		normalN = body2.normal_T(body2);
		
		//sphere position
		p1 = body1.position;
		//normal of triangle 
		//gotta calculate the normal;
		
		//triangle position
		p2 = body2.position;
		//triangel position - sphere position
		a=  p1 - p2;
		
		//std::cout<<body1.radius<<std::endl;
		
		//distance to the triangle from sphere
		
		
		Vector3 dampingV1;
		
		if(body2.checkBary(body1, body2)){
			
			d = dot(a,normalN);
			//printf("oritinal distance value");
			//std::cout<<d<<std::endl;
			pointInTriangle = p1 - d*normalN;
			correctedD = dot(pointInTriangle,normalN);
			//printf("distance value");
			//std::cout<<correctedD<<std::endl;
			
			if(abs(d) < body1.radius){
				
				u = body1.velocity - 2* dot(body1.velocity, normalN) * normalN;
				dampingV1 = u - collision_damping * u;
				
				
				body1.velocity = dampingV1;
				
				
				
				return true;
			}
		}
		
		//check each edge distance get them. 
		/*else {
		//edge AB
			pointNotInTriangle = (dot((p1 - A), (B-A)) * (B-A)) / pow((squared_distance(B,A)),2.0) + A;
			a = p1 - pointNotInTriangle;
			correctedD1 = dot(a,n);
		//edge AC
			pointNotInTriangle = (dot((p1 - A), (C-A)) * (C-A)) / pow((squared_distance(C,A)),2.0) + A;
			a = p1 - pointNotInTriangle;
			correctedD2 = dot(a,n);
			
			if(correctedD1 > correctedD2){
				correctedD = correctedD1;
			}
			else{
				correctedD = correctedD2;
			}
			
			//edge BC
			pointNotInTriangle = (dot((p1 - B), (C-B)) * (C-B)) / pow((squared_distance(C,B)),2.0) + B;
			a = p1 - pointNotInTriangle;
			correctedD3 = dot(a,n);
		if(correctedD > correctedD3)
			{
				correctedD = correctedD;
			}
		else{
				correctedD = correctedD3;
			}
		}*/				
		
		
		return false;
	}

	
	//not done here
	bool collides( SphereBody& body1, PlaneBody& body2, real_t collision_damping )
	{

		Vector3  p1,p2,a, n,u;
		real_t d;
		Vector3 dampingV1;
		
		p1 = body1.position;
		//need to calculate body normal also
		n = body2.normal;
		p2 = body2.position;
		a=  p1 - p2;
		d = dot(a,n);
		
		if(abs(d) < body1.radius){
			u = body1.velocity - 2* dot(body1.velocity, n) * n;
			
			
			body1.velocity = u;
			return true;
		}

		// TODO detect collision. If there is one, update velocity

		return false;
	}

}
