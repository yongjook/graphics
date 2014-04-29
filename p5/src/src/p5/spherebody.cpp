#include "p5/spherebody.hpp"
#include "math/vector.hpp"
#include "math/matrix.hpp"
#include "scene/sphere.hpp"
#include <iostream>
#include <exception>
#include <algorithm>

#include <ctime>			
#include <cstdlib>	

namespace _462 {

	SphereBody::SphereBody( Sphere* geom )
	{
		sphere = geom;
		position = sphere->position;
		radius = sphere->radius;
		orientation = sphere->orientation;
		mass = 0.0;
		velocity = Vector3::Zero();
		angular_velocity = Vector3::Zero();
		force = Vector3::Zero();
		torque = Vector3::Zero();
	}
	
	Vector3 SphereBody::rk4(Vector3 position ,real_t dt){
		
		
		velocity += (force / mass ) * dt ; // change in velocity is added to the velociy
		position += velocity * dt;
		
	}
	
	
	/*RK4*/
	double f(double x, real_t dt);
	double runge(double x, double y);
	
	double runge(double x, double y){
		double K2, K1, K3, K4
		
		K1 = (H * f(x,y));
		K2 = (H * f((x+1/2 * H), (y+1/2*K1)));
		K3 = (H * f((x+1/2 * H), (y+1/2*K2)));
		K4 = (H * f((x+H), (y+K3)));
		
		double runge = (y+(1/6) * (K1+2*K2+2*K3 + K4));
		return runge;
	}
	
	
	
	
	
	
	
	/*RK4*/

	Vector3 SphereBody::step_position( real_t dt, real_t motion_damping )
	{
		// Note: This function is here as a hint for an approach to take towards
		// programming RK4, you should add more functions to help you or change the
		// scheme
		// TODO return the delta in position dt in the future
		
		
		velocity += (force / mass ) * dt ; // change in velocity is added to the velociy
		position += velocity * dt;
		
		
		
		return Vector3::Zero();
	}

	Vector3 SphereBody::step_orientation( real_t dt, real_t motion_damping )
	{
		// Note: This function is here as a hint for an approach to take towards
		// programming RK4, you should add more functions to help you or change the
		// scheme
		// TODO return the delta in orientation dt in the future
		// vec.x = rotation along x axis
		// vec.y = rotation along y axis
		// vec.z = rotation along z axis
		
		return Vector3::Zero();
	}

	void SphereBody::apply_force( const Vector3& f, const Vector3& offset )
	{
		real_t I, angular_acc;
		
		// TODO apply force/torque to sphere
		
		if (offset == Vector3::Zero()){
			force = force + f;
		}
		
		
		
		/*
		else{
		//inertia 
			I = 2/5 * mass * radius * radius;
			torque = Vector3::UnitX() - angular_velocity * 0.1f;
			//formula from handout?
			angular_acc = torque / I;
			angular_velocity = torque / 	
		}
		*/
		
		
	}
}
