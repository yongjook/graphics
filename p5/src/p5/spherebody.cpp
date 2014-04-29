#include "p5/spherebody.hpp"
#include "math/vector.hpp"
#include "math/matrix.hpp"
#include "scene/sphere.hpp"
#include <iostream>
#include <exception>
#include <algorithm>
#include "math/camera.hpp"

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
	
	/*RK4*/

	Vector3 SphereBody::rungeVel(Vector3 x, real_t dt){
		Vector3 K2, K1, K3, K4;
		real_t t = 0;
		K1 = (dt * getAcc(x,t));
		K2 = (dt * getAcc((x+(1/2 * K1)), (t + 1/2*dt)));
		K3 = (dt * getAcc((x+(1/2 * K2)), (t+1/2*dt)));
		K4 = (dt * getAcc((x+K3), (t+dt)));
		Vector3 runge = (x + ((K1 + 2 * K2 + 2 * K3 + K4)/6));
		return runge;
	}

	Vector3 SphereBody::rungePos(Vector3 x, real_t dt){
		Vector3 K2, K1, K3, K4;
		real_t t = 0;
		K1 = (dt * getVelocity(x,t));
		K2 = (dt * getVelocity((x+1/2 * K1), (t + 1/2*dt)));
		K3 = (dt * getVelocity((x+1/2 * K2), (t+1/2*dt)));
		K4 = (dt * getVelocity((x+K3), (t+dt)));
		Vector3 runge = (x + ((K1 + 2 * K2 + 2 * K3 + K4)/6));
		return runge;
	}
	
	Vector3 SphereBody::getAcc(Vector3 v, real_t t){
		return force/mass;
	}
	
	Vector3 SphereBody::getVelocity(Vector3 x, real_t t){
		//return velocity * t;
		//velocity += force/mass * t;
		return velocity;
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

	Quaternion SphereBody::step_orientation( real_t dt, real_t motion_damping )
	{
		// Note: This function is here as a hint for an approach to take towards
		// programming RK4, you should add more functions to help you or change the
		// scheme
		// TODO return the delta in orientation dt in the future
		// vec.x = rotation along x axis
		// vec.y = rotation along y axis
		// vec.z = rotation along z axis
		
		Camera camera;
		camera.orientation = orientation;
		
		camera.roll(angular_velocity.x * dt);
		camera.pitch(angular_velocity.y * dt);
		camera.yaw(angular_velocity.z * dt);
		
		
		return camera.orientation;
	}

	void SphereBody::apply_force( const Vector3& f, const Vector3& offset )
	{
		real_t I;
		Vector3 angular_acc;
		
		// TODO apply force/torque to sphere
		
		if (offset == Vector3::Zero()){
			force = force + f;
		}
		
		
		else{
		//inertia 
			I = 2/5 * mass * radius * radius;
			torque = cross(offset, f);
			//formula from handout?
			angular_acc = torque * 1.0/I;
			
			force = force + angular_acc;
			//angular_velocity = torque / 
		}
		
		
		
	}
}
