#include "p5/physics.hpp"

namespace _462 {

	Physics::Physics()
	{
		
		reset();
	}

	Physics::~Physics()
	{
		reset();
	}

	void Physics::step( real_t dt )
	{	
		Vector3 velocity;
		//add gravity
		for(unsigned int g = 0; g < num_spheres(); g++){
			real_t mass1 = spheres[g]->mass;
			spheres[g]->apply_force(gravity *mass1,Vector3::Zero());
		}
		
		for(unsigned int i = 0; i < num_spheres(); i++){
			//euler's
			/*spheres[i]->step_position(dt, 0);
			spheres[i]->sphere->position = spheres[i]->position;
			*/
			
			/*RK4*/
			//suppose to update acc// input is acc
			spheres[i]->velocity = spheres[i]->rungeVel(spheres[i]->velocity, dt);
			//	printf("spheres[i] velocity:\n");
			//	std::cout<<spheres[i]->velocity<<std::endl;
			
			
			//suppose to use ^ acc to get pos// input is ve
			spheres[i]->position = spheres[i]->rungePos(spheres[i]->position,dt);
			//printf("spheres[i] velocity:\n");
			//std::cout<<spheres[i]->velocity<<std::endl;
			spheres[i]->sphere->position = spheres[i]->position;
			
			/*RK4*/
			
			/*spin*/
			
			spheres[i]->orientation = spheres[i]->step_orientation(dt, collision_damping);
			spheres[i]->sphere->orientation = spheres[i]->orientation;
			
			/*spin*/
			

			
			for(unsigned int j=0; j < num_planes(); j++){
				collides(*spheres[i], *planes[j],collision_damping);
			}
			
			for(unsigned int n=0; n < num_spheres(); n++){
				if(i == n){continue;}
				collides(*spheres[i], *spheres[n],collision_damping);
			}
			

			for(unsigned int k=0; k < num_triangles(); k++){
				collides(*spheres[i], *triangles[k], collision_damping);
			}
		}
		
		//get rid of gravity
		for(unsigned int g = 0; g < num_spheres(); g++){
			real_t mass1 = spheres[g]->mass;
			spheres[g]->apply_force(-gravity *mass1,Vector3::Zero());
		}
		
		
		
		//trial for RK4

		// TODO step the world forward by dt. Need to detect collisions, apply
		// forces, and integrate positions and orientations.
		// 
		// Note: put RK4 here, not in any of the physics bodies
		// 
		// Must use the functions that you implemented
		//
		// Note, when you change the position/orientation of a physics object,
		// change the position/orientation of the graphical object that represents
		// it
	}

	void Physics::add_sphere( SphereBody* b )
	{
		spheres.push_back( b );
	}

	size_t Physics::num_spheres() const
	{
		return spheres.size();
	}

	void Physics::add_plane( PlaneBody* p )
	{
		planes.push_back( p );
	}

	size_t Physics::num_planes() const
	{
		return planes.size();
	}

	void Physics::add_triangle( TriangleBody* t )
	{
		triangles.push_back( t );
	}

	size_t Physics::num_triangles() const
	{
		return triangles.size();
	}

	void Physics::add_spring( Spring* s )
	{
		springs.push_back( s );
	}

	size_t Physics::num_springs() const
	{
		return springs.size();
	}

	void Physics::reset()
	{
		for ( SphereList::iterator i = spheres.begin(); i != spheres.end(); i++ ) {
			delete *i;
		}
		for ( PlaneList::iterator i = planes.begin(); i != planes.end(); i++ ) {
			delete *i;
		}
		for ( TriangleList::iterator i = triangles.begin(); i != triangles.end(); i++ ) {
			delete *i;
		}
		for ( SpringList::iterator i = springs.begin(); i != springs.end(); i++ ) {
			delete *i;
		}

		spheres.clear();
		planes.clear();
		triangles.clear();
		springs.clear();
		
		gravity = Vector3::Zero();
		collision_damping = 0.0;
	}

}
