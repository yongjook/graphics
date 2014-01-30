/**
 * @file project.cpp
 * @brief OpenGL project
 *
 * @author Ling Yuan, lyuan
 * @bug Unimplemented
 */

#include "opengl/project.hpp"
#include <stdio.h>

// use this header to include the OpenGL headers
// DO NOT include gl.h or glu.h directly; it will not compile correctly.
#include "application/opengl.hpp"

// A namespace declaration. All proejct files use this namespace.
// Add this declration (and its closing) to all source/headers you create.
// Note that all #includes should be BEFORE the namespace declaration.
namespace _462 {

// definitions of functions for the OpenglProject class

// constructor, invoked when object is created
OpenglProject::OpenglProject()
{
    // TODO any basic construction or initialization of members
    // Warning: Although members' constructors are automatically called,
    // ints, floats, pointers, and classes with empty contructors all
    // will have uninitialized data!
}

// destructor, invoked when object is destroyed
OpenglProject::~OpenglProject()
{
    // TODO any final cleanup of members
    // Warning: Do not throw exceptions or call virtual functions from deconstructors!
    // They will cause undefined behavior (probably a crash, but perhaps worse).
}

/**
 * Initialize the project, doing any necessary opengl initialization.
 * @param camera An already-initialized camera.
 * @param scene The scene to render.
 * @return true on success, false on error.
 */
bool OpenglProject::initialize( Camera* camera, Scene* scene )
{
  int i, j;
  Vector3 p0, p1, p2, vnorm;

  // copy scene
  this->scene = *scene;
  std::cout << "OpenglProject::initialize" << std::endl;

  // Set up pool size
  num_water_vertices = POOL_W*POOL_L;
  num_water_triangles = (POOL_W-1)*(POOL_L-1)*2;

  // Allocate array memory
  if (!(box_normals = new Vector3 [this->scene.mesh.num_vertices]))
    return false;
  if (!(water_coords = new Vector2 [num_water_vertices]))
    return false;
  if (!(water_vertices = new Vector3 [num_water_triangles]))
    return false;
  if (!(water_triangles = new Triangle [num_water_triangles]))
    return false;
  if (!(water_normals = new Vector3 [num_water_vertices]))
    return false;

  // Set up control status
  glShadeModel(GL_SMOOTH); 
  glEnable(GL_NORMALIZE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Set up lighting
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);
  GLfloat light_pos[] = LIGHT_POS1; 
  glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

  // Set up normal vectors for box
  for (i = 0; i < this->scene.mesh.num_vertices; i++)
    box_normals[i] = Vector3(0.0,0.0,0.0);
  for (i = 0; i < this->scene.mesh.num_triangles; i++) {
    p0 = this->scene.mesh.vertices[this->scene.mesh.triangles[i].vertices[0]];
    p1 = this->scene.mesh.vertices[this->scene.mesh.triangles[i].vertices[1]];
    p2 = this->scene.mesh.vertices[this->scene.mesh.triangles[i].vertices[2]];
    vnorm = normalize(cross((p0-p2),(p1-p2)));
    for (j = 0; j < 3; j++)
      box_normals[this->scene.mesh.triangles[i].vertices[j]] += vnorm;
  }
  for (i = 0; i < this->scene.mesh.num_vertices; i++)
    box_normals[i] = normalize(box_normals[i]);

  // Set up water_coords
  for (i = 0; i < POOL_W; i++)
    for (j = 0; j < POOL_L; j++)
      water_coords[j+i*POOL_W] =
        Vector2(j*(2.0/(POOL_W-1))-1.0, i*(2.0/(POOL_L-1))-1.0);

  // Set up water_triangles
  for (i = 0; i < POOL_W-1; i++) {
    for (j = 0; j < POOL_L-1; j++) {
      water_triangles[j+i*(POOL_W-1)].vertices[0] = j+i*POOL_W;
      water_triangles[j+i*(POOL_W-1)].vertices[1] = j+(i+1)*POOL_W;
      water_triangles[j+i*(POOL_W-1)].vertices[2] = j+i*POOL_W+1;
      water_triangles[j+i*(POOL_W-1)+(POOL_W-1)*(POOL_L-1)].vertices[0] =
        j+i*POOL_W+1;
      water_triangles[j+i*(POOL_W-1)+(POOL_W-1)*(POOL_L-1)].vertices[1] =
        j+(i+1)*POOL_W;
      water_triangles[j+i*(POOL_W-1)+(POOL_W-1)*(POOL_L-1)].vertices[2] =
        j+(i+1)*POOL_W+1;
    }
  }
 
  return true;
}

/**
 * Clean up the project. Free any memory, etc.
 */
void OpenglProject::destroy()
{
  delete[] box_normals;
  delete[] water_coords;
  delete[] water_vertices;
  delete[] water_triangles;
  delete[] water_normals;
}

/**
 * Perform an update step. This happens on a regular interval.
 * @param dt The time difference from the previous frame to the current.
 */
void OpenglProject::update( real_t dt )
{
  int i, j;
  Vector2 coord;
  real_t height;
  Vector3 p0, p1, p2, vnorm;

  // update our heightmap
  scene.heightmap->update( dt );

  // Update water_vertices
  for (i = 0; i < POOL_W*POOL_L; i++) {
    coord = water_coords[i];
    height = scene.heightmap->compute_height(coord);
    water_vertices[i] = Vector3(coord.x, height, coord.y);
  }

  // Update water_normals
  for (i = 0; i < POOL_W*POOL_L; i++)
    water_normals[i] = Vector3(0.0, 0.0, 0.0);
  for (i = 0; i < num_water_triangles; i++) {
    p0 = water_vertices[water_triangles[i].vertices[0]];
    p1 = water_vertices[water_triangles[i].vertices[1]];
    p2 = water_vertices[water_triangles[i].vertices[2]];
    vnorm = normalize(cross(p1-p0, p2-p0));
    for (j = 0; j < 3; j++)
      water_normals[water_triangles[i].vertices[j]] += vnorm;
  }
  for (i = 0; i < POOL_W*POOL_L; i++)
    water_normals[i] = normalize(water_normals[i]);
}

/**
 * Clear the screen, then render the mesh using the given camera.
 * @param camera The logical camera to use.
 * @see math/camera.hpp
 */
void OpenglProject::render( const Camera* camera )
{
  Vector3 axis;
  real_t angle;
  GLfloat red[] = RED;
  GLfloat blue[] = BLUE;
  GLfloat white[] = WHITE;
  GLfloat box_shininess[] = BOX_SHINE;
  GLfloat water_shininess[] = WATER_SHINE;

  // Clear buffers
  glClearColor(0.0, 0.0, 0.0, 0.0);  
  glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
 
  // Handle camera
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  Vector3 cam_pos = camera->get_position();
  Vector3 cam_dir = camera->get_direction()+cam_pos;
  Vector3 cam_up = camera->get_up();
  gluPerspective(camera->get_fov_degrees(),
                 camera->get_aspect_ratio(),
                 camera->get_near_clip(),
                 camera->get_far_clip());
  gluLookAt(cam_pos.x, cam_pos.y, cam_pos.z,
            cam_dir.x, cam_dir.y, cam_dir.z,
            cam_up.x, cam_up.y, cam_up.z);

  // Handle objects
  glMatrixMode(GL_MODELVIEW);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_VERTEX_ARRAY);

  glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
  glMaterialfv(GL_FRONT, GL_SPECULAR, white);
  glMaterialfv(GL_FRONT, GL_SHININESS, box_shininess);
  glNormalPointer(GL_DOUBLE, 0, box_normals);
  glVertexPointer(3, GL_DOUBLE, 0, scene.mesh.vertices);
  glLoadIdentity();
  glTranslatef(scene.mesh_position.position.x,
               scene.mesh_position.position.y,
               scene.mesh_position.position.z);
  scene.mesh_position.orientation.to_axis_angle(&axis, &angle);
  glRotatef(angle/PI*180, axis.x, axis.y, axis.z);
  glScalef(scene.mesh_position.scale.x,
           scene.mesh_position.scale.y,
           scene.mesh_position.scale.z);
  glDrawElements(GL_TRIANGLES, 3*scene.mesh.num_triangles,
                 GL_UNSIGNED_INT,scene.mesh.triangles);

  glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
  glMaterialfv(GL_FRONT, GL_SPECULAR, white);
  glMaterialfv(GL_FRONT, GL_SHININESS, water_shininess);
  glNormalPointer(GL_DOUBLE, 0, water_normals);
  glVertexPointer(3, GL_DOUBLE, 0, water_vertices);
  glLoadIdentity();
  glTranslatef(scene.heightmap_position.position.x,
               scene.heightmap_position.position.y,
               scene.heightmap_position.position.z);
  scene.heightmap_position.orientation.to_axis_angle(&axis, &angle);
  glRotatef(angle/PI*180, axis.x, axis.y, axis.z);
  glScalef(scene.heightmap_position.scale.x,
           scene.heightmap_position.scale.y,
           scene.heightmap_position.scale.z);
  glDrawElements(GL_TRIANGLES, 3*num_water_triangles,
                 GL_UNSIGNED_INT, water_triangles);

  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
}

} /* _462 */

