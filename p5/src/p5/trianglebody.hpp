#ifndef _462_PHYSICS_TRIANGLE_HPP_
#define _462_PHYSICS_TRIANGLE_HPP_

#include "scene/triangle.hpp"
#include "p5/body.hpp"
#include "p5/spherebody.hpp"
#include "math/vector.hpp"

namespace _462 {

class Triangle;

class TriangleBody : public Body
{
public:
    TriangleBody( Triangle* geom );
    virtual ~TriangleBody() { }
    virtual Vector3 step_position( real_t dt, real_t motion_damping );
    virtual Vector3 step_orientation( real_t dt, real_t motion_damping );
    virtual void apply_force( const Vector3& f, const Vector3& offset );
	virtual bool checkBary(SphereBody& body1, TriangleBody& body2);
	virtual Vector3 normal_T(TriangleBody& body2);
    Vector3 vertices[3];
	
	Vector3 normal;
};

}

#endif


