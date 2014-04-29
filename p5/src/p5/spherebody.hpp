#ifndef _462_PHYSICS_SPHEREBODY_HPP_
#define _462_PHYSICS_SPHEREBODY_HPP_

#include "scene/sphere.hpp"
#include "p5/body.hpp"
#include <cstdio>

namespace _462 {

class Sphere;

class SphereBody : public Body
{
public:
  public:
    Sphere* sphere;
    real_t radius;
    real_t mass;
    Vector3 force;
    Vector3 torque;

    SphereBody( Sphere* geom );
    virtual ~SphereBody() { }
    virtual Vector3 step_position( real_t dt, real_t motion_damping );
    virtual Quaternion step_orientation( real_t dt, real_t motion_damping );
    virtual void apply_force( const Vector3& f, const Vector3& offset );
	
	virtual Vector3 rungeVel(Vector3 x, real_t y);
	virtual Vector3 rungePos(Vector3 x, real_t y);
	virtual Vector3 getAcc(Vector3 v, real_t t);
	
	virtual Vector3 getVelocity(Vector3 x, real_t t);
	
};

}

#endif

