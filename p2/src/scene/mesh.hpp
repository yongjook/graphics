/**
 * @file mesh.hpp
 * @brief Mesh class and OBJ loader.
 *
 * @author Eric Butler (edbutler)
 * @author Zeyang Li (zeyangl)
 */

#ifndef _462_SCENE_MESH_HPP_
#define _462_SCENE_MESH_HPP_

#include "math/vector.hpp"

#include <vector>
#include <cassert>
#include <cmath>

namespace _462 {

struct MeshVertex
{
    //Vector4 color;
    Vector3 position;
    Vector3 normal;
    Vector2 tex_coord;
};

struct MeshTriangle
{
    // index into the vertex list of the 3 vertices
    unsigned int vertices[3];
};

struct MeshEdge
{
    // index into the edge vertex and triangles (assuming 2 triangles or less)
	unsigned int vertices[2];
	unsigned int vertex_new;				//index of new generated vertex
    unsigned int triangles[2]; 				//obsoleted
	unsigned int vertices_neighbour[2];		//obsoleted
};

struct MeshNeighbour
{
	 /*
		 index into the neighbours vertex, this could be combined with struct vertices, I didn't do so because I don't want to introduce unwanted changes in exsiting data struct 
	*/
    
	unsigned int N;  				//obsoleted
    unsigned int vertices[20];		//obsoleted 
	std::vector< unsigned int > indices; //index of neighbours of given vertice

};

struct MeshFace
{
    /*
	the neighbour means neighbour of edges, eg. vertices_neighbour[2] means neighbour of edge (vertices[0],[1]); this could be combined with struct triangles, I didn't do so because I don't want to introduce unwanted changes in exsiting data struct 
	*/
	unsigned int vertices[3];  				
    int vertices_neighbour[3];		//index of neighbour vertices of edges
};

/**
 * A mesh of triangles.
 */
class Mesh
{
public:

    Mesh();
    virtual ~Mesh();

    typedef std::vector< MeshTriangle > MeshTriangleList;
    typedef std::vector< MeshVertex > MeshVertexList;
	
	//----------------my added members---------------
	typedef std::vector< MeshNeighbour > NeighbourList;
	typedef std::vector< MeshEdge > EdgeList; //MeshEdgeList
	typedef std::vector< MeshFace > FaceList2; //MeshFaceList
	MeshTriangleList temp_triangles;		 //temp storage place tempT
	MeshVertexList temp_vertices;            //temp storage place tempV
	NeighbourList neighbours;
	//EdgeList edges;
	FaceList2 faces; //faceList
	void GenerateEdgeNeighbour();		//generate neighbours and faces (no edges)
	int IsEdgeInTriangle(unsigned int v1,unsigned int v2,MeshTriangle t3);
	bool IsVertexInTriangle(unsigned int v,int &v1,int &v2,MeshTriangle t3);
	void AddNeighbour(unsigned int v,int v1,int v2);   //check and add neighbour in triangle
	void addOddVert();				// add odd vertices
	void addEvenVert();				// update even vertices
	int IsEdgeAdded(unsigned int v1,unsigned int v2,EdgeList e);
	//----------------------------------------------------

    // The list of all triangles in this model.
    MeshTriangleList triangles;

    // The list of all vertices in this model.
    MeshVertexList vertices;

    // scene loader stores the filename of the mesh here
    std::string filename;

    bool has_tcoords;
    bool has_normals;
    int has_colors;

    // Loads the model into a list of triangles and vertices.
    bool load();

    // Creates opengl data for rendering and computes normals if needed
    bool create_gl_data();

    bool subdivide();

    // Renders the mesh using opengl.
    void render() const;
private:
    typedef std::vector< float > FloatList;
    typedef std::vector< unsigned int > IndexList;

    // the vertex data used for GL rendering
    FloatList vertex_data;
    // the index data used for GL rendering
    IndexList index_data;

    // prevent copy/assignment
    Mesh( const Mesh& );
    Mesh& operator=( const Mesh& );

};


} /* _462 */

#endif /* _462_SCENE_MESH_HPP_ */
