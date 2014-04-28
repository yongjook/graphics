#include "math/math.hpp"
#include "math/vector.hpp"
#include "math/quaternion.hpp"
#include "math/matrix.hpp"
#include "p5/spring.hpp"
#include "p5/body.hpp"
#include "p5/spherebody.hpp"
#include <iostream>

namespace _462 {

Spring::Spring()
{
    body1_offset = Vector3::Zero();
    body2_offset = Vector3::Zero();
    damping = 0.0;
}

void Spring::step( real_t dt )
{
    // TODO apply forces to attached bodies
	
	Vector3 v1,v2;
	//damping? 
	v1 = body1->velocity;
	v2 = body2->velocity;
	
	v1 = v1 - damping *v1;
	v2 = v2 - damping * v2;	 
	
	
	//end damping
	
}

}



